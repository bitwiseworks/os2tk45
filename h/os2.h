/*static char *SCCSID = "@(#)os2.h      6.6 92/03/15";*/
/****************************** Module Header ******************************\
*                                                                          *
* Copyright (c) 1987  - 1992 IBM Corporation                               *
*                                                                          *
* Module Name: OS2.H                                                       *
*                                                                          *
* This is the top level include file that includes all the files necessary *
* for writing an OS/2 application.                                         *
*                                                                          *
\***************************************************************************/

#ifndef __OS2_H__

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

#define __OS2_H__
/* INC */

#define OS2_INCLUDED

/* Common definitions */
#include <os2def.h>

/* OS/2 Base Include File */
#ifndef INCL_NOBASEAPI
   #include <bse.h>
#endif /* INCL_NOBASEAPI */

/* OS/2 Presentation Manager Include File */
#ifndef INCL_NOPMAPI
   #include <pm.h>
#endif /* INCL_NOPMAPI */

/* NOINC */
/* Open Scripting Architecture (OSA) Include File */
#ifdef INCL_OSAAPI
   #include <osa.h>
#endif /* INCL_OSAAPI */

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

/* OpenDoc for OS/2 Include File */
#ifdef __cplusplus
#ifdef INCL_ODAPI
   #include <od.h>
#endif /* INCL_ODAPI */
#endif
/* INC */

#endif /* __OS2__ */
