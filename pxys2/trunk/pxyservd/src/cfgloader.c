/* Copyright (C) 2003, 2004 Stephane Thiell
 *
 * This file is part of pxyservd (from pxys2 suite)
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
#define RCSID "$Id: cfgloader.c,v 1.7 2006/09/04 12:48:08 spale Exp $"

#include "cfgloader.h"
#include "match.h"
#include <libbcl/memory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <peak/peak.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define XP_SERVER               "/pxyservd/server"
#define XP_SERVER_NAME          "name"
#define XP_SERVER_ID            "id"
#define XP_SERVER_NUMERIC       "numeric"
#define XP_SERVER_MAXCLIENTS    "maxclients"
#define XP_SERVER_INFO          "info"
#define XP_SERVER_SENDQ         "sendq"

#define XP_HUB                  "/pxyservd/hub"
#define XP_HUB_ADDRESS          "address"
#define XP_HUB_PORT             "port"
#define XP_HUB_BIND_ADDRESS     "bind-address"
#define XP_HUB_PASSWORD         "password"

#define XP_CLIENT               "/pxyservd/client"
#define XP_CLIENT_NICKNAME      "nickname"
#define XP_CLIENT_NICK_CHANGE_DELAY "nick-change-delay"
#define XP_CLIENT_USERNAME      "username"
#define XP_CLIENT_HOSTNAME      "hostname"
#define XP_CLIENT_REALNAME      "realname"
#define XP_CLIENT_USERIP        "userip"
#define XP_CLIENT_USERMODE      "usermode"
#define XP_CLIENT_CHANNEL       "channel"
#define XP_CLIENT_CHANMODE      "channel-mode"
#define XP_CLIENT_CHANTS        "channel-timestamp"
#define XP_CLIENT_SHOW_CACHED   "show-cached"

#define XP_NOSCAN               "/pxyservd/noscan"
#define XP_NOSCAN_SERVER        "server"
#define XP_NOSCAN_USERIP        "userip"

#define XP_SCANNER              "/pxyservd/scanner"
#define XP_SCANNER_ADDRESS      "address"
#define XP_SCANNER_PORT         "port"
#define XP_SCANNER_BIND_ADDRESS "bind-address"

#define XP_LOG                  "/pxyservd/log"
#define XP_LOG_FILE_CURRENT     "file[@type=\"current\"]"
#define XP_LOG_FILE_DAILY       "file[@type=\"daily\"]"
#define XP_LOG_FILE_SYSTEM      "file[@type=\"system\"]"
#define XP_LOG_FILE_GLINES      "file[@type=\"glines\"]"
#define XP_LOG_FILE_OPERCMDS    "file[@type=\"opercmds\"]"
#define XP_LOG_FILE_STATS       "file[@type=\"stats\"]"
#define XP_LOG_FILE_UNREACH     "file[@type=\"unreach\"]"
#define XP_LOG_FILE_CLIENTS     "file[@type=\"clients\"]"
#define XP_LOG_FILE_DAILYCLIENTS "file[@type=\"dailyclients\"]"
#define XP_LOG_FILE_PROXYTOP    "file[@type=\"proxytop\"]"
#define XP_LOG_HISTORY_DIRPATH  "history-dirpath"
#define XP_LOG_TIMEZONE         "timezone"

#define XP_GLINE                "/pxyservd/gline"
#define XP_GLINE_DELAY          "delay"
#define XP_GLINE_REASON0        "reason[@type=\"0\"]"
#define XP_GLINE_REASON1        "reason[@type=\"1\"]"
#define XP_GLINE_REASON2        "reason[@type=\"2\"]"
#define XP_GLINE_REASON3        "reason[@type=\"3\"]"
#define XP_GLINE_REASON4        "reason[@type=\"4\"]"
#define XP_GLINE_REASON5        "reason[@type=\"5\"]"
#define XP_GLINE_REASON6        "reason[@type=\"6\"]"
#define XP_GLINE_REASON7        "reason[@type=\"7\"]"
#define XP_GLINE_REASON8        "reason[@type=\"8\"]"
#define XP_GLINE_REASON9        "reason[@type=\"9\"]"
#define XP_GLINE_REASON10       "reason[@type=\"10\"]"
#define XP_GLINE_REASON11       "reason[@type=\"11\"]"

#define XP_NOTICE_LINE          "/pxyservd/notice/line"

#define XP_HELPMENU             "/pxyservd/help-menu"
#define XP_HELP_TOPIC_TITLE     "/pxyservd/help/@topic"
#define XP_HELP_TOPIC_PATTERN   "/pxyservd/help[@topic=\"%s\"]"

typedef struct CLink
  {
  struct CLink *next;
  int dummy;
  } CLink;

Config *gConfig;

#define DEF_CFG_EXCEPTIONS \
DEFINE_LOCAL_EXCEPTION(CFG_MISSING_PARAMETER) \
DEFINE_LOCAL_EXCEPTION(CFG_DUPLICATE_PARAMETER) \
DEFINE_LOCAL_EXCEPTION(CFG_EMPTY_PARAMETER) \
DEFINE_LOCAL_EXCEPTION(CFG_BAD_IP_ADDRESS) \
DEFINE_LOCAL_EXCEPTION(CFG_BAD_NETWORK_ADDRESS) \
DEFINE_LOCAL_EXCEPTION(CFG_UNKNOWN_TZ)

DEF_CFG_EXCEPTIONS

/* A few macros to make code more readable */
#define XPATH_EVAL_ONE(o, x) do { \
  o = cfgloader_eval((x), cx); \
  if (!(o) || !(o)->nodesetval->nodeNr) \
    RAISE(CFG_MISSING_PARAMETER, (void*)(x)); \
  if ((o)->nodesetval->nodeNr > 1) \
    RAISE(CFG_DUPLICATE_PARAMETER, (void*)(x)); } while(0)

#define XPATH_CXNODE_SAVE    cxn_save = cx->node
#define XPATH_CXNODE_CHANGE  cx->node = blockp->nodesetval->nodeTab[0]
#define XPATH_CXNODE_RESTORE cx->node = cxn_save

static void xfree(void *ptr);

static xmlXPathObjectPtr cfgloader_eval(const char *xpath,
                                        xmlXPathContextPtr context);

static void cfgloader_getaddr(const char *xpath,
                              xmlXPathContextPtr cx,
                              xmlDocPtr doc,
                              int m,
                              const char *debugpath,
                              struct in_addr *oaddr);

static int cfgloader_geti(const char *xpath,
                          xmlXPathContextPtr cx,
                          xmlDocPtr doc,
                          int m);

static xmlChar* cfgloader_gets(const char *xpath,
                               xmlXPathContextPtr cx,
                               xmlDocPtr doc,
                               int m);

static CLink* cfgloader_reverse_list(CLink *list);

static void cfgloader_doparse(Config *cfg,
                              xmlXPathContextPtr cx,
                              xmlDocPtr doc);

static void cfgloader_destroy(Config *cfg);


static void
xfree(void *ptr)
  {
  if (ptr)
    xmlFree(ptr);
  }

void
cfgloader_init()
  {
  gConfig = NULL;
  }

static xmlXPathObjectPtr
cfgloader_eval(const char *xpath, xmlXPathContextPtr context)
  {
  xmlXPathObjectPtr result;
  result = xmlXPathEvalExpression(xpath, context);
  
  if (result && xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
    xmlXPathFreeObject(result);
    return NULL;
    }
  return result;
  }

static void
cfgloader_getaddr(const char *xpath, xmlXPathContextPtr cx, xmlDocPtr doc,
                  int m, const char *debugpath, struct in_addr *oaddr)
  {
  xmlChar *str;
  
  if (!(str = cfgloader_gets(xpath, cx, doc, m)))
    oaddr->s_addr = INADDR_ANY;
  else
    {
    int res = inet_pton(AF_INET, str, oaddr);
    xmlFree(str);
    if (res == -1)
      RAISE(CFG_BAD_IP_ADDRESS, (void*)debugpath);
    }
  }

static int
cfgloader_geti(const char *xpath, xmlXPathContextPtr cx, xmlDocPtr doc, int m)
  {
  int result;
  xmlChar *s;
  if ((s = cfgloader_gets(xpath, cx, doc, m)) != NULL)
    {
    result = atoi(s);
    xmlFree(s);
    }
  else
    result = 0;
  return result;
  }

static xmlChar*
cfgloader_gets(const char *xpath, xmlXPathContextPtr cx, xmlDocPtr doc, int m)
  {
  xmlXPathObjectPtr result;
  xmlNodePtr node;
  xmlChar *s;
  
  result = cfgloader_eval(xpath, cx);
  if (!result || !result->nodesetval->nodeNr)
    {
    if (result)
      xmlXPathFreeObject(result);
    if (m) /* mandatory */
      RAISE(CFG_MISSING_PARAMETER, (void*)xpath);
    return NULL;
    }
  if (m > 0 && result->nodesetval->nodeNr > m)
    RAISE(CFG_DUPLICATE_PARAMETER, (void*)xpath);
  
  node = result->nodesetval->nodeTab[0]->xmlChildrenNode;
  s = xmlNodeListGetString(doc, node, 1);
  xmlXPathFreeObject(result);
  if (!s && m)
    RAISE(CFG_EMPTY_PARAMETER, (void*)xpath);
  return s;
  }

int
cfgloader_parse(const char *config_file)
  {
  xmlDocPtr doc;
  xmlXPathContextPtr cx;
  Config *cfg;
  
  if (config_file == NULL)
    return -1;
  
  cfg = (Config *)memory_alloc(sizeof(Config));
  memset(cfg, 0, sizeof(Config));

  doc = xmlParseFile(config_file);
  
  if (doc == NULL)
    return -1;
  
  cx = xmlXPathNewContext(doc);
  
  HANDLE(any, cfgloader_doparse(cfg, cx, doc));
  if (EXCEPTION_RAISED(any))
    {
    fprintf(stderr, "Error <%s> hint: %s\n", exception_current_name(),
            (char *)exception_current_parameter());
    xmlXPathFreeContext(cx);
    return -1;
    }
  /* Success */
  xmlXPathFreeContext(cx);
  cfgloader_destroy(gConfig);
  gConfig = cfg;
  return 0;
  }

static void
cfgloader_doparse(Config *cfg, xmlXPathContextPtr cx, xmlDocPtr doc)
  {
  xmlXPathObjectPtr blockp, o;
  xmlNodePtr cxn_save, node;
  xmlChar *str, *timezone_name;
  CHelpLink *help;
  int i;
  
  XPATH_CXNODE_SAVE;
  XPATH_EVAL_ONE(blockp, XP_SERVER);
  XPATH_CXNODE_CHANGE;
  
  /* Service server main config */
  cfg->server.name = cfgloader_gets(XP_SERVER_NAME, cx, doc, 1);
  cfg->server.id = cfgloader_gets(XP_SERVER_ID, cx, doc, 1);
  cfg->server.numeric = cfgloader_geti(XP_SERVER_NUMERIC, cx, doc, 1);
  cfg->server.maxclients = cfgloader_geti(XP_SERVER_MAXCLIENTS, cx, doc, 1);
  cfg->server.info = cfgloader_gets(XP_SERVER_INFO, cx, doc, 1);
  cfg->server.sendq = cfgloader_geti(XP_SERVER_SENDQ, cx, doc, 1);
  
  xmlXPathFreeObject(blockp);
  XPATH_CXNODE_RESTORE;
  
  /* Hubs */
  blockp = cfgloader_eval(XP_HUB, cx);
  if (!blockp || !blockp->nodesetval->nodeNr)
    RAISE(CFG_MISSING_PARAMETER, (void*)XP_HUB);
  
  for (i = 0; i < blockp->nodesetval->nodeNr; i++)
    {
    /* New hub */
    CHubLink hl, *hp;
    
    /* Update current node */
    cx->node = blockp->nodesetval->nodeTab[i];
    
    /* Address */
    cfgloader_getaddr(XP_HUB_ADDRESS, cx, doc, 1, "hub/address",
                      &hl.hub.address);
    /* Port */
    hl.hub.port = cfgloader_geti(XP_HUB_PORT, cx, doc, 1);
    
    /* Bind address */
    cfgloader_getaddr(XP_HUB_BIND_ADDRESS, cx, doc, 0, "hub/bind-address",
                      &hl.hub.bind_address);
    
    /* C:line password */
    hl.hub.password = cfgloader_gets(XP_HUB_PASSWORD, cx, doc, 1);
    
    *(hp = (CHubLink *)memory_alloc(sizeof(CHubLink))) = hl;
    hp->next = cfg->hublist;
    hp->refcount = 1;
    cfg->hublist = hp;
    cfg->nhubs++;
    }
  cfg->hublist = (CHubLink *)cfgloader_reverse_list((CLink *)cfg->hublist);

  xmlXPathFreeObject(blockp);
  XPATH_CXNODE_RESTORE;
  
  /* Virtual client */
  XPATH_EVAL_ONE(blockp, XP_CLIENT);
  XPATH_CXNODE_CHANGE;
  
  cfg->client.nickname = cfgloader_gets(XP_CLIENT_NICKNAME, cx, doc, 1);
  cfg->client.nick_change_delay = cfgloader_geti(XP_CLIENT_NICK_CHANGE_DELAY,
                                                 cx, doc, 0);
  cfg->client.username = cfgloader_gets(XP_CLIENT_USERNAME, cx, doc, 1);
  cfg->client.hostname = cfgloader_gets(XP_CLIENT_HOSTNAME, cx, doc, 1);
  cfg->client.realname = cfgloader_gets(XP_CLIENT_REALNAME, cx, doc, 1);
  cfgloader_getaddr(XP_CLIENT_USERIP, cx, doc, 0, "client/userip",
                    &cfg->client.userip);
  cfg->client.usermode = cfgloader_gets(XP_CLIENT_USERMODE, cx, doc, 0);
  cfg->client.channel = cfgloader_gets(XP_CLIENT_CHANNEL, cx, doc, 0);
  cfg->client.chanmode = cfgloader_gets(XP_CLIENT_CHANMODE, cx, doc, 0);
  cfg->client.chanTS = cfgloader_geti(XP_CLIENT_CHANTS, cx, doc, 0);
  if (cfg->client.chanTS == 0)
    cfg->client.chanTS = peak_time();
  cfg->client.show_cached = cfgloader_geti(XP_CLIENT_SHOW_CACHED, cx, doc, 1);
  
  xmlXPathFreeObject(blockp);
  XPATH_CXNODE_RESTORE;

  /* NoScan */
  XPATH_EVAL_ONE(blockp, XP_NOSCAN);
  XPATH_CXNODE_CHANGE;
  xmlXPathFreeObject(blockp);
  
  /* "NoScanned" servers */
  o = cfgloader_eval(XP_NOSCAN_SERVER, cx);
  if (o && o->nodesetval->nodeNr > 0)
    {
    for (i = 0; i < o->nodesetval->nodeNr; i++)
      {
      node = o->nodesetval->nodeTab[i]->xmlChildrenNode;
      str = xmlNodeListGetString(doc, node, 1);
      if (str)
        {
        CNoScanLink *nl = (CNoScanLink *)memory_alloc(sizeof(CNoScanLink));
        nl->next = cfg->noscanlist;
        nl->noscan.type = NOSCAN_TYPE_SERVER;
        nl->noscan.u.server = collapse(str);
        cfg->noscanlist = nl;
        }
      else
        RAISE(CFG_EMPTY_PARAMETER, (void*)"noscan/server");
      }
    }
  
  if (o)
    xmlXPathFreeObject(o);
  
  /* "NoScanned" userips */
  o = cfgloader_eval(XP_NOSCAN_USERIP, cx);
  if (o && o->nodesetval->nodeNr > 0)
    {
    for (i = 0; i < o->nodesetval->nodeNr; i++)
      {
      node = o->nodesetval->nodeTab[i]->xmlChildrenNode;
      str = xmlNodeListGetString(doc, node, 1);
      if (str)
        {
        CNoScanLink *nl;
        struct in_addr in, nm;
        char ipbuf[16];
        size_t len;
        int j, k, dot = 0;
        char *p;

	memset(&in,0,sizeof(struct in_addr));
	memset(&nm,0,sizeof(struct in_addr));
        
        if ((p = strchr(str, '/')) == NULL)
          nm.s_addr = htonl(0xffffffff);
        else
          {
          *p++ = '\0';
          if (!strchr(p, '.'))
            {
            int nbits;
            
            if ((nbits = atoi(p)) > 0 && nbits <= 32)
              nm.s_addr = htonl(~(0xffffffff >> nbits));
            }
          else if (inet_pton(AF_INET, p, &in) == -1)
            RAISE(CFG_BAD_NETWORK_ADDRESS, "noscan/userip");
          }
        
        len = strlen(str);
        len = len < sizeof(ipbuf) ? len : sizeof(ipbuf) - 1;
        for (j = 0; j < len; j++)
          {
          if (str[j] == '.')
            dot++;
          ipbuf[j] = str[j];
          }
        if (dot > 4 || len >= sizeof(ipbuf) - ((3 - dot) * 2))
          RAISE(CFG_BAD_NETWORK_ADDRESS, "noscan/userip");
        
        for (k = 0; k < 3 - dot; k++)
          {
          ipbuf[j++] = '.';
          ipbuf[j++] = '0';
          }
        
        assert(j < sizeof(ipbuf));
        ipbuf[j] = '\0';
        
        if (inet_pton(AF_INET, ipbuf, &in) == -1)
          RAISE(CFG_BAD_NETWORK_ADDRESS, "noscan/userip");
        
        nl = (CNoScanLink *)memory_alloc(sizeof(CNoScanLink));
        nl->next = cfg->noscanlist;
        nl->noscan.type = NOSCAN_TYPE_USERIP;
        nl->noscan.u.userip.network = in;
        nl->noscan.u.userip.netmask = nm;
        cfg->noscanlist = nl;
        
        xmlFree(str);
        }
      else
        RAISE(CFG_EMPTY_PARAMETER, (void*)"noscan/userip");
      }
    }
  
  if (o)
    xmlXPathFreeObject(o);
  
  XPATH_CXNODE_RESTORE;
  
  /* Scanner */
  XPATH_EVAL_ONE(blockp, XP_SCANNER);
  XPATH_CXNODE_CHANGE;
  /* Change current node */
  cx->node = blockp->nodesetval->nodeTab[0];
  
  cfgloader_getaddr(XP_SCANNER_ADDRESS, cx, doc, 1, "scanner/address",
                    &cfg->scanner.address);
  
  cfg->scanner.port = cfgloader_geti(XP_SCANNER_PORT, cx, doc, 1);
  
  cfgloader_getaddr(XP_SCANNER_BIND_ADDRESS, cx, doc, 0,
                    "scanner/bind-address",
                    &cfg->scanner.bind_address);
  xmlXPathFreeObject(blockp);
  XPATH_CXNODE_RESTORE;
  
  /* Log */
  XPATH_EVAL_ONE(blockp, XP_LOG);
  XPATH_CXNODE_CHANGE;
  
  cfg->log.current = cfgloader_gets(XP_LOG_FILE_CURRENT, cx, doc, 0);
  cfg->log.daily = cfgloader_gets(XP_LOG_FILE_DAILY, cx, doc, 0);
  cfg->log.system = cfgloader_gets(XP_LOG_FILE_SYSTEM, cx, doc, 0);
  cfg->log.glines = cfgloader_gets(XP_LOG_FILE_GLINES, cx, doc, 0);
  cfg->log.opercmds = cfgloader_gets(XP_LOG_FILE_OPERCMDS, cx, doc, 0);
  cfg->log.stats = cfgloader_gets(XP_LOG_FILE_STATS, cx, doc, 0);
  cfg->log.unreach = cfgloader_gets(XP_LOG_FILE_UNREACH, cx, doc, 0);
  cfg->log.clients = cfgloader_gets(XP_LOG_FILE_CLIENTS, cx, doc, 0);
  cfg->log.dailyclients = cfgloader_gets(XP_LOG_FILE_DAILYCLIENTS, cx, doc, 0);
  cfg->log.proxytop = cfgloader_gets(XP_LOG_FILE_PROXYTOP, cx, doc, 1);
  cfg->log.history_dirpath = cfgloader_gets(XP_LOG_HISTORY_DIRPATH, cx, doc,1);
  timezone_name = cfgloader_gets(XP_LOG_TIMEZONE, cx, doc, 0);
  if (!timezone_name)
    cfg->log.tz = peak_tz_create_system();
  else
    cfg->log.tz = peak_tz_create(timezone_name);
  if (!cfg->log.tz)
    RAISE(CFG_UNKNOWN_TZ, (void*)"log/timezone");
  
  xmlXPathFreeObject(blockp);
  XPATH_CXNODE_RESTORE;
  
  /* Gline */
  XPATH_EVAL_ONE(blockp, XP_GLINE);
  XPATH_CXNODE_CHANGE;
  
  cfg->gline.delay = cfgloader_geti(XP_GLINE_DELAY, cx, doc, 1);
  cfg->gline.reason[0] = cfgloader_gets(XP_GLINE_REASON0, cx, doc, 1);
  cfg->gline.reason[1] = cfgloader_gets(XP_GLINE_REASON1, cx, doc, 0);
  cfg->gline.reason[2] = cfgloader_gets(XP_GLINE_REASON2, cx, doc, 0);
  cfg->gline.reason[3] = cfgloader_gets(XP_GLINE_REASON3, cx, doc, 0);
  cfg->gline.reason[4] = cfgloader_gets(XP_GLINE_REASON4, cx, doc, 0);
  cfg->gline.reason[5] = cfgloader_gets(XP_GLINE_REASON5, cx, doc, 0);
  cfg->gline.reason[6] = cfgloader_gets(XP_GLINE_REASON6, cx, doc, 0);
  cfg->gline.reason[7] = cfgloader_gets(XP_GLINE_REASON7, cx, doc, 0);
  cfg->gline.reason[8] = cfgloader_gets(XP_GLINE_REASON8, cx, doc, 0);
  cfg->gline.reason[9] = cfgloader_gets(XP_GLINE_REASON9, cx, doc, 0);
  cfg->gline.reason[10] = cfgloader_gets(XP_GLINE_REASON10, cx, doc, 0);
  cfg->gline.reason[11] = cfgloader_gets(XP_GLINE_REASON11, cx, doc, 0);

  for (i = 1; i < 12; i++)
    if (cfg->gline.reason[i] == NULL)
      cfg->gline.reason[7] = strdup(cfg->gline.reason[0]);
  
  xmlXPathFreeObject(blockp);
  XPATH_CXNODE_RESTORE;

  /* Notice */
  blockp = cfgloader_eval(XP_NOTICE_LINE, cx);
  if (blockp)
    {
    for (i = 0; i < blockp->nodesetval->nodeNr; i++)
      {
      CNoticeLink nl, *np;
      
      node = blockp->nodesetval->nodeTab[i]->xmlChildrenNode;
      nl.line = xmlNodeListGetString(doc, node, 1);
      if (nl.line)
        {
        nl.next = cfg->noticelist;
        *(np = (CNoticeLink *)memory_alloc(sizeof(CNoticeLink))) = nl;
        cfg->noticelist = np;
        }
      }
    xmlXPathFreeObject(blockp);
    }
  cfg->noticelist = (CNoticeLink*)cfgloader_reverse_list(
                                        (CLink*)cfg->noticelist);
  
  /* Help (with topic title) */
  blockp = cfgloader_eval(XP_HELP_TOPIC_TITLE, cx);
  if (blockp)
    {
    char titlebuf[256];
    
    for (i = 0; i < blockp->nodesetval->nodeNr; i++)
      {
      help = memory_alloc(sizeof(CHelpLink));
      node = blockp->nodesetval->nodeTab[i]->xmlChildrenNode;
      help->topic.title = xmlNodeListGetString(doc, node, 1);
      /* Select content */
      snprintf(titlebuf, sizeof(titlebuf), XP_HELP_TOPIC_PATTERN,
               help->topic.title);
      XPATH_EVAL_ONE(o, titlebuf);
      node = o->nodesetval->nodeTab[0]->xmlChildrenNode;
      help->topic.content = xmlNodeListGetString(doc, node, 1);
      help->topic.content_length = xmlStrlen(help->topic.content);
      help->next = cfg->helplist;
      cfg->helplist = help;
      xmlXPathFreeObject(o);
      }
    xmlXPathFreeObject(blockp);
    }
  
  /* Help (parsed after so that we don't need to reverse the list) */
  XPATH_EVAL_ONE(blockp, XP_HELPMENU);
  help = memory_alloc(sizeof(CHelpLink));
  help->topic.title = NULL;
  node = blockp->nodesetval->nodeTab[0]->xmlChildrenNode;
  help->topic.content = xmlNodeListGetString(doc, node, 1);
  help->topic.content_length = xmlStrlen(help->topic.content);
  help->next = cfg->helplist;
  cfg->helplist = help;      /* general help always on top of list */
  xmlXPathFreeObject(blockp);
  }

static CLink*
cfgloader_reverse_list(CLink *list)
  {
  CLink *prev = NULL, *link, *next;
  
  for (link = (CLink *)list; link; link = next)
    {
    next = link->next;
    link->next = prev;
    prev = link;
    }
  return prev;
  }

CHubLink *
cfgloader_next_hublink(CHubLink *prev)
  {
  return (!prev || !prev->next) ? gConfig->hublist : prev->next;
  }

void
cfgloader_retain_hublink(CHubLink *hl)
  {
  hl->refcount++;
  }

void
cfgloader_release_hublink(CHubLink *hl)
  {
  /* We don't bother to unlink it from the list: cfgloader retains the whole
   * list and releases it completely when the conf is reloaded.
   */
  if (--hl->refcount == 0)
    {
    xfree(hl->hub.password);
    memory_free(hl);
    }
  }

static void
cfgloader_destroy(Config *cfg)
  {
  CHubLink *link, *link_next;
  CNoScanLink *noscan, *noscan_next;
  CNoticeLink *notice, *notice_next;
  CHelpLink *help, *help_next;
  
  if (cfg == NULL)
    return;
  
  xfree(cfg->server.name);
  xfree(cfg->server.id);
  xfree(cfg->server.info);
  
  for (link = cfg->hublist; link; link = link_next)
    {
    link_next = link->next;
    cfgloader_release_hublink(link);
    }
  
  xfree(cfg->client.nickname);
  xfree(cfg->client.username);
  xfree(cfg->client.hostname);
  xfree(cfg->client.realname);
  xfree(cfg->client.usermode);
  xfree(cfg->client.channel);
  xfree(cfg->client.chanmode);
  
  for (noscan = cfg->noscanlist; noscan; noscan = noscan_next)
    {
    noscan_next = noscan->next;
    if (noscan->noscan.type == NOSCAN_TYPE_SERVER)
      xfree(noscan->noscan.u.server);
    memory_free(noscan);
    }
  
  xfree(cfg->log.current);
  xfree(cfg->log.daily);
  xfree(cfg->log.system);
  xfree(cfg->log.glines);
  xfree(cfg->log.opercmds);
  xfree(cfg->log.stats);
  xfree(cfg->log.unreach);
  xfree(cfg->log.clients);
  xfree(cfg->log.dailyclients);
  xfree(cfg->log.proxytop);
  xfree(cfg->log.history_dirpath);
  if (cfg->log.tz)
    peak_release(cfg->log.tz);
  
  xfree(cfg->gline.reason[0]);
  xfree(cfg->gline.reason[1]);
  xfree(cfg->gline.reason[2]);
  xfree(cfg->gline.reason[3]);
  
  for (notice = cfg->noticelist; notice; notice = notice_next)
    {
    notice_next = notice->next;
    xfree(notice->line);
    memory_free(notice);
    }
  
  for (help = cfg->helplist; help; help = help_next)
    {
    help_next = help->next;
    xfree(help->topic.title);
    xfree(help->topic.content);
    memory_free(help);
    }
  }

void
cfgloader_dump()
  {
  Config *cfg = gConfig;
  CHubLink *link;
  CNoScanLink *noscan;
  CNoticeLink *notice;
  CHelpLink *help;
  int i;
  
  printf("Server:name = %s\n", cfg->server.name);
  printf("Server:id = %s\n", cfg->server.id);
  printf("Server:numeric = %d\n", cfg->server.numeric);
  printf("Server:maxclients = %d\n", cfg->server.maxclients);
  printf("Server:info = \"%s\"\n", cfg->server.info);
  printf("\n");
  for (link = cfg->hublist, i = 0; link; link = link->next, i++)
    {
    printf("Hub[%d]:address = %s\n", i, inet_ntoa(link->hub.address));
    printf("Hub[%d]:port = %d\n", i, link->hub.port);
    printf("Hub[%d]:bind_address = %s\n", i,
           inet_ntoa(link->hub.bind_address));
    printf("Hub[%d]:password = %s\n\n", i, link->hub.password);
    }
  printf("Client:nickname = %s\n", cfg->client.nickname);
  printf("Client:username = %s\n", cfg->client.username);
  printf("Client:hostname = %s\n", cfg->client.hostname);
  printf("Client:realname = \"%s\"\n", cfg->client.realname);
  printf("Client:userip = %s\n", inet_ntoa(cfg->client.userip));
  printf("Client:usermode = %s\n", cfg->client.usermode);
  printf("Client:channel = %s\n", cfg->client.channel);
  printf("Client:chanmode = %s\n", cfg->client.chanmode);
  printf("Client:chanTS = %d\n", cfg->client.chanTS);
  printf("\n");
  for (noscan = cfg->noscanlist; noscan; noscan = noscan->next)
    {
    switch (noscan->noscan.type)
      {
      case NOSCAN_TYPE_SERVER:
        printf("NoScan:server = %s\n", noscan->noscan.u.server);
        break;
      case NOSCAN_TYPE_USERIP:
        {
        struct in_addr netstart, netend;
        netstart.s_addr = noscan->noscan.u.userip.network.s_addr
          & noscan->noscan.u.userip.netmask.s_addr;
        netend.s_addr = netstart.s_addr
          + ~noscan->noscan.u.userip.netmask.s_addr;
        printf("NoScan:userip = %s",
               inet_ntoa(noscan->noscan.u.userip.network));
        printf("/%s\n", inet_ntoa(noscan->noscan.u.userip.netmask));
        printf("                (%s", inet_ntoa(netstart));
        printf(" - %s)\n", inet_ntoa(netend));
        break;
        }
      default:
        fprintf(stderr, "NoScan: Internal error - BAD NOSCAN TYPE\n");
        break;
      }
    }
  printf("\n");
  printf("Scanner:address = %s\n", inet_ntoa(cfg->scanner.address));
  printf("Scanner:port = %d\n", cfg->scanner.port);
  printf("Scanner:bind-address = %s\n", inet_ntoa(cfg->scanner.bind_address));
  printf("\n");
  printf("Log:current = %s\n", cfg->log.current);
  printf("Log:daily = %s\n", cfg->log.daily);
  printf("Log:system = %s\n", cfg->log.system);
  printf("Log:glines = %s\n", cfg->log.glines);
  printf("Log:opercmds = %s\n", cfg->log.opercmds);
  printf("Log:stats = %s\n", cfg->log.stats);
  printf("Log:unreach = %s\n", cfg->log.unreach);
  printf("Log:clients = %s\n", cfg->log.clients);
  printf("Log:dailyclients = %s\n", cfg->log.dailyclients);
  printf("Log:proxytop = %s\n", cfg->log.proxytop);
  printf("Log:history-dirpath = %s\n", cfg->log.history_dirpath);
  printf("Log:timezone = %s (%s)\n", peak_tz_get_name(cfg->log.tz),
         peak_tz_get_abbreviation(cfg->log.tz, peak_time()));
  printf("\n");
  printf("Gline:delay = %d\n", cfg->gline.delay);
  printf("Gline:reason[0] = \"%s\"\n", cfg->gline.reason[0]);
  printf("Gline:reason[1] = \"%s\"\n", cfg->gline.reason[1]);
  printf("Gline:reason[2] = \"%s\"\n", cfg->gline.reason[2]);
  printf("Gline:reason[3] = \"%s\"\n", cfg->gline.reason[3]);
  printf("Gline:reason[4] = \"%s\"\n", cfg->gline.reason[4]);
  printf("Gline:reason[5] = \"%s\"\n", cfg->gline.reason[5]);
  printf("Gline:reason[6] = \"%s\"\n", cfg->gline.reason[6]);
  printf("Gline:reason[7] = \"%s\"\n", cfg->gline.reason[7]);
  printf("Gline:reason[8] = \"%s\"\n", cfg->gline.reason[8]);
  printf("Gline:reason[9] = \"%s\"\n", cfg->gline.reason[9]);
  printf("Gline:reason[10] = \"%s\"\n", cfg->gline.reason[10]);
  printf("Gline:reason[11] = \"%s\"\n", cfg->gline.reason[11]);
  if (cfg->noticelist)
    printf("\n");
  for (notice = cfg->noticelist; notice; notice = notice->next)
    printf("Notice:line = \"%s\"\n", notice->line);
  printf("\n");
  help = cfg->helplist;
  printf("Help(general) = \"%s\"\n", help->topic.content);
  for (help = help->next; help; help = help->next)
    printf("Help(topic=\"%s\") = \"%s\"\n", help->topic.title,
           help->topic.content);
  printf("\n");
  }
