#ifndef __IP_ICMP_32H
#define __IP_ICMP_32H
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
 *      @(#)ip_icmp.h   7.3 (Berkeley) 12/7/87
 */

/*
 * Interface Control Message Protocol Definitions.
 * Per RFC 792, September 1981.
 */

/*
 * Structure of an icmp header.
 */
struct icmp {
        u_char  icmp_type;              /* type of message, see below */
        u_char  icmp_code;              /* type sub code */
        u_short icmp_cksum;             /* ones complement cksum of struct */
        union {
                u_char ih_pptr;                 /* ICMP_PARAMPROB */
                struct in_addr ih_gwaddr;       /* ICMP_REDIRECT */
                struct ih_idseq {
                        n_short icd_id;
                        n_short icd_seq;
                } ih_idseq;
                int ih_void;
        } icmp_hun;
#define icmp_pptr       icmp_hun.ih_pptr
#define icmp_gwaddr     icmp_hun.ih_gwaddr
#define icmp_id         icmp_hun.ih_idseq.icd_id
#define icmp_seq        icmp_hun.ih_idseq.icd_seq
#define icmp_void       icmp_hun.ih_void
        union {
                struct id_ts {
                        n_time its_otime;
                        n_time its_rtime;
                        n_time its_ttime;
                } id_ts;
                struct id_ip  {
                        struct ip idi_ip;
                        /* options and then 64 bits of data */
                } id_ip;
                u_long  id_mask;
                char    id_data[1];
        } icmp_dun;
#define icmp_otime      icmp_dun.id_ts.its_otime
#define icmp_rtime      icmp_dun.id_ts.its_rtime
#define icmp_ttime      icmp_dun.id_ts.its_ttime
#define icmp_ip         icmp_dun.id_ip.idi_ip
#define icmp_mask       icmp_dun.id_mask
#define icmp_data       icmp_dun.id_data
};

/*
 * Lower bounds on packet lengths for various types.
 * For the error advice packets must first insure that the
 * packet is large enought to contain the returned ip header.
 * Only then can we do the check to see if 64 bits of packet
 * data have been returned, since we need to check the returned
 * ip header length.
 */
#define ICMP_MINLEN     8                               /* abs minimum */
#define ICMP_TSLEN      (8 + 3 * sizeof (n_time))       /* timestamp */
#define ICMP_MASKLEN    12                              /* address mask */
#define ICMP_ADVLENMIN  (8 + sizeof (struct ip) + 8)    /* min */
#define ICMP_ADVLEN(p)  (8 + ((p)->icmp_ip.ip_hl << 2) + 8)
        /* N.B.: must separately check that ip_hl >= 5 */

/*
 * Definition of type and code field values.
 */
#define ICMP_ECHOREPLY          0               /* echo reply */
#define ICMP_UNREACH            3               /* dest unreachable, codes: */
#define         ICMP_UNREACH_NET        0               /* bad net */
#define         ICMP_UNREACH_HOST       1               /* bad host */
#define         ICMP_UNREACH_PROTOCOL   2               /* bad protocol */
#define         ICMP_UNREACH_PORT       3               /* bad port */
#define         ICMP_UNREACH_NEEDFRAG   4               /* IP_DF caused drop */
#define         ICMP_UNREACH_SRCFAIL    5               /* src route failed */
#define ICMP_SOURCEQUENCH       4               /* packet lost, slow down */
#define ICMP_REDIRECT           5               /* shorter route, codes: */
#define         ICMP_REDIRECT_NET       0               /* for network */
#define         ICMP_REDIRECT_HOST      1               /* for host */
#define         ICMP_REDIRECT_TOSNET    2               /* for tos and net */
#define         ICMP_REDIRECT_TOSHOST   3               /* for tos and host */
#define ICMP_ECHO               8               /* echo service */
#define ICMP_TIMXCEED           11              /* time exceeded, code: */
#define         ICMP_TIMXCEED_INTRANS   0               /* ttl==0 in transit */
#define         ICMP_TIMXCEED_REASS     1               /* ttl==0 in reass */
#define ICMP_PARAMPROB          12              /* ip header bad */
#define ICMP_TSTAMP             13              /* timestamp request */
#define ICMP_TSTAMPREPLY        14              /* timestamp reply */
#define ICMP_IREQ               15              /* information request */
#define ICMP_IREQREPLY          16              /* information reply */
#define ICMP_MASKREQ            17              /* address mask request */
#define ICMP_MASKREPLY          18              /* address mask reply */

#define ICMP_MAXTYPE            18

#define ICMP_INFOTYPE(type) \
        ((type) == ICMP_ECHOREPLY || (type) == ICMP_ECHO || \
        (type) == ICMP_TSTAMP || (type) == ICMP_TSTAMPREPLY || \
        (type) == ICMP_IREQ || (type) == ICMP_IREQREPLY || \
        (type) == ICMP_MASKREQ || (type) == ICMP_MASKREPLY)


#pragma pack(1)

struct  icmpstat {
/* statistics related to icmp packets generated */
        short   icps_error;             /* # of calls to icmp_error */
        short   icps_oldshort;          /* no error 'cuz old ip too short */
        short   icps_oldicmp;           /* no error 'cuz old was icmp */
        short   icps_outhist[ICMP_MAXTYPE + 1];
/* statistics related to input messages processed */
        short   icps_badcode;           /* icmp_code out of range */
        short   icps_tooshort;          /* packet < ICMP_MINLEN */
        short   icps_checksum;          /* bad checksum */
        short   icps_badlen;            /* calculated bound mismatch */
        short   icps_reflect;           /* number of responses */
        short   icps_inhist[ICMP_MAXTYPE + 1];
        u_long  icps_OutMsgs;
        u_long  icps_OutErrors;
        u_long  icps_InMsgs;
        u_long  icps_InDestUnreachs;
        u_long  icps_InTimeExcds;
        u_long  icps_InParmProbs;
        u_long  icps_InSrcQuenchs;
        u_long  icps_InRedirects;
        u_long  icps_InEchos;
        u_long  icps_InEchoReps;
        u_long  icps_InTimestamps;
        u_long  icps_InTimestampReps;
        u_long  icps_InAddrMasks;
        u_long  icps_InAddrMaskReps;
        u_long  icps_OutDestUnreachs;
        u_long  icps_OutTimeExcds;
        u_long  icps_OutParmProbs;
        u_long  icps_OutSrcQuenchs;
        u_long  icps_OutRedirects;
        u_long  icps_OutEchos;
        u_long  icps_OutEchoReps;
        u_long  icps_OutTimestamps;
        u_long  icps_OutTimestampReps;
        u_long  icps_OutAddrMasks;
        u_long  icps_OutAddrMaskReps;
};
#pragma pack()

#endif /* __IP_ICMP_32H */
