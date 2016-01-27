/*
 * Copyright (c) 1982, 1986, 1993
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
 *      @(#)ip_var.h    8.2 (Berkeley) 1/9/95
 */

#ifndef _NETINET_IP_VAR_H_
#define _NETINET_IP_VAR_H_

#ifdef TCPV40HDRS
#error error: netinet\ip_var.h is for TCP/IP toolkit 5.0 or later releases only
#else
/*
 * Overlay for ip header used by other protocols (tcp, udp).
 */
#pragma pack(1)
struct ipovly {
        caddr_t ih_next, ih_prev;       /* for protocol sequence q's */
        u_char  ih_x1;                  /* (unused) */
        u_char  ih_pr;                  /* protocol */
        short   ih_len;                 /* protocol length */
        struct  in_addr ih_src;         /* source internet address */
        struct  in_addr ih_dst;         /* destination internet address */
};
#pragma pack()

/*
 * Ip reassembly queue structure.  Each fragment
 * being reassembled is attached to one of these structures.
 * They are timed out after ipq_ttl drops to 0, and may also
 * be reclaimed if memory becomes tight.
 */
#pragma pack(1)
struct ipq {
        struct  ipq *next,*prev;        /* to other reass headers */
        u_char  ipq_ttl;                /* time for reass q to live */
        u_char  ipq_p;                  /* protocol of this fragment */
        u_short ipq_id;                 /* sequence id for reassembly */
        struct  ipasfrag *ipq_next,*ipq_prev;
                                        /* to ip headers of fragments */
        struct  in_addr ipq_src,ipq_dst;
};
#pragma pack()

/*
 * Ip header, when holding a fragment.
 *
 * Note: ipf_next must be at same offset as ipq_next above
 */
#pragma pack(1)
struct  ipasfrag {
#if BYTE_ORDER == LITTLE_ENDIAN
        unsigned ip_hl:4,
                ip_v:4;
#endif
#if BYTE_ORDER == BIG_ENDIAN
        unsigned ip_v:4,
                ip_hl:4;
#endif
        u_char  ipf_mff;                /* XXX overlays ip_tos: use low bit
                                         * to avoid destroying tos;
                                         * copied from (ip_off&IP_MF) */
        u_short ip_len;   /* should be ushort rather than short */
        u_short ip_id;
        short   ip_off;
        u_char  ip_ttl;
        u_char  ip_p;
        u_short ip_sum;
        struct  ipasfrag *ipf_next;     /* next fragment */
        struct  ipasfrag *ipf_prev;     /* previous fragment */
};
#pragma pack()

#define MAX_IPOPTLEN    40

struct  ipstat {
        u_long  ips_total;              /* total packets received */
        u_long  ips_badsum;             /* checksum bad */
        u_long  ips_tooshort;           /* packet too short */
        u_long  ips_toosmall;           /* not enough data */
        u_long  ips_badhlen;            /* ip header length < data size */
        u_long  ips_badlen;             /* ip length < ip header length */
        u_long  ips_fragments;          /* fragments received */
        u_long  ips_fragdropped;        /* frags dropped (dups, out of space)*/
        u_long  ips_fragtimeout;        /* fragments timed out */
        u_long  ips_forward;            /* packets forwarded */
        u_long  ips_cantforward;        /* packets rcvd for unreachable dest */
        u_long  ips_redirectsent;       /* packets forwarded on same net */
        u_long  ips_noproto;            /* unknown or unsupported protocol */
        u_long  ips_delivered;          /* datagrams delivered to upper level*/
        u_long  ips_localout;           /* total ip packets generated here */
        u_long  ips_odropped;           /* lost packets due to nobufs, etc. */
        u_long  ips_reassembled;        /* total packets reassembled ok */
        u_long  ips_fragmented;         /* datagrams sucessfully fragmented */
        u_long  ips_ofragments;         /* output fragments created */
        u_long  ips_cantfrag;           /* don't fragment flag was set, etc. */
        u_long  ips_badoptions;         /* error in option processing */
        u_long  ips_noroute;            /* packets discarded due to no route */
        u_long  ips_badvers;            /* ip version != 4 */
        u_long  ips_rawout;             /* total raw ip packets generated */
};

/*
 * Names for IP sysctl objects
 */
#define IPCTL_FORWARDING        1       /* act as router */
#define IPCTL_SENDREDIRECTS     2       /* may send redirects when forwarding */
#define IPCTL_INETCFG           40      /* Generic IP  Inetcfg switching */
#define IPCTL_INETVER           41      /* sysctl: AFINET Version */
#define IPCTL_FORWARD           42      /* sysctl:IP Forwarding */
#define IPCTL_FIREWALL          43      /* sysctl:FIREWALL flag */
#define IPCTL_ARPTKILLC         44      /* sysctl:ARP cache completed entry TO */
#define IPCTL_ARPTKILLI         45      /* sysctl:ARP cache incomplete entry TO */
#define FRAGCTL_TTL             46      /* sysctl:Fragments TTL */
#define IPCTL_SYNATTACK         47      /* sysctl:SYN Attack flg ON/OFF */
#define IPCTL_MULTIDEFROUTES    48      /* sysctl:Multiple def routes. lynx stuff */

#define IPCTL_NAMES { \
        { 0, 0 }, \
        { "forwarding", CTLTYPE_INT }, \
        { "redirect", CTLTYPE_INT }, \
        { "mtu", CTLTYPE_INT }, \
        { "multi-default-routes", CTLTYPE_INT }, \
        { "inetver", CTLTYPE_INETVER }, \
        { "inetcfg", CTLTYPE_INETCFG }, \
}

#ifdef KERNEL
#include <private/ip_var.h>
#endif

#endif /* TCPV40HDRS */
#endif /* _NETINET_IP_VAR_H_ */
