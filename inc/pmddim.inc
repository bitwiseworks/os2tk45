DDIM_INCLUDED	EQU	1

ifdef INCL_GRE_ARCS
NGreGetArcParameters	EQU	00004000H 
NGreSetArcParameters	EQU	00004001H 
NGreArc	EQU	00004002H 
NGrePartialArc	EQU	00004003H 
NGreFullArcInterior	EQU	00004004H 
NGreFullArcBoundary	EQU	00004005H 
NGreFullArcBoth	EQU	00004006H 
NGreBoxInterior	EQU	00004007H 
NGreBoxBoundary	EQU	00004008H 
NGreBoxBoth	EQU	00004009H 
NGreBoxExclusive	EQU	00004010H 
NGrePolyFillet	EQU	0000400AH 
NGrePolyFilletSharp	EQU	0000400BH 
NGrePolySpline	EQU	0000400CH 
NGreOval	EQU	0000400DH 
NGreCookWholePath	EQU	0000400EH 
NGreCookPathCurves	EQU	0000400FH 
NGreRenderPath	EQU	00004011H 
endif
ifdef INCL_GRE_LINES
NGreDisjointLines	EQU	00004016H 
NGreGetCurrentPosition	EQU	00004017H 
NGreSetCurrentPosition	EQU	00004018H 
NGrePolyLine	EQU	00004019H 
NGreDrawLinesInPath	EQU	0000401AH 
NGrePolyShortLine	EQU	0000401BH 
endif
ifdef INCL_GRE_SCANS
NGrePolyScanline	EQU	0000401CH 
endif
ifdef INCL_GRE_BITMAPS
NGreDrawBits	EQU	00006022H 
NGreDeviceCreateBitmap	EQU	00006023H 
NGreDeviceDeleteBitmap	EQU	00004024H 
NGreDeviceSelectBitmap	EQU	00004025H 
NGreBitblt	EQU	00006026H 
NGreGetPel	EQU	00006027H 
NGreSetPel	EQU	00004028H 
NGreImageData	EQU	00004029H 
NGreScanLR	EQU	0000602AH 
NGreFloodFill	EQU	0000602BH 
NGreSaveScreenBits	EQU	0000402CH 
NGreRestoreScreenBits	EQU	0000402DH 
NGreDrawBorder	EQU	0000602EH 
NGreDeviceSetCursor	EQU	0000402FH 
NGreGetBitmapBits	EQU	00006030H 
NGreSetBitmapBits	EQU	00006031H 
endif
ifdef INCL_WINPOINTERS
NGreSetColorCursor	EQU	00004032H 
endif
ifdef INCL_GRE_STRINGS
NGreCharString	EQU	00005035H 
NGreCharStringPos	EQU	00007036H 
NGreQueryTextBox	EQU	00005037H 
NGreQueryCharPositions	EQU	00005038H 
NGreQueryWidthTable	EQU	00005039H 
NGreQueryCharOutline	EQU	00004256H 
NGreQueryCharMetricsTable	EQU	00004258H 
NGreRealizeString	EQU	00004257H 
NGreQueryTabbedTextExtent	EQU	00004269H 
NGreTabbedCharStringAt	EQU	0000426AH 
endif
ifdef INCL_GRE_MARKERS
NGrePolyMarker	EQU	0000403AH 
endif
ifdef INCL_AVIOP
NGreCharRect	EQU	0000403BH 
NGreCharStr	EQU	0000403CH 
NGreScrollRect	EQU	0000403DH 
NGreUpdateCursor	EQU	0000403EH 
endif
ifdef INCL_WINPOINTERS
NGreDeviceSetPointer	EQU	0000403FH 
endif
ifdef INCL_GRE_PATHS
NGreBeginArea	EQU	00004046H 
NGreEndArea	EQU	00004047H 
NGreBeginPath	EQU	00004048H 
NGreEndPath	EQU	00004049H 
NGreCloseFigure	EQU	0000404AH 
NGreFillPath	EQU	0000404BH 
NGreOutlinePath	EQU	0000404CH 
NGreModifyPath	EQU	0000404DH 
NGreStrokePath	EQU	0000404EH 
NGreSelectClipPath	EQU	0000404FH 
NGreSavePath	EQU	00004050H 
NGreRestorePath	EQU	00004051H 
NGreClip1DPath	EQU	00004052H 
NGreDrawRawPath	EQU	00004053H 
NGreDrawCookedPath	EQU	00004054H 
NGreAreaSetAttributes	EQU	00006055H 
NGrePathToRegion	EQU	00004057H 
endif
ifdef INCL_GRE_POLYGON
NGrePolygonSet	EQU	00004056H 
NGreDrawRLE	EQU	00004058H 
NGreDevicePolygonSet	EQU	00004059H 
NGrePolygon3d	EQU	0000405CH 
endif
ifdef INCL_GRE_REGIONS
NGreGetRegionBox	EQU	0000405DH 
NGreGetRegionRects	EQU	0000405EH 
NGreOffsetRegion	EQU	0000405FH 
NGrePtInRegion	EQU	00004060H 
NGreRectInRegion	EQU	00004061H 
NGreCreateRectRegion	EQU	00004062H 
NGreDestroyRegion	EQU	00004063H 
NGreSetRectRegion	EQU	00004064H 
NGreCombineRegion	EQU	00004065H 
NGreCombineRectRegion	EQU	00004066H 
NGreCombineShortLineRegion	EQU	00004067H 
NGreEqualRegion	EQU	00004068H 
NGrePaintRegion	EQU	00004069H 
NGreSetRegionOwner	EQU	0000406AH 
NGreFrameRegion	EQU	0000406BH 
endif
ifdef INCL_GRE_CLIP
NGreGetClipBox	EQU	0000406EH 
NGreGetClipRects	EQU	0000406FH 
NGreOffsetClipRegion	EQU	00004070H 
NGrePtVisible	EQU	00004071H 
NGreRectVisible	EQU	00004072H 
NGreQueryClipRegion	EQU	00004073H 
NGreSelectClipRegion	EQU	00004074H 
NGreIntersectClipRectangle	EQU	00004075H 
NGreExcludeClipRectangle	EQU	00004076H 
NGreSetXformRect	EQU	00004077H 
NGreSaveRegion	EQU	0000407BH 
NGreRestoreRegion	EQU	0000407CH 
NGreClipPathCurves	EQU	0000407DH 
NGreSelectPathRegion	EQU	0000407EH 
NGreRegionSelectBitmap	EQU	0000407FH 
NGreCopyClipRegion	EQU	00004080H 
NGreSetupDC	EQU	00004081H 
endif
ifdef INCL_GRE_XFORMS
NGreGetPageUnits	EQU	00004083H 
NGreSetPageUnits	EQU	00004084H 
NGreGetModelXform	EQU	00004085H 
NGreSetModelXform	EQU	00004086H 
NGreGetWindowViewportXform	EQU	00004087H 
NGreSetWindowViewportXform	EQU	00004088H 
NGreGetGlobalViewingXform	EQU	00004089H 
NGreSetGlobalViewingXform	EQU	0000408AH 
NGreSaveXformData	EQU	0000408BH 
NGreRestoreXformData	EQU	0000408CH 
NGreGetPageViewport	EQU	0000408DH 
NGreSetPageViewport	EQU	0000408EH 
NGreGetGraphicsField	EQU	00004091H 
NGreSetGraphicsField	EQU	00004092H 
NGreGetViewingLimits	EQU	00004093H 
NGreSetViewingLimits	EQU	00004094H 
NGreQueryViewportSize	EQU	00004095H 
NGreConvert	EQU	00004096H 
NGreConvertPath	EQU	00004097H 
NGreSaveXform	EQU	00004098H 
NGreRestoreXform	EQU	00004099H 
NGreMultiplyXforms	EQU	0000409AH 
NGreConvertWithMatrix	EQU	0000409BH 
endif
ifdef INCL_GRE_DEVMISC1
NGreDeviceGetAttributes	EQU	0000609DH 
NGreDeviceSetAVIOFont2	EQU	0000409EH 
NGreGetPairKerningTable	EQU	000040A0H 
NGreDeviceSetAVIOFont	EQU	000040A1H 
NGreDeviceSetAttributes	EQU	000060A2H 
NGreDeviceSetGlobalAttribute	EQU	000060A3H 
endif
ifdef INCL_GRE_DEVMISC2
NGreNotifyClipChange	EQU	000040A4H 
NGreNotifyTransformChange	EQU	000040A5H 
NGreRealizeFont	EQU	000040A6H 
NGreErasePS	EQU	000040A7H 
NGreSetStyleRatio	EQU	000040A8H 
NGreDeviceQueryFontAttributes	EQU	000040A9H 
NGreDeviceQueryFonts	EQU	000040AAH 
NGreDeviceInvalidateVisRegion	EQU	000040ABH 
NGreSetFontMappingFlags	EQU	00004268H 
endif
ifdef INCL_GRE_PICK
NGreGetPickWindow	EQU	000040ACH 
NGreSetPickWindow	EQU	000040ADH 
endif
ifdef INCL_GRE_DEVMISC3
NGreResetBounds	EQU	000040AEH 
NGreGetBoundsData	EQU	000040AFH 
NGreAccumulateBounds	EQU	000040B0H 
NGreGetExtraError	EQU	000040B1H 
NGreSetExtraError	EQU	000040B2H 
NGreGetCodePage	EQU	000040B3H 
NGreSetCodePage	EQU	000040B4H 
NGreLockDevice	EQU	000040B5H 
NGreUnlockDevice	EQU	000040B6H 
NGreDeath	EQU	000040B7H 
NGreResurrection	EQU	000040B8H 
NGreGetDCOrigin	EQU	000040BAH 
NGreDeviceSetDCOrigin	EQU	000040BBH 
NGreGetLineOrigin	EQU	000040BCH 
NGreSetLineOrigin	EQU	000040BDH 
endif
ifdef INCL_GRE_DEVMISC2
NGreGetStyleRatio	EQU	000040BEH 
endif
ifdef INCL_GRE_COLORTABLE
NGreQueryColorData	EQU	000060C3H 
NGreQueryLogColorTable	EQU	000060C4H 
NGreCreateLogColorTable	EQU	000060C5H 
NGreRealizeColorTable	EQU	000060C6H 
NGreUnrealizeColorTable	EQU	000060C7H 
NGreQueryRealColors	EQU	000040C8H 
NGreQueryNearestColor	EQU	000040C9H 
NGreQueryColorIndex	EQU	000060CAH 
NGreQueryRGBColor	EQU	000060CBH 
endif
ifdef INCL_GRE_DEVICE
ifndef NGreQueryDevFntMetrics
NGreQueryDevFntMetrics	EQU	000060CCH 
endif
endif
ifdef INCL_GRE_PAX
NGreDeviceEnableYInversion	EQU	000040CDH 
endif
ifdef INCL_GRE_DEVICE
NGreQueryDeviceBitmaps	EQU	000040D0H 
NGreQueryDeviceCaps	EQU	000040D1H 
NGreEscape	EQU	000040D2H 
NGreQueryHardcopyCaps	EQU	000040D3H 
NGreQueryDevResource2	EQU	000040D4H 
NGreUnloadPD	EQU	0000026EH 
endif
ifdef INCL_GRE_SCREEN
NGreOpenScreenChangeArea	EQU	00004012H 
NGreGetScreenChangeArea	EQU	00004013H 
NGreCloseScreenChangeArea	EQU	00004014H 
NGreGetScreenBits	EQU	0000401DH 
NGreSetScreenBits	EQU	0000401EH 
endif
ifdef INCL_GRE_PALETTE
NGreDeviceCreatePalette	EQU	000040D5H 
NGreDeviceDeletePalette	EQU	000040D6H 
NGreDeviceSetPaletteEntries	EQU	000040D7H 
NGreDeviceAnimatePalette	EQU	000040D8H 
NGreDeviceResizePalette	EQU	000040D9H 
NGreRealizePalette	EQU	000040DAH 
NGreQueryHWPaletteInfo	EQU	000040DBH 
NGreUpdateColors	EQU	000040DCH 
NGreQueryPaletteRealization	EQU	000040DDH 
NGreGetVisRects	EQU	000040DEH 
NGreQueryNearestPaletteIndex	EQU	00000267H 
endif
ifdef INCL_GRE_DCS
NGreOpenDC	EQU	00000200H 
NGreCloseDC	EQU	00004201H 
NGreResetDC	EQU	00004202H 
NGreResetDC2	EQU	0000426BH 
NGreGetHandle	EQU	00004203H 
NGreSetHandle	EQU	00004204H 
NGreGetProcessControl	EQU	00004205H 
NGreSetProcessControl	EQU	00004206H 
NGreSaveDC	EQU	00004207H 
NGreRestoreDC	EQU	00004208H 
NGreQueryDeviceNames	EQU	00000209H 
NGreQueryEngineVersion	EQU	0000020AH 
NGreSetDCOwner	EQU	0000420BH 
endif
ifdef INCL_GRE_DEVSUPPORT
NGreQueryBitmapSelection	EQU	0000020CH 
NGreGetBitmapInfoHeader	EQU	0000020DH 
NGreConvertBitmapData	EQU	0000420EH 
NGreInvalidateVisRegion	EQU	0000420FH 
NGreCreateBitmap	EQU	00004210H 
NGreDeleteBitmap	EQU	00000211H 
NGreSelectBitmap	EQU	00004212H 
NGreGetBitmapParameters	EQU	00000213H 
NGreGetBitmapDimension	EQU	00000214H 
NGreSetBitmapDimension	EQU	00000215H 
NGreSetCursor	EQU	00004216H 
NGreGetAttributes	EQU	00004217H 
NGreSetAttributes	EQU	00004218H 
NGreSetGlobalAttribute	EQU	00004219H 
NGreSetBitmapOwner	EQU	0000021AH 
NGreGetDefaultAttributes	EQU	0000421BH 
NGreSetDefaultAttributes	EQU	0000421CH 
NGreGetDefaultArcParameters	EQU	0000421DH 
NGreSetDefaultArcParameters	EQU	0000421EH 
NGreGetDefaultViewingLimits	EQU	0000421FH 
NGreSetDefaultViewingLimits	EQU	00004220H 
NGreInitializeAttributes	EQU	00004221H 
endif
ifdef INCL_GRE_SETID
NGreDeleteSetId	EQU	00004222H 
NGreQueryNumberSetIds	EQU	00004223H 
NGreQuerySetIds	EQU	00004224H 
endif
ifdef INCL_GRE_LCID
NGreQueryBitmapHandle	EQU	00000225H 
NGreSetBitmapID	EQU	00004226H 
NGreCopyDCLoadData	EQU	00004227H 
endif
ifdef INCL_GRE_SETID
NGreQuerySetIdUsage	EQU	00004228H 
endif
ifdef INCL_GRE_FONTS
NGreQueryFullFontFileDescriptions	EQU	0000022BH 
NGreQueryLogicalFont	EQU	0000422CH 
NGreCreateLogicalFont	EQU	0000422DH 
NGreLoadFont	EQU	0000022EH 
NGreUnloadFont	EQU	0000022FH 
NGreQueryFonts	EQU	00004230H 
NGreQueryFontAttributes	EQU	00004231H 
NGreInstallIFI	EQU	00000232H 
NGreLoadPublicFont	EQU	00000233H 
NGreUnLoadPublicFont	EQU	00000234H 
NGreQueryCodePageVector	EQU	00000235H 
NGreQueryFontFileDescriptions	EQU	00000236H 
NGreQueryFaceString	EQU	00004237H 
NGreQueryFontAction	EQU	00000238H 
NGreQueryCodePageObject	EQU	00000255H 
endif
ifdef INCL_GRE_JOURNALING
NGreAccumulateJournalFile	EQU	00000239H 
NGreCreateJournalFile	EQU	0000023AH 
NGreDeleteJournalFile	EQU	0000023BH 
NGreStartJournalFile	EQU	0000023CH 
NGreStopJournalFile	EQU	0000023DH 
NGrePlayJournalFile	EQU	0000023EH 
NGreOpenJournalFile	EQU	0000023FH 
endif
ifdef INCL_GRE_PALETTE
NGreCreatePalette	EQU	00000240H 
NGreDeletePalette	EQU	00000241H 
NGreSelectPalette	EQU	00004242H 
NGreSetPaletteEntries	EQU	00000243H 
NGreAnimatePalette	EQU	00000244H 
NGreQueryPalette	EQU	00004245H 
NGreQueryPaletteInfo	EQU	00000246H 
NGreSetPaletteOwner	EQU	00000247H 
NGreResizePalette	EQU	00000248H 
NGreQueryDefaultPaletteInfo	EQU	00004249H 
endif
ifdef INCL_GRE_HFONT
NGreCreateFontHandle	EQU	00000250H 
NGreQueryFontHandle	EQU	00000251H 
NGreSelectFontHandle	EQU	00004252H 
NGreDeleteFontHandle	EQU	00000253H 
NGreSetFontHandleOwner	EQU	00000254H 
endif
ifdef INCL_GRE_INKPATH
NGreBeginInkPath	EQU	00004259H 
NGreEndInkPath	EQU	0000425AH 
NGreStrokeInkPath	EQU	0000425BH 
endif
ifdef INCL_GRE_PAX
NGrePaxGetPolyFillMode	EQU	0000425CH 
NGrePaxSetPolyFillMode	EQU	0000425DH 
NGreQueryMitreLimit	EQU	0000425EH 
NGreSetMitreLimit	EQU	0000425FH 
NGreEnableYInversion	EQU	00004260H 
NGreQueryYInversion	EQU	00004261H 
NGreQueryPath	EQU	00004262H 
NGreAllocateDCData	EQU	00004263H 
NGreQueryDCData	EQU	00004264H 
NGreQueryRasterCaps	EQU	00000265H 
NGrePaxStrokeAndFillPath	EQU	00004266H 
endif
