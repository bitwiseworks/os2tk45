/******************************************************************************
 * File Name    : CAPSAMP.C
 *
 * Description  : This file contains the C source code required for the
 *                caption sample application program.
 *
 * Concepts     : This sample program is part of MMPM/2 sample closed
 *                captioning system.  This sample closed captioning system
 *                is composed of three parts: Caption Creation Utility,
 *                Caption DLL, and Caption Sample Application (this sample).
 *                The Caption Creation Utility creates a "captioned" file.
 *                This is a text file with timing information relating to
 *                it's associated audio file.  The Caption DLL provides 3
 *                API's that drive the display and management of a "caption
 *                window" in a PM application.  The caption sample (this
 *                program) illustrates how an application uses the 3 API's
 *                provided by the caption DLL to take advantage of it's
 *                services.
 *
 *                This sample captioning system is provided to illustrate one
 *                of the many ways that captioning may be implemented using
 *                MMPM/2. As with all MMPM/2 samples, this may be used as it
 *                is provided, or it may be modified in any way you prefer.
 *                Please refer to the Application Programmers Reference for
 *                more information on this sample captioning system.
 *
 * MMPM/2 API's : List of all MMPM/2 API's that are used in
 *                this module
 *
 *                mciSendString
 *                mciGetErrorString
 *                mciQuerySysValue
 *                mmioOpen
 *                mmioClose
 *
 * Required
 *    Files     : capsamp.c        Source Code.
 *                capsamp.h        Include file.
 *                captions.h       Include file.
 *                capsamp.dlg      Dialog definition.
 *                capsamp.rc       Resources.
 *                capsamp.ipf      Help text.
 *                makefile         Make file.
 *                capsamp.def      Linker definition file.
 *                capsamp.ico      Program icon.
 *
 * Copyright (C) IBM 1993
 ******************************************************************************/

#define  INCL_WIN                   /* required to use Win APIs.           */
#define  INCL_PM                    /* required to use PM APIs.            */
#define  INCL_WINHELP               /* required to use IPF.                */
#define  INCL_GPIPRIMATIVES         /* required for GPI APIs.              */
#define  INCL_OS2MM                 /* required for MCI and MMIO headers   */
#define  INCL_SECONDARYWINDOW       /* required for secondary window       */
#define  INCL_CIRCULARSLIDER        /* required for circular slider control*/
#define  INCL_GRAPHICBUTTON         /* required for graphic button control */
#define  INCL_DOSERRORS             /* required for dos errors             */

#include <os2.h>
#include <os2me.h>
#include <stdlib.h>
#include <string.h>

#include <sw.h>

#include "capsamp.h"
#include "captions.h"

enum DeviceStates {ST_STOPPED, ST_PLAYING, ST_PAUSED};
/*
 * Procedure/Function Prototypes
 */
MRESULT EXPENTRY MainDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY ProductInfoDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2);
MRESULT EXPENTRY SettingsDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

BOOL    DoesFileExist( PSZ pszFileName );
VOID    Finalize( VOID );
VOID    Initialize( VOID );
VOID    InitializeHelp( VOID );
USHORT  MessageBox( USHORT usMessageID, ULONG  ulStyle );
BOOL    OpenTheFile( HWND hwnd );
BOOL    PlayTheAudio( HWND hwnd );
VOID    SetAudioVolume( VOID);
VOID    ShowMCIErrorMessage( ULONG ulError );
VOID    StopTheDevice( VOID );
VOID    ResumeTheAudio( VOID );
VOID    PauseTheAudio( VOID );
BOOL    SeekTheAudio( VOID );
VOID    CloseTheDevice( VOID );
VOID    StartTheCaption( HWND hwnd );
VOID    InitializeSpinButton( HWND hwnd );
VOID    SetAudioSliderPosition( HWND hwnd );
BOOL    SendString( HWND hwnd, PCHAR pcMCIString, USHORT usUserParm );

/*************** End of Procedure/Function Prototypes *************************/

/*
 * Global Variables.
 */
HAB    hab;
HMQ    hmq;
HWND   hwndFrame;                      /* Handle to the frame window.         */
HWND   hwndCaption;                    /* Window which displays caption text  */
HWND   hwndHelpInstance;               /* Handle to Main Help window.         */
HWND   hwndMainDialogBox;              /* Handle to the dialog window.        */
HWND   hwndPlayPB;                     /* Handle to the play push button      */
HWND   hwndAudioSlider;                /* Handle to the audio slider          */
HWND   hwndProductInfo = (HWND)0;      /* Handle to the product info. dlg box */
ULONG  ulAudioLength;                  /* Audio length in MM units            */
SHORT  sAudioArmRange;                 /* Slider arm range in pels            */

enum   DeviceStates   eState = ST_STOPPED;       /* state of the device       */

SHORT  sVolumeLevel   = INIT_VOLUME;   /* desired volume level                */
BOOL   fPassedDevice  = FALSE;         /* for MCI_ACQUIRE to play the file    */
BOOL   fDeviceOpen    = FALSE;         /* indicate we've opened for first time*/
CHAR   achMsgBoxTitle[MAXNAMEL];       /* Error message box title             */

CHAR   szWhite [OPTIONSLEN];           /* for caption window background color */
CHAR   szYellow[OPTIONSLEN];           /* for caption window background color */
CHAR   szGray  [OPTIONSLEN];           /* for caption window background color */
CHAR   szBlue  [OPTIONSLEN];           /* for caption window text color       */
CHAR   szBlack [OPTIONSLEN];           /* for caption window text color       */
CHAR   szRed   [OPTIONSLEN];           /* for caption window text color       */
CHAR   szCenter[OPTIONSLEN];           /* for caption window position         */
CHAR   szLeft  [OPTIONSLEN];           /* for caption window position         */
CHAR   szRight [OPTIONSLEN];           /* for caption window position         */

PCHAR  textColumns[3]     = {"15", "35", "50"};
PCHAR  textRows[3]        = {"2",  "3",  "4" };
PCHAR  backgroundColor[3] = {szWhite,  szYellow, szGray };
PCHAR  textColor[3]       = {szBlue,   szBlack,  szRed  };
PCHAR  windowPosition[3]  = {szCenter, szLeft,   szRight};

CHAR   szReturn[CCHMAXPATH];           /* return value from mciSendString     */
RECTL  rclMain;                        /* for the dimensions of the main wind.*/
HPOINTER  hptrWait, hptrArrow;  /* Pointer handles for use during long waits. */
FONTMETRICS fm;                 /* font matrics structure.                    */

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
 * Concepts     : Initialize captioning window using ccInitialize.
 *
 * MMPM/2 API's : WinLoadSecondaryWindow
 *                WinQuerySecondaryHWND
 *
 * Parameters   : None.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID Initialize( VOID)
{
   CHAR     szDefaultSize[CCHMAXPATH];   /* buffer for default size menu text */
   CHAR     achTitle[MAXNAMEL];          /* buffer for window title text      */


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
      IDS_CAP_SAM_ERROR,                    /* Which string to get.           */
      (SHORT) sizeof( achMsgBoxTitle),      /* The size of the buffer.        */
      achMsgBoxTitle);                      /* The buffer to place the string.*/

   hwndFrame =                    /* Returns the handle to the frame.         */
       WinLoadSecondaryWindow(
          HWND_DESKTOP,           /* Parent of the dialog box.                */
          HWND_DESKTOP,           /* Owner of the dialog box.                 */
          (PFNWP) MainDlgProc,    /* 'Window' procedure for the dialog box.   */
          (HMODULE) NULL,         /* Where is the dialog.  Null is EXE file.  */
          IDD_MAIN_WINDOW,        /* Dialog ID.                               */
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
    * Create the caption window and save the handle for further use.
    */
   hwndCaption = ccInitialize ( (HWND) hwndMainDialogBox );

   if (( hwndCaption == (ULONG) CCERR_INVALID_WINDOW_HANDLE ) ||
       ( hwndCaption == (HWND)0))
       MessageBox( IDS_INVALID_WINDOW_HANDLE,
                   MB_CANCEL | MB_ERROR  | MB_MOVEABLE);

   WinShowWindow( hwndFrame, TRUE );           /* Display the main window.    */

   InitializeHelp();                           /* Initialize the help.        */

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
 * MMPM/2 API's : WinDestroySecondaryWindow
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
 * Concepts     : Illustrates:
 *                  - How to participate in MMPM/2 device sharing scheme.
 *                  - How to handle notification messages.
 *                  - How to implement graphic pushbutton's
 *                  - How to implement secondary windows.
 *
 * MMPM/2 API's : WinDefSecondaryWindowProc
 *                mciSendString
 *                   - acquire
 *
 * Parameters   : hwnd - Handle for the Main dialog box.
 *                msg  - Message received by the dialog box.
 *                mp1  - Parameter 1 for the just received message.
 *                mp2  - Parameter 2 for the just received message.
 *
 * Return       : 0 or the result of default processing.
 *
 ******************************************************************************/
MRESULT EXPENTRY MainDlgProc( HWND   hwnd,
                              ULONG  msg,
                              MPARAM mp1,
                              MPARAM mp2 )
{
   LONG        lmciSendStringRC;      /* return value form mciSendString      */
   HPOINTER    hpProgramIcon;         /* handle to program's icon             */
   USHORT      usCommandMessage;      /* command message for notify           */
   USHORT      usNotifyCode;          /* notification message code            */
   SHORT       sArmPosition;          /* Updated slider arm position          */
   SHORT       sWmControlID;          /* WM_CONTROL id                        */
   ULONG       ulTime;                /* Media position in mm units           */


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
         hwndAudioSlider = WinWindowFromID( hwnd, IDC_AUDIO_SLIDER );
         hwndPlayPB      = WinWindowFromID( hwnd, IDC_GPB_PLAY );   /* play PB*/

         /*
          * Enable the audio slider.
          */
         WinEnableWindow( hwndAudioSlider, TRUE );

         /*
          * The slider control cannot be completely set from the dialog
          * template so some aspects must be set here.  We will set the
          * volume range to 0-100, increment to 1-10, and the initial
          * volume level to 75.
          */
         WinSendMsg( WinWindowFromID(hwnd, IDC_SL_VOLUME),
                     CSM_SETRANGE,
                     (MPARAM) 0L,
                     (MPARAM) 100L);

         WinSendMsg( WinWindowFromID(hwnd, IDC_SL_VOLUME),
                     CSM_SETINCREMENT,
                     (MPARAM) 10L,
                     (MPARAM) 1L);

         WinSendMsg( WinWindowFromID(hwnd, IDC_SL_VOLUME),
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

         return( (MRESULT) 0);

      case WM_CLOSE :
         /*
          * If the device is opened, close the device.
          */
         if (fDeviceOpen)
            CloseTheDevice();
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

      case WM_COMMAND :
         /*
          * To get which pushbutton was pressed the SHORT1FROMMP macro is used.
          */
         switch (SHORT1FROMMP(mp1))
         {
            case IDC_GPB_PLAY:               /* user selected "Play"          */
               switch ( eState )
               {
                  /*
                   * If the audio is currently stopped, open/load the audio
                   * file.
                   */
                  case ST_STOPPED:
                     if (OpenTheFile(hwnd))
                     {
                        /*
                         * Set the audio slider position and start playing.
                         */
                        if ( PlayTheAudio( hwnd ) )
                           /*
                            * If play was success, start captioning.
                            */
                           StartTheCaption(hwnd);
                     }
                     break;
                  /*
                   * If the audio is currently paused, resume the audio play.
                   */
                  case ST_PAUSED:
                     ResumeTheAudio();
                     break;
               }
               break;

            case IDC_GPB_PAUSE:
               switch ( eState )
               {
                 /*
                  * If the audio is currently playing, pause it.
                  */
                 case ST_PLAYING:
                    PauseTheAudio();
                    break;
                 /*
                  * If the audio is currently paused, resume the audio play.
                  */
                 case ST_PAUSED:
                    ResumeTheAudio();
                    break;
              }
              break;

            case IDC_GPB_REWIND:         /* user selected "Rewind" pushbutton */
               if (fDeviceOpen)          /* If the device is opened           */
               {
                  /*
                   * Stop the audio, hide the caption window and set the audio
                   * slider position to zero.
                   */
                  StopTheDevice();
                  ccSendCommand( CC_STOP, MPFROMHWND(hwndCaption), 0 );
                  WinSendMsg(
                         hwndAudioSlider,
                         SLM_SETSLIDERINFO,
                         MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_RANGEVALUE),
                         MPFROMSHORT( 0 ) );
               }
               break;

            case IDC_GPB_STOP:             /* user selected "Stop" pushbutton */
               /*
                * If the audio is not in stopped state, stop the device and
                * hide the text window.
                */
               if (eState != ST_STOPPED)
               {
                  StopTheDevice();
                  ccSendCommand( CC_STOP, MPFROMHWND(hwndCaption), 0 );
               }
               break;

            case IDM_SETTINGS:            /* user selected "Settings" menu */
               /*
                * Call the settings dialog window.
                */
               WinDlgBox(
                  HWND_DESKTOP,                /* Parent of the dialog box.   */
                  hwndMainDialogBox,           /* Owner of the dialog box.    */
                  (PFNWP) SettingsDlgProc,     /* Dialog box procedure.       */
                  (HMODULE) NULL,              /* Dialog is where, EXE file.  */
                  IDD_SETTINGS_DLG,            /* Dialog ID.                  */
                  (PVOID) NULL);               /* Dialog Creation Parameters. */

               return( 0 );

            case IDM_HELP_PRODUCTINFO :
               /*
                * See if the product information dialog box is already
                * displayed.
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
                         IDD_PRODUCTINFO_DLG,      /* Dialog ID.              */
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
                           MPFROMSHORT(3),
                           MPFROMSHORT(HM_RESOURCEID));
               break;

            default:
               break;

         }  /* End of Command Switch */
         return( (MRESULT) 0);

      case WM_CONTROL:
        sWmControlID  = SHORT1FROMMP(mp1);
        usNotifyCode  = (USHORT) SHORT2FROMMP(mp1);

        switch ( sWmControlID )
        {
           case IDC_SL_VOLUME:
              if ((usNotifyCode == CSN_CHANGED) ||        /* change volume?   */
                  (usNotifyCode == CSN_TRACKING))         /* tracking volume? */
              {
                /*
                 *  Every time the volume control setting is changed, save the
                 *  new value in sVolumeLevel.
                 */
                 sVolumeLevel = SHORT1FROMMP (mp2);

                /*
                 *  Set the new volume level
                 */
                 SetAudioVolume();
              }
              break;

           case IDC_AUDIO_SLIDER:
              /*
               * We need to determine if the SLN_CHANGE message caming from
               * the user or from the program.  If the slider has the focus,
               * that implies that the user updated the slider control.
               */
              if ( ( usNotifyCode != SLN_CHANGE ) ||
                   ( WinQueryFocus ( HWND_DESKTOP ) != hwndAudioSlider ) )
                 /*
                  * SLN_CHANGE messages are not originated by the user, ignore.
                  */
                 break;

              /*
               * User updated the audio slider. Change the focus to the play
               * pushbutton. This is needed to continue the above test.
               */
              WinSetFocus( HWND_DESKTOP, hwndPlayPB );

              /*
               * Get the new slider position.
               */
              sArmPosition = SHORT1FROMMP(mp2);

              if ((eState == ST_PLAYING) || (eState == ST_PAUSED))
              {
                 /*
                  * Audio is playing or paused. Stop the audio and restart
                  * from the new position.
                  */
                 StopTheDevice();
                 PlayTheAudio( hwnd );
              }
              break;
        }
         return( (MRESULT) 0);


      /*
       * The next two messages are handled so that this application can
       * participate in device sharing.  Since it opens the device as
       * spareable device, other applications can gain control of the device.
       * When this happens, we will receive a MM_MCIPASSDEVICE message.  We
       * keep track of this device passing in the fPassedDevice Boolean
       * variable.
       *
       * If we do not have access to the device when we receive an WM_ACTIVATE
       * message, then we will issue an acquire device command to gain
       * access to the device.
       */

      case MM_MCIPASSDEVICE:
         if (SHORT1FROMMP(mp2) == MCI_GAINING_USE)           /* GAINING USE */
         {
            fPassedDevice = FALSE;               /* Gaining control of device.*/
            if (eState == ST_PLAYING)            /* If the file was playing   */
            {
               /*
                * Start play button animation.
                */
               WinSendMsg( hwndPlayPB,           /* Play button handle        */
                           GBM_ANIMATE,          /* Animation control         */
                           MPFROMSHORT(TRUE),    /* Animation flag            */
                           NULL );               /* Ignore return data        */
            }
         }
         else                                                /* LOSING USE  */
         {
            fPassedDevice = TRUE;                /* Losing  control of device.*/
            if (eState == ST_PLAYING)            /* If the file was playing   */
            {
               /*
                * Stop play button animation.
                */
               WinSendMsg( hwndPlayPB,           /* Play button handle        */
                           GBM_ANIMATE,          /* Animation control         */
                           MPFROMSHORT(FALSE),   /* Animation flag            */
                           NULL );               /* Ignore return data        */
            }
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case WM_ACTIVATE:

         /* We use the WM_ACTIVATE message to participate in device sharing.
          * We first check to see if this is an activate or a deactivate
          * message (indicated by mp1).  Then, we check to see if we've passed
          * control of the audio device.  If these conditions are true,
          * then we issue an acquire device command to regain control of
          * the device, since we're now the active window on the screen.
          */
         if ((BOOL)mp1 && fPassedDevice == TRUE)
         {
           /*
            * To acquire the device, we will issue MCI_ACQUIREDEVICE
            * command to the MCI.
            */
            SendString( hwnd, "acquire capsamp notify", 0 );
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

     case MM_MCIPOSITIONCHANGE:
        /*
         * This message will be returned (in mm time) to the application
         * whenever the audio position changed. This time will be used to
         * increment the audio position slider. This message is only
         * generated during playback.
         */
        if ( eState == ST_PLAYING )
        {
           ulTime = (ULONG) LONGFROMMP(mp2);

           /*
            * Get the new slider arm position and set it.
            */
           sArmPosition =
               (SHORT) ( ( ulTime * ( sAudioArmRange - 1) ) / ulAudioLength );
           WinSendMsg(
              hwndAudioSlider,
              SLM_SETSLIDERINFO,
              MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
              MPFROMSHORT( sArmPosition ));
        }
        return 0;

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
         usNotifyCode    = (USHORT) SHORT1FROMMP( mp1);  /* low-word  */
         usCommandMessage = (USHORT) SHORT2FROMMP( mp2); /* high-word */

         switch (usCommandMessage)
         {
            case MCI_PLAY:
               switch (usNotifyCode)
               {
                  case MCI_NOTIFY_SUPERSEDED:
                  case MCI_NOTIFY_ABORTED:
                     /* we don't need to handle these messages. */
                     break;

                  default:
                     if (eState != ST_STOPPED)
                     {
                        /*
                         * Set the audio slider to the end.
                         */
                        WinSendMsg(
                           hwndAudioSlider,
                           SLM_SETSLIDERINFO,
                           MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_RANGEVALUE),
                           MPFROMSHORT( sAudioArmRange - 1 ) );

                        /*
                         * Hide the caption window and update the internal
                         * state of the dialog box.
                         */
                        ccSendCommand( CC_STOP, MPFROMHWND(hwndCaption), 0);
                        eState = ST_STOPPED;

                        /*
                         * Stop the play button animation
                         */
                        WinSendMsg( hwndPlayPB,        /* Play button handle  */
                                    GBM_ANIMATE,       /* Animation control   */
                                    MPFROMSHORT(FALSE),/* Animation flag      */
                                    NULL );            /* Ignore return data  */

                        if ( usNotifyCode != MCI_NOTIFY_SUCCESSFUL)
                           /*
                            * Notification error message. We need to display
                            * the error message to the user.
                            */
                           ShowMCIErrorMessage( usNotifyCode);
                     }
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


/*******************************************************************************
 * Name        : SettingsDlgProc
 *
 * Description : This function allows you to change the properties of
 *               caption window.
 *
 * Concepts    : Illustrates how to change the properties of the captioning
 *               window.  The following properties of the caption window may
 *               be changed using the CC_SET message via the ccSendCommand
 *               API:
 *                  - Text columns.
 *                  - Text rows.
 *                  - caption window background color.
 *                  - Text color.
 *                  - Caption window position in the main dialog box.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hwnd - Handle for the Device dialog box.
 *               msg  - Message received by the dialog box.
 *               mp1  - Parameter 1 for the just received message.
 *               mp2  - Parameter 2 for the just received message.
 *
 * Return      : 0 or the result of default processing.
 *
 ******************************************************************************/
MRESULT EXPENTRY SettingsDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   PCHAR              buffer;              /* to store the spin button text   */
   CC_SET_PARMS       ccSetParms;          /* set parms for CC_SET            */
   ULONG              ulArrayIndexValue=0; /* for spin button return value    */
   CC_STATUS_PARMS    ccStatusParms;       /* status parms for CC_STATUS      */
   HPS                hps;                 /* handle to caption window PS     */

   switch( msg )
   {
      case WM_INITDLG :
         /*
          * Load all the required option strings from the resource file.
          */
         WinLoadString( hab, (HMODULE)NULL, IDS_WHITE,  OPTIONSLEN, szWhite  );
         WinLoadString( hab, (HMODULE)NULL, IDS_YELLOW, OPTIONSLEN, szYellow );
         WinLoadString( hab, (HMODULE)NULL, IDS_GRAY,   OPTIONSLEN, szGray   );
         WinLoadString( hab, (HMODULE)NULL, IDS_BLUE,   OPTIONSLEN, szBlue   );
         WinLoadString( hab, (HMODULE)NULL, IDS_BLACK,  OPTIONSLEN, szBlack  );
         WinLoadString( hab, (HMODULE)NULL, IDS_RED,    OPTIONSLEN, szRed    );
         WinLoadString( hab, (HMODULE)NULL, IDS_CENTER, OPTIONSLEN, szCenter );
         WinLoadString( hab, (HMODULE)NULL, IDS_LEFT,   OPTIONSLEN, szLeft   );
         WinLoadString( hab, (HMODULE)NULL, IDS_RIGHT,  OPTIONSLEN, szRight  );

         /*
          * Query the size of the main window for further use.  This is
          * needed to calculate the correct position of the caption window
          * inside the main window.
          */
         WinQueryWindowRect ( hwndMainDialogBox, &rclMain );

         /*
          * Get caption window presentation space.  Get the font metrics
          * for the default font for this presentation space.  This data
          * is needed to calculate the caption window positioning inside
          * the main window.
          */
         hps = WinGetPS( hwndCaption );
         GpiQueryFontMetrics( hps, (LONG) sizeof (fm), &fm );

         /*
          * Initialize the Spinbuttons.
          */
         InitializeSpinButton(hwnd);
         return( (MRESULT) 0 );

      case WM_CLOSE :
          WinDismissDlg( hwnd, TRUE );      /* Close the Dialog box.          */
          return( 0 );

      case WM_HELP :
         /*
          * The dialog window has received a request for help from the user,
          * i.e., the Help pushbutton was pressed.  Send the HM_DISPLAY_HELP
          * message to the Help Instance with the IPF resource identifier
          * for the correct HM panel.  This will show the help panel for this
          * dialog box.
          */
         WinSendMsg( hwndHelpInstance,
                     HM_DISPLAY_HELP,
                     MPFROMSHORT(2),
                     MPFROMSHORT(HM_RESOURCEID));
         return( 0 );

      case WM_COMMAND :
         switch( SHORT1FROMMP( mp1 ) )
         {
            case IDC_OK_PB :           /* user selected "OK" pushbutton */
              /*
               * Initialize the CC_SET_PARMS with zeros.
               */
              memset( &ccSetParms, 0, sizeof(ccSetParms));

              /*
               * Query the background color spin button. The array index
               * value will be returned in ulArrayIndexValue variable.
               */
              WinSendDlgItemMsg(
                 hwnd,
                 IDC_BACKGROUND_COLOR_SB,
                 SPBM_QUERYVALUE,
                 (MPARAM) &ulArrayIndexValue,
                 MPFROMLONG(0));

              /*
               * Get the actual value.
               */
              buffer = backgroundColor[ulArrayIndexValue];

              /*
               * Initialize the CC_SET_PARMS data structure with the
               * appropriate information.
               */
              if (stricmp(buffer, szYellow) == 0)
                 ccSetParms.ulBackgroundColor = CLR_YELLOW;
              else
              if (stricmp(buffer, szWhite) == 0)
                 ccSetParms.ulBackgroundColor = CLR_WHITE;
              else
                 ccSetParms.ulBackgroundColor = CLR_DARKGRAY;

              /*
               * Query the text columns spin button. The array index
               * value will be returned in ulArrayIndexValue variable.
               */
              WinSendDlgItemMsg(
                  hwnd,
                  IDC_TEXT_COLUMNS_SB,
                  SPBM_QUERYVALUE,
                  (MPARAM) &ulArrayIndexValue,
                  MPFROMLONG(0));

              /*
               * Get the actual value.
               */
              ccSetParms.ulColumns=(ULONG)atoi(textColumns[ulArrayIndexValue]);

              /*
               * Query the text color spin buttons.
               */
              WinSendDlgItemMsg(
                 hwnd,
                 IDC_TEXT_COLOR_SB,
                 SPBM_QUERYVALUE,
                 (MPARAM) &ulArrayIndexValue,
                 MPFROMLONG(0));

              /*
               * Get the actual value.
               */
              buffer = textColor[ulArrayIndexValue];

              /*
               * Initialize the CC_SET_PARMS data structure with the
               * appropriate information.
               */
              if (stricmp(buffer, szBlue) == 0)
                 ccSetParms.ulTextColor = CLR_DARKBLUE;
              else
              if (stricmp(buffer, szRed) == 0)
                 ccSetParms.ulTextColor = CLR_RED;
              else
                 ccSetParms.ulTextColor = CLR_BLACK;

              /*
               * Query the window position spin buttons. The array index
               * value will be returned in ulArrayIndexValue variable.
               */
              WinSendDlgItemMsg(
                 hwnd,
                 IDC_WINDOW_POSITION_SB,
                 SPBM_QUERYVALUE,
                 (MPARAM) &ulArrayIndexValue,
                 MPFROMLONG(0));

              /*
               * Get the actual value.
               */
              buffer = windowPosition[ulArrayIndexValue];

              /*
               * Calculate the correct caption window position inside the
               * main window using fm.lAveCharaWidth and initialize the
               * CC_SET_PARMS data structure with the appropriate information.
               */
              if (stricmp(buffer, szLeft) == 0)
              {
                 ccSetParms.ulXposition = (ULONG) 5;
                 ccSetParms.ulYposition = (ULONG) 5;
              }
              else
              if (stricmp(buffer, szCenter) == 0)
              {
                 if (ccSetParms.ulColumns == 15)
                 {
                    ccSetParms.ulXposition = (ULONG)
                          (rclMain.xRight - fm.lAveCharWidth * 15 ) / 2;
                    ccSetParms.ulYposition = (ULONG) 5;
                 }
                 else
                 if (ccSetParms.ulColumns == 35)
                 {
                    ccSetParms.ulXposition = (ULONG)
                          (rclMain.xRight - fm.lAveCharWidth * 35 ) / 2;
                    ccSetParms.ulYposition = (ULONG) 5;
                 }
                 else
                 {
                    ccSetParms.ulXposition = (ULONG)
                          (rclMain.xRight - fm.lAveCharWidth * 50 ) / 2;
                    ccSetParms.ulYposition = (ULONG) 5;
                 }
              }
              else
              {
                 if (ccSetParms.ulColumns == 15)
                 {
                    ccSetParms.ulXposition = (ULONG)
                         (rclMain.xRight - fm.lAveCharWidth * 15 ) - 5;
                    ccSetParms.ulYposition = (ULONG) 5;
                 }
                 else
                 if (ccSetParms.ulColumns == 35)
                 {
                    ccSetParms.ulXposition = (ULONG)
                         (rclMain.xRight - fm.lAveCharWidth * 35 ) - 5;
                    ccSetParms.ulYposition = (ULONG) 5;
                 }
                 else
                 {
                    ccSetParms.ulXposition = (ULONG)
                         (rclMain.xRight - fm.lAveCharWidth * 50 ) - 5;
                    ccSetParms.ulYposition = (ULONG) 5;
                 }
              }

              /*
               * Query the text rows spin button. The array index
               * value will be returned in ulArrayIndexValue variable.
               */
              WinSendDlgItemMsg(
                 hwnd,
                 IDC_TEXT_ROWS_SB,
                 SPBM_QUERYVALUE,
                 (MPARAM) &ulArrayIndexValue,
                 MPFROMLONG(0));

              /*
               * Get the actual value and initialize the CC_SET_PARMS
               * data structure with the appropriate information.
               */
              ccSetParms.ulRows = (ULONG) atoi( textRows[ulArrayIndexValue] );

              /*
               * Issue the CC_SET command with the ccSendCommand and close
               * the dialog box.
               */
              ccSendCommand(CC_SET, MPFROMHWND(hwndCaption), &ccSetParms);
              WinDismissDlg( hwnd, TRUE );

              return( 0 );

            case DID_CANCEL:
            case IDC_CANCEL_PB:

               WinDismissDlg( hwnd, TRUE ); /* Close the Dialog box.          */

               return( 0 );

         }  /* End of Command Switch */

         return( 0 );

      default:
         return( WinDefDlgProc( hwnd, msg, mp1, mp2 ) );

   }  /* End of Switch */

   return( (MRESULT) FALSE );

} /* End of SettingsDlgProc */


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
 * MMPM/2 API's :  mciSendString
 *                    - open
 *                    - load
 *
 * Parameters   :  hwnd  - Handle for the Main dialog box.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
BOOL OpenTheFile( HWND hwnd )
{

   /*
    * Change pointer to a waiting pointer first, since this might take a
    * couple of seconds.
    */
   WinSetPointer( HWND_DESKTOP, hptrWait );

   if (! DoesFileExist( "CAPSAMP.WAV" ))
   {
      MessageBox( IDS_CANNOT_FIND_AUDIO_FILE,              /* ID of the message          */
                  MB_CANCEL | MB_ERROR  | MB_MOVEABLE);    /* style         */
      return( FALSE );
   }

   if (! fDeviceOpen )
   /* If this is the first time thru this routine, then we need to open
    * the device.
    */
   {
      /*
       * To open the device, we will issue MCI_OPEN command to the MCI.
       */
      if ( SendString(
               hwnd,
               "open capsamp.wav alias capsamp wait shareable readonly",
               0 ) )
      {
         /*
          * Open success, set the flag to indicate that next time thru this
          * routine we do not need to open the device.
          * Also, since the file's now open, we can set up the audio slider.
          */
         fDeviceOpen = TRUE;
         SetAudioSliderPosition(hwnd);
      }
      else
         return( FALSE );
   }
   else
   {
      /*
       * The device is already open,
       * so we only need to load the appropriate file onto the device.
       *
       * To load the audio file into the device, we will issue MCI_LOAD
       * command to the MCI.
       */
      if (!SendString(hwnd, "load capsamp capsamp.wav wait readonly", 0))
         return( FALSE );
   }
   /*
    * Now that we're done here, change the pointer back to the arrow.
    */
   WinSetPointer ( HWND_DESKTOP, hptrArrow );

   return( TRUE );

}  /* end of OpenTheFile */

/******************************************************************************
 * Name         :  SetAudioSliderPosition
 *
 * Description  :  This routine queries the length of the audio file, sets
 *                 up the audio slider information and resets the slider
 *                 position to 0.  Its called each time the user plays the
 *                 audio file.
 *
 * Concepts     :  Query the length of the audio file using MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - status
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  none
 *
 ******************************************************************************/
VOID  SetAudioSliderPosition( HWND hwnd )
{
    MRESULT mresult;

    /*
     * To get the length of the audio file, we will issue MCI_STATUS
     * command to the MCI. The length of the audio file will be returned
     * in the szReturn parameter of the mciSendString API, which is in the
     * SendString routine.
     */
    SendString( hwnd, "status capsamp length wait", 0 );

    /*
     * Get the audio file length.
     */
    ulAudioLength  = atol(szReturn);

    /*
     * Set the Audio position slider at the zero position.
     */
    WinSendMsg( hwndAudioSlider,
                SLM_SETSLIDERINFO,
                MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
                MPFROMSHORT( 0 ) );

    mresult = WinSendMsg( hwndAudioSlider,
                          SLM_QUERYSLIDERINFO,
                          MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
                          0 );
    sAudioArmRange = SHORT2FROMMP( (MPARAM) mresult );

    return;
}

/*************************************************************************
 * Name         :  PlayTheAudio
 *
 * Description  :  This procedure will begin the playing of an audio file.
 *                 It is invoked when the user selects the Play pushbutton
 *                 on the application's main window.
 *
 * Concepts     :  Playing a audio file using the MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - play
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
BOOL PlayTheAudio( HWND hwnd)
{


   /*
    * Seek the audio to the beginning. In this sample program we are always
    * going to play the audio from the beginning of the file.
    */
   if (!SeekTheAudio())
      return( FALSE );

   eState = ST_PLAYING;                      /* set state to PLAYING          */
   SetAudioVolume();                         /* set the starting volume       */

   /*
    * To play the audio, we will issue an MCI_PLAY command via mciSendString.
    * A MM_MCINOTIFY message will be sent to the window specified in
    * SendString when the operation is completed.
    */
   if ( SendString( hwnd, "play capsamp notify", UP_PLAY ) )
   {
      /*
       * Start the play button animation
       */
      WinSendMsg( hwndPlayPB,                 /* Play push button handle      */
                  GBM_ANIMATE,                /* Animation control            */
                  MPFROMSHORT(TRUE),          /* Animation flag               */
                  NULL );                     /* Ignore return data           */
   }

   return( TRUE );

}  /* end of PlayTheAudio */

/*************************************************************************
 * Name         :  ResumeTheAudio
 *
 * Description  :  This procedure will resume the playing of the audio
 *                 has been paused.
 *
 * Concepts     :  Resuming the audio using MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - resume
 *
 * Parameters   :  none.
 *
 * Return       :  none.
 *
 *************************************************************************/
VOID ResumeTheAudio( VOID )
{
   /*
    * To resume the audio, we will issue an MCI_RESUME command via
    * MciSendString.
    */

   if ( SendString( (HWND)NULL, "resume capsamp wait", 0 ) )
   {
      /*
       * Start the play button animation and update the internal state of
       * the dialog box.
       */
      WinSendMsg( hwndPlayPB,                    /* Play push button handle   */
                  GBM_ANIMATE,                   /* Animation control         */
                  MPFROMSHORT(TRUE),             /* Animation flag            */
                  NULL );                        /* Ignore return data        */
      eState = ST_PLAYING;
      SetAudioVolume();
   }

   return;
}  /* End of ResumeTheAudio */

/******************************************************************************
 * Name         :  PauseTheAudio
 *
 * Description  :  This procedure will pause audio that is playing.
 *
 * Concepts     :  Pausing the audio using MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - pause
 *
 * Parameters   :  none.
 *
 * Return       :  none.
 *
 ******************************************************************************/
VOID PauseTheAudio( VOID )
{

   /*
    * To pause the Audio, we will issue MCI_PAUSE command via mciSendString.
    */
   if ( SendString( (HWND)NULL, "pause capsamp wait", 0 ) )
   {
      /*
       * If pause was successful, stop the play button animation and
       * update the internal state of the dialog box.
       */
      WinSendMsg( hwndPlayPB,          /* Dialog window handle */
                  GBM_ANIMATE,         /* Animation control    */
                  MPFROMSHORT(FALSE),  /* Animation flag       */
                  NULL );              /* Ignore return data   */
      eState = ST_PAUSED;
   }

   return;
}  /* end of PauseTheAudio */

/******************************************************************************
 * Name         :  SeekTheAudio
 *
 * Description  :  This procedure will Seek to the position in an audio file.
 *                 Specified by the audio position slider on the main window.
 *
 * Concepts     :  Changing the current position in an audio file using the
 *                 MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - seek
 *
 * Parameters   :  none
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
BOOL SeekTheAudio( VOID )
{
   MRESULT mresultSlider;              /* return audio slider current position*/
   SHORT   sArmPosition;               /* Read the slider position            */
   ULONG   ulStartPos;                 /* audio stating position              */
   CHAR    szPosition[5] = "";         /* to store the starting position      */
   CHAR    szCommandString[CCHMAXPATH] =
             "seek capsamp to ";       /* string command to mciSendString     */


   /*
    * Query the slider position to determine the start of the audio play.
    */
   mresultSlider =
          WinSendMsg( hwndAudioSlider,
                      SLM_QUERYSLIDERINFO,
                      MPFROM2SHORT( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
                      0 );
   sArmPosition = SHORT1FROMMP ( (MPARAM) mresultSlider );


   if ( sArmPosition >=  ( sAudioArmRange - 1 ) )
   {
      /*
       * Slider is at the extreme right of its range, start playing from
       * the beginning.
       */
      ulStartPos = 0;
      WinSendMsg( hwndAudioSlider,
                  SLM_SETSLIDERINFO,
                  MPFROM2SHORT ( SMA_SLIDERARMPOSITION, SMA_RANGEVALUE ),
                  MPFROMSHORT  ( 0 ) );
   }
   else
      /*
       * Slider is NOT at the extreme right of its range, calculate the
       * starting position.
       */
      ulStartPos = (ULONG) ( ( sArmPosition * ulAudioLength ) / ( sAudioArmRange - 1 ) );

   _itoa(ulStartPos, szPosition, 10);
   strcat( szCommandString, szPosition);
   strcat( szCommandString, " ");
   strcat( szCommandString, "wait");

   /*
    * To seek the audio file, we will issue MCI_SEEK command to the MCI.
    */
   if (! SendString( (HWND)NULL, szCommandString, 0 ) )
      return( FALSE );

   return( TRUE );

}  /* end of SeekTheAudio */
/******************************************************************************
 * Name         :  StartTheCaption
 *
 * Description  :  This procedure will check the status of MMPM/2's captioning
 *                 flag.  If the caption flag is turned off, indicating that
 *                 user does not wish to see captioning, then application will
 *                 not invoke the Caption DLL.  If the caption flag is turned
 *                 on, then this routine will issue an API to the Caption DLL
 *                 to begin captioning.
 *
 * Concepts     :  Start captioning the file using the API's provided by the
 *                 Caption DLL.
 *
 * MMPM/2 API's :  mciQuerySysValue
 *                 mciSendString
 *                    - setpositionadvise
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  nothing.
 *
 ******************************************************************************/
VOID StartTheCaption( HWND hwnd )
{
    CC_START_PARMS   csp;             /* Closed-captioning start parameters   */
    ULONG            ulReturn;        /* Return value from ccSendCommand      */
    BOOL             bCCflag;         /* Value used in MMPM/2 caption switch. */

    /*
     * Test the MMPM/2 Closed Captioning Flag.  If it's ON then the user
     * wants to see captioning.  If it's OFF the user does not want to see
     * captioning.
     */
    mciQuerySysValue ( MSV_CLOSEDCAPTION, &bCCflag );

    if ( ! bCCflag )
    {
       /*
        * Close caption flag is OFF. The text window will not be displayed.
        * Issue a set position advise command. This will cause position
        * change messages to be sent to our window procedure.  This will
        * enable us to maintain the audio slider.
        */
       SendString(
           hwnd,
           "setpositionadvise capsamp on every 1500 notify",
           0 );
       return;
    }

    /*
     * Close caption flag is ON.
     * Fill in the CC_START_PARMS structure and the call ccSendCommand
     * to make the captioning window visible.  The hwndOwner field holds
     * the window handle that we want the Caption DLL to send the position
     * change messages to, when it is done processing them.
     */

    csp.pszDeviceName    = (PSZ)   "capsamp";        /* alias name            */
    csp.pszCaptionFile   = (PSZ)   "CAPSAMP._CC";    /* File name to use      */
    csp.hwndOwner        =  hwnd;                    /* for position change   */

    ulReturn = ccSendCommand ( CC_START, MPFROMHWND(hwndCaption), &csp );  /* Start captioning      */

    /*
     * If an error from ccSendCommand, stop the device and display an
     * error message.
     */
    if (ulReturn)
    {
       StopTheDevice();
       switch(ulReturn)
       {
          case CCERR_UNEXPECTED_EOF:
             MessageBox( IDS_UNEXPECTED_EOF,        /* ID of the message      */
                         MB_CANCEL | MB_ERROR  | MB_MOVEABLE);    /* style         */
             break;
          case CCERR_FILE_FORMAT:
             MessageBox( IDS_INVALID_FILE_FORMAT,   /* ID of the message      */
                         MB_CANCEL | MB_ERROR  | MB_MOVEABLE);    /* style         */
             break;
          case CCERR_TOO_MANY_LINES:
             MessageBox( IDS_TOO_MANY_LINES,        /* ID of the message      */
                         MB_CANCEL | MB_ERROR  | MB_MOVEABLE);    /* style         */
             break;
          case ERROR_FILE_NOT_FOUND:
          case ERROR_OPEN_FAILED:
             MessageBox( IDS_INVALID_FILE_NAME,     /* ID of the message      */
                         MB_CANCEL | MB_ERROR  | MB_MOVEABLE);    /* style         */
             break;

          default:
             StopTheDevice();
             ShowMCIErrorMessage(ulReturn);
             break;
       }
    }

    return;
}
/******************************************************************************
 * Name         :  StopTheDevice
 *
 * Description  :  This procedure will stop the device that is playing or
 *                 recording.
 *
 * Concepts     :  Stopping a device using the MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - stop
 *
 * Parameters   :  none
 *
 * Return       :  nothing.
 *
 ******************************************************************************/
VOID StopTheDevice( VOID )
{

   /*
    * To stop the device , we will issue an string command using mciSendString.
    * This stop command is done for the alias.
    */
   if (SendString( (HWND)NULL, "stop capsamp wait", 0 ) )
   {
      if (eState == ST_PLAYING)
         /*
          * Stop the play button animation
          */
         WinSendMsg( hwndPlayPB,                 /* Play push button handle   */
                     GBM_ANIMATE,                /* Animation control         */
                     MPFROMSHORT(FALSE),         /* Animation flag            */
                     NULL );                     /* Ignore return data        */
         eState = ST_STOPPED;
   }

   return;
}  /* end of StopTheDevice */


/*************************************************************************
 * Name         :  CloseTheDevice
 *
 * Description  :  This procedure will close the audio device and close the
 *                 captioning system.
 *
 * Concepts     :  Closing a device using MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - close
 *
 * Parameters   :  None.
 *
 * Return       :  nothing.
 *
 ******************************************************************************/
VOID CloseTheDevice( VOID)
{

   /*
    * To stop the device , we will issue an string command using mciSendString.
    * This stop command is done for the alias.
    */
   if ( SendString( (HWND)NULL, "close capsamp wait", 0 ) )
      /*
       * Close the captioning system. This will release all the resources that
       * were allocated.
       */
      ccTerminate(hwndCaption);

   return;

}  /* end of CloseTheDevice */


/*************************************************************************
 * Name         :  SendString
 *
 * Description  :  This procedure will send string to MCI.
 *
 * Concepts     :
 *
 * MMPM/2 API's :  mciSendString
 *
 * Parameters   :  hwnd        - window handle.
 *                 pcMCIString - string command.
 *                 usUserParm  - user parameter.
 *
 * Return       :  TRUE  - if the operation was initiated without error.
 *                 FALSE - if an error occurred.
 *
 ******************************************************************************/
BOOL  SendString( HWND hwnd, PCHAR pcMCIString, USHORT usUserParm )
{
   LONG          lmciSendStringRC;    /* return value form mciSendString     */


   lmciSendStringRC =
       mciSendString( (PSZ)pcMCIString,
                      (PSZ)szReturn,
                      CCHMAXPATH,
                      (HWND)hwnd,
                      (USHORT)usUserParm );

   if (lmciSendStringRC != 0)
   {
      ShowMCIErrorMessage(lmciSendStringRC);
      return( FALSE );
   }

   return( TRUE );
}


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
 * Parameters   :  pszFileName - The file name to be tested.
 *
 * Return       :  TRUE  -  if the a file exists matching szFilename
 *                 FALSE -  if the file does not exist
 *
 ******************************************************************************/
BOOL DoesFileExist( PSZ pszFileName )
{
   BOOL  bReturn;    /* function return value   */
   HMMIO hFile;      /* handle to file          */

 /*
  * Notice that these MMIO functions are analogous to the standard
  * C functions, fopen and fclose.
  */

   hFile = mmioOpen( pszFileName, (PMMIOINFO) NULL, MMIO_READ);

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
 * MMPM/2 API's :  mciSendString
 *                    - set
 *
 * Parameters   :  none.
 *
 * Return       :  none.
 *
 ******************************************************************************/
VOID SetAudioVolume( VOID )
{
   LONG      lmciSendStringRC;             /* return value form mciSendString */
   CHAR      szVolume[4] = "";             /* to hold the volume level        */
   CHAR      szCommandString[CCHMAXPATH] = /* string command to MCI           */
               "set capsamp audio volume ";


   if ((!fPassedDevice) && eState==ST_PLAYING)
   {
      /*
       * To set the volume,  first, build the string command to the
       * MCI.  Then an MCI_SET command should be issued to the device
       * to perform the volume change.
       */
      _itoa(sVolumeLevel, szVolume, 10);
      strcat( szCommandString, szVolume);
      strcat( szCommandString, " ");
      strcat( szCommandString, "wait");

      lmciSendStringRC =
          mciSendString( (PSZ)szCommandString   ,
                         (PSZ)szReturn,
                         CCHMAXPATH,
                         (HWND)NULL,
                         (USHORT)NULL );
      if (lmciSendStringRC != 0)
         ShowMCIErrorMessage(lmciSendStringRC);

   }
   return;
}  /* end of SetAudioVolume */

/******************************************************************************
 *  Name         : InitializeSpinButton
 *
 *  Description  : Set the initial values for the spin button controls.
 *
 *  Concepts     :
 *
 *  MMPM/2 API's :
 *
 *  Parameters   : hwnd - Handle of the Settings dialog box.
 *
 *  Returns      : none
 *
 ******************************************************************************/
VOID InitializeSpinButton( HWND hwnd )
{
   CC_STATUS_PARMS ccStatusParms;         /* status parms for CC_STATUS_PARMS */
   USHORT          ulArrayIndexValue = 0; /* return value from the spin button*/

   /*
    * Insert the values into the spin buttons.
    */
   WinSendDlgItemMsg( hwnd,                    /* Handle to the dialog box    */
                      IDC_TEXT_COLUMNS_SB,     /* ID of the spin button       */
                      SPBM_SETARRAY,           /* Set the array of data       */
                      textColumns,             /* Array of values             */
                      MPFROMLONG(3) );         /* Number of items in the array*/

   WinSendDlgItemMsg( hwnd,
                      IDC_TEXT_ROWS_SB,
                      SPBM_SETARRAY,
                      textRows,
                      MPFROMLONG(3) );

   WinSendDlgItemMsg( hwnd,
                      IDC_BACKGROUND_COLOR_SB,
                      SPBM_SETARRAY,
                      backgroundColor,
                      MPFROMLONG(3) );

   WinSendDlgItemMsg( hwnd,
                      IDC_TEXT_COLOR_SB,
                      SPBM_SETARRAY,
                      textColor,
                      MPFROMLONG(3) );

   WinSendDlgItemMsg( hwnd,
                      IDC_WINDOW_POSITION_SB,
                      SPBM_SETARRAY,
                      windowPosition,
                      MPFROMLONG(3) );
   /*
    * Query the current status of the text columns.
    * The CC_STATUS returns the actual value in the ulReturn field.
    */
   ccStatusParms.ulItem = CC_STATUS_TEXT_COLUMNS;
   ccSendCommand( CC_STATUS, MPFROMHWND(hwndCaption), &ccStatusParms );

   /*
    * Get the index value for the ulReturn field.
    */
   if (ccStatusParms.ulReturn == 15)
      ulArrayIndexValue = 0;
   else
   if (ccStatusParms.ulReturn == 35)
      ulArrayIndexValue = 1;
   else
   if (ccStatusParms.ulReturn == 50)
      ulArrayIndexValue = 2;

   /*
    * Set the current index value in the spin button.
    */
   WinSendDlgItemMsg( hwnd,                         /* Handle to the dlg box  */
                      IDC_TEXT_COLUMNS_SB,          /* ID of the spin button  */
                      SPBM_SETCURRENTVALUE,         /* Set current index value*/
                      MPFROMLONG(ulArrayIndexValue),/* Current index value    */
                      NULL );                       /* Ignore                 */
   /*
    * Query the current status of the text rows.
    * The CC_STATUS returns the actual value in the ulReturn field.
    */
   ccStatusParms.ulItem = CC_STATUS_TEXT_ROWS;
   ccSendCommand( CC_STATUS, MPFROMHWND(hwndCaption), &ccStatusParms );

   /*
    * Get the index value for the ulReturn field.
    */
   if (ccStatusParms.ulReturn == 2)
      ulArrayIndexValue = 0;
   else
   if (ccStatusParms.ulReturn == 3)
      ulArrayIndexValue = 1;
   else
   if (ccStatusParms.ulReturn == 4)
      ulArrayIndexValue = 2;

   /*
    * Set the current index value in the spin button.
    */
   WinSendDlgItemMsg( hwnd,                         /* Handle to the dlg box  */
                      IDC_TEXT_ROWS_SB,             /* ID of the spin button  */
                      SPBM_SETCURRENTVALUE,         /* Set current index value*/
                      MPFROMLONG(ulArrayIndexValue),/* Current index value    */
                      NULL );                       /* Ignore                 */
   /*
    * Query the current status of the background color.
    * The CC_STATUS returns the actual value in the ulReturn field.
    */
   ccStatusParms.ulItem = CC_STATUS_BACKGROUND_COLOR;
   ccSendCommand( CC_STATUS, MPFROMHWND(hwndCaption), &ccStatusParms );

   /*
    * Get the index value for the ulReturn field.
    */
   if (ccStatusParms.ulReturn == CLR_WHITE)
      ulArrayIndexValue = 0;
   else
   if (ccStatusParms.ulReturn == CLR_YELLOW)
      ulArrayIndexValue = 1;
   else
   if (ccStatusParms.ulReturn == CLR_DARKGRAY)
      ulArrayIndexValue = 2;

   /*
    * Set the current index value in the spin button.
    */
   WinSendDlgItemMsg( hwnd,                         /* Handle to the dlg box  */
                      IDC_BACKGROUND_COLOR_SB,      /* ID of the spin button  */
                      SPBM_SETCURRENTVALUE,         /* Set current index value*/
                      MPFROMLONG(ulArrayIndexValue),/* Current index value    */
                      NULL );                       /* Ignore                 */
   /*
    * Query the current status of the text color.
    * The CC_STATUS returns the actual value in the ulReturn field.
    */
   ccStatusParms.ulItem = CC_STATUS_TEXT_COLOR;
   ccSendCommand( CC_STATUS, MPFROMHWND(hwndCaption), &ccStatusParms );

   /*
    * Get the index value for the ulReturn field.
    */
   if (ccStatusParms.ulReturn == CLR_DARKBLUE)
      ulArrayIndexValue = 0;
   else
   if (ccStatusParms.ulReturn == CLR_BLACK)
      ulArrayIndexValue = 1;
   else
   if (ccStatusParms.ulReturn == CLR_RED)
      ulArrayIndexValue = 2;

   /*
    * Set the current index value in the spin button.
    */
   WinSendDlgItemMsg( hwnd,                         /* Handle to the dlg box  */
                      IDC_TEXT_COLOR_SB,            /* ID of the spin button  */
                      SPBM_SETCURRENTVALUE,         /* Set current index value*/
                      MPFROMLONG(ulArrayIndexValue),/* Current index value    */
                      NULL );                       /* Ignore                 */
   /*
    * Query the current window position.
    * The CC_STATUS returns the actual value in the ulReturn field.
    */
   ccStatusParms.ulItem = CC_STATUS_X_POSITION;
   ccSendCommand( CC_STATUS, MPFROMHWND(hwndCaption), &ccStatusParms );

   /*
    * Get the index value for the ulReturn field.
    */
   if ((ccStatusParms.ulReturn==(rclMain.xRight - fm.lAveCharWidth * 15)/2) ||
       (ccStatusParms.ulReturn==(rclMain.xRight - fm.lAveCharWidth * 35)/2) ||
       (ccStatusParms.ulReturn==(rclMain.xRight - fm.lAveCharWidth * 50)/2) ||
       (ccStatusParms.ulReturn == (ULONG) 0 ))
      ulArrayIndexValue = 0;                            /* Lower Left         */
   else
   if (ccStatusParms.ulReturn == 5)
      ulArrayIndexValue = 1;                            /* Lower Center       */
   else
      ulArrayIndexValue = 2;                            /* Lower Right        */

   /*
    * Set the current index value in the spin button.
    */
   WinSendDlgItemMsg( hwnd,                         /* Handle to the dlg box  */
                      IDC_WINDOW_POSITION_SB,       /* ID of the spin button  */
                      SPBM_SETCURRENTVALUE,         /* Set current index value*/
                      MPFROMLONG(ulArrayIndexValue),/* Current index value    */
                      NULL );                       /* Ignore                 */
   return;
}

/*************************************************************************
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
 *************************************************************************/
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

/*******************************************************************************
 * Name        : ProcuctInfoDlgProc
 *
 * Description : This function controls the product information dialog box.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: WinDestroySecondaryWindow
 *               WinDefSecondaryWindowProc
 *               WinInsertDefaultSize
 *
 * Parameters  : hwnd - Handle for the Include dialog box.
 *               msg  - Message received by the dialog box.
 *               mp1  - Parameter 1 for the just received message.
 *               mp2  - Parameter 2 for the just received message.
 *
 * Return      : 0 or the result of default processing.
 *
 ******************************************************************************/
MRESULT EXPENTRY ProductInfoDlgProc( HWND   hwnd,
                                     ULONG  msg,
                                     MPARAM mp1,
                                     MPARAM mp2 )
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
         hwndProductInfo = (HWND) 0;
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

} /* End of ProductInfoDlgProc */
