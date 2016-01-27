/*static char *SCCSID = "@(#)vmm_user.c	6.11 92/02/19";*/
/*==================================================================*\
 *  VMM_USER.C - routines for handling messages not processed       *
 *              by the standard message processing routine          *
 *      (C) Copyright IBM Corporation 1992                          *
 *------------------------------------------------------------------*
 *                                                                  *
 *  This module contains the code for processing messages sent      *
 *  to the standard window that the standard window does not        *
 *  process.  The application developer need only modify this       *
 *  file in order to implement new menu items or process            *
 *  messages not handled by the standard message routine.           *
 *                                                                  *
 *  This module also contains some routines that demonstate the     *
 *  various dialog box controls and message box types that can      *
 *  be used.  The sample code should be deleted when this           *
 *  module is modified for an application.  The demonstration       *
 *  code is identified by comments.                                 *
 *                                                                  *
 *------------------------------------------------------------------*
 *                                                                  *
 *  This source file contains the following functions:              *
 *                                                                  *
 *      UserCommand(mp1, mp2) - user WM_COMMAND processor           *
 *      VMM_Error(pszFunction, ulErrorCode) - Error processing      *
 *      CleanUpArray(pvAddress) - Page table maintenance            *
 *      StoreInArray(pvAddress, ulNumPages) - Page table maintenance*
 *                                                                  *
\*==================================================================*/
/*------------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs           *
\*------------------------------------------------------------------*/
#include "vmm.h"

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/
MSGENTRY    amsgentry[] =
    {
        { ERROR_INVALID_PARAMETER,
          "%s\n\nInvalid Parameter",
          MB_ICONASTERISK
        },

        { ERROR_ACCESS_DENIED,
          "%s\n\nAccess to Memory Denied",
          MB_ICONASTERISK
        },

        { ERROR_NOT_ENOUGH_MEMORY,
          "%s\n\nInsufficient Memory",
          MB_ICONASTERISK
        },

        { ERROR_INVALID_ACCESS,
          "%s\n\nAn Invalid access to memory was"
          " attempted.",
          MB_ICONASTERISK
        },

        { ERROR_INVALID_ADDRESS,
          "%s\n\nAddress Given is Invalid.",
          MB_ICONASTERISK
        },

        { ERROR_INTERRUPT,
          "%s\n\nThe call was interrupted by an external"
          " event and was not completed. Please retry the"
          " operation.",
          MB_ICONASTERISK
        },

    /* Errors following are application defined */

        { VMERR_MAX_ALLOCATED,
          "%s\n\nThe allocation has surpassed"
          " this application's maximum number of displayable"
          " pages of %u. Each page contains 4096 bytes"
          " of data.",
          MB_ICONASTERISK
        },

        { VMERR_ZERO_ALLOCATED,
          "%s\n\nA minimum of 1 byte must be specified for"
          " an allocation.",
          MB_ICONASTERISK
        },

        { VMERR_TILE_ONLY,
          "%s\n\nAllocation attempted with"
          " tiled attribute and no access"
          " protection specifed. Access protection required to"
          " properly allocate memory.",
          MB_ICONASTERISK
        },

        { VMERR_COMMIT_AND_TILE_ONLY,
          "%s\n\nAllocation attempted with no access"
          " protection specified. Access protection required to"
          " properly allocate memory.",
          MB_ICONASTERISK
        },

        { VMERR_ALREADY_FREED,
          "%s\n\nThe memory address specified to be"
          " freed has not been allocated. Only previously"
          " allocated memory may be freed.",
          MB_ICONASTERISK
        },

        { VMERR_NOT_BASEPAGE,
          "%s\n\nThe memory address specified to be"
          " freed is not a base-page address. Only"
          " base pages"
          " may be freed.",
          MB_ICONASTERISK
        },

        { VMERR_DECOMMIT_RESERVED,
          "%s\n\nThe memory specified to be"
          " decommitted is in a reserved"
          " state. Memory must"
          " be committed before it can be decommitted.",
          MB_ICONASTERISK
        },

        { VMERR_ACCESS_ON_RESERVED,
          "%s\n\nAccess protection specified for"
          " reserved memory. Access protection may "
          " only be specified for committed memory.",
          MB_ICONASTERISK
        },

        { VMERR_ACCESS_AND_DECOMMIT,
          "%s\n\nAccess protection specified when"
          " attempting to decommit memory. No access protection"
          " may be specified when decommitting memory.",
          MB_ICONASTERISK
        },

        { VMERR_COMMIT_ON_COMMITTED,
          "%s\n\nCommit Memory specified for"
          " previously"
          " committed memory. Choose \"Default\" or \""
          "Decommit\" for committed memory.",
          MB_ICONASTERISK
        },

        { VMERR_COMMIT_ONLY,
          "%s\n\nCommit Memory specified with"
          " no access"
          " protection specifed. Access protection required when"
          " committing memory.",
          MB_ICONASTERISK
        },

        { VMERR_SET_NO_PARMS,
          "%s\n\nNo parameters specified. Parameters"
          " must be specified when using %s.",
          MB_ICONASTERISK
        },

        { VMERR_SET_ZERO_SIZE,
          "%s\n\nSetting allocation attributes and/or access protection"
          " on zero bytes is not permitted."
          " A minimum size of one byte must be specified to set"
          " allocation attributes and/or access protection on memory.",
          MB_ICONASTERISK
        },

        { VMERR_SET_ON_FREE,
          "%s\n\nSetting allocation attributes and/or access protection"
          " on non-allocated memory is not"
          " permitted. Memory must be allocated before allocation attributes"
          " or access protection may be specified for it.",
          MB_ICONASTERISK
        },

        { VMERR_DEFAULT,
          "%s\n\n Error #%u occured during the call.",
          MB_ICONASTERISK
        }
    };

/****************************************************************\
 *  DOS API and Application Specific Error reporting routine    *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    VMM_Error(pszFunction, ulErrorCode)                *
 *                                                              *
 *  Purpose: Display errors returned by system calls to         *
 *           the user when they occur. All other error handling *
 *           is done inside the functions themselves            *
 *                                                              *
 *  Usage:   Routine is called for each error that the dialog   *
 *           functions do not process                           *
 *                                                              *
 *  Method:  A switch statement branches control based upon     *
 *           the error number passed. Any message that comes    *
 *           in that is not known is given a generic message.   *
 *           The message is displayed with WinMessageBox        *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID VMM_Error(PSZ pszFunction, ULONG ulErrorCode)
{
    CHAR    szTempBuf[BUFF_SIZE];
    ULONG   ulMsgIndex;

    ulMsgIndex = 0L;
                                                 /* Serach error message */
    while((amsgentry[ulMsgIndex].ulMsgNum != ulErrorCode) &&
          (amsgentry[ulMsgIndex].ulMsgNum != VMERR_DEFAULT))
        ulMsgIndex++;

    strcpy(szTempBuf,amsgentry[ulMsgIndex].szMsgText);

    if (amsgentry[ulMsgIndex].ulMsgNum == VMERR_DEFAULT)
    {
        sprintf(szBuffer,szTempBuf,pszFunction,ulErrorCode);
    }
    else if (amsgentry[ulMsgIndex].ulMsgNum == VMERR_MAX_ALLOCATED)
    {
        sprintf(szBuffer,szTempBuf,pszFunction,MAXPAGES);
    }
    else if (amsgentry[ulMsgIndex].ulMsgNum == VMERR_SET_NO_PARMS)
    {
        sprintf(szBuffer,szTempBuf,pszFunction,pszFunction);
    }
    else
    {
        sprintf(szBuffer,szTempBuf,pszFunction);
    }
    WinAlarm(HWND_DESKTOP, WA_ERROR);
    WinMessageBox(HWND_DESKTOP,
                  hwndMain,
                  szBuffer,
                  szAppName,
                  0,
                  MB_OK | MB_MOVEABLE | amsgentry[ulMsgIndex].usMsgIcon);
    return;
}

/****************************************************************\
 *  Procedure to remove freed pages from array                  *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    CleanUpArray(pvAddress)                            *
 *                                                              *
 *  Purpose: Remove pages that the user indicated from the      *
 *           array of pages stored                              *
 *                                                              *
 *  Usage:   Routine is called after each successful free       *
 *           memory call issued by the user                     *
 *                                                              *
 *  Method:  Pages are removed by finding the address in the    *
 *           array that matches what the user input, and then   *
 *           removing all subsequent pages until another base   *
 *           page is found or the last stored page is reached   *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID CleanUpArray(PVOID pvAddress)
{
    ULONG   ulIndexLow;
    ULONG   ulIndexHigh;

    /* At entry, we know that the pvAddress page is a base page and we
       need to free up the pages associated with the base page for the
       object. To do this, we find the next base address. We then make
       the copies. */

    ulIndexLow = 0;
                                            /* Look for the pointed page */
    while((apgentry[ulIndexLow].pvAddress != pvAddress) &&
          (ulIndexLow < ulFreePage))
        ulIndexLow++;

    if (ulIndexLow < ulFreePage)
    {
        apgentry[ulIndexLow].fBaseAddr = FALSE;
        ulIndexHigh = ulIndexLow + 1;
                                          /* Look for the next base page */
        while((apgentry[ulIndexHigh].fBaseAddr == FALSE) &&
              (ulIndexHigh < ulFreePage))
            ulIndexHigh++;

        while(ulIndexHigh < ulFreePage)
            apgentry[ulIndexLow++] = apgentry[ulIndexHigh++];

        ulFreePage -= (ulIndexHigh-ulIndexLow);/* Reset the starting page */
        MessageBox(HWND_DESKTOP, IDMSG_SUCCESSFREE, "Note !",  MB_OK, FALSE);
    }
              /* If DosFreeMem worked but the address is not in our page
              array, then don't modify the array or the ulFreePage count. */

    if (!ulFreePage)         /* No memory is allocated */
    {
       EnableMenuItem(hwndMain,IDM_VMMFREE,FALSE);
       EnableMenuItem(hwndMain,IDM_VMMSET,FALSE);
       EnableMenuItem(hwndMain,IDM_VMMWRITE,FALSE);
       EnableMenuItem(hwndMain,IDM_VMMREAD,FALSE);
    }
    return;
}

/****************************************************************\
 *  Procedure to store allocated pages in array                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    StoreInArray(pvAddress,ulNumPages)                 *
 *                                                              *
 *  Purpose: Store pages that were just successfully allocated  *
 *           in our array so we can track them                  *
 *                                                              *
 *  Usage:   Routine is called after each successful allocation *
 *           of memory by the user                              *
 *                                                              *
 *  Method:  The pages are added to the end of the array based  *
 *           upon the current status of our global end of array *
 *           index variable (ulFreePage)                        *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID StoreInArray(PVOID pvAddress, ULONG ulNumPages )
{
    ULONG   temp;

    /* Set all base indicators for the new pages to FALSE, and set the
       addresses for the pages. */

    for(temp = 0L; temp < ulNumPages; temp++)
    {
        apgentry[temp+ulFreePage].pvAddress = (PVOID)((ULONG)pvAddress +
                                              (PAGESIZE * temp));
        apgentry[temp+ulFreePage].fBaseAddr = FALSE;
    }
    /* Set the base indicator of the first page in the object to be
       TRUE */
    apgentry[ulFreePage].fBaseAddr = TRUE;
    ulFreePage += ulNumPages;       /* Move to new starting of free page */

    EnableMenuItem(hwndMain,IDM_VMMFREE,TRUE);
    EnableMenuItem(hwndMain,IDM_VMMSET,TRUE);
    EnableMenuItem(hwndMain,IDM_VMMWRITE,TRUE);
    EnableMenuItem(hwndMain,IDM_VMMREAD,TRUE);
    return;
}

/****************************************************************\
 *  Non-standard menu item command processing procedure         *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   UserCommand(mp1, mp2)                               *
 *                                                              *
 *  Purpose: Process any WM_COMMAND messages send to hwndMain   *
 *              that are not processed by MainCommand           *
 *                                                              *
 *  Usage:  Routine is called for each WM_COMMAND that is       *
 *          not posted by a standard menu item                  *
 *                                                              *
 *  Method: A switch statement branches control based upon      *
 *          the id of the control which posted the message      *
 *                                                              *
 *  Returns:                                                    *
\****************************************************************/
VOID UserCommand(MPARAM mp1, MPARAM mp2)
{
   PVOID   pvMemAddress=0L;

   ULONG   ulTempRegionSize;
   ULONG   flTempAttributes=0L;

   ULONG   ulPages;                /* Number of pages requested by user */
   ULONG   rc;                      /* Return Code variable from calls  */
   ULONG   rc2;         /* Return Code variable for investigative calls */

   switch(SHORT1FROMMP(mp1))
   {
   /*--------------------------------------------------------------*\
    *  Add case statements for menuitem ids you wish to process    *
   \*--------------------------------------------------------------*/
      case IDM_VMMALLOC:
          ObjAlloc.ulSize = 4;         /* Default size 4 k to allocate */
          ObjAlloc.ulAttr =                          /* Default attrib */
                    PAG_READ | PAG_WRITE | PAG_EXECUTE;
          ObjAlloc.pvAddress = '\000';

          if(WinDlgBox(HWND_DESKTOP,
                       hwndMain,
                       (PFNWP) AllocMemDlgProc,
                       (HMODULE)0,
                       (ULONG) IDD_ALLOCMEM,
                       (PVOID) &ObjAlloc) )
          {
                          /* Figure out the number of pages requested. */
              ulPages = (ObjAlloc.ulSize / PAGESIZE);
              if ((ObjAlloc.ulSize % PAGESIZE) != 0L)
                  ulPages++;                          /* One more page */

              /* If the number of pages requested is zero (zero
                 bytes specified), or the number of pages is greater
                 than the number left in the array, report the error
                 condition. Otherwise, make the DosAllocMem call and
                 set any error condition returned by the call. */

              if (ulPages == 0L)
              {
                  rc = VMERR_ZERO_ALLOCATED;
              }
              else if (ulPages + ulFreePage > MAXPAGES + 1)
              {
                  rc = VMERR_MAX_ALLOCATED;
              }
              else
              {
                  rc = DosAllocMem(&pvMemAddress,
                                   ObjAlloc.ulSize,
                                   ObjAlloc.ulAttr);
              }
              /* If an error has occured, display it. Otherwise,
                 process the request. */

              if (rc != 0L)
              {
                  if (ObjAlloc.ulAttr == 0L)
                  {
                      rc = VMERR_SET_NO_PARMS;
                  }
                  else if (ObjAlloc.ulAttr == PAG_COMMIT)
                  {
                      rc = VMERR_COMMIT_ONLY;
                  }
                  else if (ObjAlloc.ulAttr == (PAG_COMMIT | OBJ_TILE))
                  {
                      rc = VMERR_COMMIT_AND_TILE_ONLY;
                  }
                  else if (ObjAlloc.ulAttr == OBJ_TILE)
                  {
                      rc = VMERR_TILE_ONLY;
                  }
                  VMM_Error("DosAllocMem()",rc);
              }
              else
              {
                  StoreInArray(pvMemAddress,ulPages);
                  WinInvalidateRect(hwndMain, NULL, TRUE);
              }
          }
          break;

      case IDM_VMMFREE:

          ObjAlloc.pvAddress = '\000';
          if (WinDlgBox(HWND_DESKTOP,
                    hwndMain,
                    (PFNWP) FreeMemDlgProc,
                    (HMODULE)0,
                    (ULONG) IDD_FREEMEM,
                    (PVOID) &ObjAlloc ) != 0)
          {
              rc = DosFreeMem(ObjAlloc.pvAddress);

              if (rc != 0L)
              {                                  /* DosFreeMem is fail */
                  /* Try to figure out why the error occured based
                     on actual page attributes of page requested to be
                     freed. */
                  rc2 = DosQueryMem(ObjAlloc.pvAddress,
                                    &ulTempRegionSize,
                                    &flTempAttributes);
                  if (rc2 == 0L)
                  /* If the page requested to be freed was already
                     in the freed state, or the requested page in the
                     object to be freed is not the base page of the
                     object, set the appropriate error condition. */
                  {
                      if ((flTempAttributes & PAG_FREE) != 0L)
                      {
                          rc = VMERR_ALREADY_FREED;
                      }
                      else if ((flTempAttributes & PAG_BASE) == 0L)
                      {
                          rc = VMERR_NOT_BASEPAGE;
                      }
                  }
                  VMM_Error("DosFreeMem()",rc);
              }
              else
              {
                  CleanUpArray(ObjAlloc.pvAddress);
                  WinInvalidateRect(hwndMain, NULL, TRUE);
              }
          }
          break;

      case IDM_VMMSET:

          ObjAlloc.pvAddress = '\000';
          if (WinDlgBox(HWND_DESKTOP,
                        hwndMain,
                        (PFNWP) SetMem1DlgProc,
                        (HMODULE)0,
                        (ULONG) IDD_SETMEM1,
                        (PVOID) &ObjAlloc ))
          {
              if (WinDlgBox(HWND_DESKTOP,
                            hwndMain,
                            (PFNWP) SetMem2DlgProc,
                            (HMODULE)0,
                            (ULONG) IDD_SETMEM2,
                            (PVOID) &ObjAlloc ))
              {
                  rc = DosSetMem(ObjAlloc.pvAddress,
                                 ObjAlloc.ulSize,
                                 ObjAlloc.ulAttr);

                  /* If an error occured making the DosSetMem call,
                     try to figure out the cause of the error by
                     studying the requested access and the access on
                     the memory prior to the DosSetMem call. */

                  if (rc != 0L)
                  {
                      ulTempRegionSize = 1L;

                      rc2 = DosQueryMem(ObjAlloc.pvAddress,
                                        &ulTempRegionSize,
                                        &flTempAttributes);
                      if (rc2 == 0L)
                      {
                          rc = 0L;
                          /* If no attributes, commit with no other
                             attributes, or a size of zero was
                             specified, set the appropriate error
                             condition. */

                          if (ObjAlloc.ulAttr == 0L)
                          {
                              rc = VMERR_SET_NO_PARMS;
                          }
                          else if (ObjAlloc.ulAttr == PAG_COMMIT)
                          {
                              rc = VMERR_COMMIT_ONLY;
                          }
                          else if (ObjAlloc.ulSize == 0L)
                          {
                              rc = VMERR_SET_ZERO_SIZE;
                          }
                          /* If memory is not allocated then set
                             error condition. */

                          else if ((ObjAlloc.ulAttr & PAG_FREE) != 0L)
                          {
                              rc = VMERR_SET_ON_FREE;
                          }
                          /* If the memory was already in the
                             reserved state and the user attempted to
                             decommit it or set attributes for it, set
                             the error condition. */

                          else if ((flTempAttributes & PAG_COMMIT) == 0L)
                          {
                              if ((ObjAlloc.ulAttr & PAG_DECOMMIT) != 0L)
                              {
                                  rc = VMERR_DECOMMIT_RESERVED;
                              }
                              else if ((ObjAlloc.ulAttr & ~PAG_COMMIT) != 0L)
                              {
                                  rc = VMERR_ACCESS_ON_RESERVED;
                              }
                          }
                          /* If the memory was in a committed state
                             and the user attempted to decommit it and
                             set attributes, or the user attempts to
                             commit the memory, set the appropriate
                             error condition. */
                          else
                          {
                              if (((ObjAlloc.ulAttr &  PAG_DECOMMIT) != 0L)
                                && ((ObjAlloc.ulAttr & ~PAG_DECOMMIT) != 0L))
                              {
                                  rc = VMERR_ACCESS_AND_DECOMMIT;
                              }
                              else if ((ObjAlloc.ulAttr & PAG_COMMIT) != 0L)
                              {
                                  rc = VMERR_COMMIT_ON_COMMITTED;
                              }
                          }
                      }
                      VMM_Error("DosSetMem()",rc);
                  }
                  else
                  {
                      WinInvalidateRect(hwndMain, NULL, TRUE);
                  }
              }
          }
          break;

      case IDM_VMMWRITE:

          ObjAlloc.pvAddress = '\000';
          if (WinDlgBox(HWND_DESKTOP,
                        hwndMain,
                        (PFNWP) WriteMemDlgProc,
                        (HMODULE)0,
                        (ULONG) IDD_WRITEMEM,
                        (PVOID) NULL))

              WinInvalidateRect(hwndMain, NULL, TRUE);
          break;

      case IDM_VMMREAD:

          ObjAlloc.pvAddress = '\000';
          if (WinDlgBox(HWND_DESKTOP,
                        hwndMain,
                        (PFNWP) ReadMemDlgProc,
                        (HMODULE)0,
                        (ULONG) IDD_READMEM,
                        (PVOID) &ObjAlloc ))
          {
             if ( *( (CHAR *)(ObjAlloc.pvAddress) )== 0L )
             {
              sprintf(szBuffer, "Address %p doesn't contain data.",
                                    ObjAlloc.pvAddress);
             } else {
                     sprintf(szBuffer, "Address %p contains\n %s",
                               ObjAlloc.pvAddress, ObjAlloc.pvAddress);
                    }
              WinMessageBox(HWND_DESKTOP,
                            hwndMain,
                            szBuffer,
                            "Read Memory",
                            0,
                            MB_INFORMATION| MB_OK | MB_MOVEABLE);

              WinInvalidateRect(hwndMain, NULL, TRUE);
          }
          break;

      default:
          break;
   }
   /* This routine currently doesn't use the mp2 parameter but       *\
    *  it is referenced here to prevent an 'Unreferenced Parameter'  *
   \*  warning at compile time.                                      */
   return;
}                                                    /* UserCommand() */
