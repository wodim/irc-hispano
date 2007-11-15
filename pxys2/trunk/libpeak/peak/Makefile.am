
lib_LTLIBRARIES = libpeak.la

libpeakincludedir = $(includedir)/peak
libpeakinclude_HEADERS = peak.h alloc.h atomic.h db.h dict.h garbage.h mem_pool.h runtime.h signal.h stdint.h stream.h task.h time.h timer.h tz.h version.h

libpeak_la_SOURCES = \
	engine.h engine_client.h errors.h fileutils.h init_private.h internal.h semaphore.h socket.h task_class.h task_private.h task_runloop.h task_runloop_stackpool.h timer_tree.h utilities.h \
	alloc.c dict.c engine_client.c errors.c fileutils.c garbage.c init.c mem_pool.c runtime.c semaphore.c signal.c socket.c stream.c task.c task_lock.c task_mutex.c task_runloop.c task_runloop_stackpool.c time.c timer.c timer_tree.c tz.c utilities.c version.c

libpeak_la_LIBADD = @LTLIBOBJS@

libpeak_la_LDFLAGS = -version-info 1:0:1

#AM_CFLAGS = -pipe -Wall -g

DEFAULT_INCLUDES = @default_includes@
INCLUDES = @includes@

archlist = m68k powerpc sparc x86 ia64
EXTRA_DIST = 
DISTCLEANFILES = stdint.h

dist-hook:
	rm -f $(distdir)/stdint.h ; \
	mkdir $(distdir)/arch ; \
	for arch in $(archlist); do \
		mkdir $(distdir)/arch/$$arch ; \
		cp arch/$$arch/atomic.h arch/$$arch/spinlock.h $(distdir)/arch/$$arch/ ; \
	done;

