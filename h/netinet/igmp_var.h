/*
 * Copyright (c) 1988 Stephen Deering.
 * Copyright (c) 1992, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Stephen Deering of Stanford University.
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
 *      @(#)igmp_var.h  8.1 (Berkeley) 7/19/93
 */

#ifndef _NETINET_IGMP_VAR_H_
#define _NETINET_IGMP_VAR_H_

#ifdef TCPV40HDRS
#error error: netinet\igmp_var.h is for TCP/IP toolkit 5.0 or later releases only
#else
/*
 * Internet Group Management Protocol (IGMP),
 * implementation-specific definitions.
 *
 * Written by Steve Deering, Stanford, May 1988.
 *
 * MULTICAST 1.1
 */

struct igmpstat {
        u_long  igps_rcv_total;         /* total IGMP messages received */
        u_long  igps_rcv_tooshort;      /* received with too few bytes */
        u_long  igps_rcv_badsum;        /* received with bad checksum */
        u_long  igps_rcv_queries;       /* received membership queries */
        u_long  igps_rcv_badqueries;    /* received invalid queries */
        u_long  igps_rcv_reports;       /* received membership reports */
        u_long  igps_rcv_badreports;    /* received invalid reports */
        u_long  igps_rcv_ourreports;    /* received reports for our groups */
        u_long  igps_snd_reports;       /* sent membership reports */
};

#ifdef KERNEL
#include <private/igmp_var.h>
#endif

#endif /* TCPV40HDRS */
#endif /* _NETINET_IGMP_VAR_H_ */
