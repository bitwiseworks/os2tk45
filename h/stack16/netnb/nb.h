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

/*  F4334  RTW 04-08-94

$Source: u:\src\nb\rcs\NB.H $
$Date:   11 Apr 1996 11:10:36  $
$Revision:   1.1  $
$Locker:  $
$Log:   T:\tcp20vcs\pgmgkit\ship\include\nb\nb.h_v  $
 *
 *    F11495    11 Apr 1996 11:10:36   MERIDETH NORRIS
 * Replace // comments with /* comments
 *
 *
 *    Rev 1.1   10 Jan 1995 10:25:36   ccw
 * Beautify
 *
 *    Rev 1.0   10 Jan 1995 10:20:40   ccw
 * Initial revision.
 *
 *    Rev 1.1   11 Oct 1994 13:01:14   KFISHER
 * added standard comments around PVCS header info so it will compile
 *
 *    Rev 1.0   06 Oct 1994 13:50:10   SANJAY_KHANNA
 * Initial revision.
Revision 1.3  91/05/20  11:40:42  unknown
included headers and test program definition(s)

$Header:   T:\tcp20vcs\pgmgkit\ship\include\nb\nb.h_v   1.1   10 Jan 1995 10:25:36   ccw  $
*/

/*
 *  Protocols
 */

#ifndef NB_H
#define NB_H

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
#define NB_BLANK_NAME   "                "      // 16 spaces
#define NB_NULL_NAME    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"

//  The following two lines must correspond to IPPORT_RESERVED and
//  IPPORT_USERRESERVED constants defined in in.h
#define NBPORT_RESERVED "1024"
#define NBPORT_USERRESERVED "5000"


/* DEFINITION OF NETBIOS ADDRESS FORMAT */


   struct  sockaddr_nb {
          short snb_family;                    /*netbios protocol family*/
          short snb_type;                      /* unique or multicast */
          char  snb_netid[NB_NETIDLEN];         /* netbios netid*/
          char  snb_name[NB_NAMELEN];           /* netbios name */
                       };
#endif
