DEFS = -DCACHESIZE=10000000 -DLOOPS=10000000 -DIPCACHE_TEST

all:
	gcc -Wall $(DEFS) -o ipcache_test ipcache.c -lpeak

clean:
	rm -f ipcache_test
