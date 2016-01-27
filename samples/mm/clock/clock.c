/*******************************************************************************
 * File Name   : CLOCK.C
 *
 * Description : This file contains the C source code required for the clock
 *               sample program.  This sample draws a clock in a dialog
 *               window and chimes for the user at every hour, quarter hour,
 *               half hour, and three-quarter hour.  The user can also play
 *               the next hour's chime by pushing a button.
 *
 * Concepts    : This program illustrates how to create and use a Memory
 *               Playlist.  The sample also shows how to use MMIO API's
 *               used in opening, reading, and closing files.
 *
 * MMPM/2 API's: List of all MMPM/2 API's that are used in this module.
 *
 *               mciQuerySysValue
 *               mciSendCommand
 *               mciGetErrorString
 *               mmioOpen
 *               mmioGetHeader
 *               mmioRead
 *               mmioClose
 *
 * Required
 *    Files    : clock.c         Source Code.
 *               clockdrw.c      Source Code.
 *               clock.h         Include file.
 *               clock.dlg       Dialog definition.
 *               clock.rc        Resources.
 *               clock.ipf       Help text.
 *               makefile        Make file.
 *               clock.def       Linker definition file.
 *               clock.ico       Clock icon.
 *               bellc.bmp       Center bell bitmap.
 *               belllt.bmp      Left bell bitmap.
 *               bellfrlt.bmp    Far left bell bitmap.
 *               bellrt.bmp      Right bell bitmap.
 *               bellfrrt.bmp    Far right bell bitmap.
 *               clock1.wav      Quarter hour chime file.
 *               clock2.wav      Half hour chime file.
 *               clock3.wav      Hour chime file.
 *
 * Copyright (C) IBM  1991, 1992, 1993
 ******************************************************************************/

#define  INCL_OS2MM                      /* Required for MCI & MMIO headers   */
#define  INCL_WIN                        /* Required to use Win APIs.         */
#define  INCL_PM                         /* Required to use PM APIs.          */
#define  INCL_GPIPRIMATIVES              /* Required to use GPI OS/2 APIs.    */
#define  INCL_DOSSEMAPHORES              /* Required to use Semaphores.       */
#define  INCL_DOSDATETIME                /* Required for get date time info.  */
#define  INCL_DOSPROCESS                 /* Required for OS/2 process APIs.   */
#define  INCL_WINHELP                    /* Required to use IPF.              */
#define  INCL_DOSMEMMGR                  /* Required to use memory API's.     */
#define  INCL_SECONDARYWINDOW            /* required for secondary window     */

#include <os2.h>
#include <os2me.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <bsememf.h>

#include <sw.h>

#include "clock.h"

/************************* Procedure/Function Prototypes **********************/
/*
 * This definition is required to use the sample program with the IBM
 * C compiler.
 */
#pragma linkage( MainDialogProc, system )
#pragma linkage( IncludeDialogProc, system )
#pragma linkage( DisplayDialogProc, system )
#pragma linkage( DisplayListBoxProc, system )
#pragma linkage( SwingTheBell, Optlink )

MRESULT EXPENTRY MainDialogProc( HWND hwnd,
                                 ULONG msg,
                                 MPARAM mp1,
                                 MPARAM mp2 );
USHORT           ShowAMessage( CHAR   *pcMessageTitle,
                               USHORT usMessageId,
                               ULONG  ulMessageBoxStyle );
ULONG            HowBigIsTheChimeFile( USHORT usChimeFileId );
VOID             InitializeDialog( VOID );
VOID             TerminateDialog( VOID );
VOID             CopyWaveformIntoMemory( LONG   *pulBaseAddress,
                                         ULONG  ulSizeOfBuffer,
                                         USHORT usChimeFile );
VOID             FindAndPlayTheCorrectChime( CHIME_TIME_VALUE_T ctvChimeTime,
                                             USHORT usHour );
VOID             FindTheNearestChimeTimeAndPlayIt( VOID );
VOID             SetupChimeFileInformation( VOID );
VOID             InitializeHelp( VOID );
VOID             LoadBellBitmaps( VOID );
VOID             SetupPlayList( VOID );
VOID             DealWithChiming( VOID );
VOID             SwingTheBell( PVOID pvData);
VOID             OpenPlaylistChimeDevice( ULONG  ulOpenFlags,
                                         USHORT usChimeFileToPlay );
BOOL             IsItChimeTime( CHIME_TIME_VALUE_T *ctvChimeTime,
                                USHORT             *pusNowHour );
MRESULT EXPENTRY DrawTheHourHand( HPS     hps,
                                  PPOINTL pptlCircleCenter,
                                  USHORT  usHour,
                                  USHORT  usMinute);
VOID             DrawTheClockFace( HPS     hps,
                                   PPOINTL pptlCircleCenter );
MRESULT EXPENTRY DrawTheMinuteHand( HPS     hps,
                                    PPOINTL pptlCircleCenter,
                                    USHORT  usMinute);
VOID             DrawClockCircles( HPS     hps,
                                   PPOINTL pptlMarkerPosition,
                                   LONG    lCircleColor,
                                   USHORT  usCircleSize );
VOID             UpdateTheClock( VOID);
/*************** End of Procedure/Function Prototypes *************************/


/****************************** Global Variables. *****************************/

/*
 * This double array holds the playlists that will be used to play the chimes
 * for the clock.  Each array has four fields within the structure, one for
 * the playlist command( 32 bit value ) and three operands( 32 bit values ).
 * The DATA_OPERATION's first operand will contain the address to the
 * respective waveform buffers.  Once the playlist has been played, the
 * CHIME_PLAYING_HAS_STOPPED message will be sent so that the application
 * knows that the audio is finished.
 *
 * The clock will have a unique chime for each quarter hour.
 * There are three chime files that are used in different combinations to
 * create all of the chimes used for the clock.  These three files are
 * clock1.wav, clock2.wav, and clock3.wav.
 *
 * The first playlist will play quarter hour chime.  This is simply
 * clock1.wav.
 *
 * The second playlist will play the half hour chime.  This
 * consists of clock1.wav + clock2.wav.
 *
 * The third playlist will play the three quarter hour chime.  This consists
 * of clock1.wav + clock2.wav + clock1.wav.
 *
 * The fouth playlist plays the hour chime.  This consists of clock1.wav +
 * clock2.wav + clock1.wav + clock2.wav + (HOUR * clock3.wav)
 * The Number of loops to perform for the hour value
 * is kept in the high order 16 bits of the first operand.  This will be set
 * in a later procedure when the hour of the chime time is known.
 */
PLAY_LIST_STRUCTURE_T apltPlayList[ NUMBER_OF_PLAYLISTS ]
                                  [ NUMBER_OF_COMMANDS ] =
{
   /*
    * Quarter Hour Chime.
    */
   {
      DATA_OPERATION,    0, 0, 0,      /* Chime file 1.                       */
      MESSAGE_OPERATION, 0, CHIME_PLAYING_HAS_STOPPED, 0,
      EXIT_OPERATION,    0, 0, 0
   },

   /*
    * Half Hour Chime.
    */
   {
      DATA_OPERATION,    0, 0, 0,      /* Chime file 1.                       */
      DATA_OPERATION,    0, 0, 0,      /* Chime file 2.                       */
      MESSAGE_OPERATION, 0, CHIME_PLAYING_HAS_STOPPED, 0,
      EXIT_OPERATION,    0, 0, 0
   },

   /*
    * Three Quarter Hour Chime.
    */
   {
      DATA_OPERATION,    0, 0, 0,      /* Chime file 1.                       */
      DATA_OPERATION,    0, 0, 0,      /* Chime file 2.                       */
      DATA_OPERATION,    0, 0, 0,      /* Chime file 1.                       */
      MESSAGE_OPERATION, 0, CHIME_PLAYING_HAS_STOPPED, 0,
      EXIT_OPERATION,    0, 0, 0
   },

   /*
    * Hour Chime.
    */
   {
      DATA_OPERATION,    0, 0, 0,      /* Chime file 1.             < Line 0 >*/
      DATA_OPERATION,    0, 0, 0,      /* Chime file 2.             < Line 1 >*/
      DATA_OPERATION,    0, 0, 0,      /* Chime file 1.             < Line 2 >*/
      DATA_OPERATION,    0, 0, 0,      /* Chime file 2.             < Line 3 >*/
      LOOP_OPERATION,    0, 7, 0,      /* Which line to end on      < Line 4 >*/
      DATA_OPERATION,    0, 0, 0,      /* Chime file 3.             < Line 5 >*/
      BRANCH_OPERATION,  0, 4, 0,      /* Branch back to Loop       < Line 6 >*/
      MESSAGE_OPERATION, 0, CHIME_PLAYING_HAS_STOPPED, 0,
      EXIT_OPERATION,    0, 0, 0
   }
};

/*
 * This array holds the ID's that are used to retrieve the strings from
 * the string table and placed into the global double array acStringBuffer.
 * The defines that are used for the string table is used to create the
 * index into the global buffer.  Since the string table starts at 1 and
 * the global buffer starts at 0, the index is decremented by 1 when used.
 * This means when the acStringBuffer is accessed after the strings have
 * been loaded into the buffer, all indexes will be n-1, i.e.,
 * IDS_HELP_WINDOW_TITLE - 1, to get the proper string.  This is done
 * simply to keep down the number of #defines.
 */
USHORT ausMessageId[] = { IDS_MAIN_WINDOW_TITLE,
                          IDS_HELP_CREATION_ERROR,
                          IDS_HELP_LIBRARY_FILE,
                          IDS_HELP_WINDOW_TITLE,
                          IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT,
                          IDS_SWING_THREAD_NOT_STARTED,
                          IDS_CHIME_FILE_A,
                          IDS_CHIME_FILE_B,
                          IDS_CHIME_FILE_C };

/*
 * This shows the order in which the bell bitmaps will be drawn so that the
 * visible 'ringing' of the bell can be seen.  The bell will start at the
 * center position, move to the right, move back to center, as it moves to the
 * left, and finally back to the center position where it started.
 */
USHORT ausBellSwingOrder[ NUMBER_OF_BELLS_IN_A_SWING ] =
     {
        IX_CENTER_BELL,
        IX_RIGHT_BELL,
        IX_FAR_RIGHT_BELL,
        IX_RIGHT_BELL,
        IX_CENTER_BELL,
        IX_LEFT_BELL,
        IX_FAR_LEFT_BELL,
        IX_LEFT_BELL,
     };

/*
 * This array holds program required text that is pulled from the string
 * table.
 */
CHAR  acStringBuffer[ NUMBER_OF_PROGRAM_STRINGS ][ STRING_SIZE ];

HWND  hwndHelpInstance;                              /* Handle to Help window.*/
HAB   hab;                                           /* Handle to anchor blk. */
HPS   hps;                                           /* Handle to pres space. */
HPS   hpsMemory;                                     /* Handle to pres space. */
HPS   hpsScreen;                                     /* Handle to pres space. */
QMSG  qmsg;                                          /* Message queue.        */
HMQ   hmq;                                           /* Handle to the msg q.  */
HWND  hwndMainDialogBox;                             /* Handle to the dlg win.*/
HWND  hwndPlayChimePB;                               /* Handle to the Play PB */
HWND  hwndFrame;                                     /* Handle to Frame win.  */
RECTL rctClockFaceRectangle;                         /* Area of the clk face. */
HEV                hevContinueToSwingTheBell;        /* When to swing bell.   */
STOP_GO_READY_T    yngContinueToSwingTheBell = STOP, /* When to swing bell.   */
                   yngAreChimeFilesLoaded    = READY;/* Have files been loaded*/
MCI_OPEN_PARMS     mciOpenParameters;                /* Open structure.       */
HELPINIT           hmiHelpStructure;                 /* Help init. structure. */
HBITMAP ahbmBellBitMaps[ NUMBER_OF_BELL_BITMAPS ],   /* Holds the handles to  */
        hbmBitmapToShow;                             /* the bell bitmaps.     */
BOOL    fThreadIsAlive              = FALSE,         /* Has thread started.   */
        fPassed                     = FALSE;         /* device  ownership     */
MMAUDIOHEADER mmAudioHeader;                         /* Contains info for SET */
BOOL    fInitHeader = TRUE;                          /* Should we init header?*/
USHORT  usLastHour   = 99, /* The last hour when in the Draw Hands proc.      */
        usLastMinute = 0,  /* The last minute when in the Draw Minute proc.   */
        usHour       = 25, /* Contains current hour given by UpdateTheClock.  */
        usMinute     = 61; /* Contains current minute given by UpdateTheClock.*/
                           /* The 3 in the usHour is required so that the     */
                           /* hands do not start in the same overlapping      */
                           /* position.                                       */
POINTL   aptl[4];                /* for bit blt                            */
BITMAPINFOHEADER    bmpInfo;     /* bitmap info                            */

/************************** End of Globle Variables ***************************/


/*******************************************************************************
 * Name         : main
 *
 * Description  : This function calls the InitializeDialog and TerminateDialog
 *                procedures.  The message loop is also in this routine.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : None.
 *
 * Return       : TRUE is returned to the operating system.
 *
 ******************************************************************************/
INT main( VOID )
{

   InitializeDialog();

   while ( WinGetMsg( hab, (PQMSG) &qmsg, (HWND) NULL, 0, 0) )
      WinDispatchMsg( hab, (PQMSG) &qmsg );

   TerminateDialog();

   return( TRUE );

} /* End of main */


/*******************************************************************************
 * Name         : InitializeDialog
 *
 * Description  : This function performs the necessary initializations and
 *                setups that are required to show/run a dialog box as a
 *                main window.  The message queue will be created, as well as
 *                the dialog box.  Finally the message passing loop will be
 *                started to give messages to the dialog box.
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
VOID InitializeDialog( VOID )
{

   USHORT   usCounter  = 0;              /* Generic counter.                  */
   BYTE  bThreadStackSize[ THREAD_STACK_SIZE ];         /* Thread stack size.    */

   DosCreateEventSem(
      (PSZ) NULL,                         /* No semaphore Name. Do not share. */
      &hevContinueToSwingTheBell,         /* Semaphore Handle.                */
      (ULONG) NULL,                       /* Creation attributes.             */
      (BOOL32) FALSE );                   /* State of semaphore.              */

   /*
    * Create the thread that will animate the bell bitmaps.
    */
   if ( _beginthread(
           SwingTheBell,                  /* Routine to run in the thread.    */
           NULL,                          /* What parameters to pass. Ignore  */
           (ULONG)(bThreadStackSize +
                       sizeof( bThreadStackSize )),   /* Thread stack size.   */
           NULL) == -1 )
   {                                      /* What parameters to pass. Ignore  */
      fThreadIsAlive = FALSE;
   }
   else
   {
      fThreadIsAlive = TRUE;
   }

   /*
    * Load the various strings that are required by the program.
    */
   for( usCounter=0; usCounter<NUMBER_OF_PROGRAM_STRINGS; usCounter++)
   {
      WinLoadString(
         hab,                                  /* HAB for this dialog box.    */
         (HMODULE) NULL,                       /* Get string from .exe file.  */
         ausMessageId[ usCounter ],            /* Which string to get.        */
         (SHORT) STRING_SIZE,                  /* The size of the buffer.     */
         (PSZ) &acStringBuffer[ usCounter ] ); /* Buffer to place string.     */
   }

   /*
    * Setup and initialize the dialog window.
    */
   hab = WinInitialize( (USHORT) 0 );

   hmq = WinCreateMsgQueue( hab, 0 );

   hwndFrame =                  /* Returns the handle to the frame.           */
      WinLoadSecondaryWindow(
         HWND_DESKTOP,          /* Parent of the dialog box.                  */
         HWND_DESKTOP,          /* Owner of the dialog box.                   */
         (PFNWP) MainDialogProc,/* 'Window' procedure for the dialog box.     */
         (HMODULE) NULL,        /* Where is the dialog.  Null is EXE file..   */
         ID_CLOCK_DIALOG_BOX,   /* Dialog ID.                                 */
         (PVOID) NULL );        /* Creation Parameters for the dialog.        */

   /*
    * Retrieve the handle to the dialog box by specifying the QS_DIALOG flag.
    */
   hwndMainDialogBox = WinQuerySecondaryHWND(hwndFrame, QS_DIALOG);

   /*
    * Set the name of the sample program in the title bar of the dialog
    * window.  This is done so that the program name will show up in
    * the Task List window, via the FCF_TASKLIST flag AND will NOT
    * show the .exe file name of the program.  If the FCF_TASKLIST flag
    * is used the .exe file name AND the dialog title will show up
    * in the title bar.  This is not wanted.
    */
   WinSetWindowText(hwndFrame, acStringBuffer[IDS_MAIN_WINDOW_TITLE-1]);

   /*
    * Initialize the help structure and associate the help instance to this
    * dialog via it's handle to anchor block.
    */
   InitializeHelp();

} /* End of InitializeDialog */


/*******************************************************************************
 * Name         : TerminateDialog
 *
 * Description  : This routine is called after the message dispatch loop has
 *                ended because of a WM_QUIT message.  The code will destroy
 *                the help instance, messages queue, window, and bitmaps.
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
VOID TerminateDialog( VOID )
{
   USHORT usCounter = 0;                        /* Generic counter.           */

   /*
    * Destroy the Help Instance for this dialog window.
    */
   if ( hwndHelpInstance != (HWND) NULL )
   {
      WinDestroyHelpInstance( hwndHelpInstance );
   }

   WinDestroySecondaryWindow( hwndFrame);
   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );

   /*
    * Get rid of the clock bitmaps.
    */
   for( usCounter=0; usCounter<NUMBER_OF_BELL_BITMAPS; usCounter++)
   {
      GpiDeleteBitmap( ahbmBellBitMaps[ usCounter ] );
   }

   /*
    * Get rid of the space that was malloc to hold the clock waveform
    * files.
    */
   for( usCounter=0; usCounter<NUMBER_OF_AUDIO_CHIMES; usCounter++ )
   {
      free( (VOID *) apltPlayList[ usCounter ][ 0 ].ulOperandOne );
   }

}  /* End of TerminateDialog */


/*******************************************************************************
 * Name        : MainDialogProc
 *
 * Description : This function controls the main dialog box.  It will handle
 *               received messages such as pushbutton notifications, timing
 *               events, etc.
 *
 *               The following messages are handled specifically by this
 *               routine.
 *
 *                  WM_INITDLG
 *                  WM_CLOSE
 *                  WM_PAINT
 *                  WM_TIMER
 *                  WM_HELP
 *                  WM_COMMAND
 *                  WM_MINMAXFRAME
 *                  MM_MCINOTIFY
 *
 * Concepts    : The MM_MCINOTIFY message is used to indicate when to turn
 *               off the swinging of the Bells when the playlist is finished
 *               processing.
 *
 *               In order to update the clock hands when the time has changed,
 *               an OS/2 timer is used to call this window procedure when a
 *               period of time has passed.  When the WM_TIMER message is
 *               received the current time will be compared with the last time
 *               that the clock hands were updated.  If the time is different
 *               then the clock hands will be drawn in positions reflecting
 *               the new time.
 *
 *               The WM_TIMER message will cause the UpdateTheClock routine to
 *               be called which will check the time values and if necessary
 *               invalidate the clock face.  This will cause the clock hands
 *               to be drawn via the WM_PAINT message.
 *
 *               Another routine DealWithChiming is also called to see if it
 *               is time to chime and if the Closed Caption flag is set in
 *               the system.  If it is a Chime Time then the audio chime for
 *               the particular time will be played and if the Closed Caption
 *               flag is set the Bell bitmap will be swung back and forth to
 *               indicate that a chime time has been reached.
 *
 *               Below is a diagram of the the routines called by
 *               DealWithChiming.
 *
 *                                      DealWithChiming()
 *                                              ³
 *                                              ³
 *                               FindAndPlayTheCorrectChime(...)
 *                                              ³
 *                             ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
 *                     IsItChimeTime(...)                ³
 *                                                       ³
 *                                            OpenPlaylistChimeDevice(...)
 *
 *
 * MMPM/2 API's : mciSendCommand
 *
 *
 * Parameters   : hwnd - Handle for the Main dialog box.
 *                msg  - Message received by the dialog box.
 *                mp1  - Parameter 1 for the just recieved message.
 *                mp2  - Parameter 2 for the just recieved message.
 *
 * Return       :
 *
 *
 ******************************************************************************/
MRESULT EXPENTRY MainDialogProc( HWND   hwnd,
                                 ULONG  msg,
                                 MPARAM mp1,
                                 MPARAM mp2 )
{
   static PSWP pswpWindowActionMP    = (PSWP) NULL; /* State of window.       */
   static BOOL fIsTheWindowMinimized = FALSE;   /* Flag for the window state. */
   MCI_GENERIC_PARMS mciGenericParms;/* generic parms for MCI_ACQUIRE         */
   HDC      hdcScreen;
   HDC      hdcMemory;
   HPOINTER hpProgramIcon;          /* Pointer to the program icon.           */
   POINTL   ptlCircleCenter;        /* Point of the center of the circle.     */
   USHORT   usReceivedMessage,      /* MCI message that was received.         */
            usNotifyCode,           /* Notify code from the MCI Notify msg.   */
            usMCIUserParameter;     /* User parameter.                        */
   ULONG    ulPostCount;            /* Count of number of Posts for the Sem.  */
   RECTL    rctWindowRectangle;     /* The area rectangle of the window.      */
   ULONG    ulError;                /* RC for acquire device.                 */
   SIZEL    sizel;                  /* for GpiCreatePS.                       */

   switch( msg )
   {

      /*
       * The WM_INITDLG is called when the dialog is first being created
       * by the system.
       */
      case WM_INITDLG :
         /*
          * Since the startup of this program can take more than a second,
          * the hourglass icon will be shown to the user.  The main reason
          * for the delay in startup is the chime files that are read in
          * when the program is started.
          */
         WinSetPointer(
            HWND_DESKTOP,        /* Desktop window handle.                    */
            WinQuerySysPointer(  /* This API will give the handle of the PTR. */
               HWND_DESKTOP,     /* Desktop window handle.                    */
               SPTR_WAIT,        /* The Hourglass icon.                       */
               FALSE ) );        /* Return the system pointer's handle.       */

         /*
          * Get the sample programs minimize icon.
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
          * Open screen device context (DC) and create presentation space (PS)
          * associated with it. This DC is a real output device.
          */
         hdcScreen = WinOpenWindowDC ( hwnd );

         sizel.cx = 0L;
         sizel.cy = 0L;
         hpsScreen =
            GpiCreatePS (
               hab,                               /* anchor block             */
               hdcScreen,                         /* memory device context    */
               &sizel,                            /* page size data structure */
               GPIT_NORMAL | PU_LOENGLISH |       /* flags                    */
               GPIA_ASSOC );

         /*
          * Open memory device context (DC) and create presentation space (PS)
          * associated with it. This DC is not a output device and exists only
          * in the memory.
          */
         hdcMemory =
            DevOpenDC(
               hab,                               /* anchor block             */
               OD_MEMORY,                         /* DC type                  */
               "*",                               /* token not used           */
               0L,                                /* no data struct count     */
               NULL,                              /* no data strct            */
               (ULONG) NULL );                    /* compatible DC            */

         sizel.cx = 0;
         sizel.cy = 0;
         hpsMemory =
            GpiCreatePS(
               hab,                               /* anchor block             */
               hdcMemory,                         /* memory device context    */
               &sizel,                            /* page size data structure */
               PU_PELS | GPIT_MICRO | GPIA_ASSOC);/* flags                    */

         /*
          * Load the center bell bitmap into memory PS.
          */
         hbmBitmapToShow =
            GpiLoadBitmap(
               hpsMemory,
               (HMODULE) NULL,
               ID_CENTER_BELL,
               (LONG) BELL_BITMAP_X_SIZE,
               (LONG) BELL_BITMAP_Y_SIZE );

         /*
          * Display the center bell bitmap and query the bitmap parameters.
          */
         GpiSetBitmap( hpsMemory, hbmBitmapToShow );
         GpiQueryBitmapParameters( hbmBitmapToShow, &bmpInfo );

         /*
          * Load the bitmaps into the hpsMemory.
          */
         LoadBellBitmaps();

         /*
          * Load the playlist waveform files into memory and set the
          * DATA_OPERATION's first operand to the address of the buffer
          * containing the files.
          */
         SetupPlayList();

         /*
          * Place the clock hands in position based on the time.
          */
         UpdateTheClock();

         /*
          * Start the timer so that we know to check to see when the time
          * has changed so that we can move the clock hands.
          */
         WinStartTimer(
            hab,                    /* Anchor Block for the window.           */
            hwnd,                   /* Handle for the window.                 */
            CLOCK_TIMER_ID,         /* Id for this particular Timer.          */
            CLOCK_TIMER_TIMEOUT );  /* Number of seconds that the timer waits.*/

         /*
          * Now that the startup processing is over, remove the Hourglass
          * icon and replace it with the normal arrow icon.
          */
         WinSetPointer(
            HWND_DESKTOP,        /* Desktop window handle.                    */
            WinQuerySysPointer(  /* This API will give the handle of the PTR. */
               HWND_DESKTOP,     /* Desktop window handle.                    */
               SPTR_ARROW,       /* The Arrow icon.                           */
               FALSE ) );        /* Return the system pointer's handle.       */

         hwndPlayChimePB =
            WinWindowFromID(
               hwnd,                /* Handle to the window.                  */
               ID_PLAY_CHIME_PB );  /* Id of the control.                     */

         return( 0 );

      case WM_CLOSE :
         if ( mciOpenParameters.usDeviceID > 0 )
         {
            mciSendCommand(
               mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
               MCI_CLOSE,                    /* MCI message.                  */
               MCI_WAIT,                     /* Flags for the MCI message.    */
               (ULONG) NULL,                 /* Parameters for the message.   */
               (ULONG) NULL );               /* Parameter for notify message. */
         }

         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case WM_TIMER :
         /*
          * A period of time has passed so check the time and see if the
          * clock hands need to be repositioned.  Also see if Chiming should
          * take place.
          */
         UpdateTheClock();
         DealWithChiming();
         return( 0 );

      case WM_HELP :
         /*
          * The dialog window has recieved a request for help from the user,
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

      case MM_MCINOTIFY :
         /*
          * A MCI Notify message has been recieved as a result of an MCI_PLAY.
          * Process as required.
          */
         usNotifyCode       = SHORT1FROMMP( mp1 );
         usReceivedMessage  = SHORT2FROMMP( mp2 );
         usMCIUserParameter = SHORT2FROMMP( mp1 );

         /*
          * If for some reason the Notify code is not successful then the
          * file will be closed OR if the User parameter indicates that the
          * Playlist sent a message indicating that the file has been played.
          */
         if ( ( usNotifyCode != MCI_NOTIFY_SUPERSEDED &&
                usNotifyCode != MCI_NOTIFY_ABORTED &&
                usNotifyCode != MCI_NOTIFY_SUCCESSFUL ) ||
              usNotifyCode == MCI_NOTIFY_SUPERSEDED ||
              usNotifyCode == MCI_NOTIFY_ABORTED ||
              (usNotifyCode == MCI_NOTIFY_SUCCESSFUL &&
                 usReceivedMessage == MCI_PLAY ) ||
              usMCIUserParameter == CHIME_PLAYING_HAS_STOPPED )
         {
            /*
             * Turn off the swinging of the visual bells and clean up the
             * audio device.
             */
            yngContinueToSwingTheBell = STOP;
            DosResetEventSem(
               hevContinueToSwingTheBell,    /* Semaphore Handle.             */
               &ulPostCount );               /* How many times Posted.        */

            mciSendCommand(
               mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
               MCI_CLOSE,                    /* MCI message.                  */
               MCI_WAIT,                     /* Flags for the MCI message.    */
               (ULONG) NULL,                 /* Parameters for the message.   */
               (ULONG) NULL );               /* Parameter for notify message. */

            WinEnableWindow(
               hwndPlayChimePB,     /* Handle to the Play Chime push button.  */
               TRUE );              /* Enable the push button.                */

         }

         return( 0 );

      case WM_COMMAND :
         /*
          * To get which pushbutton was pressed the SHORT1FROMMP macro
          * is used.
          */
         switch( SHORT1FROMMP( mp1 ) )
         {
            case ID_PLAY_CHIME_PB :
               /*
                * Since the user want to play the next hour chime, the next
                * procedure call will get the current hour and increase it
                * to the next hour.  However this will only occur if all of
                * the chime files have been loaded correctly.
                */
               if ( yngAreChimeFilesLoaded == STOP )
               {
                  ShowAMessage(
                     acStringBuffer[
                        IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
                     IDS_CHIME_FILE_ERROR,
                     MB_CANCEL | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
                        MB_MOVEABLE );
               }
               else
               {
                  FindTheNearestChimeTimeAndPlayIt();
               }

               return( 0 );

            case DID_CANCEL:                      /* Handle ESC key */
            case ID_CANCEL_PB :
               WinSendMsg(
                  hwnd,
                  WM_CLOSE,
                  (MPARAM) NULL,
                  (MPARAM) NULL );

               return( 0 );

         }  /* End of Command Switch */

         return( 0 );

      case WM_MINMAXFRAME :

         hwndPlayChimePB =
            WinWindowFromID(
               hwnd,                /* Handle of this dialog window.          */
               ID_PLAY_CHIME_PB );  /* ID of the left most PM control.        */

         pswpWindowActionMP = (PSWP) LONGFROMMP( mp1 );

         if ( pswpWindowActionMP->fl & SWP_MINIMIZE )
         {
            fIsTheWindowMinimized = TRUE;
            WinSetWindowPos(
               hwndPlayChimePB,        /* Handle to the control.              */
               HWND_BOTTOM,            /* Place the control behind siblings.  */
               X_PLAY_CHIME_POSITION,  /* X position of the control.          */
               Y_PLAY_CHIME_POSITION,  /* Y position of the control.          */
               X_SIZE_OF_THE_BUTTONS,  /* X size of the control.              */
               Y_SIZE_OF_THE_BUTTONS,  /* Y size of the control.              */
               SWP_HIDE | SWP_ACTIVATE ); /* Positioning options.             */
         }
         else
         {
            fIsTheWindowMinimized = FALSE;
            WinSetWindowPos(
               hwndPlayChimePB,        /* Handle to the control.              */
               HWND_BOTTOM,            /* Place the control behind siblings.  */
               X_PLAY_CHIME_POSITION,  /* X position of the control.          */
               Y_PLAY_CHIME_POSITION,  /* Y position of the control.          */
               X_SIZE_OF_THE_BUTTONS,  /* X size of the control.              */
               Y_SIZE_OF_THE_BUTTONS,  /* Y size of the control.              */
               SWP_SHOW | SWP_ACTIVATE ); /* Positioning options.             */
         }

         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case WM_PAINT :
      {
         if ( fIsTheWindowMinimized == FALSE )
         {
            hps =
               WinBeginPaint(
                  hwnd,             /* Window that wants PS.                  */
                  (HPS) NULL,       /* Get a Cache Pres. Space.               */
                  (PRECTL) NULL );

            GpiSetLineWidth(
               hps,
               LINEWIDTH_NORMAL * 3 );

            /*
             * Get the size of the window.
             */
            WinQueryWindowRect(
               hwnd,                            /* Handle to the window.      */
               (PRECTL) &rctWindowRectangle );  /* Size of the window.        */

            /*
             * Fill the presentation space.
             */
            WinFillRect(
               hps,                             /* Presentation Space.        */
               (PRECTL) &rctWindowRectangle,    /* Size of the window.        */
               SYSCLR_FIELDBACKGROUND);         /* The background color.      */

            /*
             * Calculate the center of the window.
             */
            ptlCircleCenter.x =
               ( ( rctWindowRectangle.xRight - rctWindowRectangle.xLeft ) / 2 );
            ptlCircleCenter.y =
               ( ( rctWindowRectangle.yTop - Y_ADJUSTMENT_SIZE_OF_BUTTONS -
                  rctWindowRectangle.yBottom ) / 2 );

            /*
             * Calculate the rectangular area of the clock face by using the
             * center point of the window and the macros that discribe its
             * size.  The values calculated below are used to invalidate the
             * portions of the window so that the clock hands can be re-drawn
             * when the time changes.  The area that is invalidated is a
             * square that is the two minute hand lengths from side to side.
             */
            rctClockFaceRectangle.xLeft   =
               ptlCircleCenter.x - (LONG)
                  (MARKER_POSITION_FROM_CENTER - 25 );
            rctClockFaceRectangle.yBottom =
               ptlCircleCenter.y - (LONG)
                  (MARKER_POSITION_FROM_CENTER - 25 );

            rctClockFaceRectangle.xRight  =
               ptlCircleCenter.x + (LONG)
                  (MARKER_POSITION_FROM_CENTER - 25 );
            rctClockFaceRectangle.yTop    =
               ptlCircleCenter.y + (LONG)
                  (MARKER_POSITION_FROM_CENTER - 25 );

            /*
             * Draw the clock face, the hour, minute hands, and center circle.
             *   Order is important when drawing the hour and minute hands.
             *   Since the minute hand is longer than the hour, the minute
             *   hand should be drawn first followed by the hour hand.
             */
            DrawTheClockFace(
               hps,                 /* Presentation Space.                    */
               &ptlCircleCenter );  /* Point to the center of the clock face. */

            DrawTheMinuteHand(
               hps,                 /* Presentation Space.                    */
               &ptlCircleCenter,    /* Point to the center of the clock face. */
               usMinute);           /* Minute value for the clock hand.       */

            DrawTheHourHand(
               hps,                 /* Presentation Space.                    */
               &ptlCircleCenter,    /* Point to the center of the clock face. */
               usHour,              /* Hour value for the clock hand.         */
               usMinute);           /* Minute value for the clock hand.       */

            DrawClockCircles(
               hps,                 /* Handle to the Presentation space.      */
               &ptlCircleCenter,    /* Position to place the Marker.          */
               CLOCK_HAND_COLOR,    /* Color to make the end dot.             */
               (USHORT) 5 );        /* Size of the time marker circle.        */

            /*
             * Set up target rectangle.
             */
            aptl[0].x = rctWindowRectangle.xRight - BELL_BITMAP_X_SIZE - 25;
            aptl[0].y = rctWindowRectangle.yBottom + Y_BITMAP_POSITION;
            aptl[1].x = aptl[0].x + BELL_BITMAP_X_SIZE;
            aptl[1].y = aptl[0].y + BELL_BITMAP_Y_SIZE + 10;
            aptl[2].x = 0;
            aptl[2].y = 0;
            aptl[3].x = bmpInfo.cx-1;
            aptl[3].y = bmpInfo.cy-1;

            /*
             * Draw the bitmap.
             */
            GpiBitBlt(
               hpsScreen,                     /* target presentation space.   */
               hpsMemory,                     /* source presentation space.   */
               4L,                            /* # of points.                 */
               aptl,                          /* array of points.             */
               ROP_SRCCOPY,                   /* copy source over destination.*/
               BBO_IGNORE );                  /* ignore eiminated data.       */

            WinEndPaint( hps );

         }   /* End of Minimize IF */

         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      }  /* End of WM_PAINT. */

      /*
       * The next two messages are handled so that the Clock application can
       * participate in device sharing.  Since it opens the devices as
       * shareable devices, other applications can gain control of the devices.
       * When this happens, we will receive a pass device message.  We keep
       * track of this device passing in the fPassed boolean variable.
       *
       * If we do not have access to the device when we receive an activate
       * message, then we will issue an acquire device command to gain
       * access to the device.
       */

      case MM_MCIPASSDEVICE:
         if (SHORT1FROMMP(mp2) == MCI_GAINING_USE)
         {
            fPassed = FALSE;                    /* Gaining control of device */
         } else
         {
            fPassed = TRUE;                     /* Losing control of device  */
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

      case WM_ACTIVATE:

         /* We use the WM_ACTIVATE message to participate in device sharing.
          * We first check to see if this is an activate or a deactivate
          * message (indicated by mp1). Then, we check to see if we've passed
          * control of the device that we use.  If these conditions are true,
          * then we issue an acquire device command to regain control of the
          * device, since we're now the active window on the screen.
          *
          * This is one possible method that can be used to implement
          * device sharing. For applications that are more complex
          * than this sample program, developers may wish to take
          * advantage of a more robust method of device sharing.
          * This can be done by using the MCI_ACQUIRE_QUEUE flag on
          * the MCI_ACQUIREDEVICE command.  Please refer to the MMPM/2
          * documentation for more information on this flag.
          */

         /*
          * First we check to see if we've passed control of the device
          */

         if ((BOOL)mp1 && fPassed == TRUE) {

            mciGenericParms.hwndCallback =  hwnd;

            ulError = mciSendCommand( mciOpenParameters.usDeviceID,
                                      MCI_ACQUIREDEVICE,
                                      (ULONG)MCI_NOTIFY,
                                      (PVOID) &mciGenericParms,
                                      (USHORT)NULL);
            if (ulError)
            {
               ShowAMessage(acStringBuffer[IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT-1],
                            IDS_CHIME_FILE_ERROR,       /* ID of message      */
                            MB_OK | MB_INFORMATION |
                            MB_HELP | MB_APPLMODAL |
                            MB_MOVEABLE );              /* Style of msg box.  */
            }
         }
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );


      default:
         return( WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) );

   }  /* End of Switch */

   return( FALSE );

} /* End of MainDialogProc */


/*******************************************************************************
 * Name         : UpdateTheClock
 *
 * Description  : This procedure checks to see if the time has changed since
 *                the last time the clock hands where drawn.  If they have
 *                and the dialog window is not minimized then the clock hands
 *                will be redrawn.  If the dialog window is minimized then
 *                the usLastHour, usLastMinute, usHour, and usMinute global
 *                variables will be updated to pretend that the clock hands
 *                have been redrawn.  Whenever the dialog window receives a
 *                WM_PAINT message the clock hands will be placed into the
 *                correct position.
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
VOID UpdateTheClock(VOID)
{
   DATETIME dtDateTime;          /* Structure to get the date and time.       */
   USHORT   usNowHour,           /* Hour converted from the above structure.  */
            usNowMinute;         /* Minute converted from the above structure.*/

   extern USHORT usLastHour;
   extern USHORT usLastMinute;


   DosGetDateTime( &dtDateTime );

   usNowHour   = (USHORT) dtDateTime.hours;
   usNowMinute = (USHORT) dtDateTime.minutes;

   /*
    * See if the current time differs from the last updated time.  If this is
    * the case then the clock hands need to be redrawn in the new positions.
    */
   if ( usNowHour != usLastHour ||
        usNowMinute != usLastMinute )
   {
      /*
       * Update the global variables to the new time so that the WM_PAINT
       * code can set the new hand positions.
       */
      usHour   = usNowHour;
      usMinute = usNowMinute;

      /*
       * Since the Time has changed, invalidate the window so that the
       * clock hands can be redrawn.
       */

      WinInvalidateRect(
         hwndMainDialogBox,
         &rctClockFaceRectangle,
         FALSE );
   }
} /* End of UpdateTheClock */

/*****************************************************************************
 * Name         : DealWithChiming
 *
 * Description  : This procedure will handle all of the processing that is
 *                required to play a chime.  The first thing the procedure
 *                does is to check to see if it is time to chime. If it is
 *                then the chiming process will be started otherwise the
 *                procedure will return to the calling routine.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : mciQuerySysValue
 *
 * Parameters   : None.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID DealWithChiming( VOID )
{
   CHIME_TIME_VALUE_T ctvChimeTime; /* Chime time structure.                  */
   USHORT usHour;                   /* Hour time.                             */
   ULONG  fClosedCaptionIsSet;      /* Is closed caption set.                 */

   if ( IsItChimeTime( &ctvChimeTime, &usHour ) == TRUE &&
           fThreadIsAlive == TRUE )
   {

      /*
       * If the Close Caption Flag indicates that the bell should be
       * animated( swung ) then the region of the Presentation space
       * that contains the Bell bitmap is to be invalidated so that a
       * WM_PAINT will be sent to draw the Bells.
       */
      mciQuerySysValue( MSV_CLOSEDCAPTION, (PVOID)&fClosedCaptionIsSet );

      if ( (BOOL)fClosedCaptionIsSet == TRUE)
      {
          /*
           * The bell is ready to be swung if needed and the thread that will
           * swing the bell is running so allow the bell to go.
           */
           yngContinueToSwingTheBell = GO;
           DosPostEventSem(
              hevContinueToSwingTheBell );  /* Semaphore Handle.          */

      }  /* End of test for closed caption. */

      FindAndPlayTheCorrectChime(
         ctvChimeTime,  /* Current Chime Time value.                          */
         usHour );      /* The current hour.                                  */

   }  /* End of Chiming */
   else
   if ( fThreadIsAlive == FALSE )
   {
      ShowAMessage(
          acStringBuffer[
            IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT -  1 ],
            IDS_SWING_THREAD_NOT_STARTED,   /* ID of the message.       */
            MB_OK | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
               MB_MOVEABLE );                /* Message box style.      */
   }

} /* End of DealWithChiming */


/******************************************************************************
 * Name         : FindTheNearestChimeTimeAndPlayIt
 *
 * Description  : This procedure is called when the Play Chime pushbutton is
 *                pressed.  The code will calculate the next Hour chime time,
 *                check to see if the CLosed Caption flag is set so that the
 *                bell can be swung back and forth, and finally will call a
 *                procedure to start the process to play the next Hour chime.
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
VOID FindTheNearestChimeTimeAndPlayIt( VOID )
{

   DATETIME dtDateTime;          /* Date time structure.                      */
   USHORT   usNextHour;          /* The hour to play.                         */
   ULONG    fClosedCaptionIsSet; /* Is closed caption set.                    */

   /*
    * Since the startup to play the chime can take more than a second, the
    * hourglass icon will be shown to the user.
    */
   WinSetPointer(
      HWND_DESKTOP,        /* Desktop window handle.                    */
      WinQuerySysPointer(  /* This API will give the handle of the PTR. */
         HWND_DESKTOP,     /* Desktop window handle.                    */
         SPTR_WAIT,        /* The Hourglass icon.                       */
         FALSE ) );        /* Return the system pointer's handle.       */

   DosGetDateTime( &dtDateTime );

   usNextHour = (USHORT) dtDateTime.hours;

   /*
    * If it is 24:00, i.e., Midnight the system will return 0.  We want 12.
    */
   if ( usNextHour == 0 )
   {
      usNextHour = 12;
   }

   /*
    * Go to the next hour.
    */
   usNextHour++;

   /*
    * Make sure that the next hour does not go past 12 o' clock.  The time
    * is kept by the system in "military" time, i.e., 4:42 is 16:42 thus
    * we need to play the Hour chime 4 times not 16.  Therefore subtract
    * 12 from the next hour value.
    */
   if ( usNextHour > 12 )
   {
      usNextHour-= 12;
   }

    /*
    * If the Close Caption Flag indicates that the bell should be animate
    * ( swung ) then the region of the Presentation space that contains
    * the Bell bitmap is to be invalidated so that a WM_PAINT will be sent
    * to draw the Bells.
    */
   mciQuerySysValue( MSV_CLOSEDCAPTION, (PVOID)&fClosedCaptionIsSet );

   if ( (BOOL)fClosedCaptionIsSet == TRUE &&
              fThreadIsAlive == TRUE )
   {
        /*
         * The bell is ready to be swung if needed and the thread that will
         * swing the bell is running so allow the bell to go.
         */
        yngContinueToSwingTheBell = GO;
        DosPostEventSem(
           hevContinueToSwingTheBell );  /* Semaphore Handle.          */

   }  /* End of test for closed caption. */
   else
   if ( fThreadIsAlive == FALSE )
   {
      ShowAMessage(
          acStringBuffer[
            IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT -  1 ],
            IDS_SWING_THREAD_NOT_STARTED,   /* ID of the message.       */
            MB_OK | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
               MB_MOVEABLE );                /* Message box style.      */
   }

   /*
    * Now that we know what the hour is start playing the chime.
    */
   FindAndPlayTheCorrectChime(
      HOUR,               /* Current Chime Time value.                        */
      usNextHour );       /* The next hour to chime.                          */

   /*
    * Now that the startup processing is over, remove the Hourglass
    * icon and replace it with the normal arrow icon.
    */
   WinSetPointer(
      HWND_DESKTOP,        /* Desktop window handle.                    */
      WinQuerySysPointer(  /* This API will give the handle of the PTR. */
         HWND_DESKTOP,     /* Desktop window handle.                    */
         SPTR_ARROW,       /* The Hourglass icon.                       */
         FALSE ) );        /* Return the system pointer's handle.       */
}  /* End of FindTheNearestChimeTimeAndPlayIt. */

/*****************************************************************************
 * Name         : FindAndPlayTheCorrectChime
 *
 * Description  : This procedure will take the passed in values, open and
 *                eventually play the correct chime file based on the given
 *                information.
 *
 * Concepts     : Sending MCI commands to the MCD.
 *
 * MMPM/2 API's : mciSendCommand
 *                MCI_PLAY
 *
 * Parameters   : ctvChimeTime           : The time that is to be chimed.
 *                usHour                 : The Hour to play.
 *
 * Return       : None.
 *
 ******************************************************************************/
VOID FindAndPlayTheCorrectChime( CHIME_TIME_VALUE_T ctvChimeTime,
                                 USHORT             usHour )
{
   /*
    * Flags used to open the playlist.
    */
   ULONG    ulPostCount;            /* Count of number of Posts for the Sem.  */
   ULONG ulError;
   ULONG ulOpenFlags = MCI_WAIT | MCI_OPEN_PLAYLIST | MCI_OPEN_TYPE_ID |
                       MCI_OPEN_SHAREABLE;

   switch( ctvChimeTime )
   {
      case HOUR :
      {

         OpenPlaylistChimeDevice(
            ulOpenFlags,                  /* The flags to use when Opening.*/
            HOUR_PLAYLIST );              /* Which chime file to open.     */

         /*
          * Set the playlist to loop for the number of hours that is to
          * be played.  The hour value will be placed into the high order
          * of the 32 bit value in operand one of the playlist structure.
          */
         apltPlayList[ HOUR_PLAYLIST ][ LOOP_LINE ].ulOperandOne =
            ((ULONG) (usHour));

         SetupChimeFileInformation();

         /*
          * fill in handle of main window proc for callback processing
          * after play completes.
          */
         mciOpenParameters.hwndCallback = hwndMainDialogBox;


         ulError =
         mciSendCommand(
            mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
            MCI_PLAY,                     /* MCI message.                  */
            MCI_NOTIFY,                   /* Flags for the MCI message.    */
            (PVOID) &mciOpenParameters,   /* Parameters for the message.   */
            HOUR_PLAYLIST );              /* Parameter for notify message. */

         if (ulError)
         {
            /*
             * Turn off the swinging of the visual bells
             */
            yngContinueToSwingTheBell = STOP;
            DosResetEventSem(
               hevContinueToSwingTheBell,    /* Semaphore Handle.             */
               &ulPostCount );               /* How many times Posted.        */

            ShowAMessage(acStringBuffer[IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT-1],
                         IDS_CANNOT_PLAY_CHIME,       /* ID of message      */
                         MB_OK | MB_INFORMATION |
                         MB_HELP | MB_APPLMODAL |
                         MB_MOVEABLE );              /* Style of msg box.  */
            break;
         }
         /* Disable the Play Chime pushbutton while the chime is playing. */

         WinEnableWindow(
            hwndPlayChimePB,     /* Handle to the Play Chime push button.   */
            FALSE);              /* Disable the push button.                */

         break;
      }  /* End of Hour Chime. */

      case HALF_HOUR :
      {
         OpenPlaylistChimeDevice(
            ulOpenFlags,               /* The flags to use when Opening.   */
            HALF_HOUR_PLAYLIST );      /* Which chime file to open.        */

         SetupChimeFileInformation();

         /*
          * fill in handle of main window proc for callback processing
          * after play completes.
          */
         mciOpenParameters.hwndCallback = hwndMainDialogBox;

         ulError =
         mciSendCommand(
            mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
            MCI_PLAY,                     /* MCI message.                  */
            MCI_NOTIFY,                   /* Flags for the MCI message.    */
            (PVOID) &mciOpenParameters,   /* Parameters for the message.   */
            HALF_HOUR_PLAYLIST );         /* Parameter for notify message. */

         if (ulError)
         {
            /*
             * Turn off the swinging of the visual bells
             */
            yngContinueToSwingTheBell = STOP;
            DosResetEventSem(
               hevContinueToSwingTheBell,    /* Semaphore Handle.             */
               &ulPostCount );               /* How many times Posted.        */

            ShowAMessage(acStringBuffer[IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT-1],
                         IDS_CANNOT_PLAY_CHIME,       /* ID of message      */
                         MB_OK | MB_INFORMATION |
                         MB_HELP | MB_APPLMODAL |
                         MB_MOVEABLE );              /* Style of msg box.  */
         }

         break;
      }  /* End of Half Hour Chime. */

      case QUARTER_HOUR :
      {
         OpenPlaylistChimeDevice(
            ulOpenFlags,               /* The flags to use when Opening.   */
            QUARTERS_PLAYLIST );       /* Which chime file to open.        */

         SetupChimeFileInformation();

         /*
          * fill in handle of main window proc for callback processing
          * after play completes.
          */
         mciOpenParameters.hwndCallback = hwndMainDialogBox;

         ulError =
         mciSendCommand(
            mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
            MCI_PLAY,                     /* MCI message.                  */
            MCI_NOTIFY,                   /* Flags for the MCI message.    */
            (PVOID) &mciOpenParameters,   /* Parameters for the message.   */
            QUARTERS_PLAYLIST );          /* Parameter for notify message. */

         if (ulError)
         {
            /*
             * Turn off the swinging of the visual bells
             */
            yngContinueToSwingTheBell = STOP;
            DosResetEventSem(
               hevContinueToSwingTheBell,    /* Semaphore Handle.             */
               &ulPostCount );               /* How many times Posted.        */

            ShowAMessage(acStringBuffer[IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT-1],
                         IDS_CANNOT_PLAY_CHIME,       /* ID of message      */
                         MB_OK | MB_INFORMATION |
                         MB_HELP | MB_APPLMODAL |
                         MB_MOVEABLE );              /* Style of msg box.  */
         }

         break;
      }  /* End of Quarter Hour chime. */

      case THREE_QUARTER_HOUR :
      {
         OpenPlaylistChimeDevice(
            ulOpenFlags,               /* The flags to use when Opening.   */
            THREE_QUARTERS_PLAYLIST ); /* Which chime file to open.        */

         SetupChimeFileInformation();

         /*
          * fill in handle of main window proc for callback processing
          * after play completes.
          */
         mciOpenParameters.hwndCallback = hwndMainDialogBox;

         ulError =
         mciSendCommand(
            mciOpenParameters.usDeviceID, /* Device to play the chimes.    */
            MCI_PLAY,                     /* MCI message.                  */
            MCI_NOTIFY,                   /* Flags for the MCI message.    */
            (PVOID) &mciOpenParameters,   /* Parameters for the message.   */
            THREE_QUARTERS_PLAYLIST );    /* Notify Parameter.           */

         if (ulError)
         {
            /*
             * Turn off the swinging of the visual bells
             */
            yngContinueToSwingTheBell = STOP;
            DosResetEventSem(
               hevContinueToSwingTheBell,    /* Semaphore Handle.             */
               &ulPostCount );               /* How many times Posted.        */

            ShowAMessage(acStringBuffer[IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT-1],
                         IDS_CANNOT_PLAY_CHIME,       /* ID of message      */
                         MB_OK | MB_INFORMATION |
                         MB_HELP | MB_APPLMODAL |
                         MB_MOVEABLE );              /* Style of msg box.  */
         }

         break;
      }  /* End of Three Quarter Hour Chime. */
   }  /* End of Switch. */

}  /* End of FindAndPlayTheCorrectChime */


/******************************************************************************
 * Name        : SetupChimeFileInformation
 *
 * Description : This procedure tells the MCD information about the file
 *               that is about to be played.  The Samples Per Second,
 *               Bits Per Sample, and the number of channels with which
 *               the waveform file was created has to be told to the MCD.
 *
 * Concepts    : How to set information on waveform files so that they may
 *               be played.  This info was obtained by querying the header
 *               of the waveform file in the routine CopyWaveformIntoMemory.
 *               This information is stored in the global variable,
 *               mmAudioHeader.
 *               In this routine, the info is set by issuing an mciSendCommand
 *               with the MCI_SET message.
 *
 * MMPM/2 API's: mciSendCommand
 *               MCI_SET
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID SetupChimeFileInformation( VOID )
{
   MCI_WAVE_SET_PARMS mwspWaveFormParameters;   /* Waveform parameters.       */
   ULONG              ulRC;                     /* Return code.               */

   /*
    * Fill the structure with zeros.
    */
   memset( &mwspWaveFormParameters,            /* Object to fill with zeros.  */
           0,                                  /* Value to place in object.   */
           sizeof( mwspWaveFormParameters ) ); /* How many zeros's to use.    */

   /*
    * Set structure values for the MCI_SET.
    */
   mwspWaveFormParameters.ulSamplesPerSec =
      mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec;
   mwspWaveFormParameters.usBitsPerSample =
      mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample;
   mwspWaveFormParameters.usChannels     =
      mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels;
   mwspWaveFormParameters.ulAudio        =
      MCI_SET_AUDIO_ALL;

   /*
    * Set the number of channels.
    */
   ulRC =
      mciSendCommand(
         mciOpenParameters.usDeviceID,
         MCI_SET,
         MCI_WAIT | MCI_WAVE_SET_CHANNELS,
         (PVOID) &mwspWaveFormParameters,
         0 );

   /*
    * Set the samples per second for the waveform file.
    */
   ulRC =
      mciSendCommand(
         mciOpenParameters.usDeviceID,
         MCI_SET,
         MCI_WAIT | MCI_WAVE_SET_SAMPLESPERSEC,
         (PVOID) &mwspWaveFormParameters,
         0 );

   /*
    * Set the bits per second for the waveform file.
    */
   ulRC =
      mciSendCommand(
         mciOpenParameters.usDeviceID,
         MCI_SET,
         MCI_WAIT | MCI_WAVE_SET_BITSPERSAMPLE,
         (PVOID) &mwspWaveFormParameters,
         0 );

}  /* End of SetupChimeFileInformation. */


/******************************************************************************
 * Name        : OpenPlaylistChimeDevice
 *
 * Description : This procedure opens a device based on the variable
 *               usChimeFileToPlay with the open flags given in the
 *               ulOpenFlags parameter.  The code uses the given parameters
 *               to set up the open parameters structure and then simply
 *               opens the playlist.
 *
 * Concepts    : How to open playlists and how to retrieve error information
 *               from failed MMPM/2 APIs.
 *
 * MMPM/2 API's: mciSendCommand
 *               mciGetErrorString
 *               MCI_OPEN
 *
 * Parameters  : ulOpenFlags       : The flags to use when opending the file.
 *               usChimePlaylistToPlay : Which chime to play.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID OpenPlaylistChimeDevice( ULONG  ulOpenFlags,
                              USHORT usChimePlaylistToPlay )

{
   ULONG mciRC;                 /* MCI generic return code variable.          */

   /*
    * Holds error text discription.
    */
   CHAR  acErrorStringBuffer[ MCI_ERROR_STRING_LENGTH ];

   /*
    * Open the correct waveform device for the chimes with the MCI_OPEN
    * message to MCI.
    */
   mciOpenParameters.pszDeviceType    = (PSZ)
      MAKEULONG ( MCI_DEVTYPE_WAVEFORM_AUDIO, 1 );

   /*
    * The address of the buffer containing the chime waveform file.
    */
   mciOpenParameters.pszElementName =
               (PSZ)&apltPlayList[ usChimePlaylistToPlay ][ 0 ];

   /*
    * Initialize the MCI_OPEN_PARMS data structure with hwndMainDialogBox
    * as callback handle for MM_MCIPASSDEVICE, then issue the MCI_OPEN
    * command with the mciSendCommand function.  No alias is used.
    */
   mciOpenParameters.hwndCallback  = hwndMainDialogBox;
   mciOpenParameters.pszAlias      = (CHAR) NULL;

   /*
    * Open the waveform file in the playlist mode.
    */
   mciRC =
      mciSendCommand(
         0,                           /* We don't know the device yet.        */
         MCI_OPEN,                    /* MCI message.                         */
         ulOpenFlags,                 /* Flags for the MCI message.           */
         (PVOID) &mciOpenParameters,  /* Parameters for the message.          */
         0 );                         /* Parameter for notify message.        */

   if ( mciRC != 0 )
   {
      mciRC =
         mciGetErrorString(
            mciRC,
            (PSZ)&acErrorStringBuffer[ 0 ],
            (USHORT) MCI_ERROR_STRING_LENGTH );

      /*
       * Make sure that the retrieving of the error string was successfull.
       */
      if ( mciRC != MCIERR_SUCCESS )
      {
         ShowAMessage(
            acStringBuffer[
               IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
            IDS_CHIME_FILE_ERROR,
            MB_OK | MB_INFORMATION | MB_MOVEABLE | MB_HELP | MB_APPLMODAL  );

         return;

      }  /* End of IF testing for failed Get Error String API. */

      /*
       * Show the error string that was retrieved by the mciGetErrorString
       * MMPM/2 API.
       */
      WinMessageBox(
         HWND_DESKTOP,                  /* Parent handle.                     */
         hwndMainDialogBox,             /* Owner handle of the message box.   */
         acErrorStringBuffer,           /* String to show in the message box. */
         acStringBuffer[
            IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],  /* Title.               */
         ID_MESSAGE_BOX,                /* Message Box Id.                    */
         MB_OK | MB_INFORMATION );      /* The style of the message box.      */

   }  /* End of IF testing for failed MCI Send Command API. */

}  /* End of OpenPlaylistChimeDevice. */


/******************************************************************************
 * Name        : IsItChimeTime
 *
 * Description : This procedure will start the processing that is required
 *               to play a chime.  The first thing the procedure does is
 *               to check to see if it is time to chime.  If it is, then the
 *               chime file that is used will be determined and the function
 *               will return TRUE to the calling routine.  The calling code
 *               will continue with the processing requied to play a chime
 *               file.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : ctvChimeTime - What time is it, hour, quarter hour, etc.
 *               pusNowHour   - Holds the actual hour value.
 *
 * Return      : TRUE         - if it is time to chime.
 *               FALSE        - if it is NOT time to chime.
 *               ctvChimeTime - This will be returned to the calling
 *                              routine.
 *
 ******************************************************************************/
BOOL IsItChimeTime( CHIME_TIME_VALUE_T *ctvChimeTime,
                    USHORT             *pusNowHour )
{
   /*
    * The last valid chime time structure in HOUR, QUARTER....
    */
   static CHIME_TIME_VALUE_T ctvLastValidChimeTime = NONE;

   DATETIME dtDateTime;       /* Date time structure.                         */
   USHORT   usNowHour,        /* The current hour from the above structure.   */
            usNowMinute;      /* The current minute from the above structure. */

   DosGetDateTime( &dtDateTime );

   usNowHour   = (USHORT) dtDateTime.hours;
   usNowMinute = (USHORT) dtDateTime.minutes;

   /*
    * See if it is greater the 12 o' clock.  If it is then subtract 12 to get
    * the correct hour.  If the time is 0 then it is midnight which is 12.
    */
   if ( usNowHour > 12 )
   {
      usNowHour -= 12;
   }
   else
   if ( usNowHour == 0 )
   {
      usNowHour = 12;
   }

   *pusNowHour = usNowHour;

   /*
    * See if the time is one of the chime times.  If so and we have not ever
    * been in this function since the time changed to a chime time, then
    * the function will return TRUE and the correct chime time indicator.
    * Otherwise the function will return FALSE and NONE.
    */
   if ( (usNowMinute == 0 || usNowMinute == 60) &&
         ctvLastValidChimeTime != HOUR)
   {
      *ctvChimeTime         = HOUR;
      ctvLastValidChimeTime = HOUR;
      return( TRUE );
   }
   else
   if ( usNowMinute == 15 && ctvLastValidChimeTime != QUARTER_HOUR )
   {
      *ctvChimeTime         = QUARTER_HOUR;
      ctvLastValidChimeTime = QUARTER_HOUR;
      return( TRUE );
   }
   else
   if ( usNowMinute == 30 && ctvLastValidChimeTime != HALF_HOUR )
   {
      *ctvChimeTime         = HALF_HOUR;
      ctvLastValidChimeTime = HALF_HOUR;
      return( TRUE );
   }
   else
   if ( usNowMinute == 45 && ctvLastValidChimeTime != THREE_QUARTER_HOUR )
   {
      *ctvChimeTime         = THREE_QUARTER_HOUR;
      ctvLastValidChimeTime = THREE_QUARTER_HOUR;
      return( TRUE );
   }
   else
   {
      *ctvChimeTime         = NONE;
      return( FALSE );
   }

} /* End of IsItChimeTime */


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
 * Parameters  : None.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID InitializeHelp( VOID )
{

   /*
    * Get the size of the initialization structure.
    */
   hmiHelpStructure.cb              = sizeof( HELPINIT );

   hmiHelpStructure.ulReturnCode    = (ULONG) NULL; /* RC from initialization.*/
   hmiHelpStructure.pszTutorialName = (CHAR) NULL;  /* No tutorial program.   */

   hmiHelpStructure.phtHelpTable    = (PVOID)(0xffff0000 | ID_CLOCK_HELPTABLE);

   /*
    * The action bar is not used, so set the following to NULL.
    */
   hmiHelpStructure.hmodAccelActionBarModule = (HMODULE) NULL;
   hmiHelpStructure.idAccelTable             = (USHORT) NULL;
   hmiHelpStructure.idActionBar              = (USHORT) NULL;

   /*
    * The Help window title.
    */
   hmiHelpStructure.pszHelpWindowTitle  =
      acStringBuffer[ IDS_HELP_WINDOW_TITLE - 1 ];

   /*
    * The Help table is in the executable file.
    */
   hmiHelpStructure.hmodHelpTableModule = (HMODULE) NULL;

   /*
    * The help panel ID is not displayed.
    */
   hmiHelpStructure.fShowPanelId       = (ULONG) NULL;

   /*
    * The library that contains the clock help panels.
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
         acStringBuffer[
            IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],   /* Message Box title.  */
         IDS_HELP_CREATION_ERROR,                      /* ID of the message.  */
         MB_OK | MB_INFORMATION | MB_MOVEABLE | MB_APPLMODAL );
   }
   else
   {
      if ( hmiHelpStructure.ulReturnCode )
      {
         ShowAMessage(
            acStringBuffer[
               IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
            IDS_HELP_CREATION_ERROR,           /* ID of the message.          */
            MB_OK | MB_INFORMATION | MB_APPLMODAL |
               MB_MOVEABLE );                  /* Message box style.          */

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
        hwndFrame );          /* Associate to this dialog window.             */
   }

}  /* End of InitializeHelp */

/******************************************************************************
 * Name        : ShowAMessage
 *
 * Description : This function will show text in a message box.  The text
 *               is pulled from the string table that is originally kept
 *               in the resource file.  The text is retrieved by the
 *               message id that is passed into this function.  This id is
 *               used in the WinLoadString OS/2 API to get the correct
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
 ******************************************************************************/
USHORT ShowAMessage( CHAR   *pcMessageTitle,
                     USHORT usMessageId,
                     ULONG  ulMessageBoxStyle )
{

   CHAR   acStringBuffer[ STRING_SIZE ];        /* String Buffer.             */
   USHORT usMessageBoxResult;                   /* RC from WinMessageBox.     */

   /*
    * Get the string from the Resource defined string table and show it
    * in the message box.
    */
   WinLoadString(
      hab,                             /* HAB for this dialog box.            */
      (HMODULE) NULL,                  /* Get the string from the .exe file.  */
      usMessageId,                     /* Which string to get.                */
      (SHORT) STRING_SIZE,             /* The size of the buffer.             */
      acStringBuffer );                /* The buffer to place the string.     */

   usMessageBoxResult =
      WinMessageBox(
         HWND_DESKTOP,                 /* Parent handle of the message box.   */
         hwndMainDialogBox,            /* Owner handle of the message box.    */
         acStringBuffer,               /* String to show in the message box.  */
         pcMessageTitle,               /* Title to shown in the message box.  */
         ID_MESSAGE_BOX,               /* Message Box Id.                     */
         ulMessageBoxStyle );          /* The style of the message box.       */

   return( usMessageBoxResult );

}  /* End of ShowAMessage */


/******************************************************************************
 * Name        : SetupPlayList
 *
 * Description : This procedure will load the chimes waveform files into
 *               buffers as well as setting up the required playlists.
 *
 *               - Create memory buffers for the waveform( Chimes ) files.
 *               - Load the chime files into the buffers( mmioRead ).
 *               - Create the Playlist
 *
 * Concepts    : How to create and use the MMPM/2 Playlist functionality.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 *************************************************************************/
VOID SetupPlayList( VOID )
{
   /*
    * This array keeps the address of each audio chime file.
    */
   static LONG *pulBaseAddress[ NUMBER_OF_CHIME_FILES ];

   USHORT usChimeFileId;            /* Chime audio file id.                   */
   ULONG  ulSizeOfFile,             /* Size of audio file.                    */

   ulMemoryAllocationFlags = PAG_COMMIT | PAG_READ | PAG_WRITE;

   for(usChimeFileId=0; usChimeFileId<NUMBER_OF_CHIME_FILES; usChimeFileId++)
   {

      ulSizeOfFile = HowBigIsTheChimeFile( usChimeFileId );

      /*
       * If the returned file size is zero, there is a problem with the chime
       * files.  A message will already have been shown to the user by the
       * HowBigIsTheChimeFile function so get out of this routine.
       */
      if ( ulSizeOfFile == 0 )
      {
         return;
      }

      if ( (pulBaseAddress[ usChimeFileId ] = (LONG *)
              malloc( ulSizeOfFile )) == (LONG *) NULL )
      {
         /*
          * The memory for the waveform files cannot be allocated.
          * Notify the user and return from this routine.  No playlist can
          * be created/played until memory is available.
          */
         ShowAMessage(
            acStringBuffer[
               IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
            IDS_CANNOT_GET_MEMORY,              /* ID of the message to show. */
            MB_OK | MB_INFORMATION | MB_HELP |  MB_APPLMODAL |
               MB_MOVEABLE );                   /* Style of the message box.  */

         return;

      }  /* End of IF allocation fails. */

      /*
       * Place the waveform files into the memory buffer that was just
       * created.
       */
      CopyWaveformIntoMemory(
         pulBaseAddress[ usChimeFileId ],
         ulSizeOfFile,
         usChimeFileId );

      /*
       * Now that we've loaded the waveform into memory, we need to put
       * it's address and size into the playlist data statements that
       * use this particular file.
       *
       * It's address must be placed into the Data statement's first operand
       * and its size must be placed in the Data statement's second operand.
       *
       * For the four different playlist, one for each chime time, 1/4, 1/2,
       * 3/4 and 1 hour increments, the address of the chime file and it's
       * size will be loaded into each Data statement of the Playlist.
       */
      if ( usChimeFileId == 0 )
      /* If we just loaded clock1.wav */
      {

         /*
          * Put the address of this chime into the first operand of
          * every data operation that uses this particular chime.
          */
         apltPlayList[ 0 ][ 0 ].ulOperandOne =        /* 1/4 hour 1st data op */
         apltPlayList[ 1 ][ 0 ].ulOperandOne =        /* 1/2 hour 1st data op */
         apltPlayList[ 2 ][ 0 ].ulOperandOne =        /* 3/4 hour 1st data op */
         apltPlayList[ 2 ][ 2 ].ulOperandOne =        /* 3/4 hour 3rd data op */
         apltPlayList[ 3 ][ 0 ].ulOperandOne =        /* 1   hour 1st data op */
         apltPlayList[ 3 ][ 2 ].ulOperandOne =        /* 1   hour 3rd data op */
            (ULONG) pulBaseAddress[ usChimeFileId ];  /* address              */

         /*
          * Now put the size of the file into the second operand of every
          * data operation that uses this particular chime.
          */
         apltPlayList[ 0 ][ 0 ].ulOperandTwo =        /* 1/4 hour 1st data op */
         apltPlayList[ 1 ][ 0 ].ulOperandTwo =        /* 1/2 hour 1st data op */
         apltPlayList[ 2 ][ 0 ].ulOperandTwo =        /* 3/4 hour 1st data op */
         apltPlayList[ 2 ][ 2 ].ulOperandTwo =        /* 3/4 hour 3rd data op */
         apltPlayList[ 3 ][ 0 ].ulOperandTwo =        /* 1   hour 1st data op */
         apltPlayList[ 3 ][ 2 ].ulOperandTwo =        /* 1   hour 3rd data op */
            ulSizeOfFile;                             /* size                 */

      }
      else
      if ( usChimeFileId == 1 )
      /* If we just loaded clock2.wav */
      {
         /*
          * Put the address of this chime into the first operand of
          * every data operation that uses this particular chime.
          */
         apltPlayList[ 1 ][ 1 ].ulOperandOne =        /* 1/2 hour 2nd data op */
         apltPlayList[ 2 ][ 1 ].ulOperandOne =        /* 3/4 hour 2nd data op */
         apltPlayList[ 3 ][ 1 ].ulOperandOne =        /* 1   hour 2nd data op */
         apltPlayList[ 3 ][ 3 ].ulOperandOne =        /* 1   hour 4th data op */
            (ULONG) pulBaseAddress[ usChimeFileId ];  /* address              */

         /*
          * Now put the size of the file into the second operand of every
          * data operation that uses this particular chime.
          */
         apltPlayList[ 1 ][ 1 ].ulOperandTwo =        /* 1/2 hour 2nd data op */
         apltPlayList[ 2 ][ 1 ].ulOperandTwo =        /* 3/4 hour 2nd data op */
         apltPlayList[ 3 ][ 1 ].ulOperandTwo =        /* 1   hour 2nd data op */
         apltPlayList[ 3 ][ 3 ].ulOperandTwo =        /* 1   hour 4th data op */
            ulSizeOfFile;                             /* size                 */
      }
      else
      if ( usChimeFileId == 2 )
      /* If we just loaded clock3.wav (this is the gong part of the chime) */
      {
         /*
          * Put the address of this chime into the first operand of
          * every data operation that uses this particular chime.
          */
         apltPlayList[ 3 ][ 5 ].ulOperandOne =        /* 1 hour 5th data op */
            (ULONG) pulBaseAddress[ usChimeFileId ];

         /*
          * Now put the size of the file into the second operand of every
          * data operation that uses this particular chime.
          */
         apltPlayList[ 3 ][ 5 ].ulOperandTwo =        /* 1 hour 5th data op */
            ulSizeOfFile;
      }

   }  /* End of For loop of chime files. */

}  /* End of SetupPlayList */


/******************************************************************************
 * Name        : HowBigIsTheChimeFile
 *
 * Description : This function will retrieve the size of a chime file.
 *               The chime file is identified by the passed in parameter.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: mmioOpen  - MMIO_READ
 *               mmioGetHeader
 *               mmioClose
 *
 * Parameters  : usChimeFileId - Chime file id.
 *
 * Return      : Size of the file.
 *
 ******************************************************************************/
ULONG HowBigIsTheChimeFile( USHORT usChimeFileId )
{
   MMAUDIOHEADER mmAudHeader;          /* Get size of audio data from this.   */
   HMMIO hmmioFileHandle;              /* Handle to the audio file handle.    */
   ULONG ulRC;                         /* Return code from mmioGetHeader.     */
   ULONG ulBytesRead;                  /* Returned from mmioGetHeader.        */
   LONG  lMMIOResults;                 /* Results from the MMIO calls.        */
   CHAR  acFileName[ FILE_NAME_SIZE ]; /* Name of the file.                   */

   switch( usChimeFileId )
   {
      case 0 :
         strcpy( acFileName, acStringBuffer[ IDS_CHIME_FILE_A - 1] );
         break;

      case 1 :
         strcpy( acFileName, acStringBuffer[ IDS_CHIME_FILE_B - 1 ] );
         break;

      case 2 :
         strcpy( acFileName, acStringBuffer[ IDS_CHIME_FILE_C - 1 ] );
         break;

      default:
         ShowAMessage(
            acStringBuffer[
               IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
            IDS_CHIME_FILE_ERROR,               /* ID of the message to show. */
            MB_OK | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
               MB_MOVEABLE );                   /* Style of the message box.  */

         return( 0 );

   }  /* End of switch statment. */

   /*
    * Open the chime file.
    */
   hmmioFileHandle =
      mmioOpen(
         acFileName,
         (PMMIOINFO) NULL,
         MMIO_READ );

   /*
    * See if the file open was successful.  If it was not then show an
    * error message, set a flag so that NO chimes will be played and
    * return a size 0.
    */
   if ( hmmioFileHandle == (HMMIO) NULL )
   {
      yngAreChimeFilesLoaded = STOP;
      ShowAMessage(
         acStringBuffer[
            IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
         IDS_CHIME_FILE_ERROR,                  /* ID of the message to show. */
         MB_OK | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
            MB_MOVEABLE );                      /* Style of the message box.  */
      return (0);
   }

   /*
    * Get the header information from the waveform file. From this
    * information, we can get the size of the waveform data in the file.
    *
    * We only want to copy the waveform data into memory, we do not
    * want to copy the header as well.  So, we use the usAudioLengthInBytes
    * field of the XWAV_HEADERINFO structure, which indicates the size of
    * the audio data in the file.
    */
   ulRC = mmioGetHeader ( hmmioFileHandle,
                          (PVOID)&mmAudHeader,
                          sizeof (MMAUDIOHEADER),
                          (PLONG) &ulBytesRead,
                          (ULONG) NULL,
                          (ULONG) NULL);


   if (ulRC != MMIO_SUCCESS) {
      yngAreChimeFilesLoaded = STOP;
      ShowAMessage(
         acStringBuffer[
            IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
         IDS_CHIME_FILE_ERROR,                  /* ID of the message to show. */
         MB_OK | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
            MB_MOVEABLE );                      /* Style of the message box.  */
      return (0);
   }

   /*
    * If the global flag is not stop then assign the GO value.  This will allow
    * a chime file to be played.
    */
   if ( yngAreChimeFilesLoaded != STOP )
   {
      yngAreChimeFilesLoaded = GO;
   }

   /*
    * Close the file.
    */
   lMMIOResults =
      mmioClose(
         hmmioFileHandle,             /* Handle of the file to close.         */
         0 );                         /* No flags are used.                   */


   /* Return the size of the audio data in bytes.
    */

   return( mmAudHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes );

}  /* End of HowBigIsTheChimeFile */


/******************************************************************************
 * Name        : CopyWaveformIntoMemory
 *
 * Description : This procedure loads the chime waveform files from DASD into
 *               the memory buffer that was created by the routine that called
 *               CopyWaveformIntoMemory.  It also querys information in the
 *               header of the waveform file for use in a later routine,
 *               SetupChimeFileInformation.
 *
 * Concepts    : How to open and read files with the MMIO APIs.
 *
 * MMPM/2 API's: mmioOpen
 *               mmioRead
 *               mmioClose
 *               mmioGetHeader
 *
 * Parameters  : pulBaseAddress - The address of the buffer that will hold
 *                                one of the chime waveform files.
 *               ulSizeOfBuffer - The size the file buffer.
 *               usChimeFile    - The identifier for the chime file.
 *
 * Return      : None.
 *
 ******************************************************************************/
VOID CopyWaveformIntoMemory( LONG   *pulBaseAddress,
                             ULONG  ulSizeOfBuffer,
                             USHORT usChimeFile )
{

   HMMIO hmmioFileHandle;              /* Handle to the audio file handle.    */
   CHAR  acFileName[ FILE_NAME_SIZE ]; /* Name of the file.                   */
   LONG  lMMIOResults;                 /* Results from the MMIO calls.        */
   ULONG ulRC;                         /* Return code from mmioGetHeader.     */
   ULONG ulBytesRead;                  /* Returned from mmioGetHeader.        */

   switch( usChimeFile )
   {
      case 0 :
         strcpy( acFileName, acStringBuffer[ IDS_CHIME_FILE_A - 1] );
         break;

      case 1 :
         strcpy( acFileName, acStringBuffer[ IDS_CHIME_FILE_B - 1 ] );
         break;

      case 2 :
         strcpy( acFileName, acStringBuffer[ IDS_CHIME_FILE_C - 1 ] );
         break;

      default:
         ShowAMessage(
            acStringBuffer[
               IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
            IDS_CHIME_FILE_ERROR,               /* ID of the message to show. */
            MB_OK | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
               MB_MOVEABLE );                   /* Style of the message box.  */
         return;

   }  /* End of switch statment. */

   /*
    * Open the chime file.
    */
   hmmioFileHandle =
      mmioOpen(
         acFileName,
         (PMMIOINFO) NULL,
         MMIO_READ );

   if ( hmmioFileHandle == (HMMIO) NULL )
   {
      ShowAMessage(
         acStringBuffer[
            IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
         IDS_CHIME_FILE_ERROR,                  /* ID of the message to show. */
         MB_OK | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
            MB_MOVEABLE );                      /* Style of the message box.  */
      return;
   }

   /*
    * Get the header information from the waveform file.  This information
    * will be used later in the routine SetupChimeFileInformation in order
    * to setup the device to play the playlist.  We only need to do this
    * for the first file, since all of the chime files were recorded in the
    * same way. The fInitHeader boolean indicates if we've already performed
    * this query or not.
    */

   if (fInitHeader) {
      ulRC = mmioGetHeader ( hmmioFileHandle,
                             (PVOID)&mmAudioHeader,
                             sizeof (MMAUDIOHEADER),
                             (PLONG) &ulBytesRead,
                             (ULONG) NULL,
                             (ULONG) NULL);
      fInitHeader = FALSE;
   }


   /*
    * Move the data from the chime file into the buffer.
    */
   lMMIOResults =
      mmioRead(
         hmmioFileHandle,
         (PSZ)pulBaseAddress,
         ulSizeOfBuffer );

   if ( lMMIOResults == MMIO_ERROR )
   {
      ShowAMessage(
         acStringBuffer[
            IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
         IDS_CHIME_FILE_ERROR,                  /* ID of the message to show. */
         MB_OK | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
            MB_MOVEABLE );                      /* Style of the message box.  */
   }

   /*
    * Close the file.
    */
   lMMIOResults =
      mmioClose(
         hmmioFileHandle,             /* Handle of the file to close.         */
         0 );                         /* No flags are used.                   */

   if ( lMMIOResults != MMIO_SUCCESS )
   {
      ShowAMessage(
         acStringBuffer[
            IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT - 1 ],
         IDS_FAILED_MMIO_CLOSE,                 /* ID of the message to show. */
         MB_OK | MB_INFORMATION | MB_HELP | MB_APPLMODAL |
            MB_MOVEABLE );                      /* Style of the message box.  */
   }

}  /* End of CopyWaveformIntoMemory */


/******************************************************************************
 * Name        : LoadBellBitmaps
 *
 * Description : This procedure will load the bitmaps that are required to
 *               animate the bell.  The handle for each bitmap is stored in
 *               a global array for later use.
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
VOID LoadBellBitmaps( VOID )
{
   USHORT usBitmapIdCounter,     /* Bitmap id counter.                        */
          usCount;               /* Generic counter.                          */

   usBitmapIdCounter            = ID_CENTER_BELL;

   for( usCount=0; usCount<NUMBER_OF_BELL_BITMAPS; usCount++)
   {
      /*
       * Believe It or Not if the following sleep is not performed some of
       * the bitmaps will not be loaded and therefore the 'ringing' swing
       * of the bell will not work correctly.  Without the sleep some of
       * the bitmaps will not be loaded, i.e., every other one.
       */
      DosSleep( 300 );

      ahbmBellBitMaps[ usCount ] =
         GpiLoadBitmap(
            hpsMemory,
            (HMODULE) NULL,
            usBitmapIdCounter,
            (LONG) BELL_BITMAP_X_SIZE,
            (LONG) BELL_BITMAP_Y_SIZE );

      usBitmapIdCounter++;

   }  /* End of For loop to load the bitmaps. */

}  /* End of LoadBellBitmaps */


/******************************************************************************
 * Name        : SwingTheBell
 *
 * Description : This procedure will handle all of the processing that is
 *               required animate the bell bitmaps.  The bitmaps will be
 *               moved if the Closed Caption Flag is set to show a non-audio
 *               method of indicating sound.  The bells will be moved until
 *               the MCI Notify message is received by the message loop at
 *               which point a semaphore, hevContinueToSwingTheBell will
 *               be set so that the animation will stop.
 *
 *               This routine will be set up in a seperate thread to allow
 *               the main window procedure to continue processing message.
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
VOID  SwingTheBell( PVOID pvData)
{
   USHORT usCount;            /* Generic counter.                             */
   BOOL   fStopSwinging;                 /* Flag to stop the bell swinging.    */

   while( DosWaitEventSem(
             hevContinueToSwingTheBell,
             SEM_INDEFINITE_WAIT ) ==  NO_ERROR )
   {
      for( usCount=0; usCount<NUMBER_OF_BELLS_IN_A_SWING; usCount++ )
      {
         /*
          * The important element in the next function call is how the
          * correct bitmap handle is picked.  This is done by using the
          * usCount value to index into the ausBellSwingOrder array. This
          * array has the order that the bitmaps are to be shown.  The
          * ausBellSwingOrder returns a value that indexes into the array
          * that contains the handle to the bitmaps.  This will indicate
          * which bitmap is to be drawn.
          */
         if (yngContinueToSwingTheBell == GO)
         {
            GpiSetBitmap(
               hpsMemory,
               ahbmBellBitMaps[ ausBellSwingOrder[ usCount ] ] );
            fStopSwinging = FALSE;          /* swinging, set the flag.        */
         }
         else if ( fStopSwinging == FALSE )
         {
            GpiSetBitmap( hpsMemory, ahbmBellBitMaps[ IX_CENTER_BELL ] );
            fStopSwinging = TRUE;           /* No more swinging, set the flag.*/
         }
         DosSleep( 230 );
         GpiBitBlt(
            hpsScreen,                        /* target presentation space.   */
            hpsMemory,                        /* source presentation space.   */
            4L,                               /* # of points.                 */
            aptl,                             /* array of points.             */
            ROP_SRCCOPY,                      /* copy source over destination.*/
            BBO_IGNORE );                     /* ignore eiminated data.       */

      }  /* End of for loop. */
   }  /* End of while loop */
  DosExit(
     0,        /* Exit only from this thread leave other threads alone.       */
     0 );      /* Return value from the thread.                               */

} /* End of SwingTheBell */
