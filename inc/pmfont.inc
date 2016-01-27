PMFONT_INCLUDED	EQU	1


FONTRANGEENTRY	STRUC
fre_ulRun	DD	?
fre_ulSkip	DD	?
FONTRANGEENTRY	ENDS

FONTCHARDEF	STRUC
fcd_ulGlyphOffset	DD	?
fcd_sAspace	DW	?
fcd_sBspace	DW	?
fcd_sCspace	DW	?
FONTCHARDEF	ENDS

FOCAMETRICSEXT	STRUC
fme_ulSize	DD	?
fme_ulFlags	DD	?
fme_ulGlyphCount	DD	?
fme_ulDefaultIndex	DD	?
fme_ulRangeTableEntries	DD	?
fme_afreRangeTable	DB	SIZE FONTRANGEENTRY * 1 DUP (?)
FOCAMETRICSEXT	ENDS

FOCAMETRICS	STRUC
foca_ulIdentity	DD	?
foca_ulSize	DD	?
foca_szFamilyname	DB	32 DUP (?)
foca_szFacename	DB	32 DUP (?)
foca_usRegistryId	DW	?
foca_usCodePage	DW	?
foca_yEmHeight	DW	?
foca_yXHeight	DW	?
foca_yMaxAscender	DW	?
foca_yMaxDescender	DW	?
foca_yLowerCaseAscent	DW	?
foca_yLowerCaseDescent	DW	?
foca_yInternalLeading	DW	?
foca_yExternalLeading	DW	?
foca_xAveCharWidth	DW	?
foca_xMaxCharInc	DW	?
foca_xEmInc	DW	?
foca_yMaxBaselineExt	DW	?
foca_sCharSlope	DW	?
foca_sInlineDir	DW	?
foca_sCharRot	DW	?
foca_usWeightClass	DW	?
foca_usWidthClass	DW	?
foca_xDeviceRes	DW	?
foca_yDeviceRes	DW	?
foca_usFirstChar	DW	?
foca_usLastChar	DW	?
foca_usDefaultChar	DW	?
foca_usBreakChar	DW	?
foca_usNominalPointSize	DW	?
foca_usMinimumPointSize	DW	?
foca_usMaximumPointSize	DW	?
foca_fsTypeFlags	DW	?
foca_fsDefn	DW	?
foca_fsSelectionFlags	DW	?
foca_fsCapabilities	DW	?
foca_ySubscriptXSize	DW	?
foca_ySubscriptYSize	DW	?
foca_ySubscriptXOffset	DW	?
foca_ySubscriptYOffset	DW	?
foca_ySuperscriptXSize	DW	?
foca_ySuperscriptYSize	DW	?
foca_ySuperscriptXOffset	DW	?
foca_ySuperscriptYOffset	DW	?
foca_yUnderscoreSize	DW	?
foca_yUnderscorePosition	DW	?
foca_yStrikeoutSize	DW	?
foca_yStrikeoutPosition	DW	?
foca_usKerningPairs	DW	?
foca_sFamilyClass	DW	?
foca_pszDeviceNameOffset	DD	?
FOCAMETRICS	ENDS

FONTFILEMETRICS	STRUC
ffm_ulIdentity	DD	?
ffm_ulSize	DD	?
ffm_szFamilyname	DB	32 DUP (?)
ffm_szFacename	DB	32 DUP (?)
ffm_usRegistryId	DW	?
ffm_usCodePage	DW	?
ffm_yEmHeight	DW	?
ffm_yXHeight	DW	?
ffm_yMaxAscender	DW	?
ffm_yMaxDescender	DW	?
ffm_yLowerCaseAscent	DW	?
ffm_yLowerCaseDescent	DW	?
ffm_yInternalLeading	DW	?
ffm_yExternalLeading	DW	?
ffm_xAveCharWidth	DW	?
ffm_xMaxCharInc	DW	?
ffm_xEmInc	DW	?
ffm_yMaxBaselineExt	DW	?
ffm_sCharSlope	DW	?
ffm_sInlineDir	DW	?
ffm_sCharRot	DW	?
ffm_usWeightClass	DW	?
ffm_usWidthClass	DW	?
ffm_xDeviceRes	DW	?
ffm_yDeviceRes	DW	?
ffm_usFirstChar	DW	?
ffm_usLastChar	DW	?
ffm_usDefaultChar	DW	?
ffm_usBreakChar	DW	?
ffm_usNominalPointSize	DW	?
ffm_usMinimumPointSize	DW	?
ffm_usMaximumPointSize	DW	?
ffm_fsTypeFlags	DW	?
ffm_fsDefn	DW	?
ffm_fsSelectionFlags	DW	?
ffm_fsCapabilities	DW	?
ffm_ySubscriptXSize	DW	?
ffm_ySubscriptYSize	DW	?
ffm_ySubscriptXOffset	DW	?
ffm_ySubscriptYOffset	DW	?
ffm_ySuperscriptXSize	DW	?
ffm_ySuperscriptYSize	DW	?
ffm_ySuperscriptXOffset	DW	?
ffm_ySuperscriptYOffset	DW	?
ffm_yUnderscoreSize	DW	?
ffm_yUnderscorePosition	DW	?
ffm_yStrikeoutSize	DW	?
ffm_yStrikeoutPosition	DW	?
ffm_usKerningPairs	DW	?
ffm_sFamilyClass	DW	?
ffm_ulReserved	DD	?
ffm_panose	DB	SIZE PANOSE DUP (?)
FONTFILEMETRICS	ENDS

FONTDEFINITIONHEADER	STRUC
fdh_ulIdentity	DD	?
fdh_ulSize	DD	?
fdh_fsFontdef	DW	?
fdh_fsChardef	DW	?
fdh_usCellSize	DW	?
fdh_xCellWidth	DW	?
fdh_yCellHeight	DW	?
fdh_xCellIncrement	DW	?
fdh_xCellA	DW	?
fdh_xCellB	DW	?
fdh_xCellC	DW	?
fdh_pCellBaseOffset	DW	?
FONTDEFINITIONHEADER	ENDS
FONTDEFFONT1	EQU	0047H   
FONTDEFFONT2	EQU	0042H   
FONTDEFFONT3	EQU	0042H   
FONTDEFCHAR1	EQU	0081H   
FONTDEFCHAR2	EQU	0081H   
FONTDEFCHAR3	EQU	00b8H   
SPACE_UNDEF	EQU	8000H   
FONTDEFFOCA32	EQU	4000H 
FONTDEFDEVFONT	EQU	2000H   

FONTSIGNATURE	STRUC
fs_ulIdentity	DD	?
fs_ulSize	DD	?
fs_achSignature	DB	12 DUP (?)
FONTSIGNATURE	ENDS

ADDITIONALMETRICS	STRUC
am_ulIdentity	DD	?
am_ulSize	DD	?
am_panose	DB	SIZE PANOSE DUP (?)
ADDITIONALMETRICS	ENDS

FOCAFONT	STRUC
ff_fsSignature	DB	SIZE FONTSIGNATURE DUP (?)
ff_fmMetrics	DB	SIZE FOCAMETRICS DUP (?)
ff_fdDefinitions	DB	SIZE FONTDEFINITIONHEADER DUP (?)
FOCAFONT	ENDS
FONT_SIGNATURE	EQU	0fffffffeH  
FONT_METRICS	EQU	00000001H  
FONT_DEFINITION	EQU	00000002H  
FONT_KERNPAIRS	EQU	00000003H  
FONT_ADDITIONALMETRICS	EQU	00000004H  
FONT_ENDRECORD	EQU	0ffffffffH  
QUERY_PUBLIC_FONTS	EQU	0001H 
QUERY_PRIVATE_FONTS	EQU	0002H 
CDEF_GENERIC	EQU	0001H 
CDEF_BOLD	EQU	0002H 
CDEF_ITALIC	EQU	0004H 
CDEF_UNDERSCORE	EQU	0008H 
CDEF_STRIKEOUT	EQU	0010H 
CDEF_OUTLINE	EQU	0020H 
