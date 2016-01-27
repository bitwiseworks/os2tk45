/***************************************************************************
*
* Module Name: VFDPDF.H
*
* OS/2 public header file.
*
* Copyright (c) 1993  IBM Corporation
*
* Virtual Floppy Device Driver VDD/PDD Communication Interface
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

#ifndef __VFDPFD__
#define __VFDPFD__

/* Floppy PDD component definitions */

#define FLPY_NAME       "FLPY$"


/***    PDD function commands
 *
 *  Format:
 *      (*fpfnPFDProc)(PFDCMD_*, ul1, ul2);
 *
 *  PFDCMD_REGISTER:
 *      ENTRY
 *          ul1 == segment of 16:32 VDD entry point
 *          ul2 ==  offset of 16:32 VDD entry point
 *      EXIT
 *          Returns TRUE
 *      CONTEXT
 *          Init-time
 *
 *  PFDCMD_DEREGISTER:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          Returns TRUE
 *      CONTEXT
 *          Task-time
 *
 *  PFDCMD_REQFLOPPY:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT-SUCCESS
 *          returns TRUE - ownership granted
 *      EXIT-FAILURE
 *          return FALSE - floppy currently busy
 *                         delay ownership notification required
 *      CONTEXT
 *          VDM Task-time
 *      NOTE
 *          This request is non-blocking.
 *
 *  PFDCMD_FREEFLOPPY:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT
 *          Returns TRUE
 *      CONTEXT
 *          VDM Task-time
 *      NOTE
 *          VFD may call PFD to free the floppy even if PFD has never
 *          requested it.  This may occur if the VDM owner has died
 *          in which case the ownership is returned to the PFD.
 */


#define PFDCMD_REGISTER         PDDCMD_REGISTER         /* register VDD          */
#define PFDCMD_DEREGISTER       (PFDCMD_REGISTER+1)     /* de-register of VDD    */
#define PFDCMD_REQFLOPPY        (PFDCMD_REGISTER+2)     /* req. floppy ownership */
#define PFDCMD_FREEFLOPPY       (PFDCMD_REGISTER+3)     /* free floppy ownership */
#define PFDCMD_HOOKIRQ6         (PFDCMD_REGISTER+4)     /* HOOK IRQ6 INTS */

#define PFDCMD_LASTEVENT        PFDCMD_FREEFLOPPY


/***    VDD function commands
 *
 *  Format:
 *      (*fpfnVFDProc)(VFDCMD_*, ul1, ul2);
 *
 *  VFDCMD_REQFLOPPY:
 *      ENTRY
 *          ul1 == 0
 *          ul2 == 0
 *      EXIT-SUCCESS
 *          returns TRUE - ownership granted
 *      EXIT-FAILURE
 *          returns FALSE - floppy currently busy
 *                          delay ownership notification required
 *      CONTEXT
 *          Task-time
 *          Interrupt-time
 *      NOTE
 *          This request is non-blocking.
 *
 *  VFDCMD_FREEFLOPPY:
 *      ENTRY
 *          ul1 == 0
 *          ul2 == 0
 *      EXIT-SUCCESS
 *          Returns TRUE
 *      EXIT-FAILURE
 *          Returns FALSE - ownership not accepted because requester has died
 *      CONTEXT
 *          Task-time
 *          Interrupt-time
 *
 *  VFDCMD_INTERRUPTEVENT:
 *      ENTRY
 *          ul1 == 0
 *          ul2 == 0
 *      EXIT
 *          Returns TRUE
 *      CONTEXT
 *          Interrupt-time
 */

#define VFDCMD_REQFLOPPY        0x00
#define VFDCMD_FREEFLOPPY       0x01
#define VFDCMD_INTERRUPTEVENT   0x02

#define VFDCMD_LASTEVENT        VFDCMD_INTERRUPTEVENT

#endif /* __VFDPFD__ */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
