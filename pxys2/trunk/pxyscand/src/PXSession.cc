/* Copyright (C) 2003-2005 Stephane Thiell
 *
 * This file is part of pxyservd (from pxys)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
#define RCSID "$Id: PXSession.cc,v 1.3 2005/05/19 00:49:38 mbuna Exp $"

#include "PXSession.h"
#include "PXServer.h"
#include "PXConfig.h"
#include "PXCore.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::clog;
using std::endl;

PXSession::PXSession(PXServer *inServer, peak_stream inStream)
  : mServer(inServer), mStream(inStream), mTimedOut(false),
    mRefCnt(1), mStatus(PX_SESSION_STATUS_VALID), mQueryCount(0),
    mScannedCount(0), mProxyCount(0), mErrorCount(0)
  {
  static const struct opas_callbacks sessionCallbacks =
    {
    OPASSend,
    OPASQuery,
    OPASReplyNoProxy,
    OPASReplyProxy,
    NULL,
    OPASQuery6,
    OPASReply6NoProxy,
    OPASReply6Proxy,
    NULL,
    OPASMsgUser
    };
  
  peak_stream_set_buffered(mStream, 1, 0, 32768, NULL);
  peak_stream_set_context(mStream, this);
  peak_stream_set_timeout(mStream, 90);
  peak_stream_schedule(mStream, peak_task_self());
  
  mOPASSession = opas_open_session(&sessionCallbacks, this);
  }

PXSession::~PXSession()
  {
  mServer->SessionClosed(this);
  }

void
PXSession::Invalidate()
  {
  assert(mStatus == PX_SESSION_STATUS_VALID);
  
  mStatus = PX_SESSION_STATUS_INVALID;
  opas_close_session(mOPASSession);
  peak_release(mStream);
  
  this->Release();
  }

void
PXSession::Release()
  {
  if (--mRefCnt == 0 && mStatus == PX_SESSION_STATUS_INVALID)
    delete this;
  }

void
PXSession::ReplyStatus(uint32_t inUserData)
  {
  PXSStatus *status;
  size_t size;
  
  mServer->CreateStatus(&status, &size);
  
  status->sessScannedCount = htonl(mScannedCount);
  status->sessProxyCount = htonl(mProxyCount);
  
  opas_send_msg_user(mOPASSession, status, size, 1, inUserData);
  }

void
PXSession::ReplyStats(uint32_t inUserData)
  {
  PXSStats stats;
  
  stats.head.sig = htonl(PXYSCAND_SIG);
  stats.head.ver = htonl(PX_VERSION);
  stats.head.cmd = htonl(PX_CMD_STATS);
  
  mServer->GetStats(stats);
  
  // Session specifics
  stats.sessQueryCount = htonl(mQueryCount);
  stats.sessScannedCount = htonl(mScannedCount);
  stats.sessProxyCount = htonl(mProxyCount);
  stats.sessErrorCount = htonl(mErrorCount);
  
  stats.sessReadBytes = htonl(peak_stream_get_read_count(mStream));
  stats.sessWriteBytes = htonl(peak_stream_get_write_count(mStream));
  
  opas_send_msg_user(mOPASSession, &stats, sizeof(stats), 1, inUserData);
  }

void
PXSession::HandleRemove(opas_msg_user_header &umsg, void *data)
  {
  if (umsg.data_length == sizeof(PXSRemove6))
    {
    PXSRemove6 *pack = reinterpret_cast<PXSRemove6*>(data);
    pack->status = mServer->RemoveAddressFromCache(pack->addr) ? 1 : 0;
    pack->status = htonl(pack->status);
    opas_send_msg_user(mOPASSession, pack, sizeof(PXSRemove6), 1, umsg.user_data);
    }
  else if (umsg.data_length == sizeof(PXSRemove4))
    {
    PXSRemove4 *pack = reinterpret_cast<PXSRemove4*>(data);
    pack->status = mServer->RemoveAddressFromCache(pack->addr) ? 1 : 0;
    pack->status = htonl(pack->status);
    opas_send_msg_user(mOPASSession, pack, sizeof(PXSRemove4), 1, umsg.user_data);
    }
  else
    abort();
  }

void
PXSession::Query(opas_msg_query &inQuery)
  {
  // It will consult the cache. If not found create a new PXScan instance
  // when no such scan is running, or append this query to the instance found
  // otherwise (so that everyone will get a reply).
  this->Retain();
  mQueryCount++;
  mServer->NewScanIP4(inQuery, this);
  }

void
PXSession::MsgUser(opas_msg_user_header &umsg, void *data)
  {
  if (umsg.head & OPAS_FLAG_REPL)
    return;
  
  if (umsg.data_length < sizeof(PXSHeader))
    return this->MsgUserUnknown(umsg);
  
  PXSHeader *head = (PXSHeader *)data;
  uint32_t sig, ver, cmd;
  sig = ntohl(head->sig);
  ver = ntohl(head->ver);
  cmd = ntohl(head->cmd);
  
  if (sig != PXYSCAND_SIG || ver != PX_VERSION)
    return this->MsgUserUnknown(umsg);
  
  switch (cmd)
    {
    case PX_CMD_STATUS:
      if (umsg.data_length == sizeof(PXSHeader))
        this->ReplyStatus(umsg.user_data);
      break;
    case PX_CMD_STATS:
      if (umsg.data_length == sizeof(PXSHeader))
        this->ReplyStats(umsg.user_data);
      break;
    case PX_CMD_REMOVE:
      this->HandleRemove(umsg, data);
      break;
    default:
      this->MsgUserUnknown(umsg);
      break;
    }
  }

void
PXSession::MsgUserUnknown(opas_msg_user_header &umsg)
  {
  opas_send_msg_user(mOPASSession, NULL, 0, 1, umsg.user_data);
  }

void
PXSession::ScanCompletedNoProxy(const opas_msg_query &inQuery)
  {
  mServer->ScanIP4ResultNoProxy(inQuery);
  mScannedCount++;
  }

void
PXSession::ScanCompletedProxy(const opas_msg_query &inQuery,
                              uint16_t inTypeOfProxy, uint16_t inPort,
                              const char *inDescr)
  {
  mServer->ScanIP4ResultProxy(inQuery, inTypeOfProxy, inPort, inDescr);
  mScannedCount++;
  mProxyCount++;
  }

void
PXSession::ScanCompletedWithError(const opas_msg_query &inQuery)
  {
  mServer->DeleteScanIP4(inQuery);
  mErrorCount++;
  }

void
PXSession::ScanResultNoProxy(const opas_msg_query &inQuery, bool fromCache)
  {
  if (this->IsValid())
    {
    opas_msg_query query( inQuery ); // need a mutable copy, not very elegant..
    opas_reply_noproxy(mOPASSession, &query, fromCache ? 1 : 0);
    }
  this->Release();
  }

void
PXSession::ScanResultProxy(const opas_msg_query &inQuery, bool fromCache,
                           time_t ts, uint16_t inTypeOfProxy, uint16_t inPort,
                           const char *inDescr)
  {
  if (this->IsValid())
    {
    opas_msg_query query = inQuery; // need a mutable copy, not very elegant..
    opas_reply_proxy(mOPASSession, &query, fromCache ? 1 : 0, ts,
                     inTypeOfProxy, inPort, inDescr);
    }
  this->Release();
  }

void
PXSession::ScanResultError(const opas_msg_query &inQuery, int error)
  {
  if (this->IsValid())
    {
    opas_msg_query query = inQuery;
    opas_reply_error(mOPASSession, &query, (uint32_t)error);
    }
  this->Release();
  }

void
PXSession::ProcessEvent(peak_stream s, int type)
  {
  switch (type)
    {
    case PEAK_STREAM_EVT_READ:
      this->EventRead(s);
      break;
    case PEAK_STREAM_EVT_OPEN:
    case PEAK_STREAM_EVT_WRITE:
      break;
    case PEAK_STREAM_EVT_END:
      // temporary stderr logging
      clog << "PXSession::ProcessEvent: PEAK_STREAM_EVT_END" << endl;
      this->Invalidate();
      break;
    case PEAK_STREAM_EVT_ERROR:
      clog << "PXSession::ProcessEvent: PEAK_STREAM_EVT_ERROR" << endl;
      this->Invalidate();
      break;
    case PEAK_STREAM_EVT_TIMEDOUT:
      if (!mTimedOut)
        {
        mTimedOut = true;
        opas_ping(mOPASSession);
        break;
        }
      this->Invalidate();
      break;
    default:
      assert(0);
      break;
    }
  }

void
PXSession::EventRead(peak_stream s)
  {
  char buffer[1024];
  int result;
  
  if (mTimedOut)
    mTimedOut = false;
  
  result = peak_stream_read(s, buffer, sizeof(buffer));
  if (opas_feed(this->mOPASSession, buffer, result) == -1)
    {
    clog << "opas_feed failed!" << endl;
    this->Invalidate();
    }
  }

void
PXSession::EventCallback(peak_stream s, int type, void *context)
  {
  ((PXSession *)context)->ProcessEvent(s, type);
  }

void
PXSession::OPASSend(const void *data, uint32_t length, void *uptr)
  {
  PXSession *session = reinterpret_cast<PXSession *>(uptr);
  peak_stream_write_buffered(session->mStream, data, length);
  }

void
PXSession::OPASQuery(struct opas_msg_query *queryp, void *context)
  {
  PXSession *session = reinterpret_cast<PXSession *>(context);
  session->Query(*queryp);
  }

void
PXSession::OPASReplyNoProxy(struct opas_msg_query *replyp, void *context)
  {
  clog << "PXSession::OPASReplyNoProxy" << endl;
  }

void
PXSession::OPASReplyProxy(struct opas_msg_reply_proxy *replyp, void *context)
  {
  clog << "PXSession::OPASReplyProxy" << endl;
  }

void
PXSession::OPASQuery6(struct opas_msg_query6 *queryp, void *context)
  {
  clog << "PXSession::OPASQuery6" << endl;
  }

void
PXSession::OPASReply6NoProxy(struct opas_msg_query6 *replyp, void *context)
  {
  clog << "PXSession::OPASReply6NoProxy" << endl;
  }

void
PXSession::OPASReply6Proxy(struct opas_msg_reply6_proxy *replyp, void *context)
  {
  clog << "PXSession::OPASReply6Proxy" << endl;
  }

void
PXSession::OPASMsgUser(struct opas_msg_user_header *umsgp, void *data,
                      void *context)
  {
  PXSession *session = reinterpret_cast<PXSession *>(context);
  session->MsgUser(*umsgp, data);
  }
