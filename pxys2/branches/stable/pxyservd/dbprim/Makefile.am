## Prefered emacs editing mode: -*- Makefile -*-
##
## Copyright (C) 2002, 2006 by Kevin L. Mitchell <klmitch@mit.edu>
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Library General Public
## License as published by the Free Software Foundation; either
## version 2 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Library General Public License for more details.
##
## You should have received a copy of the GNU Library General Public
## License along with this library; if not, write to the Free
## Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
## MA 02111-1307, USA
##
## @(#)$Id: Makefile.am,v 1.13 2006/07/15 18:06:18 klmitch Exp $
##
## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

# The . here forces prefix ordering, so make check rebuilds the
# library first if necessary
SUBDIRS = . tests

AM_CPPFLAGS = -D__DBPRIM_LIBRARY__

lib_LTLIBRARIES = libdbprim.la
libdbprim_la_SOURCES = \
	dbprim.h \
	dbprim_err.h \
	dbprim_int.h \
	dbprim_version.c \
	\
	_hash_prime.c \
	hash_fnv1.c \
	hash_fnv1a.c \
	hash_comp.c \
	he_init.c \
	ht_add.c \
	ht_find.c \
	ht_flush.c \
	ht_free.c \
	ht_init.c \
	ht_iter.c \
	ht_move.c \
	ht_remove.c \
	ht_resize.c \
	\
	le_init.c \
	ll_add.c \
	ll_find.c \
	ll_flush.c \
	ll_init.c \
	ll_iter.c \
	ll_move.c \
	ll_remove.c \
	\
	_rb_locate.c \
	_rb_rotate.c \
	rbtree_comp.c \
	rn_init.c \
	rt_add.c \
	rt_find.c \
	rt_flush.c \
	rt_init.c \
	rt_iter.c \
	rt_move.c \
	rt_next.c \
	rt_remove.c \
	\
	sh_find.c \
	sh_flush.c \
	sh_init.c \
	sh_iter.c \
	sh_move.c \
	smat_freelist.c \
	_smat_resize.c \
	st_add.c \
	st_find.c \
	st_flush.c \
	st_free.c \
	st_init.c \
	st_iter.c \
	st_remove.c \
	st_resize.c
libdbprim_la_LIBADD = @DBPRIM_ET_OBJS@
libdbprim_la_DEPENDENCIES = @DBPRIM_ET_OBJS@
libdbprim_la_LDFLAGS = -version-info 3:0:1

pkginclude_HEADERS = dbprim.h dbprim_err.h dbprim_version.h

bin_SCRIPTS = dbprim-config

EXTRA_CSTATUS = dbprim_version.c dbprim_version.h

BUILT_SOURCES = dbprim_err.h

EXTRA_DIST = dbprim_err.et dbprim.spec dbprim.spec.in m4 \
	     autogen.sh comp_et-sh.in COPYING INSTALL AUTHORS \
	     doc/Doxyfile doc/Doxyfile.in doc/html doc/latex

SUFFIXES = .et .h

CLEANFILES = $(EXTRA_CSTATUS) \
	     dbprim_err.c dbprim_err.h

DISTCLEANFILES = dbprim-config.in

DISTHOOKCLEANFILES = $(CLEANFILES)

.et.c .et.h: comp_et-sh
	$(SHELL) comp_et-sh --com_err=@COM_ERR@ --dir=$(top_builddir) $<

dbprim_err.c dbprim_err.h: dbprim_err.et

$(EXTRA_CSTATUS): $(top_srcdir)/configure config.status
	cd $(top_builddir) && $(SHELL) ./config.status $@

dist-hook:
	for file in $(DISTHOOKCLEANFILES) ; do \
	    rm -f $(distdir)/$$file ; \
	done ; \
	find $(distdir) -name CVS -print -prune | xargs rm -rf
