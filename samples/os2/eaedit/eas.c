/*static char *SCCSID = "@(#)eas.c	6.14 92/03/18";*/
/*==============================================================*\
 *  EAS.C -New Extended Attribute Display sample PM application *
 *                                                              *
 *  (C) Copyright IBM Corporation 1996.                         *
 *--------------------------------------------------------------*
 *                                                              *
 *  This program implements an extended attributes editor.      *
 *  Included in this file are all of the PM interface           *
 *  procedures.  All of these are procedures for dialog boxes.  *
 *  This code is implemented as it was written to show how to   *
 *  use the EA editor.                                          *
 *                                                              *
 *  The EA code revolves around the HoldFEA structure.  This    *
 *  structure is a linked list which contains one EA per        *
 *  structure.  Each HoldFEA structure is dynamically allocated *
 *  as are the two string pointer members of the structure,     *
 *  szName and aValue.  This is done to make handling the EAs   *
 *  easier and more flexible.                                   *
 *                                                              *
 *  This application also serves as a template that can be      *
 *  easily modified by an application developer.  The source    *
 *  files are organized so that the overhead code that should   *
 *  be in all applications is located in the same files so      *
 *  that these files do not need to be modified.  The routines  *
 *  that deal with application specific code are also located   *
 *  in their own modules.  An application developer need only   *
 *  change these files in order to modify this template for     *
 *  his application.                                            *
 *                                                              *
 *--------------------------------------------------------------*
 *  Procedures in this file:                                    *
 *   main()         Sets up the PM environment and heap and     *
 *                       calls the MainWndProc                  *
 *   MainWndProc()  Handles the main window messages            *
 *   MainPaint()    Handles the main paint window messages      *
 *   MainCommand()  Handles the main command window messages    *
 *   ProcExit()     Handles the WinClose window messages        *
 *   UserCommand()  Handles the user command window messages    *
 *   MainControl()  Handles the main control window messages    *
 *   MessageBox     Handles all error messages                  *
 *   AddEAProc()    Handles new EA name entry and type selection*
 *   AsciiEditProc()Handles editing of EA name/value            *
 *   IconDlgProc()  Unimplemented icon procedure handler        *
 *   MultiTypeProc()Handles the Multi-type dialog box           *
 *   ShowEAType()   Shows Selected EA Type for MainDlgProc      *
 *   EditEA()       Handles selection of an EA in the listbox   *
 *   GetCurFEA()    Gets a ptr to the hi-lited EA in the listbox*
 *   GetMem()                                                   *
\*==============================================================*/
/*--------------------------------------------------------------*\
 *  Include files, macros, defined constants, and externs       *
\*--------------------------------------------------------------*/
#include "eas.h"

       /* Setup the table, the order is related to DLGTEMPLATE of IDD_ADDEA */
EADATA ConvTable[EATABLESIZE] = {
   { EA_LPASCII   ,"Length preceeded ascii\0             ",IDD_LPDATA},
   { EA_LPBINARY  ,"Length preceeded hex binary\0        ",IDD_LPDATA},
   { EA_ASCIIZ    ,"Asciiz\0                             ",IDD_ASCIIZ},
   { EA_LPMETAFILE,"Metafile\0                           ",IDD_LPDATA},
   { EA_ASCIIZFN  ,"Asciiz file name of associated data\0",IDD_ASCIIZ},
   { EA_ASCIIZEA  ,"Asciiz EA of associated data\0       ",IDD_ASCIIZ},
   { EA_MVMT      ,"Multi value multi type field\0       ",IDD_MULTILIST},
   { EA_ASN1      ,"ASN.1 field\0                        ",IDD_ASCIIZ},
   { EA_LPBITMAP  ,"Length preceeded bitmap\0            ",IDD_LPDATA},
   { EA_LPICON    ,"Length preceeded icon\0              ",IDD_LPDATA},
   { EA_MVST      ,"Multi value single type field\0      ",IDD_MULTILIST},
   { 0            ,"Non conventional format\0            ",IDD_ASCIIZ}
   };

/****************************************************************\
 *  Main routine                                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    main(int argc, char *argv[])                       *
 *                                                              *
 *  Purpose: Initializes the PM environment, calls the          *
 *           initialization routine, creates the main window,   *
 *           and polls the message queue.                       *
 *                                                              *
 *  Usage:                                                      *
 *                                                              *
 *  Method:  -obtains anchor block handle and creates message   *
 *              queue                                           *
 *           -calls the initialization routine                  *
 *           -creates the main frame window which creates the   *
 *              main client window                              *
 *           -polls the message queue via Get/Dispatch Msg loop *
 *           -upon exiting the loop, exits                      *
 *                                                              *
 *  Returns: 1 - if sucessful execution completed               *
 *           0 - if error                                       *
 *                                                              *
\****************************************************************/
INT main(int argc, char *argv[])
{
   QMSG qmsg;                                         /* message structure */
   ULONG ctlData = FCF_SYSMENU | FCF_MENU | FCF_ICON | /* frame control data */
                   FCF_TASKLIST | FCF_TITLEBAR |
                   FCF_SIZEBORDER | FCF_ACCELTABLE |
                   FCF_MINMAX | FCF_SHELLPOSITION;

   FILE_ISOPEN       = FALSE;
   FILE_CHANGED      = FALSE;
   COMMAND_LINE_FILE = FALSE;

   if(argc > 1)                  /* If a command line file, get EAs from it */
      if(ParseFileName(szFileName,argv[1]) != FILE_VALID)  /* Bad file name */
         *szFileName = 0;
      else
         COMMAND_LINE_FILE = TRUE;

   /* Note: The following 3 setup calls aren't currently checked for errors */
   hab = WinInitialize(0);
   if(!hab)  
   {
       DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
       return RETURN_ERROR;
   }

   hmq = WinCreateMsgQueue(hab, 0UL);
   if(!hmq)  
   {
       DosBeep(BEEP_WARN_FREQ, BEEP_WARN_DUR);
       WinTerminate(hab);
       return RETURN_ERROR;
   }

   if(!Init())
   {
       MessageBox(HWND_DESKTOP, IDMSG_INITFAILED, "Error !",
                           MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
       DosExit(EXIT_PROCESS, RETURN_ERROR);
   }
                                              /* create the main window */
   hwndMainFrame = WinCreateStdWindow(HWND_DESKTOP,
                                      WS_VISIBLE,
                                      (PVOID)&ctlData,
                                      (PSZ)szAppName,
                                      (PSZ)NULL,
                                      WS_VISIBLE,
                                      (HMODULE)NULL,
                                      ID_RESOURCE,
                                      (PHWND)&hwndMain);
   if(!hwndMainFrame)
   {
       MessageBox(HWND_DESKTOP, IDMSG_MAINWINCREATEFAILED, "Error !",
                           MB_OK | MB_ERROR | MB_MOVEABLE, TRUE);
       DosExit(EXIT_PROCESS, RETURN_ERROR);
   }
   InitHelp();

   while(WinGetMsg(hmq, (PQMSG)&qmsg, 0L, 0L, 0L))
           WinDispatchMsg(hmq, (PQMSG)&qmsg);

                                           /* destroy the help instance */
   DestroyHelpInstance();
                                    /* will normally be put in ExitProc */
   DosExit(EXIT_PROCESS, RETURN_SUCCESS);
   return 0;
}                                                              /* main() */

/****************************************************************\
 *  Main client window procedure                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MainWndProc(hwnd, msg, mp1, mp2)                   *
 *                                                              *
 *  Purpose: Processes the messages sent to the main client     *
 *           window. This routine processes the basic messages  *
 *           all client windows should process and passes all   *
 *           others onto UserWndProc where the developer can    *
 *           process any others.                                *
 *                                                              *
 *  Usage:   Called for each message placed in the main         *
 *           window's message queue                             *
 *                                                              *
 *  Method:  a switch statement branches to the routines to be  *
 *           performed for each message processed. Any messages *
 *           not specifically process are passed to the user's  *
 *           message processing procedure UserWndProc().        *
 *                                                              *
 *  Returns: Return values are determined by each message       *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY MainWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

   switch(msg)
   {
      case WM_CREATE:
          WinPostMsg(hwnd,WM_USER,(MPARAM)0,(MPARAM)0);
          break;

      case WM_USER:  /* Posted by WM_INITDLG allows main window to open 1st */
          if(COMMAND_LINE_FILE)
          {
             OpenFile(hwnd, ARGFILE);
          }
          break;
      case WM_PAINT:
          MainPaint(hwnd);
          break;

      case WM_COMMAND:
          MainCommand(hwnd, mp1, mp2);
          break;

      case WM_CONTROL:
          MainControl(hwnd, mp1, mp2);
          break;

   /*--------------------------------------------------*\
    *      Any messages not processed are passed on    *
    *      to the user's window proc.  It is           *
    *      responsible for passing any messages it     *
    *      doesn't handle onto WinDefWindowProc()      *
   \*--------------------------------------------------*/
      case HM_QUERY_KEYS_HELP:
          return ((MRESULT)PANEL_KEYSHELP);

   default:
      {
         MRESULT sRC = WinDefWindowProc(hwnd, msg, mp1, mp2);
         return sRC;
      }
   }
   return (MRESULT)0L;
                    /* all window procedures should return 0 as a default */
}                                                        /* MainWndProc() */

/****************************************************************\
 *  Main client painting routine                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MainPaint(hwnd)                                    *
 *                                                              *
 *  Purpose: Paints the main client window.                     *
 *                                                              *
 *  Usage:   Routine is called whenver the client window        *
 *           procedure receives a WM_PAINT message              *
 *                                                              *
 *  Method:  -begins painting by calling WinBeginPaint          *
 *              and retriving the HPS for the window            *
 *           -paint page info boxes                             *
 *                                                              *
 *           -ends painting by calling WinEndPaint              *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID MainPaint(HWND hwnd)
{
   RECTL  rect;                         /* Rectangle struct for painting */
   CHAR   szBuffer[BUFF_SIZE];
   POINTL ptl;
   HPS hps = WinBeginPaint(hwnd, 0L, (PRECTL)&rect);       /* Handle for painting   */
                                                      
   WinFillRect(hps, (PRECTL)&rect, CLR_BACKGROUND);
                 /* Fill update rectangle with window color */
   WinQueryWindowRect(hwndMain, (PRECTL)&rect);

      ptl.x = HORZ_INDENT;
      ptl.y = rect.yTop - VERT_TEXT_POS;
      memset(szBuffer, 0, sizeof(szBuffer));
      strcpy(szBuffer, "File Name:");
      GpiCharStringAt(hps, &ptl, strlen(szBuffer), szBuffer);

      ptl.x = HORZ_INDENT;
      ptl.y = rect.yTop - VERT_TEXT_POS*3;
      memset(szBuffer, 0, sizeof(szBuffer));
      strcpy(szBuffer, "EA Type:");
      GpiCharStringAt(hps, &ptl, strlen(szBuffer), szBuffer);

      ptl.x = HORZ_INDENT;
      ptl.y = rect.yTop - VERT_TEXT_POS*5;
      memset(szBuffer, 0, sizeof(szBuffer));
      strcpy(szBuffer, "EA Names:");
      GpiCharStringAt(hps, &ptl, strlen(szBuffer), szBuffer);

   if(FILE_ISOPEN)
   {
      ptl.x = HORZ_INDENT + 10;
      ptl.y = rect.yTop - VERT_TEXT_POS*2;
      GpiCharStringAt(hps, &ptl, strlen(szFileName), szFileName);

      ptl.x = HORZ_INDENT + 10;
      ptl.y = rect.yTop - VERT_TEXT_POS*4;
      GpiCharStringAt(hps, &ptl, strlen(szEAType), szEAType);
   }
   else
   {
      ptl.x = HORZ_INDENT + 10;
      ptl.y = rect.yTop - VERT_TEXT_POS*2;
      strcpy(szBuffer, "File name will appear here!");
      GpiCharStringAt(hps, &ptl, strlen(szBuffer), szBuffer);

      ptl.x = HORZ_INDENT + 10;
      ptl.y = rect.yTop - VERT_TEXT_POS*4;
      strcpy(szBuffer, "EA types will appear here!");
      GpiCharStringAt(hps, &ptl, strlen(szBuffer), szBuffer);
   }
   WinEndPaint(hps);
   return ;
}                                                        /* MainPaint() */

/****************************************************************\
 *  Main window WM_COMMAND processing procedure                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   MainCommand(hwnd, mp1, mp2)                         *
 *                                                              *
 *  Purpose:Calls the appropriate procedures that deal with     *
 *           the selected menu item.                            *
 *                                                              *
 *  Usage:  Routine is called whenever a WM_COMMAND message     *
 *          is posted to the main window.                       *
 *                                                              *
 *  Method: a switch statement branches on the id of the        *
 *          menu item that posted the message and the           *
 *          appropriate action for that item is taken.  Any     *
 *          menu ids that are not part of the standard menu     *
 *          set are passed onto the user defined WM_COMMAND     *
 *          processing procedure.                               *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID MainCommand(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
    switch(SHORT1FROMMP(mp1))
    {
        case IDM_GENERALHELP:
           HelpExtended(mp2);
           break;

        case IDM_USINGHELP:
           HelpHelpForHelp(mp2);
           break;

        case IDM_TUTORIAL:
           HelpKeys(mp2);
           break;

        case IDM_HELPINDEX:
           HelpIndex(mp2);
           break;

        case IDM_HELPPRODUCTINFO:
           HelpAbout(mp2);
           break;

    /*--------------------------------------------------*\
     *      User command processing routine is called   *
     *      here so any ids not procecessed here can be *
     *      processed                                   *
    \*--------------------------------------------------*/
        default:
            UserCommand(hwnd, mp1, mp2);
            break;
    }
    return;
}                                                /* MainCommand() */

/****************************************************************\
 *  Main window WM_CONTROL processing procedure                 *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:   MainControl(hwnd, mp1, mp2)                         *
 *                                                              *
 *  Purpose:Calls the appropriate procedures that deal with     *
 *           the selected menu item.                            *
 *                                                              *
 *  Usage:  Routine is called whenever a WM_CONTROL message     *
 *          is posted to the main window.                       *
 *                                                              *
 *  Method: a switch statement branches on the id of the        *
 *          menu item that posted the message and the           *
 *          appropriate action for that item is taken.  Any     *
 *          menu ids that are not part of the standard menu     *
 *          set are passed onto the user defined WM_CONTROL     *
 *          processing procedure.                               *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID MainControl(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
   switch(SHORT1FROMMP(mp1))
   {
      case IDD_WINLBOX:
         switch(SHORT2FROMMP(mp1))
         {
            case LN_SELECT:           /* A new item has been hi-lited */
               ShowEAType(hwnd);      /* Display appropriate EA Type  */
               break;

            case LN_ENTER:            /* Edit the selected EA         */
               EditEA(hwnd);
               break;
         }
   }
    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'  *
    \*  warning at compile time.                                      */
   mp2;
   return;
}

/****************************************************************\
 *  Exit routine                                                *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    ProcExit(hwnd, mp2)                                *
 *                                                              *
 *  Purpose: Processes the menu's Exit item.                    *
 *                                                              *
 *  Usage:   called whenever Exit from the menu is selected     *
 *                                                              *
 *  Method:  Routine posts a WM_CLOSE message to the main       *
 *           application window.                                *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID ProcExit(HWND hwnd, MPARAM mp2)
{
   USHORT   usRet;

   if(FILE_CHANGED)     /* Give user a chance to save changes   */
   {
      usRet= MessageBox(hwnd, IDMSG_FILECHANGED, "Warning !",
                        MB_YESNOCANCEL | MB_ICONEXCLAMATION, TRUE);
      switch(usRet)
      {
         case MBID_YES:            /* They want to save the EAs */
            WriteEAs();
            break;
         case MBID_CANCEL:    /* They don't really want to quit */
            break;
      }
   }
   WinPostMsg(hwndMain, WM_CLOSE, MPVOID, MPVOID);

    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'  *
    \*  warning at compile time.                                      */
   mp2;
   return;
}                                                        /* ProcExit() */

/****************************************************************\
 *  Non-standard menu item command processing procedure         *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    UserCommand(hwnd, mp1, mp2)                        *
 *                                                              *
 *  Purpose: Process any WM_COMMAND messages send to hwndMain   *
 *              that are not processed by MainCommand           *
 *                                                              *
 *  Usage:   Routine is called for each WM_COMMAND that is      *
 *           not posted by a standard menu item                 *
 *                                                              *
 *  Method:  A switch statement branches control based upon     *
 *           the id of the control which posted the message     *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID UserCommand(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
   RECTL  rect;                       /* Rectangle struct for painting */
   LONG usRet;

   switch (SHORT1FROMMP(mp1))
   {
       case IDM_OPEN:          /* Select another file to view/edit EAs */
          if(FILE_CHANGED)        /* Give them a chance to save modifications */
          {
             usRet = MessageBox(hwnd, IDMSG_FILECHANGED, "Warning !",
                                  MB_YESNOCANCEL | MB_ICONEXCLAMATION, TRUE);
             switch(usRet)
             {
                case MBID_YES:
                     WriteEAs();
                     break;

                case MBID_NO:
                     break;

                case MBID_CANCEL:
                     return;
             }
          }
          WinQueryWindowRect(hwnd, &rect);
          if(hwndChild1)
             WinDestroyWindow(hwndChild1);

          hwndChild1 = WinCreateWindow(hwnd,
                          WC_LISTBOX,
                          NULL,
                          WS_VISIBLE,
                          rect.xLeft,
                          rect.yBottom,
                          rect.xRight - rect.xLeft,
                          rect.yTop - rect.yBottom - VERT_TEXT_POS * 5,
                          hwnd,
                          HWND_TOP,
                          IDD_WINLBOX,
                          NULL,
                          (PVOID)NULL);
          if(OpenFile(hwnd,0))                         /* File open ok */
                       break;
          break;

       case IDM_SAVE:          /* Write EA's out to disk               */
          WriteEAs();
          break;

       case IDM_ADD:           /* Add an EA to current list in memory  */
          AddEA(hwnd);
          break;

       case IDM_DELETE:        /* Delete an EA from memory             */
          DeleteCurEA(hwnd);
          break;

       case IDM_EDIT:          /* Edit the hilited EA                  */
          EditEA(hwnd);
          break;

       default:
         break;
   }
    /* This routine currently doesn't use the mp2 parameter but       *\
     *  it is referenced here to prevent an 'Unreferenced Parameter'  *
    \*  warning at compile time.                                      */
   mp2;
   return;
}

/****************************************************************\
 *  Message Box procedure                                       *
 *--------------------------------------------------------------*
 *                                                              *
 *  Name:    MessageBox(hwndOwner,IdMsg,pszMsg,fsStyle,bBeep)   *
 *                                                              *
 *  Purpose: Displays the warning message box with the message  *
 *              given in nIdMsg retrived from the message table *
 *                                                              *
 *  Usage:   Called whever an error occurs and a message wishes *
 *           to be displayed to the user                        *
 *                                                              *
 *  Method:  - Message string is loaded from the process'       *
 *              message table                                   *
 *           - Alarm beep is sounded if desired                 *
 *           - Message box with the message is displayed        *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
LONG MessageBox(HWND hwndOwner, LONG IdMsg, PSZ pszMsg, LONG fsStyle,
                     BOOL bBeep)
{
    CHAR szText[MESSAGELEN];
    LONG usRet;

    if(!WinLoadMessage(hab, NULLHANDLE, IdMsg, MESSAGELEN, (PSZ)szText))
    {
        WinAlarm(HWND_DESKTOP, WA_ERROR);
        return RETURN_ERROR;
    }
    if(bBeep)
        WinAlarm(HWND_DESKTOP, WA_ERROR);

    usRet = WinMessageBox(HWND_DESKTOP,
                         hwndOwner,
                         szText,
                         (PSZ)pszMsg,
                         IDM_MSGBOX,
                         fsStyle);
    return usRet;
}                                                   /* MessageBox() */

/****************************************************************\
 *                                                              *
 *  Name:    AddEAProc(hwnd, msg, mp1, mp2)                     *
 *                                                              *
 *  Purpose: Handles the screen that allows the user to enter a *
 *           new EA Name and select the EA type.  It checks to  *
 *           make sure the name is unique and is not NULL before*
 *           returning. The user pointer is used to determine   *
 *           whether the name is allowed to change or not.      *
 *           If it points to a PassData structure, the Point    *
 *           member gives the static EA Name.                   *
 *                                                              *
 *  Usage:   Called by MainDlgProc when user select the ADD.    *
 *                                                              *
 *  Method:  Called by both the add EA routine and the m-m EA   *
 *           add routine.  This routine places the user typed   *
 *           name in global szEAName, and the EA type in global *
 *            usRetEAType.                                      *
 *                                                              *
 *  Returns: Exits with WinDismissDlg set to TRUE if the user   *
 *           selects OK, FALSE if the user selects CANCEL.      *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY AddEAProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static CHAR bPDat = 0;   /* flag indicating if the pPDat struct is valid */
   PASSDATA FAR *pPDat;     /* struct used to pass in default EA name       */
   USHORT   i;
   MRESULT  sRC;

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         bPDat = 0;
         WinSendDlgItemMsg(hwnd, IDD_EANAME,EM_SETTEXTLIMIT,
                                 MPFROM2SHORT(MAXEANAME,0),NULL);
         WinSendDlgItemMsg(hwnd, EA_LPASCII, BM_CLICK,(MPARAM)0L,(MPARAM)0L);

         pPDat=PVOIDFROMMP(mp2);
         if(pPDat)
         {
            bPDat = 1;
            WinSetDlgItemText(hwnd, IDD_EANAME, pPDat->Point);
            WinEnableWindow(WinWindowFromID(hwnd,IDD_EANAME),
                            FALSE);                /* Disable EA Name field */
            WinSetFocus(HWND_DESKTOP,WinWindowFromID(hwnd,DID_OK));

            return (MRESULT)TRUE;    /* We don't want default focus setting */
         }
         return (MRESULT)0L;

      case WM_COMMAND:
         switch(LOUSHORT(mp1))
         {
            case DID_OK:
               if(bPDat == 0)          /* It is a new EA (not multi-type)   */
               {
                  WinQueryDlgItemText(hwnd, IDD_EANAME, MAXEANAME+1, szEAName);

                  if(!strlen(szEAName))    /* Don't allow a Null EA Name    */
                  {
                     MessageBox(hwnd, IDMSG_ERROREANAME, "Error !",
                                         MB_OK | MB_ERROR, TRUE);
                     return (MRESULT)0L;
                  }

                  if(EAExists(szEAName))   /* Don't allow duplicate EA Name */
                  {
                     MessageBox(hwnd, IDMSG_DUPLICATENAME, "Error !",
                                             MB_OK | MB_ERROR, TRUE);
                     return (MRESULT)0L;
                  }
                  usRetEAType = (USHORT) WinSendDlgItemMsg(hwnd, EA_ASCIIZ,
                            BM_QUERYCHECKINDEX,(MPARAM)0L,(MPARAM)0L);
                                                         /* New MVST EAname */
                  if (ConvTable[usRetEAType].usPrefix == EA_MVST)
                  {
                     for (i=0; i<MAXEANAME+1 && szEAName[i] != '\000'; i++)
                     {                   /* Transfer lowercase to uppercase */
                        if(szEAName[i]>='a' && szEAName[i]<='z')
                           szEAName[i] = szEAName[i] - (CHAR)' ';
                     }
                                                  /* Store MVST name & type */
                     for(i=0; i<MSNUM && MSdata[i].szName != NULL;)
                              i++;
                     GetMem((PPVOID)&MSdata[i].szName, sizeof(MVSTDATA));
                     strcpy(MSdata[i].szName,szEAName);
                     MSdata[i].usType  = usRetEAType;
                  }
               }
               else                                    /* It is multi value */
               {
                  usRetEAType = (USHORT) WinSendDlgItemMsg(hwnd,
                                              EA_ASCIIZ,
                                              BM_QUERYCHECKINDEX,
                                              (MPARAM)0L,(MPARAM)0L);

                           /* Check Type Name is in the list of MVST or not */
                  for (i=0; i<MSNUM && MSdata[i].szName!=NULL &&
                                  strcmp(szEAName,MSdata[i].szName)!=0;)
                            i++;
                                         /* 2'nd value of existed MVST type */
                  if(i<MSNUM && MSdata[i].szName!=NULL)
                  {
                     if (ConvTable[usRetEAType].usPrefix == EA_MVST)
                     {
                        MessageBox(hwnd, IDMSG_ERRORMULTIVALUE, "Error !",
                                             MB_OK | MB_ERROR, TRUE);
                        return (MRESULT)0L;
                     }
                     if (ConvTable[usRetEAType].usPrefix == EA_MVMT)
                     {
                        MessageBox(hwnd, IDMSG_ERRORMULTITYPE, "Error !",
                                             MB_OK | MB_ERROR, TRUE);
                        return (MRESULT)0L;
                     }
                     MSdata[i].usType = usRetEAType;
                  }
               }
               WinDismissDlg(hwnd, TRUE);
               return (MRESULT)0L;

            case DID_CANCEL:
               WinDismissDlg(hwnd, FALSE);
               return (MRESULT)0L;
         }
         break;

      case WM_CLOSE:
         WinDismissDlg(hwnd, FALSE);
         return (MRESULT)0L;
   }
   sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
   return sRC;
}

/****************************************************************\
 *                                                              *
 *  Name:    AsciiEditProc(hwnd, msg, mp1, mp2)                 *
 *                                                              *
 *  Purpose: This proc handles the editing of EA Names and their*
 *           associated ASCII strings.                          *
 *                                                              *
 *  Method:  A PassData struct must be passed in when called.   *
 *           Note that the Value buffer is dynamically allocated*
 *           and it is the responsibility of the calling        *
 *           procedure to deallocate the buffer (szScratch) when*
 *           finished with it. Note also that memory allocations*
 *           are NOT fully error trapped.                       *
 *                                                              *
 *  Returns: Exits with WinDismissDlg set to TRUE if the user   *
 *           selects OK, FALSE if the user selects CANCEL. Global
 *           szEAName contains the modified EA Value.  the fFlag*
 *           member of the passed in data will be updated to    *
 *           reflect the currentstate of the Need bit.          *                   *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY AsciiEditProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static PASSDATA FAR *pPDat;
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         WinSendDlgItemMsg(hwnd, IDD_EANAME,EM_SETTEXTLIMIT,
                                 MPFROM2SHORT(MAXEANAME,0),NULL);
         WinSendDlgItemMsg(hwnd, IDD_EAVALUE,EM_SETTEXTLIMIT,
                                 MPFROM2SHORT(MAXEAVALUE,0),NULL);
         pPDat=PVOIDFROMMP(mp2);

         WinSetDlgItemText(hwnd, IDD_EANAME,pPDat->Point);
         WinSetDlgItemText(hwnd, IDD_EATYPE,ConvTable[usRetEAType].szFormat);
         WinSendDlgItemMsg(hwnd, IDD_NEEDBIT, BM_SETCHECK,
                           MPFROM2SHORT(pPDat->fFlag,0),NULL);

         if(szAscii)   /* Set default EA Value if global var is != NULL */
            WinSetDlgItemText(hwnd, IDD_EAVALUE,szAscii);

         if(pPDat->usIndex) /* Disable EANAME if passed in a non-zero value */
         {
            WinEnableWindow(WinWindowFromID(hwnd,IDD_EANAME),FALSE);
            WinEnableWindow(WinWindowFromID(hwnd,IDD_NEEDBIT),FALSE);
            WinSetFocus(HWND_DESKTOP,WinWindowFromID(hwnd,IDD_EAVALUE));
            return (MRESULT)TRUE;    /* We don't want default focus setting */
         }
         return (MRESULT)0L;

      case WM_COMMAND:
         switch(LOUSHORT(mp1))
         {
            case DID_OK:
               pPDat->fFlag = (BYTE) WinSendDlgItemMsg(hwnd, IDD_NEEDBIT,
                                                       BM_QUERYCHECK, 0L, 0L);
               WinQueryDlgItemText(hwnd, IDD_EANAME, MAXEANAME+1, szEAName);

               if(!strlen(szEAName))            /* They nulled out the name */
               {
                  MessageBox(hwnd, IDMSG_ERROREANAME, "Error !",
                                     MB_OK | MB_ERROR, TRUE);
                  return (MRESULT)0L;
               }

               GetMem( &szScratch, (ULONG)MAXEAVALUE+1L); /* Allocate buffer for value */
               WinQueryDlgItemText(hwnd, IDD_EAVALUE, MAXEAVALUE+1, szScratch);

               if(!strlen(szScratch))          /* They nulled out the value */
               {
                  FreeMem(szScratch);              /* Free the value buffer */
                  MessageBox(hwnd, IDMSG_ERRORVALUE, "Error !",
                                         MB_OK | MB_ERROR, TRUE);
                  return (MRESULT)0L;
               }
               if(stricmp(szEAName,pPDat->Point))    /* changed the EA name */
               {
                  if(EAExists(szEAName))   /* Can't have duplicate EA names */
                  {
                     MessageBox(hwnd, IDMSG_DUPLICATENAME, "Error !",
                                           MB_OK | MB_ERROR, TRUE);
                     FreeMem(szScratch);               /* Free value buffer */
                     return (MRESULT)0L;
                  }
               }

               DosSetMem(szScratch, strlen(szScratch)+1, fPERM|PAG_COMMIT);

               WinDismissDlg(hwnd, TRUE);
               return (MRESULT)0L;

            case DID_CANCEL:
               WinDismissDlg(hwnd, FALSE);
               return (MRESULT)0L;
         }
         break;

      case WM_CLOSE:
         WinDismissDlg(hwnd, FALSE);
         return (MRESULT)0L;
   }
   sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
   return sRC;
}

/****************************************************************\
 *  Name:    IconDlgProc(hwnd, msg, mp1, mp2)                   *
 *                                                              *
 *  Purpose: This proc is intedned to handle displaying and     *
 *            editing icons/bitmaps.                            *
 *                                                              *
 *  Usage  :                                                    *
 *                                                              *
 *  Method :                                                    *
 *                                                              *
 *  Returns:                                                    *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY IconDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         return (MRESULT)0L;

      case WM_COMMAND:
         WinDismissDlg(hwnd, TRUE);
         return (MRESULT)0L;

      case WM_PAINT:
      {
         SWP      swpTxt, swpDlg;
         HPS      hps;
         POINTL   ptl;
         HBITMAP  hbm;

         WinDefDlgProc(hwnd, WM_PAINT, mp1, mp2);  /* normal dialog painting */

         hps = WinGetPS(hwnd);                         /* now, add our stuff */
                                                    /* load and draw Example */
         hbm = GpiLoadBitmap(hps, 0, ID_BMP, 0L, 0L);

         WinQueryWindowPos(WinWindowFromID(hwnd, ID_LOGOTEXT), &swpTxt);
         WinQueryWindowPos(hwnd, &swpDlg);
         ptl.x = (LONG)((swpDlg.cx - 95) / 2);
         ptl.y = (LONG)((((swpDlg.cy - swpTxt.y) - 53) / 2) + swpTxt.y);
         WinDrawBitmap(hps,
                       hbm,
                       NULL,
                       &ptl,
                       0L,
                       0L,
                       DBM_IMAGEATTRS);
         WinReleasePS(hps);
         return (MRESULT)0L;
      }
   }
   sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
   return sRC;
}

/****************************************************************\
 *                                                              *
 *  Name:    MultiValueProc(hwnd, msg, mp1, mp2)                *
 *                                                              *
 *  Purpose: This proc handles the multi-valuee EA stuff. Allows*
 *           the fields in a given m-m to be edited.            *
 *  Usage :                                                     *
 *                                                              *
 *  Method : NOTE that this procedure is fully reentrant/       *
 *           recursive in that it calls EditEAValue() which can *
 *           call MultiValueProc. Since PassData information is *
 *           placed in static variables,the ReEnter structure is*
 *           used to hold a dynamic linked  list of values      *
 *           passed intothe proc and thus keeps the  static data*
 *           set to the proper values.                          *
 *                                                              *
 *  Returns: Always returns TRUE thru WinDismissDlg when DONE is*
 *           clicked.                                           *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY MultiValueProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static HOLDFEA *pFEA;                /* Points to the current EA         */
   static PASSDATA FAR *pPDat;          /* Points to the passed Data struct */
   static REENTER *CurEntry;            /* Points to most recent data frame */
   REENTER *ReEntHold = NULL;           /* Temp used to set up the frame    */
   PASSDATA PDat;                       /* Used for call to EditEAValue     */
   USHORT *pusPtr, cnt;                  /* Utility vars                     */
   SHORT  sOffset;
   CHAR   *pInsert, *pValue, *pDelete;
   CHAR   szCodePage[LENUSHORTBUF];
   BYTE   fNeed;                        /* Holds state of need checkbox     */
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         pPDat = PVOIDFROMMP(mp2);                    /* Set up static vars */
         pFEA  = (HOLDFEA *) pPDat->Point;

         GetMem((PPVOID)&ReEntHold, (LONG)sizeof(REENTER));  /* Allows for recusive calls */
         ReEntHold->pPDat = pPDat;
         ReEntHold->pFEA  = pFEA;
         ReEntHold->next  = CurEntry;
         CurEntry = ReEntHold;

         WinSendDlgItemMsg(hwnd, IDD_NEEDBIT, BM_SETCHECK,/* Need bit setup */
                           MPFROM2SHORT((pFEA->fEA&0x80)? TRUE : FALSE,0),
                           NULL);

         WinSetDlgItemText(hwnd, IDD_EATYPE,"");      /* Setup the codepage */
         WinSetDlgItemText(hwnd, IDD_EANAME,pFEA->szName);
         WinSendDlgItemMsg(hwnd, IDD_CODEPAGE,EM_SETTEXTLIMIT,
                                 MPFROM2SHORT(LENUSHORTBUF-1,0),NULL);

         pusPtr= (USHORT *)((CHAR *)pFEA->aValue+pPDat->usMultiOffset);
         pusPtr++;          /* Skip the EA_Type field and point to codepage */
         sprintf(szCodePage,"%u", *pusPtr);
         WinSetDlgItemText(hwnd, IDD_CODEPAGE, szCodePage);

         pusPtr++;            /* Skip codepage and point to the field count */

         for(cnt = 0; cnt < *pusPtr;cnt++)       /* Add each field to the L-Box */
         {
            pInsert = MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset, cnt);
            pValue = EAValueString(pInsert);        /* Ptr to asciiz string */

            WinSendDlgItemMsg(hwnd, IDD_LBOX, LM_INSERTITEM,
                              MPFROM2SHORT(LIT_END,0),
                              MPFROMP(pValue));
            FreeMem(pValue);                          /* Free asciiz string */
         }
         return (MRESULT)0L;

      case WM_CONTROL:
         switch(SHORT1FROMMP(mp1))
         {
            case IDD_LBOX:
               switch(SHORT2FROMMP(mp1))
               {
                  case LN_SELECT:   /* Display proper EA type for selection */
                     sOffset = (SHORT) WinSendDlgItemMsg(hwnd, IDD_LBOX,
                                                  LM_QUERYSELECTION,0,0);
                     if(sOffset<0)
                        break;

                     pValue = MultiTypeIndex(pFEA->aValue+pPDat->usMultiOffset,
                                             sOffset);
                     pusPtr = (USHORT *)pValue;

                     WinSetDlgItemText(hwnd,IDD_EATYPE,
                                ConvTable[LookupEAType(*pusPtr)].szFormat);
                     break;

                  case LN_ENTER:  /* Setup and edit a m-m field */
                     sOffset = (SHORT)WinSendDlgItemMsg(hwnd, IDD_LBOX,
                                                       LM_QUERYSELECTION,0,0);
                     if(sOffset < 0)
                        return (MRESULT)0L;

                     PDat.Point         = (CHAR *) pFEA; /* Pass curr data */
                     PDat.cbMulti       = pPDat->cbMulti;
                     PDat.usMultiOffset = pPDat->usMultiOffset;
                     PDat.usIndex       = sOffset;

                     if(EditEAValue(hwnd,&PDat))     /* They didn't cancel */
                     {
                        pInsert=MultiTypeIndex(pFEA->aValue
                                                + pPDat->usMultiOffset,
                                               sOffset);
                        pValue =EAValueString(pInsert);

                        WinSendDlgItemMsg(hwnd, IDD_LBOX, LM_SETITEMTEXT,
                                          MPFROMSHORT(sOffset),
                                          MPFROMP(pValue));
                        FreeMem(pValue);               /* Free asciiz str */
                     }
                     return (MRESULT)0L;
               }
         }
         break;

      case WM_COMMAND:
         switch(LOUSHORT(mp1))
         {
            case IDD_ADD:                        /* Add an item for a m-m */
               if(!WinDlgBox(HWND_DESKTOP,       /* Do an ascii text edit */
                             hwnd,
                             AsciiEditProc,
                             '\0',
                             IDD_ASCIIEDIT,
                             &PDat))
                     return (MRESULT)0L;              /* They said cancel */
               break;

            case IDD_EDIT:                  /* Setup and edit a m-m field */
               sOffset = (SHORT) WinSendDlgItemMsg(hwnd, IDD_LBOX,
                                                   LM_QUERYSELECTION,0,0);
               if(sOffset < 0)
               {
                  MessageBox(hwnd, IDMSG_ERRORSELECT, "Error !",
                                       MB_OK | MB_ERROR, TRUE);
                  return (MRESULT)0L;
               }

               PDat.Point         = (CHAR *) pFEA;       /* Pass curr data */
               PDat.cbMulti       = pPDat->cbMulti;
               PDat.usMultiOffset = pPDat->usMultiOffset;
               PDat.usIndex       = sOffset;

               if(EditEAValue(hwnd,&PDat))           /* They didn't cancel */
               {
                  pInsert=MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                         sOffset);
                  pValue = EAValueString(pInsert);

                  WinSendDlgItemMsg(hwnd, IDD_LBOX, LM_SETITEMTEXT,
                                    MPFROMSHORT(sOffset),
                                    MPFROMP(pValue));
                  FreeMem(pValue);                      /* Free asciiz str */
               }
               return (MRESULT)0L;

            case IDD_DELETE:                      /* Delete hi-lited item  */
               sOffset = (USHORT) WinSendDlgItemMsg(hwnd, IDD_LBOX,
                                                    LM_QUERYSELECTION,0,0);
               if(sOffset < 0)              /* No item is currently selected */
               {
                  MessageBox(hwnd, IDMSG_ERRORSELECT, "Error !",
                                       MB_OK | MB_ERROR, TRUE);
                  return (MRESULT)0L;
               }
               WinSendDlgItemMsg(hwnd, IDD_LBOX, LM_DELETEITEM,
                                 MPFROMSHORT(sOffset),0L);

               /* Get pointers to start of selected field and the following
                  field, then move the rest of the EA back, resize the
                  buffer, and change the EA Value counter                  */

               pDelete = MultiTypeIndex(pFEA->aValue+pPDat->usMultiOffset,
                                        sOffset);
               pValue  = MultiTypeIndex(pFEA->aValue+pPDat->usMultiOffset,
                                        (sOffset + 1) );
               memmove(pDelete, pValue, pFEA->cbValue - (pValue - pFEA->aValue));
               DosSetMem(pFEA->aValue, pFEA->cbValue - (pValue - pDelete), fPERM | PAG_COMMIT);
               pFEA->cbValue -= pValue - pDelete;

                                    /* Decrement the field count in the m-m */
               pusPtr  = (USHORT *)((CHAR *)pFEA->aValue +
                                              pPDat->usMultiOffset);
               pusPtr += 2;                       /* Point to the field count */
               *pusPtr -= 1;                    /* Decrement the field cnt  */

               FILE_CHANGED = TRUE;
               return (MRESULT)0L;

            case DID_DONE:
                         /* Handle the possible change of the need checkbox */
               fNeed = (BYTE)WinSendDlgItemMsg(hwnd, IDD_NEEDBIT,
                                                BM_QUERYCHECK,
                                                0L, 0L);
               if(fNeed)
                  fNeed = 0x80;
               if(fNeed != (BYTE)(pFEA->fEA & 0x80))       /* Need changed */
               {
                  pFEA->fEA = (BYTE)((pFEA->fEA & 0x7F) | fNeed);
                  FILE_CHANGED = TRUE;
               }

                              /* Handle the possible change of the codepage */
               WinQueryDlgItemText(hwnd, IDD_CODEPAGE,
                                   LENUSHORTBUF, szCodePage);
               sscanf(szCodePage,"%u",&cnt);
               pusPtr= (USHORT *)((CHAR *) pFEA->aValue+pPDat->usMultiOffset);
               pusPtr++;    /* Skip the EA_Type field and point to codepage */
               if(*pusPtr != cnt)        /* They changed the codepage value */
               {
                  *pusPtr = cnt;
                  FILE_CHANGED = TRUE;
               }
               ReEntHold = CurEntry->next;       /* Finish recursion safety */
               FreeMem(CurEntry);
               CurEntry = ReEntHold;
               if(CurEntry)
               {
                  pPDat = CurEntry->pPDat;
                  pFEA  = CurEntry->pFEA;
               }
               WinDismissDlg(hwnd, TRUE);
               return (MRESULT)0L;
         }

      case WM_CLOSE:
         WinDismissDlg(hwnd, FALSE);
         return (MRESULT)0L;
   }
   sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
   return sRC;
}

/****************************************************************\
 *                                                              *
 *  Name:    MultiTypeProc(hwnd, msg, mp1, mp2)                 *
 *                                                              *
 *  Purpose: This proc handles the multi-type EA stuff. Allows  *
 *           the fields in a given m-m to be edited.            *
 *  Usage :                                                     *
 *                                                              *
 *  Method : NOTE that this procedure is fully reentrant/       *
 *           recursive in that it calls EditEAValue() which can *
 *           call MultiTypeProc. Since PassData information is  *
 *           placed in static variables, the ReEnter structure  *
 *           is used to hold a dynamic linked list of values    *
 *           passed into the proc and thus keeps the static data*
 *           set to the proper values.                          *
 *                                                              *
 *  Returns: Always returns TRUE thru WinDismissDlg when DONE is*
 *           clicked.                                           *
 *                                                              *
\****************************************************************/
MRESULT EXPENTRY MultiTypeProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   static HOLDFEA *pFEA;                /* Points to the current EA         */
   static PASSDATA FAR *pPDat;          /* Points to the passed Data struct */
   static REENTER *CurEntry;            /* Points to most recent data frame */
   REENTER *ReEntHold=NULL;             /* Temp used to setup the frame     */
   PASSDATA PDat;                       /* Used for call to EditEAValue     */
   USHORT *pusPtr,cnt;                  /* Utility vars                     */
   SHORT  sOffset;
   CHAR   *pInsert,*pValue,*pDelete;
   CHAR   szCodePage[LENUSHORTBUF];
   BYTE   fNeed;                        /* Holds state of need checkbox     */
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
         FixSysMenu(hwnd);
         pPDat = PVOIDFROMMP(mp2);                    /* Set up static vars */
         pFEA  = (HOLDFEA *) pPDat->Point;

         GetMem((PPVOID)&ReEntHold, (ULONG)sizeof(REENTER));  /* Allows for recusive calls */
         ReEntHold->pPDat = pPDat;
         ReEntHold->pFEA  = pFEA;
         ReEntHold->next  = CurEntry;
         CurEntry = ReEntHold;

         WinSendDlgItemMsg(hwnd, IDD_NEEDBIT, BM_SETCHECK,/* Need bit setup */
                           MPFROM2SHORT((pFEA->fEA&0x80)? TRUE : FALSE,0),
                           NULL);

         WinSetDlgItemText(hwnd, IDD_EATYPE,"");      /* Setup the codepage */
         WinSetDlgItemText(hwnd, IDD_EANAME,pFEA->szName);
         WinSendDlgItemMsg(hwnd, IDD_CODEPAGE,EM_SETTEXTLIMIT,
                                 MPFROM2SHORT(LENUSHORTBUF-1,0),NULL);

         pusPtr= (USHORT *) ((CHAR *) pFEA->aValue+pPDat->usMultiOffset);
         pusPtr++;          /* Skip the EA_Type field and point to codepage */
         sprintf(szCodePage,"%u",*pusPtr);
         WinSetDlgItemText(hwnd, IDD_CODEPAGE,szCodePage);

         pusPtr++;            /* Skip codepage and point to the field count */

         for(cnt = 0; cnt < *pusPtr;cnt++)       /* Add each field to the L-Box */
         {
            pInsert = MultiTypeIndex(pFEA->aValue+pPDat->usMultiOffset,cnt);
            pValue = EAValueString(pInsert);        /* Ptr to asciiz string */

            WinSendDlgItemMsg(hwnd, IDD_LBOX, LM_INSERTITEM,
                              MPFROM2SHORT(LIT_END,0),
                              MPFROMP(pValue));
            FreeMem(pValue);                          /* Free asciiz string */
         }
         return (MRESULT)0L;

      case WM_CONTROL:
         switch(SHORT1FROMMP(mp1))
         {
            case IDD_LBOX:
               switch(SHORT2FROMMP(mp1))
               {
                  case LN_SELECT:   /* Display proper EA type for selection */
                     sOffset = (SHORT)WinSendDlgItemMsg(hwnd, IDD_LBOX,
                                                       LM_QUERYSELECTION,0,0);
                     if(sOffset<0)
                        break;

                     pValue = MultiTypeIndex(pFEA->aValue+pPDat->usMultiOffset,
                                             sOffset);
                     pusPtr = (USHORT *)pValue;

                     WinSetDlgItemText(hwnd,IDD_EATYPE,
                                ConvTable[LookupEAType(*pusPtr)].szFormat);
                     break;

                  case LN_ENTER:  /* Setup and edit a m-m field */
                     sOffset = (SHORT)WinSendDlgItemMsg(hwnd, IDD_LBOX,
                                                       LM_QUERYSELECTION,0,0);
                     if(sOffset<0)
                        return (MRESULT)0L;

                     PDat.Point         = (CHAR *) pFEA; /* Pass curr data */
                     PDat.cbMulti       = pPDat->cbMulti;
                     PDat.usMultiOffset = pPDat->usMultiOffset;
                     PDat.usIndex       = sOffset;

                     if(EditEAValue(hwnd,&PDat))     /* They didn't cancel */
                     {
                        pInsert=MultiTypeIndex(pFEA->aValue
                                                + pPDat->usMultiOffset,
                                               sOffset);
                        pValue =EAValueString(pInsert);

                        WinSendDlgItemMsg(hwnd, IDD_LBOX, LM_SETITEMTEXT,
                                          MPFROMSHORT(sOffset),
                                          MPFROMP(pValue));
                        FreeMem(pValue);               /* Free asciiz str */
                     }
                     return (MRESULT)0L;
               }
         }
         break;

      case WM_COMMAND:
         switch(LOUSHORT(mp1))
         {
            case IDD_ADD:                        /* Add an item for a m-m */
               MultiAdd(hwnd, pFEA,pPDat);
               return (MRESULT)0L;

            case IDD_EDIT:                  /* Setup and edit a m-m field */
               if(ConvTable[usRetEAType].usPrefix != EA_MVST)
               {
                  sOffset = (SHORT)WinSendDlgItemMsg(hwnd, IDD_LBOX,
                                                   LM_QUERYSELECTION,0,0);
                  if(sOffset < 0)
                  {
                     MessageBox(hwnd, IDMSG_ERRORSELECT, "Error !",
                                       MB_OK | MB_ERROR, TRUE);
                     return (MRESULT)0L;
                  }
               } else       sOffset = 0;

               PDat.Point         = (CHAR *)pFEA;       /* Pass curr data */
               PDat.cbMulti       = pPDat->cbMulti;
               PDat.usMultiOffset = pPDat->usMultiOffset;
               PDat.usIndex       = sOffset;

               if(EditEAValue(hwnd,&PDat))           /* They didn't cancel */
               {
                  pInsert=MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                         sOffset);
                  pValue =EAValueString(pInsert);

                  WinSendDlgItemMsg(hwnd, IDD_LBOX, LM_SETITEMTEXT,
                                    MPFROMSHORT(sOffset),
                                    MPFROMP(pValue));
                  FreeMem(pValue);                      /* Free asciiz str */
               }
               return (MRESULT)0L;

            case IDD_DELETE:                      /* Delete hi-lited item  */
               sOffset = (USHORT) WinSendDlgItemMsg(hwnd, IDD_LBOX,
                                                    LM_QUERYSELECTION,0,0);
               if(sOffset < 0)              /* No item is currently selected */
               {
                  MessageBox(hwnd, IDMSG_ERRORSELECT, "Error !",
                                       MB_OK | MB_ERROR, TRUE);
                  return (MRESULT)0L;
               }
               WinSendDlgItemMsg(hwnd, IDD_LBOX, LM_DELETEITEM,
                                 MPFROMSHORT(sOffset),0L);

               /* Get pointers to start of selected field and the following
                  field, then move the rest of the EA back, resize the
                  buffer, and change the EA Value counter                  */

               pDelete = MultiTypeIndex(pFEA->aValue + pPDat->usMultiOffset,
                                        sOffset);
               pValue  = MultiTypeIndex(pFEA->aValue+pPDat->usMultiOffset,
                                        (sOffset + 1) );
               memmove(pDelete,pValue,pFEA->cbValue - (pValue - pFEA->aValue));
               DosSetMem(pFEA->aValue, pFEA->cbValue - (pValue - pDelete), fPERM|PAG_COMMIT);
               pFEA->cbValue -= pValue - pDelete;

                                    /* Decrement the field count in the m-m */
               pusPtr  = (USHORT *)((CHAR *)pFEA->aValue +
                                              pPDat->usMultiOffset);
               pusPtr += 2;                     /* Point to the field count */
               *pusPtr -= 1;                    /* Decrement the field cnt  */

               FILE_CHANGED = TRUE;
               return (MRESULT)0L;

            case DID_DONE:
                         /* Handle the possible change of the need checkbox */
               fNeed = (BYTE)WinSendDlgItemMsg(hwnd, IDD_NEEDBIT,
                                                BM_QUERYCHECK,
                                                0L, 0L);
               if(fNeed)
                  fNeed = 0x80;

               if(fNeed != (BYTE) (pFEA->fEA & 0x80))       /* Need changed */
               {
                  pFEA->fEA = (BYTE)((pFEA->fEA & 0x7F) | fNeed);
                  FILE_CHANGED = TRUE;
               }

                              /* Handle the possible change of the codepage */
               WinQueryDlgItemText(hwnd, IDD_CODEPAGE,
                                   LENUSHORTBUF, szCodePage);
               sscanf(szCodePage,"%u",&cnt);
               pusPtr = (USHORT *)((CHAR *)pFEA->aValue+pPDat->usMultiOffset);
               pusPtr++;    /* Skip the EA_Type field and point to codepage */

               if(*pusPtr != cnt)        /* They changed the codepage value */
               {
                  *pusPtr = cnt;
                  FILE_CHANGED = TRUE;
               }

               ReEntHold = CurEntry->next;       /* Finish recursion safety */
               FreeMem(CurEntry);
               CurEntry = ReEntHold;
               if(CurEntry)
               {
                  pPDat = CurEntry->pPDat;
                  pFEA  = CurEntry->pFEA;
               }
               WinDismissDlg(hwnd, TRUE);
               return (MRESULT)0L;
         }

      case WM_CLOSE:
         WinDismissDlg(hwnd, FALSE);
         return (MRESULT)0L;
   }
   sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
   return sRC;
}

/****************************************************************\
 *                                                              *
 *  Name:    ShowEAType(hwnd)                                   *
 *                                                              *
 *  Purpose: This routine handles the display of the current EA *
 *           type by setting the text in the appropriate dialog *
 *           field.                                             *
 *  Usage  :                                                    *
 *  Method : Routine should be bullet proof as it does its own  *
 *           error checking. It assumes that hwnd points to the *
 *           correct window with the name listbox in it.        *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID ShowEAType(HWND hwnd)
{
   HOLDFEA *pFEA = GetCurFEA(hwnd, pHoldFEA);   /* Points to the selected EA */

   if(!pFEA)                            /* Kill line if nothing is selected */
   {
      memset(szEAType, 0, sizeof(szEAType) );
      WinInvalidateRect(hwndMain, NULL, FALSE);
      return;
   }
   strcpy(szEAType, ConvTable[usRetEAType].szFormat);
   WinInvalidateRect(hwndMain, NULL, FALSE);
   return;
}

/****************************************************************\
 *                                                              *
 *  Name:    EditEA(hwnd)                                       *
 *                                                              *
 *  Purpose: This routine handles the editing of an EA when it  *
 *           is selected from the listbox.  It get the item     *
 *           selected, sets up the PassData structure then calls*
 *           EditEAValue to do the actual edi                   *
 *  Usage  :                                                    *
 *  Method : Routine should be bullet proof as it does its own  *
 *           error checking. It assumes that hwnd points to the *
 *           correct window with the name listbox in it.        *                *
 *                                                              *
 *  Returns: TRUE if the edit was successful.                   *
 *                                                              *
\****************************************************************/
BOOL EditEA(HWND hwnd)
{
   HOLDFEA  *pFEA = pHoldFEA;        /* Pointer for selected EA */
   SHORT    soffset, sCnt;    /* Both used to reference offset of selected EA */
   PASSDATA PData;

   if(!FILE_ISOPEN)
   {
      MessageBox(hwnd, IDMSG_ERRORSELECT, "Error !",
                               MB_OK | MB_ERROR, TRUE);
      return(FALSE);
   }
   soffset = sCnt = (SHORT)WinSendDlgItemMsg(hwnd, IDD_WINLBOX,
                                            LM_QUERYSELECTION,0,0);
   if(soffset< 0)                              /* Nothing is selected */
   {
      MessageBox(hwnd, IDMSG_ERRORSELECT, "Error !",
                               MB_OK | MB_ERROR, TRUE);
      return(FALSE);
   }
   while(sCnt--)                                     /* Get to the right EA */
      pFEA = pFEA->next;

   PData.Point         = (CHAR *)pFEA;          /* Set the pass data struct */
   PData.usMultiOffset = 0;
   PData.cbMulti       = 0;

   if(EditEAValue(hwnd,&PData))                                /* It worked */
   {
      WinSendDlgItemMsg(hwnd, IDD_WINLBOX, LM_SETITEMTEXT,
                        MPFROMSHORT(soffset),MPFROMP(pFEA->szName));
      return(TRUE);
   }
   return(FALSE);
}

/****************************************************************\
 *                                                              *
 * Name:    GetCurFEA(hwnd, pFEA)                               *
 *                                                              *
 * Purpose: This routine returns a pointer to the EA currently  *
 *          highlighted in the list box.                        *
 * Usage  :                                                     *
 * Method : Routine should be bullet proof as it does its own   *
 *          error checking. It assumes that hwnd points to the  *
 *          correct window with the name listbox in it.         *
 *                                                              *
 * Returns: The current EA as determined by querying the l-box  *
 *          selector, returns NULL if there is an error or      *
 *          nothing is selected.                                *
 *                                                              *
\****************************************************************/
HOLDFEA *GetCurFEA(HWND hwnd, HOLDFEA *pFEA)
{
   LONG lOffset = (LONG)WinSendDlgItemMsg(hwnd, IDD_WINLBOX,
                                      LM_QUERYSELECTION, 0, 0);

   if(lOffset == 0xffff)                          /* Nothing is highlighted */
   {
      MessageBox(hwnd, IDMSG_ERRORSELECT, "Error !",
                               MB_OK | MB_ERROR, TRUE);
      return(NULL);
   }
   while(lOffset--)
   {
      pFEA = pFEA->next;
   }
   return pFEA;
}

/****************************************************************\
 *                                                              *
 *  Name:    GetMem (ppv, cb)                                   *
 *                                                              *
 *  Purpose: This routine returns a pointer to a available*     *
 *           memory space.                                      *
 *                                                              *
 *  Usage  :                                                    *
 *  Method : Routine should be bullet proof as it does its own  *
 *           error checking. It assumes that hwnd points to the *
 *           correct window with the name listbox in it.        *
 *                                                              *
 *  Returns: The current EA as determined by querying the l-box *
 *           selector.                                          *
 *                                                              *
\****************************************************************/
VOID GetMem (PVOID *ppv, ULONG cb)
{
    BOOL f = (BOOL) DosAllocMem(ppv, cb, fPERM|PAG_COMMIT);

    if (f)
    {
        MessageBox(HWND_DESKTOP, IDMSG_MEMORYFULL, "Error !",
                               MB_OK | MB_ERROR, TRUE);
        *ppv = NULL;
    }
    return;
}

/****************************************************************\
 *                                                              *
 *  Name:    ResizeMem(ppv, cbOld, cbNew)                       *
 *                                                              *
 *  Purpose: This routine returns a pointer to a available      *
 *           memory space.                                      *
 *                                                              *
 *  Usage  :                                                    *
 *  Method :                                                    *
 *                                                              *
 *  Returns: VOID                                               *
 *                                                              *
\****************************************************************/
VOID ResizeMem (PVOID *ppv, ULONG cbOld, ULONG cbNew)
{
    PVOID       pvNew = NULL;
    BOOL f = (BOOL)DosAllocMem(&pvNew, cbNew, fPERM|PAG_COMMIT);

    if (f) 
    {
        MessageBox(HWND_DESKTOP, IDMSG_MEMORYFULL, "Error !",
                                 MB_OK | MB_ERROR, TRUE);
        *ppv = NULL;
        return;
    }

    memcpy(pvNew, *ppv, cbOld);
    f = (BOOL)DosFreeMem(*ppv);
    *ppv = pvNew;
    return ;
}
