dnl                                               -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

AC_INIT([Proxy scanner daemon], 2.1, [mbuna@bugged.org], [pxscand]) 
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/pxyscand.cc])
AM_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_MAKE_SET
AC_DEFINE_UNQUOTED(DEBUG, 1, [Build debugging support])

dnl Checks for libraries.
AC_PATH_GENERIC(peak, 0.1.0,, AC_ERROR([required PEAK Library (libpeak) not found]))
XIPH_VAR_APPEND([CXXFLAGS], [$PEAK_CFLAGS])
XIPH_VAR_APPEND([CFLAGS], [$PEAK_CFLAGS])
XIPH_VAR_PREPEND([LIBS], [$PEAK_LIBS])

AC_PATH_GENERIC(xml2, 2.0.0,, AC_ERROR([required XML Library (libxml2) not found - See http://xmlsoft.org]))
XIPH_VAR_APPEND([CXXFLAGS], [$XML2_CFLAGS])
XIPH_VAR_PREPEND([LIBS], [$XML2_LIBS])

XIPH_VAR_APPEND([CXXFLAGS], [-I../../libopas])
XIPH_VAR_APPEND([CFLAGS], [-I../../libopas])

XIPH_VAR_APPEND([CXXFLAGS], [-I../../ipcache])
XIPH_VAR_APPEND([CFLAGS], [-I../../ipcache])

dnl Checks for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([signal.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

dnl Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([gethostbyname gettimeofday memset select socket strerror])
AC_CHECK_FUNCS([strerror_r])

AC_CONFIG_FILES([Makefile src/Makefile px/Makefile])
AC_OUTPUT

