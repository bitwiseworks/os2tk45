/********************************************************Copyright.IBM*****/
/*                                                                        */
/*                                                                        */
/*                                                                        */
/*    OCO Source Materials                                                */
/*    IBM Confidential (IBM Confidential-Restricted when combined         */
/*    with the aggregated OCO source modules for this program)            */
/*                                                                        */
/*    5622-069                                                            */
/*    (C) Copyright International Business Machines Corp. 1993, 1996      */
/*                                                                        */
/*                                                                        */
/********************************************************Copyright.IBM*****/

/*
 *  Protocols
 */

#ifndef _NETNB_NB_H_
#define _NETNB_NB_H_

#ifdef TCPV40HDRS
#include <stack16\netnb\nb.h>
#else
#define NBPROTO_NB      1     /*  NetBIOS protocol    */


/* DEFINITION OF NETBIOS ADDRESS FORMAT */
#define NB_FAMILY       18                   /* temporary */
#define NB_ADDRSIZE     sizeof (struct sockaddr_nb)
#define NB_UNIQUE       0
#define NB_GROUP        1
#define NB_BROAD        2
#define NB_NAMELEN      16
#define NB_NETIDLEN     8
#define NB_HOSTLEN      12
#define NB_PORTLEN      (NB_NAMELEN - NB_HOSTLEN)
#define NB_BCAST_NAME   "*               "      /* Asterisk plus 15 spaces */
#define NB_BLANK_NAME   "                "      /* 16 spaces */
#define NB_NULL_NAME    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"

/*  The following two lines must correspond to IPPORT_RESERVED and */
/*  IPPORT_USERRESERVED constants defined in in.h                  */
#define NBPORT_RESERVED     "49152"
#define NBPORT_USERRESERVED "65535"


/* DEFINITION OF NETBIOS ADDRESS FORMAT */


#pragma pack(1)
   struct  sockaddr_nb {
          u_char  snb_len;
          u_char  snb_family;                  /* netbios protocol family */
          short snb_type;                      /* unique or multicast */
          char  snb_netid[NB_NETIDLEN];        /* netbios netid*/
          char  snb_name[NB_NAMELEN];          /* netbios name */
                       };
#pragma pack()

#endif /* TCPV40HDRS */
#endif /* _NETNB_NB_H_ */
