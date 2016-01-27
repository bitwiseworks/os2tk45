;***	VKBD.INC
;
;
;	MVDM Virtual Keyboard Exported Definitions
;	Copyright (c) 1988,1989 Microsoft Corporation
;
;	For include file hierarchy, see MVDM.INC
;
;
;	MODIFICATION HISTORY
;	10/11/88	Created.
;	07/08/96  EA	Merge DBCS VKBD support.			;@IBMJ
;	08/07/96  NSu	remove PM_FEP  rename OS2_FS_FEP to OS2_FEP  defect355j
;


ifndef INCL_NONE	       ;include everything by default
INCL_VKBD	EQU 1
endif

ifdef  INCL_VKBD
INCL_VKBDSYSREQ		EQU 1
INCL_VKBDINTERFACES	EQU 1
endif



ifdef  INCL_VKBDSYSREQ

;***	VKBD constants
;

VKBD_NAME	EQU	"VKBD$"

; VKBD functions for use by PMVDMP
;
ifdef  INCL_NLSSHLD						      ;@IBMJ
								       ;@IBMJ
NIP_PMINPUTMETHOD	EQU	00000h	;Current Input method is of PM	 ;@IBMJ
NIP_FSINPUTMETHOD	EQU	00001h	;Current Input method is of FS	 ;@IBMJ
NIP_PS55	EQU	00000h	;PS/55 BIOS : bit 2-1 : 0 0	 ;@IBMJ
NIP_PS2		EQU	00002h	;PS/2  BIOS :		0 1	 ;@IBMJ
				    ;PS/55 GALE :(reserved) 1 0	     ;@IBMJ
				    ;Reserved	:	    1 1	     ;@IBMJ
endif  ;INCL_NLSSHLD						     //@IBMJ

VKBDSYSREQ_SETACCESS	EQU	1   ;Obtain exclusive access for VDM
VKBDSYSREQ_SETFOCUS	EQU	2   ;Notify VDM is gaining/losing focus
VKBDSYSREQ_POSTSCAN	EQU	3   ;Send scan code (typed)
VKBDSYSREQ_POSTCHAR	EQU	4   ;Send character code (pasted)
;@IBMJ VKBDSYSREQ_MAX	EQU	4
VKBDSYSREQ_MONREGIST	EQU	5   ;Register Monitor into VKBD	      ;@IBMJ
VKBDSYSREQ_MONDEREGIST	EQU	6   ;Deregister Monitor from VKBD     ;@IBMJ
VKBDSYSREQ_MONREAD	EQU	7   ;Read key-stroke		      ;@IBMJ
VKBDSYSREQ_MONWRITE	EQU	8   ;Write key-stroke		      ;@IBMJ
VKBDSYSREQ_SETSHIFT	EQU	9   ;Set Keyboard shift status	      ;@IBMJ
VKBDSYSREQ_GETSHIFT	EQU	10   ;Get Keyboard shift status	       ;@IBMJ
VKBDSYSREQ_REGWVDM	EQU	11   ;Register PMFEP into VKBD	       ;@IBMJ
VKBDSYSREQ_DEREGWVDM	EQU	12   ;Deregister PMFEP from VKBD       ;@IBMJ
VKBDSYSREQ_READWVDM	EQU	13   ;Read key-stroke		       ;@IBMJ
VKBDSYSREQ_WRITEWVDM	EQU	14   ;Write key-stroke		       ;@IBMJ
VKBDSYSREQ_MAX	EQU	14					 ;@IBMJ


;***	VKBD data types
;

DefStruc ,keypkt_s			;kp (for VKBDSYSREQ_POSTSCAN/CHAR)
    UCHAR	kp_Scan		   ;scan code
    UCHAR	kp_Char		   ;translated char
    USHORT	kp_fsState	   ;shift states
    USHORT	kp_fsDDFlags	   ;translated flags
    USHORT	kp_fsKey	   ;key flags
EndStruc KEYPKT
DefType PKEYPKT,dd
DefType PPKEYPKT,dd

DefStruc ,shiftpkt_s			;sp (for VKBDSYSREQ_SETFOCUS)
    USHORT	fSetFocus	   ;TRUE to set focus, FALSE to clear focus
    USHORT	fsShift		   ;PM shift states during set focus
				    ;  undefined during clear focus
EndStruc SHIFTPKT
DefType PSHIFTPKT,dd
DefType PPSHIFTPKT,dd

;@IBMJ Start
VKBDEVENT_CREATE	EQU	0
VKBDEVENT_TERM	EQU	1

VKBDEVENT_KEY	EQU	2
VKBDEVENT_SHIFT		EQU	3
VKBDEVENT_DOS_SHIFT	EQU	4

VKBDEVENT_CHG_ENV	EQU	5

VKBD_KKC_DOS	EQU	00000h
VKBD_KKC_SHARED		EQU	00001h

VKBD_VDMIMFEP_MODE	EQU	00000h
VKBD_VDMPMFEP_MODE	EQU	00002h

VKBD_REG_VDMIMFEP	EQU	00001h
VKBD_REG_VDMPMFEP	EQU	00002h

VKBD_US_BIOS_MODE	EQU	08000h
VKBD_JP_BIOS_DOSV	EQU	00002h	  ;1 means Environment is DOSV

VKBD_ENV_TO_FULLSCREEN	EQU	0
VKBD_ENV_TO_WINDOWED	EQU	1

DefStruc ,vkevent_s			;ve (for DBCS SystemReq)
	USHORT	ve_event_id
	SGID	ve_sgid
	UCHAR	ve_monitor_flag
	UCHAR	ve_r_scancode
	UCHAR	ve_charcode
	UCHAR	ve_scancode
	UCHAR	ve_nlsstate
	UCHAR	ve_nlsshift
	USHORT	ve_shift
	ULONG	ve_timestamp
	USHORT	ve_DDFlags
EndStruc VKEVENT

DefType PVKEVENT,dd

DBCS_INPUT_METHOD_LIST_J	EQU	"OS2_FS_FEP\0DOS_FEP\0"	    ;@ibmj 355j 551j 2938j-
DBCS_INPUT_METHOD_LIST_K	EQU	"DOS_FEP\0"   ;@IBM K-BS1116A 2938j-

;@IBMJ End

endif ;INCL_VKBDSYSREQ



;***	VKBD function prototypes
;

ifdef INCL_VKBDINTERFACES

DefType PFNSCRNP,dd

VDHENTRY VDHRegisterScreenProc		<PFNSCRNP>
VDHENTRY VDHQueryKeyShift		<HVDM>

;VPL Idle detection services
VDHENTRY VDHWakeIdle		<HVDM>
VDHENTRY VDHReportPeek		<ULONG>
VDHENTRY VDHNotIdle		<VOID>
VDHENTRY VDHGetBusyFlagPtr		<void>

;requests for SVC access to VPL polling services
VPL_NAME	EQU	"VPL_IDLE"
VDHVPL_BUSY_ADDR	EQU	0	  ;provide V86 busy byte address
VDHVPL_WAIT	EQU	1	  ;make VDM sleep
VDHVPL_NOIDLE	EQU	2	  ;Turn idle detection off
VDHVPL_WAKE	EQU	3	  ;Report busy activity

endif
