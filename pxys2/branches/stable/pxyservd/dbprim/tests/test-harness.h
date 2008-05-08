/*
** Copyright (C) 2004 by Kevin L. Mitchell <klmitch@mit.edu>
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public
** License along with this library; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
** MA 02111-1307, USA
**
** @(#)$Id: test-harness.h,v 1.3 2006/09/04 15:12:27 spale Exp $
*/
#ifndef __include_test_harness_h__
#define __include_test_harness_h__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define _TH_STATCODES	_THSC(PASS), _THSC(FAIL)
#define _TH_FATALMASK	0x00ff
#define _TH_FATAL	0x0100
#define FATAL(code)	(_TH_FATAL | (code & _TH_FATALMASK))

#ifdef __GNUC__
# if (__GNUC__ < 2) || (__GNUC__ == 2 && __GNUC_MINOR__ < 7)
#  define __attribute__(A)
# endif
#else
# define __attribute__(A)
#endif

#define _th_unused	__attribute__((unused))
#define _th_format(fmtidx, argidx)					      \
			__attribute__((format(printf, fmtidx, argidx)))

/* status codes for _th_report */
enum _th_status {
#define _THSC(code)	TR_ ## code
  _TH_STATCODES
#undef _THSC
};

/* Print out a status report */
static void _th_unused
_th_report(enum _th_status status, const char *test,
	   const char *fmt, va_list vp)
{
  static int init = 0;
  char *codes[] = {
#define _THSC(code)	#code
    _TH_STATCODES, 0
#undef _THSC
  };
  char buf[512];

  if (!init) { /* set output buffering... */
    setvbuf(stdout, 0, _IONBF, 0);
    init++;
  }

  if (status != TR_PASS && status != TR_FAIL)
    return; /* status in bounds? */

  /* Format and print the message */
  vsnprintf(buf, sizeof(buf), fmt, vp);
  printf("%s/%s:%s\n", codes[status], test, buf);
}

/* For declaring test programs... */
#define TEST_PROG(name, desc)
/* ...test program arguments... */
#define TEST_ARG(name, arg)
/* ...and test program dependencies */
#define TEST_DEP(name, arg)

/* For declaring tests to be extracted by the test data file generator;
 * obviously, no code is emitted.
 */
#define TEST_DECL(prog, test, desc)

/* For passing the test name to PASS/FAIL functions */
#define TEST_NAME(test)		#test

/* stand-alone PASS reporter */
static void _th_unused _th_format(2,3)
PASS(const char *test, const char *fmt, ...)
{
  va_list vp;

  va_start(vp, fmt);
  _th_report(TR_PASS, test, fmt, vp);
  va_end(vp);
}

/* stand-alone FAIL reporter; if fatal is non-zero, exit with that code */
static void _th_unused _th_format(3,4)
FAIL(const char *test, int fatal, const char *fmt, ...)
{
  va_list vp;

  va_start(vp, fmt);
  _th_report(TR_FAIL, test, fmt, vp);
  va_end(vp);

  if (fatal & _TH_FATAL)
    exit(fatal & _TH_FATALMASK);
}

/* The following variables and functions are for the TEST macro */
static const char *_th_test _th_unused = 0;
static int _th_fatal _th_unused = 0;

/* Report a pass */
static void _th_unused _th_format(1,2)
_th_PASS(const char *fmt, ...)
{
  va_list vp;

  va_start(vp, fmt);
  _th_report(TR_PASS, _th_test, fmt, vp);
  va_end(vp);
}

/* Report a failure */
static void _th_unused _th_format(1,2)
_th_FAIL(const char *fmt, ...)
{
  va_list vp;

  va_start(vp, fmt);
  _th_report(TR_FAIL, _th_test, fmt, vp);
  va_end(vp);

  if (_th_fatal & _TH_FATAL)
    exit(_th_fatal & _TH_FATALMASK);
}

/* perform a test and report the result */
#define TEST(prog, test, desc, cond, fatal, p_msg, f_msg)		      \
do {									      \
  /* save test name and fatal value; note that test argument must be a	      \
   * bare word!								      \
   */									      \
  _th_test = #test;							      \
  _th_fatal = (fatal);							      \
  if ((cond)) /* check condition... */					      \
    _th_PASS p_msg ; /* it passed */					      \
  else									      \
    _th_FAIL f_msg ; /* it failed */					      \
  _th_test = 0; /* zero the special global variables */			      \
  _th_fatal = 0;							      \
} while (0)

#endif /* __include_test_harness_h__ */
