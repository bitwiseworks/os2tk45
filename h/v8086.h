/***************************************************************************
*
* Module Name: V8086.H
*
* OS/2 public header file.
*
* Copyright (c) 1987 - 1992  IBM Corporation
*
*****************************************************************************/


/*INT32*/

/***ET+ Low memory defines - used by VBIOS */

#define IVT_START               0       /* Interrupt vector table start */
#define IVT_LEN                 0x400   /*     "        "     "   length */
/*end*/

/***ET+ flVdmStatus flags */

#define VDM_STATUS_VPM_32       0x00000001      /* 32 bit DPMI app */
#define VDM_STATUS_VPM_32_BIT   0
#define VDM_STATUS_VPM_APP      0x00000002      /* DPMI app started */
#define VDM_STATUS_VPM_APP_BIT  1
#define VDM_STATUS_VPM_EXEC     0x00000004      /* in VDM protected mode */
#define VDM_STATUS_VPM_EXEC_BIT 2
#define VDM_STATUS_VPM_TRAPMODE 0x00000008      /* V86 mode at kernel entry? */
#define VDM_STATUS_VPM_TRAP_BIT 3
#define VDM_STATUS_VPM_IF_FLAG  0x00000010      /* Virtual IF flag */
#define VDM_STATUS_VPM_IF_BIT   4
#define VDM_STATUS_VPM_08_HOOK  0x00000020      /* protect 08h hooked? */
#define VDM_STATUS_VPM_08_BIT   5
#define VDM_STATUS_VPM_1C_HOOK  0x00000040      /* protect 1Ch hooked? */
#define VDM_STATUS_VPM_1C_BIT   6
#define VDM_STATUS_VPM_PERM     0x00000080      /* protected mode allowed? */
#define VDM_STATUS_VPM_PERM_BIT 7
#define VDM_STATUS_VPM_XDOS     0x00000100      /* DOS API Extension active? */
#define VDM_STATUS_VPM_XDOS_BIT 8
#define VDM_STATUS_VPM_TERM     0x00000200      /* VDM terminating? */
#define VDM_STATUS_VPM_TERM_BIT 9
#define VDM_STATUS_VPM_EM_FLAG  0x00000400      /* Virtual EM flag */
#define VDM_STATUS_VPM_EM_BIT   10
#define VDM_STATUS_VPM_TASK_SW  0x00000800      /* Task switch for HW int */
#define VDM_STATUS_VPM_TASK_BIT 11
#define VDM_STATUS_VME          0x00001000      /* Virt Mode Ext present */
#define VDM_STATUS_VME_BIT      12
/*end*/

/***ET+ VDHInstallIntHook options */

#define VDH_ASM_HOOK            0x00000001L
/*end*/

/***ET+ VDHInstallIOHook options */

#define VDHIIH_ASM_HOOK         VDH_ASM_HOOK
#define VDHIIH_ALWAYS_TRAP      0x00000002L
#define VDHIIH_NO_SIMULATE      0x00000004L
#define VDHIIH_IGNORE           0x00000010L
#ifdef	SMP
#define	VDHIIH_MPSAFE		0x00000020L
#endif
/*end*/

/***ET+ VDHSetIOHookState options */

#define SETIO_TRAP              TRUE
#define SETIO_NOTRAP            FALSE
/*end*/

/***ET+ I/O type flags - VDHInstallIOPort pfnOther handler */

#define IO_TYPE_SHIFT           8
#define IO_TYPE_MASK            0x00000F00L
#define IO_TYPE_INPUT           0x00000100L
#define IO_TYPE_OUTPUT          0x00000000L
#define IO_TYPE_STRING          0x00000200L
#define IO_TYPE_BYTE            0x00000400L
#define IO_TYPE_WORD            0x00000000L
#define IO_TYPE_DWORD           0x00000800L

#define IO_TYPE_ADDR32          0x00001000L
#define IO_TYPE_REP             0x00002000L
#define IO_TYPE_REVERSE         0x00008000L

#define IO_SEG_SHIFT            16
#define IO_SEG_MASK             0x000f0000L
#define IO_SEG_ES               0x00000000L
#define IO_SEG_CS               0x00010000L
#define IO_SEG_SS               0x00020000L
#define IO_SEG_DS               0x00030000L
#define IO_SEG_FS               0x00040000L
#define IO_SEG_GS               0x00050000L
/*end*/

/***ET+ VDHCheckPagePerm and private DPMI options */

#define VPMPG_W            0x00000002      /* Writable */
#define VPMPG_U            0x00000004      /* user mode accessible */
#define VPMPG_X            0x00000008      /* eXecutable */
#define VPMPG_R            0x00000010      /* Readable */
#define VPMPG_RESERVED     0x00001000      /* Reserved for system use */
/*end*/


/***ET+ VDHAllocHook options */

#define VDH_CONTEXT_HOOK                0
#define VDH_STI_HOOK                    1
#define VDH_RETURN_HOOK                 2
#define VDH_TIMER_HOOK                  3
#define VDH_BP_HOOK                     4
/*end*/

/***ET+ VDHArmContextHook options */

#define GLOBAL_CONTEXT_HOOK     ((HVDM) -1)
/*end*/

/***ET+ VDHArmReturnHook options */

#define VDHARH_NORMAL_RET               0
#define VDHARH_NORMAL_IRET              1
#define VDHARH_RECURSIVE_RET            2
#define VDHARH_RECURSIVE_IRET           3
#define VDHARH_CSEIP_HOOK               4
#define VDHARH_RECURSIVE_CSEIP_HOOK     5
#define VDHARH_ALWAYS_ARM_HOOK          0x80000000
/*end*/

/***ET+ VDHArmReturnHook recursive user stack usage
 *
 *      VDHARH_RECURSIVE_CSEIP_HOOK stack usage will vary depend
 *      on the VDM protected mode "bitness".   If 32 bit, there
 *      will be 8 bytes on the stack; otherwise, 4 bytes.
 */

#define VDHARH_RECURSIVE_STACK_USAGE    4
/*end*/

/***ET+ VDHPushFarCall user stack usage */

#define VDHPFC_STACK_USAGE              4
/*end*/

/***ET+ VDHPushInt user stack usage */

#define VDHPI_STACK_USAGE               6
/*end*/

/***ET+ VDHPushRegs/VDHPopRegs flags */

#define VDHREG_AX               0x00000001L
#define VDHREG_BX               0x00000002L
#define VDHREG_CX               0x00000004L
#define VDHREG_DX               0x00000008L
#define VDHREG_SI               0x00000010L
#define VDHREG_DI               0x00000020L
#define VDHREG_BP               0x00000040L
#define VDHREG_SP               0x00000080L
#define VDHREG_DS               0x00000100L
#define VDHREG_ES               0x00000200L
#define VDHREG_SS               0x00000400L
#define VDHREG_FLAG             0x00000800L
#define VDHREG_EREGS            0x10000000L        /* Push/Pop 32 bit values */
#define VDHREG_ALL              (VDHREG_AX | VDHREG_BX | \
                                 VDHREG_CX | VDHREG_DX | \
                                 VDHREG_SI | VDHREG_DI | \
                                 VDHREG_BP | VDHREG_SP | \
                                 VDHREG_DS | VDHREG_ES | \
                                 VDHREG_SS | VDHREG_FLAG)
#define VDHREG_GENERAL          (VDHREG_AX | VDHREG_BX | \
                                 VDHREG_CX | VDHREG_DX | \
                                 VDHREG_SI | VDHREG_DI | \
                                 VDHREG_BP | VDHREG_DS | \
                                 VDHREG_ES | VDHREG_FLAG)
/*end*/

/***ET+ CRF - Client Register Frame
 *
 *      The EBX register points to the CRF structure on entry
 *      to I/O port handlers and VDD interrupt handlers.  VDDs
 *      can retrieve or modify the client's registers with this
 *      interface.
 */

typedef struct crf_s {
        ULONG crf_edi;
        ULONG crf_esi;
        ULONG crf_ebp;
        ULONG crf_padesp;
        ULONG crf_ebx;
        ULONG crf_edx;
        ULONG crf_ecx;
        ULONG crf_eax;
        ULONG crf_pad2[2];
        ULONG crf_eip;
        USHORT crf_cs;
        USHORT crf_padcs;
        ULONG crf_eflag;
        ULONG crf_esp;
        USHORT crf_ss;
        USHORT crf_padss;
        USHORT crf_es;
        USHORT crf_pades;
        USHORT crf_ds;
        USHORT crf_padds;
        USHORT crf_fs;
        USHORT crf_padfs;
        USHORT crf_gs;
        USHORT crf_padgs;
        ULONG crf_alteip;               /* other modes register set */
        USHORT crf_altcs;
        USHORT crf_altpadcs;
        ULONG crf_alteflag;
        ULONG crf_altesp;
        USHORT crf_altss;
        USHORT crf_altpadss;
        USHORT crf_altes;
        USHORT crf_altpades;
        USHORT crf_altds;
        USHORT crf_altpadds;
        USHORT crf_altfs;
        USHORT crf_altpadfs;
        USHORT crf_altgs;
        USHORT crf_altpadgs;
} CRF;

typedef CRF *PCRF;
/*end*/


/***ET+ Macros to improve VDM register manipulations from C */

#define AX(pcrf)    WORDOF(pcrf->crf_eax,0)
#define AL(pcrf)    BYTEOF(pcrf->crf_eax,0)
#define AH(pcrf)    BYTEOF(pcrf->crf_eax,1)
#define BX(pcrf)    WORDOF(pcrf->crf_ebx,0)
#define BL(pcrf)    BYTEOF(pcrf->crf_ebx,0)
#define BH(pcrf)    BYTEOF(pcrf->crf_ebx,1)
#define CX(pcrf)    WORDOF(pcrf->crf_ecx,0)
#define CL(pcrf)    BYTEOF(pcrf->crf_ecx,0)
#define CH(pcrf)    BYTEOF(pcrf->crf_ecx,1)
#define DX(pcrf)    WORDOF(pcrf->crf_edx,0)
#define DL(pcrf)    BYTEOF(pcrf->crf_edx,0)
#define DH(pcrf)    BYTEOF(pcrf->crf_edx,1)
#define SI(pcrf)    WORDOF(pcrf->crf_esi,0)
#define DI(pcrf)    WORDOF(pcrf->crf_edi,0)
#define BP(pcrf)    WORDOF(pcrf->crf_ebp,0)
#define SP(pcrf)    WORDOF(pcrf->crf_esp,0)
#define CS(pcrf)    WORDOF(pcrf->crf_cs,0)
#define IP(pcrf)    WORDOF(pcrf->crf_eip,0)
#define DS(pcrf)    WORDOF(pcrf->crf_ds,0)
#define ES(pcrf)    WORDOF(pcrf->crf_es,0)
#define SS(pcrf)    WORDOF(pcrf->crf_ss,0)
#define FL(pcrf)    WORDOF(pcrf->crf_eflag,0)

/* General purpose macros */

#define INT_PIC1BASE    0x08
#define INT_PIC2BASE    0x70
#define INTFROMIRQ(irq) (((irq) < 8)? (irq) + INT_PIC1BASE: \
                                      (irq) - 8 + INT_PIC2BASE)
/* VDHReadUBuf and VDHWriteUBuf flags */

#define VPM_FAULT_IF_RW_SET     0x01
#define VPM_FAULT_IF_SU_SET     0x02
#define VPM_SEL_PRESENT         0x04
#define VPM_SEL_WRITEABLE       0x08
#define VPM_FAULT_IF_RO         0x10
#define VPM_SEL_IS_SS           0x20
#define VPM_PROT_READ           0x40
#define VPM_PROT_WRITE          0x80
#define VPM_XCPTRET_ALT         0x100
#define VPM_NO_SIM_EXCPT        0x200

/***ET+ Hook routine typedefs */

typedef BYTE HOOKENTRY BIH(ULONG, PCRF);        /* I/O hook routines */
typedef VOID HOOKENTRY BOH(BYTE, ULONG, PCRF);
typedef WORD HOOKENTRY WIH(ULONG, PCRF);
typedef VOID HOOKENTRY WOH(WORD, ULONG, PCRF);
typedef BOOL HOOKENTRY OTH(ULONG, PULONG, ULONG, ULONG, PCRF);

typedef BYTE (*HOOKENTRY PBIH)(ULONG, PCRF);    /* pointers to I/O hook routines */
typedef VOID (*HOOKENTRY PBOH)(BYTE, ULONG, PCRF);
typedef WORD (*HOOKENTRY PWIH)(ULONG, PCRF);
typedef VOID (*HOOKENTRY PWOH)(WORD, ULONG, PCRF);
typedef BOOL (*HOOKENTRY POTH)(ULONG, PULONG, ULONG, ULONG, PCRF);

typedef BOOL HOOKENTRY FNHOOK(PCRF);            /* hook routine */
typedef BOOL (*HOOKENTRY PFNHOOK)(PCRF);        /* pointer to hook routine */

typedef VOID HOOKENTRY FNARM();                 /* arm hook routine */
typedef VOID (*HOOKENTRY PFNARM)();             /* pointer to arm hook routine */

/***ET+ IOH - I/O Port Hook Entry
 *
 *      This is the structure for the parameter to the VDHInstallIOHook
 *      service.  Only the byte input and output handlers are required.
 *      A null (0) entry indicates that the type of I/O should be
 *      simulated.  The structure passed by VDHInstallIOHook is
 *      copied and saved away after the NULLs are replaced with the
 *      addresses of the simulation routines.
 */

typedef struct ioh_s {
        PBIH ioh_pbihByteInput;                 /* byte input handler */
        PBOH ioh_pbohByteOutput;                /* byte output handler */
        PWIH ioh_pwihWordInput;                 /* word input handler */
        PWOH ioh_pwohWordOutput;                /* word output handler */
        POTH ioh_pothOther;                     /* dword and string I/O handler */
} IOH;

typedef IOH *PIOH;                              /* pointer to IOH entry */
/*end*/


/* 8086 emulation VDH services */

BOOL   VDHENTRY VDHInstallIOHook(HVDM, PORT, ULONG, PIOH, FLAGS);
VOID   VDHENTRY VDHRemoveIOHook(HVDM, PORT, ULONG, PIOH);
VOID   VDHENTRY VDHSetIOHookState(HVDM, PORT, ULONG, PIOH, BOOL);
BOOL   VDHENTRY VDHInstallIntHook(HVDM, ULONG, PFNHOOK, FLAGS);
HHOOK  VDHENTRY VDHAllocHook(ULONG, PFNARM, ULONG);
PVOID  VDHENTRY VDHQueryHookData(HHOOK);
VOID   VDHENTRY VDHFreeHook(HHOOK);
VOID   VDHENTRY VDHArmContextHook(HHOOK, HVDM);
VOID   VDHENTRY VDHArmSTIHook(HHOOK, HVDM);
BOOL   VDHENTRY VDHArmReturnHook(HHOOK, ULONG);
VPVOID VDHENTRY VDHArmBPHook(HHOOK);
BOOL   VDHENTRY VDHPushInt(ULONG);
BOOL   VDHENTRY VDHPopInt(VOID);
BOOL   VDHENTRY VDHPushRegs(FLAGS);
BOOL   VDHENTRY VDHPopRegs(FLAGS);
BOOL   VDHENTRY VDHPushStack(ULONG,PVOID);
BOOL   VDHENTRY VDHPopStack(ULONG,PVOID);
VOID   VDHENTRY VDHSetA20(BOOL);
BOOL   VDHENTRY VDHQueryA20(VOID);
VOID   VDHENTRY VDHSetFlags(ULONG);
VOID   VDHENTRY VDHGetFlags(PULONG);
VOID   VDHENTRY VDHSwitchToV86(VOID);
VOID   VDHENTRY VDHSwitchToVPM(VOID);
BOOL   VDHENTRY VDHCheckVPMIntVector(ULONG);
BOOL   VDHENTRY VDHCheckVPMExcept(ULONG);
BOOL   VDHENTRY VDHGetVPMIntVector(ULONG,PFPFN);
BOOL   VDHENTRY VDHGetSelBase(SEL,PULONG);
BOOL   VDHENTRY VDHChangeVPMIF(BOOL);
BOOL   VDHENTRY VDHReadUBuf(PVOID,ULONG,SEL,PULONG,ULONG);
BOOL   VDHENTRY VDHWriteUBuf(PVOID,ULONG,SEL,PULONG,ULONG);
BOOL   VDHENTRY VDHCheckPagePerm(ULONG,PVOID,ULONG,ULONG);
BOOL   VDHENTRY VDHRaiseException(ULONG,ULONG,ULONG);
BOOL   VDHENTRY VDHStartHWInt(BOOL);
BOOL   VDHENTRY VDHGetVPMExcept(ULONG,PFPFN,PBYTE);
BOOL   VDHENTRY VDHProbeUBuf(SEL,ULONG,ULONG,ULONG);
BOOL   VDHENTRY VDHPrepVPMIret(HHOOK);
BOOL   VDHENTRY VDHPopVPMFarRet(VOID);

BOOL   VDHENTRY VDHRegisterAPI(PSZ, PFNHOOK, PFNHOOK);


BOOL VDHENTRY VDHSetVPMIntVector(ULONG, FPFN);
BOOL VDHENTRY VDHSetVPMExcept(ULONG, FPFN, BYTE);
BOOL VDHENTRY VDHPushFarCall(FPFN);

// ?? Stacey
//VDHENTRY VDHSetVPMIntVector   <ULONG, ULONG, ULONG>
//VDHENTRY VDHSetVPMExcept  <ULONG, ULONG, ULONG, BYTE>
//BOOL VDHENTRY VDHPushFarCall(ULONG, ULONG);

FPFN VDHENTRY VDHArmVPMBPHook(HHOOK);
VOID VDHENTRY VDHEndUseVPMStack(VOID);
VOID VDHENTRY VDHBeginUseVPMStack(VOID);

// ?? Stacey
extern ULONG flVdmStatus;
extern BOOL  em86Is32Sel(SEL);                                    //73541

