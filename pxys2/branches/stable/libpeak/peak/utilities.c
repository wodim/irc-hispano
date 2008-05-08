/* PEAK Library
 *
 * Copyright (c) 2003
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
#define RCSID "$Id: utilities.c,v 1.1.1.1 2003/12/30 02:29:39 mbuna Exp $"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "utilities.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/resource.h>
#ifdef HAVE_SYS_MPCTL_H
#include <sys/mpctl.h>
#endif
#ifdef HAVE_SYS_SYSCTL_H
#include <sys/sysctl.h>
#endif

#include "internal.h"


/* peak_get_ncpus */

#if defined(HAVE_SYSCTL) && defined(CTL_HW) && defined(HW_NCPU)

/* BSD sysctl */

int
peak_get_ncpus()
    {
    int mib[2], ncpus;
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    len = sizeof(ncpus);
    if (sysctl(mib, 2, &ncpus, &len, NULL, 0) == -1)
        PEAK_HALT;
    return ncpus;
    }

#elif defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_ONLN)

/* POSIX.1 sysconf */

int
peak_get_ncpus()
    {
    return sysconf(_SC_NPROCESSORS_ONLN);
    }

#elif defined(HAVE_MPCTL)

/* HP-UX mpctl */

int
peak_get_ncpus()
    {
    return mpctl(MPC_GETNUMSPUS, NULL, NULL);
    }

#endif


/* peak_set_fdlimit */

#undef RLIMIT_FD_MAX
#if defined(RLIMIT_NOFILE)
# define RLIMIT_FD_MAX          RLIMIT_NOFILE
#elif defined(RLIMIT_FDMAX)
# define RLIMIT_FD_MAX          RLIMIT_FDMAX
#elif defined(RLIMIT_OFILE)
# define RLIMIT_FD_MAX          RLIMIT_OFILE
#elif defined(RLIMIT_OPEN_MAX)
# define RLIMIT_FD_MAX          RLIMIT_OPEN_MAX
#endif

#if defined(HAVE_SETRLIMIT) && defined(HAVE_SETRLIMIT) && defined(RLIMIT_FD_MAX)
#define PEAK_CAN_SETRLIMIT
#else
#warning PEAK cannot set resource limit (setrlimit)
#endif

int
peak_set_fdlimit(int maxfiles)
    {
#ifdef PEAK_CAN_SETRLIMIT
    struct rlimit limit;
    
    if (!getrlimit(RLIMIT_FD_MAX, &limit))
        {
        if (limit.rlim_max < maxfiles)
            return limit.rlim_max;
        limit.rlim_cur = limit.rlim_max;        /* make soft limit the max */
        setrlimit(RLIMIT_FD_MAX, &limit);
        }
#endif
    return maxfiles;
    }
