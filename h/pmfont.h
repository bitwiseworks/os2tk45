/****************************** Module Header ******************************\
*
* Module Name: PMFONT.H
*
* OS/2 Presentation Manager type declarations for Fonts.
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* Comments at the end of each typedef line give the name tags used in
* the assembler include version of this file.
*
*
\***************************************************************************/

/* XLATOFF */
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
/* XLATON */

/* XLATOFF */
#ifndef PMFONT_INCLUDED
   /* XLATON */
   #define PMFONT_INCLUDED
   
   /***************************************************************************\
   *  FontRangeEntry
   *
   *     ulRun         = number of consecutive glyphs contained in the font
   *     ulSkip        = number of consecutive glyphs skipped in the font,
   *                     ulSkip == 0 --> Last FontRangeEntry in table
   \***************************************************************************/
   
   typedef struct _FONTRANGEENTRY       /* fre */
   {
      ULONG   ulRun;
      ULONG   ulSkip;
   } FONTRANGEENTRY;
   typedef FONTRANGEENTRY *PFONTRANGEENTRY;
   
   /***************************************************************************\
   *  FontCharDef
   *
   *     ulGlyphOffset = offset to rendered character bitmap (0 from driver)
   *     sAspace       = pre-character space
   *     sBspace       = character width (always non-zero)
   *     sCspace       = post-character space
   \***************************************************************************/
   
   typedef struct _FONTCHARDEF          /* fcd */
   {
      ULONG   ulGlyphOffset;
      SHORT   sAspace;
      USHORT  sBspace;
      SHORT   sCspace;
   } FONTCHARDEF;
   typedef FONTCHARDEF *PFONTCHARDEF;
   
   /***************************************************************************\
   *  FocaMetricsExtension
   \***************************************************************************/
   
   typedef struct _FOCAMETRICSEXT /* fme */
   {
      ULONG          ulSize;            /* Total size of extension  */
      ULONG          ulFlags;           /* Reserved, must be 0      */
      ULONG          ulGlyphCount;
      ULONG          ulDefaultIndex;
      ULONG          ulRangeTableEntries;
      FONTRANGEENTRY afreRangeTable[1];
      
   } FOCAMETRICSEXT;
   typedef FOCAMETRICSEXT *PFOCAMETRICSEXT;
   
   /***************************************************************************/
   
   typedef struct _FOCAMETRICS    /* foca */
   {
      ULONG   ulIdentity;
      ULONG   ulSize;
      CHAR    szFamilyname[32];
      CHAR    szFacename[32];
      SHORT   usRegistryId;
      SHORT   usCodePage;
      SHORT   yEmHeight;
      SHORT   yXHeight;
      SHORT   yMaxAscender;
      SHORT   yMaxDescender;
      SHORT   yLowerCaseAscent;
      SHORT   yLowerCaseDescent;
      SHORT   yInternalLeading;
      SHORT   yExternalLeading;
      SHORT   xAveCharWidth;
      SHORT   xMaxCharInc;
      SHORT   xEmInc;
      SHORT   yMaxBaselineExt;
      SHORT   sCharSlope;
      SHORT   sInlineDir;
      SHORT   sCharRot;
      USHORT  usWeightClass;
      USHORT  usWidthClass;
      SHORT   xDeviceRes;
      SHORT   yDeviceRes;
      SHORT   usFirstChar;
      SHORT   usLastChar;
      SHORT   usDefaultChar;
      SHORT   usBreakChar;
      SHORT   usNominalPointSize;
      SHORT   usMinimumPointSize;
      SHORT   usMaximumPointSize;
      SHORT   fsTypeFlags;
      SHORT   fsDefn;
      SHORT   fsSelectionFlags;
      SHORT   fsCapabilities;
      SHORT   ySubscriptXSize;
      SHORT   ySubscriptYSize;
      SHORT   ySubscriptXOffset;
      SHORT   ySubscriptYOffset;
      SHORT   ySuperscriptXSize;
      SHORT   ySuperscriptYSize;
      SHORT   ySuperscriptXOffset;
      SHORT   ySuperscriptYOffset;
      SHORT   yUnderscoreSize;
      SHORT   yUnderscorePosition;
      SHORT   yStrikeoutSize;
      SHORT   yStrikeoutPosition;
      SHORT   usKerningPairs;
      SHORT   sFamilyClass;
      PSZ     pszDeviceNameOffset;
   } FOCAMETRICS;
   typedef FOCAMETRICS *PFOCAMETRICS;
   
   /* XLATOFF */
   /* REUSE - long offset to extension relative to FocaMetrics  */
   #define loffExtension pszDeviceNameOffset
   /* XLATON */
   
   typedef struct _FONTFILEMETRICS    /* ffm */
   {
      ULONG   ulIdentity;
      ULONG   ulSize;
      CHAR    szFamilyname[32];
      CHAR    szFacename[32];
      SHORT   usRegistryId;
      SHORT   usCodePage;
      SHORT   yEmHeight;
      SHORT   yXHeight;
      SHORT   yMaxAscender;
      SHORT   yMaxDescender;
      SHORT   yLowerCaseAscent;
      SHORT   yLowerCaseDescent;
      SHORT   yInternalLeading;
      SHORT   yExternalLeading;
      SHORT   xAveCharWidth;
      SHORT   xMaxCharInc;
      SHORT   xEmInc;
      SHORT   yMaxBaselineExt;
      SHORT   sCharSlope;
      SHORT   sInlineDir;
      SHORT   sCharRot;
      USHORT  usWeightClass;
      USHORT  usWidthClass;
      SHORT   xDeviceRes;
      SHORT   yDeviceRes;
      SHORT   usFirstChar;
      SHORT   usLastChar;
      SHORT   usDefaultChar;
      SHORT   usBreakChar;
      SHORT   usNominalPointSize;
      SHORT   usMinimumPointSize;
      SHORT   usMaximumPointSize;
      SHORT   fsTypeFlags;
      SHORT   fsDefn;
      SHORT   fsSelectionFlags;
      SHORT   fsCapabilities;
      SHORT   ySubscriptXSize;
      SHORT   ySubscriptYSize;
      SHORT   ySubscriptXOffset;
      SHORT   ySubscriptYOffset;
      SHORT   ySuperscriptXSize;
      SHORT   ySuperscriptYSize;
      SHORT   ySuperscriptXOffset;
      SHORT   ySuperscriptYOffset;
      SHORT   yUnderscoreSize;
      SHORT   yUnderscorePosition;
      SHORT   yStrikeoutSize;
      SHORT   yStrikeoutPosition;
      SHORT   usKerningPairs;
      SHORT   sFamilyClass;
      ULONG   ulReserved;
      PANOSE  panose;
   } FONTFILEMETRICS;
   typedef FONTFILEMETRICS *PFONTFILEMETRICS;
   
   typedef struct _FONTDEFINITIONHEADER    /* fdh */
   {
      ULONG   ulIdentity;
      ULONG   ulSize;
      SHORT   fsFontdef;
      SHORT   fsChardef;
      SHORT   usCellSize;
      SHORT   xCellWidth;
      SHORT   yCellHeight;
      SHORT   xCellIncrement;
      SHORT   xCellA;
      SHORT   xCellB;
      SHORT   xCellC;
      SHORT   pCellBaseOffset;
   } FONTDEFINITIONHEADER;
   typedef FONTDEFINITIONHEADER *PFONTDEFINITIONHEADER;
   
   #define FONTDEFFONT1     0x0047  /* set width, height, inc. & base offset */
   #define FONTDEFFONT2     0x0042  /* set height & base offset            */
   #define FONTDEFFONT3     0x0042  /* set height & base offset            */
   #define FONTDEFCHAR1     0x0081  /* set char offset and width           */
   #define FONTDEFCHAR2     0x0081  /* set char offset and width           */
   #define FONTDEFCHAR3     0x00b8  /* set char offset, A, B, and C space  */
   #define SPACE_UNDEF      0x8000  /* space undefined = take default      */
   #define FONTDEFFOCA32    0x4000
   #define FONTDEFDEVFONT   0x2000  /* Device or Downloadable font         */
   
   typedef struct _FONTSIGNATURE    /* fs */
   {
      ULONG   ulIdentity;
      ULONG   ulSize;
      CHAR    achSignature[12];
   } FONTSIGNATURE;
   typedef FONTSIGNATURE *PFONTSIGNATURE;
   
   typedef struct _ADDITIONALMETRICS    /* am */
   {
      ULONG   ulIdentity;
      ULONG   ulSize;
      PANOSE  panose;
   } ADDITIONALMETRICS;
   typedef ADDITIONALMETRICS *PADDITIONALMETRICS;
   
   typedef struct _FOCAFONT    /* ff */
   {
      FONTSIGNATURE     fsSignature;
      FOCAMETRICS       fmMetrics;
      FONTDEFINITIONHEADER fdDefinitions;
   } FOCAFONT;
   typedef FOCAFONT *PFOCAFONT;
   
   #define FONT_SIGNATURE          0xfffffffe /* Identity header start    */
   #define FONT_METRICS            0x00000001 /* Identity metrics         */
   #define FONT_DEFINITION         0x00000002 /* Identity definition      */
   #define FONT_KERNPAIRS          0x00000003 /* Identity Kern Pairs      */
   #define FONT_ADDITIONALMETRICS  0x00000004 /* Identity Additional Metrics */
   #define FONT_ENDRECORD          0xffffffff /* Identity record end      */
   
   typedef  FOCAFONT       FOCAFONT32;
   typedef  FOCAFONT32     *PFOCAFONT32;
   
   /* Options for QueryFonts */
   
   #define QUERY_PUBLIC_FONTS      0x0001
   #define QUERY_PRIVATE_FONTS     0x0002
   
   #define CDEF_GENERIC            0x0001
   #define CDEF_BOLD               0x0002
   #define CDEF_ITALIC             0x0004
   #define CDEF_UNDERSCORE         0x0008
   #define CDEF_STRIKEOUT          0x0010
   #define CDEF_OUTLINE            0x0020
   
   /* XLATOFF */
#endif /* PMFONT_INCLUDED */
/* XLATON */

/* XLATOFF */
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
/* XLATON */

/**************************** end of file **********************************/
