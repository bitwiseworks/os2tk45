/********************************************************copyrite.xmc***/
/*                                                                     */
/*   Licensed Materials - Property of IBM                              */
/*                                                                     */
/*   This module is "Restricted Materials of IBM":                     */
/*   (C) Copyright IBM Corporation. 1990, 1991.                        */
/*                                                                     */
/*   See IBM Copyright Instructions.                                   */
/*                                                                     */
/********************************************************copyrite.xmc***/
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
 *      @(#)tcp.h       7.4.1.1 (Berkeley) 2/7/88
 */
#ifndef _TCP
#define _TCP
#ifndef BYTE_ORDER
/*
 * Definitions for byte order,
 * according to byte significance from low address to high.
 */
#define LITTLE_ENDIAN   1234    /* least-significant byte first (vax) */
#define BIG_ENDIAN      4321    /* most-significant byte first (IBM, net) */
#define PDP_ENDIAN      3412    /* LSB first in word, MSW first in long (pdp) */

#ifdef OS2
#define BYTE_ORDER      LITTLE_ENDIAN
#else
#ifdef vax
#define BYTE_ORDER      LITTLE_ENDIAN
#else
#define BYTE_ORDER      BIG_ENDIAN      /* mc68000, tahoe, most others */
#endif
#endif
#endif

typedef u_long  tcp_seq;
/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
#pragma pack(1)
struct tcphdr {
        u_short th_sport;               /* source port */
        u_short th_dport;               /* destination port */
        tcp_seq th_seq;                 /* sequence number */
        tcp_seq th_ack;                 /* acknowledgement number */
#if BYTE_ORDER == LITTLE_ENDIAN
        u_int   th_x2:4,                /* (unused) */
                th_off:4,               /* data offset */
                th_flags:8,
                th_win:16;
#endif
#if BYTE_ORDER == BIG_ENDIAN
        u_int   th_win:16,
                th_flags:8,
                th_off:4,               /* data offset */
                th_x2:4;                /* (unused) */
#endif
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
        u_short th_sum;                 /* checksum */
        u_short th_urp;                 /* urgent pointer */
};
#pragma pack()

#define TCPOPT_EOL      0
#define TCPOPT_NOP      1
#define TCPOPT_MAXSEG   2

/*
 * Default maximum segment size for TCP.
 * With an IP MSS of 576, this is 536,
 * but 512 is probably more convenient.
 */
#ifdef  lint
#define TCP_MSS 536
#else
#ifndef IP_MSS
#define IP_MSS  576
#endif
#define TCP_MSS MIN(512, IP_MSS - 40)
#endif

/*
 * User-settable options (used with setsockopt).
 */
#define TCP_NODELAY     0x01    /* don't delay send to coalesce packets */
#define TCP_MAXSEG      0x02    /* set maximum segment size */

#pragma pack(1)
struct  tcpstat {
        u_long  tcps_connattempt;       /* connections initiated */
        u_long  tcps_accepts;           /* connections accepted */
        u_long  tcps_connects;          /* connections established */
        u_long  tcps_drops;             /* connections dropped */
        u_long  tcps_conndrops;         /* embryonic connections dropped */
        u_long  tcps_closed;            /* conn. closed (includes drops) */
        u_long  tcps_segstimed;         /* segs where we tried to get rtt */
        u_long  tcps_rttupdated;        /* times we succeeded */
        u_long  tcps_delack;            /* delayed acks sent */
        u_long  tcps_timeoutdrop;       /* conn. dropped in rxmt timeout */
        u_long  tcps_rexmttimeo;        /* retransmit timeouts */
        u_long  tcps_persisttimeo;      /* persist timeouts */
        u_long  tcps_keeptimeo;         /* keepalive timeouts */
        u_long  tcps_keepprobe;         /* keepalive probes sent */
        u_long  tcps_keepdrops;         /* connections dropped in keepalive */

        u_long  tcps_sndtotal;          /* total packets sent */
        u_long  tcps_sndpack;           /* data packets sent */
        u_long  tcps_sndbyte;           /* data bytes sent */
        u_long  tcps_sndrexmitpack;     /* data packets retransmitted */
        u_long  tcps_sndrexmitbyte;     /* data bytes retransmitted */
        u_long  tcps_sndacks;           /* ack-only packets sent */
        u_long  tcps_sndprobe;          /* window probes sent */
        u_long  tcps_sndurg;            /* packets sent with URG only */
        u_long  tcps_sndwinup;          /* window update-only packets sent */
        u_long  tcps_sndctrl;           /* control (SYN|FIN|RST) packets sent */
        u_long  tcps_sndrst;            /* RST packets sent, MIB II ... */

        u_long  tcps_rcvtotal;          /* total packets received */
        u_long  tcps_rcvpack;           /* packets received in sequence */
        u_long  tcps_rcvbyte;           /* bytes received in sequence */
        u_long  tcps_rcvbadsum;         /* packets received with ccksum errs */
        u_long  tcps_rcvbadoff;         /* packets received with bad offset */
        u_long  tcps_rcvshort;          /* packets received too short */
        u_long  tcps_rcvduppack;        /* duplicate-only packets received */
        u_long  tcps_rcvdupbyte;        /* duplicate-only bytes received */
        u_long  tcps_rcvpartduppack;    /* packets with some duplicate data */
        u_long  tcps_rcvpartdupbyte;    /* dup. bytes in part-dup. packets */
        u_long  tcps_rcvoopack;         /* out-of-order packets received */
        u_long  tcps_rcvoobyte;         /* out-of-order bytes received */
        u_long  tcps_rcvpackafterwin;   /* packets with data after window */
        u_long  tcps_rcvbyteafterwin;   /* bytes rcvd after window */
        u_long  tcps_rcvafterclose;     /* packets rcvd after "close" */
        u_long  tcps_rcvwinprobe;       /* rcvd window probe packets */
        u_long  tcps_rcvdupack;         /* rcvd duplicate acks */
        u_long  tcps_rcvacktoomuch;     /* rcvd acks for unsent data */
        u_long  tcps_rcvackpack;        /* rcvd ack packets */
        u_long  tcps_rcvackbyte;        /* bytes acked by rcvd acks */
        u_long  tcps_rcvwinupd;         /* rcvd window update packets */
};
#pragma pack()

#endif
