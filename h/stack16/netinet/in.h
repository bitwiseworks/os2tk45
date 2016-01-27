#ifndef __IN_32H
#define __IN_32H
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *      @(#)in.h        7.5 (Berkeley) 2/22/88
 */

/*
 * Constants and structures defined by the internet system,
 * Per RFC 790, September 1981.
 */

#ifdef __BORLANDC__           /* DRC01 - changed _System to _syscall */
#ifndef _System               /* DRC01 */
#define _System _syscall      /* DRC01 */
#endif                        /* DRC01 */
#endif                        /* DRC01 */

#ifdef __HIGHC__              /* DRC02 */
#define _System
#endif

/*
 * Protocols
 */
#define IPPROTO_IP              0               /* dummy for IP */
#define IPPROTO_ICMP            1               /* control message protocol */
#define IPPROTO_IGMP            2               /* group management protocol*/
#define IPPROTO_GGP             3               /* gateway^2 (deprecated) */
#define IPPROTO_TCP             6               /* tcp */
#define IPPROTO_EGP             8               /* exterior gateway protocol */
#define IPPROTO_PUP             12              /* pup */
#define IPPROTO_UDP             17              /* user datagram protocol */
#define IPPROTO_IDP             22              /* xns idp */

#define IPPROTO_RAW             255             /* raw IP packet */
#define IPPROTO_MAX             256


/*
 * Ports < IPPORT_RESERVED are reserved for
 * privileged processes (e.g. root).
 * Ports > IPPORT_USERRESERVED are reserved
 * for servers, not necessarily privileged.
 */
#define IPPORT_RESERVED         1024
#define IPPORT_USERRESERVED     5000

/*
 * Link numbers
 */
#define IMPLINK_IP              155
#define IMPLINK_LOWEXPER        156
#define IMPLINK_HIGHEXPER       158

/*
 * Internet address (a structure for historical reasons)
 */
struct in_addr {
        unsigned long s_addr;
};

/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define IN_CLASSA(i)            (((long)(i) & 0x80000000L) == 0)
#define IN_CLASSA_NET           0xff000000L
#define IN_CLASSA_NSHIFT        24
#define IN_CLASSA_HOST          0x00ffffffL
#define IN_CLASSA_MAX           128

#define IN_CLASSB(i)            (((long)(i) & 0xc0000000L) == 0x80000000L)
#define IN_CLASSB_NET           0xffff0000L
#define IN_CLASSB_NSHIFT        16
#define IN_CLASSB_HOST          0x0000ffffL
#define IN_CLASSB_MAX           65536L

#define IN_CLASSC(i)            (((long)(i) & 0xe0000000L) == 0xc0000000L)
#define IN_CLASSC_NET           0xffffff00L
#define IN_CLASSC_NSHIFT        8
#define IN_CLASSC_HOST          0x000000ffL

#define IN_CLASSD(i)            (((long)(i) & 0xf0000000L) == 0xe0000000L)
#define IN_CLASSD_NET           0xffffffffL
#define IN_CLASSD_HOST          0x00000000L
#define IN_MULTICAST(i)         IN_CLASSD(i)

#define IN_EXPERIMENTAL(i)      (((long)(i) & 0xe0000000L) == 0xe0000000L)
#define IN_BADCLASS(i)          (((long)(i) & 0xf0000000L) == 0xf0000000L)

#define INADDR_ANY              (unsigned long)0x00000000L
#define INADDR_BROADCAST        (unsigned long)0xffffffffL     /* must be masked */

#define INADDR_UNSPEC_GROUP     (unsigned long )0xe0000000      /* 224.0.0.0   */
#define INADDR_ALLHOSTS_GROUP   (unsigned long)0xe0000001      /* 224.0.0.1   */
#define INADDR_MAX_LOCAL_GROUP  (unsigned long)0xe00000ff      /* 224.0.0.255 */

#ifndef KERNEL
#define INADDR_NONE             0xffffffffL             /* -1 return */
#endif

#define IN_LOOPBACKNET          127                     /* official! */

/*
 * Socket address, internet style.
 */

struct sockaddr_in {
        short   sin_family;
        unsigned short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
};

/*
 * Options for use with [gs]etsockopt at the IP level.
 */
#define IP_OPTIONS      1               /* set/get IP per-packet options */

#define IP_MULTICAST_IF    2            /* set/get IP multicast interface*/
#define IP_MULTICAST_TTL   3            /* set/get IP multicast timetolive*/
#define IP_MULTICAST_LOOP  4            /* set/get IP multicast loopback */
#define IP_ADD_MEMBERSHIP  5            /* add  an IP group membership   */
#define IP_DROP_MEMBERSHIP 6            /* drop an IP group membership   */

#define IP_DEFAULT_MULTICAST_TTL  1     /* normally limit m'casts to 1 hop */
#define IP_DEFAULT_MULTICAST_LOOP 1     /* normally hear sends if a member */
#define IP_MAX_MEMBERSHIPS       20     /* per socket; must fit in one mbuf*/

/*
 * Argument structure for IP_ADD_MEMBERSHIP and IP_DROP_MEMBERSHIP.
 */
struct ip_mreq {
        struct in_addr  imr_multiaddr;  /* IP multicast address of group */
        struct in_addr  imr_interface;  /* local IP address of interface */
};

unsigned long _System inet_addr(char *);
struct in_addr _System inet_makeaddr(unsigned long, unsigned long);
unsigned long _System inet_network(char *);
char * _System inet_ntoa(struct in_addr);
unsigned long _System inet_lnaof(struct in_addr);
unsigned long _System inet_netof(struct in_addr);

#endif /* __IN_32H */

