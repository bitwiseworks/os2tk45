PMBITMAP_INCLUDED	EQU	1


BITMAPINFOHEADER	STRUC
bmp_cbFix	DD	?
bmp_cx	DW	?
bmp_cy	DW	?
bmp_cPlanes	DW	?
bmp_cBitCount	DW	?
BITMAPINFOHEADER	ENDS

RGB	STRUC
rgb_bBlue	DB	?
rgb_bGreen	DB	?
rgb_bRed	DB	?
RGB	ENDS

BITMAPINFO	STRUC
bmi_cbFix	DD	?
bmi_cx	DW	?
bmi_cy	DW	?
bmi_cPlanes	DW	?
bmi_cBitCount	DW	?
bmi_argbColor	DB	SIZE RGB * 1 DUP (?)
BITMAPINFO	ENDS
CBD_COMPRESSION	EQU	1 
CBD_DECOMPRESSION	EQU	2 
CBD_BITS	EQU	0 
CBD_COLOR_CONVERSION	EQU	00000001H 
BCA_UNCOMP	EQU	0 
BCA_HUFFMAN1D	EQU	3 
BCA_RLE4	EQU	2 
BCA_RLE8	EQU	1 
BCA_RLE24	EQU	4 
BRU_METRIC	EQU	0 
BRA_BOTTOMUP	EQU	0 
BRH_NOTHALFTONED	EQU	0 
BRH_ERRORDIFFUSION	EQU	1 
BRH_PANDA	EQU	2 
BRH_SUPERCIRCLE	EQU	3 
BCE_PALETTE	EQU	(-1 )
BCE_RGB	EQU	0 

BITMAPINFOHEADER2	STRUC
bmp2_cbFix	DD	?
bmp2_cx	DD	?
bmp2_cy	DD	?
bmp2_cPlanes	DW	?
bmp2_cBitCount	DW	?
bmp2_ulCompression	DD	?
bmp2_cbImage	DD	?
bmp2_cxResolution	DD	?
bmp2_cyResolution	DD	?
bmp2_cclrUsed	DD	?
bmp2_cclrImportant	DD	?
bmp2_usUnits	DW	?
bmp2_usReserved	DW	?
bmp2_usRecording	DW	?
bmp2_usRendering	DW	?
bmp2_cSize1	DD	?
bmp2_cSize2	DD	?
bmp2_ulColorEncoding	DD	?
bmp2_ulIdentifier	DD	?
BITMAPINFOHEADER2	ENDS

RGB2	STRUC
rgb2_bBlue	DB	?
rgb2_bGreen	DB	?
rgb2_bRed	DB	?
rgb2_fcOptions	DB	?
RGB2	ENDS

BITMAPINFO2	STRUC
bmi2_cbFix	DD	?
bmi2_cx	DD	?
bmi2_cy	DD	?
bmi2_cPlanes	DW	?
bmi2_cBitCount	DW	?
bmi2_ulCompression	DD	?
bmi2_cbImage	DD	?
bmi2_cxResolution	DD	?
bmi2_cyResolution	DD	?
bmi2_cclrUsed	DD	?
bmi2_cclrImportant	DD	?
bmi2_usUnits	DW	?
bmi2_usReserved	DW	?
bmi2_usRecording	DW	?
bmi2_usRendering	DW	?
bmi2_cSize1	DD	?
bmi2_cSize2	DD	?
bmi2_ulColorEncoding	DD	?
bmi2_ulIdentifier	DD	?
bmi2_argbColor	DB	SIZE RGB2 * 1 DUP (?)
BITMAPINFO2	ENDS

BITMAPFILEHEADER	STRUC
bfh_usType	DW	?
bfh_cbSize	DD	?
bfh_xHotspot	DW	?
bfh_yHotspot	DW	?
bfh_offBits	DD	?
bfh_bmp	DB	SIZE BITMAPINFOHEADER DUP (?)
BITMAPFILEHEADER	ENDS

BITMAPARRAYFILEHEADER	STRUC
bafh_usType	DW	?
bafh_cbSize	DD	?
bafh_offNext	DD	?
bafh_cxDisplay	DW	?
bafh_cyDisplay	DW	?
bafh_bfh	DB	SIZE BITMAPFILEHEADER DUP (?)
BITMAPARRAYFILEHEADER	ENDS

BITMAPFILEHEADER2	STRUC
bfh2_usType	DW	?
bfh2_cbSize	DD	?
bfh2_xHotspot	DW	?
bfh2_yHotspot	DW	?
bfh2_offBits	DD	?
bfh2_bmp2	DB	SIZE BITMAPINFOHEADER2 DUP (?)
BITMAPFILEHEADER2	ENDS

BITMAPARRAYFILEHEADER2	STRUC
bafh2_usType	DW	?
bafh2_cbSize	DD	?
bafh2_offNext	DD	?
bafh2_cxDisplay	DW	?
bafh2_cyDisplay	DW	?
bafh2_bfh2	DB	SIZE BITMAPFILEHEADER2 DUP (?)
BITMAPARRAYFILEHEADER2	ENDS
BFT_ICON	EQU	4349H    
BFT_BMAP	EQU	4d42H    
BFT_POINTER	EQU	5450H    
BFT_COLORICON	EQU	4943H    
BFT_COLORPOINTER	EQU	5043H    
BFT_BITMAPARRAY	EQU	4142H    
