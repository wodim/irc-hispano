// Copyright (C) 2003 Stephane Thiell
//
// This file is part of pxyscand (from pxys)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// $Id: PXSession.h,v 1.1.1.1 2003/12/30 17:09:02 mbuna Exp $
//
#ifndef INCLUDED_PXSESSION_H_
#define INCLUDED_PXSESSION_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <opas/opas.h>
#include <peak/peak.h>

class PXServer;

class PXSession
  {
  enum { PX_SESSION_STATUS_VALID, PX_SESSION_STATUS_INVALID };
  
  friend class PXServer;
public:
  PXSession(PXServer *inServer, peak_stream inStream);
  
  void Invalidate();
  bool IsValid() const { return mStatus == PX_SESSION_STATUS_VALID; }
  int GetStatus() const { return mStatus; }
  
  void Retain() { mRefCnt++; }
  void Release();
  
  void ScanCompletedNoProxy(const opas_msg_query &inQuery);
  void ScanCompletedProxy(const opas_msg_query &inQuery,
                          uint16_t inTypeOfProxy, uint16_t inPort,
                          const char *inDescr);
  void ScanCompletedWithError(const opas_msg_query &inQuery);
  
  void ScanResultNoProxy(const opas_msg_query &inQuery, bool fromCache);
  void ScanResultProxy(const opas_msg_query &inQuery, bool fromCache,
                       time_t ts, uint16_t inTypeOfProxy, uint16_t inPort,
                       const char *inDescr);
  void ScanResultError(const opas_msg_query &inQuery, int error);
  
protected:
  virtual ~PXSession();
  
  void ReplyStatus(uint32_t inUserData);
  void ReplyStats(uint32_t inUserData);
  void HandleRemove(opas_msg_user_header &umsg, void *data);
  
  void Query(opas_msg_query &inQuery);
  void MsgUser(opas_msg_user_header &umsg, void *data);
  void MsgUserUnknown(opas_msg_user_header &umsg);
  void ScanDone(opas_msg_query &inQuery);
  
  void ProcessEvent(peak_stream s, int type);
  void EventRead(peak_stream s);
  
  static void EventCallback(peak_stream s, int type, void *context);

  static void OPASSend(const void *data, uint32_t length, void *uptr);
  static void OPASQuery(struct opas_msg_query *queryp, void *context);
  static void OPASReplyNoProxy(struct opas_msg_query *replyp, void *context);
  static void OPASReplyProxy(struct opas_msg_reply_proxy *replyp,
                             void *context);
  
  static void OPASQuery6(struct opas_msg_query6 *queryp, void *context);
  static void OPASReply6NoProxy(struct opas_msg_query6 *replyp,
                                void *context);
  static void OPASReply6Proxy(struct opas_msg_reply6_proxy *replyp,
                              void *context);
  
  static void OPASMsgUser(struct opas_msg_user_header *umsgp, void *data,
                          void *context);
  
  PXServer *mServer;
  opas_session_t mOPASSession;
  peak_stream mStream;
  bool mTimedOut;
  int mRefCnt, mStatus;
  
  uint32_t mQueryCount;
  uint32_t mScannedCount;
  uint32_t mProxyCount;
  uint32_t mErrorCount;
  };

#endif // INCLUDED_PXSESSION_H_
