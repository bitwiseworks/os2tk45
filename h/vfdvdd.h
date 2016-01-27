/***************************************************************************
*
* Module Name: Vfdvdd.h
*
* OS/2 public header file.
*
* Copyright (c) 1987 - 1992  IBM Corporation
*
* Virtual Floppy Device Driver VFD/VDD Communication Interface
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

#ifndef __VFDVDD__
#define __VFDVDD__

#define VFLPY_NAME      "VFLPY$"


/***    VDD Requests
 *
 *  Format:
 *      (*pfnVDDReqProc)(hvdm, VFVDDREQ_*, ul1, ul2);
 *
 *  VFVDDREQ_MOTOROFF:
 *      ENTRY
 *          hvdm-> VDM
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *      EXIT-SUCCESS
 *          Returns TRUE
 *      EXIT-FAILURE
 *          Returns FALSE
 *      CONTEXT
 *          VDM Task-time
 */

#define VFVDDREQ_MOTOROFF       0x00            /* turn floppy motor off */
#define RESUME_PDD_CMD          1               /* Resume PDD ownership of floppy */  // @1809

#define ENTER_VFLPY 0                  // Used by VDSK to serialize access to VFLPY
#define EXIT_VFLPY  1

#endif /* __VFDVDD__ */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
