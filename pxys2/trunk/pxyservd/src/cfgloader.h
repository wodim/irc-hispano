/* Copyright (C) 2003 Stephane Thiell
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
#ifndef INCLUDED_CFGLOADER_H_
#define INCLUDED_CFGLOADER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libxml/tree.h>
#include <peak/peak.h>

/* Configuration structures for direct access
 */

typedef struct CServer
  {
  char *name;
  char *id;
  int numeric;
  int maxclients;
  char *info;
  int sendq;
  } CServer;

typedef struct CHub
  {
  struct in_addr address;
  int port;
  struct in_addr bind_address;
  char *password;
  } CHub;

typedef struct CHubLink
  {
  struct CHubLink *next;
  int refcount;
  CHub hub;
  } CHubLink;

typedef struct CClient
  {
  char *nickname;
  int nick_change_delay;
  char *username;
  char *hostname;
  char *realname;
  struct in_addr userip;
  char *usermode;
  char *channel;
  char *chanmode;
  int chanTS;
  int show_cached;
  } CClient;

#define NOSCAN_TYPE_SERVER 1
#define NOSCAN_TYPE_USERIP 2

typedef struct CNoScan
  {
  int type;
  union
    {
    char *server;
    struct
      {
      struct in_addr network;
      struct in_addr netmask;
      } userip;
    } u;
  } CNoScan;

typedef struct CNoScanLink
  {
  struct CNoScanLink *next;
  CNoScan noscan;
  } CNoScanLink;

typedef struct CScanner
  {
  struct in_addr address;
  int port;
  struct in_addr bind_address;
  } CScanner;

typedef struct CLog
  {
  char *current;
  char *daily;
  char *system;
  char *glines;
  char *opercmds;
  char *stats;
  char *unreach;
  char *clients;
  char *dailyclients;
  char *proxytop; /* not really a log file, here for convenience */
  char *history_dirpath;
  peak_tz tz;
  } CLog;

typedef struct CGline
  {
  int delay;
  char *reason[13];
  char *dnsblreason;
  } CGline;

typedef struct CNoticeLink
  {
  struct CNoticeLink *next;
  char *line;
  } CNoticeLink;

typedef struct CHelpTopic
  {
  char *title;
  char *content;
  int content_length;
  } CHelpTopic;

typedef struct CHelpLink
  {
  struct CHelpLink *next;
  CHelpTopic topic;
  } CHelpLink;

typedef struct Config
  {
  CServer server;
  CHubLink *hublist;
  int nhubs;
  CClient client;
  CNoScanLink *noscanlist;
  CScanner scanner;
  CLog log;
  CGline gline;
  CNoticeLink *noticelist;
  CHelpLink *helplist;
  } Config;

/* Current config */
extern Config *gConfig;

extern void cfgloader_init(void);

/* Load/reload public method */
extern int cfgloader_parse(const char *config_file);

/* Convenience accessor for hubs */
extern CHubLink * cfgloader_next_hublink(CHubLink *prev);
extern void cfgloader_retain_hublink(CHubLink *hl);
extern void cfgloader_release_hublink(CHubLink *hl);

/* Debug for -c command line option */
extern void cfgloader_dump(void);


#endif /* INCLUDED_CFGLOADER_H_ */
