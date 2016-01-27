#ifndef __NETDB_32H
#define __NETDB_32H
/*
 * Copyright (c) 1980,1983,1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *      @(#)netdb.h     5.9 (Berkeley) 4/5/88
 */


#ifdef __BORLANDC__              /* DRC01 */
#ifndef _System
#define _System _syscall
#endif
#endif

#ifdef __HIGHC__                  /* DRC01 */
#define _System
#endif

/*
 * Structures returned by network
 * data base library.  All addresses
 * are supplied in host order, and
 * returned in network order (suitable
 * for use in system calls).
 */
struct  hostent {
      char    *h_name;        /* official name of host */
      char    **h_aliases;    /* alias list */
      int     h_addrtype;     /* host address type */
      int     h_length;       /* length of address */
      char    **h_addr_list;  /* list of addresses from name server */
#define h_addr  h_addr_list[0]  /* address, for backward compatiblity */
};

/*
 * Assumption here is that a network number
 * fits in 32 bits -- probably a poor one.
 */
struct  netent {
      char            *n_name;        /* official name of net */
        char            **n_aliases;    /* alias list */
        int             n_addrtype;     /* net address type */
        unsigned long   n_net;          /* network # */
};

struct  servent {
        char    *s_name;        /* official service name */
        char    **s_aliases;    /* alias list */
        int     s_port;         /* port # */
        char    *s_proto;       /* protocol to use */
};

struct  protoent {
        char    *p_name;        /* official protocol name */
        char    **p_aliases;    /* alias list */
        int     p_proto;        /* protocol # */
};

#include <stdio.h>
#include <string.h>
#include <netinet\in.h>
#define _MAXALIASES     35
#define _MAXADDRS       35
#define _MAXLINELEN     1024
#define _HOSTBUFSIZE    (BUFSIZ + 1)

/*
 * After a successful call to gethostbyname_r()/gethostbyaddr_r(), the
 * structure hostent_data will contain the data to which pointers in
 * the hostent structure will point to.
 */

struct  hostent_data {
        struct    in_addr host_addr;              /* host address pointer */
        char      *h_addr_ptrs[_MAXADDRS + 1];    /* host address         */
        char      hostaddr[_MAXADDRS];
        char      hostbuf[_HOSTBUFSIZE + 1];      /* host data            */
        char      *host_aliases[_MAXALIASES];
        char      *host_addrs[2];
        FILE      *hostf;
        int       stayopen;                       /* AIX addon            */
        unsigned  long  host_addresses[_MAXADDRS];/* As per defect 48367. */
};                                                /*    Actual Addresses. */

struct  servent_data {          /* should be considered opaque */
        FILE *serv_fp;
        char line[_MAXLINELEN];
        char *serv_aliases[_MAXALIASES];
        int _serv_stayopen;
};

int _System gethostname( char *, int );
struct hostent * _System gethostbyname( char * );
struct hostent * _System gethostbyaddr( char *, int, int );
struct netent * _System getnetbyname( char * );
struct netent * _System getnetbyaddr( unsigned long, int );
struct servent * _System getservbyname( char *, char * );
struct servent * _System getservbyport( int, char * );
struct servent * _System getservent( void );
struct protoent * _System getprotobyname( char * );
struct protoent * _System getprotobynumber( int );
void _System sethostent( int );
struct hostent * _System gethostent( void );
void _System endhostent(void);
void _System setnetent( int );
struct netent * _System getnetent( void );
void _System endnetent(void);
void _System setprotoent( int );
struct protoent * _System getprotoent( void );
void _System endprotoent(void);
void _System setservent( int );
struct servent * _System getservent( void );
void _System endservent(void);
int _System tcp_h_errno(void);
struct hostent * _System Rgethostbyname(char *);

/*
 * Error return codes from gethostbyname() and gethostbyaddr()
 * (left in extern int h_errno).
 */

#define h_errno (tcp_h_errno())   /* Thread Re-entrant */

#define HOST_NOT_FOUND  1 /* Authoritative Answer Host not found */
#define TRY_AGAIN       2 /* Non-Authoritive Host not found, or SERVERFAIL */
#define NO_RECOVERY     3 /* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
#define NO_DATA         4 /* Valid name, no data record of requested type */
#define NO_ADDRESS      NO_DATA         /* no address, look for MX record */

#endif /* __NETDB_32H  */

