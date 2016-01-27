;***	V8086.INC
;
;
;	Title:
;
;	8086 Emulation Export Include File
;
;	Description:
;
;	Contains the exported structures, defines and VDH
;	function prototypes for 8086 emulation.
;
;	MODIFICATION HISTORY
;	--------------------
;	05/07/91 RCW  Changed all occurences of the single line comment
;		      notation to the standard notation to allow users to
;		      maintain ANSI compatibility.  This is in response to
;		      PTR B718526.  All future updates to this and all other
;		      header files should use the standard commenting format.
;  73541 10/20/93 ChangeTeam  Added em86Is32Sel.
;  93273 12 AUG 1994 ChangeTeam Added VDHGetFlags(PULONG)
;




;***	Low memory defines - used by VBIOS

IVT_START	EQU	0	;Interrupt vector table start
IVT_LEN		EQU	0400h	;    "        "	    "   length 


;***	flVdmStatus flags

VDM_STATUS_VPM_32	EQU	000000001h	;32 bit DPMI app
VDM_STATUS_VPM_32_BIT	EQU	0
VDM_STATUS_VPM_APP	EQU	000000002h	;DPMI app started
VDM_STATUS_VPM_APP_BIT	EQU	1
VDM_STATUS_VPM_EXEC	EQU	000000004h	;in VDM protected mode
VDM_STATUS_VPM_EXEC_BIT		EQU	2
VDM_STATUS_VPM_TRAPMODE		EQU	000000008h	;V86 mode at kernel entry?
VDM_STATUS_VPM_TRAP_BIT		EQU	3
VDM_STATUS_VPM_IF_FLAG	EQU	000000010h	;Virtual IF flag
VDM_STATUS_VPM_IF_BIT	EQU	4
VDM_STATUS_VPM_08_HOOK	EQU	000000020h	;protect 08h hooked?
VDM_STATUS_VPM_08_BIT	EQU	5
VDM_STATUS_VPM_1C_HOOK	EQU	000000040h	;protect 1Ch hooked?
VDM_STATUS_VPM_1C_BIT	EQU	6
VDM_STATUS_VPM_PERM	EQU	000000080h	;protected mode allowed?
VDM_STATUS_VPM_PERM_BIT		EQU	7
VDM_STATUS_VPM_XDOS	EQU	000000100h	;DOS API Extension active?
VDM_STATUS_VPM_XDOS_BIT		EQU	8
VDM_STATUS_VPM_TERM	EQU	000000200h	;VDM terminating?
VDM_STATUS_VPM_TERM_BIT		EQU	9
VDM_STATUS_VPM_EM_FLAG	EQU	000000400h	;Virtual EM flag
VDM_STATUS_VPM_EM_BIT	EQU	10
VDM_STATUS_VPM_TASK_SW	EQU	000000800h	;Task switch for HW int
VDM_STATUS_VPM_TASK_BIT		EQU	11
VDM_STATUS_VME	EQU	000001000h	;Virt Mode Ext present
VDM_STATUS_VME_BIT	EQU	12


;***	VDHInstallIntHook options

VDH_ASM_HOOK	EQU	000000001h


;***	VDHInstallIOHook options

VDHIIH_ASM_HOOK		EQU	<VDH_ASM_HOOK>
VDHIIH_ALWAYS_TRAP	EQU	000000002h
VDHIIH_NO_SIMULATE	EQU	000000004h
VDHIIH_IGNORE	EQU	000000010h
ifdef  SMP
VDHIIH_MPSAFE	EQU	000000020h
endif


;***	VDHSetIOHookState options

SETIO_TRAP	EQU	<TRUE>
SETIO_NOTRAP	EQU	<FALSE>


;***	I/O type flags - VDHInstallIOPort pfnOther handler

IO_TYPE_SHIFT	EQU	8
IO_TYPE_MASK	EQU	000000F00h
IO_TYPE_INPUT	EQU	000000100h
IO_TYPE_OUTPUT	EQU	000000000h
IO_TYPE_STRING	EQU	000000200h
IO_TYPE_BYTE	EQU	000000400h
IO_TYPE_WORD	EQU	000000000h
IO_TYPE_DWORD	EQU	000000800h

IO_TYPE_ADDR32	EQU	000001000h
IO_TYPE_REP	EQU	000002000h
IO_TYPE_REVERSE		EQU	000008000h

IO_SEG_SHIFT	EQU	16
IO_SEG_MASK	EQU	0000f0000h
IO_SEG_ES	EQU	000000000h
IO_SEG_CS	EQU	000010000h
IO_SEG_SS	EQU	000020000h
IO_SEG_DS	EQU	000030000h
IO_SEG_FS	EQU	000040000h
IO_SEG_GS	EQU	000050000h


;***	VDHCheckPagePerm and private DPMI options

VPMPG_W		EQU	000000002h	;Writable
VPMPG_U		EQU	000000004h	;user mode accessible
VPMPG_X		EQU	000000008h	;eXecutable
VPMPG_R		EQU	000000010h	;Readable
VPMPG_RESERVED	EQU	000001000h	;Reserved for system use



;***	VDHAllocHook options

VDH_CONTEXT_HOOK	EQU	0
VDH_STI_HOOK	EQU	1
VDH_RETURN_HOOK		EQU	2
VDH_TIMER_HOOK	EQU	3
VDH_BP_HOOK	EQU	4


;***	VDHArmContextHook options

GLOBAL_CONTEXT_HOOK	EQU	( -1)


;***	VDHArmReturnHook options

VDHARH_NORMAL_RET	EQU	0
VDHARH_NORMAL_IRET	EQU	1
VDHARH_RECURSIVE_RET	EQU	2
VDHARH_RECURSIVE_IRET	EQU	3
VDHARH_CSEIP_HOOK	EQU	4
VDHARH_RECURSIVE_CSEIP_HOOK	EQU	5
VDHARH_ALWAYS_ARM_HOOK	EQU	080000000h


;***	VDHArmReturnHook recursive user stack usage
;
;	VDHARH_RECURSIVE_CSEIP_HOOK stack usage will vary depend
;	on the VDM protected mode "bitness".   If 32 bit, there
;	will be 8 bytes on the stack otherwise, 4 bytes.
;

VDHARH_RECURSIVE_STACK_USAGE	EQU	4


;***	VDHPushFarCall user stack usage

VDHPFC_STACK_USAGE	EQU	4


;***	VDHPushInt user stack usage

VDHPI_STACK_USAGE	EQU	6


;***	VDHPushRegs/VDHPopRegs flags

VDHREG_AX	EQU	000000001h
VDHREG_BX	EQU	000000002h
VDHREG_CX	EQU	000000004h
VDHREG_DX	EQU	000000008h
VDHREG_SI	EQU	000000010h
VDHREG_DI	EQU	000000020h
VDHREG_BP	EQU	000000040h
VDHREG_SP	EQU	000000080h
VDHREG_DS	EQU	000000100h
VDHREG_ES	EQU	000000200h
VDHREG_SS	EQU	000000400h
VDHREG_FLAG	EQU	000000800h
VDHREG_EREGS	EQU	010000000h	  ;Push/Pop 32 bit values
VDHREG_ALL	EQU	(VDHREG_AX  OR	VDHREG_BX  OR  \
				 VDHREG_CX  OR	VDHREG_DX  OR  \
				 VDHREG_SI  OR	VDHREG_DI  OR  \
				 VDHREG_BP  OR	VDHREG_SP  OR  \
				 VDHREG_DS  OR	VDHREG_ES  OR  \
				 VDHREG_SS  OR	VDHREG_FLAG)
VDHREG_GENERAL	EQU	(VDHREG_AX  OR	VDHREG_BX  OR  \
				 VDHREG_CX  OR	VDHREG_DX  OR  \
				 VDHREG_SI  OR	VDHREG_DI  OR  \
				 VDHREG_BP  OR	VDHREG_DS  OR  \
				 VDHREG_ES  OR	VDHREG_FLAG)


;***	CRF - Client Register Frame
;
;	The EBX register points to the CRF structure on entry
;	to I/O port handlers and VDD interrupt handlers.  VDDs
;	can retrieve or modify the client's registers with this
;	interface.
;

DefStruc ,crf_s
	ULONG crf_edi
	ULONG crf_esi
	ULONG crf_ebp
	ULONG crf_padesp
	ULONG crf_ebx
	ULONG crf_edx
	ULONG crf_ecx
	ULONG crf_eax
	ULONG crf_pad2,,2
	ULONG crf_eip
	USHORT crf_cs
	USHORT crf_padcs
	ULONG crf_eflag
	ULONG crf_esp
	USHORT crf_ss
	USHORT crf_padss
	USHORT crf_es
	USHORT crf_pades
	USHORT crf_ds
	USHORT crf_padds
	USHORT crf_fs
	USHORT crf_padfs
	USHORT crf_gs
	USHORT crf_padgs
	ULONG crf_alteip	       ;other modes register set
	USHORT crf_altcs
	USHORT crf_altpadcs
	ULONG crf_alteflag
	ULONG crf_altesp
	USHORT crf_altss
	USHORT crf_altpadss
	USHORT crf_altes
	USHORT crf_altpades
	USHORT crf_altds
	USHORT crf_altpadds
	USHORT crf_altfs
	USHORT crf_altpadfs
	USHORT crf_altgs
	USHORT crf_altpadgs
EndStruc CRF

DefType PCRF,dd




; VDHReadUBuf and VDHWriteUBuf flags

VPM_FAULT_IF_RW_SET	EQU	001h
VPM_FAULT_IF_SU_SET	EQU	002h
VPM_SEL_PRESENT		EQU	004h
VPM_SEL_WRITEABLE	EQU	008h
VPM_FAULT_IF_RO		EQU	010h
VPM_SEL_IS_SS	EQU	020h
VPM_PROT_READ	EQU	040h
VPM_PROT_WRITE	EQU	080h
VPM_XCPTRET_ALT		EQU	0100h
VPM_NO_SIM_EXCPT	EQU	0200h

;***	Hook routine typedefs

HOOKENTRY BIH		<ULONG, PCRF>	     ;I/O hook routines
HOOKENTRY BOH		<_BYTE, ULONG, PCRF>
HOOKENTRY WIH		<ULONG, PCRF>
HOOKENTRY WOH		<_WORD, ULONG, PCRF>
HOOKENTRY OTH		<ULONG, PULONG, ULONG, ULONG, PCRF>

DefType PBIH,dd			;pointers to I/O hook routines
DefType PBOH,dd
DefType PWIH,dd
DefType PWOH,dd
DefType POTH,dd

HOOKENTRY FNHOOK		<PCRF>		  ;hook routine
DefType PFNHOOK,dd			;pointer to hook routine

HOOKENTRY FNARM		<>		   ;arm hook routine
DefType PFNARM,dd			;pointer to arm hook routine


;***	IOH - I/O Port Hook Entry
;
;	This is the structure for the parameter to the VDHInstallIOHook
;	service.  Only the byte input and output handlers are required.
;	A null (0) entry indicates that the type of I/O should be
;	simulated.  The structure passed by VDHInstallIOHook is
;	copied and saved away after the NULLs are replaced with the
;	addresses of the simulation routines.
;

DefStruc ,ioh_s
	PBIH ioh_pbihByteInput		       ;byte input handler
	PBOH ioh_pbohByteOutput		       ;byte output handler
	PWIH ioh_pwihWordInput		       ;word input handler
	PWOH ioh_pwohWordOutput		       ;word output handler
	POTH ioh_pothOther		       ;dword and string I/O handler
EndStruc IOH

DefType PIOH,dd			;pointer to IOH entry



; 8086 emulation VDH services

VDHENTRY VDHInstallIOHook		<HVDM, PORT, ULONG, PIOH, FLAGS>
VDHENTRY VDHRemoveIOHook		<HVDM, PORT, ULONG, PIOH>
VDHENTRY VDHSetIOHookState		<HVDM, PORT, ULONG, PIOH, BOOL>
VDHENTRY VDHInstallIntHook		<HVDM, ULONG, PFNHOOK, FLAGS>
VDHENTRY VDHAllocHook		<ULONG, PFNARM, ULONG>
VDHENTRY VDHQueryHookData		<HHOOK>
VDHENTRY VDHFreeHook		<HHOOK>
VDHENTRY VDHArmContextHook		<HHOOK, HVDM>
VDHENTRY VDHArmSTIHook		<HHOOK, HVDM>
VDHENTRY VDHArmReturnHook		<HHOOK, ULONG>
VDHENTRY VDHArmBPHook		<HHOOK>
VDHENTRY VDHPushInt		<ULONG>
VDHENTRY VDHPopInt		<VOID>
VDHENTRY VDHPushRegs		<FLAGS>
VDHENTRY VDHPopRegs		<FLAGS>
VDHENTRY VDHPushStack		<ULONG,PVOID>
VDHENTRY VDHPopStack		<ULONG,PVOID>
VDHENTRY VDHSetA20		<BOOL>
VDHENTRY VDHQueryA20		<VOID>
VDHENTRY VDHSetFlags		<ULONG>
VDHENTRY VDHGetFlags		<PULONG>
VDHENTRY VDHSwitchToV86		<VOID>
VDHENTRY VDHSwitchToVPM		<VOID>
VDHENTRY VDHCheckVPMIntVector		<ULONG>
VDHENTRY VDHCheckVPMExcept		<ULONG>
VDHENTRY VDHGetVPMIntVector		<ULONG,PFPFN>
VDHENTRY VDHGetSelBase		<SEL,PULONG>
VDHENTRY VDHChangeVPMIF		<BOOL>
VDHENTRY VDHReadUBuf		<PVOID,ULONG,SEL,PULONG,ULONG>
VDHENTRY VDHWriteUBuf		<PVOID,ULONG,SEL,PULONG,ULONG>
VDHENTRY VDHCheckPagePerm		<ULONG,PVOID,ULONG,ULONG>
VDHENTRY VDHRaiseException		<ULONG,ULONG,ULONG>
VDHENTRY VDHStartHWInt		<BOOL>
VDHENTRY VDHGetVPMExcept		<ULONG,PFPFN,PBYTE>
VDHENTRY VDHProbeUBuf		<SEL,ULONG,ULONG,ULONG>
VDHENTRY VDHPrepVPMIret		<HHOOK>
VDHENTRY VDHPopVPMFarRet		<VOID>
VDHENTRY VDHRegisterAPI		<PSZ, PFNHOOK, PFNHOOK>

VDHENTRY VDHSetVPMIntVector   <ULONG, ULONG, ULONG>
VDHENTRY VDHSetVPMExcept  <ULONG, ULONG, ULONG, _BYTE>
VDHENTRY VDHPushFarCall		<ULONG, ULONG>
VDHENTRY VDHArmVPMBPHook		<HHOOK>
VDHENTRY VDHEndUseVPMStack		<VOID>
VDHENTRY VDHBeginUseVPMStack		<VOID>
