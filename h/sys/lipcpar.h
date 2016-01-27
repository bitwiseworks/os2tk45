/*************************************************************************
*
*   Licensed Materials - Property of IBM
*   IBM TCP/IP for OS/2
*   (C) Copyright IBM Corp. 1996, 1997  All rights reserved.
*
*   US Government Users Restricted Rights -
*   Use, duplication or disclosure restricted by GSA ADP Schedule
*   Contract with IBM Corp.
*
*************************************************************************/

#ifndef _SYS_LIPCPAR_H
#define _SYS_LIPCPAR_H

#ifdef TCPV40HDRS
#error error: sys\lipcpar.h is for TCP/IP toolkit 5.0 or later releases only
#else
/*
 * Names for LIPC(AFOS2) sysctl objects
 */
#define LIPCCTL_INETCFG        60            /* Generic inetcfg sysctl code */
#define LIPCCTL_INETVER        61            /* Sysctl for afos2 */
#define LIPCCTL_ST_SENDSPACE   62            /* Stream SendSpace */
#define LIPCCTL_ST_RECVSPACE   63            /* Stream RecvSpace */
#define LIPCCTL_DG_SENDSPACE   64            /* Datagram SendSpace */
#define LIPCCTL_DG_RECVSPACE   65            /* Datagram RecvSpace */

#ifdef KERNEL
#include <private/lipcpar.h>
#endif

#endif /* TCPV40HDRS */
#endif /* !_SYS_LIPCPAR_H */
