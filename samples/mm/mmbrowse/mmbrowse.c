/**************************************************************************
 *  File name  :  MMBrowse.c
 *
 *  Description:  This application serves as a template than can be use to demonstrate the
 *                    use of the MultiMedia I/O subsytem (MMIO).
 *
 *                This source file contains the following functions:
 *
 *                GetBitMap(HWND, PSZ)
 *                GetFOURCCDialogProc ( HWND, ULONG, MPARAM, MPARAM )
 *                FixSysMenu(HWND);
 *                AboutDlgProc(HWND, ULONG, MPARAM, MPARAM);
 *                QueryAllDialogProc ( HWND, USHORT, MPARAM, MPARAM );
 *                RemovePermDialogProc ( HWND, USHORT, MPARAM, MPARAM );
 *                DisplayImageFile ( HWND, PSZ );
 *                DisplayMessageBox ( HWND, USHORT, USHORT, USHORT );
 *                OpenFileDialog ( HWND, PSZ, PSZ, PSZ, PSZ );
 *                FourccToString( FOURCC, PSZ );
 *                MakeItBusy ( BOOL );
 *                Main ( int argc, char *argv[], char *envp[] );
 *                DrawBitMap ( HWND );
 *                CenterDialog ( HWND );
 *                FixSysMenu (HWND);
 *                InstallTempIOProc ( HWND );
 *                LoadBitmap (HAB, HDC, HPS *, PSZ );
 *                InitializeHelp ( HWND);
 *                MyWindowProc(hwnd, msg, mp1, mp2)
 *                MessageBox(hwnd idMsg, fsStyle, fBeep)
 *                MainCommand(mp1, mp2)
 *
 *  Concepts   :  MMIO IOProcs
 *
 *  API's      :   mmioClose
 *                 mmioWrite
 *                 mmioRead
 *                 mmioGetHeader
 *                 mmioSetHeader
 *                 mmioQueryHeaderLength
 *                 mmioSendMessage
 *                 mmioOpen
 *                 mmioGetFormats
 *                 mmioQueryFormatCount
 *                 mmioInstallIOProc
 *                 mmioStringToFOURCC
 *                 mmioIdentifyFile
 *                 mmioIniFileHandler
 *                 mmioFOURCC
 *
 *    Files    :  MMBrowse.c  MMBrowse.h  MMBrowse.rc MMbrowse.ipf
 *
 *  Copyright (C) 1991 IBM Corporation
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
#define INCL_DOSERRORS
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_DOSMEMMGR
#define INCL_GPI
#define INCL_GPICONTROL
#define INCL_GPILCIDS
#define INCL_GPIMETAFILES
#define INCL_GPIPRIMITIVES
#define INCL_GPIREGIONS
#define INCL_PM
#define INCL_WIN
#define INCL_WINDIALOGS
#define INCL_WINERRORS
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WINMENUS
#define INCL_WINMESSAGEMGR
#define INCL_WINMLE
#define INCL_WINPOINTERS
#define INCL_WINSTDDLGS
#define INCL_WINSTDFILE
#define INCL_WINSTDSLIDER
#define INCL_WINSWITCHLIST
#define INCL_WINSYS
#define INCL_WINTIMER
#define INCL_WINWINDOWMGR

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "mmbrowse.h"
#include "mmioos2.h"

VOID main ( int argc, char *argv[], char *envp[] )
{
    HMQ   hmq;                             /* Message queue handle         */
    HWND  hwndClient;                      /* Client area window handle    */
    HWND  hwndFrame;                       /* Frame window handle          */
    QMSG  qmsg;                            /* Message from message queue   */
    ULONG flCreate;                        /* Window creation control flags*/
    BOOL  bReturnCode;
    SWCNTRL swcntrl;                       /* struct with Task Mgr info */


    hab = WinInitialize( 0 );          /* Initialize PM                */

    hmq = WinCreateMsgQueue( hab, 0 );    /* Create a message queue       */

    bReturnCode = WinRegisterClass( hab,
                                    "MMBrowser",
                                    (PFNWP) MyWindowProc,
                                    CS_SIZEREDRAW,
                                    0 );

    flCreate = FCF_STANDARD & ~FCF_SHELLPOSITION;

    hwndFrame = WinCreateStdWindow ( HWND_DESKTOP,
                                     0L,
                                     &flCreate,
                                     "MMBrowser",
                                     "",
                                     0L,
                                     0,
                                     ID_WINDOW,
                                     &hwndClient );

    bReturnCode = WinSetWindowPos( hwndFrame,
                                   HWND_TOP,
                                   100, 100, 400, 280,
                                   SWP_SIZE | SWP_MOVE | SWP_ACTIVATE | SWP_SHOW );


    memset(&swcntrl, 0, sizeof(SWCNTRL));                  /* init structure */

    swcntrl.hwnd = hwndFrame;
    strcpy(swcntrl.szSwtitle, "MMBrowser" );

    WinAddSwitchEntry((PSWCNTRL)&swcntrl);            /* add app to Task Mgr */

    InitializeHelp (hwndFrame );

    /*
     *  Now, dispatch all messages...
     */

    while( WinGetMsg( hab, &qmsg, 0, 0, 0 ) )
    {
         WinDispatchMsg( hab, &qmsg );
    }

    WinDestroyHelpInstance(hwndHelpInstance);
    WinDestroyWindow( hwndFrame );
    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );

}

VOID MakeItBusy ( BOOL BusyStatus )
{
    if( BusyStatus == TRUE )
    {
         WinSetPointer ( HWND_DESKTOP,
                         WinQuerySysPointer ( HWND_DESKTOP,
                                              SPTR_WAIT,
                                              FALSE));
    }
    else
    {
         WinSetPointer ( HWND_DESKTOP,
                         WinQuerySysPointer ( HWND_DESKTOP,
                                              SPTR_ARROW,
                                              FALSE));
    }
}

MRESULT EXPENTRY MyWindowProc ( HWND hwnd,
                                USHORT msg,
                                MPARAM mp1,
                                MPARAM mp2 )
{
    USHORT command;
    BOOL   bReturnCode;
    ULONG  ReturnCode;
    PMMINIFILEINFO  pmmIniFileInfo;
    FOURCC IOProcFourCC;
    FOURCC IOProcFourSource;
    FOURCC IOProcFourTarget;
    ULONG  ulIndex = 0;
    CHAR   szTempBuffer1[STRING_LENGTH];
    CHAR   szTempBuffer2[STRING_LENGTH];
    FOURCC fccIOProc;
    FOURCC fccSourceIOProc;
    MMFORMATINFO mmFormatInfoSource;
    MMFORMATINFO mmFormatInfoTarget;
switch( msg )
    {

         case WM_CREATE:

              /*
               * Get the open dialog title from resource file.
               */

              WinLoadString( hab,
                             0,
                             IDS_OPEN_DIALOG_TITLE,
                             STRING_LENGTH,
                             szOpenDialogTitle ) ;

              /*
               * Get the open dialog file extension from resource file.
               */

              WinLoadString( hab,
                             0,
                             IDS_OPEN_DIALOG_FILE_EXT,
                             STRING_LENGTH,
                             szOpenDialogFileExt );

              /*
               * Get the open ok button from resource file.
               */

              WinLoadString( hab,
                             0,
                             IDS_OPEN_OK_BUTTON,
                             STRING_LENGTH,
                             szOpenOKButton );

                             /*
               * Get the install ok button from resource file.
               */

              WinLoadString( hab,
                             0,
                             IDS_INSTALL_OK_BUTTON,
                             STRING_LENGTH,
                             szInstallOKButton );

              /*
               * Get the install dialog box title from resource file.
               */

              WinLoadString( hab,
                             0,
                             IDS_INSTALL_TITLE,
                             STRING_LENGTH,
                             szInstallTitle );

              /*
               * Get the install dialog box file extension from resource file.
               */

              WinLoadString( hab,
                             0,
                             IDS_INSTALL_FILE_EXT,
                             STRING_LENGTH,
                             szInstallFileExt );

              /*
               * Copy the IO Proc standard name from resource file.
               */

              WinLoadString( hab,
                             0,
                             IDS_IOPROC_NAME,
                             STRING_LENGTH,
                             szIOProcName );

         break;  // end of WM_CREATE


         case WM_HELP :
              WinSendMsg ( hwndHelpInstance,
                           HM_DISPLAY_HELP,
                           MPFROMLONG ( 1 ),
                           MPFROMSHORT ( HM_RESOURCEID ) );

              return( 0 );
         break;

         case WM_ERASEBACKGROUND:
              return ( MRFROMLONG ( TRUE ) );
         break;

         case WM_COMMAND:
              command = SHORT1FROMMP(mp1);      /* Extract the command value    */
              switch (command)
              {

                   /*
                    * Open a file (read and view)
                    */

                   case ID_FILE_OPEN:

                        /*
                         * Get filename from user to open
                         */
                        pszFileName = malloc ( MAX_FILENAME_SIZE );

                        if( pszFileName == NULL )
                        {
                             break;

                        }

                        if( OpenFileDialog ( hwnd,   // handle of owner window
                                             szOpenDialogTitle,        // title of open dialog box
                                             "*.*",                    // file extension
                                             szOpenOKButton,
                                             pszFileName ) == FALSE )  // return filename here.
                        {
                             free ( pszFileName );
                             break;
                             // error.
                        }

                        if( !GetBitMap ( hwnd, pszFileName ) )
                        {
                             free ( pszFileName );
                             break;
                             // error.
                        }


                        strcpy ( szSource, pszFileName );
                        free ( pszFileName );


                        WinInvalidateRegion( hwnd, 0, FALSE );
                   break;
                   case ID_FILE_SAVEAS:
                        if( mmioIdentifyFile( szSource,
                            0L,
                            &mmFormatInfoSource,
                            &IOProcFourSource,
                            0L,
                            0L))
                        {
                        WinMessageBox( HWND_DESKTOP,
                        HWND_DESKTOP,
                        "Please Open an Image File",
                        "Save as .....",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                         MB_ERROR );
                         break;
                        }
                        fccSourceIOProc = mmFormatInfoSource.fccIOProc;
                        SaveFile( hwnd, szSource, fccSourceIOProc);
                        WinInvalidateRegion( hwnd, 0, FALSE );
                        break;

                   case ID_CONFIG_INST_TEMP:
                        pszFileName = malloc ( MAX_FILENAME_SIZE );

                        if( pszFileName == NULL )
                        {
                       /*
                        * could not allocate enough memory for the filename.
                        */

                        WinMessageBox( HWND_DESKTOP,
                                        hwnd,
                                        "Could not allocate enough memory for filename string. Please contact author.",
                                        "Install Temporary IO Procedure",
                                        (HMODULE) NULL,
                                        (ULONG) MB_OK | MB_MOVEABLE |
                                        MB_ERROR );
                        break;
                        }

                        /*
                         *  Get the DLL filename
                         */

                        if( OpenFileDialog ( hwnd,        // handle of owner window
                        "Install Temporary IO Procedure", // title of open dialog box
                        "*.DLL",                          // file extension
                        szInstallOKButton,
                        pszFileName ) == FALSE )  // return filename here.
                        {
                          WinInvalidateRegion( hwnd, 0, FALSE );
                          break;
                        }

                        bReturnCode = InstallTempIOProc ( hwnd );
                        if (Cancel == TRUE)
                        {
                        Cancel = FALSE;
                        WinInvalidateRegion ( hwnd, 0, FALSE );
                        break;
                        } /* endif */

                        /*
                         * error messages already output before here
                         */

                        if (!bReturnCode)
                        {
                        /*
                         * Temp IO Proc installed !
                         */
                         WinMessageBox( HWND_DESKTOP,
                                        hwnd,
                                        "Temporary IO Procedure installed successfully.",
                                        "Installed Temporary IO Procedure",
                                        (HMODULE) NULL,
                                        (ULONG) MB_OK | MB_MOVEABLE |
                                        MB_INFORMATION );
                          }
                        WinInvalidateRegion ( hwnd, 0, FALSE );
                        break;


                   /*
                    * Install an IO Proc into MMEMMIO.INI file.
                    */

                   case ID_CONFIG_INST_PERM:

                        pszFileName = malloc ( MAX_FILENAME_SIZE );

                        if( pszFileName == NULL )
                        {
                             break;
                        }

                        if( OpenFileDialog ( hwnd,                  // handle of owner window
                                             szInstallTitle,        // title of open dialog box
                                             "*.DLL",               // file extension
                                             szInstallOKButton,
                                             pszFileName ) == FALSE )  // return filename here.
                        {
                             WinInvalidateRegion( hwnd, 0, FALSE );
                             break;
                        }

                        pmmIniFileInfo = malloc ( sizeof( MMINIFILEINFO ) );

                        if( pmmIniFileInfo == NULL )
                        {
                             // error. can't allocate memory.
                             break;
                        }


                        /*
                         * Get a FOURCC.
                         */

                        bReturnCode = WinDlgBox ( HWND_DESKTOP,
                                                 (HWND) hwnd,
                                                 (PFNWP) GetFOURCCDialogProc,
                                                 (HMODULE) NULL,
                                                 GET_FOURCC,   // dialog template identity
                                                (PVOID) NULL);
                        if (bReturnCode == FALSE)
                        break;

                        IOProcFourCC = mmioStringToFOURCC ( szFOURCC, MMIO_TOUPPER ) ;

                        /*
                         * Place the Four-character-code into the
                         * IniFileInfo struct.
                         */

                        pmmIniFileInfo->fccIOProc = IOProcFourCC;

                        /*
                         * Place the DLL name string into the IniFileInfo
                         * struct.
                         */

                        strcpy ( pmmIniFileInfo->szDLLName, pszFileName );

                        /*
                         * Place the Procedure name string into the IniFileInfo
                         * struct.
                         */

                        strcpy ( pmmIniFileInfo->szProcName, szIOProcName );

                        /*
                         *  Call mmioIniFileHandler to install our IO Proc.
                         */

                        ReturnCode = mmioIniFileHandler ( pmmIniFileInfo,
                                                         MMIO_FINDPROC);


                        if( ReturnCode == MMIO_SUCCESS )
                        {


                             WinMessageBox( HWND_DESKTOP,
                                            hwnd,
                                            "Already Installed",
                                            szInstallTitle,
                                            (HMODULE) NULL,
                                            MB_OK | MB_MOVEABLE |
                                            MB_ERROR );
                             break;

                        }
                        ReturnCode = mmioIniFileHandler ( pmmIniFileInfo,
                                                          MMIO_INSTALLPROC );
                        if( ReturnCode == MMIO_SUCCESS )
                        {

                        DisplayMessageBox ( hwnd,

                                            IDS_INSTALL_TITLE,
                                            IDS_INSTALL_EXIT,
                                            MB_OK | MB_MOVEABLE |
                                            MB_INFORMATION );

                        DisplayMessageBox ( hwnd,
                                            IDS_INSTALL_TITLE,
                                            IDS_INSTALL_NOTE,
                                            MB_OK | MB_MOVEABLE |
                                            MB_INFORMATION );

                        break;
                        }

                           WinMessageBox( HWND_DESKTOP,
                                          hwnd,
                                         "Installation Failed",
                                          szInstallTitle,
                                          (HMODULE) NULL,
                                          MB_OK | MB_MOVEABLE |
                                          MB_ERROR );
                             break;
                   /*
                    * Remove an IO Proc from INI file.
                    */

                   case ID_CONFIG_REM_PERM:

                        ReturnCode = WinDlgBox ( HWND_DESKTOP,
                                                 (HWND) hwnd,
                                                 (PFNWP) RemovePermDialogProc,
                                                 (HMODULE) NULL,
                                                 REMOVE_PERM_LIST_BOX,
                                                 (PVOID) NULL);

                        WinInvalidateRegion( hwnd, 0, FALSE );
                   break;

                   case ID_CONFIG_QUERY_ALL:

                        ReturnCode = WinDlgBox ( HWND_DESKTOP,
                                                 (HWND) hwnd,
                                                 (PFNWP) QueryAllDialogProc,
                                                 (HMODULE) NULL,
                                                 QUERY_ALL_LIST_BOX,
                                                 (PVOID) NULL);

                        WinInvalidateRegion( hwnd, 0, FALSE );
                   break;


                   case IDM_HELPUSINGHELP:
                        if ( WinSendMsg ( hwndHelpInstance,
                                          HM_DISPLAY_HELP,
                                          MPVOID,
                                          MPVOID ) )
                        {
                             WinMessageBox( HWND_DESKTOP,
                                            hwnd,
                                            "Cannot display Help For Help.",
                                            "Help For Help",
                                            (HMODULE) NULL,
                                            MB_OK | MB_ERROR );
                        }
                        WinInvalidateRegion( hwnd, 0, FALSE );
                   break;

                   case IDM_HELPGENERAL:
                        if ( WinSendMsg ( hwndHelpInstance,
                                          HM_DISPLAY_HELP,
                                          MPFROM2SHORT(PANEL_HELPGENERAL,NULL), MPFROMSHORT(HM_RESOURCEID)))
                        {
                        WinInvalidateRegion( hwnd, 0, FALSE );
                        break;
                        }
                        break;
                   case IDM_HELPINDEX:
                        if (WinSendMsg ( hwndHelpInstance,
                                         HM_HELP_INDEX,
                                         MPVOID,
                                         MPVOID))
                        {
                                WinMessageBox( HWND_DESKTOP,
                                               hwnd,
                                               "Cannot display Help Index.",
                                               "Help Index",
                                               (HMODULE) NULL,
                                               MB_OK | MB_ERROR );
                       }
                       WinInvalidateRegion( hwnd, 0, FALSE );
                       break;

                   case IDM_HELPPRODUCTINFO:

                        WinDlgBox(HWND_DESKTOP,
                                  hwnd,
                                  (PFNWP)AboutDlgProc,
                                  0,
                                  IDD_PRODUCTINFO,
                                  (PVOID)NULL);

                        WinInvalidateRegion( hwnd, 0, FALSE );
                   break;

                   /*
                    * Exit from program.
                    */

                   case ID_EXITPROG:
                        WinPostMsg( hwnd, WM_CLOSE, 0L, 0L );
                   break;

                   default:
                        return WinDefWindowProc( hwnd, msg, mp1, mp2 );
              }

         break; // end of WM_COMMAND


         case WM_PAINT:

              if (!WinIsWindowVisible(hwnd))
              {
                   return((MRESULT)NULL);
              }

              /*
               * Check our Presentation Space. If a bitmap exists,
               * then display it. Otherwise, do our normal stuff.
               */

              if (hpsMemory)
              {
                   MakeItBusy ( TRUE );
                   DrawBitMap(hwnd);
                   MakeItBusy ( FALSE );
              }

              WinDefWindowProc ( hwnd, msg, mp1, mp2 );

         break;
         return((MRESULT)TRUE);

         case WM_CLOSE:
              WinPostMsg( hwnd, WM_QUIT, 0L, 0L );  /* Cause termination */
         break;

         /*
          *  Everything else comes here.
          */

         default:
              return WinDefWindowProc( hwnd, msg, mp1, mp2 );
    }

    return FALSE;
}

/**********************************************************************/
/*                                                                    */
/*  Name:   OpenFileDialog                                            */
/*                                                                    */
/*  Purpose: open the standard file open dialog as file extention     */
/*           and return the filename                                  */
/*                                                                    */
/*  Usage:   called when the user needs to supply a name for          */
/*           the file to be opened                                    */
/*                                                                    */
/*  Method:  calls the standard file open dialog to get the           */
/*           file name.                                               */
/*                                                                    */
/*  Parameters: HWD hwnd         Handle of the owner window.          */
/*              PSZ szTitle      Title of open dialog.                */
/*              PSZ pszFileExt   File extention. (for example : *.txt)*/
/*              PSZ pszFullPath  PSZ for returning the file name.     */
/*                                                                    */
/*  Returns: TRUE if successful in getting a file name, FALSE         */
/*              if not in pushing CANCEL                              */
/*           PSZ pszFullPath pointer to filename (full path)          */
/*                                                                    */
/**********************************************************************/

BOOL OpenFileDialog(HWND hwndOwner,
                    PSZ szTitle,
                    PSZ szFileExt,
                    PSZ szButton,
                    PSZ szFullPath )
{
    FILEDLG fdg;

    memset(&fdg, 0, sizeof(FILEDLG));
    fdg.cbSize = sizeof(FILEDLG);          /* Size of FILEDLG.                */

    fdg.pszTitle = szTitle;                /* String to display in title bar. */

    fdg.pszOKButton = szButton ;

    fdg.ulUser = 0L;                       /* User defined field.             */
    fdg.fl = FDS_CENTER |
             FDS_OPEN_DIALOG;

    fdg.pfnDlgProc = NULL;
    fdg.lReturn = 0L;                      /* Result code from dialog dismissal. */
    fdg.lSRC = 0L;                         /* System return code.          */
    fdg.hMod = 0;                          /* Custom file dialog template. */
    fdg.usDlgId = 0;                       /* Custom file dialog ID.       */
    fdg.x = 100;                           /* X coordinate of the dialog.  */
    fdg.y = 100;                           /* Y coordinate of the dialog.  */

    /* set selected fully qualified path */
    strcpy( fdg.szFullFile, szFileExt);


    /* get the file */

    if ( !WinFileDlg ( HWND_DESKTOP,
                       hwndOwner,
                       (PFILEDLG)&fdg ) )
       return FALSE;


    /* copy file name into file name buffer */

    strcpy ( szFullPath, fdg.szFullFile );
    if (fdg.lReturn == DID_CANCEL)
    return FALSE;
    else
    return(TRUE);


}

/*************************************************************************
 * Name        :  DisplayMessageBox
 *
 * Description :  This procedure will display messages for the application
 *                based upon string IDs passed in.  The actual text will be
 *                loaded from the string table in the resource.
 *
 * Parameters  :  usTitleID   - ID of the title string
 *                usMessageID - ID of the message string
 *                usStyle     - Style of the message box (WinMessageBox)
 *
 * Return      :  none.
 *
 *************************************************************************/

VOID DisplayMessageBox ( HWND   hwnd,
                         USHORT usTitleID,
                         USHORT usMessageID,
                         USHORT usStyle)
{
   CHAR     achTitle[STRING_LENGTH];
   CHAR     achMessage[STRING_LENGTH];
   USHORT   usResult;

   /*
    * Get the string from the Resource defined string table and show it
    * in the message box.
    */

   WinLoadString(
      hab,                          /* HAB for this dialog box.            */
      (HMODULE) NULL,               /* Get the string from the .exe file.  */
      usTitleID,                    /* Which string to get.                */
      (SHORT) sizeof( achTitle),    /* The size of the buffer.             */
      achTitle );                   /* The buffer to place the string.     */

   WinLoadString(
      hab,                          /* HAB for this dialog box.            */
      (HMODULE) NULL,               /* Get the string from the .exe file.  */
      usMessageID,                  /* Which string to get.                */
      (SHORT) sizeof( achMessage),  /* The size of the buffer.             */
      achMessage );                 /* The buffer to place the string.     */

   usResult =
      WinMessageBox(
         HWND_DESKTOP,              /* Parent handle of the message box.   */
         hwnd,                      /* Owner handle of the message box.    */
         achMessage,                /* String to show in the message box.  */
         achTitle,                  /* Title to shown in the message box.  */
         (HMODULE) NULL,            /* Message Box Id.  Not Needed.        */
         (ULONG) usStyle );         /* The style of the message box.       */

}  /* End of DisplayMessageBox */


/*
 * Get FOURCC Dialog Box Procedure
 */

MRESULT EXPENTRY GetFOURCCDialogProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   HWND  hwndEntryField;         /* Window handle for entry field */

    switch ( msg )
    {
         case WM_INITDLG:

              /* Set the maximum number of characters in the
               * entry field to 4.
               */
              hwndEntryField = WinWindowFromID ( hwndDlg, GET_FOURCC_TEXT );
              WinSendMsg ( hwndEntryField, EM_SETTEXTLIMIT,
                         (MPARAM) 4, (MPARAM) NULL );
              break;


         case WM_COMMAND:                    /* Posted by pushbutton or key  */

              /*
               * PM sends a WM_COMMAND message when the user presses either
               * the Enter or Escape pushbuttons.
               */
              switch( SHORT1FROMMP( mp1 ) )     /* Extract the command value    */
              {

                   case GET_FOURCC_OK_BUTTON:

                        WinQueryWindowText( WinWindowFromID( hwndDlg,
                                                             GET_FOURCC_TEXT ),
                                            STRING_LENGTH,
                                            szFOURCC );

                        WinPostMsg( hwndDlg, WM_CLOSE, 0L, 0L );
                   break;

                   case GET_FOURCC_CANCEL_BUTTON:
                   WinDismissDlg(hwndDlg, FALSE);
                   Cancel = TRUE;
                   return (MRESULT) FALSE;
                   break;

                   case GET_FOURCC_HELP_BUTTON:
                        WinSendMsg ( hwndHelpInstance,
                                     HM_DISPLAY_HELP,
                                     MPFROM2SHORT(HELP_GET_FOURCC,NULL),
                                     MPFROMSHORT(HM_RESOURCEID ));
                   return (MRESULT) FALSE;
                   break;

                   case ID_EXITPROG:
                        WinPostMsg( hwndDlg, WM_CLOSE, 0L, 0L );
                   break;
              }

         case WM_CLOSE:

              /*
               * Come here to close the dialog box..
               */

              WinDismissDlg( hwndDlg, TRUE );  /* Removes the dialog box    */
              WinPostMsg( hwndDlg, WM_QUIT, 0L, 0L );  /* Cause termination */
              return (MRESULT) FALSE;
         break;

         default:

              /*
               * Any event messages that the dialog procedure has not processed
               * come here and are processed by WinDefDlgProc.
               */

              return WinDefDlgProc( hwndDlg, msg, mp1, mp2 );
    }
    return (MRESULT) FALSE;
}

VOID FourccToString( FOURCC fcc, PSZ pszString )
{
    pszString[0] = (BYTE) fcc;
    pszString[1] = (BYTE) (fcc >> 8);
    pszString[2] = (BYTE) (fcc >> 16);
    pszString[3] = (BYTE) (fcc >> 24);
    pszString[4] = 0;
}

/*
 * Remove permanently installed IO Procs Dialog Box procedure.
 */

MRESULT EXPENTRY RemovePermDialogProc( HWND   hwnd,
                                       USHORT msg,
                                       MPARAM mp1,
                                       MPARAM mp2 )
{
    ULONG         ulReturnCode;
    CHAR          szBuffer[ sizeof( FOURCC ) + MAX_FILENAME_SIZE + 4 ];
    MMINIFILEINFO mmIniFileInfo;
    PCHAR         pszFourCC;
    SHORT         sItemIndex;
    CHAR          szParsedFourCC [ sizeof( FOURCC) + 1 ] ;
    CHAR          szMessage[ MAX_FILENAME_SIZE + MAX_FILENAME_SIZE ];
    BOOL          bTest;
    switch( msg )

    {

         case WM_INITDLG :

              memset ( &mmIniFileInfo,
                       '\0',
                       sizeof ( MMINIFILEINFO ) );

              pszFourCC = malloc ( 4 );

              strcpy ( mmIniFileInfo.szProcName, szIOProcName );

              ulReturnCode = mmioIniFileHandler ( &mmIniFileInfo,
                                                  MMIO_FINDPROC   |
                                                  MMIO_MATCHFIRST |
                                                  MMIO_MATCHPROCEDURENAME );

              if( ulReturnCode == MMIOERR_MATCH_NOT_FOUND )
                   break;

              if( ulReturnCode != MMIO_SUCCESS )
                   break;

              /*
               *  We need to fill the listbox with the installed I/O procs.
               *  To do this, use mmioIniFileHandler with Query Next flag.
               */
              while( TRUE )   // forever loop - well, until we break
              {
                        szBuffer[0] = '\0';
                        FourccToString ( mmIniFileInfo.fccIOProc,
                                    pszFourCC );

                   sprintf ( szBuffer,
                             "%s - %s",
                             pszFourCC,
                             mmIniFileInfo.szDLLName );

                   /*
                    * Add this entry to the end of the list box.
                    */

                   WinSendMsg( WinWindowFromID( hwnd,
                               REMOVE_PERM_LIST_BOX_ENTRIES),
                               LM_INSERTITEM,
                               (MPARAM) LIT_END,
                               szBuffer);
                       bTest = 1;
                       while(bTest == 1)
                          {
                         ulReturnCode = mmioIniFileHandler ( &mmIniFileInfo,
                                                            MMIO_FINDPROC |
                                                            MMIO_MATCHNEXT ) ;

                        if (( ulReturnCode == MMIOERR_MATCH_NOT_FOUND ) ||
                        ( ulReturnCode != MMIO_SUCCESS ))
                        return FALSE;


                        bTest =strcmp(mmIniFileInfo.szProcName, "IOProc_Entry");
                        if(bTest == 0)
                            bTest = 0;
                        else
                            bTest = 1;
                          }
                }
              return( (MRESULT) 0);
         break;

         case WM_CLOSE :

              WinDismissDlg( hwnd, TRUE );  /* Removes the dialog box    */
              WinPostMsg( hwnd, WM_QUIT, 0L, 0L );  /* Cause termination */
              return (MRESULT) FALSE;

         break;

         case WM_HELP :
              WinSendMsg ( hwndHelpInstance,
                           HM_DISPLAY_HELP,
                           MPFROMLONG ( 1 ),
                           MPFROMSHORT ( HM_RESOURCEID ) );

              return( 0 );
         break;

         case WM_COMMAND :

              /*
               * To get which pushbutton was pressed the SHORT1FROMMP macro
               * is used.
               */

              switch (SHORT1FROMMP(mp1))
              {

                   case REMOVE_PERM_LIST_BOX_OK:

                        /*
                         * Remove highlighted IO procs
                         */

                        sItemIndex = (SHORT)WinSendMsg ( WinWindowFromID( hwnd,
                                                  REMOVE_PERM_LIST_BOX_ENTRIES),
                                                  LM_QUERYSELECTION,
                                                  MPFROMLONG ( LIT_FIRST ),
                                                  (MPARAM) NULL );

                        while( sItemIndex != LIT_NONE)   // change to LIT_NONE
                        {

                             WinSendMsg ( WinWindowFromID( hwnd,
                                          REMOVE_PERM_LIST_BOX_ENTRIES),
                                          LM_QUERYITEMTEXT,
                                          MPFROM2SHORT ( sItemIndex,
                                                         sizeof( szBuffer )),
                                          szBuffer );
                             /*
                              * Take buffer and parse for FOURCC and DLL name.
                              */

                             memset ( &mmIniFileInfo,
                                      '\0',
                                      sizeof ( MMINIFILEINFO ) );

                             szParsedFourCC[ 0 ] = '\0';

                             strncat ( szParsedFourCC, &szBuffer[0], 4 );

                             mmIniFileInfo.fccIOProc = mmioStringToFOURCC ( szParsedFourCC ,
                                                                            0 );

                             strcpy ( mmIniFileInfo.szDLLName, &szBuffer[7] );

                             strcpy ( mmIniFileInfo.szProcName, szIOProcName );

                             ulReturnCode = mmioIniFileHandler ( &mmIniFileInfo,
                                                                 MMIO_MATCHDLL |
                                                                 MMIO_MATCHFOURCC |
                                                                 MMIO_REMOVEPROC);
                             if( ulReturnCode == MMIO_SUCCESS )
                             {
                                  sprintf ( szMessage,
                                            "%s %s (FOURCC value: %s)",
                                            "Successfully removed. Please Reboot to have removal take permanent effect.",
                                            mmIniFileInfo.szDLLName,
                                            szParsedFourCC );
                                  WinMessageBox( HWND_DESKTOP,
                                                 hwnd,
                                                 szMessage,
                                                 "Remove Permanent IO Proc",
                                                 (HMODULE) NULL,
                                                 (ULONG) MB_OK | MB_MOVEABLE |
                                                         MB_INFORMATION );
                             }
                             else
                             {
                                  sprintf ( szMessage ,
                                            "%s %s %s %ul."
                                            "Could not remove",
                                            szMessage,
                                            "Return code from mmioIniFileHandler:",
                                            ulReturnCode );

                                  WinMessageBox( HWND_DESKTOP,
                                                 hwnd,
                                                 szMessage,
                                                 "Remove Permanent IO Proc",
                                                 (HMODULE) NULL,
                                                 (ULONG) MB_OK | MB_MOVEABLE |
                                                         MB_ERROR );
                             }

                             /*
                              * Get next selected item in the list box.
                              */

                             sItemIndex = (SHORT) WinSendMsg ( WinWindowFromID( hwnd, REMOVE_PERM_LIST_BOX_ENTRIES),
                                                       LM_QUERYSELECTION,
                                                       MPFROMLONG (sItemIndex),
                                                       (MPARAM) NULL );
                        }

                        WinSendMsg ( hwnd, WM_CLOSE, (MPARAM) NULL, (MPARAM) NULL);
                        return( (MRESULT) FALSE);

                   break;

                   case REMOVE_PERM_LIST_BOX_CANCEL:   /* user selected "Cancel"  */
                        WinSendMsg( hwnd, WM_CLOSE, (MPARAM) NULL, (MPARAM) NULL);
                   break;

                   case REMOVE_PERM_LIST_BOX_HELP:

                        WinSendMsg ( hwndHelpInstance,
                                     HM_DISPLAY_HELP,
                                     MPFROM2SHORT(ID_HELP_REMOVE_PERM_LIST_BOX, NULL),
                                     MPFROMSHORT(HM_RESOURCEID ));
                   break;

                   default:
                   break;

               }  /* End of Command Switch */
               return( (MRESULT) 0);
         break;

         default:
               return( WinDefDlgProc( hwnd, msg, mp1, mp2));
         break;

    }  /* End of msg Switch */

    return( (MRESULT) FALSE);

}

HBITMAP LoadBitmap ( HAB hab,
                     HDC hdc,
                     HPS *hps,
                     PSZ pszFileName )
{
    HBITMAP       hbm;
    MMIOINFO      mmioinfo;
    MMFORMATINFO  mmFormatInfo;
    HMMIO         hmmio;
    ULONG         ulImageHeaderLength;
    MMIMAGEHEADER mmImgHdr;
    ULONG         ulBytesRead;
    ULONG         dwNumRowBytes;
    PBYTE         pRowBuffer;
    ULONG         dwRowCount;
    SIZEL         ImageSize;
    ULONG         dwHeight, dwWidth;
    SHORT          wBitCount;
    FOURCC        fccStorageSystem;
    ULONG         dwPadBytes;
    ULONG         dwRowBits;
    ULONG         ulReturnCode;
    ULONG         dwReturnCode;
    HBITMAP       hbReturnCode;
    LONG          lReturnCode;
    FOURCC        fccIOProc;


    ulReturnCode = mmioIdentifyFile ( pszFileName,
                                      0L,
                                      &mmFormatInfo,
                                      &fccStorageSystem,
                                      0L,
                                      0L);

    /*
     *  If this file was NOT identified, then this function won't
     *  work, so return an error by indicating an empty bitmap.
     */

    if ( ulReturnCode == MMIO_ERROR )
    {
         return (0L);
    }

    /*
     *  If mmioIdentifyFile did not find a custom-written IO proc which
     *  can understand the image file, then it will return the DOS IO Proc
     *  info because the image file IS a DOS file.
     */

    if( mmFormatInfo.fccIOProc == FOURCC_DOS )
    {

         WinMessageBox( HWND_DESKTOP,
                        HWND_DESKTOP,
                        "Image file could not be interpreted by any permanently or temporarily installed IO procedures.",
                        "Open Image File",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                                MB_ERROR );
         return ( 0L );
    }

    /*
     *  Ensure this is an IMAGE IOproc, and that it can read
     *  translated data
     */

    if ( (mmFormatInfo.ulMediaType != MMIO_MEDIATYPE_IMAGE) ||
         ((mmFormatInfo.ulFlags & MMIO_CANREADTRANSLATED) == 0) )
    {

         WinMessageBox( HWND_DESKTOP,
                        HWND_DESKTOP,
                        "No IMAGE IO Procedures exist which can translate the data in the image file specified.",
                        "Open Image File",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                                MB_ERROR );
         return (0L);
    }
    else
    {
         fccIOProc = mmFormatInfo.fccIOProc;
    }

    /* Clear out and initialize mminfo structure */

    memset ( &mmioinfo,
             0L,
             sizeof ( MMIOINFO ) );

    mmioinfo.fccIOProc = fccIOProc;
    mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;

    hmmio = mmioOpen ( (PSZ) pszFileName,
                       &mmioinfo,
                       MMIO_READ | MMIO_DENYWRITE | MMIO_NOIDENTIFY );

    if ( ! hmmio )
    {
         // If file could not be opened, return with error

         return (0L);
    }

    dwReturnCode = mmioQueryHeaderLength ( hmmio,
                                         (PLONG)&ulImageHeaderLength,
                                           0L,
                                           0L);

    if ( ulImageHeaderLength != sizeof ( MMIMAGEHEADER ) )
    {
         /* We have a problem.....possibly incompatible versions */

         ulReturnCode = mmioClose (hmmio, 0L);

         return (0L);
    }

    ulReturnCode = mmioGetHeader ( hmmio,
                                   &mmImgHdr,
                                   (LONG) sizeof ( MMIMAGEHEADER ),
                                   (PLONG)&ulBytesRead,
                                   0L,
                                   0L);

    if ( ulReturnCode != MMIO_SUCCESS )
    {
         /* Header unavailable */

         ulReturnCode = mmioClose (hmmio, 0L);

         return (0L);
    }

    /*
     *  Determine the number of bytes required, per row.
     *      PLANES MUST ALWAYS BE = 1
     */

    dwHeight = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
    dwWidth = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx;
    wBitCount = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwRowBits = dwWidth * mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwNumRowBytes = dwRowBits >> 3;

    /*
     *  Account for odd bits used in 1bpp or 4bpp images that are
     *  NOT on byte boundaries.
     */

    if ( dwRowBits % 8 )
    {
         dwNumRowBytes++;
    }

    /*
     *  Ensure the row length in bytes accounts for byte padding.
     *  All bitmap data rows must are aligned on LONG/4-BYTE boundaries.
     *  The data FROM an IOProc should always appear in this form.
     */

    dwPadBytes = ( dwNumRowBytes % 4 );

    if ( dwPadBytes )
    {
         dwNumRowBytes += 4 - dwPadBytes;
    }

    /* Allocate space for ONE row of pels */

    if ( DosAllocMem( (PPVOID)&pRowBuffer,
                      (ULONG)dwNumRowBytes,
                      fALLOC))
    {
         ulReturnCode = mmioClose (hmmio, 0L);

         return(0L);
    }

    // ***************************************************
    // Create a memory presentation space that includes
    // the memory device context obtained above.
    // ***************************************************

    ImageSize.cx = dwWidth;
    ImageSize.cy = dwHeight;

    *hps = GpiCreatePS ( hab,
                         hdc,
                         &ImageSize,
                         PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC );

    if ( !*hps )
    {
         mmioClose (hmmio, 0L);
         return(0L);
    }

    // ***************************************************
    // Create an uninitialized bitmap.  This is where we
    // will put all of the bits once we read them in.
    // ***************************************************

    hbm = GpiCreateBitmap ( *hps,
                            &mmImgHdr.mmXDIBHeader.BMPInfoHeader2,
                            0L,
                            NULL,
                            NULL);

    if ( !hbm )
    {
         ulReturnCode = mmioClose (hmmio, 0L);

         return(0L);
    }

    // ***************************************************
    // Select the bitmap into the memory device context.
    // ***************************************************

    hbReturnCode = GpiSetBitmap ( *hps,
                                  hbm );

    //***************************************************************
    //  LOAD THE BITMAP DATA FROM THE FILE
    //      One line at a time, starting from the BOTTOM
    //*************************************************************** */

    for ( dwRowCount = 0; dwRowCount < dwHeight; dwRowCount++ )
    {
         ulBytesRead = (ULONG) mmioRead ( hmmio,
                                          pRowBuffer,
                                          dwNumRowBytes );

         if ( !ulBytesRead )
         {
              break;
         }

         /*
          *  Allow context switching while previewing.. Couldn't get
          *  it to work. Perhaps will get to it when time is available...
          */

         lReturnCode = GpiSetBitmapBits ( *hps,
                                          (LONG) dwRowCount,
                                          (LONG) 1,
                                          (PBYTE) pRowBuffer,
                                          (PBITMAPINFO2) &mmImgHdr.mmXDIBHeader.BMPInfoHeader2);

    }

    ulReturnCode = mmioClose (hmmio, 0L);

    DosFreeMem(pRowBuffer);

    return(hbm);
}


/*
 *  GetBitMap
 *
 *  Get a bitmap for the specified file
 */

HBITMAP GetBitMap ( HWND hwnd,
                    PSZ  pszFileName)
{
    HBITMAP hBmp;
    BOOL    bReturnCode;

    /*
     *  Load Bitmap, which will then be drawn during WM_PAINT processing
     *
     *  First, we need a memory device context. We'll keep this around
     *  to reuse for subsequent preview operations
     */

    if ( !hdcMemory )
    {
         hdcMemory = DevOpenDC ( WinQueryAnchorBlock(hwnd),
                                 OD_MEMORY,
                                 "*",
                                 0L,
                                 NULL,
                                 0);
    }

    /*
     *  Discard previous memory presentation space if present
     */

    if ( hpsMemory )
    {
         hBmp = GpiSetBitmap ( hpsMemory,
                               0);

         bReturnCode = GpiDestroyPS ( hpsMemory );

         hpsMemory = 0;
    }

    /*
     * Delete previously loaded bitmap if present
     */

    if ( hBmp && hBmp != HBM_ERROR )
    {
         bReturnCode = GpiDeleteBitmap ( hBmp );
    }

    MakeItBusy ( TRUE );

    hBmp = LoadBitmap ( WinQueryAnchorBlock(hwnd),
                        hdcMemory,
                        &hpsMemory,
                        pszFileName );

    MakeItBusy ( FALSE );

    if (!hBmp)
    {
    // Error loading bitmap
    return hBmp;
    }

    /*
     *  Be sure that the image gets repainted
     */

    bReturnCode = WinInvalidateRect(hwnd, NULL, FALSE);

    return hBmp;
}

/*
 *  DrawBitMap
 *
 *  Draw the previously loaded bitmap in the rectangle occupied by the
 *  frame ID_WINDOW.
 */

VOID DrawBitMap ( HWND hwnd )
{
    SWP    swp;
    POINTL aptl[4];
    HPS    hps;
    SIZEL  ImageSize;
    BOOL   bReturnCode;
    ULONG  ulReturnCode;
    LONG   lReturnCode;


    /*
     *  Validate memory presentation space before attempting to draw bitmap
     */

    ulReturnCode = GpiQueryPS ( hpsMemory, &ImageSize) ;

    if ( ! ulReturnCode )
    {
         return;
    }

    hps = WinBeginPaint ( hwnd,
                          0,
                          NULL);

    /*
     *  Get position of image frame
     */

    bReturnCode = WinQueryWindowPos ( hwnd, &swp);

    aptl[0].x = 0;              // target lower left
    aptl[0].y = 0;

    aptl[1].x = swp.cx;         // target upper right
    aptl[1].y = swp.cy;

    aptl[2].x = 0;              // source lower left
    aptl[2].y = 0;

    aptl[3].x = ImageSize.cx;   // source upper right
    aptl[3].y = ImageSize.cy;

    /*
     *  Call GpiBitBlt and supply 4 aptl structures.  This tells
     *  it to stretch or compress the bitmap depending on what is
     *  in the aptl structures.  See above lines for their current
     *  settings.
     */

    bReturnCode = GpiSetColor (hps, GpiQueryColor (hpsMemory));

    bReturnCode = GpiSetBackColor (hps, GpiQueryBackColor (hpsMemory));

    lReturnCode = GpiBitBlt (hps, hpsMemory, 4L, aptl, ROP_SRCCOPY, BBO_OR);

    bReturnCode = WinEndPaint (hps);
}

/*
 * Install a temporary IO Proc (into memory).
 *
 * Returns:
 *
 *    TRUE  - Error encountered.
 *    FALSE - Temporary IO Proc installed successfully.
 */

BOOL InstallTempIOProc ( HWND hwnd )
{
    ULONG     ulReturnCode;
    PMMIOPROC ioReturnCode;
    HMODULE   hDLLModule;
    CHAR      Failure[ CCHMAXPATH ];
    PMMIOPROC pIOProc;
    FOURCC    fccIOProc;


    ulReturnCode = DosLoadModule ( Failure,           // Obj name if failure
                                   CCHMAXPATH,        // Obj name size
                                   pszFileName,
                                   &(hDLLModule));     // Handle to the module.

    if ( ulReturnCode != NO_ERROR )
    {
         /*
          * Could not load the DLL into memory.
          */

         WinMessageBox( HWND_DESKTOP,
                        hwnd,
                        "Could not load the IO Procedure into memory. Please contact author.",
                        "Install Temporary IO Procedure",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                                MB_ERROR );
         return ( TRUE );
    }

    /*
     * Get the address of the IO Proc using DosQueryProcAddr.
     */

    ulReturnCode = DosQueryProcAddr ( hDLLModule,           // handle to MIDI module
                                      0,                 // get entry point
                                      "IOProc_Entry",       // ASCII procedure name
                                      (PFN *) &pIOProc );   // addr of IO Proc.

    if ( ulReturnCode != NO_ERROR )
    {
         /*
          * Could not obtain procedure address
          */

         WinMessageBox( HWND_DESKTOP,
                        hwnd,
                        "Could not obtain IO Procedure address. Please contact author.",
                        "Install Temporary IO Procedure",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                                MB_ERROR );
         return ( TRUE );
    }

    /*
     *  Ask user for the FOURCC of this temporary IO Procedure to install.
     */

    ulReturnCode = WinDlgBox ( HWND_DESKTOP,
                               (HWND) hwnd,
                               (PFNWP) GetFOURCCDialogProc,
                               (HMODULE) NULL,
                               GET_FOURCC,   // dialog template identity
                               (PVOID) NULL);

    if (Cancel == TRUE)
    {
           WinMessageBox( HWND_DESKTOP,
                          hwnd,
                         "Installation of Temporary IO Procedure failed.",
                         "Installed Temporary IO Procedure",
                          (HMODULE) NULL,
                          (ULONG) MB_OK | MB_MOVEABLE |
                          MB_ERROR );
        return (TRUE);
    } /* endif */
    fccIOProc = mmioStringToFOURCC ( szFOURCC, 0 ) ;

    ioReturnCode = mmioInstallIOProc ( fccIOProc,          // 4 char code
                                       pIOProc,            // addr of IOProc
                                       MMIO_INSTALLPROC ); // install it

    if ( ioReturnCode == NULL )
    {
         /*
          * Could not install IO procedure using mmioInstallIOProc.
          */

         WinMessageBox( HWND_DESKTOP,
                        hwnd,
                        "MMIO failed to install IO Procedure. Please contact author.",
                        "Install Temporary IO Procedure",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE | MB_ERROR );
         return ( TRUE );
    }

    /*
     *  If we got all the way here, then we successfully installed the
     *  temporary IO procedure.
     */

    return ( FALSE );

}

MRESULT EXPENTRY QueryAllDialogProc( HWND   hwnd,
                                     USHORT msg,
                                     MPARAM mp1,
                                     MPARAM mp2 )
{
    CHAR          szBuffer[ sizeof( FOURCC ) + MAX_FILENAME_SIZE + 4 ];
    LONG          lNumIOProcs;
    MMFORMATINFO  mmFormatInfo;
    PMMFORMATINFO pmmFormatInfoArray;
    PSZ           pszFourccString;
    ULONG         ulReturnCode;
    LONG          lFormatsRead;
    LONG          index;
    LONG          lBytesRead;

    switch( msg )
    {

         case WM_INITDLG :

              /*
               * Call mmioQueryFormatCount, which will return the number
               * of formats which are supported.
               */

              memset( &mmFormatInfo,
                      '\0',
                      sizeof(MMFORMATINFO) );

              mmFormatInfo.ulMediaType |= MMIO_MEDIATYPE_IMAGE;

              ulReturnCode = mmioQueryFormatCount ( &mmFormatInfo,
                                                    &lNumIOProcs,
                                                    0,
                                                    0 );

              if( ulReturnCode != MMIO_SUCCESS )
              {
                   /*
                    * Error - mmioQueryFormatCount failed.
                    */

                   break;
              }

              /*
               * Allocate enough memory for n number of FormatInfo blocks
               */

              pmmFormatInfoArray = malloc (lNumIOProcs * sizeof( MMFORMATINFO ) );

              if( pmmFormatInfoArray == NULL )
              {
                   /*
                    * Could not allocate enough memory for mmFormatInfo array.
                    */

                   break;
              }

              /*
               * call mmioGetFormats to get info on the formats supported.
               */

              ulReturnCode = mmioGetFormats( &mmFormatInfo,
                                             lNumIOProcs,
                                             pmmFormatInfoArray,
                                             &lFormatsRead,
                                             0,
                                             0 );

              if( ulReturnCode != MMIO_SUCCESS )
              {
                   /*
                    *  mmioGetFormats failed.
                    */

                   break;
              }

              if( lFormatsRead != lNumIOProcs )
              {
                   /*
                    * Error in MMIO - number of formats read in by
                    * mmioGetFormats is not equal to number of formats
                    * found by mmioQueryFormatCount.
                    */

                   break;
              }

              pszFourccString = malloc ( 4 );

              /*
               * Fill listbox entries with correct info.
               */

              for ( index = 0; index <lNumIOProcs; index++ )
              {

                   mmioGetFormatName(pmmFormatInfoArray, szBuffer, &lBytesRead, 0L, 0L);
                   FourccToString ( pmmFormatInfoArray->fccIOProc,
                                   pszFourccString );

                   /* Insert NULL string terminator */

                   *( szBuffer + lBytesRead ) = (CHAR)NULL;

                   WinSendMsg( WinWindowFromID( hwnd,
                               QUERY_ALL_LIST_BOX_ENTRIES),
                               LM_INSERTITEM,
                               (MPARAM) LIT_END,
                               szBuffer);  // buffer to insert into entry

                   /*
                    *  advance to next entry in mmFormatInfo array
                    */

                   pmmFormatInfoArray++;
              }
              return( (MRESULT) 0);
         break;

         case WM_CLOSE :

              WinDismissDlg( hwnd, TRUE );  /* Removes the dialog box    */
              WinPostMsg( hwnd, WM_QUIT, 0L, 0L );  /* Cause termination */
              return (MRESULT) FALSE;
         break;

         case WM_HELP :
              WinSendMsg ( hwndHelpInstance,
                           HM_DISPLAY_HELP,
                           MPFROMLONG ( 1 ),
                           MPFROMSHORT ( HM_RESOURCEID ) );

              return( 0 );
         break;
         case WM_COMMAND :
              /*
               * To get which pushbutton was pressed the SHORT1FROMMP macro
               * is used.
               */
              switch (SHORT1FROMMP(mp1))
              {

                   case QUERY_ALL_LIST_BOX_OK:
                        WinSendMsg ( hwnd, WM_CLOSE, (MPARAM) NULL, (MPARAM) NULL);
                        return( (MRESULT) FALSE);
                   break;

                   case QUERY_ALL_LIST_BOX_CANCEL:   /* user selected "Cancel"  */
                        WinSendMsg( hwnd, WM_CLOSE, (MPARAM) NULL, (MPARAM) NULL);
                   break;

                   case QUERY_ALL_LIST_BOX_HELP:
                        WinSendMsg (hwndHelpInstance,
                                    HM_HELP_INDEX,
                                    MPVOID,
                                    MPVOID);
                  break;

                   default:
                   break;

               }  /* End of Command Switch */
               return( (MRESULT) 0);
         break;

         default:
               return( WinDefDlgProc( hwnd, msg, mp1, mp2));
         break;

    }  /* End of msg Switch */

    return( (MRESULT) FALSE);

}

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
 * MME API's   : None.
 *
 * Parameters  : None.
 *
 * Return      : TRUE - error encountered
 *               FALSE - no errors encountered - help instance created!
 *
 *************************************************************************/


VOID InitializeHelp( HWND hwndFrame)
{

    HELPINIT hini;
    fHelpEnabled = FALSE;
    /* if we return because of an error, Help will be disabled */

    /* initialize help init structure */

    hini.cb = sizeof(HELPINIT);
    hini.ulReturnCode = 0;
    hini.pszTutorialName = (PSZ)NULL;   /* if tutorial added, add name here */
    hini.phtHelpTable = (PHELPTABLE)MAKELONG(MMBROWSER_HELP_TABLE, 0xFFFF);
    hini.hmodHelpTableModule = 0L;
    hini.hmodAccelActionBarModule = 0L;
    hini.idAccelTable = 0L;
    hini.idActionBar = 0L;
    hini.pszHelpWindowTitle = "MMBrowser Help";
    hini.fShowPanelId = CMIC_HIDE_PANEL_ID;
    hini.pszHelpLibraryName = "MMBROWSE.HLP";

    /* creating help instance */

    hwndHelpInstance = WinCreateHelpInstance(hab, &hini);

    if(!hwndHelpInstance || hini.ulReturnCode)
    {
        return;
    }

    /* associate help instance with main frame */

    if(!WinAssociateHelpInstance(hwndHelpInstance, hwndFrame))
    {
        return;

    }

    fHelpEnabled = TRUE;
    }

/****************************************************************
 *  Dialog procedure for the About dialog box
 *--------------------------------------------------------------
 *
 *  Name:    AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
 *
 *  Purpose: Handles all the messages associated with the About Box
 *
 *  Usage:   Called for each message sent to the About Box dialog box.
 *
 *  Method:  The about box only has a button control so this routine
 *           only processes WM_COMMAND messages. Any WM_COMMAND
 *           posted must have come from the Ok button so we dismiss
 *           the dialog upon receiving it.
 *
 *  Returns: Dependent upon message sent
 *
 ****************************************************************/
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   MRESULT sRC;

   switch(msg)
   {
      case WM_INITDLG:
//         FixSysMenu(hwnd);
         return (MRESULT)0L;

      case WM_COMMAND:
         WinDismissDlg(hwnd, TRUE);
         return (MRESULT)0L;

      default:
         sRC = WinDefDlgProc(hwnd, msg, mp1, mp2);
         return sRC;
   }
   return (MRESULT)0L;
}

/****************************************************************
 *  FixSysMenu procedure
 *--------------------------------------------------------------
 *
 *  Name:    FixSysMenu(HWND hwndDlg)
 *
 *  Purpose: This routine removes the Restore, Size, Minimize, and
 *           Maximize options from the system menu of a dialog.
 *
 *  Usage
 *
 *  Method:  Called during the WM_INITDLG of a dialog procedure.
 *
 *  Returns: VOID
 *
 ****************************************************************/
VOID FixSysMenu(HWND hwndDlg)
{
   HWND hwndMenu;                      /* Handle to system menu */

   hwndMenu = WinWindowFromID(hwndDlg, FID_SYSMENU);

   WinSendMsg(hwndMenu,  /* Delete Restore from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_RESTORE, TRUE),
              MPFROMP(NULL));

   WinSendMsg(hwndMenu,     /* Delete Size from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_SIZE, TRUE),
              MPFROMP(NULL));

   WinSendMsg(hwndMenu, /* Delete Minimize from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_MINIMIZE, TRUE),
              MPFROMP(NULL));

   WinSendMsg(hwndMenu, /* Delete Maximize from the system menu */
              MM_DELETEITEM,
              MPFROM2SHORT(SC_MAXIMIZE, TRUE),
              MPFROMP(NULL));
   return;

}

//*******************************************************************
// Function : DoConvert
//
//
// Description :
//    This function provides a simple method of converting an
//    image from one format to another, so long as both are
//    supported by OS/2 MME MMIO IOProcs.  It uses MMIO
//    functions to transparently open, read and write file headers,
//    read and write image data, and close the files.
//
//    This function will also signal the progress of the conversion
//    so that this progress can illustrated to a user.
//
// Parameters (all input, unchanged by function):
//      Source file name
//      Source file IOProc handler
//      Target file name
//      Target file IOProc handler
//
// Return Code:
//      0 if successful, or
//        ERR_CANCELLED
//        ERR_SOURCEOPEN
//        ERR_TARGETOPEN
//        ERR_SOURCEHEADER
//        ERR_TARGETHEADER
//        ERR_SOURCEWRITE
//        ERR_TARGETWRITE
//        ERR_SOURCECLOSE
//        ERR_TARGETCLOSE
//        ERR_MEMORY
//
//********************************************************************
BOOL DoConvert(
   PSZ    pszSource,          // Source file name
   FOURCC fccSourceIOProc,
   PSZ    pszTarget,
   FOURCC fccTargetIOProc)    //
   {
   MMIOINFO      mmioinfoSource;
   MMIOINFO      mmioinfoTarget;
   HMMIO         hmmioSource;
   HMMIO         hmmioTarget;
   ULONG         ulImageHeaderLength;
   MMIMAGEHEADER mmImgHdr;
   ULONG         ulBytesRead;
   ULONG         dwNumRowBytes;
   PBYTE         pRowBuffer;
   ULONG         dwRowCount;
   ULONG         dwWidth;
   ULONG         dwHeight;
   ULONG         dwPadBytes;
   ULONG         dwRowBits;
   LONG          rc;
   LONG          rcSrcQueryCount = 0;
   LONG          rcTrgQueryCount = 0;
   LONG          rcTrgSetImage   = 0;
   ULONG         iIndex, iCount, iCount2;



   /*******************************/
   /* Set up/open the SOURCE file */
   /*******************************/
   memset (&mmioinfoSource, 0L, sizeof (MMIOINFO));
   mmioinfoSource.fccIOProc = fccSourceIOProc;
   mmioinfoSource.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
   memset (&mmioinfoTarget, 0L, sizeof (MMIOINFO));
   mmioinfoTarget.fccIOProc = fccTargetIOProc;
   mmioinfoTarget.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;

   hmmioSource = mmioOpen ((PSZ)pszSource, &mmioinfoSource,
                          MMIO_READ | MMIO_DENYWRITE
                        | MMIO_NOIDENTIFY);

   if (!hmmioSource)
      return (ERR_SOURCEOPEN);

   /*******************************/
   /* Set up/open the TARGET file */
   /*******************************/


   hmmioTarget = mmioOpen ((PSZ)pszTarget,
                           &mmioinfoTarget,
                           MMIO_CREATE | MMIO_WRITE |
                           MMIO_DENYWRITE | MMIO_NOIDENTIFY);

   if (!hmmioTarget)
      {
      mmioClose (hmmioSource, 0L);
      return (ERR_TARGETOPEN);
      }

   // find out if source has multiple image support
   rcSrcQueryCount = mmioSendMessage(hmmioSource, MMIOM_QUERYIMAGECOUNT, (LONG)&iCount, (LONG)0);
   if (rcSrcQueryCount) iCount = 1;

   // find out if the target has multiple image support
   rcTrgQueryCount = mmioSendMessage(hmmioTarget, MMIOM_QUERYIMAGECOUNT, (LONG)&iCount2, (LONG)0);

   for (iIndex=0; iIndex<iCount; iIndex++) { /* loop through known images */

      if (!rcSrcQueryCount && !rcTrgQueryCount) {  /* if Both support images */

         /* Determine if the target can write arrays, and if not */
         /* then write the the default image from the source     */

         if (rcTrgSetImage && iIndex > 0) break;  /* Target Can't Write array */

         /* Now, determine if the target can write arrays */
         rcTrgSetImage = mmioSendMessage (hmmioTarget, MMIOM_SETIMAGE, (LONG)iIndex, (LONG)0);

         if (!rcTrgSetImage) mmioSendMessage (hmmioSource, MMIOM_SETIMAGE, (LONG)iIndex, (LONG)0);

      } else if (!rcSrcQueryCount) {      /* Source does but target doesn't */
         /* Use the default image from source to copy to target */
         /* so do set the index of the first, let it default    */
         /* get the base photo cd image (2 of 5)                */
         if (iIndex > 0) break;
      } else if (!rcTrgQueryCount) {      /* Target does but source doesn't */
         /* Use the only image to do a default write to target */
      } else {
         /* neither do: just write one image from here to there */
      }

      /****************************/
      /* Obtain the SOURCE HEADER */
      /****************************/
      mmioQueryHeaderLength (hmmioSource, (PLONG)&ulImageHeaderLength, 0L, 0L);
      if (ulImageHeaderLength != sizeof (MMIMAGEHEADER))
         /* We have a problem.....possibly incompatible versions */
         {
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return (ERR_SOURCEHEADER);
         }

      rc = (LONG)mmioGetHeader (hmmioSource, &mmImgHdr,
                     (LONG)sizeof (MMIMAGEHEADER), (PLONG)&ulBytesRead,
                     0L, 0L);

      if (rc != MMIO_SUCCESS)
         /* Header unavailable */
         {
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return (ERR_SOURCEHEADER);
         }


      /*************************/
      /* Set the TARGET HEADER */
      /*************************/
      mmioQueryHeaderLength (hmmioTarget, (PLONG)&ulImageHeaderLength, 0L, 0L);
      if (ulImageHeaderLength != sizeof (MMIMAGEHEADER))
         {
         /* We have a problem.....possibly incompatible versions */
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return (ERR_TARGETHEADER);
         }

      /* Use the SAME data as came from the SOURCE FILE.  It must be
         compatible with the OS/2 bitmaps, etc.   */
      rc = (LONG)mmioSetHeader (hmmioTarget, &mmImgHdr,
                     (LONG)sizeof (MMIMAGEHEADER), (PLONG)&ulBytesRead,
                     0L, 0L);

      if (rc != MMIO_SUCCESS)
         /* Header unavailable */
         {
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return (ERR_TARGETHEADER);
         }


      /* Determine the number of bytes required, per row */
      /*      PLANES MUST ALWAYS BE = 1 */
      dwHeight = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
      dwWidth  = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx;
      dwRowBits = dwWidth * mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
      dwNumRowBytes = dwRowBits >> 3;

      /* Account for odd bits used in 1bpp or 4bpp images that are NOT on byte boundaries. */
      if (dwRowBits % 8)
         dwNumRowBytes++;

      /* Ensure the row length in bytes accounts for byte padding.  All bitmap data rows
         must are aligned on LONG/4-BYTE boundaries.   The data FROM an IOProc
         should always appear in this form. */
      dwPadBytes = (dwNumRowBytes % 4);
      if (dwPadBytes)
         dwNumRowBytes += 4 - dwPadBytes;

      /* Allocate space for one row */
      if (DosAllocMem((PVOID)&pRowBuffer, (ULONG)dwNumRowBytes, fALLOC))
         {
         mmioClose (hmmioSource, 0L);
         mmioClose (hmmioTarget, 0L);
         return(ERR_MEMORY);
         }

      for (dwRowCount = 0;
           dwRowCount < dwHeight;
           dwRowCount++)
         {
         ulBytesRead = (ULONG)mmioRead (hmmioSource, pRowBuffer, dwNumRowBytes);

         if (ulBytesRead)
            mmioWrite (hmmioTarget, pRowBuffer, (ULONG)ulBytesRead);
         else
            break;

                                 }
   }

   mmioClose (hmmioSource, 0L);
   mmioClose (hmmioTarget, 0L);

   DosFreeMem(pRowBuffer);


   return(0);
   }
   VOID SaveFile (HWND hwndClient, PSZ pszSource, FOURCC fccSourceIOProc )
   {
   FILEDLG fdg;
   HWND hwndDlg;
   INT    Result;
   PSZ    Result1;
   CHAR Test[STRING_LENGTH];
   CHAR VID[]=".VID";
   CHAR DIB[]=".DIB";
   CHAR PCX[]=".PCX";
   CHAR TGA[]=".TGA";
   CHAR TIF[]=".TIF";
   CHAR RDI[]=".RDI";
   CHAR BMP[]=".BMP";
   FOURCC fccTargetIOProc;

   memset(&fdg, 0, sizeof(FILEDLG));
   fdg.cbSize = sizeof(FILEDLG);
   fdg.fl = FDS_SAVEAS_DIALOG | FDS_CENTER;
   fdg.pszTitle = "The Save File Dialog";
   strcpy(fdg.szFullFile, "*.bmp");
   hwndDlg= WinFileDlg(HWND_DESKTOP, hwndClient, (PFILEDLG) &fdg);
   if (hwndDlg && (fdg.lReturn == DID_OK))
   {
                         Result1 = strchr(fdg.szFullFile,'.');
                         if(Result1 == NULL)
                            Result1 = "BMP";

                         strcpy(Test,Result1);

                         strupr(Test);
                              if(strcmp(Test, VID) == 0 && mmioFOURCC('M','M','O','T') > 0)
                              fccTargetIOProc = mmioFOURCC('M','M','O','T');
                         else if(strcmp(Test, RDI) == 0 && mmioFOURCC('R','D','I','B') > 0)
                              fccTargetIOProc = mmioFOURCC('R','D','I','B');
                         else if(strcmp(Test, PCX) == 0 && mmioFOURCC('P','C','X','C') > 0)
                              fccTargetIOProc = mmioFOURCC('P','C','X','C');
                         else if(strcmp(Test, BMP) == 0 && mmioFOURCC('O','S','2','0') > 0)
                              fccTargetIOProc = mmioFOURCC('O','S','2','0');
                         else if(strcmp(Test, DIB) == 0 && mmioFOURCC('W','1','3','0') > 0)
                              fccTargetIOProc = mmioFOURCC('W','1','3','0');
                         else if(strcmp(Test, TGA) == 0 && mmioFOURCC('T','G','A','U') > 0)
                              fccTargetIOProc = mmioFOURCC('T','G','A','U');
                         else if(strcmp(Test, TIF) == 0 && mmioFOURCC('T','I','F','U') > 0)
                              fccTargetIOProc = mmioFOURCC('T','I','F','U');
                         else
                         {
                         WinMessageBox( HWND_DESKTOP,
                         HWND_DESKTOP,
                         "Will Save As a OS2.BMP",
                         "Save As....",
                         (HMODULE) NULL,
                         (ULONG) MB_OK | MB_MOVEABLE );
                          fccTargetIOProc = mmioFOURCC('O','S','2','0');
                          }
                          MakeItBusy(TRUE);
                          DoConvert(pszSource, fccSourceIOProc, fdg.szFullFile, fccTargetIOProc);
                          MakeItBusy(FALSE);
   }
}
