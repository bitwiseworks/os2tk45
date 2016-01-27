ifndef __OS2DEF__
OS2DEF_INCLUDED	EQU	1

ifndef FALSE
FALSE	EQU	0 
endif
ifndef TRUE
TRUE	EQU	1 
endif
ifndef NULL
ifdef _cplusplus
NULL	EQU	0 
else
NULL	EQU	(        0 )
endif
endif
NULLHANDLE	EQU	(           0 )
NULLSHANDLE	EQU	(           0 )
ifndef INCL_SAADEFS
endif 
ifndef INCL_SAADEFS
endif 
ifndef INCL_SAADEFS
endif 
ifndef INCL_SAADEFS
endif  
SEVERITY_NOERROR	EQU	0000H 
SEVERITY_WARNING	EQU	0004H 
SEVERITY_ERROR	EQU	0008H 
SEVERITY_SEVERE	EQU	000CH 
SEVERITY_UNRECOVERABLE	EQU	0010H 
WINERR_BASE	EQU	1000H   
GPIERR_BASE	EQU	2000H   
DEVERR_BASE	EQU	3000H   
SPLERR_BASE	EQU	4000H   
ifndef INCL_SAADEFS
endif  
ifndef INCL_SAADEFS
ifndef __HSEM__
endif
endif  

POINTL	STRUC
ptl_x	DD	?
ptl_y	DD	?
POINTL	ENDS

POINTS	STRUC
pts_x	DW	?
pts_y	DW	?
POINTS	ENDS

RECTL	STRUC
rcl_xLeft	DD	?
rcl_yBottom	DD	?
rcl_xRight	DD	?
rcl_yTop	DD	?
RECTL	ENDS

MATRIXLF	STRUC
matlf_fxM11	DD	?
matlf_fxM12	DD	?
matlf_lM13	DD	?
matlf_fxM21	DD	?
matlf_fxM22	DD	?
matlf_lM23	DD	?
matlf_lM31	DD	?
matlf_lM32	DD	?
matlf_lM33	DD	?
MATRIXLF	ENDS

DRIVDATA	STRUC
driv_cb	DD	?
driv_lVersion	DD	?
driv_szDeviceName	DB	32 DUP (?)
driv_abGeneralData	DB	1 DUP (?)
DRIVDATA	ENDS
ADDRESS	EQU	0 
ifndef INCL_SAADEFS
DRIVER_NAME	EQU	1 
DRIVER_DATA	EQU	2 
DATA_TYPE	EQU	3 
COMMENT	EQU	4 
PROC_NAME	EQU	5 
PROC_PARAMS	EQU	6 
SPL_PARAMS	EQU	7 
NETWORK_PARAMS	EQU	8 

DEVOPENSTRUC	STRUC
dop_pszLogAddress	DD	?
dop_pszDriverName	DD	?
dop_pdriv	DD	?
dop_pszDataType	DD	?
dop_pszComment	DD	?
dop_pszQueueProcName	DD	?
dop_pszQueueProcParams	DD	?
dop_pszSpoolerParams	DD	?
dop_pszNetworkParams	DD	?
DEVOPENSTRUC	ENDS
endif  

PRINTDEST	STRUC
prntdst_cb	DD	?
prntdst_lType	DD	?
prntdst_pszToken	DD	?
prntdst_lCount	DD	?
prntdst_pdopData	DD	?
prntdst_fl	DD	?
prntdst_pszPrinter	DD	?
PRINTDEST	ENDS
PD_JOB_PROPERTY	EQU	0001H       
FATTR_SEL_ITALIC	EQU	0001H 
FATTR_SEL_UNDERSCORE	EQU	0002H 
FATTR_SEL_OUTLINE	EQU	0008H 
FATTR_SEL_STRIKEOUT	EQU	0010H 
FATTR_SEL_BOLD	EQU	0020H 
FATTR_SEL_MUST_COLOR	EQU	0100H  
FATTR_SEL_MUST_MIXEDMODES	EQU	0200H  
FATTR_SEL_MUST_HOLLOW	EQU	0400H  
FATTR_TYPE_KERNING	EQU	0004H 
FATTR_TYPE_MBCS	EQU	0008H 
FATTR_TYPE_DBCS	EQU	0010H 
FATTR_TYPE_ANTIALIASED	EQU	0020H 
FATTR_FONTUSE_NOMIX	EQU	0002H 
FATTR_FONTUSE_OUTLINE	EQU	0004H 
FATTR_FONTUSE_TRANSFORMABLE	EQU	0008H 
FACESIZE	EQU	32 

FATTRS	STRUC
fat_usRecordLength	DW	?
fat_fsSelection	DW	?
fat_lMatch	DD	?
fat_szFacename	DB	FACESIZE DUP (?)
fat_idRegistry	DW	?
fat_usCodePage	DW	?
fat_lMaxBaselineExt	DD	?
fat_lAveCharWidth	DD	?
fat_fsType	DW	?
fat_fsFontUse	DW	?
FATTRS	ENDS
FM_TYPE_FIXED	EQU	0001H 
FM_TYPE_LICENSED	EQU	0002H 
FM_TYPE_KERNING	EQU	0004H 
FM_TYPE_DBCS	EQU	0010H 
FM_TYPE_MBCS	EQU	0018H 
FM_TYPE_UNICODE	EQU	0040H 
FM_TYPE_64K	EQU	8000H 
FM_TYPE_ATOMS	EQU	4000H 
FM_TYPE_FAMTRUNC	EQU	2000H 
FM_TYPE_FACETRUNC	EQU	1000H 
FM_DEFN_OUTLINE	EQU	0001H 
FM_DEFN_IFI	EQU	0002H 
FM_DEFN_WIN	EQU	0004H 
FM_DEFN_GENERIC	EQU	8000H 
FM_DEFN_LATIN1	EQU	0010H    
FM_DEFN_PC	EQU	0020H    
FM_DEFN_LATIN2	EQU	0040H    
FM_DEFN_CYRILLIC	EQU	0080H    
FM_DEFN_HEBREW	EQU	0100H    
FM_DEFN_GREEK	EQU	0200H    
FM_DEFN_ARABIC	EQU	0400H    
FM_DEFN_UGLEXT	EQU	0800H    
FM_DEFN_KANA	EQU	1000H    
FM_DEFN_THAI	EQU	2000H    
FM_DEFN_UGL383	EQU	0070H    
FM_DEFN_UGL504	EQU	00F0H    
FM_DEFN_UGL767	EQU	0FF0H    
FM_DEFN_UGL1105	EQU	3FF0H    
FM_SEL_ITALIC	EQU	0001H 
FM_SEL_UNDERSCORE	EQU	0002H 
FM_SEL_NEGATIVE	EQU	0004H 
FM_SEL_OUTLINE	EQU	0008H           
FM_SEL_STRIKEOUT	EQU	0010H 
FM_SEL_BOLD	EQU	0020H 
FM_SEL_ISO9241_TESTED	EQU	0040H 
FM_SEL_JAPAN	EQU	1000H 
FM_SEL_TAIWAN	EQU	2000H 
FM_SEL_CHINA	EQU	4000H 
FM_SEL_KOREA	EQU	8000H 
FM_SEL_DBCSMASK	EQU	0F000H 
FM_ISO_9518_640	EQU	01H 
FM_ISO_9515_640	EQU	02H 
FM_ISO_9515_1024	EQU	04H 
FM_ISO_9517_640	EQU	08H 
FM_ISO_9517_1024	EQU	10H 
FM_CAP_NOMIX	EQU	0001H 
FM_CAP_NO_COLOR	EQU	0002H 
FM_CAP_NO_MIXEDMODES	EQU	0004H 
FM_CAP_NO_HOLLOW	EQU	0008H 

PANOSE	STRUC
panose_bFamilyType	DB	?
panose_bSerifStyle	DB	?
panose_bWeight	DB	?
panose_bProportion	DB	?
panose_bContrast	DB	?
panose_bStrokeVariation	DB	?
panose_bArmStyle	DB	?
panose_bLetterform	DB	?
panose_bMidline	DB	?
panose_bXHeight	DB	?
panose_fbPassedISO	DB	?
panose_fbFailedISO	DB	?
PANOSE	ENDS

FONTMETRICS	STRUC
fm_szFamilyname	DB	FACESIZE DUP (?)
fm_szFacename	DB	FACESIZE DUP (?)
fm_idRegistry	DW	?
fm_usCodePage	DW	?
fm_lEmHeight	DD	?
fm_lXHeight	DD	?
fm_lMaxAscender	DD	?
fm_lMaxDescender	DD	?
fm_lLowerCaseAscent	DD	?
fm_lLowerCaseDescent	DD	?
fm_lInternalLeading	DD	?
fm_lExternalLeading	DD	?
fm_lAveCharWidth	DD	?
fm_lMaxCharInc	DD	?
fm_lEmInc	DD	?
fm_lMaxBaselineExt	DD	?
fm_sCharSlope	DW	?
fm_sInlineDir	DW	?
fm_sCharRot	DW	?
fm_usWeightClass	DW	?
fm_usWidthClass	DW	?
fm_sXDeviceRes	DW	?
fm_sYDeviceRes	DW	?
fm_sFirstChar	DW	?
fm_sLastChar	DW	?
fm_sDefaultChar	DW	?
fm_sBreakChar	DW	?
fm_sNominalPointSize	DW	?
fm_sMinimumPointSize	DW	?
fm_sMaximumPointSize	DW	?
fm_fsType	DW	?
fm_fsDefn	DW	?
fm_fsSelection	DW	?
fm_fsCapabilities	DW	?
fm_lSubscriptXSize	DD	?
fm_lSubscriptYSize	DD	?
fm_lSubscriptXOffset	DD	?
fm_lSubscriptYOffset	DD	?
fm_lSuperscriptXSize	DD	?
fm_lSuperscriptYSize	DD	?
fm_lSuperscriptXOffset	DD	?
fm_lSuperscriptYOffset	DD	?
fm_lUnderscoreSize	DD	?
fm_lUnderscorePosition	DD	?
fm_lStrikeoutSize	DD	?
fm_lStrikeoutPosition	DD	?
fm_sKerningPairs	DW	?
fm_sFamilyClass	DW	?
fm_lMatch	DD	?
fm_FamilyNameAtom	DD	?
fm_FaceNameAtom	DD	?
fm_panose	DB	SIZE PANOSE DUP (?)
FONTMETRICS	ENDS

ICONINFO	STRUC
icninf_cb	DD	?
icninf_fFormat	DD	?
icninf_pszFileName	DD	?
icninf_hmod	DD	?
icninf_resid	DD	?
icninf_cbIconData	DD	?
icninf_pIconData	DD	?
ICONINFO	ENDS
ICON_FILE	EQU	1          
ICON_RESOURCE	EQU	2 
ICON_DATA	EQU	3 
ICON_CLEAR	EQU	4 

ACCEL	STRUC
acc_fs	DW	?
acc_key	DW	?
acc_cmd	DW	?
ACCEL	ENDS
endif 

ifdef @Version
if @Version lt 600

if1
ifndef LONGLONG_PASS1
LONGLONG_PASS1       EQU     1
INCLUDEIT = 1
endif
endif
if2
ifndef LONGLONG_PASS2
LONGLONG_PASS2       EQU     1
INCLUDEIT = 1
endif
endif

else
ifndef INCLUDEIT
INCLUDEIT = 1
endif
endif

else
ifndef INCLUDEIT
INCLUDEIT = 1
endif
endif

ifndef INCL_LONGLONG
if INCLUDEIT   

LONGLONG	STRUC
LONGLONG_ulLo	DD	?
LONGLONG_ulHi	DD	?
LONGLONG	ENDS

ULONGLONG	STRUC
ULONGLONG_ulLo	DD	?
ULONGLONG_ulHi	DD	?
ULONGLONG	ENDS
INCLUDEIT = 0
endif

endif 
