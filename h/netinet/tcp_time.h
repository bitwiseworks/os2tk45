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
 *      @(#)tcp_timer.h 8.1 (Berkeley) 6/10/93
 */

#ifndef _NETINET_TCP_TIME_H_
#define _NETINET_TCP_TIME_H_

#ifdef TCPV40HDRS
#error error: netinet\tcp_time.h is for TCP/IP toolkit 5.0 or later releases only
#else
#define TCPTV_SRTTBASE  0                       /* base roundtrip time;
                                                   if 0, no idea yet      */
#define TCPTV_SRTTDFLT  (  3*PR_SLOWHZ)         /* assumed RTT if no info */

#define TCPTV_PERSMIN   (  5*PR_SLOWHZ)         /* retransmit persistance */
#define TCPTV_PERSMAX   ( 60*PR_SLOWHZ)         /* maximum persist interval */

#define TCPTV_KEEP_INIT ( 75*PR_SLOWHZ)         /* initial connect keep alive */
#define TCPTV_KEEP_IDLE (120*60*PR_SLOWHZ)      /* dflt time before probing */
#define TCPTV_KEEPINTVL ( 75*PR_SLOWHZ)         /* default probe interval */

#define TCPTV_MIN       (  1*PR_SLOWHZ)         /* min allowable REXMT value */
#define TCPTV_REXMTMAX  ( 64*PR_SLOWHZ)         /* max allowable REXMT value */

#define TCP_MAXRXTSHIFT 12                      /* maximum retransmits */
#define TCPTV_TWTRUNC    8                /* RTO factor to truncate TIME_WAIT */

#ifdef KERNEL
#include <private/tcp_time.h>
#endif

#endif /* TCPV40HDRS */
#endif /* _NETINET_TCP_TIME_H_ */
