/* Hacks for linux
 *
 * (c) 2004-2011 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * $Id: linux.h 395 2011-01-31 21:14:50Z hal9000 $
 *
 */

#include <dirent.h>
#include <grp.h>
#include <sys/param.h>
#include <sys/time.h>

#include <netdb.h>
#include <sys/mman.h>
#ifdef HAVE_NETINETIN
#include <netinet/in.h>
#endif
#include <arpa/inet.h>
#include <sys/socket.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif

#include <sys/uio.h>

#define READ_PERM 0664

#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_APPEND "a"

typedef int16_t int16;
typedef u_int16_t uint16;
typedef int32_t int32;
typedef u_int32_t uint32;
typedef int64_t int64;
typedef u_int64_t uint64;

