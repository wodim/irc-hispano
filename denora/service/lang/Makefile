# Make file
#
# (c) 2004-2011 Denora Team
# Contact us at info@denorastats.org
#
# Please read COPYING and README for further details.
#
# Based on the original code of Anope by Anope Team.
# Based on the original code of Thales by Lucas.
#
# $Id: Makefile 386 2011-01-04 17:16:43Z hal9000 $
#

MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'DENORALIBS=${DENORALIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
     	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
   	   'RUNGROUP=${RUNGROUP}' 'USE_MODULES=${USE_MODULES}'

LANGOBJS = en_us es de fr nl it ru pt_br
LANGSRCS = en_us.l es.l de.l fr.l nl.l it.l ru.l pt_br.l

LANGCOMP = ./langcomp
#LANGCOMP = ./langcomp -w


all: $(LANGOBJS)

distclean: spotless

install: all
	test -d $(DATDEST)/languages || mkdir $(DATDEST)/languages
	@if [ "$(RUNGROUP)" ] ; then \
		echo chgrp -R $(RUNGROUP) $(DATDEST)/languages ; \
		chgrp -R $(RUNGROUP) $(DATDEST)/languages ; \
		echo chmod -R g+rw $(DATDEST)/languages ; \
		chmod -R g+rw $(DATDEST)/languages ; \
		$(CP) $(LANGOBJS) $(DATDEST)/languages ; \
		echo chgrp -R $(RUNGROUP) $(DATDEST)/languages ; \
		chgrp -R $(RUNGROUP) $(DATDEST)/languages/* ; \
		echo chmod -R g+rw $(DATDEST)/languages/* ; \
		chmod -R g+rw $(DATDEST)/languages/* ; \
	fi
	@if [ ! "$(RUNGROUP)" ] ; then \
		chmod 700 $(DATDEST)/languages/ ; \
		$(CP) $(LANGOBJS) $(DATDEST)/languages ; \
		chmod 600 $(DATDEST)/languages/* ; \
	fi
	
clean:
	rm -f $(LANGOBJS) langcomp

spotless:
	rm -f language.h index $(LANGOBJS) langcomp

en_us:    en_us.l    langcomp index
	./langcomp $@.l
es:       es.l       langcomp index
	./langcomp $@.l
de:    de.l    langcomp index
	./langcomp $@.l
fr:    fr.l    langcomp index
	./langcomp $@.l
nl:    nl.l    langcomp index
	./langcomp $@.l
it:    it.l    langcomp index
	./langcomp $@.l
ru:    ru.l    langcomp index
	./langcomp $@.l
pt_br:    pt_br.l    langcomp index
	./langcomp $@.l
	
langcomp: langcomp.c
	$(CC) $(CFLAGS) langcomp.c -o $@


language.h: index Makefile
	@perl -e <index >$@ 'print STDERR "Generating language.h... "; $$i=0; while (<>) { chop; printf "#define %-32s %d\n", $$_, $$i++; } print "\n#define NUM_STRINGS $$i\n"; print STDERR "$$i strings\n";'

index: en_us.l
	grep '^[A-Z]' en_us.l >index
