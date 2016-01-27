/*
 * Copyright (c) 1982, 1986, 1989, 1993
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
 *      @(#)if.h        8.3 (Berkeley) 2/9/95
 */

#ifndef _NET_IF_H_
#define _NET_IF_H_

#ifdef TCPV40HDRS
#include <stack16\net\if.h>
#else
#ifndef _TIME_ /*  XXX fast fix for SNMP, going away soon */
#include <sys/time.h>
#endif

/*
 * Structure describing information about an interface
 * which may be of interest to management entities.
 */

#ifndef IFMIB_ENTRIES
#define IFMIB_ENTRIES 42
#endif

#pragma pack(1) /* force on doubleword boundary */
struct iftable {
    short  iftIndex;        /* index of this interface */
    char   iftDescr[45];    /* description             */
    short  iftType;         /* type of the interface   */
    short  iftMtu;          /* MTU of the interface   */
    char   iftPhysAddr[6];  /* MTU of the interface   */
    short  iftOperStatus;
    u_long iftSpeed;
    u_long iftLastChange;
    u_long iftInOctets;
    u_long iftOutOctets;
    u_long iftOutDiscards;
    u_long iftInDiscards;
    u_long iftInErrors;
    u_long iftOutErrors;
    u_long iftInUnknownProtos;
    u_long iftInUcastPkts;
    u_long iftOutUcastPkts;
    u_long iftInNUcastPkts;
    u_long iftOutNUcastPkts;
};
struct ifmib {
    short ifNumber;
    struct iftable iftable[IFMIB_ENTRIES];
};
#pragma pack()   /* reset to default packing */
#define IFC_ALLRTSBCAST    0x0001
#define IFC_802_3          0x0002
#define IFC_FDDI           0x0004
#define IFC_NOREDIR        0x0010

#define OPERSTATUS_UP      0x1
#define OPERSTATUS_DOWN    0x2
#define OPERSTATUS_TESTING 0x3

#define IFF_UP          0x1             /* interface is up */
#define IFF_BROADCAST   0x2             /* broadcast address valid */
#define IFF_DEBUG       0x4             /* turn on debugging */
#define IFF_LOOPBACK    0x8             /* is a loopback net */
#define IFF_POINTOPOINT 0x10            /* interface is point-to-point link */
#define IFF_LINK2       0x20            /* was trailers, not used */
#define IFF_NOTRAILERS  IFF_LINK2
#define IFF_RUNNING     0x40            /* resources allocated */
#define IFF_NOARP       0x80            /* no address resolution protocol */
#define IFF_PROMISC     0x100           /* receive all packets */
#define IFF_ALLMULTI    0x200           /* receive all multicast packets */
#define IFF_BRIDGE      0x1000          /* support token ring routine field */
#define IFF_SNAP        0x2000          /* support extended SAP header */
#define IFF_DEFMTU      0x400           /* default mtu of 1500 */
#define IFF_RFC1469_BC  1               /* using broadcast */
#define IFF_RFC1469_FA  2               /* using functional */
#define IFF_RFC1469_MA  3               /* using multicast */
#define IFF_ETHER       0x4000          /* Ethernet interface */
#define IFF_LOOPBRD     0x8000          /* loop back broadcasts */
#define IFF_MULTICAST   0x800           /* supports multicast */

#define IFF_SIMPLEX     0x10000         /* can't hear own transmissions */
#define IFF_OACTIVE     0x20000         /* transmission in progress */
#define IFF_802_3       0x40000         /* */
#define IFF_CANONICAL   0x80000         /* */
#define IFF_RUNNINGBLK  0x100000        /* threads waited for intf running */

#define IFFE_PKTTRACE   0x00000001      /* trace datalink where possible */
#define IFFE_IPTRACE    0x00000002      /* trace ONLY IP packets */

/* flags set internally only: */
#define IFF_CANTCHANGE \
        (IFF_BROADCAST|IFF_POINTOPOINT|IFF_RUNNING|IFF_OACTIVE|\
            IFF_SIMPLEX|IFF_MULTICAST|IFF_ALLMULTI)

#pragma pack(1)
struct pkt_trace_hdr {
   u_short  pt_htype;  /* header type */
   u_short  pt_len;    /* in: pt_buf len, out: packet len */
   caddr_t  pt_data;   /* packet */
   u_long   pt_tstamp; /* time stamp in milliseconds */
};
#pragma pack()

#define HT_IP           0x01  /* IP */
#define HT_ETHER        0x06  /* Ethernet */
#define HT_ISO88023     0x07  /* CSMA CD */
#define HT_ISO88025     0x09  /* Token Ring */
#define HT_SLIP         0x1c  /* Serial Line IP */
#define HT_PPP          0x18  /* PPP IP */


#pragma pack(1)
        struct  if_data {
/* generic interface information */
                u_char  ifi_type;       /* ethernet, tokenring, etc */
                u_char  ifi_addrlen;    /* media address length */
                u_char  ifi_hdrlen;     /* media header length */
                u_long  ifi_mtu;        /* maximum transmission unit */
                u_long  ifi_metric;     /* routing metric (external only) */
                u_long  ifi_baudrate;   /* linespeed */
                                        /* volatile statistics */
                u_long  ifi_collisions; /* collisions on csma interfaces */
                u_long  ifi_ibytes;     /* total number of octets received */
                u_long  ifi_obytes;     /* total number of octets sent */
                u_long  ifi_oqdrops;    /* dropped on output, this interface */
                u_long  ifi_iqdrops;    /* dropped on input, this interface */
                u_long  ifi_ierrors;    /* input errors on interface */
                u_long  ifi_oerrors;    /* output errors on interface */
                u_long  ifi_noproto;    /* destined for unsupported protocol */
                u_long  ifi_ipackets;   /* packets received on interface */
                u_long  ifi_opackets;   /* packets sent on interface */
                u_long  ifi_imcasts;    /* packets received via multicast */
                u_long  ifi_omcasts;    /* packets sent via multicast */
                short   ifi_OperStatus; /* SNMP Oper Status */
                struct  timeval ifi_lastchange;/* last updated */
                u_char  ifi_descr[45];  /* description of the interface */
        };
#pragma pack()

/*
 * Message format for use in obtaining information about interfaces
 * from getkerninfo and the routing socket
 */
#pragma pack(1)
struct if_msghdr {
        u_short ifm_msglen;     /* to skip over non-understood messages */
        u_char  ifm_version;    /* future binary compatability */
        u_char  ifm_type;       /* message type */
        int     ifm_addrs;      /* like rtm_addrs */
        int     ifm_flags;      /* value of if_flags */
        u_short ifm_index;      /* index for associated ifp */
        struct  if_data ifm_data;/* statistics and other data about if */
};
#pragma pack()

/*
 * Message format for use in obtaining information about interface addresses
 * from getkerninfo and the routing socket
 */
#pragma pack(1)
struct ifa_msghdr {
        u_short ifam_msglen;    /* to skip over non-understood messages */
        u_char  ifam_version;   /* future binary compatability */
        u_char  ifam_type;      /* message type */
        int     ifam_addrs;     /* like rtm_addrs */
        int     ifam_flags;     /* value of ifa_flags */
        u_short ifam_index;     /* index for associated ifp */
        int     ifam_metric;    /* value of ifa_metric */
};
#pragma pack()

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */
#pragma pack(1)
struct  ifreq {
#define IFNAMSIZ        16
        char    ifr_name[IFNAMSIZ];             /* if name, e.g. "en0" */
        union {
                struct  sockaddr ifru_addr;
                struct  sockaddr ifru_dstaddr;
                struct  sockaddr ifru_broadaddr;
                long    ifru_flags;
                long_int ifru_metric;
                caddr_t ifru_data;
        } ifr_ifru;
#define ifr_addr        ifr_ifru.ifru_addr      /* address */
#define ifr_dstaddr     ifr_ifru.ifru_dstaddr   /* other end of p-to-p link */
#define ifr_broadaddr   ifr_ifru.ifru_broadaddr /* broadcast address */
#define ifr_flags       ifr_ifru.ifru_flags     /* flags */
#define ifr_metric      ifr_ifru.ifru_metric    /* metric */
#define ifr_data        ifr_ifru.ifru_data      /* for use by interface */
#define ifr_eflags      ifr_ifru.ifru_data      /* extended flags */
};
#pragma pack()

#pragma pack(1)
struct ifaliasreq {
        char    ifra_name[IFNAMSIZ];            /* if name, e.g. "en0" */
        struct  sockaddr ifra_addr;
        struct  sockaddr ifra_broadaddr;
        struct  sockaddr ifra_mask;
};

#pragma pack()

/*
 * Structure used in SIOCGIFCONF request.
 * Used to retrieve interface configuration
 * for machine (useful for programs which
 * must know all networks accessible).
 */
#pragma pack(1)
struct  ifconf {
        long_int ifc_len;               /* size of associated buffer */
        union {
                caddr_t ifcu_buf;
                struct  ifreq  *ifcu_req;
        } ifc_ifcu;
#define ifc_buf ifc_ifcu.ifcu_buf       /* buffer address */
#define ifc_req ifc_ifcu.ifcu_req       /* array of structures returned */
};
#pragma pack()


#ifdef KERNEL
#include <private/if.h>
#endif

#include <net/if_arp.h>
#include <netinet/in.h>

#pragma pack(1)
struct  in_aliasreq {
        char    ifra_name[IFNAMSIZ];            /* if name, e.g. "en0" */
        struct  sockaddr_in ifra_addr;
        struct  sockaddr_in ifra_broadaddr;
#define ifra_dstaddr ifra_broadaddr
        struct  sockaddr_in ifra_mask;
};
#pragma pack()

#pragma pack(1)
struct  addrreq  {                              /* get multicast addresses */
        char    ifr_name[IFNAMSIZ];
        struct  sockaddr ifr_addrs;
        u_long  maddr[MAX_IN_MULTI];
};
#pragma pack()

#pragma pack(1)
struct  statatreq {
        u_long addr;
        short interface;
        u_long mask;
        u_long broadcast;
};
#pragma pack()

#endif /* TCPV40HDRS */

/* PPP statistics table. Moved here from previous ifstat.h */
struct ifstat {
    u_long iftLastChange;
    u_long iftInOctets;
    u_long iftOutOctets;
    u_long iftOutDiscards;
    u_long iftInDiscards;
    u_long iftInErrors;
    u_long iftOutErrors;
    u_long iftInUnknownProtos;
    u_long iftInUcastPkts;
    u_long iftOutUcastPkts;
    u_long iftInNUcastPkts;
    u_long iftOutNUcastPkts;
};

#endif /* _NET_IF_H_ */
