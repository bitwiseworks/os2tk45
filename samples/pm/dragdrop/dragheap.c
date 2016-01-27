/**************************************************************************
 *  File name  :  dragheap.c
 *
 *  Description:  This file contains all file and memory management
 *                functions for the Drag/Drop sample.
 *
 *                This source file contains the following functions:
 *
 *                FindFilePurge()
 *                FindFileFirst(pszFilename, ulAttributes)
 *                FindFileNext(pInput)
 *                FindFileName(pszName)
 *                AllocAppDraginfo(hwndClient, pDraginfo)
 *                FreeAppDraginfo(pAppDraginfo)
 *                FindAppDraginfo(pDraginfo)
 *                AllocAppDragitem(pDraginfo, pFile)
 *                FreeAppDragitem(pAppDragitem)
 *                FindDraginfo(pAppDragitem)
 *                FindDragitemFile(pAppDragitem)
 *
 *  Concepts   :  memory management
 *
 *  API's      :  DosSubFreeMem
 *                DosGetNamedSharedMem
 *                DosAllocSharedMem
 *                DosSubSetMem
 *                DosFindFirst
 *                DosSubAllocMem
 *                DosFindNext
 *                DosFindClose
 *                DrgQueryDragitemCount
 *
 *  Required
 *    Files    :  OS2.H, STRING.H, DRAGDROP.H
 *
 *  Copyright (C) 1991 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 *************************************************************************/

/*
 *        Include the required sections from the PM header file
 */
#define INCL_DOSERRORS
#define INCL_DOSFILEMGR
#define INCL_WIN
#define INCL_WINSTDDRAG

/*
 *                     Include the PM header file
 */
#include <os2.h>

/*
 *                  Include the C library header files
 */
#include <string.h>

/*
 *                 Include the application header files
 */
#include "dragdrop.h"

/*
 * Variables global to file
 */

static PBYTE            hHeap     = NULL;   /* heap from which elements are */
                                            /* allocated                    */
static PPUBFILEFINDBUF  pFirst    = NULL;   /* anchor to list of elements   */
static PAPPDRAGINFO     pFirstAppDraginfo = NULL;
                                            /* pointer to chain of source   */
                                            /* that contain information     */
                                            /* for each drag in progress    */

/**************************************************************************
 *
 *  Name       : FindFilePurge
 *
 *  Description: Removes all entries in the list.  The function is
 *               invoked from FileFindFirst to remove the old list if
 *               one is present.
 *
 *  Concepts   :
 *
 *  API's      :  DosSubFreeMem
 *
 *  Parameters :  [none]
 *
 *  Return     :  TRUE  = list was purged
 *                FALSE = purge of list was unsuccessful
 *************************************************************************/
BOOL FindFilePurge(VOID)
{
   PPUBFILEFINDBUF pList, pListNext;

   for (pList = pFirst; pList != NULL; pList = pListNext)
   {
      pListNext = pList->pNext;
      if(NO_ERROR !=
            DosSubFreeMem(hHeap, (NPBYTE)pList, sizeof(PUBFILEFINDBUF)))
         return FALSE;
   }
   pFirst = NULL;                                  /* indicate list is empty */
   return TRUE;
}


/**************************************************************************
 *
 *  Name       : FindFileFirst
 *
 *  Description: Finds all files in the input subdirectory, creating
 *               a list of these files in the heap.  It then returns
 *               the first file found onthe queue that matches the
 *               input attributes.
 *
 *  Concepts   :
 *
 *  API's      :  DosGetNamedSharedMem
 *                DosAllocSharedMem
 *                DosSubSetMem
 *                DosFindFirst
 *                DosSubAllocMem
 *                DosFindNext
 *                DosFindClose
 *
 *  Parameters :  pszFilename = the name of the file
 *                ulAttributes = attribute flags
 *
 *  Return     :  address of the first list element that matches the
 *                input attributes, or
 *                NULL = no file matched the input attributes
 *************************************************************************/
PPUBFILEFINDBUF FindFileFirst (PSZ pszFilename, ULONG ulAttributes)
{
   FILEFINDBUF3    ffbFile;
   ULONG           rc;
   ULONG           cFile;
   PPUBFILEFINDBUF pAlloc, pTail;
   NPBYTE          npBuffer;
   HDIR            hDir;
   PVOID           phHeap;

   if (!hHeap)
   {
     /*
      * Initialize the heap that will be used to contain the queue elements
      * for the files in the subdirectory
      */
     rc = DosGetNamedSharedMem((PPVOID)&hHeap, (PSZ)"\\sharemem\\dragmem",
                               PAG_READ | PAG_WRITE);
     if (rc != 0L)
     {
        rc = DosAllocSharedMem((PPVOID)&hHeap, (PSZ)"\\sharemem\\dragmem",
                             65535LU,  PAG_COMMIT | PAG_READ | PAG_WRITE);

        if(rc != 0L ||    /* Initialize the heap.                 */
           (NO_ERROR != DosSubSetMem((PVOID)hHeap, 1, 65535)))
        {
          /*
           * Heap initialization failed.
           */
          MessageBox(HWND_DESKTOP,
                    IDMSG_HEAPERROR,
                    MB_OK | MB_ERROR,
                    TRUE);
          return NULL;
        }
         /*
          * Heap is initialized - set anchor to NULL.
          */
        pFirst = NULL;
     }
   }

   /*
    * heap now initialized
    *
    * free the entries currently on the list and create the new list
    */
   if(!FindFilePurge())                        /* empty the current list   */
   {
      MessageBox(HWND_DESKTOP,
                    IDMSG_MEMFREEERROR,
                    MB_OK | MB_ERROR,
                    TRUE);
      return NULL;
   }

   cFile = 1;                                  /* buffer is large enough   */
                                               /* to contain info for 1    */
                                               /* file                     */
   hDir = HDIR_CREATE;                         /* allocate DosFind handle  */
   rc = DosFindFirst(pszFilename, &hDir, ulAttributes, &ffbFile,
                     sizeof(ffbFile), &cFile, 1);
   while (!rc)
   {
    /*
     * if we couldn't allocate a buffer from the heap, display an
     * error message and set the flag that will terminate the loop
     */
      if((NO_ERROR !=
        DosSubAllocMem((PVOID)hHeap, (PPVOID)&npBuffer, sizeof(PUBFILEFINDBUF)))
               ||     !npBuffer)
      {
         MessageBox(HWND_DESKTOP,
                    IDMSG_HEAPALLOCERROR,
                    MB_OK | MB_ERROR,
                    TRUE);
         rc = 1;
      }

    /*
     * otherwise, add the found entry to the list
     */
      else
      {
         phHeap = hHeap;
         pAlloc = (PPUBFILEFINDBUF) npBuffer;
         pAlloc->pNext = NULL;

      /*
       * if this is the first one in the list, update the head pointer
       */
         if (pFirst == NULL)
            pFirst = pAlloc;

      /*
       * otherwise, add it to the end of the list
       */
         else
           pTail ->pNext = pAlloc;

         pTail = pAlloc;                    /* update the tail pointer      */

      /*
       * copy portions of file find buffer that will be kept to the
       * buffer that we allocated.
       */
         if (ffbFile.attrFile & FILE_DIRECTORY)
            strcpy (pAlloc->achName, "\\");     /* add '\\' before directory  */
         else
            pAlloc->achName[0] = '\0';          /* just a file                */

         strcat (pAlloc->achName, ffbFile.achName);
         pAlloc->fdateLastWrite = ffbFile.fdateLastWrite;
         pAlloc->ftimeLastWrite = ffbFile.ftimeLastWrite;
         pAlloc->cbFileAlloc    = ffbFile.cbFileAlloc;
         pAlloc->attrFile       = ffbFile.attrFile;

      /*
       * get next file
       */
         rc = DosFindNext (hDir, &ffbFile, (LONG)(sizeof(ffbFile)), &cFile);
      }
   }

  /*
   * All files have been found. Free the file find handle and send the
   * appropriate return code.
   */
   DosFindClose (hDir);
   return (pFirst);
}


/**************************************************************************
 *
 *  Name       : FindFileNext
 *
 *  Description: Finds the next file int eh input subdirectory
 *
 *  Concepts   :
 *
 *  API's      :  [none]
 *
 *  Parameters :  pInput = a pointer to the input file-find buffer
 *
 *  Return     :  address of the next file that matches the
 *                input attributes, or
 *                NULL = no file matched the input attributes before
 *                       reaching the end of the list
 *************************************************************************/
PPUBFILEFINDBUF FindFileNext(PPUBFILEFINDBUF pInput)
{
   return ((pInput != NULL) ? pInput->pNext : (PPUBFILEFINDBUF)NULL);
}

/**************************************************************************
 *
 *  Name       : FindFileName
 *
 *  Description: Finds the next file in the input subdirectory
 *
 *  Concepts   :
 *
 *  API's      :  [none]
 *
 *  Parameters :  pszName = a pointer to the file name string
 *
 *  Return     :  address of the next file that matches the
 *                input file name, or
 *                NULL = no file matched the input name
 *************************************************************************/
PPUBFILEFINDBUF FindFileName (PSZ pszName)
{
   PPUBFILEFINDBUF       pList;

   pList = pFirst;

   while (pList != NULL)
   {
      if (strcmp (pList->achName, pszName) == 0)
         break;

      pList = pList -> pNext;
   }

   return pList;
}

/**************************************************************************
 *
 *  Name       : AllocAppDraginfo
 *
 *  Description: Allocates an APPDRAGINFO control block and places
 *               it on a queue to be found by the application later.
 *               The APPDRAGINFO is initialized.
 *
 *  Concepts   : drag information
 *
 *  API's      :  DosSubAllocMem
 *                DrgQueryDragitemCount
 *
 *  Parameters :  pszName = a pointer to the file name string
 *
 *  Return     :  address of the APPDRAGINFO, or
 *                NULL = the allocation failed
 *************************************************************************/
PAPPDRAGINFO AllocAppDraginfo (HWND hwndClient, PDRAGINFO pDraginfo)
{
   NPBYTE          npBuffer;
   PAPPDRAGINFO    pAlloc;
   PVOID           phHeap;

  /*
   * if we couldn't allocate a buffer from
   * the heap, display a message and abort
   */
   if(NO_ERROR != DosSubAllocMem(hHeap, (PPVOID)&npBuffer, sizeof(APPDRAGINFO)))
   {
      MessageBox(HWND_DESKTOP,
                 IDMSG_ALLOCDRAGINFO,
                 MB_OK | MB_ERROR,
                 TRUE);
      return NULL;
   }
   else
   {
    /*
     * Add entry to list.
     */
      phHeap = hHeap;
      pAlloc = (PAPPDRAGINFO) npBuffer;
      pAlloc->pNextAppDraginfo = pFirstAppDraginfo;
      pFirstAppDraginfo = pAlloc;

    /*
     * Initialize the new entry.
     */
      pAlloc->pDraginfo      = pDraginfo;
      pAlloc->hwndClient     = hwndClient;
      pAlloc->usItemsDragged = DrgQueryDragitemCount(pDraginfo);
      return pAlloc;
   }
}

/**************************************************************************
 *
 *  Name       : FreeAppDraginfo
 *
 *  Description: Removes an APPDRAGINFO from the queue and
 *               deallocates the memory it was using.
 *
 *  Concepts   : drag information
 *
 *  API's      :  DosSubFreeMem
 *
 *  Parameters :  pAppDraginfo = pointer to the drag information
 *
 *  Return     :  TRUE = memory was successfullly deallocated
 *                FALSE = the deallocation failed
 *************************************************************************/
BOOL FreeAppDraginfo(PAPPDRAGINFO pAppDraginfo)
{
   PAPPDRAGINFO    pPrevious, pCurrent;

   pPrevious = NULL;
   pCurrent = pFirstAppDraginfo;

   while (pCurrent != NULL)
   {
      if (pCurrent == pAppDraginfo)
         break;
      else
      {
         pPrevious = pCurrent;
         pCurrent = pCurrent->pNextAppDraginfo;
      }
   }

   if (pCurrent != NULL)
   {
      if (pPrevious == NULL)
         pFirstAppDraginfo = pCurrent->pNextAppDraginfo;
      else
         pPrevious->pNextAppDraginfo = pCurrent->pNextAppDraginfo;

      if (NO_ERROR ==
             DosSubFreeMem(hHeap, (NPBYTE)pAppDraginfo, sizeof(APPDRAGINFO)))
         return TRUE;
      else
         return FALSE;
   }
   else
   {
      return FALSE;
   }
}

/**************************************************************************
 *
 *  Name       : FindAppDraginfo
 *
 *  Description: Finds an APPDRAGINFO, given the address of a DRAGINFO
 *
 *  Concepts   : drag information
 *
 *  API's      :  [none]
 *
 *  Parameters :  pDraginfo = pointer to the drag information
 *
 *  Return     :  NULL = unable to find the APPDRAGINFO
 *                otherwise, the address of the APPDRAGINFO
 *************************************************************************/
PAPPDRAGINFO FindAppDraginfo(PDRAGINFO pDraginfo)
{
   PAPPDRAGINFO    pCurrent;

   pCurrent = pFirstAppDraginfo;

   while (pCurrent != NULL)
   {
     if (pCurrent->pDraginfo == pDraginfo)
        break;
     else
        pCurrent = pCurrent->pNextAppDraginfo;
   }

   return pCurrent;
}


/**************************************************************************
 *
 *  Name       : AllocAppDragitem
 *
 *  Description: Allocates an APPDRAGITEM control block and initializes it
 *
 *  Concepts   : drag information
 *
 *  API's      :  DosSubAllocMem
 *
 *  Parameters :  pDraginfo = pointer to the drag information
 *                pFile = file pointer
 *
 *  Return     :  NULL = allocation failed
 *                otherwise, the address of the APPDRAGINFO
 *************************************************************************/
PAPPDRAGITEM AllocAppDragitem(PDRAGINFO pDraginfo, PPUBFILEFINDBUF pFile)
{
   NPBYTE          npBuffer;
   PAPPDRAGITEM    pAlloc;
   PVOID           phHeap;

   if(NO_ERROR != DosSubAllocMem(hHeap, (PPVOID)&npBuffer, sizeof(APPDRAGITEM)))
   {
       MessageBox(HWND_DESKTOP,
                    IDMSG_HEAPALLOCERROR,
                    MB_OK | MB_ERROR,
                    TRUE);
       return NULL;
   }
   else
   {
     /*
      * initialize the new entry
      */
      phHeap = &hHeap;
      pAlloc = (PAPPDRAGITEM)npBuffer;
      pAlloc->pDraginfo = pDraginfo;
      pAlloc->pFile     = pFile;
      return pAlloc;
   }
}

/**************************************************************************
 *
 *  Name       : FreeAppDragitem
 *
 *  Description: Frees an APPDRAGITEM
 *
 *  Concepts   : drag information
 *
 *  API's      :  DosSubFreeMem
 *
 *  Parameters :  pAppDragitem = pointer to the drag item
 *
 *  Return     :  TRUE = memory was successfully deallocated
 *                FALSE = an error occurred
 *************************************************************************/
BOOL FreeAppDragitem(PAPPDRAGITEM pAppDragitem)
{
   return (NO_ERROR ==
         DosSubFreeMem(hHeap, (NPBYTE)pAppDragitem, sizeof(APPDRAGITEM)));
}


/**************************************************************************
 *
 *  Name       : FindDragInfo
 *
 *  Description: Finds the PDRAGINFO for a drag operation, given the
 *               address of an APPDRAGITEM
 *
 *  Concepts   : drag information
 *
 *  API's      :  [none]
 *
 *  Parameters :  pAppDragitem = pointer to the application drag item
 *
 *  Return     :  NULL = unable to find the drag information
 *                otherwise, the address of the drag information used
 *                to drag this item
 *************************************************************************/
PDRAGINFO FindDraginfo(PAPPDRAGITEM pAppDragitem)
{
   return pAppDragitem->pDraginfo;
}

/**************************************************************************
 *
 *  Name       : FindDragItemFile
 *
 *  Description: Finds the PPUBFILEFINDBUF for a drag operation, given the
 *               address of an APPDRAGITEM
 *
 *  Concepts   : drag information
 *
 *  API's      :  [none]
 *
 *  Parameters :  pAppDragitem = pointer to the drag item
 *
 *  Return     :  NULL = unable to find the PPUBFILEFINDBUF
 *                otherwise, the address of the PPUBFILEFINDBUF for the
 *                item being dragged
 *************************************************************************/
PPUBFILEFINDBUF FindDragitemFile (PAPPDRAGITEM pAppDragitem)
{
   return pAppDragitem->pFile;
}
/***************************  End of dragheap.c  *************************/
