/**************************************************************************
 *  File name  :  dragdrag.c
 *
 *  Description:  This source file contains the direct manipulation routines
 *                of the Drag/drop sample.
 *
 *                This source file contains the following functions:
 *
 *                FreeSourceResources (hab, pDraginfo)
 *                PreventClose (hwndClient, cDrag)
 *                RemoveFinalBackslash (p)
 *                InitDrag(hwndClient, hList, hptrFile, hptrFolder, hptrMulti,
 *                         pContainerName)
 *                EndConversation (hwndClient, mp1)
 *                DragLeave (void)
 *                DropHelp (hwndClient, pDraginfo)
 *                DragOver (pDraginfo, pCurrentDir)
 *                Drop (hab, hwndClient, pDraginfo, pCurrentDir)
 *                EndTarget (hwnd, pParm)
 *                CheckBusy (void)
 *                CheckClose (hwnd)
 *                TargetThread (void)
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  DrgAccessDraginfo
 *                DrgDeleteDraginfoStrHandles
 *                DrgQueryDragitemCount
 *                DrgQueryDragitemPtr
 *                DrgFreeDraginfo
 *                WinWindowFromID
 *                WinSendMsg
 *                DrgAllocDraginfo
 *                DrgAddStrHandle
 *                WinQueryLboxItemText
 *                DrgSetDragitem
 *                DrgDrag
 *                WinDeleteLboxItem
 *                DrgQueryStrName
 *                DrgVerifyRMF
 *                DrgAllocDragtransfer
 *                DrgSendTransferMsg
 *                WinStartTimer
 *                WinGetMsg
 *                WinStopTimer
 *                WinDispatchMsg
 *                DrgFreeDragtransfer
 *                DosGetNamedSharedMem
 *                DosSubAllocMem
 *                DosCreateThread
 *                DrgQueryDragitem
 *                WinPostMsg
 *                DosCopy
 *                DosDelete
 *                DosEnterCritSec
 *                WinGetLastError
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
 *        Include the required sections from the PM header file.
 */
#define INCL_DOSFILEMGR
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#define INCL_SHLERRORS
#define INCL_WIN
#define INCL_WINSTDDRAG

/*
 *                     Include the PM header file.
 */
#include <os2.h>
#include <os2def.h>
/*
 *                  Include the C library header files.
 */
#include <string.h>

/*
 *                 Include the application header files.
 */
#include "dragdrop.h"

static PDRAGTARGETTHREAD vpThreadParms = NULL;

/*
 * Variables global to this file
 */
static char   szRendMechFmtFILE []  = "<DRM_OS2FILE,DRF_UNKNOWN>";

/*
 * Because the following items used by the target, are used only
 * during the drag operation, and drag is a system modal operation,
 * a target can always accept a drop.
 *
 * In any case, the target cannot allow the application to
 * close when a drag is in progress, either as a target or a source.
 * This is because the thread performing the operation would also be
 * ended.  A target could allow a close, but only if it prompted
 * the user to request permission.
 */

static USHORT    usOperation;
      /* usOperation is used to remember the user selected
       * (or application selected default * operation) between
       * a DM_DRAGOVER
       * and a DM_DROP message, to avoid re-figuring the operation.
       * Because this is used during drag, which is a system modal
       * operation, a target can always be dropped on.
       * Used by target only.
       */

/*
 * The following is used by both the source and target to keep the total
 * number of drag operations that the application is currently involved
 * in. If the count is non-zero, the application will not allow the user
 * to close it.
 */
static ULONG    ulDragCount = 0L;

/**************************************************************************
 *
 *  Name       : FreeSourceResources (hab, pDraginfo)
 *
 *  Description: Frees the resources allocated by the source for a drag
 *               operation
 *
 *  Concepts   :  accesses and deletes an instance of drag information
 *
 *  API's      :  DrgAccessDraginfo
 *                DrgDeleteDraginfoStrHandles
 *                DrgQueryDragitemCount
 *                DrgQueryDragitemPtr
 *                DrgFreeDraginfo
 *
 *  Parameters :  pDraginfo = pointer to the drag information structure
 *
 *  Return     :  TRUE  = if resources freed successfully
 *                FALSE = unable to free all resources
 *
 *************************************************************************/
BOOL FreeSourceResources (HAB hab, PDRAGINFO pDraginfo)
{
   PDRAGITEM       pDragitem;
   ULONG           ulItemsDragged, i;
   ERRORID         errorid;

   if(DrgAccessDraginfo(pDraginfo) && DrgDeleteDraginfoStrHandles(pDraginfo))
      ;           /* drag access OK */
   else
   {
      MessageBox(HWND_DESKTOP, IDMSG_DRAGACCESSERROR, MB_OK | MB_ERROR, TRUE);
      return FALSE;
   }

   ulItemsDragged = DrgQueryDragitemCount(pDraginfo);

   for (i = 0L; i < ulItemsDragged; ++i)
   {
      pDragitem = DrgQueryDragitemPtr(pDraginfo, i);
      if(!FreeAppDragitem((PAPPDRAGITEM)pDragitem->ulItemID))
      {
         MessageBox(HWND_DESKTOP, IDMSG_MEMFREEERROR, MB_OK | MB_ERROR, TRUE);
         return FALSE;
      }
   }

   if(!FreeAppDraginfo(FindAppDraginfo(pDraginfo)))
   {
      MessageBox(HWND_DESKTOP, IDMSG_MEMFREEERROR, MB_OK | MB_ERROR, TRUE);
      return FALSE;
   }
   if(DrgFreeDraginfo(pDraginfo)
               || PMERR_SOURCE_SAME_AS_TARGET ==
               ERRORIDERROR(WinGetLastError(hab)))
      return TRUE;
   else
   {
      MessageBox(HWND_DESKTOP, IDMSG_DRAGFREEERROR, MB_OK | MB_ERROR, TRUE);
      return FALSE;
   }
}  /* End of FreeSourceResources  */


/**************************************************************************
 *
 *  Name       : PreventClose (hwndClient, cDrag)
 *
 *  Description: Disables the close option on the system menu when
 *               a drag is in progress, or enables it when all drag
 *               operations have completed.
 *
 *  Concepts   :  system menu, enablement
 *
 *  API's      :  WinWindowFromID
 *                WinSendMsg
 *
 *  Parameters :  hwndClient = client window handlenformation structure
 *                cDrag      = the enabled state of the close menu item
 *
 *  Return     :  TRUE  = if the close menu item is disabled
 *                FALSE = if the close menu item has been enabled
 *
 *************************************************************************/
VOID PreventClose(HWND hwndClient, ULONG cDrag)
{
   HWND hwndSysMenu = WinWindowFromID(ParentOf(hwndClient), FID_SYSMENU);

   WinSendMsg(hwndSysMenu, MM_SETITEMATTR, MPFROM2SHORT(SC_CLOSE, (SHORT)TRUE),
             MPFROM2SHORT(MIA_DISABLED, (SHORT)(cDrag ? MIA_DISABLED : FALSE)));

   return;
}  /*  End of PreventClose  */


/**************************************************************************
 *
 *  Name       : RemoveFinalBackslash (p)
 *
 *  Description: Checks the final character in a string and,
 *               if it is a backslash, removes it.
 *
 *  Concepts   :  pathnames
 *
 *  API's      :  [none]
 *
 *  Parameters :  p = a pointer to the string
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID RemoveFinalBackslash (PSZ p)
{
   for (; *p; ++p)
      ;                  /*  go to the end of the string  */

   if (*--p == '\\')
   {
      *p = '\0';
   }
}  /*  End of RemoveFinalBackslash  */


/**************************************************************************
 *
 *  Name       : InitDrag(hwndClient, hList, hptrFile, hptrFolder,
 *                        hptrMulti, pContainerName)
 *
 *  Description: Builds a DRAGINFO structure for the selected items,
 *               then calls DrgDrag to perform the drag operation.
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  DrgAllocDraginfo
 *                DrgAddStrHandle
 *                WinQueryLboxItemText
 *                DrgSetDragitem
 *
 *  Parameters :  hwndClient = client window handle
 *                hList      = handle of the list of file names
 *                hptrFile   = file handle
 *                hptrFolder = folder handle
 *                hptrMulti  = multiple list handle
 *                pContainerName - pointer to name of container
 *
 *  Return     :  TRUE  = the drag operation was performed
 *                FALSE = unable to perform the drag operation
 *
 *************************************************************************/
BOOL InitDrag (HAB hab, HWND hwndClient, HWND hList, HPOINTER hptrFile,
               HPOINTER hptrFolder, HPOINTER hptrMulti, PSZ pContainerName)
{
   CHAR             chDrive;
   SHORT            item;
   ULONG            i, ulItemsDragged;
   HWND             hwndTarget;
   LHANDLE          hImage;
   DRAGITEM         Dragitem;
   HSTR             hstrType, hstrRMF, hstrContainerName;
   USHORT           fsControl;
   USHORT           fsSupportedOps;
   PPUBFILEFINDBUF  pFile;
   CHAR             szItemName [64];
   CHAR             szFullContainerName [CCHMAXPATH];
   PAPPDRAGINFO     pAppDraginfo;
   PDRAGINFO        pSourceDraginfo;
   DRAGIMAGE        dimg;

   /*
    * Determine number of items selected prior to allocating the
    * DRAGINFO structure. Note that this sample program requires at
    * least one item to be selected in order to do a drag operation.
    */

   ulItemsDragged = 0L;
   item = WinQueryLboxSelItemFirst (hList);

   while (item != LIT_NONE)
   {
      ++ulItemsDragged;
      item = WinQueryLboxSelItemNext (hList, item);
   }

   if (ulItemsDragged == 0L)
   {
      MessageBox(HWND_DESKTOP, IDMSG_ATLEASTONE, MB_OK | MB_ERROR, TRUE);
      return FALSE;
   }

   /*
    * Allocate the DRAGINFO structure and the APPDRAGINFO structure.
    */
   pSourceDraginfo = DrgAllocDraginfo(ulItemsDragged);
   pAppDraginfo = AllocAppDraginfo (hwndClient, pSourceDraginfo);

   /*
    * Initialize variable used to initialize the internal DRAGITEM
    * structure.
    */
   hstrType  = DrgAddStrHandle (DRT_UNKNOWN);       /* file type is unknown  */
   hstrRMF   = DrgAddStrHandle (szRendMechFmtFILE); /* os2file, unknown      */
   strcpy (szFullContainerName, pContainerName);    /* container name        */

   RemoveFinalBackslash (szFullContainerName);
   strcat (szFullContainerName, "\\");
   hstrContainerName = DrgAddStrHandle(szFullContainerName);

   fsControl = 0;
   chDrive = (CHAR) pContainerName[0];

   if((chDrive=='A') || (chDrive=='B') || (chDrive=='a') || (chDrive=='b'))
   {       /* is source container removable media?   */
      fsControl = fsControl | (SHORT)DC_REMOVEABLEMEDIA;
   }

   fsSupportedOps = DO_COPYABLE | DO_MOVEABLE;    /* can only move or copy */

   /*
    * Initialize common portions of the internal Dragitem buffer.
    */
   Dragitem.hwndItem          = hwndClient;
   Dragitem.hstrType          = hstrType;
   Dragitem.hstrRMF           = hstrRMF;
   Dragitem.hstrContainerName = hstrContainerName;
   Dragitem.fsControl         = fsControl;
   Dragitem.fsSupportedOps    = fsSupportedOps;

   /*
    * Initialize unique portions of the internal Dragitem buffer for each
    * item that was selected.
    */
   item = LIT_FIRST;

   for (i = 0L; i < ulItemsDragged; ++i)
   {
      item = WinQueryLboxSelItemNext (hList, item);
      WinQueryLboxItemText (hList, item, szItemName, sizeof(szItemName));
      pFile = FindFileName (szItemName);

      if (pFile == NULL)
      {
         MessageBox(HWND_DESKTOP, IDMSG_CANTLOCATE, MB_OK | MB_ERROR, TRUE);
      }
      else
      {
         Dragitem.hstrSourceName = DrgAddStrHandle (pFile->achName);
         Dragitem.hstrTargetName = Dragitem.hstrSourceName;
         Dragitem.ulItemID = (ULONG)AllocAppDragitem(pSourceDraginfo, pFile);
         if(Dragitem.ulItemID == 0L)
            return FALSE;
         if (pFile->attrFile & FILE_DIRECTORY)
            Dragitem.fsControl = Dragitem.fsControl | (SHORT)DC_CONTAINER;

         if(!DrgSetDragitem(pSourceDraginfo, &Dragitem, sizeof(DRAGITEM), i))
         {
            MessageBox(HWND_DESKTOP, IDMSG_DRAGSETITEMERROR,
                       MB_OK | MB_ERROR, TRUE);
            return FALSE;
         }
      }
   }
   /*
    *  Drag the items.
    */
   if (ulItemsDragged == 1L)
      hImage = (Dragitem.fsControl & DC_CONTAINER) ? hptrFolder : hptrFile;
   else
      hImage = hptrMulti;

   ++ulDragCount;                     /* remember drag is in progress     */
   PreventClose(hwndClient, ulDragCount);

   dimg.cb       = sizeof (DRAGIMAGE);
   dimg.hImage   = hImage;
   dimg.fl       = DRG_ICON | DRG_TRANSPARENT;
   dimg.cxOffset = 0;
   dimg.cyOffset = 0;

   pSourceDraginfo->hwndSource = hwndClient;
   hwndTarget=DrgDrag(hwndClient, pSourceDraginfo, &dimg, 1L, VK_BUTTON2, NULL);

   if (hwndTarget == NULLHANDLE)
   {
     /*
      * User cancelled the drag or asked for help, so we can release
      * the resources allocated to the drag operation.
      */
      FreeSourceResources (hab, pSourceDraginfo);
      --ulDragCount;
      PreventClose (hwndClient, ulDragCount);
   }
   return TRUE;
}     /*  End of InitDrag  */


/**************************************************************************
 *
 *  Name       : EndConversation(hwndClient, mp1)
 *
 *  Description: Decrements ulItemsDragged by one for each
 *               DM_ENDCONVERSATION received.  When ulItemsDragged
 *               becomes 0, the DRAGINFO structure is freed.
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  WinWindowFromID
 *                WinDeleteLboxItem
 *                DrgFreeDraginfo
 *                WinGetLastError
 *
 *  Parameters :  hwndClient = client window handle
 *                mp1        = drag item ID
 *                mp2        = render success flag
 *
 *  Return     :  NULL = successful end to the conversation
 *
 *************************************************************************/
MRESULT EndConversation(HAB hab, HWND hwndClient, MPARAM mp1, MPARAM mp2)
{
   PAPPDRAGITEM pAppDragitem = (PAPPDRAGITEM)mp1;
   PAPPDRAGINFO pAppDraginfo;
   USHORT       i;
   HWND         hwndLbox = WinWindowFromID (hwndClient, LIST_ID);

  /*
   *   Did the target properly render the item?
   */
   if(mp2 == (MPARAM)DMFL_TARGETFAIL)
      MessageBox(HWND_DESKTOP, IDMSG_TARGETRENDER, MB_OK | MB_ERROR, TRUE);
  /*
   * delete the item from the listbox if the operation was a move
   */
   if (pAppDragitem->pDraginfo->usOperation == (USHORT)DO_MOVE)
   {
      i = WinFindLboxItem (hwndLbox, pAppDragitem->pFile->achName);
      WinDeleteLboxItem (hwndLbox, i);
   }

   pAppDraginfo = FindAppDraginfo(FindDraginfo(pAppDragitem));
   if(!pAppDraginfo)
      MessageBox(HWND_DESKTOP, IDMSG_FINDAPPDRAGINFO, MB_OK | MB_ERROR, TRUE);
   FreeAppDragitem(pAppDragitem);

   if (--pAppDraginfo->usItemsDragged == 0)
   {
      if(DrgFreeDraginfo(pAppDraginfo->pDraginfo)
               || PMERR_SOURCE_SAME_AS_TARGET ==
               ERRORIDERROR(WinGetLastError(hab)))
      {
         if(!FreeAppDraginfo(pAppDraginfo))
            MessageBox(HWND_DESKTOP, IDMSG_MEMFREEERROR, MB_OK | MB_ERROR,TRUE);
         else
         {
            --ulDragCount;                   /* indicate drag completed */
            PreventClose(hwndClient, ulDragCount);
         }
      }
      else
         MessageBox(HWND_DESKTOP, IDMSG_DRAGFREEERROR, MB_OK | MB_ERROR, TRUE);
   }
   return (MRESULT)NULL;
}  /*  End of EndConversation  */


/**************************************************************************
 *
 *  Name       : DragLeave()
 *
 *  Description: Called when a LN_DRAGLEAVE message is received.
 *               As this application does not do target emphasis,
 *               this function simply returns NULL.
 *
 *  Concepts   :  target emphasis
 *
 *  API's      :  [none]
 *
 *  Parameters :  [none]
 *
 *  Return     :  NULL = successful end
 *
 *************************************************************************/
MRESULT DragLeave (void)
{
   return (MRESULT)NULL;
}  /*  End of DragLeave  */

/**************************************************************************
 *
 *  Name       : DropHelp(hwndClient, pDraginfo)
 *
 *  Description: Issues a message to notify the user what would happen
 *               if mouse button 2 were released, based on the current
 *               operation.
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  DrgAccessDraginfo
 *                DrgDeleteDraginfoStrHandles
 *                DrgFreeDraginfo
 *
 *  Parameters :  hwndClient = client window handle
 *                pDraginfo  = pointer to drag information structure
 *
 *  Return     :  NULL = successful
 *
 *************************************************************************/
MRESULT DropHelp (HWND hwndClient, PDRAGINFO pDraginfo)
{
   char message[81];

   if(!DrgAccessDraginfo (pDraginfo))
   {
      MessageBox(HWND_DESKTOP, IDMSG_DRAGACCESSERROR, MB_OK | MB_ERROR, TRUE);
   }
   strcpy (message, "Dropping here would perform ");

   switch (pDraginfo->usOperation)
   {
   case DO_DEFAULT:
      strcat (message, "the default ");
      break;
   case DO_COPY:
      strcat (message, "a copy ");
      break;
   case DO_MOVE:
      strcat (message, "a move ");
      break;
   case DO_UNKNOWN:
      strcat (message, "an unknown (private) ");
      break;
   }

   strcat (message, "operation.");
   Message (message);

   if(DrgDeleteDraginfoStrHandles(pDraginfo) && DrgFreeDraginfo(pDraginfo))
      ;        /*  free OK  */
   else
   {
      MessageBox(HWND_DESKTOP, IDMSG_DRAGERROR, MB_OK | MB_ERROR, TRUE);
   }
   return (MRESULT)NULL;
   hwndClient;                     /* unreferenced formal parameter */
}  /*  End of DropHelp  */


/**************************************************************************
 *
 *  Name       : DragOver(pDraginfo, pCurrentDir)
 *
 *  Description: Provides visual feedback to the user as to whether it
 *               can support the drag operation
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  DrgAccessDraginfo
 *                DrgFreeDraginfo
 *                DrgQueryDragitemPtr
 *                DrgQueryStrName
 *                DrgQueryDragitemCount
 *                DrgVerifyRMF
 *                WinGetLastError
 *
 *  Parameters :  pDraginfo  = pointer to drag information structure
 *                pCurrentDir= the name of the current directory
 *
 *  Return     :  DOR_DROP, DO_MOVE for a default operation
 *
 *************************************************************************/
MRESULT DragOver(HAB hab, PDRAGINFO pDraginfo, PSZ pCurrentDir)
{
   USHORT        usIndicator, cItems, i;
   ULONG         ulBytesWritten;
   PDRAGITEM     pditem;
   CHAR          SourceDir [CCHMAXPATH], chDrive;

  /*
   * Get access to the DRAGINFO structure.
   */
   if(!DrgAccessDraginfo(pDraginfo))
   {
      MessageBox(HWND_DESKTOP, IDMSG_DRAGACCESSERROR, MB_OK | MB_ERROR, TRUE);
      return (MRFROM2SHORT (DOR_NODROPOP, 0));
   }
   usOperation = (USHORT)DO_MOVE;          /* the default operation is move */

  /*
   * Determine if a drop can be accepted.
   */
   switch (pDraginfo->usOperation)
   {
    /*
     * return DOR_NODROPOP if current operation is link or unknown.
     */
   case DO_UNKNOWN:
      if(!DrgFreeDraginfo (pDraginfo))
         MessageBox(HWND_DESKTOP, IDMSG_DRAGFREEERROR, MB_OK | MB_ERROR, TRUE);
      return (MRFROM2SHORT (DOR_NODROPOP, 0));

    /*
     * Determine default operation if current operation is default.
     * The default is move unless either the source or the target is
     * on different removable media.
     */
   case DO_DEFAULT:
      pditem = DrgQueryDragitemPtr (pDraginfo, 0);
      ulBytesWritten = DrgQueryStrName(pditem->hstrContainerName,
                                       sizeof(SourceDir),
                                       SourceDir);
      if(ulBytesWritten == 0L)
      {
         MessageBox(HWND_DESKTOP, IDMSG_DRAGQUERYSTRINGERROR,
                          MB_OK | MB_ERROR, TRUE);
         return (MRFROM2SHORT (DOR_NODROPOP, 0));
      }

         /*
          * if the source and target are on the
          * same drive, the operation is a move
          */
      if (*pCurrentDir == SourceDir[0])
         usOperation = (USHORT)DO_MOVE;
         /*
          * otherwise, different drives
          */
      else
      {
         chDrive = (CHAR)pCurrentDir[0];
           /*
            * if the target is A: or B:, or the source
            * is removeable media, the operation is a copy
            */
         if((chDrive == 'A') || (chDrive == 'B') || (chDrive == 'a') ||
            (chDrive=='b') || pditem->fsControl & DC_REMOVEABLEMEDIA)
         {
            usOperation = (USHORT)DO_COPY;
         }
           /*
            * otherwise, the default operation is a move
            */
         else
         {
            usOperation = (USHORT)DO_MOVE;
         }
      }
      break;

   case DO_MOVE:
   case DO_COPY:
      usOperation = pDraginfo->usOperation;
      break;
   }

  /*
   * In order to support the operation, the source must support the
   * operation this target has decided upon.  The source must also
   * support a rendering mechanism of <DRM_OS2FILE,DRF_UNKNOWN>.
   * This target doesn't care about the file type.
   */

   usIndicator = DOR_DROP;
   cItems = DrgQueryDragitemCount (pDraginfo);

   /*
    * inspect each item to see if it is acceptable
    */
   for (i = 0; i < cItems; ++i)
   {
      pditem = DrgQueryDragitemPtr (pDraginfo, i);

     /*
      * the item is acceptable only if it is copyable and the
      * operation is a copy, or it is moveable and the operation
      * is a move, and it can render <DRM_OS2FILE,DRF_UNKNOWN>
      */
      if (((pditem->fsSupportedOps & DO_COPYABLE) &&
                        (usOperation == (USHORT)DO_COPY)) ||
          ((pditem->fsSupportedOps & DO_MOVEABLE) &&
                          (usOperation == (USHORT)DO_MOVE)))
      {
         if (DrgVerifyRMF(pditem, "DRM_OS2FILE", "DRF_UNKNOWN"))
         {
            usIndicator = DOR_DROP;
         }
         else
         {
            usIndicator = DOR_NEVERDROP;
            break;
         }                            /* endif DrgVerifyRendMechFmt */
      }
      else
      {
         usIndicator = DOR_NODROPOP;
         break;
      }                                     /* endif supported ops        */
   }                                       /* end for all items dragged  */

   if(!DrgFreeDraginfo(pDraginfo))
      if(PMERR_SOURCE_SAME_AS_TARGET != ERRORIDERROR(WinGetLastError(hab)))
      {
         MessageBox(HWND_DESKTOP, IDMSG_DRAGFREEERROR, MB_OK | MB_ERROR, TRUE);
         return (MRFROM2SHORT (DOR_NODROPOP, 0));
      }
   return (MRFROM2SHORT(usIndicator, usOperation));
}  /*  End of DragOver  */


/**************************************************************************
 *
 *  Name       :  Drop (hwndClient, pDraginfo, pCurrentDir)
 *
 *  Description:  This function handles the drop operation
 *
 *                Since we would not receive a DM_DROP message unless a
 *                DOR_DROP reply had been given for a DM_DRAGOVER, we
 *                can assume we have successfully validated that we can
 *                accept the drop. Note that drag would havesent another
 *                DM_DRAGOVER if the operation had changed.
 *
 *                Therefore we don't have to re-validate the operation.
 *                We can go ahead and perform the operation. We will
 *                perform each copy in sequence, but will not capture
 *                the mouse. This should allow the user to perform
 *                other operations, but not require this application
 *                to start another thread and coordinate the ending of
 *                the conversation and freeing up of resources. The user
 *                will not be able to perform any other operations with
 *                this application while the operations are in progress.
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  DrgAccessDraginfo
 *                DrgQueryDragitemPtr
 *                DrgQueryStrName
 *                DrgAddStrHandle
 *                DrgSendTransferMsg
 *                WinStartTimer
 *                WinGetMsg
 *                WinStopTimer
 *                WinDispatchMsg
 *                DrgFreeDragtransfer
 *                DosGetNamedSharedMem
 *                DrgDeleteDraginfoStrHandles
 *                DrgFreeDraginfo
 *                DosSubAllocMem
 *                DosCreateThread
 *
 *  Parameters :  hwndClient = client window handle
 *                pDraginfo  = ointer to a drag information structure
 *                pCurrentDir= the name of the current directory
 *
 *  Return     :  NULL = successful
 *
 *************************************************************************/
MRESULT Drop(HAB hab, HWND hwndClient, PDRAGINFO pDraginfo, PSZ pCurrentDir)
{
   PDRAGTARGETTHREAD     pParm;
   PBYTE          aStack;
   TID           tid;
   USHORT        i;
   PDRAGTRANSFER pdxfer;
   PDRAGITEM     pditem;
   HSTR          hstrRMF;
   char          szFullTargetName  [CCHMAXPATH];
   char          szTargetFile [CCHMAXPATHCOMP];
   QMSG          qmsg;
   USHORT        cRetry, idTimer;
   ULONG         mr, ulBytesWritten;

   /*
    * Get access to the Draginfo structure.
    */
   if(!DrgAccessDraginfo (pDraginfo))
      MessageBox(HWND_DESKTOP, IDMSG_DRAGACCESSERROR, MB_OK | MB_ERROR, TRUE);
   if (pDraginfo->hwndSource == hwndClient)
   {
     /*
      * Source window and target window are the same, so don't do the
      * operation... just release the resources allocated for the drag.
      */
      FreeSourceResources (hab, pDraginfo);
      --ulDragCount;
      PreventClose (hwndClient, ulDragCount);
      return (MRESULT)NULL;
   }

   pditem = DrgQueryDragitemPtr (pDraginfo, 0);

   /*
    * if the source name wasn't provided, let the source render
    */
   if (!pditem->hstrSourceName)
   {
      pdxfer   = DrgAllocDragtransfer (pDraginfo->cditem);
      hstrRMF  = DrgAddStrHandle ("<DRM_OS2FILE, DRF_UNKNOWN>");

      for (i=0; i<pDraginfo->cditem; i++)
      {
         pditem = DrgQueryDragitemPtr (pDraginfo, i);

       /*
        * Build the fully qualified target file name.
        */
         strcpy (szFullTargetName, pCurrentDir);
         RemoveFinalBackslash (szFullTargetName);
         ulBytesWritten =DrgQueryStrName(pditem->hstrTargetName,
                                         sizeof(szTargetFile),
                                         szTargetFile);
         if(ulBytesWritten == 0L)
            MessageBox(HWND_DESKTOP, IDMSG_DRAGQUERYSTRINGERROR,
                           MB_OK | MB_ERROR, TRUE);

         if (szTargetFile[0] != '\\')
            strcat(szFullTargetName, "\\");
         strcat(szFullTargetName, szTargetFile);

       /*
        * initialize the DRAGTRANSFER
        */
         pdxfer[i].cb               = sizeof (DRAGTRANSFER);
         pdxfer[i].hwndClient       = hwndClient;
         pdxfer[i].pditem           = pditem;
         pdxfer[i].hstrSelectedRMF  = hstrRMF;
         pdxfer[i].hstrRenderToName = DrgAddStrHandle (szFullTargetName);
         pdxfer[i].ulTargetInfo     = 0L;
         pdxfer[i].usOperation      = pDraginfo->usOperation;
         pdxfer[i].fsReply          = 0;

         /*
          * send the DM_RENDER to the source, retrying as necessary
          */
         for (cRetry=0, mr=0; cRetry<MAX_RETRIES && !mr; cRetry++)
         {
            mr = (ULONG)DrgSendTransferMsg(pDraginfo->hwndSource,
                                           DM_RENDER,
                                           (MPARAM)(pdxfer + i),
                                           (MPARAM)NULL);

           /*
            * if the source didn't render, retry if we can
            */
            if (!mr)
            {
               if (pdxfer[i].fsReply & DMFL_RENDERRETRY)
               {
                  idTimer = WinStartTimer (0L, 0L, 0L, 1000L);

               /*
                * start a timer to wait, but process
                * messages so we don't hang the queue
                */
                  while (WinGetMsg (0L, &qmsg, 0L, 0, 0))
                  {
                     if ((qmsg.msg == WM_TIMER) &&
                               ((USHORT) qmsg.mp1 == idTimer))
                     {
                        WinStopTimer (0L, 0L, idTimer);
                        break;
                     }
                     else
                        WinDispatchMsg (0L, &qmsg);
                  }
               }
               else
                  DrgFreeDragtransfer(pdxfer + i);
            }
         }
      }
   }

   /*
    * Allocate the stack segment for the target thread.
    */
   else if (DosGetNamedSharedMem((PPVOID)&aStack, (PSZ)"\\sharemem\\dragmem",
                                 PAG_READ | PAG_WRITE) != 0LU)
   {
      MessageBox(HWND_DESKTOP, IDMSG_CANTCREATESTACK, MB_OK | MB_ERROR, TRUE);
      DrgDeleteDraginfoStrHandles(pDraginfo);
      DrgFreeDraginfo(pDraginfo);
   }
   else
   {
     /*
      * Build the parameter list structure for the target thread.
      */
      DosSubAllocMem(aStack, (PPVOID)&pParm, sizeof(DRAGTARGETTHREAD));

      pParm->pDraginfo   = pDraginfo;
      pParm->hwndClient  = hwndClient;
      pParm->usOperation = usOperation;
      strcpy(pParm->szTargetDir, pCurrentDir);

     /*
      * Remember that a drag is in progress and start the target thread.
      */
      ++ulDragCount;
      PreventClose (hwndClient, ulDragCount);

      vpThreadParms = pParm;

      DosCreateThread(&tid, (PFNTHREAD)TargetThread,
                      (ULONG)vpThreadParms, 0U, STACKSIZE);
   }

   return (MRESULT)NULL;
}    /*  End of Drop  */


/**************************************************************************
 *
 *  Name       :  EndTarget(hwnd, pParm)
 *
 *  Description: This function is called whenever a WM_TARGETCOMPLETE
 *               message is received. It releases all resources used
 *               during the drag operation and remembers that the drag
 *               operation has completed.
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  DrgQueryDragitemCount
 *                DrgQueryDragitem
 *                DrgSendTransferMsg
 *                DrgDeleteDraginfoStrHandles
 *                DrgFreeDraginfo
 *                WinPostMsg
 *
 *  Parameters :  hwnd       = window handle
 *                pParm      = points to drag information
 *
 *  Return     :  NULL = successful
 *
 *************************************************************************/
MRESULT EndTarget (HWND hwnd, PDRAGTARGETTHREAD pParm)
{
   USHORT                cItems, i;
   DRAGITEM              Dragitem;

  /*
   * Decrement the count of drag operations in progress and prepare to
   * release the resources allocated for the drag operation.
   */
   --ulDragCount;
   PreventClose (hwnd, ulDragCount);
   cItems = DrgQueryDragitemCount (pParm->pDraginfo);

  /*
   * End the conversation for each item in the DRAGINFO structure and
   * let source know we have completed the drag operation for each item.
   */
   for (i = 0; i < cItems; ++i)
   {
      if(!DrgQueryDragitem (pParm->pDraginfo, sizeof(Dragitem), &Dragitem, i))
         MessageBox(HWND_DESKTOP, IDMSG_DRAGQUERYITEMERROR,
                    MB_OK | MB_ERROR, TRUE);
      DrgSendTransferMsg(Dragitem.hwndItem, DM_ENDCONVERSATION,
                         MPFROMLONG(Dragitem.ulItemID),
                         MPFROMLONG(DMFL_TARGETSUCCESSFUL));
   }
  /*
   * Free the draginfo structure.
   */
   if(DrgDeleteDraginfoStrHandles(pParm->pDraginfo)
                 && DrgFreeDraginfo(pParm->pDraginfo))
      ;
   else
      MessageBox(HWND_DESKTOP, IDMSG_DRAGERROR, MB_OK | MB_ERROR, TRUE);
   WinPostMsg(hwnd, WM_REFRESH, NULL, NULL);
   return (MRESULT)NULL;
}  /*  End of EndTarget  */


/**************************************************************************
 *
 *  Name       :  CheckBusy()
 *
 *  Description:  This call determines if a drag is in progress.
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  [none]
 *
 *  Parameters :  [none]
 *
 *  Return     :  TRUE = a drag is in progress
 *                FALSE = no drag in progress
 *
 *************************************************************************/
BOOL CheckBusy (void)
{
   return ((BOOL) ulDragCount);
}  /*  End of CheckBusy  */


/**************************************************************************
 *
 *  Name       :  CheckClose(hwnd)
 *
 *  Description:  This call determines if a drag is in progress by
 *                checking ulDragCount.  If a drag is in progress,
 *                close is not allowed.
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  WinPostMsg
 *
 *  Parameters :  hwnd = window handle
 *
 *  Return     :  TRUE = a drag is in progress
 *                FALSE = no drag in progress
 *
 *************************************************************************/
MRESULT CheckClose (HWND hwnd)
{
   if (!ulDragCount)
      WinPostMsg (hwnd, WM_QUIT, (MPARAM)0L, (MPARAM)0L);

   return ((MPARAM)NULL);
}  /*  End of CheckClose  */

/**************************************************************************
 *
 *  Name       :  TargetThread()
 *
 *  Description:  This is the main function of this thread.
 *
 *  Concepts   :  direct manipulation
 *
 *  API's      :  DrgQueryDragitemCount
 *                DrgQueryDragitem
 *                DrgQueryStrName
 *                DosCopy
 *                DosDelete
 *                DosEnterCritSec
 *
 *  Parameters :  [none]
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID FAR TargetThread (void)
{
   PDRAGTARGETTHREAD pParm;
   USHORT        cItems, i;
   ULONG         ulBytesWritten;
   DRAGITEM      Dragitem;
   char          szFullSourceName  [CCHMAXPATH];
   char          szFullTargetName  [CCHMAXPATH];
   char          szSourceFile [CCHMAXPATHCOMP];
   char          szTargetFile [CCHMAXPATHCOMP];

   pParm = vpThreadParms;

  /*
   * Get the number of items being dragged.
   */

   cItems = DrgQueryDragitemCount (pParm->pDraginfo);

  /*
   * If the source has provided the file name in the DRAGITEM
   * structures, we can go ahead and do a target copy without getting
   * the source involved in the operation.  We will always copy the file
   * using the suggested target name.  In this application the names
   * are always the same.
   */

   for (i = 0; i < cItems; ++i)
   {
      if(!DrgQueryDragitem (pParm->pDraginfo, sizeof(Dragitem), &Dragitem, i))
         MessageBox(HWND_DESKTOP, IDMSG_DRAGQUERYITEMERROR,
                      MB_OK | MB_ERROR, TRUE);
      if(Dragitem.hstrSourceName    != 0L &&
         Dragitem.hstrTargetName    != 0L &&
         Dragitem.hstrContainerName != 0L)
      {
      /*
       * Build the fully qualified source file name.
       */
         ulBytesWritten = DrgQueryStrName(Dragitem.hstrContainerName,
                                          sizeof(szFullSourceName),
                                          szFullSourceName);
         if(ulBytesWritten == 0)
            MessageBox(HWND_DESKTOP, IDMSG_DRAGQUERYSTRINGERROR,
                       MB_OK | MB_ERROR, TRUE);
         ulBytesWritten = DrgQueryStrName(Dragitem.hstrSourceName,
                                          sizeof(szSourceFile),
                                          szSourceFile);
         if(ulBytesWritten == 0)
            MessageBox(HWND_DESKTOP, IDMSG_DRAGQUERYSTRINGERROR,
                        MB_OK | MB_ERROR, TRUE);

         if (szSourceFile[0] == '\\')
            strcpy (szSourceFile, szSourceFile+1);

         strcat (szFullSourceName, szSourceFile);

      /*
       * Build the fully qualified target file name.
       */
         strcpy (szFullTargetName, pParm->szTargetDir);
         RemoveFinalBackslash (szFullTargetName);
         ulBytesWritten = DrgQueryStrName(Dragitem.hstrTargetName,
                                          sizeof(szTargetFile),
                                          szTargetFile);
         if(ulBytesWritten == 0)
            MessageBox(HWND_DESKTOP, IDMSG_DRAGQUERYSTRINGERROR,
                           MB_OK | MB_ERROR, TRUE);

         if (szTargetFile[0] != '\\')
            strcat (szFullTargetName, "\\");

         strcat (szFullTargetName, szTargetFile);

      /*
       * Do the operation.  DosCopy works with different drives;
       * DosMove does not, so we will use DosCopy and DosDelete.
       */
         switch (pParm->usOperation)
         {
         case DO_MOVE:     /*  Mimic DosMove with DosCopy and DosDelete.  */
             if(DosCopy(szFullSourceName, szFullTargetName, DCPY_EXISTING)
                          == NO_ERROR)
                DosDelete(szFullSourceName);
             break;
         case DO_COPY:
             DosCopy (szFullSourceName, szFullTargetName, DCPY_EXISTING);
             break;
         }                            /* end switch usOperation */
      }                               /* end of target copy     */
   }                                  /* end of dragitem loop   */

  /*
   * Post ourselves a target complete message so the client window
   * knows we are done and can free the stack segment and the draginfo
   * structure.
   */
   DosEnterCritSec();                   /* make sure we finish before the   */
                                        /* main thread frees our stack      */

   WinPostMsg (pParm->hwndClient, WM_TARGETCOMPLETE, pParm, NULL);
}  /*  End of TargetThread  */
/***************************  End of dragdrag.c  *************************/

