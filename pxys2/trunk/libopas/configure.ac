dnl                                               -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

AC_INIT([OPAS Library], 1.0.1, [mbuna@bugged.org], [opas])
AC_CONFIG_SRCDIR([opas/opas.c])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_DEFINE_UNQUOTED(OPAS_MAJOR, 1, [Major version])
AC_DEFINE_UNQUOTED(OPAS_MINOR, 0, [Minor version])

dnl Checks for programs.
AC_DISABLE_SHARED
AM_PROG_LIBTOOL

dnl Checks for libraries.

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([netinet/in.h])
AC_CREATE_STDINT_H([opas/opas_stdint.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

dnl Checks for library functions.
AC_FUNC_ALLOCA

AC_CONFIG_FILES([Makefile opas/Makefile])
AC_OUTPUT
