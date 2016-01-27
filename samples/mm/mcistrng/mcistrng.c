/**************************************************************************
 * File Name   : MCISTRNG.C
 *
 * Description : This file contains the C source code required for the
 *               MCISTRNG sample program.  This program can be run in three ways.
 *
 *               1. The user enters MCI String commands from a combination box
 *               and presses the Send pushbutton.
 *               2. The user can enter a batch file which has a series of valid
 *               commands (one command per line) at the command prompt.
 *               3. The user can open a batch file by selecting "Batch file"
 *               push button.
 *
 *               The command is sent to the MCD and the string is placed in the
 *               combination box's list box.
 *
 *               The MCD will take the MCI String Command and process it into
 *               the correct MCI Commands.  Depending on the MCI String
 *               Command the MCD will sent MCI Notification messages back to
 *               the application.  These messages will be filtered as per the
 *               user's instructions so that only certain messages will be
 *               shown.
 *
 * Concepts    : This sample program will show how the MCI String interface
 *               is used.  Basically, the use of the interface involves
 *               sending MCI String Commands to the MCD, and handling the
 *               MCI Notification messages that are received by the Call
 *               Back window.
 *
 * MMPM/2 API's: List of all MMPM/2 API's that are used in
 *               this module
 *
 *                mciSendString
 *                mciGetErrorString
 *
 * Required
 *    Files    : mcistrng.c         Source Code.
 *               mcistrng.h         Include file.
 *               mcistrng.dlg       Dialog definition.
 *               mcistrng.rc        Resources.
 *               mcistrng.ipf       Help text for the Main Dialog.
 *               makefile           Make file.
 *               mcistrng.def       Linker definition file.
 *               mcistrng.ico       MCI String Icon.
 *
 * Copyright (C) IBM 1991, 1993
 ******************************************************************************/

#define  INCL_OS2MM                      /* required for MCI and MMIO headers */
#define  INCL_WIN                        /* Required to use Win APIs.         */
#define  INCL_PM                         /* Required to use PM APIs.          */
#define  INCL_GPIPRIMATIVES              /* Required to use GPI OS/2 APIs.    */
#define  INCL_DOSDATETIME                /* Required for get date time info.  */
#define  INCL_DOSPROCESS                 /* Required for OS/2 process APIs.   */
#define  INCL_WINHELP                    /* Required to use IPF.              */
#define  INCL_SECONDARYWINDOW            /* required for secondary window     */
#define  INCL_WINSTDFILE                 /* required for open file dlg        */
#define  INCL_WINSTDDLGS                 /* required for open file dlg        */

#include <os2.h>
#include <os2me.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sw.h>

#include "mcistrng.h"


/*********************** Procedure/Function Prototypes ************************/
/*
 * This definition is required to use the sample program with the IBM
 * C compiler.
 */
#pragma linkage( MainDialogProc, system )
#pragma linkage( IncludeDialogProc, system )
#pragma linkage( DisplayDialogProc, system )
#pragma linkage( DisplayListBoxProc, system )


MRESULT EXPENTRY MainDialogProc( HWND   hwnd,
                                 ULONG  msg,
                                 MPARAM mp1,
                                 MPARAM mp2 );
MRESULT EXPENTRY IncludeDialogProc( HWND   hwnd,
                                    ULONG  msg,
                                    MPARAM mp1,
                                    MPARAM mp2 );
MRESULT EXPENTRY DisplayDialogProc( HWND   hwnd,
                                    ULONG  msg,
                                    MPARAM mp1,
                                    MPARAM mp2 );
MRESULT EXPENTRY DisplayListBoxProc( HWND   hwnd,
                                     ULONG  msg,
                                     MPARAM mp1,
                                     MPARAM mp2 );
MRESULT EXPENTRY FileDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );

BOOL        GetStringFromComboBox( HWND hwndStringComboBox, PSZ  pszMCIString );
BOOL        IsMCIStringValid( PSZ pszMCIString );
USHORT      ShowAMessage( HWND   hwndOwnerWindow,
                          CHAR   *pcMessageTitle,
                          USHORT usMessageId,
                          ULONG  ulMessageBoxStyle );
VOID        InitializeHelp( VOID );
VOID        SendStringToMCD( HWND hwndComboBox );
VOID        InitializeDialog( VOID );
VOID        TerminateDialog( VOID );
VOID        RunTheBatchFile( HWND hwnd );
VOID        SetSystemMenu( HWND hwndDialogWindow );
VOID        SendScrollBarMessage( HWND  hwndDialogBox,
                                  SHORT sScrollBarMessage );
VOID        SaveMSTInfo( VOID );
BOOL        OpenBatchFile( CHAR szBatchFileName[] );
BOOL        GetBatchFileAndOpenIt( VOID );
VOID        Restore( HWND hwnd );

/******************** End of Procedure/Function Prototypes ********************/


/**************************** Global Variables. *******************************/

/*
 * This array holds the ID's that are used to retrieve the the strings from
 * the string table and placed into the global double array acStringBuffer.
 * The defines that are used for the string table is used to create the index
 * into the global buffer.  Since the string table starts at 1 and the global
 * buffer starts at 0, the index is decremented by 1 when used.  This means
 * when the acStringBuffer is accessed after the strings have been loaded into
 * the buffer, all indexes will be n-1, i.e., IDS_MAIN_HELP_WINDOW_TITLE - 1,
 * to get the proper string.  This is done simply to keep down the number of
 * #defines.
 */
USHORT ausMessageId[] = { IDS_MAIN_WINDOW_TITLE,
                          IDS_MAIN_HELP_WINDOW_TITLE,
                          IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT,
                          IDS_MAIN_HELP_LIBRARY_FILE,
                          IDS_RETURN_STRING,
                          IDS_UNKNOWN,
                          IDS_CONTINUE_BATCHFILE};

HELPINIT hmiHelpStructure;         /* Help initialization structures.     */

/*
 * This variable will hold the address of the window procedure that controls
 * the List Box that is in the Display Messages dialog.  This is required
 * since we subclass the List Box to handle certain user actions.
 */
PFNWP pfnwpDisplayListBoxProc;

/*
 * This array has the ID values for each of the check boxes used in the
 * Include Dialog Box.  These values are used to get the check box window
 * handles.
 */
USHORT ausCheckBoxIDs[] = { ID_NOTIFY_CB,
                            ID_PASSDEVICE_CB,
                            ID_POSITION_CHANGE_PB,
                            ID_EVENT_CB,
                            ID_CUEPOINT_CB,
                            ID_PLAYLIST_CB
                          };

/*
 * This array holds the 'check' state of the check boxes.  TRUE indicates
 * that the check box is 'checked off'.
 */
BOOL afCheckBoxState[] = { TRUE,    /* MM_MCINOTIFY is set.                   */
                           TRUE,    /* MM_MCIPASSDEVICE is set.               */
                           TRUE,    /* MM_MCIPOSITIONCHANGE is set.           */
                           TRUE,    /* MM_MCIEVENT is set.                    */
                           TRUE,    /* MM_MCICUEPOINT is set.                 */
                           TRUE     /* MM_MCIPLAYLISTMESSAGE is set.          */
                         };

BOOL   fDisplayMsgWindowIsUp;       /* for  display message window.           */
BOOL   fNotify = FALSE;             /* flag needed if notify in the string.   */
BOOL   fStringFromBatchFile;        /* source of the string command source    */
USHORT usCountOfMCIStringSent = 1;  /* Number of sent cmds                    */
USHORT usUserParameter;
/*
 * Holds the string result that is returned by the sending of a MCI String
 * Command.
 */
CHAR   acMCIReturnString[ MCI_RETURN_STRING_LENTGH ];

CHAR   acMCIString[ MCI_STRING_LENGTH ];             /* MCI String Command.   */
CHAR   acBatchFileName[CCHMAXPATH] = "";  /* Batch file name.                 */
FILE   *hFile;                           /* Batch file handle.                */
CHAR   szAppName[FILE_NAME_SIZE];        /* buffer for window title text      */
CHAR   szKeyName[] = "SWP";              /* for key name in the os2.ini file  */

/*
 * This array holds program required text that is pulled from the string
 * table.
 */
CHAR   acStringBuffer[ NUMBER_OF_PROGRAM_STRINGS ][ STRING_SIZE ];

HWND   hwndMainDialogBox,              /* Handle to the Main Dialog window.   */
       hwndIncludeDialogBox,           /* Handle to the Include Dialog box.   */
       hwndDisplayDialogBox,           /* Handle to the Display Dialog box.   */
       hwndHelpInstance,               /* Handle to the Help window.          */
       hwndDisplayListBox,             /* Handle to the Display List Box.     */
       hwndFrame,                      /* Handle to the frame window.         */
       hwndSendPB,                     /* Send PB handle.                     */
       hwndComboBox,                   /* Combo Box handle.                   */
       hwndBatchFilePB;                /* Batch file PB handle.               */

HAB    hab;          /* Handle to the Anchor Block for the program.           */
QMSG   qmsg;         /* Structure to hold a message on the message queue.     */
HMQ    hmq;          /* Handle to the message queue.                          */
SWPRESTORE  Save;    /* Structure for the Save and Restore                    */

/*
 * This structure holds text representations for the string commands.  It's
 * used during processing of the MM_MCINOTIFY message in the DisplayDialogProc.
 *
 * These strings are not in the resource file because they should not be
 * translated.  They are string commands, and string commands are not
 * translated.  These strings correspond to command message identifiers
 * listed in mmsystem.h.
 */

char  *szMessage[55] = {
      "",
      "OPEN",
      "CLOSE",
      "ESCAPE",
      "PLAY",
      "SEEK",
      "STOP",
      "PAUSE",
      "INFO",
      "CAPABILITY",
      "STATUS",
      "SPIN",
      "SET",
      "STEP",
      "RECORD",
      "SYSINFO",
      "SAVE",
      "CUE",
      "UPDATE",
      "CUEPOINT",
      "POS_ADVISE",
      "SYNC_OFFSET",
      "LOAD",
      "ACQUIRE",
      "RELEASE",
      "MASTERAUDIO",
      "GETTOC",
      "DEVICESETTINGS",
      "CONNECTOR",
      "RESUME",
      "NOT_DEFINED",
      "CONNECTORINFO",
      "DEFAULT_CONNECTION",
      "CONNECTION",
      "NOT_DEFINED",
      "NOT_DEFINED",
      "NOT_DEFINED",
      "NOT_DEFINED",
      "NOT_DEFINED",
      "NOT_DEFINED",
      "CAPTURE",
      "FREEZE",
      "GETIMAGEBUFFER",
      "GETIMAGEPALETTE",
      "PUT",
      "REALIZE",
      "REWIND",
      "RESTORE",
      "SETIMAGEBUFFER",
      "SETIMAGEPALETTE",
      "UNFREEZE",
      "WHERE",
      "WINDOW"};

/************************** End of Globle Variables ***************************/


/******************************************************************************
 * Name        : main
 *
 * Description : This function calls the InitializeDialog procedure to create
 *               the dialog box, starts up the message loop, and finally calls
 *               TerminateDialog when the program is to end.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : TRUE is returned to the operating system.
 *
 ******************************************************************************/
INT main( argc, argv )
int    argc;
char   *argv[];
{
   /*
    * If using batch file copy. The file will be opened to process in
    * WM_INITDLG of main dialog proc.
    */
   if (argc == 2)
   {
     strcpy(acBatchFileName,argv[1]);
   }

   InitializeDialog();

   WinUpdateWindow( hwndFrame );              /* Force window to paint.       */
   WinSetFocus( HWND_DESKTOP, hwndComboBox);  /* Set focus on the entry field */

   while ( WinGetMsg( hab, (PQMSG) &qmsg, (HWND) NULL, 0, 0) )
      WinDispatchMsg( hab, (PQMSG) &qmsg );

   TerminateDialog();

   return( TRUE );

} /* End of main */


/******************************************************************************
 * Name        : InitializeDialog
 *
 * Description : This function performs the necessary initializations and
 *               setups that are required to show/run a dialog box as a
 *               main window.  The message queue will be created, as will
 *               the dialog box.  Finally the message passing loop will be
 *               started to give messages to the dialog box.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID InitializeDialog( VOID )
{

   USHORT   usI;                         /* Generic Counter.                  */
   CHAR     szDefaultSize[CCHMAXPATH];   /* buffer for default size menu text */

   /*
    * Setup and initialize the dialog window.
    */
   hab = WinInitialize( (USHORT) 0 );

   /*
    * Load the various strings that are required by the program.
    */
   for( usI=0; usI<NUMBER_OF_PROGRAM_STRINGS; usI++)
   {
      WinLoadString(
         hab,                            /* HAB for this dialog box.          */
         (HMODULE) NULL,                 /* Get string from .exe file.        */
         ausMessageId[ usI ],            /* Which string to get.              */
         (SHORT) STRING_SIZE,            /* The size of the buffer.           */
         (PSZ) &acStringBuffer[ usI ] ); /* Buffer to place string.           */
   }

   hmq = WinCreateMsgQueue( hab, 0 );

   WinLoadString(
      WinQueryAnchorBlock (HWND_DESKTOP),
      (HMODULE) NULL,
      IDS_DEFAULTSIZE,
      sizeof(szDefaultSize),
      szDefaultSize);


   hwndFrame =                  /* Returns the handle to the frame.           */
      WinLoadSecondaryWindow(
         HWND_DESKTOP,          /* Parent of the dialog box.                  */
         HWND_DESKTOP,          /* Owner of the dialog box.                   */
         (PFNWP) MainDialogProc,/* 'Window' procedure for the dialog box.     */
         (HMODULE) NULL,        /* Where is the dialog.  Null is EXE file..   */
         ID_MAIN_DIALOG_BOX,    /* Dialog ID.                                 */
         (PVOID) NULL );        /* Creation Parameters for the dialog.        */

   /*
    * Retrieve the handle to the dialog box by specifying the QS_DIALOG flag.
    */
   hwndMainDialogBox = WinQuerySecondaryHWND(hwndFrame, QS_DIALOG);

   /*
    * Add Default Size menu item to system menu of the secondary window.
    */
   WinInsertDefaultSize(hwndFrame, szDefaultSize);

   /*
    * Set the name of the sample program in the title bar of the dialog
    * window.  This is done so that the program name will show up in the
    * Task List window, via the FCF_TASKLIST flag AND will NOT show the
    * .exe file name of the program.  If the FCF_TASKLIST flag is used the
    * .exe file name AND the dialog title will show up in the title bar.
    * This is not wanted.
    */
   WinSetWindowText(
      hwndFrame,
      acStringBuffer[ IDS_MAIN_WINDOW_TITLE - 1 ] );

   /*
    * This window is initially loaded but is NOT visible.  Only by pressing
    * the Display Messages Pushbutton on the main window will this dialog
    * box be displayed.  This is a non-modal dialog window.
    */
   hwndDisplayDialogBox =
      WinLoadDlg(
         HWND_DESKTOP,              /* Parent of the dialog box.              */
         (HWND) NULL,               /* Owner of the dialog box.               */
         (PFNWP) DisplayDialogProc, /* 'Window' procedure for the dialog box. */
         (HMODULE) NULL,            /* Where is the dialog.  Null is EXE file.*/
         ID_DISPLAY_DIALOG_BOX,     /* Dialog ID.                             */
         (PVOID) NULL );            /* Creation Parameters for the dialog.    */

   /*
    * Get the restored windows position information from the os2.ini
    * file before displaying.
    */
   Restore( hwndFrame );

   /*
    * Initialize the help structure and associate the help instance.
    */
   InitializeHelp();

   /*
    * If the Dispay Message Box was up at the time of destroying the
    * main window display it.
    */
   if (Save.fDisplayMsgWindowStatus == TRUE)
        WinShowWindow(
        hwndDisplayDialogBox,
        TRUE );

} /* End of InitializeDialog */


/******************************************************************************
 * Name        : TerminateDialog
 *
 * Description : This routine is called after the message dispatch loop
 *               has ended because of a WM_QUIT message.  The code will
 *               destroy the help instance, messages queue and window.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID TerminateDialog( VOID )
{

   /*
    * Save latest windows position to INI.
    */
   SaveMSTInfo();

   /*
    * Destroy the Help Instance for this dialog window.
    */
   if ( hwndHelpInstance != (HWND) NULL )
   {
      WinDestroyHelpInstance( hwndHelpInstance );
   }

   WinDestroySecondaryWindow( hwndFrame );
   WinDestroyWindow( hwndDisplayDialogBox );
   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );

}  /* End of TerminateDialog */


/******************************************************************************
 * Name        : MainDialogProc
 *
 * Description : This function controls the main dialog box.  It will handle
 *               received messages such as pushbutton notifications, timing
 *               events, etc.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hwnd - Handle for the Main dialog box.
 *               msg  - Message received by the dialog box.
 *               mp1  - Parameter 1 for the just received message.
 *               mp2  - Parameter 2 for the just received message.
 *
 * Return      : 0 or the result of default processing.
 *
 ******************************************************************************/
MRESULT EXPENTRY MainDialogProc( HWND   hwnd,
                                 ULONG  msg,
                                 MPARAM mp1,
                                 MPARAM mp2 )
{
   static PSWP  pswpWindowActionMP    = (PSWP) NULL; /* Action of Window.     */
   static BOOL  fIsTheWindowMinimized = FALSE;       /* State of Window.      */

   HPOINTER hpProgramIcon;                           /* Pointer to Icon.      */
   CHAR szFileNameBuffer[ FILE_NAME_SIZE ];

   switch( msg )
   {
      /*
       * Since the dialog was just started, setup various objects.
       */
      case WM_INITDLG :
         /*
          * Set the sample programs minimize icon.
          */
         hpProgramIcon =
            WinLoadPointer(
               HWND_DESKTOP,
               (HMODULE) NULL, /* Where the resource is kept. (Exe file)      */
               ID_ICON );      /* Which icon to use.                          */

         WinDefSecondaryWindowProc(
            hwnd,              /* Dialog window handle.                       */
            WM_SETICON,        /* Message to the dialog.  Set it's icon.      */
            (MPARAM) hpProgramIcon,
            (MPARAM) 0 );      /* mp2 no value.                               */

         /*
          * Get the handle of the Combination box so that it can be reused
          * in future processing.
          */
         hwndComboBox =
            WinWindowFromID(
               hwnd,
               ID_STRING_COMBO_BOX );

         /*
          * Get the handle of the Send push button.
          */
         hwndSendPB =
            WinWindowFromID(
               hwnd,                   /* Handle of this dialog window.       */
               ID_MAIN_SEND_PB );      /* ID of the left most PM control.     */

         /*
          * Get the handle of the Batch file push button.
          */
         hwndBatchFilePB =
            WinWindowFromID(
               hwnd,                   /* Handle of this dialog window.       */
               ID_BATCH_FILE_PB );      /* ID of the left most PM control.     */

         /*
          * Disable the Send push button until valid text has been placed
          * into the entry field.
          */
         WinEnableWindow(
               hwndSendPB,             /* Handle of this dialog window.       */
               FALSE );                /* ID of the left most PM control.     */

         /*
          * Set the String limit size for the Combo Box's Entry Field.
          */
         WinSendMsg(
            hwndComboBox,
            EM_SETTEXTLIMIT,
            MPFROMLONG( MCI_STRING_LENGTH ),
            0L );

         /*
          * If there is batch file, load the file and post a message to execute.
          */
         if ( strlen(acBatchFileName) > 0)
         {
            if (OpenBatchFile(acBatchFileName))
            {
               /*
                * Disable the Batch file push button until the batch file is
                * processed.
                */
               WinEnableWindow(
                  hwndBatchFilePB,        /* Handle of this dialog window.    */
                  FALSE );                /* ID of the left most PM control.  */
               WinPostMsg( hwnd, WM_BATCHFILE, NULL, NULL );
            }
         }
         return( 0 );

      case WM_CLOSE :
         /*
          * The call to the WinDefSecondaryWindowProc is required so that the
          * dialog box will be closed.  This is needed since the WM_QUIT is
          * not processed in the same manner for dialog boxes and windows.
          * But first cause the Display Messages Dialog to shutdown.
          */
         WinSendMsg(
            hwndDisplayDialogBox,
            WM_QUIT,
            0L,
            0L );

         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case WM_HELP :
         /*
          * The dialog window has received a request for help from the user,
          * i.e., the Help pushbutton was pressed.  Send the HM_DISPLAY_HELP
          * message to the Help Instance with the IPF resource identifier
          * for the correct HM panel.  This will show the help panel for this
          * sample program.
          */
         WinSendMsg(
            hwndHelpInstance,
            HM_DISPLAY_HELP,
            MPFROMSHORT( 1 ),
            MPFROMSHORT( HM_RESOURCEID ) );
         return( 0 );

      case WM_CONTROL :
         switch( SHORT1FROMMP( mp1 ))
         {
            case ID_STRING_COMBO_BOX :
            {
               if ( SHORT2FROMMP( mp1 ) == CBN_EFCHANGE )
               {
                 /*
                  * See if the text in the entry field is greater than zero.
                  */
                 if ( WinQueryWindowText(
                         hwndComboBox,
                         FILE_NAME_SIZE,
                         szFileNameBuffer ) > 0 )
                 {
                    /*
                     * If greater than zero enable the Send push button,
                     */
                    WinEnableWindow(
                       hwndSendPB,
                       TRUE );
                 }
                 else
                 {
                    /*
                     *  Else disable the Send push button.
                     */
                    WinEnableWindow(
                       hwndSendPB,
                       FALSE );
                 }
               }
            } /* endif */
          return( 0 );
        }

      case WM_BATCHFILE :

        RunTheBatchFile(hwnd);
        break;

      case WM_TIMER :

        WinStopTimer( hab, hwnd, 1);
        WinPostMsg( hwnd, WM_BATCHFILE, NULL, NULL );
        break;

      case WM_COMMAND :
         /*
          * To get which pushbutton was pressed the SHORT1FROMMP macro
          * is used.
          */
         switch( SHORT1FROMMP( mp1 ) )
         {
            /*
             * The Send Push Button was pressed on the main dialog window.
             */
            case ID_MAIN_SEND_PB :

               fStringFromBatchFile = FALSE;  /* User typed the string command */
               SendStringToMCD( hwndComboBox );

               return( 0 );

            /*
             * The Display Messages Push Button was pressed on the main
             * dialog window.
             */
            case ID_DISPLAY_MESSAGES_PB :

               /*
                * Show the Display Messages dialog box.  This dialog is always
                * running but is initially made invisible.  Therefore we will
                * make it visible at this point.
                */
               WinShowWindow(
                  hwndDisplayDialogBox,   /* Handle to the window.            */
                  TRUE );                 /* Make the window visible.         */

               fDisplayMsgWindowIsUp = TRUE; /* Visible, set the flag.    */

               WinSetFocus(
                  HWND_DESKTOP,           /* Window is in Desk top.           */
                  hwndDisplayDialogBox ); /* Make the Display dialog in focus.*/

               return( 0 );

            case ID_BATCH_FILE_PB :
               if ( GetBatchFileAndOpenIt() )
               {
                  /*
                   * Disable the Batch file push button until the batch
                   * file is processed.
                   */
                  WinEnableWindow(
                     hwndBatchFilePB,  /* Handle of this dialog window.    */
                     FALSE );          /* ID of the left most PM control.  */
                  WinStartTimer( hab, hwndMainDialogBox, 1, 1000UL);
               }
               return( 0 );

            /*
             * The Cancel Push Button was pressed on the main dialog window.
             * Or the ESC key was pressed.
             */
            case DID_CANCEL:
            case ID_MAIN_CANCEL_PB :
               /*
                * Send a WM_CLOSE message to the sample program so that it
                * closes itself.
                */
               WinSendMsg(
                  hwnd,                   /* Window handle.                   */
                  WM_CLOSE,               /* The message that is sent.        */
                  (MPARAM) NULL,          /* No MP1 parameter.                */
                  (MPARAM) NULL );        /* No MP2 parameter.                */

               return( 0 );

         }  /* End of Command Switch */

         return( 0 );

      case WM_MINMAXFRAME :
      {
         /*
          * The code for this message is required since this sample program
          * is using a dialog box as the main window.  There is a feature
          * of PM that some controls may have to be hidden if an icon is
          * desired when the program is minimized.  If the control is not
          * hidden then it or a piece of it may show up in place of the icon.
          *
          * Therefore we use the handle to the left most control and if the
          * message indicates that the program is to be minimized then we
          * will hide the control.  When the program is Restored the control
          * will be shown.
          */

         pswpWindowActionMP = (PSWP) LONGFROMMP( mp1 );

         if ( pswpWindowActionMP->fl & SWP_MINIMIZE )
         {
            WinShowWindow(
               hwndSendPB,             /* Handle to the control.              */
               FALSE );                /* Hide the control window.            */

           /*
            * If Display Message Window is visible, hide it.
            */
           if (fDisplayMsgWindowIsUp == TRUE)
              WinShowWindow(
                 hwndDisplayDialogBox,
                 FALSE );
         }
         else
         {
            WinShowWindow(
               hwndSendPB,             /* Handle to the control.              */
               TRUE );                 /* Show the control window.            */

           /*
            * If Display Message Window was visible at the time of minimizing
            * the main window, make it visible.
            */

           if (fDisplayMsgWindowIsUp == TRUE)
              WinShowWindow(
                 hwndDisplayDialogBox,
                 TRUE );
         }

         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );
      }  /* End of MINMAXFRAM */

      default:
          return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

   }  /* End of Switch */

   return( (MRESULT) FALSE );

} /* End of MainDialogProc */


/******************************************************************************
 * Name        : DisplayDialogProc
 *
 * Description : This function controls the Display dialog box.  It will
 *               handle received messages such as pushbutton notifications,
 *               timing events, etc.  This dialog is created with the Main
 *               dialog box and is not destroyed until the termination of the
 *               Main window.  The window is kept alive so that it can
 *               process arriving MCI Notification messages.
 *
 *               The following messages are handled specifically by this
 *               routine.
 *
 *                  WM_INITDLG
 *                  WM_CLOSE
 *                  WM_HELP
 *                  WM_COMMAND
 *                  WM_MINMAXFRAME
 *
 * Concepts    : Handling of arriving MCI Notification messages.
 *
 * MMPM/2 API's: MM_MCINOTIFY
 *               MM_MCIPASSDEVICE
 *               MM_MCIPOSITIONCHANGE
 *               MM_MCIEVENT
 *               MM_MCICUEPOINT
 *               MM_MCIPLAYLISTMESSAGE
 *
 * Parameters  : hwnd - Handle for the Display dialog box.
 *               msg  - Message received by the dialog box.
 *               mp1  - Parameter 1 for the just received message.
 *               mp2  - Parameter 2 for the just received message.
 *
 * Return      : 0 or the result of default processing.
 *
 ******************************************************************************/
MRESULT EXPENTRY DisplayDialogProc( HWND   hwnd,
                                    ULONG  msg,
                                    MPARAM mp1,
                                    MPARAM mp2 )
{

   /*
    * This array will hold the text that is used for the MCI Notification
    * messages.
    */
   static CHAR acMCINotMessage[ NUMBER_OF_NOTIFICATION_MESSAGES ]
                                       [ STRING_SIZE ];

   USHORT usI = 0,                             /* Generic Counter.            */
          usJ = 0;                             /* Generic Counter.            */
   CHAR   acTempString[ STRING_SIZE ];         /* Temp string buffer.         */
   CHAR   acErrorBuf[20];                      /* Another temp buffer.        */
   CHAR   acIDBuf[20];                         /* Another temp buffer.        */
   CHAR   acUserParmBuf[20];                   /* Another temp buffer.        */
   CHAR   acTimeBuf[20];                       /* Another temp buffer.        */

   switch( msg )
   {

      case WM_INITDLG :
         /*
          * Hide the window until it is called.
          */
         WinShowWindow(
            hwnd,
            FALSE );

         /*
          * Set the Child Dialog window's System Menu.
          */
         SetSystemMenu( hwnd );

         /*
          * Load the text for the MCI Notification Messages.  This text
          * will be shown in the Display Dialog Box's list box.
          */
         for( usI=STARTING_IDS_VALUE_FOR_MCI_NOTIFICATION_MESSAGES,usJ = 0;
              usI<STARTING_IDS_VALUE_FOR_MCI_NOTIFICATION_MESSAGES +
                     NUMBER_OF_NOTIFICATION_MESSAGES;
              usI++)
         {
            /*
             * Get the string from the Resource defined string table and
             * show it in the message box.
             */
             WinLoadString(
                hab,                     /* HAB for this dialog box.      */
                (HMODULE) NULL,          /* The string is in the exe file.*/
                usI,                     /* Which string to get.          */
                (SHORT) STRING_SIZE,     /* The size of the buffer.       */
                acMCINotMessage[ usJ ]); /* String Buffer                 */
            usJ++;
         }  /* End of For that loads the MCI message text. */

         /*
          * Get the handle to the list box that is in the Display Dialog.
          */
         hwndDisplayListBox =
            WinWindowFromID(
               hwnd,
               ID_DISPLAY_CONTROL );

         /*
          * Subclass the Display Dialog's list box so that certain actions
          * can be ignored.  Some user interactions with this List Box, i.e.,
          * selection of items, etc. will be trapped and ignored by the
          * the sample program to meet CUA requirements.
          */
         pfnwpDisplayListBoxProc =
            WinSubclassWindow(
               hwndDisplayListBox,
               (PFNWP)DisplayListBoxProc );

         return( 0 );

      case WM_CLOSE :
         /*
          * Since this window is not destroyed until the main dialog box
          * closes, this WM_CLOSE message will simply hide the window.
          */
         WinShowWindow(
            hwnd,                     /* Handle of the window to hide.        */
            FALSE );                  /* Hide the window.                     */

         fDisplayMsgWindowIsUp = FALSE;     /* Not visible, set the flag. */

         /*
          * Set the focus to the Main Dialog window.
          */
         WinSetFocus(
            HWND_DESKTOP,             /* Use window in DESKTOP.               */
            hwndMainDialogBox );      /* Set focus on the Main Dialog.        */

         WinSetFocus( HWND_DESKTOP, hwndComboBox);

         return( 0 );

      case WM_HELP :
         /*
          * The dialog window has received a request for help from the user,
          * i.e., the Help pushbutton was pressed.  Send the HM_DISPLAY_HELP
          * message to the Help Instance with the IPF resource identifier
          * for the correct HM panel.  This will show the help panel for this
          * sample program.
          */
         WinSendMsg(
            hwndHelpInstance,
            HM_DISPLAY_HELP,
            MPFROMSHORT( 2 ),
            MPFROMSHORT( HM_RESOURCEID ) );
         return( 0 );

      case WM_COMMAND :
         /*
          * To get which pushbutton was pressed the SHORT1FROMMP macro
          * is used.
          */
         switch( SHORT1FROMMP( mp1 ) )
         {
            /*
             * The Include pushbutton was pressed.  If the Include dialog
             * window is NOT currently in view then show it.  However if is
             * already shown then this message will be ignored.
             */
            case ID_DISPLAY_INCLUDE_PB :

               /*
                * Call the Include dialog window.  This call will disable
                * the Display dialog until the Include dialog is closed.
                */
               WinDlgBox(
                  HWND_DESKTOP,                /* Parent of the dialog box.   */
                  hwndDisplayDialogBox,        /* Owner of the dialog box.    */
                  (PFNWP) IncludeDialogProc,   /* Dialog box procedure.       */
                  (HMODULE) NULL,              /* Dialog is where, EXE file.  */
                  ID_INCLUDE_DIALOG_BOX,       /* Dialog ID.                  */
                  (PVOID) NULL );              /* Dialog Creation Parameters. */

               hwndIncludeDialogBox =
                  WinWindowFromID(
                     hwnd,                      /* Parent of the dialog box.  */
                     ID_INCLUDE_DIALOG_BOX );   /* Dialog ID.                 */

               return( 0 );

            /*
             * The Cancel pushbutton was pressed, or the ESC key was pressed.
             * This will just cause the window to be hidden while the focus
             * is given back to the Main dialog box.
             */
            case DID_CANCEL:
            case ID_DISPLAY_CANCEL_PB :
               /*
                * Hide the Display Messages Dialog Box.
                */
               WinShowWindow(
                  hwnd,                     /* Handle of the window to hide.  */
                  FALSE );                  /* Hide the window.               */

               fDisplayMsgWindowIsUp = FALSE; /* Not visible, set the flag.*/

               WinSetFocus(
                  HWND_DESKTOP,             /* Use window in DESKTOP.         */
                  hwndMainDialogBox );      /* Set focus on the Main Dialog.  */

               WinSetFocus( HWND_DESKTOP, hwndComboBox);

               return( 0 );

         }  /* End of Command Switch */

         return( 0 );

      /*
       * The following MCI Notification messages will be caused by the sending
       * of MCI String Commands from the Main Dialog window.  ALL of the MCI
       * messages perform a check to see if the user wishes to see the arrival
       * of the MCI messages.  This is filtered through the Include Dialog box.
       * If the user has indicated that the messages are to be viewed, the
       * MCI Notification Messages are placed into the List Box of this window.
       */
      case MM_MCINOTIFY :

         /*
          * Check to see if the user has filtered out this message.
          */

         if ( afCheckBoxState[ ID_MM_MCINOTIFY ] == TRUE )
         {
            /*
             * If the user has not filtered out this message, then display
             * the message in the list box of the Display Messages window.
             */

            WinSendMsg( hwndDisplayListBox,             /* Handle to the lb  */
                        LM_INSERTITEM,                  /* Action for the lb */
                        MPFROMSHORT( LIT_END ),         /* Order             */
                        MPFROMP( acMCINotMessage[ ID_MM_MCINOTIFY ] ));

            /* Not only do we want to display the type of message received,
             * we also want to display the information received with the
             * message, because it can be very helpful in debugging media
             * drivers.
             *
             * First display the command message in the list box.
             */

             strcpy ( acTempString, acMCINotMessage[ ID_CMD_MSG ] );

             /*
              * If the command message string is in the szMessage array
              * display it, else display UNKNOWN.
              */
             if ( HIUSHORT(mp2) <= 55 )
                strcat ( acTempString, szMessage[ HIUSHORT(mp2) ] );
             else
                strcat ( acTempString, acStringBuffer[ IDS_UNKNOWN - 1 ] );

             WinSendMsg( hwndDisplayListBox,
                         LM_INSERTITEM,
                         MPFROMSHORT( LIT_END ),
                         MPFROMP( acTempString ));

            /*
             * Next display the notify code.
             */

            if (LOUSHORT(mp1) == MCI_NOTIFY_SUPERSEDED)
               WinSendMsg( hwndDisplayListBox,
                           LM_INSERTITEM,
                           MPFROMSHORT( LIT_END ),
                           MPFROMP(acMCINotMessage[ID_MCI_NOTIFY_SUPERSEDED]));

            else if (LOUSHORT(mp1) == MCI_NOTIFY_ABORTED)
               WinSendMsg( hwndDisplayListBox,
                           LM_INSERTITEM,
                           MPFROMSHORT( LIT_END ),
                           MPFROMP(acMCINotMessage[ID_MCI_NOTIFY_ABORTED]));

            else if (LOUSHORT(mp1) == MCI_NOTIFY_SUCCESSFUL)
               WinSendMsg( hwndDisplayListBox,
                           LM_INSERTITEM,
                           MPFROMSHORT( LIT_END ),
                           MPFROMP(acMCINotMessage[ID_MCI_NOTIFY_SUCCESSFUL]));

            else /* it's MCI_NOTIFY_ERROR */
               {
                  strcpy(acTempString,acMCINotMessage[ID_MCI_NOTIFY_ERROR] );
                  _itoa(LOUSHORT(mp1), acErrorBuf, 10);
                  strcat(acTempString, acErrorBuf);
                  WinSendMsg( hwndDisplayListBox,
                              LM_INSERTITEM,
                              MPFROMSHORT( LIT_END ),
                              MPFROMP( acTempString ));
               }

            /*
             * Next, display the device ID.
             */

            strcpy (acTempString, acMCINotMessage[ID_DEVICE_ID]);
            _itoa(LOUSHORT(mp2), acIDBuf, 10);
            strcat (acTempString, acIDBuf);
            WinSendMsg( hwndDisplayListBox,
                        LM_INSERTITEM,
                        MPFROMSHORT( LIT_END ),
                        MPFROMP( acTempString ));

            /*
             * Finally, display the user parameter.
             */

            strcpy (acTempString, acMCINotMessage[ID_USER_PARM]);
            _itoa(HIUSHORT(mp1), acUserParmBuf, 10);
            strcat (acTempString, acUserParmBuf);
            WinSendMsg( hwndDisplayListBox,
                        LM_INSERTITEM,
                        MPFROMSHORT( LIT_END ),
                        MPFROMP( acTempString ));
         }

         /*
          * When processing a command (with notify flag) from a batch file,
          * we don't want to process the next command until the current
          * one completes. These tests check to see if this notification
          * message is a result of one of the string commands from a batch
          * file. If it is, then, we can go on to process the next string
          * command from the batch file.
          *
          * Check to see the fNotify flag is true. Then check to see the
          * variable usUserParameter equals to the User Parameter returned.
          * If the both conditions are met post a message to read the next
          * string command from the batch file.
          */
         if ((fNotify) && ( usUserParameter == HIUSHORT(mp1)))
         {
            fNotify = FALSE;
            WinStartTimer( hab, hwndMainDialogBox, 1, 1000UL);
         }
         return( 0 );

      case MM_MCIPASSDEVICE :

         /*
          * Check to see if the user has filtered out this message.
          */
         if ( afCheckBoxState[ ID_MM_MCIPASSDEVICE] == TRUE )
         {
            /*
             * If the user has not filtered out this message, then display
             * the message in the list box of the Display Messages window.
             */

            WinSendMsg( hwndDisplayListBox,             /* Handle to the lb  */
                        LM_INSERTITEM,                  /* Action for the lb */
                        MPFROMSHORT( LIT_END ),         /* Order             */
                        MPFROMP( acMCINotMessage[ ID_MM_MCIPASSDEVICE ] ));

            /* Not only do we want to display the type of message received,
             * we also want to display the information received with the
             * message, because it can be very helpful in debugging media
             * drivers.
             *
             * First display the ID of the device.
             */
            strcpy (acTempString, acMCINotMessage[ID_DEVICE_ID]);
            _itoa(LOUSHORT(mp1), acIDBuf, 10);
            strcat (acTempString, acIDBuf);
            WinSendMsg( hwndDisplayListBox,
                        LM_INSERTITEM,
                        MPFROMSHORT( LIT_END ),
                        MPFROMP( acTempString ));

            /*
             * Next, determine if we're gaining use, or losing use.
             */
            if ((ULONG)mp2 == MCI_LOSING_USE)
               WinSendMsg( hwndDisplayListBox,
                           LM_INSERTITEM,
                           MPFROMSHORT( LIT_END ),
                           MPFROMP(acMCINotMessage[ID_LOSING_USE]));
            else
               WinSendMsg( hwndDisplayListBox,
                           LM_INSERTITEM,
                           MPFROMSHORT( LIT_END ),
                           MPFROMP(acMCINotMessage[ID_GAINING_USE]));
         }

         return( 0 );

      case MM_MCIPOSITIONCHANGE :

         /*
          * Check to see if the user has filtered out this message.
          */
         if ( afCheckBoxState[ ID_MM_MCIPOSITIONCHANGE ] == TRUE )
         {
            /*
             * If the user has not filtered out this message, then display
             * the message in the list box of the Display Messages window.
             */

            WinSendMsg( hwndDisplayListBox,            /* Handle to the lb  */
                        LM_INSERTITEM,                 /* Action for the lb */
                        MPFROMSHORT( LIT_END ),        /* Order             */
                        MPFROMP( acMCINotMessage[ID_MM_MCIPOSITIONCHANGE] ));

            /* Not only do we want to display the type of message received,
             * we also want to display the information received with the
             * message, because it can be very helpful in debugging media
             * drivers.
             *
             * First display the ID of the device.
             */

            strcpy (acTempString, acMCINotMessage[ID_DEVICE_ID]);
            _itoa(HIUSHORT(mp1), acIDBuf, 10);
            strcat (acTempString, acIDBuf);
            WinSendMsg( hwndDisplayListBox,
                        LM_INSERTITEM,
                        MPFROMSHORT( LIT_END ),
                        MPFROMP( acTempString ));

            /*
             * Next, display the media position (time).
             */
            strcpy (acTempString, acMCINotMessage[ID_TIME]);
            _itoa((ULONG)mp2, acTimeBuf, 10);
            strcat (acTempString, acTimeBuf);
            WinSendMsg( hwndDisplayListBox,
                        LM_INSERTITEM,
                        MPFROMSHORT( LIT_END ),
                        MPFROMP( acTempString ));

            /*
             * Finally, display the user parameter.
             */
            strcpy (acTempString, acMCINotMessage[ID_USER_PARM]);
            _itoa(LOUSHORT(mp1), acUserParmBuf, 10);
            strcat (acTempString, acUserParmBuf);
            WinSendMsg( hwndDisplayListBox,
                        LM_INSERTITEM,
                        MPFROMSHORT( LIT_END ),
                        MPFROMP( acTempString ));
         }
         return( 0 );

      case MM_MCICUEPOINT :

         /*
          * Check to see if the user has filtered out this message.
          */
         if ( afCheckBoxState[ ID_MM_MCICUEPOINT ] == TRUE )
         {
            /*
             * If the user has not filtered out this message, then display
             * the message in the list box of the Display Messages window.
             */
            WinSendMsg( hwndDisplayListBox,            /* Handle to the lb  */
                        LM_INSERTITEM,                 /* Action for the lb */
                        MPFROMSHORT( LIT_END ),        /* Order             */
                        MPFROMP( acMCINotMessage[ID_MM_MCICUEPOINT] ));

            /* Not only do we want to display the type of message received,
             * we also want to display the information received with the
             * message, because it can be very helpful in debugging media
             * drivers.
             *
             * First display the ID of the device.
             */
            strcpy (acTempString, acMCINotMessage[ID_DEVICE_ID]);
            _itoa(HIUSHORT(mp1), acIDBuf, 10);
            strcat (acTempString, acIDBuf);
            WinSendMsg( hwndDisplayListBox,
                        LM_INSERTITEM,
                        MPFROMSHORT( LIT_END ),
                        MPFROMP( acTempString ));

            /*
             * Next, display the media position (time).
             */
            strcpy (acTempString, acMCINotMessage[ID_TIME]);
            _itoa((ULONG)mp2, acTimeBuf, 10);
            strcat (acTempString, acTimeBuf);
            WinSendMsg( hwndDisplayListBox,
                        LM_INSERTITEM,
                        MPFROMSHORT( LIT_END ),
                        MPFROMP( acTempString ));

            /*
             * Finally, display the user parameter.
             */
            strcpy (acTempString, acMCINotMessage[ID_USER_PARM]);
            _itoa(LOUSHORT(mp1), acUserParmBuf, 10);
            strcat (acTempString, acUserParmBuf);
            WinSendMsg( hwndDisplayListBox,
                        LM_INSERTITEM,
                        MPFROMSHORT( LIT_END ),
                        MPFROMP( acTempString ));
         }
         return( 0 );

      case MM_MCIEVENT :

         /*
          * Check to see if the user has filtered out this message.
          */
         if ( afCheckBoxState[ ID_MM_MCIEVENT ] == TRUE )
         {
            /*
             * If the user has not filtered out this message, then display
             * the message in the list box of the Display Messages window.
             *
             */
            WinSendMsg( hwndDisplayListBox,            /* Handle to the lb  */
                        LM_INSERTITEM,                 /* Action for the lb */
                        MPFROMSHORT( LIT_END ),        /* Order             */
                        MPFROMP( acMCINotMessage[ID_MM_MCIEVENT] ));
         }
         return( 0 );

      case MM_MCIPLAYLISTMESSAGE :
         /*
          * Check to see if the user has filtered out this message.
          */
         if ( afCheckBoxState[ ID_MM_MCIPLAYLISTMESSAGE ] == TRUE )
         {
            /*
             * If the user has not filtered out this message, then
             * display the message in the list box of the Display
             * Messages window.
             */
            WinSendMsg( hwndDisplayListBox,            /* Handle to the lb  */
                        LM_INSERTITEM,                 /* Action for the lb */
                        MPFROMSHORT( LIT_END ),        /* Order             */
                        MPFROMP( acMCINotMessage[ID_MM_MCIPLAYLISTMESSAGE] ));
         }
         return( 0 );

      default:
          return( WinDefDlgProc( hwnd, msg, mp1, mp2 ) );

   }  /* End of Switch */

   return( (MRESULT) FALSE );

} /* End of DisplayDialogProc */


/******************************************************************************
 * Name        : DisplayListBoxProc
 *
 * Description : This function controls the Display Messages dialog box's
 *               list box. The list box window has been subclassed when the
 *               Display Dialog box was created.  This is required since the
 *               user is not allowed to select any text displayed in the list
 *               box via the keyboard or the mouse.  Therefore this window
 *               procedure will trap all appropriate keyboard and mouse
 *               actions so that no selections can be made.  All other
 *               messages received by this function that do not involve
 *               the relevant mouse or keyboard actions are passed back to
 *               the original List Box window procedure for processing.
 *
 *               The following messages are handled specifically by this
 *               routine.
 *               WM_BUTTON1DBLCLK
 *               WM_BUTTON1DOWN
 *               WM_CHAR
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hwnd - Handle for the Display dialog box.
 *               msg  - Message received by the dialog box.
 *               mp1  - Parameter 1 for the just received message.
 *               mp2  - Parameter 2 for the just received message.
 *
 * Return      : 0 or the result of default processing.
 *
 ******************************************************************************/
MRESULT EXPENTRY DisplayListBoxProc( HWND   hwnd,
                                     ULONG  msg,
                                     MPARAM mp1,
                                     MPARAM mp2 )
{
   static RECTL    rctListBox,                  /* Placement of the List Box. */
                   rctForbiddenMouseArea;       /* Area where mouse is forbid.*/
   static BOOL     fHasThisBeenDoneYet = FALSE; /* Indicates that first time  */
                                                /* into this routine.         */

   SHORT sMouseXPosition,                       /* Mouse X position.          */
         sMouseYPosition;                       /* Mouse Y position.          */

   switch( msg )
   {

      /*
       * If the message indicates a mouse action that tries to select
       * text that is in the list box, reject the action.
       */
      case WM_BUTTON1DBLCLK :
      case WM_BUTTON1DOWN :
      {
         /*
          * The first time into this routine some initialization will be
          * performed.  Basically get the area in which the list box resides
          * so that any mouse actions in that region can be rejected.  Since
          * the control has already been created, the WM_INITDLG message
          * will not be received by this routine.  Therefore the first time
          * in this procedure we must do some initial processing.
          */
         if ( fHasThisBeenDoneYet == FALSE )
         {
            fHasThisBeenDoneYet = TRUE;

            WinQueryWindowRect(
               hwnd,                               /* Handle to the window.   */
               (PRECTL) &rctListBox );             /* Size of the window.     */

            /*
             * Calculate the area in which mouse/cursor selections are not
             * going to be allowed.
             */
            rctForbiddenMouseArea.xLeft   =
               rctListBox.xLeft + DISPLAY_CONTROL_X_POSITION;

            rctForbiddenMouseArea.yBottom =
               rctListBox.yBottom + DISPLAY_CONTROL_Y_POSITION;

            rctForbiddenMouseArea.xRight  =
               rctListBox.xRight + DISPLAY_CONTROL_X_POSITION;

            rctForbiddenMouseArea.yTop    =
               rctListBox.yTop + DISPLAY_CONTROL_Y_POSITION;

         }  /* End of the initial setup for this window procedure. */

         /*
          * Set the focus on the Display Messages dialog.
          */
         WinSetFocus(
            HWND_DESKTOP,
            hwndDisplayDialogBox );

         /*
          * Get the position of the mouse in the window.
          */
         sMouseXPosition = SHORT1FROMMP( mp1 );
         sMouseYPosition = SHORT2FROMMP( mp1 );

         /*
          * See if the mouse is in the forbidden.  If it is, ignore the
          * mouse action.
          */
         if ( sMouseXPosition < (SHORT) rctForbiddenMouseArea.xRight  ||
              sMouseXPosition > (SHORT) rctForbiddenMouseArea.xLeft   ||
              sMouseYPosition < (SHORT) rctForbiddenMouseArea.yTop    ||
              sMouseYPosition > (SHORT) rctForbiddenMouseArea.yBottom  )
         {
            return( 0 );
         }

         return( (pfnwpDisplayListBoxProc( hwnd, msg, mp1, mp2 )) );

      }  /* End of Mouse Button catches. */

      /*
       * A keyboard key has been pressed by the user.  We need to check
       * to see if the key(s) are up/down cursor keys, space bar,  AND if
       * the focus of the window is the List Box within the Display Dialog.
       */
      case WM_CHAR :
      {
         /*
          * Find out which key was pressed.  We only care about cursor keys
          * AND the Space Bar.  The space bar is to be ignored since if it
          * was passed into the default window procedure it would be used
          * and a List Box item would be selected.
          */
         switch( SHORT2FROMMP( mp2 ) )
         {
            case VK_SPACE :
            {
               return( 0 );
            }

            case VK_UP :
            {
               SendScrollBarMessage(
                  hwnd,
                  SB_LINEUP );

                  return( 0 );
            }  /* End of VK_UP */

            case VK_DOWN :
            {
               SendScrollBarMessage(
                  hwnd,
                  SB_LINEDOWN );

                  return( 0 );
            }  /* End of VK_DOWN. */

            case VK_PAGEUP   :
            {
               SendScrollBarMessage(
                  hwnd,
                  SB_PAGEUP );

               return( 0 );

            }  /* End of VK_PAGEUP. */

            case VK_PAGEDOWN :
            {
               SendScrollBarMessage(
                  hwnd,
                  SB_PAGEDOWN );

                  return( 0 );

            }  /* End of VK_PAGEDOWN. */

         }  /* End of CHAR switch. */

      }  /* End of WM_CHAR processing. */

      default:
          return( (pfnwpDisplayListBoxProc( hwnd, msg, mp1, mp2 )) );

   }  /* End of Switch */

   return( (pfnwpDisplayListBoxProc( hwnd, msg, mp1, mp2 )) );

}  /* End of DisplayListBoxProc. */


/*******************************************************************************
 * Name        : IncludeDialogProc
 *
 * Description : This function controls the Include dialog box.  It will
 *               allow, filter, MCI Notification messages to received by
 *               the Display Messages dialog.  The dialog window is
 *               application modal.
 *
 *               The following messages are handled specifically by this
 *               routine.
 *
 *                  WM_INITDLG
 *                  WM_CLOSE
 *                  WM_HELP
 *                  WM_COMMAND
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hwnd - Handle for the Include dialog box.
 *               msg  - Message received by the dialog box.
 *               mp1  - Parameter 1 for the just received message.
 *               mp2  - Parameter 2 for the just received message.
 *
 * Return      : 0 or the result of default processing.
 *
 ******************************************************************************/
MRESULT EXPENTRY IncludeDialogProc( HWND   hwnd,
                                    ULONG  msg,
                                    MPARAM mp1,
                                    MPARAM mp2 )
{
   /*
    * Holds the status of the checkboxes.
    */
   static HWND hwndCheckBoxes[ INCLUDE_NUMBER_OF_CHECKBOXES ];

   USHORT usI,                            /* Generic Counter.                 */
          usRC;                           /* Generic Return Code variable.    */

   BOOL   fOneCheckBoxWasChecked = FALSE; /* Indicates if a checkbox is used. */
   BOOL   afTempCheckBoxState[
             INCLUDE_NUMBER_OF_CHECKBOXES ]; /* Temp buffer for CB state.     */

   switch( msg )
   {

      case WM_INITDLG :

         /*
          * Set the Child Dialog window's System Menu.
          */
         SetSystemMenu( hwnd );

         /*
          * Get the window handles for the Check Boxes.  The handles
          * will be used to set the initial values in the Check Boxes
          * when the dialog is first started.
          */
         for( usI=0; usI<INCLUDE_NUMBER_OF_CHECKBOXES; usI++ )
         {
            hwndCheckBoxes[ usI ] =
               WinWindowFromID(
                  hwnd,
                  ausCheckBoxIDs[ usI ] );
         }

         /*
          * Set the state of the check boxes.
          */
         for( usI=0; usI<INCLUDE_NUMBER_OF_CHECKBOXES; usI++ )
         {
             WinSendMsg(
               hwndCheckBoxes[ usI ],
               BM_SETCHECK,
               MPFROM2SHORT( afCheckBoxState[ usI ], 0 ),
               0L );
         }  /* End of FOR that sets the Check Boxes. */

         return( 0 );

      case WM_CLOSE :
         /*
          * The Include dialog is closing.
          */
         WinDismissDlg(
            hwnd,                     /* Dialog box to close.                 */
            TRUE );                   /* Get rid of the dialog box.           */

         return( 0 );

      case WM_HELP :
         /*
          * The dialog window has received a request for help from the user,
          * i.e., the Help pushbutton was pressed.  Send the HM_DISPLAY_HELP
          * message to the Help Instance with the IPF resource identifier
          * for the correct HM panel.  This will show the help panel for this
          * sample program.
          */
         WinSendMsg(
            hwndHelpInstance,
            HM_DISPLAY_HELP,
            MPFROMSHORT( 3 ),
            MPFROMSHORT( HM_RESOURCEID ) );
         return( 0 );

      case WM_COMMAND :
         /*
          * To get which pushbutton was pressed, the SHORT1FROMMP macro
          * is used.
          */
         switch( SHORT1FROMMP( mp1 ) )
         {
            /*
             * The include pushbutton has been pressed therefore get the
             * state of the checkboxes so that the Display Messages dialog
             * can determine if MCI Notification messages are to be displayed.
             */
            case ID_INCLUDE_PB :
               /*
                * Query the checkboxes to get their current state...
                * - Set the flag that at least one checkbox has been used.
                * - Read the state of all of the checkboxes storing the result.
                * - If one of the checkboxes has been checked set the flag.
                *   This is done so that a message box can be shown indicating
                *   that NO checkboxes have been used.
                */
               fOneCheckBoxWasChecked = FALSE;

               for( usI=0; usI<INCLUDE_NUMBER_OF_CHECKBOXES; usI++ )
               {
                  afTempCheckBoxState[ usI ] = (BOOL)
                     WinSendMsg(
                        hwndCheckBoxes[ usI ],
                        BM_QUERYCHECK,
                        0L,
                        0L );

                  if ( afTempCheckBoxState[ usI ] == TRUE )
                  {
                     fOneCheckBoxWasChecked = TRUE;
                  }
               }  /* End of FOR loop that gets the state of the Check Boxes. */

               /*
                * Set the return code so that the dialog will be dismissed
                * if everything is ok.
                */
               usRC = MBID_OK;

               /*
                * See if any of the checkboxes are checked.  If not then tell
                * the user that no MCI Notification messages will be seen.
                */
               if ( fOneCheckBoxWasChecked == FALSE )
               {

                  usRC =
                     ShowAMessage(
                         hwnd,                           /* Owner Window.     */
                         acStringBuffer[ IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
                         IDS_ALL_CHECKBOXES_ARE_FALSE,   /* ID of the message.*/
                         MB_OKCANCEL  | MB_HELP | MB_MOVEABLE |
                         MB_APPLMODAL | MB_WARNING );    /* Message box style.*/
               }

               /*
                * If the user says OK, then go ahead and end the dialog after
                * getting the state of the checkboxes from the temp buffer.
                * Otherwise do not end the dialog so that the user can change
                * the checkboxes.
                */
               if ( usRC == MBID_OK )
               {

                  for( usI=0; usI<INCLUDE_NUMBER_OF_CHECKBOXES; usI++ )
                  {
                     afCheckBoxState[ usI ] = afTempCheckBoxState[ usI ];
                  }

                  WinDismissDlg(
                     hwnd,                  /* Dialog box to close.           */
                     TRUE );                /* Get rid of the dialog box.     */
               }

               return( 0 );

            /*
             * The cancel pushbutton has been pressed the dialog will be
             * dismissed. (Or the ESC key was pressed)
             */
            case DID_CANCEL:
            case ID_INCLUDE_CANCEL_PB :

               WinDismissDlg(
                  hwnd,                     /* Dialog box to close.           */
                  TRUE );                   /* Get rid of the dialog box.     */

               return( 0 );

         }  /* End of Command Switch */

         return( 0 );

      default:
         return( WinDefDlgProc( hwnd, msg, mp1, mp2 ) );

   }  /* End of Switch */

   return( (MRESULT) FALSE );

} /* End of IncludeDialogProc */


/******************************************************************************
 * Name        : SendStringToMCD
 *
 * Description : This procedure will take the string from the Combo Box,
 *               check for it's validity and send it to the MCD for processing.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: mciSendString
 *               mciGetErrorString
 *
 * Parameters  : hwndComboBox - Handle to the Combo Box.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID SendStringToMCD( HWND hwndComboBox )
{
   CHAR   acTempString[ STRING_SIZE ];                  /* Temp string buffer.*/
   USHORT usResult;
   ULONG  flStyle;
   CHAR   acErrorStringBuffer[ MCI_STRING_LENGTH ];     /* Error string buffer*/
   ULONG  ulSendStringRC    = 0,                        /* Send String API RC.*/
          lGetErrorStringRC = 0;                        /* Error String API RC*/

   /*
    * Get the MCI String Command that is supposed to be in the Entry field
    * of the combination box.
    */
   GetStringFromComboBox(
      hwndComboBox,
      &acMCIString[ 0 ] );

   if ( IsMCIStringValid( &acMCIString[ 0 ] ) == FALSE )
   {
      /*
       * The given MCI String is not valid.  Get out of this routine.
       */
      return;
   }

   /*
    * Place the string in the list box portion of the combination box.
    */
   WinSendMsg(
      hwndComboBox,                       /* Handle to the Combination Box.   */
      LM_INSERTITEM,                      /* Action for the Combo box to do.  */
      MPFROM2SHORT( LIT_END, 0),          /* MP 1.  Place text at end of list.*/
      (MPARAM) (PSZ) &acMCIString[ 0 ] ); /* MP 2.  MCI String Command.       */

   /*
    * Erase the string that is in the Entry Field of the combination box by
    * first selecting all of the text in the filed and then clearing it.
    * Then force the cursor to go back to the entry field to accept the
    * next string command.
    */
   WinSendMsg(
      hwndComboBox,                       /* Handle to the Combination Box.   */
      EM_SETSEL,                          /* Action for the Combo box to do.  */
      MPFROM2SHORT( 0,MCI_STRING_LENGTH ),/* MP 1.  Reserved.                 */
      (MPARAM) NULL );                    /* MP 2.  Reserved.                 */

   WinSendMsg(
      hwndComboBox,                       /* Handle to the Combination Box.   */
      EM_CLEAR,                           /* Action for the Combo box to do.  */
      (MPARAM) NULL,                      /* MP 1.  Reserved.                 */
      (MPARAM) NULL );                    /* MP 2.  Reserved.                 */


   /*
    * We have a 'valid' MCI string from the sample programs point of view,
    * whether it really is valid will be decided by MCD.  Send the MCI String
    * Command and increment the counter so that we know how many MCI strings
    * have been sent.
    *
    * Set the pointer to an hourglass first, since this might take a couple
    * of seconds.
    */

   WinSetPointer(
      HWND_DESKTOP,              /* Desktop window handle.                    */
      WinQuerySysPointer(        /* This API will give the handle of the PTR. */
         HWND_DESKTOP,           /* Desktop window handle.                    */
         SPTR_WAIT,              /* The Hourglass icon.                       */
         FALSE ) );              /* Return the system pointer's handle.       */

   ulSendStringRC =
      mciSendString(
         (PSZ)&acMCIString[ 0 ],          /* The MCI String Command.          */
         (PSZ)&acMCIReturnString[   0 ],  /* The place to put Return strings. */
         (USHORT) MCI_RETURN_STRING_LENTGH, /* How large is the Return space. */
         hwndDisplayDialogBox,            /* Which window receives Notifies.  */
         usCountOfMCIStringSent );        /* The user parameter.              */

   WinSetPointer(
      HWND_DESKTOP,              /* Desktop window handle.                    */
      WinQuerySysPointer(        /* This API will give the handle of the PTR. */
         HWND_DESKTOP,           /* Desktop window handle.                    */
         SPTR_ARROW,             /* The Arrow icon.                           */
         FALSE ) );              /* Return the system pointer's handle.       */

   usCountOfMCIStringSent++;

   /*
    * Display the return string from mciSendString api.
    */

   strcpy (acTempString, acStringBuffer[IDS_RETURN_STRING - 1]);
   strcat (acTempString, acMCIReturnString);
   WinSendMsg( hwndDisplayListBox,
               LM_INSERTITEM,
               MPFROMSHORT( LIT_END ),
               MPFROMP( acTempString ));

   acMCIReturnString[ 0 ] =  (CHAR) NULL;  /* Initialize the return string   */

   /*
    * Keep count up to some number which is less than the maximum for the
    * number of items in a list box.
    */
   if ( usCountOfMCIStringSent == (USHORT) 65500 )
   {
      usCountOfMCIStringSent = 1;
   }

   /*
    * IF the string return code is not 0 then there was an error.  Indicate
    * this to the user, find out what the problem was, and show that to the
    * user.
    */
   if ( ulSendStringRC != 0 )
   {

      usCountOfMCIStringSent--;

      lGetErrorStringRC =
         mciGetErrorString(
            ulSendStringRC,
            (PSZ)&acErrorStringBuffer[ 0 ],
            (USHORT)MCI_ERROR_STRING_LENGTH );

      /*
       * Make sure that the retrieving of the error string was successful.
       */
      if ( lGetErrorStringRC != MCIERR_SUCCESS )
      {
         ShowAMessage(
            hwndMainDialogBox,                   /* Owner Window.             */
            acStringBuffer[ IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
            IDS_MCI_GETTING_STRING_ERROR_FAILED,  /* ID of the message.       */
            MB_APPLMODAL | MB_MOVEABLE | MB_HELP |
               MB_OK | MB_INFORMATION );          /* Message box style.       */

         return;

      }  /* End of IF testing for failed Get Error String API. */

      /*
       * Before displaying the error, first check to see if we are processing
       * a batch file. Then, check to see if the string came from the batch
       * file. If these conditions are true, append the error message which is
       * returned by the mciGetErrorString with another message to get approval
       * of the user for further processing of the batch file.
       */
      if ( (hFile) && (fStringFromBatchFile) )
      {
         strcat(acErrorStringBuffer, acStringBuffer[IDS_CONTINUE_BATCHFILE-1]);
         flStyle = MB_YESNO;
      }
      else
         flStyle = MB_CANCEL;

      /*
       * Show the error string.
       */
      usResult =
        WinMessageBox(
           HWND_DESKTOP,                /* Parent handle of the message box.  */
           hwndMainDialogBox,           /* Owner handle of the message box.   */
           (PSZ) &acErrorStringBuffer,  /* String to show in the message box. */
           acStringBuffer[ IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],/* Title.  */
           (USHORT) ID_MESSAGE_BOX,       /* Message Box Id.                    */
           flStyle | MB_MOVEABLE | MB_APPLMODAL | MB_HELP |
              MB_INFORMATION );           /* The style of the message box.    */

      /*
       * First check to see if the user wants to continue processing the batch
       * file. Then, check to see the string command included "notify". If
       * these conditions are true post a message to read the next string
       * command.
       * If the "notify" is not included in the string command, we post a
       * message in the RunTheBatchFile function, This will avoid posting twice.
       */

      if ((usResult == MBID_YES) && (fNotify))
         /*
          * Continue processing the batch file.
          */
         WinPostMsg( hwndMainDialogBox, WM_BATCHFILE, NULL, NULL );
      else if (usResult == MBID_NO)
      {
         hFile = 0;    /* Quit processing the batch file                      */
         /*
          * Enable the Batch file push button.
          */
         WinEnableWindow(
            hwndBatchFilePB,        /* Handle of this dialog window.          */
            TRUE );                 /* ID of the left most PM control.        */
      }
   }  /* End of IF testing for failed MCI Send String API. */

   WinSetFocus( HWND_DESKTOP, hwndComboBox);

}  /* End of SendStringToMCD */


/******************************************************************************
 * Name        : InitializeHelp
 *
 * Description : This procedure will set up the initial values in the global
 *               help structure.  This help structure will then be passed on
 *               to the Help Manager when the Help Instance is created.  The
 *               handle to the Help Instance will be kept for later use.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  :
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID InitializeHelp( VOID )
{

   /*
    * Load the strings for the Help window title and library name.
    */
   hmiHelpStructure.pszHelpLibraryName  =
      acStringBuffer[ IDS_MAIN_HELP_LIBRARY_FILE - 1 ];

   hmiHelpStructure.pszHelpWindowTitle  =
      acStringBuffer[ IDS_MAIN_HELP_WINDOW_TITLE - 1 ];

   /*
    * Get the size of the initialization structure.
    */
   hmiHelpStructure.cb              = sizeof( HELPINIT );

   hmiHelpStructure.ulReturnCode    = (USHORT) NULL; /* RC from HM initial. */
   hmiHelpStructure.pszTutorialName = (PSZ) NULL;    /* No tutorial program.*/

   hmiHelpStructure.phtHelpTable    = (PVOID)(0xffff0000 |
                                         ID_MCI_STRING_HELPTABLE);
   /*
    * The action bar is not used, so set the following to NULL.
    */
   hmiHelpStructure.hmodAccelActionBarModule = (HMODULE) NULL;
   hmiHelpStructure.idAccelTable             = (USHORT) NULL;
   hmiHelpStructure.idActionBar              = (USHORT) NULL;

   /*
    * The Help table is in the executable file.
    */
   hmiHelpStructure.hmodHelpTableModule = (HMODULE) NULL;

   /*
    * The help panel ID is not displayed.
    */
   hmiHelpStructure.fShowPanelId       = (ULONG) NULL;

   /*
    * Create the Help Instance for IPF.  Give IPF the Anchor Block
    * handle and address of the IPF initialization structure, and
    * check that creation of Help was a success.
    */
   hwndHelpInstance =
       WinCreateHelpInstance(
          hab,                  /* Anchor Block Handle.                  */
          &hmiHelpStructure );  /* Help Structure.                       */

   if ( hwndHelpInstance == (HWND) NULL )
   {
      ShowAMessage(
          hwndMainDialogBox,                            /* Owner Window       */
          acStringBuffer[ IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
          IDS_MAIN_HELP_CREATION_FAILED,                /* ID of the message. */
          MB_APPLMODAL | MB_MOVEABLE |
          MB_OK | MB_INFORMATION );                     /* Message box style. */
   }
   else
   {
      if ( hmiHelpStructure.ulReturnCode )
      {
         ShowAMessage(
             hwndMainDialogBox,                      /* Owner Window          */
             acStringBuffer[ IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
             IDS_MAIN_HELP_CREATION_FAILED,          /* ID of the message.    */
             MB_APPLMODAL | MB_OK | MB_MOVEABLE |
             MB_INFORMATION );                       /* Message box style.    */

      }
      else   /* help creation worked */
      {
         WinAssociateHelpInstance(
            hwndHelpInstance,       /* The handle of the Help Instance.       */
            hwndFrame );            /* Associate to this dialog window.       */
      }
   }  /* End of IF checking the creation of the Help Instance. */

}  /* End of InitializeHelp */


/******************************************************************************
 * Name        : ShowAMessage
 *
 * Description : This function will show text in a message box.  The text
 *               is pulled from the string table that is originally kept
 *               in the resource file.  The text is retrieved by the
 *               message id that is passed into this function.  This id
 *               is used in the WinLoadString OS/2 API to get the correct
 *               string from the table.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hwndOwnerWindow   - Handle to the window which called for
 *                                   a message box.
 *               pcMessageTitle    - The tile to be placed in the message
 *                                   box.
 *               usMessageId       - The message id that identifies which
 *                                   string to retrieved from the string
 *                                   table.
 *               ulMessageBoxStyle - The style of the message box.  Which
 *                                   icons, buttons, etc., to show.
 *
 * Return      : The result from the message box.
 *
 ******************************************************************************/
USHORT ShowAMessage( HWND   hwndOwnerWindow,
                     CHAR   *pcMessageTitle,
                     USHORT usMessageId,
                     ULONG  ulMessageBoxStyle )
{

   CHAR   acStringBuffer[ CCHMAXPATH ];         /* Buffer for RC string.      */
   USHORT usMessageBoxResult;                   /* Result from Message Box.   */

   /*
    * Get the string from the Resource defined string table and show it
    * in the message box.
    */
   WinLoadString(
      hab,                             /* HAB for this dialog box.            */
      (HMODULE) NULL,                  /* Get the string from the .exe file.  */
      usMessageId,                     /* Which string to get.                */
      (SHORT) CCHMAXPATH,              /* The size of the buffer.             */
      acStringBuffer );                /* The buffer to place the string.     */

   usMessageBoxResult =
      WinMessageBox(
         HWND_DESKTOP,                 /* Parent handle of the message box.   */
         hwndOwnerWindow,              /* Owner handle of the message box.    */
         acStringBuffer,               /* String to show in the message box.  */
         pcMessageTitle,               /* Title to shown in the message box.  */
         (USHORT) ID_MESSAGE_BOX,      /* Message Box Id.                     */
         ulMessageBoxStyle );          /* The style of the message box.       */

   return( usMessageBoxResult );

}  /* End of ShowAMessage */


/******************************************************************************
 * Name        : IsMCIStringValid
 *
 * Description : This function will take a string and decide if it is valid,
 *               i.e., the string is not empty or full of blanks.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : MCIString - Points to the buffer that will hold
 *
 * Return      : TRUE if the string is valid worked otherwise FALSE.
 *
 ******************************************************************************/
BOOL IsMCIStringValid( PSZ pszMCIString )
{
   if ( strlen( pszMCIString ) == 0 )
   {
      return( FALSE );
   }

   return( TRUE );

}  /* End of IsMCIStringValid */


/******************************************************************************
 * Name        : GetStringFromComboBox
 *
 * Description : This function will get a string from the String Combo Box's
 *               Entry Field of the dialog box.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : pszMCIString - Points to the buffer that will hold
 *                              the Source File Name.
 *               hwndStringComboBox - Handle to the Combo Box.
 *
 * Return      : TRUE if the retrieval worked otherwise FALSE.
 *
 ******************************************************************************/
BOOL GetStringFromComboBox( HWND hwndStringComboBox,
                            PSZ  pszMCIString )
{
   SHORT sTextLength;                      /* Text length of combo box string.*/

   sTextLength =
      WinQueryWindowText(
         hwndStringComboBox,
         FILE_NAME_SIZE,
         (PCH) pszMCIString );

   if ( sTextLength == 0 )
   {
      pszMCIString[ 0 ] = (CHAR) NULL;

      ShowAMessage(
         hwndMainDialogBox,                    /* Owner Window.               */
         acStringBuffer[ IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
         IDS_MCI_STRING_HAS_NO_SIZE,           /* ID of the message.          */
         MB_APPLMODAL | MB_MOVEABLE | MB_HELP |
            MB_CANCEL | MB_INFORMATION );      /* Message box style.          */

      return( FALSE );

   }

   return( TRUE );

}  /* End of GetStringFromComboBox */


/******************************************************************************
 * Name        : SetSystemMenu
 *
 * Description : This procedure changes the System Menu of the given dialog
 *               window so that only the Move and Close options will be
 *               seen.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hwndDialogWindow - Handle to the dialog window whose system
 *                                  menu is to be changed.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID SetSystemMenu( HWND hwndDialogWindow )
{
   HWND     hwndSysMenu;                 /* System menu pull-down handle.     */

   /*
    * Get the handle of the system menu pull-down.
    */
   hwndSysMenu =
      WinWindowFromID(
         hwndDialogWindow,
         (USHORT) FID_SYSMENU );

   WinSendMsg(
      hwndSysMenu,
      MM_DELETEITEM,
      MPFROM2SHORT( (SHORT) SC_TASKMANAGER, (SHORT) TRUE ),
      (MPARAM) NULL );

   WinSendMsg(
      hwndSysMenu,
      MM_DELETEITEM,
      MPFROM2SHORT( SC_RESTORE, TRUE ),
      (MPARAM) NULL );

   WinSendMsg(
      hwndSysMenu,
      MM_DELETEITEM,
      MPFROM2SHORT( SC_SIZE, TRUE ),
      (MPARAM) NULL );

   WinSendMsg(
      hwndSysMenu,
      MM_DELETEITEM,
      MPFROM2SHORT( SC_MINIMIZE, TRUE ),
      (MPARAM) NULL );

   WinSendMsg(
      hwndSysMenu,
      MM_DELETEITEM,
      MPFROM2SHORT( SC_MAXIMIZE, TRUE ),
      (MPARAM) NULL );

}  /* End of SetSystemMenu. */


/******************************************************************************
 * Name        : SendScrollBarMessage
 *
 * Description : This procedure sends scroll bar messages to the vertical
 *               scroll bar in the subclassed list box in the Display
 *               Messages dialog box.  This is done so that cursor keys
 *               for the list box can be trapped to prevent the items
 *               within the list box from being highlighted, i.e., selected.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hwndControl       : Handle to the dialog window.
 *               sScrollBarMessage : Message to send to the scroll bar.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID SendScrollBarMessage(
         HWND  hwndControl,
         SHORT sScrollBarMessage )
{
   SHORT sCurrentSBPosition;     /* Position of the scroll bar.               */
   HWND  hwndVerticalSB;         /* Handle to the vertical scroll bar.        */


   if ( WinQueryFocus( HWND_DESKTOP ) == hwndControl )
   {
      /*
       * Get the handle to the list box's vertical scroll bar so that the
       * position of the scroll bar can be determined.
       */
      hwndVerticalSB =
         WinWindowFromID(
            hwndControl,
            (USHORT) LIST_BOX_VERTICAL_SB_ID );

      /*
       * Get the current position of the scroll bar.
       */
      sCurrentSBPosition = (SHORT)
         WinSendMsg(
            hwndVerticalSB,
            SBM_QUERYPOS,
            (MPARAM) 0,
            (MPARAM) 0 );

      /*
       * Send the scroll bar message that was passed into this routine to the
       * dialog box with the list box.
       */
      WinPostMsg(
         hwndControl,
         WM_VSCROLL,
         (MPARAM) (USHORT) LIST_BOX_VERTICAL_SB_ID,
         (MPARAM) MPFROM2SHORT( sCurrentSBPosition,
                                sScrollBarMessage ) );
   }

}  /* End of SendScrollBarMessage. */

/******************************************************************************
 * Name        : Restore
 *
 * Description : This procedure reads the os2.ini file for restored information
 *               and set the restored position.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hwnd  - Handle for the main dialog box.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID Restore( HWND hwnd )
{
   ULONG    ulSize;                    /* for data size in the ini file       */
   BOOL     fSuccess = FALSE;          /* for successful reading of ini file */

   /*
    * Get the window title string from the Resource string table.
    */
   WinLoadString(
      hab,                             /* HAB for this dialog box.            */
      (HMODULE) NULL,                  /* Get the string from the .exe file.  */
      IDS_MAIN_WINDOW_TITLE,           /* Which string to get.                */
      (SHORT) sizeof( szAppName),      /* The size of the buffer.             */
      szAppName);                      /* The buffer to place the string.     */

   /*
    * Look in the os2.ini file for existing data.
    *
    * This call search the os2.ini file for a Key name specified by the
    * szKeyName parameter under the application heading specified by the
    * szAppName parameter and returns the size of the data structure written
    * to the os2.ini.
    */
   if ( fSuccess = PrfQueryProfileSize(
                      HINI_USER,          /* file handle for os2.ini          */
                      szAppName,          /* Application name to look for     */
                      szKeyName,          /* Key name to look for             */
                      &ulSize ))          /* Size of return data structure    */
   {
      /*
       * Check for the size of data structure returned from the ini file is
       * equal to the actual structure. This check is required to make sure
       * that the os2.ini file is not corrupted.
       */
      if ( fSuccess = (ulSize == sizeof(SWPRESTORE)))
      {
         /*
          *  This call returns data that is written to the os2.ini file.
          */
         if (fSuccess = PrfQueryProfileData(
                           HINI_USER,         /* file handle for os2.ini      */
                           szAppName,         /* Application name to look for */
                           szKeyName,         /* Key name to look for         */
                           (PVOID)&Save,      /* Return actual data           */
                           (PULONG)&ulSize )) /* Size of return data struc    */
         {

            /*
             *  Initialize the Display message window status flag for
             *  further use.
             */
            fDisplayMsgWindowIsUp =  Save.fDisplayMsgWindowStatus;

            /*
             * Set the main to the restored position.
             */
            WinSetWindowPos(
               hwndFrame,                /* Frame window handle               */
               HWND_TOP,                 /* Place hwnd on top of all siblings */
               Save.swpMain.x,           /* Window position, x coordinate     */
               Save.swpMain.y,           /* Window position, y coordinate     */
               Save.swpMain.cx,          /* Window size                       */
               Save.swpMain.cy,          /* Window size                       */
               SWP_ZORDER | SWP_MOVE |   /* Window positioning options        */
               SWP_SIZE | SWP_SHOW);

            /*
             * If Display message window status is true, set the Display
             * message window to the restored position.
             */
            if (fDisplayMsgWindowIsUp)
            {
               WinSetWindowPos(
                  hwndDisplayDialogBox,  /* Display Message window handle     */
                  HWND_TOP,              /* Place hwnd on top of all siblings */
                  Save.swpDisplay.x,     /* Window position, x coordinate     */
                  Save.swpDisplay.y,     /* Window position, y coordinate     */
                  0L,                    /* Window size                       */
                  0L,                    /* Window size                       */
                  SWP_MOVE );            /* Window positioning options        */
            }
         }
      }
   }

   /*
    * If error while reading os2.ini file, display the main window with the
    * default size.
    */
   if (!fSuccess)
   {
      WinShowWindow( hwndFrame, TRUE );
   }
}

/******************************************************************************
 * Name        : SaveMSTInfo
 *
 * Description : This procedure saves the resorted information to the ini file
 *               before destroying the Main and Display messages windows.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID SaveMSTInfo( VOID )
{

   /*
    * Update the Display Message Window status flag.
    */
   Save.fDisplayMsgWindowStatus =  fDisplayMsgWindowIsUp;

   /*
    * Before destroying the main window query its size and position.
    */
   WinQueryWindowPos( hwndFrame, &Save.swpMain );

   /*
    * If Display messages window is up query its size and position also.
    */
   if ( fDisplayMsgWindowIsUp )
      WinQueryWindowPos( hwndDisplayDialogBox, &Save.swpDisplay );

   /*
    * Save the current values in the os2.ini file for further use.
    */
   PrfWriteProfileData(
           HINI_USER,                   /* file handle for os2.ini            */
           szAppName,                   /* Application name to look for       */
           szKeyName,                   /* Key name to look for               */
           (PVOID) &Save,               /* Save data                          */
           sizeof(SWPRESTORE));         /* Size of save data                  */
}

/******************************************************************************
 * Name        : RunTheBatchFile
 *
 * Description : This procedure reads the input batch file one line of string
 *               command at a time and copies the same into the String Combo
 *               Box's Entry Field of the dialog box.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hwnd       : Handle to the main dialog window.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID RunTheBatchFile( HWND hwnd )
{
   PCHAR   pcResult;
   PSZ     pszTemp;
   PSZ     pszMCIString;

   /*
    * Initialize the variables to hold the string command
    */
   memset( acMCIString, 0, sizeof(acMCIString) );

   if (hFile != 0)
   {
      /*
       * Read a line of string command from the opened file into acMCIString.
       */
      if ((pszMCIString = fgets(acMCIString, MCI_STRING_LENGTH, hFile)) != NULL)
      {
         fStringFromBatchFile = TRUE;/* String command came from the batch file */
         /*
          * Convert to lower case to look consistent in the combo box.
          */
         strlwr( pszMCIString );

         /*
          * fgets adding a 0xa character at the end of the string. This is
          * needed just to get rid of it.
          */
         if (pszTemp = strchr(pszMCIString, 0xa))
            pszTemp[0] = 0;

         if ( strlen( pszMCIString ) != 0 )
         {
            /*
             * Copy the string command into the combo box.
             */
            WinSetDlgItemText( hwnd, ID_STRING_COMBO_BOX, pszMCIString );

            /*
             * Check to see "notify" added to the string command.
             */
            pcResult = strstr(pszMCIString, "notify");

            /*
             * If the string command includes "notify", set fNotify to true and
             * process the string. We must wait till we get MM_MCINOTIFY message
             * back to us to set fNotify flag to false and read the next line.
             */
            if (pcResult)
            {
               fNotify = TRUE;
               usUserParameter = usCountOfMCIStringSent;
               SendStringToMCD( hwndComboBox );
            }
            /*
             * Else, process the string and post a message to read the next line.
             */
            else
            {
               SendStringToMCD( hwndComboBox );
               WinPostMsg( hwnd, WM_BATCHFILE, NULL, NULL );
            }
         }
         else      /* blank line in the batch file                            */
            WinPostMsg( hwnd, WM_BATCHFILE, NULL, NULL );
      }
      else                                 /* If EOF                          */
      {
         hFile = 0;
         ShowAMessage(
             hwndMainDialogBox,                          /* Owner Window.     */
             acStringBuffer[ IDS_MAIN_WINDOW_TITLE - 1 ],
             IDS_END_OF_BATCH_FILE,                      /* ID of the message.*/
             MB_CANCEL  | MB_MOVEABLE | MB_APPLMODAL | MB_INFORMATION );
         /*
          * Enable the Batch file push button.
          */
         WinEnableWindow(
            hwndBatchFilePB,        /* Handle of this dialog window.          */
            TRUE );                 /* ID of the left most PM control.        */
      }
   }
}

/******************************************************************************
 * Name         :  GetBatchFileAndOpenIt
 *
 * Description  :  This procedure will gets an existing batch file and opens it.
 *
 * Concepts     :  none
 *
 * MMPM/2 API's :  none
 *
 * Parameters   :  none
 *
 * Return       :  TRUE  -  if the a file open is success.
 *                 FALSE -  if could not open the file.
 *
 ******************************************************************************/
BOOL GetBatchFileAndOpenIt( VOID )
{
   FILEDLG fileDlg;                         /* Structure for open file dialog.*/
   CHAR    szTitle[FILE_NAME_SIZE];         /* Open file dialog box title.    */
   USHORT  usResult = 0;                    /* For return code.               */
   CHAR    szButtonText[STRING_SIZE];       /* For Open dialog button text.   */


   WinLoadString(
      hab,                                  /* HAB for this dialog box.       */
      (HMODULE)0,                           /* Get string from .exe file.     */
      IDS_OPEN_BATCHFILE_TITLE,             /* Which string to get.           */
      (SHORT) sizeof(szTitle) ,             /* The size of the buffer.        */
      szTitle);                             /* Buffer to place string.        */

   WinLoadString(
      hab,                                  /* HAB for this dialog box.       */
      (HMODULE)0,                           /* Get string from .exe file.     */
      IDS_OPEN_BATCHFILE_BUTTON,            /* Which string to get.           */
      (SHORT) sizeof(szButtonText),         /* The size of the buffer.        */
      szButtonText);                        /* Buffer to place string.        */

   /*
    * Initialize the structure with zeros.
    */
   memset( &fileDlg, 0, sizeof(fileDlg));

   /*
    * Initialize the file dialog data structure with the appropriate information.
    */
   fileDlg.cbSize = sizeof(FILEDLG);
   fileDlg.pfnDlgProc = (PFNWP)FileDlgProc;
   fileDlg.ulUser = (ULONG) hwndMainDialogBox;
   fileDlg.pszOKButton = (PSZ) szButtonText;
   fileDlg.pszTitle = szTitle;
   fileDlg.fl = FDS_HELPBUTTON | FDS_CENTER | FDS_OPEN_DIALOG;

   /*
    *Call the standard file dialog to get the file
    */
   usResult = WinFileDlg(HWND_DESKTOP, hwndMainDialogBox, &fileDlg);

   if (usResult != 0)
   {
      if (fileDlg.lReturn == DID_OK)
         OpenBatchFile( fileDlg.szFullFile );
      else if (fileDlg.lReturn == DID_CANCEL)
         return( FALSE );
   }
   return( TRUE );
}

/*************************************************************************
 * Name         :  OpenFileDlgProc
 *
 * Description  :  This function controls the open file dialog box.

 * Concepts     :  none
 *
 * MMPM/2 API's :  none
 *
 * Parameters   :  hwnd - Handle for the File dialog box.
 *                 msg  - Message received by the dialog box.
 *                 mp1  - Parameter 1 for the just recieved message.
 *                 mp2  - Parameter 2 for the just recieved message.
 *
 * Return       :  result of default processing.
 *************************************************************************/
MRESULT EXPENTRY FileDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{

   switch( msg )
   {

      case WM_HELP :
         /*
          * The dialog window has received a request for help from the user,
          * i.e., the Help pushbutton was pressed.  Send the HM_DISPLAY_HELP
          * message to the Help Instance with the IPF resource identifier
          * for the correct HM panel.  This will show the help panel for this
          * sample program.
          */
         WinSendMsg(
            hwndHelpInstance,
            HM_DISPLAY_HELP,
            MPFROMSHORT( 4 ),
            MPFROMSHORT( HM_RESOURCEID ) );
         return( 0 );

      default:
         return WinDefFileDlgProc(hwnd, msg, mp1, mp2);
   }
}

/*************************************************************************
 * Name         :  OpenBatchFile
 *
 * Description  :  This function opens the batch file for processing.

 * Concepts     :  none
 *
 * MMPM/2 API's :  none
 *
 * Parameters   :  szBatchFileName  - Name of the file to open.
 *
 * Return       :  TRUE  -  if the a file open is success.
 *                 FALSE -  if could not open the file.
 *************************************************************************/

BOOL OpenBatchFile( CHAR szBatchFileName[] )
{
    /*
     * Open the batch file.
     */
    hFile = fopen ( szBatchFileName, "r" );
    if ( hFile == 0 )
    {
       ShowAMessage(
            hwndMainDialogBox,              /* Owner Window.     */
            acStringBuffer[ IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
            IDS_CANNOT_FIND_BATCH_FILE,     /* ID of the message.*/
            MB_OK | MB_HELP | MB_MOVEABLE |
            MB_APPLMODAL | MB_WARNING );    /* Message box style.*/
       return( FALSE );
    }
    return( TRUE );
}
