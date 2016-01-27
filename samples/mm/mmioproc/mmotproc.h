/********************** START OF SPECIFICATIONS *************************/
/*                                                                      */
/* SOURCE FILE NAME: MMOTPROC.H                                         */
/*                                                                      */
/* DESCRIPTIVE NAME: Multi-Media I/O Procedure for M-Motion Video       */
/*                                                                      */
/* COPYRIGHT:                                                           */
/*              Copyright (c) IBM Corporation  1990, 1993               */
/*                        All Rights Reserved                           */
/*                                                                      */
/* ABSTRACT: This file is the include/header file for use with the      */
/*           M-Motion IOProc (file MMOT.C)                              */
/*                                                                      */
/*********************** END OF SPECIFICATIONS **************************/

#pragma pack(2)

static char     pszMotionExt  [] = "VID";

typedef RGB FAR *PRGB;  

/****************************************
 * Custom M-Motion Still Image File header
 ****************************************/
typedef struct _MMOTIONHEADER
    {
    CHAR    mmID[6];            /* always set to "YUV12C"            */
    CHAR    mmFuture[2];        /* 0, 0 : Reserved for future use    */
    USHORT  mmXorg;             /* X origin of YUV data              */
    USHORT  mmYorg;             /* Y origin of YUV data              */
    USHORT  mmXlen;             /* width of image                    */
    USHORT  mmYlen;             /* height of image                   */
    } MMOTIONHEADER;
typedef MMOTIONHEADER FAR *PMMOTIONHEADER;

/****************************************
 * IOProc information structure, used for every file opened
 *    by this IOProc
 ****************************************/
typedef struct _MMFILESTATUS
    {
    PBYTE   lpRGBBuf;               /* 24-bit RGB Buf for trans data */

    /* Buffer containing pel data from WRITES in bitmap format       */
    PBYTE   lpImgBuf;
    ULONG   ulRGBTotalBytes;        /* Length of 24-bit RGBBuf       */

    /* 4-byte RGB Palette for any trans data                         */
    RGB     rgbPalette[MAX_PALETTE];
    LONG    lImgBytePos;            /* Current pos in RGB buf        */

    /*  Bytes/row, incl overflow - not ULONG pads                    */
    ULONG   ulImgPelBytesPerLine;

    /* Bytes/row, incl overflow & ULONG pads '                       */
    ULONG   ulImgPaddedBytesPerLine;

    /* Length of trans data in 1, 4, 8, or 24 bbp                    */
    ULONG   ulImgTotalBytes;
    ULONG   ulFlags;                /* Misc flags                    */
    BOOL    bSetHeader;             /* TRUE if header set in WRITE mode*/

    /* MMIO handle to Storage System IOProc that provides data.      */
    HMMIO   hmmioSS;
    MMIMAGEHEADER   mmImgHdr;       /* Standard image header         */
    MMOTIONHEADER   mmotHeader;     /* Custom image header           */
    } MMFILESTATUS;
typedef MMFILESTATUS FAR *PMMFILESTATUS;

/************************************************************
 * 4-pel Packed YUV block from/to an M-Motion Still Image file
 ************************************************************/
typedef struct _YUV
    {
    USHORT  YUVWord1;
    USHORT  YUVWord2;
    USHORT  YUVWord3;
    } YUV;
typedef YUV FAR *PYUV;

/***********************************************************
 * A four-pel B, G, R ordered structure used for converting
 *   between RGB and YUV
 ***********************************************************/
typedef struct _RGB4
    {
    BYTE    bB1;  /* Pel 1 */
    BYTE    bG1;
    BYTE    bR1;
    BYTE    bB2;  /* Pel 2 */
    BYTE    bG2;
    BYTE    bR2;
    BYTE    bB3;  /* Pel 3 */
    BYTE    bG3;
    BYTE    bR3;
    BYTE    bB4;  /* Pel 4 */
    BYTE    bG4;
    BYTE    bR4;
    } RGB4;
typedef RGB4 FAR *PRGB4;

#define     MMOTION_HEADER_SIZE         sizeof (MMOTIONHEADER)
#define     ONE_BLOCK                   6L
#define     FOURCC_MMOT                 mmioFOURCC('M', 'M', 'O', 'T')

/* RC file defines */
#define     HEX_FOURCC_MMOT             0x544F4D4DL
#define     MMOTION_IOPROC_NAME_TABLE   7000
#define     MMOTION_NLS_CHARSET_INFO    7500

#ifndef BITT_NONE
#define BITT_NONE 0
#define BI_NONE   0
#endif

/**********************
 * Function Declarations
 **********************/
LONG EXPENTRY IOProc_Entry (PVOID  pmmioStr, USHORT usMsg,
                                   LONG   lParam1, LONG lParam2);

void GetYUV (PYUV pYuv, SHORT Y[], SHORT *U1, SHORT *V1);
void YUVtoRGB (SHORT Y[], SHORT U1, SHORT V1, PBYTE lpYUVtoRGBBuf);
void RGBtoYUV (SHORT YO[], SHORT *U1, SHORT *V1, PBYTE lpRGBBuf);
void PutYUV (PBYTE lpTempBuf, SHORT YO[], SHORT U1, SHORT V1);

void InitFileStruct (PMMFILESTATUS);

ULONG RGBBytesToPels (ULONG, USHORT);
ULONG PelsToRGBBytes (ULONG, USHORT);

void Convert1BitTo24Bit (PBYTE, PRGB, PRGB, ULONG);
void Convert4BitTo24Bit (PBYTE, PRGB, PRGB, ULONG);
void Convert8BitTo24Bit (PBYTE, PRGB, PRGB, ULONG);

void RGB2_To_RGB (PRGB2, PRGB, USHORT);

USHORT NumColors (USHORT);

void ConvertOneLineYUVtoRGB (PBYTE, PBYTE, ULONG);
void ConvertOneLineRGBtoYUV (PBYTE, PBYTE, ULONG);

BOOL ImgBufferFlip (PBYTE, ULONG, ULONG);
LONG ImgBytesToRGBBytes (LONG, USHORT);

LONG GetFormatString (LONG lSearchId,
                      PSZ  pszFormatString,
                      LONG lBytes);

LONG GetFormatStringLength (LONG  lSearchId,
                            PLONG plNameLength);

ULONG APIENTRY GetNLSData (PULONG, PULONG);

#pragma pack()
