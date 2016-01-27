/****************************** Module Header ******************************\
*
* Module Name: PM.H
*
* OS/2 Presentation Manager top level include file.
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The following symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_PM                  ALL of OS/2 Presentation Manager
*   INCL_AVIO                OS/2 Advanced VIO (16-Bit only)
*   INCL_BITMAPFILEFORMAT    OS/2 Bitmap File Format
*   INCL_DEV                 OS/2 Device Support
*   INCL_ERRORS              OS/2 Errors
*   INCL_FONTFILEFORMAT      OS/2 Font File Format
*   INCL_GPI                 OS/2 GPI
*   INCL_ORDERS              OS/2 Graphical Order Formats
*   INCL_PIC                 OS/2 Picture utilities
*   INCL_SPL                 OS/2 Spooler
*   INCL_WINSTDDLGS          CUA controls and dialogs
*   INCL_WIN                 OS/2 Window Manager
*   INCL_WINWORKPLACE        OS/2 Workplace Shell
*
* ===========================================================================
*
* Comments at the end of each typedef line give the name tags used in
* the assembler include version of this file.
*
* The assembler include version of this file excludes lines between NOINC
* and INC comments.
*
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

#define PM_INCLUDED


/*** if INCL_PM defined then define all the symbols ************************/
#ifdef INCL_PM
   #define INCL_AVIO
   #define INCL_BITMAPFILEFORMAT
   #define INCL_DEV
   #define INCL_ERRORS
   #define INCL_FONTFILEFORMAT
   #define INCL_GPI
   #define INCL_ORDERS
   #define INCL_PIC
   #define INCL_SPL
   #define INCL_WIN
   #define INCL_WINSTDDLGS
   #define INCL_WINWORKPLACE
#endif /* INCL_PM */


#include <pmwin.h>        /* OS/2 Window Manager definitions    */
#include <pmgpi.h>        /* OS/2 GPI definitions               */
#include <pmdev.h>        /* OS/2 Device Context definitions    */
#include <pmwp.h>         /* OS/2 Workplace Shell               */

#ifdef INCL_AVIO
   #include <pmavio.h>    /* OS/2 AVIO definitions              */
#endif

#ifdef INCL_SPL
   #include <pmspl.h>     /* OS/2 Spooler definitions           */
#endif

#ifdef INCL_PIC
   #include <pmpic.h>     /* OS/2 Picture Utilities definitions */
#endif

#ifdef INCL_ORDERS
   #include <pmord.h>     /* OS/2 Graphical Order Formats       */
#endif

#ifdef INCL_FONTFILEFORMAT
   #include <pmfont.h>    /* OS/2 Font File Format definition   */
#endif

#include <pmstddlg.h>     /* CUA controls and dialogs           */

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

/**************************** end of file **********************************/
