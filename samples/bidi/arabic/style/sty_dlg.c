/**********************************************************************
*
*  File Name   : STY_DLG.C
*
*  Description : This module contains the Dialog Procedures for the user
*                defined dialogs as well as any support code they need.
*
*  Concepts    : Dialog procedures for user defined dialogs,
*                initialization of controls in the dialog boxs,
*                use of presentation parameters.
*                BIDI - Added: Bidirectional Language processing sample code.
*
*  API's       :  DosAllocMem             WinQueryFocus
*                 DosFreeMem              WinQueryPointerPos
*                 GpiQueryFonts           WinQueryPresParams
*                 WinCreateWindow         WinQueryWindowText
*                 WinDefDlgProc           WinQueryWindowUShort
*                 WinDismissDlg           WinReleasePS
*                 WinGetPS                WinRemovePresParams
*                 WinLoadMenu             WinSendDlgItemMsg
*                 WinLoadPointer          WinSendMsg
*                 WinLoadString           WinSetDlgItemText
*                 WinMapWindowPoints      WinSetPresParams
*                 WinPopupMenu            WinWindowFromID
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
************************************************************************/

/*  Include files, macros, defined constants, and externs              */

#define INCL_WINWINDOWMGR
#define INCL_WINFRAMEMGR
#define INCL_WINPOINTERS
#define INCL_WININPUT
#define INCL_WINLISTBOXES
#define INCL_WINENTRYFIELDS
#define INCL_WINSTDSPIN
#define INCL_WINSTDBOOK
#define INCL_WINMLE
#define INCL_WINMENUS
#define INCL_WINSTATICS
#define INCL_WINSTDCNR
#define INCL_WINSTDSLIDER
#define INCL_WINSTDVALSET
#define INCL_WINDIALOGS
#define INCL_WINSYS
#define INCL_WINCOUNTRY
#define INCL_WINHOOKS
#define INCL_GPILCIDS
#define INCL_GPIBITMAPS
#define INCL_DOSMEMMGR
#define INCL_DOSNLS

#include <os2.h>
#include <string.h>
#include <stdlib.h>

#define  INCL_WINSTDDLGS
#define  INCL_CATALOGSTRUCTS
#include <pmbidi.h>
#include "sty_main.h"
#include "sty_dlg.h"
#include "sty_xtrn.h"
#include "sty_help.h"

typedef struct _USERRECORD
{
  RECORDCORE  recordCore;
/*
  CDATE       date;
  CTIME       time;
  PSZ         recordData;
*/
//BIDI
  PSZ         EnglishName;
  PSZ         NationalName;
} USERRECORD, *PUSERRECORD;

#define COLORNAMELEN    20
#define NUMCOLORS       15  /* number of colors used in PP Demo */
#define NUMSPINCOLORS   16  /* number of colors used in SPIN Demo */
#define NUMFONTS         4  /* number of fonts used in PP Demo */
#define FONTPOINTSIZE   80  /* point size to use in PP Demo */
#define PPFONTPOINTSIZE  2              /* length of font point prefix */

#define TAB_WIDTH  120
#define TAB_HEIGHT  30

/*  Global variables                                                   */
                                        /* array of clr names          */
static CHAR apszPresParamColor[NUMCOLORS][COLORNAMELEN];
static PSZ *apszPresParamFont = NULL;
static LONG lNumFonts;
static CNRINFO cnrinfo;
static ULONG ulCnrAttributes;
static RECORDINSERT recordInsert;
static PUSERRECORD pUserRecord, pStartRecord;
static PFIELDINFO pFieldInfo, firstFieldInfo;
static FIELDINFOINSERT fieldInfoInsert;
static PFIELDINFOINSERT pFieldInfoInsert;
static CHAR pszViewText[] = "Text for Text View.";
static CHAR pszViewIcon[] = "Text for Icon View.";
static CHAR pszViewName[] = "Text for Name View.";
static CHAR pszTowerEnglish[64] = "Tower";
static CHAR pszTowerNational[64];
static CHAR pszWaterfallEnglish[64] = "Waterfall";
static CHAR pszWaterfallNational[64];
static CHAR pszGlobeEnglish[64] = "Globe";
static CHAR pszGlobeNational[64];
static CHAR pszKeyEnglish[64] = "Key";
static CHAR pszKeyNational[64];
static CHAR pszColumnText1English[64]= "Picture";
static CHAR pszColumnText1National[64];
static CHAR pszColumnText2[64];
static CHAR pszColumnText3[64]= "English";
static CHAR pszSampleData[] = "Sample Data";
static CHAR pszCnrTitleEnglish[64] = "Sample Container";
static CHAR pszCnrTitleNational[64];

/*
 * Set up an array of colors in the same order as the stringtable
 * constants
 */
static ULONG aulColor[NUMCOLORS-1] =
              { CLR_BLUE, CLR_RED, CLR_PINK, CLR_GREEN, CLR_CYAN,
                CLR_YELLOW, CLR_DARKGRAY, CLR_DARKBLUE, CLR_DARKRED,
                CLR_DARKPINK, CLR_DARKGREEN, CLR_DARKCYAN, CLR_BROWN,
                CLR_PALEGRAY };

/*  Entry point declarations                                           */

MRESULT EXPENTRY ProdInfoDlgProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY DemoDlgProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY PresParamDemoDlgProc(HWND hwnd, USHORT msg, MPARAM mp1,
                                      MPARAM mp2);
BOOL InitializeValueSet(HWND hwnd);
BOOL InitializeSpinButton(HWND hwnd);
BOOL InitializeSlider(HWND hwnd);
BOOL InitializeNoteBook(HWND hwnd);
BOOL InitializeContainer(HWND hwnd);
BOOL LoadListBox(HWND hwnd);
BOOL InitPresParamDemoDlg(HWND hwnd);
BOOL UpdatePresParamDemoDlg(HWND hwnd);
VOID ShowDlgHelp(HWND hwnd);
VOID SetSysMenu(HWND hwnd);


/*********************************************************************
 *  Name : ProdInfoDlgProc
 *
 *  Description : Processes all messages sent to the Product
 *                Information dialog box.
 *
 *  Concepts : Called for each message sent to the Product
 *             Information dialog box.  The Product
 *             Information box only has a button control so
 *             this routine only processes WM_COMMAND
 *             messages.  Any WM_COMMAND posted must have come
 *             from the Ok button so we dismiss the dialog
 *             upon receiving it.
 *
 *  API's : WinDismissDlg
 *          WinDefDlgProc
 *
 * Parameters   : hwnd - Window handle to which message is addressed
 *                msg - Message type
 *                mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns : Dependent upon message sent
 *
 ****************************************************************/
MRESULT EXPENTRY ProdInfoDlgProc( HWND hwnd, USHORT msg, MPARAM mp1,
                                  MPARAM mp2)
{
   switch(msg)
   {
      case WM_COMMAND:
         /*
          * No matter what the command, close the dialog
          */
         WinDismissDlg(hwnd, TRUE);
         break;

      case WM_HELP:
         ShowDlgHelp(hwnd);
         break;

      default:
         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;
   }
   return (MRESULT)0;
}           /* End of ProdInfolDlgProc()                               */



/* 4 records - first index form 0 to 3 *
 * 3 fields  second index from 1 to 2  *
 * field titles -     first index = 4L *
 * container title    first index = 5L */

PSZ pszTexts[6][3] =
 {
    {pszTowerNational, pszTowerNational, pszTowerEnglish},
    {pszWaterfallNational, pszWaterfallNational, pszWaterfallEnglish},
    {pszGlobeNational, pszGlobeNational, pszGlobeEnglish},
    {pszKeyNational, pszKeyNational, pszKeyEnglish},
    {pszColumnText1National, pszColumnText2, pszColumnText3},
    {pszCnrTitleNational}
 };



/*********************************************************************
 *  Name : DemoDlgProc
 *
 *  Description : Processes all messages sent to all Demo dialogs.
 *
 *  Concepts : Called for each message sent to a demo dialog box.
 *             A switch statement branches to the routines to be
 *             performed for each message processed.  Any messages
 *             not specifically process are passed to the default
 *             window procedure WinDefDlgProc().
 *
 *  API's : WinDismissDlg
 *          WinLoadMenu
 *          WinQueryPointerPos
 *          WinMapWindowPoints
 *          WinPopupMenu
 *          WinSendDlgItemMsg
 *          WinSetDlgItemText
 *          WinDefDlgProc
 *
 * Parameters   : hwnd - Window handle to which message is addressed
 *                msg - Message type
 *                mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns : Dependent upon message sent
 *
 ****************************************************************/
MRESULT EXPENTRY DemoDlgProc( HWND hwnd, USHORT msg, MPARAM mp1,
                              MPARAM mp2)
{
   CHAR      acBuffer[4];
   CHAR      *cData;
   PSHORT    psIdDlg;
// int       rc;

   cnrinfo.pszCnrTitle = pszCnrTitleNational;
   ulCnrAttributes = CA_CONTAINERTITLE | CA_TITLESEPARATOR;

   switch(msg)
   {
      case WM_INITDLG:

         SetSysMenu(hwnd);
         psIdDlg = (PSHORT) mp2;

         switch(*psIdDlg)
         {
        /*
         * The ShowDemoDlg() filled mp2 with a pointer to the id of
         * the dialog template used to create the dialog box.
         */
            case IDD_LISTBOXDLG:
            /*
             * Load some text into the list boxes.  If any of the
             * LoadListBox() routines fail, abort the dialog
             */
               if (!LoadListBox(WinWindowFromID(hwnd, IDC_LISTBOX1)) ||
                   !LoadListBox(WinWindowFromID(hwnd, IDC_LISTBOX2)) ||
                   !LoadListBox(WinWindowFromID(hwnd, IDC_MULTISELLISTBOX1)) ||
                   !LoadListBox(WinWindowFromID(hwnd, IDC_MULTISELLISTBOX2)))
               {
                  WinDismissDlg(hwnd, FALSE);
               }
               break;

            case IDD_COMBOBOXDLG:
            /*
             * Load some text into the combo boxes.  If any of the
             * LoadListBox() routines fail, abort the dialog
             */
               if (!LoadListBox(WinWindowFromID(hwnd, IDC_SIMPLE)) ||
                   !LoadListBox(WinWindowFromID(hwnd, IDC_DROPDOWN)) ||
                   !LoadListBox(WinWindowFromID(hwnd, IDC_DROPDOWNLIST)))
               {
                  WinDismissDlg(hwnd, FALSE);
               }
               break;

            case IDD_SPINBUTTONDLG:
            /*
             * Initialize the master and servant Spinbuttons.
             * If the initialization routine fails, abort
             * the dialog
             */
               if (!InitializeSpinButton(hwnd))
               {
                  WinDismissDlg(hwnd, FALSE);
               }
               break;

            case IDD_VALUESETDLG:
            /*
             * Initialize the Valueset control.
             * If the initialization routine fails, abort
             * the dialog
             */
               if (!InitializeValueSet(hwnd))
               {
                  WinDismissDlg(hwnd, FALSE);
               }
               break;

            case IDD_CONTAINERDLG:
            /*
             * Initialize the Container control.
             * If the initialization routine fails, abort
             * the dialog
             */
               cnrinfo.flWindowAttr = CV_ICON | ulCnrAttributes;
               if (!InitializeContainer(hwnd))
               {
                  WinDismissDlg(hwnd, FALSE);
               }
               break;

            case IDD_NOTEBOOKDLG:
            /*
             * Initialize the Notebook control.
             * If the initialization routine fails, abort
             * the dialog
             */
               if (!InitializeNoteBook(hwnd))
               {
                  WinDismissDlg(hwnd, FALSE);
               }
               break;

            case IDD_SLIDERDLG:
            /*
             * Initialize the Slider control.
             * If the initialization routine fails, abort
             * the dialog
             */
               if (!InitializeSlider(hwnd))
               {
                  WinDismissDlg(hwnd, FALSE);
               }
               break;

            default:
               break;
         }
         break;

      case WM_CONTROL:
         /*
          * Process the WM_CONTROL messages for the slider and valueset
          * set controls.
          */
         switch(SHORT2FROMMP(mp1))
         {
            case CN_REALLOCPSZ:
               return (MRESULT) TRUE;

            case CN_CONTEXTMENU:
            {
               POINTL point;
               HWND hwndMenu;
               HWND hwndContainer;
               /*
                * If we receive a EN_CONTEXTMENU MESSAGE and if the
                * current dialog if the container dialog, display a
                * popup menu that allows the user to change the
                * container view.
                */
                hwndContainer = WinWindowFromID(hwnd,IDC_CONTAINER);

                hwndMenu = WinLoadMenu(hwnd, 0, IDD_CONTAINERMENU);
                WinQueryPointerPos(HWND_DESKTOP, &point);
                WinMapWindowPoints(HWND_DESKTOP,hwndContainer,&point,1);

                WinPopupMenu(hwndContainer, hwnd, hwndMenu,
                             (SHORT)point.x, (SHORT)point.y,
                             IDM_ICON, PU_NONE | PU_SELECTITEM |
                             PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_KEYBOARD);
               break;
            }
            case SLN_CHANGE:
            {
               LONG ulValue;

               if (SHORT1FROMMP(mp1) == IDC_HSLIDER)
               {
                  ulValue = (ULONG) WinSendDlgItemMsg(hwnd, IDC_HSLIDER,
                     SLM_QUERYSLIDERINFO,
                     MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                     NULL);
                  cData = _ltoa(ulValue,acBuffer,10);
                  WinSetDlgItemText(hwnd,IDC_HSLIDERDATA, cData);
               }
               else
               {
                  ulValue = (ULONG) WinSendDlgItemMsg(hwnd, IDC_VSLIDER,
                     SLM_QUERYSLIDERINFO,
                     MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                     NULL);
                  cData = _ltoa(ulValue,acBuffer,10);
                  WinSetDlgItemText(hwnd,IDC_VSLIDERDATA, cData);
               }
               break;
            }
            case VN_SELECT:
            case VN_ENTER:
            {
               CHAR  pszNotificationTxt[256];
               CHAR  pszTmp[5];
               ULONG ulRowCol;

               ulRowCol = (ULONG)WinSendDlgItemMsg(hwnd, IDC_VALUESET,
                  VM_QUERYSELECTEDITEM, NULL, NULL);

               if ((SHORT2FROMMP(mp1)) == VN_SELECT)
                  strcpy (pszNotificationTxt, "VN_SELECT for item at row ");
               else
                  strcpy (pszNotificationTxt, "VN_ENTER for item at row ");

               _itoa(SHORT1FROMMP(ulRowCol), pszTmp, 10);
               strcat (pszNotificationTxt, pszTmp);
               strcat (pszNotificationTxt, ",Column ");
               _itoa(SHORT2FROMMP(ulRowCol), pszTmp, 10);
               strcat (pszNotificationTxt, pszTmp);
               WinSetDlgItemText(hwnd,IDC_VALUESETTEXT, pszNotificationTxt);
               break;
            }
         }
         break;
      case WM_COMMAND:
         switch( SHORT1FROMMP( mp1 ) )     /* Extract the command value    */
         {
            /*
             * Process the command messages from the container popup menu
             */
            case IDM_ICON:
               cnrinfo.flWindowAttr = CV_ICON | ulCnrAttributes;
               WinSendDlgItemMsg(hwnd,IDC_CONTAINER,CM_SETCNRINFO,&cnrinfo,
                              MPFROMLONG(CMA_FLWINDOWATTR));
               break;

            case IDM_NAME:
               cnrinfo.flWindowAttr = CV_NAME | ulCnrAttributes;
               WinSendDlgItemMsg(hwnd,IDC_CONTAINER,CM_SETCNRINFO,&cnrinfo,
                             MPFROMLONG(CMA_FLWINDOWATTR));
               break;

            case IDM_TEXT:
               cnrinfo.flWindowAttr = CV_TEXT | ulCnrAttributes;
               WinSendDlgItemMsg(hwnd,IDC_CONTAINER,CM_SETCNRINFO,&cnrinfo,
                                MPFROMLONG(CMA_FLWINDOWATTR));
               break;

            case IDM_DETAIL:
               cnrinfo.flWindowAttr = CV_DETAIL | ulCnrAttributes |
                  CA_DETAILSVIEWTITLES;

               WinSendDlgItemMsg(hwnd,IDC_CONTAINER,CM_SETCNRINFO,&cnrinfo,
                                MPFROMLONG(CMA_FLWINDOWATTR));

               WinSendDlgItemMsg(hwnd,IDC_CONTAINER,
                                 CM_INVALIDATEDETAILFIELDINFO,      /*@@@RBS*/
                                 NULL, NULL);

               break;



EndDialog:
            case IDC_SLE_DLG_END:
            case IDC_MLE_DLG_END:
            case DID_OK:
            case DID_CANCEL:          /* The Cancel pushbutton or Escape key */
               WinDismissDlg( hwnd, TRUE );     /* Removes the dialog box    */
               return (MRESULT)FALSE;

            default:
               break;
         }
         break;

      case WM_HELP:
         ShowDlgHelp(hwnd);
         break;

      default:
         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;
   }
   return (MRESULT)0;
}                                            /* End of DemoDlgProc()        */

/*********************************************************************
 *  Name : LoadListBox
 *
 *  Description : Loads the demo list box with 0 through 9
 *
 *
 *  Concepts : Called each time a demo list box is
 *             initialized.  A for 0 to 9 loop is converts
 *             the INT to a string which is added to the
 *             listbox via LM_INSERTITEM
 *
 *  API's : WinLoadString
 *          WinSendMsg
 *
 *  Parameters : hwnd - Window handle of the list box.
 *
 *  Returns: TRUE if list box is loaded successfully, FALSE otherwise
 *
 ****************************************************************/
BOOL LoadListBox(HWND hwnd)
{
   SHORT sCounter;
   CHAR pszTemp[MESSAGELEN];
   SHORT sReturn;

   SHORT BaseStringID;
   SHORT WindowID;

   WindowID = WinQueryWindowUShort(hwnd, QWS_ID);

//BIDI
//
//  Set the listboxes (and comboboxes) bidi attributes.
//
//---------------------------------------
   {
    ULONG BidiAttr;
    switch(WindowID) {

       case IDC_LISTBOX1:
       case IDC_MULTISELLISTBOX1:
       case IDC_SIMPLE:
            BidiAttr = BDA_TEXTTYPE_VISUAL  |
                       BDA_TEXT_ORIENT_LTR  |
                       BDA_WND_ORIENT_RTL;
            break;


       case IDC_LISTBOX2:
       case IDC_MULTISELLISTBOX2:
       case IDC_DROPDOWN:
       case IDC_DROPDOWNLIST:
             BidiAttr = BDA_TEXTTYPE_IMPLICIT |
                        BDA_WND_ORIENT_RTL;
             break;

       default:
             BidiAttr = 0;
             break;

    } /* end switch */


    WinSetLangInfo(hwnd, LI_BD_WND_ATTR, BidiAttr, BDAM_ALL, LIF_WND_REFRESH, 0L);
   }
//---------------------------------------
//BIDI


   BaseStringID = (WindowID == IDC_LISTBOX1) ? IDS_LISTBOX1_1 :
                  (WindowID == IDC_LISTBOX2) ? IDS_LISTBOX2_1 :
                  (WindowID == IDC_MULTISELLISTBOX1) ? IDS_LISTBOX1_1 :
                  (WindowID == IDC_MULTISELLISTBOX2) ? IDS_LISTBOX2_1 :
                  (WindowID == IDC_SIMPLE)       ? IDS_LISTBOX1_1 :
                  (WindowID == IDC_DROPDOWN)     ? IDS_LISTBOX2_1 :
                  (WindowID == IDC_DROPDOWNLIST) ? IDS_LISTBOX2_1 :
                   IDS_LISTBOX1_1 ;

   BaseStringID += ARA_OFF;

   for(sCounter = 0; sCounter < 10; sCounter++)
   {
      if (!WinLoadString(hab, (HMODULE)0, (SHORT)(BaseStringID + sCounter),
                        MESSAGELEN, (PSZ)pszTemp))
      {
         MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
         return FALSE;
      }
      sReturn = (SHORT)WinSendMsg(hwnd, LM_INSERTITEM,
                               MPFROMSHORT(LIT_SORTASCENDING),
                               MPFROMP((PSZ)pszTemp));

      if (sReturn == LIT_ERROR || sReturn == LIT_MEMERROR)
      {
         MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);

         return FALSE;
      }
    }
    return TRUE;

}                                          /*    End of LoadListBox()       */


/*********************************************************************
 *  Name : PresParamDemoDlgProc
 *
 *  Description : Processes all messages sent to the PresParamDemo
 *                dialog.
 *
 *  Concepts : Called for each message sent to the PresParamDemo
 *             dialog box.  A switch statement branches to the
 *             routines to be performed for each message
 *             processed.  Any messages not specifically process
 *             are passed to the default window procedure
 *             WinDefDlgProc().
 *
 *  API's : WinDismissDlg
 *          WinDefDlgProc
 *
 *  Parameters   : hwnd - Window handle to which message is addressed
 *                 msg - Message type
 *                 mp1 - First message parameter
 *                 mp2 - Second message parameter
 *
 *  Returns: Dependent upon message sent
 *
 ****************************************************************/
MRESULT EXPENTRY PresParamDemoDlgProc( HWND hwnd, USHORT msg,
                                       MPARAM mp1, MPARAM mp2)
{
   switch(msg)
   {
      case WM_INITDLG:
         /*
          * If initialization fails, abort the dialog
          */
         if (!InitPresParamDemoDlg(hwnd))
             WinDismissDlg(hwnd, FALSE);
         return (MRESULT)FALSE;
         break;

      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1))
         {
            case IDC_CANCEL:
            /*
             * If done, close the dialog
             */
               WinDismissDlg(hwnd, TRUE);
               break;

            case IDC_APPLY:
            /*
             * Update the dialog box
             */
               UpdatePresParamDemoDlg(hwnd);
               break;

            default:
               break;
         }
         break;

      case WM_HELP:
         ShowDlgHelp(hwnd);
         break;

      default:
         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;
   }
   return (MRESULT)0;
}                                      /* End of PresParamDemoDlgProc() */


//BIDI
/*********************************************************************
 *  Name : TextConvertDlgProc
 *
 *  Description : Processes all messages sent to the TextConvert
 *                dialog.
 *
 *  Concepts :
 *
 *  API's : WinDismissDlg
 *          WinDefDlgProc
 *
 *  Parameters   : hwnd - Window handle to which message is addressed
 *                 msg - Message type
 *                 mp1 - First message parameter
 *                 mp2 - Second message parameter
 *
 *  Returns: Dependent upon message sent
 *
 ****************************************************************/
MRESULT EXPENTRY TextConvertDlgProc( HWND hwnd, USHORT msg,
                                       MPARAM mp1, MPARAM mp2)
{
   ULONG rc;

   switch(msg)
   {

      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1))
         {
            case IDC_CANCEL:
            /*
             * If done, close the dialog
             */
               WinDismissDlg(hwnd, TRUE);
               break;

            case IDC_TEXTCONV_START:
            /*
             * Start the Text Conversion
             */
               {
               HWND hwndResult;
               hwndResult =  WinWindowFromID (hwnd, IDC_TEXTCONV_RESULT);

               WinSetWindowText ( hwndResult, "Text Transformation in progress..." );

               rc = TransformText(hwnd);

               WinSetWindowText ( hwndResult, "Text Transformation Ended..." );

               ReportTestResult(hwnd, rc);

               break;
               }

            default:
               break;
         }
         break;


      default:
         return(WinDefDlgProc(hwnd, msg, mp1, mp2));
         break;
   }
   return (MRESULT)0;
}                                      /* End of TextConvertDlgProc() */
//BIDI End

/*********************************************************************
 *  Name : InitPresParamDemoDlg
 *
 *  Description : Places the colors and fonts into the
 *                PresParamDemoDlg's comboboxes.
 *
 *  Concepts : Called when the PP demo dialog is initialized.
 *             LM_INSERTITEM is sent to each combobox for the
 *             color or font string added to the combobox.
 *
 *  API's : WinLoadString
 *          WinWindowFromID
 *          WinSendMsg
 *          GpiQueryFonts
 *          WinGetPS
 *          WinReleasePS
 *          DosAllocMem
 *          DosFreeMem
 *
 *  Parameters : hwnd - Window handle of the dialog
 *
 *  Returns: TRUE if initialization is successfull, FALSE if not
 *
 ****************************************************************/
BOOL InitPresParamDemoDlg(HWND hwnd)
{
   HWND hwndTemp;
   SHORT sCounter;
   SHORT sReturn;
   PFONTMETRICS pfm, pfmSave;
   LONG lTemp, lFonts;
   HPS hps;
   PSZ *ppszT;
   BOOL bHaveFont;

   /*
    * Load the color table
    */
   for (sCounter = 0; sCounter < NUMCOLORS; sCounter++)
   {
      if (!WinLoadString(hab, (HMODULE)0, (SHORT)(sCounter + IDS_FIRSTCOLOR),
             COLORNAMELEN, (PSZ)apszPresParamColor[sCounter]))
      {
         MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
         return FALSE;
      }
   }
   /*
    * Load foreground color combo box
    */
   hwndTemp = WinWindowFromID(hwnd, IDC_FORECOLORLIST);

   for(sCounter = 0; sCounter < NUMCOLORS; sCounter++)
   {
      sReturn = (SHORT)WinSendMsg(hwndTemp, LM_INSERTITEM,
                          MPFROMSHORT(LIT_SORTASCENDING),
                          MPFROMP((PSZ)apszPresParamColor[sCounter]));

      if (sCounter == LIT_ERROR || sCounter == LIT_MEMERROR)
      {
         MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR,
              FALSE);
         return FALSE;
      }
   }

   /*
   * Load background color combo box
   */
   hwndTemp = WinWindowFromID(hwnd, IDC_BACKCOLORLIST);

   for(sCounter = 0; sCounter < NUMCOLORS; sCounter++)
   {
      sReturn = (SHORT)WinSendMsg(hwndTemp, LM_INSERTITEM,
                            MPFROMSHORT(LIT_SORTASCENDING),
                            MPFROMP((PSZ)apszPresParamColor[sCounter]));

       if (sCounter == LIT_ERROR || sCounter == LIT_MEMERROR)
       {
          MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR,
                FALSE);
          return FALSE;
       }
   }
   /*
    * Fonts.  Find all of the fonts of point size desired and normal
    * weight.  Put the facenames in the combo box
    */
   hwndTemp = WinWindowFromID(hwnd, IDC_FONTLIST);
   hps = WinGetPS(hwndTemp);

   /*
    * Determine the number of fonts available
    */
   lFonts = 0;
   lTemp = 0;
   lFonts = GpiQueryFonts(hps, QF_PUBLIC, NULL, &lTemp,
                  (ULONG)sizeof(FONTMETRICS), (PFONTMETRICS)NULL);

   if (lFonts == GPI_ALTERROR)
   {
       MessageBox(hwndMain, IDMSG_CANNOTLOADFONTS, MB_OK | MB_ERROR, FALSE);
       WinReleasePS(hps);
       return FALSE;
   }

   /*
    * Allocate buffer for fontmetric structures
    */
   if (DosAllocMem((PPVOID)&pfm, lFonts * (ULONG)sizeof(FONTMETRICS),
           fALLOC))
   {
      MessageBox(hwndMain, IDMSG_CANNOTALLOCATEMEMORY, MB_OK | MB_ERROR,
            FALSE);
      WinReleasePS(hps);
      return FALSE;
   }
   pfmSave = pfm;

   /*
    * Get all fonts
    */
   lTemp = GpiQueryFonts(hps, QF_PUBLIC, NULL, &lFonts,
                (ULONG)sizeof(FONTMETRICS), (PFONTMETRICS)pfm);

   WinReleasePS(hps);

   if (lTemp == GPI_ALTERROR)
   {
      MessageBox(hwndMain, IDMSG_CANNOTLOADFONTS, MB_OK | MB_ERROR, FALSE);
      return FALSE;
   }

   /*
    * Allocate buffer for array of string names, freeing the first
    * buffer if necessary
    */
   if ((BOOL)apszPresParamFont)
   {
      ppszT = apszPresParamFont;
      while ((BOOL)*ppszT)
      {
         DosFreeMem(*ppszT++);
      }

      DosFreeMem(apszPresParamFont);
   }

   if (DosAllocMem((PPVOID)&apszPresParamFont,
          (lFonts + 1) * (ULONG)sizeof(PSZ), fALLOC))
   {
      MessageBox(hwndMain, IDMSG_CANNOTALLOCATEMEMORY, MB_OK | MB_ERROR,
           FALSE);
      DosFreeMem((PVOID)pfmSave);
      apszPresParamFont = NULL;
      return FALSE;
   }

   /*
    * Initialize array to NULL
    */
   lTemp = 0;
   ppszT = apszPresParamFont;
   while (lTemp++ < lFonts)
       *ppszT++ = (PSZ)NULL;
   *ppszT = (PSZ)NULL;


   /*
    * Walk through all fonts.  If the font matches the point size
    * desired and has a weight and width class of normal (5) and
    * no special attributes (e. g. italic, underline, etc.).  If
    * it does, then add its string to the combo box
    */
   lNumFonts = 0;
   ppszT = apszPresParamFont;

   while (lFonts--)
   {
      if (pfm->sNominalPointSize == FONTPOINTSIZE &&
             pfm->usWeightClass == 5 && pfm->usWidthClass == 5 &&
             pfm->fsSelection == 0)
      {
      /*
       * Make sure we don't have this font.  If we don't,
       * then add the font to the list
       */
         lTemp = 0;
         bHaveFont = FALSE;
         while (lTemp < lNumFonts)
         {
            if (!strcmp(pfm->szFacename, apszPresParamFont[(INT)lTemp]))
            {
               bHaveFont = TRUE;
               break;
            }
            else
               lTemp++;
         }

         if (!bHaveFont)
         {
            if (DosAllocMem((PPVOID)ppszT,
                (LONG)(FACESIZE * sizeof(CHAR)), fALLOC))
            {
               MessageBox(hwndMain, IDMSG_CANNOTALLOCATEMEMORY,
                  MB_OK | MB_ERROR, FALSE);
               DosFreeMem((PVOID)pfmSave);
               return FALSE;
             }

             strcpy(*ppszT++, pfm->szFacename);
             lNumFonts++;
         }
      }
      pfm++;
   }

   /*
    * Install the name of each font into the combo box
    */
   ppszT = apszPresParamFont;
   while ((BOOL)*ppszT)
   {
      lTemp = (LONG)WinSendMsg(hwndTemp, LM_INSERTITEM,
                   MPFROMSHORT(LIT_SORTASCENDING), MPFROMP((PSZ)*ppszT++));

      if (sCounter == LIT_ERROR || sCounter == LIT_MEMERROR)
      {
         MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR,
              FALSE);
         DosFreeMem((PVOID)pfmSave);
         return FALSE;
      }
   }

   /*
    * Add "DefaulTemp" text onto the end
    */
   lTemp = (LONG)WinSendMsg(hwndTemp, LM_INSERTITEM,
                 MPFROMSHORT(LIT_SORTASCENDING),
                 MPFROMP(apszPresParamColor[IDS_DEFAULT - IDS_FIRSTCOLOR]));

   if (sCounter == LIT_ERROR || sCounter == LIT_MEMERROR)
   {
      MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR, FALSE);
      DosFreeMem((PVOID)pfmSave);
      return FALSE;
   }
   DosFreeMem((PVOID)pfmSave);
   return TRUE;
}                                      /* End of InitPresParamDemoDlg()   */

/*********************************************************************
 *  Name : UpdatePresParamDemoDlg
 *
 *  Description : Sets/Removes the Presentation Parameter of the sample
 *                text window depending upon the parameters chosen,
 *
 *  Concepts : Called when user wants to update the window.  The string
 *             of each combobox is queried and then the color or font
 *             is set depending upon the string chosen.
 *
 *  API's : WinWindowFromID
 *          WinQueryWindowText
 *          WinSendMsg
 *          WinSetPresParams
 *          WinQueryPresParams
 *          WinRemovePresParams
 *
 *  Parameters : hwnd - Handle of Presentation Params dialog
 *
 *  Returns: TRUE if update is successfull, FALSE if not
 *
 ****************************************************************/
BOOL UpdatePresParamDemoDlg(HWND hwnd)
{
   HWND hwndTemp, hwndSampleText, hwndSampleButton;
   CHAR pszTemp[COLORNAMELEN];
   BYTE abBuf[FACESIZE + PPFONTPOINTSIZE];
   ULONG ulColor, ulPresParams;
   SHORT sCounter;

   /*
    * Get window handle of the sample text box
    */
   hwndSampleText = WinWindowFromID(hwnd, IDC_SAMPLETEXT);
   hwndSampleButton = WinWindowFromID(hwnd, IDC_CHECKBOX);

   /*
    * Get the text of the foreground color combobox
   */
   hwndTemp = WinWindowFromID(hwnd, IDC_FORECOLORLIST);

   if (WinQueryWindowText(hwndTemp, COLORNAMELEN, (PSZ)pszTemp))
   {
   /*
    * Find the text in the list of color names
    */
      sCounter = 0;
      while (sCounter < NUMCOLORS)
      {
         if (!strcmp(pszTemp, apszPresParamColor[sCounter]))
         {
            ulColor = aulColor[sCounter];
            break;
         }
         else
            sCounter++;
      }

      /*
       * If color is not default, set the color.  If the default is
       * selected, then remove the color presentation parameter if
       * it exists.  If the value is not a valid color, then don't
       * don't do anything
       */
       if (sCounter < NUMCOLORS)
       {
          sCounter += IDS_FIRSTCOLOR;
          if (sCounter < IDS_DEFAULT)
          {
             if (!WinSetPresParam(hwndSampleText, PP_FOREGROUNDCOLORINDEX,
                   (ULONG)sizeof(LONG), (PVOID)&ulColor) ||
                   !WinSetPresParam(hwndSampleButton,
                   PP_FOREGROUNDCOLORINDEX, (ULONG)sizeof(LONG),
                   (PVOID)&ulColor))
             {
                MessageBox(hwndMain, IDMSG_CANNOTSETPP, MB_OK | MB_ERROR, FALSE);
             }
          }
          else
          {
          /*
           * If setting presentation parameter to the default, remove
           * the presentation parameter, but only if it has been
           * set
           */
              if (sCounter == IDS_DEFAULT &&
                   WinQueryPresParam(hwndSampleText, PP_FOREGROUNDCOLORINDEX,
                   0, &ulPresParams, (ULONG)sizeof(LONG), (PVOID)&ulColor,
                   QPF_NOINHERIT) != 0)
              {
                 if (!WinRemovePresParam(hwndSampleText,
                     PP_FOREGROUNDCOLORINDEX) ||
                     !WinRemovePresParam(hwndSampleButton,
                     PP_FOREGROUNDCOLORINDEX))
                 {
                    MessageBox(hwndMain, IDMSG_CANNOTSETPP, MB_OK | MB_ERROR,
                         FALSE);
                 }
              }
          }
       }
   }

   /*
    * Do the same for the background color combobox
    */
   hwndTemp = WinWindowFromID(hwnd, IDC_BACKCOLORLIST);
   if (WinQueryWindowText(hwndTemp, COLORNAMELEN, (PSZ)pszTemp))
   {
   /*
    * Find the text in the list of color names
   */
      sCounter = 0;
      while (sCounter < NUMCOLORS)
      {
         if (!strcmp(pszTemp, apszPresParamColor[sCounter]))
         {
            ulColor = aulColor[sCounter];
            break;
         }
         else
            sCounter++;
      }

      /*
       * If color is not default, set the color.  If the default is
       * selected, then remove the color presentation parameter.  If
       * the value is not a valid color, the don't do anything
       */
       if (sCounter < NUMCOLORS)
       {
          sCounter += IDS_FIRSTCOLOR;
          if (sCounter < IDS_DEFAULT)
          {
             if (!WinSetPresParam(hwndSampleText, PP_BACKGROUNDCOLORINDEX,
                 (ULONG)sizeof(LONG), (PVOID)&ulColor) ||
                 !WinSetPresParam(hwndSampleButton, PP_BACKGROUNDCOLORINDEX,
                 (ULONG)sizeof(LONG), (PVOID)&ulColor))
             {
                MessageBox(hwndMain, IDMSG_CANNOTSETPP, MB_OK | MB_ERROR,
                      FALSE);
             }
          }
          else
          {
             if (sCounter == IDS_DEFAULT &&
                  WinQueryPresParam(hwndSampleText,
                  PP_BACKGROUNDCOLORINDEX, 0, &ulPresParams, (ULONG)sizeof(LONG),
                  (PVOID)&ulColor, QPF_NOINHERIT) != 0)
             {
                if (!WinRemovePresParam(hwndSampleText,
                    PP_BACKGROUNDCOLORINDEX) ||
                    !WinRemovePresParam(hwndSampleButton,
                    PP_BACKGROUNDCOLORINDEX))
                {
                   MessageBox(hwndMain, IDMSG_CANNOTSETPP, MB_OK | MB_ERROR,
                         FALSE);
                }
             }
          }
       }
   }

   /*
    * Get the text of the font combobox
    */
   hwndTemp = WinWindowFromID(hwnd, IDC_FONTLIST);
   if (WinQueryWindowText(hwndTemp, FACESIZE, (PSZ)pszTemp))
   {
   /*
    * If Font selected is "Default", remove font pres. param.
    */
      if (!strcmp(pszTemp, apszPresParamColor[IDS_DEFAULT - IDS_FIRSTCOLOR]))
      {
         if (WinQueryPresParam(hwndSampleText, PP_FONTNAMESIZE, 0,
              &ulPresParams, (ULONG)(FACESIZE + PPFONTPOINTSIZE), (PVOID)abBuf,
              QPF_NOINHERIT) != 0)
         {
            if (!WinRemovePresParam(hwndSampleText, PP_FONTNAMESIZE) ||
                !WinRemovePresParam(hwndSampleButton, PP_FONTNAMESIZE))
            {
               MessageBox(hwndMain, IDMSG_CANNOTSETPP, MB_OK | MB_ERROR,
                    FALSE);
            }
         }
      }
      else
      {
      /*
       * Font is not default.
       * AbBuf will hold the font point size and name in
       * the form <pt>.<name>.  First we fill abBuf with the
       * font point prefix and then append the font name
       * retrieved from the combobox.
       */

         if (!WinLoadString(hab, (HMODULE)0, IDS_PPFONTPOINT,
              COLORNAMELEN, (PSZ)abBuf))
         {
            MessageBox(hwndMain, IDMSG_CANNOTLOADSTRING, MB_OK | MB_ERROR,
                 FALSE);
            return FALSE;
         }
         strcat((PSZ)abBuf, pszTemp);

         if (!WinSetPresParam(hwndSampleText, PP_FONTNAMESIZE,
             (ULONG)strlen((PSZ)abBuf) + 1, (PVOID)abBuf) ||
             !WinSetPresParam(hwndSampleButton, PP_FONTNAMESIZE,
             (ULONG)strlen((PSZ)abBuf) + 1, (PVOID)abBuf))
         {
            MessageBox(hwndMain, IDMSG_CANNOTSETPP, MB_OK | MB_ERROR,
                 FALSE);
         }
      }
   }
   return TRUE;
}                                     /* End of UpdatePresParamDemoDlg() */

/*********************************************************************
 *  Name : ShowDlgHelp
 *
 *  Description : Displays the help panel for the current selected
 *                item in the dialog window
 *
 *  Concepts : Called each time a WM_HELP message is posted to a
 *             dialog gets the id value of the window and determine
 *             which help panel to display.  Then sends a message
 *             to the help instance to display the panel.  If the
 *             dialog or item is not included here, then the
 *             unknown dialog or unknown item panel is displayed.
 *
 *  API's : WinQueryFocus
 *          WinQueryWindowUShort
 *          WinWindowFromID
 *
 *  Parameters : hwnd - Handle of the dialog box
 *
 *  Returns: Void
 *
 ****************************************************************/
VOID ShowDlgHelp(HWND hwnd)
{
   SHORT idPanel, idDlg, idItem;
   HWND hwndFocus;

   /*
   * Get the id of the dialog box
   */
   idDlg = WinQueryWindowUShort(hwnd, QWS_ID);

   /*
    * Finds which window has the focus and gets its id
    */
   hwndFocus = WinQueryFocus(HWND_DESKTOP);
   idItem = WinQueryWindowUShort(hwndFocus, QWS_ID);

   switch(idDlg)
   {
      case IDD_BUTTONSDLG:
         switch(idItem)
         {
            case IDC_RADIO1:
            case IDC_RADIO2:
            case IDC_RADIO3:
               idPanel = PANEL_BUTTONSDLG_RADIO;
               break;

            case IDC_CHECKBOX:
               idPanel = PANEL_BUTTONSDLG_CHECKBOX;
               break;

            case IDC_3STATE:
               idPanel = PANEL_BUTTONSDLG_THREESTATE;
               break;

            case IDC_PUSHBUTTON:
               idPanel = PANEL_BUTTONSDLG_PUSHBUTTON;
               break;

            case IDC_OK:
               idPanel = PANEL_BUTTONSDLG_OK;
               break;

            case IDC_HELP:
               idPanel = PANEL_BUTTONSDLG_HELP;
               break;

            default:
               idPanel = PANEL_UNKNOWN;
               break;
         }
         break;

      case IDD_LISTBOXDLG:
         switch(idItem)
         {
            case IDC_LISTBOX1:
            case IDC_LISTBOX2:
               idPanel = PANEL_LISTBOXDLG_SINGLE;
               break;

            case IDC_MULTISELLISTBOX1:
            case IDC_MULTISELLISTBOX2:
               idPanel = PANEL_LISTBOXDLG_MULTIPLE;
               break;

            case IDC_OK:
               idPanel = PANEL_LISTBOXDLG_OK;
               break;

            case IDC_HELP:
               idPanel = PANEL_LISTBOXDLG_HELP;
               break;

            default:
               idPanel = PANEL_UNKNOWN;
               break;
         }
         break;

      case IDD_COMBOBOXDLG:
         switch(idItem)
         {
            case IDC_SIMPLE:
               idPanel = PANEL_COMBOBOXDLG_SIMPLE;
               break;

            case IDC_DROPDOWN:
               idPanel = PANEL_COMBOBOXDLG_DROPDOWN;
               break;

            case IDC_DROPDOWNLIST:
               idPanel = PANEL_COMBOBOXDLG_DROPDOWNLIST;
               break;

            case IDC_OK:
               idPanel = PANEL_COMBOBOXDLG_OK;
               break;

            case IDC_HELP:
               idPanel = PANEL_COMBOBOXDLG_HELP;
               break;

            default:
               /*
                * Check to see if window that has the focus is the
                * entry field of the combobox.  If it is, then
                * call the appropriate combobox help panel.  If
                * not, then call the unknown panel
                */
               if (WinWindowFromID(WinWindowFromID(hwnd, IDC_SIMPLE),
                   CBID_EDIT) == hwndFocus)
               {
                  idPanel = PANEL_COMBOBOXDLG_SIMPLE;
               }
               else
                  if (WinWindowFromID(WinWindowFromID(hwnd, IDC_DROPDOWN),
                     CBID_EDIT) == hwndFocus)
                  {
                       idPanel = PANEL_COMBOBOXDLG_DROPDOWN;
                  }
                  else
                       if (WinWindowFromID(
                           WinWindowFromID(hwnd, IDC_DROPDOWNLIST),
                           CBID_EDIT) == hwndFocus)
                       {
                          idPanel = PANEL_COMBOBOXDLG_DROPDOWNLIST;
                       }
                       else
                          idPanel = PANEL_UNKNOWN;
              break;
         }
         break;

      case IDD_SL_ENTRYFIELDDLG:
      case IDD_ML_ENTRYFIELDDLG:
         switch(idItem)
         {
            case IDC_ENTRY_1:
            case IDC_ENTRY_2:
            case IDC_ENTRY_3:
            case IDC_ENTRY_4:
               idPanel = PANEL_ENTRYFIELDDLG_ENTRY;
               break;

            case IDC_MLE_1:
            case IDC_MLE_2:
               idPanel = PANEL_ENTRYFIELDDLG_MLE;
               break;

            case IDC_OK:
               idPanel = PANEL_ENTRYFIELDDLG_OK;
               break;

            case IDC_HELP:
               idPanel = PANEL_ENTRYFIELDDLG_HELP;
               break;

            default:
               idPanel = PANEL_UNKNOWN;
               break;
         }
         break;

      case IDD_STATICDLG:
         switch(idItem)
         {
            case IDC_OK:
               idPanel = PANEL_STATICDLG_OK;
               break;

            case IDC_HELP:
               idPanel = PANEL_STATICDLG_HELP;
               break;

            default:
               idPanel = PANEL_UNKNOWN;
               break;
         }
         break;

      case IDD_PPDEMODLG:
         switch(idItem)
         {
            case IDC_FORECOLORLIST:
               idPanel = PANEL_PPDEMODLG_FORECOLORLIST;
               break;

            case IDC_BACKCOLORLIST:
               idPanel = PANEL_PPDEMODLG_BACKCOLORLIST;
               break;

            case IDC_FONTLIST:
               idPanel = PANEL_PPDEMODLG_FONTLIST;
               break;

            case IDC_CHECKBOX:
               idPanel = PANEL_PPDEMODLG_TESTBUTTON;
               break;

            case IDC_CANCEL:
               idPanel = PANEL_PPDEMODLG_CANCEL;
               break;

            case IDC_APPLY:
               idPanel = PANEL_PPDEMODLG_APPLY;
               break;

            case IDC_HELP:
               idPanel = PANEL_PPDEMODLG_HELP;
               break;

            default:
            /*
             * Check to see if window that has the focus is the
             * entry field of the combobox.  If it is, then
             * call the appropriate combobox help panel.  If
             * not, then call the unknown panel
             */
               if (WinWindowFromID(
                   WinWindowFromID(hwnd, IDC_FORECOLORLIST),
                   CBID_EDIT) == hwndFocus)
               {
                  idPanel = PANEL_PPDEMODLG_FORECOLORLIST;
               }
               else
                  if (WinWindowFromID(
                      WinWindowFromID(hwnd, IDC_BACKCOLORLIST),
                      CBID_EDIT) == hwndFocus)
                  {
                     idPanel = PANEL_PPDEMODLG_BACKCOLORLIST;
                  }
                  else
                     if(WinWindowFromID(
                        WinWindowFromID(hwnd, IDC_FONTLIST),
                        CBID_EDIT) == hwndFocus)
                     {
                        idPanel = PANEL_PPDEMODLG_FONTLIST;
                     }
                     else
                        idPanel = PANEL_UNKNOWN;
               break;
         }
         break;

      case IDD_CONTAINERDLG:
         switch(idItem)
         {
            case IDC_CONTAINER:
               idPanel = PANEL_CONTAINERDLG_CONTAINER;
               break;

            case IDC_OK:
               idPanel = PANEL_CONTAINERDLG_OK;
               break;

            case IDC_CANCEL:
               idPanel = PANEL_CONTAINERDLG_CANCEL;
               break;

            case IDC_HELP:
               idPanel = PANEL_CONTAINERDLG_HELP;
               break;

            default:
               idPanel = PANEL_UNKNOWN;
               break;
         }
         break;

      case IDD_NOTEBOOKDLG:
         switch(idItem)
         {
            case IDC_NOTEBOOK:
               idPanel = PANEL_NOTEBOOKDLG_NOTEBOOK;
               break;

            case IDC_OK:
               idPanel = PANEL_NOTEBOOKDLG_OK;
               break;

            case IDC_CANCEL:
               idPanel = PANEL_NOTEBOOKDLG_CANCEL;
               break;

            case IDC_HELP:
               idPanel = PANEL_NOTEBOOKDLG_HELP;
               break;

            default:
               idPanel = PANEL_UNKNOWN;
               break;
        }
        break;
      case IDD_SLIDERDLG:
         switch(idItem)
         {
            case IDC_VSLIDER:
               idPanel = PANEL_SLIDERDLG_VSLIDER;
               break;

            case IDC_HSLIDER:
               idPanel = PANEL_SLIDERDLG_HSLIDER;
               break;

            case IDC_OK:
               idPanel = PANEL_SLIDERDLG_OK;
               break;

            case IDC_CANCEL:
               idPanel = PANEL_SLIDERDLG_CANCEL;
               break;

            case IDC_HELP:
               idPanel = PANEL_SLIDERDLG_HELP;
               break;

            default:
               idPanel = PANEL_UNKNOWN;
               break;
         }
         break;
      case IDD_VALUESETDLG:
         switch(idItem)
         {
            case IDC_VALUESET:
               idPanel = PANEL_VALUESETDLG_VALUESET;
               break;

            case IDC_OK:
               idPanel = PANEL_VALUESETDLG_OK;
               break;

            case IDC_CANCEL:
               idPanel = PANEL_VALUESETDLG_CANCEL;
               break;

            case IDC_HELP:
               idPanel = PANEL_VALUESETDLG_HELP;
               break;

            default:
               idPanel = PANEL_UNKNOWN;
               break;
         }
         break;
      case IDD_SPINBUTTONDLG:
         switch(idItem)
         {
            case IDC_MASTER:
               idPanel = PANEL_SPINBUTTONDLG_MASTER;
               break;

            case IDC_SERVANT:
               idPanel = PANEL_SPINBUTTONDLG_SERVANT;
               break;

            case IDC_OK:
               idPanel = PANEL_SPINBUTTONDLG_OK;
               break;

            case IDC_CANCEL:
               idPanel = PANEL_SPINBUTTONDLG_CANCEL;
               break;

            case IDC_HELP:
               idPanel = PANEL_SPINBUTTONDLG_HELP;
               break;

            default:
               idPanel = PANEL_UNKNOWN;
               break;
         }
         break;

      case IDD_PRODINFO:
         idPanel = PANEL_ABOUTBOX;
         break;

      default:
         idPanel = PANEL_UNKNOWNDLG;
         break;
   }
   DisplayHelpPanel(idPanel);
}                                       /* End of ShowDlgHelp()        */

/*********************************************************************
 *  Name : InitializeValueSet
 *
 *  Description : Loads the demo Value Set with 4 different icons
 *                and with 4 colors derived form the color index.
 *
 *  Concepts : Called each time a demo Value Set is initialized A
 *             for 1 to 4 loop sets an Icon in each column of the
 *             First row of the Value Set via a VM_SETITEM
 *             message.  The Second row is set to a color by
 *             sending the VM_SETITEMATTR message.
 *
 *  API's : WinSendDlgItemMsg
 *
 *  Parameters : hwnd - Handle of the Valueset dialog.
 *
 *  Returns: TRUE if Value Set is loaded successfully, FALSE otherwise
 *
 ****************************************************************/
BOOL InitializeValueSet(HWND hwnd)
{
   USHORT usIndex;
//BIDI
// Set the Value Set to have Right-To-Left text orientation and
// Visual text.
//
    WinSetLangInfo(WinWindowFromID(hwnd,IDC_VALUESET),
              LI_BD_WND_ATTR,
              (BDA_INIT            |
               BDA_TEXT_ORIENT_RTL |
               BDA_TEXTTYPE_VISUAL  ),
              BDAM_ALL, 0L, 0L);

   for (usIndex = 1; usIndex <= 4;usIndex++)
   {
     // First row
     if (usIndex ==1) {
        // First item is a Text item.
        if ( !WinSendDlgItemMsg( hwnd,IDC_VALUESET, VM_SETITEMATTR,
              MPFROM2SHORT(1,usIndex),
              MPFROM2SHORT( VIA_TEXT, TRUE )))
            return FALSE;
        if ( !WinSendDlgItemMsg(hwnd, IDC_VALUESET, VM_SETITEM,
              MPFROM2SHORT(1,usIndex),
              MPFROMP("txeT yM")))
            return FALSE;

      } else {
         // Other items are icons.
        if ( !WinSendDlgItemMsg(hwnd, IDC_VALUESET, VM_SETITEM,
              MPFROM2SHORT(1,usIndex),
              MPFROMLONG( WinLoadPointer(HWND_DESKTOP, 0,
              (SHORT) (IDR_MAIN + usIndex)))))
            return FALSE;
     }

     // Second row
     if ( !WinSendDlgItemMsg( hwnd,IDC_VALUESET, VM_SETITEMATTR,
          MPFROM2SHORT(2,usIndex),
          MPFROM2SHORT( VIA_COLORINDEX, TRUE )))
        return FALSE;

     if ( !WinSendDlgItemMsg(hwnd, IDC_VALUESET, VM_SETITEM,
          MPFROM2SHORT(2,usIndex),
          MPFROMSHORT(usIndex)))
        return FALSE;
   }
   return TRUE;
}                                       /* End of InitializeValueset    */
/*********************************************************************
 *  Name : InitializeSlider
 *
 *  Description :  Set the Sliders Tick size and Scale Text.
 *
 *  Concepts : Called each time a demo Slider controls ar initialized
 *             Ses the he initail value of the sliders output display
 *             to 0. A for 0 to 10 for loop sets the ruler text and
 *             tick size for both the Horizontal and Vertical Sliders
 *             via SLM_SETSCALETEXT and SLM_SETTICKSIZE message.
 *
 *  API's : WinSendDlgItemMsg
 *
 *  Parameters : hwnd - Handle of the Slider dialog.
 *
 *  Returns: TRUE if Sliders are initialized successfully, FALSE otherwise
 *
 *******************************************************************/
BOOL InitializeSlider(HWND hwnd)
{
   USHORT usIndex;
   CHAR   acBuffer[4];
   CHAR   *cData;

//BIDI
//
// Sets both slider to have RtL window orientation and RtL text.
//
   WinSetLangInfo(WinWindowFromID(hwnd,IDC_HSLIDERDATA),
            LI_BD_WND_ATTR, BDA_WND_ORIENT_RTL | BDA_TEXT_ORIENT_RTL | BDA_INIT,
                 BDAM_ALL, 0L, 0L);
   WinSetLangInfo(WinWindowFromID(hwnd,IDC_VSLIDERDATA),
            LI_BD_WND_ATTR, BDA_WND_ORIENT_RTL | BDA_TEXT_ORIENT_RTL | BDA_INIT,
                 BDAM_ALL, 0L, 0L);
//BIDI

   cData = _ltoa(0,acBuffer,10);
   WinSetDlgItemText(hwnd,IDC_HSLIDERDATA, cData);
   WinSetDlgItemText(hwnd,IDC_VSLIDERDATA, cData);
   for (usIndex = 0;usIndex < 10 ;usIndex ++ )
   {
      _itoa(usIndex,acBuffer,10);
      if ( !WinSendDlgItemMsg(hwnd, IDC_HSLIDER, SLM_SETTICKSIZE,
               MPFROM2SHORT(usIndex, 5), NULL))
        return FALSE;

      if ( !WinSendDlgItemMsg(hwnd, IDC_HSLIDER, SLM_SETSCALETEXT,
               MPFROMSHORT(usIndex), MPFROMP(acBuffer)))
        return FALSE;

      if ( !WinSendDlgItemMsg(hwnd, IDC_VSLIDER, SLM_SETTICKSIZE,
               MPFROM2SHORT(usIndex, 5), NULL))
        return FALSE;

      if ( !WinSendDlgItemMsg(hwnd, IDC_VSLIDER, SLM_SETSCALETEXT,
                 MPFROMSHORT(usIndex), MPFROMP(acBuffer)))
        return FALSE;
   }
   return TRUE;
}                                       /* End of InitializeSlider      */

//BIDI
/*********************************************************************
 *  Name : Bidi_QueryCp()
 *
 *  Description : Routine to query the current window codepage.
 *
 *
 *********************************************************************/
ULONG  Bidi_QueryCp()
{
ULONG  CpList[2];
ULONG  CpSize;

ULONG cp = WinQueryCp(HMQ_CURRENT);

      /* If WinQueryCp fails, (this happens sometimes), then */
            /* we use Process codepage.                            */

      if (cp == 0L)
      {

         DosQueryCp(sizeof(CpList),       /* Length of list */
                             CpList,               /* List */
                           &CpSize);             /* Length of returned list */

         cp = CpList[0];
               }

      return cp;

}  /* Bidi_QueryCp() */
// BIDI - end

/*********************************************************************
 *  Name : InitializeSpinButton
 *
 *  Description : Set the intital values for the spin button controls.
 *
 *  Concepts : Sends a SPBM_SETARRAY to the Master Spin Button to set
 *             the values (text strings) available to the control.
 *             Sets the initial value to the array index of 0 by
 *             sending a SPBM_SETCURRENTVALUE.  The Servant Spin Button
 *             is initialized to the range 0 to 100 by sending a
 *             SPBM_SETLIMITS message.  Sends a SPBM_SETMASTER message
 *             to set the MASTER/SERVANT relationship between the two
 *             spin buttons.
 *
 *  API's : WinSendDlgItemMsg
 *          WinWindowFromID
 *
 *  Parameters : hwnd - Handle of the Spinnbutton dialog.
 *
 *  Returns: TRUE if Spinbuttons are initialized successfully, FALSE otherwise
 *
 ****************************************************************/
static CHAR aspinColors[NUMSPINCOLORS][32];

BOOL InitializeSpinButton(HWND hwnd)
{
   static PSZ   spinColors[NUMSPINCOLORS];
/*
   static PSZ   spinColors[] = {"White", "Black", "Blue", "Red"," Pink",
                     "Green", "Cyan", "Yellow", "Dark Grey", "Dark Blue",
                     "Dark Red", "Dark Pink", "Dark Green", "Dark Cyan",
                     "Brown", "Pale Grey"};
*/
   ULONG I, J;

//BIDI
// Sets both spinbuttons to have right-to-left window orientation
// and right-to-left, Visual text.
//
   WinSetLangInfo(WinWindowFromID(hwnd,IDC_MASTER),
            LI_BD_WND_ATTR,
            (BDA_INIT              |
             BDA_WND_ORIENT_RTL    |
             BDA_TEXT_ORIENT_RTL   |
             BDA_TEXTTYPE_VISUAL   ),
             BDAM_ALL, 0L, 0L);
   WinSetLangInfo(WinWindowFromID(hwnd,IDC_SERVANT),
            LI_BD_WND_ATTR,
            (BDA_INIT              |
             BDA_WND_ORIENT_RTL    |
             BDA_TEXT_ORIENT_RTL   |
             BDA_TEXTTYPE_VISUAL   ),
             BDAM_ALL, 0L, 0L);

   I = IDS_SPIN_WHITE;
      I += ARA_OFF;

//BIDI

   for (J = 0; J < NUMSPINCOLORS; J++, I += 2)
   {
      WinLoadString(hab, NULLHANDLE, I, 31, aspinColors[J]);
      spinColors[J] = aspinColors[J];
   }

   if ( !WinSendDlgItemMsg( hwnd, IDC_MASTER, SPBM_SETARRAY, spinColors,
             MPFROMLONG(NUMSPINCOLORS)))
      return FALSE;

   if ( !WinSendDlgItemMsg( hwnd, IDC_MASTER, SPBM_SETCURRENTVALUE,
             MPFROMLONG(0), NULL))
      return FALSE;

   if ( !WinSendDlgItemMsg( hwnd, IDC_SERVANT, SPBM_SETLIMITS,
             MPFROMLONG(100), MPFROMLONG(0)))
      return FALSE;

   if ( !WinSendDlgItemMsg( hwnd, IDC_SERVANT, SPBM_SETCURRENTVALUE,
             MPFROMLONG(0), NULL))
      return FALSE;

   if (!WinSendDlgItemMsg(
           hwnd,
           IDC_SERVANT,
           SPBM_SETMASTER,
           MPFROMHWND( WinWindowFromID(hwnd, IDC_MASTER)),
           MPVOID)
      )
      return FALSE;
   return TRUE;
}                                       /* End of InitializeSpinbutton  */



static CHAR  pszNoteBookText[512];
static CHAR  pszStatusLine1[64];
static CHAR  pszStatusLine2[64];
static CHAR  pszStatusLine3[64];
static CHAR  pszTab1Ma[64];
static CHAR  pszTab1Mi1[64];
static CHAR  pszTab1Mi2[64];
static CHAR  pszTab2Ma[64];
static CHAR  pszCatTitle[20];
VOID LoadBookStrings(ULONG Cp)
{
   LONG Offset = ARA_OFF;

   WinLoadString(hab, NULLHANDLE, Offset+IDS_BOOK_PAGE2, 511,  pszNoteBookText);
   WinLoadString(hab, NULLHANDLE, Offset+IDS_BOOK_STL1,  63,   pszStatusLine1);
   WinLoadString(hab, NULLHANDLE, Offset+IDS_BOOK_STL2,  63,   pszStatusLine2);
   WinLoadString(hab, NULLHANDLE, Offset+IDS_BOOK_STL3,   63,  pszStatusLine3);
   WinLoadString(hab, NULLHANDLE, Offset+IDS_BOOK_TAB1MA, 63,  pszTab1Ma);
   WinLoadString(hab, NULLHANDLE, Offset+IDS_BOOK_TAB1MI1,63,  pszTab1Mi1);
   WinLoadString(hab, NULLHANDLE, Offset+IDS_BOOK_TAB1MI2,63,  pszTab1Mi2);
   WinLoadString(hab, NULLHANDLE, Offset+IDS_BOOK_TAB2MA, 63,  pszTab2Ma);
   WinLoadString(hab, NULLHANDLE, Offset+IDS_BOOK_TITLE , 19,  pszCatTitle);

}

/*********************************************************************
 *  Name : InitializeNotebook
 *
 *  Description : Set the intital pages for a notebook control.
 *
 *  Concepts : Called each time a demo NoteBook Control is
 *             initialized Sends a BKM_INSERTPAGE message to the
 *             notebook control to insert the first page into the
 *             control.  Sets the status line text for the first page
 *             by sending a BKM_SETSTATUSLINETEXT message.  Creates a
 *             window containing a bitmap and associates the whidow
 *             with the notebook by sending a BKM_SETPAGEWINDOWHWND
 *             message.  Repeats these steps with the execption of
 *             createing a MLE for use in the second notebook page.
 *
 *  API's : WinSendDlgItemMsg
 *          WinCreateWindow
 *          WinSendMsg
 *
 *  Parameters :  hwnd - Handle of the Notebook dialog
 *
 *  Returns: TRUE if The NoteBook is initialized successfully, FALSE otherwise
 *
 ****************************************************************/
BOOL InitializeNoteBook(HWND hwnd)
{
   HWND hwndPage;
   ULONG ulPageId;
   ULONG ipt = 0;
   CHAR pszMleBuffer[512];
   ULONG ChildId;
   USHORT usWidth, usHeight;
   ULONG myMR;

   BD_ATTR_MASK AttrMask;

   BOOKPAGEINFO     PageInfo;
   BOOKPAGEBIDIINFO bpBidiInfo;
/*
   {"Applications written for the Presentation Manager have full\
    access to the complete set of user interface tools: menus,\
    icons, scroll bars, etc., and often present a WYSIWYG\
    (what-you-see-is-what-you-get) view of their data.\
    Presentation Manager programs often make extensive use\
    of a mouse and display, and they have access to all the\
    32-bit features of version 2.0."};
*/
   ULONG             Cp = Bidi_QueryCp();
   LoadBookStrings(Cp);

   ChildId = IDC_NOTEBOOK;

//BIDI
//
// Sets the notebook window to be RtL with Visual/RtL text.
//
   WinSetLangInfo(WinWindowFromID(hwnd,ChildId),
            LI_BD_WND_ATTR,
            (BDA_INIT              |
             BDA_WND_ORIENT_RTL    |
             BDA_TEXT_ORIENT_RTL   |
             BDA_TEXTTYPE_VISUAL   ),
             BDAM_ALL, 0L, 0L);
   /*
    * Insert the first page.
    */
   ulPageId = (LONG)WinSendDlgItemMsg(hwnd, ChildId,
        BKM_INSERTPAGE, NULL,
        MPFROM2SHORT((BKA_STATUSTEXTON | BKA_AUTOPAGESIZE | BKA_MAJOR |
        BKA_MINOR), BKA_LAST));

   if ( !ulPageId)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
        BKM_SETSTATUSLINETEXT, MPFROMLONG(ulPageId),
      /*  MPFROMP("Page 1 of 3"))) */
       MPFROMP(pszStatusLine1)))
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
        BKM_SETTABTEXT, MPFROMLONG(ulPageId),
        MPFROMP("pszTab1Ma")))
     return FALSE;

//BIDI
// Initializes the bidi attributes of the minor tab.
// and points to it from the BookPageInfo.
//
   memset (&bpBidiInfo,0, sizeof(bpBidiInfo));
   memset (&PageInfo  ,0, sizeof(PageInfo));
   bpBidiInfo.bamMinorTab.ulBdAttr = BDA_TEXT_ORIENT_RTL |
                                     BDA_TEXTTYPE_VISUAL |
                                     BDA_INIT;
   bpBidiInfo.bamMinorTab.ulBdMask = BDAM_ALL;

   PageInfo.cb          = sizeof(PageInfo);
   PageInfo.fl          = BFA_MINORTABTEXT | BFA_BIDIINFO;
   PageInfo.cbMinorTab  = strlen(pszTab1Mi1);
   PageInfo.pszMinorTab = pszTab1Mi1;
   PageInfo.pBidiInfo   = (PVOID) &bpBidiInfo;


   if ( !WinSendDlgItemMsg(hwnd, ChildId,
        BKM_SETPAGEINFO, MPFROMLONG(ulPageId), MPFROMP(&PageInfo)))
      return FALSE;

   hwndPage =
   WinCreateWindow(               /* parent-window handle                    */
      hwnd,                       /* pointer to registered class name        */
      WC_STATIC,                  /* pointer to window text                  */
      "#6",                       /* window style                            */
      WS_VISIBLE | SS_BITMAP,     /* horizontal position of window           */
      0,                          /* vertical position of window             */
      0,                          /* window width                            */
      0,                          /* window height                           */
      0,                          /* owner-window handle                     */
      NULLHANDLE,                 /* handle to sibling window                */
      HWND_TOP,                   /* window identifier                       */
      0,                          /* pointer to buffer                       */
      NULL,                       /* pointer to structure with pres. params. */
      NULL);
//BIDI
//
// Sets the first page window to be RtL with Visual/RtL text.
//
   WinSetLangInfo(hwndPage,
            LI_BD_WND_ATTR,
            (BDA_INIT              |
             BDA_WND_ORIENT_RTL    |
             BDA_TEXT_ORIENT_RTL   |
             BDA_TEXTTYPE_VISUAL   ),
            BDAM_ALL, 0L, 0L);

   if (!hwndPage)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
         BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulPageId),
         MPFROMHWND(hwndPage)))
     return FALSE;

   /*
    * Insert the second page.
    */
    ulPageId = (LONG)WinSendDlgItemMsg(hwnd, ChildId,
         BKM_INSERTPAGE, NULL,
         MPFROM2SHORT((BKA_STATUSTEXTON | BKA_AUTOPAGESIZE | BKA_MAJOR),
         BKA_LAST));

   if (!ulPageId)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
         BKM_SETSTATUSLINETEXT, MPFROMLONG(ulPageId),
        /* MPFROMP("Page 2 of 3"))) */
       MPFROMP(pszStatusLine2)))
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
        BKM_SETTABTEXT, MPFROMLONG(ulPageId),
        MPFROMP("pszTab1Mi2")))
     return FALSE;

   AttrMask.ulBdAttr = BDA_TEXTTYPE_VISUAL | BDA_TEXT_ORIENT_RTL | BDA_INIT ;
   AttrMask.ulBdMask = BDAM_ALL;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
        BKM_SETTABTEXTBIDIATTR, MPFROMLONG(ulPageId),
        MPFROMP(&AttrMask)))
   return FALSE;


   hwndPage =
   WinCreateWindow(               /* parent-window handle                    */
      hwnd,                       /* pointer to registered class name        */
      WC_STATIC,                  /* pointer to window text                  */
      "#7",                       /* window style                            */
      WS_VISIBLE | SS_ICON,       /* horizontal position of window           */
      0,                          /* vertical position of window             */
      0,                          /* window width                            */
      0,                          /* window height                           */
      0,                          /* owner-window handle                     */
      NULLHANDLE,                 /* handle to sibling window                */
      HWND_TOP,                   /* window identifier                       */
      0,                          /* pointer to buffer                       */
      NULL,                       /* pointer to structure with pres. params. */
      NULL);
//BIDI
//
// Sets the second page window to be RtL with Visual/RtL text.
//
   WinSetLangInfo(hwndPage,
            LI_BD_WND_ATTR,
            (BDA_INIT              |
             BDA_WND_ORIENT_RTL    |
             BDA_TEXT_ORIENT_RTL   |
             BDA_TEXTTYPE_VISUAL   ),
            BDAM_ALL, 0L, 0L);

   if (!hwndPage)
     return FALSE;

   if( !WinSendDlgItemMsg(hwnd, ChildId,
         BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulPageId),
         MPFROMHWND(hwndPage)))
     return FALSE;

   /*
    * Insert the third page.
    */
    ulPageId = (LONG)WinSendDlgItemMsg(hwnd, ChildId,
         BKM_INSERTPAGE, NULL,
         MPFROM2SHORT((BKA_STATUSTEXTON | BKA_AUTOPAGESIZE | BKA_MAJOR),
         BKA_LAST));

   if (!ulPageId)
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
         BKM_SETSTATUSLINETEXT, MPFROMLONG(ulPageId),
        /* MPFROMP("Page 3 of 3"))) */
       MPFROMP(pszStatusLine3)))
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
        BKM_SETTABTEXT, MPFROMLONG(ulPageId),
        MPFROMP(pszTab2Ma)))
     return FALSE;


   // The tab text will be Visual/LtR

   AttrMask.ulBdAttr = BDA_TEXTTYPE_VISUAL | BDA_TEXT_ORIENT_LTR | BDA_INIT ;
   AttrMask.ulBdMask = BDAM_ALL;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
        BKM_SETTABTEXTBIDIATTR, MPFROMLONG(ulPageId),
        MPFROMP(&AttrMask)))
   return FALSE;

   hwndPage =
   WinCreateWindow(
      hwnd,                       /* parent-window handle                    */
      WC_MLE,                     /* pointer to registered class name        */
      NULL,                       /* pointer to window text                  */
      WS_VISIBLE | MLS_WORDWRAP | /* window style                            */
         MLS_READONLY,
      0,                          /* horizontal position of window           */
      0,                          /* vertical position of window             */
      0,                          /* window width                            */
      0,                          /* window height                           */
      NULLHANDLE,                 /* owner-window handle                     */
      HWND_TOP,                   /* handle to sibling window                */
      0,                          /* window identifier                       */
      NULL,                       /* pointer to buffer                       */
      NULL);                      /* pointer to structure with pres. params. */
//BIDI
//
// Sets the first page window to be RtL with Visual/RtL text.
// (This is for the text that is displayed in the page itself).
//
   WinSetLangInfo(hwndPage,
            LI_BD_WND_ATTR,
            (BDA_INIT              |
             BDA_WND_ORIENT_RTL    |
             BDA_TEXT_ORIENT_RTL   |
             BDA_TEXTTYPE_VISUAL   ),
            BDAM_ALL, 0L, 0L);

   if (!hwndPage)
     return FALSE;

   if ( !WinSendMsg(hwndPage, MLM_SETIMPORTEXPORT,
         MPFROMP(pszMleBuffer),
         MPFROMSHORT(sizeof(pszMleBuffer))))
     return FALSE;

     memset(pszMleBuffer,'\0',sizeof(pszMleBuffer));
     strcpy(pszMleBuffer, pszNoteBookText);

   if ( !WinSendMsg(hwndPage, MLM_IMPORT, &ipt,
         MPFROMSHORT(sizeof(pszMleBuffer))))
     return FALSE;

   if( !WinSendDlgItemMsg(hwnd, ChildId,
         BKM_SETPAGEWINDOWHWND, MPFROMLONG(ulPageId),
         MPFROMHWND(hwndPage)))
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
        BKM_SETDIMENSIONS,
        MPFROM2SHORT(TAB_WIDTH,TAB_HEIGHT),
        MPFROMSHORT(BKA_MAJORTAB)))
     return FALSE;

   if ( !WinSendDlgItemMsg(hwnd, ChildId,
        BKM_SETDIMENSIONS,
        MPFROM2SHORT(TAB_WIDTH,TAB_HEIGHT),
        MPFROMSHORT(BKA_MINORTAB)))
     return FALSE;

   return TRUE;
}                                       /* End of InitializeNotebook    */

struct _IDS_SZ
{
   ULONG ids;
   PSZ   psz;
}  ids_sz[] =

   {
      {IDS_CNR_TITLE,         pszCnrTitleNational},
      {IDS_CNR_COLUMNTITLE,   pszColumnText2},
      {IDS_CNR_PICTURE,       pszColumnText1National},
      {IDS_CNR_TOWER,         pszTowerNational},
      {IDS_CNR_WATERFALL,     pszWaterfallNational},
      {IDS_CNR_GLOBE,         pszGlobeNational},
      {IDS_CNR_KEY,           pszKeyNational}
   };

VOID LoadCnrStrings(ULONG Cp)
{
   LONG ul, idx ;
   ULONG    Offset = ARA_OFF;
   for (ul = 0; ul < sizeof (ids_sz) / sizeof (ids_sz[0]); ul++)
   {
     idx = ids_sz[ul].ids + Offset;
     WinLoadString(hab, NULLHANDLE, idx, 63L, ids_sz[ul].psz);
   }

   return;
}

/*********************************************************************
 *  Name : InitializeContainer
 *
 *  Description : Initialize and insert a record into the container.
 *
 *
 *  Concepts : Called each time a demo Container Control is initialized.
 *             Allocates and inserts a user record into a container.
 *             Allocates and inserst  details view information.
 *             Sets the containers info.
 *
 *  API's : WinLoadPointer
 *          WinSendDlgItemMsg
 *
 *  Parameters : hwnd - Handle of the container dialog.
 *
 *  Returns: TRUE if The Container is initialized successfully, FALSE otherwise
 *
 ****************************************************************/
BOOL InitializeContainer(HWND hwnd)
{

  USHORT            nRecords = 4 /*1*/;
  HPOINTER          hptrTower, hptrWaterfall, hptrGlobe, hptrKey;
  ULONG             cbRecordData;
  ULONG             Cp = Bidi_QueryCp();

  LoadCnrStrings(Cp);

//BIDI
//
// Sets the container window to RtL window orientation, and Implicit text
// with contextual text orientation.
//
  WinSetLangInfo(WinWindowFromID(hwnd,IDC_CONTAINER),
            LI_BD_WND_ATTR,
            (BDA_WND_ORIENT_RTL      |
             BDA_TEXTTYPE_IMPLICIT   |
             BDA_TEXT_ORIENT_CONTEXT |
             BDA_INIT),
            BDAM_ALL, 0L, 0L);

/*
  hptr = WinLoadPointer(HWND_DESKTOP, 0, IDR_MAIN);
*/
  hptrTower = WinLoadPointer(HWND_DESKTOP, 0, IDR_TOWER);
  hptrWaterfall = WinLoadPointer(HWND_DESKTOP, 0, IDR_WATERFALL);
  hptrGlobe = WinLoadPointer(HWND_DESKTOP, 0, IDR_GLOBE);
  hptrKey = WinLoadPointer(HWND_DESKTOP, 0, IDR_KEY);

  cbRecordData = (LONG) (sizeof(USERRECORD) - sizeof(RECORDCORE));
  pUserRecord = WinSendDlgItemMsg(hwnd,IDC_CONTAINER,
                   CM_ALLOCRECORD, MPFROMLONG(cbRecordData) ,
                   MPFROMSHORT(nRecords));
  pStartRecord = pUserRecord;

  pUserRecord->recordCore.cb = sizeof(RECORDCORE);              /*RBS*/
  pUserRecord->recordCore.pszText = pszTowerNational;
  pUserRecord->recordCore.pszIcon = pszTowerNational;
  pUserRecord->recordCore.pszName = pszTowerNational;
  pUserRecord->recordCore.hptrIcon = hptrTower;

/*
  pUserRecord->date.day = 11;
  pUserRecord->date.month = 11;
  pUserRecord->date.year = 11;
  pUserRecord->time.seconds    = 12;
  pUserRecord->time.minutes    = 12;
  pUserRecord->time.hours      = 12;
  pUserRecord->recordData = (PSZ)pszSampleData;
*/
  pUserRecord->EnglishName = pszTowerEnglish;
  pUserRecord->NationalName = pszTowerNational;

  pUserRecord = (PUSERRECORD) pUserRecord->recordCore.preccNextRecord;
  pUserRecord->recordCore.cb = sizeof(RECORDCORE);              /*RBS*/
  pUserRecord->recordCore.pszText = pszWaterfallNational;
  pUserRecord->recordCore.pszIcon = pszWaterfallNational;
  pUserRecord->recordCore.pszName = pszWaterfallNational;
  pUserRecord->recordCore.hptrIcon = hptrWaterfall;

/*
  pUserRecord->date.day = 11;
  pUserRecord->date.month = 11;
  pUserRecord->date.year = 11;
  pUserRecord->time.seconds    = 12;
  pUserRecord->time.minutes    = 12;
  pUserRecord->time.hours      = 12;
  pUserRecord->recordData = (PSZ)pszSampleData;
*/
  pUserRecord->EnglishName = pszWaterfallEnglish;
  pUserRecord->NationalName = pszWaterfallNational;

  pUserRecord = (PUSERRECORD) pUserRecord->recordCore.preccNextRecord;
  pUserRecord->recordCore.cb = sizeof(RECORDCORE);              /*RBS*/
  pUserRecord->recordCore.pszText = pszGlobeNational;
  pUserRecord->recordCore.pszIcon = pszGlobeNational;
  pUserRecord->recordCore.pszName = pszGlobeNational;
  pUserRecord->recordCore.hptrIcon = hptrGlobe;

/*
  pUserRecord->date.day = 11;
  pUserRecord->date.month = 11;
  pUserRecord->date.year = 11;
  pUserRecord->time.seconds    = 12;
  pUserRecord->time.minutes    = 12;
  pUserRecord->time.hours      = 12;
  pUserRecord->recordData = (PSZ)pszSampleData;
*/
  pUserRecord->EnglishName = pszGlobeEnglish;
  pUserRecord->NationalName = pszGlobeNational;

  pUserRecord = (PUSERRECORD) pUserRecord->recordCore.preccNextRecord;
  pUserRecord->recordCore.cb = sizeof(RECORDCORE);              /*RBS*/
  pUserRecord->recordCore.pszText = pszKeyNational;
  pUserRecord->recordCore.pszIcon = pszKeyNational;
  pUserRecord->recordCore.pszName = pszKeyNational;
  pUserRecord->recordCore.hptrIcon = hptrKey;

/*
  pUserRecord->date.day = 11;
  pUserRecord->date.month = 11;
  pUserRecord->date.year = 11;
  pUserRecord->time.seconds    = 12;
  pUserRecord->time.minutes    = 12;
  pUserRecord->time.hours      = 12;
  pUserRecord->recordData = (PSZ)pszSampleData;
*/
  pUserRecord->EnglishName = pszKeyEnglish;
  pUserRecord->NationalName = pszKeyNational;


  recordInsert.cb = sizeof(RECORDINSERT);                   /*RBS*/
  recordInsert.pRecordParent= NULL;
  recordInsert.pRecordOrder = (PRECORDCORE)CMA_END;
  recordInsert.zOrder = (USHORT)CMA_TOP;
  recordInsert.cRecordsInsert=(USHORT)4 /*1*/;
  recordInsert.fInvalidateRecord = TRUE;

  WinSendDlgItemMsg(hwnd,IDC_CONTAINER, CM_INSERTRECORD,
                   (PRECORDCORE)pStartRecord, &recordInsert);

  pFieldInfo = WinSendDlgItemMsg(hwnd, IDC_CONTAINER,
                        CM_ALLOCDETAILFIELDINFO,MPFROMLONG(3), NULL);

  firstFieldInfo = pFieldInfo;

  pFieldInfo->cb = sizeof(FIELDINFO);                       /*RBS*/
/*
  pFieldInfo->flData = CFA_STRING | CFA_HORZSEPARATOR | CFA_CENTER |
                         CFA_SEPARATOR;
*/
  pFieldInfo->flData = CFA_BITMAPORICON | CFA_HORZSEPARATOR | CFA_CENTER |
                         CFA_SEPARATOR;
  pFieldInfo->flTitle = CFA_CENTER;
  pFieldInfo->pTitleData = (PVOID) pszColumnText1National;
  pFieldInfo->offStruct = FIELDOFFSET(RECORDCORE, hptrIcon);
  pFieldInfo = pFieldInfo->pNextFieldInfo;

  pFieldInfo->cb = sizeof(FIELDINFO);                       /*RBS*/
/*
  pFieldInfo->flData = CFA_DATE | CFA_HORZSEPARATOR | CFA_CENTER |
                         CFA_SEPARATOR;
*/
  pFieldInfo->flData = CFA_STRING | CFA_HORZSEPARATOR | CFA_RIGHT |
                         CFA_SEPARATOR;
  pFieldInfo->flTitle = CFA_CENTER;
  pFieldInfo->pTitleData =  (PVOID) pszColumnText2;
  pFieldInfo->offStruct = FIELDOFFSET(USERRECORD,NationalName);
  pFieldInfo = pFieldInfo->pNextFieldInfo;

  pFieldInfo->cb = sizeof(FIELDINFO);                       /*RBS*/
/*
  pFieldInfo->flData = CFA_TIME | CFA_HORZSEPARATOR | CFA_CENTER |
                         CFA_SEPARATOR;
*/
  pFieldInfo->flData = CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT |
                         CFA_SEPARATOR;
  pFieldInfo->flTitle = CFA_CENTER;
  pFieldInfo->pTitleData = (PVOID) pszColumnText3;
  pFieldInfo->offStruct = FIELDOFFSET(USERRECORD,EnglishName);


  fieldInfoInsert.cb = (ULONG)(sizeof(FIELDINFOINSERT));    /*RBS*/
  fieldInfoInsert.pFieldInfoOrder = (PFIELDINFO)CMA_FIRST;
  fieldInfoInsert.cFieldInfoInsert = (SHORT) 3;
  fieldInfoInsert.fInvalidateFieldInfo = TRUE;              /*RBS*/

  pFieldInfoInsert = &fieldInfoInsert;

  WinSendDlgItemMsg(hwnd,IDC_CONTAINER, CM_INSERTDETAILFIELDINFO,
                    MPFROMP(firstFieldInfo),
                    MPFROMP(pFieldInfoInsert));
//BIDI
//
// Set the fields (columns) bidi attributes.
//
  {
     BD_ATTR_MASK attr_mask;
     attr_mask.ulBdAttr = BDA_INIT                |
                          BDA_WND_ORIENT_RTL      |
                          BDA_TEXT_ORIENT_CONTEXT |
                          BDA_TEXTTYPE_IMPLICIT ;

     attr_mask.ulBdMask = BDAM_INIT             |
                          BDAM_WND_ORIENTATION  |
                          BDAM_TEXT_ORIENTATION |
                          BDAM_TEXTTYPE ;

     WinSendDlgItemMsg(hwnd,IDC_CONTAINER, CM_SETFIELDBIDIATTR,
                        MPFROMP(firstFieldInfo),
                        MPFROMP(&attr_mask));
     WinSendDlgItemMsg(hwnd,IDC_CONTAINER, CM_SETFIELDBIDIATTR,
                        MPFROMP(firstFieldInfo->pNextFieldInfo),
                        MPFROMP(&attr_mask));

     attr_mask.ulBdAttr = BDA_INIT                |
                          BDA_WND_ORIENT_LTR      |
                          BDA_TEXT_ORIENT_CONTEXT |
                          BDA_TEXTTYPE_IMPLICIT ;

     attr_mask.ulBdMask = BDAM_INIT             |
                          BDAM_WND_ORIENTATION  |
                          BDAM_TEXT_ORIENTATION |
                          BDAM_TEXTTYPE ;
     WinSendDlgItemMsg(hwnd,IDC_CONTAINER, CM_SETFIELDBIDIATTR,
                    MPFROMP((firstFieldInfo->pNextFieldInfo)->pNextFieldInfo),
                        MPFROMP(&attr_mask));
  }

  WinSendDlgItemMsg(hwnd,IDC_CONTAINER,CM_SETCNRINFO,&cnrinfo,
                     MPFROMLONG(CMA_FLWINDOWATTR | CMA_CNRTITLE));

return TRUE;
}                                       /* End of InitializeContainer    */

/***********************************************************
 * Name         : SetSysMenu
 *
 * Description  : Procedure to remove unselectable items from
 *                the window system menu.
 *
 * Concepts     : This routine determines the number of items
 *                in the system menu, loops through those
 *                items removing disabled menu items and menu
 *                seperators.
 *
 * API's        : WinMessageBox
 *
 * Parameters   : None
 *
 * Return       : None
 *
 **************************************************************/
VOID SetSysMenu(HWND hwndDlg)
{
  SHORT sMenuItems;
  USHORT usItemid;
  MENUITEM menuItem;
  HWND  hwndSubMenu;

  /* Determine the definition of the system menu */
  memset( &menuItem, 0, sizeof(menuItem));
  WinSendDlgItemMsg(hwndDlg,FID_SYSMENU, MM_QUERYITEM,
                    MPFROM2SHORT((SHORT)SC_SYSMENU,FALSE),
                    MPFROMP(&menuItem));

  hwndSubMenu = menuItem.hwndSubMenu;

  /* Find the number of items in the in the submenu */
  sMenuItems = (SHORT)WinSendMsg(hwndSubMenu,
                                 MM_QUERYITEMCOUNT,NULL,NULL);
  /*
   * Loop through the submenu items and remove disabled
   * menu items and menu separators.
   */
  for (sMenuItems - 1 ; sMenuItems >= 0 ;sMenuItems-- )
  {
    /* Find the item ID for the current position. */
    usItemid = (USHORT)WinSendMsg(hwndSubMenu,MM_ITEMIDFROMPOSITION,
                         MPFROMSHORT(sMenuItems), NULL);

    /* Query the definition of the current item*/
    WinSendMsg(hwndSubMenu, MM_QUERYITEM,
               MPFROM2SHORT(usItemid,FALSE), MPFROMP(&menuItem));

   /*
    * If the menu item is disabled or the item has a style
    * of MIS_SEPARATOR - delete it.
    */
    if ((BOOL)(WinSendMsg(hwndSubMenu, MM_QUERYITEMATTR,
          MPFROM2SHORT(usItemid,FALSE), (MPARAM)MIA_DISABLED)) ||
          (menuItem.afStyle & MIS_SEPARATOR))
    {
      WinSendMsg(hwndSubMenu, MM_DELETEITEM,
                MPFROM2SHORT(usItemid,FALSE),NULL);
    }
  }
}                                       /* End of SetSysMenu()           */



/***********************************************************
 * Name         : ReportTestResult
 *
 * Description  : Reports to the user the result of a test.
 *
 **************************************************************/
VOID ReportTestResult(HWND hwnd, BOOL fResult)
{
   MessageBox ( hwnd,
                fResult ? IDMSG_TEST_SUCCESSFUL : IDMSG_TEST_ERROR,
                MB_OK | MB_MOVEABLE,
                !fResult
              );
   return;
}  /*ReportTestResult() */



