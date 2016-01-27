/* IBM Confidential                                 */
/* Licensed Materials - Property of IBM Corporation */
/* (C) Copyright IBM Corp 1990,1996. All rights reserved */

/*
 * Copyright (c) 1982, 1986, 1988, 1993
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
 *      @(#)mbuf.h      8.5 (Berkeley) 2/19/95
 */

#ifndef _SYS_MBUF_H_
#define _SYS_MBUF_H_

#ifdef TCPV40HDRS
#error error: sys\mbuf.h is for TCP/IP toolkit 5.0 or later releases only
#else
/* mbuf types */
#define MT_FREE         0       /* should be on free list */
#define MT_DATA         1       /* dynamic (data) allocation */
#define MT_HEADER       2       /* packet header */
#define MT_SOCKET       3       /* socket structure */
#define MT_PCB          4       /* protocol control block */
#define MT_RTABLE       5       /* routing tables */
#define MT_HTABLE       6       /* IMP host tables */
#define MT_ATABLE       7       /* address resolution tables */
#define MT_SONAME       8       /* socket name */
#define MT_SOOPTS       10      /* socket options */
#define MT_FTABLE       11      /* fragment reassembly header */
#define MT_RIGHTS       12      /* access rights */
#define MT_IFADDR       13      /* interface address */
#define MT_CONTROL      14      /* extra-data protocol message */
#define MT_OOBDATA      15      /* expedited data  */

/* mulitcast (os/2 only) */
#define MT_IPMOPTS      16      /* internet multicast options */
#define MT_IPMADDR      17      /* internet multicast address */
#define MT_IFMADDR      18      /* link-level multicast address */
#define MT_MRTABLE      19      /* multicast routing tables */
#define MT_MAPIO        20      /* memory map io */
/*
 * Mbuf statistics.
 */
#pragma pack(1)
struct mbstat {
        u_long  m_mbufs;        /* mbufs obtained from page pool */
        u_long  m_clusters;     /* clusters obtained from page pool */
        u_long  m_spare;        /* spare field */
        u_long  m_clfree;       /* free clusters */
        u_long  m_drops;        /* times failed to find space */
        u_long  m_wait;         /* times waited for space */
        u_long  m_drain;        /* times drained protocols for space */
        u_short m_mtypes[256];  /* type specific mbuf allocations */
};
#pragma pack()

#ifdef KERNEL
#include <private/mbuf.h>
#endif

#endif /* TCPV40HDRS */
#endif /* !_SYS_MBUF_H_ */
