;***	VDMA.INC
;
;
;	MVDM Virtual DMA Exported Definitions
;	Copyright (c) 1988,1992 IBM Corporation
;
;	For include file hierarchy, see MVDM.INC
;
;
;	MODIFICATION HISTORY
;	90/05/21  SB   Created.
;	92/11/03  JDM  Added VDMA Request VDD defines
;	93/03/06  JAG  Changed DMAREQINFO.ri_ulReserved to .ri_Address
;

; VDMA defines

VDD_DMA_MASKOFF		EQU	0	;Mask off notification to VDD
VDD_DMA_MASK	EQU	1	;Mask on  notification to VDD

VDMA_NAME	EQU	"VDMA"

VDMA_GET_INFO_CMD	EQU	0
VDMA_REGISTER_VXGA	EQU	1

DefStruc ,dmaChnlInfo_s
	ULONG	ch_addrBase
	USHORT	ch_cbCount
	USHORT	ch_txMode
EndStruc DMACHNLINFO
DefType PDMACHNLINFO,dd

DefStruc ,dmaReqInfo_s
	ULONG	  ri_iChannel
	PFNDEVREQ ri_Address
EndStruc DMAREQINFO
DefType PDMAREQINFO,dd

DefType PFNDMA,dd

; VDMA function prototypes

VDHENTRY VDHRegisterDMAChannel		<ULONG,PFNDMA>
VDHENTRY VDHCallOutDMA		<VOID>
