/*
 * Copyright (c) 1980, 1986, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *      @(#)route.h     8.5 (Berkeley) 2/8/95
 */

/*
 * Kernel resident routing tables.
 *
 * The routing tables are initialized when interface addresses
 * are set by making entries for all directly connected interfaces.
 */
#ifndef _NET_ROUTE_H_
#define _NET_ROUTE_H_

#ifdef TCPV40HDRS
#include <stack16\net\route.h>
#else
/*
 * These numbers are used by reliable protocols for determining
 * retransmission behavior and are included in the routing structure.
 */
struct rt_metrics {
        u_long  rmx_locks;      /* Kernel must leave these values alone */
        u_long  rmx_mtu;        /* MTU for this path */
        u_long  rmx_hopcount;   /* max hops expected */
        u_long  rmx_expire;     /* lifetime for route, e.g. redirect */
        u_long  rmx_recvpipe;   /* inbound delay-bandwith product */
        u_long  rmx_sendpipe;   /* outbound delay-bandwith product */
        u_long  rmx_ssthresh;   /* outbound gateway buffer limit */
        u_long  rmx_rtt;        /* estimated round trip time */
        u_long  rmx_rttvar;     /* estimated rtt variance */
        u_long  rmx_pksent;     /* packets sent using this route */
        u_long  rmx_filler[4];  /* will be used for T/TCP later */
};

/*
 * rmx_rtt and rmx_rttvar are stored as microseconds;
 * RTTTOPRHZ(rtt) converts to a value suitable for use
 * by a protocol slowtimo counter.
 */
#define RTM_RTTUNIT     1000000 /* units for rtt, rttvar, as units per sec */
#define RTTTOPRHZ(r)    ((r) / (RTM_RTTUNIT / PR_SLOWHZ))

/*
 * Following structure necessary for 4.3 compatibility;
 * We should eventually move it to a compat file.
 */
#pragma pack(1)
struct ortentry {
        u_long  rt_hash;                /* to speed lookups */
        struct  sockaddr rt_dst;        /* key */
        struct  sockaddr rt_gateway;    /* value */
        short   rt_flags;               /* up/down?, host/net */
        short   rt_refcnt;              /* # held references */
        u_long  rt_use;                 /* raw # packets forwarded */
        void    *rt_ifp;                /* the answer: interface to use */
        long  metric1;                  /* routing metrics for SNMP and Co */
        long  metric2;
        long  metric3;
        long  metric4;
};
#pragma pack()

#define RTENTRY_COUNT 512
#pragma pack(1)
struct rtentries {
       short hostcount;
       short netcount;
       struct ortentry rttable[RTENTRY_COUNT];
};
#pragma pack()


#define RTF_UP          0x1             /* route usable */
#define RTF_GATEWAY     0x2             /* destination is a gateway */
#define RTF_HOST        0x4             /* host entry (net otherwise) */
#define RTF_REJECT      0x8             /* host or net unreachable */
#define RTF_DYNAMIC     0x10            /* created dynamically (by redirect) */
#define RTF_MODIFIED    0x20            /* modified dynamically (by redirect) */
#define RTF_DONE        0x40            /* message confirmed */
#define RTF_MASK        0x80            /* subnet mask present */
#define RTF_CLONING     0x100           /* generate new routes on use */
#define RTF_XRESOLVE    0x200           /* external daemon resolves name */
#define RTF_LLINFO      0x400           /* generated by ARP or ESIS */
#define RTF_STATIC      0x800           /* manually added */
#define RTF_BLACKHOLE   0x1000          /* just discard pkts(during updates) */
#define RTF_LLTEMP      0x2000          /* manually added temporary arp entry */
#define RTF_PROTO2      0x4000          /* protocol specific routing flag */
#define RTF_PROTO1      0x8000          /* protocol specific routing flag */
#define RTF_PRCLONING   0x10000         /* protocol requires cloning */
#define RTF_WASCLONED   0x20000         /* route generated through cloning */
#define RTF_PROTO3      0x40000         /* protocol specific routing flag */
#define RTF_CHAINDELETE 0x80000         /* chain is being deleted (internal) */
#define RTF_PINNED      0x100000        /* future use */
#define RTF_TUNNEL      0x200000        /* tunnelling bit */
#define RTF_CRYPT       0x400000        /* encrypting tunnel */
#define RTF_AUTH        0x800000        /* authenticating tunnel */


/*
 * Routing statistics.
 */
struct  rtstat {
        short   rts_badredirect;        /* bogus redirect calls */
        short   rts_dynamic;            /* routes created by redirects */
        short   rts_newgateway;         /* routes modified by redirects */
        short   rts_unreach;            /* lookups which failed */
        short   rts_wildcard;           /* lookups satisfied by a wildcard */
};
/*
 * Structures for routing messages.
 */
#pragma pack(1)
struct rt_msghdr {
#define IFNAMSIZ        16
        u_short rtm_msglen;     /* to skip over non-understood messages */
        u_char  rtm_version;    /* future binary compatibility */
        u_char  rtm_type;       /* message type */
        u_short rtm_index;      /* index for associated ifp */
        short   rtm_unit;       /* new field for subnet routing */
        int     rtm_flags;      /* flags, incl. kern & message, e.g. DONE */
        int     rtm_addrs;      /* bitmask identifying sockaddrs in msg */
        pid_t   rtm_pid;        /* identify sender */
        int     rtm_seq;        /* for sender to identify action */
        int     rtm_errno;      /* why failed */
        int     rtm_use;        /* from rtentry */
        u_long  rtm_inits;      /* which metrics we are initializing */
        u_long  rtm_netmask;    /* new field for subnet routing */
        char    rtm_name[IFNAMSIZ]; /* new field for subnet routing */
        struct  rt_metrics rtm_rmx; /* metrics themselves */
};
#pragma pack()

#define RTM_VERSION     4       /* Up the ante and ignore older versions */

#define RTM_ADD         0x1     /* Add Route */
#define RTM_DELETE      0x2     /* Delete Route */
#define RTM_CHANGE      0x3     /* Change Metrics or flags */
#define RTM_GET         0x4     /* Report Metrics */
#define RTM_LOSING      0x5     /* Kernel Suspects Partitioning */
#define RTM_REDIRECT    0x6     /* Told to use different route */
#define RTM_MISS        0x7     /* Lookup failed on this address */
#define RTM_LOCK        0x8     /* fix specified metrics */
#define RTM_OLDADD      0x9     /* caused by SIOCADDRT */
#define RTM_OLDDEL      0xa     /* caused by SIOCDELRT */
#define RTM_RESOLVE     0xb     /* req to resolve dst to LL addr */
#define RTM_NEWADDR     0xc     /* address being added to iface */
#define RTM_DELADDR     0xd     /* address being removed from iface */
#define RTM_IFINFO      0xe     /* iface going up/down etc. */

#define RTV_MTU         0x1     /* init or lock _mtu */
#define RTV_HOPCOUNT    0x2     /* init or lock _hopcount */
#define RTV_EXPIRE      0x4     /* init or lock _hopcount */
#define RTV_RPIPE       0x8     /* init or lock _recvpipe */
#define RTV_SPIPE       0x10    /* init or lock _sendpipe */
#define RTV_SSTHRESH    0x20    /* init or lock _ssthresh */
#define RTV_RTT         0x40    /* init or lock _rtt */
#define RTV_RTTVAR      0x80    /* init or lock _rttvar */

/*
 * Bitmask values for rtm_addr.
 */
#define RTA_DST         0x1     /* destination sockaddr present */
#define RTA_GATEWAY     0x2     /* gateway sockaddr present */
#define RTA_NETMASK     0x4     /* netmask sockaddr present */
#define RTA_GENMASK     0x8     /* cloning mask sockaddr present */
#define RTA_IFP         0x10    /* interface name sockaddr present */
#define RTA_IFA         0x20    /* interface addr sockaddr present */
#define RTA_AUTHOR      0x40    /* sockaddr for author of redirect */
#define RTA_BRD         0x80    /* for NEWADDR, broadcast or p-p dest addr */

/*
 * Index offsets for sockaddr array for alternate internal encoding.
 */
#define RTAX_DST        0       /* destination sockaddr present */
#define RTAX_GATEWAY    1       /* gateway sockaddr present */
#define RTAX_NETMASK    2       /* netmask sockaddr present */
#define RTAX_GENMASK    3       /* cloning mask sockaddr present */
#define RTAX_IFP        4       /* interface name sockaddr present */
#define RTAX_IFA        5       /* interface addr sockaddr present */
#define RTAX_AUTHOR     6       /* sockaddr for author of redirect */
#define RTAX_BRD        7       /* for NEWADDR, broadcast or p-p dest addr */
#define RTAX_MAX        8       /* size of array to allocate */

struct rt_addrinfo {
        int     rti_addrs;
        struct  sockaddr *rti_info[RTAX_MAX];
};

struct route_cb {
        int     ip_count;
        int     ns_count;
        int     iso_count;
        int     any_count;
};
#ifdef KERNEL
#include <private/route.h>
#endif
#endif /* TCPV40HDRS */
#endif /* _NET_ROUTE_H_ */
