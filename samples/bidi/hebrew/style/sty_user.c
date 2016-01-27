/*************************************************************************
*
*  File Name   : STY_USER.C
*
*  Description : This module contains the code for processing
*                messages sent to the standard window that the
*                standard window does not process.
*                BIDI - Added: Bidirectional Language processing sample code.
*
*  Concepts    : Message processing
*
*  API's       : WinSetWindowPos
*                WinSetFocus
*                WinDefWIndowProc
*                WinDlgBox
*                WinSendMsg
*                WinOpenClipbrd
*                WinQueryClipbrdFmtInfo
*                WinCloseClipbrd
*                WinMessageBox
*                WinLoadString
*                WinSendMsg
*                GpiCreateLogFont
*                GpiSetCharSet
*                GpiQueryFontMetrics
*                GpiDeleteSetId
*                WinGetPS
*                WinReleasePS
*                WinFontDlg
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

#define INCL_WINMENUS
#define INCL_WINWINDOWMGR
#define INCL_WINCLIPBOARD
#define INCL_WINMLE
#define INCL_WINSTDFONT
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_WINSYS
#define INCL_WINSTDDLGS      // For FNTS_NATIONAL_LANGUAGE
#define INCL_WINFRAMEMGR     // For FID_MENU
#define INCL_WINCOUNTRY      // For WinQueryCp
#define INCL_WINHOOKS         // For HMQ_CURRENT



#define INCL_WINDIALOGS

#include <os2.h>
#include "sty_main.h"
#include "sty_xtrn.h"
#include "sty_dlg.h"
#include <string.h>

// BIDI
#include <pmbidi.h>
#include <layout.h>

/*  Global variables */

LONG lClrForeground = CLR_BLACK;                    /* color for window text */
LONG lClrBackground = CLR_WHITE;                     /* color for background */

LONG lClrDefaultForeground = CLR_BLACK;             /* color for window text */
LONG lClrDefaultBackground = CLR_WHITE;              /* color for background */

/*--------------------------------------------------------------*\
 *  Entry point declarations
\*--------------------------------------------------------------*/

VOID SetForegroundColor(SHORT idMenu);
VOID SetBackgroundColor(SHORT idMenu);
VOID SetFont(VOID);
VOID ConvertVectorFontSize(FIXED fxPointSize, PFATTRS pfattrs);
MRESULT EXPENTRY DemoDlgProc(HWND hwnd, USHORT msg,
                                  MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY PresParamDemoDlgProc(HWND hwnd, USHORT msg,
                                  MPARAM mp1, MPARAM mp2);
VOID ShowDemoDlg(SHORT idMenuItem);
VOID ShowDemoMsgBox(SHORT idMenuItem);

//BIDI
VOID    ToggleWndOrient(HWND hwnd);
MRESULT EXPENTRY TextConvertDlgProc(HWND hwnd, USHORT msg,
                                  MPARAM mp1, MPARAM mp2);

/*********************************************************************
 *  Name: UserWndProc
 *
 *  Description : Process any messages sent to hwndMain
 *                that are not processed by the standard
 *                window procedure.
 *
 *  Concepts : Routine is called for each message MainWndProc does
 *             not process.  A switch statement branches control
 *             based upon the message passed.  Any messages not
 *             processed here must be passed onto
 *             WinDefWindowProc()
 *
 *  API's : WinSetWindowPos
 *          WinSetFocus
 *          WinDefWIndowProc
 *
 * Parameters   : hwnd - Window handle to which message is addressed
 *                msg - Message type
 *                mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns: Return value depended upon the message processed
 *
 ****************************************************************/
MRESULT UserWndProc( HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
{
   switch(msg)
   {
      case WM_SIZE:
         /*
          * Re-size the MLE to be the same width and height
          * as the client window
          */
// BIDI
//         WinSetWindowPos(hwndMLE, HWND_TOP, 0, 0, SHORT1FROMMP(mp2),
//                            SHORT2FROMMP(mp2), SWP_SIZE);
           WinSetWindowPos(hwndMLE, HWND_TOP, 0, 0, SHORT1FROMMP(mp2),
                            (SHORT2FROMMP(mp2)/2), SWP_SIZE);
         break;

      case WM_SETFOCUS:
         if(SHORT1FROMMP(mp1))
         {
            WinPostMsg(hwnd, SM_SETFOCUS, NULL, NULL);
         }
         break;

      case SM_SETFOCUS:
         WinSetFocus(HWND_DESKTOP, hwndMLE);
         break;

      default:
         /*
          * Default must call WinDefWindowProc()
          */
         return(WinDefWindowProc(hwnd, msg, mp1, mp2));
         break;
   }

   return (MRESULT)0;

}   /*    End of UserWndProc()                                          */

/*********************************************************************
 *  Name: UserCommand
 *
 *  Description : Process any WM_COMMAND messages send to
 *                hwndMain that are not processed by MainCommand
 *
 *  Concepts : Routine is called for each WM_COMMAND that is not
 *             posted by a standard menu item. A switch statement
 *             branches control based upon the id of the control
 *             which posted the message.
 *
 *  API's : WinDlgBox
 *
 *  Parameters :  mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns: Void
 *
 ****************************************************************/
VOID UserCommand(MPARAM mp1, MPARAM mp2)
{
   switch(SHORT1FROMMP(mp1))
   {
      case IDM_OPTIONSFORECOLORBLACK:
      case IDM_OPTIONSFORECOLORBLUE:
      case IDM_OPTIONSFORECOLORRED:
      case IDM_OPTIONSFORECOLORDEFAULT:
          SetForegroundColor(SHORT1FROMMP(mp1));
          break;

      case IDM_OPTIONSBACKCOLORPINK:
      case IDM_OPTIONSBACKCOLORCYAN:
      case IDM_OPTIONSBACKCOLORYELLOW:
      case IDM_OPTIONSBACKCOLORDEFAULT:
          SetBackgroundColor(SHORT1FROMMP(mp1));
          break;

      case IDM_OPTIONSFONT:
          SetFont();
          break;

      case IDM_DEMODLGBUTTONS:
      case IDM_DEMODLGLISTBOXES:
      case IDM_DEMODLGCOMBOBOXES:
      case IDM_DEMODLG_SL_ENTRYFIELDS:
      case IDM_DEMODLG_ML_ENTRYFIELDS:
      case IDM_DEMODLGSTATIC:
      case IDM_DEMODLGSLIDER:
      case IDM_DEMODLGSPINBUTTON:
      case IDM_DEMODLGVALUESET:
      case IDM_DEMODLGNOTEBOOK:
      case IDM_DEMODLGCONTAINER:
          ShowDemoDlg(SHORT1FROMMP(mp1));
          break;

      case IDM_DEMOMSGBOXOK:
      case IDM_DEMOMSGBOXOKCANCEL:
      case IDM_DEMOMSGBOXYESNO:
      case IDM_DEMOMSGBOXYESNOCANCEL:
      case IDM_DEMOMSGBOXRETRYCANCEL:
      case IDM_DEMOMSGBOXABORT:
      case IDM_DEMOMSGBOXENTER:
      case IDM_DEMOMSGBOXENTERCANCEL:
      case IDM_DEMOMSGBOXQUERY:
      case IDM_DEMOMSGBOXWARNING:
      case IDM_DEMOMSGBOXINFO:
      case IDM_DEMOMSGBOXCRITICAL:
      case IDM_DEMOMSGBOXAPP:
      case IDM_DEMOMSGBOXSYS:
      case IDM_DEMOMSGBOXHELP:
          ShowDemoMsgBox(SHORT1FROMMP(mp1));
          break;

      case IDM_DEMODLGPP:
          WinDlgBox(hwndMain, hwndMain, (PFNWP)PresParamDemoDlgProc,
                    (HMODULE)0, IDD_PPDEMODLG, (PVOID)NULL);
          break;

//BIDI
      case IDM_TEXTCONV:
          WinDlgBox(hwndMain, hwndMain, (PFNWP)TextConvertDlgProc,
                    (HMODULE)0, IDD_TEXTCONV_DLG, (PVOID)NULL);
          break;

      case IDM_MENU_INVERSE:
          ToggleWndOrient(hwndMainFrame);
          break;
//BIDI

      default:
          break;
   }
 /*
  * This routine currently doesn't use the mp2 parameter but
  * it is referenced here to prevent an 'Unreferenced Parameter'
  * warning at compile time.
  */
  mp2;
}   /*    End of UserCommand()                                          */

/*********************************************************************
 *  Name: InitMenu
 *
 *  Description : Processes the WM_INITMENU message for the main
 *                window, disabling any menus that are not active
 *
 *  Concepts : Routine is called each time a menu is selected.  A
 *             switch statement branches control based upon the
 *             id of the menu which is being displayed.
 *
 *  API's : WinSendMsg
 *          WinOpenClipbrd
 *          WinQueryClipbrdFmtInfo
 *          WinCloseClipbrd
 *
 *  Parameters :  mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns: Nome
 *
 ****************************************************************/
VOID InitMenu( MPARAM mp1, MPARAM mp2)
{
// ULONG  ulFmtInfo;
// SHORT sRc1,sRc2;
   BOOL bEnable;

   switch(SHORT1FROMMP(mp1))
   {
      case IDM_HELP:
      /*
       * Enable or disable the Help menu depending upon whether the
       * help manager has been enabled
       */
         EnableMenuItem(HWNDFROMMP(mp2), IDM_HELPUSINGHELP, fHelpEnabled);
         EnableMenuItem(HWNDFROMMP(mp2), IDM_HELPGENERAL, fHelpEnabled);
         EnableMenuItem(HWNDFROMMP(mp2), IDM_HELPKEYS, fHelpEnabled);
         EnableMenuItem(HWNDFROMMP(mp2), IDM_HELPINDEX, fHelpEnabled);
         break;

      case IDM_EDIT:
      /*
       * if text is selected in the MLE, the enable the Cut, Copy,
       * and Clear menus.  Otherwise, do not
       */
// BIDI
//
// Removed this, so that copy is always enabled.
// Since now, 'copy to the clipbaord' copies an internal buffer.
//         sRc1 = (SHORT) WinSendMsg(hwndMLE, MLM_QUERYSEL,
//                           MPFROMSHORT(MLFQS_MINSEL), NULL);
//
//         sRc2 = (SHORT) WinSendMsg(hwndMLE, MLM_QUERYSEL,
//                           MPFROMSHORT(MLFQS_MAXSEL), NULL);
//
//         if (sRc1 != sRc2)
//            bEnable = TRUE;
//         else
//            bEnable = FALSE;

         bEnable = TRUE;

         EnableMenuItem(HWNDFROMMP(mp2), IDM_EDITCOPY_NOCONV, bEnable);
         EnableMenuItem(HWNDFROMMP(mp2), IDM_EDITCOPY_AUTOCONV, bEnable);



// BIDI
// commented.
         /*
          * determine if the clipboard has some text on it.  If it
          * doesn't, then disable the Paste menu
          */
//       if(WinOpenClipbrd(hab))
//       {
//         if (WinQueryClipbrdFmtInfo(hab, CF_TEXT, &ulFmtInfo))
//            bEnable = TRUE;
//         else
//            bEnable = FALSE;
//         WinCloseClipbrd(hab);
//       }
//       else
//          bEnable = TRUE;

         EnableMenuItem(HWNDFROMMP(mp2), IDM_EDITPASTE_NOCONV, bEnable);
         EnableMenuItem(HWNDFROMMP(mp2), IDM_EDITPASTE_AUTOCONV,   bEnable);
         break;

      case IDM_OPTIONSFORECOLOR:
         {
          LONG lColor;
          BOOL bBlack = TRUE;
          BOOL bBlue = TRUE;
          BOOL bRed = TRUE;
          BOOL bDefault = TRUE;

          lColor = (LONG)WinSendMsg(hwndMLE,MLM_QUERYTEXTCOLOR, NULL, NULL);
          switch (lColor)
          {
             case CLR_BLACK:
                bBlack = FALSE;
                break;
             case CLR_BLUE:
                bBlue = FALSE;
                break;
             case CLR_RED:
                bRed = FALSE;
                break;
             default:
                bDefault = FALSE;
                break;
           }

           EnableMenuItem(HWNDFROMMP(mp2), IDM_OPTIONSFORECOLORBLACK,
                           bBlack);
           EnableMenuItem(HWNDFROMMP(mp2), IDM_OPTIONSFORECOLORBLUE, bBlue);
           EnableMenuItem(HWNDFROMMP(mp2), IDM_OPTIONSFORECOLORRED, bRed);
           EnableMenuItem(HWNDFROMMP(mp2), IDM_OPTIONSFORECOLORDEFAULT,
                           bDefault);
         }
         break;
      case IDM_OPTIONSBACKCOLOR:
         {
          LONG lColor;
          BOOL bYellow = TRUE;
          BOOL bPink = TRUE;
          BOOL bCyan = TRUE;
          BOOL bDefault = TRUE;

          lColor = (LONG)WinSendMsg(hwndMLE,MLM_QUERYBACKCOLOR, NULL, NULL);
          switch (lColor)
          {
             case CLR_YELLOW:
                bYellow = FALSE;
                break;
             case CLR_PINK:
                bPink = FALSE;
                break;
             case CLR_CYAN:
                bCyan = FALSE;
                break;
             default:
                bDefault = FALSE;
                break;
           }

           EnableMenuItem(HWNDFROMMP(mp2), IDM_OPTIONSBACKCOLORYELLOW,
                           bYellow);
           EnableMenuItem(HWNDFROMMP(mp2), IDM_OPTIONSBACKCOLORPINK, bPink);
           EnableMenuItem(HWNDFROMMP(mp2), IDM_OPTIONSBACKCOLORCYAN, bCyan);
           EnableMenuItem(HWNDFROMMP(mp2), IDM_OPTIONSBACKCOLORDEFAULT,
                           bDefault);
         }
         break;

      default:
          break;
   }
}   /*       End of InitMenu()                                          */

/*********************************************************************
 *  Name: EnableMenuItem
 *
 *  Description : Enables or disables menu item.
 *
 *  Concepts : Called whenever a menu item is to be enabled or
 *             disabled.  Sends a MM_SETITEMATTR to the menu with
 *             the given item id.  Sets the MIA_DISABLED
 *             attribute flag if the item is to be disabled,
 *             clears the flag if enabling.
 *
 *  API's : WinSendMsg
 *
 *  Parameters   : hwnd - Window handle of the menu
 *                 sIditem  - Id of the menu item.
 *                 bEnable - Enable/Disable flag
 *
 *  Returns: Void
 *
 ****************************************************************/
VOID EnableMenuItem( HWND hwndMenu, SHORT sIditem, BOOL bEnable)
{
  SHORT sFlag;

  if(bEnable)
    sFlag = 0;
  else
    sFlag = MIA_DISABLED;

  WinSendMsg(hwndMenu, MM_SETITEMATTR, MPFROM2SHORT(sIditem, TRUE),
               MPFROM2SHORT(MIA_DISABLED, sFlag));

}   /*         End of EnableMenuItem()                                  */

/*********************************************************************
 *  Name: ShowDemoDlg
 *
 *  Description : Displays the demonstration dialog for the menu
 *                id chosen.
 *
 *  Concepts : Called whenever a menu item from the Dialog Box
 *             menu of the Demo menu is selected.  Determines the
 *             id of the dialog template and creates a dialog of
 *             that template.
 *
 *  API's : WinDlgBox
 *
 *  Parameters :   sIdMenuItem - Id of the menu item.
 *
 *  Returns: Void
 *
 ****************************************************************/
VOID ShowDemoDlg( SHORT sIdMenuItem)
{
  static SHORT sIdDlg;

  switch(sIdMenuItem)
  {
     case IDM_DEMODLGBUTTONS:
         sIdDlg = IDD_BUTTONSDLG;
         break;

     case IDM_DEMODLGLISTBOXES:
         sIdDlg = IDD_LISTBOXDLG;
         break;

     case IDM_DEMODLGCOMBOBOXES:
         sIdDlg = IDD_COMBOBOXDLG;
         break;

     case IDM_DEMODLG_SL_ENTRYFIELDS:
         sIdDlg = IDD_SL_ENTRYFIELDDLG;
         break;

     case IDM_DEMODLG_ML_ENTRYFIELDS:
         sIdDlg = IDD_ML_ENTRYFIELDDLG;
         break;

     case IDM_DEMODLGSTATIC:
         sIdDlg = IDD_STATICDLG;
         break;

     case IDM_DEMODLGSLIDER:
         sIdDlg = IDD_SLIDERDLG;
         break;

     case IDM_DEMODLGSPINBUTTON:
         sIdDlg = IDD_SPINBUTTONDLG;
         break;

     case IDM_DEMODLGVALUESET:
         sIdDlg = IDD_VALUESETDLG;
         break;

     case IDM_DEMODLGNOTEBOOK:
         sIdDlg = IDD_NOTEBOOKDLG;
         break;

     case IDM_DEMODLGCONTAINER:
         sIdDlg = IDD_CONTAINERDLG;
         break;

     default:    /* unknown menu id */
         return;
         break;
  }

  WinDlgBox(HWND_DESKTOP, hwndMain,
           (PFNWP)DemoDlgProc,          /* all demos use DemoDlgProc    */
           (HMODULE)0,
           sIdDlg,                      /* id of template               */
           (PVOID)&sIdDlg);             /* pass id as mp2 of WM_INITDLG */

}   /*      End of ShowDemoDlg()                                        */


/*********************************************************************
 *  Name: ShowDemoMsgBox
 *
 *  Description : Displays the demonstration message box for the menu
 *                id chosen.
 *
 *  Concepts :  Called whenever a menu item from the Message Box menu
 *              of the Demo menu is selected.  Determines the options
 *              for the message box and then creates the box.
 *
 *  API's : WinLoadString
 *          WinMessageBox
 *
 *  Parameters : sIdMenuItem - Id of the menu item.
 *
 *  Returns: VOID
 *
 ****************************************************************/
VOID ShowDemoMsgBox( SHORT sIdMenuItem)
{
  SHORT sOptions, sIdText;
  CHAR szText[MESSAGELEN];

  switch(sIdMenuItem)
  {
     case IDM_DEMOMSGBOXOK:
        sOptions = MB_OK | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXOK;
        break;

     case IDM_DEMOMSGBOXOKCANCEL:
        sOptions = MB_OKCANCEL | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXOKCANCEL;
        break;

     case IDM_DEMOMSGBOXYESNO:
        sOptions = MB_YESNO | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXYESNO;
        break;

     case IDM_DEMOMSGBOXYESNOCANCEL:
        sOptions = MB_YESNOCANCEL | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXYESNOCANCEL;
        break;

     case IDM_DEMOMSGBOXRETRYCANCEL:
        sOptions = MB_RETRYCANCEL | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXRETRYCANCEL;
        break;

     case IDM_DEMOMSGBOXABORT:
        sOptions = MB_ABORTRETRYIGNORE | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXABORT;
        break;

     case IDM_DEMOMSGBOXENTER:
        sOptions = MB_ENTER | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXENTER;
        break;

     case IDM_DEMOMSGBOXENTERCANCEL:
        sOptions = MB_ENTERCANCEL | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXENTERCANCEL;
        break;

     case IDM_DEMOMSGBOXQUERY:
        sOptions = MB_OK | MB_QUERY | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXQUERY;
        break;

     case IDM_DEMOMSGBOXWARNING:
        sOptions = MB_OK | MB_WARNING | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXWARNING;
        break;

     case IDM_DEMOMSGBOXINFO:
        sOptions = MB_OK | MB_INFORMATION | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXINFO;
        break;

     case IDM_DEMOMSGBOXCRITICAL:
        sOptions = MB_OK | MB_CRITICAL | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXCRITICAL;
        break;

     case IDM_DEMOMSGBOXAPP:
        sOptions = MB_OK | MB_APPLMODAL | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXAPP;
        break;

     case IDM_DEMOMSGBOXSYS:
        sOptions = MB_OK | MB_SYSTEMMODAL | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXSYS;
        break;

     case IDM_DEMOMSGBOXHELP:
        sOptions = MB_OK | MB_HELP | MB_MOVEABLE;
        sIdText = IDS_DEMOMSGBOXHELP;
        break;

     default:
        /*
        * Unknown menu id
        */
        return;
        break;
  }

  /*
  * Get the text for the message box
  */
  if(!WinLoadString(hab, (HMODULE)0, sIdText, MESSAGELEN, (PSZ)szText))
  {
     MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
        return;
  }

  /* bring up the message box                                           */
  WinMessageBox(HWND_DESKTOP, hwndMain, szText, szAppName, IDD_DEMOMSGBOX,
                sOptions);

}   /*   End of ShowDemoMsgBox()                                        */

/*********************************************************************
 *  Name: SetForegroundColor
 *
 *  Description : Allows the user to select a color for the
 *                window foreground
 *
 *  Concepts : Routine is called each time the user selects one
 *             of the Foreground colors listed in the Foreground
 *             Color submenu of the Options menu.  A switch
 *             statement determines which menu item was chosen
 *             and then the appropriate color is placed into
 *             lClrForeground.
 *
 *  API's : WinSendMsg
 *
 *  Parameters : sIdItem - Id of the menu item.
 *
 *  Returns: Void
 *
 ****************************************************************/
VOID SetForegroundColor(SHORT sIdMenu)
{
   switch(sIdMenu)
   {
      case IDM_OPTIONSFORECOLORBLACK:
         lClrForeground = CLR_BLACK;
         break;

      case IDM_OPTIONSFORECOLORBLUE:
         lClrForeground = CLR_BLUE;
         break;

      case IDM_OPTIONSFORECOLORRED:
         lClrForeground = CLR_RED;
         break;

      /*
       *  For any others, including IDM_OPTIONSBACKCOLORDEFAULT, set
       *  the background color to the default back color
       */
      default:
         lClrForeground = lClrDefaultForeground;
         break;
   }
   WinSendMsg(hwndMLE, MLM_SETTEXTCOLOR, MPFROMLONG(lClrForeground), NULL);
}   /*   End of SetForegroundColor()                                    */

/*********************************************************************
 *  Name: SetBackgroundColor
 *
 *  Description : Allows the user to select a color for the
 *                window Background
 *
 *  Concepts : Routine is called each time the user selects one
 *             of the Background colors listed in the Background
 *             Color submenu of the Options menu.  A switch
 *             statement determines which menu item was chosen
 *             and then the appropriate color is placed into
 *             lClrBackground.
 *
 *  API's : WinSendMsg
 *
 *  Parameters : sIdItem - Id of the menu item.
 *
 *  Returns: Void
 *
 ****************************************************************/
VOID SetBackgroundColor( SHORT idMenu)
{
   switch(idMenu)
   {
      case IDM_OPTIONSBACKCOLORPINK:
         lClrBackground = CLR_PINK;
         break;

      case IDM_OPTIONSBACKCOLORCYAN:
         lClrBackground = CLR_CYAN;
         break;

      case IDM_OPTIONSBACKCOLORYELLOW:
         lClrBackground = CLR_YELLOW;
         break;

      /*
       *  For any others, including IDM_OPTIONSBACKCOLORDEFAULT, set
       *  the background color to the default back color
       */
      default:
         lClrBackground = lClrDefaultBackground;
         break;
   }
   WinSendMsg(hwndMLE, MLM_SETBACKCOLOR, MPFROMLONG(lClrBackground), NULL);
}   /*  End of SetBackgroundColor()                                     */


/*********************************************************************
 *  Name: SetFont
 *
 *  Description : Allows the user to select a font for the text
 *                displayed in the MLE.
 *
 *  Concepts : Routine is called each time the user selects the
 *             Font menu item from the Options menu.  The
 *             standard font dialog is called with the current
 *             available fonts.  If the user selects one, then
 *             the MLM_SETFONT message is sent to the MLE to
 *             display its text to the font chosen.
 *
 *  API's : WinSendMsg
 *          GpiCreateLogFont
 *          GpiSetCharSet
 *          GpiQueryFontMetrics
 *          GpiDeleteSetId
 *          WinGetPS
 *          WinReleasePS
 *          WinLoadString
 *          WinFontDlg
 *
 *  Parameters :  None
 *
 *  Returns: None
 *
 ****************************************************************/
VOID SetFont(void)
{
   FONTDLG fontDlg;
   HPS hps;
   FONTMETRICS fontMetrics;
   CHAR szTitle[MESSAGELEN];
   CHAR szFamily[CCHMAXPATH];
   static fxPointSize = 0;            /* keep track of this for vector fonts */

   memset(&fontDlg, 0, sizeof(fontDlg));            /* initialize all fields */
   /*
    * Get the current font attributes
    */
   hps = WinGetPS(hwndMLE);
   WinSendMsg(hwndMLE, MLM_QUERYFONT,
      MPFROMP((PFATTRS)&(fontDlg.fAttrs)), NULL);

   /* create system default font */

   GpiCreateLogFont(
      hps,
      (PSTR8)fontDlg.fAttrs.szFacename,
      1,
      &(fontDlg.fAttrs));

   GpiSetCharSet(hps, 1);

   GpiQueryFontMetrics(hps, sizeof(FONTMETRICS), &fontMetrics);

   GpiSetCharSet(hps, LCID_DEFAULT);
   GpiDeleteSetId(hps, 1);
   WinReleasePS(hps);

   /*
    * Initialize the FONTDLG structure with the current font
    */
   fontDlg.cbSize     = sizeof(FONTDLG);                  /* sizeof(FONTDLG) */
   fontDlg.hpsScreen  = WinGetScreenPS(HWND_DESKTOP);  /* Screen presentation space */
   fontDlg.hpsPrinter = NULLHANDLE;            /* Printer presentation space */

   if(!WinLoadString(hab, (HMODULE)0, IDS_FONTDLGTITLE, MESSAGELEN, szTitle))
   {
      MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, TRUE);
      return;
   }

// BIDI
//
// Commented out the 'application supplied title' so that the default
// title (which may be in Arabic/Hebrew) can be displayed.

// fontDlg.pszTitle      = szTitle;       /* Application supplied title      */
   fontDlg.pszTitle      = NULL;          /* Default text.                   */
   fontDlg.pszPreview    = NULL;          /* String to print in preview wndw */
   fontDlg.pszPtSizeList = NULL;          /* Application provided size list  */
   fontDlg.pfnDlgProc    = NULL;          /* Dialog subclass procedure       */
   strcpy(szFamily, fontMetrics.szFamilyname); /* Family name of font        */
   fontDlg.pszFamilyname = szFamily;      /* point to Family name of font    */
   fontDlg.fxPointSize = fxPointSize;     /* Point size the user selected    */
   fontDlg.fl           = FNTS_CENTER |   /* FNTS_* flags - dialog styles    */
                          FNTS_INITFROMFATTRS ;
//BIDI
// The following line can be made active to always force
// Right-to-left and National language in the font dialog.
// When commented, the RtL and National Language are dependent
// on the STDDLGLANG=NATIONAL environement variable.
// fontDlg.fl          |= FNTS_NATIONAL_LANGUAGE;
   fontDlg.flFlags      = 0;              /* FNTF_* state flags              */
                                          /* Font type option bits           */
   fontDlg.flType       = (LONG) fontMetrics.fsType;
   fontDlg.flTypeMask   = 0;              /* Mask of which font types to use */
   fontDlg.flStyle      = 0;              /* The selected style bits         */
   fontDlg.flStyleMask  = 0;              /* Mask of which style bits to use */
   fontDlg.clrFore      = lClrForeground; /* Selected foreground color       */
   fontDlg.clrBack      = lClrBackground; /* Selected background color       */
   fontDlg.ulUser       = 0;              /* Blank field for application     */
   fontDlg.lReturn      = 0;              /* Return Value of the Dialog      */
   fontDlg.lSRC;                          /* System return code.             */
   fontDlg.lEmHeight    = 0;              /* Em height of the current font   */
   fontDlg.lXHeight     = 0;              /* X height of the current font    */
   fontDlg.lExternalLeading = 0;          /* External Leading of font        */
   fontDlg.hMod;                          /* Module to load custom template  */
                                          /* Nominal Point Size of font      */
   fontDlg.sNominalPointSize = fontMetrics.sNominalPointSize;
   fontDlg.usWeight = fontMetrics.usWeightClass; /* The boldness of the font */
   fontDlg.usWidth = fontMetrics.usWidthClass;  /* The width of the font     */
   fontDlg.x            = 0;              /* X coordinate of the dialog      */
   fontDlg.y            = 0;              /* Y coordinate of the dialog      */
   fontDlg.usDlgId      = IDD_FONT;       /* ID of a custom dialog template  */
   fontDlg.usFamilyBufLen = sizeof(szFamily); /*Length of family name buffer */
   fontDlg.fAttrs;                        /* Font attribute structure        */

   /*
    *   Bring up the standard Font Dialog
    */

   if(WinFontDlg(HWND_DESKTOP, hwndMLE, &fontDlg) != DID_OK)
   {
      WinReleasePS(fontDlg.hpsScreen);
      return;
   }
   fxPointSize = fontDlg.fxPointSize;     /* save point size for next dialog */

   /*
    *   If outline font, calculate the maxbaselineext and
    *   avecharwidth for the point size selected
    */

   if ( fontDlg.fAttrs.fsFontUse == FATTR_FONTUSE_OUTLINE )
   {
      ConvertVectorFontSize(fontDlg.fxPointSize, &fontDlg.fAttrs);
   }

   WinReleasePS(fontDlg.hpsScreen);
   WinSendMsg(hwndMLE, MLM_SETFONT, MPFROMP(&(fontDlg.fAttrs)), NULL);

}   /* End of SetFont()                                                 */

/*
 *   Convert vector font size using point size and fAttrs structure and
 *   return it in that structure.
 */

VOID ConvertVectorFontSize(FIXED fxPointSize, PFATTRS pfattrs)
{
  HPS   hps;
  HDC   hDC;
  LONG  lxFontResolution;
  LONG  lyFontResolution;
  SIZEF sizef;

  hps = WinGetScreenPS(HWND_DESKTOP);        /* Screen presentation space */

  /*
   *   Query device context for the screen and then query
   *   the resolution of the device for the device context.
   */

  hDC = GpiQueryDevice(hps);
  DevQueryCaps( hDC, CAPS_HORIZONTAL_FONT_RES, (LONG)1, &lxFontResolution);
  DevQueryCaps( hDC, CAPS_VERTICAL_FONT_RES, (LONG)1, &lyFontResolution);

  /*
   *   Calculate the size of the character box, based on the
   *   point size selected and the resolution of the device.
   *   The size parameters are of type FIXED, NOT int.
   *   NOTE: 1 point == 1/72 of an inch.
   */

  sizef.cx = (FIXED)(((fxPointSize) / 72 ) * lxFontResolution );
  sizef.cy = (FIXED)(((fxPointSize) / 72 ) * lyFontResolution );

  pfattrs->lMaxBaselineExt = MAKELONG( HIUSHORT( sizef.cy ), 0 );
  pfattrs->lAveCharWidth   = MAKELONG( HIUSHORT( sizef.cx ), 0 );
  WinReleasePS(hps);

}   /* end ConvertVectorPointSize() */


//BIDI
/*********************************************************************
 *  Name: TransformText()
 *
 *  Description : Performs bidirectional text conversion/transformation.
 *
 *
 *  Concepts : Prepares and calls a Layout Object to do the text
 *             transformation.
 *
 *  Parameters :  Window handle of dialog
 *
 *  Returns: TRUE=Success, FALSE=Failure
 *
 ****************************************************************/
ULONG TransformText(HWND hwnd)
{ HWND            hwndSource,
                  hwndTarget;
  LAYOUT_OBJECT   hLayout;
  LAYOUT_VALUES   Layout[4];
  UCHAR           String[128];
  APIRET          RC=0;
  ULONG           SrcBDAtts,
                  TrgBDAtts,
                  BDMask,
                  index,
                  ulLength;

                                 /* Get handles of static text windows */
  hwndTarget =  WinWindowFromID (hwnd, IDC_TEXTCONV_TARGET);
  hwndSource =  WinWindowFromID (hwnd, IDC_TEXTCONV_SOURCE);

                      /* Load string, initialize attributes and create */
                      /* layout object according to current codepage   */

                                                             /* Arabic */
  if (WinQueryCp(HMQ_CURRENT) == 864)
  {
      WinLoadString( hab, NULLHANDLE, IDS_CONVTEXT+ARA_OFF,127, String);
      BDMask    =  BDAM_TEXT_ORIENTATION | BDAM_TEXTTYPE |
                   BDAM_NUMERALS | BDAM_TEXT_SHAPE;
      SrcBDAtts =  BDA_TEXT_ORIENT_RTL | BDA_TEXTTYPE_IMPLICIT |
                   BDA_NUMERALS_NOMINAL | BDA_TEXT_NOMINAL;
      TrgBDAtts =  BDA_TEXT_ORIENT_LTR | BDA_TEXTTYPE_VISUAL   |
                   BDA_NUMERALS_CONTEXTUAL | BDA_TEXT_DISPLAY_SHAPED;
      RC = LayoutCreateObject (Locale_Arabic,&hLayout);
  }
  else
                                                           /* Hebrew */
  {
      WinLoadString( hab, NULLHANDLE, IDS_CONVTEXT, 127, String);
      BDMask    =  BDAM_TEXT_ORIENTATION | BDAM_TEXTTYPE;
      SrcBDAtts =  BDA_TEXT_ORIENT_CONTEXT | BDA_TEXTTYPE_IMPLICIT;
      TrgBDAtts =  BDA_TEXT_ORIENT_LTR     | BDA_TEXTTYPE_VISUAL;
      RC = LayoutCreateObject (Locale_Hebrew,&hLayout);
  }

  if (RC)
  {
   WinMessageBox(HWND_DESKTOP, hwnd, "Failed to create layout object",
                "Bidi text transform - Error",
                 IDD_MSGBOX, MB_ERROR | MB_OK);
     return FALSE;
  }

  ulLength=strlen(String);

                                             /* Display Source String */
  WinSetWindowText ( hwndSource, String );

                                           /* Initialize layout array */
  Layout[0].name  =  InOutTextDescrMask;
  Layout[0].value =  &BDMask;

  Layout[1].name  =  InOnlyTextDescr;
  Layout[1].value =  &SrcBDAtts;

  Layout[2].name  =  OutOnlyTextDescr;
  Layout[2].value =  &TrgBDAtts;

  Layout[3].name = 0;
                                                    /* Set layout values */
  RC = LayoutSetValues (hLayout,Layout,&index);
  if (RC)
  {
   WinMessageBox(HWND_DESKTOP, hwnd, "Failed to set layout values",
                "Bidi text transform - Error",
                 IDD_MSGBOX, MB_ERROR | MB_OK);
     return FALSE;
  }
                                        /* Do actual text transformation */
  RC = LayoutTransformText (hLayout,
                            String,
                            &ulLength,
                            String ,
                            &ulLength, NULL, NULL, NULL);
  if (RC)
  {
   WinMessageBox(HWND_DESKTOP, hwnd, "LayoutTransformText failed",
                "Bidi text transform - Error",
                 IDD_MSGBOX, MB_ERROR | MB_OK);
   return FALSE;
  }

                                           /* Display converted String */
  WinSetWindowText ( hwndTarget, String );

  return TRUE;

}

/*********************************************************************
 *  Name: ToggleWndOrient()
 *
 *  Description : Toggles a window orientation.
 *
 *
 *  Parameters :  window handle of which window orientation is toggled.
 *
 *  Returns: None
 *
 ****************************************************************/
VOID ToggleWndOrient(HWND hwnd)
{
ULONG BidiAttr = WinQueryLangInfo( hwnd, LI_BD_WND_ATTR, 0L, 0L);

      BidiAttr ^= BDAM_WND_ORIENTATION ;

      WinSetLangInfo (hwnd, LI_BD_WND_ATTR, BidiAttr,
                      BDAM_WND_ORIENTATION, LIF_WND_REFRESH, 0L);
} /* ToggleWndOrient() */

