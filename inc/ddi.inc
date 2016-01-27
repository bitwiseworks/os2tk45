DDI_INCLUDED	EQU	1

RC_SUCCESS	EQU	0 
RC_SIMULATE	EQU	1 
RC_UNSUPPORTED	EQU	2 
RC_ERROR	EQU	3 
RC_ERROR_IN_BACKGROUND	EQU	4 
RC_ERROR_NO_HANDLE	EQU	5 
RC_ERROR_RESOURCE_NOT_FOUND	EQU	6 
RC_ERROR_INVALID_PARAMETER	EQU	7 
RC_ERROR_OUT_OF_MEMORY	EQU	8 
RC_DISABLED	EQU	9 
RC_ENABLED	EQU	10 

BMAPINFO	STRUC
bmapinfo_ulLength	DD	?
bmapinfo_ulType	DD	?
bmapinfo_ulWidth	DD	?
bmapinfo_ulHeight	DD	?
bmapinfo_ulBpp	DD	?
bmapinfo_ulBytesPerLine	DD	?
bmapinfo_pBits	DD	?
BMAPINFO	ENDS

BLTRECT	STRUC
bltrect_ulXOrg	DD	?
bltrect_ulYOrg	DD	?
bltrect_ulXExt	DD	?
bltrect_ulYExt	DD	?
BLTRECT	ENDS
BMAP_VRAM	EQU	00000000H 
BMAP_MEMORY	EQU	00000001H 
BMAP_BANKED	EQU	00000010H 
BMAP_VERTICAL_SCAN	EQU	10000000H 
BMAP_FOCAFONT	EQU	20000000H 
TYPE_GRDBNKINFO	EQU	00000001H 
TYPE_CMYKINFO	EQU	00000002H 

GRDBNKINFO	STRUC
gbi_ulLength	DD	?
gbi_ulType	DD	?
gbi_ulAperture	DD	?
gbi_ulGID	DD	?
gbi_ulCurrBank	DD	?
gbi_ulReserved	DD	?
gbi_pfnBankSwitch	DD	?
gbi_pvReserved	DD	?
GRDBNKINFO	ENDS
CMYK_DEVICE	EQU	00000002H 
CMY_ONLY	EQU	00000004H 
CMYK_INTERLEAVE	EQU	00000008H 

BITBLTINFO	STRUC
bitbltinfo_ulLength	DD	?
bitbltinfo_ulBltFlags	DD	?
bitbltinfo_cBlits	DD	?
bitbltinfo_ulROP	DD	?
bitbltinfo_ulMonoBackROP	DD	?
bitbltinfo_ulSrcFGColor	DD	?
bitbltinfo_ulSrcBGColor	DD	?
bitbltinfo_ulPatFGColor	DD	?
bitbltinfo_ulPatBGColor	DD	?
bitbltinfo_abColors	DD	?
bitbltinfo_pSrcBmapInfo	DD	?
bitbltinfo_pDstBmapInfo	DD	?
bitbltinfo_pPatBmapInfo	DD	?
bitbltinfo_aptlSrcOrg	DD	?
bitbltinfo_aptlPatOrg	DD	?
bitbltinfo_abrDst	DD	?
bitbltinfo_prclSrcBounds	DD	?
bitbltinfo_prclDstBounds	DD	?
BITBLTINFO	ENDS
BF_DEFAULT_STATE	EQU	00000000H   
BF_DIR_X_NEGATIVE	EQU	00000001H 
BF_DIR_RIGHT_LEFT	EQU	BF_DIR_X_NEGATIVE
BF_DIR_Y_NEGATIVE	EQU	00000002H 
BF_DIR_BOTTOM_TOP	EQU	BF_DIR_Y_NEGATIVE
BF_ROP_INCL_SRC	EQU	00000004H 
BF_ROP_INCL_PAT	EQU	00000008H 
BF_SRC_TRANSPARENT	EQU	00000010H 
BF_DST_TRANSPARENT	EQU	00000020H 
BF_PAT_TRANSPARENT	EQU	00000040H 
BF_PAT_SOLID	EQU	00000080H 
BF_PAT_HOLLOW	EQU	00000100H 
BF_APPLY_BACK_ROP	EQU	00000200H 
BF_SRC_MONOINVERT	EQU	00000400H 
BF_PAT_MONOINVERT	EQU	00000800H 
BF_DST_MONOINVERT	EQU	00020000H 
BF_SRC_BITS_EXTERNAL	EQU	00001000H 
BF_LAST_BLT	EQU	00002000H 
BF_SRC_Y_FLIP	EQU	00004000H 
BF_SRC_X_FLIP	EQU	00008000H 
BF_SRC_VERTICAL_SCAN	EQU	00010000H 

LINEPACK	STRUC
linepack_ulStyleStep	DD	?
linepack_ulStyleValue	DD	?
linepack_ulFlags	DD	?
linepack_plpkNext	DD	?
linepack_ulAbsDeltaX	DD	?
linepack_ulAbsDeltaY	DD	?
linepack_ptlClipStart	DB	SIZE POINTL DUP (?)
linepack_ptlClipEnd	DB	SIZE POINTL DUP (?)
linepack_ptlStart	DB	SIZE POINTL DUP (?)
linepack_ptlEnd	DB	SIZE POINTL DUP (?)
linepack_lClipStartError	DD	?
LINEPACK	ENDS
LINE_DO_FIRST_PEL	EQU	00000002H 
LINE_DIR_Y_POSITIVE	EQU	00000004H 
LINE_HORIZONTAL	EQU	00000008H 
LINE_X_MAJOR	EQU	00000010H 
LINE_DIR_X_POSITIVE	EQU	00000020H 
LINE_VERTICAL	EQU	00001000H 
LINE_STYLE_X_MAJOR	EQU	00002000H 
LINE_DO_LAST_PEL	EQU	00004000H 
LINE_MONO_INVERT	EQU	00008000H   
LINE_START_CLIP	EQU	00010000H   
LINE_END_CLIP	EQU	00020000H   
LINE_Y_FLIP	EQU	00040000H   
LINE_ALL_RADIAL	EQU	00080000H   
LINE_DISJOINT	EQU	00100000H   

LINEINFO	STRUC
linfo_ulLength	DD	?
linfo_ulType	DD	?
linfo_ulStyleMask	DD	?
linfo_cLines	DD	?
linfo_ulFGColor	DD	?
linfo_ulBGColor	DD	?
linfo_usForeROP	DW	?
linfo_usBackROP	DW	?
linfo_pDstBmapInfo	DD	?
linfo_alpkLinePack	DD	?
linfo_prclBounds	DD	?
LINEINFO	ENDS

LINEINFO2	STRUC
linfo2_ulLength	DD	?
linfo2_ulType	DD	?
linfo2_ulStyleMask	DD	?
linfo2_cLines	DD	?
linfo2_ulFGColor	DD	?
linfo2_ulBGColor	DD	?
linfo2_usForeROP	DW	?
linfo2_usBackROP	DW	?
linfo2_pDstBmapInfo	DD	?
linfo2_ulFlags	DD	?
linfo2_ulXYStyleStep	DD	?
linfo2_pulStyleValue	DD	?
linfo2_ptlOrigin	DB	SIZE POINTL DUP (?)
linfo2_pptlStart	DD	?
linfo2_pptlLines	DD	?
linfo2_ptlClipStart	DB	SIZE POINTL DUP (?)
linfo2_ptlClipEnd	DB	SIZE POINTL DUP (?)
linfo2_lClipStartError	DD	?
linfo2_prclBounds	DD	?
LINEINFO2	ENDS
LINE_SOLID	EQU	01H 
LINE_INVISIBLE	EQU	02H 
LINE_ALTERNATE	EQU	03H 
LR_ZERO	EQU	0    
LR_INVERTMERGEPAT	EQU	1    
LR_MASKINVERTPAT	EQU	2    
LR_INVERTCOPYPAT	EQU	3    
LR_MASKPATINVERT	EQU	4    
LR_INVERT	EQU	5    
LR_XORPAT	EQU	6    
LR_INVERTMASKPAT	EQU	7    
LR_MASKPAT	EQU	8    
LR_INVERTXORPAT	EQU	9    
LR_LEAVEALONE	EQU	10    
LR_MERGEINVERTPAT	EQU	11    
LR_PATCOPY	EQU	12    
LR_MERGEPATINVERT	EQU	13    
LR_MERGEPAT	EQU	14    
LR_ONE	EQU	15    

GLYPHINFO	STRUC
glphyinfo_bAspace	DB	?
glphyinfo_bBspace	DB	?
glphyinfo_bCspace	DB	?
glphyinfo_bPad	DB	?
glphyinfo_bmapinfo	DB	SIZE BMAPINFO DUP (?)
GLYPHINFO	ENDS

GLBTBL	STRUC
glbtbl_pGlyphInfo	DW	1 DUP (?)
GLBTBL	ENDS

GHBTBL	STRUC
ghbtbl_pglbTbl	DW	1 DUP (?)
GHBTBL	ENDS

DEVFONTINFO	STRUC
dfi_ulFntCnt	DD	?
dfi_fFontInfo	DD	?
dfi_ulEngTag	DD	?
dfi_ulUniqueFntID	DD	?
dfi_ulMaxHeight	DD	?
dfi_ulMaxWidth	DD	?
dfi_pghbTbl	DW	?
dfi_ulHalfWidth	DD	?
dfi_szGlyphlist	DB	16 DUP (?)
dfi_ulReserved1	DD	?
dfi_ulReserved2	DD	?
DEVFONTINFO	ENDS
GRETAG	EQU	('G' + ('R' SHL 8 ) + ('E' SHL 16 ) + ('_' SHL 24 ))    
WINTAG	EQU	('W' + ('I' SHL 8 ) + ('N' SHL 16 ) + ('_' SHL 24 ))    
DFI_FIXED_FONT	EQU	00000001H 
DFI_DBCS_FONT	EQU	00000002H 

TEXTBLTINFO	STRUC
tbi_ulLength	DD	?
tbi_flOptions	DD	?
tbi_lGlyphCnt	DD	?
tbi_pGlyphIndices	DD	?
tbi_ulFGMix	DD	?
tbi_ulBGMix	DD	?
tbi_ulFGColor	DD	?
tbi_ulBGColor	DD	?
tbi_pDstBmapInfo	DD	?
tbi_pDevFntInfo	DW	?
tbi_ulClpCnt	DD	?
tbi_abrClipRects	DD	?
tbi_aptlSrcOrg	DD	?
tbi_abrDst	DD	?
TEXTBLTINFO	ENDS

HWPALETTEINFO	STRUC
hwpaletteinfo_ulLength	DD	?
hwpaletteinfo_fFlags	DD	?
hwpaletteinfo_ulStartIndex	DD	?
hwpaletteinfo_ulNumEntries	DD	?
hwpaletteinfo_pRGBs	DD	?
HWPALETTEINFO	ENDS
PALETTE_GET	EQU	0001H 
PALETTE_SET	EQU	0002H 

HWMOVEPTRIN	STRUC
hwmoveptrin_ulLength	DD	?
hwmoveptrin_ptlPos	DB	SIZE POINTL DUP (?)
HWMOVEPTRIN	ENDS

HWSETPTRIN	STRUC
hwsetptrin_ulLength	DD	?
hwsetptrin_pbANDMask	DD	?
hwsetptrin_pbXORMask	DD	?
hwsetptrin_pbBits	DD	?
hwsetptrin_ulBpp	DD	?
hwsetptrin_ulWidth	DD	?
hwsetptrin_ulHeight	DD	?
hwsetptrin_ptlHotspot	DB	SIZE POINTL DUP (?)
HWSETPTRIN	ENDS

HWSETPTROUT	STRUC
hwsetptrout_ulLength	DD	?
hwsetptrout_ulStatus	DD	?
HWSETPTROUT	ENDS
POINTER_VISIBLE	EQU	0001H 
POINTER_COLOR	EQU	0002H 
POINTER_SOFTWARE	EQU	0004H 
POINTER_FAST	EQU	0008H 

HWSHOWPTRIN	STRUC
hwshowptrin_ulLength	DD	?
hwshowptrin_fShow	DD	?
HWSHOWPTRIN	ENDS
REQUEST_HW	EQU	1H 
REQUEST_SEM_ONLY	EQU	2H 

HWREQIN	STRUC
hwreqin_ulLength	DD	?
hwreqin_ulFlags	DD	?
hwreqin_cScrChangeRects	DD	?
hwreqin_arectlScreen	DD	?
HWREQIN	ENDS

HWBANKIN	STRUC
hwbankin_ulLength	DD	?
hwbankin_ulFlags	DD	?
hwbankin_ulBank	DD	?
HWBANKIN	ENDS
BANK_SET	EQU	1 
BANK_GET	EQU	2 

HWBANKOUT	STRUC
hwbankout_ulLength	DD	?
hwbankout_ulBank	DD	?
HWBANKOUT	ENDS
