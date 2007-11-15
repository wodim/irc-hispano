#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <pthread.h>

#define RCSID "$Id: PXSecret.c,v 1.1 2006/09/10 22:18:56 spale Exp $"

#include "sha256.h"

#define HASH_UPDATE 3600

static time_t last_ts = 0;
static char unsigned hash[32] = { 0 };
static char hashstr[65]  = { 0 };
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *__memmem(const char *l, size_t l_len, const char *s, size_t s_len);

int ScannerSha256StringCheck(const char *string, int len, const char *secret) {
	time_t ts = time(NULL);
	sha256_context ctx;
	int i;

	if ( string == NULL || secret == NULL )
		return 0;

	if ( ts - last_ts >= HASH_UPDATE ) {
		char timestring[100];

		pthread_mutex_lock(&mutex);

		sprintf(timestring, "%li",(long int)ts - ( ts % HASH_UPDATE ));

		sha256_starts(&ctx);
		sha256_update(&ctx, (uint8_t*)secret, strlen(secret));
		sha256_update(&ctx, (uint8_t*)timestring, strlen(timestring));
		sha256_finish(&ctx, hash);


		for(i=0; i<32; i++)
			sprintf(hashstr + (i*2), "%02X", hash[i]);

		last_ts = ts - ( ts % HASH_UPDATE );

		pthread_mutex_unlock(&mutex);
	}

	if ( __memmem(string,len,hashstr,64) != NULL )
		return 1;

	return 0;
}

void *__memmem(const char *l, size_t l_len, const char *s, size_t s_len) {
	char *cur, *last;

	if ( l_len == 0 || s_len == 0 )
		return NULL;

	if ( l_len < s_len )
		return NULL;

	if ( s_len == 1 )
		return memchr(l, s[0], l_len);

	last = (char*)l + l_len - s_len;

	for(cur = (char*)l; cur <= last; cur++)
		if (cur[0] == s[0] && memcmp(cur, s, s_len) == 0)
			return cur;
	return NULL;
}
