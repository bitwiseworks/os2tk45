/**************************************************************************
 *  File name  :  prtpage.c
 *
 *  Description:  This module contains all the functions needed to display
 *                the page setup dialog
 *
 *                This source file contains the following functions:
 *
 *                MMtoFixedInch( lMM )
 *                PageDlgProc( hwnd, msg, mp1, mp2 )
 *                QueryDlgItemFixed( hwnd, idItem, pfxResult )
 *                SetDlgItemFixed( hwnd, idItem, fxValue )
 *                VerifyFixedLimits( fxValue, fxMin, fxMax )
 *
 *  Concepts   :  Forms, page margins
 *
 *  API's      :  DosBeep
 *                WinInsertLboxItem
 *                WinQueryLboxSelectedItem
 *                WinSendMsg
 *                WinSetWindowULong
 *                WinShowWindow
 *                WinWindowFromID
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
#define INCL_DEVERRORS
#define INCL_GPIBITMAPS
#define INCL_GPICONTROL
#define INCL_GPILCIDS
#define INCL_GPIMETAFILES
#define INCL_GPITRANSFORMS
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_WINDIALOGS
#define INCL_WINERRORS
#define INCL_WINENTRYFIELDS
#define INCL_WINHELP
#define INCL_WINLISTBOXES
#define INCL_WINSTDFILE
#define INCL_WINSTDFONT
#define INCL_WINWINDOWMGR
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

FIXED MMtoFixedInch( LONG lMM );
MRESULT EXPENTRY PageDlgProc(HWND, USHORT, MPARAM, MPARAM);
BOOL QueryDlgItemFixed( HWND hwnd, ULONG idItem, PFIXED pfxResult);
BOOL SetDlgItemFixed( HWND hwnd, ULONG idItem, FIXED fxValue);
FIXED VerifyFixedLimits( FIXED fxValue, FIXED fxMin, FIXED fxMax );


/*************************************************************************
 *  Function   :  PageDlgProc
 *
 *  Description:  This function is the dialog window procedure for page setup
 *
 *************************************************************************/
MRESULT EXPENTRY PageDlgProc( HWND hwnd,
                              USHORT msg,
                              MPARAM mp1,
                              MPARAM mp2 )
{
   static PMAIN_PARM   pmp;
   SHORT               i, index;
   PSZ                 psz;
   HAB                 hab;
   HWND                hwndListbox;
   ULONG               ulPanel;
   FIXED               fxMargin, fxOther;
   BOOL                bOK;
   FIXED               fxWidth, fxHeight, fxLeft, fxRight, fxTop, fxBottom;

   switch (msg)
   {
   case WM_INITDLG:
      /* get application specific data and store in window ULONG */
      pmp = (PMAIN_PARM) mp2;
      WinSetWindowULong( hwnd, QWL_USER, (ULONG) mp2 );
      hab = pmp->hab;

      /*
       * Hide the duplex control.  The reason it is included in the
       * print sample DLG file is for the benefit of programmers who
       * want their dialogs to look exactly like those presented in
       * the Programming Guide for printing (Chapter 18).
       */
      WinShowWindow(WinWindowFromID(hwnd, IDD_DUPLEX), FALSE);

      /* fill listbox with form names */
      hwndListbox = WinWindowFromID( hwnd, IDD_FORM );
      pmassert( hab, hwndListbox );

      for (i = 0; i < pmp->cForms; ++i)
      {
         psz = pmp->hcinfo[i].szFormname;
         index = WinInsertLboxItem( hwndListbox, LIT_END, psz );

         /* preselect this one? */
         if (! strcmp( psz, pmp->form.achFormName ) )
         {
            WinSendMsg( hwndListbox, LM_SELECTITEM,
                                            (MPARAM)index, (MPARAM)TRUE );
         }
      }

      /* one must be selected */
      index = WinQueryLboxSelectedItem( hwndListbox );
      pmassert( hab, index != LIT_NONE );

      /* fill entry fields with user margins */
      bOK = SetDlgItemFixed(hwnd, IDD_MARGIN_LEFT, pmp->form.fxLeftMargin );
      pmassert( hab, bOK );
      bOK = SetDlgItemFixed(hwnd, IDD_MARGIN_RIGHT, pmp->form.fxRightMargin );
      pmassert( hab, bOK );
      bOK = SetDlgItemFixed(hwnd, IDD_MARGIN_TOP, pmp->form.fxTopMargin );
      pmassert( hab, bOK );
      bOK = SetDlgItemFixed(hwnd, IDD_MARGIN_BOTTOM, pmp->form.fxBottomMargin );
      pmassert( hab, bOK );

      return FALSE;

   case WM_HELP:
      /* Get pointer to application specific data */
      pmp = (PMAIN_PARM)WinQueryWindowULong( hwnd, QWL_USER );

      if( pmp->hwndHelp )
      {
         ulPanel = PANEL_QUERYFORM;
         WinSendMsg( pmp->hwndHelp, HM_DISPLAY_HELP,
                     (MPARAM)&ulPanel, (MPARAM)HM_RESOURCEID );
         return (MRESULT)NULL;
      }
      break;


   case WM_CONTROL:
      /* Get pointer to application specific data */
      pmp = (PMAIN_PARM)WinQueryWindowULong( hwnd, QWL_USER );
      hab = pmp->hab;

      /* initialize clip limits in fixed inch units */
      index = pmp->iForm;
      fxWidth = MMtoFixedInch( pmp->hcinfo[index].cx );
      fxHeight = MMtoFixedInch( pmp->hcinfo[index].cy );
      fxLeft = MMtoFixedInch( pmp->hcinfo[index].xLeftClip );
      fxRight = fxWidth - MMtoFixedInch( pmp->hcinfo[index].xRightClip );
      fxTop = fxHeight - MMtoFixedInch( pmp->hcinfo[index].yTopClip );
      fxBottom = MMtoFixedInch( pmp->hcinfo[index].yBottomClip );

      switch(SHORT1FROMMP(mp1))
      {
      case IDD_MARGIN_LEFT:

         switch (SHORT2FROMMP(mp1))
         {
         case EN_CHANGE:
            /*
             *  Verify that a fixed number has been typed in the entry field
             *  If not, beep and set it back to the original value
             */
             if (!QueryDlgItemFixed(hwnd, IDD_MARGIN_LEFT, &fxMargin) )
             {
                DosBeep(600, 5);
                bOK = SetDlgItemFixed(hwnd, IDD_MARGIN_LEFT,
                                      pmp->form.fxLeftMargin );
                pmassert( hab, bOK );
                return 0;
             }
             break;

         case EN_KILLFOCUS:
            /*
             *  Verify that a fixed number has been typed in the entry field
             *  and it is not outside the hardware clip limits of the printer
             *  and fits in the form
             */
             fxOther = fxWidth- fxRight;
             QueryDlgItemFixed( hwnd, IDD_MARGIN_LEFT, &fxMargin );
             fxMargin = VerifyFixedLimits( fxMargin, fxLeft, fxOther );
             bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_LEFT, fxMargin );
             pmassert( hab, bOK );

             /* verify the right margin is still within the form */
             fxOther = fxWidth - fxMargin;
             QueryDlgItemFixed(hwnd, IDD_MARGIN_RIGHT, &fxMargin);
             fxMargin = VerifyFixedLimits( fxMargin, fxRight, fxOther );
             bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_RIGHT, fxMargin );
             pmassert( hab, bOK );
             break;
         }
         return 0;

      case IDD_MARGIN_RIGHT:

         switch (SHORT2FROMMP(mp1))
         {
         case EN_CHANGE:
            /*
             *  Verify that a fixed number has been typed in the entry field
             *  If not, beep and set it back to the original value
             */
             if (!QueryDlgItemFixed(hwnd, IDD_MARGIN_RIGHT, &fxMargin) )
             {
                bOK = SetDlgItemFixed(hwnd, IDD_MARGIN_RIGHT,
                                      pmp->form.fxRightMargin );
                pmassert( hab, bOK );
                DosBeep(600, 5);
                return 0;
             }
             break;

         case EN_KILLFOCUS:
            /*
             *  Verify that a fixed number has been typed in the entry field
             *  and it is not outside the hardware clip limits of the printer
             *  and fits in the form
             */
             QueryDlgItemFixed( hwnd, IDD_MARGIN_LEFT, &fxMargin );
             fxOther = fxWidth - fxMargin;
             QueryDlgItemFixed(hwnd, IDD_MARGIN_RIGHT, &fxMargin);
             fxMargin = VerifyFixedLimits( fxMargin, fxRight, fxOther );
             bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_RIGHT, fxMargin );
             pmassert( hab, bOK );
             break;
         }
         return 0;

      case IDD_MARGIN_TOP:

         switch (SHORT2FROMMP(mp1))
         {
         case EN_CHANGE:
            /*
             *  Verify that a fixed number has been typed in the entry field
             *  If not, beep and set it back to the original value
             */
             if (!QueryDlgItemFixed(hwnd, IDD_MARGIN_TOP, &fxMargin) )
             {
                bOK = SetDlgItemFixed(hwnd, IDD_MARGIN_TOP,
                                      pmp->form.fxTopMargin );
                pmassert( hab, bOK );
                DosBeep(600, 5);
                return 0;
             }
             break;

         case EN_KILLFOCUS:
            /*
             *  Verify that a fixed number has been typed in the entry field
             *  and it is not outside the hardware clip limits of the printer
             */
             QueryDlgItemFixed(hwnd, IDD_MARGIN_BOTTOM, &fxMargin);
             fxOther = fxHeight - fxMargin;
             QueryDlgItemFixed( hwnd, IDD_MARGIN_TOP, &fxMargin );
             fxMargin = VerifyFixedLimits( fxMargin, fxTop, fxOther );
             bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_TOP, fxMargin );
             pmassert( hab, bOK );
             break;
         }
         return 0;

      case IDD_MARGIN_BOTTOM:

         switch (SHORT2FROMMP(mp1))
         {
         case EN_CHANGE:
            /*
             *  Verify that a fixed number has been typed in the entry field
             *  If not, beep and set it back to the original value
             */
             if (!QueryDlgItemFixed(hwnd, IDD_MARGIN_BOTTOM, &fxMargin) )
             {
                bOK = SetDlgItemFixed(hwnd, IDD_MARGIN_BOTTOM,
                                      pmp->form.fxBottomMargin );
                pmassert( hab, bOK );
                DosBeep(600, 5);
                return 0;
             }
             break;

         case EN_KILLFOCUS:
            /*
             *  Verify that a fixed number has been typed in the entry field
             *  and it is not outside the hardware clip limits of the printer
             */
             fxOther = fxHeight - fxBottom;
             QueryDlgItemFixed( hwnd, IDD_MARGIN_BOTTOM, &fxMargin );
             fxMargin = VerifyFixedLimits( fxMargin, fxBottom, fxOther );
             bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_BOTTOM, fxMargin );
             pmassert( hab, bOK );

             /* verify the top margin is still within the form */
             fxOther = fxHeight - fxMargin;
             QueryDlgItemFixed( hwnd, IDD_MARGIN_TOP, &fxMargin );
             fxMargin = VerifyFixedLimits( fxMargin, fxTop, fxOther );
             bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_TOP, fxMargin );
             pmassert( hab, bOK );
             break;
         }
         return 0;

      case IDD_FORM:
         /*
          * Check if a new form has been selected. Choose the maximum of
          * the current margin and the form clip limit and show the new
          * margins. The actual form is not changed until the user presses
          * OK to ensure that CANCEL still works.
          */
         if (SHORT2FROMMP(mp1) == LN_SELECT)
         {
            hwndListbox = WinWindowFromID( hwnd, IDD_FORM );
            pmassert( hab, hwndListbox );
            index = (SHORT) WinSendMsg( hwndListbox, LM_QUERYSELECTION,
                                    (MPARAM)LIT_FIRST, 0 );
            pmassert( hab, index != LIT_NONE );

            /* reset clip limits to new form */
            fxWidth = MMtoFixedInch( pmp->hcinfo[index].cx );
            fxHeight = MMtoFixedInch( pmp->hcinfo[index].cy );
            fxLeft = MMtoFixedInch( pmp->hcinfo[index].xLeftClip );
            fxRight = fxWidth - MMtoFixedInch( pmp->hcinfo[index].xRightClip );
            fxTop = fxHeight - MMtoFixedInch( pmp->hcinfo[index].yTopClip );
            fxBottom = MMtoFixedInch( pmp->hcinfo[index].yBottomClip );

            bOK = QueryDlgItemFixed(hwnd, IDD_MARGIN_LEFT, &fxMargin);
            pmassert( hab, bOK );
            bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_LEFT,
                                   max( fxMargin, fxLeft ) );
            pmassert( hab, bOK );

            bOK = QueryDlgItemFixed(hwnd, IDD_MARGIN_RIGHT, &fxMargin);
            pmassert( hab, bOK );
            bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_RIGHT,
                                   max( fxMargin, fxRight ) );
            pmassert( hab, bOK );

            bOK = QueryDlgItemFixed(hwnd, IDD_MARGIN_TOP, &fxMargin);
            pmassert( hab, bOK );
            bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_TOP,
                                   max( fxMargin, fxTop ) );
            pmassert( hab, bOK );

            bOK = QueryDlgItemFixed(hwnd, IDD_MARGIN_BOTTOM, &fxMargin);
            pmassert( hab, bOK );
            bOK = SetDlgItemFixed( hwnd, IDD_MARGIN_BOTTOM,
                                   max( fxMargin, fxBottom ) );
            pmassert( hab, bOK );
         }
         return 0;

      case IDD_DUPLEX:
         /* duplex control is not used in this sample program */
         return 0;
      }
      break;


   case WM_COMMAND:
      /* Get pointer to application specific data */
      pmp = (PMAIN_PARM)WinQueryWindowULong( hwnd, QWL_USER );
      hab = pmp->hab;


      switch(SHORT1FROMMP(mp1))
      {

      case DID_OK:

         /* retrieve form chosen by user */
         hwndListbox = WinWindowFromID( hwnd, IDD_FORM );
         pmassert( hab, hwndListbox );
         index = WinQueryLboxSelectedItem( hwndListbox );
         pmassert( hab, index != LIT_NONE );
         pmp->iForm = index;
         strcpy( pmp->form.achFormName, pmp->hcinfo[index].szFormname );

         /*
          * Get each margin and check that it is within the clip and
          * form limits. Assign each margin to the form data structure
          *
          * Once the left and top margins are evaluated, the maximum value
          * for the right and bottom margins is defined as whatever is
          * left over in the form
          */

         fxWidth = MMtoFixedInch( pmp->hcinfo[index].cx );
         fxHeight = MMtoFixedInch( pmp->hcinfo[index].cy );
         fxLeft = MMtoFixedInch( pmp->hcinfo[index].xLeftClip );
         fxRight = fxWidth - MMtoFixedInch( pmp->hcinfo[index].xRightClip );
         fxTop = fxHeight - MMtoFixedInch( pmp->hcinfo[index].yTopClip );
         fxBottom = MMtoFixedInch( pmp->hcinfo[index].yBottomClip );

         fxOther = fxWidth - fxRight;
         bOK = QueryDlgItemFixed(hwnd, IDD_MARGIN_LEFT, &fxMargin);
         pmassert( hab, bOK );
         pmp->form.fxLeftMargin = VerifyFixedLimits( fxMargin, fxLeft, fxOther );

         fxOther = fxWidth - pmp->form.fxLeftMargin;
         bOK = QueryDlgItemFixed(hwnd, IDD_MARGIN_RIGHT, &fxMargin);
         pmassert( hab, bOK );
         pmp->form.fxRightMargin = VerifyFixedLimits( fxMargin, fxRight, fxOther );

         fxOther = fxHeight - fxBottom;
         bOK = QueryDlgItemFixed(hwnd, IDD_MARGIN_BOTTOM, &fxMargin);
         pmassert( hab, bOK );
         pmp->form.fxBottomMargin = VerifyFixedLimits( fxMargin, fxBottom, fxOther );

         fxOther = fxHeight - pmp->form.fxBottomMargin;
         bOK = QueryDlgItemFixed(hwnd, IDD_MARGIN_TOP, &fxMargin);
         pmassert( hab, bOK );
         pmp->form.fxTopMargin = VerifyFixedLimits( fxMargin, fxTop, fxOther );

         WinDismissDlg(hwnd, DID_OK);
         return 0;

      case DID_CANCEL:
         WinDismissDlg(hwnd, DID_CANCEL);
         return (MRESULT)NULL;
      }
      break;

   }
   return( WinDefDlgProc(hwnd, msg, mp1, mp2) );
} /* End of QueryPrintDlgProc */


/*************************************************************************
 *  Function   :  QueryPrinterForms
 *
 *  Description:  This function queries the forms that are available in the
 *                printer or returns some default forms.
 *                It uses DevPostDeviceModes API.
 *
 *  API's      :  DevQueryHardcopyCaps
 *
 *************************************************************************/
LONG QueryPrinterForms( PMAIN_PARM pmp, HCINFO hcinfo[], ULONG cMaxForms )
{
   LONG cForms;

   /* if printer is defined, then query forms using DevPostDeviceModes */
   if (pmp->hpsPrinterInfo)
   {
       pmassert( pmp->hab, pmp->hdcPrinterInfo );
       cForms = DevQueryHardcopyCaps( pmp->hdcPrinterInfo, 0L,
                                      cMaxForms, hcinfo );
       pmassert( pmp->hab, pmp->cForms != DQHC_ERROR );
   }
   else
   {
      /* hard-code some default forms */
      cForms = 0;

      strcpy( hcinfo[cForms].szFormname, "Letter" );
      hcinfo[cForms].cx = 216;
      hcinfo[cForms].cy = 279;
      hcinfo[cForms].flAttributes = HCAPS_CURRENT | HCAPS_SELECTABLE;
      hcinfo[cForms].xLeftClip = 10;
      hcinfo[cForms].yBottomClip = 10;
      hcinfo[cForms].xRightClip = 206;
      hcinfo[cForms].yTopClip = 269;
      cForms++;

      strcpy( hcinfo[cForms].szFormname, "A4" );
      hcinfo[cForms].cx = 211;
      hcinfo[cForms].cy = 297;
      hcinfo[cForms].flAttributes = 0;
      hcinfo[cForms].xLeftClip = 6;
      hcinfo[cForms].yBottomClip = 10;
      hcinfo[cForms].xRightClip = 205;
      hcinfo[cForms].yTopClip = 287;
      cForms++;
   }

   return cForms;
} /* End of QueryPrinterForms */


/*************************************************************************
 *  Function   :  MMtoFixedInch
 *
 *  Description:  Convert Millimetres to Inches in FIXED format
 *
 *  Parameters :  LONG       millimetres
 *
 *  API's      :  [none]
 *
 *  Return     :  FIXED      inches
 *
 *************************************************************************/
FIXED MMtoFixedInch( LONG lMM )
{
   USHORT integer;
   USHORT fractional;

   integer = (USHORT)( ((float)(lMM)) / 25.4 );
   fractional = (USHORT)( (((float)(lMM)) / 25.4 - integer) * 0x10000 + 0.5 );
   return MAKEFIXED(integer, fractional);

} /* end of MMtoFixedInch */



/*************************************************************************
 *  Function   :  QueryDlgItemFixed
 *
 *  Description:  Query a dialog item for a fixed value
 *
 *  Parameters :  HWND       dialog window handle
 *                ULONG      dialog item to query
 *                PFIXED     value queried from dialog item
 *
 *  API's      :  WinQueryDlgItemText
 *
 *  Return     :  BOOL (TRUE means a FIXED value was found)
 *
 *************************************************************************/
BOOL QueryDlgItemFixed( HWND hwnd, ULONG idItem, PFIXED pfxResult)
{
   CHAR  sz[20];
   float Result;
   SHORT integer;
   SHORT fractional;

   *pfxResult = MAKEFIXED( 0, 0 );
   WinQueryDlgItemText (hwnd, idItem, 20, sz );
   if (! sz)
      return FALSE;
   Result = atof( sz );
   integer = (SHORT) Result;
   fractional = (SHORT)( (Result - ((float)integer)) * 0x10000 + 0.5 );
   *pfxResult = MAKEFIXED( integer, fractional );
   return TRUE;
} /* end of QueryDlgItemFixed */


/*************************************************************************
 *  Function   :  SetDlgItemFixed
 *
 *  Description:  Set a dialog item to a fixed value
 *
 *  Parameters :  HWND       dialog window handle
 *                ULONG      dialog item to query
 *                FIXED      value to set in dialog item
 *
 *  API's      :  WinSetDlgItemText
 *
 *  Return     :  BOOL (TRUE means function was successful)
 *
 *************************************************************************/
BOOL SetDlgItemFixed( HWND hwnd, ULONG idItem, FIXED fxValue)
{
   CHAR  sz[20];
   BOOL  bResult;

   sprintf(sz, "%-3.2f",
           ((float)FIXEDINT(fxValue) + (float)FIXEDFRAC(fxValue) / 0x10000 ) );
   bResult = WinSetDlgItemText( hwnd, idItem, sz);
   return bResult;
} /* end of SetDlgItemFixed */


/*************************************************************************
 *  Function   :  VerifyFixedLimits
 *
 *  Description:  Verify that a fixed value in within a min and max limit
 *                and return the nearest limit if outside
 *
 *  Parameters :  FIXED      value to verify
 *                FIXED      minimum limit
 *                FIXED      maximum limit
 *
 *  API's      :  [none]
 *
 *  Return     :  FIXED      closest limit or value if within limits
 *
 *************************************************************************/
FIXED VerifyFixedLimits( FIXED fxValue, FIXED fxMin, FIXED fxMax )
{
   if ( fxValue < fxMin )
   {
      return fxMin;
   }
   if ( fxValue > fxMax )
   {
      return fxMax;
   }
   return fxValue;
} /* end of VerifyFixedLimits */



/*************************************************************************
 *  Function   :  FindForm
 *
 *  Description:  Finds a form name in an array of form structures
 *                Only the first word of the form names are compared so that
 *                "Letter" and "Letter (8.5 x 11)" are considered the same
 *                This routine cannot coope with the difference between
 *                "EXEC" and "EXECUTIVE" or "10" and "COM-10"
 *
 *  Parameters :  PSZ        value to form name of find
 *                PHCINFO    array of form data structures
 *                LONG       number of forms for comparison
 *
 *  API's      :  [none]
 *
 *  Return     :  LONG       index of form name found or -1 if not found
 *
 *************************************************************************/
LONG FindForm( PSZ pszFormName, PHCINFO pForms, LONG lCount)
{
   LONG   i;
   CHAR   szFormtoFind[32], szFormtoCompare[32];
   PSZ    pszTmp;

   strcpy( szFormtoFind, pszFormName );
   pszTmp = strchr( szFormtoFind, ' ' );
   if (pszTmp)
   {
      *pszTmp = '\0';
   }
   for (i=0L; i < lCount; ++i)
   {
      strcpy( szFormtoCompare, pForms[i].szFormname );
      pszTmp = strchr( szFormtoCompare, ' ' );
      if (pszTmp)
      {
          *pszTmp = '\0';
      }
      if ( !strcmp( szFormtoFind, szFormtoCompare ) )
          return i;
   }

   return -1L;
} /* end of FindForm */


/*************************************************************************
 *  Function   :  QueryForm
 *
 *  Description:  Query all forms, display them in a list box and do a selection
 *
 *  Parameters :  PMAIN_PARM  pmp has global application data
 *                BOOL        flag, if TRUE the page setup dialog is displayed
 *
 *  API's      :  WinDlgBox
 *                WinLoadString
 *                WinMessageBox
 *                WinPostMsg
 *                WinSendMsg
 *
 *  Return     :  BOOL        TRUE if the form changed, otherwise false
 *
 *************************************************************************/
BOOL QueryForm( PMAIN_PARM pmp, BOOL fNeedDialog )

{
   SHORT               i, index;
   BOOL                bOK;
   ULONG               ulLeftMargin;
   ULONG               ulRightMargin;
   ULONG               ulTopMargin;
   ULONG               ulBottomMargin;
   CHAR                szTmp[ LEN_WORKSTRING ], szWork[ LEN_WORKSTRING ];
   CHAR                szOldForm[32];
   ULONG               ul;

   /* retrieve form names and sizes */
   pmp->cForms = QueryPrinterForms( pmp, pmp->hcinfo, MAX_FORMS );
   pmassert( pmp->hab, pmp->cForms );

   /*
    * Find form name if previously set, a return of -1 means that the form
    * name was not found.
    */
   strcpy( szOldForm, pmp->form.achFormName );
   index = FindForm( szOldForm, pmp->hcinfo, pmp->cForms );

   /*
    * Select form name from pmp structure. Either use the form
    * previously found or if it wasn't found, then pre-select the
    * one with HCAPS_CURRENT set.
    * Fill in all the form sizes and margins before selecting the form.
    */
   for (i = 0; i < pmp->cForms; ++i)
   {
      if ( ((index == -1) && (pmp->hcinfo[i].flAttributes & HCAPS_CURRENT)) ||
           (i == index) )
      {
         pmp->iForm = i;
         strcpy(pmp->form.achFormName, pmp->hcinfo[i].szFormname );

         /* check that the margins are greater than the clip limits */
         pmp->form.fxLeftMargin = max( pmp->form.fxLeftMargin,
                                MMtoFixedInch(pmp->hcinfo[i].xLeftClip) );
         pmp->form.fxRightMargin = max( pmp->form.fxRightMargin,
             MMtoFixedInch(pmp->hcinfo[i].cx - pmp->hcinfo[i].xRightClip) );
         pmp->form.fxTopMargin = max( pmp->form.fxTopMargin,
             MMtoFixedInch(pmp->hcinfo[i].cy - pmp->hcinfo[i].yTopClip) );
         pmp->form.fxBottomMargin = max( pmp->form.fxBottomMargin,
                                MMtoFixedInch(pmp->hcinfo[i].yBottomClip) );
      }
   }

   /*
    * if form is not found and a previous form was not initialized,
    * display a warning message box, that a default form has been chosen
    */
   if ( index == -1L && strcmp( szOldForm, "" ) )
   {
      bOK = WinLoadString( pmp->hab, (HMODULE)0,
                   ERRMSG_FORM_CHANGE, LEN_WORKSTRING, szTmp );
      pmassert( pmp->hab, bOK );
      sprintf( szWork, szTmp, szOldForm, pmp->form.achFormName );
      WinMessageBox( HWND_DESKTOP,
                     pmp->hwndFrame,
                     szWork,
                     pmp->pszTitle,
                     (USHORT)0,
                     MB_OK | MB_MOVEABLE | MB_CUAWARNING | MB_APPLMODAL);

   }

   if ( fNeedDialog )
   {
      /*
       * Query user page setup parameters.
       */
      ul = WinDlgBox( HWND_DESKTOP,
                      pmp->hwndFrame,
                      (PFNWP)PageDlgProc,
                      (HMODULE)0,
                      IDD_PAGESETUP,
                      (PVOID)pmp);

      pmassert( pmp->hab, ul != DID_ERROR );

      if( ul == DID_CANCEL )
      {
           /* indicate to caller that no change occurred  */
           return FALSE;
      }

   }
   /*
    * At this point the form or margins changed so recalculate the
    * form and page size
    */
   i = pmp->iForm;
   pmp->form.fxxWidth = MMtoFixedInch( pmp->hcinfo[i].cx );
   pmp->form.fxyHeight = MMtoFixedInch( pmp->hcinfo[i].cy );
   ulLeftMargin = FixedInchesToTwips(pmp->form.fxLeftMargin);
   ulRightMargin = FixedInchesToTwips(pmp->form.fxRightMargin);
   ulTopMargin = FixedInchesToTwips(pmp->form.fxTopMargin);
   ulBottomMargin = FixedInchesToTwips(pmp->form.fxBottomMargin);
   pmp->sizelPage.cx = FixedInchesToTwips(pmp->form.fxxWidth) -
                       ulLeftMargin - ulRightMargin;
   pmp->sizelPage.cy = FixedInchesToTwips(pmp->form.fxyHeight) -
                       ulTopMargin - ulBottomMargin;

   /* Cause a redraw of the screen to show the new margins */
   WinSendMsg( pmp->hwndClient, WM_SIZE, 0, 0 );
   WinSendMsg( pmp->hwndClient, WM_USER_DISABLE_CLIENT, 0, 0 );


   /* re-paginate text for the screen */
   if( pmp->f )
   {
       /* seek top of file to display document in new font */
       fseek( pmp->f, 0, SEEK_SET );
       WinPostMsg( pmp->hwndObject, WM_USER_PAGINATE,
                   (MPARAM)pmp->hwndClient,
                   (MPARAM)FLAGS_SCREEN );
   }

   /* return indication that something changed  */
   return TRUE;

} /* end of QueryForm */

/***************************  End of prtpage.c *****************************/
