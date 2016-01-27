Copyright (C) IBM 1993
/******************************************************************/
/* Name: Asymrec.scr                                              */
/*                                                                */
/* This sample source code illustrates some of functions that     */
/* could be used when creating a asymmetric recorder application  */
/* or a application that uses asymmetric recording.               */
/*                                                                */
/* This sample code has been extracted from the Recorder          */
/* Application.  It needs to be noted that this is functional     */
/* code but the functions have been altered to be strictly sample */
/* code without alot of error checking and additional code that a */
/* complete application may need to have.                         */
/*                                                                */
/* Note: Because this is complex sample code most error checking  */
/*       has been removed.                                        */
/*                                                                */
/* Refer to the Asymrec.h file for header information.            */
/*                                                                */
/*                                                                */
/* Functions :  OpenMMIO                                          */
/*              IdentifyIOProc                                    */
/*              AssociateCodec                                    */
/*              InitiateFrameStepRecord                           */
/*              CompressBuffer                                    */
/*              StartVideoThread                                  */
/*                                                                */
/******************************************************************/

/******************************************************************/
/* Name : OpenMMIO                                                */
/*                                                                */
/* This function initializes the environment for frame step       */
/* record.                                                        */
/*                                                                */
/******************************************************************/

RC OpenMMIO(PSWVRCB pCB) /* pCB is a pointer to the PSWVRCB which is */
                         /* a structure in the asymrec.h.  This is   */
                         /* a user defined structure and may be      */
                         /* altered to a users needs.                */
{
  /**********************************************************************/
  /*  Do an MMIOOpen                                                    */
  /*  Save the mmio handle into our SWVRCB.  Use this handle later      */
  /*  to do MMIO_COMPRESS.                                              */
  /**********************************************************************/
   MMMOVIEHEADER     mmMovieHdr;                /* Movie header structure   */
                                                /* contains general info    */
                                                /* about the movie.         */
   MMVIDEOHEADER     mmVideoHdr;                /* Video header structure   */
                                                /* contains info specific to*/
                                                /* the track(video or audio)*/
   MMAUDIOHEADER     mmAudioHdr;                /* Audio header structure   */
   LONG              lBytesWritten;             /* Number of bytes to write */
   ULONG             ulReserved=0;              /* Reserved                 */
   ULONG             ulFlags=0;                 /* Flags                    */
   USHORT            rc=0;                      /* Return code              */
   MMIOINFO          mmioinfo;                  /* MMIO info structure      */


  /**********************************************************************/
  /* Identify and load the IO Proc for creating a new movie file        */
  /**********************************************************************/

   rc = IdentifyIOProc( (PSZ) TEMP_VIDEO_FILENAME,
                         &pCB->hmmioTemp,
                         MMIO_READWRITE |
                         MMIO_CREATE |
                         MMIO_TRANSLATEHEADER,
                         &mmioinfo );

/****************************************************************************/
/* The values chosen for the following structures are values that have been */
/* specifically tweaked for the  Recorder Application or are default.       */
/* These values could change depending on a applications implementation.    */
/* Values that have been changed to default are noted as such.              */
/****************************************************************************/
/****************************************************************************/
/* Set up the movie header structure with appropriate params for the movie. */
/* Once the structure is filled in, we'll go to the video header and set    */
/* that information up.                                                     */
/****************************************************************************/
                                                /* Length of this structure */
   mmMovieHdr.ulStructLen           = sizeof(MMMOVIEHEADER);

   mmMovieHdr.ulContentType         = 0L;       /* Reserved for future use  */
                                                /* Video Media Type         */
   mmMovieHdr.ulMediaType           = MMIO_MEDIATYPE_MOVIE;

   mmMovieHdr.ulMovieCapsFlags      = MOVIE_HAS_VIDEO |
                                      MOVIE_HAS_AUDIO |
                                      MOVIE_CAN_SEEK  |
                                      MOVIE_CAN_SCAN;   /* Capabilities     */

   mmMovieHdr.ulMaxBytesPerSec      = 0L;     /* Maximum transfer rate      */

   mmMovieHdr.ulPaddingGranularity  = 4096;/* Pad to a multiple of this size*/

   mmMovieHdr.ulSuggestedBufferSize = 4096;                 /* Buffer sizer */

   mmMovieHdr.ulStart               = 0L;

   mmMovieHdr.ulLength              = 100L;      /* Length of movie         */

   mmMovieHdr.ulNextTrackID         = 0L;        /* Next available track id */

   mmMovieHdr.ulNumEntries          = 2L;        /* Number of track entries */

   mmMovieHdr.pmmTrackInfoList      = NULL;      /* Track information       */

   mmMovieHdr.ulCountry             = 0L;  /* Country code for title string */

                                      /* Country code page for title string */
   mmMovieHdr.ulCodePage            = 0L;

  /***************************************************************/
  /* This function is called to set the track and to setup       */
  /* extended file information for the movie.  For movie         */
  /* information we reset to -1.                                 */
  /***************************************************************/
   SetActiveTrack ( pCB->hmmioTemp, MMIO_RESETTRACKS );
                                                    /* MMIO_RESETTRACKS = -1 */

  /****************************************************************/
  /* Once we have setup the movie information and set the track   */
  /* for the extended file information we need to set the movie   */
  /* header information.  Track -1 will contain the movie info.   */
  /****************************************************************/
   mmioSetHeader( pCB->hmmioTemp, &mmMovieHdr, sizeof(MMMOVIEHEADER),
                  &lBytesWritten, ulReserved, ulFlags );
  /****************************************************************/
  /* Set up the video header structure with appropriate params    */
  /* for the video.                                               */
  /****************************************************************/

                                               /* length of this structure  */
   mmVideoHdr.ulStructLen        = sizeof(MMVIDEOHEADER);
                                                     /* video content type  */
   mmVideoHdr.ulContentType      = MMIO_VIDEO_DATA;
                                                       /* video media type  */
   mmVideoHdr.ulMediaType        = MMIO_MEDIATYPE_DIGITALVIDEO;
                                                       /* capabilities      */
   mmVideoHdr.ulVideoCapsFlags   = 0L;
                                                    /* video width in pels  */
   mmVideoHdr.ulWidth            = pCB->recopts.usMovieSizeX;
                                                   /* video height in pels  */
   mmVideoHdr.ulHeight           = pCB->recopts.usMovieSizeY;
                                                   /* Scale                 */
   mmVideoHdr.ulScale            = 01L;
                                           /* Rate / Scale == frames/second */
   mmVideoHdr.ulRate             = pCB->recopts.MovieFPS;
   mmVideoHdr.ulStart            = 0L;

                                         /* total number of frames * MMTIME */
   mmVideoHdr.ulLength           = 0L;   /* per frame                       */

   mmVideoHdr.ulTotalFrames      = 0L;      /* total number of video frames */
   mmVideoHdr.ulInitialFrames    = 0L;
                                                /* frame display time or 0L */
   mmVideoHdr.mmtimePerFrame     = 3000/pCB->recopts.MovieFPS;
   mmVideoHdr.genpalVideo.ulStartIndex    = 0L;              /* Start index */
   mmVideoHdr.genpalVideo.ulNumColors     = 0L;                  /* palette */
   mmVideoHdr.genpalVideo.prgb2Entries    = NULL;
                                           /* windows DIB compatible header */
   mmVideoHdr.pmmXDIBHeader               = 0L;

  /***************************************************************/
  /* This function is called to set the track and to setup       */
  /* extended file information for the video.  For video         */
  /* information we set to 0.                                    */
  /***************************************************************/
   SetActiveTrack ( pCB->hmmioTemp, 0L );

  /****************************************************************/
  /* Once we have setup the video information and set the track   */
  /* for the extended file information we need to set the video   */
  /* header information.  Track 0 will contain the video info.    */
  /****************************************************************/
   mmioSetHeader( pCB->hmmioTemp, &mmVideoHdr, sizeof(MMVIDEOHEADER),
                &lBytesWritten, ulReserved, ulFlags );


  /****************************************************************/
  /* Update/create audio header information.  It is being shown   */
  /* in this sample but it is assumed that audio data is recorded */
  /* at the time of interleaving.  This audio header information  */
  /* is provided only for initial setup and reference when opening*/
  /* mmio.                                                        */
  /****************************************************************/
   mmAudioHdr.dwHeaderLength  = sizeof(MMAUDIOHEADER);
   mmAudioHdr.dwContentType   = MMIO_AUDIO_MUSIC;
   mmAudioHdr.dwMediaType     = MMIO_MEDIATYPE_AUDIO;

  /*********************************************************/
  /* Assign values to - mmAudioHdr.mmXWAVHeader.WAVRHeader */
  /*********************************************************/
   mmAudioHdr.mmXWAVHeader.WAVEHeader.usFormatTag      = 01L;
                                                           /* Stero vs mono */
   mmAudioHdr.mmXWAVHeader.WAVEHeader.usChannels       = 02L;

                                                           /* Sampling size */
   mmAudioHdr.mmXWAVHeader.WAVEHeader.ulSamplesPerSec  = 22050L;

                                                      /* Avg bytest per sec */
   mmAudioHdr.mmXWAVHeader.WAVEHeader.ulAvgBytesPerSec = 0L;

                                                /* Block alignment in bytes */
   mmAudioHdr.mmXWAVHeader.WAVEHeader.usBlockAlign     = 02L;

                                                         /* Bits per sample */
   mmAudioHdr.mmXWAVHeader.WAVEHeader.usBitsPerSample  = 16L;

  /***************************************************************/
  /* Assign values to - mmAudioHdr.mmXWAVHeader.XWAVEHeaderInfo  */
  /***************************************************************/

   mmAudioHdr.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInMS    = 0L;
   mmAudioHdr.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes = 0L;
   mmAudioHdr.mmXWAVHeader.XWAVHeaderInfo.ulReserved           = 0L;

  /***************************************************************/
  /* This function is called to set the track and to setup       */
  /* extended file information for the audio.  For audio         */
  /* information we set to 1.                                    */
  /***************************************************************/
   SetActiveTrack ( pCB->hmmioTemp, 1L );


  /****************************************************************/
  /* Once we have setup the audio information and set the track   */
  /* for the extended file information we need to set the audio   */
  /* header information.  Track 1 will contain the audio info.    */
  /****************************************************************/
   rc = mmioSetHeader( pCB->hmmioTemp, &mmAudioHdr, sizeof(MMAUDIOHEADER),
                       &lBytesWritten, ulReserved, ulFlags );

  /***************************************************************/
  /* This function is called to reset the track and to setup     */
  /* extended file information.                                  */
  /***************************************************************/
   SetActiveTrack ( pCB->hmmioTemp, MMIO_RESETTRACKS );
                                                    /* MMIO_RESETTRACKS = -1*/
   return(rc);

}  /* OpenMMIO */

/******************************************************************/
/* Name : IdentifyIOProc                                          */
/*                                                                */
/* This function identifies the AVIO procedure.                   */
/*                                                                */
/******************************************************************/
RC   IdentifyIOProc( PSZ       fname,             /* File name              */
                     HMMIO    *phmmio,            /* Handle to open file    */
                     ULONG     ulOpenFlags,       /* Flags                  */
                     MMIOINFO *pmmioinfo )        /* mmio info structure    */
{
  APIRET             rc = MCIERR_SUCCESS;
  MMIOINFO           mmioinfo;
  LPMMIOPROC         pAnswer = 0;
  CHAR               LoadError[100];
  FOURCC             fcc;
  ULONG              ulrc;
  HMODULE            hmod;

  /***********************************/
  /* Open/Identify IOProc to install */
  /***********************************/
  memset(pmmioinfo, '\0', sizeof(MMIOINFO));  /* Reset mmioinfo area      */


  /******************************************/
  /* Extract from the file header and place */
  /* in the appropriate fileds.              */
  /******************************************/
  pmmioinfo->dwTranslate = MMIO_TRANSLATEHEADER;

  /******************************************/
  /* Media type is Movie.                   */
  /******************************************/
  pmmioinfo->adwInfo[3] = MMIO_MEDIATYPE_MOVIE;

  rc = 0;  /* reset DosLoadModule call */

  /*************************************************************************/
  /* Check to see if it is a known IO proc if not then load AVIO straight  */
  /*************************************************************************/
  fcc = mmioStringToFOURCC( (PSZ) "AVI ", MMIO_TOUPPER);
  if (!(pAnswer = mmioInstallIOProc(fcc, NULL, MMIO_FINDPROC)))
    {
    /********************************************/
    /* Load The IO Proc                         */
    /********************************************/
    rc = DosLoadModule( (PSZ)LoadError, sizeof(LoadError), (PSZ) "AVIO", &hmod);

    /********************************************/
    /* Obtain Procedure Entry Point             */
    /********************************************/
    if (rc = DosQueryProcAddr(hmod, 0L, (PSZ) "IOProc_Entry",
                              (PFN *)&(pmmioinfo->pIOProc)))
      {
      ulrc = DosFreeModule(hmod);
      return(rc);
      }

    /********************************************/
    /* Install the IO Proc                      */
    /********************************************/
    pAnswer = mmioInstallIOProc(fcc,
                                pmmioinfo->pIOProc,
                                MMIO_INSTALLPROC);
    if ((pAnswer != pmmioinfo->pIOProc) || (!pAnswer))
      {
      ulrc = DosFreeModule(hmod);
      return (MCIERR_CANNOT_LOAD_DRIVER);
      }
    } /* FindProc */

   /**************************************************/
   /* Try to Open a new Movie file using this IOProc */
   /**************************************************/
  pmmioinfo->pIOProc = pAnswer;  /* Install worked! */
  pmmioinfo->fccIOProc = 0;
  pmmioinfo->dwTranslate = MMIO_TRANSLATEHEADER;
  pmmioinfo->hmmio = *phmmio;
  *phmmio = mmioOpen((PSZ) fname, pmmioinfo, ulOpenFlags );
  if (!*phmmio)
     rc = MCIERR_CANNOT_LOAD_DRIVER;
  return(rc);

}  /* end: IdentifyIOProc               */

/**********************************************************************/
/* Name : AssociateCodec                                              */
/*                                                                    */
/* This function gets the selected codec and tells MMIO to associate  */
/* it to the IOPROC for later use in the compression. Sets up color   */
/* coding from the card and various structures for initial association*/
/* of codec.                                                          */
/**********************************************************************/
LONG AssociateCodec( HMMIO              hmmio,   /* Handle to open file     */
                     PSWVRCB            pCB,     /* Pointer to control block*/
                     CODECINIFILEINFO  *pcifi,/* Pointer to codec file info */
                     PVOID              pControlHdr,
                     ULONG              ulCODECFlags )   /* Flags           */
{
   RC                rc;

   MMEXTENDINFO      mmextendinfo;
   MMVIDEOOPEN       mmvideoopen;

  /*****************************************************************/
  /* The following structures are used by the codec during the     */
  /* recording process.                                            */
  /*****************************************************************/
   CODECASSOC        codecassoc;
   CODECOPEN         codecopen;
   CODECVIDEOHEADER  cvhSrc;
   CODECVIDEOHEADER  cvhDst;

   MCI_STATUS_PARMS  mciStatusParms;
   ULONG             ulSrcColorEncoding = MMIO_RGB_5_6_5;

  /*****************************************************************/
  /* Get the color encoding of the video capture card              */
  /* otherwise hardcode to RGB                                     */
  /*****************************************************************/
   memset(&mciStatusParms, 0, sizeof(MCI_STATUS_PARMS));
   mciStatusParms.dwCallback   = 0;
   mciStatusParms.dwItem       = MCI_DGV_STATUS_IMAGE_PELFORMAT;
   rc = mciSendCommand (pCB->OutputMovie.wDeviceID,     /* Device ID        */
                        MCI_STATUS,                     /* Command          */
                        MCI_WAIT | MCI_STATUS_ITEM,     /* dwFlags          */
                        (ULONG) (&mciStatusParms),      /* In/Out Parameters*/
                         0 );                           /* User Parameter   */
   rc &= 0x0000FFFF ;                  /* Mask out the high (device ID) word*/
   if (!rc)
     ulSrcColorEncoding = mciStatusParms.dwReturn;

  /*****************************************************************/
  /* Initialize source codec video header                          */
  /* This structure is used later in this routine as a field       */
  /* in the codecopen structure.                                   */
  /*****************************************************************/
   cvhSrc.ulStructLen      = sizeof(CODECVIDEOHEADER);
   cvhSrc.cx               = pCB->recopts.usMovieSizeX;  /* 320 Default     */
   cvhSrc.cy               = pCB->recopts.usMovieSizeY;  /* 240 Default     */
   cvhSrc.cPlanes          = 1;
   cvhSrc.cBitCount        = 16;
   cvhSrc.ulColorEncoding  = ulSrcColorEncoding;

  /*****************************************************************/
  /* Initialize destination codec video header                     */
  /* This structure is used later in this routine as a field       */
  /* in the codecopen structure.                                   */
  /*****************************************************************/
   cvhDst.ulStructLen      = sizeof(CODECVIDEOHEADER);
   cvhDst.cx               = pCB->recopts.usMovieSizeX;
   cvhDst.cy               = pCB->recopts.usMovieSizeY;
   cvhDst.cPlanes          = 1;
   cvhDst.cBitCount        = 16;
   cvhDst.ulColorEncoding  = MMIO_COMPRESSED;

  /*****************************************************************/
  /* Initialize mmvideoopen structure                              */
  /* This structure is used later in this routine as a field       */
  /* in the codecopen structure.                                   */
  /*****************************************************************/
   mmvideoopen.ulStructLen = sizeof(MMVIDEOOPEN);

  /*****************************************************************/
  /* This is max quality derived from quality slider               */
  /* in options notebook. 10,000 for high quality.                 */
  /* Some codecs do not support setting quality a max              */
  /* data rate.                                                    */
  /*****************************************************************/
   mmvideoopen.ulQuality   = pCB->recopts.usQuality;

   if ( pCB->recopts.fInsertRefFrames )  /* Is the reference frame flag set*/
   /*****************************************/
   /* Maximum refresh time = 30 for default */
   /*****************************************/
      mmvideoopen.ulKeyFrameRate    = pCB->recopts.usMaxRefreshTime;
   else
      mmvideoopen.ulKeyFrameRate    = 0;
   mmvideoopen.ulScale              = 1;
   /****************************************/
   /* MovieFPS = input 15 output 15.       */
   /* This is to compensate frame rate for */
   /* difference in input and output.      */
   /****************************************/
   mmvideoopen.ulRate               = pCB->recopts.MovieFPS;
   /****************************************/
   /* MaxDataRate = 1000 bytes/sec         */
   /****************************************/
   mmvideoopen.ulDataConstraint     = pCB->recopts.usMaxDataRate << 10;
   mmvideoopen.ulConstraintInterval = pCB->recopts.MovieFPS;

  /****************************************************************/
  /* Initialize codec open structure                              */
  /****************************************************************/
   memset((PVOID)&codecopen,'\0', sizeof(CODECOPEN));

  /****************************************************************/
  /* Codec - ulFlags need to be specific if the codec supports    */
  /* compress and decompress.                                     */
  /****************************************************************/
               /* flags & events - Refer to ulCapsFlags in CODECINIFILEINFO   */

  /****************************************************************/
  /* Initialize the codecopen and codecassoc structure with the   */
  /* the following information.                                   */
  /****************************************************************/

   codecopen.ulFlags                = ulCODECFlags;
                                       /* control header - (codec specific)   */
   codecopen.pControlHdr            = pControlHdr;
                                          /* source header information from   */
                                          /* the CODECVIDEOHEADER             */
   codecopen.pSrcHdr                = (PVOID)&cvhSrc;
                                     /* destination header information from   */
                                     /* the CODECVIDEOHEADER                  */
   codecopen.pDstHdr                = (PVOID)&cvhDst;
                                           /* other information MMVIDEOOPEN   */
   codecopen.pOtherInfo             = (PVOID)&mmvideoopen;
                                              /* Codec specific open header   */
   codecassoc.pCodecOpen            = &codecopen;
                                              /* Codecinifileinfo             */
   codecassoc.pCODECIniFileInfo     = pcifi;

  /****************************************************************/
  /* Initialize mmextedinfo structure for MMIO_SET                */
  /****************************************************************/
   memset((PVOID)&mmextendinfo,'\0', sizeof(MMEXTENDINFO));
   mmextendinfo.ulStructLen         = sizeof(MMEXTENDINFO);
   mmextendinfo.ulFlags             = MMIO_CODEC_ASSOC;
   mmextendinfo.ulNumCODECs         = 1;
   mmextendinfo.pCODECAssoc         = &codecassoc;

  /****************************************************************/
  /* Send MIOM_SET to IOproc to associate the codec               */
  /****************************************************************/
   rc =  mmioSendMessage( hmmio,
                          MMIOM_SET,
                          (LONG)&mmextendinfo,
                          MMIO_SET_EXTENDEDINFO);
   return(rc);

} /* AssociateCodec */

/******************************************************************/
/* Name : InitiateFrameStepRecord                                 */
/*                                                                */
/* This function initializes the environment for frame step       */
/* record.                                                        */
/*                                                                */
/******************************************************************/
LONG InitiateFrameStepRecord( PSWVRCB pswvrcb)
{
  ULONG    rc;                           /* return code for random usage   */
  BOOL     fInitErr = FALSE;
  PRECCB   pRecCB;                       /* ptr to a recording cb structure*/
  ULONG    ulBufferSize;


 /********************************************************************/
 /* save a global pointer to the control block                       */
 /********************************************************************/
  threadpointer = pswvrcb;

 /********************************************************************/
 /* Allocate record control if it doesn't exist and set it           */
 /* into our control block.  Then clear it out before we start       */
 /* using it.                                                        */
 /********************************************************************/
  if (!pswvrcb->pRecCB)

  {
    pswvrcb->pRecCB = malloc( sizeof(RECCB));       /* Allocate record CB  */
  } /* endif */
  pRecCB = pswvrcb->pRecCB;

  memset(pRecCB, '\0', sizeof(RECCB));

 /********************************************************************/
 /* Set the threads running state as ok.  This flag is check in      */
 /* StartVideoThread routine.                                        */
 /********************************************************************/
  pRecCB->fStillRunning= TRUE;

 /********************************************************************/
 /* Create an event semaphore to indicate paused recording           */
 /* Set error flag for failure                                       */
 /********************************************************************/
  rc = DosCreateEventSem((PSZ) "\\SEM32\\ASYM\\RECPAUSE", /*semaphore name   */
                         &pRecCB->hevRecordPaused,        /*semaphore handle */
                         (ULONG) 0,                    /* attribute - unused */
                         (ULONG) 0);                   /* initial state set  */

  if (rc) fInitErr = TRUE;

 /********************************************************************/
 /* Create an event semaphore to signal frame step complete          */
 /* Set error flag for failure                                       */
 /********************************************************************/
  rc = DosCreateEventSem((PSZ) "\\SEM32\\ASYM\\FRMSTEP",  /* semaphore name  */
                         &pRecCB->hevFrameStep,           /* semaphore handle*/
                         (ULONG) 0,                    /* attribute - unused */
                         (ULONG) 0);                   /* initial state set  */

  if (rc) fInitErr = TRUE;

 /********************************************************************/
 /* Get a buffer for capturing the raw image.  Set error flag for    */
 /* failure.  The value we are using for the multiply represents     */
 /* 16 bit.  1 = 8 bit, 2 = 16 bit, 3 = 24 bit.                      */
 /********************************************************************/
  ulBufferSize = pswvrcb->recopts[pswvrcb->usCurrProf].usMovieSizeX *
                 pswvrcb->recopts[pswvrcb->usCurrProf].usMovieSizeY * 2;

  rc = DosAllocMem ( (PPVOID) &pRecCB->rawimagebuf,
                     ulBufferSize,
                     (ULONG) PAG_COMMIT | PAG_READ | PAG_WRITE);

  if (rc) fInitErr = TRUE;

 /********************************************************************/
 /* Get a buffer for holding the compressed image                    */
 /* Set error flag for failure                                       */
 /********************************************************************/
  rc = DosAllocMem ( (PPVOID) &pRecCB->compimagebuf, ulBufferSize,
                     (ULONG) PAG_COMMIT | PAG_READ | PAG_WRITE);

  if (rc) fInitErr = TRUE;

 /*********************************************************************/
 /* If an error occured return error, so that recording will not start*/
 /*********************************************************************/
  if (fInitErr) {
     return(1);
  } /* endif */

 /******************************************************************/
 /* Save the Wave file that was previously recorded if it has not  */
 /* been saved.                                                    */
 /******************************************************************/
  lpWaveSave.dwCallback = 0;
  lpWaveSave.lpFileName = 0;
  rcode =
          mciSendCommand (
          pswvrcb->wWaveDID,       /* Device ID returned from OPEN */
          MCI_SAVE,                /* Command                      */
          MCI_WAIT,                /* dwFlags                      */
         (ULONG)&lpWaveSave,       /* In/Out Parameters            */
           0 );                    /* User Parameter               */

 /******************************************************************/
 /* We need to close the waveaudio so we can open digitalvideo02.  */
 /* This is done because some audio adapters do not support        */
 /* multiple instances.                                            */
 /******************************************************************/
  rcode = mciSendCommand( pswvrcb->wWaveDID,
                          MCI_CLOSE,
                          MCI_WAIT,
                          (ULONG) &lpGenparms, 0 );

  if (SendString ((PSZ) "OPEN",
                        (PSZ) pswvrcb->OutputMovie.szDeviceName,
                        (PSZ) "ALIAS",
                        (PSZ) pswvrcb->OutputMovie.szDeviceAlias,
                        (PSZ) "WAIT",
                         NULL,
                         NULL))
    {
       WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,
          (PSZ) "error opening MCD",
          (PSZ) errnostr, 0, MB_OK | MB_MOVEABLE);
    }
    else
    {
     pswvrcb->OutputMovie.wDeviceID =
                   mciGetDeviceID( (LPSTR) pswvrcb->OutputMovie.szDeviceAlias);
    }
 /*********************************************************************/
 /* If we have any problems with the codec data, initializing our     */
 /* environment for frame step recording or associating the codec     */
 /* then return.  We cannot proceed with recording.                   */
 /*********************************************************************/
  if (!pswvrcb->pCODECData      ||
      (rc = OpenMMIO(pswvrcb)) ||
      (rc =  AssociateCodec( pswvrcb->hmmioTemp,
                             pswvrcb,
                             &pswvrcb->pCODECData->cifi,
                             (PVOID)NULL,
                             CODEC_COMPRESS )));
  {
  return(rc);
  }

 /********************************************************************/
 /* Everything is ok so far, start the frame step recording thread   */
 /********************************************************************/
  rc = DosCreateThread ((PTID) &pRecCB->RecordThreadID,   /* Thread ID       */
                        (PFNTHREAD) StartVideoThread,     /* Thread addr     */
                        (ULONG) pswvrcb,
                        (ULONG) 0,                        /* Flags           */
                        (ULONG) RecordThreadStackSize);   /* stack size      */

 /********************************************************************/
 /* Change record thread to regular class, delta = -4.               */
 /* Set error flag for failure                                       */
 /********************************************************************/
  rc = DosSetPriority ( 2L, 2L, -4L, pRecCB->RecordThreadID);

  if (rc)
  {
     fInitErr = TRUE;
     return(rc);
  }

  return(0L);
} /* end: InitiateFrameStepRecord      */

/*********************************************************************/
/* Name : CompressBuffer                                             */
/*                                                                   */
/* This function calls the IO proc to compress a raw image buffer    */
/*                                                                   */
/*********************************************************************/
RC CompressBuffer (HMMIO         hmmio        /* Handle to open file */
                   PSWVRCB       pCB,       /* Control block pointer */
                   PRECCB        pRecCB)  /* Pointer to record block */
{
 /********************************************************************/
 /* Compression variables                                            */
 /********************************************************************/
  RC                        rcode = NO_ERROR;
  MMCOMPRESS                CompressInfo;
  MMVIDEOCOMPRESS           VideoCompressInfo;

  /*******************************************************************/
  /* Initialize the compression information                          */
  /*******************************************************************/
   memset( &VideoCompressInfo, 0, sizeof(MMVIDEOCOMPRESS));

   CompressInfo.ulStructLen   = sizeof( MMCOMPRESS );

   CompressInfo.pRunTimeInfo  = &VideoCompressInfo;

   CompressInfo.ulSrcBufLen   = pswrcb->recopts.usMovieSizeX *
                                pswrcb->recopts.usMovieSizeY * 2;

   CompressInfo.pSrcBuf       = pRecCB->rawimagebuf;

   CompressInfo.ulDstBufLen   = pswrcb->recopts.usMovieSizeX *
                                pswrcb->recopts.usMovieSizeY * 2;

   CompressInfo.pDstBuf       = pRecCB->compimagebuf;

  /*******************************************************************/
  /* If we are inserting reference frames, check to see if it's now. */
  /* If its the 15th frame or 1st frame insert a reference frame.    */
  /*******************************************************************/
   if ( pCB->recopts.fInsertRefFrames )
      if ( !( pRecCB->FramesCounted % pCB->recopts.usMaxRefreshTime ) )
      {
        /**************************************************************/
        /* Compress a reference frame                                 */
        /**************************************************************/
         CompressInfo.ulFlags = MMIO_IS_KEY_FRAME;
         rcode = mmioSendMessage( pCB->hmmioTemp, MMIOM_COMPRESS,
                               (ULONG)&CompressInfo, 0L);
      }
      else
      {
        /**************************************************************/
        /* Compress a delta frame                                     */
        /**************************************************************/
         CompressInfo.ulFlags = 0;
         rcode = mmioSendMessage( pCB->hmmioTemp, MMIOM_COMPRESS,
                               (ULONG)&CompressInfo, 0L);
      }
  /**************************************************************/
  /* Increment our frame counter                                */
  /**************************************************************/
   pRecCB->FramesCounted++;

  /**************************************************************/
  /* Determine total data rate written for this frame           */
  /**************************************************************/
   pRecCB->outbuflen  = CompressInfo.ulDstBufLen;

   return(rcode);

} /* Compress Buffer */

/***************************************************************************/
/* Name : StartVideoThread                                                 */
/*                                                                         */
/* Function: Recording thread for frame step.  This function is assuming   */
/* that we have previously recorded the audio to a temporary file that     */
/* will be used in this function.  We also create a temporary file that    */
/* is used for the data after it has been interleaved with the audio.      */
/*                                                                         */
/* InputMovie is referencing the laser disk device and OutputMovie is      */
/* referencing the digitalvideo02 adapter.                                 */
/*                                                                         */
/* Once everything is setup, compress the video then send the              */
/* MMIOM_MULTITRACKWRITE message to interleave the audio and video data    */
/* and create a movie file.  Since this is only a sample we are only       */
/* writing out 1 frame of video and whatever audio that goes with that     */
/* video.  This structure may be set up for multiple records of video and  */
/* audio.                                                                  */
/*                                                                         */
/***************************************************************************/
VOID StartVideoThread( VOID )
{
 /*****************************************************************/
 /* Various return code variables and strings                     */
 /*****************************************************************/
  ULONG                  rcode = NO_ERROR;
  ULONG                  rc;
  char                   errnostr[20];

 /*****************************************************************/
 /* Semaphore variables                                           */
 /*****************************************************************/
  ULONG                  Postcount;
  HMQ                    hmq = WinCreateMsgQueue( hab, 0 );

 /******************************************************************/
 /* External device mci variables structs for mcisendcommand       */
 /******************************************************************/
  MCI_SEEK_PARMS         lpExtdevSeek;
  MCI_STEP_PARMS         lpExtdevStep;
  MCI_STATUS_PARMS       lpStatusParms;

 /******************************************************************/
 /* Audio device mci variables structs for mcisendcommand          */
 /******************************************************************/
  MCI_SAVE_PARMS         lpWaveSave;
  MCI_GENERIC_PARMS      lpGenparms;

 /******************************************************************/
 /* Variables for maintaining data rate requested.                 */
 /* Must use for skipping frames to maintain desired data rate     */
 /******************************************************************/
  ULONG                  ulCurrentSkipPct,
                         ulSkipPercentage;
  ULONG                  ulFrames;

 /******************************************************************/
 /* Compression variables                                          */
 /******************************************************************/
  MCI_IMAGE_PARMS        mciImageParms;
  IRECT                  dest_rect;
  IRECT                  source_rect;

 /******************************************************************/
 /* Varibles needed to frame step record video and interleave      */
 /* the audio.                                                     */
 /******************************************************************/
  MMMULTITRACKWRITE      MMMultiTrackWrite; /* MMMULTITRACKWRITE Structure */

  TRACKMAP               TrackMap[2];           /* TRACKMAP Structure      */

  RECORDTAB              Aud;      /* Audio RECORDTAB Structure            */

  RECORDTABWRITE         Vid;      /* Video RECORDTAB Structure            */

 /******************************************************************/
 /* Varibles needed to break up Audio into 1 frame chunks.         */
 /******************************************************************/
  HMMIO                  hmmiowav;
  PMMAUDIOHEADER         pMMAudioHeader;
  LONG                   lBytes;
  LONG                   lBytesRead;
  LONG                   lSize;
  ULONG                  ulAudioBytesPerFrame;
  ULONG                  ulReserved;
  ULONG                  ulFlags;
  PSZ                    pAudioBuffer;

 /******************************************************************/
 /* Main control block and record control block                    */
 /* Get main from global threadpointer since it can't be passed in */
 /******************************************************************/
  PRECCB                 pRecCB;
  PSWVRCB                pswvrcb = threadpointer;

 /******************************************************************/
 /* Get record control block off of main control block             */
 /******************************************************************/
  pRecCB = pswvrcb->pRecCB;


 /******************************************************************/
 /* Get exclusive control of the Output device now.  We want       */
 /* exclusive use so we don't loose control in the middle of       */
 /* recording.                                                     */
 /******************************************************************/
  lpGenparms.dwCallback = 0;
  rcode = mciSendCommand( pswvrcb->OutputMovie.wDeviceID,
                          MCI_ACQUIREDEVICE,
                          MCI_WAIT | MCI_EXCLUSIVE,
                          (ULONG) &lpGenparms, 0 );

 /******************************************************************/
 /* Get exclusive control of the Input device now if we have one   */
 /******************************************************************/
  if (!rcode && pswvrcb->InputMovie.wDeviceID)
  {
    lpGenparms.dwCallback = 0;
    rcode = mciSendCommand( pswvrcb->InputMovie.wDeviceID,
                            MCI_ACQUIREDEVICE,
                            MCI_WAIT | MCI_EXCLUSIVE,
                            (ULONG) &lpGenparms, 0 );
  }

 /******************************************************************/
 /* Initialize current input positions (laser disk)                */
 /******************************************************************/
  if (!rcode)
  {
    pRecCB->dwSrcCurrentPosition   = pswvrcb->dwSrcRecordFrom;
    pRecCB->dwMovieRecordFrom      = 0;
    pRecCB->dwMovieCurrentPosition = 0;

  } /* endif */

 /*******************************************************************/
 /* Set the time format on external video device to MMTIME format   */
 /*******************************************************************/
  lpExtdevSet.dwCallback   = 0;
  lpExtdevSet.dwTimeFormat = MCI_FORMAT_MMTIME;
  rcode = mciSendCommand (pswvrcb->InputMovie.wDeviceID,     /* Device ID */
                          MCI_SET,                           /* Command   */
                          MCI_WAIT | MCI_SET_TIME_FORMAT,    /* dwFlags   */
                          (ULONG) (&lpExtdevSet),    /* In/Out Parameters */
                          0 );                       /* User Parameter    */

 /******************************************************************/
 /*    Seek external video device to first frame                   */
 /******************************************************************/
  lpExtdevSeek.dwCallback = 0;
  lpExtdevSeek.dwTo       = pswvrcb->dwSrcRecordFrom;
  rcode = mciSendCommand ( pswvrcb->InputMovie.wDeviceID,  /* Device ID      */
                           MCI_SEEK,                       /* Command        */
                           MCI_WAIT | MCI_TO,              /* dwFlags        */
                           (ULONG) (&lpExtdevSeek), /* In/Out Parameters     */
                           0 );                     /* User Parameter        */


  if (!rcode)
    {
    /******************************************************************/
    /* Calculate the number of frames on the source device we need to */
    /* skip per movie frame to maintain the desired capture frame rate*/
    /******************************************************************/
    ulCurrentSkipPct = 0L;
    ulSkipPercentage = (((pswvrcb->recopts.SrcFPS -
                          pswvrcb->recopts.MovieFPS) * 1000 ) +
                          pswvrcb->recopts.MovieFPS - 1 ) /
                          pswvrcb->recopts.MovieFPS;

    /******************************************************************/
    /* Set the source rectangle size for the capture.                 */
    /* This is for how much we are capturing.                         */
    /******************************************************************/
    source_rect.X_left   =  pswvrcb->recopts.usCapPosX; /* Default 0         */
    source_rect.Y_top    =  pswvrcb->recopts.usCapPosY; /* Default 0         */
    source_rect.X_width  =  pswvrcb->recopts.usCapSizeX;/* Default 640       */
    source_rect.Y_height =  pswvrcb->recopts.usCapSizeY;/* Default 480       */

    /******************************************************************/
    /* Set the destination rectangle size to the desired movie size   */
    /******************************************************************/
    dest_rect.X_left   = 0;
    dest_rect.Y_top    = 0;
                                  /* Destination rectangle width for capture */
    dest_rect.X_width  = pswvrcb->recopts.usMovieSizeX;  /* default 320      */
                                 /* Destination rectangle height for capture */
    dest_rect.Y_height = pswvrcb->recopts.usMovieSizeY;  /* default 240      */
  } /* endif */

 /******************************************************************/
 /* Open up the Audio file.                                        */
 /******************************************************************/
   hmmiowav = mmioOpen( TempAudioFilename,
                        NULL,
                        0);

 /******************************************************************/
 /* Query header length from audio file.                           */
 /******************************************************************/
  if (!rcode)
    rcode = mmioQueryHeaderLength(hmmiowav,&lSize,0L,0L);

  if (!rcode)
    pMMAudioHeader = malloc(lSize);

 /***********************************************************************/
 /***********************************************************************/
 /* Get main header information from the previously recorded audio file.*/
 /* This information will be set into the new movie files audio track   */
 /* header.                                                             */
 /***********************************************************************/
  if (!rcode)
    rcode = mmioGetHeader(hmmiowav,
                          (PVOID)(pMMAudioHeader),
                          lSize, &lBytesRead, 0L, 0L);

  /***************************************************************/
  /* This function is called to set the track and to setup       */
  /* extended file information for the audio.  For audio         */
  /* information we set to 1.                                    */
  /***************************************************************/
   SetActiveTrack ( pswvrcb->hmmioTemp, 1L );

  /******************************************************************/
  /* Set the audio header information in the new movie file.        */
  /******************************************************************/
   rc = mmioSetHeader( pswvrcb->hmmioTemp,
                                pMMAudioHeader,
                                sizeof(MMAUDIOHEADER),
                                &lBytes,
                                ulReserved,
                                ulFlags );

  /***************************************************************/
  /* This function is called to reset the track and to setup     */
  /* extended file information for the audio.                    */
  /***************************************************************/
   SetActiveTrack ( pswvrcb->hmmioTemp, MMIO_RESETTRACKS );
                                                    /* MMIO_RESETTRACKS = -1 */

  /******************************************************************/
  /* Calculate Number of Audio Bytes Per Frame of Video because     */
  /* we will be interleaving the audio and video at 1 to 1.         */
  /******************************************************************/
   ulAudioBytesPerFrame = pMMAudioHeader->mmXWAVHeader.
                                          WAVEHeader.
                                          ulAvgBytesPerSec /
                                          pswvrcb->recopts.MovieFPS;
   pAudioBuffer = malloc(ulAudioBytesPerFrame);
   memset(pAudioBuffer, 0, ulAudioBytesPerFrame);

   free(pMMAudioHeader);

 /******************************************************************/
 /* This is the frame step recording loop                          */
 /* Recording continues until we reach the recording end point     */
 /* on the source device, or until the user presses stop.          */
 /******************************************************************/

  /***************************************************************/
  /* This function is called to reset the track and to setup     */
  /* extended file information.                                  */
  /***************************************************************/
  rcode = SetActiveTrack(pswvrcb->hmmioTemp, MMIO_RESETTRACKS);
                                                    /* MMIO_RESETTRACKS = -1 */

  while (!rcode &&
          pswvrcb->fRecordActive &&
         (pswvrcb->InputMovie.wDeviceID ?
         (pRecCB->dwSrcCurrentPosition <= pswvrcb->dwSrcRecordTo) : TRUE) )
  {
   /******************************************************************/
   /*    Block at this point if record operation is paused           */
   /******************************************************************/
     while ( pswvrcb->fRecordPaused && pRecCB->fStillRunning )
     {
        rc = DosResetEventSem( pRecCB->hevRecordPaused, &Postcount);
        rc = DosWaitEventSem( pRecCB->hevRecordPaused, (ULONG) -1);
     } /* endwhile */

     /******************************************************************/
     /* make sure we were not stopped while we were paused             */
     /******************************************************************/
     if ( pswvrcb->fRecordActive && !pswvrcb->fRecordPaused )
     {

        if (pswvrcb->recopts.fCompress)
        {

          /*****************************************************************/
          /* Initialize mciImageParm variable before sending to MCD        */
          /*****************************************************************/
           memset( &mciImageParms, 0, sizeof(mciImageParms));
           mciImageParms.dwBufLen = pswvrcb->recopts.usMovieSizeX *
                                    pswvrcb->recopts.usMovieSizeY * 2;
          /*****************************************************************/
          /* rawimagebuf is where we put our output from the GetImageBuffer*/
          /*****************************************************************/
           mciImageParms.pPelBuffer=(PVOID *)pRecCB->rawimagebuf;
           mciImageParms.rect.xLeft        = source_rect.X_left;
           mciImageParms.rect.yBottom      = source_rect.Y_top;
           mciImageParms.rect.xRight       = source_rect.X_left +
                                             source_rect.X_width;
           mciImageParms.rect.yTop         = source_rect.Y_top  +
                                             source_rect.Y_height;
           mciImageParms.dwPelBufferWidth  = pswvrcb->recopts.usMovieSizeX;
           mciImageParms.dwPelBufferHeight = pswvrcb->recopts.usMovieSizeY;

          /*****************************************************************/
          /* Get image data for this frame into raw image buffer           */
          /*****************************************************************/
           rcode = mciSendCommand( pswvrcb->OutputMovie.wDeviceID,
                                   MCI_GETIMAGEBUFFER,
                                   MCI_WAIT | MCI_USE_HW_BUFFER,
                                   (ULONG)&mciImageParms,
                                   0);

        }

       /*******************************************************************/
       /* Figure out how many frames to advance the external video device */
       /* to maintain desired data rate                                   */
       /*******************************************************************/
        ulCurrentSkipPct           += ulSkipPercentage;
        ulFrames                    = ulCurrentSkipPct / 1000L;
        ulCurrentSkipPct           -= ulFrames * 1000L;
        if ( ulCurrentSkipPct < 50L )
           ulCurrentSkipPct = 0L;

       /********************************************************************/
       /* Advance the input device.                                        */
       /* If device is not controllable, ask the user to do it.            */
       /********************************************************************/
        if (!rcode && pswvrcb->InputMovie.wDeviceID)
        {

          /*****************************************************************/
          /* Clear the semaphore that signals the external device          */
          /* frame step complete                                           */
          /*****************************************************************/
           rc = DosResetEventSem( pRecCB->hevFrameStep, &Postcount);

          /*****************************************************************/
          /* Send STEP to external video device.  This is stepping the     */
          /* input device (laser disk).                                    */
          /*****************************************************************/
           lpExtdevStep.dwCallback = vhwndPlayerDialog;
           ulFrames++;
           lpExtdevStep.dwStep     = ulFrames * 3000/pswvrcb->recopts.SrcFPS;
           mciSendCommand ( pswvrcb->InputMovie.wDeviceID,
                            MCI_STEP,
                            MCI_NOTIFY | MCI_STEP_FRAMES,
                            (ULONG) (&lpExtdevStep),
                            lpExtdevStep.dwStep );
        }

       /*****************************************************************/
       /* Since we are compressing during record,                       */
       /* compress the image contained in rawimagebuf and put the       */
       /* compressed output into the stream buffer                      */
       /*****************************************************************/
        if (!rcode && pswvrcb->recopts.fCompress)
        {

           /*****************************************************************/
           /* Compress - Input raw image buffer is in pRecCB->rawimagebuf.  */
           /*****************************************************************/
            rcode = CompressBuffer (pswvrcb->hmmioTemp,
                                    pswvrcb,
                                    pRecCB);

           /*****************************************************/
           /* Get a chunk of Audio data                         */
           /*****************************************************/
            lBytes = mmioRead( hmmiowav,
                             (PSZ)pAudioBuffer,
                             ulAudioBytesPerFrame);

           /*****************************************************/
           /* Initialize these MMMultiTrackWrite fields for     */
           /* later use in the mmioSendMessage.                 */
           /*****************************************************/

            MMMultiTrackWrite.ulFlags = 0L;

            MMMultiTrackWrite.Reserved = 0L;

          /****************************************************************/
          /* Specifies the number of TRACKMAP entries in pTrackMapList on */
          /* output.  Currently we have audio and video                   */
          /****************************************************************/
            MMMultiTrackWrite.ulNumTracks = 2;

           /*****************************************************************/
           /* We do not need to assign/specify the next element.  Using     */
           /* the MMMULTITRACKWRITE the IO proc handles this.               */
           /* The MMMULTITRACKWRITE does the interleaving of the video data */
           /* and audio data for you.                                       */
           /*****************************************************************/

            MMMultiTrackWrite.pTrackMapList = &TrackMap[0];

           /**********************************************************/
           /* Initialize the TRACKMAP entries for video data         */
           /* before sending the MMMultiTrackWrite message.          */
           /**********************************************************/

            TrackMap[0].ulTrackID = 0;      /* video data                */

            TrackMap[0].ulNumEntries = 1;   /* Number of video entries   */

            TrackMap[0].pRecordTabList = &Vid; /* Point to the RECORDTAB */
                                               /* for video              */

            /*******************************************************/
            /* Initialize the RECORDTAB table entries for video    */
            /* before sending the MMMultiTrackWrite message.       */
            /*******************************************************/

            Vid.pRecord = pRecCB->compimagebuf;

            Vid.ulLength = pRecCB->outbuflen;

            Vid.ulParm1 = 0L;

            Vid.ulParm2 = 0L;

            /***************************************************/
            /* Initialize the TRACKMAP entries for audio data */
            /* before sending the MMMultiTrackWrite message.   */
            /***************************************************/

            TrackMap[1].ulTrackID = 1;      /* audio data                */

            TrackMap[1].ulNumEntries = 1;   /* Number of audio entries   */


            TrackMap[1].pRecordTabList = &Aud; /* Point to the RECORDTAB */
                                               /* for audio              */

            /*******************************************************/
            /* Initialize the RECORDTAB table entries for audio    */
            /* before sending the MMMultiTrackWrite message.       */
            /*******************************************************/

            Aud.pRecord = pAudioBuffer;

            Aud.ulLength = ulAudioBytesPerFrame;

            Aud.ulParm1 = 0L;

            Aud.ulParm2 = 0L;

            /***************************************************************/
            /* Once everything is setup,send the message to interleave and */
            /* compress the video and audio data                           */
            /***************************************************************/

             rcode = mmioSendMessage(pswvrcb->hmmioTemp,
                                     MMIOM_MULTITRACKWRITE,
                                     (LONG)&MMMultiTrackWrite,
                                     sizeof(MMMultiTrackWrite));

        } /* End if - if compressing during record */

         /******************************************************************/
         /* Wait here until the external video device completes the        */
         /* frame step                                                     */
         /******************************************************************/
          DosWaitEventSem ( pRecCB->hevFrameStep, (ULONG)-1 );

     } /* End - if not stopped while paused */

  }   /* End while - frame step recording frame loop */

  free(pAudioBuffer);
 /*****************************************************************/
 /* Close the temp video output file since                        */
 /* recording is finished                                         */
 /*****************************************************************/
  mmioClose( pswvrcb->hmmioTemp, 0);    /* Close the final file   */
  mmioClose( hmmiowav, 0);              /* Close the audio file   */

 /*****************************************************************/
 /* Close event semaphore to signal frame step complete           */
 /*****************************************************************/
  DosCloseEventSem ( pRecCB->hevFrameStep );

 /*****************************************************************/
 /* Close event semaphore to signal recording paused              */
 /*****************************************************************/
  DosCloseEventSem ( pRecCB->hevRecordPaused );

 /*****************************************************************/
 /* Free buffer for capturing the raw image                       */
 /*****************************************************************/
  DosFreeMem ( pRecCB->rawimagebuf );

 /*****************************************************************/
 /* Free buffer for capturing the compressed image                */
 /*****************************************************************/
  DosFreeMem ( pRecCB->compimagebuf );

 /******************************************************************/
 /* Send a message back to our window so it knows recording is done*/
 /******************************************************************/
  WinSendMsg( vhwndPlayerDialog, WM_FINISHEDRECORDING,
              (MPARAM) rcode, (MPARAM) 0);

 /*****************************************************************/
 /* Destroy message queue                                         */
 /*****************************************************************/
  WinDestroyMsgQueue( hmq );

 /*****************************************************************/
 /* Exit thread                                                   */
 /*****************************************************************/
  DosExit( (ULONG) 0, (ULONG) 0);

} /* end: StartVideoThread          */
