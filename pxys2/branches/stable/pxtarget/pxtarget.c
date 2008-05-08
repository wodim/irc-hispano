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

#define RCSID "$Id: pxtarget.c,v 1.15 2006/09/10 22:24:06 spale Exp $"


#define HTTP_RESPONSE   "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\n"
#define COMMON_RESPONSE "*** PXSCAN STRING: "
#define DEFAULT_IP      "0.0.0.0"
#define DEFAULT_PORT    9000
#define DEFAULT_MAX_CLI 20
#define ABS_MAX_CLI     1000

/* globas variables */
char *logfile = NULL;
int debug = 0;


/* functions */
char *build_sha256(char *secret);
void mylog(char *fmt, ...);
void daemonise(void);
void usage(char *prg);
void run_client(int cli, struct sockaddr_in *sockcli, char *secret);
int  run_core(uint32_t ip, uint16_t port, char *str, unsigned int max_cli);

/* print usage */
void usage(char *prg) {
	fprintf(stderr,"PX Target\n");
	fprintf(stderr,"usage: %s [-h] [-d] [-m num] [-i ip] [-p port] <-s string> [-l logfile]\n",prg);
	fprintf(stderr,"-h : This page.\n");
	fprintf(stderr,"-d : Debug and stay in foreground\n");
	fprintf(stderr,"-s : Shared secret (default: NONE)\n");
	fprintf(stderr,"-i : Local IP Address (default: %s)\n",DEFAULT_IP);
	fprintf(stderr,"-p : Local Port (default: %i)\n",DEFAULT_PORT);
	fprintf(stderr,"-m : Max clients (default: %i min:1 max:%i)\n",DEFAULT_MAX_CLI, ABS_MAX_CLI);
	fprintf(stderr,"-l : Logfile (default: NONE)\n");
	exit(EXIT_FAILURE);
}

/* main */
int main(int argc, char *argv[]) {
	char *prg = *argv;
	uint32_t listen_ip = inet_addr(DEFAULT_IP);
	uint16_t listen_port = htons(DEFAULT_PORT);
	char *secret = NULL;
	unsigned int max_cli = DEFAULT_MAX_CLI;

	argc--;
	argv++;

	if ( !argc ) usage(prg);

	/* command line arguments parsing */
	while(argc) {
		if ( strcmp(*argv,"-d") == 0 ) {
			debug = 1;
		}
		else if ( strcmp(*argv,"-i") == 0 ) {
			argc--;
			argv++;
			if ( !argc ) usage(prg);

			listen_ip = inet_addr(*argv);
			if ( listen_ip == INADDR_NONE ) {
				fprintf(stderr,"-i invalid value.\n");
				usage(prg);
			}
		}
		else if ( strcmp(*argv,"-l") == 0 ) {
			argc--;
			argv++;
			if ( !argc ) usage(prg);

			logfile = *argv;
		}
		else if ( strcmp(*argv,"-p") == 0 ) {
			argc--;
			argv++;
			if ( !argc ) usage(prg);

			listen_port = htons(strtoul(*argv, NULL, 0));
			if ( listen_port < 1 ) {
				fprintf(stderr,"-p invalid value.\n");
				usage(prg);
			}
		}
		else if ( strcmp(*argv,"-m") == 0 ) {
			argc--;
			argv++;
			if ( !argc ) usage(prg);

			max_cli = strtoul(*argv, NULL, 0);

			if ( max_cli < 1 || max_cli > ABS_MAX_CLI ) {
				fprintf(stderr,"-m invalid value.\n");
				usage(prg);
			}
		}
		else if ( strcmp(*argv,"-s") == 0 ) {
			argc--;
			argv++;
			if ( !argc ) usage(prg);

			secret = *argv;
		}
		else if ( strcmp(*argv,"-h") == 0 )
			usage(prg);
		else
			usage(prg);

		argc--;
		argv++;
	}

	if ( argc ) usage(prg);

	if ( secret == NULL ) {
		fprintf(stderr,"missing shared secret (-s)\n");
		usage(prg);
	}

	if ( debug ) mylog("debug mode enabled.");

	run_core(listen_ip,listen_port,secret,max_cli);

	return 0;
}

int run_core(uint32_t ip, uint16_t port, char *secret, unsigned int max_cli) {
	int s;
	unsigned int run_cli = 0;
	struct sockaddr_in sockin;

	sockin.sin_family = AF_INET;
	sockin.sin_port   = port;
	sockin.sin_addr.s_addr = ip;

	if ( ( s = socket(PF_INET, SOCK_STREAM, 0) ) == -1 ) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if ( fcntl(s, F_SETFL, O_NONBLOCK) == -1 ) {
		perror("fcntl");
		exit(EXIT_FAILURE);
	}

	if ( bind(s, (struct sockaddr*)&sockin, sizeof(struct sockaddr_in) ) == -1 ) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if ( listen(s,10) == -1 ) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	if ( !debug )
		daemonise();

	while(1) {
		struct sockaddr_in sockcli;
		socklen_t socklen = sizeof(struct sockaddr_in);
		int cli;

		if((cli=accept(s, (struct sockaddr*)&sockcli, &socklen))>0) {
			if ( run_cli >= max_cli ) {
				mylog("ip=%s port=%i status=\"max clients\"",
					inet_ntoa(sockcli.sin_addr),
					ntohs(sockcli.sin_port));
				close(cli);
				continue;
			}

			switch(fork()) {
				case -1:
					mylog("ip=%s port=%i status=\"fork '%s'\"",
						inet_ntoa(sockcli.sin_addr),
						ntohs(sockcli.sin_port),
						strerror(errno));
					break;
				case 0:
					run_client(cli, &sockcli, secret);
					exit(EXIT_SUCCESS);
				default:
					run_cli++;
					break;
			}
			close(cli);
		}
		else
			usleep(100000);

		if ( run_cli > 0 ) {
			int status;
			if ( waitpid(-1, &status, WNOHANG) > 0 )
				run_cli--;
		}
	}

	close(s);

	return 0;
}

void run_client(int s, struct sockaddr_in *sockin, char *secret) {
	fd_set fdset;
	struct timeval to;
	char buf[8192];
	char *string = NULL;
	char *cli_type = "undefined";
	char *cli_status = "undefined";
	int is_http = 0;

	FD_ZERO(&fdset);
	FD_SET(s,&fdset);

	to.tv_sec  = 1;
	to.tv_usec = 0;

	if ( fcntl(s, F_SETFL, O_NONBLOCK) == -1 ) {
		cli_status = "setsockopt_error";
		goto client_end;
	}


	if ( select(s+1, &fdset, NULL, NULL, &to) == 1 ) {
		ssize_t len;
		char sbuf[1024];
		if ( ( len = recv(s, sbuf, sizeof(sbuf)-1, 0 ) ) > 0 ) {
			sbuf[len] = '\0';

			if ( memcmp(sbuf,"GET ",  sizeof("GET ")-1)  == 0 ) {
				cli_type = "http_get";
				is_http = 1;
			}
			else if ( memcmp(sbuf,"POST ", sizeof("POST ")-1) == 0 ) {
				cli_type = "http_post";
				is_http = 1;
			}
			else {
				cli_type = "undefined";
			}
		}
		else {
			cli_type = "no_cli_data";
		}
	}
	else
		cli_type = "no_cli_data";

	buf[0] = '\0';

	if ( is_http ) strcpy(buf,HTTP_RESPONSE);

	strcpy(buf + strlen(buf), COMMON_RESPONSE);

	string = build_sha256(secret);

	strcpy(buf + strlen(buf), string);

	if ( send(s,buf, strlen(buf), 0) != strlen(buf) ) {
		cli_status = "write_error";
		goto client_end;
	}

	cli_status = "ok";

	sleep(1);


	client_end:

	mylog("ip=%s port=%u type=%s status=%s",
		inet_ntoa(sockin->sin_addr),
		ntohs(sockin->sin_port),
		cli_type, cli_status );

	close(s);
}

void mylog(char *fmt, ...) {
	FILE *out = NULL;
	va_list ap;
	time_t ts = time(NULL);
	char *tstr = ctime(&ts);

	if ( tstr != NULL )
		tstr[strlen(tstr)-1] = '\0';

	if ( logfile != NULL )
		out = fopen(logfile,"a+");

	if ( out == NULL )
		out = stdout;

	va_start(ap, fmt);
	fprintf(out,"[%s] ",tstr);
	vfprintf(out,fmt,ap);
	fprintf(out,"\n");
	va_end(ap);

	if ( out != stdout )
		fclose(out);
}

void daemonise(void) {

	switch(fork()) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			break;
		default:
			exit(EXIT_SUCCESS);
	}

	setsid();
	freopen("/dev/null", "r+", stdin);
	freopen("/dev/null", "r+", stdout);
	freopen("/dev/null", "r+", stderr);
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
