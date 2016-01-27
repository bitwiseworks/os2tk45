/***************************************************************************/
/* IBM CONFIDENTIAL                                                        */
/* OCO Source Materials                                                    */
/* IBM TCP/IP for OS/2                                                     */
/* (c) Copyright IBM Corp 1992,1997                                        */
/* The source code for this program is not published or otherwise          */
/* divested of its trade secrets, irrespective of what has been            */
/* deposited with the U.S. Copyright Office                                */
/***************************************************************************/
/*
 ******************************************************************************
 *
 *  $Author:   SANJAY_KHANNA  $
 *  $Revision:   1.0  $
 *  $Modtime:   25 Aug 1994 11:22:14  $
 *
 *  $Header:   T:\tcp20vcs\basekit\src\convstck\sockets\dprintf.h_v   1.0   06 Oct 1994 13:33:48   SANJAY_KHANNA  $
 *
 *  $Log:   T:\tcp20vcs\basekit\src\convstck\sockets\dprintf.h_v  $
 *
 *    Rev 1.0   06 Oct 1994 13:33:48   SANJAY_KHANNA
 * Initial revision.
 *
 *    Rev 1.2   24 Jan 1994 18:12:06   Jonathan_Rosen
 * - Added D_SLEEP tracing.
 *
 *    Rev 1.1   09 Nov 1993 11:02:26   Jonathan_Rosen
 * - Added prototype for getdbgbuf() to request a copy of the current internal
 *   debug buffer.
 *
 *    Rev 1.0   28 Oct 1993 07:56:36   Jonathan_Rosen
 * Initial revision.
 *
 ******************************************************************************
 ******************************************************************************
 */

#define  DPRINTF(_d, _f, _p)      {if((_f) & (_d)) dprintf##_p;}
#define  DVPRINTF(_c, _d, _f, _x, _y) {if((_f) & (_d)) dvprintf(-c, _d, _x, _y);}
#define  IFD(_d, _f, _s)          {if((_f) & (_d)) _s;}


#define  D_SOCKET       0x00000001
#define  D_SLEEP        0x00000002
#define  D_AFINET       0x00000004         // Added for (debug_flag & D_FLAG) for afos2
#define  D_AFOS2        0x00000008         // Added for (debug_flag & D_FLAG) for afinet
#define  D_TCPDEBUG     0x00000010         // Added for (debug_flag & D_FLAG) for TCPDEBUG
#define  D_TOBUFFER     0x00800000
#define  D_ALL          0x00ffffff
#define  D_NOCKSUM      0x01000000


#ifdef SOCKETS
extern long dvprintf();
//extern int dprintf();
extern long dprintf(short ComPort, u_long debug_flag, char  *fmt, ...);
extern u_short getdbgbuf(char *buf, u_short len);
extern u_long dbgexp(char *str);
extern char * dbgflags(char *buf, u_long debug);

extern char *DbgBuf;
extern char *DbgBufPtr;

#else

#ifndef _16BITCODE
extern unsigned long *_debug_flag_p;
#undef debug_flag
#define debug_flag *_debug_flag_p

#else

extern unsigned long *debug_flag_p;
#undef debug_flag
#define debug_flag *debug_flag_p

#endif
#endif




