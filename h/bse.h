/*static char *SCCSID = "@(#)bse.h      6.8 92/03/15";*/
/****************************** Module Header ******************************\
*
* Module Name: BSE.H
*
* This file includes the definitions necessary for writing Base OS/2 applications.
*
* Copyright (c) 1987, 1992  IBM Corporation
*
*
*
* ===========================================================================
*
* The following symbols are used in this file for conditional sections.
*
*   INCL_BASE      -  ALL of OS/2 Base
*   INCL_DOS       -  OS/2 DOS Kernel
*   INCL_SUB       -  OS/2 VIO/KBD/MOU
*   INCL_DOSERRORS -  OS/2 Errors         - only included if symbol defined
*   INCL_ORDINALS  -  OS/2 Ordinals       - only included if symbol defined
\***************************************************************************/

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
#ifdef __cplusplus
      extern "C" {
#endif
/* INC */

#ifndef __BSE__
/* NOINC */
#define __BSE__
/* INC */

#define INCL_BASEINCLUDED

/* if INCL_BASE defined then define all the symbols */

#ifdef INCL_BASE
   #define INCL_DOS
   #define INCL_SUB
   #define INCL_DOSERRORS
#endif /* INCL_BASE */

#include <bsedos.h>       /* Base definitions */


#ifdef INCL_DOSDEVIOCTL
   #include <bsedev.h>    /* Structures and constants for DosDevIOCtl */
#endif /* INCL_DOSDEVIOCTL */

#include <bsesub.h>       /* VIO/KBD/MOU definitions */
#include <bseerr.h>       /* Base error code definitions */

#ifdef INCL_ORDINALS
#include <bseord.h>     /* ordinals */
#endif /* INCL_ORDINALS */

#endif /* __BSE__ */

/* NOINC */
#ifdef __cplusplus
        }
#endif
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
