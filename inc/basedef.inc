;***	BASEDEF.INC
;
;
;	Basic constants and types
;	Copyright (c) 1988,1989 Microsoft Corporation
;
;
;	MODIFICATION HISTORY
;	    10/10/88  JTP    Created.
;	    10/11/88  JTP    Added more base types.
;
;	To control what is included use the following:
;
;	    INCL_TYPES	     basic typedefs
;


;*** Basic constants
;


NULL  equ     0
TRUE  equ    -1
FALSE equ     0

CTRUE	EQU	1

KSIZE	EQU	1024

ifdef  INCL_PAGE
PAGESIZE	EQU	4096
endif


;*** Basic data types
;


DefType VOID

DefEntry CENTRY CDECL FAR		 ;to declare misc. C-based interfaces
DefEntry PRIVENTRY PASCAL FAR		 ;to declare private routines
DefEntry EXPENTRY PASCAL FAR		 ;to declare exported routines
DefEntry API32ENTRY PASCAL FAR		 ;to declare exported API routines
DefEntry DYNENTRY PASCAL FAR _export	 ;to declare exported dynlink routines

ifdef  INCL_TYPES

DefType _INT,dd			;i
DefType UINT,dd			;u
DefType BOOL,_INT			;f

DefType _BYTE,db			;b
DefType _WORD,dw			;w
DefType _DWORD,dd			;dw

DefType CHAR,db			;ch
DefType UCHAR,db			;uch
DefType _SHORT,dw			;s
DefType USHORT,dw			;us
DefType LONG,dd			;l
DefType ULONG,dd			;ul

DefType BBOOL,UCHAR			;bf
DefType SBOOL,USHORT			;sf
DefType LBOOL,ULONG			;lf

DefType FLAGS,ULONG			;fl
DefType PORT,ULONG			;port


;*** Basic pointer types
;
DefType PVOID,dd			;p
DefType NPVOID,dd			;np


;*** Basic pointer-to-basic-data types
;
DefType PINT,dd			;pi
DefType NPINT,dd			;npi

DefType PUINT,dd			;pu
DefType NPUINT,dd			;npu

DefType PBYTE,dd			;pb
DefType NPBYTE,dd			;npb

DefType PWORD,dd			;pw
DefType NPWORD,dd			;npw

DefType PDWORD,dd			;pdw
DefType NPDWORD,dd			;npdw

DefType PCHAR,dd			;pch
DefType NPCHAR,dd			;npch

DefType PSHORT,dd			;ps
DefType NPSHORT,dd			;nps

DefType PLONG,dd			;pl
DefType NPLONG,dd			;npl

DefType PUCHAR,dd			;puch
DefType NPUCHAR,dd			;npuch

DefType PUSHORT,dd			;pus
DefType NPUSHORT,dd			;npus

DefType PULONG,dd			;pul
DefType NPULONG,dd			;npul

DefType PBOOL,dd			;pf
DefType NPBOOL,dd			;npf

DefType PBBOOL,dd			;pbf
DefType NPBBOOL,dd			;npbf

DefType PSBOOL,dd			;psf
DefType NPSBOOL,dd			;npsf

DefType PLBOOL,dd			;plf
DefType NPLBOOL,dd			;nplf

DefType PFLAGS,dd			;pfl
DefType NPFLAGS,dd			;npfl

DefType PPVOID,dd			;pp
DefType NPPVOID,dd			;npp


;*** Other common types (and their pointers)
;
DefType BUF,_BYTE			;buf
DefType PBUF,dd			;pbuf

DefType NAM,CHAR			;nam
DefType SZ,CHAR			;sz
DefType PSZ,dd			;psz
DefType NPSZ,dd			;npsz
DefType SZZ,CHAR			;szz
DefType PSZZ,dd			;pszz
DefType NPSZZ,dd			;npszz

DefType SEL,USHORT			;sel
DefType PSEL,dd			;psel
DefType NPSEL,dd			;npsel

DefType SOFFSET,USHORT			;soff
DefType _OFFSET,ULONG			;off

DefType PPHYS,ULONG			;pphys

DefType PFN,dd			;pfn
DefType NPFN,dd			;npfn
DefType FPFN,dq			;fpfn
DefType FPVOID,dq			;fp (the only kind of FPVOID allowed)

DefType PPFN,dd			;ppfn
DefType NPPFN,dd			;nppfn
DefType PFPFN,dd			;pfpfn
DefType NPFPFN,dd			;npfpfn

DefType SHANDLE,USHORT			;sh
DefType HANDLE,PVOID			;h

DefStruc ,_POINTL			;ptl
    LONG  x
    LONG  y
EndStruc POINTL
DefType PPOINTL,dd

DefStruc ,_RECTL			;rcl
    LONG  xLeft
    LONG  yBottom
    LONG  xRight
    LONG  yTop
EndStruc RECTL
DefType PRECTL,dd

DefStruc ,_RGB			;rgb
    _BYTE bBlue
    _BYTE bGreen
    _BYTE bRed
EndStruc RGB
DefType PRGB,dd


;*** OS/2-specific types
;
DefType ERRCODE,ULONG			;errc
DefType RETCODE,ULONG			;rc
DefType SRETCODE,USHORT			;src

DefType PID,USHORT			;pid
DefType SGID,USHORT			;sgid
DefType TID,USHORT			;tid

DefStruc ,_RESULTCODES			;rsc
    USHORT codeTerminate
    USHORT codeResult
EndStruc RESULTCODES
DefType PRESULTCODES,dd

DefType HFILE,SHANDLE			;shf
DefType PHFILE,dd			;pshf

endif  ;INCL_TYPES
