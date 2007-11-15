## Copyright (C) 2004 by Kevin L. Mitchell <klmitch@mit.edu>
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
## @(#)$Id: Makefile.am,v 1.8 2004/09/22 03:25:48 klmitch Exp $
##
## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -D__DBPRIM_LIBRARY__ -I$(top_builddir) -I$(top_srcdir)
LDADD = $(top_builddir)/libdbprim.la @COM_ERR_LDADD@
AM_LDFLAGS = -static

check_PROGRAMS = test-harness \
		 \
		 t_linklists \
		 t_hashtab \
		 t_smat \
		 t_redblack

test_harness_SOURCES = test-harness.c test-harness.h
test_harness_LDADD =
test_harness_LDFLAGS =
TEST_OPTIONS =

EXTRA_DIST = test-harness.dat

DISTCLEANFILES = test-harness.log

check:
	./test-harness -I. -I$(srcdir) $(TEST_OPTIONS)
