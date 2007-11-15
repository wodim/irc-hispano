# $Id: Makefile,v 1.5 2007/05/24 12:54:10 mbuna Exp $

CC=gcc
# for darwin/mac os x - uncomment on linux
LOCKDEFS=-DHAVE__SPIN_LOCK=1 -DHAVE__SPIN_LOCK_TRY=1 -DHAVE__SPIN_UNLOCK=1

all: dummy
	@echo "Syntax: make <arch>"
	@echo "Available arch: m68k, powerpc, sparc, x86, ia64"

m68k: dummy
	$(CC) -Wall -O2 -DNOATOMIC -I../m68k -o test atomictest.c -pthread
	$(CC) -Wall -O2 -I../m68k -o atomictest atomictest.c -pthread
	$(CC) -Wall -O2 -I../m68k -o spinlocktest spinlocktest.c -pthread

powerpc: dummy
	$(CC) -Wall -O2 -DNOATOMIC -I../powerpc -o test atomictest.c
	$(CC) -Wall -O2 -I../powerpc -o atomictest atomictest.c
	$(CC) -Wall -O2 $(LOCKDEFS) -I../powerpc -o spinlocktest spinlocktest.c

sparc: dummy
	$(CC) -Wall -O2 -DNOATOMIC -I../sparc -o test atomictest.c -pthreads
	$(CC) -Wall -O2 -I../sparc -o atomictest atomictest.c -pthreads
	$(CC) -Wall -O2 -I../sparc -o spinlocktest spinlocktest.c -pthreads -lrt

x86: dummy
	$(CC) -Wall -O2 -DNOATOMIC -I../x86 -o test atomictest.c -pthread
	$(CC) -Wall -O2 -I../x86 -o atomictest atomictest.c -pthread
	$(CC) -Wall -O2 -I../x86 -o spinlocktest spinlocktest.c -pthread

ia64: dummy
	$(CC) -Wall -O2 -DNOATOMIC -I../ia64 -o test atomictest.c -pthread
	$(CC) -Wall -O2 -I../ia64 -o atomictest atomictest.c -pthread
	$(CC) -Wall -O2 -I../ia64 -o spinlocktest spinlocktest.c -pthread
	

clean: dummy
	rm -f test atomictest spinlocktest

dummy:
