/* Copyright (C) 2003, 2004 Stephane Thiell
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
#define RCSID "$Id"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cfgloader.h"
#include "debug.h"
#include "pxyservd_log.h"
#include "irc_send.h"

#include <assert.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <peak/peak.h>

static struct LogFDs
  {
  int fd;
  char *path;
  } log_fds[MAX_LOGID + 1];

static int lastcut_tog = 0;
static peak_timer log_timer;

static void log_timer_callback(peak_timer ti, void *context);
static char * log_copy_path(const char *p);
static void log_open(struct LogFDs *lfd);
static void log_stamp(void);

void
log_init()
  {
  log_fds[LOGID_CURRENT].fd = -1;
  log_fds[LOGID_CURRENT].path = log_copy_path(gConfig->log.current);
  log_fds[LOGID_SYSTEM].fd = -1;
  log_fds[LOGID_SYSTEM].path = log_copy_path(gConfig->log.system);
  log_fds[LOGID_GLINES].fd = -1;
  log_fds[LOGID_GLINES].path = log_copy_path(gConfig->log.glines);
  log_fds[LOGID_OPERCMDS].fd = -1;
  log_fds[LOGID_OPERCMDS].path = log_copy_path(gConfig->log.opercmds);
  log_fds[LOGID_STATS].fd = -1;
  log_fds[LOGID_STATS].path = log_copy_path(gConfig->log.stats);
  log_fds[LOGID_UNREACH].fd = -1;
  log_fds[LOGID_UNREACH].path = log_copy_path(gConfig->log.unreach);
  log_fds[LOGID_CLIENTS].fd = -1;
  log_fds[LOGID_CLIENTS].path = log_copy_path(gConfig->log.clients);
  
#if 0
  for (i = LOGID_CURRENT; i <= MAX_LOGID; i++)
    log_open(&log_fds[i]);
#endif
  
  /* We could try to find the right amount of time to wait until the next
   * rotation. But we might have problems with clock changes then, I'm not
   * sure.
   */
  log_timer = peak_timer_create(60, 60, log_timer_callback, NULL);
  if (log_timer == NULL)
    abort();
  peak_task_timer_add(peak_task_self(), log_timer);
  peak_release(log_timer); /* Give timer's ownership to task */
  }

void
log_finalize()
  {
  int i;
  /* Remove and auto release timer. */
  peak_task_timer_remove(peak_task_self(), log_timer);
  
  /* Close files. */
  for (i = LOGID_CURRENT; i <= MAX_LOGID; i++)
    if (log_fds[i].fd > -1)
      close(log_fds[i].fd);
  }

static void
log_timer_callback(peak_timer ti, void *context)
  {
  int count = log_rotate_check();
  
  if (count >= 0)
    send_to_console("[%s] Rotating log files (%d total)", gConfig->server.id,
                    count);
  }

static char *
log_copy_path(const char *p)
  {
  return (p != NULL) ? peak_strdup(p) : NULL;
  }

static void
log_open(struct LogFDs *lfd)
  {
  if (!lfd->path)
    return;
  
  if (lfd->fd < 0)
    lfd->fd = open(lfd->path, O_WRONLY | O_CREAT | O_APPEND,
                   S_IREAD | S_IWRITE);
  if (lfd->fd < 0)
    fprintf(stderr, "log_open: cannot open file '%s'\n", lfd->path);
  }

void
log_write(enum LogIDs id, const char *format, ...)
  {
  va_list vl;
  
  va_start(vl, format);
  log_vwrite(id, format, vl);
  va_end(vl);
  }

void
log_vwrite(enum LogIDs id, const char *format, va_list vl)
  {
  struct LogFDs* lfd;
  struct iovec vector[3];
  peak_time_date gdate;
  char logbuf[1024];
  char timebuf[32];
  
  assert(id <= MAX_LOGID);
  
  lfd = &log_fds[id];
  
  if (lfd->fd < 0)
    log_open(lfd);
  if (lfd->fd < 0)
    return;
  
  gdate = peak_time_get_date(peak_time(), gConfig->log.tz);
  
  vector[0].iov_len = snprintf(timebuf, sizeof(timebuf),
                               "[%d-%d-%d %d:%02d:%02d %s] ",
                               gdate.year, gdate.month, gdate.day,
                               gdate.hour, gdate.minute, (int)gdate.second,
                               peak_tz_get_abbreviation(gConfig->log.tz,
                                                        peak_time()));
  
  vector[0].iov_base = timebuf;
  vector[1].iov_len = vsnprintf(logbuf, sizeof(logbuf), format, vl);
  vector[1].iov_base = logbuf;
  vector[2].iov_base = "\n";
  vector[2].iov_len = 1;
  
  writev(lfd->fd, vector, 3);
  }

void
log_write_raw(enum LogIDs id, const char *format, ...)
  {
  va_list vl;
  
  va_start(vl, format);
  log_vwrite_raw(id, format, vl);
  va_end(vl);
  }

void
log_vwrite_raw(enum LogIDs id, const char *format, va_list vl)
  {
  struct LogFDs* lfd;
  struct iovec vector[2];
  char logbuf[512];
  
  assert(id <= MAX_LOGID);
  
  lfd = &log_fds[id];
  
  if (lfd->fd < 0)
    log_open(lfd);
  if (lfd->fd < 0)
    return;
  
  vector[0].iov_len = vsnprintf(logbuf, sizeof(logbuf), format, vl);
  vector[0].iov_base = logbuf;
  vector[1].iov_base = "\n";
  vector[1].iov_len = 1;
  writev(lfd->fd, vector, 2);
  }

/* Convenience */
void
log_system(const char *format, ...)
  {
  va_list vl;
  
  va_start(vl, format);
  log_vwrite(LOGID_SYSTEM, format, vl);
  va_end(vl);
  }


/* ROTATING ENGINE FOR LOG FILES
 * (from pxys v1 but improved with the help of libpeak)
 * 
 * log_rotate_check: Check if we need to sync the log files now
 * log_rotate_sync : Check log files TS at startup
 * log_rotate_cut  : Do the cut proxy.log -> proxy-yesterday.log
 * log_rotate_hist : Backup into history directory
 * log_stamp       : Write new TS in current log files
 */

int
log_rotate_check()
  {
  int res = -1;
  peak_time_date gdate;
  
  gdate = peak_time_get_date(peak_time(), gConfig->log.tz);
  
  if (gdate.hour == 0)
    {
    if (!lastcut_tog && gConfig->log.current && gConfig->log.daily)
      {
      close(log_fds[LOGID_CURRENT].fd);
      log_fds[LOGID_CURRENT].fd = -1;
      res = log_rotate_cut(gConfig->log.current, gConfig->log.daily, 1);
      log_stamp();
      
      if (log_fds[LOGID_CLIENTS].fd >= 0 && gConfig->log.clients)
        {
        close(log_fds[LOGID_CLIENTS].fd);
        log_fds[LOGID_CLIENTS].fd = -1;
        res = log_rotate_cut(gConfig->log.clients,
                             gConfig->log.dailyclients, 0);
        
        }
      
      lastcut_tog = 1;
      }
    }
  else
    lastcut_tog = 0;
  
  return res;
  }

void
log_rotate_sync()
  {
  FILE *fp;
  char buf[512];
  char *res;
  time_t last, now = peak_time();
  peak_time_date gdate_last, gdate_now;
  
  if (!(fp = fopen(gConfig->log.current, "r")))
    {
    log_stamp();
    return;
    }
  
  res = fgets(buf, sizeof(buf), fp);
  fclose(fp);
  
  if (!res)
    return;
  
  last = strtol(buf, NULL, 10);
  
  if (last <= 0)
    {
    /* Interesting... */
    unlink(gConfig->log.current);
    log_stamp();
    return;
    }
  
  if (now - last >= 86400)
    {
    /* Old proxy-current.log
     * case 1: it was yesterday: store it in proxy-yesterday.log
     * case 2: it was earlier: store it in history
     */
    if (now - last < 172800) /* 2 days */
      log_rotate_cut(gConfig->log.current, gConfig->log.daily, 1);
    else
      log_rotate_hist(1);
    }
  else if (now - last >= 3600)
    {
    /* Still a current log ? be careful of update hour... */
    gdate_last = peak_time_get_date(last, gConfig->log.tz);
    gdate_now = peak_time_get_date(now, gConfig->log.tz);
    
    if (gdate_now.hour < gdate_last.hour)
      /* one day passed! */
      log_rotate_cut(gConfig->log.current, gConfig->log.daily, 1);
    }
  else
    lastcut_tog = 1; /* avoid an autosync by log_rotate_check */
  }

/* Do the cut proxy.log -> proxy-yesterday.log
 */
int
log_rotate_cut(const char *srcpath, const char *dstpath, int history)
  {
  int nproxy = 0; /* result */
  time_t t;
  FILE *fp, *dstfp;
  char linebuf[512];
  
  /* Backup old proxy-yesterday.log to the history directory. */
  if (history && log_rotate_hist(0) < 0)
    goto log_rotate_cut;
  
  if (!(fp = fopen(srcpath, "r")))
    {
    log_system("log_rotate_cut: cannot open %s (%s)", srcpath,
               strerror(errno));
    goto log_rotate_cut;
    }
  
  if (!(dstfp = fopen(dstpath, "w")))
    {
    log_system("log_rotate_cut: cannot open %s for writing(%s)", dstpath,
               strerror(errno));
    fclose(fp);
    goto log_rotate_cut;
    }
  
  if (fgets(linebuf, sizeof(linebuf), fp))
    {
    peak_time_date gdate;
    
    t = strtol(linebuf, NULL, 10);
    fprintf(dstfp, "%s", linebuf); /* TS */
    
    gdate = peak_time_get_date(t, gConfig->log.tz);
    fprintf(dstfp, "Session started at %d-%d-%d %d:%02d:%02d %s\n",
            gdate.year, gdate.month, gdate.day,
            gdate.hour, gdate.minute, (int)gdate.second,
            peak_tz_get_abbreviation(gConfig->log.tz, t));
    
    gdate = peak_time_get_date(peak_time(), gConfig->log.tz);
    fprintf(dstfp, "Session closed at %d-%d-%d %d:%02d:%02d %s\n\n",
            gdate.year, gdate.month, gdate.day,
            gdate.hour, gdate.minute, (int)gdate.second,
            peak_tz_get_abbreviation(gConfig->log.tz, t));
    
    for ( ; fgets(linebuf, sizeof(linebuf), fp); nproxy++)
      fprintf(dstfp, "%s", linebuf);
    }
  fclose(dstfp);
  fclose(fp);
  
  unlink(srcpath);
log_rotate_cut:
  return nproxy;
  }

/* log_rotate_hist
 * resync:  0 -- move daily->history
 *          1 -- move current->history
 * result:  0 -- done
           -1 -- failed
 */
int
log_rotate_hist(int resync)
  {
  FILE *fp;
  time_t t;
  int res;
  char *str;
  peak_time_date gdate;
  const char *logfile = resync ? gConfig->log.current : gConfig->log.daily;
  char linebuf[512];
  char pathto[MAXPATHLEN];
  
  if (!(fp = fopen(logfile, "r")))
    {
    Debug((DL_MAX, "log_rotate_hist: cannot open file: %s", logfile));
    if (resync)
      {
      log_system("log_rotate_hist: cannot open file: %s", logfile);
      log_stamp();
      return -1;
      }
    return 0; /* not fatal for daily->history */
    }
  
  /* Get recorded TS */
  str = fgets(linebuf, sizeof(linebuf), fp);
  fclose(fp);
  
  if (!str)
    return -1;
  
  t = strtol(linebuf, NULL, 10);
  if (t <= 0)
    return -1; /* bah! */
  
  gdate = peak_time_get_date(t, gConfig->log.tz);
  snprintf(pathto, sizeof(pathto), "%s/proxy%d-%d-%d.log",
           gConfig->log.history_dirpath,
           gdate.year, gdate.month, gdate.day);
  
  res = rename(logfile, pathto);
  if (res < 0)
    {
    log_system("log_rotate_hist: cannot rename: %s -> %s",
               logfile, pathto);
    return -1;
    }
  return 0;
  }

static void
log_stamp()
  {
  struct LogFDs *lfd;
  char buf[32];
  
  snprintf(buf, sizeof(buf), "%ld\n", peak_time());
  lfd = &log_fds[LOGID_CURRENT];
  
  log_open(lfd);
  if (lfd->fd < 0)
    {
    fprintf(stderr, "\nlog_stamp: cannot open file %s\n", lfd->path);
    log_system("log_stamp: cannot open file %s", lfd->path);
    exit(1);
    }
  write(lfd->fd, buf, strlen(buf));
  }
