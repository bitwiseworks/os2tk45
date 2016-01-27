/******************************************************************************
 * File Name    :  RECORDER.C
 *
 * Description  :  This file contains the C source code required for the
 *                 Recorder sample program.
 *
 * Concepts     :  This sample program illustrates the concepts of recording
 *                 audio data through the media control interface. It also
 *                 illustrates how to change the characteristics of the audio
 *                 recording and the audio device (such as bits per sample,
 *                 samples per second, input level, input source, etc.)
 *
 * MMPM/2 API's :  List of all MMPM/2 API's that are used in
 *                 this module
 *
 *                 mciSendCommand    MCI_OPEN
 *                                   MCI_PLAY
 *                                   MCI_RECORD
 *                                   MCI_CLOSE
 *                                   MCI_SET
 *                                   MCI_STOP
 *                                   MCI_CONNECTOR
 *                                   MCI_CONNECTION
 *                                   MCI_STATUS
 *                                   MCI_SYSINFO
 *                                   MCI_GETDEVCAPS
 *                 mciGetErrorString
 *                 mmioOpen
 *                 mmioClose
 *
 * Required
 *    Files     :  recorder.c        Source Code.
 *                 options.c         Source Code.
 *                 recorder.h        Include file.
 *                 recorder.dlg      Dialog definition.
 *                 recorder.rc       Resources.
 *                 recorder.ipf      Help text.
 *                 makefile          Make file.
 *                 recorder.def      Linker definition file.
 *                 recorder.ico      Program icon.
 *
 * Copyright (C) IBM 1993
 ******************************************************************************/
#define  INCL_OS2MM                 /* required for MCI and MMIO headers   */
#define  INCL_WIN                   /* required to use Win APIs.           */
#define  INCL_PM                    /* required to use PM APIs.            */
#define  INCL_WINHELP               /* required to use IPF.                */
#define  INCL_SW                    /* required for seondary window        */
#define  INCL_WINSTDSLIDER          /* required for using slider control   */
#define  INCL_WINSTDFILE            /* required for open file dlg          */
#define  INCL_WINSTDDLGS            /* required for open file dlg          */

#include <os2.h>
#include <os2me.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sw.h>

#include "recorder.h"

enum DeviceStates {ST_STOPPED, ST_PLAYING, ST_RECORDING};

/*
 * Procedure/Function Prototypes
 */
MRESULT EXPENTRY MainDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY OpenFileDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY ProductInfoDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

VOID    AdjustTheDlg( HWND hwnd, BOOL fShow );
VOID    CloseTheDevice( VOID );
VOID    DeselectTheOption( USHORT idOption );
BOOL    DoesFileExist(VOID);
BOOL    FileSaveAs(HWND hwnd);
VOID    Finalize( VOID);
VOID    FindTheDeviceCapability( VOID );
VOID    EvaluateReturnCode (ULONG ulBitsPerSample, ULONG ulSampPerSec, ULONG ulChannels, ULONG ulRC);
BOOL    GetFileName( PSZ szFileName, ULONG ulStringLength);
VOID    Initialize( VOID);
VOID    InitializeHelp( VOID );
USHORT  MessageBox( USHORT usMessageID, ULONG  ulStyle );
BOOL    OpenTheFile( HWND hwnd, PSZ pszFileName );
BOOL    PlayTheAudio( HWND hwnd );
VOID    QueryTheDefaultConnection( VOID );
BOOL    RecordTheAudio( HWND hwnd );
VOID    SaveTheAudio( HWND hwnd );
USHORT  SaveTheChanges( HWND hwnd );
BOOL    SeekTheAudio( HWND hwnd, ULONG ulPosition );
VOID    SetAudioVolume( HWND hwnd );
BOOL    SetTheOptions( BOOL fSelectedFileType );
VOID    SelectTheOption( USHORT idOption );
VOID    ShowMCIErrorMessage( ULONG ulError );
BOOL    ShowStandardFileDlg( USHORT usSelection );
VOID    StopTheDevice( HWND hwnd );
VOID    UpdateTitleText( HWND hwnd, PSZ pszFileName );
VOID    UpdateTheStatusLine( HWND hwnd, USHORT usStatus );

extern VOID  DisableTheOption( HWND hwndMenu, USHORT idOption );

/*************** End of Procedure/Function Prototypes *************************/

/*
 * Global Variables.
 */
HAB     hab;
HMQ     hmq;
HWND    hwndFrame;                 /* Handle to the frame window.             */
HWND    hwndHelpInstance;          /* Handle to Main Help window.             */
HWND    hwndMainDialogBox;         /* Handle to the dialog window.            */
HWND    hwndMenu;                  /* Handle to the menu                      */
HWND    hwndPlayPB;                /* Handle to the play push button          */
HWND    hwndRecordPB;              /* Handle to the record push button        */
HWND    hwndProductInfo = 0;       /* Handle to the product info. dialog box  */

enum    DeviceStates   eState = ST_STOPPED;      /* state of the device       */

SHORT   sVolumeLevel  = INIT_VOLUME;             /* desired volume level      */
USHORT  usDeviceType  = MCI_MICROPHONE_CONNECTOR,/* for microphone or line in */
        usStereoMono  = STEREO,                  /* for stereo or mono        */
        usQualityType = MUSIC,                   /* for quality selection     */
        usInputLevel  = MEDIUM_INPUT_LEVEL,      /* for input level selection */
        usBitType     = BIT_8,                   /* for 8 bit or 16 bit       */
        usDeviceTypeOpened;                      /* for device type opened    */

BOOL    fMonoSupported        = TRUE;  /* Device supports mono files          */
BOOL    fStereoSupported      = TRUE;  /* Device supports stereo files        */
BOOL    fVoiceSupported       = TRUE;  /* Device supports 11 kHz files        */
BOOL    fMusicSupported       = TRUE;  /* Device supports 22 kHz files        */
BOOL    fHiFiSupported        = TRUE;  /* Device supports 44 kHz files        */
BOOL    fHighQualitySupported = TRUE;  /* Device supports 16 bps files        */
BOOL    fLoQualitySupported   = TRUE;  /* Device supports 8 bps files         */
BOOL    fMonitor              = TRUE;  /* for Monotor - on/off                */
BOOL    fPassedDevice         = FALSE; /* for MCI_ACQUIRE to play the file    */
BOOL    fFileOpen             = FALSE; /* for file open or not                */
BOOL    fTempFileInUse        = FALSE; /* for temporary file open or not      */
BOOL    fModified             = FALSE; /* for opened file modified or not     */
BOOL    fFirstOpen            = TRUE;  /* indicate we've opened for first time*/
CHAR    szFileName[FILE_NAME_SIZE];    /* buffer for fully qualified file name*/
CHAR    acTempFileName[MESSAGELEN];    /* buffer for temporary file name      */
CHAR    achTitle[MAXNAMEL];            /* buffer for window title text        */
CHAR    achMsgBoxTitle[MAXNAMEL];      /* Error message box title             */
MCI_OPEN_PARMS   mciOpenParms;         /* open parms for MCI_OPEN             */
HPOINTER  hptrWait, hptrArrow;  /* Pointer handles for use during long waits. */

/************************** End of Global Variables ***************************/

/******************************************************************************
 * Name         : main
 *
 * Description  : This function calls the Initialize procedure to prepare
 *                everything for the program's operation, enters the
 *                message loop, then call Finalize to shut everything down
 *                when the program is terminated.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : argc - Number of parameters passed into the program.
 *                argv - Command line parameters.
 *
 * Return       : TRUE is returned to the operating system.
 *
 ******************************************************************************/
INT main( VOID )
{
   QMSG    qmsg;

   Initialize();

   while ( WinGetMsg( hab, (PQMSG) &qmsg, (HWND) NULL, 0, 0) )
      WinDispatchMsg( hab, (PQMSG) &qmsg );

   Finalize();

   return( TRUE);

} /* End of main */


/******************************************************************************
 * Name         : Initialize
 *
 * Description  : This function performs the necessary initializations and
 *                setups that are required to show/run a dialog box as a
 *                main window.  The message queue will be created, as will
 *                the dialog box.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : none
 *
 * Parameters   : None.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID Initialize( VOID)
{
   CHAR     szDefaultSize[MESSAGELEN];   /* buffer for default size menu text */
   CHAR     szCommandString[STRING_COMMAND_SIZE];
   CHAR     szReturn[STRING_COMMAND_SIZE] = "";


   hab       = WinInitialize( 0);
   hmq       = WinCreateMsgQueue( hab, 0);

   hptrArrow = WinQuerySysPointer ( HWND_DESKTOP, SPTR_ARROW, FALSE );
   hptrWait  = WinQuerySysPointer ( HWND_DESKTOP, SPTR_WAIT,  FALSE );

   /*
    * Initialize the dialog window. First, change pointer to a waiting
    * pointer, since this might take a couple of seconds.
    */
   WinSetPointer ( HWND_DESKTOP, hptrWait );

   WinLoadString(
      hab,                                  /* HAB for this dialog box.       */
      (HMODULE) NULL,                       /* Get the string from the .exe.  */
      IDS_DEFAULTSIZE,                      /* Which string to get.           */
      sizeof(szDefaultSize),                /* The size of the buffer.        */
      szDefaultSize);                       /* The buffer to place the string.*/

   WinLoadString(
      hab,                                  /* HAB for this dialog box.       */
      (HMODULE) NULL,                       /* Get the string from the .exe.  */
      IDS_UNTITLED,                         /* Which string to get.           */
      (SHORT) sizeof( acTempFileName),      /* The size of the buffer.        */
      acTempFileName);                      /* The buffer to place the string.*/

   WinLoadString(
      hab,                                  /* HAB for this dialog box.       */
      (HMODULE) NULL,                       /* Get the string from the .exe.  */
      IDS_AUDIO_RECORDER_ERROR,             /* Which string to get.           */
      (SHORT) sizeof( achMsgBoxTitle),      /* The size of the buffer.        */
      achMsgBoxTitle);                      /* The buffer to place the string.*/

   hwndFrame =                    /* Returns the handle to the frame.         */
       WinLoadSecondaryWindow(
          HWND_DESKTOP,           /* Parent of the dialog box.                */
          HWND_DESKTOP,           /* Owner of the dialog box.                 */
          (PFNWP) MainDlgProc,    /* 'Window' procedure for the dialog box.   */
          (HMODULE) NULL,         /* Where is the dialog.  Null is EXE file.  */
          ID_DLG_MAIN,            /* Dialog ID.                               */
          (PVOID) NULL);          /* Creation Parameters for the dialog.      */

   /*
    * Retrieve the handle to the dialog box by specifying the QS_DIALOG flag.
    */
   hwndMainDialogBox = WinQuerySecondaryHWND(hwndFrame, QS_DIALOG);

   /*
    * Add Default Size menu item to system menu of the secondary window.
    */
   WinInsertDefaultSize(hwndFrame, szDefaultSize);

   /*
    * Get the window title string from the Resource string table
    * and set it as the window text for the dialog window.
    */
   WinLoadString(
      hab,                                  /* HAB for this dialog box.       */
      (HMODULE) NULL,                       /* Get the string from the .exe.  */
      IDS_PROGRAM_TITLE,                    /* Which string to get.           */
      (SHORT) sizeof( achTitle),            /* The size of the buffer.        */
      achTitle);                            /* The buffer to place the string.*/
   WinSetWindowText( hwndFrame, achTitle);

   /*
    * Query the device for the default connections.
    */
   QueryTheDefaultConnection();

   /*
    * Open the audio device with a untitled file loaded.
    */
   if (OpenTheFile( hwndMainDialogBox, acTempFileName ))
      fTempFileInUse =  TRUE;
   /*
    * Find the opened device capabilities like can able to play and record
    * the wave audio files. Also find the device has the ability to play
    * and record 16 bit audio files.
    */
   FindTheDeviceCapability();

   WinShowWindow( hwndFrame, TRUE );
   InitializeHelp();

   /*
    * Now that we're done here, change the pointer back to the arrow.
    */
   WinSetPointer ( HWND_DESKTOP, hptrArrow );

} /* End of Initialize */

/******************************************************************************
 * Name         : Finalize
 *
 * Description  : This routine is called after the message dispatch loop
 *                has ended because of a WM_QUIT message.  The code will
 *                destroy the help instance, messages queue, and window.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : None.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID Finalize( VOID )
{
   /*
    * Destroy the Help Instances.
    */
   if ( hwndHelpInstance != (HWND) NULL)
   {
      WinDestroyHelpInstance( hwndHelpInstance );
   }

   WinDestroySecondaryWindow( hwndFrame );
   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );

}  /* End of Finalize */

/******************************************************************************
 * Name         : MainDlgProc
 *
 * Description  : This function controls the main dialog box.  It will handle
 *                received messages such as pushbutton notifications, timing
 *                events, etc.
 *
 * Concepts     : - How to participate in MMPM/2 device sharing scheme.
 *                - How to handle notification messages.
 *
 * MMPM/2 API's : mciSendMessage   MCI_ACQUIREDEVICE
 *
 * Parameters   : hwnd - Handle for the Main dialog box.
 *                msg  - Message received by the dialog box.
 *                mp1  - Parameter 1 for the just received message.
 *                mp2  - Parameter 2 for the just received message.
 *
 * Return       :
 *
 ******************************************************************************/
MRESULT EXPENTRY MainDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   ULONG             ulError;                 /* rc for MCI_ACQUIRE send cmd  */
   HPOINTER          hpProgramIcon;           /* handle to program's icon     */
   MCI_GENERIC_PARMS mciGenericParms;         /* generic parms for MCI_ACQUIRE*/
   USHORT            usCommandMessage;        /* command message for notify   */
   USHORT            usNotifyCode;            /* notification message code    */
   static BOOL       fShowStatus;             /* for status line              */
   BOOL              fError;                  /* Error return code            */


   switch( msg )
   {
      case WM_INITDLG :
         hpProgramIcon =
            WinLoadPointer(
               HWND_DESKTOP,
               (HMODULE) NULL,              /* Resource is kept in .Exe file. */
               ID_ICON );                   /* Which icon to use.             */

         WinDefSecondaryWindowProc(
            hwnd,                    /* Dialog window handle.                 */
            WM_SETICON,              /* Message to the dialog.  Set it's icon.*/
            (MPARAM) hpProgramIcon,
            (MPARAM) 0 );            /* mp2 no value.                         */

         /*
          * get all the required handles.
          */
         hwndPlayPB     = WinWindowFromID( hwnd, ID_GPB_PLAY );   /* play PB  */
         hwndRecordPB   = WinWindowFromID( hwnd, ID_GPB_RECORD ); /* record PB*/
         hwndMenu       = WinWindowFromID(                        /* menu     */
                             WinQuerySecondaryHWND(hwnd, QS_FRAME),
                             FID_MENU );
         /*
          * The slider control cannot be completely set from the dialog
          * template so some aspects must be set here.  We will set the
          * volume range to 0-100, increment to 1-10, and the initial
          * volume level to 75.
          */
         WinSendMsg( WinWindowFromID(hwnd, ID_SL_VOLUME),
                     CSM_SETRANGE,
                     (MPARAM) 0L,
                     (MPARAM) 100L);

         WinSendMsg( WinWindowFromID(hwnd, ID_SL_VOLUME),
                     CSM_SETINCREMENT,
                     (MPARAM) 10L,
                     (MPARAM) 1L);

         WinSendMsg( WinWindowFromID(hwnd, ID_SL_VOLUME),
                     CSM_SETVALUE,
                     (MPARAM) sVolumeLevel,
                     (MPARAM) NULL);

         /*
          * Set up the PLAY graphic pushbutton.
          */
         WinSendMsg (
            hwndPlayPB,                   /* Play push button handle          */
            GBM_SETANIMATIONRATE,         /* Animation rate control           */
            MPFROMLONG(100L),             /* Update play bitmap every .1 sec  */
            NULL);                        /* Ignore return data               */

         /*
          * Initialize all the default selections in the menu pulldowns.
          */
         WinSendMsg( hwndMenu,          /* select the "Microphone" as default */
                     MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_MICROPHONE, TRUE),
                     MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));

         WinSendMsg( hwndMenu,   /* select the "Medium input level" as default*/
                     MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_MEDIUM_INPUT, TRUE),
                     MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));

         WinSendMsg( hwndMenu,      /* select the "Monitor input" as default  */
                     MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_MONITOR_INPUT, TRUE),
                     MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));

         WinSendMsg( hwndMenu,     /* select the "Stereo" as default          */
                     MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_STEREO, TRUE),
                     MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));

         WinSendMsg( hwndMenu,     /* select "Music (22 khz)" as default      */
                     MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_MUSIC, TRUE),
                     MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));

         WinSendMsg( hwndMenu,     /* select the "Low quality" as default     */
                     MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_LOW_QUALITY, TRUE),
                     MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));

         WinSendMsg( hwndMenu,     /* select "Show status line" as default    */
                     MM_SETITEMATTR,
                     MPFROM2SHORT(IDM_OPTIONS_STATUSLINE, TRUE),
                     MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));

         fShowStatus = TRUE;    /* set the "fShowStatus" flag to true         */

         return( (MRESULT) 0);

      case WM_CLOSE :
         /*
          * Before closing the device, inform the user if there is a
          * modified file open.
          */
         if (!fFirstOpen)
         {
            if (fModified )
            {
               if ( SaveTheChanges( hwnd ) == MBID_CANCEL )
                  return( (MRESULT) 0);
               else
                  CloseTheDevice();
            }
            else
               CloseTheDevice();
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case WM_HELP :
         /*
          * The dialog window has received a request for help from the user,
          * i.e., the Help pushbutton was pressed.  Send the HM_DISPLAY_HELP
          * message to the Help Instance with the IPF resource identifier
          * for the correct HM panel.  This will show the help panel for this
          * sample program.
          */
         WinSendMsg( hwndHelpInstance,
                     HM_DISPLAY_HELP,
                     MPFROMSHORT(1),
                     MPFROMSHORT(HM_RESOURCEID));
         return( (MRESULT) 0);

      case WM_CONTROL:
         if (SHORT1FROMMP(mp1)==ID_SL_VOLUME)
         {
            if ((SHORT2FROMMP(mp1)==CSN_CHANGED) ||    /* change volume?   */
                (SHORT2FROMMP(mp1)==CSN_TRACKING))     /* tracking volume? */
            {
               sVolumeLevel = SHORT1FROMMP (mp2);
               SetAudioVolume(hwnd);
            }
         }
         return( (MRESULT) 0);

      case WM_COMMAND :
         /*
          * To get which pushbutton was pressed the SHORT1FROMMP macro is used.
          */
         switch (SHORT1FROMMP(mp1))
         {
            case ID_GPB_PLAY:               /* user selected "Play"           */
               /*
                * If the audio is stopped and an existing file or a tempfile is
                * open,  PLAY.
                */
               if (eState == ST_STOPPED)
               {
                  if (PlayTheAudio( hwnd))
                  {
                     /*
                      * Update the status line with appropriate message.
                      */
                     UpdateTheStatusLine(hwnd, IDS_PLAYING);

                     /*
                      * If playing was successful, start the play button
                      * animation.
                      */
                     WinSendMsg( hwndPlayPB,          /* Dialog window handle */
                                 GBM_ANIMATE,         /* Animation control    */
                                 MPFROMSHORT(TRUE),   /* Animation flag       */
                                 NULL );              /* Ignore return data   */
                  }
                  else      /* If playing failed, set the state to stopped    */
                      eState = ST_STOPPED;
               }
               break;

            case ID_GPB_RECORD:                    /* user selected "Record"  */
               /*
                * If the audio is in stopped state, record
                */
               if (eState == ST_STOPPED)
               {
                  RecordTheAudio( hwnd );
               }
               break;

            case ID_GPB_STOP:              /* user selected "Stop"            */
               /*
                * If the audio is not in stopped state, stop the device
                */
               if (eState != ST_STOPPED)
                  StopTheDevice(hwnd);
               break;

            case ID_GPB_REWIND:            /* user selected "Rewind"          */
               /*
                * If an existing file open, or a temp file is modified
                * seek the file.
                */
               if ((fFileOpen) || (fModified))
               {
                  /*
                   * Update the status line with the appropriate message and
                   * seek the audio to the begining.
                   */
                  UpdateTheStatusLine(hwnd, IDS_REWINDING);
                  SeekTheAudio( hwnd, MCI_TO_START );
               }
               break;

            case ID_GPB_FF:                /* user selected "Fast Forward"    */
               /*
                * If an existing file open, or a temp file is modified
                * seek the file.
                */
               if ((fFileOpen) || (fModified))
               {
                  /*
                   * Update the status line with the appropriate message and
                   * seek the audio to the end.
                   */
                  UpdateTheStatusLine(hwnd, IDS_FASTFORWARDING);
                  SeekTheAudio( hwnd, MCI_TO_END );
               }
               break;

            case IDM_FILE_NEW:             /* user selected "Filenew"         */
               if (eState != ST_STOPPED)
                  StopTheDevice(hwnd);
               /*
                * First, if any modified file is open, ask the user if he wants
                * to save the changes or not.
                */
               if (fModified)
               {
                  if ( SaveTheChanges( hwnd ) == MBID_CANCEL )
                     return( (MRESULT) FALSE);
               }

               /*
                * If opening is success, set the file varables.
                */
               if ( OpenTheFile( hwnd, acTempFileName ))
               {
                  /*
                   * Get the selected file header information and check the
                   * correct options and disable all the other options in the
                   * Type menu pulldown.
                   */
                  SetTheOptions(NEW_FILE);

                  fTempFileInUse =  TRUE;
                  fFileOpen      =  FALSE;
                  UpdateTheStatusLine(hwnd, IDS_STOPPED);
               }
               return( (MRESULT) TRUE);
               break;

            case IDM_FILE_OPEN:                   /* user selected "Fileopen" */
               /*
                * First, if any modified file is open, ask the user if he wants
                * to save the changes or not.
                */
               if (fModified)
               {
                  if ( SaveTheChanges( hwnd ) == MBID_CANCEL )
                     return( (MRESULT) FALSE);
               }

               /*
                * Allow the user to select a file using Open dialog.
                *
                */
               if (ShowStandardFileDlg(IDM_FILE_OPEN))
               {
                  /*
                   * If the audio is not in stop state, stop it.
                   */
                  if (eState != ST_STOPPED)
                     StopTheDevice(hwnd);

                  if (!fFirstOpen)       /* If we've opened the device        */
                  {
                     /*
                      * Get the selected file header information and check the
                      * correct options and disable all the other options in
                      * the Type menu pulldown.
                      */
                     fError = SetTheOptions(EXISTING_FILE);
                     /* If we get an error from SetTheOptions, we want to
                      * exit now, before opening the file.
                      */
                     if (!fError) {
                        return( (MRESULT) FALSE);
                     }
                  }
                  /*
                   * Now that the user has selected a file, open it.
                   */
                  if (OpenTheFile( hwnd, szFileName ))
                  {
                     fFileOpen      =  TRUE;
                     fTempFileInUse =  FALSE;
                     UpdateTheStatusLine(hwnd, IDS_STOPPED);
                  }
               }
               return( (MRESULT) TRUE);
               break;

            case IDM_FILE_SAVE:                   /* user selected "Save"     */

               if ((!fTempFileInUse) && (!fFileOpen) || (fTempFileInUse))
               {
                  if ( FileSaveAs( hwnd ) )
                  {
                     /*
                      * If the audio is not in stop state, stop it.
                      */
                     if (eState != ST_STOPPED)
                        StopTheDevice(hwnd);

                     fFileOpen = TRUE;
                     UpdateTitleText( hwndFrame, szFileName );
                  }
               }
               else
                  SaveTheAudio(hwnd);

               return( (MRESULT) TRUE);
               break;

            case IDM_FILE_SAVE_AS:                /* user selected "Saveas"   */

               if ( FileSaveAs( hwnd ) )
               {
                  /*
                   * If the audio is not in stop state, stop it.
                   */
                  if (eState != ST_STOPPED)
                     StopTheDevice(hwnd);

                  fFileOpen = TRUE;
                  UpdateTitleText( hwndFrame, szFileName );
               }
               return( (MRESULT) TRUE);
               break;

            case IDM_MICROPHONE:             /* user selected "Microphone"    */
               /*
                * First, select the "Microphone" then, deselect the "Line in"
                * and set the usDeviceType.
                */
               SelectTheOption(IDM_MICROPHONE);
               DeselectTheOption(IDM_LINEIN);
               usDeviceType =  MCI_MICROPHONE_CONNECTOR;
               return( 0 );

            case IDM_LINEIN:                /* user selected "Line in"        */
               /*
                * First, select the "Line in" then, deselect the "Microphone"
                * and set the usDeviceType.
                */
               SelectTheOption(IDM_LINEIN);
               DeselectTheOption(IDM_MICROPHONE);
               usDeviceType =  MCI_LINE_IN_CONNECTOR;
               return( 0 );

            case IDM_LOW_INPUT:             /* user selected "Low input level"*/
               /*
                * First, select the "Low input level" then, deselect the
                * "Medium input level" and "High input level" and set the
                * usInputLevel.
                */
               SelectTheOption(IDM_LOW_INPUT);
               DeselectTheOption( IDM_MEDIUM_INPUT );
               DeselectTheOption( IDM_HIGH_INPUT );
               usInputLevel = LOW_INPUT_LEVEL;
               return( 0 );

            case IDM_MEDIUM_INPUT:       /* user selected "Medium input level"*/
               /*
                * First, select the "Medium input level" then, deselect the
                * "Low input level" and "High input level" and set the
                * usInputLevel.
                */
               SelectTheOption(IDM_MEDIUM_INPUT);
               DeselectTheOption( IDM_LOW_INPUT );
               DeselectTheOption( IDM_HIGH_INPUT );
               usInputLevel = MEDIUM_INPUT_LEVEL;
               return( 0 );

            case IDM_HIGH_INPUT:          /* user selected "High input level" */
               /*
                * First, select the "High input level" then, deselect the
                * "Low input level" and "Medium input level" and set the
                * usInputLevel.
                */
               SelectTheOption(IDM_HIGH_INPUT);
               DeselectTheOption( IDM_LOW_INPUT );
               DeselectTheOption( IDM_MEDIUM_INPUT );
               usInputLevel = HIGH_INPUT_LEVEL;
               return( 0 );

            case IDM_MONITOR_INPUT:        /* user selected "Monitor input"   */
               /*
                * Check to see the status of the "Monitor input" option.
                */
               if (fMonitor)
               {
                  /*
                   * Previously this item was checked, so now uncheck the
                   * "Monitor input" item and set fMonitor to false.
                   */
                  WinSendMsg( hwndMenu,
                              MM_SETITEMATTR,
                              MPFROM2SHORT(IDM_MONITOR_INPUT, TRUE),
                              MPFROM2SHORT(MIA_CHECKED, FALSE));
                  fMonitor = FALSE;
               }
               else
               {
                  /*
                   * Previously this item was unchecked, so now check the
                   * "Monitor input" item and set fMonitor to true.
                   */
                  WinSendMsg( hwndMenu,
                              MM_SETITEMATTR,
                              MPFROM2SHORT(IDM_MONITOR_INPUT, TRUE),
                              MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));
                  fMonitor = TRUE;
               }
               return( 0 );

            case IDM_STEREO:                 /* user selected "Stereo"        */
               /*
                * First, select the "Stereo" then, deselect the "Mono" and
                * set the usStereoMono.
                */
               SelectTheOption(IDM_STEREO);
               DeselectTheOption( IDM_MONO );
               usStereoMono =  STEREO;
               return( 0 );

            case IDM_MONO:                   /* user selected "Mono"          */
               /*
                * First, select the "Mono" then, deselect the "Stereo" and
                * and set the usStereoMono
                */
               SelectTheOption(IDM_MONO);
               DeselectTheOption( IDM_STEREO );
               usStereoMono =  MONO;
               return( 0 );

            case IDM_VOICE:                 /* user selected "Voice (11 khz)" */
               /*
                * First, select the "Voice (11 khz)" then, deselect the
                * "Music (22 khz)" and "High fidelity (44 khz)" and set the
                * usQualityType
                */
               SelectTheOption(IDM_VOICE);
               DeselectTheOption( IDM_MUSIC );
               DeselectTheOption( IDM_HIGH_FIDELITY );
               usQualityType =  VOICE;
               return( 0 );

            case IDM_MUSIC:                 /* user selected "Music (22 khz)" */
               /*
                * First, select the "Music (22 khz)" then, deselect the
                * "Voice (11 khz)" and "High fidelity (44 khz)" and set the
                * usQualityType
                */
               SelectTheOption(IDM_MUSIC);
               DeselectTheOption( IDM_VOICE );
               DeselectTheOption( IDM_HIGH_FIDELITY );
               usQualityType = MUSIC;
               return( 0 );

            case IDM_HIGH_FIDELITY:  /* user selected "High fidelity (44 khz)"*/
               /*
                * First, select the "High fidelity (44 khz)" then, deselect
                * the "Voice (11 khz)" and "Music (22 khz)" and set the
                * usQualityType
                */
               SelectTheOption(IDM_HIGH_FIDELITY);
               DeselectTheOption( IDM_VOICE );
               DeselectTheOption( IDM_MUSIC );
               usQualityType =  HIGH_FIDELITY;
               return( 0 );

            case IDM_LOW_QUALITY:              /* user selected "Low quality" */
               /*
                * First, select the "Low quality" then, deselect the
                * "High quality" and set the usBitType.
                */
               SelectTheOption(IDM_LOW_QUALITY);
               DeselectTheOption( IDM_HIGH_QUALITY );
               usBitType =  BIT_8;
               return( 0 );

            case IDM_HIGH_QUALITY:             /* user selected "High quality"*/
               /*
                * First, select the "High quality" then, deselect the
                * "Low quality" and set the usBitType.
                */
               SelectTheOption(IDM_HIGH_QUALITY);
               DeselectTheOption( IDM_LOW_QUALITY );
               usBitType =  BIT_16;
               return( 0 );

            case IDM_OPTIONS_STATUSLINE :
               /*
                * Check to see the status of the "Show status line" option.
                */
               if (fShowStatus)
               {
                  /*
                   * Previously this item was checked, so now uncheck the
                   * "Show status line" item and set fShowStatus to false.
                   */
                  WinSendMsg( hwndMenu,
                              MM_SETITEMATTR,
                              MPFROM2SHORT(IDM_OPTIONS_STATUSLINE, TRUE),
                              MPFROM2SHORT(MIA_CHECKED, FALSE));
                  fShowStatus = FALSE;

                  /*
                   * Adjust the Dialog window.
                   */
                  AdjustTheDlg(hwnd, fShowStatus);
               }
               else
               {
                  /*
                   * Previously this item was unchecked, so now check the
                   * "Show status line" item and set fShowStatus to true.
                   */
                  WinSendMsg( hwndMenu,
                              MM_SETITEMATTR,
                              MPFROM2SHORT(IDM_OPTIONS_STATUSLINE, TRUE),
                              MPFROM2SHORT(MIA_CHECKED, MIA_CHECKED));
                  fShowStatus = TRUE;

                  /*
                   * Adjust the Dialog window.
                   */
                  AdjustTheDlg(hwnd, fShowStatus);
               }
               return( 0 );

            case IDM_HELP_PRODUCTINFO :
               /*
                * See if the product information dialog box was created.
                */
               if (!hwndProductInfo)
               {
                  /*
                   * Product Window was not created, create and display the
                   * window.
                   */
                  hwndProductInfo =
                      WinLoadSecondaryWindow(
                         HWND_DESKTOP,             /* Parent of the dialog box*/
                         hwndFrame,                /* Owner of the dialog box.*/
                         (PFNWP)ProductInfoDlgProc,/* Dialog box procedure.   */
                         (HMODULE) NULL,           /* Dialog is where,EXE file*/
                         ID_DLG_PRODUCTINFO,       /* Dialog ID.              */
                         (PVOID) NULL);            /* Dialog Creation Params  */
              }
              else
              {
                 /*
                  * Product window was created earlier, reposition the window
                  * and give the focus.
                  */
                 WinSetWindowPos(
                    hwndProductInfo,
                    HWND_TOP,
                    (SHORT) NULL,
                    (SHORT) NULL,
                    (SHORT) NULL,
                    (SHORT) NULL,
                    SWP_RESTORE | SWP_SHOW | SWP_ACTIVATE );

                 WinSetFocus( HWND_DESKTOP, hwndProductInfo );
              }
               return( 0 );

            case IDM_HELP_INDEX :            /* user selected "Help Index"    */
               /*
                * Call system default help index panel.
                */
               WinSendMsg( hwndHelpInstance,
                           HM_HELP_INDEX,
                           NULL,
                           NULL);
               break;

            case IDM_HELP_GENERAL :          /* user selected "General Help"  */
               /*
                * Call general help panel.
                */
               WinSendMsg( hwndHelpInstance,
                           HM_DISPLAY_HELP,
                           MPFROMSHORT(1),
                           MPFROMSHORT(HM_RESOURCEID));
               break;

            case IDM_HELP_USING :            /* user selected "Using Help"    */
               /*
                * Call system default using help panel.
                */
               WinSendMsg( hwndHelpInstance,
                           HM_DISPLAY_HELP,
                           NULL,
                           NULL);
               break;

            case IDM_HELP_KEYS :             /* user selected "Keys Help"     */
               /*
                * Call keys help panel.
                */
               WinSendMsg( hwndHelpInstance,
                           HM_DISPLAY_HELP,
                           MPFROMSHORT(2),
                           MPFROMSHORT(HM_RESOURCEID));
               break;

            default:
               break;

         }  /* End of Command Switch */
         return( (MRESULT) 0);

      /*
       * The next two messages are handled so that the audio recorder
       * application can participate in device sharing.  Since it opens
       * the device as shareable device, other applications can gain
       * control of the device.  When this happens, we will receive a
       * MM_MCIPASSDEVICE message.  We keep track of this device passing in
       * the fPassedDevice boolean variable.
       *
       * If we do not have access to the device when we receive an WM_ACTIVATE
       * message, then we will issue an acquire device command to gain
       * access to the device.
       *
       *
       * This is one possible method that can be used to implement
       * device sharing. For applications that are more complex
       * than this sample program, developers may wish to take
       * advantage of a more robust method of device sharing.
       * This can be done by using the MCI_ACQUIRE_QUEUE flag on
       * the MCI_ACQUIREDEVICE command.  Please refer to the MMPM/2
       * documentation for more information on this flag.
       */

      case MM_MCIPASSDEVICE:
         if (SHORT1FROMMP(mp2) == MCI_GAINING_USE)           /* GAINING USE */
         {
            fPassedDevice = FALSE;               /* Gaining control of device.*/
            if (eState == ST_PLAYING)            /* If the file was playing   */
            {
               /*
                * Start play button animation and update the status line
                * with appropriate text.
                */
               WinSendMsg( hwndPlayPB,           /* Play button handle        */
                           GBM_ANIMATE,          /* Animation control         */
                           MPFROMSHORT(TRUE),    /* Animation flag            */
                           NULL );               /* Ignore return data        */
               UpdateTheStatusLine(hwnd, IDS_PLAYING);
            }
            else if (eState == ST_RECORDING)
            {
               /*
                * Start record button animation and update the status line
                * with appropriate text.
                */
               WinSendMsg( hwndRecordPB,         /* Record button handle      */
                           GBM_ANIMATE,          /* Animation control         */
                           MPFROMSHORT(TRUE),    /* Animation flag            */
                           NULL );               /* Ignore return data        */
               UpdateTheStatusLine(hwnd, IDS_RECORDING);
            }
            else
               /*
                * Update the status line with appropriate text.
                */
               UpdateTheStatusLine(hwnd, IDS_STOPPED);
         }
         else                     /* LOSING USE  */
         {
            fPassedDevice = TRUE;                /* Losing  control of device.*/
            if (eState == ST_PLAYING)            /* If the file was playing   */
            {
               /*
                * Stop play button animation and update the status line
                * with appropriate text.
                */
               WinSendMsg( hwndPlayPB,           /* Play button handle        */
                           GBM_ANIMATE,          /* Animation control         */
                           MPFROMSHORT(FALSE),   /* Animation flag            */
                           NULL );               /* Ignore return data        */
               UpdateTheStatusLine(hwnd, IDS_PLAYING);
            }
            else if (eState == ST_RECORDING)
            {
               /*
                * Stop record button animation and update the status line
                * with appropriate text.
                */
               WinSendMsg( hwndRecordPB,         /* Record button handle      */
                           GBM_ANIMATE,          /* Animation control         */
                           MPFROMSHORT(FALSE),   /* Animation flag            */
                           NULL );               /* Ignore return data        */
               UpdateTheStatusLine(hwnd, IDS_RECORDING);
            }
            else
               /*
                * Update the status line with appropriate text.
                */
               UpdateTheStatusLine(hwnd, IDS_STOPPED);
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case WM_ACTIVATE:

         /* We use the WM_ACTIVATE message to participate in device
          * sharing.  We first check to see if this is an activate
          * or a deactivate message (indicated by mp1).  Then,
          * we check to see if we've passed control of the audio'
          * devices.  If these conditions are true, then
          * we issue an acquire device command to regain control of
          * the device, since we're now the active window on the screen.
          */

         if ((BOOL)mp1 && fPassedDevice == TRUE)
         {
            mciGenericParms.hwndCallback = hwnd;

            ulError = mciSendCommand( mciOpenParms.usDeviceID,
                                      MCI_ACQUIREDEVICE,
                                      (ULONG)MCI_NOTIFY,
                                      (PVOID) &mciGenericParms,
                                      (USHORT)NULL);
            if (ulError)
               ShowMCIErrorMessage( ulError);
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case MM_MCINOTIFY:
         /*
          * This message is returned to an application when a device
          * successfully completes a command that was issued with a NOTIFY
          * flag, or when an error occurs with the command.
          *
          * This message returns two values. A user parameter (mp1) and
          * the command message (mp2) that was issued. The low word of mp1
          * is the Notification Message Code which indicates the status of the
          * command like success or failure. The high word of mp2 is the
          * Command Message which indicates the source of the command.
          */

         usNotifyCode = (USHORT) SHORT1FROMMP( mp1);  /* low-word    */
         usCommandMessage = (USHORT) SHORT2FROMMP( mp2); /* high-word */

         switch (usCommandMessage)
         {
            case MCI_PLAY:
               switch (usNotifyCode)
               {
                  case MCI_NOTIFY_SUCCESSFUL:
                     if (eState != ST_STOPPED)
                     {
                        /*
                         * Update the status line with appropriate message.
                         */
                        UpdateTheStatusLine(hwnd, IDS_STOPPED);
                        eState = ST_STOPPED;

                        /*
                         * Stop the play button animation
                         */
                        WinSendMsg( hwndPlayPB,        /* Play button handle  */
                                    GBM_ANIMATE,       /* Animation control   */
                                    MPFROMSHORT(FALSE),/* Animation flag      */
                                    NULL );            /* Ignore return data  */
                     }
                     break;

                  case MCI_NOTIFY_SUPERSEDED:
                  case MCI_NOTIFY_ABORTED:
                     /* we don't need to handle these messages. */
                     break;

                  default:
                     /*
                      * If the message is none of the above, then it must be
                      * a notification error message.
                      */
                     ShowMCIErrorMessage( usNotifyCode);
                     eState = ST_STOPPED;

                     /*
                      * Stop the play button animation and update the status
                      * line with appropriate text.
                      */
                     WinSendMsg( hwndPlayPB,           /* Play button handle  */
                                 GBM_ANIMATE,          /* Animation control   */
                                 MPFROMSHORT(FALSE),   /* Animation flag      */
                                 NULL );               /* Ignore return data  */
                     UpdateTheStatusLine(hwnd, IDS_STOPPED);
                     break;
               }
               break;
         }
         return( (MRESULT) 0);

      default:
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2));

   }  /* End of msg Switch */

   return( (MRESULT) FALSE);

} /* End of MainDlgProc */

/******************************************************************************
 * Name         : InitializeHelp
 *
 * Description  : This procedure will set up the initial values in the
 *                global help structure.  This help structure will then
 *                be passed on to the Help Manager when the Help Instance
 *                is created.  The handle to the Help Instance will be
 *                kept for later use.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : None.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID InitializeHelp( VOID )
{

   HELPINIT helpInit;                 /* Help initialization structure.          */
   CHAR     achHelpLibraryName[LEN_HELP_LIBRARY_NAME];
   CHAR     achHelpWindowTitle[LEN_HELP_WINDOW_TITLE];

   /*
    * Load the strings for the Help window title and library name.
    * Initialize the help structure and associate the help instance.
    */
   WinLoadString( hab,
                  (HMODULE) NULL,
                  IDS_HELP_WINDOW_TITLE,
                  (SHORT) sizeof( achHelpWindowTitle),
                  achHelpWindowTitle);

   WinLoadString( hab,
                  (HMODULE) NULL,
                  IDS_HELP_LIBRARY_NAME,
                  (SHORT) sizeof( achHelpLibraryName),
                  achHelpLibraryName);

   memset ( &helpInit, 0, sizeof(helpInit) );
   /*
    * Initialize the help structure.
    */
   helpInit.cb                 = sizeof( helpInit);  /* size of the help struc*/
   helpInit.ulReturnCode       = (ULONG) 0;          /* RC from HM init       */
   helpInit.pszTutorialName    = (PSZ) NULL;         /* No tutorial program   */
   helpInit.pszHelpWindowTitle = achHelpWindowTitle; /* The Help window title.*/
   helpInit.fShowPanelId       = (ULONG) 0;          /* help panel ID.        */
   helpInit.pszHelpLibraryName = achHelpLibraryName; /* library name          */
   helpInit.phtHelpTable       = (PVOID)(0xffff0000 | ID_AUDIO_HELPTABLE);

   /*
    * Create the Help Instance for IPF.
    * Give IPF the Anchor Block handle and address of the IPF initialization
    * structure, and check that creation of Help was a success.
    */
   hwndHelpInstance = WinCreateHelpInstance(
                         hab,                   /* Anchor Block Handle.       */
                         &helpInit );           /* Help Structure.            */

   if ( hwndHelpInstance == (HWND) NULL)
   {
      MessageBox( IDS_HELP_CREATION_FAILED,     /* ID of the message          */
                  MB_OK | MB_INFORMATION  | MB_MOVEABLE);    /* style         */
   }
   else
   {
      if ( helpInit.ulReturnCode)
      {
         WinDestroyHelpInstance( hwndHelpInstance);
         MessageBox( IDS_HELP_CREATION_FAILED,     /* ID of the message       */
                     MB_OK | MB_INFORMATION | MB_MOVEABLE);     /* style      */
      }
      else  /* help creation worked */
         WinAssociateHelpInstance(
            hwndHelpInstance,        /* The handle of the Help Instance.      */
            hwndFrame);              /* Associate to this dialog window.      */
   }  /* End of IF checking the creation of the Help Instance. */

}  /* End of InitializeHelp */

/******************************************************************************
 * Name        : ProcuctInfoDlgProc
 *
 * Description : This function controls the product information dialog box.
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
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   switch( msg )
   {
     case WM_INITDLG :
         /*
          * Add Default Size menu item to system menu of the secondary window.
          */
         WinInsertDefaultSize(hwnd, "~DefaultSize");
         break;

     case WM_CLOSE :
         WinDestroySecondaryWindow( hwnd ); /* Close the Dialog box.          */
         return( 0 );
         break;

    case WM_DESTROY:
         hwndProductInfo = 0;
         break;

      case WM_COMMAND :
         switch( SHORT1FROMMP( mp1 ) )
         {
            case DID_OK :

            case DID_CANCEL:
               WinDestroySecondaryWindow( hwnd ); /* Close the Dialog box.          */
               return( (MRESULT)TRUE);
            break;
         }  /* End of Command Switch */

   }  /* End of Switch */

   return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

} /* End of FileDlgProc */

/******************************************************************************
 * Name         :  OpenTheFile
 *
 * Description  :  This procedure will Open the audio device with a specific
 *                 file loaded, if the audio device is not already opened.
 *                 If the audio device is open, an audio file will be loaded
 *                 onto it.
 *
 * Concepts     :  - Open a device using the MCI interface.
 *                 - Loading a file into an already open device.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_OPEN
 *                                   MCI_LOAD
 *
 * Parameters   :  hwnd        - Handle for the Main dialog box.
 *                 pszFileName - selected file name to open
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
BOOL OpenTheFile( HWND hwnd, PSZ pszFileName )
{
   ULONG          ulError;             /* return value for mci command        */
   MCI_LOAD_PARMS mciLoadParms;        /* load parms for MCI_LOAD             */
   BOOL           bReturn = TRUE;      /* function return value               */


   if (fFirstOpen)
   /* If this is the first time thru this routine, then we need to open
    * the device.
    *
    * On subsequent calls to this routine, the devices are already open,
    * so we only need to load the appropriate file onto the device.
    *
    */
   {
      /*
       * Update the status line with appropriate message.
       */
      UpdateTheStatusLine(hwnd, IDS_OPENINGDEVICE);
      /*
       * To Open the device, we will issue an MCI_OPEN command via
       * mciSendCommand using an MCI_OPEN_PARMS structure.  The device ID
       * will be returned in the device ID field of the open data structure.
       */

      memset( &mciOpenParms,            /* Object to fill with zeros.       */
              0,                        /* Value to place into the object.  */
              sizeof( mciOpenParms ) ); /* How many zero's to use.          */

      mciOpenParms.hwndCallback = hwnd; /* For MM_MCIPASSDEVICE */
      mciOpenParms.usDeviceID   = (USHORT)  NULL; /* this is returned   */

      /*
       * Specify the device type. The pszDeviceType is to be interpreted as
       * the low-order word is a standard device type, and the high-order
       * word is the ordinal index for the device. In other words we are
       * explecitely trying to open the default device on the system. This
       * gives the ability to load a non existing (untitled) audio file.
       */
      mciOpenParms.pszDeviceType    = (PSZ) MAKEULONG(
                                              MCI_DEVTYPE_WAVEFORM_AUDIO,
                                              usDeviceTypeOpened);

      /*
       * Specify the name of the file to be opened.
       */
      mciOpenParms.pszElementName   = (PSZ)NULL;

      ulError = mciSendCommand( (USHORT) 0,
                                MCI_OPEN,
                                MCI_WAIT | MCI_OPEN_TYPE_ID |
                                MCI_OPEN_SHAREABLE | MCI_OPEN_ELEMENT,
                                (PVOID) &mciOpenParms,
                                UP_OPEN);
      if (ulError)
      {
         ShowMCIErrorMessage( ulError);
         UpdateTheStatusLine(hwnd, IDS_STOPPED);
         bReturn = FALSE;
      }
      else
         /*
          * Open success, set the flag to indicate that next time thru this
          * routine we do not need to open the device.
          */
         fFirstOpen = FALSE;
   }
   else
   {
       /*
        * Change pointer to a waiting pointer first, since this might take a
        * couple of seconds.
        */
       WinSetPointer( HWND_DESKTOP, hptrWait );

       /*
        * The device is already open. We just need to load the appropriate
        * audio file.
        *
        * Update the status line with appropriate message.
        */
       UpdateTheStatusLine(hwnd, IDS_LOADINGFILE);

       mciLoadParms.hwndCallback     = (HWND) NULL;    /* We're waiting */

       /*
        * If we're not using a temporary file, load the filename.
        * Otherwise, just pass NULL as the filename and let the audio
        * device handle the temporary file for us.
        */
       if ( strcmp ( pszFileName, acTempFileName) )
          mciLoadParms.pszElementName   = (PSZ)pszFileName;
       else
          mciLoadParms.pszElementName   = (PSZ)NULL;

       ulError = mciSendCommand( mciOpenParms.usDeviceID,
                                 MCI_LOAD,
                                 MCI_WAIT,
                                 (PVOID) &mciLoadParms,
                                 (ULONG) NULL);
       if (ulError)
       {
          ShowMCIErrorMessage( ulError );  /* Display the error message      */
          UpdateTheStatusLine(hwnd, IDS_STOPPED);
          bReturn = FALSE;
       }

       /*
        * Now that we're done here, change the pointer back to the arrow.
        */
       WinSetPointer ( HWND_DESKTOP, hptrArrow );
   }

   if (bReturn)
      /*
       * Update the main window title bar with the selected file name.
       */
      UpdateTitleText(hwndFrame, pszFileName);

   return( bReturn);

}  /* end of OpenTheFile */

/******************************************************************************
 * Name         :  QueryTheDefaultConnection
 *
 * Description  :  This procedure will query the default connection and find
 *                 the device play/record capabilities. Also find the device
 *                 ability to play/record 16 bit audio files.
 *
 * Concepts     :  - Query the default device of waveaudio type.
 *                 - Get the ordinal value of the first device of the waveaudio.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_SYSINFO
 *
 * Parameters   :  none
 *
 * Return       :  none
 *
 ******************************************************************************/
VOID QueryTheDefaultConnection( VOID )
{
   MCI_SYSINFO_PARMS         mciSysinfoParms;
   MCI_SYSINFO_DEFAULTDEVICE mciSysinfoDefaultdevice;
   MCI_SYSINFO_QUERY_NAME    mciSysinfoQueryName;
   ULONG                     ulError;


   memset( &mciSysinfoParms, 0, sizeof( mciSysinfoParms ) );
   memset( &mciSysinfoDefaultdevice, 0, sizeof(mciSysinfoDefaultdevice));

   /*
    * Set up parameters to query the default device for
    * MCI_DEVTYPE_WAVEFORM_AUDIO device type. If no explicit default exists,
    * then the first device of the indicated type is implicitely the default.
    *
    * First, initialize MCI_SYSINFO_PARMS and MCI_SYSINFO_DEFAULTDEVICE
    * structures with the pertinent information then issue MCI_SYSINFO
    * command via mciSendCommand. The pSysInfoParm field points to the
    * MCI_SYSINFO_DEFAULTDEVICE structure. The default device name will be
    * returned in the szInstallName field.
    */
   mciSysinfoParms.ulItem  = MCI_SYSINFO_QUERY_DEFAULT;
   mciSysinfoParms.pSysInfoParm  = &mciSysinfoDefaultdevice;
   mciSysinfoDefaultdevice.usDeviceType  = MCI_DEVTYPE_WAVEFORM_AUDIO;

   ulError = mciSendCommand( (USHORT) 0,
                             MCI_SYSINFO,
                             MCI_WAIT | MCI_SYSINFO_ITEM,
                             (PVOID) &mciSysinfoParms,
                             0);

   if (ulError == 0L)
   {
      memset( &mciSysinfoParms , 0, sizeof(MCI_SYSINFO_PARMS));
      memset( &mciSysinfoQueryName , 0, sizeof(MCI_SYSINFO_QUERY_NAME));

      /*
       * Set up parameters to query the device ordinal value. This message
       * query the given device name which is in the szInstallName field of the
       * MCI_SYSINFO_DEFAULTDEVICE structure and returns the ordinal value of
       * the first device of that type. The pSysInfoParm field points to the
       * MCI_SYSINFO_QUERY_NAME structure which has the default device name.
       */
      mciSysinfoParms.ulItem  = MCI_SYSINFO_QUERY_NAMES;
      mciSysinfoParms.pSysInfoParm  = &mciSysinfoQueryName;
      memmove( &mciSysinfoQueryName.szInstallName,
               &mciSysinfoDefaultdevice.szInstallName,
               sizeof(mciSysinfoQueryName.szInstallName ) );

      ulError = mciSendCommand( (USHORT) 0,
                                MCI_SYSINFO,
                                MCI_WAIT | MCI_SYSINFO_ITEM,
                                (PVOID) &mciSysinfoParms,
                                0);
      if (ulError == 0L)
         /*
          * Get the device type ordinal value for further use.
          */
         usDeviceTypeOpened = (USHORT)mciSysinfoQueryName.usDeviceOrd;
   }

   if (ulError)
      ShowMCIErrorMessage(ulError);

   return;
}
/******************************************************************************
 * Name         :  FindTheDeviceCapability
 *
 * Description  :  This procedure will determine whether or not the audio
 *                 device supports all of the items on the Type menu.
 *                 If any of these options are not supported, they will be
 *                 disabled.
 *
 * Concepts     :  - Find the capabilities of an audio device.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_GETDEVCAPS
 *
 * Parameters   :  none
 *
 * Return       :  none
 *
 ******************************************************************************/
VOID  FindTheDeviceCapability( VOID )
{
    ULONG                     ulRC;            /* return code from api        */
    MCI_WAVE_GETDEVCAPS_PARMS mciAudioCaps;    /* Dev Caps data structure     */

    memset( &mciAudioCaps , 0, sizeof(MCI_WAVE_GETDEVCAPS_PARMS));

    /* Set up the fields of the mciAudioCaps structure to see if the
     * device supports the different options on the Type menu of this
     * sample program. These items include:
     * Mono or Stereo, 11 kHz or 22 kHz or 44 kHz, and 8 bit or 16 bit.
     * 12 different combinations are possible, so we will
     * issue the query 12 times.  If any of the options are not supported
     * in ONE combination, we will disable that option on the Type menu.
     * Play and Record will also be tested with this api, so we can disable
     * the Play and Record pushbuttons if the device does not support the
     * function.
     */

    /* Test to see if the device can play 11 kHz, 8 bit, mono files.     */
    mciAudioCaps.ulBitsPerSample = 8;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 11025;
    mciAudioCaps.ulChannels      = 1;
    mciAudioCaps.ulFormatMode    = MCI_PLAY;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (8, 11025, 1, ulRC);
    }

    /* Test to see if the device can record 11 kHz, 16 bit, mono files.  */
    mciAudioCaps.ulBitsPerSample = 16;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 11025;
    mciAudioCaps.ulChannels      = 1;
    mciAudioCaps.ulFormatMode    = MCI_RECORD;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (16, 11025, 1, ulRC);
    }

    /* Test to see if the device can play 22 kHz, 8 bit, mono files.     */
    mciAudioCaps.ulBitsPerSample = 8;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 22050;
    mciAudioCaps.ulChannels      = 1;
    mciAudioCaps.ulFormatMode    = MCI_PLAY;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (8, 22050, 1, ulRC);
    }

    /* Test to see if the device can record 22 kHz, 16 bit, mono files.  */
    mciAudioCaps.ulBitsPerSample = 16;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 22050;
    mciAudioCaps.ulChannels      = 1;
    mciAudioCaps.ulFormatMode    = MCI_RECORD;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (16, 22050, 1, ulRC);
    }

    /* Test to see if the device can play 44 kHz, 8 bit, mono files.     */
    mciAudioCaps.ulBitsPerSample = 8;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 44100;
    mciAudioCaps.ulChannels      = 1;
    mciAudioCaps.ulFormatMode    = MCI_PLAY;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (8, 44100, 1, ulRC);
    }

    /* Test to see if the device can record 44 kHz, 16 bit, mono files. */
    mciAudioCaps.ulBitsPerSample = 16;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 44100;
    mciAudioCaps.ulChannels      = 1;
    mciAudioCaps.ulFormatMode    = MCI_RECORD;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (16, 44100, 1, ulRC);
    }

    /* Test to see if the device can record 11 kHz, 8 bit, stereo files. */
    mciAudioCaps.ulBitsPerSample = 8;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 11025;
    mciAudioCaps.ulChannels      = 2;
    mciAudioCaps.ulFormatMode    = MCI_RECORD;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (8, 11025, 2, ulRC);
    }

    /* Test to see if the device can support playing 11 kHz, 16 bit, stereo files. */
    mciAudioCaps.ulBitsPerSample = 16;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 11025;
    mciAudioCaps.ulChannels      = 2;
    mciAudioCaps.ulFormatMode    = MCI_PLAY;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (16, 11025, 2, ulRC);
    }


    /* Test to see if the device can support recording 22 kHz, 8 bit, stereo files. */
    mciAudioCaps.ulBitsPerSample = 8;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 22050;
    mciAudioCaps.ulChannels      = 2;
    mciAudioCaps.ulFormatMode    = MCI_RECORD;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (8, 22050, 2, ulRC);
    }

    /* Test to see if the device can play 22 kHz, 16 bit, stereo files.  */
    mciAudioCaps.ulBitsPerSample = 16;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 22050;
    mciAudioCaps.ulChannels      = 2;
    mciAudioCaps.ulFormatMode    = MCI_PLAY;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (16, 22050, 2, ulRC);
    }

    /* Test to see if the device can record 44 kHz, 8 bit, stereo files. */
    mciAudioCaps.ulBitsPerSample = 8;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 44100;
    mciAudioCaps.ulChannels      = 2;
    mciAudioCaps.ulFormatMode    = MCI_RECORD;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (8, 44100, 2, ulRC);
    }

    /* Test to see if the device  can play 44 kHz, 16 bit, stereo files.  */
    mciAudioCaps.ulBitsPerSample = 16;
    mciAudioCaps.ulFormatTag     = DATATYPE_WAVEFORM;
    mciAudioCaps.ulSamplesPerSec = 44100;
    mciAudioCaps.ulChannels      = 2;
    mciAudioCaps.ulFormatMode    = MCI_PLAY;
    mciAudioCaps.ulItem          = MCI_GETDEVCAPS_WAVE_FORMAT;

    ulRC = mciSendCommand (mciOpenParms.usDeviceID,       /* Device ID        */
                           MCI_GETDEVCAPS,
                           MCI_WAIT | MCI_GETDEVCAPS_EXTENDED
                                | MCI_GETDEVCAPS_ITEM,
                           (PVOID) &mciAudioCaps,
                           0);
    ulRC &= 0x0000FFFF;
    if (ulRC != MCIERR_SUCCESS)
    {
       EvaluateReturnCode (16, 44100, 2, ulRC);
    }

   /* We Shall Let 8 bit records and playback through */




    return;
}

/******************************************************************************
 * Name         :  EvaluateReturnCode
 *
 * Description  :  This procedure will evaluate the return code from the
 *                 mciSendCommand - MCI_GETDEVCAPS.  It will disable the
 *                 appropriate items on the Recorder's main window and on
 *                 the Recorder's Type menu.
 *
 * Concepts     :  None
 *
 * MMPM/2 API's :  None
 *
 * Parameters   :  ULONG ulBitsPerSample        Either 8 or 16
 *                 ULONG ulSampPerSec           Either 11025, 22050, or 44100
 *                 ULONG ulChannels             Either 1 or 2
 *                 ULONG ulRC                   Return code to evaluate
 *
 * Return       :  VOID
 *
 ******************************************************************************/
VOID EvaluateReturnCode( ULONG ulBitsPerSample,
                         ULONG ulSampPerSec,
                         ULONG ulChannels,
                         ULONG ulRC)
{


    /* The return code indicates unsupported samples per second value. */
    if (ulRC == MCIERR_UNSUPP_SAMPLESPERSEC)
    {
       /* Samples per second value was 11.025 kHz */
       if (ulSampPerSec == 11025)
       {
          /* Disable the "Voice" option on the Type menu */
          DisableTheOption(
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_VOICE );
          fVoiceSupported = FALSE;
       }
       else
       /* Samples per second value was 22.050 kHz */
       if (ulSampPerSec == 22050)
       {
          /* Disable the "Music" option on the Type menu */
          DisableTheOption(
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_MUSIC );
          fMusicSupported = FALSE;
       }
       else
       /* Samples per second value was 44.100 kHz */
       {
          /* Disable the "High Fidelity" option on the Type menu */
          DisableTheOption(
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_HIGH_FIDELITY );
          fHiFiSupported = FALSE;
       }
    }

    /* The return code indicates unsupported bits per sample value. */
    if (ulRC == MCIERR_UNSUPP_BITSPERSAMPLE)
    {
       /* Samples per second value was 8 */
       if (ulBitsPerSample == 8)
       {
          /* Disable the "Low quality" item on the Options menu. */
          DisableTheOption(
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_LOW_QUALITY );
          fLoQualitySupported = FALSE;
       }
       else
       /* Samples per second value was 16 */
       {
          /* Disable the "High quality" item on the Options menu. */
          DisableTheOption(
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_HIGH_QUALITY );
          fHighQualitySupported = FALSE;
       }
    }

    /* The return code indicates unsupported channels value. */
    if (ulRC == MCIERR_UNSUPP_CHANNELS)
    {
       /* Stereo is not supported */
       if (ulChannels == 2)
       {
          DeselectTheOption( IDM_STEREO );     /* Deselect the "Stereo" option*/
          DisableTheOption(
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_STEREO );
          SelectTheOption( IDM_MONO );         /* Select the "Mono" option    */
          usStereoMono        =  MONO;         /* Set the usStereoMono to Mono*/
          fStereoSupported = FALSE;
       }
       else /* Mono is not supported */
       {
          DeselectTheOption( IDM_MONO );     /* Deselect the "Stereo" option*/
          DisableTheOption(
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_MONO );
          SelectTheOption( IDM_STEREO );         /* Select the "Mono" option    */
          usStereoMono        =  STEREO;         /* Set the usStereoMono to Mono*/
          fMonoSupported = FALSE;
       }
    }

    /* The return code indicates unsupported mode value. */
    if (ulRC == MCIERR_UNSUPP_FORMAT_MODE)
    {
       /*
        * If the device doesn't support either play or record,
        * then do the following disables.
        */


       if (!fMonoSupported &&
           !fStereoSupported &&
           !fVoiceSupported &&
           !fMusicSupported &&
           !fHiFiSupported &&
           !fHighQualitySupported &&
           !fLoQualitySupported)


       {
          WinEnableWindow (hwndPlayPB, FALSE);  /* Disable the Play pushbutton.  */
          WinEnableWindow (hwndRecordPB, FALSE);/* Disable the Record pushbutton.*/

          DisableTheOption(                     /* Disable the "New"  option     */
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_FILE_NEW );

          DisableTheOption(                     /* Disable the "Save"  option    */
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_FILE_SAVE );

          DisableTheOption(                     /* Disable the "Save as" option  */
             WinWindowFromID(
                WinQuerySecondaryHWND(hwndMainDialogBox, QS_FRAME), FID_MENU ),
                IDM_FILE_SAVE_AS );

       }
    }
    return;
}

/******************************************************************************
 * Name         :  PlayTheAudio
 *
 * Description  :  This procedure will begin the playing of an audio file.
 *                 It is invoked when the user selects the Play pushbutton
 *                 on the Recorders main window.
 *
 * Concepts     :  Playing a audio file using the MCI interface.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_PLAY
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
BOOL PlayTheAudio( HWND hwnd)
{
   ULONG             ulError;                /* error value from mci calls    */
   MCI_PLAY_PARMS    mciPlayParms;           /* play parms for MCI_PLAY       */

   /*
    * In this sample program we are always going to play the audio from the
    * begining of the file.
    */
   if (!SeekTheAudio( hwnd, MCI_TO_START ))
      return( FALSE );

   eState = ST_PLAYING;                      /* set state to PLAYING          */
   SetAudioVolume(hwnd);                     /* set the starting volume       */

   /*
    * Initialize MCI_PLAY_PARMS structure with the pertinent information then
    * issue an MCI_PLAY command via mciSendCommand. A MM_MCINOTIFY message
    * will be sent to the window specified in hwndCallback field when the
    * operation is completed. Since we want the whole audio file to be played,
    * we won't specify flags MCI_FROM or MCI_TO.
    */
   mciPlayParms.hwndCallback = hwnd;         /* notify our window             */

   ulError = mciSendCommand( mciOpenParms.usDeviceID,
                             MCI_PLAY,
                             MCI_NOTIFY,
                             (PVOID) &mciPlayParms,
                             UP_PLAY);
   if (ulError)
   {
      /*
       * If we couldn't play - we need to display the error to the user, halt
       * audio gracefully and get to stable, known state.
       */
      ShowMCIErrorMessage( ulError);
      eState = ST_STOPPED;
      StopTheDevice(hwnd);
      return ( FALSE );
   }
   return( TRUE );
}  /* end of PlayTheAudio */


/******************************************************************************
 * Name         : RecordTheAudio
 *
 * Description  : This procedure records an audio file.  In order to do this
 *                it first configures the device settings (such as input level
 *                and input source) and file attributes (such as sample size
 *                and sample rate). Finally, it records the audio data. It is
 *                invoked when the user selects the record pushbutton from the
 *                Recorder's main window.
 *
 * Concepts     : - Record an audio file using MCI interface.
 *                - Setup device and file for recording.
 *
 * MMPM/2 API's : mciSendCommand  MCI_SET
 *                                MCI_CONNECTION
 *                                MCI_CONNECTOR
 *                                MCI_RECORD
 *
 *
 * Parameters   : hwnd - Handle for the Main dialog box.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 ******************************************************************************/
BOOL RecordTheAudio( HWND hwnd )
{
   MCI_RECORD_PARMS     mciRecordParms;      /* for MCI_RECORD                */
   MCI_CONNECTOR_PARMS  mciConnectorParms;   /* for MCI_CONNECTOR             */
   MCI_WAVE_SET_PARMS   mciWaveSetParms;     /* for MCI_SET, waveform devices */
   MCI_GENERIC_PARMS    mciGenericParms;     /* for MCI_ACQUIRE               */
   MCI_AMP_SET_PARMS    mciAmpSetParms;      /* for MCI_SET, amp/mix devices  */
   MCI_CONNECTION_PARMS mciConnectionParms;  /* for MCI_CONNECTION            */
   USHORT               usAmpMixerDeviceID;  /* for amp/mix device ID         */
   LONG                 lmmioAPIRc;          /* MMIO API return codes         */
   ULONG                ulError;             /* for error return code         */


   memset( &mciWaveSetParms,            /* Object to fill with zeros.      */
           0,                           /* Value to place into the object. */
           sizeof( mciWaveSetParms ) ); /* How many zero's to use.         */

   /*
    * Set up parameters for the audio data file (such as stereo/mono, sample
    * rate and sample size).
    *
    * First, initialize MCI_WAVE_SET_PARMS structure with the pertinent
    * information then issue an MCI_SET command via mciSendCommand.
    */
   mciWaveSetParms.usChannels = (USHORT) usStereoMono;  /* specify MONO/STEREO*/
   mciWaveSetParms.ulSamplesPerSec =
                             (ULONG) usQualityType;  /* used for waveform  */
   mciWaveSetParms.usBitsPerSample = (USHORT) usBitType;/* bits per sample */

   ulError =
     mciSendCommand( mciOpenParms.usDeviceID,
                     MCI_SET,
                     MCI_WAIT | MCI_WAVE_SET_CHANNELS |
                     MCI_WAVE_SET_SAMPLESPERSEC |
                     MCI_WAVE_SET_BITSPERSAMPLE,
                     (PVOID) &mciWaveSetParms,
                     UP_SET );
   if (ulError)
   {
      ShowMCIErrorMessage( ulError);
      return( FALSE);
   }

   /*
    * Initialize MCI_CONNECTOR_PARMS structure with zero's
    */
   memset( &mciConnectorParms,            /* Object to fill with zeros.       */
           0,                             /* Value to place into the object.  */
           sizeof( mciConnectorParms ) ); /* How many zero's to use.          */

   /*
    * Set up input source - microphone or line in.
    * initialize MCI_CONNECTOR_PARMS structure with the pertinent
    * information then issue an MCI_CONNECTOR command via mciSendCommand.
    */
   mciConnectorParms.ulConnectorType = usDeviceType; /* microphone/linein  */

   ulError = mciSendCommand( mciOpenParms.usDeviceID,
                             MCI_CONNECTOR,
                             MCI_WAIT | MCI_CONNECTOR_TYPE |
                             MCI_ENABLE_CONNECTOR,
                             (PVOID) &mciConnectorParms,
                             0 );
   if (ulError)
   {
      ShowMCIErrorMessage( ulError);
      return( FALSE);
   }

   /*
    * Initialize MCI_CONNECTION_PARMS structure with zero's
    */
   memset( &mciConnectionParms,           /* Object to fill with zeros.       */
           0,                             /* Value to place into the object.  */
           sizeof( mciConnectionParms ) );/* How many zero's to use.          */

   /*
    * Get the Amp/Mixer device ID.
    *
    * First, initialize MCI_CONNECTION_PARMS structure ulConnectorType with
    * the MCI_WAVE_STREAM_CONNECTOR. The digital input or output for the
    * waveform audio device is streamed to an Amp/Mixer device.
    */
   mciConnectionParms.ulConnectorType  =   MCI_WAVE_STREAM_CONNECTOR;

   /*
    * Next, issue an MCI_CONNECTION command via mciSendCommand. This will
    * return the Amp/Mixer device ID.
    */
   ulError = mciSendCommand( mciOpenParms.usDeviceID,
                             MCI_CONNECTION,
                             MCI_WAIT | MCI_QUERY_CONNECTION |
                             MCI_CONNECTOR_TYPE,
                             (PVOID) &mciConnectionParms,
                             UP_CONNECTION );
   if (ulError)
   {
      ShowMCIErrorMessage( ulError);
      return( FALSE);
   }

   /*
    * Amp/Mixer device ID is needed to control the "Monitor input" and the
    * "Input level" off of the Options menu.
    */
   usAmpMixerDeviceID = mciConnectionParms.usToDeviceID;

   /*
    * Initialize MCI_AMP_SET_PARMS structure with zero's
    */
   memset( &mciAmpSetParms,            /* Object to fill with zeros.       */
           0,                          /* Value to place into the object.  */
           sizeof( mciAmpSetParms ) ); /* How many zero's to use.          */

   /*
    * Set the Monitor on if the user requested it.
    */
   if ( fMonitor )
   {
      /*
       * User selected the "Monitor input" item from the Options menu - turn
       * the monitor ON.
       * First, initialize MCI_AMP_SET_PARMS structure ulItem with
       * MCI_AMP_SET_MONITOR.  Using this in conjunction with the MCI_SET_ON
       * flag will instruct the ampmix device to monitor the currently
       * selected connector.  This item is used to listen to (monitor) a
       * source while it is being recorded.
       */
      mciAmpSetParms.ulItem     = MCI_AMP_SET_MONITOR;

      /*
       * Next, issue an MCI_SET command via mciSendCommand. This message will
       * turn the monitor ON.
       */
      ulError = mciSendCommand( usAmpMixerDeviceID,
                                MCI_SET,
                                MCI_WAIT | MCI_SET_ITEM | MCI_SET_ON,
                                (PVOID) &mciAmpSetParms,
                                UP_SETMONITOR );
   }
   else
   {
      /*
       * User deselected the "Monitor input" item from the Options menu - turn
       * the monitor OFF.
       * First, initialize MCI_AMP_SET_PARMS structure ulItem with
       * MCI_AMP_SET_MONITOR.  Using this in conjunction with the MCI_SET_OFF
       * flag will instruct the ampmix device to NOT to monitor the currently
       * selected connector.
       */
      mciAmpSetParms.ulItem     = MCI_AMP_SET_MONITOR;

      /*
       * Next, issue an MCI_SET command via mciSendCommand.  This message will
       * turn the monitor OFF.
       */
      ulError = mciSendCommand( usAmpMixerDeviceID,
                                MCI_SET,
                                MCI_WAIT | MCI_SET_ITEM | MCI_SET_OFF,
                                (PVOID) &mciAmpSetParms,
                                UP_SETMONITOR );
   }

   if (ulError)
   {
      ShowMCIErrorMessage( ulError);
      return( FALSE);
   }
   /*
    * Set the appropriate input level.
    * Initialize MCI_AMP_SET_PARMS structure with the pertinent
    * information then issue an MCI_SET command via mciSendCommand.
    */
   mciAmpSetParms.ulAudio    = MCI_SET_AUDIO_ALL;    /* all the channels. */
   mciAmpSetParms.ulLevel    = (ULONG) usInputLevel; /* set audio level.  */

   ulError = mciSendCommand( usAmpMixerDeviceID,
                             MCI_SET,
                             MCI_WAIT | MCI_SET_AUDIO | MCI_AMP_SET_GAIN,
                             (PVOID) &mciAmpSetParms,
                             UP_SETAMP );
   if (ulError)
   {
      ShowMCIErrorMessage( ulError);
      return( FALSE);
   }

   /*
    * Now all the setup is done - Ready to Record.
    *
    * First, initialize MCI_RECORD_PARMS structure with zero's
    */
   memset( &mciRecordParms,            /* Object to fill with zeros.       */
           0,                          /* Value to place into the object.  */
           sizeof( mciRecordParms ) ); /* How many zero's to use.          */

   /*
    * Next, initialize MCI_RECORD_PARMS structure with the pertinent
    * information then issue an MCI_RECORD command via mciSendCommand.
    */
   mciRecordParms.hwndCallback = hwnd; /* notify main window     */

   ulError = mciSendCommand( mciOpenParms.usDeviceID,
                             MCI_RECORD,
                             MCI_NOTIFY | MCI_RECORD_OVERWRITE,  /* flags */
                             (PVOID) &mciRecordParms,
                             UP_RECORD );
   if (ulError)
   {
      ShowMCIErrorMessage( ulError );
      StopTheDevice(hwnd);
      return(FALSE);
   }
   else
   {
     fModified = TRUE;
     eState    = ST_RECORDING;
     UpdateTheStatusLine(hwnd, IDS_RECORDING);
     /*
      * Start record button animation
      */
     WinSendMsg ( hwndRecordPB,              /* Record push button handle  */
                  GBM_ANIMATE,               /* Animation control          */
                  MPFROMSHORT(TRUE),         /* Animation flag             */
                  NULL );                    /* Ignore return data         */
   }
   return( TRUE );

}  /* End of RecordTheAudio*/


/******************************************************************************
 * Name         :  SeekTheAudio
 *
 * Description  :  This procedure will Seek to the specified position in
 *                 an audio file.
 *
 * Concepts     :  Changing the current position in an audio file using the
 *                 MCI interface.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_SEEK
 *
 * Parameters   :  hwnd       - Handle for the Main dialog box.
 *                 ulPosition - seeking position.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
BOOL SeekTheAudio(HWND hwnd, ULONG ulPosition )
{
   MCI_SEEK_PARMS  mciSeekParms;     /* for MCI_SEEK                  */
   ULONG           ulError;          /* return value for mci command  */
   BOOL            bReturn = TRUE;   /* function return value         */


   memset( &mciSeekParms,            /* Object to fill with zeros.       */
           0,                        /* Value to place into the object.  */
           sizeof( mciSeekParms ) ); /* How many zero's to use.          */

   mciSeekParms.hwndCallback = hwnd;

   ulError = mciSendCommand( mciOpenParms.usDeviceID,
                             MCI_SEEK,
                             MCI_WAIT | ulPosition,
                             (PVOID) &mciSeekParms,
                             UP_SEEK);
   /*
    * If the recorder is currently recording
    */
   if (eState == ST_RECORDING)
   {
      /*
       * Stop the record button animation
       */
      WinSendMsg ( hwndRecordPB,                /* Record push button handle  */
                   GBM_ANIMATE,                 /* Animation control          */
                   MPFROMSHORT(FALSE),          /* Animation flag             */
                   NULL );                      /* Ignore return data         */
      eState = ST_STOPPED;                      /* Set state to stopped       */
   }
   /*
    * If the recorder is currently playing
    */
   else if (eState == ST_PLAYING)
   {
      /*
       * Stop the play button animation
       */
      WinSendMsg( hwndPlayPB,                    /* Play push button handle   */
                  GBM_ANIMATE,                   /* Animation control         */
                  MPFROMSHORT(FALSE),            /* Animation flag            */
                  NULL );                        /* Ignore return data        */
      eState = ST_STOPPED;                       /* Set state to stopped      */
   }

   if (ulError) {                             /* If an error     */
      ShowMCIErrorMessage( ulError);
      bReturn = FALSE;
   }
   /*
    * Update the status line with appropriate message.
    */
   UpdateTheStatusLine(hwnd, IDS_STOPPED);

   return( bReturn );
}  /* end of SeekTheAudio */


/******************************************************************************
 * Name         :  StopTheDevice
 *
 * Description  :  This procedure will stop the device that is playing or
 *                 recording.
 *
 * Concepts     :  Stopping a device using the MCI interface.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_STOP
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  nothing.
 *
 ******************************************************************************/
VOID StopTheDevice(HWND hwnd)
{
   ULONG                ulError;          /* return value for mci command  */
   MCI_GENERIC_PARMS    mciGenericParms;  /* info data structure for cmd.  */


   /*
    * To stop the device , we will issue an MCI_STOP command via mciSendCommand
    * using an MCI_GENERIC_PARMS structure.  This stop command is done for
    * the device ID.
    */
   mciGenericParms.hwndCallback = hwndMainDialogBox;

   ulError = mciSendCommand( mciOpenParms.usDeviceID,
                             MCI_STOP,
                             MCI_WAIT,
                             (PVOID) &mciGenericParms,
                             UP_STOP);
   if (ulError)
      ShowMCIErrorMessage( ulError);

   if (eState == ST_RECORDING)
   {
      /*
       * Stop the record button animation
       */
      WinSendMsg ( hwndRecordPB,                /* Record push button handle  */
                   GBM_ANIMATE,                 /* Animation control          */
                   MPFROMSHORT(FALSE),          /* Animation flag             */
                   NULL );                      /* Ignore return data         */
   }
   else
   {
      /*
       * Stop the play button animation
       */
      WinSendMsg( hwndPlayPB,                    /* Play push button handle   */
                  GBM_ANIMATE,                   /* Animation control         */
                  MPFROMSHORT(FALSE),            /* Animation flag            */
                  NULL );                        /* Ignore return data        */
   }

   /*
    * Update the status line with appropriate message.
    */
   UpdateTheStatusLine(hwnd, IDS_STOPPED);
   eState = ST_STOPPED;

   return;
}  /* end of StopTheDevice */


/******************************************************************************
 * Name         :  SaveTheAudio
 *
 * Description  :  This procedure will save the audio file that is currently
 *                 loaded.
 *
 * Concepts     :  Save an audio file using MCI interface.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_SAVE
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  nothing.
 *
 ******************************************************************************/
VOID SaveTheAudio(HWND hwnd)
{
   ULONG                ulError;          /* return value for mci command  */
   MCI_GENERIC_PARMS    mciGenericParms;  /* info data structure for cmd.  */
   MCI_SAVE_PARMS       mciSaveParms;


   /*
    * Change pointer to a waiting pointer first, since this might take a
    * couple of seconds.
    */
   WinSetPointer ( HWND_DESKTOP, hptrWait );

   /*
    * Update the status line with appropriate message.
    */
   UpdateTheStatusLine(hwnd, IDS_SAVINGFILE);

   memset( &mciSaveParms,            /* Object to fill with zeros.       */
           0,                        /* Value to place into the object.  */
           sizeof( mciSaveParms ) ); /* How many zero's to use.          */
   /*
    * Initialize the MCI_SAVE_PARMS data structure with the pertinent
    * information then issue an MCI_SAVE command via mciSendCommand
    * to save the currently opened audio file.
    */
   mciSaveParms.hwndCallback = hwnd;
   mciSaveParms.pszFileName = (PSZ)szFileName;   /* audio file to save      */

   ulError = mciSendCommand( mciOpenParms.usDeviceID,
                             MCI_SAVE,
                             MCI_WAIT | MCI_SAVE_FILE,
                             (PVOID) &mciSaveParms,
                             0 );
   /*
    * Change the pointer back to an arrow and update the status line with the
    * appropriate message.
    */
   WinSetPointer ( HWND_DESKTOP, hptrArrow );
   UpdateTheStatusLine(hwnd, IDS_STOPPED);
   if (ulError)
   {
      MessageBox( IDS_CANNOT_SAVE_FILE,                /* ID of the message  */
                  MB_OK | MB_ERROR  | MB_MOVEABLE);    /* style              */
      return;
   }

   /*
    * Get the currently loaded file header information and check the correct
    * options disable all the other option options in the Type menu pulldown.
    */
   SetTheOptions(EXISTING_FILE);


   /*
    * File has been saved. Set the flags.
    */
   fTempFileInUse = FALSE;
   fModified      = FALSE;
   return;
}

/******************************************************************************
 * Name         :  CloseTheDevice
 *
 * Description  :  This procedure will close the audio device.
 *
 * Concepts     :  Closing a device using MCI interface.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_CLOSE
 *
 * Parameters   :  None.
 *
 * Return       :  nothing.
 *
 ******************************************************************************/

VOID CloseTheDevice( VOID)
{
   ULONG                ulError;          /* return value for mci command  */
   MCI_GENERIC_PARMS    mciGenericParms;  /* info data structure for cmd.  */

   /*
    * Now, we close the device. This is done by issuing MCI_CLOSE command.
    */
   ulError = mciSendCommand( mciOpenParms.usDeviceID,
                             MCI_CLOSE,
                             MCI_WAIT,
                             (PVOID) &mciGenericParms,
                             UP_CLOSE);
   if (ulError)
      ShowMCIErrorMessage( ulError);

   mciOpenParms.usDeviceID = (USHORT) NULL;

   return;

}  /* end of CloseTheDevice */


/******************************************************************************
 * Name         :  MessageBox
 *
 * Description  :  This procedure will display messages for the application
 *                 based upon string IDs passed in.  The actual text will be
 *                 loaded from the string table in the resource.
 *
 * Concepts     :  None.
 *
 * MMPM/2 API's :  None.
 *
 * Parameters   :  usMessageID - ID of the message string
 *                 ulStyle     - Style of the message box (WinMessageBox)
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
USHORT MessageBox( USHORT usMessageID, ULONG  ulStyle)
{
   CHAR     achMessage[LEN_ERROR_MESSAGE];
   USHORT   usResult;

   /*
    * Get the string from the Resource defined string table and show it
    * in the message box.
    */
   WinLoadString(
      hab,                             /* HAB for this dialog box.            */
      (HMODULE) NULL,                  /* Get the string from the .exe file.  */
      usMessageID,                     /* Which string to get.                */
      (SHORT) sizeof( achMessage),     /* The size of the buffer.             */
      achMessage );                    /* The buffer to place the string.     */

   usResult =
      WinMessageBox(
         HWND_DESKTOP,                 /* Parent handle of the message box.   */
         hwndMainDialogBox,            /* Owner handle of the message box.    */
         achMessage,                   /* String to show in the message box.  */
         achMsgBoxTitle,               /* Title to shown in the message box.  */
         (USHORT) ID_MESSAGEBOX,       /* Message Box Id.                     */
         ulStyle );                    /* The style of the message box.       */

   return( usResult );

}  /* End of MessageBox */


/******************************************************************************
 * Name         :  ShowMCIErrorMessage
 *
 * Description  :  This window procedure displays an MCI error message
 *                 based upon a ulError return code.  The MCI function
 *                 mciGetErrorString is used to convert the error code into
 *                 a text string and the title is pulled from the resource
 *                 based upon a string id.
 *
 * Concepts     :  Using mciGetErrorString to convert an error code into
 *                 a textual message.
 *
 * MMPM/2 API's :  mciGetErrorString
 *
 * Parameters   :  ulError  -  MCI error code.
 *
 * Return       :  nothing
 *
 ******************************************************************************/
VOID  ShowMCIErrorMessage( ULONG ulError)
{
   CHAR  achBuffer[LEN_ERROR_MESSAGE];

   switch(mciGetErrorString( ulError, (PSZ)achBuffer,   sizeof( achBuffer)))
   {
      case MCIERR_SUCCESS:
         /*
          * This is what we want.  We were able to use mciGetErrorString to
          * retrieve a textual error message we can show in a message box.
          */
         WinMessageBox( HWND_DESKTOP,
                        hwndMainDialogBox,
                        achBuffer,
                        achMsgBoxTitle,
                        (USHORT) ID_MESSAGEBOX,
                        MB_CANCEL | MB_HELP | MB_ERROR | MB_MOVEABLE);
         break;

      case MCIERR_INVALID_DEVICE_ID:
      case MCIERR_OUTOFRANGE:
      case MCIERR_INVALID_BUFFER:
      default:
         MessageBox( IDS_UNKNOWN,
                     MB_CANCEL | MB_HELP | MB_ERROR | MB_MOVEABLE);
         break;
   }

   return;

}  /* end of ShowMCIErrorMessage */


/******************************************************************************
 * Name         :  DoesFileExist
 *
 * Description  :  This helper function determines if a file with a given
 *                 file name exists.
 *
 * Concepts     :  Using MMIO interface to access a data file.
 *
 * MMPM/2 API's :  mmioOpen
 *                 mmioClose
 *
 * Parameters   :  none
 *
 * Return       :  TRUE  -  if a file exists matching szFilename
 *                 FALSE -  if the file does not exist
 *
 ******************************************************************************/
BOOL DoesFileExist( VOID )
{
   BOOL  bReturn;    /* function return value   */
   HMMIO hFile;      /* handle to file          */

   /*
    * Notice that these MMIO functions are analogous to the standard
    * C functions, fopen and fclose.
    */

   hFile = mmioOpen( szFileName, (PMMIOINFO) NULL, MMIO_READ);

   if (hFile != (HMMIO) NULL)
   {
      mmioClose( hFile, 0);
      bReturn = TRUE;
   }
   else
      bReturn = FALSE;

   return( bReturn);
}

/******************************************************************************
 * Name         :  SetAudioVolume
 *
 * Description  :  This helper function sets the audio file volume based upon
 *                 the position of the volume slider.  The slider will be
 *                 queried and the audio file volume will be set.
 *
 * Concepts     :  Setting the volume of a device.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_SET
 *
 * Parameters   :  hwnd      - Handle for the Main dialog box.
 *
 * Return       :  nothing.
 *
 ******************************************************************************/
VOID SetAudioVolume( HWND hwnd )
{
   ULONG                ulError;       /* error value for mci returns   */
   MCI_WAVE_SET_PARMS   mspSet;        /* set values for volume, etc.   */


   if ((!fPassedDevice) && (eState==ST_PLAYING || eState==ST_RECORDING))
   {
      /*
       * To set the volume,  first, the MCI_SET_PARMS structure must be
       * filled with the necessary values.  Then an MCI_SET command
       * should be issued to the device to perform the volume change.
       */
      mspSet.hwndCallback = hwnd;
      mspSet.ulAudio    = MCI_SET_AUDIO_ALL;    /* set all channels     */
      mspSet.ulLevel    = (ULONG) sVolumeLevel; /* volume level desired */

      ulError = mciSendCommand( mciOpenParms.usDeviceID,
                                MCI_SET,
                                MCI_NOTIFY | MCI_SET_AUDIO | MCI_SET_VOLUME,
                                (PVOID) &mspSet,
                                UP_VOLUME);
      if (ulError)
         ShowMCIErrorMessage( ulError);
   }
   return;
}  /* end of SetAudioVolume */

/******************************************************************************
 *
 *  Name         : UpdateTitleText
 *
 *  Description  : Updates the text in the main window's title bar to
 *                 display the app name, followed by the separator,
 *                 followed by the file name.
 *
 *  Concepts    :  none
 *
 *  MMPM/2 API's:  none
 *
 *  Parameters  :  hwnd        = Frame window handle
 *                 pszFileName = filename to update the title bar
 *
 *  Return      :  none
 *
 ******************************************************************************/
VOID UpdateTitleText(HWND hwnd, PSZ pszFileName)
{
   CHAR  szBuffer[                                /* To store the program name*/
           MAXNAMEL+TITLESEPARATORLEN+CCHMAXPATH];/* + separator + file name  */
   CHAR  szSeparator[TITLESEPARATORLEN+1];        /* To store the separator   */
   PCHAR pcFileName;                              /* Buffer for file name     */


   /*
    * Load the program title into the szBuffer.
    */
   WinLoadString(hab, (HMODULE)0, IDS_PROGRAM_TITLE, MAXNAMEL, szBuffer);

   /*
    * Load the title separator into the szSeparator.
    */
   WinLoadString( hab,
                  (HMODULE)0,
                  IDS_TITLEBARSEPARATOR,
                  TITLESEPARATORLEN,
                  szSeparator);

   /*
    * pszFileName is the fully qualified file name. Parse it to get just
    * the file name to display it on the title bar.
    */
   pcFileName =  strrchr(((PCHAR)pszFileName + 1), '\\');

   if (!pcFileName)
      pcFileName = pszFileName;
   else
       pcFileName++;

   strcat(szBuffer, szSeparator);/* First append the separator to the title   */
   strcat(szBuffer, pcFileName); /* Next append the audio file name           */

   /*
    * Display the title on the program title bar.
    */
   WinSetWindowText(WinWindowFromID(hwnd, FID_TITLEBAR), szBuffer);

}   /* End of UpdateTitleText   */

/******************************************************************************
 *
 *  Name         : FileSaveAs
 *
 *  Description  : Processes the File menu's Save as item.  Called whenever
 *                 SAVE AS from the FILE menu is selected. Routine prompts the
 *                 user for the name of the file, then saves the file.
 *
 *  Concepts     : None
 *
 *  MMPM/2 API's : None
 *
 *  Parameters   : hwnd - Handle for the Main dialog box.
 *
 *  Return       : TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 *************************************************************************/
BOOL FileSaveAs(HWND hwnd)
{
   USHORT usResult;

   /*
    * Display Save As Dialog box.
    */
   if ( !ShowStandardFileDlg(IDM_FILE_SAVE_AS) )
   {
      return( FALSE );
   }

   /*
    * if the selected file exists, ask if we want to overwrite it
    */
   if ( DoesFileExist() )
   {
      usResult =
        MessageBox( IDS_FILE_EXISTS, MB_QUERY | MB_OKCANCEL);

      /*
       * If the user wants to overwrite the file, call the SaveTheAudio routine.
       */
      if( usResult == MBID_OK )
         SaveTheAudio(hwnd);
      /*
       * If the user doesnot want to overwrite the file, put up the Save As
       * dialog box again to enter a new name.
       */
      else if ( usResult == MBID_CANCEL )
         FileSaveAs(hwnd);
      else
         return( FALSE );
   }
   /*
    * The selected file name does not exist in the path. Save the currently
    * loaded file with the selected file name.
    */
   else
      SaveTheAudio(hwnd);

   return( TRUE );
}   /* End of FileSaveAs   */

/******************************************************************************
 * Name         :  ShowStandardFileDlg
 *
 * Description  :  Based upon the parameters passed in, show the user either
 *                 the Open dialog box or the Save as dialog box.
 *
 * Concepts     :  None.
 *
 * MMPM/2 API's :  None.
 *
 * Parameters   :  usSelection - Indicates which dlg to display-Open or Save as
 *
 * Return       :  TRUE  -  if the a file open is success.
 *                 FALSE -  if could not open the file.
 *
 ******************************************************************************/
BOOL ShowStandardFileDlg( USHORT usSelection)
{
   FILEDLG fileDialog;                     /* Open file dialog structure.     */
   CHAR    szTitle[FILE_NAME_SIZE];        /* Application title.              */
   CHAR    szButtonText[FILE_NAME_SIZE];   /* Save as dialog box button text. */
   USHORT  usResult = 0;                   /* To store return result.         */

   /*
    * Initialize the structure
    */
   memset( &fileDialog,                    /* Object to fill with zeros.      */
           0,                              /* Value to place into the object. */
           sizeof(fileDialog));            /* How many zero's to use.         */

   fileDialog.cbSize = sizeof(FILEDLG);
   fileDialog.pfnDlgProc = (PFNWP)OpenFileDlgProc;
   fileDialog.ulUser = (ULONG) hwndMainDialogBox;

   switch (usSelection)
   {
      case IDM_FILE_OPEN :

         WinLoadString(hab,(HMODULE)0,IDS_FILEOPEN_TITLE,CCHMAXPATH,szTitle);
         fileDialog.pszTitle = szTitle;
         fileDialog.fl = FDS_HELPBUTTON | FDS_CENTER | FDS_OPEN_DIALOG;
         fileDialog.usDlgId = IDD_FILEWINDOW;

         WinLoadString(hab, (HMODULE)0, IDS_FILEOPENEXT, CCHMAXPATH,
                       fileDialog.szFullFile);
         break;

      case IDM_FILE_SAVE_AS :

         WinLoadString(hab, (HMODULE)0, IDS_SAVE_AS, CCHMAXPATH, szButtonText);
         WinLoadString(hab,(HMODULE)0,IDS_FILESAVEAS_TITLE,CCHMAXPATH,szTitle);
         fileDialog.pszTitle = szTitle;
         fileDialog.fl = FDS_HELPBUTTON | FDS_CENTER | FDS_SAVEAS_DIALOG;
         fileDialog.usDlgId = FILESAVE;
         fileDialog.pszOKButton = (PSZ) szButtonText;
         break;
   }
   /*
    *Call the standard file dialog to get the file
    */
   usResult = WinFileDlg(HWND_DESKTOP, hwndMainDialogBox, &fileDialog);

   if (usResult != 0)
   {
      if (fileDialog.lReturn == DID_OK)
      {
         /*
          * initialize szFileName variable to get a new file name.
          */
         memset( szFileName, 0, sizeof(szFileName ));

         strncpy( (PCHAR)szFileName, fileDialog.szFullFile,
                  min( strlen( &fileDialog.szFullFile[0]),
                       CCHMAXPATH ));
      }
      else if (fileDialog.lReturn == DID_CANCEL)
      {
         return( FALSE );
      }
   }
   return( TRUE );
}

/******************************************************************************
 * Name         :  OpenFileDlgProc
 *
 * Description  :  This function displays the standard open dialog box.

 * Concepts     :  None.
 *
 * MMPM/2 API's :  None.
 *
 * Parameters   :  hwnd - Handle for the File dialog box.
 *                 msg  - Message received by the dialog box.
 *                 mp1  - Parameter 1 for the just received message.
 *                 mp2  - Parameter 2 for the just received message.
 *
 * Return       :  result of default processing.
 ******************************************************************************/

MRESULT EXPENTRY OpenFileDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   return WinDefFileDlgProc(hwnd, msg, mp1, mp2);
}

/******************************************************************************
 * Name         :  SaveTheChanges
 *
 * Description  :  If the file has been modified, this function asks the user
 *                 if he wants to save the changes or not.
 *
 * Concepts     :  none
 *
 * MMPM/2 API's :  none
 *
 * Parameters   :  hwnd  - Handle to the main dialog box.
 *
 * Return       :  TRUE  -  if changes saved or not saved .
 *                 FALSE -  if cancel out.
 *************************************************************************/
USHORT SaveTheChanges( HWND hwnd )
{
   USHORT   usResult;

   /*
    * Display a message box asking the user to save the changes made to the
    * audio file or not.
    */
   usResult =
     MessageBox( IDS_FILE_CHANGED,
                 MB_WARNING | MB_YESNOCANCEL | MB_MOVEABLE | MB_HELP);
   if (usResult == MBID_YES)
   {
      /*
       * User wants to save the changes made to the audio file. If the
       * currently loaded file is a tempfile display Save as dialog box for
       * the file name. Else call SaveTheAudio routine to save the audio file.
       */
      if (fTempFileInUse)
         FileSaveAs(hwnd);
      else
         SaveTheAudio(hwnd);
   }
   /*
    * If the user does not want to save the changes made to the file set the
    * flags.
    */
   else if (usResult == MBID_NO)
   {
      fModified      = FALSE;
      fTempFileInUse = FALSE;
   }

   return( usResult );
}
