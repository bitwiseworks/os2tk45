/*static char *SCCSID = "@(#)avcinst.c  13.4 92/03/02";*/
/*************************************************************************
 * File Name   : AVCINST.C
 *
 * Description : This file contains the C source code required for the
 *               AVCINST sample program.
 *
 * Concepts    : The program will illustrate how to install and remove an
 *               Installable IO procedure.
 *
 * MMPM/2 API's: mmioInstallIOProc
 *
 * Required
 *    Files    : avcinst.c         Source Code.
 *               avcinst.h         Include file.
 *               avcinst.dlg       Dialog definition.
 *               avcinst.rc        Resources.
 *               avcinst.ipf       Help text.
 *               avcinst.mak       Make file.
 *               avcinst.def       Linker definition file.
 *               avcinst.ico       Program icon.
 *
 * Copyright (C) IBM 1991, 1993
 *************************************************************************/

#define  INCL_WIN                        /* Required to use Win APIs.         */
#define  INCL_PM                         /* Required to use PM APIs.          */
#define  INCL_WINHELP                    /* Required to use IPF.              */
#define  INCL_DOSMODULEMGR               /* Required for DosLoadModule.       */
#define  INCL_SECONDARYWINDOW            /* required for secondary window     */
#define  INCL_OS2MM

#include <os2.h>
#include <os2me.h>

#include <sw.h>

#include "avcinst.h"

/****************** Procedure/Function Prototypes *****************************/
/*
 * This definition is required to use the sample program with the IBM
 * C compiler.
 */
#pragma linkage( MainDialogProc, system )

MRESULT EXPENTRY MainDialogProc( HWND   hwnd,
                                 ULONG  msg,
                                 MPARAM mp1,
                                 MPARAM mp2 );
USHORT           ShowAMessage( USHORT usMessageId,
                               ULONG  ulMessageBoxStyle );
BOOL             DoTheInstallation( HWND hwnd);
VOID             InitializeDialog( VOID );
VOID             TerminateDialog( VOID );
VOID             InitializeHelp( VOID );

/******************** End of Procedure/Function Prototypes ********************/


/*************************** Global Variables. ********************************/
/*
 * This array holds the ID's that are used to retrieve the the strings from
 * the string table and placed into the global double array acStringBuffer.
 * The defines that are used for the string table is used to create the
 * index into the global buffer.  Since the string table starts at 1 and
 * the global buffer starts at 0, the index is decremented by 1 when used.
 * This means when the acStringBuffer is accessed after the strings have
 * been loaded into the buffer, all indexes will be n-1, i.e.,
 * IDS_DEFAULT_TARGET_FILE_NAME - 1, to get the proper string.  This is done
 * simply to keep down the number of #defines.
 */

USHORT ausMessageId[] = { IDS_HELP_WINDOW_TITLE,
                          IDS_MESSAGE_BOX_TITLE,
                          IDS_MMIO_INSTALLPROC_NAME,
                          IDS_MMIO_INSTALLPROC_DLL_NAME,
                          IDS_HELP_LIBRARY_FILE,
                          IDS_MAIN_WINDOW_TITLE };

HELPINIT hmiHelpStructure;             /* Help initialization structure.      */

/*
 * This array holds program required text that is pulled from the string
 * table.
 */
CHAR   acStringBuffer[ NUMBER_OF_PROGRAM_STRINGS ][ STRING_SIZE ];

HWND   hwndMainDialogBox,              /* Handle to the dialog window.        */
       hwndFrame,                     /* Handle to the frame window.         */
       hwndHelpInstance;               /* Handle to Help window.              */
HAB    hab;                            /* Handle to the Anchor Block.         */
QMSG   qmsg;                           /* Structure for messages on the queue.*/
HMQ    hmq;                            /* Handle to the queue.                */
BOOL   IOProcIsInstalled= FALSE;       /* Indicates current state of IO Proc  */
/************************** End of Gloable Variables **************************/


/*************************************************************************
 * Name        : main
 *
 * Description : This function calls the InitializeDialog procedure
 *               to create the dialog box, runs the message loop, and
 *               finally ends the program by calling the
 *               TerminateDialog.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : TRUE is returned to the operating system.
 *
 *************************************************************************/
INT main( VOID )
{

   InitializeDialog();

   while ( WinGetMsg( hab, (PQMSG) &qmsg, (HWND) NULL, 0, 0) )
      WinDispatchMsg( hab, (PQMSG) &qmsg );

   TerminateDialog();

   return( TRUE );

} /* End of main */

/*************************************************************************
 * Name        : InitializeDialog
 *
 * Description : This function performs the necessary initializations and
 *               setups that are required to show/run a dialog box as a
 *               main window.  The message queue will be created, as will
 *               the dialog box.  Help will also initialized.
 *
 *               This routine will start the window.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 *************************************************************************/
VOID InitializeDialog( VOID )
{

   USHORT   usI;                         /* Generic counter.                                      */
   CHAR     szDefaultSize[CCHMAXPATH];   /* buffer for default size menu text */

   /*
    * Setup and initialize the dialog window.
    */
   hab = WinInitialize( (USHORT) 0 );

   /*
    * Load the various strings that are required by the program.
    */
   for( usI = 0; usI < NUMBER_OF_PROGRAM_STRINGS; usI++)
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
    * window.  This is done so that the program name will show up in
    * the Task List window, via the FCF_TASKLIST flag AND will NOT
    * show the .exe file name of the program.  If the FCF_TASKLIST flag
    * is used the .exe file name AND the dialog title will show up
    * in the title bar.  This is not wanted.
    */
   WinSetWindowText(
      hwndFrame,
      acStringBuffer[ IDS_MAIN_WINDOW_TITLE - 1 ] );

   /*
    * Initialize the help structure and associate the help instance to this
    * dialog via it's handle to anchor block.
    */
   InitializeHelp();

} /* End of InitializeDialog */


/*************************************************************************
 * Name        : TerminateDialog
 *
 * Description : This routine is called after the message dispatch loop
 *               has ended because of a WM_QUIT message.  The code will
 *               destroy the help instance, messages queue, and window.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 *************************************************************************/
VOID TerminateDialog( VOID )
{
   USHORT usCounter = 0;   /* Generic counter.                                */

   /*
    * Destroy the Help Instance for this dialog window.
    */
   if ( hwndHelpInstance != (HWND) NULL )
   {
      WinDestroyHelpInstance( hwndHelpInstance );
   }

   WinDestroySecondaryWindow( hwndFrame );
   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );

}  /* End of TerminateDialog */


/*************************************************************************
 * Name        : MainDialogProc
 *
 * Description : This function controls the main dialog box.  It will
 *               handle received messages such as pushbutton notifications,
 *               help requests, minimize actions, etc.
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
 * Concepts    : None.
 *
 *
 * MMPM/2 API's: None.
 *
 *
 * Parameters  : hwnd - Handle for the Main dialog box.
 *               msg  - Message received by the dialog box.
 *               mp1  - Parameter 1 for the just recieved message.
 *               mp2  - Parameter 2 for the just recieved message.
 *
 * Return      : 0 or result of default processing.
 *
 *************************************************************************/
MRESULT EXPENTRY MainDialogProc( HWND   hwnd,
                                 ULONG  msg,
                                 MPARAM mp1,
                                 MPARAM mp2 )
{

   static BOOL fIsProgramMinimized  = FALSE;    /* Flag for minimized state.  */
   static PSWP pswpWindowActionMP = (PSWP) NULL;/* Indicates Min./Max window. */
   static HWND hwndSetPB;                       /* Handle to the Conver PB.   */
   HPOINTER hpProgramIcon;                      /* Program Icon pointer.      */

   switch( msg )
   {

      /*
       * Perform the initial creation of the dialog window and any one time
       * processing that is required.
       */
      case WM_INITDLG :

         /*
          * Set the icon to be used for this dialog box.
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


         WinSendDlgItemMsg(hwnd, ID_INSTAL, BM_SETCHECK,
                           MPFROM2SHORT (TRUE, 0), NULL);


         hwndSetPB =
            WinWindowFromID(
               hwnd,                   /* Handle of this dialog window.       */
               ID_OK_PB );             /* ID of the left most PM control.     */

      /*
       * A close message was received as a result of the Close on the System
       * Menu being selected.
       */
      case WM_CLOSE :
         /*
          * The call to the WinDefSecondaryWindowProc is required so that the dialog
          * box will be closed.  This is needed since the WM_QUIT is not
          * processed in the same manner for dialog boxes and windows.
          */
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      /*
       * The dialog window has recieved a request for help from the user,
       * i.e., the Help pushbutton was pressed.  Send the HM_DISPLAY_HELP
       * message to the Help Instance with the IPF resource identifier
       * for the correct HM panel.  This will show the help panel for this
       * sample program.
       */
      case WM_HELP :
      {
         WinSendMsg(
            hwndHelpInstance,
            HM_DISPLAY_HELP,
            MPFROMSHORT( 1 ),
            MPFROMSHORT( HM_RESOURCEID ) );
         return( 0 );
      }  /* End of WM_HELP. */

      /*
       * This message is sent whenever a pushbutton is selected.
       */
      case WM_COMMAND :
         /*
          * To get which pushbutton was pressed the SHORT1FROMMP macro
          * is used.
          */
         switch( SHORT1FROMMP( mp1 ) )
         {
            case ID_OK_PB :

               /* When the user selects the Set PB, we'll call a routine
                * to install or deinstall the IO Procedure.
                */

               if (DoTheInstallation (hwnd) == TRUE)
               {
                 ShowAMessage(
                      IDS_INSTALLATION_IS_COMPLETE,
                      MB_OK | MB_INFORMATION | MB_HELP |
                      MB_MOVEABLE );

                 return ( 0 );
               }
               return( 0 );

            case DID_CANCEL:                        /* Handle ESC key */
            case ID_CANCEL_PB :
               /*
                * Send a WM_CLOSE message to this program so that it can
                * be ended.
                */
               WinSendMsg(
                  hwnd,
                  WM_CLOSE,
                  (MPARAM) NULL,
                  (MPARAM) NULL );

               return( 0 );

         }  /* End of Command Switch */

         return( 0 );

      /*
       * This message is received whenever a PM control is used in the
       * dialog window.
       */

      case WM_MINMAXFRAME :
      {
         /*
          * In PM, some controls may have to be hidden if an icon is
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
            fIsProgramMinimized = TRUE;

            WinShowWindow(
               hwndSetPB,             /* Handle to the control.              */
               FALSE );                /* Hide the control window.            */
         }
         else
         {
            fIsProgramMinimized = FALSE;

            WinShowWindow(
               hwndSetPB,          /* Handle to the control.              */
               TRUE );                 /* Show the control window.            */
         }

         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );
      }  /* End of MINMAXFRAME */

      default:
          return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

   }  /* End of Switch */

   return( FALSE );

} /* End of MainDialogProc */


/*************************************************************************
 * Name        : DoTheInstallation
 *
 * Description : This function will query the radiobutton on the dialog
 *               and determine if the user wants to install or deinstall the
 *               IO Proc.  This routine will have all of the MMIO
 *               specific code.
 *
 * Concepts    : How to use the mmioInstallIOPRoc aou ti install or deinstall
 *               a custom IO Proc.
 *
 * MMPM/2 API's: mmioInstallIOProc
 *
 * Return      : True for success.
 *               False for failure.
 *
 *************************************************************************/
BOOL  DoTheInstallation( HWND hwnd)
{
   PMMIOPROC  lpmmioprocIoProc;           /* Pointer to the Installabe Proc.  */
   PMMIOPROC  pmmioprocSpecialIOProc;     /* Pointer once Proc. is installed. */
   HMODULE    hmModHandle;                /* Handle to the DLL module.        */
   FOURCC     fccIOProc;                  /* 4 character id of Install. Proc. */
   CHAR       acFailureNameBuffer[ FILE_NAME_SIZE ];  /* Failed DLL object.   */
   LONG       lmmioAPIRc,                             /* MMIO API return codes*/
              ldosAPIRc;                              /* DOS API return codes.*/

   /*
    * Create the Identifier for this particular MMIO procedure.  This id
    * will be used to route any further api's to the correct
    * MMIO Installable procedure.
    */
   fccIOProc = mmioFOURCC( 'A', 'V', 'C', 'A' ) ;

   /*
    * Get the DLL that contains the MMIO Installable procedure.
    */
   ldosAPIRc =
      DosLoadModule(
         &acFailureNameBuffer[ 0 ],    /* Object name if failure occurs.      */
         FILE_NAME_SIZE,               /* Size of the name buffer.            */
         acStringBuffer[ IDS_MMIO_INSTALLPROC_DLL_NAME - 1 ], /* DLL Name.    */
         &hmModHandle );               /* Handle to the module.               */

   if ( ldosAPIRc != NO_ERROR )
   {
      ShowAMessage(
         IDS_LOADMODULE_FAILED,                      /* ID of the message.    */
         MB_OK | MB_INFORMATION |  MB_HELP |
            MB_MOVEABLE );                           /* Message box style.    */

      return( FALSE );
   }

   /*
    * Get the address of the MMIO Installable procedure.
    */
   ldosAPIRc =
      DosQueryProcAddr(
         hmModHandle,                  /* Handle to the DLL module.           */
         0L,                           /* NULL gives the entry point.         */
         acStringBuffer[ IDS_MMIO_INSTALLPROC_NAME - 1 ], /* Procedure name   */
         (PFN *) &lpmmioprocIoProc );  /* Pointer to the Installable proc.    */

   if ( ldosAPIRc != NO_ERROR )
   {
      ShowAMessage(
         IDS_QUERY_PROC_ADDR_FAILED,                 /* ID of the message.    */
         MB_OK | MB_INFORMATION |  MB_HELP |
            MB_MOVEABLE );                           /* Message box style.    */

      return( FALSE );
   }

   /*
    * Check the radio button to see if the user wants to install or
    * deinstall...
    */
   if (WinSendDlgItemMsg ( hwnd
                         , ID_INSTAL
                         , BM_QUERYCHECK
                         , NULL, NULL))
   {

      /*
       * If the user wants to install the AVC IO Procedure, we have to make sure
       * that it isn't already installed.  The state of the io procedure (whether
       * it's installed or not) is indicated by the IOProcIsInstalled
       * flag.
       */

      if (IOProcIsInstalled == FALSE)
      {
         /*
          * If the IO proc is not installed yet, then issue the
          * mmioInstallIOProc api.
          */

         pmmioprocSpecialIOProc =
             mmioInstallIOProc(
             fccIOProc,                       /* The identifier of the procedure. */
             lpmmioprocIoProc,                /* Pointer to the Installable proc. */
             MMIO_INSTALLPROC );              /* Flag to Install the proc.        */

         if ( pmmioprocSpecialIOProc == (PMMIOPROC) NULL )
         {
            /*
             * If the Install failed, put up an error msg.
             */
            ShowAMessage(
               IDS_MMIOPROC_COULD_NOT_BE_INSTALLED,        /* ID of the message.    */
               MB_OK | MB_INFORMATION |  MB_HELP |
                  MB_MOVEABLE );                           /* Message box style.    */

            return( FALSE );

         } /* End of IF the installion  failed. */

         IOProcIsInstalled = TRUE;      /*  The install succeded. Set the flag  */
         return ( TRUE );
      }
      else
      {

         /* The IOProcIsInstalled variable is TRUE.  This indicates that
          * the user is trying to install the IO Proc without deinstalling it
          * in between. This is not allowed, so display an error message
          * and return.
          */

         ShowAMessage(
              IDS_ALREADY_INSTALLED,                /* ID of the message.    */
              MB_OK | MB_INFORMATION |  MB_HELP |
              MB_MOVEABLE );                        /* Message box style.    */
         return( FALSE );

      }
   }
   else
   {

   /* If the "Install IO Proc" radio button is not selected, then the
    * "Deinstall IO Proc" radio button must be selected - since radio buttons
    * are mutually exclusive.
    * Check to make sure that the IO Proc is installed, before you let the
    * user deinstall it.
    */

      if (IOProcIsInstalled == TRUE)
      {
         /*
          * Issue the mmioInstallIOProc api to deinstall the AVC IO Proc.
          */

         pmmioprocSpecialIOProc =
            mmioInstallIOProc(
               fccIOProc,               /* The identifier of the procedure. */
               lpmmioprocIoProc,        /* Pointer to the Installable proc. */
               MMIO_REMOVEPROC );       /* Flag to deInstall the proc.      */

         IOProcIsInstalled = FALSE;     /* DeInstall succeded. Set the flag */
         return ( TRUE );
      }
      else
      {

         /* The IOProcIsInstalled variable is FALSE.  This indicates that
          * the user is trying to deinstall the IO Proc without installing it
          * inbetween.
          * This is not allowed, so display an error message and return.
          */

         ShowAMessage(
             IDS_INSTALL_FIRST,                      /* ID of the message.    */
             MB_OK | MB_INFORMATION |  MB_HELP |
             MB_MOVEABLE );                          /* Message box style.    */

         return( FALSE );
      }
   } /* End of IF that checks which radio button is selected. */

}  /* End of DoTheInstallation. */

/*************************************************************************
 * Name        : InitializeHelp
 *
 * Description : This procedure will set up the initial values in the
 *               global help structure.  This help structure will then
 *               be passed on to the Help Manager when the Help Instance
 *               is created.  The handle to the Help Instance will be
 *               kept for later use.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 *************************************************************************/
VOID InitializeHelp( VOID )
{

   /*
    * Get the size of the initialization structure.
    */
   hmiHelpStructure.cb              = sizeof( HELPINIT );

   hmiHelpStructure.ulReturnCode    = (ULONG) NULL; /* RC from initialization.*/
   hmiHelpStructure.pszTutorialName = (CHAR) NULL;  /* No tutorial program.   */

   hmiHelpStructure.phtHelpTable    = (PVOID)(0xffff0000 |
      ID_AVC_HELPTABLE );

   /*
    * The action bar is not used, so set the following to NULL.
    */
   hmiHelpStructure.hmodAccelActionBarModule = (HMODULE) NULL;
   hmiHelpStructure.idAccelTable             = (USHORT) NULL;
   hmiHelpStructure.idActionBar              = (USHORT) NULL;

   /*
    * The Help window title.
    */
   hmiHelpStructure.pszHelpWindowTitle  = (PSZ)
      acStringBuffer[ IDS_HELP_WINDOW_TITLE - 1];

   /*
    * The Help table is in the executable file.
    */
   hmiHelpStructure.hmodHelpTableModule = (HMODULE) NULL;

   /*
    * The help panel ID is not displayed.
    */
   hmiHelpStructure.fShowPanelId       = (ULONG) NULL;

   /*
    * The library that contains the help panels.
    */
   hmiHelpStructure.pszHelpLibraryName  =
      acStringBuffer[ IDS_HELP_LIBRARY_FILE - 1 ];

   /*
    * Create the Help Instance for IPF.
    * Give IPF the Anchor Block handle and address of the IPF initialization
    * structure, and check that creation of Help was a success.
    */
   hwndHelpInstance = WinCreateHelpInstance(
                         hab,                   /* Anchor Block Handle.       */
                         &hmiHelpStructure );   /* Help Structure.            */

   if ( hwndHelpInstance == (HWND) NULL )
   {
      ShowAMessage(
         IDS_HELP_CREATION_FAILED,                     /* ID of the message.  */
         MB_OK | MB_INFORMATION |
            MB_MOVEABLE );                             /* Message box style.  */
   }
   else
   {
      if ( hmiHelpStructure.ulReturnCode )
      {
         ShowAMessage(
            IDS_HELP_CREATION_FAILED,                     /* ID of the message*/
            MB_OK | MB_INFORMATION |
               MB_MOVEABLE );                             /* Message box style*/

         WinDestroyHelpInstance( hwndHelpInstance );
      }
   }  /* End of IF checking the creation of the Help Instance. */

   /*
    * Associate the Help Instance of the IPF to this dialog window.
    */
   if ( hwndHelpInstance != (HWND) NULL )
   {
     WinAssociateHelpInstance(
        hwndHelpInstance,     /* The handle of the Help Instance.             */
        hwndFrame );          /* Associate to this frame window.             */
   }

}  /* End of InitializeHelp */


/*************************************************************************
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
 * Parameters  : pcMessageTitle    - The tile to be placed in the message
 *                                   box.
 *               usMessageId       - The message id that identifies which
 *                                   string to retreived from the string
 *                                   table.
 *               ulMessageBoxStyle - The style of the message box.  Which
 *                                   icons, buttons, etc., to show.
 *
 * Return      : The result from the message box.
 *
 *************************************************************************/
USHORT ShowAMessage( USHORT usMessageId,
                     ULONG  ulMessageBoxStyle )
{

   CHAR   acStringTableBuffer[ STRING_SIZE ];   /* Buffer for Resource string.*/
   USHORT usMessageBoxResult;                   /* User input from Mess. Box. */

   /*
    * Get the string from the Resource defined string table and show it
    * in the message box.
    */
   WinLoadString(
      hab,                          /* HAB for this dialog box.               */
      (HMODULE) NULL,               /* Get the string from the .exe file.     */
      usMessageId,                  /* Which string to get.                   */
      (SHORT) STRING_SIZE,          /* The size of the buffer.                */
      acStringTableBuffer );        /* The buffer to place the string.        */

   usMessageBoxResult =
      WinMessageBox(
         HWND_DESKTOP,                 /* Parent handle of the message box.   */
         hwndMainDialogBox,            /* Owner handle of the message box.    */
         acStringTableBuffer,          /* String to show in the message box.  */
         acStringBuffer[
            IDS_MESSAGE_BOX_TITLE - 1],/* Title to show in the message box.   */
         ID_MESSAGE_BOX,               /* Message Box Id.                     */
         ulMessageBoxStyle );          /* The style of the message box.       */

   return( usMessageBoxResult );

}  /* End of ShowAMessage */


