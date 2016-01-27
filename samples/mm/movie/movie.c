/*************************************************************************
 * File Name    :  movie.c
 *
 * Description  :  This file contains the C source code required for the
 *                 movie sample program.
 *
 * Concepts     :  The sample program illustrates how an application
 *                 can play and edit a movie in a hardware independent manner.
 *
 *                 This sample application will demonstrate device control
 *                 of a Software Motion Video device, playing and editing of
 *                 a movie in an application specified window and in a window
 *                 provided by the Softare Motion Video subsystem.
 *
 * MMPM/2 API's :  List of all MMPM/2 API's that are used in
 *                 this module:
 *
 *                 mciSendCommand    MCI_OPEN
 *                                   MCI_PLAY
 *                                   MCI_PAUSE
 *                                   MCI_RESUME
 *                                   MCI_SEEK
 *                                   MCI_CLOSE
 *                                   MCI_SET
 *                                   MCI_WINDOW
 *                                   MCI_PUT
 *                                   MCI_CUT
 *                                   MCI_COPY
 *                                   MCI_PASTE
 *                  mmioGetHeader
 *                  mmioSet
 *                  mmioOpen
 *                  mmioClose
 *
 * Required
 *    Files     :  movie.c        Source Code.
 *                 movie.h        Include file.
 *                 movie.dlg      Dialog definition.
 *                 movie.rc       Resources.
 *                 movie.mak      Make file.
 *                 movie.def      Linker definition file.
 *                 movie.ico      Program icon.
 *                 movie.wav      Program icon.
 *
 * Copyright (C) IBM 1993
 *************************************************************************/

#define  INCL_WIN                   /* required to use Win APIs.           */
#define  INCL_PM                    /* required to use PM APIs.            */
#define  INCL_WINHELP               /* required to use IPF.                */
#define  INCL_OS2MM                 /* required for MCI and MMIO headers   */
#define  INCL_MMIOOS2               /* required for MMVIDEOHEADER          */
#define  INCL_MMIO_CODEC            /* required for circular, secondary    */
#define  INCL_SW                    /* required for circular, secondary    */
                                    /* windows and graphic buttons         */
#define  INCL_WINSTDFILE            /* required for open file dlg          */
#define  INCL_WINSTDDLGS            /* required for open file dlg          */


#include <os2.h>
#include <os2me.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dive.h>

#include <sw.h>


#include "movie.h"

enum DeviceStates {ST_STOPPED, ST_PLAYING, ST_PAUSED, ST_REWIND};
/*
 * Procedure/Function Prototypes
 */

MRESULT EXPENTRY MainDlgProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );

BOOL    DoesFileExist( PSZ pszFileName );
VOID    Finalize( VOID );
BOOL    Initialize( VOID );
VOID    InitializeHelp( VOID );
USHORT  MessageBox( USHORT usMessageID, ULONG  ulStyle );
BOOL    PlayTheAppFile( HWND hwnd );
BOOL    PlayTheDefaultFile( HWND hwnd );
BOOL    PlayTheMovie( HWND hwnd );
VOID    SetMovieVolume( VOID );
VOID    ShowMCIErrorMessage( ULONG ulError );
VOID    StopTheDevice( VOID );
VOID    ResumeTheMovie( HWND hwnd );
VOID    PauseTheMovie( HWND hwnd );
VOID    CloseTheDevice( VOID );
BOOL    OpenTheDevice( HWND hwnd);
VOID    EditTheMovie( HWND hwnd );
BOOL    SendString( HWND hwnd, PCHAR pcMCIString, USHORT usUserParm );
VOID    ResizeMovieWindow(HWND hwnd);
VOID    InsertHelpMenuItem (PCHAR );
BOOL    QueryDirectMode(VOID);

/*************** End of Procedure/Function Prototypes *************************/

  /********************/
  /* Global Variables.*/
  /********************/
HAB    hab;
HMQ    hmq;
HWND   hwndFrame;                      /* Handle to the frame window.         */
HWND   hwndClient;                     /* Handle to the client App window     */
HWND   hwndAppFrame;                   /* Handle to the App frame window      */
HWND   hwndHelpInstance;               /* Handle to Main Help window.         */
HWND   hwndMainDialogBox;              /* Handle to the dialog window.        */
HWND   hwndPlayPB;                     /* Handle to the play push button      */
HWND   hwndRewindPB;                   /* Handle to the rewind push button    */
HWND   hwndStopPB;                     /* Handle to the stop push button      */
HWND   hwndPausePB;                    /* Handle to the pause push button     */
HWND   hwndEditPB;                     /* Handle to the edit push button      */
HWND   hwndApplication;                /* Handle to the Application RB        */
HWND   hwndStandard;                   /* Handle to the Standard RB           */
HWND   hwndVolumeSlider;               /* Handle to the Volume slider         */
ULONG  ulMovieLength;                  /* Length of movie in mmtimeperframe   */
ULONG  ulMovieWidth;                   /* Width of movie                      */
ULONG  ulMovieHeight;                  /* Height of movie                     */
SWP    swpAppFrame;
HMMIO  hFile;                          /* handle to file                      */

enum   DeviceStates   eState = ST_STOPPED;      /* state of the device        */

SHORT  sVolumeLevel   = INIT_VOLUME;   /* desired volume level                */
BOOL   fAudioOn       = FALSE;         /* Indicates that sound is supported   */
BOOL   fPassedDevice  = FALSE;         /* for MCI_ACQUIRE to play the file    */
BOOL   fDeviceOpen    = FALSE;         /* indicate we've opened for first time*/
BOOL   fMovieLoaded   = FALSE;         /* indicate if we are loaded           */
CHAR   achMsgBoxTitle[MAXNAMEL];       /* Error message box title             */
CHAR   achTitle[MAXNAMEL];             /* buffer for window title text      */

CHAR   szReturn[CCHMAXPATH];           /* return value from mciSendString     */
SHORT  sResizeWindow = 0;              /* flag set for resizing the widow     */
SHORT  sKindOfPlay = 0;                /* flag set for which movie is playing */
HPOINTER  hptrWait, hptrArrow;  /* Pointer handles for use during long waits. */

/************************** End of Global Variables ***************************/

/******************************************************************************
 * Name         : main
 *
 * Description  : This function calls the Initialize procedure to prepare
 *                everything for the program's operation, enters the
 *                message loop, then calls Finalize to shut everything down
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



   if( !Initialize() )
   {
   while ( WinGetMsg( hab, (PQMSG) &qmsg, (HWND) NULL, 0, 0) )
      WinDispatchMsg( hab, (PQMSG) &qmsg );

   Finalize();
   }

   return( TRUE);

} /* End of main */

/******************************************************************************
 * Name         : Initialize
 *
 * Description  : This function performs the necessary initializations and
 *                setups that are required to show/run a secondary window
 *                as a main window.  The message queue will be created, as will
 *                the dialog box.
 *
 * Concepts     : Illustrates how to use secondary windows
 *
 * MMPM/2 API's : WinLoadSecondaryWindow
 *
 * Parameters   : None.
 *
 * Return       : BOOL, FALSE = success, TRUE = fail
 *
 ******************************************************************************/
BOOL Initialize( VOID)
{
   CHAR     szHelp[CCHMAXPATH];          /* buffer for help menu text         */
   CHAR     szDefaultSize[CCHMAXPATH];   /* buffer for default size menu text */
   LONG     lmciSendStringRC;            /* return code from SendString       */
   LONG     scrwidth,scrheight;          /* Width and height of screen        */
   SWP      swpSEFrame;          /* struc containing information on Frame     */



   hab       = WinInitialize( 0);
   hmq       = WinCreateMsgQueue( hab, 0);


   /*****************************************************************/
   /* Make sure that Direct Video is supported                      */
   /*****************************************************************/

   if (!QueryDirectMode())
   {
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                    (PSZ)"Direct software motion video mode is not supported on this system.",
                    (PSZ)"Ultieyes - Direct Mode Support Detection",
                    0,
                    MB_CANCEL | MB_ERROR | MB_MOVEABLE );

      return( TRUE );
   }




   hptrArrow = WinQuerySysPointer ( HWND_DESKTOP, SPTR_ARROW, FALSE );
   hptrWait  = WinQuerySysPointer ( HWND_DESKTOP, SPTR_WAIT,  FALSE );

   /*****************************************************************/
   /* Initialize the window. First, change pointer to a waiting     */
   /* pointer, since this might take a couple of seconds.           */
   /*****************************************************************/

   WinSetPointer ( HWND_DESKTOP, hptrWait );

   WinLoadString(
      hab,                                  /* HAB for this window.           */
      (HMODULE) NULL,                       /* Get the string from the .exe.  */
      IDS_DEFAULTSIZE,                      /* Which string to get.           */
      sizeof(szDefaultSize),                /* The size of the buffer.        */
      szDefaultSize);                       /* The buffer to place the string.*/

   WinLoadString(
      hab,                                  /* HAB for this window.           */
      (HMODULE) NULL,                       /* Get the string from the .exe.  */
      IDS_HELP,                             /* Which string to get.           */
      sizeof(szHelp),                       /* The size of the buffer.        */
      szHelp);                              /* The buffer to place the string.*/

   WinLoadString(
      hab,                                  /* HAB for this dialog box.       */
      (HMODULE) NULL,                       /* Get the string from the .exe.  */
      IDS_MOVIE_ERROR,                      /* Which string to get.           */
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

   /***************************************************************************/
   /* Retrieve the handle to the dialog box by specifying the QS_DIALOG flag. */
   /***************************************************************************/

   hwndMainDialogBox = WinQuerySecondaryHWND(hwndFrame, QS_DIALOG);

   /***************************************************************************/
   /* Add Default Size menu item to system menu of the secondary window.      */
   /***************************************************************************/

   WinInsertDefaultSize(hwndFrame, szDefaultSize);

   /***************************************************************************/
   /* Add Help menu item to system menu of the secondary window.              */
   /***************************************************************************/

   InsertHelpMenuItem(szHelp);

   /******************************************************************/
   /* Get the window title string from the Resource string table     */
   /* and set it as the window text for the dialog window.           */
   /******************************************************************/
   WinLoadString(
      hab,                                  /* HAB for this dialog box.       */
      (HMODULE) NULL,                       /* Get the string from the .exe.  */
      IDS_PROGRAM_TITLE,                    /* Which string to get.           */
      (SHORT) sizeof( achTitle),            /* The size of the buffer.        */
      achTitle);                            /* The buffer to place the string.*/

   WinSetWindowText( hwndFrame, achTitle);


   /******************************************************************/
   /* Get the system values for screen width and height and then     */
   /* place the main window appropriately(centered on the screen).   */
   /******************************************************************/

   WinQueryWindowPos( HWND_DESKTOP, &swpSEFrame );
   scrwidth  = swpSEFrame.cx;
   scrheight = swpSEFrame.cy;

   WinQueryWindowPos( hwndFrame, &swpSEFrame );
   scrwidth  = ceil((scrwidth - swpSEFrame.cx)/2);
   scrheight = ceil((scrheight - swpSEFrame.cy)/2);

   WinSetWindowPos(hwndFrame,
                   HWND_TOP,
                   scrwidth,
                   scrheight,
                   swpSEFrame.cx,
                   swpSEFrame.cy,
                   SWP_SIZE | SWP_MOVE);

  /*************************************************************************/
  /* Create a new window within our secondary window to play the movie.    */
  /* When playing a movie in this window the Sendstring                    */
  /* function will be passed this windows handle in order to know where    */
  /* to put the movie before playing.                                      */
  /*************************************************************************/

    WinRegisterClass(
      hab,
    "MovieWindow",
    (PFNWP) NULL,
    CS_SIZEREDRAW | CS_MOVENOTIFY,
    0 );

  hwndAppFrame =  WinCreateWindow((HWND)hwndMainDialogBox,
                                 "MovieWindow",
                                  NULL,
                                  WS_VISIBLE,
                                  0,
                                  0,
                                  0,
                                  0,
                                  (HWND)hwndMainDialogBox,
                                  HWND_TOP, ID_APPWINDOW, NULL, NULL );

   WinShowWindow( hwndFrame, TRUE );           /* Display the main window.    */

   InitializeHelp();                           /* Initialize the help.        */

   /*
    * Now that we're done here, change the pointer back to the arrow.
    */
   WinSetPointer ( HWND_DESKTOP, hptrArrow );

   return( FALSE );

} /* end initialization */

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
   SHORT       sWmControlID;          /* WM_CONTROL id                        */
   ULONG       ulFrame;               /* video position in frames.            */
   MRESULT     mresult;               /* return result                        */

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

         /*********************************/
         /* get all the required handles. */
         /*********************************/
                                                         /* volume slider     */
         hwndVolumeSlider = WinWindowFromID( hwnd, IDC_VOLUME_SLIDER );

                                                         /* play push button  */

         hwndPlayPB      = WinWindowFromID( hwnd, IDC_GPB_PLAY );
         hwndRewindPB    = WinWindowFromID( hwnd, IDC_GPB_REWIND );
         hwndStopPB      = WinWindowFromID( hwnd, IDC_GPB_STOP);
         hwndPausePB     = WinWindowFromID( hwnd, IDC_GPB_PAUSE );

         hwndEditPB      = WinWindowFromID( hwnd, IDC_GPB_EDIT );

                                                         /* application RB    */
         hwndApplication = WinWindowFromID( hwnd, IDC_RADIO1 );

         if (!DoesFileExist( "movie.avi" ))
          {
             MessageBox( IDS_CANNOT_FIND_MOVIE_FILE,     /* ID of the message */
                         MB_CANCEL | MB_ERROR  | MB_MOVEABLE);    /* style    */

             WinEnableWindow( hwndPlayPB, FALSE );
             WinEnableWindow( hwndEditPB, FALSE );
          }


         /********************************************************************/
         /*  Check to see if we have audio support                           */
         /*                                                                  */
         /********************************************************************/

         lmciSendStringRC =
               SendString(hwnd, "sysinfo all name 1", 0);

         if ( strstr( szReturn, "Waveaudio" ) )
         {
            fAudioOn = TRUE;

            /********************************************************************/
            /* The slider control cannot be completely set from the dialog      */
            /* template so some aspects must be set here.  We will set the      */
            /* volume range to 0-100, increment 1-10, and set the initial       */
            /* volume level to 100.                                             */
            /********************************************************************/
            WinSendMsg( hwndVolumeSlider,
                        CSM_SETRANGE,
                        (MPARAM) 0L,
                        (MPARAM) 100L);

            WinSendMsg( hwndVolumeSlider,
                        CSM_SETINCREMENT,
                        (MPARAM) 10L,
                        (MPARAM) 1L);

            WinSendMsg( hwndVolumeSlider,
                        CSM_SETVALUE,
                        (MPARAM) sVolumeLevel,
                        (MPARAM) NULL);
         }

            /*****************/
            /* Volume Slider */
            /*****************/

            WinEnableWindow( hwndVolumeSlider, TRUE );

         /**********************************************/
         /* Set the application window as the default  */
         /**********************************************/
         WinSendMsg(hwndApplication, BM_SETCHECK, MPFROMSHORT(1), NULL);

         return( (MRESULT) 0);

      case WM_CLOSE :

         /*********************************************/
         /* If the device is opened, close the device */
         /*********************************************/

         if (fDeviceOpen)
            CloseTheDevice();

         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );


      /*************************************************************/
      /* This message is recieved when the user changes the volume */
      /* or the radio buttons.                                     */
      /*************************************************************/

      case WM_CONTROL:
        sWmControlID  = SHORT1FROMMP(mp1);
        usNotifyCode  = (USHORT) SHORT2FROMMP(mp1);

        switch ( sWmControlID )
        {

           case IDC_VOLUME_SLIDER:
              if ( ( usNotifyCode == CSN_CHANGED ) ||
                   ( usNotifyCode == CSN_TRACKING ) )
              {

               /**************************************************************/
               /*  Every time the volume control setting is changed, save the*/
               /*  new value in sVolumeLevel.                                */
               /**************************************************************/

                 sVolumeLevel = SHORT1FROMMP (mp2);

                /*****************************/
                /*  Set the new volume level */
                /*****************************/

                 SetMovieVolume();
              }
              break;

           case IDC_RADIO1:
                    sKindOfPlay = 0;          /* play in application window */
                break;

           case IDC_RADIO2:
                    sKindOfPlay = 1;          /* play in the default window */
                    break;


        }

        return( (MRESULT) 0);

      case WM_COMMAND :

        /**********************************************************************/
        /* To get which pushbutton was pressed the SHORT1FROMMP macro is used.*/
        /**********************************************************************/

         switch (SHORT1FROMMP(mp1))
         {
            case IDC_GPB_PLAY:               /* user selected "Play"        */

               switch ( eState )
               {
                  /**********************************************************/
                  /* If the Movie is currently stopped, check to see if we  */
                  /* were playing. if we were then resume.                  */
                  /**********************************************************/
                  case ST_STOPPED:

                    /****************************************************/
                    /* Open the movie, resize the window if needed for  */
                    /* the movie, then start playing.                   */
                    /****************************************************/
                     if (!fDeviceOpen)
                       {
                         OpenTheDevice ( hwnd );
                       } /* if */
                     if(sResizeWindow == 0 )
                       {
                         ResizeMovieWindow(hwnd);
                         sResizeWindow = 1;
                       }
                     PlayTheMovie( hwnd );
                     break;

                  /******************************************************/
                  /* If the Movie is currently paused, resume the Movie */
                  /******************************************************/
                  case ST_PAUSED:
                     ResumeTheMovie(hwnd);
                     break;

                  case ST_REWIND:
                     PlayTheMovie(hwnd);
                     break;
               }
               break;

            case IDC_GPB_PAUSE:
               switch ( eState )
               {
                 /************************************************/
                 /* If the Movie is currently playing, pause it. */
                 /************************************************/

                 case ST_PLAYING:
                    PauseTheMovie(hwnd);
                    break;

                 /***********************************************************/
                 /* If the Movie is currently paused, resume the Movie play.*/
                 /***********************************************************/

                 case ST_PAUSED:
                    ResumeTheMovie(hwnd);
                    break;
               }
              break;

            case IDC_GPB_REWIND:         /* User selected "Rewind" pushbutton */
               if (fDeviceOpen)          /* If the device is opened.          */
                 {
                  eState = ST_REWIND;
                  lmciSendStringRC =
                            SendString(hwnd, "seek movie to start notify", 0);
                 }
               break;

            case IDC_GPB_EDIT:           /* User selected "Edit" pushbutton. */
                 {
                 MessageBox( IDS_EDIT_MOVIE,
                             MB_OK | MB_INFORMATION  | MB_MOVEABLE);
                 if (!fDeviceOpen)     /* check to see if the device is open.*/
                   {
                     OpenTheDevice ( hwnd );
                   } /* if */
                                       /* Stop the device if we are in a */
                                       /* playing or paused state.       */
                 if ((eState == ST_PLAYING) || (eState == ST_PAUSED))
                   {
                     StopTheDevice();
                   } /* if */
                  /*****************************************************/
                  /* This function illustrates the cut, copy and paste */
                  /* functionality.                                    */
                  /*****************************************************/
                  EditTheMovie(hwnd);

                 }
               break;

            case IDC_GPB_STOP:         /* user selected "Stop" pushbutton */

               /***********************************************************/
               /* If the Movie is not in stopped state, stop the device.  */
               /***********************************************************/

               if (eState != ST_STOPPED)
                 {
                    StopTheDevice();
                 }
               break;

            case WM_HELP:      /* User selected "Help" from System menu   */

       /*********************************************************************/
       /* The dialog window has received a request for help from the user.  */
       /* Send the HM_DISPLAY_HELP message to the Help Instance with the IPF*/
       /* resource identifier for the correct HM panel.  This will show the */
       /* help panel for this sample program.                               */
       /*********************************************************************/

         WinSendMsg( hwndHelpInstance,
                     HM_DISPLAY_HELP,
                     MPFROMSHORT(1),
                     MPFROMSHORT(HM_RESOURCEID));

               break;

            default:
               break;

         }  /* End of Command Switch */

         return( (MRESULT) 0);

      /**********************************************************************/
      /* The next two messages are handled so that this application can     */
      /* participate in device sharing.  Since it opens the device as a     */
      /* shareable device, other applications can gain control of the       */
      /* device.  When this happens, we will receive a MM_MCIPASSDEVICE     */
      /* message.  We keep track of this device passing in the              */
      /* fPassedDevice Boolean variable.                                    */
      /*                                                                    */
      /* We use the WM_ACTIVATE message to participate in device            */
      /* sharing.  We first check to see if this is an activate             */
      /* or a deactivate message (indicated by mp1).  Then,                 */
      /* we check to see if we've passed control of the                     */
      /* devices.  If these conditions are true, then                       */
      /* we issue an acquire device command to regain control of            */
      /* the device, since we're now the active window on the screen.       */
      /*                                                                    */
      /* This is one possible method that can be used to implement          */
      /* device sharing. For applications that are more complex             */
      /* than this sample program, developers may wish to take              */
      /* advantage of a more robust method of device sharing.               */
      /* This can be done by using the MCI_ACQUIRE_QUEUE flag on            */
      /* the MCI_ACQUIREDEVICE command.  Please refer to the MMPM/2         */
      /* documentation for more information on this flag.                   */
      /**********************************************************************/

      case MM_MCIPASSDEVICE:
         if (SHORT1FROMMP(mp2) == MCI_GAINING_USE)           /* GAINING USE */
           {
             fPassedDevice = FALSE;            /* Gaining control of device.*/
           }
         else                                               /* Losing use   */
           {
             fPassedDevice = TRUE;             /* Losing  control of device.*/
           }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case WM_ACTIVATE:

       /*********************************************************************/
       /* We use the WM_ACTIVATE message to participate in device sharing.  */
       /* We first check to see if this is an activate or a deactivate      */
       /* message (indicated by mp1).  Then, we check to see if we've passed*/
       /* control of the Movie device.  If these conditions are true,       */
       /* then we issue an acquire device command to regain control of      */
       /* the device, since we're now the active window on the screen.      */
       /*********************************************************************/
         if ((BOOL)mp1 && fPassedDevice == TRUE)
         {
           /***********************************************************/
           /* To acquire the device, we will issue MCI_ACQUIREDEVICE  */
           /* command to the MCI.                                     */
           /***********************************************************/

            SendString( hwnd, "acquire movie notify", 0 );
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case MM_MCINOTIFY:

       /***********************************************************************/
       /* This message is returned to an application when a device            */
       /* successfully completes a command that was issued with a NOTIFY      */
       /* flag, or when an error occurs with the command.                     */
       /*                                                                     */
       /* This message returns two values. A user parameter (mp1) and         */
       /* the command message (mp2) that was issued. The low word of mp1      */
       /* is the Notification Message Code which indicates the status of the  */
       /* command like success or failure. The high word of mp2 is the        */
       /* Command Message which indicates the source of the command.          */
       /***********************************************************************/

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

                  /********************************************************/
                  /* This case is used for either successful completion   */
                  /* of a command or for an error.                        */
                  /********************************************************/

                  default:
                   if (eState != ST_STOPPED)
                     {
                        eState = ST_STOPPED;
                        if ( usNotifyCode != MCI_NOTIFY_SUCCESSFUL)

                           /*************************************************/
                           /* Notification error message. We need to display*/
                           /* the error message to the user.                */
                           /*************************************************/

                           ShowMCIErrorMessage( usNotifyCode);
                           fMovieLoaded = FALSE;

                     }

                    break;
               }
               break;
         }
         return( (MRESULT) 0);

      default:
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2));

   }  /* End of msg Switch */

} /* End of MainDlgProc */

/******************************************************************************
 * Name         :  PlayTheAppFile
 *
 * Description  :  This procedure will load a specific movie file into the
 *                 application defined video window.
 *
 * Concepts     :  - Playing a movie in a application defined window.
 *                   in a device independent manner.
 *                 - Loading a file into an already open device.
 *
 * MMPM/2 API's :  mciSendString
 *                    - load
 *                    - put
 *                    - window handle
 *
 * Parameters   :  hwnd  - Handle for the main window.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
BOOL PlayTheAppFile( HWND hwnd )
{
   LONG    lmciSendStringRC;         /* return code from SendString           */

   CHAR    szHandle[12] = "";        /* string used for window handle         */
   CHAR    szx[5]       = "";        /* string used for x position of window  */
   CHAR    szy[5]       = "";        /* string used for y position of window  */
   CHAR    szcx[5]      = "";        /* string used for cx position of window */
   CHAR    szcy[5]      = "";        /* string used for cy position of window */

   /*******************************************************************/
   /* The szWindowString and szPutString are used as a foundation     */
   /* for building a string command to send to sendstring             */
   /*******************************************************************/

   CHAR    szWindowString[CCHMAXPATH] =
             "window movie handle ";      /* string command to mciSendString */

   CHAR    szPutString[CCHMAXPATH] =
            "put movie destination at ";  /* string command to mciSendString */

   /**********************************************************************/
   /* Change pointer to a waiting pointer first, since this might take a */
   /* couple of seconds.                                                 */
   /**********************************************************************/

   WinSetPointer( HWND_DESKTOP, hptrWait );

   /******************************************************/
   /* Convert the Frame window handle to a string so     */
   /* we can use the mciSendStringCommand.               */
   /******************************************************/

   hwndAppFrame    = WinWindowFromID( hwnd, ID_APPWINDOW );

   _ultoa(hwndAppFrame,szHandle,10);


   /*****************************************/
   /* Concatenate the converted handle to   */
   /* the window string so we can issue the */
   /* send string command.                  */
   /*****************************************/
   strcat (szWindowString, szHandle);
   strcat (szWindowString, " ");
   strcat (szWindowString, "wait");

   /* Send the window handle to the alias window */
   lmciSendStringRC = SendString(hwnd, szWindowString, 0);

   /* Load the movie if it needs to be loaded */

   if (!fMovieLoaded)
     {
       lmciSendStringRC = SendString(hwnd, "load movie movie.avi wait", 0);
       if ( lmciSendStringRC )
         {
           fMovieLoaded = TRUE;
         }
       else
         {
           MessageBox( IDS_CANNOT_LOAD_MOVIE_FILE,
                       MB_CANCEL | MB_ERROR  | MB_MOVEABLE);
           return( FALSE );
         } /* else */

     } /* if */

   /******************************************************/
   /* Convert the frame windows sizes to strings so      */
   /* we can use the mciSendStringCommand to put the     */
   /* video in our application frame window.             */
   /******************************************************/

     WinQueryWindowPos (hwndAppFrame, &swpAppFrame);

       swpAppFrame.x = 0;
       swpAppFrame.y = 0;

     /**********************************************************/
     /* convert the application windows coordinates to strings */
     /* so we can issue the the sendstring command             */
     /**********************************************************/
     _ultoa(swpAppFrame.x,szx,10);
     _ultoa(swpAppFrame.y,szy,10);
     _ultoa(swpAppFrame.cx,szcx,10);
     _ultoa(swpAppFrame.cy,szcy,10);

     /**********************************************************/
     /* concatenate the converted strings to the the put string*/
     /* so we can issue the the sendstring command             */
     /**********************************************************/

     strcat (szPutString, szx);
     strcat (szPutString, " ");
     strcat (szPutString, szy);
     strcat (szPutString, " ");
     strcat (szPutString, szcx);
     strcat (szPutString, " ");
     strcat (szPutString, szcy);
     strcat (szPutString, " ");
     strcat (szPutString, "wait");

   /* Send the put command along with the window coordinates. */

     lmciSendStringRC = SendString( hwnd, szPutString, 0 );

   /******************************************************************/
   /* Now that we're done here, change the pointer back to the arrow.*/
   /******************************************************************/
   WinSetPointer ( HWND_DESKTOP, hptrArrow );

   return( TRUE );

}  /* end of PlayTheAppFile */

/******************************************************************************
 * Name         :  PlayTheDefaultFile
 *
 * Description  :  This procedure will load a specific movie file into
 *                 the applications default video window.  Default video
 *                 window will appear in the lower left origin of the screen.
 *
 *
 * Concepts     :  - Playing a movie in the default window in a device
 *                   independent manner.
 *                 - Loading a file into an already open device.
 *
 * MMPM/2 API's :  mciSendString
 *                    - load
 *
 * Parameters   :  hwnd  - Handle for the Main dialog box.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
BOOL PlayTheDefaultFile( HWND hwnd )
{
   LONG    lmciSendStringRC;            /* return code from SendString   */

   /**********************************************************************/
   /* Change pointer to a waiting pointer first, since this might take a */
   /* couple of seconds.                                                 */
   /**********************************************************************/

   WinSetPointer( HWND_DESKTOP, hptrWait );

   if (!fMovieLoaded)
     {
       lmciSendStringRC = SendString(hwnd, "load movie movie.avi wait", 0);
       if (lmciSendStringRC)
         {
           fMovieLoaded = TRUE;
         }
       else
         {
           MessageBox( IDS_CANNOT_LOAD_MOVIE_FILE,
                       MB_CANCEL | MB_ERROR  | MB_MOVEABLE);
           return( FALSE );
         } /* else */
     } /* if */


   /* Send the window command to play the movie in the default window. */
   SendString(hwnd,"window movie handle default wait", 0 );

   /******************************************************************/
   /* Now that we're done here, change the pointer back to the arrow.*/
   /******************************************************************/
   WinSetPointer ( HWND_DESKTOP, hptrArrow );

   return( TRUE );

}  /* end of PlayTheDefaultFile */

/*************************************************************************
 * Name         :  PlayTheMovie
 *
 * Description  :  This procedure will begin playing a movie file.
 *                 It is invoked when the user selects the Play pushbutton
 *                 on the application's main window.  The movie will be
 *                 played in either a application defined window or in a
 *                 default window depending on the state of the radio buttons.
 *
 *
 * Concepts     :  Playing a movie file using the MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - play
 *
 * Parameters   :  hwnd - Handle for the main dialog box.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 ******************************************************************************/
BOOL PlayTheMovie( HWND hwnd)
{

   /*******************************************/
   /* Check to see which window we want to    */
   /* play the movie in.                      */
   /*******************************************/

   if (WinSendMsg(hwndApplication, BM_QUERYCHECK, NULL,NULL ))
     {
       /* Play the movie in the application window */
       if (!PlayTheAppFile(hwnd))
         {
           return (FALSE);
         }

     } /* if */
   else
     {
       /* Play the movie in the default window */
       if (!PlayTheDefaultFile(hwnd))
         {
           return( FALSE );
         } /* if */
     }

   eState = ST_PLAYING;                      /* set state to PLAYING         */

   SetMovieVolume();                         /* set the starting volume      */

   /**************************************************************************/
   /* To play the movie, we will issue an MCI_PLAY command via mciSendString.*/
   /* A MM_MCINOTIFY message will be sent to the window specified in         */
   /* SendString when the operation is completed.                            */
   /**************************************************************************/
   SendString( hwnd,"play movie notify", 0 );

   return( TRUE );

}  /* end of PlayTheMovie */

/*************************************************************************
 * Name         :  ResumeTheMovie
 *
 * Description  :  This procedure will resume the playing of the movie
 *                 once it has been paused.
 *
 * Concepts     :  Resuming the movie using MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - resume
 *
 * Parameters   :  hwnd - window handle
 *
 * Return       :  none.
 *
 *************************************************************************/
VOID ResumeTheMovie( HWND hwnd)
{
     /*********************************************/
     /* Check to see which pushbutton is selected.*/
     /*********************************************/

      if (!WinSendMsg(hwndApplication, BM_QUERYCHECK, NULL, NULL))
        {
          PlayTheDefaultFile(hwnd);
        } /* if */
      else
        {
          PlayTheAppFile(hwnd);
        } /* else */

   /*******************************************************************/
   /* To resume the movie, we will issue an MCI_RESUME command via    */
   /* MciSendString.                                                  */
   /*******************************************************************/
   if (SendString( (HWND)NULL, "resume movie wait", 0 ) )
     {
       eState = ST_PLAYING;
       SetMovieVolume();
     }

   return;
}  /* End of ResumeTheMovie */

/******************************************************************************
 * Name         :  PauseTheMovie
 *
 * Description  :  This procedure will pause the playing of the movie that
 *                 that is currently playing.
 *
 * Concepts     :  Pausing the movie using MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - pause
 *
 * Parameters   :  hwnd - window handle
 *
 * Return       :  none.
 *
 ******************************************************************************/
VOID PauseTheMovie( HWND hwnd)
{

  USHORT sCheckState;

   /*******************************************************************/
   /* To pause the movie, we will issue an MCI_PAUSE command via      */
   /* MciSendString.                                                  */
   /*******************************************************************/

   if ( SendString( (HWND)NULL, "pause movie wait", 0 ) )
    {
       eState = ST_PAUSED;
    }

   return;
}  /* end of PauseTheMovie */

/******************************************************************************
 * Name         :  StopTheDevice
 *
 * Description  :  This procedure will stop the device that is playing.
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
 *****************************************************************************/
VOID StopTheDevice( VOID )
{

 /****************************************************************************/
 /* To stop the device , we will issue an string command using mciSendString.*/
 /* This stop command is done for the alias.                                 */
 /****************************************************************************/

   if (SendString( (HWND)NULL, "stop movie wait", 0 ) )
   {
      if (eState == ST_PLAYING)
        {
          eState = ST_STOPPED;
        }
   }

   return;
}  /* end of StopTheDevice */


/*************************************************************************
 * Name         :  CloseTheDevice
 *
 * Description  :  This procedure will close the Movie device.
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
 *****************************************************************************/
VOID CloseTheDevice( VOID)
{

 /****************************************************************************/
 /* To close the device , we will issue a string command using mciSendString.*/
 /* This close command is done for the alias.                                */
 /****************************************************************************/

   fDeviceOpen = FALSE;
   fMovieLoaded = FALSE;
   SendString((HWND)NULL, "close movie wait", 0 );

   return;

}  /* end of CloseTheDevice */

/*************************************************************************
 * Name         :  OpenTheDevice
 *
 * Description  :  This procedure will open the Movie device.
 *
 * Concepts     :  Opening a device using MCI interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - open
 *
 * Parameters   :  hwnd - window handle
 *
 * Return       :  None
 *
 *****************************************************************************/
BOOL OpenTheDevice( HWND hwnd)
{
  if (!fDeviceOpen )

  {
     /******************************************************************/
     /* To open the device, we will issue MCI_OPEN command to the MCI  */
     /* for digital video.                                             */
     /******************************************************************/
     if ( SendString( hwnd,
                      "open digitalvideo alias movie wait shareable",
                      0 ) )

     {
        /* Open success, set the flag and return true */

        fDeviceOpen = TRUE;

        return(TRUE);
     }
  }

  return( FALSE );

}  /* end of OpenTheDevice */

/*************************************************************************
 * Name         :  EditTheMovie
 *
 * Description  :  This procedure will edit the movie file in the following
 *                 manner:  first we will load the movie, then set the data
 *                 format to frames.  The next thing we do is copy the first
 *                 25 frames which places these frames into the clipboard.
 *                 Then paste the 25 frames 4 times, which actually duplicates
 *                 our first 25 frames 4 times.  The last step is to cut the
 *                 first 10 frames from the movie which places them in the
 *                 clipboard then seek to the end of the movie and paste them
 *                 to the end.  The cut API actually deletes the specified
 *                 number of frames from the position you specified the cut.
 *
 * Concepts     :  Editing a movie using the MCI string interface.
 *
 * MMPM/2 API's :  mciSendString
 *                    - cut
 *                    - copy
 *                    - paste
 *
 * Parameters   :  hwnd - window handle
 *
 * Return       :  nothing.
 *
 ******************************************************************************/
VOID EditTheMovie( HWND hwnd)
{
   LONG    lmciSendStringRC;            /* return code from SendString */

 /**********************************************************************/
 /* Change pointer to a waiting pointer first, since this might take a */
 /* couple of seconds.                                                 */
 /**********************************************************************/
   WinSetPointer ( HWND_DESKTOP, hptrWait );

   if (!fMovieLoaded)   /* If the movie is not loaded then load it */
     {
       lmciSendStringRC = SendString(hwnd, "load movie movie.avi wait", 0);
       if (lmciSendStringRC)
         {
           fMovieLoaded = TRUE;
         }
       else
         {
           MessageBox( IDS_CANNOT_LOAD_MOVIE_FILE,
                       MB_CANCEL | MB_ERROR  | MB_MOVEABLE);
           return;
         } /* else */
     } /* if */

   /* Seek to the beginning of the movie */
   lmciSendStringRC = SendString(hwnd, "seek movie to start notify", 0);

 /********************************************************************/
 /* Before we start editing, change the data format to frames. Frame */
 /* format is sometimes easier to work with.                         */
 /********************************************************************/
   lmciSendStringRC = SendString( hwnd, "set movie time format frames wait",
                                  0 );

 /***********************************************************/
 /* copy the first 25 frames. The copy will place the       */
 /* data into the clipboard.                                */
 /***********************************************************/

   lmciSendStringRC = SendString(hwnd,"copy movie from 1 to 25 wait", 0);

 /*******************************************************************/
 /* Paste our information we just copied.  We will paste the 25     */
 /* frames 4 times which will duplciate what we have just copied.   */
 /* The paste will start at the current frame of the movie.         */
 /*******************************************************************/

   lmciSendStringRC = SendString(hwnd, "paste movie from 25 wait", 0);

   lmciSendStringRC = SendString(hwnd, "paste movie from 25 wait", 0);

   lmciSendStringRC = SendString(hwnd, "paste movie from 25 wait", 0);

   lmciSendStringRC = SendString(hwnd, "paste movie from 25 wait", 0);

 /******************************************/
 /* Cut the first 10 frames from the movie.*/
 /* This will delete the first 10 frames.  */
 /******************************************/

   lmciSendStringRC = SendString(hwnd, "cut movie from 1 to 25 wait", 0);

 /******************************************/
 /* Seek to the end of the movie so we can */
 /* paste the first 25 frames to the end.  */
 /******************************************/

   lmciSendStringRC = SendString(hwnd, "seek movie to end wait", 0);

   lmciSendStringRC = SendString(hwnd, "paste movie wait",0);

 /*******************************************************************/
 /* Seek to the beginning of the file so we can be ready to play.   */
 /*******************************************************************/

   lmciSendStringRC = SendString(hwnd, "seek movie to start notify", 0);

 /******************************************************************/
 /* Now that we're done here, change the pointer back to the arrow.*/
 /******************************************************************/

   WinSetPointer ( HWND_DESKTOP, hptrArrow );

}  /* end of EditTheMovie */



/*************************************************************************
 * Name         :  SendString
 *
 * Description  :  This procedure will send string to MCI.
 *
 * Concepts     :  Sending a string command using the mciSendString interface.
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
   LONG           lmciSendStringRC;    /* return value fromm mciSendString    */


   lmciSendStringRC =
       mciSendString( (PSZ)pcMCIString,
                      (PSZ)szReturn,
                      (USHORT)CCHMAXPATH,
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

   /***************************************************************************/
   /* Get the string from the Resource defined string table and show it       */
   /* in the message box.                                                     */
   /***************************************************************************/
   WinLoadString(
      hab,                             /* HAB for this dialog box.            */
      (HMODULE) NULL,                  /* Get the string from the .exe file.  */
      usMessageID,                     /* Which string to get.                */
      (SHORT) sizeof( achMessage),     /* The size of the buffer.             */
      achMessage );                    /* The buffer to place the string.     */

   if ( !(ulStyle & MB_INFORMATION) )
      {
      usResult =
         WinMessageBox(
            HWND_DESKTOP,              /* Parent handle of the message box.   */
            hwndMainDialogBox,         /* Owner handle of the message box.    */
            achMessage,                /* String to show in the message box.  */
            achMsgBoxTitle,            /* Title to shown in the message box.  */
            (USHORT) ID_MESSAGEBOX,    /* Message Box Id.                     */
            ulStyle );                 /* The style of the message box.       */
      }
   else
      {
      usResult =
         WinMessageBox(
            HWND_DESKTOP,              /* Parent handle of the message box.   */
            hwndMainDialogBox,         /* Owner handle of the message box.    */
            achMessage,                /* String to show in the message box.  */
            achTitle,                  /* Title to shown in the message box.  */
            (USHORT) ID_MESSAGEBOX,    /* Message Box Id.                     */
            ulStyle );                 /* The style of the message box.       */
      }

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

         /*******************************************************************/
         /* This is what we want.  We were able to use mciGetErrorString to */
         /* retrieve a textual error message we can show in a message box.  */
         /*******************************************************************/
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
 *                 file name exists. If it does, we want to attain specific
 *                 information for future use.  This information is attained
 *                 by getting the track ID of the movie.
 *
 * Concepts     :  Using MMIO interface to access a data file and header
 *                 information.
 *
 * MMPM/2 API's :  mmioOpen
 *                 mmioClose
 *                 mmioGetHeader
 *                 mmioSet
 *
 * Parameters   :  pszFileName - The file name to be tested.
 *
 * Return       :  TRUE  -  if the a file exists matching pszFilename
 *                          and there are no errors getting the movie file
 *                          header information.
 *                 FALSE -  if the file does not exist or problem getting
 *                          the movie file header information
 *
 *
 ******************************************************************************/
BOOL DoesFileExist( PSZ pszFileName )
{
   BOOL  bReturn = 0;                    /* Function return value       */
   HMMIO hFile;                          /* Handle to file              */
   LONG  lHeaderLengthMovie;             /* Header length of movie file */
   LONG  lHeaderLengthVideo;             /* Header length of movie file */
   ULONG ulTrackID;                      /* Track ID                    */
   LONG  lBytes;                         /* Number of bytes read        */
   LONG  lSize;                          /* Size of Header information  */
   PMMMOVIEHEADER pmmMovieHeader;        /* Std Movie Header            */
   MMVIDEOHEADER  mmVideoHeader;         /* Std Video Header            */
   MMEXTENDINFO   mmExtendinfo;          /* Std Extended Information    */
   MMIOINFO       mmioinfo;              /* Structure for mmio info     */
   PMMIOPROC      pIOProc;
   PMMIOPROC      pAnswer;
   HMODULE        hmod;
   FOURCC         fcc;
   CHAR           LoadError[100];
   ULONG          rc = MMIO_SUCCESS;


   /*********************************************************/
   /* Reset all my my mmio information structures           */
   /*********************************************************/

   memset(&mmioinfo, '\0', sizeof(MMIOINFO));
   memset(&mmExtendinfo, '\0', sizeof(MMEXTENDINFO));
   memset(&mmVideoHeader, '\0', sizeof(MMVIDEOHEADER));

   mmioinfo.ulTranslate =  MMIO_TRANSLATEHEADER;   /* Set to std translation */

                                    /* Set all operations on the active track*/
   mmExtendinfo.ulFlags = MMIO_TRACK;

   hFile = mmioOpen( pszFileName, &mmioinfo, MMIO_READ );

   if (hFile != (HMMIO) NULL)
    {

      mmExtendinfo.ulTrackID = -1;                     /* reset the track id */

     /*********************************************************/
     /* The mmioSet function associates an MMIO handle with   */
     /* the CODEC information.  The MMIO_SET_EXTENDEDINFO     */
     /* is set to associate the CODEC Procs with the file.    */
     /*********************************************************/

      bReturn = mmioSet(hFile, &mmExtendinfo, MMIO_SET_EXTENDEDINFO);

     /*********************************************************/
     /* The mmioQueryHeaderLength returns the length of the   */
     /* header in bytes.  This length is actually variable    */
     /* because a chunk of information gets added to each     */
     /* track or stream in the movie.                         */
     /*********************************************************/
      bReturn = mmioQueryHeaderLength(hFile, &lHeaderLengthMovie,0L, 0L);

      if (!bReturn)
        {
          pmmMovieHeader = malloc(lHeaderLengthMovie);
        } /* if */

     /*********************************************************/
     /* The mmioGetHeader function returns the header of the  */
     /* media structure of the file opened by mmioOpen.       */
     /* The header contains information about the media.      */
     /*********************************************************/
      if (!(bReturn = mmioGetHeader(hFile,
                                    pmmMovieHeader,
                                    lHeaderLengthMovie,
                                    &lBytes,
                                    0L,
                                    0L)))
       {

         /*********************************************************/
         /* Read the movie header informationn and get the track  */
         /* information.                                          */
         /*********************************************************/

          mmExtendinfo.ulTrackID = pmmMovieHeader->ulNextTrackID;

          /**************************************************************/
          /* Here we will use the mmioSet to set the extended info      */
          /* to the correct track after issuing the mmioGetHeader. Once */
          /* we have the correct track we can issue mmioGetHeader       */
          /* requesting the movie information we wish to have. In this  */
          /* sample we are asking for the height, width, length of the  */
          /* movie.                                                     */
          /**************************************************************/

          bReturn = mmioSet(hFile, &mmExtendinfo, MMIO_SET_EXTENDEDINFO);

          lHeaderLengthVideo = sizeof(MMVIDEOHEADER);

          /*******************************************************/
          /* Get the information we need from the video header   */
          /* height, width, length.                              */
          /*******************************************************/

          bReturn = mmioGetHeader(hFile,
                                  &mmVideoHeader,
                                  lHeaderLengthVideo,
                                  &lBytes,
                                  0L,
                                  0L);

          ulMovieWidth  = mmVideoHeader.ulWidth;

          ulMovieHeight = mmVideoHeader.ulHeight;

          ulMovieLength = mmVideoHeader.ulLength;

          /*****************************************************/
          /* Here we are using the mmioSet to reset the tracks */
          /* We do this just in case we would need to do some- */
          /* thing different with the movie file.              */
          /*****************************************************/
          mmExtendinfo.ulTrackID = MMIO_RESETTRACKS;

          bReturn = mmioSet(hFile, &mmExtendinfo,MMIO_SET_EXTENDEDINFO);

          mmioClose( hFile, 0);

          bReturn = TRUE;
          return (bReturn);
          free(pmmMovieHeader);
       }
    }
   bReturn = FALSE;
   return (bReturn);


}
/******************************************************************************
 * Name         :  SetMovieVolume
 *
 * Description  :  This helper function sets the Movie file volume based upon
 *                 the position of the volume slider.  The slider will be
 *                 queried and the Movie file volume will be set.
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
VOID SetMovieVolume( VOID )
{
   LONG      lmciSendStringRC;             /* return value form mciSendString */
   CHAR      szVolume[4] = "";             /* to hold the volume level        */
   CHAR      szCommandString[CCHMAXPATH] = /* string command to MCI           */
               "set movie audio volume ";

   if( !fAudioOn ) return;

   if ((!fPassedDevice) && (eState == ST_PLAYING ))
   {
      /*****************************************************************/
      /* To set the volume,  first, build the string command for the   */
      /* MCI.  Then an MCI_SET command should be issued to the device  */
      /* to perform the volume change.                                 */
      /*****************************************************************/
      _itoa(sVolumeLevel, szVolume, 10);
      strcat( szCommandString, szVolume);
      strcat( szCommandString, " ");
      strcat( szCommandString, "wait");

      lmciSendStringRC =
          mciSendString( (PSZ)szCommandString,
                         (PSZ)szReturn,
                         CCHMAXPATH,
                         (HWND)NULL,
                         (USHORT)NULL );
      if (lmciSendStringRC != 0)
         ShowMCIErrorMessage(lmciSendStringRC);

   }
   return;
}  /* end of SetMovieVolume */

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

   HELPINIT helpInit;                 /* Help initialization structure. */
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
   helpInit.phtHelpTable       = (PVOID)(0xffff0000 | ID_MOVIE_HELPTABLE);

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
         MessageBox( IDS_HELP_CREATION_FAILED,
                     MB_OK | MB_INFORMATION | MB_MOVEABLE);
      }
      else  /* help creation worked */
         WinAssociateHelpInstance(
            hwndHelpInstance,        /* The handle of the Help Instance.      */
            hwndFrame);              /* Associate to this dialog window.      */
   }  /* End of IF checking the creation of the Help Instance. */

}  /* End of InitializeHelp */

/*************************************************************************
 * Name         : InsertHelpMenuItem
 *
 * Description  : This procedure will set up "Help" menu item to
 *                the system menu.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : Pointer to the "Help" text data.
 *
 * Return       : None.
 *
 *************************************************************************/
VOID InsertHelpMenuItem( PCHAR szHelp )
{
   MENUITEM MenuItem;                     /* Menu-Item Structure           */
   HWND     hwndSysMenuPullDown;          /* window handle for pulldown    */
   USHORT   idSysMenu;                    /*                               */
   HWND     hwndSysMenu;                  /* window handle for system menu */
   USHORT   i;                            /*                               */
   USHORT   iDefaultSize = MIT_END;       /*                               */
   BOOL     fSearch = TRUE;               /*                               */
   LONG     cntSeparator = 0;             /*                               */


   /*
    *  Get the system pull down menu window handle.
    */
   hwndSysMenu = WinWindowFromID(hwndFrame,
                                   FID_SYSMENU);
   idSysMenu = SHORT1FROMMR (WinSendMsg (hwndSysMenu,
                               MM_ITEMIDFROMPOSITION, 0L, 0L));
   WinSendMsg (hwndSysMenu, MM_QUERYITEM,
                 MPFROM2SHORT (idSysMenu, FALSE), MPFROMP(&MenuItem));
   hwndSysMenuPullDown = MenuItem.hwndSubMenu;

   /*
    *  Find the position to insert "Help" item.
    */
   for (i=0; fSearch; i++)
   {
      idSysMenu = SHORT1FROMMR (WinSendMsg (hwndSysMenuPullDown,
                                MM_ITEMIDFROMPOSITION, (MPARAM)i, 0L));
      fSearch = (BOOL)WinSendMsg (hwndSysMenuPullDown, MM_QUERYITEM,
                  MPFROM2SHORT (idSysMenu, FALSE), MPFROMP(&MenuItem));

      if (fSearch && MenuItem.afStyle & MIS_SEPARATOR)
      {
         cntSeparator ++;
         if (cntSeparator == 2)
         {
            cntSeparator ++;
            break;
         } /* endif */
      } /* endif */

   } /* endfor */

   if (fSearch)
   {
      if (i==0)
      {
         iDefaultSize = MIT_FIRST;
      }
      else
      {
         iDefaultSize = i;
      }
   }

   /*
    *  Insert separator first.
    */
   MenuItem.iPosition   = iDefaultSize;
   MenuItem.afStyle     = MIS_SEPARATOR;
   MenuItem.afAttribute = (ULONG)NULL;
   MenuItem.id          = (ULONG)NULL;
   MenuItem.hwndSubMenu = (HWND)NULL;
   MenuItem.hItem       = (ULONG)NULL;
   WinSendMsg (hwndSysMenuPullDown, MM_INSERTITEM,
                 (MPARAM)&MenuItem, (MPARAM)NULL);
   /*
    *  Insert "Help" text next.
    */
   MenuItem.iPosition   = ++iDefaultSize;
   MenuItem.afStyle     = (ULONG)NULL;
   MenuItem.afAttribute = (ULONG)NULL;
   MenuItem.id          = WM_HELP;
   MenuItem.hwndSubMenu = (HWND)NULL;
   MenuItem.hItem       = (ULONG)NULL;
   WinSendMsg (hwndSysMenuPullDown, MM_INSERTITEM,
                 (MPARAM)&MenuItem, (MPARAM)szHelp);

}  /* End of InsertHelpMenuItem */

/******************************************************************************
 * Name         : ResizeMovieWindow
 *
 * Description  : This function will resize the applications movie window
 *                according to the height and width of the movie file being
 *                opened.
 *
 * Parameters   : hwnd - Handle of the secondary window.
 *
 * Return       : none.
 *
 ******************************************************************************/
VOID ResizeMovieWindow (HWND hwnd)
{

   HWND hwndSEFrame;             /* handle for the Secondary Window Frame     */
   SWP  swpSEFrame;              /* struc containing information on Frame     */
   SWP  swpRewind;               /* struc containing information rewind button*/
   LONG cxWidthBorder = 0;
   LONG cyWidthBorder = 0;
   LONG cyTitleBar = 0;

  /*************************************************************/
  /* Query the position of the secondary window frame          */
  /* and the rewind button so we can postion our application   */
  /* frame for the movie.                                      */
  /*************************************************************/

   WinQueryWindowPos( hwndFrame, &swpSEFrame );

   WinQueryWindowPos( WinWindowFromID( hwnd, IDC_GPB_REWIND ),
                      &swpRewind );

   cxWidthBorder = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
   cyWidthBorder = (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
   cyTitleBar =    (LONG) WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);

  /*************************************************************/
  /* Check to see if our movie size is larger than our frame.  */
  /* If it is then resize our frame to fit the movie.          */
  /*************************************************************/

 if ((swpSEFrame.cx - (2 * cxWidthBorder)) <= ulMovieWidth)
  {
    swpSEFrame.cx = ulMovieWidth + (2 * cxWidthBorder);
  } /* if */
 if ((swpSEFrame.cy - swpRewind.cy - ulMovieHeight -
                                     cyTitleBar - (2 * cyWidthBorder)
                                      < ulMovieHeight ))
  {
    swpSEFrame.cy = ulMovieHeight + cyTitleBar + (2 * cyWidthBorder);
  } /* if */

  /*************************************************************/
  /* Set the position and size of the application window       */
  /* to the position and size of the movie.                    */
  /*************************************************************/

     WinSetWindowPos( hwndAppFrame, HWND_TOP,
                      (swpSEFrame.cx - ulMovieWidth - cxWidthBorder) / 2,
                      (cyWidthBorder + swpRewind.y + swpRewind.cy) + 5,
                      ulMovieWidth,
                      ulMovieHeight,
                      SWP_SIZE | SWP_MOVE);

}




/*****************************************************************
 * Name : QueryDirectMode
 *
 * Function: Determine if direct mode is supported.
 *
 ******************************************************************/

BOOL  QueryDirectMode(VOID)
{
    DIVE_CAPS    DiveCaps;             /* Dive Query Capabilities */
    ULONG        rc;                   /* API Return Code         */
    FOURCC       fccFormats[100] = {0};

    memset (&DiveCaps,0,sizeof(DiveCaps));
    DiveCaps.pFormatData = fccFormats;
    DiveCaps.ulFormatLength = 120;
    DiveCaps.ulStructLen = sizeof(DIVE_CAPS);
    rc = DiveQueryCaps( &DiveCaps, DIVE_BUFFER_SCREEN);
    if (!rc)
      {
        if (DiveCaps.fScreenDirect)
         {
           if (DiveCaps.ulDepth < 8)
              return (BOOL)(FALSE);
         }
         else
              return (BOOL)(FALSE);

      }
    return (BOOL)(TRUE);
}

