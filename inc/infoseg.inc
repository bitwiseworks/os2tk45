
InfoSegGDT	STRUC
SIS_BigTime	DD	?
SIS_MsCount	DD	?
SIS_HrsTime	DB	?
SIS_MinTime	DB	?
SIS_SecTime	DB	?
SIS_HunTime	DB	?
SIS_TimeZone	DW	?
SIS_ClkIntrvl	DW	?
SIS_DayDate	DB	?
SIS_MonDate	DB	?
SIS_YrsDate	DW	?
SIS_DOWDate	DB	?
SIS_VerMajor	DB	?
SIS_VerMinor	DB	?
SIS_RevLettr	DB	?
SIS_CurScrnGrp	DB	?
SIS_MaxScrnGrp	DB	?
SIS_HugeShfCnt	DB	?
SIS_ProtMdOnly	DB	?
SIS_FgndPID	DW	?
SIS_Dynamic	DB	?
SIS_MaxWait	DB	?
SIS_MinSlice	DW	?
SIS_MaxSlice	DW	?
SIS_BootDrv	DW	?
SIS_mec_table	DB	32 DUP (?)
SIS_MaxVioWinSG	DB	?
SIS_MaxPresMgrSG	DB	?
SIS_SysLog	DW	?
SIS_MMIOBase	DW	?
SIS_MMIOAddr	DD	?
SIS_MaxVDMs	DB	?
SIS_Reserved	DB	?
SIS_perf_mec_table	DB	32 DUP (?)
InfoSegGDT	ENDS

SIS_LEN	EQU	SIZE          InfoSegGDT 
SIS_TIMELEN     equ     21      
        .errnz  SIS_BigTime
        .errnz  SIS_VerMajor - SIS_TIMELEN


InfoSegLDT	STRUC
LIS_CurProcID	DW	?
LIS_ParProcID	DW	?
LIS_CurThrdPri	DW	?
LIS_CurThrdID	DW	?
LIS_CurScrnGrp	DW	?
LIS_ProcStatus	DB	?
LIS_fillbyte1	DB	?
LIS_Fgnd	DW	?
LIS_ProcType	DB	?
LIS_fillbyte2	DB	?
LIS_AX	DW	?
LIS_BX	DW	?
LIS_CX	DW	?
LIS_DX	DW	?
LIS_SI	DW	?
LIS_DI	DW	?
LIS_DS	DW	?
LIS_PackSel	DW	?
LIS_PackShrSel	DW	?
LIS_PackPckSel	DW	?
ifdef SMP
LIS_pTIB	DD	?
LIS_pPIB	DD	?
endif
InfoSegLDT	ENDS

LIS_RealMode	EQU	LIS_ProcType
LIS_LEN	EQU	SIZE          InfoSegLDT 
LIS_PT_FULLSCRN	EQU	0        
LIS_PT_REALMODE	EQU	1        
LIS_PT_VIOWIN	EQU	2        
LIS_PT_PRESMGR	EQU	3        
LIS_PT_DETACHED	EQU	4        
LIS_PS_EXITLIST	EQU	01H     
LF_LOGENABLE	EQU	0001H           
LF_LOGAVAILABLE	EQU	0002H           

infoseg_s	STRUC
is_va	DD	?
is_hob	DW	?
is_sel	DW	?
is_ppf	DD	?
infoseg_s	ENDS

