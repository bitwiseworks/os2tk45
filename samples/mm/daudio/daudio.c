

 /***************************************************************************
 *                                                                          *
 * File Name        : DAUDIO.C                                              *
 *                                                                          *
 * Description      : This program provides sample code for using the       *
 *                    Multimedia API's related to direct audio support.     *
 *                                                                          *
 *                                                                          *
 * Concepts         : A high speed audio interface has been created by      *
 *                    allowing an application to send audio data directly   *
 *                    to the Amp-Mixer device. This application will        *
 *                    demonstrate the steps required to setup and use ths   *
 *                    new interface for recording and playback of digital   *
 *                    audio.                                                *
 *                                                                          *
 * Entry Points     :                                                       *
 *                    MyEvent                                               *
 *                    MciError                                              *
 *                    Close                                                 *
 *                    LoadFile                                              *
 *                    ResetRecord                                            *
 *                    OpenPlayBack                                          *
 *                    StartPlayBack                                         *
 *                    MainDialogProc                                        *
 *                    main                                                  *
 *                                                                          *
 *                                                                          *
 *                                                                          *
 *                                                                          *
 * Copyright        : COPYRIGHT IBM CORPORATION, 1991, 1992, 1993           *
 *                                                                          *
 *       DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *       sample code created by IBM Corporation. This sample code is not    *
 *       part of any standard or IBM product and is provided to you solely  *
 *       for  the purpose of assisting you in the development of your       *
 *       applications.  The code is provided "AS IS", without               *
 *       warranty of any kind.  IBM shall not be liable for any damages     *
 *       arising out of your use of the sample code, even if they have been *
 *       advised of the possibility of such damages.                        *
 *                                                                          *
 ****************************************************************************/


#define  INCL_PM                    /* required to use PM APIs.            */
#define  INCL_OS2MM                 /* required for MCI and MMIO headers   */
#define  INCL_DOS

#include <os2.h>
#include <os2me.h>
#include <string.h>
#include <malloc.h>
#include "daudio.h"

USHORT               usDeviceID;                /* Amp Mixer device id     */
HWND                 hwndFrame;                 /* Dialog fram window id   */

ULONG                ulBufferCount;             /* Current file buffer     */
ULONG                ulNumBuffers;              /* Number of file buffers  */
ULONG                ulNumReturnedBuffers;      /* Num of returned buffers */

MCI_MIX_BUFFER       MixBuffers[MAX_BUFFERS];   /* Device buffers          */
MCI_MIXSETUP_PARMS   MixSetupParms;             /* Mixer parameters        */
MCI_BUFFER_PARMS     BufferParms;               /* Device buffer parms     */


/***************************************************************************
 * Name        : MyEvent
 *
 * Description : The address to this procedure is passed to the mixer
 *               device in the MIX_SETUP_PARMS structure. The mixer
 *               device then calls this procedure when it has expended
 *               a buffer.
 *
 *               NOTE: This is a high priority thread. Too much code here
 *                     will bog the system down.
 *
 * Parameters  : ulStatus - Detailed error message
 *               pBuffer  - Pointer to expended buffer
 *               ulFlags  - Indicates the type of event
 *
 *
 * Return      : BOOL   - TRUE  = failure
 *                      - FALSE = success
 *
 ***************************************************************************/

LONG APIENTRY MyEvent ( ULONG ulStatus,
                        PMCI_MIX_BUFFER  pBuffer,
                        ULONG  ulFlags  )

{
   ulNumReturnedBuffers++;                      /* Adjust buffer count   */
   switch( ulFlags )
   {
   case MIX_STREAM_ERROR | MIX_READ_COMPLETE :  /* error occur in device */
   case MIX_STREAM_ERROR | MIX_WRITE_COMPLETE:  /* error occur in device */

      if ( ulStatus == ERROR_DEVICE_UNDERRUN)
      {
         /* handle ERROR_DEVICE_UNDERRUN or OVERRUN here
          */

         if( ulBufferCount < ulNumBuffers )
         {
            MixSetupParms.pmixWrite( MixSetupParms.ulMixHandle,
                                     &MixBuffers[ulBufferCount],
                                     1 );
            ulBufferCount++;
         }

         if( ulNumReturnedBuffers  == (ulNumBuffers - 1) )
         {
            WinPostMsg( hwndFrame, WM_COMMAND, ( PVOID )ID_STOP, 0L );
         }
      }

      break;

   case MIX_READ_COMPLETE :           /* for recording */

      if( ulBufferCount < ulNumBuffers )
      {
         MixSetupParms.pmixRead( MixSetupParms.ulMixHandle,
                                 &MixBuffers[ulBufferCount],
                                 1 );
         ulBufferCount++;
      }

      if( ulNumReturnedBuffers  == (ulNumBuffers - 1) )
      {
         WinPostMsg( hwndFrame, WM_COMMAND, ( PVOID )ID_STOP, 0L );
      }
      break;

   case MIX_WRITE_COMPLETE:           /* for playback  */

      if( ulBufferCount < ulNumBuffers )
      {
         MixSetupParms.pmixWrite( MixSetupParms.ulMixHandle,
                                  &MixBuffers[ulBufferCount],
                                  1 );
         ulBufferCount++;
      }

      if( ulNumReturnedBuffers  == (ulNumBuffers - 1) )
      {
         WinPostMsg( hwndFrame, WM_COMMAND, ( PVOID )ID_STOP, 0L );
      }
      break;

   } /* end switch */

   return( TRUE );

} /* end MyEvent */




/***************************************************************************
 * Name        : MciError
 *
 * Description : Display a message box with the string corresponding to
 *               the passed mci error code.
 *
 * Parameters  : ulError = mci error code
 *
 * Return      : None
 *
 ***************************************************************************/
VOID MciError( ULONG ulError )
{
   SZ       szBuffer[ STRING_LENGTH ];
   ULONG    rc;

   rc = mciGetErrorString( ulError, szBuffer, STRING_LENGTH );

   WinMessageBox( HWND_DESKTOP,
                  HWND_DESKTOP,
                  szBuffer,
                  "MCI ERROR",
                  0,
                  MB_OK | MB_ERROR | MB_MOVEABLE);

}


/***************************************************************************
 * Name        : Close
 *
 * Description : Deallocate memory and close the Amp-Mixer Device.
 *
 *
 * Parameters  : None
 *
 * Return      : BOOL   - TRUE  = failure
 *                      - FALSE = success
 *
 ***************************************************************************/
VOID Close( VOID )
{
   MCI_GENERIC_PARMS    GenericParms;
   ULONG                rc;

   rc = mciSendCommand( usDeviceID,
                        MCI_BUFFER,
                        MCI_WAIT | MCI_DEALLOCATE_MEMORY,
                        ( PVOID )&BufferParms,
                        0 );

   if ( rc != MCIERR_SUCCESS )
   {
      MciError( rc );
      return;
   }

   rc = mciSendCommand( usDeviceID,
                        MCI_CLOSE,
                        MCI_WAIT ,
                        ( PVOID )&GenericParms,
                        0 );

   if ( rc != MCIERR_SUCCESS )
   {
      MciError( rc );
      return;
   }

}



/***************************************************************************
 * Name        : LoadFile
 *
 * Description : Allocate application buffers for audio file and load
 *               audio file. These buffers will be copied to device
 *               buffers by another thread.
 *
 *               Before this procedure loads the audio file, the global
 *               MixSetupParms data structure is loaded with information
 *               from the audio files header.
 *
 *
 * Parameters  : CHAR szFilename[]     - Name of wave file to open
 *
 * Return      : BOOL   - TRUE  = failure
 *                      - FALSE = success
 *
 ***************************************************************************/
BOOL LoadFile( CHAR szFileName[] )
{
   MMIOINFO             mmioInfo;
   MMAUDIOHEADER        mmAudioHeader;
   HMMIO                hmmio;
   LONG                 lBytesRead;
   LONG                 ulBufferOffset;
   MCI_AMP_OPEN_PARMS   AmpOpenParms;
   ULONG                rc, ulIndex;



   /* open the mixer device
    */
   memset ( &AmpOpenParms, '\0', sizeof ( MCI_AMP_OPEN_PARMS ) );
   AmpOpenParms.usDeviceID = ( USHORT ) 0;
   AmpOpenParms.pszDeviceType = ( PSZ ) MCI_DEVTYPE_AUDIO_AMPMIX;

   rc = mciSendCommand( 0,
                       MCI_OPEN,
                       MCI_WAIT | MCI_OPEN_TYPE_ID | MCI_OPEN_SHAREABLE,
                       ( PVOID ) &AmpOpenParms,
                       0 );

   if ( rc != MCIERR_SUCCESS )
   {
      MciError( rc );
      return( TRUE );
   }

   usDeviceID = AmpOpenParms.usDeviceID;



   /* Open the audio file.
    */
   memset( &mmioInfo, '\0', sizeof( MMIOINFO ));
   mmioInfo.fccIOProc = mmioFOURCC( 'W', 'A', 'V', 'E' );
   hmmio = mmioOpen( szFileName, &mmioInfo, MMIO_READ | MMIO_DENYNONE );

   if( !hmmio )
   {
      WinMessageBox( HWND_DESKTOP,
                     HWND_DESKTOP,
                     "Unable to open wave file",
                     "MMIO Error",
                     0,
                     MB_OK | MB_ERROR | MB_MOVEABLE);

      return( TRUE );
   }

   /* Get the audio file header.
    */
   mmioGetHeader( hmmio,
                  &mmAudioHeader,
                  sizeof( MMAUDIOHEADER ),
                  &lBytesRead,
                  0,
                  0);

   /* Set the MixSetupParms data structure to match the loaded file.
    * This is a global that is used to setup the mixer.
    */
   memset( &MixSetupParms, '\0', sizeof( MCI_MIXSETUP_PARMS ) );

   MixSetupParms.ulBitsPerSample =
         mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample;

   MixSetupParms.ulFormatTag =
         mmAudioHeader.mmXWAVHeader.WAVEHeader.usFormatTag;

   MixSetupParms.ulSamplesPerSec =
         mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec;

   MixSetupParms.ulChannels =
         mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels;

   /* Setup the mixer for playback of wave data
    */
   MixSetupParms.ulFormatMode = MCI_PLAY;
   MixSetupParms.ulDeviceType = MCI_DEVTYPE_WAVEFORM_AUDIO;
   MixSetupParms.pmixEvent    = MyEvent;

   rc = mciSendCommand( usDeviceID,
                        MCI_MIXSETUP,
                        MCI_WAIT | MCI_MIXSETUP_INIT,
                        ( PVOID ) &MixSetupParms,
                        0 );

   if ( rc != MCIERR_SUCCESS )
   {
      MciError( rc );
      return( TRUE );
   }

   /* Use the suggested buffer size provide by the mixer device
    * and the size of the audio file to calculate the required
    * number of Amp-Mixer buffers.
    * Note: The result is rounded up 1 to make sure we get the
    *       tail end of the file.
    */
   ulNumBuffers =
         mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes
         / ( MixSetupParms.ulBufferSize ) + 1;


   /* Set up the BufferParms data structure and allocate
    * device buffers from the Amp-Mixer
    */
   BufferParms.ulNumBuffers = ulNumBuffers;
   BufferParms.ulBufferSize = MixSetupParms.ulBufferSize;
   BufferParms.pBufList = MixBuffers;

   rc = mciSendCommand( usDeviceID,
                        MCI_BUFFER,
                        MCI_WAIT | MCI_ALLOCATE_MEMORY,
                        ( PVOID ) &BufferParms,
                        0 );

    if ( ULONG_LOWD( rc) != MCIERR_SUCCESS )
    {
      MciError( rc );
      return( TRUE );
    }

   /* Fill all device buffers with data from the audio file.
    */
   for( ulIndex = 0; ulIndex < ulNumBuffers; ulIndex++)
   {
      memset( MixBuffers[ ulIndex ].pBuffer, '\0', BufferParms.ulBufferSize );
      MixBuffers[ ulIndex ].ulBufferLength = BufferParms.ulBufferSize;

      rc = mmioRead ( hmmio,
                      MixBuffers[ ulIndex ].pBuffer,
                      MixBuffers[ ulIndex ].ulBufferLength );

   }

   /* Set the "end-of-stream" flag
    */
   MixBuffers[ulNumBuffers - 1].ulFlags = MIX_BUFFER_EOS;

   mmioClose( hmmio, 0 );

   return( FALSE );

}



/***************************************************************************
 * Name        : ResetRecord
 *
 * Description : Using the information stored in the MixSetupParms data
 *               structure, the Amp-Mixer device is opened for recording.
 *               Enough device buffers are allocated to fill preallocated
 *               application buffers. Recording is then started and will
 *               continue until all of the device buffers are full ( or
 *               until the user selects the STOP button.
 *
 *
 * Parameters  : None
 *
 * Return      : BOOL   - TRUE  = failure
 *                      - FALSE = success
 *
 ***************************************************************************/
BOOL ResetRecord( VOID )
{
   MCI_CONNECTOR_PARMS  ConnectorParms;
   MCI_AMP_SET_PARMS    AmpSetParms ;
   ULONG                rc;


   /* Deinitialize the Amp-Mixer to reset for record mode
    */
   rc = mciSendCommand( usDeviceID,
                        MCI_MIXSETUP,
                        MCI_WAIT | MCI_MIXSETUP_DEINIT,
                        ( PVOID )&MixSetupParms,
                        0 );

   if ( rc != MCIERR_SUCCESS )
   {
      MciError( rc );
      return( TRUE );
   }


   /* Setup the mixer for record
    * Note: MixSetupParms is a global data structure. Most of this
    *       structure must be set before this point. The LoadFile
    *       procedure is the first place that this is done.
    */
   MixSetupParms.ulFormatMode = MCI_RECORD;

   rc = mciSendCommand( usDeviceID,
                        MCI_MIXSETUP,
                        MCI_WAIT | MCI_MIXSETUP_INIT,
                        ( PVOID ) &MixSetupParms,
                        0 );

   if ( rc != MCIERR_SUCCESS )
   {
      MciError( rc );
      return( TRUE );
   }



   /* Zero out the device buffers.
    */
   for( ulBufferCount = 0; ulBufferCount < ulNumBuffers; ulBufferCount++ )
      memset( MixBuffers[ulBufferCount].pBuffer,
              '/0',
              MixBuffers[ulBufferCount].ulBufferLength );

  /* Set the connector to 'line in'
   */
  memset( &ConnectorParms, '\0', sizeof( MCI_CONNECTOR_PARMS ) );
  ConnectorParms.ulConnectorType = MCI_LINE_IN_CONNECTOR;
  rc = mciSendCommand( usDeviceID,
                       MCI_CONNECTOR,
                       MCI_WAIT |
                       MCI_ENABLE_CONNECTOR |
                       MCI_CONNECTOR_TYPE,
                       ( PVOID ) &ConnectorParms,
                       0 );

  /* Allow the user to hear what is being recorded
   * by turning the monitor on
   */
  memset( &AmpSetParms, '\0', sizeof( MCI_AMP_SET_PARMS ) );
  AmpSetParms.ulItem = MCI_AMP_SET_MONITOR;
  rc = mciSendCommand( usDeviceID,
                       MCI_SET,
                       MCI_WAIT | MCI_SET_ON | MCI_SET_ITEM,
                       ( PVOID ) &AmpSetParms,
                       0 );

  /* Set volume to max
   */
  memset( &AmpSetParms, '\0', sizeof( MCI_AMP_SET_PARMS ) );
  AmpSetParms.ulAudio = MCI_SET_AUDIO_ALL;
  AmpSetParms.ulItem = 0;
  AmpSetParms.ulLevel = 100L;
  rc = mciSendCommand( usDeviceID,
                       MCI_SET,
                       MCI_WAIT | MCI_SET_AUDIO | MCI_SET_VOLUME,
                       ( PVOID ) &AmpSetParms,
                       0 );

   /* Set the buffer counter
    */
   ulBufferCount = ulNumBuffers;
   ulNumReturnedBuffers = 0;              /* Reset Num of returned buffers */

   /* Kick off the mixer.
    */
   if( ulNumBuffers < 2)
   {
   /* If there is only one buffer to read  then tell the mixer that
    * this is the end of the stream. If this flag is not set then
    * the mixer would not start with less than two buffers.
    */
      MixBuffers[0].ulFlags = MIX_BUFFER_EOS;

      MixSetupParms.pmixRead( MixSetupParms.ulMixHandle,
                              &MixBuffers[ulBufferCount],
                              1 );
   }
   else
   {
      MixSetupParms.pmixRead( MixSetupParms.ulMixHandle,
                              MixBuffers,
                              ulNumBuffers );
   }


   return( FALSE );


}




/****************************************************************************
 * Name        : ResetPlayBack
 *
 * Description : Reinit the Amp-Mix device for play-back, allocate  device
 *               buffers, copy audio data to device buffers and kick off
 *               the Amp-Mixer device.
 *
 * Parameters  : None
 *
 * Return      : None
 *
 ***************************************************************************/
BOOL ResetPlayBack( VOID )
{
   MCI_CONNECTOR_PARMS  ConnectorParms;
   MCI_AMP_SET_PARMS    AmpSetParms ;
   ULONG                rc;


   /* Deinitialize the Amp-Mixer to reset for record mode
    */
   rc = mciSendCommand( usDeviceID,
                        MCI_MIXSETUP,
                        MCI_WAIT | MCI_MIXSETUP_DEINIT,
                        ( PVOID )&MixSetupParms,
                        0 );

   if ( rc != MCIERR_SUCCESS )
   {
      MciError( rc );
      return( TRUE );
   }


   /* Setup the mixer for play-back
    * Note: MixSetupParms is a global data structure. Most of this
    *       structure must be set before this point. The LoadFile
    *       procedure is the first place that this is done.
    */
   MixSetupParms.ulFormatMode = MCI_PLAY;

   rc = mciSendCommand( usDeviceID,
                        MCI_MIXSETUP,
                        MCI_WAIT | MCI_MIXSETUP_INIT,
                        ( PVOID ) &MixSetupParms,
                        0 );

   if ( rc != MCIERR_SUCCESS )
   {
      MciError( rc );
      return( TRUE );
   }


   return( FALSE );
}







/****************************************************************************
 * Name        : StartPlayBack
 *
 * Description : Copy all of the application buffers to device buffers
 *               and kick off the Amp-Mixer device.
 *
 * Parameters  : None
 *
 * Return      : None
 *
 ***************************************************************************/
VOID StartPlayBack( VOID )
{
   ULONG       ulIndex;                /* Device buffer index           */
   ULONG       ulCount;                /* Number of posts               */



   if( ulNumBuffers > 8 )
      ulBufferCount = 8;
   else
      ulBufferCount = ulNumBuffers;
   ulNumReturnedBuffers = 0;              /* Reset Num of returned buffers */

   /* Write buffers to kick off the amp mixer.
    */
   MixSetupParms.pmixWrite( MixSetupParms.ulMixHandle,
                            MixBuffers,
                            ulBufferCount );

}



/***************************************************************************
 * Name        : MainDialogProc
 *
 * Description : This function controls the main dialog box.  It will handle
 *               received messages such as pushbutton notifications, and
 *               entry field messages.
 *
 *
 ***************************************************************************/
MRESULT EXPENTRY MainDialogProc( HWND hwnd,
                                 ULONG msg,
                                 MPARAM mp1,
                                 MPARAM mp2)
{

   MCI_GENERIC_PARMS    GenericParms;
   static HWND          hwndPlayButton,
                        hwndRecordButton,
                        hwndStopButton;
   static BOOL          fPassedDevice = TRUE,
                        fRecording = FALSE;

   ULONG                ulIndex,
                        rc;

   switch (msg)
   {
   case WM_INITDLG:

      /* Get the handles for the PLAY and RECORD buttons
       */
      hwndPlayButton = WinWindowFromID( hwnd, ID_PLAY );
      hwndRecordButton = WinWindowFromID( hwnd, ID_RECORD );
      hwndStopButton = WinWindowFromID( hwnd, ID_STOP );

      /* Disable the stop button
       */
      WinEnableWindow( hwndStopButton, FALSE );

      /* Load the audio file and setup for playback.
       */
      rc = LoadFile( DEFAULT_FILE );

      if( rc )
      {
         MciError( rc );
         WinPostMsg( hwnd, WM_QUIT, 0L, 0L );
         break;
      }

      break;

   case MM_MCIPASSDEVICE:

      /* Check if we are gaining or passing use of the amp-mixer device
       */
      if( SHORT1FROMMP( mp2 ) == MCI_GAINING_USE )
         fPassedDevice = FALSE;
      else
         fPassedDevice = TRUE;

      break;

   case WM_ACTIVATE:

      /* Check if this window is the active window and if we have passed
       * use of the Amp-Mixer device. If yes, then send MCI_ACQUIREDEVICE
       * message.
       */
      if( ( BOOL )mp1 && fPassedDevice )
      {
         GenericParms.hwndCallback = hwnd;

         rc = mciSendCommand( usDeviceID,
                              MCI_ACQUIREDEVICE,
                              MCI_NOTIFY | MCI_ACQUIRE_QUEUE,
                              ( PVOID )&GenericParms,
                              0 );
         if( rc )
            MciError( rc );
      }

      break;

   case WM_COMMAND:

      switch ( SHORT1FROMMP ( mp1 ) )
      {
      case ID_PLAY:

         /* Disable the play and record buttons
          * Enable the stop button.
          */
         WinEnableWindow( hwndPlayButton, FALSE );
         WinEnableWindow( hwndRecordButton, FALSE );
         WinEnableWindow( hwndStopButton, TRUE );

         /* Start playing
          */
         StartPlayBack();

         return( FALSE );

      case ID_RECORD:

         /* The new recording will overwrite the file that was loaded
          * at the start of the program. If the user presses the PLAY
          * button, the newly recorded file will be played.
          */
         if( !ResetRecord() )
         {
            fRecording = TRUE;

            /* Disable the play and record buttons
             * Enable the stop button.
             */
            WinEnableWindow( hwndPlayButton, FALSE );
            WinEnableWindow( hwndRecordButton, FALSE );
            WinEnableWindow( hwndStopButton, TRUE );
         }

         return( FALSE );

      case ID_STOP:

         /* Send message to stop the audio device
          */
         mciSendCommand( usDeviceID,
                         MCI_STOP,
                         MCI_WAIT,
                         ( PVOID )&GenericParms,
                         0 );

         /* If we were recording then reset for playback
          */
         if( fRecording )
         {

            /* Reset the Amp-Mixer device for playback
             */
            ResetPlayBack();
            fRecording = FALSE;

         } /* end if */


         /* Enable the play and record buttons
          */
         WinEnableWindow( hwndRecordButton, TRUE );
         WinEnableWindow( hwndPlayButton, TRUE );
         WinEnableWindow( hwndStopButton, FALSE );

         return( FALSE );
      } /* end switch */

   case WM_CLOSE:

      Close();

      WinPostMsg( hwnd, WM_QUIT, 0L, 0L );
      return( FALSE );

   } /* end switch */

   /* Pass messages on to the frame window
    */
   return ( WinDefDlgProc( hwnd, msg, mp1, mp2 ) );

} /* End MainDialogProc */







 /*************************************************************************
 * Name         : main
 *
 * Description  : Before sending messages to the dialog procedure, the
 *                input file is loaded and the Amp-Mixer device is
 *                setup for playback. This is done to minimise the
 *                percieved delay when the user selects the PLAY button
 *
 * Parameters   : None.
 *
 * Return       : None.
 *
 *************************************************************************/

main ( void )
{
   ULONG    rc;

   HAB      hab;
   HMQ      hmq;
   QMSG     qmsg;
   ULONG    ulIndex;

   /* Initialize PM
    */
   hab = WinInitialize(0);

   /* create a message queue for the window
    */
   hmq = WinCreateMsgQueue(hab,0);

   /* Load the Dialog - This will return the Handle to the Frame
    */
   hwndFrame = WinLoadDlg( HWND_DESKTOP,
                           HWND_DESKTOP,
                           ( PFNWP )MainDialogProc,
                           NULLHANDLE,
                           ID_DIALOG,
                           NULL );

   /* Dispatch messages
    */
   while( WinGetMsg( hab, &qmsg, (HWND) NULL, 0, 0 ) )
      WinDispatchMsg( hab, &qmsg );


   WinDismissDlg( hwndFrame, TRUE );
   WinDestroyMsgQueue( hmq );
   WinTerminate( hab );

   return( FALSE );



} /* end main */
