dnl                                               -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

AC_INIT([Undernet IRCD proxy scanner],[2.0],[mbuna@bugged.org],[pxyservd]) 
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/pxyservd.c])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_SUBDIRS([libbcl])
AM_MAINTAINER_MODE

dnl Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_CACHE_SAVE

dnl Checks for libraries.
AC_PATH_GENERIC(dbprim, 0.9.1, [
XIPH_VAR_APPEND([CFLAGS], [$DBPRIM_CFLAGS])
XIPH_VAR_APPEND([CFLAGS], [$DBPRIM_CFLAGS/dbprim])
XIPH_VAR_PREPEND([LIBS], [$DBPRIM_LIBS])
], [
dbprim_ldadd="../dbprim/.libs/libdbprim.a"
AC_SUBST(dbprim_ldadd)
AC_CONFIG_SUBDIRS([dbprim])
XIPH_VAR_APPEND([CFLAGS], [-I..])
XIPH_VAR_APPEND([CFLAGS], [-I../dbprim])
AC_MSG_NOTICE([Will use dbprim provided in this package (static linkage)])
])

AC_PATH_GENERIC(peak, 0.1.0,, AC_ERROR([required PEAK Library (libpeak) not found]))
XIPH_VAR_APPEND([CFLAGS], [$PEAK_CFLAGS])
XIPH_VAR_PREPEND([LIBS], [$PEAK_LIBS])

AC_PATH_GENERIC(xml2, 2.0.0,, AC_ERROR([required XML Library (libxml2) not found - See http://xmlsoft.org]))
XIPH_VAR_APPEND([CFLAGS], [$XML2_CFLAGS])
XIPH_VAR_PREPEND([LIBS], [$XML2_LIBS])

XIPH_VAR_APPEND([CFLAGS], [-I../../ipcache])
XIPH_VAR_APPEND([CFLAGS], [-I../../libopas])

dnl Checks for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([signal.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

dnl Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_CHECK_FUNCS([gethostbyname gettimeofday memset select socket strerror])
AC_CHECK_FUNCS([strerror_r])

dnl The --enable-debug option
AC_MSG_CHECKING([whether to enable debug mode])
AC_ARG_ENABLE([debug],
[  --enable-debug          turn on debugging mode],
[pxyservd_cv_enable_debug=$enable_debug],
[AC_CACHE_VAL(pxyservd_cv_enable_debug,
[pxyservd_cv_enable_debug=no])])
AC_MSG_RESULT([$pxyservd_cv_enable_debug])
if test x"$pxyservd_cv_enable_debug" = xyes; then
    AC_DEFINE([DEBUG], , [Enable debugging code])
fi

dnl The --with-maxnicklen: maximum nick length supported
pxyservd_maxnicklen=15
AC_MSG_CHECKING([maximum nick length])
AC_ARG_WITH([maxnicklen],
[  --with-maxnicklen=NUMBER  maximum nick length of network clients [[default=15]]],
[pxyservd_cv_with_maxnicklen=$with_maxnicklen],
[AC_CACHE_VAL(pxyservd_cv_with_maxnicklen,
[pxyservd_cv_with_maxnicklen=$pxyservd_maxnicklen])])
if test x"$pxyservd_cv_with_maxnicklen" = xyes -o "$pxyservd_cv_with_maxnicklen" = xno; then
    pxyservd_cv_with_maxnicklen=$pxyservd_maxnicklen
fi
AC_MSG_RESULT([$pxyservd_cv_with_maxnicklen])
AC_DEFINE_UNQUOTED(NICKLEN, $pxyservd_cv_with_maxnicklen,
    [Maximum nick length of network clients])

AC_DEFINE([IRC_HISPANO], 1, [Red IRC de IRC-Hispano])
AC_DEFINE([SPANISH], 1, [Mensajes en castellano])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

