/***************************************************************************
*
* Module Name: vtimer.h
*
* OS/2 public header file.
*
* MVDM Virtual Timer Exported Definitions
* Copyright (c) 1993  IBM Corporation
*
*****************************************************************************/

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
/* INC */

#ifndef __VTIMER__
#define __VTIMER__

#ifndef INCL_NONE               /* include everything by default  */
#define INCL_VTIMER
#endif

#ifdef  INCL_VTIMER
#define INCL_VTIMERSYSREQ
#define INCL_VTIMERINTERFACES
#endif



#ifdef INCL_VTIMERSYSREQ

/***    VTIMER constants
 */

#define VTIMER_NAME             "VTIMER$"

/* VTIMER functions for use by PMVDMP
 */
#define VTMRSYSREQ_SETACCESS    1   /* Obtain exclusive access for VDM    */
#define VTMRSYSREQ_SETFOCUS     2   /* Notify VDM is gaining/losing focus */
#define VTMRSYSREQ_MAX          2

/* VDMA-VTIMER communication commands */

#define VTMR_CMD_INIT       0     /* Init command to get the comm. routine    */
#define VTMR_CMD_START      1     /* Start sending the interrupt notification */
#define VTMR_CMD_STOP       2     /* Stop sending the interrupt notification  */

/* VWIN-VTIMER communication commands */

#define VTMR_CMD_SEAMLESS   3     /* Notification from VWIN for SEAMLESS      */

/***    VTIMER data types
 */

#endif /* INCL_VTIMERSYSREQ */


/***    VTIMER function prototypes
 */

#ifdef INCL_VTIMERINTERFACES

#endif

#endif /* __VTIMER__ */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
