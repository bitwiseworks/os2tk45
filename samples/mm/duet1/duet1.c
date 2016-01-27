/*************************************************************************
 * File Name    :  DUET1.C
 *
 * Description  :  This file contains the C source code required for the
 *                 DUET1 sample program.
 *
 * Concepts     :  Two parts of a duet will be played as a non-piecemeal group.
 *                 Each part will be streamed from a waveform file.  Also, the
 *                 volume of the group (and therefore each device) will be
 *                 manipulated.  Additionally, an audio help message will be
 *                 incorporated  into IPF help.
 *
 * MMPM/2 API's :  List of all MMPM/2 API's that are used in
 *                 this module
 *
 *                 mciSendCommand    MCI_OPEN
 *                                   MCI_LOAD
 *                                   MCI_CUE
 *                                   MCI_PLAY
 *                                   MCI_PAUSE
 *                                   MCI_RESUME
 *                                   MCI_CLOSE
 *                                   MCI_SET
 *                                   MCI_GROUP
 *                 mciGetErrorString
 *                 mmioOpen
 *                 mmioClose
 *
 * Required
 *    Files     :  duet1.c        Source Code.
 *                 duet1.h        Include file.
 *                 duet1.dlg      Dialog definition.
 *                 duet1.rc       Resources.
 *                 duet1.ipf      Help text.
 *                 duet1.mak      Make file.
 *                 duet1.def      Linker definition file.
 *                 duet1.ico      Program icon.
 *
 * Copyright (C) IBM 1991, 1992, 1993
 *************************************************************************/
#define  INCL_OS2MM                 /* required for MCI and MMIO headers   */
#define  INCL_WIN                   /* required to use Win APIs.           */
#define  INCL_PM                    /* required to use PM APIs.            */
#define  INCL_WINHELP               /* required to use IPF.                */
#define  INCL_WINSTDSLIDER          /* required for using slider control   */
#define  INCL_SECONDARYWINDOW       /* required for secondary window       */
#define  INCL_CIRCULARSLIDER        /* required for circular slider control*/
#define  INCL_GRAPHICBUTTON         /* required for graphic button control */
#define  INCL_MACHDR                /* required for mciplyfile             */

#include <os2.h>
#include <os2me.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sw.h>

#include "duet1.h"


typedef struct
   {
   CHAR  achTitle[LEN_DUET_TITLE];     /* the duet's title                 */
   CHAR  achPart1[LEN_AUDIO_FILENAME]; /* the filename of one duet part    */
   CHAR  achPart2[LEN_AUDIO_FILENAME]; /* the filename of the other part   */
   }DUET;

enum DuetStates {ST_CLOSED, ST_STOPPED, ST_PLAYING, ST_PAUSED};

/*
 * Procedure/Function Prototypes
 */
MRESULT EXPENTRY  MainDialogProc( HWND hwnd,
                                  ULONG msg,
                                  MPARAM mp1,
                                  MPARAM mp2);

USHORT            DuetMessageBox( USHORT usTitleID,
                                  USHORT usMessageID,
                                  ULONG  ulStyle);

VOID              ShowMCIErrorMessage( ULONG ulError);

VOID              Initialize( VOID);
VOID              Finalize( VOID);
VOID              InitializeHelp( VOID);
BOOL              PlayTheDuet( HWND hwnd);
BOOL              PauseTheDuet( HWND hwnd);
BOOL              ResumeTheDuet( HWND hwnd);
VOID              StopTheDuet( HWND hwnd);
VOID              CloseTheDuet( VOID);
BOOL              DoesFileExist( PSZ pszFilename);
VOID              SetDuetVolume( HWND hwnd);


/*************** End of Procedure/Function Prototypes *************************/


/*
 * Global Variables.
 */
HELPINIT hmiHelpStructure;       /* Help initialization structure.         */

HAB      hab;
QMSG     qmsg;
HMQ      hmq;
HWND     hwndMainDialogBox;      /* Handle to the dialog window.           */
HWND     hwndFrame;              /* Handle to the frame window.            */
HWND     hwndHelpInstance;       /* Handle to Help window.                 */

enum     DuetStates   eState;
/* state of the selected duet - playing, paused, or stopped */

DUET     aDuet[NUM_DUETS]; /* array of duets with information about each   */

CHAR     achHelpWindowTitle[LEN_HELP_WINDOW_TITLE];
CHAR     achHelpLibraryName[LEN_HELP_LIBRARY_NAME];

USHORT   usDuetPart1ID;              /* device ID for part 1 of the duet      */
USHORT   usDuetPart2ID;              /* device ID for part 2 of the duet      */
USHORT   usGroupHandle;              /* handle to group - duet                */
SHORT    sVolumeLevel;               /* desired volume level                  */

BOOL     fPassedDuet      = FALSE;/* for MCI_ACQUIRE to play the Duet         */
BOOL     fSecondDuetPass  = FALSE;/* for setting fPassedDuet-2 devices in grp */
BOOL     fFirstPlay       = TRUE;    /* Indicates we've played for first time */
CHAR     achAudioHelpFile[LEN_AUDIO_FILENAME];
/************************** End of Global Variables ***************************/


/*************************************************************************
 * Name         : main
 *
 * Description  : This function calls the Intialize procedure to prepare
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
 *************************************************************************/
INT main( VOID )
{

   Initialize();

   while ( WinGetMsg( hab, (PQMSG) &qmsg, (HWND) NULL, 0, 0) )
      WinDispatchMsg( hab, (PQMSG) &qmsg );

   Finalize();

   return( TRUE);

} /* End of main */



/*************************************************************************
 * Name         : Initialize
 *
 * Description  : This function performs the necessary initializations and
 *                setups that are required to show/run a dialog box as a
 *                main window.  The message queue will be created, as will
 *                the dialog box.
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
VOID Initialize( VOID)
{
   CHAR     achTitle[LEN_PROGRAM_TITLE]; /* buffer for window title text      */
   CHAR     szDefaultSize[CCHMAXPATH];   /* buffer for default size menu text */
   ULONG    ulrc;                        /* API Return Code                   */
   SZ       szProdInfo[124];             /* Product Information Buffer        */
   MCI_OPEN_PARMS    OpenParms;          /* open parms for MCI_OPEN           */
   MCI_INFO_PARMS    InfoParms;          /* Info parms for MCI_INFO           */

   /*
    * Setup and initialize the dialog window.
    * Change pointer to a waiting pointer first, since this might take a
    * couple of seconds.
    */



   hab = WinInitialize( 0);

   hmq = WinCreateMsgQueue( hab, 0);


   WinSetPointer(
      HWND_DESKTOP,        /* Desktop window handle.                    */
      WinQuerySysPointer(  /* This API will give the handle of the PTR. */
         HWND_DESKTOP,     /* Desktop window handle.                    */
         SPTR_WAIT,        /* The waiting icon.                         */
         FALSE ) );        /* Return the system pointer's handle.       */

   /* Check the Default Audio Device. IBM M-AUDIO Card supports
    * Playback of two mono PCM Instances simulataneously. If we
    * are not running on this device we will terminate. The Source
    * code can be modified easily to run on other known devices
    * that support playback of two mono instances simultaneously.
    */

    /* Open The Default WaveAudio Device   */

    memset (&OpenParms,0,sizeof(OpenParms));

    OpenParms.usDeviceID       = (USHORT)  NULL; /* this is returned   */
    OpenParms.pszDeviceType    = (PSZ)MAKEULONG(MCI_DEVTYPE_WAVEFORM_AUDIO,1) ;

    ulrc =   mciSendCommand( (USHORT) 0,
                              MCI_OPEN,
                              MCI_WAIT | MCI_OPEN_TYPE_ID |
                              MCI_OPEN_SHAREABLE,
                              (PVOID) &OpenParms,
                              UP_OPEN);

    /* Query the product Information */

    if (!ulrc)
      {
        InfoParms.pszReturn =(PSZ)szProdInfo;
        InfoParms.ulRetSize = 124;
        ulrc =   mciSendCommand( (USHORT) OpenParms.usDeviceID,
                                  MCI_INFO,
                                  MCI_WAIT | MCI_INFO_PRODUCT,
                                  (PVOID) &InfoParms,
                                  0);
        if (!ulrc && (InfoParms.pszReturn != NULL))
         {
            if (strnicmp (InfoParms.pszReturn,"IBM M-AUDIO CAPTURE",18) != 0)
             {
                /* We are not running on an IBM M Audio Adapter */
                WinMessageBox( HWND_DESKTOP,
                               hwndMainDialogBox,
                               "The Duet Player Will only run on IBM M-AUDIO CARD or any other card(s) supporting simultaneous playback of 2 Mono Instances",
                               "Duet Player Information",
                               (USHORT) ID_MESSAGEBOX,
                               MB_OK | MB_INFORMATION | MB_MOVEABLE);

                /* Close the WaveAudio Instance Just opened */
                ulrc =   mciSendCommand( (USHORT) OpenParms.usDeviceID,
                                          MCI_CLOSE,
                                          MCI_WAIT ,
                                          (PVOID) NULL,
                                          0);

                DosExit (EXIT_PROCESS,0);

             }/* not MACPA Card */
         }/* MCI_INFO Ok */
        /* Close the WaveAudio Instance Just opened */
        mciSendCommand( (USHORT) OpenParms.usDeviceID,
                        MCI_CLOSE,
                        MCI_WAIT ,
                        (PVOID) NULL,
                        0);

      } /* MCI_OPEN Ok  */

   /*
    * Load the strings for the Duet Titles and filenames from the resource
    * file.  This must be done before the dialog box is loaded since it
    * fills the listbox with titles when it receives the WM_INITDLG message.
    */
   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DUET_1_TITLE,
      (SHORT) sizeof( aDuet[0].achTitle),
      aDuet[0].achTitle);

   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DUET_1_PART1_FILE,
      (SHORT) sizeof( aDuet[0].achPart1),
      aDuet[0].achPart1);

   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DUET_1_PART2_FILE,
      (SHORT) sizeof( aDuet[0].achPart2),
      aDuet[0].achPart2);


   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DUET_2_TITLE,
      (SHORT) sizeof( aDuet[1].achTitle),
      aDuet[1].achTitle);

   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DUET_2_PART1_FILE,
      (SHORT) sizeof( aDuet[1].achPart1),
      aDuet[1].achPart1);

   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DUET_2_PART2_FILE,
      (SHORT) sizeof( aDuet[1].achPart2),
      aDuet[1].achPart2);


   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DUET_3_TITLE,
      (SHORT) sizeof( aDuet[2].achTitle),
      aDuet[2].achTitle);

   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DUET_3_PART1_FILE,
      (SHORT) sizeof( aDuet[2].achPart1),
      aDuet[2].achPart1);

   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DUET_3_PART2_FILE,
      (SHORT) sizeof( aDuet[2].achPart2),
      aDuet[2].achPart2);

   WinLoadString(
      hab,
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
         ID_DLG_MAIN,           /* Dialog ID.                                 */
         (PVOID) NULL);         /* Creation Parameters for the dialog.        */

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
      hab,                          /* HAB for this dialog box.            */
      (HMODULE) NULL,               /* Get the string from the .exe file.  */
      IDS_PROGRAM_TITLE,            /* Which string to get.                */
      (SHORT) sizeof( achTitle),    /* The size of the buffer.             */
      achTitle);                    /* The buffer to place the string.     */


   WinSetWindowText( hwndFrame, achTitle);


   /*
    * Initialize the help structure and associate the help instance to this
    * dialog via it's handle to anchor block.
    */
   InitializeHelp();

   /*
    * Now that we're done here, change the pointer back to the arrow.
    */

   WinSetPointer(
      HWND_DESKTOP,        /* Desktop window handle.                    */
      WinQuerySysPointer(  /* This API will give the handle of the PTR. */
         HWND_DESKTOP,     /* Desktop window handle.                    */
         SPTR_ARROW,       /* The Arrow icon.                           */
         FALSE ) );        /* Return the system pointer's handle.       */

} /* End of Initialize */



/*************************************************************************
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
 *************************************************************************/
VOID Finalize( VOID )
{
   /*
    * Destroy the Help Instance for this dialog window.
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
 * Name         : MainDialogProc
 *
 * Description  : This function controls the main dialog box.  It will
 *                handle received messages such as pushbutton notifications,
 *                timing events, etc.
 *
 * Concepts     : None.
 *
 *
 * MMPM/2 API's : None.
 *
 *
 * Parameters   : hwnd - Handle for the Main dialog box.
 *                msg  - Message received by the dialog box.
 *                mp1  - Parameter 1 for the just recieved message.
 *                mp2  - Parameter 2 for the just recieved message.
 *
 * Return       :
 *
 ******************************************************************************/
MRESULT EXPENTRY MainDialogProc( HWND   hwnd,
                                 ULONG  msg,
                                 MPARAM mp1,
                                 MPARAM mp2 )
{
   HPOINTER          hpProgramIcon;           /* handle to program's icon     */
   PSWP              pswpWindowActionMP;      /* hold status of window        */
   int               iDuet;                   /* index of duet number         */
   USHORT            usUserParm;              /* user parameter returned      */
   USHORT            usNotifyCode;            /* notification message code    */
   USHORT            usCommandMessage;        /* command message for notify   */
   MCI_GENERIC_PARMS mciGenericParms;         /* generic parms for MCI_ACQUIRE*/
   ULONG             ulError;                 /* rc for MCI_ACQUIRE send cmd  */
   CHAR              achAudioHelpFile[LEN_AUDIO_FILENAME];/* Audio file name  */


   switch( msg )
   {

      case WM_INITDLG :

         /*
          * Initialize the dialog window.
          * Change pointer to a waiting pointer first, since this might take a
          * couple of seconds.
          */

         WinSetPointer(
            HWND_DESKTOP,        /* Desktop window handle.                    */
            WinQuerySysPointer(  /* This API will give the handle of the PTR. */
               HWND_DESKTOP,     /* Desktop window handle.                    */
               SPTR_WAIT,        /* The waiting icon.                         */
               FALSE ) );        /* Return the system pointer's handle.       */

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
          * We need to fill the listbox with the titles of the duets.
          * To do this, loop through each duet and send a LM_INSERTITEM
          * message to the listbox with the text of the duet's title.
          */
         for(iDuet=0; iDuet<NUM_DUETS; iDuet++)
         {
            WinSendMsg( WinWindowFromID( hwnd, ID_LB_DUET),
                        LM_INSERTITEM,
                        (MPARAM) LIT_END,
                        aDuet[iDuet].achTitle);
         }


         /* select the first duet in the listbox by default */

         WinSendMsg( WinWindowFromID (hwnd, ID_LB_DUET),
                     LM_SELECTITEM,
                     (MPARAM) 0,
                     (MPARAM) TRUE);

         sVolumeLevel = INIT_VOLUME;  /* initialize the desired volume level */
         /*
          * The slider control cannot be completely set from the dialog
          * template so some aspects must be set here.  We will set the
          * volume range to 0-100, increment to 1-10, and the initial
          * volume level to 75.
          */
         WinSendMsg( WinWindowFromID (hwnd, ID_SL_VOLUME),
                     CSM_SETRANGE,
                     (MPARAM) 0L,
                     (MPARAM) 100L);

         WinSendMsg( WinWindowFromID (hwnd, ID_SL_VOLUME),
                     CSM_SETINCREMENT,
                     (MPARAM) 10L,
                     (MPARAM) 1L);

         WinSendMsg( WinWindowFromID (hwnd, ID_SL_VOLUME),
                     CSM_SETVALUE,
                     (MPARAM) sVolumeLevel,
                     (MPARAM) NULL);

         /*
          * Set up the PLAY graphic pushbutton.
          */
         WinSendMsg (
            WinWindowFromID(
               hwnd,                /* Dialog window handle             */
               ID_GPB_PLAY),        /* Id - Play graphic button         */
            GBM_SETANIMATIONRATE,   /* Animation rate control           */
            MPFROMLONG(100L),       /* Update play bitmap every .1 sec  */
            NULL);                  /* Ignore return data               */

         eState = ST_CLOSED;       /* The initial duet state is closed.      */
         /*
          * Now that we're done here, change the pointer back to the arrow.
          */

         WinSetPointer(
            HWND_DESKTOP,        /* Desktop window handle.                    */
            WinQuerySysPointer(  /* This API will give the handle of the PTR. */
               HWND_DESKTOP,     /* Desktop window handle.                    */
               SPTR_ARROW,       /* The Arrow icon.                           */
               FALSE ) );        /* Return the system pointer's handle.       */

         return( (MRESULT) 0);


      case WM_CLOSE :

         /*
          * Clean up the devices and group.
          */

         if (!fFirstPlay) {        /* If we've opened the devices */
            CloseTheDuet();
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2));

      case WM_HELP :
         /*
          * The dialog window has recieved a request for help from the user,
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
          * To get which pushbutton was pressed the SHORT1FROMMP macro
          * is used.
          */
         switch (SHORT1FROMMP(mp1))
         {

            case ID_GPB_PLAY:     /* user selected "Play"    */
               if ((eState==ST_CLOSED) || (eState==ST_STOPPED))
               {

                  if (fPassedDuet)
                  {                       /* If we've passed the device away */

                     /* If we don't have control of the device (ie. if we've
                      * passed it) then put up an error message.
                      */

                     DuetMessageBox( IDS_DUET_PLAYER_ERROR,
                                     IDS_CANT_PROCESS_MESSAGE,
                                     MB_OK | MB_INFORMATION  | MB_MOVEABLE);
                  }
                  else                   /* If we haven't passed the device */
                  {
                     /* eState will be set in PlayTheDuet */
                     if (PlayTheDuet( hwnd))
                     {

                        WinEnableWindow( WinWindowFromID( hwnd, ID_LB_DUET),
                                         FALSE);
                        /*
                         * Start the play button animation
                         */
                        WinSendMsg(
                           WinWindowFromID (
                              hwnd,            /* Dialog window handle      */
                              ID_GPB_PLAY),    /* Id - Play graphic button  */
                           GBM_ANIMATE,        /* Animation control         */
                           MPFROMSHORT(TRUE),  /* Animation flag            */
                           NULL);              /* Ignore return data        */
                     }
                     else
                        eState = ST_STOPPED;
                  }  /* End of if we've passed the device away              */
               }
               else if (eState==ST_PAUSED)
               {
                  if (fPassedDuet)
                  {     /* If we've passed the device away                  */

                     /* If we don't have control of the device (ie. if we've
                      * passed it) then put up an error message.
                      */

                     DuetMessageBox( IDS_DUET_PLAYER_ERROR,
                                     IDS_CANT_PROCESS_MESSAGE,
                                     MB_OK | MB_INFORMATION  | MB_MOVEABLE);
                  }
                  else  /* If we haven't passed the device away             */
                  {
                     if (ResumeTheDuet(hwnd))
                        eState = ST_PLAYING;
                  }  /* End of if we've passed the device away.             */
               }
               break;

            case ID_GPB_PAUSE:    /* user selected "Pause"   */
               if (eState==ST_PLAYING || eState==ST_PAUSED)
               {
                  if (fPassedDuet)
                  {  /* If we've passed the device away...                   */

                     /* If we don't have control of the device (ie. if we've
                      * passed it) then put up an error message.
                      */

                     DuetMessageBox( IDS_DUET_PLAYER_ERROR,
                                     IDS_CANT_PROCESS_MESSAGE,
                                     MB_OK | MB_INFORMATION  | MB_MOVEABLE);
                  }
                  else  /* If we haven't passed the device away...           */
                  {
                     if (eState==ST_PLAYING)
                     {
                        if (PauseTheDuet(hwnd))
                           eState = ST_PAUSED;
                     }
                     else
                     {
                        if (ResumeTheDuet(hwnd))
                           eState = ST_PLAYING;
                     }
                  }  /* End of if we've passed the device away               */
               }
               break;

            case ID_GPB_STOP:     /* user selected "Stop"    */
               if (eState==ST_PLAYING || eState==ST_PAUSED)
               {
                  if (fPassedDuet)
                  {      /* If we've passed control of device away           */

                     /* If we don't have control of the device (ie. if we've
                      * passed it) then put up an error message.
                      */

                     DuetMessageBox( IDS_DUET_PLAYER_ERROR,
                                     IDS_CANT_PROCESS_MESSAGE,
                                     MB_OK | MB_INFORMATION  | MB_MOVEABLE);
                  }
                  else
                  {
                     eState = ST_STOPPED;
                     StopTheDuet(hwnd);
                     WinEnableWindow( WinWindowFromID( hwnd, ID_LB_DUET), TRUE);
                  }
               }
               break;

            case ID_PB_CANCEL:   /* user selected "Cancel"  */
               WinSendMsg( hwnd, WM_CLOSE, (MPARAM) NULL, (MPARAM) NULL);
               break;

            default:
               break;

         }  /* End of Command Switch */
         return( (MRESULT) 0);

      case WM_CONTROL:
         if (SHORT1FROMMP(mp1)==ID_SL_VOLUME)
         {
            if ((SHORT2FROMMP(mp1)==CSN_CHANGED) ||    /* change volume?   */
                (SHORT2FROMMP(mp1)==CSN_TRACKING))     /* tracking volume? */
            {
               sVolumeLevel = SHORT1FROMMP (mp2);
               SetDuetVolume(hwnd);
            }
         }
         return( (MRESULT) 0);

      case WM_MINMAXFRAME:

         /*
          * Handling this message is required since this program is using
          * a dialog box as the main window.  In PM, the controls in the
          * bottom left corner of the dialog box may overlap the icon when
          * the progrma is minimized.
          *
          * Therefore, if the message action indicates that the program is
          * about to be minimized then we will hide the pushbutton in the
          * lower lefthand corner.  Otherwise, we assume the program is about
          * be restored so we will allow the pushbutton to be shown.
          */

         pswpWindowActionMP = (PSWP) LONGFROMMP( mp1 );

         if ( pswpWindowActionMP->fl & SWP_MINIMIZE )
            WinShowWindow( WinWindowFromID( hwnd, ID_PB_CANCEL), FALSE);
         else
            WinShowWindow( WinWindowFromID( hwnd, ID_PB_CANCEL), TRUE);

         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      /*
       * The next two messages are handled so that the Duet Player
       * application can participate in device sharing.  Since it opens
       * the devices as shareable devices, other applications can gain
       * control of the devices.  When this happens, we will receive a
       * pass device message.  We keep track of this device passing in
       * the fPassedDuet boolean variable.
       *
       * Be careful, though, because we'll be getting a pass device for
       * each device in the group.  Don't issue the acquire till we've
       * gotten the pass device for both devices in the group.  This is
       * kept track of by the fSecondDuetPass variable.
       *
       * If we do not have access to the device when we receive an activate
       * message, then we will issue an acquire device command to gain
       * access to the device.
       */

      case MM_MCIPASSDEVICE:
         if (SHORT1FROMMP(mp2) == MCI_GAINING_USE) {          /* GAINING USE */

            if (fSecondDuetPass) {           /* If this is the 2nd pass msg. */

               fPassedDuet = FALSE;          /* Gaining control of device.   */
               fSecondDuetPass = FALSE;      /* Reset BOOL for next test.    */

               if (eState == ST_PLAYING) {      /* If the duet was playing   */
                  WinSendMsg(                   /* Start Play button animation*/
                     WinWindowFromID (
                        hwnd,                   /* Dialog window handle       */
                        ID_GPB_PLAY),           /* Id - Play graphic button   */
                     GBM_ANIMATE,               /* Animation control          */
                     MPFROMSHORT(TRUE),         /* Animation flag             */
                     NULL);                     /* Ignore return data         */
               }

            }
            else                             /* If this is the 1st pass msg. */
            {
               fSecondDuetPass = TRUE;       /* Set BOOL for next test.      */
            }

         } else {                                             /* LOSING USE  */

            if (fSecondDuetPass) {           /* If this is the 2nd pass msg. */

               fPassedDuet = TRUE;           /* Losing  control of device.   */
               fSecondDuetPass = FALSE;      /* Reset BOOL for next test.    */

               if (eState == ST_PLAYING) {      /* If the duet was playing   */
                  WinSendMsg(                   /* Start Play button animation. */
                     WinWindowFromID (
                        hwnd,                   /* Dialog window handle         */
                        ID_GPB_PLAY),           /* Id - Play graphic button     */
                     GBM_ANIMATE,               /* Animation control            */
                     MPFROMSHORT(FALSE),        /* Animation flag               */
                     NULL);                     /* Ignore return data           */
                }
            }
            else                             /* If this is the 1st pass msg. */
            {
               fSecondDuetPass = TRUE;       /* Set BOOL for next test.      */
            }
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case WM_ACTIVATE:

      /**************************************************************
       * We use the WM_ACTIVATE message to participate in device
       * sharing.  We first check to see if this is an activate
       * or a deactivate message (indicated by mp1).  Then,
       * we check to see if we've passed control of the duets'
       * devices.  If these conditions are true, then
       * we issue an acquire device command to regain control of
       * the device, since we're now the active window on the screen.
       *
       * This is one possible method that can be used to implement
       * device sharing. For applications that are more complex
       * than this sample program, developers may wish to take
       * advantage of a more robust method of device sharing.
       * This can be done by using the MCI_ACQUIRE_QUEUE flag on
       * the MCI_ACQUIREDEVICE command.  Please refer to the MMPM/2
       * documentation for more information on this flag.
       **************************************************************/

         if ((BOOL)mp1 && fPassedDuet == TRUE) {

            mciGenericParms.hwndCallback = hwnd;

            ulError = mciSendCommand( usGroupHandle,
                                      MCI_ACQUIREDEVICE,
                                     (ULONG)MCI_NOTIFY,
                                     (PVOID) &mciGenericParms,
                                     (USHORT)NULL);
            if (ulError)
            {
               ShowMCIErrorMessage( ulError);
            }

         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case MM_MCINOTIFY:
         usNotifyCode = (USHORT) SHORT1FROMMP( mp1);
         usUserParm  = (USHORT) SHORT2FROMMP( mp1);

         usCommandMessage = (USHORT) SHORT2FROMMP( mp2); /* high-word */

         switch (usCommandMessage)
         {
            case MCI_PLAY:
               switch (usNotifyCode)
               {
                  case MCI_NOTIFY_SUCCESSFUL:
                     if (eState != ST_STOPPED && eState != ST_CLOSED)
                     {
                        /*
                        * We will receive the MCI_NOTIFY_SUCCESSFUL message
                        * for each device in the group, so we need to be sure
                        * to only do this action once.  That's why we are
                        * checking the eState and then immediately setting it
                        * to ST_STOPPED.
                        */

                        eState = ST_STOPPED;

                        /*
                         * Stop the play button animation
                         */

                        WinSendMsg (
                           WinWindowFromID(
                              hwnd,             /* Dialog window handle      */
                              ID_GPB_PLAY),     /* Id - Play graphic button  */
                           GBM_ANIMATE,         /* Animation control         */
                           MPFROMSHORT(FALSE),  /* Animation flag            */
                           NULL);               /* Ignore return data        */

                        WinEnableWindow( WinWindowFromID( hwnd, ID_LB_DUET),
                              TRUE);
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
                      * Stop the play button animation
                      */

                     WinSendMsg (
                        WinWindowFromID(
                           hwnd,             /* Dialog window handle      */
                           ID_GPB_PLAY),     /* Id - Play graphic button  */
                        GBM_ANIMATE,         /* Animation control         */
                        MPFROMSHORT(FALSE),  /* Animation flag            */
                        NULL);               /* Ignore return data        */

                     WinEnableWindow( WinWindowFromID( hwnd, ID_LB_DUET),
                           TRUE);
                     break;

               }
               break;

         }
         return( (MRESULT) 0);

      case HM_INFORM:
         /*
          * The user has selected the "Play Audio Help" selection in the
          * IPF help panel.
          *
          * To initiate the playing of audio help, we need to issue mciPlayFile.
          *
          * Note that we assume the HM_INFORM message came from the "Play
          * Audio Help" selection since it is the only :link. with an inform
          * reftype in the .ipf file.  If there were more, we would have to
          * check the resource identifier to determine for which selection
          * this message was generated.
          */

         /*
          * Load the name of the audio help file from the resource
          */
         WinLoadString( hab,
                        (HMODULE) NULL,
                        IDS_HELP_WAVEFILE,
                        (SHORT) sizeof( achAudioHelpFile),
                        achAudioHelpFile);

         ulError = mciPlayFile( (HWND)NULL,            /* Ignore owner window */
                                (PSZ)achAudioHelpFile, /* Audio file to play  */
                                MCI_ASYNC,             /* Flags               */
                                (PSZ)NULL,             /* Ignore title        */
                                (HWND)NULL );          /* Ignore viewport wnd */
         if (ulError)
         {
            ShowMCIErrorMessage( ulError);
         }
         return( (MRESULT) 0);

      default:
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2));

   }  /* End of msg Switch */

   return( (MRESULT) FALSE);

} /* End of MainDialogProc */


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
   BOOL  fHelpCreated = FALSE;


   /*
    * Load the strings for the Help window title and library name.
    */

   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_HELP_WINDOW_TITLE,
      (SHORT) sizeof( achHelpWindowTitle),
      achHelpWindowTitle);

   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_HELP_LIBRARY_NAME,
      (SHORT) sizeof( achHelpLibraryName),
      achHelpLibraryName);


   /*
    * Get the size of the initialization structure.
    */
   hmiHelpStructure.cb              = sizeof( HELPINIT);

   hmiHelpStructure.ulReturnCode    = (ULONG) 0;   /* RC from HM init      */
   hmiHelpStructure.pszTutorialName = (PSZ) NULL;  /* No tutorial program  */

   hmiHelpStructure.phtHelpTable    = (PVOID)(0xffff0000 | ID_DUET_HELPTABLE);


   /*
    * The action bar is not used, so set the following to NULL.
    */
   hmiHelpStructure.hmodAccelActionBarModule = (HMODULE) 0;
   hmiHelpStructure.idAccelTable             = (USHORT) 0;
   hmiHelpStructure.idActionBar              = (USHORT) 0;

   /*
    * The Help window title.
    */
   hmiHelpStructure.pszHelpWindowTitle  = achHelpWindowTitle;

   /*
    * The Help table is in the executable file.
    */
   hmiHelpStructure.hmodHelpTableModule = (HMODULE) 0;

   /*
    * The help panel ID is not displayed.
    */
   hmiHelpStructure.fShowPanelId       = (ULONG) 0;

   /*
    * The library that contains the help panels.
    */
   hmiHelpStructure.pszHelpLibraryName  = achHelpLibraryName;

   /*
    * Create the Help Instance for IPF.
    * Give IPF the Anchor Block handle and address of the IPF initialization
    * structure, and check that creation of Help was a success.
    */
   hwndHelpInstance = WinCreateHelpInstance(
                         hab,                   /* Anchor Block Handle.    */
                         &hmiHelpStructure );   /* Help Structure.         */

   if ( hwndHelpInstance == (HWND) NULL)
   {
      fHelpCreated = FALSE;
   }
   else
   {
      if ( hmiHelpStructure.ulReturnCode)
      {
         WinDestroyHelpInstance( hwndHelpInstance);
         fHelpCreated = FALSE;
      }
      else  /* help creation worked */
      {
         WinAssociateHelpInstance(
            hwndHelpInstance,     /* The handle of the Help Instance.      */
            hwndFrame);           /* Associate to this dialog window.      */

            fHelpCreated = TRUE;
      }
   }  /* End of IF checking the creation of the Help Instance. */

   /*
    * Associate the Help Instance of the IPF to this dialog window.
    */
   if (!fHelpCreated)
   {
      DuetMessageBox( IDS_DUET_PLAYER_ERROR,    /* ID of the title         */
                      IDS_HELP_CREATION_FAILED, /* ID of the message       */
                      MB_OK | MB_INFORMATION  | MB_MOVEABLE);  /* style    */
   }

}  /* End of InitializeHelp */


/*************************************************************************
 * Name         :  DuetMessageBox
 *
 * Description  :  This procedure will display messages for the application
 *                 based upon string IDs passed in.  The actual text will be
 *                 loaded from the string table in the resource.
 *
 * Concepts     :  None.
 *
 * MMPM/2 API's :  None.
 *
 * Parameters   :  usTitleID   - ID of the title string
 *                 usMessageID - ID of the message string
 *                 ulStyle     - Style of the message box (WinMessageBox)
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 *************************************************************************/
USHORT DuetMessageBox(  USHORT usTitleID,
                        USHORT usMessageID,
                        ULONG  ulStyle)
{
   CHAR     achTitle[LEN_ERROR_TITLE];
   CHAR     achMessage[LEN_ERROR_MESSAGE];
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
         hwndMainDialogBox,         /* Owner handle of the message box.    */
         achMessage,                /* String to show in the message box.  */
         achTitle,                  /* Title to shown in the message box.  */
         (USHORT) ID_MESSAGEBOX,    /* Message Box Id.                     */
         ulStyle );                 /* The style of the message box.       */

   return( usResult);

}  /* End of DuetMessageBox */



/*************************************************************************
 * Name         :  PlayTheDuet
 *
 * Description  :  This procedure will begin the playing of the duet.
 *
 * Concepts     :  Opening mci devices.
 *                 Creating a group.
 *                 Playing a group.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_OPEN
 *                                   MCI_PLAY
 *                                   MCI_CUE
 *                                   MCI_LOAD
 *                                   MCI_GROUP
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 * IMPORTANT NOTE ABOUT DEVICE SHARING UNDER MMPM/2:
 *
 * To be a well behaved MMPM/2 application, you should participate in
 * the system's device sharing scheme.  This is done by processing the
 * MM_MCIPASSDEVICE message and the WM_ACTIVATE message in the window
 * procedure of your application.  When you open the device, you specify
 * the handle of your window procedure as the callback window handle of
 * the MCI_OPEN_PARMS data structure.  This handle tells MMPM/2 what
 * window procedure to send the MM_MCIPASSDEVICE message to when control
 * of the device you're opening is gained or lost by your app. Also, when
 * you open the device, you specify if you want exclusive use of the
 * device, or if you're willing to share the device.  The Duet Player
 * opens all of its devices as shareable devices.  The MMPM/2 default
 * is to open the device exclusively unless otherwise specified by the
 * app.
 *
 * The implementation shown in the Duet Player Sample Program is just
 * one possible way to participate in MMPM/2's device sharing scheme.
 * This implementation was kept simple for the purposes of illustration.
 * More robust implementations are possible.
 *
 * For example, a more complex and robust way of implementing this device
 * sharing scheme may include logic to make sure we don't lose the device
 * between the time we open it and the time we play it.  Also, logic
 * could be added to make sure that another application doesn't have
 * exclusive use of the device when we open it.  The current implementation
 * in the Duet Player will simply display an error message to the user
 * when one of these two situations exist.
 *
 *************************************************************************/
BOOL PlayTheDuet( HWND hwnd)
{
   ULONG             ulError;                /* error value from mci calls    */
   SHORT             sDuet;                  /* duet number to play           */
   BOOL              bReturn = FALSE;        /* function return value         */
   MCI_OPEN_PARMS    mopDuetPart;            /* open parms for MCI_OPEN       */
   MCI_PLAY_PARMS    mppGroup;               /* play parms for MCI_PLAY       */
   ULONG             ulDeviceList[NUM_PARTS];/* array of device IDs to group  */
   MCI_GENERIC_PARMS mciGenericParms;        /* generic parms for MCI_CLOSE   */
   MCI_LOAD_PARMS    mlpLoad;                /* load parms for MCI_LOAD       */
   MCI_GROUP_PARMS   mgpGroupParms;          /* Group Parms for MCI_GROUP     */

   /* Get the duet selection from the listbox */

   sDuet = (USHORT) WinSendMsg( WinWindowFromID( hwnd, ID_LB_DUET),
                                LM_QUERYSELECTION,
                                (MPARAM) LIT_FIRST,
                                (MPARAM) NULL);

   /* this is a safety net - one should always be selected  */

   if (sDuet==(SHORT)LIT_NONE)
   {
      DuetMessageBox( IDS_DUET_PLAYER_ERROR,
                      IDS_NO_DUET_SELECTED,
                      MB_CANCEL | MB_HELP | MB_ERROR | MB_MOVEABLE);

      return( bReturn);
   }


   /*
    * Before we get ready to open the audio file, we will check to
    * see it the file exists.  This is important to do since we
    * can open a file that does not exist to record into it.
    */
   if ( DoesFileExist( aDuet[sDuet].achPart1) &&
        DoesFileExist( aDuet[sDuet].achPart2))
   {

      /*
       * Change pointer to a waiting pointer first, since this might take a
       * couple of seconds.
       */

      WinSetPointer(
         HWND_DESKTOP,        /* Desktop window handle.                    */
         WinQuerySysPointer(  /* This API will give the handle of the PTR. */
            HWND_DESKTOP,     /* Desktop window handle.                    */
            SPTR_WAIT,        /* The waiting icon.                         */
            FALSE ) );        /* Return the system pointer's handle.       */

      if (fFirstPlay)
      /* If this is the first time thru this routine, then we need to open
       * the devices and make the group.  That's what this IF is for.
       *
       * On subsequent calls to this routine, the devices are already open
       * and the group is already made, so we only need to load the
       * appropriate files onto the devices.
       */
      {
         /*
          * Open one part of the duet. The first step is to initialize an
          * MCI_OPEN_PARMS data structure with the appropriate information,
          * then issue the MCI_OPEN command with the mciSendCommand function.
          * We will be using an open with only the element name specified.
          * This will cause the default connection, as specified in the MMPM.INI
          * file, for the data type.
          */
         mopDuetPart.hwndCallback       = hwnd;         /* For MM_MCIPASSDEVICE */
         mopDuetPart.usDeviceID       = (USHORT)  NULL; /* this is returned   */
         mopDuetPart.pszDeviceType    = (PSZ)   NULL; /* using default conn.  */
         mopDuetPart.pszElementName   = (PSZ)   aDuet[sDuet].achPart1;

         ulError = mciSendCommand( (USHORT) 0,
                                   MCI_OPEN,
                                   MCI_WAIT | MCI_OPEN_ELEMENT |
                                   MCI_OPEN_SHAREABLE | MCI_READONLY,
                                   (PVOID) &mopDuetPart,
                                   UP_OPEN);

         if (!ulError)  /* if we opened part 1 */
         {
            usDuetPart1ID = mopDuetPart.usDeviceID;

            /*
             * Now, open the other part
             */
            mopDuetPart.pszElementName    = (PSZ)   aDuet[sDuet].achPart2;

            ulError = mciSendCommand( (USHORT) 0,
                                      MCI_OPEN,
                                      MCI_WAIT | MCI_OPEN_ELEMENT |
                                      MCI_OPEN_SHAREABLE | MCI_READONLY,
                                      (PVOID) &mopDuetPart,
                                      UP_OPEN);

            if (!ulError)  /* if we opened part 2 */
            {
               usDuetPart2ID = mopDuetPart.usDeviceID;

               /*
                * So far, so good.  Now we need to create a group.  To do this,
                * we need to fill an array with the ID's of the already open
                * devices that we want to group.  Then we call MCI_GROUP to
                * create the group and return us a handle to it.
                */

               ulDeviceList[0] = (ULONG)usDuetPart1ID;
               ulDeviceList[1] = (ULONG)usDuetPart2ID;

               mgpGroupParms.hwndCallback = (HWND) NULL;          /* Not needed - we're waiting */
               mgpGroupParms.ulNumDevices = NUM_PARTS;            /* Count of devices           */
               mgpGroupParms.paulDeviceID = (PULONG)&ulDeviceList;/* Array of devices           */
               mgpGroupParms.ulStructLength = sizeof (mgpGroupParms);

               ulError = mciSendCommand( (USHORT) 0,
                                         MCI_GROUP,
                                         MCI_WAIT | MCI_GROUP_MAKE|
                                         MCI_NOPIECEMEAL,
                                         (PVOID) &mgpGroupParms,
                                         UP_GROUP);

               fFirstPlay = FALSE;

               if (ulError!=MCIERR_SUCCESS)   /* if we did not made the group */
               {
                  WinSetPointer(
                     HWND_DESKTOP,        /* Desktop window handle.             */
                     WinQuerySysPointer(  /* This API gives handle of the PTR.  */
                        HWND_DESKTOP,     /* Desktop window handle.             */
                        SPTR_ARROW,       /* The Arrow icon.                    */
                        FALSE ) );        /* Return the sys pointer's handle.   */

                  DuetMessageBox( IDS_DUET_PLAYER_ERROR,
                                  IDS_CANNOT_MAKE_GROUP,
                                  MB_CANCEL | MB_HELP | MB_ERROR | MB_MOVEABLE);

                  /* we need to close parts 1 and 2 */

                  mciSendCommand( usDuetPart1ID,
                                  MCI_CLOSE,
                                  MCI_WAIT,
                                  (PVOID) &mciGenericParms,
                                  UP_CLOSE);

                  usDuetPart1ID = (USHORT) NULL;

                  mciSendCommand( usDuetPart2ID,
                                  MCI_CLOSE,
                                  MCI_WAIT,
                                  (PVOID) &mciGenericParms,
                                  UP_CLOSE);

                  usDuetPart2ID = (USHORT) NULL;
               }
               else  /* We did make the group */
               {
                  /* The handle of the group is returned in the usGroupID
                   * field of the structure.  Here, assign it to our global
                   * variable.
                   */
                  usGroupHandle = mgpGroupParms.usGroupID;
               }
            }
            else  /* we didn't open part 2 */
            {

               /*
                * Change the pointer back to an arrow.
                */

               WinSetPointer(
                  HWND_DESKTOP,        /* Desktop window handle.             */
                  WinQuerySysPointer(  /* This API gives handle of the PTR.  */
                     HWND_DESKTOP,     /* Desktop window handle.             */
                     SPTR_ARROW,       /* The Arrow icon.                    */
                     FALSE ) );        /* Return the sys pointer's handle.   */

               ShowMCIErrorMessage( ulError);

               /* we need to close part 1 */
               mciSendCommand( usDuetPart1ID,
                               MCI_CLOSE,
                               MCI_WAIT,
                               (PVOID) &mciGenericParms,
                               UP_CLOSE);

               usDuetPart1ID = (USHORT) NULL;
            }
         }
         else  /* we didn't open part 1 */
         {
            /*
             * Change the pointer back to an arrow.
             */

            WinSetPointer(
               HWND_DESKTOP,        /* Desktop window handle.             */
               WinQuerySysPointer(  /* This API gives handle of the PTR.  */
                  HWND_DESKTOP,     /* Desktop window handle.             */
                  SPTR_ARROW,       /* The Arrow icon.                    */
                  FALSE ) );        /* Return the sys pointer's handle.   */
            ShowMCIErrorMessage( ulError);
         }
      }
      else        /* Just load the files  */
      /* If this is the first time thru this routine, then we need to open
       * the devices and make the group.
       *
       * On subsequent calls to this routine, the devices are already open
       * and the group is already made, so we only need to load the
       * appropriate files onto the devices.  That's what this ELSE is for.
       */
      {
          /*
           * Load one part of the duet. The first step is to initialize an
           * MCI_LOAD_PARMS data structure with the appropriate information,
           * then issue the MCI_LOAD command with the mciSendCommand function.
           */
          mlpLoad.hwndCallback       = (ULONG) NULL;                   /* We're waiting */
          mlpLoad.pszElementName   = (PSZ)   aDuet[sDuet].achPart1;  /* part 1 of duet */

          ulError = mciSendCommand( usDuetPart1ID,
                                    MCI_LOAD,
                                    MCI_WAIT | MCI_READONLY,
                                    (PVOID) &mlpLoad,
                                    (ULONG) NULL);
          if (!ulError)  /* if we loaded part 1 of the duet on the first device */
          {
               /*
                * Now, open the other part
                */
               mlpLoad.pszElementName    = (PSZ)   aDuet[sDuet].achPart2;  /* part 2 of duet */

               ulError = mciSendCommand( usDuetPart2ID,
                                         MCI_LOAD,
                                         MCI_WAIT | MCI_READONLY,
                                         (PVOID) &mlpLoad,
                                         (ULONG) NULL);
              if (ulError)  /* if we could not loaded part 2 of the duet */
              {
                 /*
                  * Change the pointer back to an arrow.
                  */

                 WinSetPointer(
                    HWND_DESKTOP,        /* Desktop window handle.             */
                    WinQuerySysPointer(  /* This API gives handle of the PTR.  */
                       HWND_DESKTOP,     /* Desktop window handle.             */
                       SPTR_ARROW,       /* The Arrow icon.                    */
                       FALSE ) );        /* Return the sys pointer's handle.   */
                 ShowMCIErrorMessage (ulError);
              }
          }
          else /* if we couldn't load part one of the duet */
          {
             /*
              * Change the pointer back to an arrow.
              */

             WinSetPointer(
                HWND_DESKTOP,        /* Desktop window handle.             */
                WinQuerySysPointer(  /* This API gives handle of the PTR.  */
                   HWND_DESKTOP,     /* Desktop window handle.             */
                   SPTR_ARROW,       /* The Arrow icon.                    */
                   FALSE ) );        /* Return the sys pointer's handle.   */
             ShowMCIErrorMessage (ulError);
          }

      }
      if (ulError == MCIERR_SUCCESS)
      {
          eState = ST_PLAYING;               /* set state to PLAYING    */
          SetDuetVolume(hwnd);               /* set the starting volume */

          /*
           * OK, we've made the group - now we need to start it playing.
           * First, preroll the devices so they will both start playing
           * at precisely the same time without any delay.
           */
          mciSendCommand( usDuetPart1ID,              /* ID of device   */
                          MCI_CUE,                    /* CUE message    */
                          MCI_WAIT | MCI_CUE_OUTPUT,  /* standard flags */
                          (PVOID)&mciGenericParms,    /* generic struc  */
                          (USHORT)NULL);              /* no user parm   */

          mciSendCommand( usDuetPart2ID,              /* ID of device   */
                          MCI_CUE,                    /* CUE message    */
                          MCI_WAIT | MCI_CUE_OUTPUT,  /* standard flags */
                          (PVOID)&mciGenericParms,    /* generic struc  */
                          (USHORT)NULL);              /* no user parm   */

           /*
            * Next, we need to initialize an MCI_PLAY_PARMS structure with
            * the pertinent information then issue an MCI_PLAY command via
            * mciSendCommand.  This time we will be using the notification
            * version of the command - a MM_MCINOTIFY message will be sent
            * to the window specified in hwndCallback when the operation is
            * completed.  Since we want the whole audio file to be played,
            * we won't specify flags of MCI_FROM or MCI_TO so we don't
            * need to fill in these parameters in the structure.
            */

           mppGroup.hwndCallback = hwnd;            /* notify our window    */

           ulError = mciSendCommand( usGroupHandle,
                                     MCI_PLAY,
                                     MCI_NOTIFY,
                                     (PVOID) &mppGroup,
                                     UP_PLAY);

           /*
            * Change the pointer back to an arrow.
            */

           WinSetPointer(
              HWND_DESKTOP,        /* Desktop window handle.             */
              WinQuerySysPointer(  /* This API gives handle of the PTR.  */
                 HWND_DESKTOP,     /* Desktop window handle.             */
                 SPTR_ARROW,       /* The Arrow icon.                    */
                 FALSE ) );        /* Return the sys pointer's handle.   */

           if (ulError)
           {
              ShowMCIErrorMessage( ulError);

              /*
               * If we couldn't play - we need to halt audio gracefully
               * and get to stable, known state.
               */

              eState = ST_STOPPED;
              StopTheDuet(hwnd);
           }
           else
              bReturn = TRUE;
      }
   }
   else  /* cannot find audio file */
   {
      DuetMessageBox( IDS_DUET_PLAYER_ERROR,
                      IDS_CANNOT_FIND_AUDIO_FILE,
                      MB_CANCEL | MB_HELP | MB_ERROR | MB_MOVEABLE);
   }


   return( bReturn);

}  /* end of PlayTheDuet */



/*************************************************************************
 * Name         :  PauseTheDuet
 *
 * Description  :  This procedure will pause a duet that is playing.
 *
 * Concepts     :  Pausing a group.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_PAUSE
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  TRUE  -  if the operation was initiated without error.
 *                 FALSE -  if an error occurred.
 *
 *************************************************************************/
BOOL PauseTheDuet(HWND hwnd)
{
   ULONG                ulError;          /* return value for mci command  */
   MCI_GENERIC_PARMS    mciGenericParms;  /* info data structure for cmd.  */


   /*
    * Stop the play button animation
    */
   WinSendMsg (
      WinWindowFromID(
         hwnd,              /* Dialog window handle      */
         ID_GPB_PLAY),      /* Id - Play graphic button  */
      GBM_ANIMATE,          /* Animation control         */
      MPFROMSHORT(FALSE),   /* Animation flag            */
      NULL);                /* Ignore return data        */

   /*
    * To pause the duet, we will issue an MCI_PAUSE command via mciSendCommand
    * using an MCI_GENERIC_PARMS structure.  This pause command is done for
    * the group handle rather than the individual device IDs.  This will cause
    * the action to be performed for the each device in the group.
    */
   mciGenericParms.hwndCallback = hwndMainDialogBox;

   ulError = mciSendCommand( usGroupHandle,
                             MCI_PAUSE,
                             MCI_NOTIFY,
                             (PVOID) &mciGenericParms,
                             UP_PAUSE);

   if (ulError)
   {
      ShowMCIErrorMessage( ulError);
      return( FALSE);
   }
   else
      return( TRUE);

}  /* end of PauseTheDuet */



/*************************************************************************
 * Name         :  ResumeTheDuet
 *
 * Description  :  This procedure will resume the playing of the duet that
 *                 has been paused.
 *
 * Concepts     :  Resuming a group.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_RESUME
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  TRUE if the operation was initiated without error.
 *                 FALSE if an error occurred.
 *
 *************************************************************************/
BOOL ResumeTheDuet(HWND hwnd)
{
   ULONG                ulError;          /* return value for mci command  */
   MCI_GENERIC_PARMS    mciGenericParms;  /* info data structure for cmd.  */


   /*
    * To resume the duet, we will issue an MCI_RESUME command via mciSendCommand
    * using an MCI_GENERIC_PARMS structure.  This pause command is done for
    * the group handle rather than the individual device IDs.  This will cause
    * the action to be performed for the each device in the group.
    */
   mciGenericParms.hwndCallback = hwndMainDialogBox;

   ulError = mciSendCommand( usGroupHandle,
                             MCI_RESUME,
                             MCI_NOTIFY,
                             (PVOID) &mciGenericParms,
                             UP_RESUME);


   if (ulError)
   {
      ShowMCIErrorMessage( ulError);
      return( FALSE);
   }
   else
   {
      /*
       * Start the play button animation
       */
      WinSendMsg (
         WinWindowFromID(
            hwnd,             /* Dialog window handle      */
            ID_GPB_PLAY),     /* Id - Play graphic button  */
         GBM_ANIMATE,         /* Animation control         */
         MPFROMSHORT(TRUE),   /* Animation flag            */
         NULL);               /* Ignore return data        */
      return( TRUE);
   }

}  /* End of ResumeTheDuet */



/*************************************************************************
 * Name         :  StopTheDuet
 *
 * Description  :  This procedure will stop the duet that is playing or
 *                 paused.
 *
 * Concepts     :  Stopping a group.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_STOP
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  nothing.
 *
 *************************************************************************/
VOID StopTheDuet(HWND hwnd)
{
   ULONG                ulError;          /* return value for mci command  */
   MCI_GENERIC_PARMS    mciGenericParms;  /* info data structure for cmd.  */

   /*
    * Stop the play button animation
    */
   WinSendMsg (
      WinWindowFromID(
         hwnd,             /* Dialog window handle      */
         ID_GPB_PLAY),     /* Id - Play graphic button  */
      GBM_ANIMATE,         /* Animation control         */
      MPFROMSHORT(FALSE),  /* Animation flag            */
      NULL);               /* Ignore return data        */

   /*
    * To stop the duet, we will issue an MCI_STOP command via mciSendCommand
    * using an MCI_GENERIC_PARMS structure.  This pause command is done for
    * the group handle rather than the individual device IDs.  This will cause
    * the action to be performed for the each device in the group.
    */
   mciGenericParms.hwndCallback = hwndMainDialogBox;

   ulError = mciSendCommand( usGroupHandle,
                             MCI_STOP,
                             MCI_NOTIFY,
                             (PVOID) &mciGenericParms,
                             UP_STOP);
   if (ulError)
      ShowMCIErrorMessage( ulError);

   return;

}  /* end of StopTheDuet */



/*************************************************************************
 * Name         :  CloseTheDuet
 *
 * Description  :  This procedure will close both parts of the duet and
 *                 delete the group.
 *
 * Concepts     :  Deleting a group.
 *                 Closing mci devices.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_CLOSE
 *                                   MCI_GROUP
 *
 * Parameters   :  None.
 *
 * Return       :  nothing.
 *
 *************************************************************************/

VOID CloseTheDuet( VOID)
{
   ULONG                ulError;          /* return value for mci command  */
   MCI_GENERIC_PARMS    mciGenericParms;  /* info data structure for cmd.  */
   MCI_GROUP_PARMS   mgpGroupParms;       /* Group Parms for MCI_GROUP     */

   mciGenericParms.hwndCallback = hwndMainDialogBox;

   /*
    * Now, we will close the group.  This is done by issuing an
    * MCI_CLOSE command to the groups ID.  Also, we'll set the global
    * device IDs to NULL since they will no longer be valid.
    */
   ulError = mciSendCommand( usGroupHandle,
                             MCI_CLOSE,
                             MCI_NOTIFY,
                             (PVOID) &mciGenericParms,
                             UP_CLOSE);
   if (ulError)
      ShowMCIErrorMessage( ulError);

   usDuetPart1ID = (USHORT) NULL;
   usDuetPart2ID = (USHORT) NULL;

   /* Now, we need to kill the group - also we'll set its handle to NULL.  */

    mgpGroupParms.hwndCallback = (HWND) NULL;          /* Not needed - we're waiting */
    mgpGroupParms.usGroupID    = usGroupHandle;        /* Id of group to delete      */
    mgpGroupParms.ulStructLength = sizeof (mgpGroupParms);

    ulError = mciSendCommand( (USHORT) 0,
                                         MCI_GROUP,
                                         MCI_WAIT | MCI_GROUP_DELETE,
                                         (PVOID) &mgpGroupParms,
                                         UP_GROUP);
   usGroupHandle = (USHORT) NULL;

   return;

}  /* end of CloseTheDuet */



/*************************************************************************
 * Name         :  ShowMCIErrorMessage
 *
 * Description  :  This window procedure displays an MCI error message based
 *                 based upon a ulError return code.  The MCI function
 *                 mciGetErrorString is used to convert the error code into
 *                 a text string and the title is pulled from the resource
 *                 based upon a string id.
 *
 * Concepts     :  Using mciGetErrorString to convert an error code into
 *                 a textual message.  Error handling of mciGetErrorString.
 *
 * MMPM/2 API's :  mciGetErrorString
 *
 * Parameters   :  ulError  -  MCI error code.
 *
 * Return       :  nothing
 *
 *************************************************************************/
VOID  ShowMCIErrorMessage( ULONG ulError)
{
   CHAR  achTitle[LEN_ERROR_TITLE];
   CHAR  achBuffer[LEN_ERROR_MESSAGE];

   switch(mciGetErrorString( ulError, (PSZ)achBuffer,   sizeof( achBuffer)))
   {
      case MCIERR_SUCCESS:
         /*
          * This is what we want.  We were able to use mciGetErrorString to
          * retrieve a textual error message we can show in a message box.
          * Now, we need to load the string for the title of the message box.
          * Then, we'll show it to the user.
          */
         WinLoadString( hab,
                        (HMODULE) NULL,
                        IDS_DUET_PLAYER_ERROR,
                        (SHORT) sizeof( achTitle),
                        achTitle );

         WinMessageBox( HWND_DESKTOP,
                        hwndMainDialogBox,
                        achBuffer,
                        achTitle,
                        (USHORT) ID_MESSAGEBOX,
                        MB_CANCEL | MB_HELP | MB_ERROR | MB_MOVEABLE);
         break;

      case MCIERR_INVALID_DEVICE_ID:
      case MCIERR_OUTOFRANGE:
      case MCIERR_INVALID_BUFFER:
      default:
         DuetMessageBox( IDS_DUET_PLAYER_ERROR,
                         IDS_UNKNOWN,
                         MB_CANCEL | MB_HELP | MB_ERROR | MB_MOVEABLE);
         break;
   }

   return;

}  /* end of ShowMCIErrorMessage */



/*************************************************************************
 * Name         :  DoesFileExist
 *
 * Description  :  This helper function determines if a file with a given
 *                filename exists.
 *
 * Concepts     :  Using MMIO interface to access a data file.
 *
 * MMPM/2 API's :  mmioOpen
 *                 mmioClose
 *
 * Parameters   :  pszFilename -  The filename to be tested
 *
 * Return       :  TRUE  -  if the a file exists matching pszFilename
 *                 FALSE -  if the file does not exist
 *
 *************************************************************************/
BOOL DoesFileExist( PSZ pszFilename)
{
   BOOL  bReturn;    /* function return value   */
   HMMIO hFile;      /* handle to file          */

   /*
    * Notice that these MMIO functions are analogous to the standard
    * C functions, fopen and fclose.
    */

   hFile = mmioOpen( pszFilename, (PMMIOINFO) NULL, MMIO_READ);

   if (hFile != (HMMIO) NULL)
   {
      mmioClose( hFile, 0);
      bReturn = TRUE;
   }
   else
      bReturn = FALSE;

   return( bReturn);
}



/*************************************************************************
 * Name         :  SetDuetVolume
 *
 * Description  :  This helper function sets the duet volume based upon the
 *                 position of the volume slider.  The slider will be queried
 *                 and the duet volume will be set.
 *
 * Concepts     :  Setting the volume of a group.
 *
 * MMPM/2 API's :  mciSendCommand    MCI_SET
 *
 * Parameters   :  hwnd - Handle for the Main dialog box.
 *
 * Return       :  nothing.
 *
 *************************************************************************/
VOID SetDuetVolume( HWND hwnd)
{
   ULONG                ulError;       /* error value for mci returns   */
   MCI_WAVE_SET_PARMS   mspSet;        /* set values for volume, etc.   */


   if ((!fPassedDuet) && (eState==ST_PLAYING || eState==ST_PAUSED))
   {
      /*
       * To set the volume,  first, the MCI_SET_PARMS structure must be
       * filled with the necessary values.  Then an MCI_SET command
       * should be issued to each device to perform the volume change.
       */
      mspSet.hwndCallback = hwnd;
      mspSet.ulAudio    = MCI_SET_AUDIO_ALL;    /* set all channels     */
      mspSet.ulLevel    = (ULONG) sVolumeLevel; /* volume level desired */

      ulError = mciSendCommand( usDuetPart1ID,
                                MCI_SET,
                                MCI_NOTIFY | MCI_SET_AUDIO | MCI_SET_VOLUME,
                                (PVOID) &mspSet,
                                UP_VOLUME);

      ulError = mciSendCommand( usDuetPart2ID,
                                MCI_SET,
                                MCI_NOTIFY | MCI_SET_AUDIO | MCI_SET_VOLUME,
                                (PVOID) &mspSet,
                                UP_VOLUME);
      if (ulError)
         ShowMCIErrorMessage( ulError);
   }

   return;

}  /* end of SetDuetVolume */
