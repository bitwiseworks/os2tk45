/****************************** Module Header ******************************\
*
* Module Name: PMDDI.H
*
* OS/2 Presentation Manager entry points, macros, and types for the
* Graphics Engine.
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* The following symbols are used in this file for conditional sections.
*
*   #define:                To include:
*
*   INCL_DDIGRE             don't want OS2DEF, PMGPI, and PMDEV included
*   INCL_DDIDEFS            don't want the above, but do want GRE structs
*
* It is expected that callers of the Engine will want to define INCL_DDIGRE
* and possibly INCL_DDIDEFS.  The Engine itself and device drivers define
* neither of these.
*
* Further defines must be made to specify which of the GRE function macros
* should be defined.
*
*   #define:                To include:
*
*   INCL_GREALL           All of the following
*   INCL_GRE_ARCS         Arcs functions
*   INCL_GRE_BITMAPS      Bitmap functions
*   INCL_GRE_CLIP            etc...
*   INCL_GRE_COLORTABLE
*   INCL_GRE_DCS
*   INCL_GRE_PAX
*   INCL_GRE_DEVICE
*   INCL_GRE_DEVMISC
*   INCL_GRE_DEVSUPPORT
*   INCL_GRE_FONTS
*   INCL_GRE_INKPATH
*   INCL_GRE_JOURNALING
*   INCL_GRE_LCID
*   INCL_GRE_LINES
*   INCL_GRE_MARKERS
*   INCL_GRE_PALETTE
*   INCL_GRE_PATHS
*   INCL_GRE_PICK
*   INCL_GRE_POLYGON
*   INCL_GRE_REGIONS
*   INCL_GRE_SCANS
*   INCL_GRE_SETID
*   INCL_GRE_STRINGS
*   INCL_GRE_XFORMS
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


#ifdef INCL_GREALL
   #define INCL_GRE_ARCS
   #define INCL_GRE_BITMAPS
   #define INCL_GRE_CLIP
   #define INCL_GRE_COLORTABLE
   #define INCL_GRE_DCS
   #define INCL_GRE_PAX
   #define INCL_GRE_DEVICE
   #define INCL_GRE_DEVMISC
   #define INCL_GRE_DEVSUPPORT
   #define INCL_GRE_FONTS
   #define INCL_GRE_INKPATH
   #define INCL_GRE_JOURNALING
   #define INCL_GRE_LCID
   #define INCL_GRE_LINES
   #define INCL_GRE_MARKERS
   #define INCL_GRE_PALETTE
   #define INCL_GRE_PATHS
   #define INCL_GRE_PICK
   #define INCL_GRE_POLYGON
   #define INCL_GRE_REGIONS
   #define INCL_GRE_SCANS
   #define INCL_GRE_SCREEN
   #define INCL_GRE_SETID
   #define INCL_GRE_STRINGS
   #define INCL_GRE_XFORMS
#endif /* INCL_GREALL */

#ifdef INCL_GRE_DEVMISC
   #define INCL_GRE_DEVMISC1
   #define INCL_GRE_DEVMISC2
   #define INCL_GRE_DEVMISC3
#endif  /* INCL_GRE_DEVMISC */

#ifdef INCL_VMANDDI
   #define INCL_DDIBUNDLES
#endif  /* INCL_VMANDDI */

/*** Engine dispatch tables ************************************************/

#define MAXTABLEENTRIES        223
#define NUMBEROFMAJORFUN       MAXTABLEENTRIES
#define NUMBEROFMINORFUN       86

typedef struct _DISPATCHER    /* dr */
{
   PFN    pfnTblEntry[MAXTABLEENTRIES];
} DISPATCHER;
typedef DISPATCHER *PDISPATCHER;

typedef struct _FUNINFOTABLE    /* fit */
{
   BYTE   FunInfo_00[NUMBEROFMAJORFUN];
   BYTE   FunInfo_02[NUMBEROFMINORFUN];
} FUNINFOTABLE;
typedef FUNINFOTABLE *PFUNINFOTABLE;

/*** exported Engine DDI functions *****************************************/

LONG  APIENTRY SetDriverInfo(ULONG hDrv,
                             LHANDLE hEng,
                             ULONG ulIndex,
                             HDC hdc);

LONG  APIENTRY GetDriverInfo(LHANDLE hEng,
                             ULONG ulIndex,
                             HDC hdc);

#ifdef __cplusplus
LONG  APIENTRY PostDeviceModes(PDRIVDATA pDrvData,
                               PCSZ  pszDrvName,
                               PCSZ  pszDevName,
                               PCSZ  pszPrtName,
                               ULONG ulOptions);
#else
LONG  APIENTRY PostDeviceModes(PDRIVDATA pDrvData,
                               PSZ pszDrvName,
                               PSZ pszDevName,
                               PSZ pszPrtName,
                               ULONG ulOptions);
#endif

#ifdef __cplusplus
LONG  APIENTRY PostEscape(PCSZ  pszDrvName,
                          PCSZ  pszDevName,
                          PCSZ  pszQueueName,
                          PCSZ  pszSplPrinterName,
                          ULONG ulFuncNum,
                          ULONG cbParm1,
                          PBYTE pbParm1,
                          ULONG cpParm2,
                          PBYTE pbParm2);
#else
LONG  APIENTRY PostEscape(PSZ pszDrvName,
                          PSZ pszDevName,
                          PSZ pszQueueName,
                          PSZ pszSplPrinterName,
                          ULONG ulFuncNum,
                          ULONG cbParm1,
                          PBYTE pbParm1,
                          ULONG cpParm2,
                          PBYTE pbParm2);
#endif

BOOL  APIENTRY GreInitialize(VOID);

ULONG APIENTRY SSAllocMem(PVOID ppAddr,
                          ULONG ulSize,
                          ULONG ulReserved);

ULONG APIENTRY SSFreeMem(PVOID pAddr);

/*** define common types in the Engine and DDI *****************************/

typedef struct _RECTS   /* rcs */
{
   POINTS pts1;
   POINTS pts2;
} RECTS;
typedef RECTS *PRECTS;

/* NOINC */
#ifndef POINTFX_DEFINED
   #define POINTFX_DEFINED
   /* INC */
   typedef struct _POINTFX   /* ptfx */
   {
      FIXED x;
      FIXED y;
   } POINTFX;
   typedef POINTFX *PPOINTFX;
   /* NOINC */
#endif
/* INC */

typedef struct _RECTFX   /* rcfx */
{
   POINTFX ptfx1;
   POINTFX ptfx2;
} RECTFX;
typedef RECTFX *PRECTFX;

typedef struct _XFORM   /* xform */
{
   FIXED fxM11;
   FIXED fxM12;
   FIXED fxM21;
   FIXED fxM22;
   LONG  lM41;
   LONG  lM42;
} XFORM;
typedef XFORM *PXFORM;

typedef LONG LCID;      /* locally-coded id */
typedef LCID *PLCID;
typedef LONG PHID;      /* path id */
typedef ULONG HDEVPAL;

#ifdef INCL_GRE_HFONT
   typedef LHANDLE HFONT;       /* font handle */
#endif /* INCL_GRE_HFONT */

#ifdef INCL_DDICOMFLAGS
   #define COM_DRAW16                      0x00000001L
   #define COM_BOUND16                     0x00000002L
   #define COM_CORRELATE16                 0x00000004L
   #define COM_ALT_BOUND16                 0x00000008L
   #define COM_AREA16                      0x00000010L
   #define COM_PATH16                      0x00000020L
   #define COM_TRANSFORM16                 0x00000040L
   #define COM_RECORDING16                 0x00000080L
   #define COM_DEVICE16                    0x00000100L
   #define COM_SCR_BOUND16                 0x00000200L
   #define COM_PRECLIP16                   0x04000000L
   
   #define COM_DRAW                        0x00010000L
   #define COM_BOUND                       0x00020000L
   #define COM_CORRELATE                   0x00040000L
   #define COM_ALT_BOUND                   0x00080000L
   #define COM_AREA                        0x00100000L
   #define COM_PATH                        0x00200000L
   #define COM_TRANSFORM                   0x00400000L
   #define COM_RECORDING                   0x00800000L
   #define COM_DEVICE                      0x01000000L
   #define COM_SCR_BOUND                   0x02000000L
   #define COM_PRECLIP                     0x04000000L
#endif /* INCL_DDICOMFLAGS */

#ifdef INCL_GRE_ARCS
   
   /* BoxBoundary */
   /* BoxInterior */
   /* BoxBoth */
   
   typedef struct _BOXPARAMS      /* boxp */
   {
      POINTL ptl;
      SIZEL  sizl;
   } BOXPARAMS;
   typedef BOXPARAMS *PBOXPARAMS;
#endif /* INCL_GRE_ARCS */

#ifdef INCL_GRE_CLIP
   
   /* CopyClipRegion */
   
   #define COPYCRGN_ALLINTERSECT        0L
   #define COPYCRGN_VISRGN              1L
   #define COPYCRGN_CLIPRGN             2L
   
   /* SetupDC */
   
   #define SETUPDC_VISRGN               0x00000001L
   #define SETUPDC_ORIGIN               0x00000002L
   #define SETUPDC_ACCUMBOUNDSON        0x00000004L
   #define SETUPDC_ACCUMBOUNDSOFF       0x00000008L
   #define SETUPDC_RECALCCLIP           0x00000010L
   #define SETUPDC_SETOWNER             0x00000020L
   #define SETUPDC_CLEANDC              0x00000040L
   
#endif /* INCL_GRE_CLIP */

#ifdef INCL_GRE_XFORMS
   
   /* QueryViewportSize */
   typedef struct _VIEWPORTSIZE   /* vs */
   {
      ULONG cx;
      ULONG cy;
   } VIEWPORTSIZE;
   typedef VIEWPORTSIZE *PVIEWPORTSIZE;
   
#endif /* INCL_GRE_XFORMS */

#ifdef INCL_GRE_DEVSUPPORT
   
   /* Constants for GreInitializeAttributes */
   
   #define INAT_DEFAULTATTRIBUTES       1L
   #define INAT_CURRENTATTRIBUTES       2L
   
   /* InvalidateVisRegion */
   
   typedef struct _DC_BLOCK   /* ivr */
   {
      ULONG hdc;
      ULONG hddc;
   } DC_BLOCK;
   typedef DC_BLOCK *PDC_BLOCK;
   
   /* Constants for DeviceEnableYInversion */
   
   #define CAP_YINVERT_NONE                 0x00000000L
   #define CAP_YINVERT_CHARSTRINGPOS        0x00000001L
   #define CAP_YINVERT_CHARSTRING           0x00000002L
   #define CAP_YINVERT_QUERYTEXTBOX         0x00000004L
   #define CAP_YINVERT_QUERYCHARPOSITIONS   0x00000008L
   #define CAP_YINVERT_DEVICEQUERYFONTS     0x00000010L
   #define CAP_YINVERT_DONTCARE             0x80000000L
   #define CAP_YINVERT_IMPLEMENTED          CAP_YINVERT_CHARSTRINGPOS
   
#endif /* INCL_GRE_DEVSUPPORT */

#ifdef INCL_GRE_FONTS
   
   /* QueryCodePageObject */
   
   typedef ULONG GLYPH;
   typedef ULONG *PGLYPH;
   
   /* NOINC */
   typedef  ULONG (* APIENTRY PXLATECHAR)(PVOID object,
                   ULONG ulCod,
                   PGLYPH pGly);
   typedef  ULONG (* APIENTRY PXLATESTRING)(PVOID object,
                   PUCHAR pchCod,
                   ULONG  ulCodLen,
                   PGLYPH pGly,
                   PULONG pulGlyLen,
                   ULONG flOptions);
   typedef  ULONG (* APIENTRY PINVXLATECHAR)(PVOID object,
                   GLYPH gGly,
                   PULONG pulCod);
   typedef  ULONG (* APIENTRY PINVXLATESTRING)(PVOID object,
                   PGLYPH pGly,
                   ULONG ulGlyLen,
                   PUCHAR pchCod,
                   PULONG pulCodLen,
                   ULONG flOptions);
   typedef struct _CPOBJMEMBERS    /* cpm */
   {
      PXLATECHAR  pfnXlateChar;
      PXLATESTRING pfnXlateString;
      PINVXLATECHAR pfnInvXlateChar;
      PINVXLATESTRING pfnInvXlateString;
   } CPOBJMEMBERS;
   typedef CPOBJMEMBERS *PCPOBJMEMBERS;
   
   typedef struct _CODEPAGEOBJECT    /* cpo */
   {
      ULONG    Identity;
      ULONG    ulCodePage;
      UCHAR    szGlyphList[16];
      ULONG    fl;
      PUCHAR   pDbcsEnv;
      ULONG    ulReserved;
      CPOBJMEMBERS Member;
   } CODEPAGEOBJECT;
   typedef CODEPAGEOBJECT *PCODEPAGEOBJECT;
   /* INC */
   
   #define  CPO_SBCS_CODEPAGE    0x00000001L
   #define  CPO_MBCS_CODEPAGE    0x00000002L
   #define  CPO_DBCS_CODEPAGE    0x00000004L
   
#endif /* INCL_GRE_FONTS */

#ifdef INCL_GRE_STRINGS
   
   /* QueryCharoutline */
   
   #define  QCO_FORMAT_GOCA      0x00000001L
   #define  QCO_FORMAT_IFI       0x00000002L
   #define  QCO_NO_TRANSFORM     0x00000004L
   #define  QCO_UNICODE          0x00000008L
   
   /* QueryCharMetricsTable */
   
   typedef struct _CHARMETRICS    /* cm */
   {
      LONG     lA;
      ULONG    ulB;
      LONG     lC;
      RECTL    rclBoundBox;
   } CHARMETRICS;
   typedef CHARMETRICS *PCHARMETRICS;
   
   #define QCMT_NO_TRANSFORM     0x00000001L
   
   /* RealizeString */
   
   #define RS_NO_TRANSLATION     0x00000001L
   #define RS_UNICODE            0x00000002L
   #define RS_FONT_CACHE         0x00000004L
   #define RS_CREATE_FTA         0x00000008L
   #define RS_DESTROY_FTA        0x00000010L
   #define RS_DELETE_CHARS       0x00000020L
   #define RS_COPY_CHARS         0x00000040L
   
#endif /* INCL_GRE_STRINGS */

#ifdef INCL_DDIMISC
   
   /* Display information resource structure (RT_DISPLAYINFO) */
   typedef struct _DISPLAYINFO           /* dspinfo */
   {
      USHORT cb;
      SHORT cxIcon;
      SHORT cyIcon;
      SHORT cxPointer;
      SHORT cyPointer;
      SHORT cxBorder;
      SHORT cyBorder;
      SHORT cxHSlider;
      SHORT cyVSlider;
      SHORT cxSizeBorder;
      SHORT cySizeBorder;
      SHORT cxDeviceAlign;
      SHORT cyDeviceAlign;
   } DISPLAYINFO;
   typedef DISPLAYINFO *PDISPLAYINFO;
   
   /* Parameters for the DC Enable function */
   
   typedef struct _DENPARAMS   /* den */
   {
      ULONG ulStateInfo;
      ULONG ulType;
      ULONG ulHDC;
   } DENPARAMS;
   typedef DENPARAMS *PDENPARAMS;
   
   
   typedef struct _STYLERATIO   /* sr */
   {
      BYTE dx;
      BYTE dy;
   } STYLERATIO;
   typedef STYLERATIO *PSTYLERATIO;
   
   /* Options flags for SetGlobalAttribute */
   
   #define GATTR_DEFAULT                   1L
   
   /* Attribute Types for SetGlobalAttribute */
   
   #define ATYPE_COLOR                     1L
   #define ATYPE_BACK_COLOR                2L
   #define ATYPE_MIX_MODE                  3L
   #define ATYPE_BACK_MIX_MODE             4L
   
   /* Options for CharStringPos */
   
   #define CHS_START_XY                    0x00000020L
   #define CHS_ATTR_INFO                   0x00000040L
   
   typedef struct _CSP_INFO   /* csp */
   {
      LONG  cSize;
      LONG  lColor;
      LONG  lBackColor;
   } CSP_INFO;
   typedef CSP_INFO *PCSP_INFO;
   
   /* Set/GetProcessControl */
   
   #define PCTL_DRAW                       0x00000001L
   #define PCTL_BOUND                      0x00000002L
   #define PCTL_CORRELATE                  0x00000004L
   #define PCTL_USERBOUNDS                 0x00000008L
   #define PCTL_AREA                       0x00000010L
   
   /* ResetBounds */
   
   #define RB_GPI                          0x00000001L
   #define RB_USER                         0x00000002L
   
   /* GetBoundsData */
   
   #define GBD_GPI                         0L
   #define GBD_USER                        1L
   
   /* EndArea Cancel Option */
   
   #define EA_DRAW                         0x00000000L
   #define EA_CANCEL                       0x00000001L
   
   /* Bitblt Style */
   
   #define BLTMODE_SRC_BITMAP              0x00010000L
   #define BLTMODE_ATTRS_PRES              0x00020000L
   #define BBO_TARGWORLD                   0x00000100L
   
   typedef struct _BITBLTPARAMETERS   /* bbp */
   {
      RECTL rclTarg;
      RECTL rclSrc;
   } BITBLTPARAMETERS;
   typedef BITBLTPARAMETERS *PBITBLTPARAMETERS;
   
   typedef struct _BITBLTATTRS   /* bba */
   {
      LONG cSize;
      LONG lColor;
      LONG lBackColor;
   } BITBLTATTRS;
   typedef BITBLTATTRS *PBITBLTATTRS;
   
   /* LCIDs */
   
   #define LCID_AVIO_1                     (-2L)
   #define LCID_AVIO_2                     (-3L)
   #define LCID_AVIO_3                     (-4L)
   #define LCID_BITMAP_ID_REQUEST          (-5L)
   
   #define LCID_RANGE_GPI                  1L
   #define LCID_RANGE_AVIO                 2L
   #define LCID_RANGE_BOTH                 3L
   #define LCID_GRAPHICS_MIN               1
   #define LCID_GRAPHICS_MAX               254
   
   #define LCIDT_NONE                      0L
   
   /* ResetDC */
   
   #define RDC_RGBMODE                     0x1L
   #define RDC_SETOWNERTOSHELL             0x2L
   
   /* SetRandomXform */
   
   #define SX_UNITY                        0L
   #define SX_CAT_AFTER                    1L
   #define SX_CAT_BEFORE                   2L
   #define SX_OVERWRITE                    3L
   
   /* transform accelerators                    */
   /*  These bits are only valid if the MATRIX_SIMPLE bit is set.           */
   /*  The X and Y negate flags are only meaningful if MATRIX_UNITS is set.*/
   
   #define MATRIX_SIMPLE            0x0001L  /* two entries are zero */
   #define MATRIX_UNITS             0x0002L  /* all entries are +1 or -1 */
   #define MATRIX_XY_EXCHANGE       0x0004L  /* zeros are on the diagonal*/
   #define MATRIX_X_NEGATE          0x0008L  /* X is hit by negative */
   #define MATRIX_Y_NEGATE          0x0010L  /* Y is hit by negative */
   #define MATRIX_TRANSLATION       0x0020L  /* non-zero translation */
   
   /* NotifyClipChange */
   
   #define NCC_CLEANDC              0x0002L  /* clear DC dirty bit */
   
   /* NotifyTransformChange */
   
   typedef struct _NOTIFYTRANSFORMDATA   /* ntd */
   {
      ULONG  usType;
      XFORM  xform;
   } NOTIFYTRANSFORMDATA;
   typedef NOTIFYTRANSFORMDATA *PNOTIFYTRANSFORMDATA;
   
   /* ColorTable */
   
   #define LCOL_SYSCOLORS           0x0010L
   
   
   /* query device caps */
   
   typedef struct _QCDARRAY   /* qcd */
   {
      LONG    iFormat;
      LONG    iSmallest;
      LONG    iLargest;
      LONG    cAvailable;
      LONG    cSpecifiable;
      LONG    iMax;
   } QCDARRAY;
   typedef QCDARRAY *PQCDARRAY;
   
   #define CAPS_MIX_OR                  0x00000001L
   #define CAPS_MIX_COPY                0x00000002L
   #define CAPS_MIX_UNDERPAINT          0x00000004L
   #define CAPS_MIX_XOR                 0x00000008L
   #define CAPS_MIX_INVISIBLE           0x00000010L
   #define CAPS_MIX_AND                 0x00000020L
   #define CAPS_MIX_OTHER               0x00000040L
   
   #define CAPS_DEV_FONT_SIM_BOLD       1L      /* for CAPS_DEVICE_FONT_SIM */
   #define CAPS_DEV_FONT_SIM_ITALIC     2L
   #define CAPS_DEV_FONT_SIM_UNDERSCORE 4L
   #define CAPS_DEV_FONT_SIM_STRIKEOUT  8L
   
   #define CAPS_BACKMIX_OR              0x00000001L
   #define CAPS_BACKMIX_COPY            0x00000002L
   #define CAPS_BACKMIX_UNDERPAINT      0x00000004L
   #define CAPS_BACKMIX_XOR             0x00000008L
   #define CAPS_BACKMIX_INVISIBLE       0x00000010L
   
   
   /*#define CAPS_RASTER_BITBLT         0x00000001L defined in pmdev.h */
   /*#define CAPS_RASTER_BANDING        0x00000002L */
   /*#define CAPS_RASTER_STRETCHBLT     0x00000004L */
   /*#define CAPS_RASTER_SETPEL         0x00000010L */
   #define CAPS_FONT_OUTLINE_MANAGE     16L
   #define CAPS_FONT_IMAGE_MANAGE       32L
   #define SFONT_RASTER                 100
   #define SFONT_OUTLINE                101
   #define FONT              1000       /* must not conflict with RT_XXX */
   /* constants in BSEDOS.H */
   
   /* DCCaps */
   
   #define DCCAPS_LINE                  0x0100
   #define DCCAPS_CURVE                 0x0200
   #define DCCAPS_AREA                  0x0400
   #define DCCAPS_MARKER                0x0800
   #define DCCAPS_TEXT                  0x1000
   
   /* DeviceDeleteBitmap */
   #define BITMAP_USAGE_TRANSLATE       0x0004
   
   /* DeleteBitmap return structure */
   typedef struct _DELETERETURN   /* dr */
   {
      ULONG      pInfo;
      ULONG      pBits;
   } DELETERETURN;
   typedef DELETERETURN *PDELETERETURN;
   
   /* Short Line Header */
   
   #define SLH_FORMAT_IS_16_DOT_16 1
   #define PSL_YMAJOR 0x8000    /* bit mask for usStyle */
   
   typedef struct _SHORTLINEHEADER   /* slh */
   {
      ULONG  ulStyle;
      ULONG  ulFormat;
      POINTL ptlStart;
      POINTL ptlStop;
      LONG   lxLeft;
      LONG   lxRight;
      struct _SHORTLINEHEADER *pslhNext;
      struct _SHORTLINEHEADER *pslhPrev;
   } SHORTLINEHEADER;
   typedef SHORTLINEHEADER *PSHORTLINEHEADER;
   /* Short Line */
   
   typedef struct _SHORTLINE   /* sl */
   {
      SHORTLINEHEADER slh;
      LONG      ax[1];
   } SHORTLINE;
   typedef SHORTLINE *PSHORTLINE;
   
   /* Bounding rectangle for subpaths. */
   
   typedef struct _BRECTL          /* brctl */
   {
      POINTL ptl1;
      POINTL ptl2;
   } BRECTL;
   typedef BRECTL *PBRECTL;
   
   typedef struct _RLEHDR   /* rle */
   {
      LONG     lType;
      BRECTL   brectlBounds;
      PVOID    pRLE;
   } RLEHDR;
   typedef RLEHDR *PRLEHDR;
   
   
   typedef struct _SCANDATA   /* sd */
   {
      PSHORTLINE pslFirstLeft;
      PSHORTLINE pslLastLeft;
      PSHORTLINE pslFirstRight;
      PSHORTLINE pslLastRight;
      ULONG      c;
      RECTL      rclBound;
   } SCANDATA;
   typedef SCANDATA *PSCANDATA;
   
   /* Index for Set/GetDriverInfo */
   
   #define DI_HDC                      0x00000000L
   #define DI_HBITMAP                  0x00000001L
   
#endif  /* INCL_DDIMISC */

#ifdef INCL_DDIMISC2
   
   /* RealizeFont */
   
   #define REALIZE_FONT                 1
   #define REALIZE_ENGINE_FONT          2
   #define DELETE_FONT                  3
   
   #define RF_DEVICE_FONT               1
   #define RF_LOAD_ENGINE_FONT          2
   #define RF_DELETE_FONT               3
   #define RF_DELETE_ENGINE_FONT        4
   
#endif  /* INCL_DDIMISC2 */

#ifdef INCL_DDIBUNDLES
   
   /* Device Line Bundle */
   
   typedef struct _LINEDEFS   /* ldef */
   {
      ULONG      defType;
   } LINEDEFS;
   typedef LINEDEFS *PLINDEFS;
   
   typedef struct _DLINEBUNDLE   /* dlbnd */
   {
      LONG       cAttr;
      LONG       cDefs;
      LINEBUNDLE lbnd;
      LINEDEFS   ldef;
   } DLINEBUNDLE;
   typedef DLINEBUNDLE *PDLINEBUNDLE;
   
   /* Device Area Bundle */
   
   typedef struct _AREADEFS   /* adef */
   {
      ULONG      defSet;
      ULONG      fFlags;
      ULONG      CodePage;
   } AREADEFS;
   typedef AREADEFS *PAREADEFS;
   
   typedef struct _DAREABUNDLE   /* dabnd */
   {
      LONG       cAttr;
      LONG       cDefs;
      AREABUNDLE abnd;
      AREADEFS   adef;
   } DAREABUNDLE;
   typedef DAREABUNDLE *PDAREABUNDLE;
   
   /* Device Character Bundle */
   
   typedef struct _CHARDEFS   /* cdef */
   {
      ULONG      defSet;
      ULONG      fFlags;
      ULONG      CodePage;
      ULONG      charSpacing;
   } CHARDEFS;
   typedef CHARDEFS *PCHARDEFS;
   
   typedef struct _DCHARBUNDLE   /* dcbnd */
   {
      LONG       cAttr;
      LONG       cDefs;
      CHARBUNDLE cbnd;
      CHARDEFS   cdef;
   } DCHARBUNDLE;
   typedef DCHARBUNDLE *PDCHARBUNDLE;
   
   /* Device Image Bundle */
   
   #ifdef BOGUS
      typedef struct _IMAGEDEFS   /* idef */
      {
      } IMAGEDEFS;
   #endif       /* BOGUS */
   
   typedef struct _DIMAGEBUNDLE   /* dibnd */
   {
      LONG       cAttr;
      LONG       cDefs;
      IMAGEBUNDLE ibnd;
      /* IMAGEDEFS     idef; */
   } DIMAGEBUNDLE;
   typedef DIMAGEBUNDLE *PDIMAGEBUNDLE;
   
   /* Device Marker Bundle */
   
   typedef struct _MARKERDEFS   /* mdef */
   {
      ULONG      defSet;
      ULONG      fFlags;
      ULONG      CodePage;
   } MARKERDEFS;
   typedef MARKERDEFS *PMARKERDEFS;
   
   typedef struct _DMARKERBUNDLE   /* dmbnd */
   {
      LONG       cAttr;
      LONG       cDefs;
      MARKERBUNDLE mbnd;
      MARKERDEFS   mdef;
   } DMARKERBUNDLE;
   typedef DMARKERBUNDLE *PDMARKERBUNDLE;
   
   
#endif /* INCL_DDIBUNDLES */

#ifdef INCL_DDIFONTSTRUCS
   #include <pmfont.h>
#endif /* INCL_DDIFONTSTRUCS */

#ifdef INCL_DDIBITMAPFILE
   #include <pmbitmap.h>
#endif /* INCL_DDIBITMAPFILE */

#ifdef INCL_DDIPATHS
   /* Curve types */
   
   #define CURVE_IDENTIFIER             0x43
   #define LINE_IDENTIFIER              0x4C
   #define CURVE_DO_FIRST_PEL           0x0002
   
   typedef struct _CURVE   /* cv */
   {
      BYTE        bIdent;             /*  1    1  */
      BYTE        bType;              /*  1    2  */
      USHORT      usReserved;         /*  2    4  */
      ULONG       flStyle;            /*  4    8  */
      ULONG       fl;                 /*  4   12  */
      struct      _CURVE *pcvNext;    /*  4   16  */
      struct      _CURVE *pcvPrev;    /*  4   20  */
      struct      _CURVE *pcvAttrs;   /*  4   24  */
      POINTFX     ptfxA;              /*  8   32  */
      POINTFX     ptfxC;              /*  8   40  */
      BYTE        Reserved2[28];      /* 28   68  */
   } CURVE;
   typedef CURVE *PCURVE;
   
   typedef struct _LINE   /* ln */
   {
      BYTE        bIdent;             /*  1    1  */
      BYTE        bType;              /*  1    2  */
      USHORT      usReserved;         /*  2    4  */
      ULONG       flStyle;            /*  4    8  */
      ULONG       fl;                 /*  4   12  */
      PCURVE      pcvNext;            /*  4   16  */
      PCURVE      pcvPrev;            /*  4   20  */
      PCURVE      pcvAttrs;           /*  4   24  */
      POINTFX     ptfxA;              /*  8   32  */
      POINTFX     ptfxC;              /*  8   40  */
      POINTL      ptlA;               /*  8   48  */
      POINTL      ptlC;               /*  8   56  */
      FIXED       fxRslope;           /*  4   60  */
      BYTE        Reserved1[8];       /*  8   68  */
   } LINE;
   typedef LINE *PLINE;
   
#endif  /* INCL_DDIPATHS */

#ifdef INCL_GRE_JOURNALING
   #define JNL_TEMP_FILE            0x00000001L
   #define JNL_PERM_FILE            0x00000002L
   #define JNL_ENGINERAM_FILE       0x00000004L
   #define JNL_USERRAM_FILE         0x00000008L
   #define JNL_DRAW_OPTIMIZATION    0x00000010L
   #define JNL_BOUNDS_OPTIMIZATION  0x00000020L
#endif  /* INCL_GRE_JOURNALING */


#ifdef INCL_GRE_DEVICE
   
   /* QueryDeviceBitmaps */
   
   typedef struct _BITMAPFORMAT   /* bmf */
   {
      ULONG cPlanes;
      ULONG cBitCount;
   } BITMAPFORMAT;
   typedef BITMAPFORMAT *PBITMAPFORMAT;
   
#endif /* INCL_GRE_DEVICE */

#ifdef INCL_GRE_PALETTE
   
   typedef struct _PALETTEINFOHEADER   /* palinfohdr */
   {
      ULONG  flCmd;         /* options from creation                   */
      ULONG  ulFormat;      /* specifies format of entries at creation */
      ULONG  cclr;          /* number of elements supplied at creation */
   } PALETTEINFOHEADER;
   typedef PALETTEINFOHEADER *NPPALETTEINFOHEADER;
   typedef PALETTEINFOHEADER *PPALETTEINFOHEADER;
   
   typedef struct _PALETTEINFO   /* palinfo */
   {
      ULONG  flCmd;         /* options from creation                   */
      ULONG  ulFormat;      /* specifies format of entries at creation */
      ULONG  cclr;          /* number of elements supplied at creation */
      RGB2   argb[1];       /* the palette entries                     */
   } PALETTEINFO;
   typedef PALETTEINFO *NPPALETTEINFO;
   typedef PALETTEINFO *PPALETTEINFO;
   
   /* flType values for RealizePalette */
   #define RP_BACKGROUND                0
   #define RP_FOREGROUND                1
   #define RP_DEFAULTSCHANGED           2
   
#endif

#ifdef INCL_GRE_BITMAPS
   #define LR_CLIPPED   2
   #define LR_NOTBORDER 0
   #define LR_BORDER    1
   #define LR_LEFT      2
   #define LR_RIGHT     4
#endif

/* Flags  for GreOutlinePath */
#define OPTH_NO_CLIPPING_REQD         0x0001L
#define OPTH_QRY_PATH_POINTS          0x0002L

/*** 32-bit cursor/pointer stuff ******************************/
typedef struct _MCDESCRIPTION   /* mcdesc */
{
   PVOID    pMoveCursor;
   ULONG    ulCodeLength;
   PVOID    pCursorData;
   ULONG    ulDataLength;
}  MCDESCRIPTION;
typedef MCDESCRIPTION *PMCDESCRIPTION;

typedef struct _WINCURSORDATA   /* winmc */
{
   ULONG      pMoveCursor;
   ULONG      pMoveCursorAlias;
   ULONG      pCursorData;
   ULONG      pCursorDataAlias;
   ULONG      ulRoutineType;
   HMODULE    hModDisplay;
   PVOID      pWCThunkCode;
   ULONG      ulThunkLen;
   USHORT     usR0CS;
   USHORT     usR0DS;
}  WINCURSORDATA;
typedef WINCURSORDATA *PWINCURSORDATA;

#define MC_32BIT   32
#define MC_16BIT   16

#ifdef INCL_GRE_SCREEN
   
   /* GetScreenBits */
   
   typedef LHANDLE   HSCA;     /* hsca */
   typedef HSCA *PHSCA;
   
   #define GSB_OPT_4BPP    0x0000L
   #define GSB_OPT_8BPP    0x0001L
   #define GSB_OPT_16BPP   0x0002L
   #define GSB_OPT_LINEAR  0x0000L
   #define GSB_OPT_PLANAR  0x0008L
   #define GSB_OPT_HRGN    0x0010L
   
#endif /* INCL_GRE_SCREEN */

#ifdef INCL_VMANDDI
   
   #define CAPS_MAX_CAPS    41
   #define DEFAULT_PATTERNS_NUMBER 20
   
   //*******************************************************************************
   // Halftoning and Dithering support for OS/2
   //*******************************************************************************
   
   /******************************************************************************/
   /*    PPOINTL     pptlSrc;            // src left, bottom, width and height.  */
   /*    PPOINTL     pptlDst;            // dst left, bottom, width and height.  */
   /*    ULONG       ulSrcBytesPerLine;  // src bytes per line                   */
   /*    PBYTE       pbSrcBits;          // pointer to src image data            */
   /*    ULONG       ulTrgBytesPerLine;  // dst bytes per line                   */
   /*    PBYTE       pbTrgBits;          // pointer to dst image data            */
   /*    ULONG       ulOptions;          // options                              */
   /*    ULONG       ulcClrs;            // count of colors in rgb2 table        */
   /*    PRGB2       pargb2;             // rgb2 table                           */
   /******************************************************************************/
   
   typedef struct _IMAGEINFOS          /* imginfo */
   {
      PPOINTL     pptlSrc;
      PPOINTL     pptlDst;
      ULONG       ulSrcBpp;
      ULONG       ulSrcBytesPerLine;
      PBYTE       pbSrcBits;
      ULONG       ulcSrcClrs;
      ULONG       ulSrcClrType;
      PRGB2       pargb2Src;
      ULONG       ulTrgBpp;
      ULONG       ulTrgBytesPerLine;
      PBYTE       pbTrgBits;
      ULONG       ulcTrgClrs;
      ULONG       ulTrgClrType;
      PRGB2       pargb2Trg;
      ULONG       ulOptions;
   } IMAGEINFOS;
   typedef IMAGEINFOS * PIMAGEINFO;    /* pimginfo */
   
   //
   // ulSrcClrType, ulTrgClrType
   // color RGB types.
   //
   #define COLOR_RGB2            0x0000  // color is in RGB2              (default)
   #define COLOR_RGB             0x0001  // color is in RGB
   
   #define COLOR_24BIT_BGR       0x0000  // 1st byte=blue, 2nd byte=green (default)
   // 3rd byte=red
   #define COLOR_24BIT_RGB       0x0001  // 1st byte=red,  2nd byte=green
   // 3rd byte=blue
   
   #define COLOR_16BIT_565_BGR   0x0000  // blue - 5 bits,                (default)
   // green - 6 bits, red - 5 bits
   // ---------------------------------
   // |b|b|b|b|b|g|g|g|g|g|g|r|r|r|r|r|
   // ---------------------------------
   #define COLOR_16BIT_565_RGB   0x0001  // red - 5 bits,
   // green - 6 bits, blue - 5 bits
   // ---------------------------------
   // |r|r|r|r|r|g|g|g|g|g|g|b|b|b|b|b|
   // ---------------------------------
   #define COLOR_16BIT_1555_BGR  0x0002  // overlay - 1, red - 5 bits,
   // green - 5 bits, blue - 5 bits
   // ---------------------------------
   // |o|b|b|b|b|b|g|g|g|g|g|r|r|r|r|r|
   // ---------------------------------
   #define COLOR_16BIT_1555_RGB  0x0004  // overlay - 1, red - 5 bits,
   // green - 5 bits, blue - 5 bits
   // ---------------------------------
   // |o|r|r|r|r|r|g|g|g|g|g|b|b|b|b|b|
   // ---------------------------------
   #define COLOR_32BIT_BGR       0x0000  // 1st byte=blue, 2nd byte=green (default)
   // 3rd byte=red,  4th byte=not used.
   #define COLOR_32BIT_RGB       0x0001  // 1st byte=red,  2nd byte=green
   // 3rd byte=blue, 4th byte=not used.
   
   //
   // ulOptions
   //
   //   Exact color match.
   //     source pels that are the target BG color become target BG color.
   //     All other pels become target FG color.
   #define IMAGEINFO_CLR_TO_MONO_BG_EXACT     0x00000001 // src pels that are trg BG clr -> 1
   // all other pels               -> 0
   #define IMAGEINFO_CLR_TO_MONO_BG_EXACT_INV 0x00000002 // src pels that are trg BG clr -> 0
   // all other pels               -> 1
   //   Nearest color match.
   //     source pels that are close to target BG color become target BG color.
   //     All other pels become target FG color.
   #define IMAGEINFO_CLR_TO_MONO_BG_MAP       0x00000004 // src pels that are close to trg BG clr -> 1
   // all other pels                        -> 0
   #define IMAGEINFO_CLR_TO_MONO_BG_MAP_INV   0x00000008 // src pels that are close to trg BG clr -> 0
   // all other pels                        -> 1
   #define IMAGEINFO_SRC_Y_FLIP               0x00000010 // source is going Y negative direction
   // i.e. second line = first line - ulBytesPerLine.
   #define IMAGEINFO_TRG_Y_FLIP               0x00000020 // target is going Y negative direction
   
   /*******************************************************************************************/
   /*    ULONG       ulLength;      // Length of dither matrix structure                      */
   /*    ULONG       fExt2IntSup;   // Gre Dither Flags for Ext to Int                        */
   /*    PVOID       pExt2IntDI;    // Used for GDM_USERDEF - pointer to users dither info    */
   /*    ULONG       fStretchSup;   // Gre Dither Flags for Stretch Support                   */
   /*    PVOID       pStretchDI;    // Used for GDM_USERDEF - pointer to users dither info    */
   /*    ULONG       fLog2PhysSup;  // Gre Dither Flags for Log to Phys clr mapping           */
   /*    PVOID       pLog2PhysDI;   // Used for GDM_USERDEF - pointer to users dither info    */
   /*    PFN         pfnDither;     // Used for GDM_USERDEF - pointer to users dither function*/
   /*******************************************************************************************/
   
   typedef struct _DITHERMATRIX   /* dthmtx */
   {
      ULONG       ulLength;
      ULONG       fExt2IntSup;
      PVOID       pExt2IntDI;
      ULONG       fStretchSup;
      PVOID       pStretchDI;
      ULONG       fLog2PhysSup;
      PVOID       pLog2PhysDI;
      PFN         pfnDither;
   } DITHERMATRIX;
   typedef DITHERMATRIX *PDITHERMATRIX;  /* pdthmtx */
   
   //
   // fExt2IntSup/fLog2IntSup
   //
   #define  GDM_NO_DITHER               0x0000   // No dithering in system
   #define  GDM_USERDEF_DITHER          0x0001   // Users supplies own Dithering routines
   #define  GDM_MATRIX_DITHER           0x0002   // Use System ordered dithering
   #define  GDM_ERRORDIF_DITHER         0x0004   // Use System error diffusion dithering
   #define  GDM_DITHER_BEGIN            0x0008   // Use System Floyd-Steinberg dithering
   #define  GDM_DITHER_END              0x0010   // Use System error propigation dithering
   #define  GDM_COLOR_CONVERT           0x0020   // Use device's clr mapping functions
   
   typedef struct _COLORCONVERTINFO             /* cci  */
   {
      ULONG              ulLength;
      ULONG              ulType;
      ULONG              ulOptions;
      ULONG (* APIENTRY pfnRGB2ToPhyIndex) () ;
      ULONG (* APIENTRY pfnPhyIndexToRGB2) () ;
      ULONG (* APIENTRY pfnConvert)        () ;
   } COLORCONVERTINFO;
   typedef COLORCONVERTINFO *PCOLORCONVERTINFO; /* pcci */
   
   ULONG (* APIENTRY pfnRGB2ToPhyIndex) (ULONG ulClr) ;
   ULONG (* APIENTRY pfnPhyIndexToRGB2) (ULONG ulClr) ;
   ULONG (* APIENTRY pfnConvert)        (PIMAGEINFO pimginfo);
   
   // pfnDither(PDITHERINFO pdi, PIMAGEINFO pii, ULONG ulFlags);
   
   #define  GDM_EXT2INT_DITHER   0x01
   #define  GDM_STRETCH_DITHER   0x02
   #define  GDM_LOG2PHY_DITHER   0x04
   
   /*******************************************************************************************/
   /*    ULONG       ulLength;      // length of structure                 - 88               */
   /*    ULONG       ulType;        // type of dither info structure       - GDM_MATRIX_DITHER*/
   /*    ULONG       fOptions;      // dither info options - DI_MONOINVERT - 00               */
   /*    ULONG       ulIntensity;   // RGB Gama Correction Value           - 00               */
   /*    BYTE        bRedWt;        // weight of primary color red         - 25               */
   /*    BYTE        bGreenWt;      // weight of primary color green       - 60               */
   /*    BYTE        bBlueWt;       // weight of primary color blue        - 15               */
   /*    BYTE        bPad;          // 4 byte align                        - 00               */
   /*    SIZEL       szMatrix;      // halftone pattern size               - 8                */
   /*    BYTE        bHalftone[];   // array of halftone patterns          - see 32gdata.c    */
   /*******************************************************************************************/
   
   typedef struct _MATRIXDITHERINFO      /* mtrxdi */
   {
      ULONG       ulLength;
      ULONG       ulType;
      ULONG       fOptions;
      ULONG       ulIntensity;
      BYTE        bRedWt;
      BYTE        bGreenWt;
      BYTE        bBlueWt;
      BYTE        bPad;
      SIZEL       szMatrix;
      BYTE        bHalftone[1];
   } MATRIXDITHERINFO;
   typedef MATRIXDITHERINFO *PMDI;  /* pmtrxdi */
   
   #define STUCKI_DIF 0x01
   #define JJN_DIF    0x02
   #define RND_DIF    0x04
   #define USER_DIF   0x08
   
   typedef struct _DIFFUSIONDITHERINFO      /* difdi */
   {
      ULONG       ulLength;
      ULONG       ulType;        // Stucki, J,J & N, RANDOM, User defined
      ULONG       fOptions;
      ULONG       ulIntensity;
      BYTE        bRedWt;
      BYTE        bGreenWt;
      BYTE        bBlueWt;
      BYTE        bPad;
      PBYTE       pBuffer;
      SIZEL       szFilter;
      BYTE        bFilterArry[1];
      
   } DIFFUSIONDITHERINFO;
   typedef DIFFUSIONDITHERINFO *PDDI;  /* pdifdi */
   
   typedef struct _DEVICESURFACE          /* devsur */
   {
      ULONG         ulLength;
      BMAPINFO      SurfaceBmapInfo;
      ULONG         ulDSFlgs;
      ULONG         ulStyleRatio;
      BMAPINFO      abmapinfoDefPattern[DEFAULT_PATTERNS_NUMBER];
      PVOID         pHWPalette;
      DITHERMATRIX  DitherMatrix;
      PFN           pfnDevLockDC;
      PFN           pfnDevUnLockDC;
      PFN           pfnBitBlt;
      PFN           pfnLine;
      #ifdef DEV_FNT_SUP
         PFN           pfnTextBlt;
      #else
         PFN           pfnReserved;
      #endif
      PFN           pfnLockPhysDev;
      PFN           pfnUnLockPhysDev;
      ULONG         hddc;
      //ULONG       ulReserved[4];
      ULONG         ulReserved[2]; //ulReserved[0]: used to pass pddc to drivers.
      ULONG         ulDS2Flgs;     // DS flags too
      PFN           pfnPolygon;
      ULONG         ulCapsCnt;
      ULONG         DevCaps[CAPS_MAX_CAPS + 1];
   } DEVICESURFACE;
   typedef DEVICESURFACE *PDEVICESURFACE; /* pdevsur */
   
   APIRET APIENTRY GreSetDitherInfo(HDC hdc,
                                    ULONG ulDitherType,
                                    PVOID pDitherInfo);
   APIRET APIENTRY SetDeviceSurface(HDC hdc,
                                    PDEVICESURFACE pds);
   
   // Surface definition flags
   // for ulDSFlgs
   #define DS_MEMORY                      0x00000001  // memory DS for a memory DC.
   #define DS_ROTATION                    0x00000002  // 90 degree rotation.
   
   #define DS_HORIZONTAL_SCAN             0x00000004  // horizontal scan DS.
   #define DS_VERTICAL_SCAN               0x00000008  // vertical scan DS.
   // DS must be TOPBOTTOM.
   #define DS_BYTEALIGN                   0x00000010  // DS scan line alignment.
   #define DS_WORDALIGN                   0x00000020  // DS scan line alignment
   #define DS_DWORDALIGN                  0x00000040  // DS scan line alignment
   
   #define DS_KEEP_EXTFORMAT              0x00000100  // bitmaps are created
   // in the external format.
   #define DS_MONO_INVERT                 0x00000200  // mono ROPs are inverted.
   #define DS_EXTERNAL_BITMAP_SELECTED    0x00000400  // Reserved.
   #define DS_NOTIFY_LAST_SDBITBLT        0x00000800  //
   #define DS_QUEUED_RAW_DEFAULT          0x00001000  //
   
   // DS_DOWNLOAD_FONTS
   // DS_FONT_CLIP_SUPPORT
   // DS_DEVICE_FONTS
   // SHOULD match GC_* in gradd.h
   #define DS_DOWNLOAD_FONTS              0x00002000  // Device Supports Downloaded Fonts
   #define DS_FONT_CLIP_SUPPORT           0x00004000  // Device will clip fonts
   #define DS_DEVICE_FONTS                0x00008000  // Device has Hardware Fonts
   
   #define DS_BOTTOMTOP                   0x00010000  // bottom to top is Y positive.
   #define DS_TOPBOTTOM                   0x00020000  // top to bottom is Y positive.
   
   // textblt clipping option
   #define DS_CLIP_100_RULE               0x00000000  // draw character if 100% is in clip rect
   #define DS_CLIP_66_RULE                0x00040000  // draw character if 2/3 is in clip rect
   #define DS_CLIP_PRECISE_RULE           0x00080000  // precise clipping
   
   // rows along Y and scans along X are default.
   // row positive and scan positive are default.
   #define DS_BITBLT_XROW_YSCAN           0x00100000  // rows along X and scans are along Y.
   #define DS_BITBLT_ROW_NEGATIVE         0x00200000  // next row = this row - bytes per row.
   #define DS_BITBLT_SCAN_NEGATIVE        0x00400000  // next pel = this pel - bytes per pel.
   
   // SHOULD match GC_SIMPLE_LINES in gradd.h
   #ifdef DS_SIMPLE_LINES
      #undef DS_SIMPLE_LINES
   #endif
   #define DS_SIMPLE_LINES                0x00800000  // device handles LINEINFO2.
   
   #define DS_NO_Y_FLIP                   0x01000000  // DS and DC both are either bot->top or top->bot.
   
   // SHOULD match GC_SRC_CONVERT in gradd.h
   #define DS_SRC_CONVERT                 0x02000000  // device can handle src convert on bitblts. see below
   
   // SHOULD match GC_POLYGON_FILL in gradd.h
   #define DS_POLYGON_FILL                0x08000000  // device can handle convex polygon fills.
   #define DS_BITBLT_RESERVED             0x04000000  // reserved.
   
   #define DS_COLOR_RGB2                  0x00000000  // color is in RGB2             (default)
   #define DS_COLOR_RGB                   0x10000000  // color is in RGB
   
   #define DS_COLOR_24BIT_BGR             0x00000000  // 1st byte=blue,               (default)
   // 2nd byte=green 3rd byte=red
   #define DS_COLOR_24BIT_RGB             0x10000000  // 1st byte=red, 2nd byte=green
   // 3rd byte=blue
   
   #define DS_COLOR_16BIT_565_BGR         0x00000000  // blue - 5 bits,               (default)
   // green - 6 bits, red - 5 bits
   // ---------------------------------
   // |b|b|b|b|b|g|g|g|g|g|g|r|r|r|r|r|
   // ---------------------------------
   #define DS_COLOR_16BIT_565_RGB         0x10000000  // red - 5 bits,
   // green - 6 bits, blue - 5 bits
   // ---------------------------------
   // |r|r|r|r|r|g|g|g|g|g|g|b|b|b|b|b|
   // ---------------------------------
   #define DS_COLOR_16BIT_1555_BGR        0x20000000  // overlay - 1, red - 5 bits,
   // green - 5 bits, blue - 5 bits
   // ---------------------------------
   // |o|b|b|b|b|b|g|g|g|g|g|r|r|r|r|r|
   // ---------------------------------
   #define DS_COLOR_16BIT_1555_RGB        0x40000000  // overlay - 1, red - 5 bits,
   // green - 5 bits, blue - 5 bits
   // ---------------------------------
   // |o|r|r|r|r|r|g|g|g|g|g|b|b|b|b|b|
   // ---------------------------------
   
   #define DS_COLOR_32BIT_BGR             0x00000000  // 1st byte=blue, 2nd byte=green  (default)
   // 3rd byte=red,  4th byte = not used.
   #define DS_COLOR_32BIT_RGB             0x10000000  // 1st byte=red,  2nd byte=green
   // 3rd byte=blue  4th byte = not used.
   
   // Surface definition flags
   // for ulDS2Flgs
   //
   // SHOULD match GC_* in gradd.h
   #define DS2_SRC_STRETCH                0x00000008  // device handles stretchblts
   #define DS2_POLYGON_SIZE_ANY           0x00000010  // device can handle polygon(concave or convex)
   // of any size.
   // DS_POLYGON_FILL must be set
   #define DS2_CLIP                       0x00000020  // device can handle single clip rect of
   // polygons for pfnPolygon and
   // source bitmap for pfnBitblt with
   // DS2_SRC_STRETCH set.
   // see DS2_CLP_COMPLEX
   #define DS2_CLIP_COMPLEX               0x00000040  // device can handle clipping with
   // more than one clip rect.
   // DS2_CLIP must be set.
   
   ///
   /// DS_SRC_CONVERT
   ///
   /// send Device Independent Bitmaps(DIB) without converting to device internal format.
   /// Gradd will convert during blts or call softdraw to convert.
   /// This conversion involves converting DIB to device internal format and
   /// leaving transparent pixel alone or applying transparent pixel color to
   /// transparent pixels. (see BF_TRANS_PIXEL_COLOR)
   /// If not set, GRE will convert DIB in a temporary buffer.
   /// Gradd should set this based on performance.
   /// set this flag based DS_SRC_CONVERT in CapsInfo.ulFCFlags.
   
   #ifndef FONTDEFFONT1
      #include <pmfont.h>
   #endif
   
   #ifndef DDI_INCLUDED
      #include <ddi.h>
   #endif
   
   //----------------------------------------------------------------------
   // DevFontMetrics
   //
   //    Structure used to inform GRE about device fonts
   //----------------------------------------------------------------------
   
   typedef struct _DEVFONTMETRICS                        /* dfm */
   {
      ULONG                  flOptions;
      PFOCAFONT              pFoca;
      PVOID                  pUniPanose;
      char                   achFileName[260];
      PSZ                    pszFullFamilyName;
      PSZ                    pszFullFaceName;
      char                   achGlyphListName[16];
      PFN                    pfnDevQueryCodePage;
      LONG                   lDevMatch;
   } DEVFONTMETRICS;
   typedef DEVFONTMETRICS *PDEVFONTMETRICS;
   
   // options for flOptions of DEVFONTMETRICS
   
   #define DEVFONT_DEFAULT   0x00010000  /* default device font */
   #define DEVFONT_EXTENSION 0x00020000  /* extended structure definition */
   
   //------------------------------------------------------------------STJ-
   // DevFontMetrics2
   //
   //    Structure used to inform GRE about device fonts, with extension
   //----------------------------------------------------------------------
   
   typedef struct _DEVFONTMETRICS2                       /* dfm2 */
   {
      ULONG                  flOptions;
      PFOCAFONT              pFoca;
      PVOID                  pUniPanose;
      char                   achFileName[260];
      PSZ                    pszFullFamilyName;
      PSZ                    pszFullFaceName;
      char                   achGlyphListName[16];
      PFN                    pfnDevQueryCodePage;
      LONG                   lDevMatch;
      
      // Additional fields if DEVFONT_EXTENSION is set
      
      ULONG                  ulSize;
      PSZ                    pszBackupFont;
      ULONG                  aulDeviceSpecific[4];
   } DEVFONTMETRICS2;
   typedef DEVFONTMETRICS2 *PDEVFONTMETRICS2;
   
   //------------------------------------------------------------------STJ-
   
   // Correction of field name spelling
   /* XLATOFF */
   #define pUniPanhose pUniPanose
   /* XLATON */
   
   typedef struct _FONTINFO       /* fntinfo */
   {
      DEVFONTINFO            dfi;
      FOCAFONT               foca;
   } FONTINFO;
   typedef FONTINFO *PFONTINFO;
   
   typedef struct _PHYSBND        /* physbnd */
   {
      ULONG  ulColor;            /* foreground color physical index         */
      ULONG  ulBackColor;        /* background color physical index         */
      ULONG  ulrgb2LogColor;     /* foreground logical color rgb, see below */
      ULONG  ulrgb2LogBackColor; /* background logical color rgb, see below */
      ULONG  ulMix;              /* forground ROP                           */
      ULONG  ulBackMix;          /* background ROP                          */
   } PHYSBND;
   typedef PHYSBND *PPHYSBND;
   
   #define COLOR_LOG_TO_PHY_MAPPED  0x80
   
   typedef struct _STATEINFO2         /* stateinfo2 */
   {
      POINTL   ptlOrigin;             /* long version of DC origin                       */
      
      XFORM    xformWD;               /* WORLD to DEVICE xform                           */
      ULONG    xform_flags;           /* the flags from NotifyXformChange                */
      
      /* Settable defaults information. */
      
      PVOID    pDAttr;                /* defaults if they've changed                     */
      
      ULONG    ulCodePage;            /* current code page.                              */
      
      POINTL   ptlCurPosWorld;        /* current postion in world coordinates.           */
      POINTL   ptlCurPosScreen;       /* current postion in screen coordinates.          */
      ULONG    ulLineStyle;           /* line style                                      */
      /* ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ */
      /* ³    high word   ³  5 bits  ³ 3 bits ³ 8 bits ³ */
      /* ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ */
      /* ³ first/last pel ³ not used ³ mask   ³ error  ³ */
      
   } STATEINFO2;
   typedef STATEINFO2 *PSTATEINFO2;   /* pstateinfo2 */
   
   typedef struct _STATEINFO1    /*stateinfo1 */
   {
      
      LONG     hddc;
      PSTATEINFO2     pStateInfo2;
      
      /* RDC without the DMATRIX's   */
      /* *** Arcs Component Data *** */
      
      ARCPARAMS     arc_parameters;
      LONG          norm;
      LONG          direction;
      
      /* Attribute Bundles */
      
      DLINEBUNDLE   line_bundle;
      DAREABUNDLE   pat_bundle;
      DCHARBUNDLE   char_bundle;
      DIMAGEBUNDLE  image_bundle;
      DMARKERBUNDLE marker_bundle;
      
      LONG          line_default;     /* line defaults mask                     */
      LONG          char_default;     /* char defaults mask                     */
      LONG          marker_default;   /* marker defaults mask                   */
      LONG          pat_default;      /* pat  defaults mask                     */
      LONG          image_default;    /* image defaults mask                    */
      
      ULONG         ulOldDriver;      /* Flags for old drivers, see 32const.h   */
      /* End RDC                                */
      
      ULONG    flPattern ;            /* flag                                   */
      /* PATTERN_SOLID                          */
      /* PATTERN_DEFAULT_SOLID                  */
      /* PATTERN_BITS_ALLOCATED                 */
      /* PATTERN_REF_POINT_SCREEN_DIRTY         */
      
      BMAPINFO bmapinfoPattern;       /* currently selected pattern bitmap info.*/
      POINTL   ptlPatRefPointWorld;   /* pattern origin in world coordinates.   */
      POINTL   ptlPatRefPointScreen;  /* pattern origin in screen coordinates.  */
      
      ULONG    ulLineType;            /* line type.                             */
      
      PHYSBND  physChar;
      PHYSBND  physLine;
      PHYSBND  physMarker;
      PHYSBND  physImage;
      PHYSBND  physArea;
      
   } STATEINFO1;
   typedef STATEINFO1 *PSTATEINFO1;   /* pstateinfo1 */
   
#endif /* INCL_VMANDDI */

#include <pmddim.h>

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
