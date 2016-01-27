F_ID	EQU	00200000H       
F_VIP	EQU	00100000H       
F_VIF	EQU	00080000H       
F_AC	EQU	00040000H       
F_VM	EQU	00020000H       
F_RF	EQU	00010000H       
F_NT	EQU	00004000H       
F_NTCLEAR	EQU	( NOT F_NT)
F_IOPL0	EQU	0 
F_IOPL1	EQU	00001000H 
F_IOPL2	EQU	00002000H 
F_IOPL3	EQU	00003000H 
F_IOPLMASK	EQU	00003000H       
F_IOPLSYS	EQU	F_IOPL3         
F_IOPLclear	EQU	( NOT F_IOPLMASK)
F_OVERFLOW	EQU	00000800H 
F_DIRECTION	EQU	00000400H 
F_INTERRUPT	EQU	00000200H 
F_TRACE	EQU	00000100H 
F_SIGN	EQU	00000080H 
F_ZERO	EQU	00000040H 
F_AUX	EQU	00000010H 
F_PARITY	EQU	00000004H 
F_CARRY	EQU	00000001H 
F_UNDEFINED	EQU	0000802AH 
CR0_PE	EQU	00000001H       
CR0_MP	EQU	00000002H       
CR0_EM	EQU	00000004H       
CR0_TS	EQU	00000008H       
CR0_ET	EQU	00000010H       
CR0_NE	EQU	00000020H       
CR0_WP	EQU	00010000H       
CR0_AM	EQU	00040000H       
CR0_NW	EQU	20000000H       
CR0_CD	EQU	40000000H       
CR0_PG	EQU	80000000H       
CR0_RESERVED	EQU	1ffaffc0H       
MSW_PE	EQU	CR0_PE
MSW_MP	EQU	CR0_MP
MSW_EM	EQU	CR0_EM
MSW_TS	EQU	CR0_TS
MSW_ET	EQU	CR0_ET
VME_SUPPORTED	EQU	02H 
DE_SUPPORTED	EQU	04H 
CR3_WRITETHROUGH	EQU	00000008H      
CR3_CACHEDISABLE	EQU	00000010H      
CR3_FRAME	EQU	0fffff000H      
CR3_RESERVED	EQU	00000fe7H      
CR4_VME	EQU	00000001H 
CR4_PVI	EQU	00000002H 
CR4_DE	EQU	00000008H 
CR4_OSFXSR	EQU	00000200H 
CR4_OSXMMEXCPT	EQU	00000400H 
DR_COUNT	EQU	4H              
DR6_B0	EQU	00000001H       
DR6_B1	EQU	00000002H       
DR6_B2	EQU	00000004H       
DR6_B3	EQU	00000008H       
DR6_BD	EQU	00002000H       
DR6_BS_BIT_INDEX	EQU	0eH       
DR6_BS	EQU	(1   SHL  DR6_BS_BIT_INDEX)
DR6_BT	EQU	00008000H       
DR6_VALID	EQU	(DR6_B0 OR DR6_B1 OR DR6_B2 OR DR6_B3 OR DR6_BD OR DR6_BS OR DR6_BT)
DR6_RESERVED	EQU	 NOT (DR6_VALID)
DR7_L0	EQU	00000001H       
DR7_G0	EQU	00000002H       
DR7_L1	EQU	00000004H       
DR7_G1	EQU	00000008H       
DR7_L2	EQU	00000010H       
DR7_G2	EQU	00000020H       
DR7_L3	EQU	00000040H       
DR7_G3	EQU	00000080H       
DR7_LE	EQU	00000100H       
DR7_GE	EQU	00000200H       
DR7_RW0	EQU	00030000H       
DR7_LEN0	EQU	000c0000H       
DR7_RW1	EQU	00300000H       
DR7_LEN1	EQU	00c00000H       
DR7_RW2	EQU	03000000H       
DR7_LEN2	EQU	0c000000H       
DR7_RW3	EQU	30000000H       
DR7_LEN3	EQU	0c0000000H       
DR7_RESERVED	EQU	0000fc00H       
DR7_EXECUTE	EQU	0H              
DR7_WRITE	EQU	1H              
DR7_IO	EQU	2H              
DR7_READWRITE	EQU	3H              
DR7_LEN_1	EQU	0H              
DR7_LEN_2	EQU	1H              
DR7_LEN_4	EQU	3H              
MI_ARPL	EQU	63H             
MI_HLT	EQU	0f4H             
MI_OPERANDSIZE	EQU	66H             
MI_ADDRESSSIZE	EQU	67H             
MI_TWOBYTEOP	EQU	0fH             
MI_POP_DS	EQU	1fH 
MI_POP_ES	EQU	07H 
MI_POP_FS	EQU	0A1H             
MI_POP_GS	EQU	0A9H             
MI_INT3	EQU	0CCH 
MI_INT	EQU	0CDH 
MI_IRET	EQU	0CFH 
MI_LONG_JMP	EQU	0EAH 
MI_LONG_CALL	EQU	9AH 
MI_LONG_RET	EQU	0CBH 
MI_LONG_RETn	EQU	0CAH 
MI_NEAR_RET	EQU	0C3H 
MI_IN_PORT_AL	EQU	0E4H             
MI_IN_PORT_AX	EQU	0E5H             
MI_OUT_PORT_AL	EQU	0E6H             
MI_OUT_PORT_AX	EQU	0E7H             
MI_IN_DX_AL	EQU	0ECH             
MI_IN_DX_AX	EQU	0EDH             
MI_OUT_DX_AL	EQU	0EEH             
MI_OUT_DX_AX	EQU	0EFH             
MI_GROUP5	EQU	0FFH             
MI_SEGES	EQU	26H             
MI_SEGCS	EQU	2EH             
MI_SEGSS	EQU	36H             
MI_SEGDS	EQU	3EH             
MI_SEGFS	EQU	64H             
MI_SEGGS	EQU	65H             
MI_ESCMASK	EQU	0F8H 
MI_ESC	EQU	0D8H 
MI_MODMASK	EQU	0C0H             
MI_MODSHIFT	EQU	6                
MI_MODNONE	EQU	00H             
MI_MODBYTE	EQU	40H             
MI_MODWORD	EQU	80H             
MI_MODREG	EQU	0C0H             
MI_REGMASK	EQU	38H             
MI_REGSHIFT	EQU	3                
MI_REGAX	EQU	00H             
MI_REGCX	EQU	08H             
MI_REGDX	EQU	10H             
MI_REGBX	EQU	18H             
MI_REG3	EQU	18H             
MI_REGSP	EQU	20H             
MI_REGBP	EQU	28H             
MI_REGSI	EQU	30H             
MI_REGDI	EQU	38H             
MI_REGES	EQU	00H             
MI_REGCS	EQU	08H             
MI_REGSS	EQU	10H             
MI_REGDS	EQU	18H             
MI_REGFS	EQU	20H             
MI_REGGS	EQU	28H             
MI_RMMASK	EQU	07H             
MI_RMSHIFT	EQU	0                
MI_RMBXSI	EQU	00H             
MI_RMBXDI	EQU	01H             
MI_RMBPSI	EQU	02H             
MI_RMBPDI	EQU	03H             
MI_RMSI	EQU	04H             
MI_RMDI	EQU	05H             
MI_RMBP	EQU	06H             
MI_RMBX	EQU	07H             
MI_SIB_SSMASK	EQU	0c0H 
MI_SIB_SSSHIFT	EQU	06H 
MI_SIB_INDEXMASK	EQU	38H 
MI_SIB_INDEXSHIFT	EQU	03H 
MI_SIB_INDEXNONE	EQU	20H 
MI_SIB_BASEMASK	EQU	07H 
MI_SIB_BASESHIFT	EQU	00H 
MI_SIB_BASEESP	EQU	04H 
MI_SIB_BASENONE	EQU	05H 
MI_RMEDX	EQU	02H 
MI_RMSIB	EQU	04H 
MI_RMDISP	EQU	05H 
MI_RMEBP	EQU	05H 
MI_REG6	EQU	30H 
MI_REGCR0	EQU	00H 
MI_PUSH_AX	EQU	50H     
MI_PUSH_IMM	EQU	68H     
MI_MOV_REG_IMM	EQU	0B8H     
MI_MOV_REG_IMMEDIATE	EQU	0B8H     
MI_MOV_REG_REGMEM	EQU	8BH     
MI_ADD_AX_IMM	EQU	05H     
MI_CALL_NEAR_REL	EQU	0E8H     
SIZE_CALL_NEAR_REL	EQU	5        
MI_LMSW_OPCODE	EQU	01H     
MI_GET_CRx_OPCODE	EQU	20H     
MI_GET_DRx_OPCODE	EQU	21H     
MI_SET_CRx_OPCODE	EQU	22H     
MI_SET_DRx_OPCODE	EQU	23H     
MI_GET_TRx_OPCODE	EQU	24H     
MI_SET_TRx_OPCODE	EQU	26H     
MI_MOV_REG8_MEM8	EQU	8AH     
MI_MOV_SEG_MEM_OPCODE	EQU	8eH     

w_s	STRUC
lobyte	DB	?
hibyte	DB	?
w_s	ENDS

LowByte	EQU	lobyte
HighByte	EQU	hibyte

dw_s	STRUC
loword	DW	?
hiword	DW	?
dw_s	ENDS

LowWord	EQU	loword
HighWord	EQU	hiword

FarPtr	STRUC
Offst	DW	?
Segmt	DW	?
FarPtr	ENDS


FarPtr32	STRUC
Offst32	DD	?
Segmt32	DW	?
Pad32	DW	?
FarPtr32	ENDS


retf16_s	STRUC
retf16_ip	DW	?
retf16_cs	DW	?
retf16_s	ENDS


retf32_s	STRUC
retf32_eip	DD	?
retf32_cs	DW	?
retf32_padcs	DW	?
retf32_s	ENDS


iret16_s	STRUC
iret16_ip	DW	?
iret16_cs	DW	?
iret16_flag	DW	?
iret16_s	ENDS


Iret_s	STRUC
I_CSIP	DB	SIZE FarPtr DUP (?)
I_FLAGS	DW	?
Iret_s	ENDS


IretFrame	STRUC
IretIP	DW	?
IretCS	DW	?
IretFLAGS	DW	?
IretFrame	ENDS

IretCSIP EQU     <DWORD PTR IretIP>   

iret32_s	STRUC
iret32_eip	DD	?
iret32_cs	DW	?
iret32_padcs	DW	?
iret32_eflag	DD	?
iret32_s	ENDS


Iret32_s	STRUC
I32_CSEIP	DB	SIZE FarPtr32 DUP (?)
I32_EFLAGS	DD	?
Iret32_s	ENDS

I32_CS          EQU     <I32_CSEIP.Segmt32>
I32_EIP         EQU     <I32_CSEIP.Offst32>
I32_IP          EQU     <I32_CSEIP.Offst32.loword>
I32_FLAGS       EQU     <I32_EFLAGS.loword>


pltiret16_s	STRUC
pltiret16_ip	DW	?
pltiret16_cs	DW	?
pltiret16_flag	DW	?
pltiret16_sp	DW	?
pltiret16_ss	DW	?
pltiret16_s	ENDS


PLTIret_s	STRUC
PI_CSIP	DB	SIZE FarPtr DUP (?)
PI_FLAGS	DW	?
PI_SSSP	DB	SIZE FarPtr DUP (?)
PLTIret_s	ENDS


PLTIretFrame	STRUC
PLTIretIP	DW	?
PLTIretCS	DW	?
PLTIretFLAGS	DW	?
PLTIretSP	DW	?
PLTIretSS	DW	?
PLTIretFrame	ENDS

PLTIretCSIP     EQU     DWORD PTR PLTIretIP
PLTIretSSSP     EQU     DWORD PTR PLTIretSP


pltiret32_s	STRUC
pltiret32_eip	DD	?
pltiret32_cs	DW	?
pltiret32_padcs	DW	?
pltiret32_eflag	DD	?
pltiret32_esp	DD	?
pltiret32_ss	DW	?
pltiret32_padss	DW	?
pltiret32_s	ENDS


PLTIret32_s	STRUC
PI32_CSEIP	DB	SIZE FarPtr32 DUP (?)
PI32_EFLAGS	DD	?
PI32_SSESP	DB	SIZE FarPtr32 DUP (?)
PLTIret32_s	ENDS

PI32_CS         EQU     <PI32_CSEIP.Segmt32>
PI32_EIP        EQU     <PI32_CSEIP.Offst32>
PI32_SS         EQU     <PI32_SSESP.Segmt32>
PI32_ESP        EQU     <PI32_SSESP.Offst32>
PI32_FLAGS      EQU     <WORD PTR PI32_EFLAGS>

ptr_t   STRUC
        ptr_flat        DD      ?
ptr_t   ENDS
ptr_off         equ     <ptr_flat.Offst>
ptr_sel         equ     <ptr_flat.Segmt>


pusha_s	STRUC
pas_di	DW	?
pas_si	DW	?
pas_bp	DW	?
pas_sp	DW	?
pas_bx	DW	?
pas_dx	DW	?
pas_cx	DW	?
pas_ax	DW	?
pusha_s	ENDS


pushad_s	STRUC
pads_edi	DD	?
pads_esi	DD	?
pads_ebp	DD	?
pads_esp	DD	?
pads_ebx	DD	?
pads_edx	DD	?
pads_ecx	DD	?
pads_eax	DD	?
pushad_s	ENDS

pads_di EQU     <WORD PTR pads_edi>
pads_si EQU     <WORD PTR pads_esi>
pads_bp EQU     <WORD PTR pads_ebp>
pads_sp EQU     <WORD PTR pads_esp>
pads_bx EQU     <WORD PTR pads_ebx>
pads_dx EQU     <WORD PTR pads_edx>
pads_cx EQU     <WORD PTR pads_ecx>
pads_ax EQU     <WORD PTR pads_eax>

