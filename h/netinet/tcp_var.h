/*
 * Copyright (c) 1982, 1986, 1993, 1994, 1995
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
 *      @(#)tcp_var.h   8.4 (Berkeley) 5/24/95
 */

#ifndef _NETINET_TCP_VAR_H_
#define _NETINET_TCP_VAR_H_

#ifdef TCPV40HDRS
#error error: netinet\tcp_var.h is for TCP/IP toolkit 5.0 or later releases only
#else
/*
 * TCP statistics.
 * Many of these should be kept per connection,
 * but that's inconvenient at the moment.
 */
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
        u_long  tcps_sndrst;            /* RST packets send .... MIB II       */

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
        u_long  tcps_pawsdrop;          /* segments dropped due to PAWS */
        u_long  tcps_predack;           /* times hdr predict ok for acks */
        u_long  tcps_preddat;           /* times hdr predict ok for data pkts*/
        u_long  tcps_pcbcachemiss;
        u_long  tcps_persistdrop;       /* timeout in persist state */
        u_long  tcps_badsyn;            /* bogus SYN, e.g. premature ACK */

        /* the fillowing 7 are for mtudisco and ttcp */

        u_long  tcps_mturesent;         /* resends due to MTU discovery */
        u_long  tcps_cachedrtt;         /* times cached RTT in route updated */
        u_long  tcps_cachedrttvar;      /* times cached rttvar updated */
        u_long  tcps_cachedssthresh;    /* times cached ssthresh updated */
        u_long  tcps_usedrtt;           /* times RTT initialized from route */
        u_long  tcps_usedrttvar;        /* times RTTVAR initialized from rt */
        u_long  tcps_usedssthresh;      /* times ssthresh initialized from rt*/
};

/*
 * Names for TCP sysctl objects
 */
#define TCPCTL_MSSDFLT          1       /* MSS default */
#define TCPCTL_STATS            2       /* statistics (read-only) */
#define TCPCTL_RTTDFLT          3       /* default RTT estimate */
#define TCPCTL_MSL              4       /* No Of Keepalive probes */
#define TCPCTL_INETCFG          6       /* Generic TCP Inetcfg switching */
#define TCPCTL_LINGERTIME       7       /* Linger On close timer */
#define TCPCTL_KEEPCNT          8       /* No Of Keepalive probes */
#define TCPCTL_TCPSWIN          9       /* TCP Send Window Size    */
#define TCPCTL_TCPRWIN          10      /* TCP Recieve Window Size */
#define TCPCTL_TTL              11      /* TTL for TCP  packets */
#define TCPCTL_MTU              12      /* Path MTU Discovery ON/OFF flg */
#define TCPCTL_WINSCALE         13      /* Winow Scale (Fat Pipe) ON/OFF flg */
#define TCPCTL_TIMESTMP         14      /* TCP TimeStamp ON/OFF flg */
#define TCPCTL_CC               15      /* Cc, CCnew & Echo ON/OFF flg */
#define TCPCTL_REALSLOW         16      /* Real Slow Timer for Time Wait Q */
#define TCPCTL_REUSETW          17      /* Reuse TW inetcfg */
#define TCPCTL_SYNCOOKIE        18      /* Syncookie inetcfg */
#define TCPCTL_PERFHTTP        19      /* Fast Path HTTP */


/*Inetcfg ioctl constants */
#define ICFG_SETKEEPALIVE       20      /* sysctl code:Set the KeepAlive timer */
#define ICFG_GETKEEPALIVE       21      /* sysctl code:Get the KeepAlive timer defaults */

#define TCPCTL_NAMES { \
        { 0, 0 }, \
        { "mssdflt", CTLTYPE_INT },    \
        { "stats",   CTLTYPE_STRUCT }, \
        { "rttdflt", CTLTYPE_INT },    \
        { "inetcfg", CTLTYPE_INETCFG },\
}

#ifdef KERNEL
#include <private/tcp_var.h>
#endif

#endif /* TCPV40HDRS */
#endif /* _NETINET_TCP_VAR_H_ */
