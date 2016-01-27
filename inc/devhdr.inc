DEV_CBNAME	EQU	8 

SysDev	STRUC
SDevNext	DD	?
SDevAtt	DW	?
SDevStrat	DW	?
SDevInt	DW	?
SDevName        DB      "        "

SDevProtCS	DW	?
SDevProtDS	DW	?
SDevRealCS	DW	?
SDevRealDS	DW	?
SysDev	ENDS


SysDev3	STRUC
SysDevBef3	DB	SIZE SysDev DUP (?)
SDevCaps	DD	?
SysDev3	ENDS

DEV_IOCTL2	EQU	0001H  
DEV_16MB	EQU	0002H  
DEV_PARALLEL	EQU	0004H  
DEV_ADAPTER_DD	EQU	0008H  
DEV_INITCOMPLETE	EQU	0010H  
DEV_SAVERESTORE	EQU	0020H 
DEV_LVM	EQU	0040H  
DEV_CIN	EQU	0001H   
DEV_COUT	EQU	0002H   
DEV_NULL	EQU	0004H   
DEV_CLOCK	EQU	0008H   
DEV_SPEC	EQU	0010H   
DEV_ADD_ON	EQU	0020H   
DEV_GIOCTL	EQU	0040H   
DEV_FCNLEV	EQU	0380H   
DEV_30	EQU	0800H   
DEV_SHARE	EQU	1000H   
DEV_NON_IBM	EQU	2000H   
DEV_IOCTL	EQU	4000H   
DEV_CHAR_DEV	EQU	8000H   
DEVLEV_0	EQU	0000H       
DEVLEV_1	EQU	0080H       
DEVLEV_2	EQU	0100H       
DEVLEV_3	EQU	0180H       
Dev_FuncLev     macro   ReqLev, DevAttr
        and     DevAttr,DEV_FCNLEV          
        cmp     DevAttr,ReqLev              
                                            
endm

