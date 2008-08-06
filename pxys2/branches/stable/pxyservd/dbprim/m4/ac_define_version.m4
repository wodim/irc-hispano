dnl AC_DEFINE_VERSION(library, header, cfile, version, prefix)
AC_DEFUN([AC_DEFINE_VERSION],
[# create files to designate the version
dnl get upper- and lower-case versions of string `LIBRARY'
m4_pushdef([UP], m4_translit([$1], [a-z], [A-Z]))dnl
m4_pushdef([DOWN], m4_translit([$1], [A-Z], [a-z]))dnl
m4_pushdef([UPREFIX], m4_if([$5], , UP, m4_translit([$5], [a-z], [A-Z])))dnl
m4_pushdef([LPREFIX], m4_if([$5], , DOWN, m4_translit([$5], [A-Z], [a-z])))dnl
m4_pushdef([HEADER], m4_if([$2], , $1[]_version.h, [$2]))dnl
m4_pushdef([CFILE], m4_if([$3], , $1[]_version.c, [$3]))dnl

UPREFIX[]_VERSION_HEADER=HEADER
UPREFIX[]_VERSION_CFILE=CFILE
lib_version=[]m4_if([$4], , "$UP[]_VERSION", [$4])

UPREFIX[]_MAJOR_VERSION=`echo "$lib_version" | \
    sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
UPREFIX[]_MINOR_VERSION=`echo "$lib_version" | \
    sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
UPREFIX[]_MICRO_VERSION=`echo "$lib_version" | \
    sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

AC_MSG_NOTICE([configuring $1 version: $UPREFIX[]_MAJOR_VERSION.$UPREFIX[]_MINOR_VERSION.$UPREFIX[]_MICRO_VERSION])

AC_CONFIG_COMMANDS(HEADER,
AC_MSG_NOTICE([creating HEADER])
rm -f HEADER
echo "/* HEADER library version header" >> HEADER
echo ' * generated by ac_define_version.m4' >> HEADER
echo ' */' >> HEADER
echo '@%:@ifndef included_[]LPREFIX[]_version_h__' >> HEADER
echo '@%:@define included_[]LPREFIX[]_version_h__' >> HEADER
echo '' >> HEADER
echo "@%:@define UPREFIX[]_MAJOR_VERSION ($UPREFIX[]_MAJOR_VERSION)" >> HEADER
echo "@%:@define UPREFIX[]_MINOR_VERSION ($UPREFIX[]_MINOR_VERSION)" >> HEADER
echo "@%:@define UPREFIX[]_MICRO_VERSION ($UPREFIX[]_MICRO_VERSION)" >> HEADER
echo '' >> HEADER
echo 'extern const unsigned int LPREFIX[]_major_version;' >> HEADER
echo 'extern const unsigned int LPREFIX[]_minor_version;' >> HEADER
echo 'extern const unsigned int LPREFIX[]_micro_version;' >> HEADER
echo '' >> HEADER
echo '@%:@endif /* included_[]LPREFIX[]_version_h__ */' >> HEADER,
UPREFIX[]_MAJOR_VERSION=$UPREFIX[]_MAJOR_VERSION
UPREFIX[]_MINOR_VERSION=$UPREFIX[]_MINOR_VERSION
UPREFIX[]_MICRO_VERSION=$UPREFIX[]_MICRO_VERSION)

AC_CONFIG_COMMANDS(CFILE,
AC_MSG_NOTICE([creating CFILE])
rm -f CFILE
echo "/* CFILE library C file" >> CFILE
echo ' * generated by ac_define_version.m4' >> CFILE
echo ' */' >> CFILE
echo '' >> CFILE
echo "const unsigned int LPREFIX[]_major_version = $UPREFIX[]_MAJOR_VERSION;" >> CFILE
echo "const unsigned int LPREFIX[]_minor_version = $UPREFIX[]_MINOR_VERSION;" >> CFILE
echo "const unsigned int LPREFIX[]_micro_version = $UPREFIX[]_MICRO_VERSION;" >> CFILE)

AC_SUBST(UPREFIX[]_MAJOR_VERSION)
AC_SUBST(UPREFIX[]_MINOR_VERSION)
AC_SUBST(UPREFIX[]_MICRO_VERSION)
AC_SUBST_FILE(UPREFIX[]_VERSION_HEADER)
AC_SUBST_FILE(UPREFIX[]_VERSION_CFILE)

dnl Pop the macros we defined earlier
m4_popdef([UP])dnl
m4_popdef([DOWN])dnl
m4_popdef([UPREFIX])dnl
m4_popdef([LPREFIX])dnl
m4_popdef([HEADER])dnl
m4_popdef([CFILE])dnl
])# AC_DEFINE_VERSION