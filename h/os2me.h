/****************************************************************************/
/*                                                                          */
/* Module Name: OS2ME.H                                                     */
/*                                                                          */
/* OS/2 2.0 Multimedia Extensions top level include file that includes all  */
/* the files necessary for writing an OS/2 Multimedia application.          */
/*                                                                          */
/*        Copyright (c) International Business Machines Corporation         */
/*                          1991, 1992, 1993                                */
/*                        All Rights Reserved                               */
/*                                                                          */
/****************************************************************************/
#ifdef __cplusplus
   extern "C" {
   #endif
   
   
   #define OS2ME_INCLUDED
   
   
   /* OS/2 Type Headers for MCI and MMIO           */
   #ifdef INCL_OS2MM
      #define INCL_MCIOS2
      #define INCL_MMIOOS2
   #endif
   
   /* Common definitions */
   #include <os2medef.h>
   #include <meerror.h>
   
   /* Performance stub for pagetuned pragma header files */
   #ifdef INCL_MM_PERFSTUB
      #ifndef PERFSTUB_INCLUDED
         #define  PERFSTUB_INCLUDED
         #include "perfstub.h"
      #endif
   #endif
   
   #ifdef INCL_MACHDR
      #include <mciapi.h>
   #endif
   
   /* OS/2 SyncStream Manager Include File */
   #ifndef _SSM_H_
      #include <ssm.h>
   #endif
   
   /* OS/2 Media Control Manager Include File */
   #ifdef INCL_MCIOS2
      #include <mcios2.h>                   /* OS/2 MCI Application interface */
      #include <mmdrvos2.h>                 /* OS/2 MCI driver include file   */
   #else
      #include <mmsystem.h>                 /* MCI Application interface */
      #include <mcidrv.h>                   /* MCI driver include file   */
   #endif
   
   /* OS/2 Multimedia IO Services Include File */
   #ifdef INCL_MMIO
      #include <mmio.h>
      #ifdef INCL_MMIO_CODEC
         #include <codec.h>
      #endif
   #else
      #ifdef INCL_MMIOOS2
         #include <mmioos2.h>
         #ifdef INCL_MMIO_CODEC
            #include <codec.h>
         #endif
      #endif
   #endif
   
   /* OS/2 Multimedia Secondary Windows Include File */
   #ifdef INCL_SW
      #define INCL_SECONDARYWINDOW
      #define INCL_GRAPHICBUTTON
      #define INCL_CIRCULARSLIDER
      
      #include <sw.h>
   #endif
   
   #ifdef __cplusplus
   }
#endif

