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
 *      @(#)if_ether.h  8.3 (Berkeley) 5/2/95
 */

#ifndef _NETINET_IF_ETHER_H_
#define _NETINET_IF_ETHER_H_

#ifdef TCPV40HDRS
#include <stack16\netinet\if_ether.h>
#else
#pragma pack(1)
struct sockaddr_inarp {
        u_char  sin_len;
        u_char  sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        struct  in_addr sin_srcaddr;
        u_short sin_tos;
        u_short sin_other;
#define SIN_PROXY 1
};
#pragma pack()

#pragma pack(1)
struct  oarptab {
        struct  in_addr at_iaddr;       /* internet address */
        u_char  at_enaddr[6];           /* ethernet address */
        u_char  at_timer;               /* minutes since last reference */
        u_char  at_flags;               /* flags */
#ifdef KERNEL
        struct  mbuf *at_hold;          /* last packet until resolved/timeout */
#else
        void * at_hold;
#endif
        u_short at_rcf;                 /* token ring routing control field */
        u_short at_rseg[8];             /* token ring routing segments */
        u_long  at_millisec;            /* TOD milliseconds of last update */
        u_short at_interface;           /* interface index */
};
#pragma pack()

#ifdef KERNEL
#include <private\if_ether.h>
#endif
/*
 * IP and ethernet specific routing flags
 */
#define RTF_USETRAILERS RTF_PROTO1      /* use trailers */
#define RTF_ANNOUNCE    RTF_PROTO2      /* announce new arp entry */

#endif /* TCPV40HDRS */
#endif /* _NETINET_IF_ETHER_H_ */
