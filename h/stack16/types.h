#ifndef __TYPES_32H
#define __TYPES_32H

#define INET
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned short ushort;
typedef unsigned int u_int;
typedef unsigned char u_char;

#ifndef __off_t
typedef long off_t;
#define __off_t
#endif /* off_t */

typedef char *caddr_t;
typedef long daddr_t;
#define NIL ((char *) 0)

#define PZERO 0
#define BSD 43
#include <errno.h>
#include <nerrno.h>
#include <sys/time.h>
#include <utils.h>

#define MAXHOSTNAMELEN 120
#ifndef MAXPATHLEN
#define MAXPATHLEN 80
#endif
#define MAXSOCKETS 2048

#ifndef X11     /* defined by Xos.h */
#define index(string,c) strchr((string),(c))
#endif

#define TRUE 1
#define FALSE 0
#define SIGALRM 0

#endif /* __TYPES_32H */
