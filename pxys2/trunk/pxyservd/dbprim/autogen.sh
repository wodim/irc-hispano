#! /bin/sh

aclocal -I m4
autoconf
libtoolize --automake --copy
automake -a -c
