/****************************** Module Header ******************************\
*
* Module Name: PMDDIM.H
*
* OS/2 Presentation Manager entry points macros for the Graphics Engine
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* This file is included from PMDDI.H
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

#define DDIM_INCLUDED

#define  GreCallConv APIENTRY


#define  GreEntry2   Gre32Entry2
#define  GreEntry3   Gre32Entry3
#define  GreEntry4   Gre32Entry4
#define  GreEntry5   Gre32Entry5
#define  GreEntry6   Gre32Entry6
#define  GreEntry7   Gre32Entry7
#define  GreEntry8   Gre32Entry8
#define  GreEntry9   Gre32Entry9
#define  GreEntry10  Gre32Entry10


ULONG GreCallConv GreEntry2(ULONG,ULONG);
ULONG GreCallConv GreEntry3(ULONG,ULONG,ULONG);
ULONG GreCallConv GreEntry4(ULONG,ULONG,ULONG,ULONG);
ULONG GreCallConv GreEntry5(ULONG,ULONG,ULONG,ULONG,ULONG);
ULONG GreCallConv GreEntry6(ULONG,ULONG,ULONG,ULONG,ULONG,ULONG);
ULONG GreCallConv GreEntry7(ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG);
ULONG GreCallConv GreEntry8(ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG);
ULONG GreCallConv GreEntry9(ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG);
ULONG GreCallConv GreEntry10(ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG,ULONG);

#ifdef INCL_GRE_ARCS
   #define GreGetArcParameters(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PARCPARAMS)(b),0L,0x00004000L)
   #define NGreGetArcParameters 0x00004000L
   #define GreSetArcParameters(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PARCPARAMS)(b),0L,0x00004001L)
   #define NGreSetArcParameters 0x00004001L
   #define GreArc(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x00004002L)
   #define NGreArc 0x00004002L
   #define GrePartialArc(a,b,c,d,e) (INT) GreEntry7((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(FIXED)(c),(ULONG)(FIXED)(d),(ULONG)(FIXED)(e),0L,0x00004003L)
   #define NGrePartialArc 0x00004003L
   #define GreFullArcInterior(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(FIXED)(b),0L,0x00004004L)
   #define NGreFullArcInterior 0x00004004L
   #define GreFullArcBoundary(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(FIXED)(b),0L,0x00004005L)
   #define NGreFullArcBoundary 0x00004005L
   #define GreFullArcBoth(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(FIXED)(b),0L,0x00004006L)
   #define NGreFullArcBoth 0x00004006L
   #define GreBoxInterior(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PBOXPARAMS)(b),0L,0x00004007L)
   #define NGreBoxInterior 0x00004007L
   #define GreBoxBoundary(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PBOXPARAMS)(b),0L,0x00004008L)
   #define NGreBoxBoundary 0x00004008L
   #define GreBoxBoth(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PBOXPARAMS)(b),0L,0x00004009L)
   #define NGreBoxBoth 0x00004009L
   #define GreBoxExclusive(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(PBOXPARAMS)(b),(ULONG)(c),0L,0x00004010L)
   #define NGreBoxExclusive 0x00004010L
   #define GrePolyFillet(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(LONG)(c),0L,0x0000400AL)
   #define NGrePolyFillet 0x0000400AL
   #define GrePolyFilletSharp(a,b,c,d) (INT) GreEntry6((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(LONG)(c),(ULONG)(PFIXED)(d),0L,0x0000400BL)
   #define NGrePolyFilletSharp 0x0000400BL
   #define GrePolySpline(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(LONG)(c),0L,0x0000400CL)
   #define NGrePolySpline 0x0000400CL
   #define GreOval(a,b,c,d,e) (INT) GreEntry7((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(FIXED)(c),(ULONG)(FIXED)(d),(ULONG)(e),0L,0x0000400DL)
   #define NGreOval 0x0000400DL
   /* #define GreDrawConicsInPath(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(PPATH)(b),(ULONG)(PCURVE)(c),(ULONG)(LONG)(d),0L,0x0000400DL) */
   /* #define NGreDrawConicsInPath 0x0000400DL                                                                                                    */
   #define GreCookWholePath(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPATH)(b),0L,0x0000400EL)
   #define NGreCookWholePath 0x0000400EL
   #define GreCookPathCurves(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPATH)(b),(ULONG)(PPIPELINEINFO)(c),0L,0x0000400FL)
   #define NGreCookPathCurves 0x0000400FL
   #define GreRenderPath(a,b,c,d,e,f,g,h) (BOOL) GreEntry10((ULONG)(HDC)(a),(ULONG)(PPATH)(b),(ULONG)(PCURVE)(c),(ULONG)(LONG)(d),(ULONG)(LONG)(e),(ULONG)(LONG)(f),(ULONG)(PSHORTLINE)(g),(ULONG)(LONG)(h),0L,0x00004011L)
   #define NGreRenderPath 0x00004011L
#endif
#ifdef INCL_GRE_LINES
   #define GreDisjointLines(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(LONG)(c),0L,0x00004016L)
   #define NGreDisjointLines 0x00004016L
   #define GreGetCurrentPosition(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x00004017L)
   #define NGreGetCurrentPosition 0x00004017L
   #define GreSetCurrentPosition(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x00004018L)
   #define NGreSetCurrentPosition 0x00004018L
   #define GrePolyLine(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(LONG)(c),0L,0x00004019L)
   #define NGrePolyLine 0x00004019L
   #define GreDrawLinesInPath(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(PPATH)(b),(ULONG)(PCURVE)(c),(ULONG)(LONG)(d),0L,0x0000401AL)
   #define NGreDrawLinesInPath 0x0000401AL
   #define GrePolyShortLine(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PSHORTLINE)(b),0L,0x0000401BL)
   #define NGrePolyShortLine 0x0000401BL
#endif
#ifdef INCL_GRE_SCANS
   #define GrePolyScanline(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(PSCANDATA)(b),0L,0x0000401CL)
   #define NGrePolyScanline 0x0000401CL
#endif
#ifdef INCL_GRE_BITMAPS
   #define GreDrawBits(a,b,c,d,e,f,g) (LONG) GreEntry9((ULONG)(HDC)(a),(ULONG)(PBYTE)(b),(ULONG)(PBITMAPINFO2)(c),(ULONG)(LONG)(d),(ULONG)(PPOINTL)(e),(ULONG)(LONG)(f),(ULONG)(ULONG)(g),0L,0x00006022L)
   #define NGreDrawBits 0x00006022L
   #define GreDeviceCreateBitmap(a,b,c,d,e) (HBITMAP) GreEntry7((ULONG)(HDC)(a),(ULONG)(PBITMAPINFOHEADER2)(b),(ULONG)(ULONG)(c),(ULONG)(PBYTE)(d),(ULONG)(PBITMAPINFO2)(e),0L,0x00006023L)
   #define NGreDeviceCreateBitmap 0x00006023L
   #define GreDeviceDeleteBitmap(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(HBITMAP)(b),(ULONG)(PDELETERETURN)(c),(ULONG)(ULONG)(d),0L,0x00004024L)
   #define NGreDeviceDeleteBitmap 0x00004024L
   #define GreDeviceSelectBitmap(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(HBITMAP)(b),0L,0x00004025L)
   #define NGreDeviceSelectBitmap 0x00004025L
   #define GreBitblt(a,b,c,d,e,f,g) (INT) GreEntry9((ULONG)(HDC)(a),(ULONG)(HDC)(b),(ULONG)(LONG)(c),(ULONG)(PBITBLTPARAMETERS)(d),(ULONG)(LONG)(e),(ULONG)(ULONG)(f),(ULONG)(PBITBLTATTRS)(g),0L,0x00006026L)
   #define NGreBitblt 0x00006026L
   #define GreGetPel(a,b) (LONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x00006027L)
   #define NGreGetPel 0x00006027L
   #define GreSetPel(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x00004028L)
   #define NGreSetPel 0x00004028L
   #define GreImageData(a,b,c,d) (INT) GreEntry6((ULONG)(HDC)(a),(ULONG)(PBYTE)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),0L,0x00004029L)
   #define NGreImageData 0x00004029L
   #define GreScanLR(a,b,c,d,e,f) (INT) GreEntry8((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),(ULONG)(PLONG)(e),(ULONG)(PLONG)(f),0L,0x0000602AL)
   #define NGreScanLR 0x0000602AL
   #define GreFloodFill(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(LONG)(c),0L,0x0000602BL)
   #define NGreFloodFill 0x0000602BL
   #define GreSaveScreenBits(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x0000402CL)
   #define NGreSaveScreenBits 0x0000402CL
   #define GreRestoreScreenBits(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(LHANDLE)(b),(ULONG)(PRECTL)(c),(ULONG)(ULONG)(d),0L,0x0000402DL)
   #define NGreRestoreScreenBits 0x0000402DL
   #define GreDrawBorder(a,b,c,d,e,f,g) (BOOL) GreEntry9((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),(ULONG)(COLOR)(e),(ULONG)(COLOR)(f),(ULONG)(ULONG)(g),0L,0x0000602EL)
   #define NGreDrawBorder 0x0000602EL
   #define GreDeviceSetCursor(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(HBITMAP)(c),0L,0x0000402FL)
   #define NGreDeviceSetCursor 0x0000402FL
   #define GreGetBitmapBits(a,b,c,d,e,f) (LONG) GreEntry8((ULONG)(HDC)(a),(ULONG)(HBITMAP)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),(ULONG)(PBYTE)(e),(ULONG)(PBITMAPINFO2)(f),0L,0x00006030L)
   #define NGreGetBitmapBits 0x00006030L
   #define GreSetBitmapBits(a,b,c,d,e,f) (LONG) GreEntry8((ULONG)(HDC)(a),(ULONG)(HBITMAP)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),(ULONG)(PBYTE)(e),(ULONG)(PBITMAPINFO2)(f),0L,0x00006031L)
   #define NGreSetBitmapBits 0x00006031L
#endif
#ifdef INCL_WINPOINTERS
   #define GreSetColorCursor(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTERINFO)(b),0L,0x00004032L)
   #define NGreSetColorCursor 0x00004032L
#endif
#ifdef INCL_GRE_STRINGS
   #define GreCharString(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PCH)(c),0L,0x00005035L)
   #define NGreCharString 0x00005035L
   #define GreCharStringPos(a,b,c,d,e,f,g,h) (INT) GreEntry10((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(PRECTL)(c),(ULONG)(ULONG)(d),(ULONG)(LONG)(e),(ULONG)(PCH)(f),(ULONG)(PLONG)(g),(ULONG)(PCSP_INFO)(h),0L,0x00007036L)
   #define NGreCharStringPos 0x00007036L
   #define GreQueryTextBox(a,b,c,d,e) (BOOL) GreEntry7((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PCH)(c),(ULONG)(LONG)(d),(ULONG)(PPOINTL)(e),0L,0x00005037L)
   #define NGreQueryTextBox 0x00005037L
   #define GreQueryCharPositions(a,b,c,d,e,f,g) (ULONG) GreEntry9((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(ULONG)(c),(ULONG)(LONG)(d),(ULONG)(PCH)(e),(ULONG)(PLONG)(f),(ULONG)(PPOINTL)(g),0L,0x00005038L)
   #define NGreQueryCharPositions 0x00005038L
   #define GreQueryWidthTable(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(LONG)(c),(ULONG)(PLONG)(d),0L,0x00005039L)
   #define NGreQueryWidthTable 0x00005039L
   #define GreQueryCharOutline(a,b,c,d,e) (LONG)GreEntry7((HDC)(a),(ULONG)(b),(ULONG)(PBYTE)(c),(ULONG)(d),(ULONG)(e),0L,0x00004256L)
   #define NGreQueryCharOutline 0x00004256L
   #define GreQueryCharMetricsTable(a,b,c,d,e,f) (BOOL)GreEntry8((HDC)(a),(ULONG)(b),(ULONG)(c),(ULONG)(PCHARMETRICS)(d),(ULONG)(e),(ULONG)(f),0L,0x00004258L)
   #define NGreQueryCharMetricsTable 0x00004258L
   #define GreRealizeString(a,b,c,d,e,f) (LONG)GreEntry8((HDC)(a),(ULONG)(b),(ULONG)(c),(ULONG)(d),(ULONG)(e),(ULONG)(f),0L,0x00004257L)
   #define NGreRealizeString 0x00004257L
   #define GreQueryTabbedTextExtent(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(a),(ULONG)(b),(ULONG)(c),(ULONG)(d), (ULONG)(e), 0L, 0x00004269L)
   #define NGreQueryTabbedTextExtent 0x00004269L
   #define GreTabbedCharStringAt(a,b,c,d,e,f,g,h) (ULONG) GreEntry10((ULONG)(a),(ULONG)(PPOINTL)(b),(ULONG)(PRECTL)(c),(ULONG)(d), (ULONG)(e),(ULONG)(PCH)(f),(ULONG)(PLONG)(g),(ULONG)(PULONG)(h), 0L, 0x0000426AL)
   #define NGreTabbedCharStringAt 0x0000426AL
#endif
#ifdef INCL_GRE_MARKERS
   #define GrePolyMarker(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(LONG)(c),0L,0x0000403AL)
   #define NGrePolyMarker 0x0000403AL
#endif
#ifdef INCL_AVIOP
   #define GreCharRect(a,b,c) (ULONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(VioPresentationSpace *)(b),(ULONG)(LPGridRectRef)(c),0L,0x0000403BL)
   #define NGreCharRect 0x0000403BL
   #define GreCharStr(a,b,c) (ULONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(VioPresentationSpace *)(b),(ULONG)(LPGridStringRef)(c),0L,0x0000403CL)
   #define NGreCharStr 0x0000403CL
   #define GreScrollRect(a,b,c) (ULONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(VioPresentationSpace *)(b),(ULONG)(LPScrollRectRef)(c),0L,0x0000403DL)
   #define NGreScrollRect 0x0000403DL
   #define GreUpdateCursor(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(VioPresentationSpace *)(b),0L,0x0000403EL)
   #define NGreUpdateCursor 0x0000403EL
#endif
#ifdef INCL_WINPOINTERS
   #define GreDeviceSetPointer(a,b) (LONG) GreEntry4((a), (PHWSETPTRIN)(b), 0L, 0x0000403FL)
   #define NGreDeviceSetPointer 0x0000403FL
#endif
#ifdef INCL_GRE_PATHS
   #define GreBeginArea(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x00004046L)
   #define NGreBeginArea 0x00004046L
   #define GreEndArea(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x00004047L)
   #define NGreEndArea 0x00004047L
   #define GreBeginPath(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PHID)(b),0L,0x00004048L)
   #define NGreBeginPath 0x00004048L
   #define GreEndPath(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x00004049L)
   #define NGreEndPath 0x00004049L
   #define GreCloseFigure(a) (BOOL) GreEntry3((ULONG)(HDC)(a),0L,0x0000404AL)
   #define NGreCloseFigure 0x0000404AL
   #define GreFillPath(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PHID)(b),(ULONG)(ULONG)(c),0L,0x0000404BL)
   #define NGreFillPath 0x0000404BL
   #define GreOutlinePath(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PHID)(b),(ULONG)(ULONG)(c),0L,0x0000404CL)
   #define NGreOutlinePath 0x0000404CL
   #define GreModifyPath(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PHID)(b),(ULONG)(ULONG)(c),0L,0x0000404DL)
   #define NGreModifyPath 0x0000404DL
   #define GreStrokePath(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PHID)(b),(ULONG)(ULONG)(c),0L,0x0000404EL)
   #define NGreStrokePath 0x0000404EL
   #define GreSelectClipPath(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PHID)(b),(ULONG)(ULONG)(c),0L,0x0000404FL)
   #define NGreSelectClipPath 0x0000404FL
   #define GreSavePath(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(LONG)(b),0L,0x00004050L)
   #define NGreSavePath 0x00004050L
   #define GreRestorePath(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(LONG)(b),0L,0x00004051L)
   #define NGreRestorePath 0x00004051L
   #define GreClip1DPath(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPATH)(b),(ULONG)(PPIPELINEINFO)(c),0L,0x00004052L)
   #define NGreClip1DPath 0x00004052L
   #define GreDrawRawPath(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPATH)(b),0L,0x00004053L)
   #define NGreDrawRawPath 0x00004053L
   #define GreDrawCookedPath(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(PPATH)(b),(ULONG)(PCURVE)(c),(ULONG)(LONG)(d),0L,0x00004054L)
   #define NGreDrawCookedPath 0x00004054L
   #define GreAreaSetAttributes(a,b,c,d,e) (BOOL) GreEntry7((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(PBUNDLE)(e),0L,0x00006055L)
   #define NGreAreaSetAttributes 0x00006055L
   #define GrePathToRegion(a,b,c) (HRGN) GreEntry5((ULONG)(HDC)(a),(ULONG)(PHID)(b),(ULONG)(ULONG)(c),0L,0x00004057L)
   #define NGrePathToRegion 0x00004057L
#endif
#ifdef INCL_GRE_POLYGON
   #define GrePolygonSet(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(HDC)(a),(ULONG)(b),(ULONG)(c),(ULONG)(PPOLYGON)(d),(ULONG)(e),0L,0x00004056L)
   #define NGrePolygonSet 0x00004056L
   #define GreDrawRLE(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRLEHDR)(b),0L,0x00004058L)
   #define NGreDrawRLE 0x00004058L
   #define GreDevicePolygonSet(a,b) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOLYSET)(b),0L,0x00004059L)
   #define NGreDevicePolygonSet 0x00004059L
   #define GrePolygon3d(a,b,c) (ULONG) Gre32Entry5((ULONG)(HDC)(a),(ULONG)(PPOLYSINFO)(b),(ULONG)(ULONG)(c),0L,0x0000405CL)
   #define NGrePolygon3d 0x0000405CL
#endif
#ifdef INCL_GRE_REGIONS
   #define GreGetRegionBox(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PRECTL)(c),0L,0x0000405DL)
   #define NGreGetRegionBox 0x0000405DL
   #define GreGetRegionRects(a,b,c,d,e) (BOOL) GreEntry7((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PRECTL)(c),(ULONG)(PRGNRECT)(d),(ULONG)(PRECTL)(e),0L,0x0000405EL)
   #define NGreGetRegionRects 0x0000405EL
   #define GreOffsetRegion(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PPOINTL)(c),0L,0x0000405FL)
   #define NGreOffsetRegion 0x0000405FL
   #define GrePtInRegion(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PPOINTL)(c),0L,0x00004060L)
   #define NGrePtInRegion 0x00004060L
   #define GreRectInRegion(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PRECTL)(c),0L,0x00004061L)
   #define NGreRectInRegion 0x00004061L
   #define GreCreateRectRegion(a,b,c) (HRGN) GreEntry5((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),(ULONG)(LONG)(c),0L,0x00004062L)
   #define NGreCreateRectRegion 0x00004062L
   #define GreDestroyRegion(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(HRGN)(b),0L,0x00004063L)
   #define NGreDestroyRegion 0x00004063L
   #define GreSetRectRegion(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PRECTL)(c),(ULONG)(LONG)(d),0L,0x00004064L)
   #define NGreSetRectRegion 0x00004064L
   #define GreCombineRegion(a,b,c,d,e) (INT) GreEntry7((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(HRGN)(c),(ULONG)(HRGN)(d),(ULONG)(ULONG)(e),0L,0x00004065L)
   #define NGreCombineRegion 0x00004065L
   #define GreCombineRectRegion(a,b,c,d,e) (INT) GreEntry7((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PRECTL)(c),(ULONG)(HRGN)(d),(ULONG)(ULONG)(e),0L,0x00004066L)
   #define NGreCombineRectRegion 0x00004066L
   #define GreCombineShortLineRegion(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PSHORTLINE)(c),0L,0x00004067L)
   #define NGreCombineShortLineRegion 0x00004067L
   #define GreEqualRegion(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(HRGN)(c),0L,0x00004068L)
   #define NGreEqualRegion 0x00004068L
   #define GrePaintRegion(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(HRGN)(b),0L,0x00004069L)
   #define NGrePaintRegion 0x00004069L
   #define GreSetRegionOwner(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(ULONG)(c),0L,0x0000406AL)
   #define NGreSetRegionOwner 0x0000406AL
   #define GreFrameRegion(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PSIZEL)(c),0L,0x0000406BL)
   #define NGreFrameRegion 0x0000406BL
#endif
#ifdef INCL_GRE_CLIP
   #define GreGetClipBox(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x0000406EL)
   #define NGreGetClipBox 0x0000406EL
   #define GreGetClipRects(a,b,c,d) (INT) GreEntry6((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),(ULONG)(PRGNRECT)(c),(ULONG)(PRECTL)(d),0L,0x0000406FL)
   #define NGreGetClipRects 0x0000406FL
   #define GreOffsetClipRegion(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x00004070L)
   #define NGreOffsetClipRegion 0x00004070L
   #define GrePtVisible(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x00004071L)
   #define NGrePtVisible 0x00004071L
   #define GreRectVisible(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x00004072L)
   #define NGreRectVisible 0x00004072L
   #define GreQueryClipRegion(a) (HRGN) GreEntry3((ULONG)(HDC)(a),0L,0x00004073L)
   #define NGreQueryClipRegion 0x00004073L
   #define GreSelectClipRegion(a,b,c) (INT) GreEntry5((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PHRGN)(c),0L,0x00004074L)
   #define NGreSelectClipRegion 0x00004074L
   #define GreIntersectClipRectangle(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x00004075L)
   #define NGreIntersectClipRectangle 0x00004075L
   #define GreExcludeClipRectangle(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x00004076L)
   #define NGreExcludeClipRectangle 0x00004076L
   #define GreSetXformRect(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x00004077L)
   #define NGreSetXformRect 0x00004077L
   #define GreSaveRegion(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(LONG)(b),0L,0x0000407BL)
   #define NGreSaveRegion 0x0000407BL
   #define GreRestoreRegion(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(LONG)(b),0L,0x0000407CL)
   #define NGreRestoreRegion 0x0000407CL
   #define GreClipPathCurves(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPATH)(b),(ULONG)(PPIPELINEINFO)(c),0L,0x0000407DL)
   #define NGreClipPathCurves 0x0000407DL
   #define GreSelectPathRegion(a,b) (INT) GreEntry4((ULONG)(HDC)(a),(ULONG)(HRGN)(b),0L,0x0000407EL)
   #define NGreSelectPathRegion 0x0000407EL
   #define GreRegionSelectBitmap(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(HBITMAP)(b),0L,0x0000407FL)
   #define NGreRegionSelectBitmap 0x0000407FL
   #define GreCopyClipRegion(a,b,c,d) (INT) GreEntry6((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(PRECTL)(c),(ULONG)(ULONG)(d),0L,0x00004080L)
   #define NGreCopyClipRegion 0x00004080L
   #define GreSetupDC(a,b,c,d,e,f) (BOOL) GreEntry8((ULONG)(HDC)(a),(ULONG)(HRGN)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),(ULONG)(PRECTL)(e),(ULONG)(ULONG)(f),0L,0x00004081L)
   #define NGreSetupDC 0x00004081L
#endif
#ifdef INCL_GRE_XFORMS
   #define GreGetPageUnits(a,b) (LONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(PULONG)(b),0L,0x00004083L)
   #define NGreGetPageUnits 0x00004083L
   #define GreSetPageUnits(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),0L,0x00004084L)
   #define NGreSetPageUnits 0x00004084L
   #define GreGetModelXform(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PXFORM)(b),0L,0x00004085L)
   #define NGreGetModelXform 0x00004085L
   #define GreSetModelXform(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PXFORM)(b),(ULONG)(ULONG)(c),0L,0x00004086L)
   #define NGreSetModelXform 0x00004086L
   #define GreGetWindowViewportXform(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PXFORM)(b),0L,0x00004087L)
   #define NGreGetWindowViewportXform 0x00004087L
   #define GreSetWindowViewportXform(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PXFORM)(b),(ULONG)(ULONG)(c),0L,0x00004088L)
   #define NGreSetWindowViewportXform 0x00004088L
   #define GreGetGlobalViewingXform(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PXFORM)(b),0L,0x00004089L)
   #define NGreGetGlobalViewingXform 0x00004089L
   #define GreSetGlobalViewingXform(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PXFORM)(b),(ULONG)(ULONG)(c),0L,0x0000408AL)
   #define NGreSetGlobalViewingXform 0x0000408AL
   #define GreSaveXformData(a,b,c) (LONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PBYTE)(c),0L,0x0000408BL)
   #define NGreSaveXformData 0x0000408BL
   #define GreRestoreXformData(a,b,c) (LONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PBYTE)(c),0L,0x0000408CL)
   #define NGreRestoreXformData 0x0000408CL
   #define GreGetPageViewport(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x0000408DL)
   #define NGreGetPageViewport 0x0000408DL
   #define GreSetPageViewport(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),(ULONG)(ULONG)(c),0L,0x0000408EL)
   #define NGreSetPageViewport 0x0000408EL
   #define GreGetGraphicsField(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x00004091L)
   #define NGreGetGraphicsField 0x00004091L
   #define GreSetGraphicsField(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x00004092L)
   #define NGreSetGraphicsField 0x00004092L
   #define GreGetViewingLimits(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x00004093L)
   #define NGreGetViewingLimits 0x00004093L
   #define GreSetViewingLimits(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x00004094L)
   #define NGreSetViewingLimits 0x00004094L
   #define GreQueryViewportSize(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(LONG)(c),(ULONG)(PVIEWPORTSIZE)(d),0L,0x00004095L)
   #define NGreQueryViewportSize 0x00004095L
   #define GreConvert(a,b,c,d,e) (BOOL) GreEntry7((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(LONG)(c),(ULONG)(PPOINTL)(d),(ULONG)(LONG)(e),0L,0x00004096L)
   #define NGreConvert 0x00004096L
   #define GreConvertPath(a,b,c,d,e,f) (BOOL) GreEntry8((ULONG)(HDC)(a),(ULONG)(PPATH)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),(ULONG)(PCURVE)(e),(ULONG)(LONG)(f),0L,0x00004097L)
   #define NGreConvertPath 0x00004097L
   #define GreSaveXform(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(LONG)(b),0L,0x00004098L)
   #define NGreSaveXform 0x00004098L
   #define GreRestoreXform(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(LONG)(b),0L,0x00004099L)
   #define NGreRestoreXform 0x00004099L
   #define GreMultiplyXforms(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(PXFORM)(b),(ULONG)(PXFORM)(c),(ULONG)(ULONG)(d),0L,0x0000409AL)
   #define NGreMultiplyXforms 0x0000409AL
   #define GreConvertWithMatrix(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(LONG)(c),(ULONG)(PXFORM)(d),0L,0x0000409BL)
   #define NGreConvertWithMatrix 0x0000409BL
#endif
#ifdef INCL_GRE_DEVMISC1
   #define GreDeviceGetAttributes(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(PBUNDLE)(d),0L,0x0000609DL)
   #define NGreDeviceGetAttributes 0x0000609DL
   #define GreDeviceSetAVIOFont2(a,b,c,d) (ULONG) GreEntry6((ULONG)(HDC)(a),(ULONG)(PFATTRS)(b),(ULONG)(PFOCAFONT)(c),(ULONG)(LCID)(d),0L,0x0000409EL)
   #define NGreDeviceSetAVIOFont2 0x0000409EL
   #define GreGetPairKerningTable(a,b,c) (ULONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PKERNINGPAIRS)(c),0L,0x000040A0L)
   #define NGreGetPairKerningTable 0x000040A0L
   #define GreDeviceSetAVIOFont(a,b,c) (ULONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(PFOCAFONT)(b),(ULONG)(LCID)(c),0L,0x000040A1L)
   #define NGreDeviceSetAVIOFont 0x000040A1L
   #define GreDeviceSetAttributes(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(PBUNDLE)(e),0L,0x000060A2L)
   #define NGreDeviceSetAttributes 0x000060A2L
   #define GreDeviceSetGlobalAttribute(a,b,c,d) (ULONG) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),0L,0x000060A3L)
   #define NGreDeviceSetGlobalAttribute 0x000060A3L
#endif
#ifdef INCL_GRE_DEVMISC2
   #define GreNotifyClipChange(a,b,c,d) (ULONG) GreEntry6((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),(ULONG)(LONG)(c),(ULONG)(ULONG)(d),0L,0x000040A4L)
   #define NGreNotifyClipChange 0x000040A4L
   #define GreNotifyTransformChange(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(PNOTIFYTRANSFORMDATA)(c),0L,0x000040A5L)
   #define NGreNotifyTransformChange 0x000040A5L
   #define GreRealizeFont(a,b,c,d) (ULONG) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(PFATTRS)(c),(ULONG)(PFOCAFONT)(d),0L,0x000040A6L)
   #define NGreRealizeFont 0x000040A6L
   #define GreErasePS(a) (BOOL) GreEntry3((ULONG)(HDC)(a),0L,0x000040A7L)
   #define NGreErasePS 0x000040A7L
   #define GreSetStyleRatio(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PBYTE)(b),0L,0x000040A8L)
   #define NGreSetStyleRatio 0x000040A8L
   #define GreDeviceQueryFontAttributes(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PFONTMETRICS)(c),0L,0x000040A9L)
   #define NGreDeviceQueryFontAttributes 0x000040A9L
   #define GreDeviceQueryFonts(a,b,c,d,e,f) (LONG) GreEntry8((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(PSZ)(c),(ULONG)(PFONTMETRICS)(d),(ULONG)(LONG)(e),(ULONG)(PLONG)(f),0L,0x000040AAL)
   #define NGreDeviceQueryFonts 0x000040AAL
   #define GreDeviceInvalidateVisRegion(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PDC_BLOCK)(c),0L,0x000040ABL)
   #define NGreDeviceInvalidateVisRegion 0x000040ABL
   #define GreSetFontMappingFlags(a,b) (LONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(LONG)(b),0L,0x00004268L)
   #define NGreSetFontMappingFlags 0x00004268L
#endif
#ifdef INCL_GRE_PICK
   #define GreGetPickWindow(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x000040ACL)
   #define NGreGetPickWindow 0x000040ACL
   #define GreSetPickWindow(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x000040ADL)
   #define NGreSetPickWindow 0x000040ADL
#endif
#ifdef INCL_GRE_DEVMISC3
   #define GreResetBounds(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x000040AEL)
   #define NGreResetBounds 0x000040AEL
   #define GreGetBoundsData(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(PRECTL)(c),0L,0x000040AFL)
   #define NGreGetBoundsData 0x000040AFL
   #define GreAccumulateBounds(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x000040B0L)
   #define NGreAccumulateBounds 0x000040B0L
   #define GreGetExtraError(a) (FIXED) GreEntry3((ULONG)(HDC)(a),0L,0x000040B1L)
   #define NGreGetExtraError 0x000040B1L
   #define GreSetExtraError(a,b) (VOID) GreEntry4((ULONG)(HDC)(a),(ULONG)(FIXED)(b),0L,0x000040B2L)
   #define NGreSetExtraError 0x000040B2L
   #define GreGetCodePage(a) (LONG) GreEntry3((ULONG)(HDC)(a),0L,0x000040B3L)
   #define NGreGetCodePage 0x000040B3L
   #define GreSetCodePage(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x000040B4L)
   #define NGreSetCodePage 0x000040B4L
   #define GreLockDevice(a) (BOOL) GreEntry3((ULONG)(HDC)(a),0L,0x000040B5L)
   #define NGreLockDevice 0x000040B5L
   #define GreUnlockDevice(a) (BOOL) GreEntry3((ULONG)(HDC)(a),0L,0x000040B6L)
   #define NGreUnlockDevice 0x000040B6L
   #define GreDeath(a) (BOOL) GreEntry3((ULONG)(HDC)(a),0L,0x000040B7L)
   #define NGreDeath 0x000040B7L
   #define GreResurrection(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PBYTE)(c),0L,0x000040B8L)
   #define NGreResurrection 0x000040B8L
   #define GreGetDCOrigin(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x000040BAL)
   #define NGreGetDCOrigin 0x000040BAL
   #define GreDeviceSetDCOrigin(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x000040BBL)
   #define NGreDeviceSetDCOrigin 0x000040BBL
   #define GreGetLineOrigin(a,b) (LONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),0L,0x000040BCL)
   #define NGreGetLineOrigin 0x000040BCL
   #define GreSetLineOrigin(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(LONG)(c),0L,0x000040BDL)
   #define NGreSetLineOrigin 0x000040BDL
#endif
#ifdef INCL_GRE_DEVMISC2
   #define GreGetStyleRatio(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PBYTE)(b),0L,0x000040BEL)
   #define NGreGetStyleRatio 0x000040BEL
#endif
#ifdef INCL_GRE_COLORTABLE
   #define GreQueryColorData(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PLONG)(c),0L,0x000060C3L)
   #define NGreQueryColorData 0x000060C3L
   #define GreQueryLogColorTable(a,b,c,d,e) (LONG) GreEntry7((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),(ULONG)(PLONG)(e),0L,0x000060C4L)
   #define NGreQueryLogColorTable 0x000060C4L
   #define GreCreateLogColorTable(a,b,c,d,e,f) (BOOL) GreEntry8((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(LONG)(d),(ULONG)(LONG)(e),(ULONG)(PLONG)(f),0L,0x000060C5L)
   #define NGreCreateLogColorTable 0x000060C5L
   #define GreRealizeColorTable(a) (BOOL) GreEntry3((ULONG)(HDC)(a),0L,0x000060C6L)
   #define NGreRealizeColorTable 0x000060C6L
   #define GreUnrealizeColorTable(a) (BOOL) GreEntry3((ULONG)(HDC)(a),0L,0x000060C7L)
   #define NGreUnrealizeColorTable 0x000060C7L
   #define GreQueryRealColors(a,b,c,d,e) (LONG) GreEntry7((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(LONG)(c),(ULONG)(LONG)(d),(ULONG)(PLONG)(e),0L,0x000040C8L)
   #define NGreQueryRealColors 0x000040C8L
   #define GreQueryNearestColor(a,b,c) (LONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(COLOR)(c),0L,0x000040C9L)
   #define NGreQueryNearestColor 0x000040C9L
   #define GreQueryColorIndex(a,b,c) (LONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(COLOR)(c),0L,0x000060CAL)
   #define NGreQueryColorIndex 0x000060CAL
   #define GreQueryRGBColor(a,b,c) (LONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(COLOR)(c),0L,0x000060CBL)
   #define NGreQueryRGBColor 0x000060CBL
#endif
#ifdef INCL_GRE_DEVICE
   #ifndef NGreQueryDevFntMetrics
      #define GreQueryDeviceFontMetrics(a,b,c) (ULONG)GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(LONG)(c),0L,0x000060CCL)
      #define NGreQueryDevFntMetrics 0x000060CCL
   #endif
#endif
#ifdef INCL_GRE_PAX
   #define GreDeviceEnableYInversion(a,b) (ULONG) GreEntry4((ULONG)(a),(LONG)(b),0L,0x000040CDL)
   #define NGreDeviceEnableYInversion 0x000040CDL
#endif
#ifdef INCL_GRE_DEVICE
   #define GreQueryDeviceBitmaps(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PBITMAPFORMAT)(b),(ULONG)(LONG)(c),0L,0x000040D0L)
   #define NGreQueryDeviceBitmaps 0x000040D0L
   #define GreQueryDeviceCaps(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PLONG)(c),(ULONG)(LONG)(d),0L,0x000040D1L)
   #define NGreQueryDeviceCaps 0x000040D1L
   #define GreEscape(a,b,c,d,e,f) (LONG) GreEntry8((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(LONG)(c),(ULONG)(PBYTE)(d),(ULONG)(PLONG)(e),(ULONG)(PBYTE)(f),0L,0x000040D2L)
   #define NGreEscape 0x000040D2L
   #define GreQueryHardcopyCaps(a,b,c,d) (LONG) GreEntry6((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(LONG)(c),(ULONG)(PHCINFO)(d),0L,0x000040D3L)
   #define NGreQueryHardcopyCaps 0x000040D3L
   #define GreQueryDevResource2(a,b,c) (PVOID) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),0L,0x000040D4L)
   #define NGreQueryDevResource2 0x000040D4L
   #define GreUnloadPD(a,b) (PVOID) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x0000026EL)
   #define NGreUnloadPD 0x0000026EL
#endif
#ifdef INCL_GRE_SCREEN
   #define GreOpenScreenChangeArea(a) (ULONG) GreEntry3((ULONG)(HDC)(a),0L,0x00004012L)
   #define NGreOpenScreenChangeArea 0x00004012L
   #define GreGetScreenChangeArea(a,b,c) (ULONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),0L,0x00004013L)
   #define NGreGetScreenChangeArea 0x00004013L
   #define GreCloseScreenChangeArea(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x00004014L)
   #define NGreCloseScreenChangeArea 0x00004014L
   #define GreGetScreenBits(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(ULONG)(e),0L,0x0000401DL)
   #define NGreGetScreenBits 0x0000401DL
   #define GreSetScreenBits(a,b,c,d) (ULONG) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),0L,0x0000401EL)
   #define NGreSetScreenBits 0x0000401EL
#endif
#ifdef INCL_GRE_PALETTE
   #define GreDeviceCreatePalette(a,b,c) (ULONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPALETTEINFO)(b),(ULONG)(HDEVPAL)(c),0L,0x000040D5L)
   #define NGreDeviceCreatePalette 0x000040D5L
   #define GreDeviceDeletePalette(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(HDEVPAL)(b),0L,0x000040D6L)
   #define NGreDeviceDeletePalette 0x000040D6L
   #define GreDeviceSetPaletteEntries(a,b,c,d,e,f) (ULONG) GreEntry8((ULONG)(HDC)(a),(ULONG)(HDEVPAL)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(ULONG)(e),(ULONG)(PULONG)(f),0L,0x000040D7L)
   #define NGreDeviceSetPaletteEntries 0x000040D7L
   #define GreDeviceAnimatePalette(a,b,c,d,e,f) (ULONG) GreEntry8((ULONG)(HDC)(a),(ULONG)(HDEVPAL)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(ULONG)(e),(ULONG)(PULONG)(f),0L,0x000040D8L)
   #define NGreDeviceAnimatePalette 0x000040D8L
   #define GreDeviceResizePalette(a,b,c) (ULONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(HDEVPAL)(b),(ULONG)(ULONG)(c),0L,0x000040D9L)
   #define NGreDeviceResizePalette 0x000040D9L
   #define GreRealizePalette(a,b,c) (ULONG) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(PULONG)(c),0L,0x000040DAL)
   #define NGreRealizePalette 0x000040DAL
   #define GreQueryHWPaletteInfo(a,b,c,d) (ULONG) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(PULONG)(d),0L,0x000040DBL)
   #define NGreQueryHWPaletteInfo 0x000040DBL
   #define GreUpdateColors(a) (ULONG) GreEntry3((ULONG)(HDC)(a),0L,0x000040DCL)
   #define NGreUpdateColors 0x000040DCL
   #define GreQueryPaletteRealization(a,b,c,d) (ULONG) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(PULONG)(d),0L,0x000040DDL)
   #define NGreQueryPaletteRealization 0x000040DDL
   #define GreGetVisRects(a,b,c,d) (INT) GreEntry6((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),(ULONG)(PRGNRECT)(c),(ULONG)(PRECTL)(d),0L,0x000040DEL)
   #define NGreGetVisRects 0x000040DEL
   #define GreQueryNearestPaletteIndex(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRGB2)(b),0L,0x00000267L)
   #define NGreQueryNearestPaletteIndex 0x00000267L
#endif
#ifdef INCL_GRE_DCS
   #define GreOpenDC(a,b,c,d,e) (HDC) GreEntry7((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(PSZ)(c),(ULONG)(LONG)(d),(ULONG)(PDEVOPENDATA)(e),0L,0x00000200L)
   #define NGreOpenDC 0x00000200L
   #define GreCloseDC(a) (BOOL) GreEntry3((ULONG)(HDC)(a),0L,0x00004201L)
   #define NGreCloseDC 0x00004201L
   #define GreResetDC(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x00004202L)
   #define NGreResetDC 0x00004202L
   #define GreResetDC2(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x0000426BL)
   #define NGreResetDC2 0x0000426BL
   #define GreGetHandle(a,b) (LONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x00004203L)
   #define NGreGetHandle 0x00004203L
   #define GreSetHandle(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(LHANDLE)(b),(ULONG)(ULONG)(c),0L,0x00004204L)
   #define NGreSetHandle 0x00004204L
   #define GreGetProcessControl(a) (LONG) GreEntry3((ULONG)(HDC)(a),0L,0x00004205L)
   #define NGreGetProcessControl 0x00004205L
   #define GreSetProcessControl(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),0L,0x00004206L)
   #define NGreSetProcessControl 0x00004206L
   #define GreSaveDC(a) (LONG) GreEntry3((ULONG)(HDC)(a),0L,0x00004207L)
   #define NGreSaveDC 0x00004207L
   #define GreRestoreDC(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(LONG)(b),0L,0x00004208L)
   #define NGreRestoreDC 0x00004208L
   #define GreQueryDeviceNames(a,b,c,d,e,f) (BOOL) GreEntry8((ULONG)(PSZ)(a),(ULONG)(PLONG)(b),(ULONG)(PSTR32)(c),(ULONG)(PSTR64)(d),(ULONG)(PLONG)(e),(ULONG)(PSTR16)(f),0L,0x00000209L)
   #define NGreQueryDeviceNames 0x00000209L
   #define GreQueryEngineVersion() (LONG) GreEntry2(0L,0x0000020AL)
   #define NGreQueryEngineVersion 0x0000020AL
   #define GreSetDCOwner(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x0000420BL)
   #define NGreSetDCOwner 0x0000420BL
#endif
#ifdef INCL_GRE_DEVSUPPORT
   #define GreQueryBitmapSelection(a) (HDC) GreEntry3((ULONG)(HBITMAP)(a),0L,0x0000020CL)
   #define NGreQueryBitmapSelection 0x0000020CL
   #define GreGetBitmapInfoHeader(a,b) (BOOL) GreEntry4((ULONG)(HBITMAP)(a),(ULONG)(PBITMAPINFOHEADER2)(b),0L,0x0000020DL)
   #define NGreGetBitmapInfoHeader 0x0000020DL
   #define GreConvertBitmapData(a,b,c,d,e,f,g) (ULONG) GreEntry9((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(PBITMAPINFO2)(d),(ULONG)(PBYTE)(e),(ULONG)(PBITMAPINFO2)(f),(ULONG)(PBYTE)(g),0L,0x0000420EL)
   #define NGreConvertBitmapData 0x0000420EL
   #define GreInvalidateVisRegion(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PDC_BLOCK)(c),0L,0x0000420FL)
   #define NGreInvalidateVisRegion 0x0000420FL
   #define GreCreateBitmap(a,b,c,d,e) (HBITMAP) GreEntry7((ULONG)(HDC)(a),(ULONG)(PBITMAPINFOHEADER2)(b),(ULONG)(ULONG)(c),(ULONG)(PBYTE)(d),(ULONG)(PBITMAPINFO2)(e),0L,0x00004210L)
   #define NGreCreateBitmap 0x00004210L
   #define GreDeleteBitmap(a) (BOOL) GreEntry3((ULONG)(HBITMAP)(a),0L,0x00000211L)
   #define NGreDeleteBitmap 0x00000211L
   #define GreSelectBitmap(a,b) (HBITMAP) GreEntry4((ULONG)(HDC)(a),(ULONG)(HBITMAP)(b),0L,0x00004212L)
   #define NGreSelectBitmap 0x00004212L
   #define GreGetBitmapParameters(a,b) (BOOL) GreEntry4((ULONG)(HBITMAP)(a),(ULONG)(PBITMAPINFOHEADER)(b),0L,0x00000213L)
   #define NGreGetBitmapParameters 0x00000213L
   #define GreGetBitmapDimension(a,b) (BOOL) GreEntry4((ULONG)(HBITMAP)(a),(ULONG)(PSIZEL)(b),0L,0x00000214L)
   #define NGreGetBitmapDimension 0x00000214L
   #define GreSetBitmapDimension(a,b) (BOOL) GreEntry4((ULONG)(HBITMAP)(a),(ULONG)(PSIZEL)(b),0L,0x00000215L)
   #define NGreSetBitmapDimension 0x00000215L
   #define GreSetCursor(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(PPOINTL)(b),(ULONG)(HBITMAP)(c),0L,0x00004216L)
   #define NGreSetCursor 0x00004216L
   #define GreGetAttributes(a,b,c,d) (LONG) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(PBUNDLE)(d),0L,0x00004217L)
   #define NGreGetAttributes 0x00004217L
   #define GreSetAttributes(a,b,c,d,e) (BOOL) GreEntry7((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(PBUNDLE)(e),0L,0x00004218L)
   #define NGreSetAttributes 0x00004218L
   #define GreSetGlobalAttribute(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),0L,0x00004219L)
   #define NGreSetGlobalAttribute 0x00004219L
   #define GreSetBitmapOwner(a,b) (BOOL) GreEntry4((ULONG)(HBITMAP)(a),(ULONG)(ULONG)(b),0L,0x0000021AL)
   #define NGreSetBitmapOwner 0x0000021AL
   #define GreGetDefaultAttributes(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(PBUNDLE)(d),0L,0x0000421BL)
   #define NGreGetDefaultAttributes 0x0000421BL
   #define GreSetDefaultAttributes(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(PBUNDLE)(d),0L,0x0000421CL)
   #define NGreSetDefaultAttributes 0x0000421CL
   #define GreGetDefaultArcParameters(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PARCPARAMS)(b),0L,0x0000421DL)
   #define NGreGetDefaultArcParameters 0x0000421DL
   #define GreSetDefaultArcParameters(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PARCPARAMS)(b),0L,0x0000421EL)
   #define NGreSetDefaultArcParameters 0x0000421EL
   #define GreGetDefaultViewingLimits(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x0000421FL)
   #define NGreGetDefaultViewingLimits 0x0000421FL
   #define GreSetDefaultViewingLimits(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(PRECTL)(b),0L,0x00004220L)
   #define NGreSetDefaultViewingLimits 0x00004220L
   #define GreInitializeAttributes(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x00004221L)
   #define NGreInitializeAttributes 0x00004221L
#endif
#ifdef INCL_GRE_SETID
   #define GreDeleteSetId(a,b) (BOOL) GreEntry4((ULONG)(HDC)(a),(ULONG)(LCID)(b),0L,0x00004222L)
   #define NGreDeleteSetId 0x00004222L
   #define GreQueryNumberSetIds(a,b) (LONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(ULONG)(b),0L,0x00004223L)
   #define NGreQueryNumberSetIds 0x00004223L
   #define GreQuerySetIds(a,b,c,d,e,f) (BOOL) GreEntry8((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PLONG)(c),(ULONG)(PSTR8)(d),(ULONG)(PLCID)(e),(ULONG)(ULONG)(f),0L,0x00004224L)
   #define NGreQuerySetIds 0x00004224L
#endif
#ifdef INCL_GRE_LCID
   #define GreQueryBitmapHandle(a,b) (HBITMAP) GreEntry4((ULONG)(HDC)(a),(ULONG)(LCID)(b),0L,0x00000225L)
   #define NGreQueryBitmapHandle 0x00000225L
   #define GreSetBitmapID(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(HBITMAP)(b),(ULONG)(LCID)(c),0L,0x00004226L)
   #define NGreSetBitmapID 0x00004226L
   #define GreCopyDCLoadData(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(HDC)(c),0L,0x00004227L)
   #define NGreCopyDCLoadData 0x00004227L
#endif
#ifdef INCL_GRE_SETID
   #define GreQuerySetIdUsage(a,b) (LONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(LCID)(b),0L,0x00004228L)
   #define NGreQuerySetIdUsage 0x00004228L
#endif
#ifdef INCL_GRE_FONTS
   #define GreQueryFullFontFileDescriptions(a,b,c,d) (ULONG) GreEntry6((ULONG)(PSZ)(a),(ULONG)(PULONG)(b),(ULONG)(PVOID)(c),(ULONG)(PLONG)(d),0L,0x0000022BL)
   #define NGreQueryFullFontFileDescriptions 0x0000022BL
   #define GreQueryLogicalFont(a,b,c,d,e) (BOOL) GreEntry7((ULONG)(HDC)(a),(ULONG)(LCID)(b),(ULONG)(PSTR8)(c),(ULONG)(PFATTRS)(d),(ULONG)(LONG)(e),0L,0x0000422CL)
   #define NGreQueryLogicalFont 0x0000422CL
   #define GreCreateLogicalFont(a,b,c,d) (BOOL) GreEntry6((ULONG)(HDC)(a),(ULONG)(LCID)(b),(ULONG)(PSTR8)(c),(ULONG)(PFATTRS)(d),0L,0x0000422DL)
   #define NGreCreateLogicalFont 0x0000422DL
   #define GreLoadFont(a) (BOOL) GreEntry3((ULONG)(PSZ)(a),0L,0x0000022EL)
   #define NGreLoadFont 0x0000022EL
   #define GreUnloadFont(a) (BOOL) GreEntry3((ULONG)(PSZ)(a),0L,0x0000022FL)
   #define NGreUnloadFont 0x0000022FL
   #define GreQueryFonts(a,b,c,d,e,f) (LONG) GreEntry8((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(PSZ)(c),(ULONG)(PFONTMETRICS)(d),(ULONG)(LONG)(e),(ULONG)(PLONG)(f),0L,0x00004230L)
   #define NGreQueryFonts 0x00004230L
   #define GreQueryFontAttributes(a,b,c) (BOOL) GreEntry5((ULONG)(HDC)(a),(ULONG)(LONG)(b),(ULONG)(PFONTMETRICS)(c),0L,0x00004231L)
   #define NGreQueryFontAttributes 0x00004231L
   #define GreInstallIFI(a,b,c,d) (LONG) GreEntry6((ULONG)(ULONG)(a),(ULONG)(PSZ)(b),(ULONG)(PSZ)(c),(ULONG)(PSZ)(d),0L,0x00000232L)
   #define NGreInstallIFI 0x00000232L
   #define GreLoadPublicFont(a) (BOOL) GreEntry3((ULONG)(PSZ)(a),0L,0x00000233L)
   #define NGreLoadPublicFont 0x00000233L
   #define GreUnLoadPublicFont(a) (BOOL) GreEntry3((ULONG)(PSZ)(a),0L,0x00000234L)
   #define NGreUnLoadPublicFont 0x00000234L
   #define GreQueryCodePageVector(a) (ULONG) GreEntry3((ULONG)(ULONG)(a),0L,0x00000235L)
   #define NGreQueryCodePageVector 0x00000235L
   #define GreQueryFontFileDescriptions(a,b,c) (ULONG) GreEntry5((ULONG)(PSZ)(a),(ULONG)(PLONG)(b),(ULONG)(PFFDESCS)(c),0L,0x00000236L)
   #define NGreQueryFontFileDescriptions 0x00000236L
   #define GreQueryFaceString(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(HDC)(a),(ULONG)(PSZ)(b),(ULONG)(PFACENAMEDESC)(c),(ULONG)(ULONG)(d),(ULONG)(PSZ)(e),0L,0x00004237L)
   #define NGreQueryFaceString 0x00004237L
   #define GreQueryFontAction(a) (ULONG) GreEntry3((ULONG)(ULONG)(a),0L,0x00000238L)
   #define NGreQueryFontAction 0x00000238L
   #define GreQueryCodePageObject(a,b,c) (ULONG)GreEntry5((ULONG)(a),(ULONG)(PSZ)(b),(ULONG)(PCODEPAGEOBJECT *)(c),0L,0x00000255L)
   #define NGreQueryCodePageObject 0x00000255L
#endif
#ifdef INCL_GRE_JOURNALING
   #define GreAccumulateJournalFile(a) (ULONG) GreEntry3((ULONG)(ULONG)(a),0L,0x00000239L)
   #define NGreAccumulateJournalFile 0x00000239L
   #define GreCreateJournalFile(a,b,c) (ULONG) GreEntry5((ULONG)(PSZ)(a),(ULONG)(ULONG)(b),(ULONG)(LONG)(c),0L,0x0000023AL)
   #define NGreCreateJournalFile 0x0000023AL
   #define GreDeleteJournalFile(a) (ULONG) GreEntry3((ULONG)(LHANDLE)(a),0L,0x0000023BL)
   #define NGreDeleteJournalFile 0x0000023BL
   #define GreStartJournalFile(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(LHANDLE)(b),0L,0x0000023CL)
   #define NGreStartJournalFile 0x0000023CL
   #define GreStopJournalFile(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(LHANDLE)(b),0L,0x0000023DL)
   #define NGreStopJournalFile 0x0000023DL
   #define GrePlayJournalFile(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(LHANDLE)(b),0L,0x0000023EL)
   #define NGrePlayJournalFile 0x0000023EL
   #define GreOpenJournalFile(a,b,c) (ULONG) GreEntry5((ULONG)(PSZ)(a),(ULONG)(ULONG)(b),(ULONG)(LONG)(c),0L,0x0000023FL)
   #define NGreOpenJournalFile 0x0000023FL
#endif
#ifdef INCL_GRE_PALETTE
   #define GreCreatePalette(a,b,c,d) (HPAL) GreEntry6((ULONG)(ULONG)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(PULONG)(d),0L,0x00000240L)
   #define NGreCreatePalette 0x00000240L
   #define GreDeletePalette(a) (ULONG) GreEntry3((ULONG)(HPAL)(a),0L,0x00000241L)
   #define NGreDeletePalette 0x00000241L
   #define GreSelectPalette(a,b) (ULONG) GreEntry4((ULONG)(HDC)(a),(ULONG)(HPAL)(b),0L,0x00004242L)
   #define NGreSelectPalette 0x00004242L
   #define GreSetPaletteEntries(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(HPAL)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(PULONG)(e),0L,0x00000243L)
   #define NGreSetPaletteEntries 0x00000243L
   #define GreAnimatePalette(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(HPAL)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(PULONG)(e),0L,0x00000244L)
   #define NGreAnimatePalette 0x00000244L
   #define GreQueryPalette(a) (ULONG) GreEntry3((ULONG)(HDC)(a),0L,0x00004245L)
   #define NGreQueryPalette 0x00004245L
   #define GreQueryPaletteInfo(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(HPAL)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(PULONG)(e),0L,0x00000246L)
   #define NGreQueryPaletteInfo 0x00000246L
   #define GreSetPaletteOwner(a,b) (BOOL) GreEntry4((ULONG)(HPAL)(a),(ULONG)(ULONG)(b),0L,0x00000247L)
   #define NGreSetPaletteOwner 0x00000247L
   #define GreResizePalette(a,b) (ULONG) GreEntry4((ULONG)(HPAL)(a),(ULONG)(ULONG)(b),0L,0x00000248L)
   #define NGreResizePalette 0x00000248L
   #define GreQueryDefaultPaletteInfo(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(HDC)(a),(ULONG)(ULONG)(b),(ULONG)(ULONG)(c),(ULONG)(ULONG)(d),(ULONG)(PULONG)(e),0L,0x00004249L)
   #define NGreQueryDefaultPaletteInfo 0x00004249L
#endif
#ifdef INCL_GRE_HFONT
   #define GreCreateFontHandle(a,b,c) (HFONT) GreEntry5((ULONG)(ULONG)(a),(ULONG)(PSTR8)(b),(ULONG)(PFATTRS)(c),0L,0x00000250L)
   #define NGreCreateFontHandle 0x00000250L
   #define GreQueryFontHandle(a,b,c,d,e) (ULONG) GreEntry7((ULONG)(HFONT)(a),(ULONG)(PULONG)(b),(ULONG)(PSTR8)(c),(ULONG)(PFATTRS)(d),(ULONG)(ULONG)(e),0L,0x00000251L)
   #define NGreQueryFontHandle 0x00000251L
   #define GreSelectFontHandle(a,b) (HFONT) GreEntry4((ULONG)(HDC)(a),(ULONG)(HFONT)(b),0L,0x00004252L)
   #define NGreSelectFontHandle 0x00004252L
   #define GreDeleteFontHandle(a) (ULONG) GreEntry3((ULONG)(HFONT)(a),0L,0x00000253L)
   #define NGreDeleteFontHandle 0x00000253L
   #define GreSetFontHandleOwner(a,b) (ULONG) GreEntry4((ULONG)(HFONT)(a),(ULONG)(ULONG)(b),0L,0x00000254L)
   #define NGreSetFontHandleOwner 0x00000254L
#endif
#ifdef INCL_GRE_INKPATH
   #define NGreBeginInkPath 0x00004259L
   #define GreBeginInkPath(a,b,c) (BOOL) GreEntry5((HDC)(a),(LONG)(b),(ULONG)(c),0L,NGreBeginInkPath)
   #define NGreEndInkPath 0x0000425AL
   #define GreEndInkPath(a,b) (BOOL) GreEntry4((HDC)(a),(ULONG)(b),0L,NGreEndInkPath)
   #define NGreStrokeInkPath 0x0000425BL
   #define GreStrokeInkPath(a,b,c,d,e) (BOOL) GreEntry7((HDC)(a),(LONG)(b),(LONG)(c),(PPOINTL)(d),(ULONG)(e),0L,NGreStrokeInkPath)
#endif
#ifdef INCL_GRE_PAX
   #define GrePaxGetPolyFillMode(a) (ULONG) GreEntry3((ULONG)(a),0L,0x0000425CL)
   #define NGrePaxGetPolyFillMode  0x0000425CL
   #define GrePaxSetPolyFillMode(a,b) (ULONG) GreEntry4((ULONG)(a),(ULONG)(b),0L,0x0000425DL)
   #define NGrePaxSetPolyFillMode  0x0000425DL
   #define GreQueryMitreLimit(a) (FIXED) GreEntry3((ULONG)(a),0L,0x0000425EL)
   #define NGreQueryMitreLimit  0x0000425EL
   #define GreSetMitreLimit(a,b) (FIXED) GreEntry4((ULONG)(a),(ULONG)(b),0L,0x0000425FL)
   #define NGreSetMitreLimit  0x0000425FL
   #define GreEnableYInversion(a,b) (ULONG) GreEntry4((ULONG)(a),(LONG)(b),0L,0x00004260L)
   #define NGreEnableYInversion 0x00004260L
   #define GreQueryYInversion(a) (ULONG) GreEntry3((ULONG)(a),0L,0x00004261L)
   #define NGreQueryYInversion 0x00004261L
   #define GreQueryPath(a,b,c,d) (ULONG) GreEntry6((ULONG)(a),(ULONG)(b),(ULONG)(c),(ULONG)(d),0L,0x00004262L)
   #define NGreQueryPath  0x00004262L
   #define GreAllocateDCData(a,b) (ULONG) GreEntry4((ULONG)(a),(ULONG)(b),0L,0x00004263L)
   #define NGreAllocateDCData  0x00004263L
   #define GreQueryDCData(a) (ULONG) GreEntry3((ULONG)(a),0L,0x00004264L)
   #define NGreQueryDCData  0x00004264L
   #define GreQueryRasterCaps(a) (ULONG) GreEntry3((ULONG)(a), 0L, 0x00000265L)
   #define NGreQueryRasterCaps 0x00000265L
   #define GrePaxStrokeAndFillPath(a,b,c,d) (ULONG) GreEntry6((ULONG)(a),(ULONG)(b),(ULONG)(c),(ULONG)(d), 0L, 0x00004266L)
   #define NGrePaxStrokeAndFillPath 0x00004266L
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
