dnl Prefered emacs editing mode: -*- Autoconf -*-
dnl 
dnl Process this file with autoconf to produce a configure script.
dnl 
dnl Copyright (c) 2003, 2004, 2005, 2006, 2007
dnl      Stephane Thiell <mbuna@bugged.org>. All rights reserved.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
dnl "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
dnl TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
dnl PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
dnl CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
dnl EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
dnl PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
dnl OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
dnl WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
dnl OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
dnl ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
dnl
dnl $Id: configure.ac,v 1.14 2007/05/24 13:26:43 mbuna Exp $

AC_INIT([PEAK Library], 0.1.2, [mbuna@bugged.org], [libpeak])
AC_CONFIG_SRCDIR([peak/peak.h])
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE

dnl Assume GCC compatibility.
cflags='-O2 -g -Wall'
default_includes=' -I. -I$(srcdir) -I$(top_builddir)'
includes='-I- -I$(top_builddir)'

case "$host" in
    *-*-aix*)
	if test -z "$CC"; then
	    AC_CHECK_PROGS(CC, [cc_r xlc_r gcc cc])
	fi
	if test "$CC" = "cc_r" -o "$CC" = "xlc_r"; then
	    cflags='-O3 -qthreaded -qwarn64'
	    default_includes='-I$(top_builddir)'
	    includes=
	elif test "$CC" = gcc; then
	    AC_MSG_WARN([GCC not recommended on AIX])
	else
	    AC_MSG_WARN([cc_r on AIX not found; threads wont work!])
	    default_includes='-I$(top_builddir)'
	    includes=
	fi
	;;

    *-*-netbsd*)
        includes='-I- -I/usr/pkg/include -I$(top_builddir)'
        PTHREAD_CFLAGS='-I/usr/pkg/include'
        PTHREAD_LIBS='-L/usr/pkg/lib -lpthread'
        ;;

    powerpc-*-darwin*)
	dnl We support the PowerPC IBM compiler on Darwin.
	if test -z "$CC"; then
	    AC_CHECK_PROGS(CC, [gcc cc_r cc])
	fi
	if test "$CC" = "cc_r" -o "$CC" = "xlc_r"; then
	    cflags='-O3 -qthreaded -qtune=auto'
	    default_includes='-I$(top_builddir)'
	    includes='-I$(ARCH_INCLUDES)'
	fi
	;;

    *)
        ;;
esac
CFLAGS=$cflags
AC_SUBST(default_includes)
AC_SUBST(includes)

dnl Check for asm includes
AC_PROG_LN_S
AC_MSG_CHECKING(for architecture-dependent code)
arch=unknown
case "$host" in
    powerpc-*)
        arch=powerpc
        ;;

    i*86-* | x86-* | x86_64-*)
        arch=x86
        ;;

    m68k-*)
        arch=m68k
        ;;

    sparc-*)
        arch=sparc
        ;;

    ia64-*)
        arch=ia64
        ;;

    *)
	AC_MSG_RESULT([failed!])
        AC_MSG_ERROR([libpeak is not available for your architecture yet,
                  please submit a feature request for:
                  ${host}])
        ;;
esac
AC_CONFIG_LINKS([peak/atomic.h:peak/arch/${arch}/atomic.h
		 peak/spinlock.h:peak/arch/${arch}/spinlock.h])
AC_MSG_RESULT([peak/arch/${arch} will be used.])

AC_SUBST_FILE([atomic_header])
atomic_header="peak/atomic.h"

dnl Initialize libtool
AM_PROG_LIBTOOL

dnl Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
ACX_PTHREAD
LIBS="$PTHREAD_CFLAGS $PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CC="$PTHREAD_CC"

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([pthread.h sched.h mach/mach_init.h mach/mach.h])
AC_CHECK_HEADERS([mach/semaphore.h mach/task.h signal.h sys/signal.h])
AC_CHECK_HEADERS([machine/hal_sysinfo.h sys/atomic_op.h])
AC_CHECK_HEADERS([poll.h sys/poll.h sys/devpoll.h sys/epoll.h sys/event.h])
AC_CHECK_HEADERS([sys/mpctl.h sys/sysctl.h sys/sysinfo.h])
AC_CREATE_STDINT_H([peak/stdint.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
ACX_STRUCT_TZHEAD
ACX_FD_SETSIZE

dnl Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_ALLOCA
AC_FUNC_SELECT_ARGTYPES

dnl Do all the checks necessary to figure out -lnsl / -lsocket stuff
AC_LIBRARY_NET

AC_CHECK_FUNCS([gettimeofday memset strerror])
AC_CHECK_FUNCS([getpagesize getsysinfo mpctl sysconf sysctl])
AC_CHECK_FUNCS([epoll_create poll kqueue getrlimit getrusage setrlimit])
AC_CHECK_FUNCS([semaphore_create semaphore_signal])
AC_CHECK_FUNCS([_spin_lock _spin_lock_try _spin_unlock])
AC_SEARCH_LIBS([sched_yield], [rt posix4])

dnl Now choose the engine
AC_MSG_CHECKING([for event engine module type])
AC_ARG_WITH([engine],
    [  --with-engine=ENGINE         Force event engine module type [[automatic]]],
    [peak_engine=$with_engine])
if test -z "$peak_engine" -o x"$peak_engine" = xno; then
    if test x"$ac_cv_header_sys_event_h" = xyes -a x"$ac_cv_func_kqueue" = xyes; then
        peak_engine=kqueue
    elif test x"$ac_cv_header_sys_epoll_h" = xyes; then
       peak_engine=epoll
    elif test x"$ac_cv_header_sys_devpoll_h" = xyes; then
        case "$host" in
            *-*-linux*) dnl Do not use devpoll on Linux.
                peak_engine=poll
                ;;
            *)
                peak_engine=devpoll
                ;;
        esac
    elif test x"$ac_cv_header_poll_h" = xyes; then
        peak_engine=poll
    else
        peak_engine=select
    fi
fi
case "$peak_engine" in
    devpoll)
        AC_LIBOBJ([engine_mod_devpoll])
	AC_DEFINE_UNQUOTED(PEAK_ENGINE_DEVPOLL, 1, [Use engine devpoll.])
        ;;
    epoll)
        AC_LIBOBJ([engine_mod_epoll])
	AC_DEFINE_UNQUOTED(PEAK_ENGINE_EPOLL, 1, [Use engine epoll.])
        ;;
    kqueue)
        AC_LIBOBJ([engine_mod_kqueue])
	AC_DEFINE_UNQUOTED(PEAK_ENGINE_KQUEUE, 1, [Use engine kqueue.])
        ;;
    poll)
        AC_LIBOBJ([engine_mod_poll])
	AC_DEFINE_UNQUOTED(PEAK_ENGINE_POLL, 1, [Use engine poll.])
        ;;
    select)
	AC_DEFINE_UNQUOTED(PEAK_ENGINE_SELECT, 1, [Use engine select.])
        AC_LIBOBJ([engine_mod_select])
        ;;
    *)
        AC_MSG_ERROR([Unable to find requested engine module type])
        ;;
esac
AC_MSG_RESULT([$peak_engine])


dnl Now check for --enable-profile
AC_MSG_CHECKING([whether to enable profiling support (gprof)])
AC_ARG_ENABLE([profile],
[  --enable-profile        Enable profiling support (add -pg to CFLAGS)],
[peak_cv_enable_profile=$enable_profile],
[AC_CACHE_VAL(peak_cv_enable_profile,
[peak_cv_enable_profile=no])])
AC_MSG_RESULT([$peak_cv_enable_profile])

if test x"$peak_cv_enable_profile" = xyes; then
    CFLAGS="-pg $CFLAGS"
fi


dnl Now check for --enable-pedantic
AC_MSG_CHECKING([whether to enable pedantic compiler warnings])
AC_ARG_ENABLE([pedantic],
[  --enable-pedantic       Enable pedantic warnings (add -pedantic to CFLAGS)],
[peak_cv_enable_pedantic=$enable_pedantic],
[AC_CACHE_VAL(peak_cv_enable_pedantic,
[peak_cv_enable_pedantic=no])])
AC_MSG_RESULT([$peak_cv_enable_pedantic])

if test x"$peak_cv_enable_pedantic" = xyes; then
    CFLAGS="-pedantic $CFLAGS"
fi

dnl Now check for --enable-uniprocessor-only
AC_MSG_CHECKING([whether to enable uniprocessor-only code])
AC_ARG_ENABLE([uniprocessor-only],
[  --enable-uniprocessor-only Enable uniprocessor-only code optimization
                             Warning: library binary no more SMP safe
                             [[default=no]]],
[peak_cv_enable_uniprocessor_only=$enable_uniprocessor_only],
[AC_CACHE_VAL(peak_cv_enable_uniprocessor_only,
[peak_cv_enable_uniprocessor_only=no])])
AC_MSG_RESULT([$peak_cv_enable_uniprocessor_only])

if test x"$peak_cv_enable_uniprocessor_only" = xyes; then
    AC_DEFINE_UNQUOTED(PEAK_CONFIG_UNIPROCESSOR, 1, [Enable uniprocessor-only code.])
fi


dnl --with-default-maxfds
peak_default_maxfds=256
AC_MSG_CHECKING([for default maxfds flavor])
AC_ARG_WITH([default-maxfds],
[  --with-default-maxfds=MAXFDS Default MAXFDS flavor number [[default=256]]],
[peak_cv_with_default_maxfds=$with_default_maxfds],
[AC_CACHE_VAL(peak_cv_with_default_maxfds,
[peak_cv_with_default_maxfds=$peak_default_maxfds])])

if test x"$peak_cv_with_default_maxfds" = xyes -o x"$peak_cv_with_default_maxfds" = xno; then
    peak_cv_with_default_maxfds=$peak_default_maxfds
fi
AC_MSG_RESULT([$peak_cv_with_default_maxfds])
AC_DEFINE_UNQUOTED(PEAK_DEFAULT_FLAVOR_MAXFDS, $peak_cv_with_default_maxfds,
[Default maxfds flavor number])


if test $peak_engine = "select"; then

dnl --with-select-maxfds allows us to set the number of fds for systems that support
dnl                      it and with the select engine only
AC_MSG_CHECKING([for max fds for select])
AC_ARG_WITH([select-maxfds],
    [  --with-select-maxfds=MAXFDS  Maximum number of fds the library can handle
                          (make sense for select engine only)
                          [[default=FD_SETSIZE]]],
    [select_maxfds=$with_select_maxfds])

if test -n "$select_maxfds"; then
    AC_MSG_RESULT([$select_maxfds])
    AC_DEFINE_UNQUOTED(PEAK_FD_SETSIZE, $peak_cv_with_select_maxfds,
        [Maximum number of fds the library can handle with select])
else
    AC_MSG_RESULT([n/a])
fi

fi


dnl We need another host scan for atomic.h configuration
ATOMIC_HEADERS=
case "$host" in
    *-*-aix*)
        if test x"$ac_cv_header_atomic_op_h" = xyes; then
            ATOMIC_HEADERS=[#include <sys/atomic_op.h> /* AIX */]
        fi
	;;
    i*86-* | x86-*)
        if test x"$peak_cv_enable_uniprocessor_only" = xyes; then
            ATOMIC_HEADERS=[#define PEAK_CONFIG_UNIPROCESSOR]
        fi
        ;;
    *)
        ;;
esac
AC_SUBST(ATOMIC_HEADERS)


PEAK_CFLAGS=
PEAK_LIBS=-lpeak
PEAK_LIBDEPS=$LIBS
PEAK_VERSION=$PACKAGE_VERSION

AC_CONFIG_LIBCONFIG_IN([peak])

AC_CONFIG_FILES([Makefile peak-config peak/Makefile tests/Makefile
                 peak/arch/ia64/atomic.h
                 peak/arch/ia64/spinlock.h
                 peak/arch/m68k/atomic.h
                 peak/arch/m68k/spinlock.h
                 peak/arch/powerpc/atomic.h
                 peak/arch/powerpc/spinlock.h
                 peak/arch/sparc/atomic.h
                 peak/arch/sparc/spinlock.h
                 peak/arch/x86/atomic.h
                 peak/arch/x86/spinlock.h])

AC_OUTPUT

eval e_bindir=${bindir}; eval e_bindir=${e_bindir}
eval e_includedir=${includedir}; eval e_includedir=${e_includedir}
eval e_libdir=${libdir}; eval e_libdir=${e_libdir}
AC_MSG_RESULT([
PEAK library compile-time configuration done.

Installation base directory:      ${prefix}
Installation binary directory:    ${e_bindir}
Installation include directory:   ${e_includedir}
Installation library directory:   ${e_libdir}
Architecture:                     ${arch}
Engine module:                    ${peak_engine}

Type 'make' to compile the library
then 'make check' to compile and run the test suite (optional)
then 'make install' to install libpeak.])

