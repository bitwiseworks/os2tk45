/***************************************************************************/
/* IBM CONFIDENTIAL                                                        */
/* OCO Source Materials                                                    */
/* IBM TCP/IP for OS/2                                                     */
/* (c) Copyright IBM Corp 1992,1997                                        */
/* The source code for this program is not published or otherwise          */
/* divested of its trade secrets, irrespective of what has been            */
/* deposited with the U.S. Copyright Office                                */
/***************************************************************************/
#ifndef pddentry_h
#define pddentry_h

/* IBM Confidential                                 */
/* Licensed Materials - Property of IBM Corporation */
/* (C) Copyright IBM Corp 1990. All rights reserved */
/*
 * $Author:   vashaw  $
 * $Revision:   1.1  $
 * $Modtime:   04 Nov 1994 13:01:22  $
 * $Header:   K:\tcp20vcs\basekit\src\convstck\sockets\pddentry.h_v   1.1   04 Nov 1994 13:01:32   vashaw  $
 * $Log:   K:\tcp20vcs\basekit\src\convstck\sockets\pddentry.h_v  $
 *
 *    Rev 1.1   04 Nov 1994 13:01:32   vashaw
 * Got rid of warnings on // comments.
 *
 *    Rev 1.0   06 Oct 1994 13:34:10   SANJAY_KHANNA
 * Initial revision.
 *
 *    Rev 1.4   22 Mar 1994 15:05:24   SANJAY_KHANNA
 * Definitions of Worker(0 and DynamicAPIEntry() for callgate support
 *
 *    Rev 1.3   09 Nov 1993 10:53:30   Jonathan_Rosen
 * - Added support for DYNAMIC sockets entry point.
 *
 *    Rev 1.2   22 Mar 1993 16:52:20   Jonathan_Rosen
 * - Added missing #define for PDDENTRY_CANCEL.
 *
 *    Rev 1.1   04 Mar 1993 17:17:16   Jonathan_Rosen
 * Added prototype for _PDDEntry to allow a call to DevHlp_RegisterPDD().
 *
 *    Rev 1.0   04 Mar 1993 16:48:00   Jonathan_Rosen
 * Initial revision.
 */

#define PDDCMD_REGISTER         0   /* to register VDD entry point with PDD */
#define PDDCMD_DEREGISTER       1   /* To deregister a VDD at MVDM shutdown */
#define PDDCMD_SOCKET           2
#define PDDCMD_BIND             3
#define PDDCMD_LISTEN           4
#define PDDCMD_ACCEPT           5
#define PDDCMD_CONNECT          6
#define PDDCMD_SENDTO           7
#define PDDCMD_SEND             8
#define PDDCMD_SENDV            9
#define PDDCMD_RECVV           10
#define PDDCMD_SENDMSG         11
#define PDDCMD_RECVFROM        12
#define PDDCMD_RECV            13
#define PDDCMD_RECVMSG         14
#define PDDCMD_SHUTDOWN        15
#define PDDCMD_SETSOCKOPT      16
#define PDDCMD_GETSOCKOPT      17
#define PDDCMD_GETSOCKNAME     18
#define PDDCMD_GETPEERNAME     19
#define PDDCMD_CLOSE           20
#define PDDCMD_ABORT           21
#define PDDCMD_IOCTL           22
#define PDDCMD_SELECT          23
#define PDDCMD_GETHOSTID       24
#define PDDCMD_CANCEL          25

#ifdef KERNEL
extern void DevHelp_RegisterPDD(char *PDD_Name, void *PDD_Function);
//extern int  _PDDEntry();
extern unsigned short  _PDDEntry();
extern void *DevHelp_DynamicAPI(short Parm_Count, void *Worker);
extern void _loadds _saveregs Worker();
#endif

#endif /*pddentry_h*/
