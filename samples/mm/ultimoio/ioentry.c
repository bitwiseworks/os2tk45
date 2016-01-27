/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IOENTRY.C                                           */
/*                                                                        */
/* DESCRIPTIVE NAME: FileFormat IO Proc Main (Entry Point) Module         */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/* FUNCTION: This source module contains the entry point for the IO Proc. */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*      IOProc_Entry                                                      */
/*      IOInvalidEntry                                                    */
/*                                                                        */
/************************** END OF SPECIFICATIONS *************************/

#include        <stdio.h>
#include        <string.h>
#include        <stdlib.h>
#include        <memory.h>

#define         INCL_DOS                        /* #define  INCL_DOSPROCESS.*/
#define         INCL_GPIBITMAPS
#define         INCL_ERRORS
#define         INCL_WIN
#define         INCL_GPI
#include        <os2.h>                         /* OS/2 headers.            */
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>                       /* Multi-Media IO extensions.*/
#include        <hhpheap.h>
#include        <ioi.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: IOProc_Entry                                            */
/*                                                                          */
/* DESCRIPTIVE NAME: IO Proc entrypoint.                                    */
/*                                                                          */
/* FUNCTION: This function provides access to the functionality of this     */
/*           IO Proc. This is the sole entrypoint.                          */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProc_Entry                                                */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              PVOID   pmmioStr   - Pointer to MMIOINFO staus structure    */
/*              WORD    wMessage   - MMIO Message request number            */
/*              LONG    lParam1    - Primary message dependent parameter    */
/*              LONG    lParam2    - Secondary message dependent parameter  */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIOERR_UNSUPPORTED_MESSAGE                                 */
/*              Message specific error                                      */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

LONG APIENTRY IOProc_Entry ( PVOID pmmioStr,
                             WORD wMessage,
                             LONG lParam1,
                             LONG lParam2  )
{
   LONG        lReserved = 0;
   LONG        lRetCode;                         /* Return code.            */
   PMMIOINFO   pmmioinfo;                        /* Local work structure.   */
   PINSTANCE   pinstance;

   // Clear the error return before anything happens to insure valid results.

   if (pmmioStr) {
      pmmioinfo = (PMMIOINFO) pmmioStr;
      pmmioinfo->ulErrorRet = MMIO_SUCCESS;
      }
   else {
      pmmioinfo = NULL;
      }

   switch (wMessage) {
      /*****************************************************/
      /* Standard IO Proc Messages that must be supported. */
      /*****************************************************/

      case MMIOM_OPEN :     /* Open file, setup for read. */
         if ( ffiofnct.pfnIOProcOpen ) {
            lRetCode = ffiofnct.pfnIOProcOpen(pmmioinfo,(PSZ)lParam1);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_CLOSE :    /* Close and free buffers. */
         if ( ffiofnct.pfnIOProcClose ) {
            lRetCode = ffiofnct.pfnIOProcClose(pmmioinfo);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_SAVE:
         if ( ffiofnct.pfnIOProcSave ) {
            lRetCode = ffiofnct.pfnIOProcSave(pmmioinfo,(PSZ)lParam1);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_READ :     /* Read file. */
         if ( ffiofnct.pfnIOProcRead ) {
            lRetCode = ffiofnct.pfnIOProcRead(pmmioinfo,(PSZ)lParam1,lParam2);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_WRITE:
         if ( ffiofnct.pfnIOProcWrite ) {
            lRetCode = ffiofnct.pfnIOProcWrite(pmmioinfo,(PSZ)lParam1,lParam2);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_SEEK :     /* Resets file postition to indicate offset. */
         if ( ffiofnct.pfnIOProcSeek ) {
            lRetCode = ffiofnct.pfnIOProcSeek(pmmioinfo,lParam1,lParam2);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_SEEKBYTIME:
         if ( ffiofnct.pfnIOProcSeekByTime ) {
            lRetCode = ffiofnct.pfnIOProcSeekByTime(pmmioinfo,(PSZ)lParam1, lParam2);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_GETFORMATNAME :     /* Text describes File format. */
         if ( ffiofnct.pfnIOProcGetFormatName ) {
            lRetCode = ffiofnct.pfnIOProcGetFormatName((PSZ)lParam1,lParam2);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_GETFORMATINFO :     /* Get file format information. */
         if ( ffiofnct.pfnIOProcGetFormatInfo ) {
            lRetCode = ffiofnct.pfnIOProcGetFormatInfo((PMMFORMATINFO)lParam1);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_GETHEADER :     /* Get File or Track header */
         if ( ffiofnct.pfnIOProcGetHeader ) {
            lRetCode = ffiofnct.pfnIOProcGetHeader(pmmioinfo,(PVOID)lParam1,lParam2,lReserved);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_IDENTIFYFILE :     /* Determine if this IO Proc can process this file  */
         if ( ffiofnct.pfnIOProcIdentifyFile ) {
            lRetCode = ffiofnct.pfnIOProcIdentifyFile(pmmioinfo,(PSZ)lParam1,(HMMIO)lParam2);
         }
         else {
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance,
                                       wMessage, lParam1, lParam2 );
         }
         break;

      case MMIOM_QUERYHEADERLENGTH :     /* Query length of file or track header */
         if ( ffiofnct.pfnIOProcQueryHeaderLength )
            lRetCode = ffiofnct.pfnIOProcQueryHeaderLength(pmmioinfo,lReserved);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_SET:     /* general query and set function */
         if ( ffiofnct.pfnIOProcSet )
            lRetCode = ffiofnct.pfnIOProcSet(pmmioinfo,lParam1,lParam2);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_BEGININSERT:
         if ( ffiofnct.pfnIOProcBeginInsert )
            lRetCode = ffiofnct.pfnIOProcBeginInsert(pmmioinfo,(PSZ)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_ENDINSERT:
         if ( ffiofnct.pfnIOProcEndInsert )
            lRetCode = ffiofnct.pfnIOProcEndInsert(pmmioinfo,(PSZ)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_SETHEADER :     /* Set File or Track header */
         if ( ffiofnct.pfnIOProcSetHeader )
            lRetCode = ffiofnct.pfnIOProcSetHeader(pmmioinfo,(PVOID)lParam1,lParam2,lReserved);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      /*****************************************************/
      /* SendMsg IO Proc Messages supported.               */
      /*****************************************************/

      case MMIOM_MULTITRACKREAD:     /* Read multiple tracks by track  */
         if ( ffiofnct.pfnIOProcMultiTrackRead )
            lRetCode = ffiofnct.pfnIOProcMultiTrackRead(pmmioinfo,(PMMMULTITRACKREAD)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_MULTITRACKWRITE:    /* Write multiple tracks by track */
         if ( ffiofnct.pfnIOProcMultiTrackWrite )
            lRetCode = ffiofnct.pfnIOProcMultiTrackWrite(pmmioinfo,(PMMMULTITRACKWRITE)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_COMPRESS:  /* compress buffer */
         if ( ffiofnct.pfnIOProcCompress )
            lRetCode = ffiofnct.pfnIOProcCompress(pmmioinfo,(PSZ)lParam1);
         else
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_DECOMPRESS:  /* Decompress a buffer */
         if ( ffiofnct.pfnIOProcDecompress )
            lRetCode = ffiofnct.pfnIOProcDecompress(pmmioinfo,(PMMDECOMPRESS)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_TEMPCHANGE:
         if ( ffiofnct.pfnIOProcTempChange )
            lRetCode = ffiofnct.pfnIOProcTempChange(pmmioinfo,(PSZ)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_COPY:
         if (ffiofnct.pfnIOProcCopy)
            lRetCode = ffiofnct.pfnIOProcCopy(pmmioinfo,(PMMIO_EDIT_PARMS)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_CUT:
         if (ffiofnct.pfnIOProcCut)
            lRetCode = ffiofnct.pfnIOProcCut(pmmioinfo,(PMMIO_EDIT_PARMS)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_PASTE:
         if (ffiofnct.pfnIOProcPaste)
            lRetCode = ffiofnct.pfnIOProcPaste(pmmioinfo,(PMMIO_EDIT_PARMS)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_CLEAR:
         if (ffiofnct.pfnIOProcClear)
            lRetCode = ffiofnct.pfnIOProcClear(pmmioinfo,(PMMIO_EDIT_PARMS)lParam1);
         else
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_UNDO:
         if (ffiofnct.pfnIOProcUndo)
            lRetCode = ffiofnct.pfnIOProcUndo(pmmioinfo,(PMMIO_EDIT_PARMS)lParam1);
         else
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_REDO:
         if (ffiofnct.pfnIOProcRedo)
            lRetCode = ffiofnct.pfnIOProcRedo(pmmioinfo,(PMMIO_EDIT_PARMS)lParam1);
         else
            lRetCode = ioInvalidEntry( pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_STATUS:
         if (ffiofnct.pfnIOProcStatus)
            lRetCode = ffiofnct.pfnIOProcStatus(pmmioinfo, lParam1, (PMMIO_STATUS_PARMS)lParam2);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      case MMIOM_WINMSG:
         if (ffiofnct.pfnIOProcWinMsg)
            lRetCode = ffiofnct.pfnIOProcWinMsg(pmmioinfo, (PMMIO_WINMSG)lParam1);
         else
            lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      default :

         lRetCode = ioInvalidEntry(pmmioinfo, &pinstance, wMessage, lParam1, lParam2);
         break;

      } /* end switch */

   return lRetCode;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioInvalidEntry                                          */
/*                                                                          */
/* DESCRIPTIVE NAME: IO Proc invalid entrypoint handler.                    */
/*                                                                          */
/* FUNCTION: If an IO Proc has a child IO Proc, then instead of             */
/*           returning UNSUPPORTED_MESSAGE, send the message to             */
/*           the child IO Proc to see if it can understand and              */
/*           process the message.                                           */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: ioInvalidEntry                                              */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*           PMMIOINFO pmmioinfo  - MMIOINFO instant structure.             */
/*           PINSTANCE *pinstance - ptr to instance structure.              */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIOERR_UNSUPPORTED_MESSAGE                                 */
/*              Message specific error                                      */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
LONG ioInvalidEntry( PMMIOINFO pmmioinfo,
                     PINSTANCE *ppinstance,
                     WORD wMessage,
                     LONG lParam1,
                     LONG lParam2 )
{
   LONG        lRetCode;

   if ( lRetCode = ioGetPtrInstance( pmmioinfo, ppinstance ))
      return( lRetCode );

   if ( (*ppinstance)->hmmioFileHandle ) {
      return( mmioSendMessage( (*ppinstance)->hmmioFileHandle,
                               wMessage,
                               lParam1,
                               lParam2 ));
      }
   else {
      pmmioinfo->ulErrorRet = MMIOERR_UNSUPPORTED_MESSAGE;
      return (MMIOERR_UNSUPPORTED_MESSAGE);
      }
}
