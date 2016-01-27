/*static char *SCCSID = "@(#)convproc.h	13.2 92/03/06";*/

/********************** START OF SPECIFICATIONS *****************************/
/*                                                                          */
/* SOURCE FILE NAME: CONVPROC.H                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Case Converter Input/Output (CONV) include file.       */
/*                                                                          */
/* COPYRIGHT:                                                               */
/*              Copyright (c) IBM Corporation 1992, 1993                    */
/*                        All Rights Reserved                               */
/*                                                                          */
/* STATUS: MMPM/2 Release 1.10                                              */
/*                                                                          */
/* ABSTRACT: Contains data structures and prototypes that are used in       */
/*           CONV routines.                                                 */
/*                                                                          */
/*                                                                          */
/*********************** END OF SPECIFICATIONS ******************************/

#ifndef _CONV_H_
#define _CONV_H_

#pragma pack(4)

#if defined(INCL_CONVPROC)

// Defines needed for CONV proc

#define DOS_FILE_HANDLE             0            // adwInfo[0] holds DOS handle
#define STORAGE_SYS_HMMIO           1            // adwInfo[1] hold SS hmmio
#define MMIO_USING_BUFFERED_IO      0X00800000L  // buffered I/O flag
#define MMIO_PSEUDO_READ            0X00400000L  // WRITEONLY w/ buffered I/O
#define HEADER_TEXT_SIZE            29           // size of Text in header
#define CONV_MODIFIED_FILE          1            // flag for file modification

#define HEADER_STRING               "LOWER TO UP IO PROC SAMPLE.."

// Messages for CONV Proc

#define CONVM_TOUPPER               MMIOM_USER + 1    // use MMIOM_USER 
#define CONVM_TOLOWER               MMIOM_USER + 2    // from MMIO.H 

typedef struct _CONVPROCINFO
   {
   HFILE hFileHandle;
   HMMIO hmmioSS;
   ULONG ulDataSize;
   } CONVPROCINFO;

typedef CONVPROCINFO *PCONVPROCINFO;

typedef struct _CONVHEADERINFO
   {
   ULONG ulHeaderLength;
   ULONG ulFlags;
   CHAR  szHeaderText[HEADER_TEXT_SIZE];
   } CONVHEADERINFO;

typedef CONVHEADERINFO *PCONVHEADERINFO;

// Function protoypes for CONV proc

LONG APIENTRY CONVProc( PVOID pmmioStr,
                        USHORT  usMsg,
                        LONG lParam1,
                        LONG lParam2 );

ULONG APIENTRY convhlpGetFormatStringLength( FOURCC fccIOProc,
                                             PLONG plNameLength );

LONG APIENTRY convhlpGetFormatString( FOURCC fccIOProc,
                                      PSZ pszFormatString,
                                      LONG lBytes );

ULONG APIENTRY convhlpGetNLSData( PULONG pulCodePage,
                                  PULONG pulLanguage );

VOID APIENTRY convhlpToUpper( PUCHAR psz );

VOID APIENTRY convhlpToLower( PUCHAR psz );

#endif    // INCL_CONVPROC

// Defines for the resource file - CONVPROC.RC

#define CONV_IOPROC_NAME_TABLE   7000
#define CONV_NLS_CHARSET_INFO    7500
#define FOURCC_FFT               mmioFOURCC( 'F','F','T',' ' )
#define HEX_FOURCC_FFT           0x20544646L

#pragma pack()

#endif   // _CONV_H_
