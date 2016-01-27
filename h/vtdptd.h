/***************************************************************************
*
* Module Name: Vtdpdt.h
*
* OS/2 public header file.
*
* Copyright (c) 1993  IBM Corporation
*
* Virtual Timer Device Driver VDD/PDD Communication Interface
*
*      All Rights Reserved
*
*
*/

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
/* INC */

#ifndef __VTDPTD__
#define __VTDPTD__

// Floppy PDD component definitions

#define PTD_NAME        "TIMER$"


/***    PDD function commands
 *
 *  Format:
 *      (*fpfnPTimerProc)(PTDCMD_*, ul1, ul2);
 *
 *  PTDCMD_REGISTER:
 *      ENTRY
 *          ul1 == segment of 16:32 VDD entry point
 *          ul2 ==  offset of 16:32 VDD entry point
 *      EXIT
 *          returns TRUE
 *      CONTEXT
 *          Init-time
 *
 *  PTDCMD_DEREGISTER:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          returns TRUE
 *      CONTEXT
 *          Task-time
 *
 *  PTDCMD_ENABLETICK:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          returns TRUE
 *      CONTEXT
 *          VDM Task-time
 *
 *  PTDCMD_DISABLETICK:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          returns TRUE
 *      CONTEXT
 *          VDM Task-time
 */


#define PTDCMD_REGISTER         PDDCMD_REGISTER         //register VDD
#define PTDCMD_DEREGISTER       (PTDCMD_REGISTER+1)     //de-register of VDD
#define PTDCMD_ENABLETICK       (PTDCMD_REGISTER+2)     //enable tick int.
#define PTDCMD_DISABLETICK      (PTDCMD_REGISTER+3)     //disable tick int.

#define PTDCMD_LASTEVENT        PTDCMD_DISABLETICK


/***    VDD function commands
 *
 *  Format:
 *      (*fpfnVTimerProc)(VTDCMD_*, ul1, ul2);
 *
 *  VTDCMD_TICKEVENT:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          returns TRUE
 *      CONTEXT
 *          Interrupt-time
 *
 *  VTDCMD_PREEMPTT0:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          returns TRUE
 *      CONTEXT
 *          Task-time
 *
 *  VTDCMD_RELEASET0:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          returns TRUE
 *      CONTEXT
 *          Task-time
 *
 *  VTDCMD_PREEMPTT2:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          returns TRUE
 *      CONTEXT
 *          Task-time
 *
 *  VTDCMD_RELEASET2:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          returns TRUE
 *      CONTEXT
 *          Task-time
 */

#define VTDCMD_TICKEVENT        0x00
#define VTDCMD_PREEMPTT0        0x01
#define VTDCMD_RELEASET0        0x02
#define VTDCMD_PREEMPTT2        0x03
#define VTDCMD_RELEASET2        0x04

#define VTDCMD_LASTEVENT        VTDCMD_RELEASET2

#endif /* __VTDPTD__ */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
