/*************************************************************************
*
*  File Name   : STY_EDIT.C
*
*  Description : This module contains the code for the WM_COMMAND
*                messages posted by the standard edit menu.
*
*  Concepts    : Demonstrates the cut, paste, copy, undo, and
*                clear features of an MLE control.
*                BIDI - Added: Bidirectional Language processing sample code.
*
*  API's       : WinSendMsg
*
*  Copyright (C) 1992 IBM Corporation
*
*      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*      sample code created by IBM Corporation. This sample code is not
*      part of any standard or IBM product and is provided to you solely
*      for  the purpose of assisting you in the development of your
*      applications.  The code is provided "AS IS", without
*      warranty of any kind.  IBM shall not be liable for any damages
*      arising out of your use of the sample code, even if they have been
*      advised of the possibility of such damages.                                                    *
*
************************************************************************/

/*  Include files, macros, defined constants, and externs               */

#define INCL_WIN

#include <os2.h>
#include <pmbidi.h>
#include <string.h>
#include "sty_main.h"
#include "sty_xtrn.h"

extern LONG lClrForeground;                         /* color for window text */
extern LONG lClrBackground;                          /* color for background */

CHAR   szPasteText[80];

//CHAR   szCopyVisualText[]    = "ABCDÉÇÅÄ";
//CHAR   szCopyImplicitText[]  = "ABCDÄÅÇÉ";
CHAR   szCopyVisualText[128]  ;
CHAR   szCopyImplicitText[128];


/*********************************************************************
 *  Name: EditCopy
 *
 *  Description : Processes the Edit menu's Copy item.
 *
 *  Concepts : Called whenever Copy from the Edit menu is selected
 *             Sends a MLM_COPY message to the MLE control.
 *
 *  API's : WinSendMsg
 *
 *  Parameters : mp1 - Message parameter
 *             : mp2 - Message parameter
 *
 *  Returns: Void
 *
 ****************************************************************/
VOID EditCopy(MPARAM mp1, MPARAM mp2)
{
   HAB hab;
   PCHAR pchClipText, pText;
   APIRET rc;
   ULONG  BidiAttr;
   BOOL   fSetClipAttr;
   LONG   Offset =0;

      rc = DosAllocSharedMem ( (PPVOID) &pchClipText,
                    NULL,
                    sizeof(szPasteText),
                    (PAG_READ | PAG_WRITE | PAG_COMMIT | OBJ_GIVEABLE) );

      if (rc) {
          DosBeep(100,100);
          return;
          } /* endif */

      hab =  WinQueryAnchorBlock( hwndMain);

//BIDI
//
// Load the national strings from the resource file.
// These will be copied to the clipbaord.
//
      Offset = ARA_OFF;

      WinLoadString( hab, NULLHANDLE, IDS_COPYVISUALTEXT+Offset,
                     127, szCopyVisualText);
      WinLoadString( hab, NULLHANDLE, IDS_COPYIMPLICITTEXT+Offset,
                     127, szCopyImplicitText);

      //
      // Check if we have to SET the Clipboard Bidi Attribute
      // and if so, with which value.
      //
      switch(SHORT1FROMMP(mp1)) {
      case IDM_EDITCOPY_NOCONV:
         BidiAttr     = 0L;
         pText        = szCopyImplicitText;
         fSetClipAttr = TRUE;
         break;

      case IDM_EDITCOPY_CONV_VISUAL:
         BidiAttr     = BDA_TEXTTYPE_VISUAL | BDA_INIT | BDA_LEVEL ;
         pText        = szCopyVisualText;
         fSetClipAttr = TRUE;
         break;

      case IDM_EDITCOPY_CONV_IMPLICIT:
         BidiAttr     = BDA_TEXTTYPE_IMPLICIT | BDA_INIT | BDA_LEVEL;
         pText        = szCopyImplicitText;
         fSetClipAttr = TRUE;
         break;

      case IDM_EDITCOPY_AUTOCONV:
      default:
         pText        = szCopyImplicitText;
         fSetClipAttr = FALSE;
         break;

      }  /* end switch*/

      //
      // Copy the 'relevant' text to the memory that we are
      // going to hand to the clipboard.
      //
      strcpy(pchClipText, pText );

      /* Now, make give this text to the clipboard. */
      hab =  WinQueryAnchorBlock( hwndMain);
      WinOpenClipbrd  ( hab ) ;
      WinEmptyClipbrd ( hab ) ;
      WinSetClipbrdData ( hab, (ULONG) pchClipText, CF_TEXT, CFI_POINTER) ;


      //
      // If conversion is required, mark what kind of conversion
      // (or none) is required.
      //
      if (fSetClipAttr)
          WinSetLangInfo (NULLHANDLE,
                          LI_BD_CLIP_ATTR,
                          BidiAttr,
                          0L,
                          (ULONG)CF_TEXT,
                          0L);


      WinCloseClipbrd ( hab ) ;


}   /*   End of EditCopy()                                              */

/*********************************************************************
 *  Name: EditPaste
 *
 *  Description : Processes the Edit menu's Paste item.
 *
 *  Concepts : Called whenever Paste from the Edit menu is selected
 *             Sends a MLM_PASTE message to the MLE control.
 *
 *  API's : WinSendMsg
 *
 *  Parameters : mp2 - Message parameter
 *
 *  Returns: Void
 *
 ****************************************************************/
VOID EditPaste(MPARAM mp1, MPARAM mp2)
{
   HAB hab;
   PCHAR pchClipText;
   APIRET rc;
   ULONG  BidiAttr;
   BOOL   fSetClipConv;
   PCHAR  pCompareText;
   BOOL   fSuccess;


   szPasteText[0] = '\0'; /* Init */


   //
   // Check if we have to set the 'Clipboard Conversion'
   // BidiAttribute, and if so - to which value.
   //
   switch(SHORT1FROMMP(mp1)) {

      case IDM_EDITPASTE_NOCONV:
         BidiAttr     = 0L;
         fSetClipConv = TRUE;
         pCompareText = szCopyImplicitText;
         break;

      case IDM_EDITPASTE_CONV_VISUAL:
         BidiAttr     = BDA_TEXTTYPE_VISUAL | BDA_INIT| BDA_LEVEL;
         fSetClipConv = TRUE;
         pCompareText = szCopyVisualText;
         break;

      case IDM_EDITPASTE_CONV_IMPLICIT:
         BidiAttr     = BDA_TEXTTYPE_IMPLICIT | BDA_INIT| BDA_LEVEL;
         fSetClipConv = TRUE;
         pCompareText = szCopyImplicitText;
         break;

      case IDM_EDITPASTE_AUTOCONV:
      default:
         BidiAttr     = 0L;
         fSetClipConv = FALSE;
         pCompareText = szCopyImplicitText;

   } /* end switch */


   hab =  WinQueryAnchorBlock( hwndMain);
   WinOpenClipbrd  ( hab ) ;

   //
   // Here we set (if necessary) the clipboard conversion attr
   //
   if (fSetClipConv == TRUE)
       WinSetLangInfo (NULLHANDLE,
                       LI_BD_CLIP_CONV_ATTR,
                       BidiAttr,
                       0L,
                       (ULONG)CF_TEXT,
                       0L);



   pchClipText =  (PCHAR) WinQueryClipbrdData (hab, CF_TEXT ) ;
   if (pchClipText != 0) {

      //
      // Get the clipboard text, and compare it to pCompareText.
      //
      strcpy(szPasteText, pchClipText);


      //if (strcmp( szPasteText, pCompareText) != 0) {
      //  fSuccess = FALSE;
      //} else {
      //      fSuccess = TRUE;  // Good!
      //}
      //
      //ReportTestResult(HWND_DESKTOP, fSuccess);

   }  /* endif */

   WinCloseClipbrd ( hab ) ;

   WinInvalidateRect(hwndMain, NULL, FALSE);

}   /* End of EditPaste()                                               */


