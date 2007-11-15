dnl @(#)$Id: com_err-obj.m4,v 1.3 2006/09/04 15:12:26 spale Exp $
dnl
dnl KLM_COM_ERR_OBJ
dnl
dnl Defines the given OBJ if com_err is enabled.

AC_DEFUN([KLM_COM_ERR_OBJ],
[AC_REQUIRE([KLM_WITH_COM_ERR])
if test "x$COM_ERR" = "xtrue"; then
    $1="$2"
else
    $1=
fi
AC_SUBST($1)
])
