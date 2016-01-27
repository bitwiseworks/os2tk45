/*-----------------------------------------------------------------
Copyright (c) 1989 Regents of the University of California.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
        This product includes software developed by the University of
        California, Berkeley and its contributors.
4. Neither the name of the University nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

-----------------------------------------------------------------
Portions Copyright (c) 1993, 1994 by NEC Systems Laboratory.

Permission to use, copy, modify, and distribute this software for
any purpose with or without fee is hereby granted, provided that
the above copyright notice and this permission notice appear in all
copies, and that the name of NEC Systems Laboratory not be used in
advertising or publicity pertaining to distribution of the document
or software without specific, written prior permission.

THE SOFTWARE IS PROVIDED ``AS IS'' AND NEC SYSTEMS LABORATORY DISCLAIMS
ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL NEC
SYSTEMS LABORATORY BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
OR PERFORMANCE OF THIS SOFTWARE.

-----------------------------------------------------------------*/
#include <netinet\in.h>

/*
 * Default port number for SOCKS services.
 */
#define SOCKS_DEF_PORT  1080

/* Current SOCKS protocol version */
#define SOCKS_VERSION   4

/*
**  Response commands/codes
*/
#define SOCKS_CONNECT   1
#define SOCKS_BIND      2
#define SOCKS_RESULT    90
#define SOCKS_FAIL      91
#define SOCKS_NO_IDENTD 92 /* Failed to connect to Identd on client machine */
#define SOCKS_BAD_ID    93 /* Client's Identd reported a different user-id */

typedef unsigned long u_int32;

typedef struct {
        u_int32                 host; /* in network byte order */
        unsigned short          port; /* in network byte oreder */
        unsigned char           version;
        unsigned char           cmd;
} Socks_t;

typedef struct socksdata {
   /* the following feilds are for socks */
   char * socks_server;
   char * socks_serverlist;
   int direct;
   struct sockaddr_in cursin;
   int socks_conn_sock;
   unsigned short socks_port;
   unsigned short socks_conn_port;
   unsigned short socks_last_conn_port;
   unsigned long  socks_conn_host;
   unsigned long  socks_last_conn_host;
   struct sockaddr_in socks_nsin;
   struct sockaddr_in me;
} SD;
