#ifndef __UTILS_32H
#define __UTILS_32H

#ifdef __BORLANDC__         /* DRC01 - changed _System to _syscall */
#ifndef _System             /* DRC01 */
#define _System _syscall    /* DRC01 */
#endif                      /* DRC01 */
#endif                      /* DRC01 */

#ifdef __HIGHC__            /* DRC04 - changed _System */
#define _System             /* DRC04 */
#endif                      /* DRC04 */


unsigned long _System lswap(unsigned long);
unsigned short _System bswap(unsigned short);
int _System rexec(char **, int, char *, char *, char *, int *);
/* int _System getpid(void); */

/* Definition for bswap */
#define htonl(x)   (lswap(x))
#define ntohl(x)   (lswap(x))
#define htons(x)   (bswap(x))
#define ntohs(x)   (bswap(x))

#ifndef X11
#define bzero(x,y) memset((x),'\0',(y))
#define bcopy(x,y,z) memcpy((y),(x),(z))
#define bcmp(x,y,z)  memcmp((y),(x),(z))
#endif /* X11 */
#define ovbcopy(x,y,z) bcopy((x),(y),(z))
#define copyout(x,y,z) memcpy((y),(x),(z))
#define strcasecmp(x,y) strcmpi((x),(y))
#define strncasecmp(x,y,z) strnicmp(x,y,z)
#define sleep(x) DosSleep(((long)(x))*1000L)

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#define imin(x,y) MIN((x),(y))
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

#define timercmp(t1,t2,op) (((t1)->tv_sec op (t2)->tv_sec) || \
                           (((t1)->tv_sec == (t2)->tv_sec) \
                           && ((t1)->tv_usec op (t2)->tv_usec)))

#ifndef __BORLANDC__         /* DRC03 - if using BORLAND then use */
                             /* DRC03 - its random function */
#define random() ((unsigned long)rand())
#endif

#define srandom(x) srand(x)

#endif /* __UTILS_32H */

