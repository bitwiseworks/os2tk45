/***************************************************************************/
/* IBM CONFIDENTIAL                                                        */
/* OCO Source Materials                                                    */
/* IBM TCP/IP for OS/2                                                     */
/* (c) Copyright IBM Corp 1992,1997                                        */
/* The source code for this program is not published or otherwise          */
/* divested of its trade secrets, irrespective of what has been            */
/* deposited with the U.S. Copyright Office                                */
/***************************************************************************/
#ifndef SYS_ITYPES_H
#define SYS_ITYPES_H

#include <types.h>
#include <nerrno.h>

/* The socket bit array is a one dimensional array of   */
/* longs which is used to represent 32K-1 sockets.      */
/* Each bit represents a socket and is used to indicate */
/* if the socket is in use (bit is 0) or can be used    */
/* (bit is 1).  The socket number start in element zero */
/* of the array with the least significant bit of the   */
/* long representing socket 0 and the most significant  */
/* bit being socket 31, element 1 represents sockets 32 */
/* through 63, and so on.                               */

#define BITSPERINT           32     /* bit per element                   */
#define MAX_ELEMENTS         1024   /* number of elements                */
#define START_ELEMENT_SMALL  0      /* start from array element 0        */
#define MAX_ELEMENTS_SMALL   64     /* end at element 63 (64 elements)   */
#define MAX_SOCKETS_SMALL     (MAX_ELEMENTS_SMALL*BITSPERINT)
#define START_ELEMENT_LARGE  64     /* start from array element 64       */
#define MAX_ELEMENTS_LARGE   960    /* end at element 1023 (1024 element */
#define MAX_SOCKETS_LARGE     (MAX_ELEMENTS_LARGE*BITSPERINT)
#undef  MAXSOCKETS
#define MAXSOCKETS           (MAX_SOCKETS_SMALL + MAX_SOCKETS_LARGE)
#define SMALLSOCK            0
#define LARGESOCK            1

#define S_ELEMENT(x) (x/BITSPERINT)
#define S_BIT(x)  (x%BITSPERINT)

/* set the x-th bit in the array a */
#define SET(a, x)  (a[S_ELEMENT(x)] |= (1 << S_BIT(x)))

/* clear the x-th bit in the array a */
#define CLR(a, x)  (a[S_ELEMENT(x)] &= ~(1 << S_BIT(x)))

/* determine if the x-th in array a is set */
#define ISSET(a, x)  (a[S_ELEMENT(x)] & (1 << S_BIT(x)))

/* A one-dimensional array is used as a hash table to      */
/* maintain a hashed linked list of the allocated sockets. */
/* This mechanism allows access of the socket via it's     */
/* designated socket number (i.e. the socket number can be */
/* hashed to obtain the associated hash bucket and then    */
/* the linked list of sockets can be searched to obtain    */
/* the desired socket - this implies you must provide both */
/* the socket number and a pointer to the socket).         */

#define MAX_BUCKETS    4096
#define S_BUCKET(x)    (x%MAX_BUCKETS)

typedef long daddr_t;
typedef unsigned (*FUNC)();

#ifndef __P
#define __P(args)  args
#endif
#ifdef VACPP
void * _Optlink memset (void *s, char c, size_t n);
void * _Optlink memmove(void *s, const void *ct, size_t n);
int    _Optlink memcmp (const void *cs, void *c, size_t n);
void * _Optlink memcpy (void *s, const void *ct, size_t n);
#endif

#if defined(__EXTENDED__)

  #ifdef _M_I386
   /* following code came from vacppos2\include\stdlib.h */
   /* they were originally inside #if __EXTENDED__ & _M_I386 */
   void  _Builtin __srotl(unsigned short, int);
   #define _srotl( x, y) __srotl( (x), (y) )
   void  _Builtin __lrotr(unsigned long, int);
   #define _lrotr( x, y) __lrotr( (x), (y) )
   void  _Builtin __lrotl(unsigned long, int);
   #define _lrotl( x, y) __lrotl( (x), (y) )
  #endif
#endif

#ifndef INLINE
#ifdef VACPP
unsigned long _Optlink _lswap(unsigned long l);
unsigned short _Optlink _bswap(unsigned short s);
#define bswap(s) (_srotl((s),8))
#define _bswap(s) (_srotl((s),8))
#define lswap(s) ( (_lrotr(s,8) & 0xff00ff00) | (_lrotl(s,8) & 0x00ff00ff) )
#define _lswap(s) ( (_lrotr(s,8) & 0xff00ff00) | (_lrotl(s,8) & 0x00ff00ff) )
#else
#define lswap(x)   ((x<<24)|(x>>24)|((x&0xff00)<<8)|((x&0xff0000)>>8))
#define bswap(x)   (((unsigned)x<<8)|((unsigned)x>>8))
#endif
#else
these can be more efficient...see inetntol.asm
unsigned long lswap(unsigned long);
#define in_lswap(x,y)   _asm { \
        _asm mov ax,(x) \
        _asm mov dx,(x)+2 \
        _asm xchg dh,al \
        _asm xchg dl,ah \
        _asm mov (y),ax \
        _asm mov (y)+2,dx \
        }
#define bswap(x)   (((unsigned)x<<8)|((unsigned)x>>8))
#define splis(x)   _asm { \
        _asm pushf \
        _asm pop ax \
        _asm and ax,0200h \
        _asm mov x,ax \
        }
#endif

#define htonl(x)   (_lswap(x))
#define ntohl(x)   (_lswap(x))
#define htons(x)   (_bswap(x))
#define ntohs(x)   (_bswap(x))

#define PZERO 0
#define TRUE 1
#define FALSE 0

#define NIL  ((char  *) 0)

#ifndef NULL
#define NULL ((void  *) 0)
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#define imin(x,y) MIN((x),(y))
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

#ifdef SPL16
   #define sploff( ) _asm {sti}
   #define splon( )  _asm {cli}
#else
#if defined(__EXTENDED__)

  #ifdef _M_I386
   /* following code came from vacppos2\include\stdlib.h */
   /* they were originally inside #if __EXTENDED__ & _M_I386 */
   void  _Builtin __enable( void );
   void  _Builtin __disable( void );

   #define sploff( )  __enable( )
   #define splon( ) __disable( )
  #endif
#endif
#endif /* endif SPL16 */

#define splx(x) sploff()
#define splimp() splon()
#define splnet() splon()
#ifndef splhigh
#define splhigh() 3
#endif
#ifndef splimp
#define splimp() 2
#endif
#ifndef splclock
#define splclock(x) 4
#endif


#define copyout(x,y,z) memcpy((y),(x),(z))

#define strcasecmp(x,y) strcmpi((x),(y))
#define strncasecmp(x,y,z) strnicmp(x,y,z)
#define ovbcopy(x,y,z) memmove((y),(x),(z))

#define timercmp(t1,t2,op) (((t1)->tv_sec op (t2)->tv_sec) || \
                           (((t1)->tv_sec == (t2)->tv_sec) \
                           && ((t1)->tv_usec op (t2)->tv_usec)))
//#define random() ((u_long)rand())
//#define srandom(x) srand(x)

typedef unsigned long uid_t;
typedef unsigned long gid_t;
//typedef char * va_list;


#define LAN_RCF_BROADALL        0x8000  /* all-routes broadcast */
#define LAN_RCF_BROADSINGLE     0xc000  /* single-route broadcast */

#ifndef __TCPPROTO
#define __TCPPROTO(args) args
#endif /* __TCPPROTO */


#ifdef KERNEL


#define index(s, c) strchr((s), (c))
#define bzero(p, l) memset((void *)(p),(int)0, (size_t)(l))
#define bcmp(s1, s2, l) memcmp((void *)(s1),(void *)(s2), (size_t)(l))
#define bcopy(f, t, l)  memcpy((void *)(t), (void *)(f), (size_t)(l))
#ifndef copyout
#define copyout(src,dst,len) memcpy((void *)(dst),(void *)(src),(size_t)(len))
#endif
#define copyin(src,dst,len)  memcpy((void *)(dst),(void *)(src),(size_t)(len))
#ifndef max
#define max(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a)<(b)) ? (a) : (b))
#endif
#ifndef imin
#define imin(a,b) (((a)<(b)) ? (a) : (b))
#endif
#ifndef strcasecmp
#define strcasecmp  strcmp
#endif
#define roundup(x, y)   ((((x)+((y)-1))/(y))*(y))
#define BMALLOC(ptr,cast,sz,x,y) ptr=(cast)get_cluster()
#define BFREE(ptr,x) free_cluster(ptr)
#define MALLOC(ppp,cast,sz,x,y) {if (sz<= MLEN) {struct mbuf *m_temp=m_get(y,x); if (m_temp) ppp=mtod(m_temp, cast); else ppp=0;} else panic("MALLOC");}
#define FREE(ptr,x) m_free(dtom(ptr))
//#define ntohl(lg) (((unsigned long)lg >> 24) | ((unsigned long)lg << 24) | \
//((lg << 8) & 0xFF0000) | ((lg >> 8) & 0xFF00))
//#define ntohs(sh) ((unsigned short)( ((unsigned short)sh >> 8) | \
//                                     ((unsigned short)sh << 8) ) )
#ifndef ntohl
#define ntohl(x)   swaplong((x))
#define ntohs(x)   swapshort((x))
#define htonl(x)   ntohl(x)
#define htons(x)   ntohs(x)
#define NTOHL(x)   x=ntohl(x);
#define NTOHS(x)   x=ntohs(x);
#define HTONL(x)   x=htonl(x);
#define HTONS(x)   x=htons(x);
#endif
#define panic(str) { \
                     extern short ComPort;   \
                     extern unsigned long  debug_flag; \
                     dprintf(ComPort,debug_flag,str,"\n"); \
                     int3break(); \
                   }
#define log(typ,msg) dprintf(ComPort,debug_flag,msg);
#define log1(typ,msg,p1) dprintf(ComPort,debug_flag,msg,p1);
#define log2(typ,msg,p1,p2) dprintf(ComPort,debug_flag,msg,p1,p2);
#define log3(typ,msg,p1,p2,p3) dprintf(ComPort,debug_flag,msg,p1,p2,p3);
#define SHRT_MAX  0x7FFF
#define LONG_MAX  0x7FFFFFFF /* perf:40587 --Vani*/

#ifdef DEKKO
#define TCPDEKKOB(a,b) DHOOKD0(F,4,0,a,b)
#else
#define TCPDEKKOB(a,b)
#endif
#ifdef DEKKO
#define TCPDEKKOE(a,b) DHOOKD0(F,4,80,a,b)
#else
#define TCPDEKKOE(a,b)
#endif

/* used by afinet */
extern struct timeval afinet_time;
extern struct timezone tz;                      /* XXX */
extern long tick;                       /* usec per tick (1000000 / hz) */
extern long hz;                         /* system clock's frequency */

#endif


#endif /* !SYS_ITYPES_H */















