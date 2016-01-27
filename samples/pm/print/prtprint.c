/**************************************************************************
 *  File name  :  prtprint.c
 *
 *  Description:  This module contains all the functions needed to display
 *                a listbox with the valid printer queues and to select a
 *                printer and the print properties.
 *
 *                This source file contains the following functions:
 *
 *                Print( hwnd, pmp )
 *                PrintingDlgProc( hwnd, msg, mp1, mp2 )
 *                PrintDlgProc( hwnd, msg, mp1, mp2 )
 *                QueryPrintQueue( hwnd, pmp, fShowDialog)
 *                QueryPrintDlgProc( hwnd, msg, mp1, mp2 )
 *                QueryJobProperties( hab, pQueueInfo )
 *                ProcessUserPrint( hwnd, pmp )
 *                PrinterBitblt( hwnd, pmp )
 *                PrinterPaginate( hwnd, pmp )
 *                PrinterPlayMetaFile( hwnd, pmp )
 *                FindQueue( pszQueueName, pQueueInfo, sCount )
 *                SortQueues( pQueue1, pQueue2 )
 *
 *  Concepts   :  Listbox, printer queue display and printer property dislpay
 *                and modification.
 *
 *  API's      :  DevCloseDC
 *                DevEscape
 *                DevOpenDC
 *                DevPostDeviceModes
 *                DevQueryCaps
 *
 *                GpiAssociate
 *                GpiBitblt
 *                GpiConvert
 *                GpiCreateLogFont
 *                GpiCreatePS
 *                GpiDeleteBitmap
 *                GpiDeleteSetId
 *                GpiDestroyPS
 *                GpiPlayMetaFile
 *                GpiQueryPS
 *                GpiSetBitmap
 *                GpiSetCharSet
 *
 *                PrfQueryProfileString
 *
 *                SplEnumQueue
 *
 *                WinDefDlgProc
 *                WinDismissDlg
 *                WinDlgBox
 *                WinEnableControl
 *                WinLoadDlg
 *                WinLoadString
 *                WinMessageBox
 *                WinPostMsg
 *                WinQueryDlgItemShort
 *                WinQueryDlgItemText
 *                WinQueryWindowULong
 *                WinSendMsg
 *                WinSetDlgItemShort
 *                WinSetDlgItemText
 *                WinSetWindowULong
 *                WinSendDlgItemMsg
 *                WinShowWindow
 *
 *    Files    :  OS2.H, PRTSAMP.H, PRTSDLG.H, PRTSHLP.H, PMASSERT.H
 *
 *  Copyright (C) 1991-1993 IBM Corporation
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

/* Include the required sections from the PM header file.  */
#define INCL_DEV
#define INCL_DOSMISC
#define INCL_GPIBITMAPS
#define INCL_GPICONTROL
#define INCL_GPILCIDS
#define INCL_GPIMETAFILES
#define INCL_GPIPRIMITIVES
#define INCL_GPITRANSFORMS
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_WINBUTTONS
#define INCL_WINDIALOGS
#define INCL_WINENTRYFIELDS
#define INCL_WINERRORS
#define INCL_WINHELP
#define INCL_WININPUT
#define INCL_WINLISTBOXES
#define INCL_WINMENUS
#define INCL_WINMESSAGEMGR
#define INCL_WINSTDFILE
#define INCL_WINSTDFONT
#define INCL_WINWINDOWMGR
#define INCL_WINSHELLDATA 
#include <os2.h>

/* c language includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <process.h>
#include <memory.h>
#include <sys\types.h>
#include <sys\stat.h>

/* application includes */
#include "prtsamp.h"
#include "prtsdlg.h"
#include "pmassert.h"
#include "prtshlp.h"


/* static function prototypes */

static int _Optlink SortQueues(const void *pQueue1, const void *pQueue2);
MRESULT EXPENTRY PrintDlgProc(HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY PrintingDlgProc(HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY QueryPrintDlgProc(HWND, USHORT, MPARAM, MPARAM);
BOOL QueryJobProperties(HAB hab, PPRQINFO3 pPrQ);
LONG FindQueue(PSZ pszQueueName, PPRQINFO3 pQueueInfo, LONG lCount);
VOID PrinterPaginate(HWND hwnd, PMAIN_PARM pmp);
VOID PrinterBitblt(HWND hwnd, PMAIN_PARM pmp);
VOID PrinterPlayMetaFile(HWND hwnd, PMAIN_PARM pmp);
VOID ResetPrinterInfoDC( PMAIN_PARM pmp );

/*************************************************************************
 *  Function   :  Print
 *
 *  Description:  This function displays the print dialog.
 *
 *  API's      :  WinDlgBox
 *                WinSendMsg
 *                WinLoadDlg
 *
 *  Parameters :  HWND         window handle of frame window
 *                PMAIN_PARM   global application data
 *
 *  Return     :  TRUE if print successful, otherwise FALSE
 *
 *************************************************************************/
BOOL Print(HWND hwnd, PMAIN_PARM pmp)
{
   SHORT      index;
   PPRQINFO3  pQueueInfo;
   ULONG      ul;
   ULONG      cbNeeded;
   BOOL       bOK;
   CHAR       szWork[LEN_WORKSTRING],szTmp[LEN_WORKSTRING];

   /*
    * Ensure that queue is correct, the user may have initiated a print
    * without going thruogh any other of the dialogs
    */
   QueryPrintQueue( pmp, FALSE );

   /* Query to get latest forms data */
   pmp->cForms = QueryPrinterForms( pmp, pmp->hcinfo, MAX_FORMS );
   pmassert( pmp->hab, pmp->cForms );

   /*
    * Check that the form selected by the user in page setup matches the
    * form in job properties
    */
   index = FindForm( pmp->form.achFormName, pmp->hcinfo, pmp->cForms );
   pmassert( pmp->hab, index != -1L);
   if (! (pmp->hcinfo[index].flAttributes & HCAPS_CURRENT))
   {
      /* display warning message box */
      bOK = WinLoadString( pmp->hab,
                           (HMODULE)NULLHANDLE,
                           ERRMSG_WRONG_FORM,
                           LEN_WORKSTRING,
                           szTmp );
      pmassert( pmp->hab, bOK );
      sprintf( szWork, szTmp, pmp->form.achFormName );
      WinMessageBox( HWND_DESKTOP,
                     pmp->hwndFrame,
                     szWork,
                     pmp->pszTitle,
                     (USHORT)0,
                     MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);

      /* query queue information */
      ul = SplQueryQueue( NULL, pmp->achQueueName, 3, NULL, 0L, &cbNeeded );
      pmassert( pmp->hab, ul );
      pQueueInfo = malloc( cbNeeded );
      ul = SplQueryQueue( NULL, pmp->achQueueName, 3, pQueueInfo, cbNeeded,
                          &cbNeeded );
      pmassert( pmp->hab, ul == 0 );

     /*
      * store current driver data info into SplEnumQueue info
      * so that when user wants to change a print property
      * but not necessarily a printer, his previous settings
      * will be shown in the print properties dialog
      */
     if( pQueueInfo->pDriverData->cb == pmp->cbDriverDataLen )
     {
       memcpy( pQueueInfo->pDriverData, pmp->pDriverData, pmp->cbDriverDataLen );
     }

     /* display job properties dialog */
     QueryJobProperties( pmp->hab, pQueueInfo);

     /* destroy allocations for printer driver data */
     if( pmp->pDriverData )
     {
         free( pmp->pDriverData );
         pmp->pDriverData      = NULL;
         pmp->cbDriverDataLen  = 0;
     }

     /* store new driver data length in pmp  */
     pmp->cbDriverDataLen  = pQueueInfo->pDriverData->cb;

     /* store new driver data in pmp  */
     pmp->pDriverData = (PDRIVDATA) malloc( pmp->cbDriverDataLen );
     pmassert( pmp->hab, pmp->pDriverData );
     memcpy( pmp->pDriverData, pQueueInfo->pDriverData, pmp->cbDriverDataLen );

     /* destroy allocation for Queue information */
     free( pQueueInfo );

     /*
      * reset the printer info device context so the changed form
      * will be found next time
      */
     ResetPrinterInfoDC( pmp );
   }

   /*
    * Query user print options. See the PrintDlgProc for details.
    */
   if (WinDlgBox( HWND_DESKTOP,
                  hwnd,
                  (PFNWP)PrintDlgProc,
                  (HMODULE)NULLHANDLE,
                  IDD_PRINT,
                  (PVOID)pmp) )
   {
      /*
       * User wants to print, so start print job and display text
       * showing job printing status.
       */
      WinSendMsg( hwnd, WM_USER_DISABLE_CLIENT, (MPARAM)0L, (MPARAM)0L );

      pmp->hwndPrinting = WinLoadDlg( HWND_DESKTOP,
                                      hwnd,
                                      (PFNWP)PrintingDlgProc,
                                      (HMODULE)NULLHANDLE,
                                      IDD_PRINTING,
                                      (PVOID)pmp);
   }
   return FALSE;
}

/*************************************************************************
 *  Function   : PrintingDlgProc
 *
 *  Description:  This function displays the print dialog.
 *
 *  API's      :  WinPostMsg
 *                WinDismissDlg
 *                WinEnableControl
 *                WinSetDlgItemText
 *                WinDefDlgProc
 *
 *  Parameters :  HWND         window handle of printer selection dialog
 *                USHORT       message
 *                MPARAM       message parameter 1
 *                MPARAM       message parameter 2
 *
 *  Return     :  message result
 *
 *************************************************************************/
MRESULT EXPENTRY PrintingDlgProc(HWND hwnd,
                                 USHORT msg,
                                 MPARAM mp1,
                                 MPARAM mp2)
{
   static PMAIN_PARM   pmp;
   PSZ                 psz;
   CHAR                szWork[LEN_WORKSTRING], szPrintString[LEN_WORKSTRING];
   BOOL                bOK;

   switch (msg)
   {
   case WM_INITDLG:
      /* Get pointer to application specific data */
      pmp = (PMAIN_PARM) PVOIDFROMMP(mp2);

      WinSetDlgItemText(hwnd, IDD_PRINTPRINTER, (PSZ)pmp->achQueueDesc);

      /* Show the document name */
      psz = strrchr( pmp->szFilename, '\\' );
      if (psz && *psz)
      {
          ++psz;
      }
      else
      {
          psz = pmp->szFilename;
      }
      WinSetDlgItemText(hwnd, IDD_PRINTDOCUMENT, psz);

      /* set focus to cancel pushbutton */
      WinSetFocus (HWND_DESKTOP, WinWindowFromID (hwnd, (USHORT)DID_CANCEL));

      /*
       * send WM_USER_PRINT message
       * disable client -- object window enables client
       *   when complete
       */
      pmp->fCancel = FALSE;
      WinPostMsg(pmp->hwndObject, WM_USER_PRINT, (MPARAM)hwnd, (MPARAM)0L);
      return 0L;

   case WM_USER_ACK:
      /*
       *  The job has either completed or aborted, so tell the client
       *  window and get out.
       */

      WinPostMsg(pmp->hwndClient, WM_USER_ACK, 0L, 0L);

      WinDismissDlg(hwnd, FALSE);
      return 0L;

   case WM_USER_NEW_PAGE:
      /*
       * Increment the page counter on the print confirmation dialog
       */
      bOK = WinLoadString( pmp->hab,
                           (HMODULE)NULLHANDLE,
                           PRINT_PAGE_MSG,
                           LEN_WORKSTRING,
                           szPrintString );
      pmassert( pmp->hab, bOK );
      sprintf(szWork, szPrintString, (ULONG)mp1);
      WinSetDlgItemText( hwnd, IDD_PRINTINGTEXT, szWork);

      /* set focus to cancel pushbutton */
      WinSetFocus (HWND_DESKTOP, WinWindowFromID (hwnd, (USHORT)DID_CANCEL));
      break;

   case WM_USER_SKIP_PAGE:
      /*
       * Skip the page counter on the print confirmation dialog
       */
      bOK = WinLoadString( pmp->hab,
                           (HMODULE)NULLHANDLE,
                           PRINT_SCAN_MSG,
                           LEN_WORKSTRING,
                           szPrintString );
      pmassert( pmp->hab, bOK );
      sprintf(szWork, szPrintString, (ULONG)mp1);
      WinSetDlgItemText( hwnd, IDD_PRINTINGTEXT, szWork);

      /* set focus to cancel pushbutton */
      WinSetFocus (HWND_DESKTOP, WinWindowFromID (hwnd, (USHORT)DID_CANCEL));
      break;

   case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
      {
      case DID_CANCEL:
         pmp->fCancel = TRUE;
         WinEnableControl( hwnd, DID_CANCEL, FALSE );
         bOK = WinLoadString( pmp->hab,
                              (HMODULE)NULLHANDLE,
                              PRINT_ABORT_MSG,
                              LEN_WORKSTRING,
                              szWork );
         pmassert( pmp->hab, bOK );
         WinSetDlgItemText( hwnd, IDD_PRINTINGTEXT, szWork);
         return 0;
      }
      break;
   }
   return( WinDefDlgProc( hwnd, msg, mp1, mp2 ) );
}  /* End of PrintingDlgProc */

/*************************************************************************
 *  Function   :  PrintDlgProc
 *
 *  Description:
 *
 *  API's      :  WinSetDlgItemText
 *                WinSetDlgItemShort
 *                WinShowWindow
 *                WinSendMsg
 *                WinQueryDlgItemShort
 *                WinQueryDlgItemText
 *                WinDismissDlg
 *                WinDefDlgProc
 *
 *  Parameters :  HWND         window handle of printer selection dialog
 *                USHORT       message
 *                MPARAM       message parameter 1
 *                MPARAM       message parameter 2
 *
 *  Return     :  MRESULT         message result
 *
 *************************************************************************/
MRESULT EXPENTRY PrintDlgProc(HWND hwnd,
                              USHORT msg,
                              MPARAM mp1,
                              MPARAM mp2)
{
   static PMAIN_PARM pmp;
   PSZ         psz;
   ULONG       ulPanel;
   USHORT      usCopies, usFirstPage, usLastPage;
   CHAR        szEntryField[4];            /* text contents of entry field */

   switch (msg)
   {
   case WM_INITDLG:
      /* Get pointer to application specific data */
      pmp = (PMAIN_PARM) PVOIDFROMMP(mp2);

      /* Show the printer name */
      WinSetDlgItemText(hwnd, IDD_PRTPRINTER, (PSZ)pmp->achQueueDesc);

      /* Show the document name */
      psz = strrchr( pmp->szFilename, '\\' );
      if (psz && *psz)
      {
          ++psz;
      }
      else
      {
          psz = pmp->szFilename;
      }
      WinSetDlgItemText(hwnd, IDD_PRTDOCUMENT, psz);

      /* Make entry fields read only */
      WinSendMsg( WinWindowFromID( hwnd, IDD_PRTPRINTER ),
                  EM_SETREADONLY, (MPARAM)TRUE, (MPARAM)0L );
      WinSendMsg( WinWindowFromID( hwnd, IDD_PRTDOCUMENT ),
                  EM_SETREADONLY, (MPARAM)TRUE, (MPARAM)0L );

      /* Initialize the number of copies */
      WinSetDlgItemShort(hwnd, IDD_PRTCOPIES, pmp->usCopies, FALSE);

      /*
       * If the file is TEXT then set the page fields. Otherwise reset the
       * fields and disable the all fields and pushbuttons
       */
      if (pmp->ulMode == MODE_TEXT)
      {
         WinSetDlgItemShort(hwnd, IDD_PRTFIRSTPAGE, pmp->usFirstPage, FALSE);
         WinSetDlgItemShort(hwnd, IDD_PRTLASTPAGE, pmp->usLastPage, FALSE);
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTFIRSTPAGE), TRUE);
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTLASTPAGE), TRUE);
      }
      else
      {
         WinSetDlgItemShort(hwnd, IDD_PRTFIRSTPAGE, 1, FALSE);
         WinSetDlgItemShort(hwnd, IDD_PRTLASTPAGE, 1, FALSE);
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTALL), FALSE);
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTSELECT), FALSE);
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTFIRSTPAGE), FALSE);
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTLASTPAGE), FALSE);
      }

      /* Set the state of the page select pushbuttons */
      if (pmp->fPrintAllPages)
      {
         WinCheckButton( hwnd, IDD_PRTALL, 1);
         WinCheckButton( hwnd, IDD_PRTSELECT, 0);
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTFIRSTPAGE), FALSE);
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTLASTPAGE), FALSE);
      }
      else
      {
         WinCheckButton( hwnd, IDD_PRTALL, 0);
         WinCheckButton( hwnd, IDD_PRTSELECT, 1);
      }

      /*
       * Hide the preview control.  The reason it is included in the
       * print sample DLG file is for the benefit of programmers who
       * want their dialogs to look exactly like those presented in
       * the Programming Guide for printing (Chapter 18).
       */
      WinShowWindow(WinWindowFromID(hwnd, IDD_PRTPREVIEW), FALSE);

      return 0;

   case WM_HELP:
      if( pmp->hwndHelp )
      {
         ulPanel = PANEL_PRINTDLG;
         WinSendMsg( pmp->hwndHelp, HM_DISPLAY_HELP,
                   (MPARAM)&ulPanel, (MPARAM)HM_RESOURCEID );
         return (MRESULT)NULL;
      }
      break;

   case WM_CONTROL:
      switch(SHORT1FROMMP(mp1))
      {
      case IDD_PRTDOCUMENT:
      case IDD_PRTPRINTER:
         /* these fields are read-only */
         return 0;

      case IDD_PRTCOPIES:
         if (SHORT2FROMMP(mp1) == EN_CHANGE)
         {
            /* Verify that a non-zero number was typed in the entry field. */
            if (!WinQueryDlgItemShort( hwnd, IDD_PRTCOPIES,
                                       (PSHORT)&usCopies, FALSE) ||
                                       !usCopies )
            {
               /* If the field has length, clear the value and beep */
               if (WinQueryDlgItemText( hwnd, IDD_PRTCOPIES,
                                        sizeof(szEntryField), szEntryField ) )
               {
                  WinSetDlgItemText( hwnd, IDD_PRTCOPIES, "" );
                  DosBeep(600, 5);
               }
            }
         }
         return 0;

      case IDD_PRTALL:
         /* check buttons appropriately */
         WinCheckButton( hwnd, IDD_PRTALL, 1);
         WinCheckButton( hwnd, IDD_PRTSELECT, 0);
         /* disable first and last page entry fields */
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTFIRSTPAGE), FALSE);
         WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTLASTPAGE), FALSE);
         return 0;

      case IDD_PRTSELECT:
         /* check buttons appropriately */
         WinCheckButton( hwnd, IDD_PRTALL, 0);
         WinCheckButton( hwnd, IDD_PRTSELECT, 1);
         /*
          * if the mode is TEXT and the page selection is active, enable the
          * the first and last page controls otherwise disable them
          */
         if (pmp->ulMode == MODE_TEXT)
         {
            WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTFIRSTPAGE), TRUE);
            WinEnableWindow(WinWindowFromID(hwnd, IDD_PRTLASTPAGE), TRUE);
         }
         return 0;

      case IDD_PRTFIRSTPAGE:
         if (SHORT2FROMMP(mp1) == EN_CHANGE)
         {
            /* Verify that a non-zero number was typed in the entry field. */
            if (!WinQueryDlgItemShort( hwnd, IDD_PRTFIRSTPAGE,
                                       (PSHORT)&usFirstPage, FALSE) ||
                                       !usFirstPage )
               /* If the field has length, clear the value and beep */
               if (WinQueryDlgItemText( hwnd, IDD_PRTFIRSTPAGE,
                                        sizeof(szEntryField), szEntryField ) )
               {
                  WinSetDlgItemText( hwnd, IDD_PRTFIRSTPAGE, "" );
                  DosBeep(600, 5);
               }
         }
         return 0;

      case IDD_PRTLASTPAGE:
         if (SHORT2FROMMP(mp1) == EN_CHANGE)
         {
            /* Verify that a non-zero number was typed in the entry field. */
            if (!WinQueryDlgItemShort( hwnd, IDD_PRTLASTPAGE,
                                       (PSHORT)&usLastPage, FALSE) ||
                                       !usLastPage )
            {
               /* If the field has length, clear the value and beep */
               if (WinQueryDlgItemText( hwnd, IDD_PRTLASTPAGE,
                                        sizeof(szEntryField), szEntryField ) )
               {
                  WinSetDlgItemText( hwnd, IDD_PRTLASTPAGE, "" );
                  DosBeep(600, 5);
               }
            }
         }
         return 0;


      case IDD_PRTPREVIEW:
         /* this field is not used in this sample program */
         return 0;
      }
      break;

   case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
      {
      case DID_OK:
         /* get number of copies and store in global data structure */
         WinQueryDlgItemShort( hwnd, IDD_PRTCOPIES,
                               (PSHORT)&pmp->usCopies, FALSE);

         /* print all pages or selective pages */
         pmp->fPrintAllPages =
             (BOOL)WinSendDlgItemMsg( hwnd, IDD_PRTALL, BM_QUERYCHECK,
                                      (MPARAM)TRUE, (MPARAM)0L );

         /*
          * If mode is TEXT, get first and last page, even though
          * page select may be greyed
          */
         if (pmp->ulMode == MODE_TEXT)
         {
            WinQueryDlgItemShort( hwnd, IDD_PRTFIRSTPAGE,
                                  (PSHORT)&pmp->usFirstPage, FALSE);
            WinQueryDlgItemShort( hwnd, IDD_PRTLASTPAGE,
                                  (PSHORT)&pmp->usLastPage, FALSE);
         }
         if (pmp->usLastPage < pmp->usFirstPage)
         {
            pmp->fPrintAllPages = TRUE;
            pmp->usFirstPage = 1;
            pmp->usLastPage = 1;
         }

         WinDismissDlg(hwnd, TRUE);
         return 0;

      case DID_CANCEL:
         WinDismissDlg(hwnd, FALSE);
         return 0;

      case DID_HELP_BUTTON:
         /* don't do anything yet */
         return 0;
      }
      break;

   }
   return(WinDefDlgProc(hwnd, msg, mp1, mp2));

} /* End of PrintDlgProc */

/*************************************************************************
 *  Function   :  QueryPrintQueue
 *
 *  Description:  Query all printer queues, display them in a listbox
 *                and do a selection.
 *
 *  Parameters:   PMAIN_PARM  pmp has global application data
 *                fShowDialog     a flag:  should the dialog be displayed?
 *
 *  API's      :  SplEnumQueue
 *                WinLoadString
 *                WinMessageBox
 *                DosQuerySysInfo
 *                PrfQueryProfileString
 *                WinDlgBox
 *
 *  Return     :  TRUE if printer changed, otherwise FALSE
 *
 *************************************************************************/
BOOL QueryPrintQueue(PMAIN_PARM pmp, BOOL fShowDialog)
{
    ULONG           ul, cReturned, cTotal, cbNeeded;
    PPRQINFO3       pQueueInfo, pqi;
    LONG            i;
    BOOL            fResult, bOK;
    CHAR            szWork[LEN_WORKSTRING], *pch;
    ULONG           aulVersion[2];

   /*
    * Get count of total number of print queues and the size of the
    * buffer needed (cbNeeded) to hold them.
    */
   ul = SplEnumQueue( NULL,          /* pszComputerName - local machine */
                      3,             /* information level 3 requested   */
                      NULL,          /* pBuf - NULL for first call      */
                      0L,            /* cbBuf - 0 to get size needed    */
                      &cReturned,    /* number of queues returned       */
                      &cTotal,       /* total number of queues          */
                      &cbNeeded,     /* number bytes needed to store    */
                                     /* all requested information       */
                      NULL );        /* reserved                        */

   /* If there are no print queues display Message Box */
   if (cTotal == 0L)
   {           /* this system has no printers defined */
      bOK = WinLoadString( pmp->hab,
                           (HMODULE)NULLHANDLE,
                           ERRMSG_NO_QUEUES_DEFINED,
                           LEN_WORKSTRING,
                           szWork );
      pmassert( pmp->hab, bOK );

      WinMessageBox( HWND_DESKTOP,
                     pmp->hwndFrame,
                     szWork,
                     pmp->pszTitle,
                     (USHORT)0,
                     MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);

      /* Grey Print menu item */
      WinEnableMenuItem( pmp->hwndMenubar, IDM_PRINT, FALSE );

      /*
       * There are no printers now. A previously defined printer may have
       * have been deleted, so reset all the fields in the global data
       */
      pmp->hdcPrinter = (HDC)NULL;
      pmp->hpsPrinter = (HPS)NULL;
      pmp->hdcPrinterInfo = (HDC)NULL;
      pmp->achQueueName[0] = '\0';
      pmp->achQueueDesc[0] = '\0';
      pmp->pDriverData = (PDRIVDATA)NULL;

      /*
       * Return TRUE if a printer was previously defined and then reset
       * this field so that some default forms can be returned for
       * the Page Setup dialog
       */
      fResult = pmp->hpsPrinterInfo;
      pmp->hpsPrinterInfo = (HPS)NULL;
      return fResult;
   }

   /*
    * allocate memory to store the enumerated queue information
    */
   pQueueInfo = malloc( cbNeeded ) ;
    pmassert( pmp->hab, pQueueInfo );

    /* Call again to get print queue data. */
   ul = SplEnumQueue( NULL,             /* pszComputerName - local machine */
                      3,                /* information level 3 requested   */
                      pQueueInfo,       /* pBuf - gets enumerated queues   */
                      cbNeeded,         /* cbBuf - size of pBuf            */
                      &cReturned,       /* number of queues returned       */
                      &cTotal,          /* total number of queues          */
                      &cbNeeded,        /* number bytes needed to store    */
                                        /*   all requested information     */
                      NULL );           /* reserved                        */

    pmassert( pmp->hab, ul == 0 );

    /*
     * Validate that the user-preferred queue exists and if not then
     * set preferred queue to none. User's preference of queues is
     * named by the variable pmp->achQueueName.
     */

    if( *pmp->achQueueName != 0 )
    {
      i = FindQueue( pmp->achQueueName, pQueueInfo, cReturned );
      if (i == -1)
      {
        *pmp->achQueueName = 0;
      }
    }

    /*
     * Validate that the user-preferred queue has been initialized OK.
     * If not, initialize it now.
     */

    if ( *pmp->achQueueName == 0 )
    {
       /*
        * There is no preferred queue name because there was none
        * in the INI file (perhaps this is the first time the
        * program has run on this system?) or the profile reading
        * failed somehow. Give the user the application default
        * queue.
        */

       /* check OS/2 version number */
       ul = DosQuerySysInfo( QSV_VERSION_MAJOR, QSV_VERSION_MINOR,
                        aulVersion, 8 );
       pmassert( pmp->hab, ul == 0 );

       if (aulVersion[0] == 20 && aulVersion[1] == 0 )
       {
         ul = PrfQueryProfileString( HINI_PROFILE,
                                     "PM_SPOOLER",
                                     "QUEUE",
                                     NULL,
                                     szWork,
                                     LEN_WORKSTRING );

         if ( ul )
         {
            /* truncate queuename at terminating semicolon */
            pch = strchr( szWork, ';' );
            pmassert( pmp->hab, pch );
            *pch = 0;
            strcpy( pmp->achQueueName, szWork );
         }
       }
       else
       {
         for (i = 0L; i < cTotal; ++i)
         {
            if ( pQueueInfo[i].fsType & PRQ3_TYPE_APPDEFAULT)
            {
               strcpy( pmp->achQueueName, pQueueInfo[i].pszName );
               break;
            }
         }
       }

       /*
        * Still no application default queue? odd. Give him the first queue
        * from the SplEnumQueue calls
        */
       if ( *pmp->achQueueName == 0 )
       {
          pmassert( pmp->hab, NULL == "Error?? No application default queue" );
          strcpy( pmp->achQueueName, pQueueInfo->pszName );
       }
    }
    pmassert( pmp->hab, 0 < strlen( pmp->achQueueName ));

    /* find queue and queue settings including the driver data info */
    i = FindQueue( pmp->achQueueName, pQueueInfo, cReturned );
    pmassert( pmp->hab, i != -1 );
    pqi = &pQueueInfo[i];

    /*
     * Store current driver data info into SplEnumQueue info
     * so that when user wants to change a job property
     * but not necessarily a printer, his previous settings
     * will be shown in the print properties dialog. Otherwise use
     * the defaults from the SplEnumQueue info.
     *
     * Note if the device name, driver data length or version does not match,
     * the driver may not be able to understand or correctly use the
     * driver data info
     */
    if (pmp->cbDriverDataLen)
    {
      if ( ! strcmp( pqi->pDriverData->szDeviceName,
                     pmp->pDriverData->szDeviceName ) )
      {
        if ( (pqi->pDriverData->cb == pmp->cbDriverDataLen ) &&
             (pqi->pDriverData->lVersion == pmp->pDriverData->lVersion ) )
        {
          memcpy( pqi->pDriverData, pmp->pDriverData, pmp->cbDriverDataLen );
        }
        else
        {
          bOK = WinLoadString( pmp->hab,
                               (HMODULE)NULLHANDLE,
                               ERRMSG_NEW_DRIVER_VERSION,
                               LEN_WORKSTRING,
                               szWork );
          pmassert( pmp->hab, bOK );

          WinMessageBox( HWND_DESKTOP,
                         pmp->hwndFrame,
                         szWork,
                         pmp->pszTitle,
                         (USHORT)0,
                         MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);
        }
      }
      else
      {
        bOK = WinLoadString( pmp->hab,
                             (HMODULE)NULLHANDLE,
                             ERRMSG_DEVICE_CHANGED,
                             LEN_WORKSTRING,
                             szWork );
        pmassert( pmp->hab, bOK );

        WinMessageBox( HWND_DESKTOP,
                       pmp->hwndFrame,
                       szWork,
                       pmp->pszTitle,
                       (USHORT)0,
                       MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);
      }
    }

    /*
     * Display dialog box to select a printer
     */


    if (fShowDialog)
    {
        /* sort the data returned from SplEnumQueue by queue name  */
        qsort((void *)pQueueInfo,
          (size_t)cReturned,
          (size_t)sizeof(PRQINFO3),
          SortQueues);

        /*
         * Dialog procedure requires the data from SplEnumQueue.
         * Use the pmp to pass this data to the dialog.
         */
        pmp->pQueueInfo  = pQueueInfo;
        pmp->lQueueCount = cReturned;

        /* present the dialog  */
        ul = WinDlgBox( HWND_DESKTOP,
                        pmp->hwndFrame,
                        (PFNWP)QueryPrintDlgProc,
                        (HMODULE)0,
                        IDD_QUERYPRINT,
                        (PVOID)pmp);

        pmassert( pmp->hab, ul != DID_ERROR );

        /* done with these vars for now  */
        pmp->pQueueInfo  = NULL;
        pmp->lQueueCount = 0;


        if( ul == DID_CANCEL )
        {
             /* free queueinfo buffer for SplEnumQueue   */
             free( pQueueInfo );
             /* indicate to caller that no change occurred  */
             return FALSE;
        }
    }




    /*
     * At this point, something changed about the printer destination:
     * form, printer, queue, resolution, something.
     *
     * Prepare a printer info DC and associate a printer info PS with it
     * for the print queue named by pmp->achQueueName.
     */



    /* set pqi to point to the PRQINFO3 structure for pmp->achQueueName  */

    i = FindQueue( pmp->achQueueName, pQueueInfo, cReturned );
    pmassert( pmp->hab, i  != -1 );
    pqi = &pQueueInfo[i];

    /* destroy previous printer driver data */
    if( pmp->pDriverData )
    {
        free( pmp->pDriverData );
        pmp->pDriverData      = NULL;
        pmp->cbDriverDataLen  = 0;
    }

    /* store new driver name in pmp  */
    strcpy( pmp->achDriverName, pqi->pszDriverName );

    /* store new driver data length in pmp  */
    pmp->cbDriverDataLen  = pqi->pDriverData->cb;

    /* store new driver data in pmp  */
    pmp->pDriverData = (PDRIVDATA) malloc( pmp->cbDriverDataLen );
    pmassert( pmp->hab, pmp->pDriverData );
    memcpy( pmp->pDriverData, pqi->pDriverData, pmp->cbDriverDataLen );

    /* store new queue description in pmp  */
    strncpy(pmp->achQueueDesc, pqi->pszComment, QUEUEDESC_LENGTH);
    pmp->achQueueDesc[QUEUEDESC_LENGTH-1] = 0;

    /*
     * reset the printer info device context so that any changes
     * will be found next time
     */
    ResetPrinterInfoDC( pmp );


    /* free the buffer allocated for SplEnumQueue call  */
    free( pQueueInfo );

    /* enable the Print menu item */
    WinEnableMenuItem( pmp->hwndMenubar, IDM_PRINT, TRUE );

    /* return indication that something changed  */
    return TRUE;
}   /*  end of QueryPrintQueue */

/*************************************************************************
 *  Function   :  QueryPrintDlgProc
 *
 *  Description:  This function is the dialog window procedure for printer
 *                selection.
 *                This dialog presents to the user a list of printer
 *                destinations, or queues.  Typically, the user would
 *                hilight a queue, then press the Job Properties... button
 *                to set items like orientation, form, resolution, etc.
 *                The printer driver presents the job properties dialog
 *                when the application calls DevPostDeviceModes.
 *
 *                Today, there is no standard, uniform way to tell whether
 *                the user pressed OK or Cancel at the Job Properties dialog;
 *                therefore, the dialog processing assumes that
 *                job properties were changed no matter if the
 *                user pressed OK or cancel. See the use of the
 *                fVisitedJobProperties Boolean variable.
 *
 *                This function is associated with the dialog box that is
 *                included in the function name of the procedure. It
 *                provides the service routines for the events (messages)
 *                that occur because the end user operates one of the dialog
 *                box's buttons, entry fields, or controls.
 *
 *                The SWITCH statement in the function distributes the dialog
 *                box messages to the respective service routines, which are
 *                set apart by the CASE clauses. Like any other PM window,
 *                the Dialog Window procedures must provide an appropriate
 *                service routine for their end user initiated messages as
 *                well as for the general PM messages (like the WM_CLOSE
 *                message). If a message is sent to this procedure for which
 *                there is no programmed CASE condition (no service routine),
 *                the message is defaulted to the WinDefDlgProc function,
 *                where it is disposed of by PM.
 *
 *  Parameters :  HWND         window handle of printer selection dialog
 *                USHORT       message
 *                MPARAM       message parameter 1
 *                MPARAM       message parameter 2
 *
 *  API's      :  WinSetWindowULong
 *                WinSendDlgItemMsg
 *                WinEnableControl
 *                WinSendDlgItemMsg
 *                WinQueryWindowULong
 *                WinDismissDlg
 *                WinDefDlgProc
 *
 * Return      :  message result
 *
 *************************************************************************/
MRESULT EXPENTRY QueryPrintDlgProc( HWND hwnd,
                                    USHORT msg,
                                    MPARAM mp1,
                                    MPARAM mp2 )
{
   static BOOL         fVisitedJobProperties;
   PMAIN_PARM          pmp;
   INT                 i, index;
   PSZ                 psz;
   HWND                hwndListbox;
   ULONG               ulPanel;

   switch (msg)
   {
   case WM_BUTTON1DBLCLK:
      WinSendMsg( hwnd, WM_COMMAND, (MPARAM)DID_OK, (MPARAM)0L );
      break;


   case WM_INITDLG:
      /* Get pointer to application specific data */
      pmp = (PMAIN_PARM) mp2;
      WinSetWindowULong( hwnd, QWL_USER, (ULONG) mp2 );


      /*
       *  Assume user will not select job properties.
       *  This affects how to dismiss this dialog.
       */
      fVisitedJobProperties = FALSE;

      /* Fill listbox with print objects */

        hwndListbox = WinWindowFromID( hwnd, IDD_QPLISTBOX );
        pmassert( pmp->hab, hwndListbox );

      for (i = 0; i < pmp->lQueueCount; ++i)
      {
       /* use printer comment if possible, else use queue name for display */
         psz = *pmp->pQueueInfo[i].pszComment ?
                pmp->pQueueInfo[i].pszComment :
                pmp->pQueueInfo[i].pszName;
            /* pmwin.h macro for inserting list box items   */
            index = WinInsertLboxItem( hwndListbox, LIT_END, psz );

            /* pre-select this one?  */
            if( 0 == strcmp( pmp->pQueueInfo[i].pszName, pmp->achQueueName ))
            {
               WinSendMsg( hwndListbox, LM_SELECTITEM,
                                            (MPARAM)index, (MPARAM)TRUE );
            }
      }

        /* one must be selected */
        index = WinQueryLboxSelectedItem( hwndListbox );
        pmassert( pmp->hab, index != LIT_NONE );

      return FALSE;

   case WM_HELP:
      /* get pointer to structure from window words of this dialog */
      pmp = (PMAIN_PARM)WinQueryWindowULong( hwnd, QWL_USER );

      if( pmp->hwndHelp )
      {
         ulPanel = PANEL_QUERYPRINT;
         WinSendMsg( pmp->hwndHelp, HM_DISPLAY_HELP,
                   (MPARAM)&ulPanel, (MPARAM)HM_RESOURCEID );
         return (MRESULT)NULL;
      }
      break;


   case WM_COMMAND:
      pmp = (PMAIN_PARM) WinQueryWindowULong( hwnd, QWL_USER );
      switch(SHORT1FROMMP(mp1))
      {
      case IDD_QPJOBPROP:
          /*
           * Flag that we visited the job properties dialog. Since
           * DevPostDeviceModes (OS2_PM_DRV_DEVMODES) option
           * DPDM_POSTJOBPROP has no return code or standard way to
           * indicate user cancel, we have to assume something might
           * have changed.
           */
          fVisitedJobProperties = TRUE;
          hwndListbox = WinWindowFromID( hwnd, IDD_QPLISTBOX );
          pmassert( pmp->hab, hwndListbox );
          index = WinQueryLboxSelectedItem( hwndListbox );
          pmassert( pmp->hab, index != LIT_NONE );
          QueryJobProperties( pmp->hab, &pmp->pQueueInfo[ index ] );
          return 0;

      case DID_OK:
            hwndListbox = WinWindowFromID( hwnd, IDD_QPLISTBOX );
            pmassert( pmp->hab, hwndListbox );
            index = WinQueryLboxSelectedItem( hwndListbox );
            pmassert( pmp->hab, index != LIT_NONE );

         /* modify pmp->achQueueName; it is now the user's preferred queue */
            strcpy( pmp->achQueueName,  pmp->pQueueInfo[ index ].pszName );
            WinDismissDlg( hwnd, DID_OK );
            return 0;


      case DID_CANCEL:
         WinDismissDlg(hwnd, fVisitedJobProperties ? DID_OK : DID_CANCEL );
         return (MRESULT)NULL;
      }
      break;

   }
   return( WinDefDlgProc(hwnd, msg, mp1, mp2) );
} /* End of QueryPrintDlgProc */


/*************************************************************************
 *  Function   :  QueryJobProperties
 *
 *  Description:  Query job properties (print properties) of the printer
 *                driver.
 *
 *  Parameters :  HAB             anchor block handle of the process
 *                PPRQINFO3       pointer to queue information structure
 *
 *  API's      :  DevPostDeviceModes
 *
 *  Return     :  TRUE if ok, otherwise FALSE
 *
 *************************************************************************/
BOOL QueryJobProperties(HAB hab, PPRQINFO3 pQueueInfo)
{
   CHAR        achDriverName[DRIVERNAME_LENGTH];
   CHAR        achDeviceName[DEVICENAME_LENGTH];
   INT         i;
   LONG        l;
   PSZ         pszTemp;

   /*
    * The pszDriverName is of the form DRIVER.DEVICE (e.g.,
    * LASERJET.HP LaserJet IID) so we need to separate it
    * at the dot
    */
   i = strcspn(pQueueInfo->pszDriverName, ".");
   if (i)
   {
      strncpy(achDriverName, pQueueInfo->pszDriverName, i);
      achDriverName[i] = '\0';
      strcpy(achDeviceName, &pQueueInfo->pszDriverName[i + 1]);
   }
   else
   {
      strcpy(achDriverName, pQueueInfo->pszDriverName);
      *achDeviceName = '\0';
   }

   /*
    * There may be more than one printer assigned to this print queue
    * We will use the first in the comma separated list.  We would
    * need an expanded dialog for the user to be more specific.
    */
   pszTemp = strchr(pQueueInfo->pszPrinters, ',');
   if ( pszTemp )
   {
      /* Strip off comma and trailing printer names */
      *pszTemp = '\0' ;
   }

   /*
    * Post the job properties dialog for the printer to allow the
    * user to modify the options
    */
   l = DevPostDeviceModes( hab,
                           pQueueInfo->pDriverData,
                           achDriverName,
                           achDeviceName,
                           pQueueInfo->pszPrinters,
                           DPDM_POSTJOBPROP );

   return (l == DEV_OK);
}  /* End of QueryJobProperties */

/*************************************************************************
 *
 *  Name:         ProcessUserPrint
 *
 *  Description:  This routine handles the user print (WM_USER_PRINT)
 *                message. We're running thread 2 here because it can be
 *                a long operation. The device context and presentation
 *                space are created here, then one of three functions
 *                is called to print depending on whether we have a
 *                bitmap, metafile or text document.
 *
 *  Parameters :  HWND           window handle
 *                PMAIN_PARM     global application data
 *
 *  API's      :  DevOpenDC
 *                GpiCreatePS
 *                DevEscape
 *                GpiAssociate
 *                GpiDestroyPS
 *                DevCloseDC
 *
 *  Return     :  VOID
 *
 *
 *************************************************************************/

VOID ProcessUserPrint( HWND hwnd, PMAIN_PARM pmp)
{
   DEVOPENSTRUC    dos;
   VOID            (*pfnPrintDocument)(HWND, PMAIN_PARM);
   CHAR            achDriverName[DRIVERNAME_LENGTH];
   CHAR            achQueueProcParams[8];
   CHAR            achSpoolerParams[40];
   SIZEL           sizel;
   PSZ             psz;

   /* Build the device context data for DevOpenDC */
   memset((PVOID)&dos, 0, sizeof(dos));

   strcpy(achDriverName, pmp->achDriverName);
   achDriverName[strcspn(achDriverName, ".")] = '\0';
   sprintf(achQueueProcParams, "COP=%d", pmp->usCopies);
   sprintf(achSpoolerParams, "FORM=%s", pmp->form.achFormName);

   dos.pszLogAddress = pmp->achQueueName;
   dos.pszDriverName = (PSZ)achDriverName;
   dos.pdriv = pmp->pDriverData;
   dos.pszDataType = (PSZ)"PM_Q_STD";
   dos.pszComment = pmp->pszTitle;
   dos.pszQueueProcParams = (PSZ)achQueueProcParams;
   dos.pszSpoolerParams = (PSZ)achSpoolerParams;

   /*
    * Switch on whether we are writing text, a bitmap
    * or a metafile into the PS, and process
    */
   switch (pmp->ulMode)
   {
   case MODE_TEXT:
        /* Seek to beginning of user-specified file */
      fseek(pmp->f, 0, SEEK_SET);
      pfnPrintDocument = PrinterPaginate;
      break;

   case MODE_BITMAP:
      pfnPrintDocument = PrinterBitblt;
      break;

   case MODE_METAFILE:
      pfnPrintDocument = PrinterPlayMetaFile;
      break;

   default:
      pmassert(pmp->hab, FALSE);
      return;
   }

   /* Create DC for the printer */
   pmp->hdcPrinter = DevOpenDC( pmp->hab,
                                OD_QUEUED,
                                "*",
                                9L,
                                (PVOID)&dos,
                                (HDC)NULLHANDLE );

   /* Create PS for the printer */
   pmp->hpsPrinter = GPI_ERROR;
   if (pmp->hdcPrinter != DEV_ERROR)
   {
      /*
       * Set page size to 0,0. A page size should not be defined for printing
       * because when the job is printed, the output will be scaled to fit
       * the page size specified on GpiCreatePS
       */
      sizel.cx = 0;
      sizel.cy = 0;
      pmp->hpsPrinter = GpiCreatePS( pmp->hab,
                                     pmp->hdcPrinter,
                                     &sizel,
                                     PU_TWIPS | GPIA_ASSOC );
      pmassert( pmp->hab, pmp->hpsPrinter != GPI_ERROR );

      if (pmp->hpsPrinter != GPI_ERROR)
      {
         /* parse full-qualified filename to just get filename and extension */
         psz = strrchr( pmp->szFilename, '\\' );
         if (psz && *psz)
         {
             ++psz;
         }
         else
         {
             psz = pmp->szFilename;
         }

         /* Issue STARTDOC to begin printing */
         DevEscape( pmp->hdcPrinter,
                    DEVESC_STARTDOC,
                    (LONG)strlen(psz),
                    (PBYTE)psz,
                    (PLONG)NULL, (PBYTE)NULL );

         /* Print the document */
         (*pfnPrintDocument)(hwnd, pmp);

         /*
          * Issue ABORTDOC if user cancelled print job, or ENDDOC
          * to for normal job termination.
          */
         DevEscape( pmp->hdcPrinter,
                    pmp->fCancel ? DEVESC_ABORTDOC : DEVESC_ENDDOC,
                    0L, (PBYTE)NULL, (PLONG)NULL, (PBYTE)NULL );

         /* Release and destroy the PS */
         GpiAssociate( pmp->hpsPrinter, (HDC)NULLHANDLE );
         GpiDestroyPS( pmp->hpsPrinter );
      }
   }

   /* Clean up the DC */
   if (pmp->hdcPrinter != DEV_ERROR)
   {
      DevCloseDC(pmp->hdcPrinter);
      pmp->hdcPrinter = DEV_ERROR;
   }

   return;
}

/*************************************************************************
 *
 *  Name       :  PrinterBitblt
 *
 *  Description:  Load a bitmap into a memory DC using the WM_USER_LOAD_BITMAP
 *                processing. Calculate what size to print the bitmap in order
 *                to preserve the bitmap aspect ratio and still have it fit on
 *                the page. Then bit blit into the printer PS, allowing the
 *                device presentation driver to do the stretch blit.
 *
 *  Parameters :  HWND           window handle
 *                PMAIN_PARM     global application data
 *
 *  API's      :  WinSendMsg
 *                GpiQueryPS
 *                GpiConvert
 *                GpiBitblt
 *                GpiSetBitmap
 *                GpiDeleteBitmap
 *                GpiAssociate
 *                GpiDestroyPS
 *                DevCloseDC
 *
 *  Return     :  VOID
 *
 *************************************************************************/

VOID PrinterBitblt(HWND hwnd, PMAIN_PARM pmp)
{

    /* Clear our variables to ensure USER_LOAD_BITMAP starts fresh */
    pmp->hdcMem4Printer = (HDC)NULLHANDLE;
    pmp->hpsMem4Printer = (HPS)NULLHANDLE;
    pmp->hbm4Printer = (HBITMAP)NULLHANDLE;

    /* update print confirmation dialog */
    WinSendMsg( pmp->hwndPrinting, WM_USER_NEW_PAGE, MPFROMLONG(1L), NULL);

    /* Send message to create a printer PS with our bitmap set into it */
    WinSendMsg(hwnd, WM_USER_LOAD_BITMAP, (MPARAM)hwnd,
               (MPARAM)FLAGS_PRINTER);

    /* draw bitmap into presentation space */
    PaintBitmap( pmp, FLAGS_PRINTER );

    /* Clean up bitmap, memory PS and memory DC */
    GpiDeleteBitmap( pmp->hbm4Printer );
    GpiAssociate( pmp->hpsMem4Printer, (HDC)NULLHANDLE );
    GpiDestroyPS( pmp->hpsMem4Printer );
    DevCloseDC( pmp->hdcMem4Printer );

    return;
}  /* End of PrinterBitblt */

/*************************************************************************
 *
 *  Name:      :  PrinterPaginate
 *
 *  Description:  This function uses WM_USER_PAGINATE processing to
 *                print text to printer There is a check of the fCancel
 *                Boolean in the main parameters after paginating each
 *                page. If this Boolean becomes true, then printing the
 *                text is aborted.
 *
 *  API's      :  GpiCreateLogFont
 *                GpiSetCharSet
 *                WinSendMsg
 *                DevEscape
 *                GpiSetCharSet
 *                GpiDeleteSetId
 *
 *  Return     :  VOID
 *
 *************************************************************************/
VOID PrinterPaginate(HWND hwnd, PMAIN_PARM pmp)
{
   ULONG   ul;
   BOOL    bOK;
   ULONG   cPage = 0;
   ULONG   cPagesPrinted = 0;
   CHAR    szWork[LEN_WORKSTRING];

   /*
    * Create a logical font for the printer and set it
    * into the PS
    */
   ul = (ULONG)GpiCreateLogFont( pmp->hpsPrinter,
                                 (PSTR8)NULL,
                                 1L,
                                 (PFATTRS)&pmp->fontdlg.fAttrs );
   pmassert(pmp->hab, ul != GPI_ERROR);

   bOK = GpiSetCharSet( pmp->hpsPrinter, 1L );
   pmassert( pmp->hab, bOK );

   if (!pmp->fPrintAllPages && pmp->usFirstPage != 1)
   {
      WinSendMsg( pmp->hwndPrinting, WM_USER_SKIP_PAGE,
                  MPFROMLONG(pmp->usFirstPage), NULL);
   }

   do
   {
      cPage++;
      /*
       * Print to info PS until correct page is reached
       */
      if (!pmp->fPrintAllPages && cPage < pmp->usFirstPage)
      {
         ul = (ULONG)WinSendMsg( hwnd, WM_USER_PAGINATE, (MPARAM)hwnd,
                                 (MPARAM)FLAGS_INFO );
      }
      else
      {
         WinSendMsg( pmp->hwndPrinting, WM_USER_NEW_PAGE,
                     MPFROMLONG(cPage), NULL);
         ul = (ULONG)WinSendMsg( hwnd, WM_USER_PAGINATE, (MPARAM)hwnd,
                                 (MPARAM)FLAGS_PRINTER );
         cPagesPrinted++;
      }

      /* break out of loop if no more pages to print */
      if (!pmp->fPrintAllPages && cPage + 1 > pmp->usLastPage)
      {
         break;
      }

      if ( ul == PAGINATE_NOT_EOF )
      {
         DevEscape( pmp->hdcPrinter,
                    DEVESC_NEWFRAME,
                    0L,
                    (PBYTE)NULL,
                    (PLONG)NULL,
                    (PBYTE)NULL );
      }
      else
      {
           /* End of file; Break out of this loop and return to do enddoc */
          break;
      }
    } while ( !pmp->fCancel );

    /* Report error if no pages actually printed */
    if (! cPagesPrinted)
    {
      bOK = WinLoadString( pmp->hab,
                           (HMODULE)NULLHANDLE,
                           ERRMSG_NOTHING_PRINTED,
                           LEN_WORKSTRING,
                           szWork );
      pmassert( pmp->hab, bOK );
      WinMessageBox( HWND_DESKTOP,
                     pmp->hwndFrame,
                     szWork,
                     pmp->pszTitle,
                     (USHORT)0,
                     MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);
    }

    /* Clean up logical font id */
    GpiSetCharSet(pmp->hpsPrinter, 0L);
    GpiDeleteSetId(pmp->hpsPrinter, 1L);

   return;
}  /* End of PrinterPaginate */

/*************************************************************************
 *
 *  Name       :  PrinterPlayMetafile
 *
 *  Description:  Play metafile into printer PS without the reset and
 *                suppress options.
 *
 *  Parameters :  HWND           window handle
 *                PMAIN_PARM     global application data
 *
 *  Return     :  VOID
 *
 *************************************************************************/
VOID PrinterPlayMetaFile(HWND hwnd, PMAIN_PARM pmp)
{
    /* update print confirmation dialog */
    WinSendMsg( pmp->hwndPrinting, WM_USER_NEW_PAGE, MPFROMLONG(1L), NULL);
    /* print metafile */
    PaintMetaFile( pmp, FLAGS_PRINTER );
    return;
}  /* End of PrinterPlayMetafile */

/*************************************************************************
 *
 *   Function   :  FindQueue
 *
 *   Description:  Finds the queue name in the PRQINFO3 structure.
 *
 *   Parameters :  PSZ          pointer to the name of the queue
 *                 PPRQINFO3    pointer to queue selection structure 1
 *                 SHORT        number of queue names to compare
 *
 *   API's      :  [none]
 *
 *************************************************************************/
LONG FindQueue(PSZ pszQueueName, PPRQINFO3 pQueueInfo, LONG lCount)
{
   LONG i;

   for (i = 0L; i < lCount; ++i)
   {
      if ( !strcmp( pszQueueName, pQueueInfo[i].pszName ) )
         return i;
   }

   return -1L;                 /*  if not found  */
}  /* End of FindQueue */

/*************************************************************************
 *  Function   :  SortQueues
 *
 *  Description:  Compare routine for sorting the queue names table.
 *
 *  Parameters :  PPRQINFO3    pointer to queue selection structure 1
 *                PPRQINFO3    pointer to queue selection structure 2
 *
 *  API's      :  [none]
 *
 *  Return     :  result of comparison
 *
 *************************************************************************/
static int _Optlink SortQueues(const void *pQueue1, const void *pQueue2)
{
return(strcmp(((PPRQINFO3)pQueue1)->pszComment,
              ((PPRQINFO3)pQueue2)->pszComment));
} /* end of SortQueues() */


/*************************************************************************
 *  Function   :  ResetPrinterInfoDC
 *
 *  Description:  Close the info DC, PS and re-open another one
 *
 *  Parameters :  PMP          pointer to global data structure
 *
 *  API's      :  [none]
 *
 *  Return     :  [none]
 *
 *************************************************************************/
VOID ResetPrinterInfoDC( PMAIN_PARM pmp )
{
    CHAR            achDriverName[DRIVERNAME_LENGTH];
    SIZEL           sizel;
    DEVOPENSTRUC    dos;

    /* initialize local variables to zeros */
    memset( &dos,   0, sizeof(dos));
    memset( &sizel, 0, sizeof(sizel));


    /* destroy previous printer info PS's and DC's */
    if( pmp->hpsPrinterInfo )
    {
        GpiAssociate(pmp->hpsPrinterInfo, (HDC)NULL);
        GpiDestroyPS(pmp->hpsPrinterInfo);
        pmp->hpsPrinterInfo = (HPS)0;
    }
    if( pmp->hdcPrinterInfo )
    {
        DevCloseDC(pmp->hdcPrinterInfo);
        pmp->hdcPrinterInfo = (HDC)0;
    }

    /* build a devopenstruct for the call to DevOpenDC */
    dos.pszLogAddress = pmp->achQueueName;
    strcpy( achDriverName, pmp->achDriverName );
    achDriverName[strcspn(achDriverName, ".")] = '\0';
    dos.pszDriverName = (PSZ)achDriverName;
    dos.pdriv = pmp->pDriverData;

    /* Create an OD_INFO device context */
    pmp->hdcPrinterInfo = DevOpenDC(
                            pmp->hab,
                            OD_INFO,
                            "*",
                            3L,
                            (PVOID)&dos,
                            (HDC)NULL);
    pmassert(pmp->hab, pmp->hdcPrinterInfo != DEV_ERROR);


    /* create PS for printer info -- not a full-blown PS */
    pmp->hpsPrinterInfo = GpiCreatePS(
                        pmp->hab,
                        pmp->hdcPrinterInfo,
                        (PSIZEL)&sizel,
                        PU_TWIPS | GPIA_ASSOC);
    pmassert(pmp->hab, pmp->hpsPrinterInfo != GPI_ERROR);

} /* end of ResetPrinterInfoDC */

/***************************  End of prtprint.c ****************************/
