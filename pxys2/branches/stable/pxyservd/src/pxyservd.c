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
#define RCSID "$Id: pxyservd.c,v 1.3 2004/01/16 18:10:18 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cfgloader.h"
#include "debug.h"
#include "pxyservd_log.h"
#include "session.h"

#include <peak/peak.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

enum {
  OPT_FORK = 1 << 0,
  OPT_CHKCFG = 1 << 1,
  OPT_DLMAX = 1 << 2,
  OPT_DBOUT = 1 << 3
  };

static void display_usage(void);
static void display_version(void);
static void log_pid(void);
static int parse_options(int argc, char *argv[]);
static void signals_init(void);
static void configure_peak(void);

time_t gBirthTime;

static char *configFile;
static int main_options;

static void
display_usage()
  {
  fprintf(stderr, "*** Usage: pxyservd [-ctv] -f <config_file>\n");
  fprintf(stderr, "***   -c      check configuration file\n");
  fprintf(stderr, "***   -d      max debug mode\n");
  fprintf(stderr, "***   -t      run in foreground\n");
  fprintf(stderr, "***   -v      show version and exit\n");
  exit(1);
  }

static void
display_version()
  {
  fprintf(stderr, "*** pxyservd %s\n", VERSION /* TODO */);
  fprintf(stderr, "*** Part of pxys v.2\n");
  fprintf(stderr, "*** %s\n", RCSID);
  exit(0);
  }

static int
parse_options(int argc, char *argv[])
  {
  extern char *optarg;
  int c, options = OPT_FORK|OPT_DBOUT; /* default */

  while ((c = getopt(argc, argv, "cdf:tv")) != -1)
    {
    switch (c)
      {
      case 'c':
        options = (options | OPT_CHKCFG) & ~OPT_FORK;
        break;
      case 'd':
#ifdef DEBUG
        options |= OPT_DLMAX;
#else
        fprintf(stderr, "*** Sorry, the -d option is only available when "
                "--enable-debug is specified at compilation time\n");
        fprintf(stderr, "*** pxyservd not started\n");
        exit(1);
#endif
        break;
      case 'f':
        configFile = strdup(optarg);
        break;
      case 't':
        options &= ~OPT_FORK;
        break;
      case 'v':
        display_version();
        break;
      case '?':
      default:
        display_usage();
        break;
      }
    }
  
  if (!configFile)
    display_usage();
  
  return options;
  }

static void
log_pid()
  {
  char pidfile[] = "pxys.pid";
  FILE *fp;
  pid_t mypid;
  
  if ((fp = fopen(pidfile, "w")) == NULL)
    {
    fprintf(stderr, "Error opening pid file %s (%s)\n", pidfile,
            strerror(errno));
    exit(EXIT_FAILURE);
    }
  mypid = getpid();
  fprintf(fp, "%ld\n", (long)mypid);
  fclose(fp);
  }

static void
signals_callback(peak_signal s, int sig, void *context)
  {
  switch (sig)
    {
    case SIGHUP:
      cfgloader_parse(configFile);
      session_reload();
      break;
    case SIGINT:
      if (main_options & OPT_FORK)
        {
        fprintf(stderr, "Received SIGINT. Use SIGTERM to terminate "
                "pxyservd.\n");
        break;
        }
    case SIGTERM:
      session_stop("Terminated");
      log_finalize();
      break;
    }
  }

static void
signals_init()
  {
  int i, sn[] = { SIGHUP, SIGINT, SIGTERM };
  
  for (i = 0; i < 3; i++)
    peak_signal_schedule(peak_signal_create(sn[i], signals_callback, NULL),
                         peak_task_self());
  
  peak_signal_ignore(SIGPIPE);
  }

static void
configure_peak()
  {
  int val = 1;
  
  /* Very important, we want only one thread for event processing, and
   * it's not only because I am lazy and don't want to synchronize all
   * the stuffs. Trust me, there is no serious reason to enable it here...
   */
  peak_task_set_info(peak_task_self(), PEAK_TASK_FLAVOR_NTHREADS, &val);
  }

int
main(int argc, char *argv[])
  {
  main_options = parse_options(argc, argv);
  
  /* Pre fork */
  if ((main_options & OPT_FORK) && fork())
    exit(0);
  
  gBirthTime = time(0);
  srand((unsigned int)gBirthTime^getpid());
  
  configure_peak();
  
  DebugInit(main_options & OPT_DLMAX ? DL_MAX : DL_BASIC,
            main_options & OPT_DBOUT);
  
  cfgloader_init();
  if (cfgloader_parse(configFile) < 0)
    {
    fprintf(stderr, "Failed to load configuration file '%s'\n", configFile);
    return -1;
    }
  if (main_options & OPT_CHKCFG)
    {
    cfgloader_dump();
    fprintf(stderr, "Syntax OK\n");
    return 0;
    }
  
  log_init();
  log_rotate_sync();
  signals_init();
  log_pid();
  
  if (main_options & OPT_FORK)
    {
    /* Ready to daemonize */
    close(STDOUT_FILENO);
    close(STDIN_FILENO);
#if 0
    close(STDERR_FILENO);
#endif
    setsid();
    }
  
  session_run();
  return 0;
  }
