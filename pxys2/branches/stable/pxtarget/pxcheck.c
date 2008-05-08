/* Copyright (C) 2006 Pascal Gloor
 *
 * This file is part of pxtarget (from pxys)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <errno.h>

#include "sha256.h"

#define RCSID "$Id: pxcheck.c,v 1.3 2006/09/10 22:24:06 spale Exp $"

#define HTTP_RESPONSE   "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\n"
#define COMMON_RESPONSE "*** PXSCAN STRING: "

/* functions */
void chop(char *str);
uint16_t resolve_port(char *port);
uint32_t resolve_host(char *host);
char *build_sha256(char *secret);
void usage(char *prg);

/* print usage */
void usage(char *prg) {
	fprintf(stderr,"PX Target\n");
	fprintf(stderr,"usage: %s [-h] <-s string> <host:port> [host:port] ...\n",prg);
	fprintf(stderr,"-h : This page.\n");
	fprintf(stderr,"-s : Shared secret\n");
	exit(EXIT_FAILURE);
}

/* main */
int main(int argc, char *argv[]) {
	char *prg = *argv;
	char *secret = NULL;

	argc--;
	argv++;

	if ( !argc ) usage(prg);

	/* command line arguments parsing */
	while(argc) {
		if ( strcmp(*argv,"-s") == 0 ) {
			argc--;
			argv++;
			if ( !argc ) usage(prg);

			secret = *argv;
		}
		else if ( strcmp(*argv,"-h") == 0 )
			usage(prg);
		else
			break;

		argc--;
		argv++;
	}

	if ( !argc ) usage(prg);

	if ( secret == NULL ) {
		fprintf(stderr,"missing shared secret (-s)\n");
		usage(prg);
	}

	while(argc) {
		int s = -1;
		ssize_t len;
		struct sockaddr_in sockin;
		char buf[1024];
		char *strhost;
		char *strport;
		char *string;

		if ( strlen(*argv) >= sizeof(buf) ) {
			fprintf(stderr,"Invalid host:port '%s'\n",*argv);
			goto next;
		}

		strcpy(buf,*argv);

		if ( ( strhost = strtok(buf,":") ) == NULL ) {
			fprintf(stderr,"Invalid host:port '%s'\n",*argv);
			goto next;
		}

		if ( ( strport = strtok(NULL,"" ) ) == NULL ) {
			fprintf(stderr,"Invalid host:port '%s'\n",*argv);
			goto next;
		}

		sockin.sin_family = AF_INET;
		sockin.sin_addr.s_addr = resolve_host(strhost);
		sockin.sin_port = resolve_port(strport);

		printf("host=%s port=%u ip=%s ",strhost,
			ntohs(sockin.sin_port),inet_ntoa(sockin.sin_addr));

		if ( ( s = socket(PF_INET, SOCK_STREAM, 0) ) == -1 ) {
			printf("error=\"socket(%s)\"",strerror(errno));
			goto next;
		}

		if ( ( connect(s, (struct sockaddr*)&sockin, sizeof(struct sockaddr_in)) ) == -1 ) {
			printf("error=\"connect(%s)\"",strerror(errno));
			goto next;
		}

		if ( ( send(s, "\r\n", 2, 0) ) != 2 ) {
			printf("error=\"send(%s)\"",strerror(errno));
			goto next;
		}

		if ( ( len = recv(s, buf, sizeof(buf)-1, 0) ) < 1 ) {
			printf("error=\"recv(%s)\"",strerror(errno));
			goto next;
		}

		buf[len] = '\0';
		chop(buf);

		if ( strlen(buf) < sizeof(COMMON_RESPONSE) - 1 + 64 ) {
			printf("error=\"invalid_string(%s)\"",buf);
			goto next;
		}

		if ( memcmp(buf, COMMON_RESPONSE, sizeof(COMMON_RESPONSE)-1) != 0 ) {
			printf("error=\"invalid_string(%s)\"",buf);
			goto next;
		}

		string = build_sha256(secret);

		if ( memcmp(buf + sizeof(COMMON_RESPONSE) - 1, string, 64 ) != 0 ) {
			printf("error=\"sha256_mismatch(%s)\"",buf);
			goto next;
		}

		printf("error=none");

		next:

		printf("\n");

		if ( s > 0 ) close(s);

		argc--;
		argv++;

	}

	return 0;
}

uint16_t resolve_port(char *port) {
	return htons(strtoul(port, NULL, 0));
}

uint32_t resolve_host(char *host) {
	struct hostent *he;

	if ( ( he = gethostbyname(host) ) == NULL ) {
		fprintf(stderr,"gethostbyname('%s'): %s\n",
			host, hstrerror(h_errno));
		return INADDR_NONE;
	}

	return *(uint32_t*)he->h_addr;
}

char *build_sha256(char *secret) {
	static char buf[1024];
	unsigned char hash[32];
	sha256_context ctx;
	time_t ts = time(NULL);
	int i;

	sprintf(buf,"%li",(long int)ts - ( ts % 3600 ));

	sha256_starts(&ctx);
	sha256_update(&ctx, (uint8_t*)secret, strlen(secret));
	sha256_update(&ctx, (uint8_t*)buf, strlen(buf));
	sha256_finish(&ctx, hash);

	for(i=0; i<32; i++)
		sprintf(buf + (i*2),"%02X",hash[i]);

	strcpy(buf + 64,"\r\n");

	return buf;
}

void chop(char *str) {
	int len;

	if ( str == NULL )
		return;

	len = strlen(str);

	if ( str[len-2] == '\r' && str[len-1] == '\n' )
		str[len-2] = '\0';
	else if ( str[len-1] == '\n' )
		str[len-1] = '\0';
}
