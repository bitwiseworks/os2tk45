/*************************************************************************
*
*  File Name   : Teldir.c
*
*  Description : This PM sample application demonstates the
*                Arabic PM programming interface.
*
*  Concepts    : PM window creation
*                Menu Creation
*                Dialog box creation
*                Initizalition and display of PM controls
*                Error messages creation
*
*  API's       :
*  -------------
*  DosBeep                   WinDispatchMsg              WinQueryWindowText
*  WinBeginPaint             WinEnableWindowUpdate       WinRegisterClass
*  WinCreateMsgQueue         WinEndPaint                 WinSendMsg
*  WinCreateStdWindow        WinFillRect                 WinSetFocus
*  WinCreateWindow           WinGetMsg                   WinSetLangInfo
*  WinDefDlgProc             WinInitialize               WinSetWindowPos
*  WinDefWindowProc          WinLoadMenu                 WinSetWindowText
*  WinDestroyMsgQueue        WinLoadString               WinTerminate
*  WinDestroyWindow          WinMessageBox               WinUpper
*  WinDismissDlg             WinPostMsg                  WinWindowFromID
*
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
/******************************************************************************/
/* A sample PM application to demonstrate the Arabic PM programming interface */
/*                                                                            */
/* The application is a telephone directory that the user can use to save the */
/* person name and his telephone number. He can then search for a specific    */
/* name, delete a name, add a name or update it.                              */
/* The list of names are displayed in a listbox. All the action that can be   */
/* performed on the list of names are shown as radio buttons. The user choose */
/* the action he would like to take by clicking of the appropriate radio      */
/* button, then to perform it, he has to click on the push button at the      */
/* buttom of the panel.                                                       */
/* The name and telephone number are entered through two entry fields.        */
/*                                                                            */
/* To fully understand this program, the reader is assumed to be familiar     */
/* with the PM programming.                                                   */
/******************************************************************************/

#define INCL_WIN
#define INCL_WINWINDOWMGR

#include   <os2.h>
#include   <pmbidi.h>
#include   <layout.h>
                                  /* Standard C library routine include files */
#include   <string.h>
#include   <io.h>
#include   <stdio.h>
#include   <stdlib.h>

#include   "teldir.h"                  /* Include the header file             */

                                       /* Global Declarations                 */
#define       Eng_Offset  100         /* Offset of English strings in the */
                                      /* string table in teldir.rc */

#define       MESSAGELEN  80          /* Length of string used in reading */
                                      /* messages from teldir.rc */

HWND          hwndFrame,              /* handle to the main frame window */
              hwndStatic1,            /* handle to the 1st static text */
              hwndStatic2,            /* handle to the 2nd static text */
              hwndStatic3,            /* handle to the 3rd static text */
              hwndStatic4,            /* handle to the 4th static text */
              hwndListBox,            /* handle to the list box */
              hwndGroupBox,           /* handle to the group box */
              hwndRadioButton1,       /* handles to the radio buttons 1..4 */
              hwndRadioButton2,
              hwndRadioButton3,
              hwndRadioButton4,
              hwndName,               /* handle to entry field for user */
                                      /* entery of the name */
              hwndTel,                /* handle to entry field for user */
                                      /* entry of the telephone number */
              hwndPushButton,         /* handle to push button */
              hWndObject;             /* handle to object window */
HAB           hab;                    /* anchor block for the process */
HMQ           hmq;                    /* handle to the process' message queue */
ULONG         flcreate1;              /* Creation flags */
HWND          hwndc1;                 /* handle to window */
int           Count,                  /* Counter */
              Sel;                    /* The selected button's number */
BOOL          Delete  = FALSE,        /* Flag to indicate that deletion is to */
                                      /* take place */
              Search  = FALSE;        /* Flag to indicate that searching is to */
                                      /* take place */
int           Position,               /* Indicates whether sorting is Ascending*/
                                      /* or Descending */
              Index,                  /* Index of the selected item */
              ItemsLeft;              /* ItemsLeft in List Box */
BOOL          Found;                  /* Indicates whether item was found in */
                                      /* List Box or not */
CHAR          NameStr[100],           /* Arrray to hold the name */
              Temp_Name[100];         /* Arrray to hold the name queried from */
                                      /* entry filed */
CHAR          TelStr[10],             /* Array to hold the telephone number */
              Temp_Tel[10];           /* Arrray to hold the Tel queried from */
                                      /* entry filed */
BOOL          Arb_Layer=TRUE;         /* Bool indicating which layer is active*/
                                      /* TRUE means it is Arabic. FALSE means */
                                      /* it is ENGLISH */
CHAR          DisplayText[MESSAGELEN]; /* Array for holding the message's text */



TELLIST       TelDir[MAX_NAMES];     /* Array to hold names and tel numbers */


BOOL   fHelpEnabled;             /* flag to determine if help is enabled */
static CHAR szLibName[HELPLIBRARYNAMELEN];
static CHAR szWindowTitle[HELPLIBRARYNAMELEN];
static HWND hwndHelpInstance;
/****************************************************************
 *  Name:   main()
 *
 *  Description: Entry point of program.
 *
 *  Concepts: Obtains anchor block handle and creates message
 *            queue.  Calls the initialization routine.
 *            Creates the main frame window which creates the
 *            main client window.  Polls the message queue
 *            via Get/Dispatch Msg loop.  Upon exiting the
 *            loop, exits.
 *
 *  API's   :  WinInitialize
 *             DosBeep
 *             WinCreateMsgQueue
 *             WinTerminate
 *             WinCreateStdWindow
 *             WinSetWindowText
 *             WinGetMsg
 *             WinDispatchMsg
 *
 *  Parameters: NONE
 *
 *  Returns: VOID
 *
\****************************************************************/
int  main (VOID)
{
    QMSG  qmsg;

    hab=WinInitialize(0L);                       /* initialization */
                                                 /* create a message queue */
    hmq=WinCreateMsgQueue(hab,0);
                                           /* Create an object window to   */
                                           /* be the owner of the message  */
                                           /* boxes. They will inherit its */
                                           /* bidi attributes */
    hWndObject = WinCreateWindow(HWND_OBJECT,WC_BUTTON,0L,
                                 BS_AUTORADIOBUTTON, 0,0,0,0,0L,HWND_TOP,
                                 0L,NULL,NULL);

                                      /* Register the class for the client */
    WinRegisterClass(hab,"Window1",WinProc1,CS_SIZEREDRAW,0);

    flcreate1= FCF_BORDER | FCF_MINBUTTON | FCF_SYSMENU | FCF_ICON |
               FCF_TITLEBAR | FCF_MENU | FCF_TASKLIST | FCF_ACCELTABLE;

    hwndFrame = WinCreateStdWindow(HWND_DESKTOP,
                                   WS_VISIBLE,&flcreate1,"Window1","",0L,
                                   0L,IDM_ORGMENU,(PHWND)&hwndc1);


    /************************* The Arabic PM section *************************/
                                          /* Load Arabic interface initially */
    Arb_Interface();

                                            /* The system menu has the same */
                                   /* attributes in both Arabic and English */
                                              /* since it is not translated */

    WinSetLangInfo(WinWindowFromID(hwndFrame, FID_SYSMENU), LI_BD_WND_ATTR,
                   BDA_NUMERALS_PASSTHRU | BDA_WND_ORIENT_LTR |
                   BDA_TEXT_ORIENT_LTR,
                   BDAM_NUMERALS | BDAM_WND_ORIENTATION |
                   BDAM_TEXT_ORIENTATION,
                   0L, 0L);

    /****************** End of Arabic PM APIs section ************************/

    LoadFile();                                  /* fill the list box from */
                                                 /* a file on disk         */

                                                /* Adjust the window position*/
    WinSetWindowPos(hwndFrame,HWND_BOTTOM,7,80,498,385,
                          SWP_MOVE | SWP_SIZE | SWP_ACTIVATE);

    WinSendMsg (hwndRadioButton1, BM_SETCHECK, MPFROMSHORT ((USHORT) 1), 0L);

    WinSetFocus (HWND_DESKTOP, hwndName);          /* Set focus on the name */
                                                             /* entry field */

                                                /* Set the check mark on the */
                                                /* Ascending menu item */
    WinSendMsg ( WinWindowFromID (hwndFrame, FID_MENU),
                 MM_SETITEMATTR,
                 MPFROM2SHORT ((USHORT) IDM_ASCENDING, (BOOL) TRUE),
                 MPFROM2SHORT ((USHORT) MIA_CHECKED, (BOOL) MIA_CHECKED));

                                                /* Reset the check mark on   */
                                                /* the decending menu item */
    WinSendMsg ( WinWindowFromID (hwndFrame, FID_MENU),
                 MM_SETITEMATTR,
                 MPFROM2SHORT ((USHORT) IDM_DESCENDING, (BOOL) TRUE),
                 MPFROM2SHORT ((USHORT) MIA_CHECKED, (BOOL) FALSE));
    while ( WinGetMsg(hab,(PQMSG)&qmsg, (HWND)NULL,0,0))
           WinDispatchMsg( hab, (PQMSG)&qmsg );

    DestroyHelpInstance();
    WinDestroyWindow(hwndFrame);
    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );
    return 0;
}    /* End of main */

/****************************************************************
 *  Name:   SortList
 *
 *  Description : deletes all items in the names listbox, then inserts
 *                them again either in acending or descending order
 *
 *  Concepts : Sends an LM_DELETEALL message to the list box
 *             to delete all its items.  It then sends an
 *             LM_INSERTITEM message to the same list box to insert
 *             the items according to the value of Position (ascending
 *             or desecnding).
 *
 *  API's : WinSendMsg
 *
 * Parameters   : Position - indicateds whether sorting is
 *                           ascending or descending.
 *
 *  Returns:  VOID
 *
 ****************************************************************/
VOID SortList (int Position)
{
   int i;

   WinSendMsg (hwndListBox, LM_DELETEALL, NULL, NULL);
                                            /* Delete all list box items */
   i=0;
   while (i<Count)
   {
      if(strcmp(TelDir[i].Name, "") != 0)
      {
         WinSendMsg (hwndListBox, LM_INSERTITEM,   /* Insert items one by one */
                     MPFROMSHORT ((SHORT) Position),
                     MPFROMP ((PSZ) TelDir[i++].Name));
      }
      else i++;
   }/* endwhile */
}    /* End of SortList */

/****************************************************************
 *  Name:   WinProc1
 *
 *  Description : Window procedure for the main clent window.
 *
 *  Concepts : Processes the messages sent to the main client
 *             window.  This routine processes the basic
 *             messages all client windows should process.
 *
 *  API's : WinBeginPaint
 *          WinCreateWindow
 *          WinDefWindowProc
 *          WinEndPaint
 *          WinFillRect
 *          WinMessageBox
 *          WinPostMsg
 *          WinSendMsg
 *          WinSetWindowText
 *          WinWindowFromID
 *
 * Parameters   : hwnd - Window handle to which message is addressed
 *                msg - Message type
 *                mp1 - First message parameter
 *                mp2 - Second message parameter
 *
 *  Returns:  Return values are determined by each message
 *
 ****************************************************************/

MRESULT EXPENTRY WinProc1(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
    HPS          hps;
    RECTL        rc;
    USHORT       Answer;
    CHAR         Name[100];
    CHAR         stext[75];
    USHORT       i;
    HWND         hwndFocus;
    USHORT       ControlID;

   switch( msg )
    {
        case WM_CREATE:
            Count = 0;
            Sel = 1;
                                                /* Create child controls */

                                                 /* Text : List of names */
            hwndStatic1=WinCreateWindow(hwnd,WC_STATIC, Message(ID_NAMES_A),
                  SS_TEXT | DT_LEFT | WS_VISIBLE,206,305,270,
                  16,hwnd,HWND_TOP, ID_NAMES_LIST,NULL,NULL);

                                                              /* Listbox */
            hwndListBox=WinCreateWindow(hwnd,WC_LISTBOX,"",
                   WS_CLIPCHILDREN | WS_PARENTCLIP |
                   WS_SYNCPAINT | WS_VISIBLE | WS_TABSTOP ,
                   206,160,270,140,hwnd, HWND_TOP, ID_LISTBOX,NULL,NULL);

                                        /* Radiobuttons for add, delete */
                                                     /* modify and find */
            hwndRadioButton1=WinCreateWindow(hwnd,WC_BUTTON,Message(ID_ADD_A),
                   BS_AUTORADIOBUTTON | WS_PARENTCLIP |
                   WS_TABSTOP | WS_SYNCPAINT | WS_VISIBLE,
                   60,250,85,25,hwnd,HWND_TOP,
                   ID_ADDNAME,NULL,NULL);

            hwndRadioButton2=WinCreateWindow(hwnd,WC_BUTTON,Message(ID_REMOVE_A),
                   BS_AUTORADIOBUTTON | WS_TABSTOP | WS_VISIBLE,
                   60,225,85,25,hwnd,HWND_TOP,
                   ID_DELETE,NULL,NULL);

            hwndRadioButton3=WinCreateWindow(hwnd,WC_BUTTON,Message(ID_MODIFY_A),
                   BS_AUTORADIOBUTTON | WS_TABSTOP | WS_VISIBLE,
                   60,200,85,25,hwnd,HWND_TOP,
                  ID_UPDATE,NULL,NULL);

            hwndRadioButton4=WinCreateWindow(hwnd,WC_BUTTON,Message(ID_FIND_A),
                   BS_AUTORADIOBUTTON | WS_TABSTOP | WS_VISIBLE,
                   60,175,85,25,hwnd,HWND_TOP,
                   ID_SEARCH,NULL,NULL);

                                                      /* Groupbox : Choices */
            hwndGroupBox=WinCreateWindow(hwnd,WC_STATIC,Message(ID_CHOICE_A),
                  SS_GROUPBOX | WS_PARENTCLIP |
                  WS_SYNCPAINT | WS_VISIBLE,39,160,120,140,hwnd,HWND_TOP,
                  ID_CHOICES,NULL,NULL);

                                                         /* Name entryfield */
            hwndName=WinCreateWindow(hwnd,WC_ENTRYFIELD,"",
                   ES_MARGIN | ES_AUTOSCROLL | WS_TABSTOP |
                   WS_VISIBLE,209,99,264,18,hwnd,HWND_TOP,
                   ID_NAME,NULL,NULL);

                                                   /* Telephone entryfield */
            hwndTel=WinCreateWindow(hwnd,WC_ENTRYFIELD,"",
                   ES_MARGIN | ES_AUTOSCROLL | WS_PARENTCLIP | WS_TABSTOP |
                   ES_RIGHT | WS_SYNCPAINT | WS_VISIBLE,43,98,120,19,
                   hwnd,HWND_TOP, ID_TELEPHONE,NULL,NULL);

                                                   /* Text : Name */
            hwndStatic3=WinCreateWindow(hwnd,WC_STATIC,Message(ID_ENTERNAME_A),
                  SS_TEXT | DT_LEFT | WS_VISIBLE,
                  209,128,264,20,hwnd,HWND_TOP,
                  ID_INPUT_NAME,NULL,NULL);

                                                      /* Text : Telephone */
            hwndStatic4=WinCreateWindow(hwnd,WC_STATIC,Message(ID_TELNUMBER_A),
                  SS_TEXT | DT_LEFT | WS_VISIBLE,43,128,125,21,hwnd,HWND_TOP,
                  ID_TELEPHONE_NUMBER,NULL,NULL);

                                                             /* OK button */
            hwndPushButton=WinCreateWindow(hwnd,WC_BUTTON,Message(ID_DO_A),
                   BS_PUSHBUTTON | WS_PARENTCLIP | WS_TABSTOP |
                   WS_SYNCPAINT | WS_VISIBLE,38,21,439,29,hwnd,HWND_TOP,
                  IDM_OK,NULL,NULL);
            break;

        case  WM_COMMAND:
            switch(SHORT1FROMMP( mp1 ))
            {
              case IDM_SAVE:     /* Save */
                 SaveData();
                 break;

              case IDM_QUIT:     /* Quit */
                 Message(ID_EXIT_A);
                 strcpy(stext,DisplayText);
                 Answer = WinMessageBox(HWND_DESKTOP,
                                        (HWND)hWndObject,
                                        Message(ID_SAVEFILE_A),
                                        stext,
                                        0,
                                        MB_YESNO);

                 if(Answer == MBID_YES)
                    SaveData();
                 WinPostMsg (hwnd, WM_QUIT, NULL, NULL);
                 break;

              case IDM_ENG:
                 if (Arb_Layer)
                    ChangeLayer();
                 break;

              case IDM_ARB:
                 if (!Arb_Layer)
                    ChangeLayer();
                 break;


              case IDM_HELPEXTENDED:
                                         /* display the help contents */
                  if (fHelpEnabled)
                  {
                    WinSendMsg(hwndHelpInstance, HM_HELP_CONTENTS, NULL, NULL);
                  } /* endif */
                 break;


              case IDM_ABOUT:     /* About ... */
                     MessageBox(ID_USES_A, ID_INFORMATION_A);
                  break;

              case IDM_ASCENDING:                  /* Sort Ascending */
                 WinSendMsg ( WinWindowFromID (hwndFrame, FID_MENU),
                              MM_SETITEMATTR,
                              MPFROM2SHORT ((USHORT) IDM_ASCENDING,
                                            (BOOL) TRUE),
                              MPFROM2SHORT ((USHORT) MIA_CHECKED,
                                            (BOOL) MIA_CHECKED));
                 WinSendMsg ( WinWindowFromID (hwndFrame, FID_MENU),
                              MM_SETITEMATTR,
                              MPFROM2SHORT ((USHORT) IDM_DESCENDING,
                                            (BOOL) TRUE),
                              MPFROM2SHORT ((USHORT) MIA_CHECKED,
                                            (BOOL) FALSE));
                 SortList (LIT_SORTASCENDING);
                 break;

              case IDM_DESCENDING:              /* Sort Descending */
                 WinSendMsg ( WinWindowFromID (hwndFrame, FID_MENU),
                              MM_SETITEMATTR,
                              MPFROM2SHORT ((USHORT) IDM_DESCENDING,
                                            (BOOL) TRUE),
                              MPFROM2SHORT ((USHORT) MIA_CHECKED,
                                            (BOOL) MIA_CHECKED));
                 WinSendMsg ( WinWindowFromID (hwndFrame, FID_MENU),
                              MM_SETITEMATTR,
                              MPFROM2SHORT ((USHORT) IDM_ASCENDING,
                                            (BOOL) TRUE),
                              MPFROM2SHORT ((USHORT) MIA_CHECKED,
                                            (BOOL) FALSE));
                 SortList (LIT_SORTDESCENDING);
                 break;

              case IDM_OK:             /* Perform required action */
                 Button_Handling();
                 break;

             } /* endswitch */
             break;

        case WM_CONTROL:
             switch (SHORT2FROMMP(mp1))
             {
                              /* Display telephone number for selected name */
                case LN_SELECT:
                   if(hwndListBox == (HWND)mp2)
                   {
                      Index = (SHORT) WinSendMsg (hwndListBox, LM_QUERYSELECTION,
                                               MPFROMSHORT ((SHORT) LIT_FIRST),
                                                  NULL);
                      if (Index != LIT_NONE)
                      {
                         WinSendMsg (hwndListBox, LM_QUERYITEMTEXT,
                                            MPFROM2SHORT((SHORT)Index,
                                                         (SHORT)sizeof(Name)),
                                            MPFROMP ((PSZ) Name));
                         i=0;
                         Found = FALSE;
                         while ((i<Count) && (!Found))
                         {
                            if (strcmp(Name, TelDir[i].Name) == 0)
                                Found = TRUE;
                            else i++;
                         } /* endwhile */
                         if (Found)
                         {
                            WinSetWindowText (hwndName,"");
                            WinSetWindowText (hwndName,TelDir[i].Name);
                            WinSetWindowText (hwndTel,"");
                            WinSetWindowText (hwndTel,TelDir[i].Number);
                         }
                      }
                   }
                   break;
             } /* endswitch */

             switch (SHORT1FROMMP(mp1))
             {
                case ID_ADDNAME:          /*Add */
                   Sel = 1;
                   WinSetWindowText (hwndName,"");
                   WinSetWindowText (hwndTel,"");
                   break;

                case ID_DELETE:     /*Delete*/
                   Sel = 2;
                   WinSetWindowText (hwndName,"");
                   WinSetWindowText (hwndTel,"");
                   break;

                case ID_UPDATE:     /*Update*/
                   Sel = 3 ;
                   break;

                case ID_SEARCH:     /*Search*/
                   Sel = 4 ;
                   WinSetWindowText (hwndName,"");
                   WinSetWindowText (hwndTel,"");
                   break;
             }
           break;

        case WM_PAINT:
           hps = WinBeginPaint( hwnd, (HPS)NULL, (PRECTL)&rc );
           WinFillRect( hps, (PRECTL)&rc, SYSCLR_ACTIVETITLETEXT);
           WinEndPaint( hps );
           break;

     case WM_HELP:
        // Get the handle of the window that has focus
        hwndFocus = WinQueryFocus(HWND_DESKTOP);

        ControlID =(USHORT)WinQueryWindowUShort(hwndFocus,QWS_ID);

        // For controls that have help, display the
        // correct help panel
        switch (ControlID)
        {
         case ID_LISTBOX:
         case ID_ADDNAME:
         case ID_DELETE:
         case ID_UPDATE:
         case ID_SEARCH:
         case ID_NAME:
         case ID_TELEPHONE:
         case IDM_OK:
              DisplayHelpPanel(ControlID);
         break;

      } /* endswitch */
     break;

        default:
           return WinDefWindowProc(hwnd, msg, mp1,mp2);
           break;
    } /* endswirch msg */
    return FALSE;
}  /* End of WinProc1 */

/*********************************************************************
 *  Name:   MessageBox
 *
 *  Description : Message Box procedure
 *
 *  Concepts : Displays the warning message box with the message
 *             given in sMsg retrived from the message table
 *             Called whever a message wishes to be displayed to
 *             the user
 *
 *  API's : WinMessageBox
 *
 *  Parameters :  sMsg     - id of message to be retrieved from
 *                            resource file
 *                sTitle   - message box title
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID MessageBox(USHORT sMsg, USHORT sTitle)
{
   CHAR    TitleText[MESSAGELEN];

   Message(sTitle);
   strcpy(TitleText, DisplayText);

   WinMessageBox( HWND_DESKTOP,
                 (HWND)hWndObject,
                 Message(sMsg),
                 TitleText,
                 0, MB_OK );

} /* End of MessageBox */

/*********************************************************************
 *  Name:   Button_Handling
 *
 *  Description : Handlind the options of the radio buttons
 *
 *  Concepts : Handles the processing of the options Add, Delete,
 *             Modify and Search.
 *
 *  API's : WinQueryWindowText
 *          WinSendMsg
 *          WinSetWindowText
 *          WinWindowFromID
 *
 *  Parameters :  VOID
 *
 *  Returns: VOID
 *
\****************************************************************/

VOID Button_Handling(VOID)
{  ULONG i;

   switch(Sel)
  {
     case 1:                           /* Add */
        WinQueryWindowText (hwndName, sizeof(NameStr), NameStr);
        WinQueryWindowText (hwndTel, sizeof(TelStr), TelStr);
        if(strcmp(NameStr,"") != 0)
        {
           if(strcmp(TelStr,"") != 0)
           {
              strcpy(TelDir[Count].Name, NameStr);
              strcpy(TelDir[Count].Number, TelStr);
              Position = LIT_SORTDESCENDING;
              if ( (SHORT) WinSendMsg (
                          WinWindowFromID (hwndFrame, FID_MENU),
                          MM_QUERYITEMATTR,
                          MPFROM2SHORT ((USHORT) IDM_ASCENDING,
                                        (BOOL) TRUE),
                          MPFROMSHORT ((USHORT) MIA_CHECKED)) )
                   Position = LIT_SORTASCENDING;
              Index = (SHORT) WinSendMsg (hwndListBox, LM_INSERTITEM,
                                          MPFROMSHORT ((SHORT) Position),
                                          MPFROMP ((PSZ) TelDir[Count].Name));
              Count++;
              WinSendMsg (hwndListBox, LM_SELECTITEM,
                          MPFROMSHORT ((SHORT) Index),
                          MPFROMSHORT ((BOOL) TRUE));
              break;
           }
           else
              DosBeep (1000, 50);

              MessageBox(ID_ERRINADD_A, ID_ERROR_A);
              break;
        }
        else
           DosBeep (1000, 50);

           MessageBox(ID_ERRINADD_A, ID_ERROR_A);
           WinSetWindowText (hwndName,"");
           WinSetWindowText (hwndTel,"");
        break;

     case 2:                           /* Delete */
        WinQueryWindowText (hwndName, sizeof(NameStr), NameStr);
        if(strcmp(NameStr,"") != 0)
        {
           Index = (SHORT) WinSendMsg (hwndListBox, LM_SEARCHSTRING,
                             MPFROM2SHORT ((USHORT) LSS_CASESENSITIVE,
                                           (SHORT) LIT_FIRST),
                             MPFROMP ((PSZ) NameStr));
           if (Index != LIT_NONE)
              Delete = TRUE;
           else
           {
              DosBeep (1000, 50);
              MessageBox(ID_ERRINREMOVE_A, ID_ERROR_A);
              break;
           }
        }
        else              /* NameStr = "" */
        {
           Index = (SHORT) WinSendMsg (hwndListBox, LM_QUERYSELECTION,
                             MPFROMSHORT ((SHORT) LIT_FIRST),
                             NULL);
           if (Index != LIT_NONE)
           {
              Delete = TRUE;
              WinSendMsg (hwndListBox, LM_QUERYITEMTEXT,
                                MPFROM2SHORT((SHORT)Index,
                                             (SHORT)sizeof(NameStr)),
                                MPFROMP ((PSZ) NameStr));
           }
           else
           {
              DosBeep (1000, 50);

              MessageBox(ID_ERRINREMOVE_A, ID_ERROR_A);
              break;
           }
        }
        if(Delete)
        {
           ItemsLeft = (SHORT) WinSendMsg (hwndListBox, LM_DELETEITEM,
                                MPFROMSHORT ((SHORT) Index),
                                NULL);
           if(ItemsLeft != 0)
           {
              if(Index != 0)
                WinSendMsg (hwndListBox, LM_SELECTITEM,
                            MPFROMSHORT ((SHORT) Index-1),
                            MPFROMSHORT ((BOOL) TRUE));
              else
              {
                 WinSendMsg (hwndListBox, LM_SELECTITEM,
                             MPFROMSHORT ((SHORT) Index),
                             MPFROMSHORT ((BOOL) TRUE));
              }
           }
           if(ItemsLeft == 1)
           {
              Index = (SHORT) WinSendMsg (hwndListBox, LM_QUERYTOPINDEX,
                                          NULL, NULL);

              WinSendMsg (hwndListBox, LM_SELECTITEM,
                          MPFROMSHORT ((SHORT) Index),
                          MPFROMSHORT ((BOOL) TRUE));
           }
           i=0;
           Found = FALSE;
           while ((i<Count) && (!Found))
           {
              if (strcmp(NameStr, TelDir[i].Name) == 0)
                  Found = TRUE;
              else i++;
           } /* endwhile */
           if (Found)
           {
                 strcpy(TelDir[i].Name, "");
                 strcpy(TelDir[i].Number, "");
           }
           WinSetWindowText (hwndName,"");
           WinSetWindowText (hwndTel,"");
        }
        break;

     case 3:                           /* Update */
        Index = (SHORT) WinSendMsg (hwndListBox, LM_QUERYSELECTION,
                                    MPFROMSHORT ((SHORT) LIT_FIRST),
                                    NULL);

        if (Index != LIT_NONE)
        {
           WinSendMsg (hwndListBox, LM_QUERYITEMTEXT,
                              MPFROM2SHORT((SHORT)Index,
                                           (SHORT)sizeof(NameStr)),
                              MPFROMP ((PSZ) NameStr));
           i=0;
           Found = FALSE;
           while ((i<Count) && (!Found))
           {
              if (strcmp(NameStr, TelDir[i].Name) == 0)
                  Found = TRUE;
              else i++;
           } /* endwhile */
           if (Found)
           {
            WinQueryWindowText (hwndName, sizeof(Temp_Name), Temp_Name);
            WinQueryWindowText (hwndTel, sizeof(Temp_Tel), Temp_Tel);
            if(strcmp(Temp_Name, "") != 0)
            {
             if(strcmp(Temp_Tel, "") != 0)
             {
               if (strcmp(NameStr, Temp_Name) != 0)
               {
                 (SHORT) WinSendMsg (hwndListBox, LM_DELETEITEM,
                                     MPFROMSHORT ((SHORT) Index),
                                     NULL);
                 strcpy(TelDir[i].Name, Temp_Name);
                 strcpy(TelDir[i].Number, Temp_Tel);
                 Position = LIT_SORTDESCENDING;
                 if ( (SHORT) WinSendMsg (
                             WinWindowFromID (hwndFrame, FID_MENU),
                             MM_QUERYITEMATTR,
                             MPFROM2SHORT ((USHORT) IDM_ASCENDING,
                                           (BOOL) TRUE),
                             MPFROMSHORT ((USHORT) MIA_CHECKED)) )
                      Position = LIT_SORTASCENDING;
                 Index = (SHORT) WinSendMsg (hwndListBox, LM_INSERTITEM,
                                             MPFROMSHORT ((SHORT) Position),
                                             MPFROMP ((PSZ) TelDir[i].Name));
                 WinSendMsg (hwndListBox, LM_SELECTITEM,
                             MPFROMSHORT ((SHORT) Index),
                             MPFROMSHORT ((BOOL) TRUE));
               }
               else
                 strcpy(TelDir[i].Number, Temp_Tel);
                 break;
             }
             else
                DosBeep (1000, 50);
                MessageBox(ID_ERRINMODIFY_A, ID_ERROR_A);
                break;
            }
            else
                DosBeep (1000, 50);
                MessageBox(ID_ERRINMODIFY_A, ID_ERROR_A);

               break;
           }/* end found */
        }/* endif LIT_NONE */
        else
           DosBeep (1000, 50);
           MessageBox(ID_ERRINMODIFY_A, ID_ERROR_A);
        break;

     case 4:                           /* Search */
        WinQueryWindowText (hwndName, sizeof(NameStr), NameStr);
        if(strcmp(NameStr,"") != 0)
        {
           Index = (SHORT) WinSendMsg (hwndListBox, LM_SEARCHSTRING,
                             MPFROM2SHORT ((USHORT) LSS_CASESENSITIVE,
                                           (SHORT) LIT_FIRST),
                             MPFROMP ((PSZ) NameStr));
           if (Index != LIT_NONE) {
              Search = TRUE;
              WinSendMsg (hwndListBox, LM_SELECTITEM,
                          MPFROMSHORT ((SHORT) Index),
                          MPFROMSHORT ((BOOL) TRUE));

           }
           else
           {
              DosBeep (1000, 50);
              MessageBox(ID_ERRINSEARCH_A, ID_ERROR_A);
              break;
           }
        }
        else              /* NameStr = "" */
        {
           Index = (SHORT) WinSendMsg (hwndListBox, LM_QUERYSELECTION,
                             MPFROMSHORT ((SHORT) LIT_FIRST),
                             NULL);
           if (Index != LIT_NONE)
           {
              Search = TRUE;
              WinSendMsg (hwndListBox, LM_QUERYITEMTEXT,
                                MPFROM2SHORT((SHORT)Index,
                                             (SHORT)sizeof(NameStr)),
                                MPFROMP ((PSZ) NameStr));
           }
           else
           {
              DosBeep (1000, 50);
              MessageBox(ID_ERRINSEARCH_A, ID_ERROR_A);
              break;
           }
        }
        if(Search)
        {
           i=0;
           Found = FALSE;
           while ((i<Count) && (!Found))
           {
              if (strcmp(NameStr, TelDir[i].Name) == 0)
                  Found = TRUE;
              else i++;
           } /* endwhile */
           if (Found)
           {
              WinSetWindowText (hwndName,"");
              WinSetWindowText (hwndName,TelDir[i].Name);
              WinSetWindowText (hwndTel,"");
              WinSetWindowText (hwndTel,TelDir[i].Number);
           }
        }
        break;
  } /* endswitch */
}  /* End of Button_Handling */

/****************************************************************\
 *  Name:   SaveData
 *
 *  Description : Saves the items to the data file teldir.dat
 *
 *  Concepts : Opens the data file with the write option and saves
 *             the data from memory to the file.
 *
 *  API's : DosBeep
 *
 *  Parameters :  VOID
 *
 *  Returns: VOID
 *
\****************************************************************/

VOID SaveData(VOID)
{
  FILE      *Stream;
  ULONG     i;

    if ((Stream = fopen ("teldir.dat","w")) != NULL)
    {
       i=0;
       while (i<Count)
       {
             if(strcmp(TelDir[i].Name, "") != 0)
             {
                fputs (TelDir[i].Name, Stream);
                fputc ('\n', Stream);
                fputs (TelDir[i++].Number, Stream);
                fputc ('\n', Stream);
             }
             else i++;
       }/* endwhile */
       fputc (0x1A,Stream);
       fclose (Stream);
    }/* endif */
    else
      DosBeep (5000L, 2L);
} /* End of SaveData */

/****************************************************************\
 *  Name:   LoadFile
 *
 *  Description : Loads the items from the data file teldir.dat
 *
 *  Concepts : Opens the data file with the read option and loads
 *             the data from the file into memory in ascending order.
 *
 *  API's : DosBeep
 *          WinSendMsg
 *
 *  Parameters :  VOID
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID LoadFile(VOID)
{
  int          Num;
  FILE         *Stream;
  BOOL         Elem_Found = FALSE;

    if ((Stream = fopen ("teldir.dat", "r")) != NULL)
    {
       Num=0;
       while (!feof(Stream))
       {
            if(fgets(TelDir[Num].Name, sizeof(TelDir[Num].Name), Stream)
                                                                     != NULL)
            {
               TelDir[Num].Name[strlen(TelDir[Num].Name)-1] = 0;
               Elem_Found = TRUE;
               fgets(TelDir[Num].Number, sizeof(TelDir[Num].Number), Stream);
               TelDir[Num].Number[strlen(TelDir[Num].Number)-1] = 0;
               Num++;
            }
       }/* endwhile */
       fclose (Stream);
       if(Elem_Found)
       {
          Count = 0;
          while(Count < Num)
          {
             Position = LIT_SORTASCENDING;
             Index = (SHORT) WinSendMsg (hwndListBox, LM_INSERTITEM,
                                         MPFROMSHORT ((SHORT) Position),
                                         MPFROMP ((PSZ) TelDir[Count].Name));
             Count++;
          }
             WinSendMsg (hwndListBox, LM_SELECTITEM,
                         MPFROMSHORT ((SHORT) 0),
                         MPFROMSHORT ((BOOL) TRUE));
       }
    }/* endif */
    else
      DosBeep (5000L, 2L);
}  /* End of LoadFile */


/****************************************************************\
 *  Name:   ChangeLayer
 *
 *  Description : Changes the current language
 *
 *  Concepts : Switches the interface language from Arabic to English
 *             and vice versa
 *
 *  API's : None
 *
 *  Parameters :  VOID
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID ChangeLayer(VOID)
{
   if (Arb_Layer){
      Arb_Layer = FALSE;
      Eng_Interface();
   }
   else{
      Arb_Layer = TRUE;
      Arb_Interface();
   }
}  /* End of ChangeLayer */

/****************************************************************\
 *  Name:   Arb_Interface
 *
 *  Description : Arabizes the interface
 *
 *  Concepts : Sets the controls' attributes and loads the strings
 *             to be displayed in each.
 *
 *  API's : WinDestroyWindow
 *          WinEnableWindowUpdate
 *          WinSetLangInfo
 *          WinLoadMenu
 *          WinSendMsg
 *          WinSetWindowText
 *          WinWindowFromID
 *
 *  Parameters :  VOID
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID Arb_Interface(VOID)
{

  WinEnableWindowUpdate(hwndFrame, FALSE);

   // Destroy previous help instance if it exists
   DestroyHelpInstance();
   // Load help with current language setting
   InitHelp();
                                 /* Set frame and all its children to RTL */
  WinSetLangInfo(hwndFrame, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_RTL | BDA_TEXT_ORIENT_RTL |
                 BDA_TEXTTYPE_IMPLICIT | BDA_NUMERALS_NATIONAL |
                 BDA_SYM_SWAP_OFF | BDA_TEXT_DISPLAY_SHAPED,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION |
                 BDAM_TEXTTYPE | BDAM_NUMERALS | BDAM_SYM_SWAP |
                 BDAM_TEXT_SHAPE,
                 LIF_CHILD_INHERIT | LIF_WND_REFRESH,
                 0L);

                                            /* Set object window to RTL */
  WinSetLangInfo(hWndObject, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_RTL | BDA_TEXT_ORIENT_RTL |
                 BDA_TEXTTYPE_IMPLICIT | BDA_NUMERALS_NATIONAL |
                 BDA_SYM_SWAP_OFF | BDA_TEXT_DISPLAY_SHAPED,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION |
                 BDAM_TEXTTYPE | BDAM_NUMERALS | BDAM_SYM_SWAP |
                 BDAM_TEXT_SHAPE,
                 0L,
                 0L);

                                    /* Set telephone entryfield to LTR */
                                    /* as numbers are always LTR       */
  WinSetLangInfo(hwndTel, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_LTR | BDA_TEXT_ORIENT_LTR,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

  WinSetWindowText (hwndStatic1, Message(ID_NAMES_A));

  WinSetLangInfo(hwndStatic1, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_RTL | BDA_TEXT_ORIENT_RTL,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

  WinSetWindowText (hwndStatic3, Message(ID_ENTERNAME_A));

  WinSetLangInfo(hwndStatic3, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_RTL | BDA_TEXT_ORIENT_LTR,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

  WinSetWindowText (hwndStatic4, Message(ID_TELNUMBER_A));

  WinSetLangInfo(hwndStatic4, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_RTL | BDA_TEXT_ORIENT_RTL,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

  WinSetWindowText (hwndGroupBox,"                            ");

  WinSetLangInfo(hwndGroupBox, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_RTL | BDA_TEXT_ORIENT_RTL,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

  WinSetWindowText (hwndGroupBox, Message(ID_CHOICE_A));

  WinSetWindowText (hwndPushButton, Message(ID_DO_A));

  WinSetWindowText (hwndRadioButton1, Message(ID_ADD_A));

  WinSetWindowText (hwndRadioButton2, Message(ID_REMOVE_A));

  WinSetWindowText (hwndRadioButton3, Message(ID_MODIFY_A));

  WinSetWindowText (hwndRadioButton4, Message(ID_FIND_A));

  DisplayTitle (ID_TITLE_A);

  WinDestroyWindow (WinWindowFromID(hwndFrame, FID_MENU));
  WinLoadMenu (hwndFrame ,NULLHANDLE, IDM_ORGMENU);
  WinEnableWindowUpdate(hwndFrame, TRUE);
  WinSendMsg (hwndFrame, WM_UPDATEFRAME, 0L, 0L);

} /* End of Arb_Interface */

/****************************************************************\
 *  Name:   Eng_Interface
 *
 *  Description : Changes the interface to English
 *
 *  Concepts : Sets the controls' attributes and loads the strings
 *             to be displayed in each.
 *
 *  API's : WinDestroyWindow
 *          WinEnableWindowUpdate
 *          WinSetLangInfo
 *          WinLoadMenu
 *          WinSendMsg
 *          WinSetWindowText
 *          WinWindowFromID
 *
 *  Parameters :  VOID
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID Eng_Interface(VOID)
{

  WinEnableWindowUpdate(hwndFrame, FALSE);

   // Destroy previous help instance if it exists
   DestroyHelpInstance();
   // Load help with current language setting
   InitHelp();
                                 /* Set frame and all its children to LTR */
  WinSetLangInfo(hwndFrame, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_LTR | BDA_TEXT_ORIENT_LTR |
                 BDA_TEXTTYPE_IMPLICIT | BDA_NUMERALS_NOMINAL |
                 BDA_SYM_SWAP_OFF | BDA_TEXT_DISPLAY_SHAPED,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION |
                 BDAM_TEXTTYPE | BDAM_NUMERALS | BDAM_SYM_SWAP |
                 BDAM_TEXT_SHAPE,
                 LIF_CHILD_INHERIT | LIF_WND_REFRESH,
                 0L);

                                            /* Set object window to LTR */
  WinSetLangInfo(hWndObject, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_LTR | BDA_TEXT_ORIENT_LTR |
                 BDA_TEXTTYPE_IMPLICIT | BDA_NUMERALS_NOMINAL |
                 BDA_SYM_SWAP_OFF | BDA_TEXT_DISPLAY_SHAPED,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION |
                 BDAM_TEXTTYPE | BDAM_NUMERALS | BDAM_SYM_SWAP |
                 BDAM_TEXT_SHAPE,
                 0L,
                 0L);

  WinSetWindowText (hwndStatic1, Message(ID_NAMES_A));


  WinSetLangInfo(hwndStatic1, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_LTR | BDA_TEXT_ORIENT_LTR,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

  WinSetWindowText (hwndStatic3, Message(ID_ENTERNAME_A));


  WinSetLangInfo(hwndStatic3, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_LTR | BDA_TEXT_ORIENT_LTR,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

  WinSetWindowText (hwndStatic4, Message(ID_TELNUMBER_A));

  WinSetLangInfo(hwndStatic4, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_LTR | BDA_TEXT_ORIENT_LTR,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

  WinSetLangInfo(hwndGroupBox, LI_BD_WND_ATTR,
                 BDA_WND_ORIENT_LTR | BDA_TEXT_ORIENT_LTR,
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

  WinSetWindowText (hwndGroupBox, Message(ID_CHOICE_A));

  WinSetWindowText (hwndPushButton, Message(ID_DO_A));

  WinSetWindowText (hwndRadioButton1, Message(ID_ADD_A));

  WinSetWindowText (hwndRadioButton2, Message(ID_REMOVE_A));

  WinSetWindowText (hwndRadioButton3, Message(ID_MODIFY_A));

  WinSetWindowText (hwndRadioButton4, Message(ID_FIND_A));

  WinSetLangInfo(WinWindowFromID(hwndFrame, FID_TITLEBAR), LI_BD_WND_ATTR,
                 BDA_NUMERALS_PASSTHRU | BDA_TEXT_SAVE_SHAPED |
                 BDA_WND_ORIENT_LTR | BDA_TEXT_ORIENT_LTR,
                 BDAM_NUMERALS | BDAM_TEXT_SHAPE |
                 BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION,
                 0L, 0L);

                                               /* Set the title bar        */
                                               /* orientation LtR          */
  WinSetWindowText(hwndFrame, Message(ID_TITLE_A));     /* Set the title bar text    */

  WinDestroyWindow (WinWindowFromID(hwndFrame, FID_MENU));
  WinLoadMenu (hwndFrame ,NULLHANDLE, IDM_ENGLISHMENU);
  WinEnableWindowUpdate(hwndFrame, TRUE);
  WinSendMsg (hwndFrame, WM_UPDATEFRAME, 0L, 0L);

}  /* End of Eng_Interface */

/*********************************************************************
 *  Name:   Message
 *
 *  Description : Message retrieval procedure
 *
 *  Concepts : Returns the wanted string in the global variable
 *             DisplayText. It checks on the language layer to know
 *             whether to retrieve the Arabic or English string.
 *
 *  API's : DosBeep
 *          WinLoadString
 *          WinMessageBox
 *
 *  Parameters : iMsg  -  id of string to be retrieved from
 *                        string table
 *
 *  Returns: PCHAR
 *
\****************************************************************/

PCHAR Message(USHORT iMsg)

{
 if (!WinLoadString(hab, (HMODULE)0, (SHORT)(Arb_Layer?iMsg:iMsg+ Eng_Offset),
                        MESSAGELEN, (PSZ)DisplayText))
      {
         DosBeep (1000, 50);
         WinMessageBox(hwndFrame, hwndFrame, "Failed to load string.", "Error", 0, MB_OK | MB_ERROR);
         return (NULL);
      }
   return (DisplayText);
}  /* End of Message */


/*********************************************************************
 *  Name:   DisplayTitle
 *
 *  Description : Displays the title bar
 *
 *  Concepts : The title bar requires special handling to make
 *             the string in the task list appear readable.  When
 *             the DeskTop attributes are VISUAL LTR, the text which
 *             was saved in typing order would appear reversed in the
 *             task list.  Therefore, the title bar text which is IMPLICIT
 *             is going to be converted according to the DeskTop attributes
 *             to make it like that in the task list.
 *
 *  API's : WinSetLangInfo
 *          WinSetWindowText
 *          WinUpper
 *          WinWindowFromID
 *
 *  Parameters : iMsg   -   id of string to be retrieved from
 *                          string table
 *
 *  Returns: VOID
 *
\****************************************************************/

VOID DisplayTitle(USHORT iMsg)
{
 CHAR         *BidiAttrStr;
 ULONG        DeskTopAtts;


  Message(iMsg);            /* DisplayText will hold the title text */

  DeskTopAtts = WinQueryLangInfo(HWND_DESKTOP, LI_BD_WND_ATTR, BDAM_ALL, 0L);

  ConvertBidiString(DisplayText, DisplayText,
                    BDA_TEXT_ORIENT_LTR | BDA_TEXTTYPE_IMPLICIT,
                    QUERY_BD_VALUE (DeskTopAtts, BDAM_TEXT_ORIENTATION) |
                    QUERY_BD_VALUE (DeskTopAtts, BDAM_TEXTTYPE),
                    BDAM_TEXT_ORIENTATION | BDAM_TEXTTYPE);

             /* To conform with the text in the task list, *
              * the desktop TEXT_ORIENT and TEXTTYPE are   *
              * going to be used in the title bar          */

  WinSetLangInfo(WinWindowFromID(hwndFrame, FID_TITLEBAR), LI_BD_WND_ATTR,
              BDA_NUMERALS_PASSTHRU | BDA_TEXT_DISPLAY_SHAPED |
              BDA_WND_ORIENT_RTL |
              QUERY_BD_VALUE (DeskTopAtts, BDAM_TEXT_ORIENTATION) |
              QUERY_BD_VALUE (DeskTopAtts, BDAM_TEXTTYPE),
              BDAM_NUMERALS | BDAM_TEXT_SHAPE |
              BDAM_WND_ORIENTATION | BDAM_TEXT_ORIENTATION | BDAM_TEXTTYPE,
              0L, 0L);


                                               /* Set the title bar        */
                                               /* orientation RtL          */

  WinSetWindowText(hwndFrame, DisplayText);        /* Set the title bar text   */

}  /* End of DisplayTitle */


/**********************************************************************
 *
 *  Name:   ConvertBidiString
 *
 *  Description : Converts text according to source and
 *                target attributes
 *
 *  Concepts : The text in SourceStr is transformed using layout
 *             functions and output to TargetStr.  ConvertBidiString is
 *             implemented to be general.  It takes source and target
 *             strings and attributes plus the mask and converts the
 *             text accordindly.
 *
 *  API's : LayoutCreateObject
 *          LayoutDestroyObject
 *          LayoutSetValues
 *          LayoutTransformText
 *          WinMessageBox
 *
 *  Parameters : SourceStr  - the source string
 *               TargetStr  - the target string where the converted text is put
 *               SrcBDAtts  - Bidi attributes of the source string
 *               TrgBDAtts  - Bidi attributes of the target string
 *               BDMask     - Bidi attributes mask
 *
 *  Returns: VOID
 *
\****************************************************************/

VOID ConvertBidiString (PUCHAR SourceStr, PUCHAR TargetStr,
                        ULONG SrcBDAtts, ULONG TrgBDAtts, ULONG BDMask)
{
  LAYOUT_OBJECT   hLayout;
  LAYOUT_VALUES   Layout[4];
  APIRET          RC=0;
  ULONG           index,
                  ulLength;


  RC = LayoutCreateObject (Locale_Arabic,&hLayout);

  if (RC)
  {
   WinMessageBox(HWND_DESKTOP, hwndFrame, "Failed to create layout object",
                "Bidi text transform - Error",
                 0, MB_ERROR | MB_OK);
     return;
  }

  ulLength=strlen(SourceStr);


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
   WinMessageBox(HWND_DESKTOP, hwndFrame, "Failed to set layout values",
                "Bidi text transform - Error",
                 0, MB_ERROR | MB_OK);
     return;
  }
                                        /* Do actual text transformation */
  RC = LayoutTransformText (hLayout,
                            SourceStr,
                            &ulLength,
                            TargetStr,
                            &ulLength, NULL, NULL, NULL);
  if (RC)
  {
   WinMessageBox(HWND_DESKTOP, hwndFrame, "LayoutTransformText failed",
                "Bidi text transform - Error",
                 0, MB_ERROR | MB_OK);
   return;
  }

  LayoutDestroyObject(hLayout);

  return;

}   /* End of ConvertBidiString */

/****************************************************************\
 *  Routine for initializing the help manager
 *--------------------------------------------------------------
 *
 *  Name:    InitHelp(VOID)
 *
 *  Purpose: Initializes the IPF help facility
 *
 *  Usage:   Called every time the interface language is changed
 *
 *  Method:  Initializes the HELPINIT structure and creates the help
 *           instance. If successful, the help instance is associated
 *           with the main window
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID InitHelp(VOID)
{
   HELPINIT hini;
   ULONG    NlsOffset = Arb_Layer ? 0 : Eng_Offset ;

   /* If we return because of an error, Help will be disabled */
   fHelpEnabled = FALSE;
                             /* Initialize help init structure */
   hini.cb = sizeof(HELPINIT);
   hini.ulReturnCode = 0;
                             /* If tutorial added, add name here */
   hini.pszTutorialName = (PSZ)NULL;

   hini.phtHelpTable = (PHELPTABLE)MAKELONG(TELDIR_HELP_TABLE,0xFFFF);
   hini.hmodHelpTableModule = 0;
   hini.hmodAccelActionBarModule = 0;
   hini.idAccelTable = 0;
   hini.idActionBar = 0;

   if(!WinLoadString(hab,
                     0,
                     ID_HELPWINDOWTITLE_A+NlsOffset,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szWindowTitle))
   {
       MessageBox(ID_ERROR_A, ID_CANNOTLOADSTRING_A);
       return;
   }
   hini.pszHelpWindowTitle = (PSZ)szWindowTitle;

   hini.fShowPanelId = CMIC_HIDE_PANEL_ID;

   if(!WinLoadString(hab,
                     0,
                     ID_HELPLIBRARYNAME_A+NlsOffset,
                     HELPLIBRARYNAMELEN,
                     (PSZ)szLibName))
   {
       MessageBox(ID_ERROR_A, ID_CANNOTLOADSTRING_A);
       return;
   }

   hini.pszHelpLibraryName = (PSZ)szLibName;
                                       /* Creating help instance */
   hwndHelpInstance = WinCreateHelpInstance(hab, &hini);

   if(hwndHelpInstance == 0L || hini.ulReturnCode)
   {
       MessageBox(ID_ERROR_A, ID_HELPLOADERROR_A);
       return;
   }
                             /* Associate help instance with main frame */
   if(!WinAssociateHelpInstance(hwndHelpInstance, hwndFrame))
   {
       MessageBox(ID_ERROR_A, ID_HELPLOADERROR_A);
       return;
   }

   /* Help manager is successfully initialized so set flag to TRUE */
   fHelpEnabled = TRUE;
   return;

}   /* InitHelp() */

/****************************************************************\
 *  Displays the help panel indicated
 *--------------------------------------------------------------
 *
 *  Name:    DisplayHelpPanel(LONG idPanel)
 *
 *  Purpose: Displays the help panel whose id is given
 *
 *  Usage:   Called whenever a help panel is desired to be displayed,
 *           from the WM_HELP processing.
 *
 *  Method:  Sends HM_DISPLAY_HELP message to the help instance
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID DisplayHelpPanel(LONG idPanel)
                        /* ID of the help panel to be displayed */
{
    if(fHelpEnabled)
        if((LONG)WinSendMsg(hwndHelpInstance,
                      HM_DISPLAY_HELP,
                      MPFROMLONG(MAKELONG(idPanel, NULL)),
                      MPFROMSHORT(HM_RESOURCEID)))

            MessageBox(ID_ERROR_A,
                       ID_HELPDISPLAYERROR_A);
    return;

}   /* DisplayHelpPanel() */

/****************************************************************\
 *  Destroys the help instance
 *--------------------------------------------------------------
 *
 *  Name:    DestroyHelpInstance(VOID)
 *
 *  Purpose: Destroys the help instance for the application
 *
 *  Usage:   Called when switching interface language and
 *           after exit from message loop
 *
 *  Method:  Calls WinDestroyHelpInstance() to destroy the help instance
 *
 *  Returns: VOID
 *
\****************************************************************/
VOID DestroyHelpInstance(VOID)
{
    if(hwndHelpInstance != 0L)
    {
        WinDestroyHelpInstance(hwndHelpInstance);
    }
    return;

}   /* DestroyHelpInstance() */
