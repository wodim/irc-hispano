dnl @(#)$Id: with-com_err.m4,v 1.3 2006/09/04 15:12:26 spale Exp $
dnl
dnl KLM_WITH_COM_ERR
dnl
dnl Adds --with-com_err, which tells it where to look for com_err

AC_DEFUN([KLM_WITH_COM_ERR],
[klm_path_com_err=no

AC_ARG_WITH([com_err],
[  --with-com_err=dir      Use the com_err installed in dir (/usr/local)],
[if test "x${withval}" = "xyes"; then
    klm_path_com_err=
else
    klm_path_com_err="${withval}"
fi
])

if test "x${klm_path_com_err}" != "xno"; then
    AC_CACHE_CHECK([com_err install location], klm_cv_path_com_err,
    [if test "x${klm_path_com_err}" = x; then
	klm_cv_path_com_err=/usr/local
    else
	klm_cv_path_com_err="${klm_path_com_err}"
    fi
    ])

    COM_ERR=true
else
    klm_cv_path_com_err=
    COM_ERR=false
fi
AC_SUBST([COM_ERR])
])
