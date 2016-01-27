
;

;***	Low memory defines - used by VBIOS

DCA_START	EQU	0501h	;DOS communication area start
DCA_LEN		EQU	01FFh	; "        "	    "   length

DOSVEC_FIRST	EQU	020h	;DOS-reserved vectors
DOSVEC_LAST	EQU	03Fh	;initialized to -> IRET by VBIOS


;***	DOSDDEOL - End of DOS DD List Tag
;

DOSDDEOL	EQU	( -1)


;***	DOSDDTYPE - DOS DD Header
;
;	This structure defines a DOS DD Header
;

DefStruc ,_dosddtype
    VPVOID ddd_vpdosddtype
    USHORT ddd_usAttribute
    USHORT ddd_pfnStrategy
    USHORT ddd_pfnInterrupt
    CHAR ddd_achName,,8
EndStruc DOSDDTYPE

DefType PDOSDDTYPE,dd
DefType VPDOSDDTYPE,dd


; PROCEDURES

VDHENTRY VDHSetDosDevice		<VPDOSDDTYPE>
