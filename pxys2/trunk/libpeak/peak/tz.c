/* PEAK Library
 *
 * Copyright (c) 2003,2004
 *      Stephane Thiell <mbuna@bugged.org>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#define RCSID "$Id: tz.c,v 1.5 2005/01/27 16:31:50 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <peak/tz.h>
#include <peak/alloc.h>
#include <peak/dict.h>
#include "fileutils.h"
#include "internal.h"

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#ifdef HAVE_TZFILE_H
#include <tzfile.h>
#endif

#ifndef HAVE_STRUCT_TZHEAD
struct tzhead
  {
  char	tzh_reserved[24];	/* reserved for future use */
  char	tzh_ttisstdcnt[4];	/* coded number of trans. time flags */
  char	tzh_leapcnt[4];		/* coded number of leap seconds */
  char	tzh_timecnt[4];		/* coded number of transition times */
  char	tzh_typecnt[4];		/* coded number of local time types */
  char	tzh_charcnt[4];		/* coded number of abbr. chars */
  };
#endif

#ifndef TZDIR
#define TZDIR "/usr/share/zoneinfo"
#endif
#ifndef TZDEFAULT
#define TZDEFAULT "/etc/localtime"
#endif

static const peak_dict_init_entry abbrev2name[] =
  { { "ADT", "America/Halifax" },
    { "AFT", "Asia/Kabul" },
    { "AKDT", "America/Juneau" },
    { "AKST", "America/Juneau" },
    { "AST", "America/Halifax" },
    { "CDT", "America/Chicago" },
    { "CEST", "Europe/Rome" },
    { "CET", "Europe/Rome" },
    { "CST", "America/Chicago" },
    { "EDT", "America/New_York" },
    { "EEST", "Europe/Warsaw" },
    { "EET", "Europe/Warsaw" },
    { "EST", "America/New_York" },
    { "GMT", "GMT" },
    { "HKST", "Asia/Hong_Kong" },
    { "HST", "Pacific/Honolulu" },
    { "JST", "Asia/Tokyo" },
    { "MDT", "America/Denver" },
    { "MSD", "Europe/Moscow" },
    { "MSK", "Europe/Moscow" },
    { "MST", "America/Denver" },
    { "NZDT", "Pacific/Auckland" },
    { "NZST", "Pacific/Auckland" },
    { "PDT", "America/Los_Angeles" },
    { "PST", "America/Los_Angeles" },
    { "UTC", "UTC" },
    { "WEST", "Europe/Paris" },
    { "WET", "Europe/Paris" },
    { "YDT", "America/Yakutat" },
    { "YST", "America/Yakutat" }
  };

#define NUM_ABBREV (sizeof(abbrev2name)/sizeof(peak_dict_init_entry))

struct __peak_tz_period
  {
  time_t start;
  const char *abbrev;
  int32_t info; /* bit 30 stolen for DST - the rest is the offset */
  };

struct __peak_tz
  {
  PEAK_STRUCT_RT_HEADER;
  const char *_name;
  struct __peak_tz_period *_periods;
  int _period_cnt;
  };


static void __peak_tz_init(peak_tz tz, va_list vp, void *ctcx);
static void __peak_tz_finalize(peak_tz tz);
static struct __peak_tz_period * __peak_tz_bsearch_period(peak_tz tz,
                                                          time_t at);

PEAK_CLASS_BASE_DECLARE(tz);


/* Helpers */

static int32_t
__peak_tz_detzcode(const char * const codep)
  {
  int32_t result;
  int i;
  
  result = (int32_t)((codep[0] & 0x80) ? ~0L : 0L);
  for (i = 0; i < 4; ++i)
    result = (result << 8) | (codep[i] & 0xff);
  return result;
  }

void
__peak_tz_period_init(struct __peak_tz_period *period, int32_t start,
                      const char *abbrev, int32_t offset, int dst)
  {
  period->start = start;
  period->abbrev = peak_strdup(abbrev);
  period->info = abs(offset);
  
  if (offset < 0)
    period->info |= (1 << 31);
  if (dst)
    period->info |= (1 << 30);
  else
    period->info &= ~(1 << 30);
  }

int
__peak_tz_period_compare(const void *v1, const void *v2)
  {
  struct __peak_tz_period *tzp1 = (struct __peak_tz_period *)v1;
  struct __peak_tz_period *tzp2 = (struct __peak_tz_period *)v2;
  
  if (tzp1->start < tzp2->start)
    return -1;
  else
    return (tzp1->start == tzp2->start) ? 0 : 1;
  }

peak_tz
peak_tz_create(const char *tz_name)
  {
  char path_buf[MAXPATHLEN+1];
  void *data_buf;
  long data_len;
  const char *name = NULL;
  peak_dict abbrevs;
  
  if (!tz_name)
    return NULL;
  
  if (*tz_name != '/')
    {
    abbrevs = peak_dict_create(&peak_dict_string_key_ops,
                               &peak_dict_string_value_ops,
                               abbrev2name,
                               NUM_ABBREV);
    
    if (abbrevs)
      {
      name = (const char *)peak_dict_get_value(abbrevs, tz_name);
      peak_release(abbrevs);
      }
    if (!name)
      name = tz_name;
    snprintf(path_buf, sizeof(path_buf), "%s/%s", TZDIR, name);
    }
  else
    {
    char *p;
    
    strncpy(path_buf, tz_name, sizeof(path_buf) - 1);
    path_buf[sizeof(path_buf) - 1] = '\0'; 
    name = ((p = strrchr(path_buf, '/'))) ? p + 1 : path_buf;
    }
  
  if (!peak_read_file(path_buf, &data_buf, &data_len))
    return NULL;
  
  return PEAK_CLASS_CONSTRUCT3(tz, name, data_buf, data_len);
  }

static void
__peak_tz_init(peak_tz tz, va_list vp, void *ctcx)
  {
  long len;
  int i;
  int32_t timecnt, typecnt, charcnt, cnt, start, offset;
  const char *p, *timep, *typep, *ttypp, *charp;
  char **abbrs;
  uint8_t type, dst, idx;
  struct __peak_tz_period *tzp;
  int result = 1;
  
  tz->_name = peak_strdup(va_arg(vp, const char *));
  p = va_arg(vp, void*);
  len = va_arg(vp, long);
  
  if (len < (int)sizeof(struct tzhead))
    PEAK_CT_RAISE("failed to read zone info: truncated tzhead", 0);
  
  p += 24 + 4 + 4; /* reserved, ttisstdcnt, leapcnt */
  
  timecnt = __peak_tz_detzcode(p);
  p += 4;
  typecnt = __peak_tz_detzcode(p);
  p += 4;
  charcnt = __peak_tz_detzcode(p);
  p += 4;
  if ((typecnt <= 0 || timecnt < 0 || charcnt < 0)
      || (len - sizeof(struct tzhead) < (4 + 1) * timecnt
          + (4 + 1 + 1) * typecnt
          + charcnt))
    PEAK_CT_RAISE("failed to read zone info: bad tzhead values", 0);
  
  assert(typecnt > 0);
  
  timep = p;                    /* coded transition times a la time(2) */
  typep = p + 4 * timecnt;      /* types of local time starting at above */
  ttypp = typep + timecnt;      /* local time types */
  charp = ttypp + 6 * typecnt;  /* 0-terminated zone abbreviations */
  cnt = (0 < timecnt) ? timecnt : 1;
  tzp = peak_allocate(cnt * sizeof(struct __peak_tz_period));
  memset(tzp, 0, sizeof(cnt * sizeof(struct __peak_tz_period)));
#ifdef HAVE_ALLOCA
  abbrs = (char **)alloca((charcnt + 1) * sizeof(char *));
#else
  abbrs = (char **)peak_allocate((charcnt + 1) * sizeof(char *));
#endif
  for (i = charcnt; i--; )
    abbrs[i] = NULL;
  
  for (i = 0; i < cnt; i++)
    {
    if (0 == timecnt)
      start = INT_MIN;
    else
      {
      start = __peak_tz_detzcode(timep);
      timep += 4;
      }
    type = (0 < timecnt) ? (uint8_t)*typep++ : 0;
    if (typecnt <= type)
      {
      result = 0;
      break;
      }
    offset = __peak_tz_detzcode(ttypp + 6 * type);
    dst = (uint8_t)*(ttypp + 6 * type + 4);
    if (dst != 0 && dst != 1)
      {
      result = 0;
      break;
      }
    idx = (uint8_t)*(ttypp + 6 * type + 5);
    if (charcnt < idx)
      {
      result = 0;
      break;
      }
    if (!abbrs[idx])
      abbrs[idx] = (char *)(charp + idx);
    
    __peak_tz_period_init(tzp + i, start, abbrs[idx], offset, dst);
    }
#ifndef HAVE_ALLOCA
  peak_deallocate(abbrs);
#endif
  if (!result)
    {
    peak_deallocate(tzp);
    PEAK_CT_RAISE("failed to read zone info: can't parse data", 0);
    }
  
  /* cleanup */
  for (i = 0; i < cnt; i++)
    {
    if ((tzp[i].start == INT_MIN)
        && (i + 1 < cnt)
        && (tzp[i + 1].start == INT_MIN))
      {
      cnt--;
      memmove(&tzp[i], &tzp[i + 1],
              sizeof(struct __peak_tz_period) * (cnt - i));
      i--;
      }
    }
  for (i = 0; i < cnt; i++)
    {
    if ((tzp[i].start == INT_MAX)
        && (0 < i)
        && (tzp[i - 1].start == INT_MAX))
      {
      cnt--;
      memmove(&tzp[i], &tzp[i + 1],
              sizeof(struct __peak_tz_period) * (cnt - i));
      i--;
      }
    }
  qsort(tzp, cnt, sizeof(struct __peak_tz_period), __peak_tz_period_compare);
  
  tz->_periods = tzp;
  tz->_period_cnt = cnt;
  }

static void
__peak_tz_finalize(peak_tz tz)
  {
  /* XXX */
  if (tz->_periods)
    peak_deallocate(tz->_periods);
  
  peak_deallocate((void*)tz->_name);
  }

peak_tz
peak_tz_create_system()
  {
  char *tzenv;
  peak_tz tz;
  int res;
  char *name;
  char linkbuf[MAXPATHLEN+1];
  
  tzenv = getenv("TZFILE");
  if (tzenv != NULL)
    {
    tz = peak_tz_create(tzenv);
    if (tz)
      return tz;
    }
  
  tzenv = getenv("TZ");
  if (tzenv != NULL)
    {
    tz = peak_tz_create(tzenv);
    if (tz)
      return tz;
    }
  
  if ((res = readlink(TZDEFAULT, linkbuf, sizeof(linkbuf) - 1)) > 0)
    {
    linkbuf[res] = '\0';
    if (strncmp(linkbuf, TZDIR, sizeof(TZDIR) - 1) == 0)
      {
      name = linkbuf + sizeof(TZDIR);
      while (*name == '/')
        name++;
      }
    else
      name = linkbuf;
    
    if ((tz = peak_tz_create(name)))
      return tz;
    }
  return peak_tz_create("GMT");
  }

const char *
peak_tz_get_name(peak_tz tz)
  {
  return tz->_name;
  }

static struct __peak_tz_period *
__peak_tz_bsearch_period(peak_tz tz, time_t t)
  {
  struct __peak_tz_period *base = tz->_periods;
  struct __peak_tz_period *p;
  struct __peak_tz_period *result = NULL;
  size_t lim;
  
  for (lim = tz->_period_cnt; lim; lim >>= 1)
    {
    p = base + (lim >> 1);
    if (t >= p->start) /* key >= p: move right */
      {
      result = p;
      base = p + 1;
      lim--;
      } /* else move left */
    }
  
  if (result == NULL)
    {
    int i;
    
#if 1
    fprintf(stderr, "libpeak: __peak_tz_bsearch_period failed\n");
#endif
    for (i = 0; i < tz->_period_cnt && (tz->_periods[i].info & (1 << 31));
         i++)
      ;
    if (i < tz->_period_cnt)
      result = &tz->_periods[i];
    else
      result = &tz->_periods[0];
    }
  else
    {
    if (result > tz->_periods)
      {
      assert(result->start > (result - 1)->start);
      }
    if (result < (tz->_periods + tz->_period_cnt - 1))
      {
      assert(result->start < (result + 1)->start);
      }
    }
  return result;
  }

const char *
peak_tz_get_abbreviation(peak_tz tz, time_t t)
  {
  struct __peak_tz_period *period;
  
  period = __peak_tz_bsearch_period(tz, t);
  assert(period != NULL);
  
  return period->abbrev;
  }

time_t
peak_tz_get_gmt_offset(peak_tz tz, time_t t)
  {
  struct __peak_tz_period *period;
  time_t result;
  
  period = __peak_tz_bsearch_period(tz, t);
  assert(period != NULL);
  
  result = period->info & ~(3 << 30);
  if (period->info & (1 << 31))
    result = -result;
  return result;
  }


int
peak_tz_is_dst(peak_tz tz, time_t t)
  {
  struct __peak_tz_period *period;
  
  period = __peak_tz_bsearch_period(tz, t);
  assert(period != NULL);
  
  return (period->info & (1 << 30));
  }


