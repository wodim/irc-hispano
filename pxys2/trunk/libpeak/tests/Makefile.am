AUTOMAKE_OPTS = foreign

SRCS = t_atomic1 t_atomic2 t_signal1 t_signal2 t_timer1 t_timer2 t_timer3
check_PROGRAMS = $(SRCS)

# We use a special check script
#TESTS = $(check_PROGRAMS)

t_atomic1_SOURCES = t_atomic1.c
t_atomic1_LDADD = ../peak/libpeak.la
t_atomic1_LDFLAGS = -static

t_atomic2_SOURCES = t_atomic2.c
t_atomic2_LDADD = ../peak/libpeak.la
t_atomic2_LDFLAGS = -static

t_signal1_SOURCES = t_signal1.c
t_signal1_LDADD = ../peak/libpeak.la
t_signal1_LDFLAGS = -static

t_signal2_SOURCES = t_signal2.c
t_signal2_LDADD = ../peak/libpeak.la
t_signal2_LDFLAGS = -static

t_timer1_SOURCES = t_timer1.c
t_timer1_LDADD = ../peak/libpeak.la
t_timer1_LDFLAGS = -static

t_timer2_SOURCES = t_timer2.c
t_timer2_LDADD = ../peak/libpeak.la
t_timer2_LDFLAGS = -static

t_timer3_SOURCES = t_timer3.c
t_timer3_LDADD = ../peak/libpeak.la
t_timer3_LDFLAGS = -static

INCLUDES = -I..

CLEANFILES = STDERR.log STDOUT.log
EXTRA_DIST = test-suite.pl test.desc

check:
	@perl ./test-suite.pl $(SRCS)
