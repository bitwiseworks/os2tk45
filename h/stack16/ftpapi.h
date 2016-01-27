#ifndef __FTPAPI_32H
#define __FTPAPI_32H

#define INCL_WINMESSAGEMGR
#include <os2.h>

/**************************/
/* DRC01 - Define _System */

#ifdef __BORLANDC__
#define _System _syscall
#endif

#ifdef __HIGHC__
#define _System
#endif

/* window message id for post xfer updates */
#define WM_FTPAPI_XFER_UPDATE    WM_USER + 1000

#define T_ASCII      1
#define T_EBCDIC     2
#define T_BINARY     3

/* command/reply trace file modes */
#define M_OVERLAY    1
#define M_APPEND     2

/* command/reply tracing error codes */
#define TRCMODE      1     /* invalid trace file open mode */
#define TRCOPEN      2     /* unable to open trace file */

extern int ftperrno;
#define FTPSERVICE    1    /* ftp: ftp/tcp: unknown service */
#define FTPHOST       2    /* unknown host */
#define FTPSOCKET     3    /* unable to obtain socket */
#define FTPCONNECT    4    /* unable to connect to server */
#define FTPLOGIN      5    /* login failed */
#define FTPABORT      6    /* transfer aborted */
#define FTPLOCALFILE  7    /* problem openning local file */
#define FTPDATACONN   8    /* problem initializing data connection */
#define FTPCOMMAND    9    /* command failed */
#define FTPPROXYTHIRD 10   /* proxy server does not support third party transfers */
#define FTPNOPRIMARY  11   /* No primary connection for proxy transfer */
#define FTPNOXLATETBL 12   /* No code page translation table was loded */

/* ping error codes */
#define PINGREPLY     -1   /* host does not reply */
#define PINGSOCKET    -3   /* unable to obtain socket */
#define PINGPROTO     -4   /* unknown protcol ICMP */
#define PINGSEND      -5   /* send failed */
#define PINGRECV      -6   /* recv failed */
#define PINGHOST      -7   /* can't resolve the host name */

void _System ftplogoff(void);
int _System ftpget(char *, char *, char *, char *, char *, char *, char *, int);
int _System ftpput(char *, char *, char *, char *, char *, char *, int);
int _System ftpappend(char *, char *, char *, char *, char *, char *, int);
int _System ftpputunique(char *, char *, char *, char *, char *, char *, int);
int _System ftpcd(char *, char *, char *, char *, char *);
int _System ftpmkd(char *, char *, char *, char *, char *);
int _System ftprmd(char *, char *, char *, char *, char *);
int _System ftpdelete(char *, char *, char *, char *, char *);
int _System ftprename(char *, char *, char *, char *, char *, char *);
int _System ftpls(char *, char *, char *, char *, char *, char *);
int _System ftpdir(char *, char *, char *, char *, char *, char *);
int _System ftpproxy(char *, char *, char *, char *, char *, char *, char *, char *, char *, char *, int);
int _System ftpsite(char *, char *, char *, char *, char *);
int _System ftpquote(char *, char *, char *, char *, char *);
int _System ftpping(char *, int, unsigned long *);
int _System ftppwd(char *, char *, char *, char *, char *, int);
int _System ftpsys(char *, char *, char *, char *, char *, int);
int _System ping(unsigned long, int);
int _System ftp_errno(void);
int _System ftpxlate(unsigned long, char *);
int _System ftpver(char *, int);
int _System ftptrcon(char *, int);
int _System ftptrcoff(void);
void _System ftpxferwnd(HWND hwnd);
#endif /* __FTPAPI_32H */

