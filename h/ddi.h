/*****************************************************************************
 *
 * SOURCE FILE NAME = DDI.H
 *
 * DESCRIPTIVE NAME = Video and Printer DDI Structures and defines
 *
 * Copyright : COPYRIGHT IBM CORPORATION, 1993
 *             LICENSED MATERIAL - PROGRAM PROPERTY OF IBM
 *             REFER TO COPYRIGHT INSTRUCTION FORM#G120-2083
 *             RESTRICTED MATERIALS OF IBM
 *             IBM CONFIDENTIAL
 *
 * ==========================================================================
 *
 * The folowing symbols are used in this file for conditional sections.
 *
 *   #define:                To include:
 *
 *   INCL_VMANDDI            Include all VMI interface definitions
 *   INCL_VMANWIN            Include VMAN to VDD interface definitions
 *   INCL_VMANHELP           Include VMAN Helper definitions and prototypes
 *
 * ==========================================================================
 *
 * VERSION = V2.1
 *
 * DESCRIPTION
 *      This file contains all of the defines and
 *      data structures that are shared between the
 *      video manager and the translation layers.
 *
 * FUNCTIONS
 *
 * NOTES
 *
 * STRUCTURES
 *
 * EXTERNAL REFERENCES
 *
 * EXTERNAL FUNCTIONS
 *
 * CHANGE ACTIVITY =
 *  DATE      FLAG        APAR   CHANGE DESCRIPTION
 *  --------  ----------  -----  --------------------------------------
 *  mm/dd/yy  @Vr.mpppxx  xxxxx  xxxxxxx
 *
 ****************************************************************************/

/* NOINC */
#ifndef DDI_INCLUDED
/* INC */

  #define DDI_INCLUDED

  /**  Possible return codes from the individual DDI's
  */
  #define RC_SUCCESS                   0
  #define RC_SIMULATE                  1
  #define RC_UNSUPPORTED               2
  #define RC_ERROR                     3
  #define RC_ERROR_IN_BACKGROUND       4
  #define RC_ERROR_NO_HANDLE           5
  #define RC_ERROR_RESOURCE_NOT_FOUND  6
  #define RC_ERROR_INVALID_PARAMETER   7
  #define RC_ERROR_OUT_OF_MEMORY       8
  #define RC_DISABLED                  9
  #define RC_ENABLED                   10


  /*
  **  Defines and structures for the GHI_CMD_BITBLT DDI
  */
  typedef struct _BMAPINFO {        /* bmapinfo */
     ULONG  ulLength;
     ULONG  ulType;
     ULONG  ulWidth;
     ULONG  ulHeight;
     ULONG  ulBpp;
     ULONG  ulBytesPerLine;
     PBYTE  pBits;
  } BMAPINFO;
  typedef BMAPINFO  *PBMAPINFO;

  typedef struct _BLTRECT {         /* bltrect */
     ULONG  ulXOrg;
     ULONG  ulYOrg;
     ULONG  ulXExt;
     ULONG  ulYExt;
  } BLTRECT;
  typedef BLTRECT  *PBLTRECT;

  /*
  **  Defines for the ulType field of the BMAPINFO data structure
  */
  #define BMAP_VRAM            0x00000000
  #define BMAP_MEMORY          0x00000001
  #define BMAP_BANKED          0x00000010
  #define BMAP_ALPHA_CHANNEL   0x00000020   // see below(Reserved, not used now)
  #define BMAP_ALPHA_PREMUL    0x00000040   // see below(Reserved, not used now)
  #define BMAP_ALPHA_RASTER    0x00000080   // see below(Reserved, not used now)
  #define BMAP_16BPP_1555      0x00000100
  #define BMAP_VERTICAL_SCAN   0x10000000
  #define BMAP_FOCAFONT        0x20000000

  // BMAP_ALPHA_CHANNEL - valid only for 4, 8 and 32 bpp
  //   This flag indicates that the high byte of pBits for 32BPP or
  //   the high byte of pulColors for 8/4 bpp contains alpha value.
  //   Otherwise, that byte is ignored.
  //
  // BMAP_ALPHA_PREMUL - BMAP_ALPHA_CHANNEL must be set.
  //   This flag indicates that the color value is premultiplied by the alpha.
  //
  // BMAP_ALPHA_RASTER - valid only for 8 bpp.
  //   This flag indicates that the 8bit value is alpha.


  /* Bank info passed to SD from GRE2VMAN       */
  /* Passed in Overloaded 2nd function param    */
  #define TYPE_GRDBNKINFO     0x00000001
  #define TYPE_CMYKINFO       0x00000002

  typedef struct _GRDBNKINFO {       /* gbi */
     ULONG ulLength;                 /* denotes length of ver. x                   */
     ULONG ulType;                   /* 2nd parameter structure type               */
     ULONG ulAperture;               /* Aperture Size for banked devices           */
     ULONG ulGID;                    /* Graphics Adapter ID used by VMAN           */
     ULONG ulCurrBank;               /* Current bank (to avoid unnecessary change) */
     ULONG ulReserved;               /* System Reserved ulong                      */
     PFN   pfnBankSwitch;            /* Function Pointer to Bank Switching Routine */
     PVOID pvReserved;               /* System Reserved Pointer                    */
  }  GRDBNKINFO;
  typedef GRDBNKINFO *PGRDBNKINFO;

  #define CMYK_DEVICE          0x00000002
  #define CMY_ONLY             0x00000004
  #define CMYK_INTERLEAVE      0x00000008

  typedef struct _BITBLTINFO {      /* bitbltinfo */
     ULONG        ulLength;
     ULONG        ulBltFlags;
     ULONG        cBlits;
     ULONG        ulROP;
     ULONG        ulMonoBackROP;
     ULONG        ulSrcFGColor;
     ULONG        ulSrcBGColor;
     ULONG        ulPatFGColor;
     ULONG        ulPatBGColor;
     PBYTE        abColors;
     PBMAPINFO    pSrcBmapInfo;
     PBMAPINFO    pDstBmapInfo;
     PBMAPINFO    pPatBmapInfo;
     PPOINTL      aptlSrcOrg;
     PPOINTL      aptlPatOrg;
     PBLTRECT     abrDst;
     PRECTL       prclSrcBounds;    /* incl/excl bounds. */
     PRECTL       prclDstBounds;    /* incl/excl bounds. */
  } BITBLTINFO;
  typedef BITBLTINFO *PBITBLTINFO;


  // Reserved, not used now
  //
  // Alpha Compositing Rules
  //
  // Alpha compositing rules for combining source and destination
  // pixels to achieve blending and transparency effects
  // with graphics and images.
  //
  // The rules are described in
  // T. Porter and T. Duff, "Compositing Digital Images",
  // SIGGRAPH 84, 253-259.
  //
  // If any input does not have an alpha channel,
  // an alpha value of 1.0 (completely opaque) is assumed for all pixels.
  //
  // A constant alpha value can also be specified to be
  // multiplied with the alpha value of the source pixels.
  //
  // The following abbreviations are used in the description of the rules:
  // Cs = one of the Color components of the Source Pixel.
  // Cd = one of the Color components of the Destination Pixel.
  // As = Alpha component of the Source Pixel.
  // Ad = Alpha component of the Destination Pixel.
  // Fs = fraction of the Source Pixel which contributes to the output.
  // Fd = fraction of the input Destination Pixel which contributes to the output.
  //
  // The color and alpha components produced by the compositing operation are
  // calculated as follows:
  //
  // 	Cd = Cs*Fs + Cd*Fd
  // 	Ad = As*Fs + Ad*Fd
  //
  // where Fs and Fd are specified by each rule.  The above equations assume
  // that both source and destination pixels have the color components
  // premultiplied by the alpha component.  Similarly, the equations expressed
  // in the definitions of compositing rules below assume premultiplied alpha.
  //
  #define ALPHA_RULE_DST            0x00000000 // 0 == Leave alone
  #define ALPHA_RULE_CLEAR          0x00000001 // java.awt.AlphaComposite.CLEAR
  #define ALPHA_RULE_SRC            0x00000002 // java.awt.AlphaComposite.SRC
  #define ALPHA_RULE_SRC_OVER_DST   0x00000003 // java.awt.AlphaComposite.SRC_OVER
  #define ALPHA_RULE_DST_OVER_SRC   0x00000004 // java.awt.AlphaComposite.DST_OVER
  #define ALPHA_RULE_SRC_IN_DST     0x00000005 // java.awt.AlphaComposite.SRC_IN
  #define ALPHA_RULE_DST_IN_SRC     0x00000006 // java.awt.AlphaComposite.DST_IN
  #define ALPHA_RULE_SRC_OUT_DST    0x00000007 // java.awt.AlphaComposite.SRC_OUT
  #define ALPHA_RULE_DST_OUT_SRC    0x00000008 // java.awt.AlphaComposite.SRC_OVER
  #define ALPHA_RULE_SRC_ATOP_DST   0x00000009
  #define ALPHA_RULE_DST_ATOP_SRC   0x0000000A
  #define ALPHA_RULE_SRC_XOR_DST    0x0000000B

  // Reserved, not used now
  //=====================================================================
  // AlphaBitBltInfo structure
  //
  //   14 APR 98 - STJONES
  //
  // The length of this structure will differentiate it from the old
  // BITBLTINFO structure.  As much as possible, fields with similar
  // functions will remain at the same offset relative to the start of
  // the structure.
  //=====================================================================
  typedef struct _ALPHABITBLTINFO         /* abbi */
  {
     ULONG        ulLength;             // unchanged
     ULONG        ulBltFlags;           // unchanged
     ULONG        cBlits;               // unchanged
     ULONG        ulRule;               // Rule vs ROP

     ULONG        ulXORColor;           // redefined
     ULONG        ulFGColor;            // similar
     ULONG        ulBGColor;            // similar
     ULONG        ulGradColor1;         // redefined
     ULONG        ulGradColor2;         // redefined

     PBYTE        abColors;             // unchanged
     PBMAPINFO    pSrcBmapInfo;         // unchanged
     PBMAPINFO    pDstBmapInfo;         // unchanged

     PBMAPINFO    pPatBmapInfo;         // unchanged (gradient info?)
     PPOINTL      aptlSrcOrg;           // unchanged
     PPOINTL      aptlPatOrg;           // unchanged (gradient info?)
     PBLTRECT     abrDst;               // unchanged

     ULONG        ulExtraAlpha;         // redefined
     PULONG       pulDstClrInvLUT;      // redefined

  } ALPHABITBLTINFO;
  typedef ALPHABITBLTINFO *PALPHABITBLTINFO;

  /*
  ** Defines for ulBltFlags field of the BITBLTINFO data structure
  **
  ** The default state of the flags are as follows:
  **
  ** DIRECTION      = Left to right - top to bottom
  ** TRANSPARENCY   = NONE
  ** PATTERN STYLE  = UNKNOWN
  ** BACKGROUND ROP = INVALID
  */

  #define BF_DEFAULT_STATE          0x00000000  // blt dir X and Y positive.

  #define BF_DIR_X_NEGATIVE         0x00000001
  #define BF_DIR_RIGHT_LEFT         BF_DIR_X_NEGATIVE

  #define BF_DIR_Y_NEGATIVE         0x00000002
  #define BF_DIR_BOTTOM_TOP         BF_DIR_Y_NEGATIVE

  #define BF_ROP_INCL_SRC           0x00000004
  #define BF_ROP_INCL_PAT           0x00000008

  #define BF_SRC_TRANSPARENT        0x00000010
  #define BF_DST_TRANSPARENT        0x00000020
  #define BF_PAT_TRANSPARENT        0x00000040

  #define BF_PAT_SOLID              0x00000080
  #define BF_PAT_HOLLOW             0x00000100

  #define BF_APPLY_BACK_ROP         0x00000200

  #define BF_SRC_MONOINVERT         0x00000400
  #define BF_PAT_MONOINVERT         0x00000800
  #define BF_SRC_CONVERT            0x00001000
  #define BF_SRC_BITS_EXTERNAL      BF_SRC_CONVERT
  #define BF_LAST_BLT               0x00002000
  #define BF_SRC_Y_FLIP             0x00004000
  #define BF_SRC_X_FLIP             0x00008000
  #define BF_SRC_VERTICAL_SCAN      0x00010000
  #define BF_DST_MONOINVERT         0x00020000

  #define BF_ALPHA_BLENDING         0x00040000  // see below(Reserved, not used now)
  #define BF_ALPHA_RASTER           0x00080000  // see below(Reserved, not used now)
  #define BF_ALPHA_ANTIALIAS        0x00100000  // see below(Reserved, not used now)
  #define BF_STROKE_FILL            0x00200000
  #define BF_SRC_STRETCH            0x00400000  // see below
  #define BF_SRC_STRETCH_AND        0x00800000  // see below
  #define BF_SRC_STRETCH_IGNORE     0x01000000  // see below
  #define BF_CLIP                   0x02000000  // see below

  //---------------------------------------------------------------------
  // The BF_ALPHA_BLENDING indicates that this is an AlphaBlending RULE
  // rather than a BitBlt ROP.  The ulExtraAlpha field is always valid
  // for an AlphaBlending RULE.
  //---------------------------------------------------------------------
  //
  // BF_ALPHA_RASTER
  //   Contains alpha raster in SrcBmapInfo (but BF_ROP_INCL_SRC shuold not be set).
  //   SrcBmap must be 8bpp.
  //   FG clr and FG alpha will be mixed with destination using alpha
  //
  // BF_ALPHA_ANTIALIASED
  //    valid if BF_ALPHA_RASTER is also set.
  //    if set alpha values are in the range 0(transparent) - 255(opaque).
  //    if not set, alpha vaues are either transparent (alpha=0) otherwise opaque.
  //
  // Apply ulBGColor to transparent pixel in the source bitmap.
  // A pixel is transparent if pixel's alpha is 0.
  // BF_SRC_CONVERT or BF_ALPHA_BLENDING  must be set.
  #define BF_TRANS_PIXEL_COLOR       BF_APPLY_BACK_ROP    // Reserved, not used now

  // BF_SRC_STRETCH
  //   src is stretched or compressed from source to target bltrect.
  //   the source and target bltrects are passed in
  //   (PBLTRECT)aptlSrcOrg.
  //
  //   if src is compressed, then any eliminated rows and colums are ORed,
  //   unless either BF_SRC_STRETCH_AND or BF_SRC_STRETCH_IGNORE is set
  //
  // BF_SRC_STRETCH_AND
  //   if src is compressed, then any eliminated rows and colums are ANDed.
  //
  // BF_SRC_STRETCH_IGNORE
  //   if src is compressed, then any eliminated rows and colums are ignored.
  //

  // BF_CLIP              
  //   clip polygons for pfnPolygon and
  //   source bitmap for pfnBitblt with BF_SRC_STRETCH.
  //   abrDst will contain cBlits clip rects. These clip rects are
  //   Y ordered and X ordered in device surface coordinate system.

  /*
  **  Defines and structures for the GHI_CMD_LINE DDI
  */
  typedef struct _LINEPACK {           /* linepack */
     ULONG            ulStyleStep;
     ULONG            ulStyleValue;
     ULONG            ulFlags;
     struct _LINEPACK * plpkNext;
     ULONG            ulAbsDeltaX;
     ULONG            ulAbsDeltaY;
     POINTL           ptlClipStart;
     POINTL           ptlClipEnd;
     POINTL           ptlStart;
     POINTL           ptlEnd;
     LONG             lClipStartError; /* Bresenham error at the clip start.*/
                                       /* not valid for horizontal or       */
                                       /* vertical lines.                   */
  } LINEPACK;    /* lpk */
  typedef LINEPACK  *PLINEPACK;   /* plpk */


  /*
  **  Defines for the ulFlags field of the LINEPACK/LINEINFO2 data structure
  */
  #define LINE_DO_FIRST_PEL      0x00000002
  #define LINE_DIR_Y_POSITIVE    0x00000004
  #define LINE_HORIZONTAL        0x00000008
  #define LINE_X_MAJOR           0x00000010
  #define LINE_DIR_X_POSITIVE    0x00000020
  #define LINE_VERTICAL          0x00001000
  #define LINE_STYLE_X_MAJOR     0x00002000
  #define LINE_DO_LAST_PEL       0x00004000
  #define LINE_MONO_INVERT       0x00008000  /* need to set for the first              */
                                             /* line only, if required.                */
  #define LINE_START_CLIP        0x00010000  /* first line in LINEINFO2 is clipped     */
  #define LINE_END_CLIP          0x00020000  /* last line in LINEINFO2 is clipped      */
  #define LINE_Y_FLIP            0x00040000  /* flip Y for all lines in LINEINFO2      */
  #define LINE_ALL_RADIAL        0x00080000  /* all lines in LINEINFO2 are vertical    */
                                             /* or horizontal or 45/135/225/315 degree */
  #define LINE_DISJOINT          0x00100000  /* disjoint lines                         */

  /*  Line drawing
  **
  **    The line starts from ptlStart and ends at ptlEnd(inclusive).
  **    The device can do the Bresenham either from ptlStart or
  **    from the ptlClipStart using the lClipStartError and
  **    will only set the pels from ptlClipStart to ptlClipEnd(inclusive).
  **
  **  ulAbsDeltaX :
  **    absolute (ptlStart.x - ptlEnd.x)
  **
  **  ulAbsDeltaY :
  **    absolute (ptlStart.y - ptlEnd.y)
  **
  **  lClipStartError:
  **
  **    This is the standard Bresenham error at the clipped start point.
  **    This is calculated from the initial error at the start point and
  **    the error increments for major step and diagonal step.
  **
  **    The initial error and the error increments are:
  **
  **    MAX           = maximum (ulAbsDeltaX, ulAbsDeltaY)
  **    MIN           = minimum (ulAbsDeltaX, ulAbsDeltaY)
  **    Major Inc.    = Increment to the error for
  **                    stepping along the major axis.
  **                  = 2 * MIN
  **    Diagonal Inc. = Increment to the error for
  **                    stepping along the major and minor axis.
  **                  = 2 * MIN - 2 * MAX
  **    Initial error = Error at the start point.
  **                  = 2 * MIN - MAX  if LINE_DIR_X_POSITIVE is on
  **                  = 2 * MIN - MAX - 1 if LINE_DIR_X_POSITIVE is off.
  **
  **   Horizontal and vertical lines.
  **      The line is drawn from the clipped start to clipped end.
  **      The lClipStartError will not be given.
  **
  **   First pel consideration.
  **      Set the the first pel at the ptlStart (not ptlClipStart) only
  **      if LINE_DO_FIRST_PEL is set and the first pel is not clipped out.
  **
  **   Last pel consideration.
  **      Set the the last pel at the ptlEnd (not ptlClipEnd) only
  **      if LINE_DO_LAST_PEL is set and the last pel is not clipped out.
  **
  **
  **   Styling.
  **      Lines are styled using the ulStyleMask, ulStyleStep and ulStyleValue.
  **
  **      ulStyleMask:
  **         A 32 bit style mask.
  **
  **      ulStyleStep:
  **         The value to be added to ulStyleValue
  **         on each pel stepped along the style major direction.
  **
  **      ulStyleValue:
  **         The style value at the current pel.
  **         it is composed of an error value and a mask position as :
  **
  **         ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
  **         ³    high word   ³  3 bits  ³  5 bits  ³   8 bits    ³
  **         ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
  **         ³    not used    ³ not used ³ mask pos ³ error value ³
  **
  **      Error Value :
  **         Error value at the current pel.
  **
  **      Mask Position :
  **         Bit position of the ulStyleMask.
  **         if this bit is on,
  **            set the current pel to the ulFGColor through usForeROP,
  **         otherwise
  **            set the current pel to the ulBGColor through usBackRop
  **
  */
  typedef struct _LINEINFO {        /* linfo */
     ULONG          ulLength;
     ULONG          ulType;
     ULONG          ulStyleMask;
     ULONG          cLines;
     ULONG          ulFGColor;
     ULONG          ulBGColor;
     USHORT         usForeROP;
     USHORT         usBackROP;
     PBMAPINFO      pDstBmapInfo;
     PLINEPACK      alpkLinePack;
     PRECTL         prclBounds;     /* incl/incl bounds. */
  } LINEINFO;      /* linfo */
  typedef  LINEINFO *PLINEINFO;     /* plinfo */

  typedef struct _LINEINFO2 {       /* linfo2 */
     ULONG          ulLength;       /* same as lineinfo */
     ULONG          ulType;         /* same as lineinfo */
     ULONG          ulStyleMask;    /* same as lineinfo */
     ULONG          cLines;         /* same as lineinfo */
     ULONG          ulFGColor;      /* same as lineinfo */
     ULONG          ulBGColor;      /* same as lineinfo */
     USHORT         usForeROP;      /* same as lineinfo */
     USHORT         usBackROP;      /* same as lineinfo */
     PBMAPINFO      pDstBmapInfo;   /* same as lineinfo */
     ULONG          ulFlags;        /* LINE_* see above */
     ULONG          ulXYStyleStep;  /* low byte of low word: x style, high byte of low word: y style */
     PULONG         pulStyleValue;  /* style value at start point */
     POINTL         ptlOrigin;      /* origin           */
     PPOINTL        pptlStart;      /* start point      */
     PPOINTL        pptlLines;      /* points array     */
     POINTL         ptlClipStart;   /* clipped start point if it is clipped */
     POINTL         ptlClipEnd;     /* clipped end point if it is clipped   */
     LONG           lClipStartError;/* Bresenham error at the clip start.   */
                                    /* for first line.                      */
                                    /* not valid for horizontal or          */
                                    /* vertical lines.                      */
     PRECTL         prclBounds;     /* incl/incl bounds.*/

  } LINEINFO2;      /* linfo2 */
  typedef  LINEINFO2 *PLINEINFO2;   /* plinfo2 */

  /*
  **  Defines for the ulType field of the LINEINFO data structure
  */
  #define LINE_SOLID       0x01
  #define LINE_INVISIBLE   0x02
  #define LINE_ALTERNATE   0x03


  /*
  ** Defines for 2 way Line ROPS
  */
                                    /* PATTERN = 0xC and DEST = 0xA */
  #define LR_ZERO               0   /* 0x0                          */
  #define LR_INVERTMERGEPAT     1   /* ~(LR_DEST | LR_PATTERN)      */
  #define LR_MASKINVERTPAT      2   /* (~LR_PATTERN) & LR_DEST      */
  #define LR_INVERTCOPYPAT      3   /* ~LR_PATTERN                  */
  #define LR_MASKPATINVERT      4   /* (~LR_DEST) & LR_PATTERN      */
  #define LR_INVERT             5   /* ~LR_DEST                     */
  #define LR_XORPAT             6   /* LR_DEST ^ LR_PATTERN         */
  #define LR_INVERTMASKPAT      7   /* ~(LR_DEST & LR_PATTERN)      */
  #define LR_MASKPAT            8   /* LR_DEST & LR_PATTERN         */
  #define LR_INVERTXORPAT       9   /* ~(LR_DEST ^ LR_PATTERN)      */
  #define LR_LEAVEALONE        10   /* LR_DEST                      */
  #define LR_MERGEINVERTPAT    11   /* (~LR_PATTERN) | LR_DEST      */
  #define LR_PATCOPY           12   /* LR_PATTERN                   */
  #define LR_MERGEPATINVERT    13   /* (~LR_DEST) | LR_PATTERN      */
  #define LR_MERGEPAT          14   /* LR_PATTERN | LR_DEST         */
  #define LR_ONE               15   /* 0x0F                         */

  #ifndef FLOAT
     typedef float  FLOAT;           /* f   */
     typedef FLOAT  *PFLOAT;         /* pf  */
  #endif
  #ifndef DOUBLE
     typedef double DOUBLE;          /* d   */
     typedef DOUBLE *PDOUBLE;        /* pd  */
  #endif
  //
  // VERTEXDATA
  //
  typedef struct _VERTEXDATA
  {
     USHORT usDataType;        // see below
     USHORT usStride;          // stride in bytes for next data
     union
     {
        PBYTE   pbData;        // signed byte  (8 bits)
        PSHORT  psData;        // signed short (16 bits)
        PLONG   plData;        // signed long  (32 bits)
        PFLOAT  pfData;        // float        (32 bits)
        PDOUBLE pdData;        // double       (64 bits)
     };
  } VERTEXDATA;
  typedef VERTEXDATA *PVERTEXDATA;

  //
  // usDataType for VERTEXDATA
  //
  #define VERTEXDATA_BYTE    0x01   // byte   (8 bits)
  #define VERTEXDATA_SHORT   0x02   // short  (16 bits)
  #define VERTEXDATA_LONG    0x03   // long   (32 bits)
  #define VERTEXDATA_FLOAT   0x04   // float  (32 bits)
  #define VERTEXDATA_DOUBLE  0x05   // double (64 bits)

  //
  // POLYGON2D
  //
  typedef struct _POLYGON2D
  {
     ULONG      cVertex;       // number of vertices
     ULONG      ulTessType;    // tessellation type, see below
     ULONG      ulVertexMask;  // see below
     VERTEXDATA vdx;           // vertex x coordinate data
     VERTEXDATA vdy;           // vertex y coordinate data
     VERTEXDATA vda;           // vertex color alpha data
     VERTEXDATA vdr;           // vertex color red data
     VERTEXDATA vdg;           // vertex color green data
     VERTEXDATA vdb;           // vertex color blue data
  } POLYGON2D;
  typedef POLYGON2D *PPOLYGON2D;

  //
  // POLYGON3D
  //
  typedef struct _POLYGON3D
  {
     ULONG      cVertex;       // number of vertices
     ULONG      ulTessType;    // tessellation type, see below
     ULONG      ulVertexMask;  // see below
     VERTEXDATA vdx;           // vertex x coordinate data
     VERTEXDATA vdy;           // vertex y coordinate data
     VERTEXDATA vdz;           // vertex z coordinate data
     VERTEXDATA vdw;           // vertex w data
     VERTEXDATA vda;           // vertex color alpha data
     VERTEXDATA vdr;           // vertex color red data
     VERTEXDATA vdg;           // vertex color green data
     VERTEXDATA vdb;           // vertex color blue data
     VERTEXDATA vddx;          // vertex normal vector dx data
     VERTEXDATA vddy;          // vertex normal vector dy data
     VERTEXDATA vddz;          // vertex normal vector dz data
     VERTEXDATA vddw;          // vertex normal vector dw data
     VERTEXDATA vdtx;          // vertex texture x coordinate data
     VERTEXDATA vdty;          // vertex texture y coordinate data
     VERTEXDATA vdtz;          // vertex texture z coordinate data
     VERTEXDATA vdtw;          // vertex texture w coordinate data
  } POLYGON3D;
  typedef POLYGON3D *PPOLYGON3D;

  //
  // ulTessType for  POLYGON2D and POLYGON3D
  //
  #define POLYGON_GENERAL        0x01 // convex or concave polygon of 
                                      // any cVertex sides.

  #define POLYGON_TRIANGLE       0x02 // series of triangles using vertices
                                      // v1,v2,v3 then
                                      // v4,v5,v6 and so on.
                                      // cVertex should be of minimum 3
                                      // if cVertex isn't an exact multiple of 3,
                                      // the final one or two vertices are ignored

  #define POLYGON_TRIANGLE_STRIP 0x03 // series of triangles using vertices
                                      // v1,v2,v3 then
                                      // v3,v2,v4(note the order), then
                                      // v3,v4,v5 and so on.
                                      // the ordering is to ensure that the
                                      // triangles are all drawn with
                                      // same orientation so that the strip
                                      // can correctly form part of a surface.
                                      // cVertex should be of minimum 3


  #define POLYGON_TRIANGLE_FAN   0x04 // first vertex (v1) is center of fan.
                                      // series of triangles using vertices
                                      // v1,v2,v3 then
                                      // v1,v3,v4 then
                                      // v1,v4,v5 and so on.
                                      // the ordering is to ensure that the
                                      // triangles are all drawn with
                                      // same orientation so that the fan
                                      // can correctly form part of a surface.
                                      // cVertex should be of minimum 3

  #define POLYGON_QUAD           0x05 // series of quadrilaterals using vertices
                                      // v1,v2,v3,v4 then
                                      // v5,v6,v7,v8 and so on.
                                      // cVertex should be of minimum 4
                                      // if cVertex isn't an exact multiple of 4,
                                      // the final one, two or three vertices are ignored
                                      // Note: Quads may be concave or bow-tie.

  #define POLYGON_QUAD_STRIP     0x06 // series of quadrilaterals using vertices
                                      // v1,v2,v4,v3 then
                                      // v3,v4,v6,v5 then
                                      // v5,v6,v8,v7 and so on.
                                      // the ordering is to ensure that the
                                      // quadrilaterals are all drawn with
                                      // same orientation so that the strip
                                      // can correctly form part of a surface
                                      // cVertex should be of minimum 4.
                                      // if cVertex isn't even, the final vertex is ignored.
                                      // Note: Quads may be concave or bow-tie.




  // ulVertexMask for POLYGON2D and POLYGON3D
  // VERTEXDATAs in POLYGON2D/3D contain valid data,
  // if ulVertexMask has the corresponding VERTEX_HAS_* bit set.
  //
  #define VERTEX_HAS_X     0x0001
  #define VERTEX_HAS_Y     0x0002
  #define VERTEX_HAS_Z     0x0004
  #define VERTEX_HAS_W     0x0008
  #define VERTEX_HAS_A     0x0010
  #define VERTEX_HAS_R     0x0020
  #define VERTEX_HAS_G     0x0040
  #define VERTEX_HAS_B     0x0080
  #define VERTEX_HAS_DX    0x0100
  #define VERTEX_HAS_DY    0x0200
  #define VERTEX_HAS_DZ    0x0400
  #define VERTEX_HAS_DW    0x0800
  #define VERTEX_HAS_TX    0x1000
  #define VERTEX_HAS_TY    0x2000
  #define VERTEX_HAS_TZ    0x4000
  #define VERTEX_HAS_TW    0x8000

  //
  // POLYSINFO
  //
  typedef struct _POLYSINFO
  {
     ULONG cPolys;                 // number of polys
     ULONG ulPolyType;             // POLYGON_2D or POLYGON_3D
     union
     {
        PPOLYGON2D abPolygon2d;    // array of polygon2ds
        PPOLYGON3D abPolygon3d;    // array of polygon3ds
     };
  } POLYSINFO;
  typedef POLYSINFO *PPOLYSINFO;

  //
  // ulPolyType for POLYSINFO
  //
  #define POLYGON_2D  0x01
  #define POLYGON_3D  0x02

  //
  // POLYGONINFO
  //
  typedef struct _POLYGONINFO
  {
     ULONG        ulLength;
     ULONG        ulBltFlags;
     ULONG        cBlits;          // number of bltrects in abrDst
     ULONG        ulROP;
     ULONG        ulMonoBackROP;
     ULONG        ulSrcFGColor;
     ULONG        ulSrcBGColor;
     ULONG        ulPatFGColor;
     ULONG        ulPatBGColor;
     PBYTE        abColors;
     PPOLYSINFO   pPolysInfo;      // polys info
     PBMAPINFO    pDstBmapInfo;
     PBMAPINFO    pPatBmapInfo;
     PPOINTL      pptlBounds;     // minimum and maximum points
     PPOINTL      aptlPatOrg;
     PBLTRECT     abrDst;
     PVOID        pvReserved1;
     PVOID        pvReserved2;
  } POLYGONINFO;
  typedef POLYGONINFO *PPOLYGONINFO;

  //
  // softdraw function pointer defintion
  //
  typedef APIRET (APIENTRY SDFN) (PVOID, PVOID);
  typedef SDFN *PSDFN;


  //
  // SDCALLBACK
  //
  typedef struct _SDCALLBACK     // softdraw call back for SDPOLYGON
  {
     PSDFN        psdfnCallBack;
     PVOID        pOutput;
  } SDCALLBACK;
  typedef SDCALLBACK *PSDCALLBACK;


  /*
  **  Defines and structures for the TextBlt
  */
  typedef struct _GLYPHINFO {                  /* glphyinfo */
          char      bAspace;
          char      bBspace;
          char      bCspace;
          char      bPad;
          BMAPINFO  bmapinfo;
  } GLYPHINFO;                                /* glyphinfo */
  typedef GLYPHINFO *PGLYPHINFO;              /* pglyphinfo */


  typedef struct _GLBTBL {                     /* glbtbl */
          PGLYPHINFO          pGlyphInfo[1];   // Up to 256 entries per table
  } GLBTBL;
  typedef GLBTBL *PGLBTBL;

  typedef struct _GHBTBL {                     /* ghbtbl */
          PGLBTBL             pglbTbl[1];      // Up to 256 entries per table
  } GHBTBL;
  typedef GHBTBL *PGHBTBL;

  // A subset of FontInfo ( pmddi.h ) for TextBlt
  typedef struct _DEVFONTINFO {                      /* dfi */
          ULONG                  ulFntCnt;           // Maximum glyphs contained in this font.
          ULONG                  fFontInfo;          // Flags
          ULONG                  ulEngTag;           // Renderer Tag - i.e. 'GRE'
          ULONG                  ulUniqueFntID;
          ULONG                  ulMaxHeight;
          ULONG                  ulMaxWidth;
          PGHBTBL                pghbTbl;
          ULONG                  ulHalfWidth;
          CHAR                   szGlyphlist[16];
          ULONG                  ulReserved1;
          ULONG                  ulReserved2;
  } DEVFONTINFO;
  typedef DEVFONTINFO *PDEVFONTINFO;

  #define  GRETAG     ('G' + ('R'<<8) + ('E'<<16) + ('_'<<24))    // "GRE_"
  #define  WINTAG     ('W' + ('I'<<8) + ('N'<<16) + ('_'<<24))    // "WIN_"

  #define  DFI_FIXED_FONT        0x00000001
  #define  DFI_DBCS_FONT         0x00000002

  typedef struct _TEXTBLTINFO {                /* tbi */
          ULONG               ulLength;
          ULONG               flOptions;
          ULONG               lGlyphCnt;
          PLONG               pGlyphIndices;
          ULONG               ulFGMix;
          ULONG               ulBGMix;
          ULONG               ulFGColor;
          ULONG               ulBGColor;
          PBMAPINFO           pDstBmapInfo;
          PDEVFONTINFO        pDevFntInfo;
          ULONG               ulClpCnt;        // Non Clipping Drivers Ignore
          PBLTRECT            abrClipRects;    // Non Clipping Drivers Ignore
          PPOINTL             aptlSrcOrg;      // Clipping Drivers Ignore
          PBLTRECT            abrDst;          // Clipping Drivers Ignore .ulXExt & ulYExt
  } TEXTBLTINFO;
  typedef TEXTBLTINFO *PTEXTBLTINFO;


  /*
  **  Defines and structures for the GHI_CMD_PALETTE
  */
  typedef struct _HWPALETTEINFO { /* hwpaletteinfo */
     ULONG  ulLength;
     ULONG  fFlags;               /* Get/Set palette                      */
     ULONG  ulStartIndex;         /*                                      */
     ULONG  ulNumEntries;         /*                                      */
     PRGB2  pRGBs;                /*                                      */
  } HWPALETTEINFO;
  typedef HWPALETTEINFO *PHWPALETTEINFO;

  #define PALETTE_GET             0x0001
  #define PALETTE_SET             0x0002

  /*
  **  Defines and structures for the GHI_CMD_MOVEPTR DDI
  */
  typedef struct _HWMOVEPTRIN {     /* hwmoveptrin */
     ULONG  ulLength;
     POINTL ptlPos;
  } HWMOVEPTRIN;
  typedef HWMOVEPTRIN *PHWMOVEPTRIN;


  /*
  **  Defines and structures for the GHI_CMD_SETPTR DDI
  */
  typedef struct _HWSETPTRIN {      /* hwsetptrin */
     ULONG  ulLength;
     PBYTE  pbANDMask;
     PBYTE  pbXORMask;
     PBYTE  pbBits;
     ULONG  ulBpp;
     ULONG  ulWidth;
     ULONG  ulHeight;
     POINTL ptlHotspot;
  } HWSETPTRIN;
  typedef HWSETPTRIN *PHWSETPTRIN;

  typedef struct _HWSETPTROUT {      /* hwsetptrout */
     ULONG ulLength;
     ULONG ulStatus;
  } HWSETPTROUT;
  typedef HWSETPTROUT *PHWSETPTROUT;

  /*
  ** Defines for the ulStatus field of the HWSETPTROUT structure
  */
  #define POINTER_VISIBLE       0x0001
  #define POINTER_COLOR         0x0002

  /*
  ** This flag is set by the GRADD if the pointer just set in
  ** via the GHI_CMD_SETPTR command is not a hardware sprite.
  ** VMAN uses this flag to determine whether it must remove
  ** the pointer on drawing operations that overlap the pointer.
  */
  #define POINTER_SOFTWARE      0x0004

  /*
  ** This flag is set by the GRADD if the pointer just set in
  ** via the GHI_CMD_SETPTR command does not require serialization
  ** with the normal drawing operations.  This flag will typically
  ** be set if GRADD is using a hardware sprite, since moving
  ** a sprite often only requires changing registers which do not
  ** effect the drawing engine. When this flag is set, pointer
  ** movement performance is drastically improved.
  */
  #define POINTER_FAST          0x0008

  /*
  **  Defines and structures for the GHI_CMD_SHOWPTR DDI
  */

  typedef struct _HWSHOWPTRIN {     /* hwshowptrin */
     ULONG ulLength;
     BOOL  fShow;
  } HWSHOWPTRIN;
  typedef HWSHOWPTRIN *PHWSHOWPTRIN;

  /*
  **  Defines and structures for the GHI_CMD_REQUESTHW command
  */
  /*
  ** Defines for the ulFlags field of HWREQIN
  */
  #define REQUEST_HW               0x1
  #define REQUEST_SEM_ONLY         0x2

  typedef struct _HWREQIN {        /* hwreqin */
     ULONG  ulLength;
     ULONG  ulFlags;
     ULONG  cScrChangeRects;
     PRECTL arectlScreen;
  } HWREQIN;
  typedef HWREQIN *PHWREQIN;


  /*
  **  Defines and structures for the GHI_CMD_BANK command
  */
  typedef struct _HWBANKIN {       /* hwbankin  */
     ULONG  ulLength;
     ULONG  ulFlags;               /* Get/Set bank */
     ULONG  ulBank;                /* Bank number  */
  } HWBANKIN;
  typedef HWBANKIN *PHWBANKIN;

  /*
  **  Defines for the ulFlags field of the HWBANKIN data structure.
  */
  #define BANK_SET                     1
  #define BANK_GET                     2

  typedef struct _HWBANKOUT {      /* hwbankout */
     ULONG  ulLength;
     ULONG  ulBank;                /* Bank number  */
  } HWBANKOUT;
  typedef HWBANKOUT *PHWBANKOUT;

/* NOINC */
#endif /* DDI_INCLUDED */
/* INC */

