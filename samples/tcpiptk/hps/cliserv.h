/* Common includes and defines for UDP, TCP, and T/TCP
 * clients and servers */

#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <types.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <net/route.h>
#include <net/if.h>
//#include <net/if_dl.h>
//#include <net/if_types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/uio.h>
#include <netdb.h>

#define REQUEST 400                             /* max size of request, in bytes */
#define REPLY   400                             /* max size of reply, in bytes */

#define UDP_SERV_PORT   7777    /* UDP server's well-known port */
#define TCP_SERV_PORT   8888    /* TCP server's well-known port */
#define TTCP_SERV_PORT  9999    /* T/TCP server's well-known port */

/* Added ... */
#define NUMHPSBLOCKS 15
#define HPSBLOCKSIZE 4096
#define ITERATIONS   1024
#define SERV_CHAR    'a'
#define CLI_CHAR     'A'
#define BUFCNT        4

/* Following shortens all the type casts of pointer arguments */
#define SA      struct sockaddr *

void    err_quit(const char *, ...);
void    err_sys(const char *, ...);
int     read_stream(int, char *, int);

/* following for timing versions of client-server */
void    start_timer(void);
double  print_timer(void);
void    sleep_us(unsigned int);

#define min(a,b)        ((a) < (b) ? (a) : (b))

#define close(x) soclose(x)
