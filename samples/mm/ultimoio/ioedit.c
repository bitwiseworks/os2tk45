/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IOEDIT.C                                            */
/*                                                                        */
/* DESCRIPTIVE NAME: Clipboard Editing support for SMV files              */
/*                                                                        */
/* COPYRIGHT:   Copyright (c) IBM Corporation  1993                       */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.1                                               */
/*                                                                        */
/* FUNCTION: This source module contains all the routines to support      */
/*           copy, cut, delete, paste, undo and redo.                     */
/*                                                                        */
/* NOTES:                                                                 */
/*    DEPENDENCIES: none                                                  */
/*    RESTRICTIONS: Runs in 32 bit protect mode (OS/2 2.0)                */
/*                                                                        */
/* ENTRY POINTS:                                                          */
/*      ioEditSetup                                                       */
/*      ioEditCleanup                                                     */
/*      ioEditFindHeaders                                                 */
/*      ioEditCloneEditList                                               */
/*      ioEditTimeToFrame                                                 */
/*      ioEditFrameToTime                                                 */
/*      ioEditDuration                                                    */
/*      ioEditGetFullName                                                 */
/*      IOProcWinMsg                                                      */
/*                                                                        */
/************************** END OF SPECIFICATIONS *************************/

#define         INCL_DOS
#define         INCL_ERRORS
#define         INCL_WIN
#define         INCL_GPI
#include        <os2.h>
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>
#include        <hhpheap.h>
#define         INCL_DBCSCRT
#include        <mmiocrt.h>
#include        <ioi.h>

   extern HHUGEHEAP  hheap;
   extern HMTX       hmtxGlobalHeap;

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioEditFindHeaders                                       */
/*                                                                          */
/* DESCRIPTIVE NAME: Find the video and audio headers                       */
/*                                                                          */
/* FUNCTION: Return the address of the video and audio headers.             */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

LONG ioEditFindHeaders(PINSTANCE pinstance, PMMVIDEOHEADER *pVideo,
                       PMMAUDIOHEADER *pAudio) {
   LONG           i;
   PTRACKI        pTrackList;
   PMMMOVIEHEADER pMovieHeader;
   PMMTRACKINFO   pTrackInfo;
   PMMVIDEOHEADER pTHVideo = NULL;
   PMMAUDIOHEADER pTHAudio = NULL;
   ULONG          numTracks;
   LONG           videoTrackID = -1, audioTrackID = -1;

   pMovieHeader = pinstance->pmmhdr;
   if (pMovieHeader == NULL) goto bad_structure;

   // only support one mandatory video track and one optional audio track

   numTracks = pMovieHeader->ulNumEntries;
   if ((numTracks == 0) || (numTracks > 2)) goto bad_structure;

   pTrackInfo = pMovieHeader->pmmTrackInfoList;
   if (pTrackInfo == NULL) goto bad_structure;

   for (i = 0; i < numTracks; i++, pTrackInfo++) { // get track IDs
      if (IS_VIDEO_TRACK(pTrackInfo->ulMediaType)) {
         if (videoTrackID == -1) videoTrackID = pTrackInfo->ulTrackID;
         else goto bad_structure;                  // 2nd video track
      }
      if (IS_AUDIO_TRACK(pTrackInfo->ulMediaType)) {
         if (audioTrackID == -1) audioTrackID = pTrackInfo->ulTrackID;
         else goto bad_structure;                  // 2nd audio track
      }
   }

   if (videoTrackID == -1) goto bad_structure;     // must have a video track

   pTrackList = pinstance->ptrackiList;            // linked list of TRACKI structures

   while (pTrackList != NULL) {
      if (IS_VIDEO_TRACK(pTrackList->ulMediaType))
         pTHVideo = (PMMVIDEOHEADER)(pTrackList->pTrackHeader);
      if (IS_AUDIO_TRACK(pTrackList->ulMediaType))
         pTHAudio = (PMMAUDIOHEADER)(pTrackList->pTrackHeader);
      pTrackList = pTrackList->ptrackiNext;
   }

   if (pTHVideo == NULL) goto bad_structure;

   *pVideo = pTHVideo;
   *pAudio = pTHAudio;

   return MMIO_SUCCESS;

bad_structure:                                     // error exit
   return MMIOERR_INVALID_STRUCTURE;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioEditSetup                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Prepare the Edit List                                  */
/*                                                                          */
/* FUNCTION: This function prepares the edit list and is called when the    */
/*           first editing message is received.                             */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

LONG ioEditSetup(PMMIOINFO pmmioinfo, PMMIO_EDIT_PARMS pParms) {
   LONG           rc;
   PEDITELEMENT   pNew;
   PINSTANCE      pinstance;
   PMMVIDEOHEADER pVideo;
   PMMAUDIOHEADER pAudio;

   rc = ioGetPtrInstance(pmmioinfo, &pinstance);
   if (rc != MMIO_SUCCESS) {
      pmmioinfo->ulErrorRet = rc;
      return MMIO_ERROR;
   }

   // make sure that the movie and track headers have been setup

   if (pinstance->pmmhdr == NULL) {
      pmmioinfo->ulErrorRet = MMIOERR_INVALID_STRUCTURE;
      return MMIO_ERROR;
   }

   rc = ioEditFindHeaders(pinstance, &pVideo, &pAudio);
   if (rc != MMIO_SUCCESS) {
      pmmioinfo->ulErrorRet = MMIOERR_INVALID_STRUCTURE;
      return MMIO_ERROR;
   }

   // if the 1st time, allocate a virgin EDITELEMENT

   pNew = pinstance->pEditList;          // pEditList is null if the file
   if (pNew == NULL) {                   // ... has not been edited
      ENTERCRITX;
      pNew = (PEDITELEMENT)HhpAllocMem(hheap, sizeof(EDITELEMENT));
      if (pNew == NULL) {
         EXITCRIT;
         pmmioinfo->ulErrorRet = MMIOERR_OUTOFMEMORY;
         return MMIO_ERROR;
      }
      EXITCRIT;

      // fill in the non-zero values

      pNew->ulDuration     = pVideo->ulTotalFrames;
      pNew->pInstance      = pinstance;
      pNew->ulFilePosition = pParms->ulCurrentFilePosition;

      pinstance->pEditList = pinstance->pCurrentElement = pNew;
   }

   pmmioinfo->ulErrorRet = 0;
   return MMIO_SUCCESS;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioEditCloneEditList                                     */
/*                                                                          */
/* DESCRIPTIVE NAME: Make a copy of the current edit list                   */
/*                                                                          */
/* FUNCTION: This function makes a copy of the edit list so that it         */
/*           can be placed on the undo list.                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

PEDITELEMENT ioEditCloneEditList(PINSTANCE pinstance) {
   LONG           rc;
   PEDITELEMENT   p1, p2, p3, pOld, pNew;

   pOld = pinstance->pEditList;          // be sure a list is there
   if (pOld == NULL) return NULL;

   ENTERCRIT(rc);
   if (rc) return NULL;

   // get storage for the first new EDITELEMENT

   pNew = (PEDITELEMENT)HhpAllocMem(hheap, sizeof(EDITELEMENT));
   if (pNew == NULL) {
         EXITCRIT;
         return NULL;                     // error exit
   }
   *pNew           = *pOld;               // initialize the first new EDITELEMENT
   pNew->pNext     = NULL;
   pNew->pPrevious = NULL;
   p1              = pOld->pNext;
   p3              = pNew;

   // get storage for the rest in the list

   while (p1 != NULL) {                   // p1 walks the old list

      p2 = (PEDITELEMENT)HhpAllocMem(hheap, sizeof(EDITELEMENT));
      if (p2 == NULL) {
         p1 = pNew;                       // free any gotten so far
         while (p1 != NULL) {
            p2 = p1->pNext;
            HhpFreeMem(hheap, p1);        // free current EDITELEMENT
            p1 = p2;
         }
         EXITCRIT;
         return NULL;                     // error exit
       }

      p3->pNext     = p2;                 // chain this one from the previous
      *p2           = *p1;                // copy old to new
      p2->pNext     = NULL;
      p2->pPrevious = p3;

      p3 = p2;                            // save for pPrevious
      p1 = p1->pNext;                     // move to next old EDITELEMENT
   }

   EXITCRIT;
   return pNew;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioEditCleanup                                           */
/*                                                                          */
/* DESCRIPTIVE NAME: Cleanup editing lists                                  */
/*                                                                          */
/* FUNCTION: This function frees the storage of all control blocks anchored */
/*           from the INSTANCE structure that were created by editing.      */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

/**************************************************
 * ioEditFreeEditList
 *
 * free all EDITELEMENTs in a chain
 **************************************************/

static void ioEditFreeEditList(PINSTANCE pinstance) {
   PEDITELEMENT p1, p2;

   p1 = pinstance->pEditList;       // set p1 to first EDITELEMENT in list

   while (p1 != NULL) {
      p2 = p1->pNext;               // remember next EDITELEMENT
      HhpFreeMem(hheap, p1);        // free current EDITELEMENT
      p1 = p2;                      // set p1 to next EDITELEMENT in list
   }

   pinstance->pEditList = NULL;
}

/*****************************************************
 * ioEditFreeFileList
 *
 * free all FILEELEMENTs in a chain
 *****************************************************/

static void ioEditFreeFileList(PINSTANCE pinstance) {
   PFILEELEMENT p1, p2;

   p1 = pinstance->pFileList;       // p1 -> first FILEELEMENT in list

   while (p1 != NULL) {
      p2 = p1->pNext;               // remember next FILEELEMENT
      HhpFreeMem(hheap, p1);        // free current FILEELEMENT
      p1 = p2;                      // set p1 to next FILEELEMENT in list
   }

   pinstance->pFileList = NULL;
}

/*******************************************************
 * ioEditFreeUndoList
 *
 * free all Edit List chains anchored on the Undo List
 *******************************************************/

static void ioEditFreeUndoList(PINSTANCE pinstance) {
   PEDITELEMENT p1, p2, p3, p4;

   p3 = pinstance->pUndoList;          // p3 -> first edit list

   while (p3 != NULL) {

      p4 = p3->pUndoRedoChain;         // p4 -> next edit list
      p1 = p3;                         // p1 -> current edit list

      while (p1 != NULL) {
         p2 = p1->pNext;               // remember next EDITELEMENT
         HhpFreeMem(hheap, p1);        // free current EDITELEMENT
         p1 = p2;                      // set p1 to next EDITELEMENT in list
      };

      p3 = p4;                         // p3 -> next edit list
   }

   pinstance->pUndoList = NULL;
}

/*********************************************************
 * ioEditFreeRedoList
 *
 * free all Edit List chains anchored on the Redo List
 *********************************************************/

static void ioEditFreeRedoList(PINSTANCE pinstance) {
   PEDITELEMENT p1, p2, p3, p4;

   p3 = pinstance->pRedoList;          // p3 -> first edit list

   while (p3 != NULL) {

      p4 = p3->pUndoRedoChain;         // p4 -> next edit list
      p1 = p3;                         // p1 -> current edit list

      while (p1 != NULL) {
         p2 = p1->pNext;               // remember next EDITELEMENT
         HhpFreeMem(hheap, p1);        // free current EDITELEMENT
         p1 = p2;                      // set p1 to next EDITELEMENT in list
      };

      p3 = p4;                         // p3 -> next edit list
   }

   pinstance->pRedoList = NULL;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioEditCleanup                                           */
/*                                                                          */
/* DESCRIPTIVE NAME: Clean up after editing                                 */
/*                                                                          */
/* FUNCTION: Frees all storage for editing data structures.                 */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

void ioEditCleanup(PINSTANCE pinstance) {

      // caller must have acquired the global semaphore

      ioEditFreeFileList(pinstance);
      ioEditFreeEditList(pinstance);
      ioEditFreeUndoList(pinstance);
      ioEditFreeRedoList(pinstance);

      pinstance->ulEditFlags = 0;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioEditTimeToFrame                                       */
/*                                                                          */
/* DESCRIPTIVE NAME: Convert time to frame number                           */
/*                                                                          */
/* FUNCTION: This function converts a time to a frame number.               */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

ULONG ioEditTimeToFrame(PMMVIDEOHEADER pTHVideo, ULONG ulTime)

{
// this number is rounded slightly to take into account rounding down errors
// that occur from converting mmtime numbers to microseconds.  The 0.01 at
// the end of the expression is used to round movies with a frame rate of
// slightly less than a whole number (i.e. 14.999925) which at the end of
// file would lose the last frame from a cut.

   double val = (((double) (ulTime + 1.0) * (double) (pTHVideo->ulRate)) / (1000000.0 * (double)(pTHVideo->ulScale)) + 0.01);

   return((ULONG) (val));

//   return((ULONG) (((double) ulTime * (double) (pTHVideo->ulRate)) / (1000000.0 * (double)(pTHVideo->ulScale))));

//   fps  = (double)(pTHVideo->ulRate) / (double)(pTHVideo->ulScale);
//   uspf = 1000000.0 / fps;
//   return (double)ulTime / uspf;       // ULONG coersion is like floor()
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioEditFrameToTime                                       */
/*                                                                          */
/* DESCRIPTIVE NAME: Frame number to microseconds                           */
/*                                                                          */
/* FUNCTION: This function converts a frame number to microseconds.         */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

ULONG ioEditFrameToTime(PMMVIDEOHEADER pTHVideo, ULONG frame)

{
   double fps;

   return ((double)frame * (double)pTHVideo->mmtimePerFrame * 1000.0 / 3.0);

// fps  = (double)(pTHVideo->ulRate) / (double)(pTHVideo->ulScale);
// return ((double)frame * 1000000.0 / fps) + 0.5;              // @T01

}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioEditDuration                                          */
/*                                                                          */
/* DESCRIPTIVE NAME: Find the length of an edited file in frames            */
/*                                                                          */
/* FUNCTION: This function returns the number of frames in an edited file.  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

ULONG ioEditDuration(PINSTANCE pinstance, PMMIO_EDIT_PARMS pParms) {
   PEDITELEMENT   pTemp;
   ULONG          duration = 0, rc;
   PMMVIDEOHEADER pVideo = NULL;
   PMMAUDIOHEADER pAudio = NULL;

   pTemp = pinstance->pEditList;

   while (pTemp != NULL) {
      duration += pTemp->ulDuration;
      pTemp = pTemp->pNext;
   }

   pParms->ulNewFileLength = 0;

   rc = ioEditFindHeaders(pinstance, &pVideo, &pAudio);

   pParms->ulNewFileLength = ioEditFrameToTime(pVideo, duration);

   return duration;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ioEditGetFullName                                       */
/*                                                                          */
/* DESCRIPTIVE NAME: Get Full Name                                          */
/*                                                                          */
/* FUNCTION: Form a fully qualified file name for a file                    */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

LONG ioEditGetFullName(PSZ inputName, PSZ outputName) {
   ULONG          driveNumber, logicalDriveMap, dirPathLen = CCHMAXPATH;
   ULONG          length;
   APIRET         apiRC;
   BYTE           dirPath[CCHMAXPATH];
   SZ             tempBuffer[CCHMAXPATH];

   length = strlen(inputName);

   if (length >= CCHMAXPATH) return MMIOERR_INVALID_FILENAME;

   if ((length >= 2) && (inputName[1] == ':')) {
      strcpy(outputName, inputName);
      return MMIO_SUCCESS;
   }

   outputName[0] = '\0';                     // be sure output is a string

   // copy the input to a temporary buffer so that the same area can
   // be used for input and output

   if (length == 0) tempBuffer[0] = '\0';
   else strcpy(tempBuffer, inputName);

   // get current drive and directory info

   apiRC = DosQueryCurrentDisk(&driveNumber, &logicalDriveMap);

   outputName[0] = 'A' + driveNumber - 1;
   outputName[1] = ':';
   outputName[2] = '\0';

   // if the input begins with a '\' then all we need to add is the drive,
   // otherwise we also need to add the current path

   if (tempBuffer[0] != '\\') {
      outputName[2] = '\\';
      outputName[3] = '\0';

      apiRC = DosQueryCurrentDir(0, &dirPath[0], &dirPathLen);
      if (apiRC != NO_ERROR) return MMIOERR_INTERNAL_SYSTEM;

      // if dirpath is empty, we don't want to append "\\"

      if (strcmp (dirPath, ""))
      {
         strcat(outputName, &dirPath[0]);
         strcat(outputName, "\\");
      }

   }

   strcat(outputName, tempBuffer);

   return MMIO_SUCCESS;
}

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: IOProcWinMsg                                            */
/*                                                                          */
/* DESCRIPTIVE NAME: Process Window Messages                                */
/*                                                                          */
/* FUNCTION: Handle window messages passed from the MCD                     */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: IOProcWinMsg                                                */
/*                                                                          */
/* INPUT:    PMMIOINFO  pmmioinfo - pointer to MMIOINFO instance structure  */
/*           PMMIO_WINMSG         - pointer to MMIO_WINMSG structure        */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              MMIO_SUCCESS                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              MMIO_ERROR                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

LONG IOProcWinMsg(PMMIOINFO pmmioinfo, PMMIO_WINMSG pParms) {
   LONG           rc;
   PINSTANCE      pinstance;

   if (pmmioinfo == NULL) return MMIO_ERROR;

   if (CheckMem((PVOID)pmmioinfo, sizeof(MMIOINFO), PAG_WRITE))
      return MMIO_ERROR;

   pmmioinfo->ulErrorRet = 0;

   rc = ioGetPtrInstance(pmmioinfo, &pinstance);
   if (rc != MMIO_SUCCESS) {
      pmmioinfo->ulErrorRet = rc;
      return MMIO_ERROR;
   }

   switch (pParms->usMessage) {

      case WM_DESTROYCLIPBOARD:
         pinstance->ulEditFlags &= ~ACTIVE_CLIPBRD_REFERENCE;
         break;

      default:
         break;
   }

   return MMIO_SUCCESS;
}

