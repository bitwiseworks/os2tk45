;***	VPIC.INC
;
;
;	MVDM Virtual PIC Exported Definitions
;	Copyright (c) 1988,1989 Microsoft Corporation
;
;	For include file hierarchy, see MVDM.INC
;
;
;	MODIFICATION HISTORY
;	    10/11/88	    Created.
;


; VPIC constants

; VDHOpenVIRQ handler definitions

VPIC_NOEOIHOOK	EQU	<NULL>
VPIC_NOIRETHOOK		EQU	<NULL>
VPIC_NOIRETHOOK_TIMEOUT		EQU	-1

; VDHOpenVIRQ option definitions

VPIC_SHARE_IRQ	EQU	00001h

; VDHQueryVIRQ bit definitions

VPICQ_REQUEST_PENDING	EQU	00001h
VPICQ_IN_SERVICE	EQU	00002h
VPICQ_VIRT_MASK		EQU	00004h
VPICQ_IRET_PENDING	EQU	00008h

; VDHWaitVIRRS hook type

VDH_WAITVIRRS_HOOK	EQU	(VDH_RETURN_HOOK)

; VPIC data types

;***	IRQN - IRQ number
;
;	IRQ number definition.
;

DefType IRQN,ULONG


;***	HIRQ - IRQ handle
;
;	IRQ handle defination
;

DefType HIRQ,ULONG




; VPIC function prototypes

VDHENTRY VDHOpenVIRQ		<IRQN, PFN, PFN, ULONG, ULONG>
VDHENTRY VDHCloseVIRQ		<HIRQ>
VDHENTRY VDHSetVIRR		<HVDM, HIRQ>
VDHENTRY VDHClearVIRR		<HVDM, HIRQ>
VDHENTRY VDHQueryVIRQ		<HVDM, HIRQ>
VDHENTRY VDHSendVEOI		<HIRQ>
VDHENTRY VDHWaitVIRRs		<HHOOK>
VDHENTRY VDHWakeVIRRs		<HVDM>
