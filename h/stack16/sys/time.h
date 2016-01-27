#ifndef _TIMEVAL
#define _TIMEVAL

#ifdef __BORLANDC__           /* DRC01 - changed _System to _syscall */
#ifndef _System               /* DRC01 */
#define _System _syscall      /* DRC01 */
#endif                        /* DRC01 */
#endif                        /* DRC01 */

#ifdef __HIGHC__              /* DRC02 - changed _System */
#define _System               /* DRC02 */
#endif                        /* DRC02 */

struct timeval {
      unsigned long  tv_sec;
      unsigned long  tv_usec;
};

struct timezone {
  unsigned short tmz;
};

struct itimerval {
      struct timeval it_interval;
      struct timeval it_value;
};

int _System gettimeofday (struct timeval *, struct timezone *);

#endif  /* _TIMEVAL */

