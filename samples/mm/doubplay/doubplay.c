/*******************************************************************************
 * File Name   : DOUPLAY.C
 *
 * Description : This file contains the C source code required for the Double
 *               Buffering Playlist sample program.  This sample allows
 *               the user to play a wave file continuously by pushing the
 *               play button.  The user can stop playing the wave file by
 *               pushing the stop button.  Both of these push buttons are
 *               animated.
 *
 * Concepts    : This program illustrates how to use a Memory
 *               Playlist in a double buffering manner for reading and playing
 *               a wave file at the same time.
 *               This concept can be used in a lot of real time recording
 *               applications such as:
 *               1. Real Time recording and playing of the wave file
 *               2. Censoring a Speech being transmitted.
 *               The sample also shows how to use MMIO API's used in opening,
 *               reading, seeking, and closing files.
 *
 * MMPM/2 API's: List of all MMPM/2 API's that are used in this module.
 *
 *               mciSendCommand
 *                  MCI_ACQUIREDEVICE
 *                  MCI_OPEN
 *                  MCI_PLAY
 *                  MCI_SET
 *               mciGetErrorString
 *               mmioOpen
 *               mmioGetHeader
 *               mmioRead
 *               mmioSeek
 *               mmioClose
 *
 * Required
 *    Files    : doubplay.c         Source Code.
 *               doubapp.c          Source Code.
 *               doubplay.h         Include file.
 *               doubplay.dlg       Dialog definition.
 *               doubplay.rc        Resources.
 *               makefile           Make file.
 *               doubplay.def       Linker definition file.
 *               doubplay.ico       Doubplay icon.
 *               mywave.wav         Wave File Played.
 *               play0.bmp          Play PushButton Bitmap 0
 *               play1.bmp          Play PushButton Bitmap 1
 *               play2.bmp          Play PushButton Bitmap 2
 *               play3.bmp          Play PushButton Bitmap 3
 *               play4.bmp          Play PushButton Bitmap 4
 *               stop.bmp           Stop PushButton Bitmap
 *
 * Copyright (C) IBM  1993
 ******************************************************************************/

#define  INCL_DOS                   /* required to use Dos APIs.           */
#define  INCL_WIN                   /* required to use Win APIs.           */
#define  INCL_PM                    /* required to use PM APIs.            */
#define  INCL_OS2MM                 /* required for MCI and MMIO headers   */
#define  INCL_WINSTDSLIDER          /* required for using slider control   */
#define  INCL_SECONDARYWINDOW       /* required for secondary window       */
#define  INCL_GRAPHICBUTTON         /* required for graphic button control */

#include <os2.h>
#include <stdio.h>
#include <os2me.h>
#include <stdlib.h>
#include <string.h>

#include <sw.h>

#include "doubplay.h"               /* Dialog Ids, Resource Ids etc        */


#define WAVE_FILE_NAME "mywave.wav"
/*******************************************************************/

/* Procedure / Function prototypes */

#pragma linkage( MainDialogProc, system)
MRESULT EXPENTRY MainDialogProc(HWND, ULONG, MPARAM, MPARAM);
VOID APIENTRY PlayThread ( ULONG parm1 );
VOID main(int i,char *p[]);
VOID InitializeDialog(void);
VOID Finalize(void);
ULONG InitializePlayList( VOID );
VOID OpenAudioDevice(HWND hwnd);
VOID CloseAll( VOID );
ULONG PlayTheWave( VOID );

/*******************************************************************/

/* Global Variables */

HAB      hab;                                 /* anchor block handle         */
QMSG     qmsg;                                /* Message Que                 */
HMQ      hmq;
HWND     hwndDiag;                            /* Application Dialog handle   */
HWND     hwndFrame;                           /* Application Frame handle    */
BOOL     fPassedDevice = FALSE;               /* Do we own the Audio device  */
TID      tidPlayThread;                       /* Id for the PlayWave Thread  */
USHORT   usWaveDeviceId = 0;                  /* Id for the Wave Device      */
CHAR     achDataBuffer[MAXBUFF][MAXSIZE];     /* Data Buffer which will hold */
                                              /* wave for Memory Playlist    */
HEV      hevSem[MAXBUFF];
HMMIO    hmmioFileHandle;                     /* Handle to the Wave File     */
LONG     lState = STATE_IS_STOP;              /* state of the application    */
                                              /* Can be STOP or PLAYING      */
BOOL     fEndPlay;                            /* Used to end the play thread */


PLAY_LIST_STRUCTURE_T aplayList[2*MAXBUFF+1]; /* Play List memory Structure  */

/******************************************************************************
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
 * Return       : None.
 *
 *************************************************************************/
void main(int argc, char *argv[])
{
InitializeDialog();

/* Handle the messages: */
while(WinGetMsg(hab,&qmsg,0,0,0))
        WinDispatchMsg(hab,&qmsg);

Finalize();

}

/*************************************************************************
 * Name         : InitializeDialog
 *
 * Description  : This function performs the necessary initializations and
 *                setups that are required to show/run a dialog box as a
 *                main window.  The message queue will be created, as will
 *                the dialog box.
 *
 * Concepts     : Secondary Windows is used to create and show a dialog box.
 *                A system clock is displayed till the application is loaded.
 *
 * MMPM/2 API's : WinLoadSecondaryWindow
 *                WinQuerySecondaryHWND
 *
 * Parameters   : None.
 *
 * Return       : None.
 *
 *************************************************************************/
void InitializeDialog()
{
CHAR achTitle[STRING_SIZE] = "";
CHAR achDefaultSize[STRING_SIZE] = "";


   /* Initialize the Dialog window */
   hab = WinInitialize(0);

   /* create a message queue for the window */
   hmq = WinCreateMsgQueue(hab,0);


   /* Load the Dialog - This will return the Handle to the Frame */
   hwndFrame =
       WinLoadSecondaryWindow(HWND_DESKTOP
                            ,HWND_DESKTOP
                            ,(PFNWP)MainDialogProc
                            ,(HMODULE)NULL
                            ,ID_SAMPLE
                            ,(PVOID)NULL);


   /*
    * By specifying the QS_DIALOG flag we will get the handle to the Dialog of
    * the frame
    */
   hwndDiag = WinQuerySecondaryHWND(hwndFrame,QS_DIALOG);

   /* Get the string for default size */
   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_DEFAULT_SIZE,
      (SHORT) sizeof( achDefaultSize),
      achDefaultSize);

   /* Add Default Size menu item to system menu of the secondary window. */
   WinInsertDefaultSize(hwndFrame, achDefaultSize);

   /*
    * Get the window title string from the Resource string table
    * and set it as the window text for the dialog window.
    */
   WinLoadString(
      hab,
      (HMODULE) NULL,
      IDS_MAIN_WINDOW_TITLE,
      (SHORT) sizeof( achTitle),
      achTitle);

   /* Set the Title of the Dialog */
   WinSetWindowText( hwndFrame, achTitle);

}

/******************************************************************************
 * Name         : CloseAll
 *
 * Description  : This function is called when the current Thread finishes
 *                execution and is placed in DosExitList during the WM_INITDLG
 *                message.  This function allows to terminate itself and all
 *                of its children processes.
 *
 * Concepts     : None.
 *
 * MMPM/2 API's : None.
 *
 * Parameters   : None
 *
 * Return       : None.
 *
 *************************************************************************/
void CloseAll()
{
     DosExit(EXIT_PROCESS, 0);
     DosExitList(EXLST_EXIT, NULL);
}

/*************************************************************************
 * Name         : Finalize
 *
 * Description  : This routine is called after the message dispatch loop
 *                has ended because of a WM_QUIT message.  The code will
 *                destroy the messages queue, and window.
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
   WinDestroySecondaryWindow( hwndFrame );
   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );

}  /* End of Finalize */
/*******************************************************************************
 * Name        : MainDialogProc
 *
 * Description : This function controls the main dialog box.  It will handle
 *               received messages such as pushbutton notifications, playlist
 *               messages, etc
 *
 *               The following messages are handled specifically by this
 *               routine.
 *
 *                  WM_INITDLG
 *                  WM_CLOSE
 *                  WM_COMMAND
 *                  MM_MCIPASSDEVICE
 *                  WM_ACTIVATE
 *
 *
 * MMPM/2 API's : mciSendCommand
 *                   MCI_ACQUIREDEVICE
 *                mmioClose
 *                mmioRead
 *                mmioSeek
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
MRESULT EXPENTRY MainDialogProc(
                HWND hwnd,
                 ULONG msg,
                 MPARAM mp1,
                 MPARAM mp2)
{
static MCI_GENERIC_PARMS  lpWaveGeneric; /* Used for MCI_ACQUIREDEVICE      */
ULONG                     ulReturn;      /* Ret Code for Dos and MCI calls  */


switch (msg)
   {

    case WM_CLOSE:

       if ( usWaveDeviceId > 0 )
            mciSendCommand(
               usWaveDeviceId,               /* Device to play the chimes.    */
               MCI_CLOSE,                    /* MCI message.                  */
               MCI_WAIT,                     /* Flags for the MCI message.    */
               (ULONG) NULL,                 /* Parameters for the message.   */
               (ULONG) NULL );               /* Parameter for notify message. */

       WinPostMsg( hwnd, WM_QUIT, 0L, 0L );
       return ((MRESULT) NULL);

    case WM_INITDLG:

       /*
        * Put the Playlist Instructions into the Playlist structure.
        */
       ulReturn = InitializePlayList();

       if( ulReturn )
         {
         ShowAMessage(
             IDS_ERROR_TITLE,
             IDS_CANT_CREATE_SEMAPHORE,
             MB_OK | MB_INFORMATION | MB_MOVEABLE | MB_HELP |
             MB_APPLMODAL,
             FALSE );

         WinPostMsg( hwnd, WM_QUIT, 0L, 0L );
         }

       /* Opens the Audio device for playlist */
       OpenAudioDevice(hwnd);

       /*
        * Since the play button is an animated button, we need to initialize it
        */
       WinSendMsg (
           WinWindowFromID(
                hwnd,             /* Dialog window handle             */
                ID_PLAY),         /* Id - Play graphic button         */
           GBM_SETANIMATIONRATE,  /* Animation rate control           */
           MPFROMLONG(100L),      /* Update play bitmap every .1 sec  */
           NULL);                 /* Ignore return data               */

       break;


    /*  Service all of the button pushes */
    case WM_COMMAND:
       switch (SHORT1FROMMP(mp1))
         {
         case ID_END:

            /*
             * Send command to MCI to stop the playlist
             */
            mciSendCommand( usWaveDeviceId,  /* Device to play the chimes.    */
                            MCI_STOP,        /* MCI message.                  */
                            MCI_WAIT,        /* Flags for the MCI message.    */
                            (ULONG) NULL,    /* Parameters for the message.   */
                            (ULONG) NULL );  /* Parameter for notify message. */

            fEndPlay = TRUE;

            /*
             * Disable the animation of the Play button
             */
            WinSendMsg (   WinWindowFromID(
                             hwnd,            /* Dialog window handle        */
                             ID_PLAY),        /* Id - Play graphic button    */
                           GBM_ANIMATE,       /* Animation control           */
                           MPFROMSHORT(FALSE),/* Animation flag              */
                           NULL);             /* Ignore return data          */

            lState = STATE_IS_STOP;

            break;


         case ID_PLAY:

            /*
             * If we are not playing the playlist only then Play it
             */
            if (lState == STATE_IS_STOP)
               {
               if (fPassedDevice)
                  {
                  /*
                   * If we don't have the device then display a message.
                   * FALSE is passed because this is not a MCI error.
                   */

                  ShowAMessage(
                       IDS_ERROR_TITLE,
                       IDS_CANT_PROCESS_MESSAGE,
                       MB_OK | MB_INFORMATION | MB_MOVEABLE | MB_HELP |
                       MB_APPLMODAL ,
                       FALSE);
                  }
               else
                  /*
                   * We have the device so no problem.
                   * Call the subroutine that starts the audio playlist.
                   */
                  ulReturn = PlayTheWave();

               if ( ulReturn )
                  {
                  /*
                   * Play failed so display an error message
                   */
                  ShowAMessage(
                      IDS_ERROR_TITLE,
                      IDS_CANT_PROCESS_MESSAGE,
                      MB_OK | MB_INFORMATION | MB_MOVEABLE | MB_HELP |
                      MB_APPLMODAL,
                      FALSE );
                  }
               else
                  {
                  /*
                   * Play was successful so update the flag and animate
                   * the play button.
                   */
                  lState = STATE_IS_PLAYING;

                  WinSendMsg (
                     WinWindowFromID(
                       hwndDiag,         /* Dialog window handle      */
                       ID_PLAY),         /* Id - Play graphic button  */
                     GBM_ANIMATE,        /* Animation control         */
                     MPFROMSHORT(TRUE),  /* Animation flag            */
                     NULL);              /* Ignore return data        */

                  }


               }

            break;

         } /* end switch */
       return((MRESULT)NULL);

   /*
    * This message is used to set a flag that indicates wether this
    * application has position of the audio device. This information
    * is used to implement device sharing.
    */

    case MM_MCIPASSDEVICE:
       if( SHORT1FROMMP( mp2 ) == MCI_GAINING_USE )
          fPassedDevice = FALSE;
       else
       {
          fPassedDevice = TRUE;
          if( lState == STATE_IS_PLAYING )

            mciSendCommand( usWaveDeviceId,  /* Device to play the chimes.    */
                            MCI_PAUSE,       /* MCI message.                  */
                            MCI_WAIT,        /* Flags for the MCI message.    */
                            (ULONG) NULL,    /* Parameters for the message.   */
                            (ULONG) NULL );  /* Parameter for notify message. */

       } /* end else */
       break;


    /*
     * We use the WM_ACTIVATE message to participate in device sharing.
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
    case WM_ACTIVATE:
       /* First we check to see if we've passed control of the device */
       if ((BOOL)mp1 && fPassedDevice == TRUE)
       {
          lpWaveGeneric.hwndCallback =   hwnd;
          ulReturn = mciSendCommand( usWaveDeviceId,
                                     MCI_ACQUIREDEVICE,
                                     (ULONG)MCI_WAIT,
                                     (PVOID)&lpWaveGeneric,
                                     (USHORT)NULL );
          if (ulReturn != 0)
             /*
              * This is an MCI Error so show the message by passing TRUE
              */
             ShowAMessage(
                     IDS_ERROR_TITLE,
                     ulReturn,
                     MB_OK | MB_INFORMATION | MB_MOVEABLE | MB_HELP |
                     MB_APPLMODAL,
                     TRUE);

          if( lState == STATE_IS_PLAYING )

            mciSendCommand( usWaveDeviceId,  /* Device to play the chimes.    */
                            MCI_RESUME,      /* MCI message.                  */
                            MCI_WAIT,        /* Flags for the MCI message.    */
                            (ULONG) NULL,    /* Parameters for the message.   */
                            (ULONG) NULL );  /* Parameter for notify message. */

       } /* end if */

       break;
    } /* end switch */

    return ( WinDefDlgProc( hwnd, msg, mp1, mp2 ) );
}



/******************************************************************************
 * Name        : InitializePlayList
 *
 * Description : This procedure will initialize the playlist structure with
 *               playlist instructions.
 *               Every even number of the playlist structure subscript contains
 *               a DATA_OPERATION.  Operand One of the data instruction
 *               is pointing to the pre-allocated buffer where the wave file
 *               will be copied into.  Operand two of the data instruction
 *               contains the maximum size of the buffer.
 *               Every odd number of the playlist structure subscript contains
 *               a SEMPOST_OPERATION. Operand one contains the handle to the
 *               semaphore to be posted. Each buffer in the playlist is
 *               associated with a unique semaphore. These semaphores are
 *               used by another thread to determine when the buffer needs
 *               to be refilled with new wave data.
 *               The last entry in the playlist is a branch command that
 *               causes the playlist to jump to the begining. Thus, the wav
 *               file is played continuously.
 *
 *
 *
 * Concepts    : Initialize the memory playlist data structure.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 *************************************************************************/
 ULONG InitializePlayList()
 {
 ULONG ulNumber;
 ULONG ulReturn;

   for (ulNumber=0; ulNumber<MAXBUFF; ulNumber++)
   {

      /*
       * Create a semaphore for each buffer in the playlist.
       * These semaphores will be used to control the thread
       * that updates the buffers in the playlist.
       */
      ulReturn = DosCreateEventSem ( NULL, &hevSem[ulNumber], 0L, FALSE );

      if ( ulReturn )
         return ( ulReturn );


      /*
       * every 0, 2, 4, 6 etc index of the playlist contains the
       * DATA instruction.  Operand one contains the buffer of the data
       * Operand two contains the maxmimum size for the data buffer.
       * Operand three is ignored.
       */
      aplayList[2*ulNumber].ulCommand = DATA_OPERATION;
      aplayList[2*ulNumber].ulOperandOne = (ULONG) achDataBuffer[ulNumber];
      aplayList[2*ulNumber].ulOperandTwo = (ULONG) MAXSIZE;

      /*
       * Every 1, 3, 5, 7 etc index of the playlist contains the
       * SEMPOST instruction. Operand one contains the id of the
       * semaphore to post.
       */
      aplayList[2*ulNumber+1].ulCommand = SEMPOST_OPERATION;
      aplayList[2*ulNumber+1].ulOperandOne = (ULONG) hevSem[ulNumber];

   }

   /*
    * The last playlist command is a BRANCH command that
    * loops back to the begining of the playlist.
    */
   aplayList[2*MAXBUFF].ulCommand = BRANCH_OPERATION;
   aplayList[2*MAXBUFF].ulOperandTwo = 0;

   return ( 0 );

 }




/************************************************************************
 * Name        : OpenAudioDevice
 *
 * Description : This procedure will open the device for Playlist.
 *
 *
 * Concepts    : How to Open a Device to play a wave file from Playlist.
 *
 * MMPM/2 API's:  mciSendCommand         MCI_OPEN
 *
 * Parameters  : None.
 *
 * Return      : None.
 *
 *************************************************************************/

void  OpenAudioDevice(HWND hwnd)
{

MCI_OPEN_PARMS          lpWaveOpen;        /* Used for MCI_OPEN messages */
ULONG                   ulReturn;          /* Used for ret code for API  */


   /*
    * Initialize the MCI_OPEN_PARMS data structure with hwndMainDialogBox
    * as callback handle for MM_MCIPASSDEVICE, then issue the MCI_OPEN
    * command with the mciSendCommand function.  No alias is used.
    */

   lpWaveOpen.hwndCallback       =  hwnd;
   lpWaveOpen.pszAlias       = (PSZ) NULL;


   /*
    * Open the correct waveform device with the MCI_OPEN message to MCI.
    * Use 0 instead of 1 because this will then use the default audio device.
    */
   lpWaveOpen.pszDeviceType  =
         (PSZ) (MAKEULONG(MCI_DEVTYPE_WAVEFORM_AUDIO,0));

   /*
    * The address of the buffer which will record and play the wave.
    */
   lpWaveOpen.pszElementName = (PSZ) &aplayList[0];


   /*
    * Open the waveform file in the playlist mode.
    */
   ulReturn =
       mciSendCommand(0,
                      MCI_OPEN,
                      MCI_WAIT | MCI_OPEN_PLAYLIST |
                      MCI_OPEN_TYPE_ID | MCI_OPEN_SHAREABLE,
                      (PVOID) &lpWaveOpen,
                      (USHORT) NULL);

   if (ulReturn != 0)
      {
      /* MCI Open was not successfull so show a message by passing a TRUE */
       ShowAMessage(
             IDS_ERROR_TITLE,
             ulReturn,
             MB_OK | MB_INFORMATION | MB_MOVEABLE | MB_HELP | MB_APPLMODAL,
             TRUE);
      }

   /* Save the Device Id in the global variable to be used later */
   usWaveDeviceId = lpWaveOpen.usDeviceID;
}


/******************************************************************************
 * Name        : ShowAMessage
 *
 * Description : This function will show text in a message box.
 *               If the Message is an MCI Error then the text is pulled
 *               by issuing a mciGetErrorString.  The text is retrieved by the
 *               Return code of the mci call which failed.
 *               If the Message is not an MCI Error then the text
 *               is pulled from the string table that is originally kept
 *               in the resource file .  The text is retrieved by the
 *               message id that is passed into this function.  This id is
 *               used in the WinLoadString OS/2 API to get the correct
 *               string from the table.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: mciGetErrorString
 *
 * Parameters  : usTitleId         - The tile id that identifies which string
 *                                   to retrieve from the string table.
 *               usTextId          - The message text to be placed in the
 *                                   message box.  This value is the Return
 *                                   value for the failed MCI call if fMCIError
 *                                   is true and is the Message id (application
 *                                   owned) if fMCIError is False.
 *               ulMessageBoxStyle - The style of the message box.  Which
 *                                   icons, buttons, etc., to show.
 *               fMCIError         - If the Error is an MCIError (TRUE) or
 *                                   if it is an application message id (FALSE).
 *
 * Return      : The result from the message box.
 *
 ******************************************************************************/
USHORT ShowAMessage( USHORT usTitleId,
                     USHORT usTextId,
                     ULONG  ulMessageBoxStyle,
                     BOOL   fMCIError)
{

   CHAR   achStringBuffer[ STRING_SIZE ];       /* Title String Buffer.      */
   USHORT usMessageBoxResult;                   /* RC from WinMessageBox.    */
   CHAR   achMessageText[STRING_SIZE];          /* Message String Buffer     */
   ULONG  ulReturn;                             /* Return from MCI ERROR     */

   /*
    * Load the Title for the message box from the string table defined in
    * the resources.
    */

   WinLoadString(
      hab,                           /* HAB for this dialog box.            */
      (HMODULE) NULL,                /* Get the string from the .exe file.  */
      usTitleId,                     /* Which string to get.                */
      (SHORT) STRING_SIZE,           /* The size of the buffer.             */
      achStringBuffer );             /* The buffer to place the string.     */

   /* If we are to show a MCIERROR, then load it by mciGetErrorString API */
   if (fMCIError)
     {
        ulReturn =
            mciGetErrorString(
                            usTextId,
                            (PSZ)achMessageText,
                            (USHORT) STRING_SIZE );


           /*
            * Make sure that the retrieving of the error string was successfull.
            */
           if ( ulReturn != MCIERR_SUCCESS )
             {
              WinLoadString(
                    hab,                       /* HAB for this dialog box.   */
                    (HMODULE) NULL,            /* Get string from .exe file. */
                    IDS_CANT_PROCESS_MESSAGE,  /* Which string to get.       */
                    (SHORT) STRING_SIZE,       /* The size of the buffer.    */
                    achMessageText);           /* Buffer to place the string.*/

              ulMessageBoxStyle = MB_OK | MB_INFORMATION | MB_MOVEABLE | MB_HELP |
                                  MB_APPLMODAL;
             }  /* End of IF testing for failed Get Error String API. */

     }
   else
     {
     /*
      This is not an MCIError so load the Text for Message Box from
      the string table defined in the resources.
      */
       WinLoadString(
           hab,                     /* HAB for this dialog box.   */
           (HMODULE) NULL,          /* Get string from .exe file. */
           usTextId,                /* Which string to get.       */
           (SHORT) STRING_SIZE,     /* The size of the buffer.    */
           achMessageText );        /* Buffer to place the string.*/
     }

   usMessageBoxResult =
      WinMessageBox(
         HWND_DESKTOP,              /* Parent handle of the message box.   */
         hwndDiag,                  /* Owner handle of the message box.    */
         achMessageText,            /* String to show in the message box.  */
         achStringBuffer,           /* Title to shown in the message box.  */
         ID_MESSAGE_BOX,            /* Message Box Id.                     */
         ulMessageBoxStyle );       /* The style of the message box.       */

   return( usMessageBoxResult );

}  /* End of ShowAMessage */



/*************************************************************************
 * Name         : PlayTheWave
 *
 * Description : This function initially opens the wave file to be played
 *               and tells the MCD information about the file
 *               that is about to be played.  The Samples Per Second,
 *               Bits Per Sample, and the number of channels with which
 *               the waveform file was created has to be told to the MCD.
 *               This function initially fills up the allocated buffer
 *               of the playlist with the wave file.  For 0 - MAXBUFF
 *               the wave file is filled into the memory.  The Wave file
 *               is read in continously from 0 -MAXBUFF.
 *               This same buffer is dynamically re-filled again and again
 *               by the PlayThread thread.
 *
 * Concepts     : The wave file is first opened and then continuously read
 *                into the buffer.  If the end of the wave file is reached
 *                then we seek to the starting of the wave and keep on reading
 *                the wave file till the buffer fills up. This displays the
 *                double buffering concept because, while the playlist is
 *                continuously looping and playing the wave file, the
 *                playlist buffers are constantly being filled with data from
 *                the wave file in the PlayThread thread.
 *
 *
 *
 * MMPM/2 API's : mmioOpen
 *                mmioGetHeader
 *                mmioRead
 *                mmioSeek
 *                mciSendCommand
 *                  MCI_SET
 *                  MCI_CUE
 *
 * Parameters   : None.
 *
 * Return       : ulReturn
 *
 *************************************************************************/
ULONG PlayTheWave()
{

   MCI_WAVE_SET_PARMS    lpWaveSet;
   ULONG                 ulReturn;
   MMAUDIOHEADER         mmHeader;
   int i;
   long                  ulBytesRead;


   memset ( &lpWaveSet,
            0,
            sizeof( MCI_WAVE_SET_PARMS ) );

   /* Open the Wave File MyWave.Wav for Reading */
   hmmioFileHandle = mmioOpen(WAVE_FILE_NAME,
                             (PMMIOINFO) NULL,
                             MMIO_READ);

   /* If the Wave File could not be opened */
   if (!hmmioFileHandle)
      {

      ShowAMessage(
         IDS_ERROR_TITLE,
         IDS_CANT_OPEN_WAVE,
         MB_OK | MB_INFORMATION | MB_MOVEABLE | MB_HELP |
         MB_APPLMODAL,
         FALSE );

      return ( TRUE );
      }

   /*
    * Get the Header Information for the file so that we can set the channels,
    * Samples Per Second and Bits Per Sample to play the memory playlist.
    */

   ulReturn = mmioGetHeader(hmmioFileHandle,
                            (PVOID) &mmHeader,
                            sizeof(MMAUDIOHEADER),
                            (PLONG) &ulBytesRead,
                            (ULONG) NULL,
                            (ULONG) NULL);
   if (ulReturn != 0 )
     return ( ulReturn );

   /* Set the WaveSet Structure */

   lpWaveSet.ulLevel = 100;
   lpWaveSet.ulSamplesPerSec = mmHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec;
   lpWaveSet.usBitsPerSample = mmHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample;
   lpWaveSet.usChannels = mmHeader.mmXWAVHeader.WAVEHeader.usChannels;
   lpWaveSet.ulAudio = MCI_SET_AUDIO_ALL;
   lpWaveSet.hwndCallback = (HWND) NULL;

   /* Set the Channels for the MCD */
   ulReturn
        = mciSendCommand(usWaveDeviceId,
                         MCI_SET,
                         MCI_WAIT | MCI_WAVE_SET_CHANNELS,
                         (PVOID) &lpWaveSet,
                         (USHORT)NULL);
   if (ulReturn != 0 )
     return ( ulReturn );

   /* Set the Samples Per Second */
   ulReturn
        = mciSendCommand(usWaveDeviceId,
                         MCI_SET,
                         MCI_WAIT | MCI_WAVE_SET_SAMPLESPERSEC,
                         (PVOID) &lpWaveSet,
                         (USHORT)NULL);

   if (ulReturn != 0 )
     return ( ulReturn );

   /* Set the Bits per Sample */
   ulReturn
        = mciSendCommand(usWaveDeviceId,
                         MCI_SET,
                         MCI_WAIT | MCI_WAVE_SET_BITSPERSAMPLE,
                         (PVOID) &lpWaveSet,
                         (USHORT)NULL);

   if (ulReturn != 0 )
     return ( ulReturn );


  /* From 0 - MAXBUFF, fill the memory Playlist buffer with the wave file. */

   for (i=0; i<MAXBUFF; i++)
   {

     ulBytesRead =
           mmioRead(hmmioFileHandle,
                    (HPSTR) achDataBuffer[i],
                    MAXSIZE);
     /*
      * If the end of the wave file is reached then Seek to the starting
      * of the wave file and start reading the wave into the appropriate
      * buffer
     */
     if (ulBytesRead < MAXSIZE)
        {
         mmioSeek(hmmioFileHandle, 0, SEEK_SET);

         ulBytesRead =
           mmioRead(hmmioFileHandle,
                    (HPSTR) achDataBuffer[i],
                    MAXSIZE);
         }


   }


   /*
    * Create the thread that will maintain the playlist buffer
    */
   fEndPlay = FALSE;
   if ( DosCreateThread ( &tidPlayThread,
                         (PFNTHREAD) PlayThread,
                         0L, 0L, 8192L))
      return ( TRUE );

   return ( FALSE );

}







/*************************************************************************
 *   Name PlayThread
 *
 *   Description:    This thread first starts the playlist by sending
 *                   an MCI_PLAY command. The thread then executes a
 *                   DosWaitEventSem using the first semaphore in the
 *                   playlist. When the first buffer in the playlist is
 *                   consumed, this semaphore gets posted by the
 *                   SEMPOST_OPERATION command in the playlist. This is
 *                   the queue to begin refilling this buffer.
 *                   Once the buffer is full, the thread repeates the
 *                   same procedure for the next semaphore. This
 *                   procedure is repeated for each semaphore until
 *                   each buffer has been refilled and then starts over
 *                   with the first semaphore.
 *                     In this way, the entire wav file is played
 *                   continuously until the user either closes the
 *                   application or selects the stop button.
 *
 *
 *
 *************************************************************************/

VOID  APIENTRY PlayThread ( ULONG parm1 )
{

   ULONG    ulBytesRead;
   ULONG    ulBufferNum;
   ULONG    ulPostCount;
   ULONG    ulReturn;
   MCI_PLAY_PARMS        mciPlayParameters;



   mciPlayParameters.hwndCallback =  hwndDiag;

   /*
    * Send the Play Command to begin the playing of the memory playlist.
    */
   ulReturn = mciSendCommand(usWaveDeviceId,
                             MCI_PLAY,
                             MCI_NOTIFY,
                             (PVOID) &mciPlayParameters,
                             0);

   /*
    * Loop until the playlist is stopped.
    */
   while( !fEndPlay )
   {

      for( ulBufferNum = 0; ulBufferNum < MAXBUFF; ulBufferNum++ )
      {

         /*
          * Use DosWaitEventSem to pause util the playlist plays this
          * buffer. Then refill the buffer and post the semaphore.
          */
         ulReturn = DosWaitEventSem ( hevSem[ulBufferNum], 5000L );

         DosResetEventSem ( hevSem[ulBufferNum], &ulPostCount );

         if( fEndPlay ) break;

            /*
             * Read the Next MAXSIZE bytes of the wave file
             */
            ulBytesRead =
                   mmioRead(hmmioFileHandle,
                            (HPSTR) achDataBuffer[ulBufferNum],
                            MAXSIZE);


            /*
             * If we have reached the end of the wave file then set the file marker
             * to the begining of the file and start reading into the bufferno.
             * This causes the continuous filling and playing of the data buffer
             * again and again as long as the END push button is not pressed.
             * The wave file is going to keep on playing without any interruptions
             * util ther user selects the stop button.
             */
            if (ulBytesRead < MAXSIZE)
              {
               /* Seek to the starting of the wave file */
               mmioSeek(hmmioFileHandle, 0, SEEK_SET);

               /* Read the Next MAXSIZE bytes of the wave file */
               mmioRead(hmmioFileHandle,
                       (HPSTR) achDataBuffer[ulBufferNum],
                       MAXSIZE);


              }


      } /* End of for loop */


   } /* End of while loop */


}   /* End of function PlayThread */

