
//   Menu items definitions
//
#define ID_MAINWND      256
#define ID_OPTIONS      257
#define ID_EXIT         258
#define ID_START        259
#define ID_NEWTEXT      260
#define ID_DIRECT       261
#define ID_FULLSCRN     262
#define ID_NORMAL       263
#define ID_COMPILED     264
#define ID_TIMER        1

// Misc defs.
//
#define SAVED_REGS         6

#define  STRING_LENGTH  128
#define  MAX_SOUNDS     4
#define  BUFFER_NUMBER  100
#define  BUFFER_SIZE    512

#define  MONO           1
#define  STEREO         2
#define  LOW_QUALITY    8
#define  HIGH_QUALITY   16
#define  VOICE          11025
#define  MUSIC          22050
#define  HIGH_FI        44100

// Full screen dive definitions
//
#define  WS_DesktopDive    0x00000000L   // Desktop dive window style
#define  WS_MaxDesktopDive 0x00000001L   // Maximized desktop dive window style
#define  WS_FullScreenDive 0x00000002L   // Full-screen 320x200x256 dive style

#define  WM_SetVideoMode            0x04A0
#define  WM_NotifyVideoModeChange   0x04A1
#define  WM_GetVideoModeTable       0x04A2

// For using with DosAllocMem
//
#define MY_ALLOC_FLAGS     PAG_COMMIT | PAG_EXECUTE | PAG_READ | PAG_WRITE


// New data types
//
typedef  PBYTE    *PPBYTE;

//   Bitmap data structure
//
typedef struct _BMPDATA
{
   ULONG    ulImage;                // Dive image buffer number
   PBYTE    pbBuffer;               // Pointer to image buffer
   ULONG    ulWidth;                // Bitmap Width in pels
   ULONG    ulHeight;               // Bitmap Height in pels

   // Each bitmap will have its own custom blitter
   //
   VOID  ( *  _System pfnBlit )( PBYTE pSourceBuffer,
                                 PBYTE pDestinationBuffer,
                                 ULONG ulPositionX,
                                 ULONG ulPositionY );
} BMPDATA, *PBMPDATA;

// This structure is used to build a linked list that tracks the location
// and attitude of each sprite in the scene.
//
typedef struct _SPRITEDATA
{
   PBMPDATA pbmp;                   // Pointer to current image data
   ULONG    ulPositionX;            // Sprite position in scene
   ULONG    ulPositionY;            // Sprite position in scene
   LONG     lDeltaX;                // Horizontal velocity
   LONG     lDeltaY;                // Vertical velocity
   ULONG    ulState;                // Current state of sprite
   struct _SPRITEDATA *pNextSprite; // next sprite in the list
} SPRITEDATA, * PSPRITEDATA;


// Every PCX file has a header that can must be interpreted in order
// to decode the image stored in the file. This structure is designed
// to mirror the header of a typical PCX file. The size of this structure
// can be used with "sizeof" to load the entire header into the structure
// with a single fread.
//
typedef struct _PCX_HEADER
{
   CHAR     chManufacturer;         // Always 10
   CHAR     chVersion;              // 0 - Ver 2.5 Paintbrush
                                    // 2 - Ver 2.8 with palette
                                    // 3 - Ver 2.8 use default palette
                                    // 5 - Ver 3.0 or better of paintbrush
   CHAR     encoding;               // Always 1 meaning RLE encoding
   CHAR     chBitsPerPel;
   SHORT    shX;                    // -
   SHORT    shY;                    // Upper left corner of image
   SHORT    shWidth;
   SHORT    shHeight;
   SHORT    shHorzRez;              // Number of pixels in the x direction
   SHORT    shVertRez;              // Number of pixels in the y direction
   CHAR     chEgaPalette[48];       // Ignore
   CHAR     chReserved;
   CHAR     chNumColorPlanes;
   SHORT    shBytesPerLine;
   SHORT    shPaletteType;
   CHAR     chPadding[58];          // Unused

} PCX_HEADER, * PPCX_HEADER;


// The LoadSound prcedure returns a structure of this type. This structure
// contains all of the audio data for the file as well as a description of
// the type of audio data.
//
typedef struct _SOUND
{
   PBYTE    pData;               // Pointer to audio data
   ULONG    ulDataSize;          // Size of audio data
   ULONG    ulFormatTag;
   ULONG    ulSamplesPerSec;
   ULONG    ulBitsPerSample;
   ULONG    ulChannels;
} SOUND, *PSOUND;



// Procedure prototypes.
//

// This procedure is exported by the BLITGEN module. It compiles a sprite
// and provides a pointer to the new code.
//
ULONG GenBlitSprite( PPVOID ppfnCode,  // Pointer to generated code pointer
                     PBYTE pbSource,   // Pointer to sprite color data
                     ULONG ulSizeX,    // Width of sprite
                     ULONG ulSizeY,    // Height of sprite
                     ULONG ulTargetWidth,   // Width of target video buffer
                     ULONG ulTransparentColor );

// The encoding methode for PCX files is not yet supported by the
// base operating system. This procedure will decode a PCX file and
// fill a BMPDATA structure with the image information.
//
ULONG ReadFile ( PSZ pszFile, PBMPDATA pbmpData );

// Each PCX file has it's own pallete. This procedure will extract the
// palette information from the passed file. The palette is stored in
// an RGB. 4 bytes per color format.
//
ULONG ExtractPalette( PSZ pszFile, PBYTE pbPalette );

// This procedure will take the passed image data and map the colors
// to the system palette.
//
ULONG  ConvertImage( PBMPDATA pbmpData, PBYTE pbPalette );



PSOUND LoadSound( CHAR szFileName[] );

VOID FreeSound( PSOUND pSound );

VOID CloseMixer( VOID );

BOOL SetupMixer( VOID );

VOID QueueSound( PSOUND pSound );

VOID  QueueContinuousSound( PSOUND pSound );

VOID DequeueContinuousSound( VOID );




