;***	VDMM.INC
;
;
;	Copyright (c) 1991,1992 IBM Corporation
;
;	For include file hierarchy, see MVDM.INC
;
;	Title:
;		VDMM exports
;
;	Description:
;		VDM Manager Exported Definitions
;
;


;***************************************
;	     VDMM Defines
;***************************************


CURRENT_VDM	EQU	(0)

;***	VDHAllocMem: flOptions

VDHAM_FIXED	EQU	00000h
VDHAM_SWAPPABLE		EQU	00001h



;***	VDHAllocPage: flOptions values

VDHAP_SPECIFIC	EQU	00001h
VDHAP_SYSTEM	EQU	00002h
VDHAP_FIXED	EQU	00004h
VDHAP_SWAPPABLE		EQU	00000h
VDHAP_PHYSICAL	EQU	00008h



;***	VDHMapPages: flOptions values

VDHMT_INVALID	EQU	00001h
VDHMT_LINEAR	EQU	00002h
VDHMT_PHYSICAL	EQU	00003h
VDHMT_BLACK_HOLE	EQU	00004h


;***	VDHInstallFaultHook: flOptions values

VDHIFH_ADDR	EQU	00001h


;***	VDHAllocMem Flags: flOptions Value

VDHCBP_SWAPPABLE	EQU	00001h


;***	VDHSetPriority: flClass values

VDHSP_NO_CHANGE		EQU	000000000h
VDHSP_IDLE	EQU	000000001h
VDHSP_REGULAR	EQU	000000002h
VDHSP_TIME_CRITICAL	EQU	000000003h
VDHSP_SERVER	EQU	000000004h
VDHSP_SIMULATED_INTERRUPT	EQU	000000005h

VDHSP_CLASS_MASK	EQU	00000ffffh
VDHSP_NUM_CLASSES	EQU	5

VDHSP_DEFAULT_ACTION	EQU	000000000h
VDHSP_START_USE		EQU	000010000h
VDHSP_CONTINUE_USE	EQU	000020000h
VDHSP_END_USE	EQU	000030000h

VDHSP_ACTION_MASK	EQU	0ffff0000h


;***	VDHYield - flags

VDH_YIELD_TIME_CRITICAL		EQU	1


;***	VDHLockMem flags - flOptions

VDHLM_RESIDENT	EQU	00000h
VDHLM_READ	EQU	00001h
VDHLM_WRITE	EQU	00002h
VDHLM_CONTIGUOUS	EQU	00004h
VDHLM_NOBLOCK	EQU	00008h
VDHLM_16M	EQU	00010h
VDHLM_ALIGN	EQU	00020h

VDHLM_NO_ADDR	EQU	-1


VDH_FAILURE	EQU	0
VDH_SUCCESS	EQU	1

;***	VDM States and VDD Exit defines
;
;   These are VDM states for which VDDs get notification. Few of them
;   like PDB_* are not truely VDM state changes but they are useful
;   events for which few VDDs may be intereseted. For arguments passed
;   and callable contexts see VDHInstallUserHook.
;

VDM_CREATE	EQU	0   ;A VDM is being created
VDM_TERMINATE	EQU	1   ;A VDM is being terminated
VDM_FOREGROUND	EQU	2   ;A VDM coming to foreground
VDM_BACKGROUND	EQU	3   ;A VDM going to background
VDD_EXIT	EQU	4   ;MVDM shutting down
VDM_VDD_CREATE_DONE	EQU	5   ;Called Soon after VDM_CREATE
VDM_PDB_DESTROY		EQU	6   ;A PDB destroyed in a VDM
VDM_PDB_CHANGE	EQU	7   ;A new PDB is switched to
VDM_CODEPAGE_CHANGE	EQU	8   ;A new code page is activated
VDM_TITLE_CHANGE	EQU	9   ;VDM Title change event
VDM_MEMORY_MAPPED_IN	EQU	10  ;Memory mapped in
VDM_MEMORY_UN_MAPPED	EQU	11  ;Memory unmapped
VDM_CREATE_DONE		EQU	12  ;VDM created successfully
VDM_BEGIN_VPM_TASK	EQU	13  ;VPM task initially started
VDM_END_VPM_TASK	EQU	14  ;VPM task terminated


;***	VDHPopup Flags

VDHP_FAIL	EQU	00001h
VDHP_TERMINATE_SESSION	EQU	00002h
VDHP_RETRY	EQU	00004h
VDHP_IGNORE	EQU	00008h


;***	VDHOpen: ActionTaken Flags

VDHOPEN_FILE_EXISTED	EQU	00001h
VDHOPEN_FILE_CREATED	EQU	00002h
VDHOPEN_FILE_TRUNCATED	EQU	00003h


;***	VDHOpen: FileAttribute Flags

VDHOPEN_FILE_NORMAL	EQU	00000h
VDHOPEN_FILE_READONLY	EQU	00001h
VDHOPEN_FILE_HIDDEN	EQU	00002h
VDHOPEN_FILE_SYSTEM	EQU	00004h
VDHOPEN_FILE_DIRECTORY	EQU	00010h
VDHOPEN_FILE_ARCHIVED	EQU	00020h


;***	VDHOpen: OpenFlag Flags

VDHOPEN_FILE_OPEN	EQU	00001h
VDHOPEN_FILE_REPLACE	EQU	00002h
VDHOPEN_FILE_CREATE	EQU	00010h

;     this nibble applies if file already exists		      xxxx

VDHOPEN_ACTION_FAIL_IF_EXISTS	EQU	00000h	   ;---- ---- ---- 0000
VDHOPEN_ACTION_OPEN_IF_EXISTS	EQU	00001h	   ;---- ---- ---- 0001
VDHOPEN_ACTION_REPLACE_IF_EXISTS	EQU	00002h	   ;---- ---- ---- 0010

;     this nibble applies if file does not exist		 xxxx

VDHOPEN_ACTION_FAIL_IF_NEW	EQU	00000h	   ;---- ---- 0000 ----
VDHOPEN_ACTION_CREATE_IF_NEW	EQU	00010h	   ;---- ---- 0001 ----


;***	VDHOpen: OpenMode Flags

VDHOPEN_ACCESS_READONLY		EQU	00000h	   ;---- ---- ---- -000
VDHOPEN_ACCESS_WRITEONLY	EQU	00001h	   ;---- ---- ---- -001
VDHOPEN_ACCESS_READWRITE	EQU	00002h	   ;---- ---- ---- -010
VDHOPEN_SHARE_DENYREADWRITE	EQU	00010h	   ;---- ---- -001 ----
VDHOPEN_SHARE_DENYWRITE		EQU	00020h	   ;---- ---- -010 ----
VDHOPEN_SHARE_DENYREAD	EQU	00030h	   ;---- ---- -011 ----
VDHOPEN_SHARE_DENYNONE	EQU	00040h	   ;---- ---- -100 ----
VDHOPEN_FLAGS_NOINHERIT		EQU	00080h	   ;---- ---- 1--- ----
VDHOPEN_FLAGS_NO_LOCALITY	EQU	00000h	   ;---- -000 ---- ----
VDHOPEN_FLAGS_SEQUENTIAL	EQU	00100h	   ;---- -001 ---- ----
VDHOPEN_FLAGS_RANDOM	EQU	00200h	   ;---- -010 ---- ----
VDHOPEN_FLAGS_RANDOMSEQUENTIAL	EQU	00300h	   ;---- -011 ---- ----
VDHOPEN_FLAGS_NO_CACHE	EQU	01000h	   ;---1 ---- ---- ----
VDHOPEN_FLAGS_FAIL_ON_ERROR	EQU	02000h	   ;--1- ---- ---- ----
VDHOPEN_FLAGS_WRITE_THROUGH	EQU	04000h	   ;-1-- ---- ---- ----
VDHOPEN_FLAGS_DASD	EQU	08000h	   ;1--- ---- ---- ----
VDHOPEN_FLAGS_NONSPOOLED	EQU	000040000h


;***	VDHSeek: ultype options

VDHSK_ABSOLUTE	EQU	0
VDHSK_CURRENT_POSITION	EQU	1
VDHSK_END_OF_FILE	EQU	2


;***	VDHPhysicalDisk: functions

VDHPHYD_GET_DISKS	EQU	1
VDHPHYD_GET_HANDLE	EQU	2
VDHPHYD_RELEASE_HANDLE	EQU	3


;***	VDHCreateSem Flags

VDH_EVENTSEM	EQU	00000h
VDH_MUTEXSEM	EQU	00001h


;***	VDHArmTimerHook Flags

VDH_TIMER_INTERRUPT_HOOK	EQU	(1)
VDH_TIMER_GLOBAL_CONTEXT	EQU	(-1)

;***	VDHQuerySysValue ordinals

; Global Values		 Ordinal      Type   Units	Range

VDHGSV_DAY	EQU	0   ;ULONG  days       1 <= x <= 31
VDHGSV_MONTH	EQU	1   ;ULONG  months     1 <= x <= 12
VDHGSV_YEAR	EQU	2   ;ULONG  years   1980 <= x <= MAXULONG
VDHGSV_DAYOFWEEK	EQU	3   ;ULONG  days       0 <= x <= 6
VDHGSV_HOUR	EQU	4   ;ULONG  hours      0 <= x <	 24
VDHGSV_MINUTE	EQU	5   ;ULONG  minutes    0 <= x <	 60
VDHGSV_SECOND	EQU	6   ;ULONG  secs       0 <= x <	 60
VDHGSV_HUNDREDTH	EQU	7   ;ULONG  1/100s     0 <= x <	 100
VDHGSV_SECONDS1970	EQU	8   ;ULONG  secs       0 <= x <= MAXULONG
VDHGSV_TIMEZONE		EQU	9   ;ULONG  minutes    0 <= x <= MAXULONG
VDHGSV_MSECSBOOT	EQU	10   ;ULONG  ms		0 <= x <= MAXULONG
VDHGSV_TIMERINTERVAL	EQU	11   ;ULONG  ms		0 <= x <= 1000
VDHGSV_DYNVARIATION	EQU	12   ;BOOL		TRUE/FALSE
VDHGSV_MAXWAIT	EQU	13   ;ULONG  secs	0 <= x <= MAXULONG
VDHGSV_MINTIMESLICE	EQU	14   ;ULONG  ms		0 <= x <= MAXULONG
VDHGSV_MAXTIMESLICE	EQU	15   ;ULONG  ms		0 <= x <= MAXULONG
VDHGSV_YIELD	EQU	16   ;BOOL		TRUE/FALSE
VDHGSV_TCYIELD	EQU	17   ;BOOL		TRUE/FALSE
VDHGSV_VERMAJOR		EQU	18   ;ULONG		0 <= x <= MAXULONG
VDHGSV_VERMINOR		EQU	19   ;ULONG		0 <= x <= MAXULONG
VDHGSV_VERREVISION	EQU	20   ;CHAR		A <= x <= Z
VDHGSV_MACHINETYPE	EQU	21   ;ULONG		MACHINE_TYPE_*
VDHGSV_BLACKHOLEADDR	EQU	22   ;PVOID  bytes	0 <= x <= MAXULONG
VDHGSV_BLACKHOLESIZE	EQU	23   ;ULONG  bytes	0 <= x <= MAXULONG
VDHGSV_FGNDSESSIONID	EQU	24   ;ULONG		0 <= x <  MAXSESSIONS
VDHGSV_MAXFPSESSIONS	EQU	25   ;ULONG	       ;TO BE DELETED
VDHGSV_MAXWPSESSIONS	EQU	26   ;ULONG	       ;TO BE DELETED
VDHGSV_MAXPMSESSIONS	EQU	27   ;ULONG	       ;TO BE DELETED
VDHGSV_RESERVESESSIONS	EQU	28   ;Reserved	       ;TO BE DELETED
VDHGSV_ARPLADDR		EQU	29   ;ULONG
VDHGSV_MACHINEINFO	EQU	30   ;ULONG ptr to System Configuration Table
VDHGSV_PPOSREGS		EQU	31   ;ULONG pointer to POS Regs structure
VDHGSV_PICMASK	EQU	32   ;ULONG original PIC mask values   6.13
VDHGSV_IRETADDR		EQU	33   ;ULONG address of IRET in ROM
VDHGSV_BOOTDRV	EQU	34   ;ULONG	       1=A:,2=B:,3=C:,etc.
VDHGSV_MMIOADDR		EQU	35   ;ULONG mmio address
VDHGSV_MECADDR	EQU	36   ;ULONG mec table address
; if you add a new entry, please update GSVMAX in src\dos\mvdm\vdmvdh.inc

; Local Values		 Ordinal      Type   Units	Range

VDHLSV_HVDM	EQU	4096   ;HVDM
VDHLSV_PID	EQU	4097   ;PID
VDHLSV_PCRF	EQU	4098   ;PCRF
VDHLSV_SESSIONID	EQU	4099   ;ULONG		  N <= x <  MAXSESSIONS
VDHLSV_FOREGROUND	EQU	4100   ;BOOL		  TRUE/FALSE
VDHLSV_RMSIZE	EQU	4101   ;ULONG  kbytes	  0 <  x <= 640
VDHLSV_CODEPAGEID	EQU	4102   ;ULONG		  See DosGetCP
VDHLSV_PRIORITYCLASS	EQU	4103   ;ULONG		  See VDHSetPriority
VDHLSV_PRIORITYLEVEL	EQU	4104   ;ULONG		  See VDHSetPriority
VDHLSV_VPICBASE		EQU	4105   ;UCHAR[4]


;  Locations of base vectors in PIC base vector array returned from
;     VDHQuerySysValue
;

VPMMASTER_PORT_INDEX	EQU	0
VPMSLAVE_PORT_INDEX	EQU	1



;***	Return values for VDHGSV_MACHINETYPE

MACHINE_TYPE_AT		EQU	1
MACHINE_TYPE_PS2	EQU	2
MACHINE_TYPE_PARKCITY	EQU	3


;***	Timeout values for semaphore services

SEM_INDEFINITE_WAIT	EQU	-1
SEM_IMMEDIATE_RETURN	EQU	0


;***	VDD-to_PDD commands (used internally during VDHOpenPDD)

PDDCMD_REGISTER		EQU	0   ;to register VDD entry point with PDD
PDDCMD_DEREGISTER	EQU	1   ;To deregister a VDD at MVDM shutdown


;*** VDHDecodeProperty defines **
VDH_DP_DECIMAL	EQU	10
VDH_DP_HEX	EQU	16


;***************************************
;	   VDMM Data Types
;***************************************


;***	MISC VDMM data types
;
DefType HVDM,HANDLE
DefType PHVDM,dd
DefType HBLOCK,HANDLE			;Block pool handle
DefType HLOCK,ULONG			;Memory lock handle
DefType HVDD,ULONG			;VDD Handle
DefType PPSZ,dd			;Pointer to PSZ
DefType PHVDD,dd			;VDD Handle Pointer
DefType VDHSEM,ULONG			;It should be VOID
DefType HVDHSEM,dd			;but vddvdh.inc has
DefType PHVDHSEM,dd			;problems with it
DefType PFAULTHOOK,dd
DefType PTIMERHOOK,dd


;***	HHOOK - hook handle
;
;	This is the handle return from the VDHAllocHook and used with
;	the VDHArm functions.
;

DefType HHOOK,PVOID			;hook handle
DefType PHHOOK,dd			;pointer to hook handle



;***	Pointer types for VDD-PDD communication and V86 address manipulation
;

FAR16	EQU	<NEAR>		;(pseudo 16:16 modifier)
DefType F16PVOID,dd			;f16p (pseudo 16:16 pointer)

FARV86	EQU	<NEAR>		;(pseudo-V86 modifier)
DefType VPVOID,dd			;vp (pseudo-V86 pointer)
DefType PVPVOID,dd			;pvp




;***	FNPDD - PDD function for "standard" VDD-to-PDD communication
;
;   ENTRY
;	ulFun - function code
;
;	ul1   - function-specific
;		if a pointer, it will generally be 16:16, and
;		will generally be an input buffer, the first
;		word of which is the length of the entire buffer
;
;	ul2   - function-specific
;		if a pointer, it will generally be 16:16, and
;		will generally be an output buffer, the first
;		word of which is the length of the entire buffer
;
;	if uFunc == PDDCMD_REGISTER (from VDM Manager during VDHOpenPDD)
;	    ul1.off == VDD's CS
;	    ul1.sel == zero
;	    ul2.off == low 16-bits of EIP for VDD entry point
;	    ul2.sel == high 16-bits of EIP for VDD entry point
;
;   EXIT
;	SUCCESS
;	    !0
;	FAILURE
;	     0 (the nature of the failure, such as an error code,
;		if needed, should be returned in the ul2 output buffer)
;
;   CALLING CONVENTIONS
;	32-bit PASCAL calling conventions.  More specifically:
;
;	Parameters are pushed on the stack from left to right, and are
;	removed by the callee.  The callee preserves EBX, ESI, EDI, EBP,
;	DS and ES.  Assuming the PDD function is completely 16-bit
;	(with the exception of any 16:32 calls back to the VDD, and the
;	16:32 RETFD to return to the VDD), the high words of all the
;	extended registers are preserved by virtue of being unused by the
;	PDD.  Therefore, only BX, SI, DI, BP, DS and ES need to be explicitly
;	preserved.
;
;	Furthermore, the PDD is responsible for loading its own DS.  To
;	do this, it must have a word in its code segment fixed-up to contain
;	its data segment selector.
;
;	The return code from the PDD must be in AX  0 implies failure,
;	whereas a non-zero (generally meaningful) value implies success.
;

DefType FPFNPDD,dq			;fpfnpdd


;***	FNVDD - VDD function for "standard" PDD-to-VDD communication
;
;   ENTRY
;	ulFun - function code
;
;	ul1   - function-specific
;		if a pointer, it will generally be 16:16, and
;		will generally be an input buffer, the first
;		word of which is the length of the entire buffer
;
;	ul2   - function-specific
;		if a pointer, it will generally be 16:16, and
;		will generally be an output buffer, the first
;		word of which is the length of the entire buffer
;
;   EXIT
;	SUCCESS
;	    !0
;	FAILURE
;	     0 (the nature of the failure, such as an error code,
;		if needed, should be returned in the ul2 output buffer)
;
;   CALLING CONVENTIONS
;	32-bit PASCAL calling conventions.  More specifically:
;
;	Parameters are pushed on the stack from left to right, and are
;	removed by the callee.  The callee must preserve EBX, ESI, EDI, EBP,
;	DS and ES.
;
;	Furthermore, the VDD is responsible for loading its own DS and ES.
;	In C, this would be a "_loadds" function (or any function in a module
;	compiled with the Au rather than Aw option).
;
;	The return code from the VDD must be in AX  0 implies failure,
;	whereas a non-zero (generally meaningful) value implies success.
;

DefType FPFNVDD,dq			;fpfnvdd



VDDREQ_PASS	EQU	-1

;***	FNSYSREQ - DOS32RequestVDD VDD function-type
;
;   ENTRY
;	sgid  - screen group
;	ulFun - function code
;	nbIn  - Input buffer size, in bytes (0 if no input buffer)
;	pIn   - Input buffer
;	nbOut - Output buffer size, in bytes (0 if no output buffer)
;	pOut  - Output buffer
;   EXIT
;	SUCCESS
;	    Zero
;	FAILURE
;	    Error code, or -1 to pass control to next VDD
;
;   CALLING CONVENTIONS
;	32-bit small-model PASCAL calling conventions.  More specifically:
;
;	Parameters are pushed on the stack from left to right, and are
;	removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
;	DS and ES.  DS is assumed to contain the callee's DGROUP selector
;	on entry.  EAX must be set to the return code on exit.
;

DefType PFNSYSREQ,dd


;***	FNDEVREQ - VDHRequestVDD VDD function-type
;
;   ENTRY
;	hvdm  - handle to vdm
;	ulFun - function code
;	ul1   - function-specific
;		if a pointer, it will generally be 0:32, and
;		will generally be an input buffer  if the size of the
;		buffer is not implied, the first dword of the buffer should
;		contain a total size
;	ul2   - function-specific
;		if a pointer, it will generally be 0:32, and
;		will generally be an output buffer  if the size of the
;		buffer is not implied, the first dword of the buffer should
;		contain a total size
;   EXIT
;	SUCCESS
;	    Non-zero value (other than -1)
;	FAILURE
;	    FALSE, or -1 to pass control to next VDD (in the first case,
;	    the VDD should use VDHSetError to describe the error condition)
;
;   CALLING CONVENTIONS
;	32-bit small-model PASCAL calling conventions.  More specifically:
;
;	Parameters are pushed on the stack from left to right, and are
;	removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
;	DS and ES.  DS is assumed to contain the callee's DGROUP selector
;	on entry.  EAX must be set to the return code on exit.
;

DefType PFNDEVREQ,dd



;***	PFNVDHRP - VDD property function
;
;	This function performs property setting and validation.
;
;	The set operation may be requested at any time after a VDM is created.
;
;	The Validation operation may be requested at any time (even before
;	the a VDM is created.  Validation will be requested only for
;	VDMP_STRING and VDMP_MLSTR types, since all other types can be
;	validated using the information supplied by VDHRegisterProperty.
;
;	ENTRY op   = Operation to perform (enumeration)
;		     VDHPROP_VALIDATE
;			 Validate property for any process.  Only called
;			 for VDMP_STRING and VDMP_MLSTR properties.
;		     VDHPROP_SET
;			 Set an already validated property for specified
;			 HVDM.  The return code is ignored.
;	      hvdm = Handle of VDM.  Undefined if op = VDHPROP_VALIDATE.
;	      cb   = Count of bytes pointed to by pch.
;		     Value depends upon proptype:
;		       VDMPROP_BOOL
;			  Undefined.
;		       VDMPROP_INT
;			  Undefined.
;		       VDMPROP_ENUM
;			  Length of ASCIIZ string, including NULL terminator.
;		       VDMPROP_STRING
;			  Length of ASCIIZ string, including NULL terminator.
;		       VDMPROP_MLSTR
;			  Length of ASCIIZ string, including NULL terminator.
;
;	      pch  = Value to set/validate.
;		     The format depends on the proptype:
;		       VDMPROP_BOOL
;			  pch is interpreted as a BOOL.
;			  Value 0 is FALSE !0 is TRUE.
;		       VDMPROP_INT
;			  pch is interpreted as a ULONG. It is guaranteed
;			  to meet the registered bounds.
;		       VDMPROP_ENUM
;			  pch points to an ASCIIZ string. It is guaranteed
;			  to be one of the registered enumeration strings.
;		       VDMPROP_STRING
;			  pch points to an ASCIIZ string.  It is guaranteed
;			  to be less than or equal to the registered maximum
;			  string length.
;		       VDMPROP_MLSTR
;			  pch points to an ASCIIZ string.  Multiple lines
;			  are separated by a line feed (00Ah).  It is
;			  guaranteed to be less than or equal to the
;			  registered maximum string length.
;
;
;	EXIT-SUCCESS
;	    returns 0
;
;	EXIT-FAILURE
;	    returns non-zero error code
;		ERROR_INVALID_DATA
;		    This should be returned if the value is not valid.
;
;	CONTEXT
;	    Task
;

DefType PFNVDHRP,dd



;***	PENUMHOOK - VDD hooks for VDHEnumerateVDMs
;
;   ENTRY
;	hvdm  - VDM handle
;	ulData - Handler data
;
;   EXIT
;	SUCCESS
;	    TRUE
;	FAILURE
;	     FALSE
;
;   CALLING CONVENTIONS
;	32-bit small-model PASCAL calling conventions.  More specifically:
;
;	Parameters are pushed on the stack from left to right, and are
;	removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
;	DS and ES.  DS is assumed to contain the callee's DGROUP selector
;	on entry.  EAX must be set to the return code on exit.
;

DefType PENUMHOOK,dd			;penumh



;***	USERHOOK - VDD user-event-hook function
;
;   ENTRY
;	hvdm  - VDM handle
;
;   EXIT
;	SUCCESS
;	    !0
;	FAILURE
;	     0 (failure is only support for the VDM_CREATE hook, and
;		implies that the VDD cannot support/sustain another VDM).
;
;   CALLING CONVENTIONS
;	32-bit small-model PASCAL calling conventions.  More specifically:
;
;	Parameters are pushed on the stack from left to right, and are
;	removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
;	DS and ES.  DS is assumed to contain the callee's DGROUP selector
;	on entry.  EAX must be set to the return code on exit.
;

DefType PUSERHOOK,dd			;puserh


; Definition for thread routine
;
DefType PVDDTHREAD,dd
DefType PVDDTID,PULONG


;***	VDHPAGELIST - structure to describe dis-contiguous physical ranges
;		      for VDHLockMem
;
;	vdhpl_paddr - base physical address of one range
;	vdhpl_cb    - bytes contiguously mapped from vdhpl_paddr
;
DefStruc ,VDHPageList_s			;vdhpl
    ULONG     vdhpl_paddr
    ULONG     vdhpl_cb
EndStruc VDHPAGELIST
DefType PVDHPAGELIST,dd			;pvdhpl


;***	VDHMAPTARGET - target area definition for VDHMapPages
;
;	vdhmt_laddr - Target address in V86-space (0 <= vdhmt_laddr < 1M+64K)
;		      to be mapped.
;	vdhmt_cpg   - Count of pages to map
;	vdhmt_hmap  - Mapping handle
;
DefStruc ,VDHMapTarget_s			;vdhmt
    ULONG     vdhmt_laddr  ;address in v86-space for mapping
    ULONG     vdhmt_cpg	   ;count of pages to map
    ULONG     vdhmt_hmap   ;handle of mapping.  Must be zero on first call
			    ;to VDHMapPages for region.
EndStruc VDHMAPTARGET
DefType PVDHMAPTARGET,dd			;pvdhmt



;***	VDHMAPSOURCE - source area definition for VDHMapPages
;
;	vdhms_laddr - Source address to be mapped.
;	vdhms_hobj  - Object handle.
;
DefStruc ,VDHMapSource_s			;vdhms
    ULONG   vdhms_laddr	   ;linear address of source memory object
    ULONG   vdhms_hobj	   ;memory object handle
EndStruc VDHMAPSOURCE
DefType PVDHMAPSOURCE,dd			;pvdhms



;***	VDHQuerySem: SemState structure

DefStruc ,VDHSemState_s
	UCHAR	vss_SemType	    ;VDH_EVENTSEM/VDH_MUTEXSEM
	UCHAR	vss_fOwned	    ;0 -> Not Owned; 1 -> Owned
	USHORT	vss_fWaiter	    ;0 -> No one waiting; 1 -> Waiting
	USHORT	vss_cRequest	    ;request count in mutex case
	TID	vss_tid		    ;tid of the owner if owned
EndStruc VDHSEMSTATE

DefType PVDHSEMSTATE,dd


; Defines for POS Registers
POS_MAX_SIZE	EQU	8     ;Count of POS registers
POS_MAX_SLOT	EQU	8     ;Maximum number of slots in machine
POS_CEN_ARB_REG		EQU	090h	 ;Central Arbitration register
POS_SEL_FDBK_REG	EQU	091h	 ;Card Selected Feedback
POS_SYS_CTRLA_REG	EQU	092h	 ;System Control Port A
POS_SYS_SETUP_REG	EQU	094h	 ;System setup/enable register
POS_ADP_SETUP_REG	EQU	096h	 ;Adapter setup/enable register
POS_BASE_REG	EQU	0100h	  ;Base POS register
POS_TOP_REG	EQU	0107h	  ;Highest POS Register
POS_NULLPORT	EQU	0FFh	 ;null value for ports not active
POS_CARDFEEDBACK	EQU	0FEh	 ;pretend to send back card selected
POS_PLANAR_SELECT	EQU	080h	 ;port 94 bit value to select planar
POS_VIDEO_SELECT	EQU	020h	 ;port 94 bit value to select video
POS_OPTION_SELECT	EQU	008h	 ;port 96 bit value to select card

; Structure for POS Registers

DefStruc ,_POS_s
  _BYTE	   aPOSReg,,POS_MAX_SIZE
EndStruc POS_s
DefType PPOS,dd

DefStruc ,_POSREGS
  POS_s	  VideoPOS		       ;Video POS data
  POS_s	  PlanarPOS		       ;Planar POS data
  POS_s	  SlotPOS,,POS_MAX_SLOT	       ;Adapter POS data
  _BYTE	   Port90			;Central Arbiter
  _BYTE	   Port92			;System Control Port A
  _BYTE	   Port94			;System setup/enable reg
  _BYTE	   Port96			;Adapter setup/enable reg
EndStruc POSREGS
DefType PPOSREGS,dd

DefStruc ,_SYS_CONFIG_TABLE
  USHORT  Reserve		       ;Reserved
  _BYTE	   Model			;Model byte
  _BYTE	   Submodel			;Submodel byte
  _BYTE	   BIOS_revision		;BIOS revision level
  _BYTE	   Reserve1			;Reserved
  _BYTE	   Reserve2			;Reserved
  _BYTE	   Reserve3			;Reserved
  _BYTE	   Reserve4			;Reserved
  _BYTE	   Reserve5			;Reserved
EndStruc SYS_CONFIG_TABLE
DefType PSYS_CONFIG_TABLE,dd

; VDD initialization routine prototype

EXPENTRY VDDInit		<PSZ>


;***************************************
;	     VDMM Macros
;***************************************


;  Accessing VDM data:
;
;   If the context is the desired VDM, then the following works for
;   ROM BIOS data area access, using the dynamically-fixed-up-to-zero
;   VDMBase data structure:
;
;	VDMBase.rb_field = 0
;
;   For instance data, if you put all of it inside a structure, the instance
;   of which is VDMData, and create a type, PVDMDATA, that points to such a
;   structure, you can reference instance data like so:
;
;	VDMData.fFgnd = TRUE
;
;   And if you have a PVOID (p) created with PFROMVP or PFROMVADDR, use:
;
;	*(PBYTE)p = 0
;
;
;   If the context is NOT the desired VDM, then you have to use the
;   VDM's handle (hvdm).  For ROM BIOS data area access, use the "pVDMBase"
;   macro:
;
;	pVDMBase(hvdm)->rb_field = 0
;
;   For instance data, reference similarly with the "pVDMData" macro:
;
;	pVDMData(hvdm)->fFgnd = TRUE
;
;   And if you have a PVOID (p) created with PFROMVP or PFROMVADDR, you can
;   use the "pVDM" macro to convert the pointer to one that is valid for the
;   given VDM and of the given type:
;
;	*pVDM(hvdm,PBYTE,p) = 0
;
;   An alternative to "pVDMData" (but not preferred, because it is less
;   maintainable and overrides type-checking) if you don't want to put all
;   your instance data in a structure, is:
;
;	REFHVDM(hvdm,BOOL,VDMData.fFgnd) = TRUE
;
;   Note that for any *resident* instance data, the same guidelines apply
;   simply substitute VDMRESDATA for VDMDATA.
;


ifdef VDHSTRICT
      ASSERTTASK	macro
		  PCall VDHAssertTask
		  endm
      ASSERTVDMTASK	macro
		  PCall VDHAssertVDMTask
		  endm
      ASSERTTASKONLY	macro
		  PCall VDHAssertTaskOnly
		  endm
      ASSERTVDMTASKONLY macro
		  PCall VDHAssertVDMTaskOnly
		  endm
      ASSERTINITONLY	macro
		  PCall VDHAssertInitOnly
		  endm
      CHECKHVDM		macro hvdm
		  PCall VDHCheckHVDM, hvdm
		  endm
else
      ASSERTTASK	macro
			endm
      ASSERTVDMTASK	macro
			endm
      ASSERTTASKONLY	macro
			endm
      ASSERTVDMTASKONLY macro
			endm
      ASSERTINITONLY	macro
			endm
      CHECKHVDM		macro hvdm
			endm
endif


;***************************************
;      VDMM Function Prototype
;***************************************


;     Memory Management Function Prototypes

VDHENTRY VDHAllocSharedMem		<ULONG, PULONG, PULONG>
VDHENTRY VDHFreeSharedMem		<ULONG, ULONG>
VDHENTRY VDHAllocMem		<ULONG cb, ULONG flOptions>
VDHENTRY VDHFreeMem		<PVOID>
VDHENTRY VDHAllocDosMem		<ULONG cb>
VDHENTRY VDHCreateBlockPool		<ULONG cbBlock, ULONG flOptions>
VDHENTRY VDHAllocBlock		<HBLOCK>
VDHENTRY VDHFreeBlock		<HBLOCK,PVOID>
VDHENTRY VDHDestroyBlockPool		<HBLOCK>
VDHENTRY VDHCopyMem		<PVOID,PVOID,ULONG>
VDHENTRY VDHExchangeMem		<PVOID,PVOID,ULONG>
VDHENTRY VDHLockMem		<PVOID,ULONG,ULONG,PVOID,PVOID>
VDHENTRY VDHUnlockMem		<HLOCK>
VDHENTRY VDHQuerySel		<PVOID>
VDHENTRY VDHQueryLin		<F16PVOID>
VDHENTRY VDHCreateSel		<PVOID,ULONG>
VDHENTRY VDHDestroySel		<SEL>
VDHENTRY VDHGetCodePageFont		<ULONG,ULONG,PPVOID>
VDHENTRY VDHReleaseCodePageFont		<PVOID>
VDHENTRY VDHAllocUserMem		<ULONG, ULONG, PVOID>
VDHENTRY VDHFreeUserMem		<PVOID>
VDHENTRY VDHSetUserMem		<PVOID, ULONG, ULONG>
VDHENTRY VDHQueryUserMem		<PVOID, PULONG, PULONG>
VDHENTRY VDHQueryArena		<ULONG, PULONG>

;     Page Management Function Prototypes

VDHENTRY VDHAllocPages		<PVOID,ULONG,ULONG>
VDHENTRY VDHReallocPages		<PVOID,ULONG,ULONG>
VDHENTRY VDHFreePages		<PVOID>
VDHENTRY VDHFindFreePages		<PVOID,PULONG>
VDHENTRY VDHGetDirtyPageInfo		<HVDM,PVOID,ULONG>
VDHENTRY VDHQueryFreePages		<VOID>
VDHENTRY VDHReservePages		<PVOID,ULONG>
VDHENTRY VDHUnreservePages		<PVOID,ULONG>
VDHENTRY VDHMapPages		<PVDHMAPSOURCE,PVDHMAPTARGET,ULONG>
VDHENTRY VDHInstallFaultHook		<HVDM,PVOID,ULONG,PFAULTHOOK,BOOL>
VDHENTRY VDHRemoveFaultHook		<HVDM,PVOID,ULONG,PFAULTHOOK>
VDHENTRY VDHAllocDMABuffer		<ULONG,BOOL,PULONG>
VDHENTRY VDHFreeDMABuffer		<PVOID>


;     Inter-DD Communication Function Prototypes

VDHENTRY VDHOpenPDD		<PSZ,FPFNVDD>
VDHENTRY VDHRegisterVDD		<PSZ,PFNSYSREQ,PFNDEVREQ>
VDHENTRY VDHOpenVDD		<PSZ>
VDHENTRY VDHCloseVDD		<HVDD>
VDHENTRY VDHRequestVDD		<HVDD,HVDM,ULONG,PVOID,PVOID>
VDHENTRY VDHGetError		<VOID>
VDHENTRY VDHSetError		<ULONG>
VDHENTRY VDHInstallUserHook		<ULONG,PUSERHOOK>
VDHENTRY VDHEnumerateVDMs		<PENUMHOOK,ULONG>
VDHENTRY VDHHandleFromSGID		<SGID>
VDHENTRY VDHHandleFromPID		<PID>			  ;TO BE DELETED
VDHENTRY VDHQuerySysValue		<HVDM,ULONG>
VDHENTRY VDHPutSysValue		<ULONG,ULONG>
VDHENTRY VDHOpen		<PSZ,PHFILE,PULONG,ULONG,ULONG,ULONG,ULONG,PVOID>
VDHENTRY VDHClose		<HFILE>
VDHENTRY VDHRead		<HFILE,PVOID,ULONG>
VDHENTRY VDHWrite		<HFILE,PVOID,ULONG>
VDHENTRY VDHSeek		<HFILE,ULONG,ULONG>
VDHENTRY VDHDevIOCtl		<HFILE,ULONG,ULONG,PVOID,ULONG,PULONG,PVOID,ULONG,PULONG>
VDHENTRY VDHFSCtl		<PVOID,ULONG,PULONG,PVOID,ULONG,PULONG,ULONG,PSZ,HFILE,ULONG>
VDHENTRY VDHPhysicalDisk		<ULONG,PULONG,ULONG,PULONG,ULONG>
VDHENTRY VDHViewHandle		<HFILE,USHORT>
VDHENTRY VDHJFNFromHandle		<HFILE>

;     Semaphore Function Prototypes

VDHENTRY VDHCreateSem		<PHVDHSEM, ULONG>
VDHENTRY VDHDestroySem		<HVDHSEM>
VDHENTRY VDHQuerySem		<HVDHSEM,PVDHSEMSTATE>
VDHENTRY VDHResetEventSem		<HVDHSEM>
VDHENTRY VDHPostEventSem		<HVDHSEM>
VDHENTRY VDHWaitEventSem		<HVDHSEM,ULONG>
VDHENTRY VDHRequestMutexSem		<HVDHSEM,ULONG>
VDHENTRY VDHReleaseMutexSem		<HVDHSEM>

;     Timer Function Prototypes

VDHENTRY VDHArmTimerHook		<HHOOK,ULONG,HVDM>
VDHENTRY VDHDisarmTimerHook		<HHOOK>

;     Sound Function Prototypes

VDHENTRY VDHDevBeep		<ULONG, ULONG>

;     Property Function Prototypes

VDHENTRY VDHRegisterProperty		<PSZ,PSZ,ULONG,VPTYPE,VPORD,ULONG,PVOID,PVOID,PFNVDHRP>
VDHENTRY VDHQueryProperty		<PSZ>
VDHENTRY VDHDecodeProperty		<PPSZ,PULONG,PULONG,ULONG>

;     Process Control Function Prototypes

VDHENTRY VDHPopup		<PSZZ,ULONG,ULONG,PULONG,ULONG,PSZ>
VDHENTRY VDHKillVDM		<HVDM>
VDHENTRY VDHHaltSystem		<VOID>
VDHENTRY VDHSetPriority		<HVDM,ULONG,LONG>	    ;TO BE "FIXED"
VDHENTRY VDHFreezeVDM		<HVDM>
VDHENTRY VDHThawVDM		<HVDM>
VDHENTRY VDHIsVDMFrozen		<HVDM>
VDHENTRY VDHYield		<BOOL>
VDHENTRY VDHCreateThread		<PVDDTID, PVDDTHREAD>
VDHENTRY VDHExitThread		<ULONG>
VDHENTRY VDHInstallFaultHandler		<PVOID>

;     Perfview Prototypes

VDHENTRY VDHRegisterPerfCtrs		<PBYTE,PBYTE,ULONG>

;     Miscellaneous Function Prototypes (for kernel debugging only)

VDHENTRY VDHAssertTask		<VOID>
VDHENTRY VDHAssertVDMTask		<VOID>
VDHENTRY VDHAssertTaskOnly		<VOID>
VDHENTRY VDHAssertVDMTaskOnly		<VOID>
VDHENTRY VDHAssertInitOnly		<VOID>
VDHENTRY VDHCheckHVDM		<HVDM>
CENTRY VDHPanic		<PCHAR, ...>
VDHENTRY VDHSetTimer0		<ULONG>
VDHENTRY VDHSetMaxFileHandles		<ULONG>


;*** PANICXX - Issue a panic if condition XX is set

;   ENTRY
;	msg    quoted error message

;   EXAMPLES
;	PANIC  <"VPICSetVIRR: invalid handle %u">,<h>

;     or

;	IFDEF  VDDSTRICT
;	or     ebx,ebx
;	PANICZ <"em86ReflectInt: null pointer">
;	ENDIF


?PANIC	macro	cond,msg,args
	local	l1,szMsg
	ifndef	VDHPanic
	DefCode IMPORT,GLOBAL,C
	DefFn	VDHPanic
	EndCode
	endif
	ifnb	<cond>
	j&cond	short l1
	endif
	?a substr <msg>,1,1
	% ifidn <?a>,<">
	DefData LOCAL,CONST,C
	SZ	szMsg,<msg,0>
	EndData
	CallFn	VDHPanic,<FLAToffset szMsg>,args
	else
	CallFn	VDHPanic,<FLAToffset msg>,args
	endif
	ifnb	<cond>
l1:
	endif
	endm

PANIC	equ	<?PANIC ,>
PANICC	equ	<?PANIC nc,>
PANICNC equ	<?PANIC c,>
PANICZ	equ	<?PANIC nz,>
PANICNZ equ	<?PANIC z,>
PANICE	equ	<?PANIC ne,>
PANICNE equ	<?PANIC e,>
PANICB	equ	<?PANIC ae,>
PANICBE equ	<?PANIC a,>
PANICA	equ	<?PANIC be,>
PANICAE equ	<?PANIC b,>

ifdef  VDDDEBUG

CENTRY printf		<PCHAR, ...>


PRINTDEBUG macro msg,args
	local	szMsg
	ifndef	printf
	DefCode IMPORT,GLOBAL,C
	DefFn	printf
	EndCode
	endif
	?a substr <msg>,1,1
	% ifidn <?a>,<">
	DefData LOCAL,CONST,C
	SZ	szMsg,<msg,0>
	EndData
	CallFn	printf,< <FLAToffset szMsg>,<args> >
	else
	CallFn	printf,< <FLAToffset msg>,<args> >
	endif
	endm

else


PRINTDEBUG macro msg
      endm

endif
