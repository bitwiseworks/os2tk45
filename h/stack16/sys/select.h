#ifndef __SELECT_32H
#define __SELECT_32H

/*
** Check for BORLAND Compiler
*/
#ifdef __BORLANDC__       /* DRC01 - BORLAND */
#ifndef _System           /* DRC01 */
#define _System _syscall  /* DRC01 - redefine _System to _syscall */
#endif                    /* DRC01 */
#endif                    /* DRC01 */

#ifdef __HIGHC__          /* DRC02 - changed _System */
#define _System           /* DRC02 */
#endif                    /* DRC02 */


#define NBBY    8               /* number of bits in a byte */
#ifndef FD_SETSIZE
#define FD_SETSIZE      2048
#endif

typedef unsigned int    fd_mask;
#define NFDBITS (sizeof(fd_mask) * NBBY)        /* bits per mask */
#ifndef howmany
#define howmany(x, y)   (((x)+((y)-1))/(y))
#endif

typedef struct fd_set {
        fd_mask fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;


#define FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)      bzero((char *)(p), sizeof(*(p)))

#ifdef BSD_SELECT
#define select(a,b,c,d,e) bsdselect(a,b,c,d,e)
int _System bsdselect(int,
                       struct fd_set *,
                       struct fd_set *,
                       struct fd_set *,
                       struct timeval *);
#endif

#endif /* __SELECT_32H */

