#ifndef _NERRNO_H_
#define _NERRNO_H_

#ifdef TCPV40HDRS
#include <stack16\nerrno.h>
#else
/*
 * The re-defination of error constants are necessary to avoid conflict with
 * ANSI standard error constants.
 *
 * All OS/2 SOCKET API error constants are biased by SOCBASEERR from the "normal"
 *
 */

#ifdef KERNEL
#define SOCBASEERR              0
#else
#define SOCBASEERR              10000
#endif

#define SOCEPERM                (SOCBASEERR+1)             /* Not owner */
#define SOCENOENT               (SOCBASEERR+2)             /* No such file or directory */
#define SOCESRCH                (SOCBASEERR+3)             /* No such process */
#define SOCEINTR                (SOCBASEERR+4)             /* Interrupted system call */
#define SOCEIO                  (SOCBASEERR+5)             /* Input/output error */
#define SOCENXIO                (SOCBASEERR+6)             /* No such device or address */
#define SOCE2BIG                (SOCBASEERR+7)             /* Argument list too long */
#define SOCENOEXEC              (SOCBASEERR+8)             /* Exec format error */
#define SOCEBADF                (SOCBASEERR+9)             /* Bad file number */
#define SOCECHILD               (SOCBASEERR+10)            /* No child processes */
#define SOCEDEADLK              (SOCBASEERR+11)            /* Resource deadlock avoided */
#define SOCENOMEM               (SOCBASEERR+12)            /* Cannot allocate memory */
#define SOCEACCES               (SOCBASEERR+13)            /* Permission denied */
#define SOCEFAULT               (SOCBASEERR+14)            /* Bad address */
#define SOCENOTBLK              (SOCBASEERR+15)            /* Block device required */
#define SOCEBUSY                (SOCBASEERR+16)            /* Device busy */
#define SOCEEXIST               (SOCBASEERR+17)            /* File exists */
#define SOCEXDEV                (SOCBASEERR+18)            /* Cross-device link */
#define SOCENODEV               (SOCBASEERR+19)            /* Operation not supported by device */
#define SOCENOTDIR              (SOCBASEERR+20)            /* Not a directory */
#define SOCEISDIR               (SOCBASEERR+21)            /* Is a directory */
#define SOCEINVAL               (SOCBASEERR+22)            /* Invalid argument */
#define SOCENFILE               (SOCBASEERR+23)            /* Too many open files in system*/
#define SOCEMFILE               (SOCBASEERR+24)            /* Too many open files */
#define SOCENOTTY               (SOCBASEERR+25)            /* Inappropriate ioctl for device */
#define SOCETXTBSY              (SOCBASEERR+26)            /* Text file busy */
#define SOCEFBIG                (SOCBASEERR+27)            /* File too large */
#define SOCENOSPC               (SOCBASEERR+28)            /* No space left on device */
#define SOCESPIPE               (SOCBASEERR+29)            /* Illegal seek */
#define SOCEROFS                (SOCBASEERR+30)            /* Read-only file system */
#define SOCEMLINK               (SOCBASEERR+31)            /* Too many links */
#define SOCEPIPE                (SOCBASEERR+32)            /* Broken pipe */

/* math software */
#define SOCEDOM                 (SOCBASEERR+33)            /* Numerical argument out of domain */
#define SOCERANGE               (SOCBASEERR+34)            /* Result too large */

/* non-blocking and interrupt i/o */
#define SOCEAGAIN               (SOCBASEERR+35)            /* Resource temporarily unavailable */
#define SOCEWOULDBLOCK          SOCEAGAIN                  /* Operation would block */
#define SOCEINPROGRESS          (SOCBASEERR+36)            /* Operation now in progress */
#define SOCEALREADY             (SOCBASEERR+37)            /* Operation already in progress */

/* ipc/network software -- argument errors */
#define SOCENOTSOCK             (SOCBASEERR+38)            /* Socket operation on non-socket */
#define SOCEDESTADDRREQ         (SOCBASEERR+39)            /* Destination address required */
#define SOCEMSGSIZE             (SOCBASEERR+40)            /* Message too long */
#define SOCEPROTOTYPE           (SOCBASEERR+41)            /* Protocol wrong type for socket */
#define SOCENOPROTOOPT          (SOCBASEERR+42)            /* Protocol not available */
#define SOCEPROTONOSUPPORT      (SOCBASEERR+43)            /* Protocol not supported */
#define SOCESOCKTNOSUPPORT      (SOCBASEERR+44)            /* Socket type not supported */
#define SOCEOPNOTSUPP           (SOCBASEERR+45)            /* Operation not supported */
#define SOCEPFNOSUPPORT         (SOCBASEERR+46)            /* Protocol family not supported */
#define SOCEAFNOSUPPORT         (SOCBASEERR+47)            /* Address family not supported by protocol family */
#define SOCEADDRINUSE           (SOCBASEERR+48)            /* Address already in use */
#define SOCEADDRNOTAVAIL        (SOCBASEERR+49)            /* Can't assign requested address */

/* ipc/network software -- operational errors */
#define SOCENETDOWN             (SOCBASEERR+50)            /* Network is down */
#define SOCENETUNREACH          (SOCBASEERR+51)            /* Network is unreachable */
#define SOCENETRESET            (SOCBASEERR+52)            /* Network dropped connection on reset */
#define SOCECONNABORTED         (SOCBASEERR+53)            /* Software caused connection abort */
#define SOCECONNRESET           (SOCBASEERR+54)            /* Connection reset by peer */
#define SOCENOBUFS              (SOCBASEERR+55)            /* No buffer space available */
#define SOCEISCONN              (SOCBASEERR+56)            /* Socket is already connected */
#define SOCENOTCONN             (SOCBASEERR+57)            /* Socket is not connected */
#define SOCESHUTDOWN            (SOCBASEERR+58)            /* Can't send after socket shutdown */
#define SOCETOOMANYREFS         (SOCBASEERR+59)            /* Too many references: can't splice */
#define SOCETIMEDOUT            (SOCBASEERR+60)            /* Operation timed out */
#define SOCECONNREFUSED         (SOCBASEERR+61)            /* Connection refused */

#define SOCELOOP                (SOCBASEERR+62)            /* Too many levels of symbolic links */
#define SOCENAMETOOLONG         (SOCBASEERR+63)            /* File name too long */

/* should be rearranged */
#define SOCEHOSTDOWN            (SOCBASEERR+64)             /* Host is down */
#define SOCEHOSTUNREACH         (SOCBASEERR+65)             /* No route to host */
#define SOCENOTEMPTY            (SOCBASEERR+66)             /* Directory not empty */

/* quotas & mush */
#define SOCEPROCLIM             (SOCBASEERR+67)             /* Too many processes */
#define SOCEUSERS               (SOCBASEERR+68)             /* Too many users */
#define SOCEDQUOT               (SOCBASEERR+69)             /* Disc quota exceeded */

/* Network File System */
#define SOCESTALE               (SOCBASEERR+70)             /* Stale NFS file handle */
#define SOCEREMOTE              (SOCBASEERR+71)             /* Too many levels of remote in path */
#define SOCEBADRPC              (SOCBASEERR+72)             /* RPC struct is bad */
#define SOCERPCMISMATCH         (SOCBASEERR+73)             /* RPC version wrong */
#define SOCEPROGUNAVAIL         (SOCBASEERR+74)             /* RPC prog. not avail */
#define SOCEPROGMISMATCH        (SOCBASEERR+75)             /* Program version wrong */
#define SOCEPROCUNAVAIL         (SOCBASEERR+76)             /* Bad procedure for program */

#define SOCENOLCK               (SOCBASEERR+77)             /* No locks available */
#define SOCENOSYS               (SOCBASEERR+78)             /* Function not implemented */

#define SOCEFTYPE               (SOCBASEERR+79)             /* Inappropriate file type or format */
#define SOCEAUTH                (SOCBASEERR+80)             /* Authentication error */
#define SOCENEEDAUTH            (SOCBASEERR+81)             /* Need authenticator */

#define SOCEOS2ERR              (SOCBASEERR+100)            /* OS/2 Error */
#define SOCELAST                (SOCBASEERR+100)            /* Must be equal largest errno */

/*
 * OS/2 SOCKET API errors redefined as regular BSD error constants
 */

#ifndef ENOENT
#define ENOENT                  SOCENOENT
#endif

#ifndef EFAULT
#define EFAULT                  SOCEFAULT
#endif

#ifndef EBUSY
#define EBUSY                   SOCEBUSY
#endif

#ifndef ENXIO
#define ENXIO                   SOCENXIO
#endif

#ifndef EACCES
#define EACCES                  SOCEACCES
#endif

#ifndef ENOMEM
#define ENOMEM                  SOCENOMEM
#endif

#ifndef ENOTDIR
#define ENOTDIR                 SOCENOTDIR
#endif

#ifndef EPERM
#define EPERM                   SOCEPERM
#endif

#ifndef ESRCH
#define ESRCH                   SOCESRCH
#endif

#ifndef EDQUOT
#define EDQUOT                  SOCEDQUOT
#endif

#ifndef EEXIST
#define EEXIST                  SOCEEXIST
#endif

#ifndef EBUSY
#define EBUSY                   SOCEBUSY
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK             SOCEWOULDBLOCK
#endif

#ifndef EINPROGRESS
#define EINPROGRESS             SOCEINPROGRESS
#endif

#ifndef EALREADY
#define EALREADY                SOCEALREADY
#endif

#ifndef ENOTSOCK
#define ENOTSOCK                SOCENOTSOCK
#endif

#ifndef EDESTADDRREQ
#define EDESTADDRREQ            SOCEDESTADDRREQ
#endif

#ifndef EMSGSIZE
#define EMSGSIZE                SOCEMSGSIZE
#endif

#ifndef EPROTOTYPE
#define EPROTOTYPE              SOCEPROTOTYPE
#endif

#ifndef ENOPROTOOPT
#define ENOPROTOOPT             SOCENOPROTOOPT
#endif

#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT         SOCEPROTONOSUPPORT
#endif

#ifndef ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT         SOCESOCKTNOSUPPORT
#endif

#ifndef EOPNOTSUPP
#define EOPNOTSUPP              SOCEOPNOTSUPP
#endif

#ifndef EPFNOSUPPORT
#define EPFNOSUPPORT            SOCEPFNOSUPPORT
#endif

#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT            SOCEAFNOSUPPORT
#endif

#ifndef EADDRINUSE
#define EADDRINUSE              SOCEADDRINUSE
#endif

#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL           SOCEADDRNOTAVAIL
#endif

#ifndef ENETDOWN
#define ENETDOWN                SOCENETDOWN
#endif

#ifndef ENETUNREACH
#define ENETUNREACH             SOCENETUNREACH
#endif

#ifndef ENETRESET
#define ENETRESET               SOCENETRESET
#endif

#ifndef ECONNABORTED
#define ECONNABORTED            SOCECONNABORTED
#endif

#ifndef ECONNRESET
#define ECONNRESET              SOCECONNRESET
#endif

#ifndef ENOBUFS
#define ENOBUFS                 SOCENOBUFS
#endif

#ifndef EISCONN
#define EISCONN                 SOCEISCONN
#endif

#ifndef ENOTCONN
#define ENOTCONN                SOCENOTCONN
#endif

#ifndef ESHUTDOWN
#define ESHUTDOWN               SOCESHUTDOWN
#endif

#ifndef ETOOMANYREFS
#define ETOOMANYREFS            SOCETOOMANYREFS
#endif

#ifndef ETIMEDOUT
#define ETIMEDOUT               SOCETIMEDOUT
#endif

#ifndef ECONNREFUSED
#define ECONNREFUSED            SOCECONNREFUSED
#endif

#ifndef ELOOP
#define ELOOP                   SOCELOOP
#endif

#ifndef ENAMETOOLONG            /* Borland and Watcom define this */
#define ENAMETOOLONG            SOCENAMETOOLONG
#endif

#ifndef EHOSTDOWN
#define EHOSTDOWN               SOCEHOSTDOWN
#endif

#ifndef EHOSTUNREACH
#define EHOSTUNREACH            SOCEHOSTUNREACH
#endif

#ifndef ENOTEMPTY               /* Watcom defines this */
#define ENOTEMPTY               SOCENOTEMPTY
#endif

#ifndef EINVAL
#define EINVAL                  SOCEINVAL
#endif

#ifndef EINTR
#define EINTR                   SOCEINTR
#endif

#ifndef EMFILE
#define EMFILE                  SOCEMFILE
#endif

#ifndef EPIPE
#define EPIPE                   SOCEPIPE
#endif

#endif /* TCPV40HDRS */
#endif  /* _NERRNO_H_ */
