/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  ULGDAT.C                                            */
/*                                                                        */
/* DESCRIPTIVE NAME: Ultimotion IO Proc Global variables                  */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/************************* END OF SPECIFICATIONS **************************/

#include        <stdio.h>

#define         INCL_DOS                       /* #define  INCL_DOSPROCESS. */
#define         INCL_ERRORS
#define         INCL_WIN
#define         INCL_GPI
#include        <os2.h>                        /* OS/2 headers.             */
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>                      /* Multi-Media IO extensions.*/
#include        <hhpheap.h>
#include        <ioi.h>

/********************************************************************/
/*  FILE FORMAT SPECIFIC                                            */
/*  DLL Global Process Instance Data - one copy per each process    */
/********************************************************************/

/************************************************************************************/
/*  FILE FORMAT SPECIFIC                                                            */
/* Global Shared Data Segment - File Format specific enabled capabilities           */
/*    - typedef in ioi.h declared here to be FFIO specific and externed back        */
/************************************************************************************/

#pragma data_seg (SHR_SEG)           // Put this into a seperate data seg

FFIOFNCT ffiofnct = {

   (PFN)NULL,                          // MMIOM_GETCF
   (PFN)NULL,                          // MMIOM_GETCFENTRY

   (PFN)IOProcOpen,                    // MMIOM_CLOSE
   (PFN)IOProcClose,                   // MMIOM_OPEN
   (PFN)IOProcRead,                    // MMIOM_READ
   (PFN)IOProcSeek,                    // MMIOM_SEEK
   (PFN)NULL,                          // MMIOM_WRITE

   (PFN)IOProcIdentifyFile,            // MMIOM_IDENTIFYFILE
   (PFN)IOProcGetHeader,               // MMIOM_GETHEADER
   (PFN)NULL,                          // MMIOM_SETHEADER
   (PFN)IOProcQueryHeaderLength,       // MMIOM_QUERYHEADERLENGTH
   (PFN)IOProcGetFormatName,           // MMIOM_GETFORMATNAME
   (PFN)IOProcGetFormatInfo,           // MMIOM_GETFORMATINFO
   (PFN)IOProcSeek,                    // MMIOM_SEEKBYTIME
   (PFN)NULL,                          // MMIOM_TEMPCHANGE
   (PFN)NULL,                          // MMIOM_BEGININSERT
   (PFN)NULL,                          // MMIOM_ENDINSERT
   (PFN)NULL,                          // MMIOM_SAVE
   (PFN)IOProcSet,                     // MMIOM_SET
   (PFN)NULL,                          // MMIOM_COMPRESS
   (PFN)IOProcDecompress,              // MMIOM_DECOMPRESS
   (PFN)IOProcMultiTrackRead,          // MMIOM_MULTITRACKREAD
   (PFN)NULL,                          // MMIOM_MULTITRACKWRITE

   (PFN)NULL,                          // MMOIM_DELETE
   (PFN)NULL,                          // MMIOM_BEGINGROUP
   (PFN)NULL,                          // MMIOM_ENDGROUP
   (PFN)NULL,                          // MMIOM_UNDO
   (PFN)NULL,                          // MMIOM_REDO
   (PFN)NULL,                          // MMIOM_BEGINSTREAM
   (PFN)NULL,                          // MMIOM_ENDSTREAM

   (PFN)NULL,                          // MMIOM_COPY
   (PFN)NULL,                          // MMIOM_CUT
   (PFN)NULL,                          // MMIOM_PASTE
   (PFN)NULL,                          // MMIOM_CLEAR
   (PFN)NULL,                          // MMIOM_STATUS
   (PFN)NULL                           // MMIOM_WINMSG
};

#pragma data_seg ()

/****************************************************************************/
/*              CODEC ini file info structure                               */
/*                                                                          */
/* *NOTE* - This table should be represented in the MMIO.INI file instead of*/
/*          HARD-CODED in this IO PROC.                                     */
/****************************************************************************/
CODECINIFILEINFO acifiTable[] = {
   {
      sizeof(CODECINIFILEINFO),
      FOURCC_FFIO,
      "ULBDC4",                                     /* szDCIODLLName[] - Decompression IO Proc DLL name */
      "CodecEntry",                                 /* szDCIOProcName[] - Decompression IO Proc entry point procedure name */
      UM_VIDEO_COMPRESSION_TYPE_BH146,              /* ulDecompressionType - Id of each decompression type */
      0L,
      MMIO_MEDIATYPE_DIGITALVIDEO,
      CODEC_DECOMPRESS+                             /* ulCapsFlags - Capabilities Flag */
         CODEC_SELFHEAL+                            /* ulCapsFlags - Capabilities Flag */
         CODEC_ORIGIN_UPPERLEFT+                    /* ulCapsFlags - Capabilities Flag */
         CODEC_4_BIT_COLOR,                         /* ulCapsFlags - Capabilities Flag */
      0,
      0,
      0,
      0,
      0,
      8,
      8,
      0,
   },
   {
      sizeof(CODECINIFILEINFO),
      FOURCC_FFIO,
      "ULBDC8",                                     /* szDCIODLLName[] - Decompression IO Proc DLL name */
      "CodecEntry",                                 /* szDCIOProcName[] - Decompression IO Proc entry point procedure name */
      UM_VIDEO_COMPRESSION_TYPE_BH146,              /* ulDecompressionType - Id of each decompression type */
      0L,
      MMIO_MEDIATYPE_DIGITALVIDEO,
      CODEC_DECOMPRESS+                             /* ulCapsFlags - Capabilities Flag */
         CODEC_SELFHEAL+                            /* ulCapsFlags - Capabilities Flag */
         CODEC_ORIGIN_UPPERLEFT+                    /* ulCapsFlags - Capabilities Flag */
         CODEC_MULAPERTURE+                         /* ulCapsFlags - Capabilities Flag */
         CODEC_DIRECT_DISPLAY+                      /* ulCapsFlags - Capabilities Flag */
         CODEC_8_BIT_COLOR,                         /* ulCapsFlags - Capabilities Flag */
      0,
      0,
      0,
      0,
      0,
      8,
      8,
      0,
   },
   {
      sizeof(CODECINIFILEINFO),
      FOURCC_FFIO,
      "ULBDC16",                                    /* szDCIODLLName[] - Decompression IO Proc DLL name */
      "CodecEntry",                                 /* szDCIOProcName[] - Decompression IO Proc entry point procedure name */
      UM_VIDEO_COMPRESSION_TYPE_BH146,              /* ulDecompressionType - Id of each decompression type */
      0L,
      MMIO_MEDIATYPE_DIGITALVIDEO,
      CODEC_DECOMPRESS+                             /* ulCapsFlags - Capabilities Flag */
         CODEC_SELFHEAL+                            /* ulCapsFlags - Capabilities Flag */
         CODEC_ORIGIN_UPPERLEFT+                    /* ulCapsFlags - Capabilities Flag */
         CODEC_DIRECT_DISPLAY+                      /* ulCapsFlags - Capabilities Flag */
         CODEC_16_BIT_COLOR,                        /* ulCapsFlags - Capabilities Flag */
      0,
      0,
      0,
      0,
      0,
      8,
      8,
      0,
   },
};

