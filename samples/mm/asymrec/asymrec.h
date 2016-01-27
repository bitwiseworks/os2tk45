Copyright (C) IBM 1993

typedef struct _RECCB                /* Record control block                */
  {
   HEV       hevRecordPaused; /* Event semaphore handle for paused recording*/
   HEV       hevFrameStep;           /* Frame step                          */

   BYTE      * rawimagebuf;          /* Pointer to raw image buffer         */
   BYTE      * compimagebuf;         /* Pointer to compressed image buffer  */

   ULONG     outbuflen;              /* Number of bytes in output buffer    */

   ULONG     FramesCounted;          /* Number frames counted               */
   BOOL      fStillRunning;          /* Flag                                */

   ULONG     dwSrcCurrentPosition;   /* Position of current source          */
   ULONG     dwMovieRecordFrom;      /* Record from position                */
   ULONG     dwMovieCurrentPosition; /* Current position                    */

   TID       RecordThreadID;   /* Thread ID for asymetric recording thread  */
  } RECCB;
typedef RECCB * PRECCB;


/*************************************/
/*          Global Defines           */
/*************************************/

#define  TEMP_VIDEO_FILENAME    "AVIFILE.AVI"

/***********************/
/* CODEC list          */
/***********************/
typedef struct _CODEC_DATA
{
  CODECINIFILEINFO     cifi;                   /* Codec ini file information */
  struct _CODEC_DATA   *pNext;                 /* Next codec in list         */
} CODEC_DATA, *PCODEC_DATA;

typedef struct _RECOPTS
  {
  USHORT SrcFPS;                            /* Source Frames per second      */
  USHORT MovieFPS;                          /* Rate/Scale = Frames per second*/
  BOOL   fInsertRefFrames;                  /* Reference Frame flag          */
  USHORT usMaxRefreshTime;                  /* Maximum refresh time          */
  BOOL   fCompress;                         /* Compress flag                 */
  USHORT usCapPosX;                         /* Caption position is 0         */
  USHORT usCapPosY;                         /* Caption position is 0         */
  USHORT usCapSizeX;                        /* Capture size from the card 640*/
  USHORT usCapSizeY;                        /* Caption size from the card 480*/
  USHORT usMaxDataRate;                     /* Data constraint               */
  USHORT usQuality;                         /* Video quality of the movie    */

/**************************************************************************/
/* usMovieSizeX = 320, usMovieSizeY = 240                                 */
/* This is half the capture size.  In the recorder application this would */
/* be a 2 to 1 zoom.                                                      */
/**************************************************************************/
  USHORT usMovieSizeX;                      /* Video width in Pels           */
  USHORT usMovieSizeY;                      /* Video height in Pels          */
  } RECOPTS;

/*******************************************************/
/* Control block structure for "global" values.        */
/*******************************************************/

typedef struct _SWVRCB
{
  /*********************************/
  /** data for input and output   **/
  /*********************************/
  MOVIE_MODE_DATA  OutputMovie;           /* Movie data                      */
  MOVIE_MODE_DATA  InputMovie;            /* Source data                     */

  /*************************/
  /** User interface data **/
  /*************************/
  HAB              hab;                   /* anchor block handle             */

  /** Recording Data     **/
  RECOPTS          recopts;               /* Record options structure        */
  PVOID            pRecCB;                /* Pointer to recording CB         */
  ULONG            dwSrcRecordFrom;       /* Media positions from marks ...  */
  ULONG            dwSrcRecordTo;         /* When to stop recording          */
  USHORT             wWaveDID;            /* audio capture device ID         */

  /****************/
  /** Flags      **/
  /****************/
  BOOL             fRecordActive;         /* recording in progress indicator */
  BOOL             fRecordPaused;         /* recording paused flag           */

  /****************/
  /** Strings    **/
  /****************/
  CHAR             szPath[CCHMAXPATH];        /*  default path for files     */
  CHAR             AudioFilename[CCHMAXPATH]; /* file for saving audio       */

  /*********************/
  /*** Miscellaneous ***/
  /*********************/
  HMMIO            hmmioTemp;                    /* Temporary handle         */
  PCODEC_DATA      pCODECData;                   /* Codec chosen             */
  USHORT           usCurrProf;                   /* Current profile          */
} SWVRCB, *PSWVRCB;
