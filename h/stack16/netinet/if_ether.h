/********************************************************copyrite.xmc***/
/*                                                                     */
/*   Licensed Materials - Property of IBM                              */
/*                                                                     */
/*   This module is "Restricted Materials of IBM":                     */
/*      Program Number:   5798RXW                                      */
/*      Program Name:     IBM TCP/IP Version 1.2 for OS/2              */
/*   (C) Copyright IBM Corporation. 1990, 1991.                        */
/*                                                                     */
/*   See IBM Copyright Instructions.                                   */
/*                                                                     */
/********************************************************copyrite.xmc***/
#ifndef __IF_ETHER_32H
#define __IF_ETHER_32H
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
 *      @(#)ip.h        7.6.1.1 (Berkeley) 3/15/88
 */

/*
 * Structure of a 10Mb/s Ethernet header.
 */
struct  ether_header {
        u_char  ether_dhost[6];
        u_char  ether_shost[6];
        u_short ether_type;
};

#define ETHERTYPE_PUP   0x0200          /* PUP protocol */
#define ETHERTYPE_IP    0x0800          /* IP protocol */
#ifdef OS2
#define ETHERTYPE_ARP   0x0608          /* Addr. resolution protocol */
#else
#define ETHERTYPE_ARP   0x0806          /* Addr. resolution protocol */
#endif
/*
 * The ETHERTYPE_NTRAILER packet types starting at ETHERTYPE_TRAIL have
 * (type-ETHERTYPE_TRAIL)*512 bytes of data followed
 * by an ETHER type (as given above) and then the (variable-length) header.
 */
#define ETHERTYPE_TRAIL         0x1000          /* Trailer packet */
#define ETHERTYPE_NTRAILER      16

#define ETHERMTU        1500
#define ETHERMIN        (60-14)

/*
 * Ethernet Address Resolution Protocol.
 *
 * See RFC 826 for protocol description.  Structure below is adapted
 * to resolving internet addresses.  Field names used correspond to
 * RFC 826.
 */
struct  ether_arp {
        struct  arphdr ea_hdr;  /* fixed-size header */
        u_char  arp_sha[6];     /* sender hardware address */
        u_char  arp_spa[4];     /* sender protocol address */
        u_char  arp_tha[6];     /* target hardware address */
        u_char  arp_tpa[4];     /* target protocol address */
};
#define arp_hrd ea_hdr.ar_hrd
#define arp_pro ea_hdr.ar_pro
#define arp_hln ea_hdr.ar_hln
#define arp_pln ea_hdr.ar_pln
#define arp_op  ea_hdr.ar_op


/*
 * Structure shared between the ethernet driver modules and
 * the address resolution code.  For example, each ec_softc or il_softc
 * begins with this structure.
 */
struct  arpcom {
        struct  ifnet ac_if;            /* network-visible interface */
        u_char  ac_enaddr[6];           /* ethernet hardware address */
        struct in_addr ac_ipaddr;       /* copy of ip address- XXX */
};

/*
 * Internet to ethernet address resolution table.
 */
#pragma pack(1) /* force on doubleword boundary */
struct  arptab {
        struct  in_addr at_iaddr;       /* internet address */
        u_char  at_enaddr[6];           /* ethernet address */
        u_char  at_timer;               /* minutes since last reference */
        u_char  at_flags;               /* flags */
        struct  mbuf *at_hold;          /* last packet until resolved/timeout */
        u_short at_rcf;                 /* token ring routing control field */
        u_short at_rseg[8];             /* token ring routing segments */
#ifdef OS2
        u_long  at_millisec;            /* TOD milliseconds of last update */
        short   at_interface;           /* interface index */
#endif
};
#pragma pack()   /* reset to default packing */

#ifdef  KERNEL
u_char etherbroadcastaddr[6];
struct  arptab *arptnew();
char *ether_sprintf();
#endif

#endif /* __IF_ETHER_32H */
