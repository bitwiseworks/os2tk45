#ifndef __NAMESER_32H
#define __NAMESER_32H

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *      @(#)nameser.h   5.20 (Berkeley) 10/7/88
 */

/* Modification History: *****************************************************/
/* Date:      By:   Tag:    Desc:                                            */
/* 09.24.96   DRC   DRC01   Added definition of _System for Borland Compiler */
/* 10.04.96   MRN  F17505   Added prototypes for res_query, res_search and   */
/*                          res_querydomain.                                 */
/*****************************************************************************/
/* DRC01 - Borland fix */
#ifdef __BORLANDC__
#define _System _syscall
#endif

/*
 * Define constants based on rfc883
 */
#define PACKETSZ        512             /* maximum packet size */
#define MAXDNAME        256             /* maximum domain name */
#define MAXCDNAME       255             /* maximum compressed domain name */
#define MAXLABEL        63              /* maximum length of domain label */
      /* Number of bytes of fixed size data in query structure */
#define QFIXEDSZ        4
      /* number of bytes of fixed size data in resource record */
#define RRFIXEDSZ       10

/*
 * Internet nameserver port number
 */
#define NAMESERVER_PORT 53

/*
 * Currently defined opcodes
 */
#define QUERY           0               /* standard query */
#define IQUERY          1               /* inverse query */
#define CQUERYM         2               /* completion query (multiple) */
#define CQUERYU         3               /* completion query (unique) */
      /* non standard */
#define UPDATEA         100             /* add resource record */
#define UPDATED         101             /* delete resource record */
#define UPDATEM         102             /* modify resource record */
#define ZONEINIT        103             /* initial zone transfer */
#define ZONEREF         104             /* incremental zone referesh */

/*
 * Currently defined response codes
 */
#define NOERROR         0               /* no error */
#define FORMERR         1               /* format error */
#define SERVFAIL        2               /* server failure */
#define NXDOMAIN        3               /* non existent domain */
#define NOTIMP          4               /* not implemented */
#define REFUSED         5               /* query refused */
        /* non standard */
#define NOCHANGE        100             /* update failed to change db */

/*
 * Type values for resources and queries
 */
#define T_A             1               /* host address */
#define T_NS            2               /* authoritative server */
#define T_MD            3               /* mail destination */
#define T_MF            4               /* mail forwarder */
#define T_CNAME         5               /* connonical name */
#define T_SOA           6               /* start of authority zone */
#define T_MB            7               /* mailbox domain name */
#define T_MG            8               /* mail group member */
#define T_MR            9               /* mail rename name */
#define T_NULL          10              /* null resource record */
#define T_WKS           11              /* well known service */
#define T_PTR           12              /* domain name pointer */
#define T_HINFO         13              /* host information */
#define T_MINFO         14              /* mailbox information */
#define T_MX            15              /* mail routing information */
        /* non standard */
#define T_UINFO         100             /* user (finger) information */
#define T_UID           101             /* user ID */
#define T_GID           102             /* group ID */
        /* Query type values which do not appear in resource records */
#define T_AXFR          252             /* transfer zone of authority */
#define T_MAILB         253             /* transfer mailbox records */
#define T_MAILA         254             /* transfer mail agent records */
#define T_ANY           255             /* wildcard match */

/*
 * Values for class field
 */

#define C_IN            1               /* the arpa internet */
#define C_CHAOS         3               /* for chaos net at MIT */
        /* Query class values which do not appear in resource records */
#define C_ANY           255             /* wildcard match */

/*
 * Structure for query header, the order of the fields is machine and
 * compiler dependent, in our case, the bits within a byte are assignd
 * least significant first, while the order of transmition is most
 * significant first.  This requires a somewhat confusing rearrangement.
 */

typedef struct {
        unsigned short id;             /* query identification number */
#ifdef OS2
        unsigned int   rd:1;           /* recursion desired */
        unsigned int   tc:1;           /* truncated message */
        unsigned int   aa:1;           /* authoritive answer */
        unsigned int   opcode:4;       /* purpose of message */
        unsigned int   qr:1;           /* response flag */
                        /* fields in fourth byte */
        unsigned int   rcode:4;        /* response code */
        unsigned int   unused:2;       /* unused bits */
        unsigned int   pr:1;           /* primary server required (non standard) */
        unsigned int   ra:1;           /* recursion available */
#else
#if defined (sun) || defined (sel) || defined (pyr) || defined (is68k) \
|| defined (tahoe) || defined (ibm032) || defined (ibm370) \
|| defined (BIT_ZERO_ON_LEFT)
        /* Bit zero on left:  Gould and similar architectures */
                        /* fields in third byte */
        unsigned char  qr:1;           /* response flag */
        unsigned char  opcode:4;       /* purpose of message */
        unsigned char  aa:1;           /* authoritive answer */
        unsigned char  tc:1;           /* truncated message */
        unsigned char  rd:1;           /* recursion desired */
                        /* fields in fourth byte */
        unsigned char  ra:1;           /* recursion available */
        unsigned char  pr:1;           /* primary server required (non standard) */
        unsigned char  unused:2;       /* unused bits */
        unsigned char  rcode:4;        /* response code */
#else
#if defined (vax) || defined (BIT_ZERO_ON_RIGHT)
        /* Bit zero on right:  VAX */
                        /* fields in third byte */
        unsigned char  rd:1;           /* recursion desired */
        unsigned char  tc:1;           /* truncated message */
        unsigned char  aa:1;           /* authoritive answer */
        unsigned char  opcode:4;       /* purpose of message */
        unsigned char  qr:1;           /* response flag */
                        /* fields in fourth byte */
        unsigned char  rcode:4;        /* response code */
        unsigned char  unused:2;       /* unused bits */
        unsigned char  pr:1;           /* primary server required (non standard) */
        unsigned char  ra:1;           /* recursion available */
#else
        /* you must determine what the correct bit order is for your compiler */
        UNDEFINED_BIT_ORDER;
#endif
#endif
#endif
                        /* remaining bytes */
        unsigned short qdcount;        /* number of question entries */
        unsigned short ancount;        /* number of answer entries */
        unsigned short nscount;        /* number of authority entries */
        unsigned short arcount;        /* number of resource entries */
} HEADER;

/*
 * Defines for handling compressed domain names
 */
#define INDIR_MASK      0xc0

/*
 * Structure for passing resource records around.
 */
#pragma pack(4) /* force on doubleword boundary */
struct rrec {
        short   r_zone;                 /* zone number */
        short   r_class;                /* class number */
        short   r_type;                 /* type number */
        unsigned long  r_ttl;                  /* time to live */
        int     r_size;                 /* size of data area */
        char    *r_data;                /* pointer to data */
};
#pragma pack()   /* reset to default packing */

int _System dn_expand(unsigned char *, unsigned char *, unsigned char *, unsigned char *, int);
int _System dn_comp(unsigned char *, unsigned char *, int, unsigned char **, unsigned char **);
int _System dn_find(unsigned char *, unsigned char *, unsigned char **, unsigned char **);
int _System dn_skipname(unsigned char *, unsigned char *);
unsigned short _System _getshort( unsigned char *);
unsigned long _System  _getlong( unsigned char *);
void _System putshort( unsigned short , unsigned char *);
void _System putlong( unsigned long , unsigned char *);
int _System res_init(void);
int _System res_mkquery(int, char *, int, int, char *, int, struct rrec *, char *, int);
int _System res_send(char *, int, char *, int);
/* char * _System p_type(int); */
/* char * _System p_class(int); */
/* char * _System p_time(u_long); */
int _System res_query(char *, int, int, u_char *, int);
int _System res_search(char *, int, int, u_char *, int);
int _System res_querydomain(char *, char *, int, int, u_char *, int);
/* char * _System  hostalias( char *); */
/* extern  u_short getshort(); */
/* extern  u_long  getlong(); */

#endif /* __NAMESER_32H */

