/****************************** Module Header ******************************\
*
* Module Name: PMDEV.H
*
* OS/2 Presentation Manager Device Context constants, types and
* function declarations
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The following symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_DEVERRORS          error definitions
*   INCL_DEV                Dev APIs and structures
*
* There is a symbol used in this file called INCL_DDIDEFS. This is used to
* include only the definitions for the DDI. The programmer using the GPI
* can ignore this symbol
*
* There is a symbol used in this file called INCL_SAADEFS. This is used to
* include only the definitions for the SAA. The programmer using the GPI
* can ignore this symbol
*
* ===========================================================================
*
* Comments at the end of each typedef line give the name tags used in
* the assembler include version of this file.
*
* The assembler include version of this file excludes lines between NOINC
* and INC comments.
*
\***************************************************************************/

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( none )
   #endif
   #pragma info( restore )
#endif
#ifdef __cplusplus
      extern "C" {
#endif
/* INC */

#define DEV_INCLUDED


#ifdef INCL_ERRORS /* if errors are required then allow DEV errors */
   #define INCL_DEVERRORS
#endif /* INCL_ERRORS */

#ifdef INCL_DDIDEFS /* if only DDI required then enable all of DEV */
   #define INCL_DEV
#endif /* INCL_DDIDEFS */

#ifdef INCL_SAADEFS /* if only SAA required then enable all of DEV */
   #define INCL_DEV
#endif /* INCL_SAADEFS */

#if (defined(INCL_DEV) || !defined(INCL_NOCOMMON))
   
   /* General DEV return values */
   #define DEV_ERROR                       0L
   #define DEV_OK                          1L
   #define DEV_BAD_PARAMETERS              2L
   #define DEV_WARNING                     3L
   #define DEV_PROP_BUF_TOO_SMALL          4L
   #define DEV_ITEM_BUF_TOO_SMALL          5L
   #define DEV_INV_INP_JOBPROPERTIES       6L
   
   /* DC type for DevOpenDC */
   #define OD_SCREEN                       0L
   #define OD_QUEUED                       2L
   #define OD_DIRECT                       5L
   #define OD_INFO                         6L
   #define OD_METAFILE                     7L
   #define OD_MEMORY                       8L
   #define OD_METAFILE_NOQUERY             9L
   
   /* codes for DevQueryCaps */
   #define CAPS_FAMILY                     0L
   #define CAPS_IO_CAPS                    1L
   #define CAPS_TECHNOLOGY                 2L
   #define CAPS_DRIVER_VERSION             3L
   #define CAPS_WIDTH                      4L      /* pels            */
   #define CAPS_HEIGHT                     5L      /* pels            */
   #define CAPS_WIDTH_IN_CHARS             6L
   #define CAPS_HEIGHT_IN_CHARS            7L
   #define CAPS_HORIZONTAL_RESOLUTION      8L      /* pels per meter  */
   #define CAPS_VERTICAL_RESOLUTION        9L      /* pels per meter  */
   #define CAPS_CHAR_WIDTH                10L      /* pels            */
   #define CAPS_CHAR_HEIGHT               11L      /* pels            */
   #define CAPS_SMALL_CHAR_WIDTH          12L      /* pels            */
   #define CAPS_SMALL_CHAR_HEIGHT         13L      /* pels            */
   #define CAPS_COLORS                    14L
   #define CAPS_COLOR_PLANES              15L
   #define CAPS_COLOR_BITCOUNT            16L
   #define CAPS_COLOR_TABLE_SUPPORT       17L
   #define CAPS_MOUSE_BUTTONS             18L
   #define CAPS_FOREGROUND_MIX_SUPPORT    19L
   #define CAPS_BACKGROUND_MIX_SUPPORT    20L
   #define CAPS_DEVICE_WINDOWING          31L
   #define CAPS_ADDITIONAL_GRAPHICS       32L
   #define CAPS_VIO_LOADABLE_FONTS        21L
   #define CAPS_WINDOW_BYTE_ALIGNMENT     22L
   #define CAPS_BITMAP_FORMATS            23L
   #define CAPS_RASTER_CAPS               24L
   #define CAPS_MARKER_HEIGHT             25L      /* pels            */
   #define CAPS_MARKER_WIDTH              26L      /* pels            */
   #define CAPS_DEVICE_FONTS              27L
   #define CAPS_GRAPHICS_SUBSET           28L
   #define CAPS_GRAPHICS_VERSION          29L
   #define CAPS_GRAPHICS_VECTOR_SUBSET    30L
   #define CAPS_PHYS_COLORS               33L
   #define CAPS_COLOR_INDEX               34L
   #define CAPS_GRAPHICS_CHAR_WIDTH       35L
   #define CAPS_GRAPHICS_CHAR_HEIGHT      36L
   #define CAPS_HORIZONTAL_FONT_RES       37L
   #define CAPS_VERTICAL_FONT_RES         38L
   #define CAPS_DEVICE_FONT_SIM           39L
   #define CAPS_LINEWIDTH_THICK           40L
   #define CAPS_DEVICE_POLYSET_POINTS     41L
   
   /* Constants for CAPS_IO_CAPS */
   #define CAPS_IO_DUMMY                   1L
   #define CAPS_IO_SUPPORTS_OP             2L
   #define CAPS_IO_SUPPORTS_IP             3L
   #define CAPS_IO_SUPPORTS_IO             4L
   
   /* Constants for CAPS_TECHNOLOGY */
   #define CAPS_TECH_UNKNOWN               0L
   #define CAPS_TECH_VECTOR_PLOTTER        1L
   #define CAPS_TECH_RASTER_DISPLAY        2L
   #define CAPS_TECH_RASTER_PRINTER        3L
   #define CAPS_TECH_RASTER_CAMERA         4L
   #define CAPS_TECH_POSTSCRIPT            5L
   
   /* Constants for CAPS_COLOR_TABLE_SUPPORT */
   #define CAPS_COLTABL_RGB_8              1L
   #define CAPS_COLTABL_RGB_8_PLUS         2L
   #define CAPS_COLTABL_TRUE_MIX           4L
   #define CAPS_COLTABL_REALIZE            8L
   
   /* Constants for CAPS_FOREGROUND_MIX_SUPPORT */
   #define CAPS_FM_OR                      1L
   #define CAPS_FM_OVERPAINT               2L
   #define CAPS_FM_XOR                     8L
   #define CAPS_FM_LEAVEALONE             16L
   #define CAPS_FM_AND                    32L
   #define CAPS_FM_GENERAL_BOOLEAN        64L
   
   /* Constants for CAPS_BACKGROUND_MIX_SUPPORT */
   #define CAPS_BM_OR                      1L
   #define CAPS_BM_OVERPAINT               2L
   #define CAPS_BM_XOR                     8L
   #define CAPS_BM_LEAVEALONE             16L
   #define CAPS_BM_AND                    32L
   #define CAPS_BM_GENERAL_BOOLEAN        64L
   #define CAPS_BM_SRCTRANSPARENT        128L
   #define CAPS_BM_DESTTRANSPARENT       256L
   
   /* Constants for CAPS_DEVICE_WINDOWING */
   #define CAPS_DEV_WINDOWING_SUPPORT      1L
   
   /* Constants for CAPS_DEV_FONT_SIM     */
   #define CAPS_DEV_FONT_SIM_BOLD          1L
   #define CAPS_DEV_FONT_SIM_ITALIC        2L
   #define CAPS_DEV_FONT_SIM_UNDERSCORE    4L
   #define CAPS_DEV_FONT_SIM_STRIKEOUT     8L
   
   /* Constants for CAPS_ADDITIONAL_GRAPHICS */
   #define CAPS_VDD_DDB_TRANSFER            1L
   #define CAPS_GRAPHICS_KERNING_SUPPORT    2L
   #define CAPS_FONT_OUTLINE_DEFAULT        4L
   #define CAPS_FONT_IMAGE_DEFAULT          8L
   /* bits represented by values 16L and 32L are reserved */
   #define CAPS_SCALED_DEFAULT_MARKERS     64L
   #define CAPS_COLOR_CURSOR_SUPPORT      128L
   #define CAPS_PALETTE_MANAGER           256L
   #define CAPS_COSMETIC_WIDELINE_SUPPORT 512L
   #define CAPS_DIRECT_FILL              1024L
   #define CAPS_REBUILD_FILLS            2048L
   #define CAPS_CLIP_FILLS               0x00001000 /* 4096L  */
   #define CAPS_ENHANCED_FONTMETRICS     0x00002000 /* 8192L  */
   #define CAPS_TRANSFORM_SUPPORT        0x00004000 /* 16384L */
   #define CAPS_EXTERNAL_16_BITCOUNT     0x00008000 /* 32768L */
   #define CAPS_EXTERNAL_32_BITCOUNT     0x00010000 /* 65536L */
   
   #ifndef INCL_SAADEFS
      
      /* Constants for CAPS_WINDOW_BYTE_ALIGNMENT */
      #define CAPS_BYTE_ALIGN_REQUIRED        0L
      #define CAPS_BYTE_ALIGN_RECOMMENDED     1L
      #define CAPS_BYTE_ALIGN_NOT_REQUIRED    2L
   #endif /* no INCL_SAADEFS */
   
   /* Constants for CAPS_RASTER_CAPS */
   #define CAPS_RASTER_BITBLT              1L
   #define CAPS_RASTER_BANDING             2L
   #define CAPS_RASTER_BITBLT_SCALING      4L
   #define CAPS_RASTER_SET_PEL            16L
   #define CAPS_RASTER_FONTS              32L
   #define CAPS_RASTER_FLOOD_FILL         64L
   
   #ifndef INCL_DDIDEFS
      
#ifdef __cplusplus
      HDC  APIENTRY DevOpenDC(HAB hab,
                              LONG lType,
                              PCSZ  pszToken,
                              LONG lCount,
                              PDEVOPENDATA pdopData,
                              HDC hdcComp);
#else
      HDC  APIENTRY DevOpenDC(HAB hab,
                              LONG lType,
                              PSZ pszToken,
                              LONG lCount,
                              PDEVOPENDATA pdopData,
                              HDC hdcComp);
#endif
      
      HMF  APIENTRY DevCloseDC(HDC hdc);
      
      BOOL  APIENTRY DevQueryCaps(HDC hdc,
                                  LONG lStart,
                                  LONG lCount,
                                  PLONG alArray);
   #endif /* no INCL_DDIDEFS */
   
#endif /* common DEV */

#ifdef INCL_DEV
   
   #ifndef INCL_SAADEFS
      
      /* structures for DEVESC_QUERYVIOCELLSIZES */
      typedef struct _VIOSIZECOUNT    /* vios */
      {
         LONG maxcount;
         LONG count;
      } VIOSIZECOUNT;
      typedef VIOSIZECOUNT *PVIOSIZECOUNT;
      
      typedef struct _VIOFONTCELLSIZE    /* viof */
      {
         LONG cx;
         LONG cy;
      } VIOFONTCELLSIZE;
      typedef VIOFONTCELLSIZE *PVIOFONTCELLSIZE;
      
      /* structure for DEVESC_GETSCALINGFACTOR */
      typedef struct _SFACTORS    /* sfactors */
      {
         LONG x;
         LONG y;
      } SFACTORS;
      typedef SFACTORS *PSFACTORS;
      
      /* structure for DEVESC_NEXTBAND */
      typedef struct _BANDRECT    /* bandrect */
      {
         LONG  xleft;
         LONG  ybottom;
         LONG  xright;
         LONG  ytop;
      } BANDRECT;
      typedef BANDRECT *PBANDRECT;
      
      /* return codes for DevEscape */
      #define DEVESC_ERROR                  (-1L)
      #define DEVESC_NOTIMPLEMENTED           0L
      
      /* codes for DevEscape */
      #define DEVESC_QUERYESCSUPPORT          0L
      #define DEVESC_GETSCALINGFACTOR         1L
      #define DEVESC_QUERYVIOCELLSIZES        2L
      #define DEVESC_GETCP                 8000L
      /* New codes for SetAbortProc and QueryPDL */
      /* Not metafiled but passed to engine      */
      #define DEVESC_SETABORTPROC          8001L
      #define DEVESC_QUERYPDL              8002L
      
      #define DEVESC_STARTDOC              8150L
      #define DEVESC_ENDDOC                8151L
      #define DEVESC_NEXTBAND              8152L
      #define DEVESC_ABORTDOC              8153L
      #define DEVESC_GETJOBID              8160L
      #define DEVESC_QUERY_RASTER          8161L
      /* New codes for Dynamic Job Properties */
      #define DEVESC_QUERYSIZE             8162L
      #define DEVESC_QUERYJOBPROPERTIES    8163L
      #define DEVESC_SETJOBPROPERTIES      8164L
      #define DEVESC_DEFAULTJOBPROPERTIES  8165L
      #define DEVESC_CHANGEOUTPUTPORT      8166L
      
      #define DEVESC_NEWFRAME             16300L
      #define DEVESC_DRAFTMODE            16301L
      #define DEVESC_FLUSHOUTPUT          16302L
      #define DEVESC_RAWDATA              16303L
      #define DEVESC_SETMODE              16304L
      #define DEVESC_SEP                  16305L
      #define DEVESC_MACRO                16307L
      #define DEVESC_BEGIN_BITBLT         16309L
      #define DEVESC_END_BITBLT           16310L
      #define DEVESC_SEND_COMPDATA        16311L
      
      #define DEVESC_DBE_FIRST            24450L
      #define DEVESC_DBE_LAST             24455L
      
      /* DevEscape codes for adding extra space to character strings */
      #define DEVESC_CHAR_EXTRA           16998L
      #define DEVESC_BREAK_EXTRA          16999L
      
      /* codes for DevEscape PM_Q_ESC spool files */
      #define DEVESC_STD_JOURNAL          32600L
      
      /* New code for SetPS                   */
      /* This does not go directly to drivers, but is handled in GPI/GRE special code */
      #define DEVESC_SETPS                49149L
      
      /* New codes for Dynamic Job Properties */
      #define DEVESC_STARTDOC_WPROP       49150L
      #define DEVESC_NEWFRAME_WPROP       49151L
      
      /* structure for DEVESC_SETMODE */
      typedef struct _ESCMODE     /* em */
      {
         ULONG mode;
         BYTE  modedata[1];
      } ESCMODE;
      typedef ESCMODE *PESCMODE;
      
      /* NOINC */
      /* structure for DEVESC_BEGIN_BITBLT */
      typedef  struct _COMPBITBLT   /* cbb */
      {
         ULONG ultype;
         RECTL rclDst;            /* Destination rectangle */
         ULONG ulColumns;
         ULONG ulRows;
         union
         {
            struct _ccitt {      /* used by COMP_G4_MMR, COMP_G3_MH, */
               /* COMP_G3_MMR                      */
               BOOL  bEndOfLine;
               BOOL  bEncodedByteAlign;
               BOOL  bEndOfBlock;
               BOOL  bBlackIs1;
               ULONG K;           /* Only used by COMP_G3_MMR */
            } ccitt;
            struct _dct          /* used by COMP_JPEG */
            {
               ULONG ulColors;
               PVOID pQuantTables;
               ULONG ulQFactor;
               PVOID pHuffTables;
               ULONG ColorTransform;
            } dct;
         } bitblt;
      }  COMPBITBLT;
      typedef COMPBITBLT *PCOMPBITBLT;
      /* INC */
      
      /* structures for DEVESC_QUERY_RASTER */
      typedef  struct _COMP_BITMAPTYPES   /* cbmt */
      {
         USHORT cPlanes;              /* # of bit planes                   */
         USHORT cBitCount;            /* # of bits per pel within a plane  */
         ULONG  ulCompressionType;    /* Compresstype                      */
         ULONG  ulCaps;               /* Bitmap capabilites bitstring      */
      }  COMP_BITMAPTYPES;
      
      typedef COMP_BITMAPTYPES *PCOMP_BITMAPTYPES;
      
      typedef  struct _QUERY_RASTER   /* qr */
      {
         ULONG  cCount;               /* Count of struct above that a device */
         /* supports                            */
         ULONG  ulCaps;               /* Bitmaps capabilities bitstring      */
      }  QUERY_RASTER;
      typedef QUERY_RASTER *PQUERY_RASTER;
      
      
      typedef  struct _QUERY_ONE_RASTER   /* qor */
      {
         BOOL    bSupported;          /* Is mode supported with              */
         /*    DEVESC_BEGIN_BITBLT?             */
         BOOL    ulCaps;              /* Bitmaps capabitities bitstring      */
         /* with gpi calls on a page.           */
      }  QUERY_ONE_RASTER;
      typedef QUERY_ONE_RASTER *PQUERY_ONE_RASTER;
      
      /* NOINC */
      typedef BOOL   (* APIENTRY PABORTPROC)( HDC,
                      INT );
      
      typedef struct _SETABORTPROC    /* sap */
      {
         HDC    hdc;                   /* HDC of the windows app */
         PABORTPROC   pAbortProc;      /* Function to call       */
      } SETABORTPROC;
      typedef SETABORTPROC *PSETABORTPROC;
      /* INC */
      
      /* structure for Info on each printer description language (PDL) */
      typedef struct _PDL               /* pdl */
      {
         ULONG   ulPDL;                  /* PDL ID (Like PCL or Postscript   */
         ULONG   ulPDLLevel;             /* PDL level (like 6 in PCL 6)      */
         ULONG   ulMajorPDLVersion;      /* Major PDL Version (like 2 in 2.0)*/
         ULONG   ulMinorPDLVersion;      /* Minor PDL Version (like 0 in 2.0)*/
         CHAR    szMinorCharID[32];      /* We need an additional PDL identifier for (PCL 5.2 C) or future Character IDs*/
         ULONG   ulReserved;             /* Reserved should be set to zero */
         ULONG   ulFlags;                /* Flags */
         CHAR    szPDLDescription[128]; /* PDL description string */
         CHAR    szTranslatedPDLDescription[256]; /* Translated PDL description string */
      } PDL;
      typedef PDL *PPDL;
      
      /* ulFlags values for PDL structure */
      
      #define PDLCAPS_CURRENT        0x0001  /* Current PDL selected  on the device */
      #define PDLCAPS_SOFTWARE_EMUL  0x0002  /* PDL is emulated through software */
      #define PDLCAPS_SELECTABLE     0x0004  /* PDL is selectable       */
      #define PDLCAPS_MACROSUPPORT   0x0008  /* Macros are supported  */
      
      /* input structure for DEVESC_QUERYPDL */
      typedef struct _IQUERYPDL /* iqpdl */
      {
         LONG lBufSize;  /* Total size of input buffer in bytes */
         LONG lCmd;      /* Type of query                       */
         LONG lStart;    /* Index of PDL to start with (0 based)*/
         LONG lCount;    /* Count of PDLs wanted                */
      } IQUERYPDL;
      typedef IQUERYPDL *PIQUERYPDL;
      
      /* Type of query                 */
      #define QUERYPDL_CMD_QUERY_SIZE  1    /* NOTE: uses OQUERYPDL struct */
      #define QUERYPDL_CMD_QUERY_PDL   2    /* NOTE: uses OQUERYPDL struct */
      
      /* output structure for DEVESC_QUERYPDL */
      typedef struct _OQUERYPDL /* oqpdl */
      {
         LONG lBufSize;  /* Total size of output buffer in bytes  */
         LONG lMaxCount; /* Max number of PDLs/versions supported */
         LONG lCount;    /* Count of PDLs returned                */
         PDL  aPDL[1];
      } OQUERYPDL;
      typedef OQUERYPDL *POQUERYPDL;
      
      /* structure for DEVESC_MACRO */
      typedef struct _ESCMACRO          /* devmac */
      {
         PDL     sPDL;           /* PDL structure as returned from DEVESC_QUERYPDL */
         ULONG   ulMacroLength;  /* Length of macro data */
         ULONG   ulFlags;        /* Various flags */
         BYTE    abMacro[1];     /* Array of bytes containing the macro data */
      } ESCMACRO;
      typedef ESCMACRO *PESCMACRO;
      
      /****************************** PDL types ******************************/
      #define PDL_DONTCARE    (ULONG)-1 /* Use this for fields to ignore */
      
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>> IPP PDL types <<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
      /* 1-53 are from IPP spec */
      #define PDL_other        1  /* Not on this list                                           */
      #define PDL_PCL          3  /* PCL.  Starting with PCL version 5, HP-GL/2 is included as  */
      #define PDL_HPGL         4  /* Hewlett-Packard Graphics Language.  HP-GL is a registered  */
      #define PDL_PJL          5  /* Peripheral Job Language.  Appears in the data stream       */
      #define PDL_PS           6  /* PostScript Language (tm) Postscript - a trademark of Adobe */
      #define PDL_IPDS         7  /* Intelligent Printer Data Stream Bi-directional print data  */
      #define PDL_PPDS         8  /* IBM Personal Printer Data Stream.  Originally called IBM   */
      #define PDL_EscapeP      9  /* Epson Corp.                                                */
      #define PDL_Epson        10 /* Epson Corp.                                                */
      #define PDL_DDIF         11 /* Digital Document Interchange Format Digital Equipment      */
      #define PDL_Interpress   12 /* Xerox Corp.                                                */
      #define PDL_ISO6429      13 /* ISO 6429.  Control functions for Coded Character Sets      */
      #define PDL_LineData     14 /* line-data : Lines of data as separate ASCII or EBCDIC      */
      #define PDL_MODCA        15 /* Mixed Object Document Content Architecture Definitions     */
      #define PDL_REGIS        16 /* Remote Graphics Instruction Set, Digital Equipment         */
      #define PDL_SCS          17 /* SNA Character String Bi-directional print data stream for  */
      #define PDL_SPDL         18 /* ISO 10180 Standard Page Description Language ISO           */
      #define PDL_TEK4014      19 /* Tektronix Corp.                                            */
      #define PDL_PDS          20
      #define PDL_IGP          21 /* Printronix Corp.                                           */
      #define PDL_CodeV        22 /* Magnum Code-V, Image and printer control language used     */
      #define PDL_DSCDSE       23 /* DSC-DSE : Data Stream Compatible and Emulation Bi-         */
      #define PDL_WPS          24 /* Windows Printing System, Resource based command/data       */
      #define PDL_LN03         25 /* Early DEC-PPL3, Digital Equipment Corp.                    */
      #define PDL_CCITT        26
      #define PDL_QUIC         27 /* QUIC (Quality Information Code), Page Description          */
      #define PDL_CPAP         28 /* Common Printer Access Protocol Digital Equipment Corp      */
      #define PDL_DecPPL       29 /* Digital ANSI-Compliant Printing Protocol (DEC-PPL)         */
      #define PDL_SimpleText   30 /* simple-text : character coded data, including NUL,         */
      #define PDL_NPAP         31 /* Network Printer Alliance Protocol (NPAP).  This protocol   */
      #define PDL_DOC          32 /* Document Option Commands, Appears in the data stream       */
      #define PDL_imPress      33 /* imPRESS, Page description language originally              */
      #define PDL_Pinwriter    34 /* 24 wire dot matrix printer for USA, Europe, and            */
      #define PDL_NPDL         35 /* Page printer for Japanese market.  NEC                     */
      #define PDL_NEC201PL     36 /* Serial printer language used in the Japanese market.       */
      #define PDL_Automatic    37 /* Automatic PDL sensing.  Automatic sensing of the           */
      #define PDL_Pages        38 /* Page printer Advanced Graphic Escape Set IBM Japan         */
      #define PDL_LIPS         39 /* LBP Image Processing System                                */
      #define PDL_TIFF         40 /* Tagged Image File Format (Aldus)                           */
      #define PDL_Diagnostic   41 /* A hex dump of the input to the interpreter                 */
      #define PDL_PSPrinter    42 /* The PostScript Language used for control (with any         */
      #define PDL_CaPSL        43 /* Canon Print Systems Language                               */
      #define PDL_EXCL         44 /* Extended Command Language Talaris Systems Inc              */
      #define PDL_LCDS         45 /* Line Conditioned Data Stream Xerox Corporatio              */
      #define PDL_XES          46 /* Xerox Escape Sequences Xerox Corporation                   */
      #define PDL_PCLXL        47 /* Printer Control Language.  Extended language features      */
      #define PDL_ART          48 /* Advanced Rendering Tools (ART).  Page Description          */
      #define PDL_TIPSI        49 /* Transport Independent Printer System Interface (ref.       */
      #define PDL_Prescribe    50 /* Page description and printer control language.  It         */
      #define PDL_LinePrinter  51 /* A simple-text character stream which supports the          */
      #define PDL_IDP          52 /* Imaging Device Protocol Apple Computer.                    */
      #define PDL_XJCL         53 /* Xerox Corp.                                                */
      #define PDL_ALPS         54 /* ALPS Corp.                                                 */
      #define PDL_Olivetti     55 /* Olivetti Corp.                                             */
      #define PDL_Deskjet      56 /* Hewlett-Packard Deskjet subset (IPCL)                      */
      #define PDL_Paintjet     57 /* Hewlett-Packard Paintjet subset                            */
      #define PDL_Seiko        58 /* Seiko Corp.                                                */
      #define PDL_PassThru     59 /* Does not alter data stream                                 */
      
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>> PDL Levels <<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
      
      /************************** LEVELS FOR PDL_PS **************************/
      #define LEVEL_PS_LEVEL1         1     /* Postscript Level  1*/
      #define LEVEL_PS_LEVEL2         2     /* Postscript Level  2*/
      
      /************************* LEVELS for PDL_PCL **************************/
      #define LEVEL_PCL2              1
      #define LEVEL_PCL3              2
      #define LEVEL_PCL3C             3
      #define LEVEL_PCL4              4
      #define LEVEL_PCL5              5
      #define LEVEL_PCL5C             6
      #define LEVEL_PCL6              7
      #define LEVEL_PCL5E             8
      
      /************************* LEVELS FOR PDL_HPGL *************************/
      #define LEVEL_HPGL1             1
      #define LEVEL_HPGL2             2
      #define LEVEL_HPGL2_RTL         3     /* HPGL2 with RTL language used for raster transfer */
      #define LEVEL_HPGL2_PCLRTL      4     /* HPGL2 with PCL used for raster transfer */
      #define LEVEL_HPGL2_MC          5     /* HPGL2 with MC command support */
      
      /************************ LEVELS FOR PDL_EPSON *************************/
      #define LEVEL_ESC               1
      #define LEVEL_ESCP              2
      #define LEVEL_ESCP_2            3
      #define LEVEL_ESCP_2J           4     /* Japan version */
      
      /********************** LEVELS FOR PDL_SimpleText **********************/
      #define LEVEL_ASCII_TEXT        1
      #define LEVEL_ASCII_PROPRINTER  2
      #define LEVEL_ASCII_QUITWRITER  3
      #define LEVEL_ASCII_JISASCII    4
      
      /*********************** LEVELS FOR PDL_Deskjet ************************/
      #define LEVEL_DESKJET           1
      #define LEVEL_DESKJETJ          2     /* Japan version */
      
      /* END LEVELS */
      
      /******************************************************************/
      /*           Bitmap capabitities bitstring defines                */
      /******************************************************************/
      #define BITMAP_WITH_GPI                0x00000001
      #define BITMAP_SMOOTHING               0x00000002
      #define BITMAP_STRETCHING              0x00000004
      #define BITMAP_ROTATION                0x00000008
      #define BITMAP_COLOR                   0x00000010
      /******************************************************************/
      /*                   Compression types defines                    */
      /******************************************************************/
      /* Group 4 fax compression */
      #define COMP_IBM_MMR                   0x00000001
      #define COMP_PACKBITS                  0x00000002
      #define COMP_ABIC                      0x00000008
      #define COMP_TIFF_2                    0x00000009
      #define COMP_ABIC_CONCAT               0x0000000A
      #define COMP_G3_MH                     0x00000080
      #define COMP_G3_MR                     0x00000081
      #define COMP_G4_MMR                    0x00000082
      #define COMP_JPEG                      0x0000CA83
      #define COMP_UNCOMPRESS                0x00001000
      
      /* return codes for DevPostDeviceModes */
      #define DPDM_ERROR                    (-1L)
      #define DPDM_NONE                       0L
      
      /* codes for DevPostDeviceModes */
      #define DPDM_POSTJOBPROP                0L
      #define DPDM_CHANGEPROP                 1L
      #define DPDM_QUERYJOBPROP               2L
      
      /* string types for DevQueryDeviceNames */
      typedef CHAR STR16[16];     /* str16 */
      typedef STR16 *PSTR16;
      typedef CHAR STR32[32];     /* str32 */
      typedef STR32 *PSTR32;
      typedef CHAR STR64[64];     /* str64 */
      typedef STR64 *PSTR64;
      
      /* return code for DevQueryHardcopyCaps */
      #define DQHC_ERROR                    (-1L)
      
      /* codes for DevQueryHardcopyCaps */
      #define HCAPS_CURRENT                   1L
      #define HCAPS_SELECTABLE                2L
      
      /* structure for DevQueryHardcopyCaps */
      typedef struct _HCINFO           /* hci */
      {
         CHAR   szFormname[32];
         LONG   cx;
         LONG   cy;
         LONG   xLeftClip;
         LONG   yBottomClip;
         LONG   xRightClip;
         LONG   yTopClip;
         LONG   xPels;
         LONG   yPels;
         LONG   flAttributes;
      } HCINFO;
      typedef HCINFO *PHCINFO;
      
      /* return codes for DevPostEscape */
      #define DEVPE_ERROR                     -1L
      #define DEVPE_NOTIMPLEMENTED            -2L
      #define DEVPE_NOTSUPPORTED              -3L
      
      /* codes for DevPostEscape */
      #define DEVPE_QUERYSUPPORT               1L
      #define DEVPE_QUERYSIZE                  2L
      #define DEVPE_QUERYJOBPROPERTIES         3L
      #define DEVPE_SETJOBPROPERTIES           4L
      #define DEVPE_DEFAULTJOBPROPERTIES       5L
      
      #ifdef INCL_DEVDJP
         #include <pmdjp.h>
      #endif /* INCL_DEVDJP */
      
      /* Device Context Functions */
      #ifndef INCL_DDIDEFS
         
         LONG  APIENTRY DevEscape(HDC hdc,
                                  LONG lCode,
                                  LONG lInCount,
                                  PBYTE pbInData,
                                  PLONG plOutCount,
                                  PBYTE pbOutData);
         
#ifdef __cplusplus
         BOOL  APIENTRY DevQueryDeviceNames(HAB hab,
                                            PCSZ  pszDriverName,
                                            PLONG pldn,
                                            PSTR32 aDeviceName,
                                            PSTR64 aDeviceDesc,
                                            PLONG pldt,
                                            PSTR16 aDataType);
#else
         BOOL  APIENTRY DevQueryDeviceNames(HAB hab,
                                            PSZ pszDriverName,
                                            PLONG pldn,
                                            PSTR32 aDeviceName,
                                            PSTR64 aDeviceDesc,
                                            PLONG pldt,
                                            PSTR16 aDataType);
#endif
         
         LONG  APIENTRY DevQueryHardcopyCaps(HDC hdc,
                                             LONG lStartForm,
                                             LONG lForms,
                                             PHCINFO phciHcInfo);
         
#ifdef __cplusplus
         LONG  APIENTRY DevPostDeviceModes(HAB hab,
                                           PDRIVDATA pdrivDriverData ,
                                           PCSZ  pszDriverName,
                                           PCSZ  pszDeviceName,
                                           PCSZ  pszName,
                                           ULONG flOptions);
#else
         LONG  APIENTRY DevPostDeviceModes(HAB hab,
                                           PDRIVDATA pdrivDriverData ,
                                           PSZ pszDriverName,
                                           PSZ pszDeviceName,
                                           PSZ pszName,
                                           ULONG flOptions);
#endif
         
         /* This is a pointer to the DevPostEscape API.  It is provided
         ** as a typedef so that you can DosQueryProcAddr on PMGPI.DLL
         ** with the ordinal ORD_DEV32POSTESCAPE
         */
         /* NOINC */
         typedef LONG (* APIENTRY PDEVPOSTESCAPE_FUNC)(PSZ pszDriverName,
                       PSZ pszDeviceName,
                       PSZ pszQueueName,
                       PSZ pszSplPrinterName,
                       ULONG ulFuncNum,
                       ULONG cbParm1,
                       PBYTE pbParm1,
                       ULONG cbParm2,
                       PBYTE pbParm2);
         /* INC */
         
#ifdef __cplusplus
         LONG  APIENTRY DevPostEscape(PCSZ  pszDriverName,
                                      PCSZ  pszDeviceName,
                                      PCSZ  pszQueueName,
                                      PCSZ  pszSplPrinterName,
                                      ULONG ulFuncNum,
                                      ULONG cbParm1,
                                      PBYTE pbParm1,
                                      ULONG cbParm2,
                                      PBYTE pbParm2);
#else
         LONG  APIENTRY DevPostEscape(PSZ pszDriverName,
                                      PSZ pszDeviceName,
                                      PSZ pszQueueName,
                                      PSZ pszSplPrinterName,
                                      ULONG ulFuncNum,
                                      ULONG cbParm1,
                                      PBYTE pbParm1,
                                      ULONG cbParm2,
                                      PBYTE pbParm2);
#endif
         
      #endif /* no INCL_DDIDEFS */
   #endif /* no INCL_SAADEFS */
#endif /* non-common DEV */

#ifdef INCL_DEVERRORS
   /* AAB error codes for the DEV - same as GPI errors at present */
#endif /* INCL_DEVERRORS */


/* NOINC */
#ifdef __cplusplus
        }
#endif
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( restore )
   #endif
   #pragma info( restore )
#endif
/* INC */

/**************************** end of file **********************************/
