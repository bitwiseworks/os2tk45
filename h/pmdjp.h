/****************************** Module Header ******************************\
*
* Module Name: PMDJP.H
*
* OS/2 Presentation Manager Dynamic Job Properties
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The following symbols are used in this file for conditional sections.
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

#ifndef DJP_INCLUDED
   
   /* NOINC */
   #define DJP_INCLUDED
   /* INC */
   
   /* -----------------------------------------------------------------
   ** Tuple Item used for QUERYSIZE
   */
   typedef struct _djpQueryTuple   /* djpQRT */
   {
      ULONG       ulProperty;     /* I - Property                      */
      LONG        lType;          /* I - type (DJP_ALL or DJP_CURRENT) */
      /*                                                               */
   } QUERYTUPLE, *PQUERYTUPLE;
   
   /* -----------------------------------------------------------------
   ** Query Size Structure for DEVESC_QUERYSIZE
   */
   typedef struct _djpQuerySize   /* djpQRS */
   {
      ULONG       cb;             /* I - Size of entire structure      */
      ULONG       ulSizeNeeded;   /* O - Size returned;                */
      QUERYTUPLE  aTuples[1];     /* I - Start of tuple list           */
      /*                                 use DJP_NONE for end of list  */
   } QUERYSIZE, *PQUERYSIZE;
   
   #define QUERYSIZE_HEADER_SIZE  (sizeof (QUERYSIZE) - sizeof (((PQUERYSIZE)NULL)->aTuples))
   
   /* -----------------------------------------------------------------
   ** Dynamic Job Property Item
   */
   typedef struct _djpItem   /* djpITM */
   {
      ULONG       cb;             /* I/O - sizeof DJP_ITEM structure   */
      ULONG       ulProperty;     /* I   - Which property              */
      LONG        lType;          /* I/O - DJP_ALL or DJP_CURRENT.     */
      /*                                   DJP_ERROR_XXX if error.     */
      ULONG       ulNumReturned;  /* O   - How many elements have been */
      /*                                   returned                    */
      ULONG       ulValue;        /* O   - Variably sized based on     */
      /*                                   ulProperty.  The smallest   */
      /*                                   is a ULONG in size          */
   } DJP_ITEM, *PDJP_ITEM;
   
   #define DJP_HEADER_SIZE        (sizeof (DJP_ITEM) - sizeof (((PDJP_ITEM)NULL)->ulValue))
   #define DJP_NEXT_STRUCTP(p)    ((PDJP_ITEM)((PBYTE)(p) + (p)->cb))
   #define DJP_ELEMENTP(s,t)      ((t*)&((s).ulValue))
   #define DJP_SET_ELEMENT(s,t,e) (*DJP_ELEMENTP (s,t) = (e))
   
   /* -----------------------------------------------------------------
   ** Types of Dynamic Job Properties
   **
   ** To see if the driver is enabled use,
   **     DevEscape      (DEVESC_QUERYESCSUPPORT,
   **                     DEVESC_STARTDOC_WPROP, ...)
   ** and DevQueryDevice (DEVQRY_QUERYSUPPORT,
   **                     DEVESC_QUERYJOBPROPERTIES, ...)
   **
   ** NOTE: The C/S in the defines indicate the complexity or size of
   **       the information.  If it is s, then the size is ULONG sized
   **       and no special processing for the next element needs to be
   **       done.
   **       The J/P in the defines indicate the class.  J stands for job
   **       properties and P stands for printer properties.
   */
   #define DJP_NONE                          0L     /* also End Of List marker */
   #define DJP_SJ_ORIENTATION                1L
   #define DJP_CJ_RESOLUTION                 2L
   #define DJP_SJ_BITSPERPEL                 3L
   #define DJP_SJ_COLOR                      4L
   #define DJP_SJ_PRINTQUALITY               5L
   #define DJP_SJ_PAPERSIZE                  6L
   #define DJP_SJ_TRAYTYPE                   7L
   #define DJP_SJ_MEDIA                      8L
   #define DJP_SJ_MEDIA_COLOR                9L
   #define DJP_CJ_FORM                      10L
   #define DJP_CJ_MIXEDFORMS                11L
   #define DJP_SJ_FONTDOWNLOADING           12L
   #define DJP_SJ_DUPLEX                    13L
   #define DJP_SJ_COLLATE                   14L
   #define DJP_SJ_FEED                      15L
   #define DJP_SJ_COPIES                    16L
   #define DJP_SJ_SCALING                   17L
   #define DJP_SJ_FORMFEEDCONTROL           18L
   #define DJP_SJ_N_UP                      19L
   #define DJP_CJ_OUTPUTBIN                 20L
   #define DJP_CJ_TRAYNAME                  21L
   
   /* Types for DEVESC_QUERYJOBPROPERTIES / DEVESC_SETJOBPROPERTIES
   */
   #define DJP_ALL                           1L  /* enumerate the property */
   #define DJP_CURRENT                       2L  /* from job properties    */
   
   /* Errors for DEVESC_QUERYJOBPROPERTIES / DEVESC_SETJOBPROPERTIES
   */
   #define DJP_ERROR_NOT_SUPPORTED          -1L  /* driver doesnt support that property */
   #define DJP_ERROR_OUT_OF_RANGE           -2L  /* not in the valid range              */
   #define DJP_ERROR_NOT_ENUM               -3L  /* not enumerateable                   */
   #define DJP_ERROR_INV_PARMS              -4L  /* field not proper value              */
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_ORIENTATION
   */
   #define DJP_ORI_PORTRAIT                  1L
   #define DJP_ORI_LANDSCAPE                 2L
   #define DJP_ORI_REV_PORTRAIT              3L
   #define DJP_ORI_REV_LANDSCAPE             4L
   
   typedef ULONG DJPT_ORIENTATION;
   typedef ULONG *PDJPT_ORIENTATION;
   
   /* -----------------------------------------------------------------
   ** DJP_CJ_RESOLUTION
   */
   typedef struct _djpResolution   /* djpRES */
   {
      USHORT     usXResolution;    /* X resolution (in dots per inch) */
      USHORT     usYResolution;    /* Y resolution (in dots per inch) */
   } DJPT_RESOLUTION, *PDJPT_RESOLUTION;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_BITSPERPEL
   */
   typedef ULONG DJPT_BITSPERPEL;
   typedef ULONG *PDJPT_BITSPERPEL;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_COLOR
   */
   #define DJP_CLR_MONOCHROME                1L
   #define DJP_CLR_COLOR                     2L
   
   typedef ULONG DJPT_COLOR;
   typedef ULONG *PDJPT_COLOR;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_PRINTQUALITY
   **
   ** Note: DJP_PQL_DRAFT is the worst quality.  In the future, there
   **       may be better qualities (such as DJP_PQL_ULTRA_HIGH) which
   **       will be numerically greater than DJP_PQL_HIGH.
   */
   #define DJP_PQL_DRAFT                     1L
   #define DJP_PQL_LOW                       2L
   #define DJP_PQL_MEDIUM                    3L
   #define DJP_PQL_HIGH                      4L
   #define DJP_PQL_LAST                      DJP_PQL_HIGH
   
   typedef ULONG DJPT_PRINTQUALITY;
   typedef ULONG *PDJPT_PRINTQUALITY;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_PAPERSIZE
   **
   ** Note: it is recommended to use DJP_CJ_FORM to chage the papersize.
   **                                               approximate size
   */                                           /*  inches       millimeters */
   #define DJP_PSI_NONE                      0L
   #define DJP_PSI_LETTER                    1L /*  8.5  x 11    216 x 279   */
   #define DJP_PSI_LEGAL                     2L /*  8.5  x 14    216 x 356   */
   #define DJP_PSI_WIDE                      3L /* 13.58 x 11    345 x 279   */
   #define DJP_PSI_CSHEET                    4L /* 17    x 22    431 x 558   */
   #define DJP_PSI_DSHEET                    5L /* 22    x 34    558 x 863   */
   #define DJP_PSI_ESHEET                    6L /* 34    x 44    863 x 1117  */
   #define DJP_PSI_LETTERSMALL               7L /*                           */
   #define DJP_PSI_TABLOID                   8L /* 11    x 17    279 x 431   */
   #define DJP_PSI_LEDGER                    9L /* 17    x 11    431 x 279   */
   #define DJP_PSI_STATEMENT                10L /*  5.5  x  8.5  139 x 216   */
   #define DJP_PSI_EXECUTIVE                11L /*  7.25 x 10.5  184 x 266   */
   #define DJP_PSI_A0                       12L /* 33.11 x 46.81 841 x 1189  */
   #define DJP_PSI_A1                       13L /* 23.39 x 33.11 594 x 841   */
   #define DJP_PSI_A2                       14L /* 16.54 x 23.39 420 x 594   */
   #define DJP_PSI_A3                       15L /* 11.7  x 16.54 297 x 420   */
   #define DJP_PSI_A4                       16L /*  8.3  x 11.7  210 x 297   */
   #define DJP_PSI_A4_SMALL                 17L /*                           */
   #define DJP_PSI_A5                       18L /*  5.83 x  8.27 148 x 210   */
   #define DJP_PSI_B4                       19L /*  9.84 x 13.94 250 x 354   */
   #define DJP_PSI_B5                       20L /*  7.17 x 10.12 182 x 257   */
   #define DJP_PSI_FOLIO                    21L /*  8.5  x 13    216 x 330   */
   #define DJP_PSI_QUATRO                   22L /*  8.46 x 10.83 215 x 275   */
   #define DJP_PSI_10X14                    23L /* 10    x 14    254 x 355   */
   #define DJP_PSI_11X17                    24L /* 11    x 17    279 x 431   */
   #define DJP_PSI_NOTE                     25L /*                           */
   #define DJP_PSI_ENV_9                    26L /*                           */
   #define DJP_PSI_ENV_10                   27L /*                           */
   #define DJP_PSI_ENV_11                   28L /*                           */
   #define DJP_PSI_ENV_12                   29L /*                           */
   #define DJP_PSI_ENV_14                   30L /*                           */
   #define DJP_PSI_ENV_DL                   31L /*                           */
   #define DJP_PSI_ENV_A2                   32L /*                           */
   #define DJP_PSI_ENV_C3                   33L /*                           */
   #define DJP_PSI_ENV_C4                   34L /*                           */
   #define DJP_PSI_ENV_C5                   35L /*                           */
   #define DJP_PSI_ENV_C6                   36L /*                           */
   #define DJP_PSI_ENV_C65                  37L /*                           */
   #define DJP_PSI_ENV_C9                   38L /*                           */
   #define DJP_PSI_ENV_C10                  39L /*                           */
   #define DJP_PSI_ENV_B4                   40L /*                           */
   #define DJP_PSI_ENV_B5                   41L /*                           */
   #define DJP_PSI_ENV_B6                   42L /*                           */
   #define DJP_PSI_ENV_ITALY                43L /*                           */
   #define DJP_PSI_ENV_MONARCH              44L /*                           */
   #define DJP_PSI_ENV_PERSONAL             45L /*                           */
   #define DJP_PSI_FANFOLD_US               46L /*                           */
   #define DJP_PSI_FANFOLD_STD_GERMAN       47L /*                           */
   #define DJP_PSI_FANFOLD_LGL_GERMAN       48L /*                           */
   #define DJP_PSI_ARCHITECT_BSHEET         49L /*                           */
   #define DJP_PSI_ARCHITECT_CSHEET         50L /*                           */
   #define DJP_PSI_ARCHITECT_DSHEET         51L /*                           */
   #define DJP_PSI_ARCHITECT_ESHEET         52L /*                           */
   #define DJP_PSI_CARD_A6                  53L /*                           */
   #define DJP_PSI_CARD_4X6                 54L /*                           */
   #define DJP_PSI_CARD_5X8                 55L /*                           */
   #define DJP_PSI_CARD_HAGAKI              56L /*                           */
   #define DJP_PSI_LABEL_STANDARD           57L /*  1.10 x 3.50   28 x 89    */
   #define DJP_PSI_LABEL_SHIPPING           58L /*  3.98 x 2.13  101 x 54    */
   #define DJP_PSI_LABEL_DISK               59L /*  2.76 x 2.13   70 x 54    */
   #define DJP_PSI_LABEL_EURO               60L /*  3.50 x 1.42   89 x 36    */
   #define DJP_PSI_CARD_OUFUKU_HAGAKI       61L /*                           */
   #define DJP_PSI_B0                       62L /*                           */
   #define DJP_PSI_B1                       63L /*                           */
   #define DJP_PSI_B2                       64L /*                           */
   #define DJP_PSI_B3                       65L /*                           */
   #define DJP_PSI_B6                       66L /*                           */
   #define DJP_PSI_B7                       67L /*                           */
   #define DJP_PSI_B8                       68L /*                           */
   #define DJP_PSI_B9                       69L /*                           */
   #define DJP_PSI_B10                      70L /*                           */
   #define DJP_PSI_B0_JIS                   71L /*                           */
   #define DJP_PSI_B1_JIS                   72L /*                           */
   #define DJP_PSI_B2_JIS                   73L /*                           */
   #define DJP_PSI_B3_JIS                   74L /*                           */
   #define DJP_PSI_B4_JIS                   75L /*                           */
   #define DJP_PSI_B5_JIS                   76L /*                           */
   #define DJP_PSI_B6_JIS                   77L /*                           */
   #define DJP_PSI_B7_JIS                   78L /*                           */
   #define DJP_PSI_B8_JIS                   79L /*                           */
   #define DJP_PSI_B9_JIS                   80L /*                           */
   #define DJP_PSI_B10_JIS                  81L /*                           */
   #define DJP_PSI_LAST                     DJP_PSI_B10_JIS
   
   typedef LONG DJPT_PAPERSIZE;
   typedef LONG *PDJPT_PAPERSIZE;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_TRAYTYPE
   **
   ** Note: it is recommended to use DJP_CJ_FORM to chage the tray type.
   */
   #define DJP_TRY_NONE                      0L
   #define DJP_TRY_UPPER                     1L
   #define DJP_TRY_ONLYONE                   DJP_TRY_UPPER
   #define DJP_TRY_LOWER                     2L
   #define DJP_TRY_MIDDLE                    3L
   #define DJP_TRY_MANUAL                    4L
   #define DJP_TRY_ENVELOPE                  5L
   #define DJP_TRY_ENVMANUAL                 6L
   #define DJP_TRY_AUTO                      7L
   #define DJP_TRY_TRACTOR                   8L
   #define DJP_TRY_SMALLFMT                  9L
   #define DJP_TRY_LARGEFMT                 10L
   #define DJP_TRY_LARGECAPACITY            11L
   #define DJP_TRY_CASSETTE                 12L
   #define DJP_TRY_LAST                     DJP_TRY_CASSETTE
   
   typedef ULONG DJPT_TRAYTYPE;
   typedef ULONG *PDJPT_TRAYTYPE;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_MEDIA
   **
   ** Note: it is recommended to use DJP_CJ_FORM to chage the media type.
   */
   #define DJP_MED_NONE                      0L
   #define DJP_MED_PLAIN                     1L
   #define DJP_MED_TRANSPARENCY              2L
   #define DJP_MED_GLOSSY                    3L
   #define DJP_MED_SPECIAL                   4L
   #define DJP_MED_COATED                    5L
   #define DJP_MED_BACKPRINT                 6L
   #define DJP_MED_CLOTH                     7L
   #define DJP_MED_THICK                     8L
   #define DJP_MED_STATIONARY                9L
   #define DJP_MED_ENVELOPE                 10L
   #define DJP_MED_CONTINUOUS_LONG          11L
   #define DJP_MED_CONTINUOUS_SHORT         12L
   #define DJP_MED_TAB_STOCK                13L
   #define DJP_MED_MULTI_PART_FORM          14L
   #define DJP_MED_LABELS                   15L
   #define DJP_MED_LAST                     DJP_MED_LABELS
   
   typedef ULONG DJPT_MEDIA;
   typedef ULONG *PDJPT_MEDIA;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_MEDIA_COLOR
   **
   ** Select the media color (for the same media types).
   */
   #define DJP_MDC_BLUE                      1L
   #define DJP_MDC_BLUFF                     2L
   #define DJP_MDC_GOLDENROD                 3L
   #define DJP_MDC_GREEN                     4L
   #define DJP_MDC_PINK                      5L
   #define DJP_MDC_TRANSPARENT               6L
   #define DJP_MDC_WHITE                     7L
   #define DJP_MDC_YELLOW                    8L
   #define DJP_MDC_LAST                      DJP_MDC_YELLOW
   
   typedef ULONG DJPT_MEDIA_COLOR;
   typedef ULONG *PDJPT_MEDIA_COLOR;
   
   /* -----------------------------------------------------------------
   ** DJP_CJ_FORM
   **
   ** Setting will match all three fields.  If szTrayname or szMedianame
   **    is null then it will be defaulted to the first one found.
   **
   ** Querying will return all fields filled in.
   */
   typedef struct _djpForm   /* djpFRM */
   {
      CHAR            szFormname[32];        /* System Form name                  */
      CHAR            szTrayname[32];        /* System Tray name                  */
      CHAR            szMedianame[32];       /* System Media name                 */
      /*                      v-= Informational only =-v                          */
      HCINFO          hcInfo;                /* Corresponding hard copy info      */
      CHAR            szDisplayFormname[64]; /* Display Form name  (translated)   */
      CHAR            szDisplayTrayname[64]; /* Display Tray name  (translated)   */
      CHAR            szDisplayMedianame[64];/* Display Media name (translated)   */
      DJPT_PAPERSIZE  djppsFormID;           /* Simple form id  (if not DJP_NONE) */
      DJPT_TRAYTYPE   djpttTrayID;           /* Simple tray id  (if not DJP_NONE) */
      DJPT_MEDIA      djpmdMediaID;          /* Simple media id (if not DJP_NONE) */
   } DJPT_FORM, *PDJPT_FORM;
   
   /* -----------------------------------------------------------------
   ** DJP_CJ_MIXEDFORMS
   **
   ** This is unique in that both setting and querying can have multiple
   ** elements.  Both the first page and the last page are DJP_MXF_INFINITY
   ** (which is the separator for individual elements).
   ** Some examples are:
   **
   ** - Only one form (form1) for the entire job.
   **   (DJP_MXF_INFINITY, DJP_MXF_INFINITY, form1)
   **
   ** - Page 1 has form1, then pages 2 ... n have form2.
   **   (DJP_MXF_INFINITY, 1, form1) (2, DJP_MXF_INFINITY, form2)
   **
   ** - Even pages have form1 and odd pages have form2
   **   (DJP_MXF_INFINITY, DJP_MXF_ODD, form1) (DJP_MXF_EVEN, DJP_MXF_INFINITY, form2)
   **   or (DJP_MXF_INFINITY, DJP_MXF_EVEN, form2) (DJP_MXF_ODD, DJP_MXF_INFINITY, form1)
   **
   ** - First page has form1, even pages have form2 and odd pages have form3
   **   (DJP_MXF_INFINITY, 1, form1) (DJP_MXF_ODD, DJP_MXF_ODD, form1) (DJP_MXF_EVEN, DJP_MXF_INFINITY, form2)
   **   or (DJP_MXF_INFINITY, 1, form1) (DJP_MXF_EVEN, DJP_MXF_EVEN, form2) (DJP_MXF_ODD, DJP_MXF_INFINITY, form1)
   **
   */
   #define DJP_MXF_INFINITY                 -1L
   #define DJP_MXF_ODD                      -2L
   #define DJP_MXF_EVEN                     -3L
   
   typedef struct _djpMixedForms   /* djpMXF */
   {
      LONG            lStartRange;           /* Starting page number           */
      LONG            lEndRange;             /* Ending page number             */
      DJPT_FORM       djpfmForm;             /* Form associated with the range */
   } DJPT_MIXEDFORMS, *PDJPT_MIXEDFORMS;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_FONTDOWNLOADING
   */
   #define DJP_FDL_NONE                      0L /* Device does not support downloading*/
   #define DJP_FDL_DOWNLOAD                  1L /* Download fonts to printer          */
   #define DJP_FDL_BITMAP                    2L /* Rasterize fonts                    */
   #define DJP_FDL_SUBSTITUTE                3L /* Substitute device fonts for system */
   
   typedef ULONG DJPT_FONTDOWNLOADING;
   typedef ULONG *PDJPT_FONTDOWNLOADING;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_DUPLEX
   */
   #define DJP_DUP_NONE                      0L /* Device does not support duplex     */
   #define DJP_DUP_OFF                       1L /* Duplex is turned off               */
   #define DJP_DUP_BOOK                      2L
   #define DJP_DUP_FLIP                      3L
   
   typedef ULONG DJPT_DUPLEX;
   typedef ULONG *PDJPT_DUPLEX;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_COLLATE
   */
   #define DJP_COL_NONE                      0L /* Device does not support collation  */
   #define DJP_COL_OFF                       1L
   #define DJP_COL_ON                        2L
   #define DJP_COL_PRINTER_SETTING           3L /* more for printer dialogs than      */
   /* programmatic control.  Use         */
   /* the setting on the printer panel.  */
   
   typedef ULONG DJPT_COLLATE;
   typedef ULONG *PDJPT_COLLATE;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_FEED
   */
   #define DJP_FED_MANUAL                    1L
   #define DJP_FED_AUTOMATIC                 2L
   
   typedef ULONG DJPT_FEED;
   typedef ULONG *PDJPT_FEED;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_COPIES
   **
   ** This is the number of copies on a per page basis.  This is not
   ** enumerateable.
   */
   typedef ULONG DJPT_COPIES;
   typedef ULONG *PDJPT_COPIES;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_SCALING
   **
   ** This is a percentage value.  This is not enumerateable.
   */
   typedef LONG DJPT_SCALING;
   typedef LONG *PDJPT_SCALING;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_FORMFEEDCONTROL
   **
   ** This is a property that effects raw data jobs (print from the
   ** command line, DOS print jobs, Windows print jobs). This checks
   ** the very last byte of the data stream to see if it is a form
   ** feed control character.
   */
   #define DJP_FFC_NONE                      1L    /* Never add        */
   #define DJP_FFC_CONDITIONAL               2L    /* Add if not seen  */
   #define DJP_FFC_COMPULSORY                3L    /* Always add       */
   
   typedef ULONG DJPT_FORMFEEDCONTROL;
   typedef ULONG *PDJPT_FORMFEEDCONTROL;
   
   /* -----------------------------------------------------------------
   ** DJP_SJ_N_UP
   **
   ** Number of logical pages per physical page (ex: 2-up, 4-up)
   */
   typedef LONG DJPT_NUP;
   typedef LONG *PDJPT_NUP;
   
   /* -----------------------------------------------------------------
   ** DJP_CJ_OUTPUTBIN
   **
   ** Setting will only use szBinname.
   **
   ** Querying will return all fields filled in.
   */
   typedef struct _djpOutputBin   /* djpOBN */
   {
      CHAR            szBinname[32];         /* System Bin name                   */
      /*                      v-= Informational only =-v                          */
      CHAR            szDisplayBinname[64];  /* Display Bin name  (translated)    */
      LONG            lBinId;                /* Bin id # (-1 for no id)           */
   } DJPT_OUTPUTBIN, *PDJPT_OUTPUTBIN;
   
   /* -----------------------------------------------------------------
   ** DJP_CJ_TRAYNAME
   **
   ** Setting will match only szTrayname.  The perfered way to set which
   ** tray to use is DJP_CJ_FORM.  Otherwise, you are not guaranteed a
   ** unique match for all three form, tray, and media possibilities.
   **
   ** Querying will return all fields filled in.
   */
   typedef struct _djpInputTray   /* djpTry */
   {
      CHAR            szTrayname[32];        /* System Tray name                  */
      /*                      v-= Informational only =-v                          */
      CHAR            szDisplayTrayname[64]; /* Display Tray name  (translated)   */
      DJPT_TRAYTYPE   djpttTrayID;           /* Simple tray id  (if not DJP_NONE) */
   } DJPT_TRAYNAME, *PDJPT_TRAYNAME;
   
#endif

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
