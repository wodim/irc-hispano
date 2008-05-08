dnl ACX_STRUCT_TZHEAD
dnl Written by Stephane Thiell <mbuna@bugged.org>
dnl 
AC_DEFUN([ACX_STRUCT_TZHEAD],
[AC_CHECK_HEADERS([tzfile.h])
AC_MSG_CHECKING([whether struct tzhead is defined in tzfile.h])
AC_CACHE_VAL(acx_cv_struct_tzhead,
cat > conftest.$ac_ext <<EOF
[
#include "confdefs.h"
#ifdef HAVE_TZFILE_H
#include <tzfile.h>
#endif

int main()
{
  int size = sizeof(struct tzhead);
  exit(0);
}]
EOF
if AC_TRY_EVAL(ac_compile); then
  acx_cv_struct_tzhead=yes
else
  acx_cv_struct_tzhead=no
fi
rm -fr conftest*)
AC_MSG_RESULT([$acx_cv_struct_tzhead])
if test x$acx_cv_struct_tzhead = xyes; then
  AC_DEFINE(HAVE_STRUCT_TZHEAD, 1, [struct tzhead is defined in tzfile.h])
fi])dnl ACX_STRUCT_TZHEAD


dnl ACX_FD_SETSIZE
dnl Written by Stephane Thiell <mbuna@bugged.org>
dnl 
AC_DEFUN([ACX_FD_SETSIZE],
[AC_CHECK_FUNCS(select)
AC_MSG_CHECKING(whether we can redefine FD_SETSIZE)
AC_CACHE_VAL(acx_cv_fd_setsize,
ss_status=0
cat > conftest.$ac_ext <<EOF
[
#define FD_SETSIZE 1234
#include "confdefs.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>

int main()
{
  int status = 0;
  int val = FD_SETSIZE;
  
  if (val == 1234) status = 1;
  else status = 2;
  
  exit(status);
}]
EOF
if AC_TRY_EVAL(ac_link) && test -s conftest${ac_exeext} 2>/dev/null; then
  (./conftest; exit; ) 2>/dev/null
  ss_status=$?
  if test x$ss_status = x1; then
    acx_cv_fd_setsize=yes
  else
    acx_cv_fd_setsize=no
  fi
else
  acx_cv_fd_setsize=no
fi
rm -fr conftest*)
AC_MSG_RESULT([$acx_cv_fd_setsize])
if test x$acx_cv_fd_setsize = xyes; then
  AC_DEFINE(PEAK_FD_SETSIZE_REDEFINABLE, 1, [The system accepts to redefine FD_SETSIZE])
fi])dnl ACX_FD_SETSIZE


dnl ACX_PTHREAD
dnl Written by Steven G. Johnson <stevenj@alum.mit.edu>
dnl            Alejandro Forero Cuervo <bachue@bachue.com>
dnl 
AC_DEFUN([ACX_PTHREAD], [
AC_REQUIRE([AC_CANONICAL_HOST])
AC_LANG_SAVE
AC_LANG_C
acx_pthread_ok=no

# We used to check for pthread.h first, but this fails if pthread.h
# requires special compiler flags (e.g. on True64 or Sequent).
# It gets checked for in the link test anyway.

# First of all, check if the user has set any of the PTHREAD_LIBS,
# etcetera environment variables, and if threads linking works using
# them:
if test x"$PTHREAD_LIBS$PTHREAD_CFLAGS" != x; then
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        AC_MSG_CHECKING([for pthread_join in LIBS=$PTHREAD_LIBS with CFLAGS=$PTHREAD_CFLAGS])
        AC_TRY_LINK_FUNC(pthread_join, acx_pthread_ok=yes)
        AC_MSG_RESULT($acx_pthread_ok)
        if test x"$acx_pthread_ok" = xno; then
                PTHREAD_LIBS=""
                PTHREAD_CFLAGS=""
        fi
        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"
fi

# We must check for the threads library under a number of different
# names; the ordering is very important because some systems
# (e.g. DEC) have both -lpthread and -lpthreads, where one of the
# libraries is broken (non-POSIX).

# Create a list of thread flags to try.  Items starting with a "-" are
# C compiler flags, and other items are library names, except for "none"
# which indicates that we try without any flags at all.

acx_pthread_flags="pthreads none -Kthread -kthread lthread -pthread -pthreads -mthreads pthread --thread-safe -mt"

# The ordering *is* (sometimes) important.  Some notes on the
# individual items follow:

# pthreads: AIX (must check this before -lpthread)
# none: in case threads are in libc; should be tried before -Kthread and
#       other compiler flags to prevent continual compiler warnings
# -Kthread: Sequent (threads in libc, but -Kthread needed for pthread.h)
# -kthread: FreeBSD kernel threads (preferred to -pthread since SMP-able)
# lthread: LinuxThreads port on FreeBSD (also preferred to -pthread)
# -pthread: Linux/gcc (kernel threads), BSD/gcc (userland threads)
# -pthreads: Solaris/gcc
# -mthreads: Mingw32/gcc, Lynx/gcc
# -mt: Sun Workshop C (may only link SunOS threads [-lthread], but it
#      doesn't hurt to check since this sometimes defines pthreads too;
#      also defines -D_REENTRANT)
# pthread: Linux, etcetera
# --thread-safe: KAI C++

case "${host_cpu}-${host_os}" in
        *solaris*)

        # On Solaris (at least, for some versions), libc contains stubbed
        # (non-functional) versions of the pthreads routines, so link-based
        # tests will erroneously succeed.  (We need to link with -pthread or
        # -lpthread.)  (The stubs are missing pthread_cleanup_push, or rather
        # a function called by this macro, so we could check for that, but
        # who knows whether they'll stub that too in a future libc.)  So,
        # we'll just look for -pthreads and -lpthread first:

        acx_pthread_flags="-pthread -pthreads pthread -mt $acx_pthread_flags"
        ;;
esac

if test x"$acx_pthread_ok" = xno; then
for flag in $acx_pthread_flags; do

        case $flag in
                none)
                AC_MSG_CHECKING([whether pthreads work without any flags])
                ;;

                -*)
                AC_MSG_CHECKING([whether pthreads work with $flag])
                PTHREAD_CFLAGS="$flag"
                ;;

                *)
                AC_MSG_CHECKING([for the pthreads library -l$flag])
                PTHREAD_LIBS="-l$flag"
                ;;
        esac

        save_LIBS="$LIBS"
        save_CFLAGS="$CFLAGS"
        LIBS="$PTHREAD_LIBS $LIBS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Check for various functions.  We must include pthread.h,
        # since some functions may be macros.  (On the Sequent, we
        # need a special flag -Kthread to make this header compile.)
        # We check for pthread_join because it is in -lpthread on IRIX
        # while pthread_create is in libc.  We check for pthread_attr_init
        # due to DEC craziness with -lpthreads.  We check for
        # pthread_cleanup_push because it is one of the few pthread
        # functions on Solaris that doesn't have a non-functional libc stub.
        # We try pthread_create on general principles.
        AC_TRY_LINK([#include <pthread.h>],
                    [pthread_t th; pthread_join(th, 0);
                     pthread_attr_init(0); pthread_cleanup_push(0, 0);
                     pthread_create(0,0,0,0); pthread_cleanup_pop(0); ],
                    [acx_pthread_ok=yes])

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        AC_MSG_RESULT($acx_pthread_ok)
        if test "x$acx_pthread_ok" = xyes; then
                break;
        fi

        PTHREAD_LIBS=""
        PTHREAD_CFLAGS=""
done
fi

# Various other checks:
if test "x$acx_pthread_ok" = xyes; then
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Detect AIX lossage: threads are created detached by default
        # and the JOINABLE attribute has a nonstandard name (UNDETACHED).
        AC_MSG_CHECKING([for joinable pthread attribute])
        AC_TRY_LINK([#include <pthread.h>],
                    [int attr=PTHREAD_CREATE_JOINABLE;],
                    ok=PTHREAD_CREATE_JOINABLE, ok=unknown)
        if test x"$ok" = xunknown; then
                AC_TRY_LINK([#include <pthread.h>],
                            [int attr=PTHREAD_CREATE_UNDETACHED;],
                            ok=PTHREAD_CREATE_UNDETACHED, ok=unknown)
        fi
        if test x"$ok" != xPTHREAD_CREATE_JOINABLE; then
                AC_DEFINE(PTHREAD_CREATE_JOINABLE, $ok,
                          [Define to the necessary symbol if this constant
                           uses a non-standard name on your system.])
        fi
        AC_MSG_RESULT(${ok})
        if test x"$ok" = xunknown; then
                AC_MSG_WARN([we do not know how to create joinable pthreads])
        fi

        AC_MSG_CHECKING([if more special flags are required for pthreads])
        flag=no
        case "${host_cpu}-${host_os}" in
                *-aix* | *-freebsd*)     flag="-D_THREAD_SAFE";;
                *solaris* | *-osf* | *-hpux*) flag="-D_REENTRANT";;
        esac
        AC_MSG_RESULT(${flag})
        if test "x$flag" != xno; then
                PTHREAD_CFLAGS="$flag $PTHREAD_CFLAGS"
        fi

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"
fi
dnl Removed AIX cc_r check as we already perform it in configure.ac --mbuna
PTHREAD_CC="$CC"

AC_SUBST(PTHREAD_LIBS)
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_CC)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_pthread_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_PTHREAD,1,[Define if you have POSIX threads libraries and header files.]),[$1])
        :
else
        acx_pthread_ok=no
        $2
fi
AC_LANG_RESTORE
])dnl ACX_PTHREAD



dnl AC_COMPILE_CHECK_SIZEOF
dnl Written by Kaveh Ghazi <ghazi@caip.rutgers.edu>
dnl 
AC_DEFUN([AC_COMPILE_CHECK_SIZEOF],
[changequote(<<, >>)dnl
dnl The name to #define.
define(<<AC_TYPE_NAME>>, translit(sizeof_$1, [a-z *], [A-Z_P]))dnl
dnl The cache variable name.
define(<<AC_CV_NAME>>, translit(ac_cv_sizeof_$1, [ *], [_p]))dnl
changequote([, ])dnl
AC_MSG_CHECKING(size of $1)
AC_CACHE_VAL(AC_CV_NAME,
[for ac_size in 4 8 1 2 16 $2 ; do # List sizes in rough order of prevalence.
  AC_TRY_COMPILE([#include "confdefs.h"
#include <sys/types.h>
$2
], [switch (0) case 0: case (sizeof ($1) == $ac_size):;], AC_CV_NAME=$ac_size)
  if test x$AC_CV_NAME != x ; then break; fi
done
])
if test x$AC_CV_NAME = x ; then
  AC_MSG_ERROR([cannot determine a size for $1])
fi
AC_MSG_RESULT($AC_CV_NAME)
AC_DEFINE_UNQUOTED(AC_TYPE_NAME, $AC_CV_NAME, [The number of bytes in type $1])
undefine([AC_TYPE_NAME])dnl
undefine([AC_CV_NAME])dnl
])



dnl Written by Guido Draheim <guidod@gmx.de>
dnl 
AC_DEFUN([AC_CREATE_STDINT_H],
[# ------ AC CREATE STDINT H -------------------------------------
AC_MSG_CHECKING([for stdint-types....])
ac_stdint_h=`echo ifelse($1, , _stdint.h, $1)`
if test "$ac_stdint_h" = "stdint.h" ; then
 AC_MSG_RESULT("(are you sure you want them in ./stdint.h?)")
elif test "$ac_stdint_h" = "inttypes.h" ; then
 AC_MSG_RESULT("(are you sure you want them in ./inttypes.h?)")
else
 AC_MSG_RESULT("(putting them into $ac_stdint_h)")
fi

inttype_headers=`echo inttypes.h sys/inttypes.h sys/inttypes.h $2 \
| sed -e 's/,/ /g'`

 ac_cv_header_stdint_x="no-file"
 ac_cv_header_stdint_o="no-file"
 ac_cv_header_stdint_u="no-file"
 for i in stdint.h $inttype_headers ; do
   unset ac_cv_type_uintptr_t
   unset ac_cv_type_uint64_t
   _AC_CHECK_TYPE_NEW(uintptr_t,[ac_cv_header_stdint_x=$i],dnl
     continue,[#include <$i>])
   AC_CHECK_TYPE(uint64_t,[and64="(uint64_t too)"],[and64=""],[#include<$i>])
   AC_MSG_RESULT(... seen our uintptr_t in $i $and64)
   break;
 done
 if test "$ac_cv_header_stdint_x" = "no-file" ; then
 for i in stdint.h $inttype_headers ; do
   unset ac_cv_type_uint32_t
   unset ac_cv_type_uint64_t
   AC_CHECK_TYPE(uint32_t,[ac_cv_header_stdint_o=$i],dnl
     continue,[#include <$i>])
   AC_CHECK_TYPE(uint64_t,[and64="(uint64_t too)"],[and64=""],[#include<$i>])
   AC_MSG_RESULT(... seen our uint32_t in $i $and64)
   break;
 done
 if test "$ac_cv_header_stdint_o" = "no-file" ; then
 for i in sys/types.h $inttype_headers ; do
   unset ac_cv_type_u_int32_t
   unset ac_cv_type_u_int64_t
   AC_CHECK_TYPE(u_int32_t,[ac_cv_header_stdint_u=$i],dnl
     continue,[#include <$i>])
   AC_CHECK_TYPE(uint64_t,[and64="(u_int64_t too)"],[and64=""],[#include<$i>])
   AC_MSG_RESULT(... seen our u_int32_t in $i $and64)
   break;
 done
 fi
 fi

# ----------------- DONE inttypes.h checks MAYBE C basic types --------

if test "$ac_cv_header_stdint_x" = "no-file" ; then
   AC_COMPILE_CHECK_SIZEOF(char)
   AC_COMPILE_CHECK_SIZEOF(short)
   AC_COMPILE_CHECK_SIZEOF(int)
   AC_COMPILE_CHECK_SIZEOF(long)
   AC_COMPILE_CHECK_SIZEOF(void*)
   ac_cv_header_stdint_test="yes"
else
   ac_cv_header_stdint_test="no"
fi

# ----------------- DONE inttypes.h checks START header -------------
_ac_stdint_h=AS_TR_CPP(_$ac_stdint_h)
AC_MSG_RESULT(creating $ac_stdint_h : $_ac_stdint_h)
echo "#ifndef" $_ac_stdint_h >$ac_stdint_h
echo "#define" $_ac_stdint_h "1" >>$ac_stdint_h
echo "#ifndef" _GENERATED_STDINT_H >>$ac_stdint_h
echo "#define" _GENERATED_STDINT_H '"'$PACKAGE $VERSION'"' >>$ac_stdint_h
if test "$GCC" = "yes" ; then
  echo "/* generated using a gnu compiler version" `$CC --version` "*/" \
  >>$ac_stdint_h
else
  echo "/* generated using $CC */" >>$ac_stdint_h
fi
echo "" >>$ac_stdint_h

if test "$ac_cv_header_stdint_x" != "no-file" ; then
   ac_cv_header_stdint="$ac_cv_header_stdint_x"
elif  test "$ac_cv_header_stdint_o" != "no-file" ; then
   ac_cv_header_stdint="$ac_cv_header_stdint_o"
elif  test "$ac_cv_header_stdint_u" != "no-file" ; then
   ac_cv_header_stdint="$ac_cv_header_stdint_u"
else
   ac_cv_header_stdint="stddef.h"
fi

# ----------------- See if int_least and int_fast types are present
unset ac_cv_type_int_least32_t
unset ac_cv_type_int_fast32_t
AC_CHECK_TYPE(int_least32_t,,,[#include <$ac_cv_header_stdint>])
AC_CHECK_TYPE(int_fast32_t,,,[#include<$ac_cv_header_stdint>])

if test "$ac_cv_header_stdint" != "stddef.h" ; then
if test "$ac_cv_header_stdint" != "stdint.h" ; then
AC_MSG_RESULT(..adding include stddef.h)
   echo "#include <stddef.h>" >>$ac_stdint_h
fi ; fi
AC_MSG_RESULT(..adding include $ac_cv_header_stdint)
   echo "#include <$ac_cv_header_stdint>" >>$ac_stdint_h
echo "" >>$ac_stdint_h

# ----------------- DONE header START basic int types -------------
if test "$ac_cv_header_stdint_x" = "no-file" ; then
   AC_MSG_RESULT(... need to look at C basic types)
dnl ac_cv_header_stdint_test="yes" # moved up before creating the file
else
   AC_MSG_RESULT(... seen good stdint.h inttypes)
dnl ac_cv_header_stdint_test="no"  # moved up before creating the file
fi

if test "$ac_cv_header_stdint_u" != "no-file" ; then
   AC_MSG_RESULT(... seen bsd/sysv typedefs)
   cat >>$ac_stdint_h <<EOF

/* int8_t int16_t int32_t defined by inet code, redeclare the u_intXX types */
typedef u_int8_t uint8_t;
typedef u_int16_t uint16_t;
typedef u_int32_t uint32_t;
EOF
    cat >>$ac_stdint_h <<EOF

/* glibc compatibility */
#ifndef __int8_t_defined
#define __int8_t_defined
#endif
EOF
fi

ac_cv_sizeof_x="$ac_cv_sizeof_char:$ac_cv_sizeof_short"
ac_cv_sizeof_X="$ac_cv_sizeof_x:$ac_cv_sizeof_int"
ac_cv_sizeof_X="$ac_cv_sizeof_X:$ac_cv_sizeof_voidp:$ac_cv_sizeof_long"
if test "$ac_cv_header_stdint" = "stddef.h" ; then
#   we must guess all the basic types. Apart from byte-adressable system,
# there a few 32-bit-only dsp-systems. nibble-addressable systems are way off.
    cat >>$ac_stdint_h <<EOF
/* ------------ BITSPECIFIC INTTYPES SECTION --------------- */
EOF
    t="typedefs for a"
    case "$ac_cv_sizeof_X" in
     1:2:2:2:4) AC_MSG_RESULT(..adding $t normal 16-bit system)
                cat >>$ac_stdint_h <<EOF
/*              a normal 16-bit system                       */
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long   uint32_t;
#ifndef __int8_t_defined
#define __int8_t_defined
typedef          char    int8_t;
typedef          short   int16_t;
typedef          long    int32_t;
#endif
EOF
;;
     1:2:2:4:4) AC_MSG_RESULT(..adding $t 32-bit system derived from a 16-bit)
                cat >>$ac_stdint_h <<EOF
/*              a 32-bit system derived from a 16-bit        */
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
#ifndef __int8_t_defined
#define __int8_t_defined
typedef          char    int8_t;
typedef          short   int16_t;
typedef          int     int32_t;
#endif
EOF
;;
     1:2:4:4:4) AC_MSG_RESULT(..adding $t normal 32-bit system)
                cat >>$ac_stdint_h <<EOF
/*              a normal 32-bit system                       */
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
#ifndef __int8_t_defined
#define __int8_t_defined
typedef          char    int8_t;
typedef          short   int16_t;
typedef          int     int32_t;
#endif
EOF
;;
     1:2:4:4:8) AC_MSG_RESULT(..adding $t 32-bit system prepared for 64-bit)
                cat >>$ac_stdint_h <<EOF

/*              a 32-bit system prepared for 64-bit          */
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
#ifndef __int8_t_defined
#define __int8_t_defined
typedef          char    int8_t;
typedef          short   int16_t;
typedef          int     int32_t;
#endif
EOF
;;
     1:2:4:8:8) AC_MSG_RESULT(..adding $t normal 64-bit system)
                cat >>$ac_stdint_h <<EOF

/*              a normal 64-bit system                       */
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
#ifndef __int8_t_defined
#define __int8_t_defined
typedef          char    int8_t;
typedef          short   int16_t;
typedef          int     int32_t;
#endif
EOF
;;
     1:2:4:8:4) AC_MSG_RESULT(..adding $t 64-bit system derived from a 32-bit)
                cat >>$ac_stdint_h <<EOF

/*              a 64-bit system derived from a 32-bit system */
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
#ifndef __int8_t_defined
#define __int8_t_defined
typedef          char    int8_t;
typedef          short   int16_t;
typedef          int     int32_t;
#endif
EOF
;;
  *)
    AC_MSG_ERROR([ $ac_cv_sizeof_X dnl
 what is that a system? contact the author, quick! http://ac-archive.sf.net])
    exit 1
;;
   esac
fi

# ------------- DONE basic int types START int64_t types ------------
if test "$ac_cv_type_uint64_t" = "yes"
then AC_MSG_RESULT(... seen good uint64_t)
     cat >>$ac_stdint_h <<EOF

/* system headers have good uint64_t */
#ifndef _HAVE_UINT64_T
#define _HAVE_UINT64_T
#endif
EOF

elif test "$ac_cv_type_u_int64_t" = "yes"
then AC_MSG_RESULT(..adding typedef u_int64_t uint64_t)
     cat >>$ac_stdint_h <<EOF

/* system headers have an u_int64_t */
#ifndef _HAVE_UINT64_T
#define _HAVE_UINT64_T
typedef u_int64_t uint64_t;
#endif
EOF
else AC_MSG_RESULT(..adding generic uint64_t runtime checks)
     cat >>$ac_stdint_h <<EOF

/* -------------------- 64 BIT GENERIC SECTION -------------------- */
/* here are some common heuristics using compiler runtime specifics */
#if defined __STDC_VERSION__ && defined __STDC_VERSION__ > 199901L

#ifndef _HAVE_UINT64_T
#define _HAVE_UINT64_T
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

#elif !defined __STRICT_ANSI__
#if defined _MSC_VER || defined __WATCOMC__ || defined __BORLANDC__

#ifndef _HAVE_UINT64_T
#define _HAVE_UINT64_T
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif

#elif defined __GNUC__ || defined __MWERKS__ || defined __ELF__
dnl /* note: all ELF-systems seem to have loff-support which needs 64-bit */

#if !defined _NO_LONGLONG
#ifndef _HAVE_UINT64_T
#define _HAVE_UINT64_T
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif
#endif

#elif defined __alpha || (defined __mips && defined _ABIN32)

#if !defined _NO_LONGLONG
#ifndef _HAVE_UINT64_T
#define _HAVE_UINT64_T
typedef long int64_t;
typedef unsigned long uint64_t;
#endif
#endif
  /* compiler/cpu type ... or just ISO C99 */
#endif
#endif
EOF

# plus a default 64-bit for systems that are likely to be 64-bit ready
  case "$ac_cv_sizeof_x:$ac_cv_sizeof_voidp:$ac_cv_sizeof_long" in
    1:2:8:8) AC_MSG_RESULT(..adding uint64_t default, normal 64-bit system)
cat >>$ac_stdint_h <<EOF
/* DEFAULT: */
/* seen normal 64-bit system, CC has sizeof(long and void*) == 8 bytes */
#ifndef _HAVE_UINT64_T
#define _HAVE_UINT64_T
typedef long int64_t;
typedef unsigned long uint64_t;
#endif
EOF
;;
    1:2:4:8) AC_MSG_RESULT(..adding uint64_t default, typedef to long)
cat >>$ac_stdint_h <<EOF
/* DEFAULT: */
/* seen 32-bit system prepared for 64-bit, CC has sizeof(long) == 8 bytes */
#ifndef _HAVE_UINT64_T
#define _HAVE_UINT64_T
typedef long int64_t;
typedef unsigned long uint64_t;
#endif
EOF
;;
    1:2:8:4) AC_MSG_RESULT(..adding uint64_t default, typedef long long)
cat >>$ac_stdint_h <<EOF
/* DEFAULT: */
/* seen 64-bit derived from a 32-bit, CC has sizeof(long) == 4 bytes */
#ifndef _HAVE_UINT64_T
#define _HAVE_UINT64_T
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif
EOF
;;
   *)
cat >>$ac_stdint_h <<EOF
/* NOTE: */
/* the configure-checks for the basic types did not make us believe */
/* that we could add a fallback to a 'long long' typedef to int64_t */
EOF
  esac
fi

# ------------- DONE int64_t types START intptr types ------------
if test "$ac_cv_header_stdint_x" = "no-file" ; then
  cat >>$ac_stdint_h <<EOF

/* -------------------------- INPTR SECTION --------------------------- */
EOF
  case "$ac_cv_sizeof_x:$ac_cv_sizeof_voidp" in
  1:2:2)
    a="int16_t" ; cat >>$ac_stdint_h <<EOF
/* we tested sizeof(void*) to be of 2 chars, hence we declare it 16-bit */

typedef uint16_t uintptr_t;
typedef  int16_t  intptr_t;
EOF
;;
  1:2:4)
    a="int32_t" ; cat >>$ac_stdint_h <<EOF
/* we tested sizeof(void*) to be of 4 chars, hence we declare it 32-bit */

typedef uint32_t uintptr_t;
typedef  int32_t  intptr_t;
EOF
;;
  1:2:8)
    a="int64_t" ; cat >>$ac_stdint_h <<EOF
/* we tested sizeof(void*) to be of 8 chars, hence we declare it 64-bit */

typedef uint64_t uintptr_t;
typedef  int64_t  intptr_t;
EOF
;;
  *)
    a="long" ; cat >>$ac_stdint_h <<EOF
/* we tested sizeof(void*) but got no guess, hence we declare it as if long */

typedef unsigned long uintptr_t;
typedef          long  intptr_t;
EOF
;;
  esac
AC_MSG_RESULT(..adding typedef $a intptr_t)
fi

# ------------- DONE intptr types START int_least types ------------
if test "$ac_cv_type_int_least32_t" = "no"; then
AC_MSG_RESULT(..adding generic int_least-types)
     cat >>$ac_stdint_h <<EOF

/* --------------GENERIC INT_LEAST ------------------ */

typedef  int8_t    int_least8_t;
typedef  int16_t   int_least16_t;
typedef  int32_t   int_least32_t;
#ifdef _HAVE_UINT64_T
typedef  int64_t   int_least64_t;
#endif

typedef uint8_t   uint_least8_t;
typedef uint16_t  uint_least16_t;
typedef uint32_t  uint_least32_t;
#ifdef _HAVE_UINT64_T
typedef uint64_t  uint_least64_t;
#endif
EOF
fi

# ------------- DONE intptr types START int_least types ------------
if test "$ac_cv_type_int_fast32_t" = "no"; then
AC_MSG_RESULT(..adding generic int_fast-types)
     cat >>$ac_stdint_h <<EOF

/* --------------GENERIC INT_FAST ------------------ */

typedef  int8_t    int_fast8_t;
typedef  int32_t   int_fast16_t;
typedef  int32_t   int_fast32_t;
#ifdef _HAVE_UINT64_T
typedef  int64_t   int_fast64_t;
#endif

typedef uint8_t   uint_fast8_t;
typedef uint32_t  uint_fast16_t;
typedef uint32_t  uint_fast32_t;
#ifdef _HAVE_UINT64_T
typedef uint64_t  uint_fast64_t;
#endif
EOF
fi

if test "$ac_cv_header_stdint_x" = "no-file" ; then
     cat >>$ac_stdint_h <<EOF

#ifdef _HAVE_UINT64_T
typedef int64_t        intmax_t;
typedef uint64_t      uintmax_t;
#else
typedef long int       intmax_t;
typedef unsigned long uintmax_t;
#endif
EOF
fi

AC_MSG_RESULT(... DONE $ac_stdint_h)
   cat >>$ac_stdint_h <<EOF

  /* once */
#endif
#endif
EOF
])

dnl quote from SunOS-5.8 sys/inttypes.h:
dnl Use at your own risk.  As of February 1996, the committee is squarely
dnl behind the fixed sized types; the "least" and "fast" types are still being
dnl discussed.  The probability that the "fast" types may be removed before
dnl the standard is finalized is high enough that they are not currently
dnl implemented.



dnl AC_LIBRARY_NET
dnl Written by John Hawkinson <jhawk@mit.edu>. This code is in the Public
dnl Domain.
dnl
dnl This test is for network applications that need socket() and
dnl gethostbyname() -ish functions.  Under Solaris, those applications need to
dnl link with "-lsocket -lnsl".  Under IRIX, they should *not* link with
dnl "-lsocket" because libsocket.a breaks a number of things (for instance:
dnl gethostbyname() under IRIX 5.2, and snoop sockets under most versions of
dnl IRIX).
dnl 
dnl Unfortunately, many application developers are not aware of this, and
dnl mistakenly write tests that cause -lsocket to be used under IRIX.  It is
dnl also easy to write tests that cause -lnsl to be used under operating
dnl systems where neither are necessary (or useful), such as SunOS 4.1.4, which
dnl uses -lnsl for TLI.
dnl 
dnl This test exists so that every application developer does not test this in
dnl a different, and subtly broken fashion.
dnl 
dnl It has been argued that this test should be broken up into two seperate
dnl tests, one for the resolver libraries, and one for the libraries necessary
dnl for using Sockets API. Unfortunately, the two are carefully intertwined and
dnl allowing the autoconf user to use them independantly potentially results in
dnl unfortunate ordering dependancies -- as such, such component macros would
dnl have to carefully use indirection and be aware if the other components were
dnl executed. Since other autoconf macros do not go to this trouble, and almost
dnl no applications use sockets without the resolver, this complexity has not
dnl been implemented.
dnl
dnl The check for libresolv is in case you are attempting to link statically
dnl and happen to have a libresolv.a lying around (and no libnsl.a).
dnl
AC_DEFUN(AC_LIBRARY_NET, [
   # Most operating systems have gethostbyname() in the default searched
   # libraries (i.e. libc):
   AC_CHECK_FUNC(gethostbyname, ,
     # Some OSes (eg. Solaris) place it in libnsl:
     AC_CHECK_LIB(nsl, gethostbyname, , 
       # Some strange OSes (SINIX) have it in libsocket:
       AC_CHECK_LIB(socket, gethostbyname, ,
          # Unfortunately libsocket sometimes depends on libnsl.
          # AC_CHECK_LIB's API is essentially broken so the following
          # ugliness is necessary:
          AC_CHECK_LIB(socket, gethostbyname,
             LIBS="-lsocket -lnsl $LIBS",
               AC_CHECK_LIB(resolv, gethostbyname),
             -lnsl)
       )
     )
   )
  AC_CHECK_FUNC(socket, , AC_CHECK_LIB(socket, socket, ,
    AC_CHECK_LIB(socket, socket, LIBS="-lsocket -lnsl $LIBS", , -lnsl)))
  ])


dnl +little fix for solaris sh --mbuna
dnl 
dnl ______ /usr/share/aclocal/rleigh/ac_config_libconfig_in.m4 ______
dnl @synopsis AC_CONFIG_LIBCONFIG_IN [(LIBRARY [, DESTINATION [, MODULES]])]
dnl
dnl Creates a custom LIBRARY-config script.  The script supports
dnl --cflags, --libs and --version options, as well as all of the
dnl standard configure variables e.g. --libdir and --sysconfdir.
dnl --prefix and --exec-prefix may be specified to override the
dnl defaults.  --help gives a complete list of available options.
dnl Modules allow multiple sets of --libs, --cflags and --version
dnl options to be used, and combined together.
dnl
dnl This macro saves you all the typing for a library-config.in script;
dnl you don't even need to distribute one along. Place this macro
dnl in your configure.ac, et voila, you got one that you want to install.
dnl
dnl The options:
dnl
dnl  $1 = LIBRARY       e.g. gtk, ncurses
dnl  $2 = DESTINATION   Directory path, e.g. src/scripts
dnl  $3 = MODULES       Additional library modules, not including the
dnl                     default ($1).
dnl
dnl It is suggested that the following CFLAGS and LIBS variables are
dnl used in your configure.in.  library_libs is *essential*.
dnl library_cflags is important, but not always needed.  If they do not
dnl exist, defaults will be taken from LIBRARY_CFLAGS, LIBRARY_LIBS
dnl (should be -llibrary *only*) and LIBRARY_LIBDEPS (-l options for
dnl libraries your library depends upon.
dnl LIBLIBRARY_LIBS is simply $LIBRARY_LIBS $LIBRARY_LIBDEPS.
dnl NB. LIBRARY and library are the name of your library, in upper and
dnl lower case repectively e.g. GTK, gtk.
dnl
dnl  LIBRARY_CFLAGS:    cflags for compiling libraries and example progs
dnl  LIBRARY_LIBS:      libraries for linking programs
dnl  LIBRARY_LIBDEPS*:  libraries for linking libraries against (needed
dnl                     to link -static
dnl  library_cflags*:   cflags to store in library-config
dnl  library_libs*:     libs to store in library-config
dnl  LIBLIBRARY_LIBS:   libs to link programs IN THIS PACKAGE ONLY against
dnl  LIBRARY_VERSION*:  the version of your library (x.y.z recommended)
dnl    *=required if you want sensible output, otherwise they will be
dnl      *guessed* (DWIM)
dnl
dnl Support for pkg-config is available by default.  libs, cflags and
dnl version information will be obtained from the pkg-config metadata
dnl file library.pc if it exists in the pkgconfig data directory.
dnl The above information still needs to be specified, as it will still
dnl be used if pkg-config is not found on the host system at configure
dnl time.
dnl
dnl -I and -L flags will be obtained from *both* pkgconfig and the script.
dnl AC_CONFIG_LIBCONFIG_IN_PKGCONFIG enables pkg-config support (default)
dnl AC_CONFIG_LIBCONFIG_IN_STATIC disables pkg-config support
dnl
dnl There is also an AC_SUBST(LIBRARY_CONFIG) that will be set to
dnl the name of the file that we output in this macro. Use as:
dnl
dnl  install-exec-local: install-config
dnl  install-config:
dnl     $(mkinstalldirs) $(DESTDIR)$(bindir)
dnl     $(INSTALL_EXEC) @LIBRARY_CONFIG@ $(DESTDIR)$(bindir)
dnl
dnl Or, if using automake:
dnl
dnl  bin_SCRIPTS = @LIBRARY_CONFIG@
dnl
dnl Based upon AC_CREATE_GENERIC_CONFIG (ac_create_generic_config.m4)
dnl by Guido Draheim <guidod@gmx.de> and gtk-config by Owen Taylor.
dnl
dnl @version %Id: ac_config_libconfig_in.m4,v 1.4 2002/09/12 22:11:52 guidod Exp %
dnl @author Roger Leigh <roger@whinlatter.uklinux.net>
dnl
# AC_CONFIG_LIBCONFIG_IN(LIBRARY, DESTINATION, MODULES)
# -----------------------------------------------------
# Generate a custom LIBRARY-config script.  Create the script in the
# DESTINATION directory, including support for MODULES.
AC_DEFUN([AC_CONFIG_LIBCONFIG_IN],
[# create a custom library-config file ($1-config)
m4_if(AC_CONFIG_LIBCONFIG_IN_USEPKGCONFIG, [true],
      [AC_PATH_PROG(PKG_CONFIG, pkg-config)])
pushdef([LIBCONFIG_DIR], [m4_if([$2], , , [$2/])])
LIBCONFIG_FILE="LIBCONFIG_DIR[]$1-config.in"
AC_SUBST(target)dnl
AC_SUBST(host)dnl
AC_SUBST(build)dnl
dnl create directory if it does not preexist
m4_if([$2], , , [AS_MKDIR_P([$2])])
AC_MSG_NOTICE([creating $LIBCONFIG_FILE])
echo '#! /bin/sh' >$LIBCONFIG_FILE
echo "# $1-config library configuration script" >>$LIBCONFIG_FILE
echo '# generated by ac_config_libconfig_in.m4' >>$LIBCONFIG_FILE
echo ' ' >>$LIBCONFIG_FILE
echo 'template_version="1.0.0"' >>$LIBCONFIG_FILE
echo ' ' >>$LIBCONFIG_FILE
echo 'package="@PACKAGE@"' >>$LIBCONFIG_FILE
echo ' ' >>$LIBCONFIG_FILE
echo '# usage instructions if no options given' >>$LIBCONFIG_FILE
echo 'if test "'"\$""#"'" -eq 0; then' >>$LIBCONFIG_FILE
echo '   cat <<EOF' >>$LIBCONFIG_FILE
m4_if($3, ,
      [echo 'Usage: $1-config [[OPTIONS]]' >>$LIBCONFIG_FILE],
      [echo 'Usage: $1-config [[OPTIONS]] [[LIBRARIES]]' >>$LIBCONFIG_FILE])
echo 'Options:' >>$LIBCONFIG_FILE
echo '        [[--prefix[=DIR]]]' >>$LIBCONFIG_FILE
echo '        [[--exec-prefix[=DIR]]]' >>$LIBCONFIG_FILE
echo '        [[--package]]' >>$LIBCONFIG_FILE
echo '        [[--version]]' >>$LIBCONFIG_FILE
echo '        [[--cflags]]' >>$LIBCONFIG_FILE
echo '        [[--libs]]' >>$LIBCONFIG_FILE
echo '        [[--help]]' >>$LIBCONFIG_FILE
m4_if($3, , ,
      [echo 'Libraries:' >>$LIBCONFIG_FILE
       for module in $1 $3 ; do
         echo "        $module" >>$LIBCONFIG_FILE ;
       done])
echo 'EOF' >>$LIBCONFIG_FILE
echo 'fi' >>$LIBCONFIG_FILE
echo ' ' >>$LIBCONFIG_FILE
echo '# parse options' >>$LIBCONFIG_FILE
echo 'o=""' >>$LIBCONFIG_FILE
echo 'h=""' >>$LIBCONFIG_FILE
echo 'for i in $''*; do' >>$LIBCONFIG_FILE
echo '  case $i in' >>$LIBCONFIG_FILE
options="prefix exec-prefix eprefix package version cflags libs bindir sbindir libexecdir datadir sysconfdir sharedstatedir localstatedir libdir infodir mandir target host build pkgdatadir pkglibdir pkgincludedir template-version help"
echo '    --prefix=*) prefix=`echo $i | sed -e "s/--prefix=//"` ;;' >>$LIBCONFIG_FILE
echo '    --exec-prefix=*) exec_prefix=`echo $i | sed -e "s/--exec-prefix=//"` ;;' >>$LIBCONFIG_FILE
echo '    --eprefix=*) exec_prefix=`echo $i | sed -e "s/--eprefix=//"` ;;' >>$LIBCONFIG_FILE
for option in $options ; do
  case $option in
    exec-prefix)  echo "    --$option) echo_exec_prefix=\"yes\" ;;" >>$LIBCONFIG_FILE ;;
    template-version)  echo "    --$option) echo_template_version=\"yes\" ;;" >>$LIBCONFIG_FILE ;;
    *)  echo "    --$option) echo_$option=\"yes\" ;;" >>$LIBCONFIG_FILE ;;
  esac
done
m4_if($3, , ,
      [for module in $1 $3 ; do
         echo "  $module) echo_module_$module=\"yes\" ;" >>$LIBCONFIG_FILE ;
         echo '    echo_module="yes" ;;' >>$LIBCONFIG_FILE ;
       done])
echo '    //*|/*//*|./*//*)        echo_extra="yes" ;;' >>$LIBCONFIG_FILE
echo '    *) eval "echo Unknown option: $i" ; exit 1 ;;' >>$LIBCONFIG_FILE
echo '  esac' >>$LIBCONFIG_FILE
echo 'done' >>$LIBCONFIG_FILE
echo ' ' >>$LIBCONFIG_FILE
# in the order of occurence a standard automake Makefile
echo '# defaults from configure; set only if not set previously' >>$LIBCONFIG_FILE
vars="prefix exec_prefix bindir sbindir libexecdir datadir sysconfdir sharedstatedir localstatedir libdir infodir mandir includedir target host build"
for var in $vars ; do
  echo "if test -z \"\$$var\" ; then" >>$LIBCONFIG_FILE
  echo "  $var=\"@$var@\"" >>$LIBCONFIG_FILE
  echo 'fi' >>$LIBCONFIG_FILE
done
echo ' ' >>$LIBCONFIG_FILE
echo '# generate output' >>$LIBCONFIG_FILE
echo 'if test x$echo_module != xyes ; then' >>$LIBCONFIG_FILE
echo '  echo_module_$1="yes"' >>$LIBCONFIG_FILE
echo 'fi' >>$LIBCONFIG_FILE
AC_CONFIG_LIBCONFIG_IN_MODULES(m4_if([$3], , [$1], [m4_translit([$1 $3], [ ], [,])]))dnl
for option in $options extra; do
  case $option in
    exec-prefix)  echo "if test x\$echo_exec_prefix = xyes ; then" >>$LIBCONFIG_FILE ;;
    template-version)  echo "if test x\$echo_template_version = xyes ; then" >>$LIBCONFIG_FILE ;;
    *)  echo "if test x\$echo_$option = xyes ; then" >>$LIBCONFIG_FILE ;;
  esac
  case $option in
    exec-prefix | eprefix)  echo '  o="$o $exec_prefix"' >>$LIBCONFIG_FILE ;;
    template-version)  echo '  o="$o $template_version"' >>$LIBCONFIG_FILE ;;
    cflags)
      echo '  i=`eval echo "$includedir"`' >>$LIBCONFIG_FILE ;
      echo '  i=`eval echo "$i"`' >>$LIBCONFIG_FILE ;
      echo '  if test "_$i" != "_/usr/include" ; then' >>$LIBCONFIG_FILE ;
      echo '    o="$o -I$includedir"' >>$LIBCONFIG_FILE ;
      echo '  fi' >>$LIBCONFIG_FILE ;
      echo '  o="$o $cflags"' >>$LIBCONFIG_FILE ;;
    libs)  echo '  o="$o -L$libdir $libs"' >>$LIBCONFIG_FILE ;;
    help)  echo '  h="1"' >>$LIBCONFIG_FILE ;;
    pkgdatadir)  echo "  o=\"$o \${datadir}/\${package}\"" >>$LIBCONFIG_FILE ;;
    pkglibdir)  echo "  o=\"$o \${libdir}/\${package}\"" >>$LIBCONFIG_FILE ;;
    pkgincludedir)  echo "  o=\"$o \${includedir}/\${package}\"" >>$LIBCONFIG_FILE ;;
    extra)
      echo '  v=`echo $i | sed -e s://:\$:g`' >>$LIBCONFIG_FILE ;
      echo '  v=`eval "echo $v"`' >>$LIBCONFIG_FILE ;
      echo '  o="$o $v"' >>$LIBCONFIG_FILE ;;
    *)  echo "  o=\"$o \$$option\"" >>$LIBCONFIG_FILE
  esac
  echo 'fi' >>$LIBCONFIG_FILE
done
echo ' ' >>$LIBCONFIG_FILE
echo '# output data' >>$LIBCONFIG_FILE
echo 'o=`eval "echo $o"`' >>$LIBCONFIG_FILE
echo 'o=`eval "echo $o"`' >>$LIBCONFIG_FILE
echo 'if test -n "$o" ; then ' >>$LIBCONFIG_FILE
echo '  eval "echo $o"' >>$LIBCONFIG_FILE
echo 'fi' >>$LIBCONFIG_FILE
echo ' ' >>$LIBCONFIG_FILE
echo '# help text' >>$LIBCONFIG_FILE
echo 'if test ! -z "$h" ; then ' >>$LIBCONFIG_FILE
echo '  cat <<EOF' >>$LIBCONFIG_FILE
echo 'All available options:' >>$LIBCONFIG_FILE
echo '  --prefix=DIR and   change \$prefix and \$exec-prefix' >>$LIBCONFIG_FILE
echo '  --exec-prefix=DIR  (affects all other options)' >>$LIBCONFIG_FILE
echo '  --prefix           \$prefix        $prefix' >>$LIBCONFIG_FILE
echo '  --exec_prefix  or... ' >>$LIBCONFIG_FILE
echo '  --eprefix          \$exec_prefix   $exec_prefix' >>$LIBCONFIG_FILE
echo '  --version          \$version       $version' >>$LIBCONFIG_FILE
echo '  --cflags           -I\$includedir  unless it is /usr/include' >>$LIBCONFIG_FILE
echo '  --libs             -L\$libdir \$LIBS $libs' >>$LIBCONFIG_FILE
echo '  --package          \$package       $package' >>$LIBCONFIG_FILE
echo '  --bindir           \$bindir        $bindir' >>$LIBCONFIG_FILE
echo '  --sbindir          \$sbindir       $sbindir' >>$LIBCONFIG_FILE
echo '  --libexecdir       \$libexecdir    $libexecdir' >>$LIBCONFIG_FILE
echo '  --datadir          \$datadir       $datadir' >>$LIBCONFIG_FILE
echo '  --sysconfdir       \$sysconfdir    $sysconfdir' >>$LIBCONFIG_FILE
echo '  --sharedstatedir   \$sharedstatedir$sharedstatedir' >>$LIBCONFIG_FILE
echo '  --localstatedir    \$localstatedir $localstatedir' >>$LIBCONFIG_FILE
echo '  --libdir           \$libdir        $libdir' >>$LIBCONFIG_FILE
echo '  --infodir          \$infodir       $infodir' >>$LIBCONFIG_FILE
echo '  --mandir           \$mandir        $mandir' >>$LIBCONFIG_FILE
echo '  --target           \$target        $target' >>$LIBCONFIG_FILE
echo '  --host             \$host          $host' >>$LIBCONFIG_FILE
echo '  --build            \$build         $build' >>$LIBCONFIG_FILE
echo '  --pkgdatadir       \$datadir/\$package    ${datadir}/${package}'    >>$LIBCONFIG_FILE
echo '  --pkglibdir        \$libdir/\$package     ${libdir}/${package}' >>$LIBCONFIG_FILE
echo '  --pkgincludedir    \$includedir/\$package ${includedir}/${package}' >>$LIBCONFIG_FILE
echo '  --template-version \$template_version     $template_version' >>$LIBCONFIG_FILE
echo '  --help' >>$LIBCONFIG_FILE
echo 'EOF' >>$LIBCONFIG_FILE
echo 'fi' >>$LIBCONFIG_FILE
m4_pushdef([LIBCONFIG_UP], [m4_translit([$1], [a-z], [A-Z])])dnl
LIBCONFIG_UP[]_CONFIG="LIBCONFIG_DIR[]$1-config"
AC_SUBST(LIBCONFIG_UP[]_CONFIG)
dnl AC_CONFIG_FILES(LIBCONFIG_DIR[]$1[-config], [chmod +x ]LIBCONFIG_DIR[]$1[-config])
m4_popdef([LIBCONFIG_DIR])
m4_popdef([LIBCONFIG_UP])
])


# AC_CONFIG_LIBCONFIG_IN_MODULES [(MODULE1 [, MODULE2 [, ...]])]
# --------------------------------------------------------------
#Output shell script using custom module variables.
AC_DEFUN([AC_CONFIG_LIBCONFIG_IN_MODULES],
[m4_if([$1], , ,
       [# create module definition for $1
dnl we're going to need uppercase, lowercase and user-friendly versions of the
dnl string `MODULE'
m4_pushdef([MODULE_UP], m4_translit([$1], [a-z], [A-Z]))dnl
m4_pushdef([MODULE_DOWN], m4_translit([$1], [A-Z], [a-z]))dnl
if test -z "$MODULE_DOWN[]_cflags" ; then
  if test -n "$MODULE_UP[]_CFLAGS" ; then
      MODULE_DOWN[]_cflags="$MODULE_UP[]_CFLAGS"
  else
dnl    AC_MSG_WARN([variable `MODULE_DOWN[]_cflags' undefined])
    MODULE_DOWN[]_cflags=''
  fi
fi
AC_SUBST(MODULE_DOWN[]_cflags)dnl
if test -z "$MODULE_DOWN[]_libs" ; then
  if test -n "$MODULE_UP[]_LIBS" ; then
    MODULE_DOWN[]_libs="$MODULE_UP[]_LIBS"
  else
    AC_MSG_WARN([variable `MODULE_DOWN[]_libs' and `MODULE_UP[]_LIBS' undefined])
    MODULE_DOWN[]_libs='-l$1'
  fi
  if test -n "$MODULE_UP[]_LIBDEPS" ; then
    MODULE_DOWN[]_libs="$MODULE_DOWN[]_libs $MODULE_UP[]_LIBDEPS"
  fi
fi
AC_SUBST(MODULE_DOWN[]_libs)dnl
if test -z "$MODULE_UP[]_VERSION" ; then
  AC_MSG_WARN([variable `MODULE_UP[]_VERSION' undefined])
  MODULE_UP[]_VERSION="$VERSION"
fi
AC_SUBST(MODULE_UP[]_VERSION)dnl
echo 'if test x$echo_module_$1 = xyes ; then' >>$LIBCONFIG_FILE
AC_CONFIG_LIBCONFIG_IN_MODULES_VARS([cflags], [MODULE_DOWN[]_cflags], [cflags])
AC_CONFIG_LIBCONFIG_IN_MODULES_VARS([libs], [MODULE_DOWN[]_libs], [libs])
AC_CONFIG_LIBCONFIG_IN_MODULES_VARS([version], [MODULE_UP[]_VERSION], [modversion])
echo 'fi' >>$LIBCONFIG_FILE
m4_popdef([MODULE_UP])dnl
m4_popdef([MODULE_DOWN])dnl
AC_CONFIG_LIBCONFIG_IN_MODULES(m4_shift($@))])dnl
])


# AC_CONFIG_LIBCONFIG_IN_MODULES_VARS [(VAR, SUBSTITUTION,
# PKGCONFIG_ARGS)]
# --------------------------------------------------------
# Output AC_CONFIG_LIBCONFIG_IN_MODULES variables.
#   VAR = variable to set
#   SUBSTITUTION = set if pkg-config is not available
#   PKGCONFIG_ARGS = args for pkg-config
AC_DEFUN([AC_CONFIG_LIBCONFIG_IN_MODULES_VARS],
[m4_if(AC_CONFIG_LIBCONFIG_IN_USEPKGCONFIG, [true],
[
echo 'if test -x "`which pkg-config`" ; then' >>$LIBCONFIG_FILE
echo '  if pkg-config --atleast-pkgconfig-version=0.7 --exists "MODULE_DOWN"; then' >>$LIBCONFIG_FILE
echo '    $1="@S|@$1 `pkg-config --$3 MODULE_DOWN`"' >>$LIBCONFIG_FILE
echo '  fi' >>$LIBCONFIG_FILE
echo 'else' >>$LIBCONFIG_FILE
echo '  $1="@S|@$1 @$2@"' >>$LIBCONFIG_FILE
echo 'fi' >>$LIBCONFIG_FILE
],
[echo '  $1="@S|@$1 @$2@"' >>$LIBCONFIG_FILE
])])


# AC_CONFIG_LIBCONFIG_IN_PKGCONFIG
# --------------------------------
# Enable pkgconfig support in libconfig script (default)
AC_DEFUN([AC_CONFIG_LIBCONFIG_IN_PKGCONFIG],
[m4_define([AC_CONFIG_LIBCONFIG_IN_USEPKGCONFIG], [true])
])dnl


# AC_CONFIG_LIBCONFIG_IN_STATIC
# -----------------------------
# Disable pkgconfig support in libconfig script
AC_DEFUN([AC_CONFIG_LIBCONFIG_IN_STATIC],
[m4_define([AC_CONFIG_LIBCONFIG_IN_USEPKGCONFIG], [false])
])dnl
