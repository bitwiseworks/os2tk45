;***	MVDM.INC
;
;
;	MVDM constants and types
;	Copyright (c) 1988,1989 Microsoft Corporation
;
;	Include file hierarchy:
;
;	  mvdm.inc
;	    basemac.inc
;	    basedef.inc
;	    vdmprop.inc
;	    vdmm.inc
;	    v8086.inc
;	    vdos.inc
;	    vpic.inc
;	    vdmbios.inc
;	    vkbd.inc
;	    vlpt.inc
;	    vdma.inc
;
;	By default, ALL portions of ALL include files are included (ie,
;	INCL_ALL is assumed).
;
;	To control what is included, use INCL_NONE and then select from
;	the following:
;
;	    INCL_DEF
;	    INCL_MI	    BUGBUG 22-Feb-89 JeffPar:  NOT included by default
;	    INCL_TYPES
;	    INCL_MVDMTYPES
;	    INCL_SSTODS
;	    INCL_ROMBIOS
;	    INCL_PAGE
;	    INCL_VDH
;	    INCL_VDHVPIC
;	    INCL_VDHALL
;	    INCL_VDDHELPERS
;	    INCL_VDHVKBD
;	    INCL_VDHVLPT
;	    INCL_VDHVDMA
;
;
;	MODIFICATION HISTORY
;	10/11/88    JTP		 Created.
;	12/02/88    SB		 Deleted VDMINFO include.
;	12/04/88    JTP		 Moved MVDM-specific definitions to here.
;	02/15/96    MNH	 148300	 Merge VVIDEO DBCS-J (IBM J) source w/SBCS source
;


ifndef INCL_NONE
INCL_DEF	EQU 1
INCL_TYPES	EQU 1
INCL_MVDMTYPES	EQU 1
INCL_SSTODS	EQU 1
INCL_ROMBIOS	EQU 1
INCL_PAGE	EQU 1
INCL_VDHALL	EQU 1
INCL_VDDHELPERS		EQU 1
INCL_VDHVKBD	EQU 1
INCL_VDHVLPT	EQU 1
INCL_VDHVDMA	EQU 1
endif

ifdef  INCL_VDHALL	       ;if all VDH services, set defines
INCL_VDH	EQU 1
INCL_VDHVPIC	EQU 1
INCL_VDHVKBD	EQU 1
INCL_VDHVLPT	EQU 1
INCL_VDHVDMA	EQU 1
endif

ifdef  INCL_VDH		       ;VDH services require MVDM types
INCL_MVDMTYPES	EQU 1
endif

ifdef  INCL_VDHVPIC	       ;VDH services require MVDM types
INCL_MVDMTYPES	EQU 1
endif

ifdef  INCL_VDHVKBD	       ;VDH services require MVDM types
INCL_MVDMTYPES	EQU 1
endif

ifdef  INCL_VDHVLPT	       ;VDH services require MVDM types
INCL_MVDMTYPES	EQU 1
endif

ifdef  INCL_VDHVDMA	       ;VDH services require MVDM types
INCL_MVDMTYPES	EQU 1
endif

ifdef  INCL_MVDMTYPES	       ;MVDM types depend on basic types
INCL_TYPES	EQU 1
endif

ifdef  INCL_TYPES	       ;types depend on basemaca.inc macros
INCL_DEF	EQU 1
endif


;*** Required generic include files
;

include basemaca.inc	      ;macros for MASM

	include	basedef.inc		;basic OS/2 data types


;*** MVDM-specific equates (required for function prototypes)
;

DefEntry VDHENTRY PASCAL NEAR	 ;to declare exported VDH services
DefEntry HOOKENTRY PASCAL NEAR	 ;to declare "hook" entry points
DefEntry PDDENTRY PASCAL FAR32	 ;to declare PDD and VDD entry points
DefEntry VDDENTRY PASCAL FAR32 _loadds

VDDInit		EQU	<main>		;kludge to force compiler to ref. __acrtused


;*** MVDM-specific macros
;



ifdef  INCL_VDDHELPERS

PRIVENTRY vdhSaveFlags		<VOID>	   ;prototypes for helper externals
PRIVENTRY vdhRestoreFlags		<ULONG>
PRIVENTRY vdhInt3		<VOID>
PRIVENTRY vdhXCHG		<PULONG, ULONG>
PRIVENTRY vdhBTC		<PULONG, ULONG>
PRIVENTRY vdhBTR		<PULONG, ULONG>
PRIVENTRY vdhBTS		<PULONG, ULONG>
PRIVENTRY vdhBSF		<ULONG>
PRIVENTRY vdhBSR		<ULONG>
PRIVENTRY vdhOutSB		<PORT, PBYTE, ULONG, BOOL>
PRIVENTRY vdhFindToken		<PSZ>
PRIVENTRY vdhCompareStr		<PSZ, PSZ, BOOL>
PRIVENTRY vdhMoveMem		<PVOID, PVOID, _INT, _INT, UINT, BOOL>


DISABLE macro		    ;alias for CLI (similar to C)
	cli
	endm

ENABLE	macro		    ;alias for STI (similar to C)
	sti
	endm

PUSHFLAGS macro		    ;alias for PUSHF
	pushf
	endm

POPFLAGS macro		    ;alias for POPF
	popf
	endm

INT3	macro		    ;alias for _INT 3
	int 3
	endm

INB	macro	ral,rdx	    ;alias for IN AL,DX (also supports IN AH,DX)
	ifidni	<ral>,<ah>
	  xchg	  al,ah
	  in	  al,rdx
	  xchg	  al,ah
	else
	  in	  ral,rdx
	endif
	endm

INW	macro	rax,rdx	    ;alias for IN AX,DX (provides support for IO8BIT)
	IFNDEF IO8BIT
	in	rax,rdx
	ELSE
	in	al,rdx
	xchg	al,ah
	inc	rdx
	jmp	$+2
	in	al,rdx
	xchg	al,ah
	dec	rdx
	ENDIF
	endm

OUTB	macro	rdx,ral	    ;alias for OUT DX,AL (also supports OUT DX,AH)
	ifidni	<ral>,<ah>
	  xchg	  al,ah
	  out	  rdx,al
	  xchg	  al,ah
	else
	  out	  rdx,ral
	endif
	endm

OUTW	macro	rdx,rax	    ;alias for OUT DX,AX (provides support for IO8BIT)
	IFNDEF IO8BIT
	out	rdx,rax
	ELSE
	out	rdx,al
	xchg	al,ah
	inc	rdx
	jmp	$+2
	out	rdx,al
	xchg	al,ah
	dec	rdx
	ENDIF
	endm

endif  ;INCL_VDDHELPERS


;*** MVDM-specific types
;

ifdef  INCL_MVDMTYPES

DefType PVDM,PVOID			;linear pointer into VDM      ;pvdm

DefType PBVDM,PBYTE			;same as PVDM, but often needs less casting

DefType VBREG,_BYTE			;virtual byte register	      ;vb

endif  ;INCL_MVDMTYPES


;*** MVDM-specific include files
;

ifdef  INCL_VDH
	include	vdmprop.inc		;DOS property services
	include	vdmm.inc		;VDM Manager services
	include	v8086.inc		;8086 emulation services
	include	vdos.inc		;DOS emulation services
endif

ifdef  INCL_VDHVPIC
	include	vpic.inc		;VPIC services
endif

ifdef  INCL_VDHVKBD
	include	vkbd.inc		;VKBD services
endif

ifdef  INCL_VDHVLPT
	include	vlpt.inc		;VLPT services
endif

ifdef  INCL_VDHVDMA
	include	vdma.inc		;VDMA services
endif

ifdef  INCL_ROMBIOS	       ;ROM BIOS Data Area definitions
	include	vdmbios.inc
endif

ifdef  INCL_MI		       ;machine instructions
DefType _t,db			;BUGBUG 22-Feb-89 JeffPar:
DefType _t,dw			 MI.INC needs to be fixed!!!
DefType _t,dd

	include	mi.inc
endif
