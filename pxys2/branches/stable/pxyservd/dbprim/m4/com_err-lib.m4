dnl @(#)$Id: com_err-lib.m4,v 1.3 2006/09/04 15:12:26 spale Exp $
dnl
dnl KLM_COM_ERR_LIB
dnl
dnl Checks to see if the com_err libraries are locatable

AC_DEFUN([KLM_COM_ERR_LIB],
[AC_REQUIRE([KLM_WITH_COM_ERR])
if test "x$COM_ERR" = "xtrue"; then
    t_LIBS=${LIBS}
    LIBS="${LIBS} -L${klm_cv_path_com_err}/lib"

    AC_CHECK_LIB([com_err], [com_err], ,
	[AC_MSG_ERROR([Unable to find com_err libraries])])

    LIBS=${t_LIBS}
    COM_ERR_LIBPATH="${klm_cv_path_com_err}/lib"
    COM_ERR_LDADD="-L${COM_ERR_LIBPATH} -lcom_err"
else
    COM_ERR_LIBPATH=
    COM_ERR_LDADD=
fi
AC_SUBST([COM_ERR_LIBPATH])
AC_SUBST([COM_ERR_LDADD])])
