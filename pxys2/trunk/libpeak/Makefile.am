AUTOMAKE_OPTIONS = foreign
SUBDIRS = peak . tests

docs: dummydoc
	doxygen doc/Doxyfile

dummydoc:

bin_SCRIPTS = peak-config

EXTRA_DIST = CREDITS README.platforms \
	doc/Doxyfile doc/Doxyfooter.html doc/html doc/images doc/latex
#DISTCLEANFILES = peak-config.in

dist-hook:
	rm -f $(distdir)/peak-config.in
	find $(distdir) -name CVS -print -prune | xargs rm -rf
	find $(distdir) -name .DS_Store -print -prune | xargs rm -rf 
