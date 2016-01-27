/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  ULIDENT.C                                           */
/*                                                                        */
/* DESCRIPTIVE NAME: Ultimotion IO Proc routine for MMIOM_IDENTIFYFILE    */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains the identify routine.            */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*     IOProcIdentifyFile                                                 */
/*                                                                        */
/************************* END OF SPECIFICATIONS **************************/


#include        <stdio.h>
#include        <string.h>
#include        <stdlib.h>
#include        <memory.h>

#define         INCL_DOS                        /* #define  INCL_DOSPROCESS.*/
#define         INCL_ERRORS
#define         INCL_WIN
#define         INCL_GPI
#include        <os2.h>                         /* OS/2 headers.            */
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>                       /* Multi-Media IO extensions.*/
#include        <ioi.h>



/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: IOProcIdentifyFile                                      */
/*                                                                          */
/* DESCRIPTIVE NAME: Identify file type.                                    */
/*                                                                          */
/* FUNCTION: This function tests if the file is Ultimotion type file.       */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcIdentifyFile                                          */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*        PMMIOINFO  pmmioinfo   - MMIOINFO structure. (Maybe NULL)         */
/*        PSZ        pszFileName - Pointer to file name.                    */
/*        HMMIO      FileHandle  - MMIO file handle obtained from mmioOpen. */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG IOProcIdentifyFile ( PMMIOINFO pmmioinfo,
                          PSZ pszFileName,
                          HMMIO FileHandle )

{
  ULONG                  rc;                         /* Return code.        */
  LONG                   lFilePosition;              /* Temp file position. */
  HMMIO                  WorkHandle;                 /* Local work handle.  */
  MMCKINFO               mmckinfo;


  WorkHandle = FileHandle;
 /***************************************************************************/
 /* file hasn't been open, must have file name else error returned.         */
 /***************************************************************************/
  if (WorkHandle == (HMMIO)0L){
      if (pszFileName == (PSZ) 0L){
          return(MMIOERR_INVALID_FILENAME);
          } /* end if */

      WorkHandle = mmioOpen(pszFileName, NULL,MMIO_NOIDENTIFY);

     /***********************************************************************/
     /* If open file failed, clean up IOProc instant struct, error returned.*/
     /***********************************************************************/
      if (WorkHandle == (HMMIO)0L){
         return(MMIO_ERROR);
          } /* end if */
      } /* end if */

 /***************************************************************************/
 /* if file has been opened, temporary resets file position to beginning    */
 /* of file.                                                                */
 /***************************************************************************/
  if (WorkHandle != (HMMIO)0L){
      lFilePosition = ioSeekFile((PLONG)&rc,WorkHandle,0L);
      if (lFilePosition < MMIO_SUCCESS) {
         return(MMIO_ERROR);
         } /* end if */
      } /* end if */

  /* Find SMV Riff chunk (This verifies the file is an SMV movie file also */
  memset (&mmckinfo, '\0', sizeof(MMCKINFO));
  mmckinfo.fccType = mmioStringToFOURCC(UMFORM_MOVIE ,0L);
  rc = mmioDescend(WorkHandle,&mmckinfo,NULL,MMIO_FINDRIFF);
  return (rc);

}
