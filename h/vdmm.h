/***************************************************************************
*
* Module Name: VDMM.H
*
* OS/2 public header file.
*
* Copyright (c) 1987 - 1992  IBM Corporation
*
*****************************************************************************/


/****************************************/
/*            VDMM Defines              */
/****************************************/


#define CURRENT_VDM                     ((HVDM)0)

/***ET+ VDHAllocMem: flOptions                                   */

#define VDHAM_FIXED                     0x0000
#define VDHAM_SWAPPABLE                 0x0001

/*end*/

/***ET+ VDHAllocPage: flOptions values                           */

#define VDHAP_SPECIFIC                  0x0001
#define VDHAP_SYSTEM                    0x0002
#define VDHAP_FIXED                     0x0004
#define VDHAP_SWAPPABLE                 0x0000
#define VDHAP_PHYSICAL                  0x0008
/*end*/


/***ET+ VDHMapPages: flOptions values                            */

#define VDHMT_INVALID                   0x0001
#define VDHMT_LINEAR                    0x0002
#define VDHMT_PHYSICAL                  0x0003
#define VDHMT_BLACK_HOLE                0x0004
/*end*/

/***ET+ VDHInstallFaultHook: flOptions values                    */

#define VDHIFH_ADDR                     0x0001
/*end*/

/***ET+ VDHAllocMem Flags: flOptions Value                       */

#define VDHCBP_SWAPPABLE                0x0001
/*end*/

/***ET+ VDHSetPriority: flClass values                           */

#define VDHSP_NO_CHANGE                 0x00000000
#define VDHSP_IDLE                      0x00000001
#define VDHSP_REGULAR                   0x00000002
#define VDHSP_TIME_CRITICAL             0x00000003
#define VDHSP_SERVER                    0x00000004
#define VDHSP_SIMULATED_INTERRUPT       0x00000005

#define VDHSP_CLASS_MASK                0x0000ffff
#define VDHSP_NUM_CLASSES               5

#define VDHSP_DEFAULT_ACTION            0x00000000
#define VDHSP_START_USE                 0x00010000
#define VDHSP_CONTINUE_USE              0x00020000
#define VDHSP_END_USE                   0x00030000

#define VDHSP_ACTION_MASK               0xffff0000
/*end*/

/***ET+ VDHYield - flags                                        */

#define VDH_YIELD_TIME_CRITICAL         1
/*end*/

/***ET+ VDHLockMem flags - flOptions                            */

#define VDHLM_RESIDENT                  0x0000
#define VDHLM_READ                      0x0001
#define VDHLM_WRITE                     0x0002
#define VDHLM_CONTIGUOUS                0x0004
#define VDHLM_NOBLOCK                   0x0008
#define VDHLM_16M                       0x0010
#define VDHLM_ALIGN                     0x0020

#define VDHLM_NO_ADDR                   -1
/*end*/

#define VDH_FAILURE                     0
#define VDH_SUCCESS                     1

/***ET+ VDM States and VDD Exit defines
 *
 *  These are VDM states for which VDDs get notification. Few of them
 *  like PDB_* are not truely VDM state changes but they are useful
 *  events for which few VDDs may be interested. For arguments passed
 *  and callable contexts see VDHInstallUserHook.
 */

#define VDM_CREATE                      0   /* A VDM is being created       */
#define VDM_TERMINATE                   1   /* A VDM is being terminated    */
#define VDM_FOREGROUND                  2   /* A VDM coming to foreground   */
#define VDM_BACKGROUND                  3   /* A VDM going to background    */
#define VDD_EXIT                        4   /* MVDM shutting down           */
#define VDM_VDD_CREATE_DONE             5   /* Called Soon after VDM_CREATE */
#define VDM_PDB_DESTROY                 6   /* A PDB destroyed in a VDM     */
#define VDM_PDB_CHANGE                  7   /* A new PDB is switched to     */
#define VDM_CODEPAGE_CHANGE             8   /* A new code page is activated */
#define VDM_TITLE_CHANGE                9   /* VDM Title change event       */
#define VDM_MEMORY_MAPPED_IN            10  /* Memory mapped in             */
#define VDM_MEMORY_UN_MAPPED            11  /* Memory unmapped              */
#define VDM_CREATE_DONE                 12  /* VDM created successfully     */
#define VDM_BEGIN_VPM_TASK              13  /* VPM task initially started   */
#define VDM_END_VPM_TASK                14  /* VPM task terminated          */
/*end*/

/***ET+ VDHPopup Flags                                          */

#define VDHP_FAIL                       0x0001
#define VDHP_TERMINATE_SESSION          0x0002
#define VDHP_RETRY                      0x0004
#define VDHP_IGNORE                     0x0008
/*end*/

/***ET+ VDHOpen: ActionTaken Flags                              */

#define VDHOPEN_FILE_EXISTED            0x0001
#define VDHOPEN_FILE_CREATED            0x0002
#define VDHOPEN_FILE_TRUNCATED          0x0003
/*end*/

/***ET+ VDHOpen: FileAttribute Flags                            */

#define VDHOPEN_FILE_NORMAL             0x0000
#define VDHOPEN_FILE_READONLY           0x0001
#define VDHOPEN_FILE_HIDDEN             0x0002
#define VDHOPEN_FILE_SYSTEM             0x0004
#define VDHOPEN_FILE_DIRECTORY          0x0010
#define VDHOPEN_FILE_ARCHIVED           0x0020
/*end*/

/***ET+ VDHOpen: OpenFlag Flags                                 */

#define VDHOPEN_FILE_OPEN               0x0001
#define VDHOPEN_FILE_REPLACE            0x0002
#define VDHOPEN_FILE_CREATE             0x0010

/*     this nibble applies if file already exists                      xxxx */

#define VDHOPEN_ACTION_FAIL_IF_EXISTS     0x0000     /* ---- ---- ---- 0000 */
#define VDHOPEN_ACTION_OPEN_IF_EXISTS     0x0001     /* ---- ---- ---- 0001 */
#define VDHOPEN_ACTION_REPLACE_IF_EXISTS  0x0002     /* ---- ---- ---- 0010 */

/*     this nibble applies if file does not exist                 xxxx      */

#define VDHOPEN_ACTION_FAIL_IF_NEW        0x0000     /* ---- ---- 0000 ---- */
#define VDHOPEN_ACTION_CREATE_IF_NEW      0x0010     /* ---- ---- 0001 ---- */
/*end*/

/***ET+ VDHOpen: OpenMode Flags                                 */

#define VDHOPEN_ACCESS_READONLY           0x0000     /* ---- ---- ---- -000 */
#define VDHOPEN_ACCESS_WRITEONLY          0x0001     /* ---- ---- ---- -001 */
#define VDHOPEN_ACCESS_READWRITE          0x0002     /* ---- ---- ---- -010 */
#define VDHOPEN_SHARE_DENYREADWRITE       0x0010     /* ---- ---- -001 ---- */
#define VDHOPEN_SHARE_DENYWRITE           0x0020     /* ---- ---- -010 ---- */
#define VDHOPEN_SHARE_DENYREAD            0x0030     /* ---- ---- -011 ---- */
#define VDHOPEN_SHARE_DENYNONE            0x0040     /* ---- ---- -100 ---- */
#define VDHOPEN_FLAGS_NOINHERIT           0x0080     /* ---- ---- 1--- ---- */
#define VDHOPEN_FLAGS_NO_LOCALITY         0x0000     /* ---- -000 ---- ---- */
#define VDHOPEN_FLAGS_SEQUENTIAL          0x0100     /* ---- -001 ---- ---- */
#define VDHOPEN_FLAGS_RANDOM              0x0200     /* ---- -010 ---- ---- */
#define VDHOPEN_FLAGS_RANDOMSEQUENTIAL    0x0300     /* ---- -011 ---- ---- */
#define VDHOPEN_FLAGS_NO_CACHE            0x1000     /* ---1 ---- ---- ---- */
#define VDHOPEN_FLAGS_FAIL_ON_ERROR       0x2000     /* --1- ---- ---- ---- */
#define VDHOPEN_FLAGS_WRITE_THROUGH       0x4000     /* -1-- ---- ---- ---- */
#define VDHOPEN_FLAGS_DASD                0x8000     /* 1--- ---- ---- ---- */
#define VDHOPEN_FLAGS_NONSPOOLED          0x00040000
/*end*/

/***ET+ VDHSeek: ultype options                                 */

#define VDHSK_ABSOLUTE                  0
#define VDHSK_CURRENT_POSITION          1
#define VDHSK_END_OF_FILE               2
/*end*/

/***ET+ VDHPhysicalDisk: functions                              */

#define VDHPHYD_GET_DISKS               1
#define VDHPHYD_GET_HANDLE              2
#define VDHPHYD_RELEASE_HANDLE          3
/*end*/

/***ET+ VDHCreateSem Flags                                      */

#define VDH_EVENTSEM                    0x0000
#define VDH_MUTEXSEM                    0x0001
/*end*/

/***ET+ VDHArmTimerHook Flags                                   */

#define VDH_TIMER_INTERRUPT_HOOK        ((HVDM)1)
#define VDH_TIMER_GLOBAL_CONTEXT        ((HVDM)-1)

/***ET+ VDHQuerySysValue ordinals                               */

/* Global Values          Ordinal      Type   Units      Range */

#define VDHGSV_DAY              0   // ULONG  days       1 <= x <= 31
#define VDHGSV_MONTH            1   // ULONG  months     1 <= x <= 12
#define VDHGSV_YEAR             2   // ULONG  years   1980 <= x <= MAXULONG
#define VDHGSV_DAYOFWEEK        3   // ULONG  days       0 <= x <= 6
#define VDHGSV_HOUR             4   // ULONG  hours      0 <= x <  24
#define VDHGSV_MINUTE           5   // ULONG  minutes    0 <= x <  60
#define VDHGSV_SECOND           6   // ULONG  secs       0 <= x <  60
#define VDHGSV_HUNDREDTH        7   // ULONG  1/100s     0 <= x <  100
#define VDHGSV_SECONDS1970      8   // ULONG  secs       0 <= x <= MAXULONG
#define VDHGSV_TIMEZONE         9   // ULONG  minutes    0 <= x <= MAXULONG
#define VDHGSV_MSECSBOOT       10   // ULONG  ms         0 <= x <= MAXULONG
#define VDHGSV_TIMERINTERVAL   11   // ULONG  ms         0 <= x <= 1000
#define VDHGSV_DYNVARIATION    12   // BOOL              TRUE/FALSE
#define VDHGSV_MAXWAIT         13   // ULONG  secs       0 <= x <= MAXULONG
#define VDHGSV_MINTIMESLICE    14   // ULONG  ms         0 <= x <= MAXULONG
#define VDHGSV_MAXTIMESLICE    15   // ULONG  ms         0 <= x <= MAXULONG
#define VDHGSV_YIELD           16   // BOOL              TRUE/FALSE
#define VDHGSV_TCYIELD         17   // BOOL              TRUE/FALSE
#define VDHGSV_VERMAJOR        18   // ULONG             0 <= x <= MAXULONG
#define VDHGSV_VERMINOR        19   // ULONG             0 <= x <= MAXULONG
#define VDHGSV_VERREVISION     20   // CHAR              A <= x <= Z
#define VDHGSV_MACHINETYPE     21   // ULONG             MACHINE_TYPE_*
#define VDHGSV_BLACKHOLEADDR   22   // PVOID  bytes      0 <= x <= MAXULONG
#define VDHGSV_BLACKHOLESIZE   23   // ULONG  bytes      0 <= x <= MAXULONG
#define VDHGSV_FGNDSESSIONID   24   // ULONG             0 <= x <  MAXSESSIONS
#define VDHGSV_MAXFPSESSIONS   25   // ULONG            //TO BE DELETED
#define VDHGSV_MAXWPSESSIONS   26   // ULONG            //TO BE DELETED
#define VDHGSV_MAXPMSESSIONS   27   // ULONG            //TO BE DELETED
#define VDHGSV_RESERVESESSIONS 28   // Reserved         //TO BE DELETED
#define VDHGSV_ARPLADDR        29   // ULONG
#define VDHGSV_MACHINEINFO     30   // ULONG ptr to System Configuration Table
#define VDHGSV_PPOSREGS        31   // ULONG pointer to POS Regs structure
#define VDHGSV_PICMASK         32   // ULONG original PIC mask values   6.13
#define VDHGSV_IRETADDR        33   // ULONG address of IRET in ROM
#define VDHGSV_BOOTDRV         34   // ULONG            1=A:,2=B:,3=C:,etc.
#define VDHGSV_MMIOADDR        35   // ULONG mmio address
#define VDHGSV_MECADDR         36   // ULONG mec table address
/* if you add a new entry, please update GSVMAX in src\dos\mvdm\vdmvdh.h */

/* Local Values           Ordinal      Type   Units      Range */

#define VDHLSV_HVDM          4096   // HVDM
#define VDHLSV_PID           4097   // PID
#define VDHLSV_PCRF          4098   // PCRF
#define VDHLSV_SESSIONID     4099   // ULONG             N <= x <  MAXSESSIONS
#define VDHLSV_FOREGROUND    4100   // BOOL              TRUE/FALSE
#define VDHLSV_RMSIZE        4101   // ULONG  kbytes     0 <  x <= 640
#define VDHLSV_CODEPAGEID    4102   // ULONG             See DosGetCP
#define VDHLSV_PRIORITYCLASS 4103   // ULONG             See VDHSetPriority
#define VDHLSV_PRIORITYLEVEL 4104   // ULONG             See VDHSetPriority
#define VDHLSV_VPICBASE      4105   // UCHAR[4]

/*
 * Locations of base vectors in PIC base vector array returned from
 *    VDHQuerySysValue
 */

#define VPMMASTER_PORT_INDEX            0
#define VPMSLAVE_PORT_INDEX             1

/*end*/

/***ET+ Return values for VDHGSV_MACHINETYPE */

#define MACHINE_TYPE_AT         1
#define MACHINE_TYPE_PS2        2
#define MACHINE_TYPE_PARKCITY   3
/*end*/

/***ET+ Timeout values for semaphore services */

#define SEM_INDEFINITE_WAIT    -1
#define SEM_IMMEDIATE_RETURN    0
/*end*/

/***ET+ VDD-to_PDD commands (used internally during VDHOpenPDD) */

#define PDDCMD_REGISTER         0   // to register VDD entry point with PDD
#define PDDCMD_DEREGISTER       1   // To deregister a VDD at MVDM shutdown
/*end*/

/*** VDHDecodeProperty defines ***/
#define VDH_DP_DECIMAL          10
#define VDH_DP_HEX              16


/****************************************/
/*          VDMM Data Types             */
/****************************************/


/***ET+ MISC VDMM data types
 */
typedef HANDLE  HVDM;
typedef HVDM    *PHVDM;
typedef HANDLE  HBLOCK;                         // Block pool handle
typedef ULONG   HLOCK;                          // Memory lock handle
typedef ULONG   HVDD;                           // VDD Handle
typedef PSZ     *PPSZ;                          // Pointer to PSZ
typedef HVDD    *PHVDD;                         // VDD Handle Pointer
typedef ULONG   VDHSEM;                         // It should be VOID
typedef VDHSEM  *HVDHSEM;                       // but vddvdh.inc has
typedef HVDHSEM *PHVDHSEM;                      // problems with it
typedef VOID    (*HOOKENTRY PFAULTHOOK)(PVDM);
typedef VOID    (*HOOKENTRY PTIMERHOOK)(ULONG);
/*end*/

/***ET+ HHOOK - hook handle
 *
 *      This is the handle return from the VDHAllocHook and used with
 *      the VDHArm functions.
 */

typedef PVOID HHOOK;                            // hook handle
typedef HHOOK *PHHOOK;                          // pointer to hook handle
/*end*/


/***ET+ Pointer types for VDD-PDD communication and V86 address manipulation
 */

#define FAR16           NEAR                    // (pseudo 16:16 modifier)
typedef VOID FAR16     *F16PVOID;               // f16p (pseudo 16:16 pointer)

#define FARV86          NEAR                    // (pseudo-V86 modifier)
typedef VOID FARV86    *VPVOID;                 // vp (pseudo-V86 pointer)
typedef VPVOID FAR     *PVPVOID;                // pvp
/*end*/


#define F16PNULL        ((F16PVOID)0)
#define F16PFROMP(p)    ((F16PVOID)(((ULONG)VDHQuerySel(p)<<16) | (USHORT)(p)))
#define F16PFROMSSP(p)  ((F16PVOID)(((ULONG)VDHQuerySel(SSToDS(p))<<16) | (USHORT)(p)))



/***ET+ FNPDD - PDD function for "standard" VDD-to-PDD communication
 *
 *  ENTRY
 *      ulFun - function code
 *
 *      ul1   - function-specific;
 *              if a pointer, it will generally be 16:16, and
 *              will generally be an input buffer, the first
 *              word of which is the length of the entire buffer
 *
 *      ul2   - function-specific;
 *              if a pointer, it will generally be 16:16, and
 *              will generally be an output buffer, the first
 *              word of which is the length of the entire buffer
 *
 *      if uFunc == PDDCMD_REGISTER (from VDM Manager during VDHOpenPDD)
 *          ul1.off == VDD's CS
 *          ul1.sel == zero
 *          ul2.off == low 16-bits of EIP for VDD entry point
 *          ul2.sel == high 16-bits of EIP for VDD entry point
 *
 *  EXIT
 *      SUCCESS
 *          !0
 *      FAILURE
 *           0 (the nature of the failure, such as an error code,
 *              if needed, should be returned in the ul2 output buffer)
 *
 *  CALLING CONVENTIONS
 *      32-bit PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee preserves EBX, ESI, EDI, EBP,
 *      DS and ES.  Assuming the PDD function is completely 16-bit
 *      (with the exception of any 16:32 calls back to the VDD, and the
 *      16:32 RETFD to return to the VDD), the high words of all the
 *      extended registers are preserved by virtue of being unused by the
 *      PDD.  Therefore, only BX, SI, DI, BP, DS and ES need to be explicitly
 *      preserved.
 *
 *      Furthermore, the PDD is responsible for loading its own DS.  To
 *      do this, it must have a word in its code segment fixed-up to contain
 *      its data segment selector.
 *
 *      The return code from the PDD must be in AX;  0 implies failure,
 *      whereas a non-zero (generally meaningful) value implies success.
 */

typedef SBOOL (*VDDENTRY FPFNVDD)(ULONG, F16PVOID, F16PVOID);   /* fpfnvdd */
typedef SBOOL (*PDDENTRY FPFNPDD)(ULONG, F16PVOID, F16PVOID);   /* fpfnvdd */
/*end*/


/***ET+ FNVDD - VDD function for "standard" PDD-to-VDD communication
 *
 *  ENTRY
 *      ulFun - function code
 *
 *      ul1   - function-specific;
 *              if a pointer, it will generally be 16:16, and
 *              will generally be an input buffer, the first
 *              word of which is the length of the entire buffer
 *
 *      ul2   - function-specific;
 *              if a pointer, it will generally be 16:16, and
 *              will generally be an output buffer, the first
 *              word of which is the length of the entire buffer
 *
 *  EXIT
 *      SUCCESS
 *          !0
 *      FAILURE
 *           0 (the nature of the failure, such as an error code,
 *              if needed, should be returned in the ul2 output buffer)
 *
 *  CALLING CONVENTIONS
 *      32-bit PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee must preserve EBX, ESI, EDI, EBP,
 *      DS and ES.
 *
 *
 *      The return code from the VDD must be in AX;  0 implies failure,
 *      whereas a non-zero (generally meaningful) value implies success.
 */

typedef SBOOL (*VDDENTRY FPFNVDD)(ULONG, F16PVOID, F16PVOID);   /* fpfnvdd */
/*end*/


#define VDDREQ_PASS -1

/***ET+ FNSYSREQ - DOS32RequestVDD VDD function-type
 *
 *  ENTRY
 *      sgid  - screen group
 *      ulFun - function code
 *      nbIn  - Input buffer size, in bytes (0 if no input buffer)
 *      pIn   - Input buffer
 *      nbOut - Output buffer size, in bytes (0 if no output buffer)
 *      pOut  - Output buffer
 *  EXIT
 *      SUCCESS
 *          Zero
 *      FAILURE
 *          Error code, or -1 to pass control to next VDD
 *
 *  CALLING CONVENTIONS
 *      32-bit small-model PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
 *      DS and ES.  DS is assumed to contain the callee's DGROUP selector
 *      on entry.  EAX must be set to the return code on exit.
 */

typedef LONG (*EXPENTRY PFNSYSREQ)(SGID, ULONG, ULONG, PVOID, ULONG, PVOID);
/*end*/

/***ET+ FNDEVREQ - VDHRequestVDD VDD function-type
 *
 *  ENTRY
 *      hvdm  - handle to vdm
 *      ulFun - function code
 *      ul1   - function-specific;
 *              if a pointer, it will generally be 0:32, and
 *              will generally be an input buffer;  if the size of the
 *              buffer is not implied, the first dword of the buffer should
 *              contain a total size
 *      ul2   - function-specific;
 *              if a pointer, it will generally be 0:32, and
 *              will generally be an output buffer;  if the size of the
 *              buffer is not implied, the first dword of the buffer should
 *              contain a total size
 *  EXIT
 *      SUCCESS
 *          Non-zero value (other than -1)
 *      FAILURE
 *          FALSE, or -1 to pass control to next VDD (in the first case,
 *          the VDD should use VDHSetError to describe the error condition)
 *
 *  CALLING CONVENTIONS
 *      32-bit small-model PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
 *      DS and ES.  DS is assumed to contain the callee's DGROUP selector
 *      on entry.  EAX must be set to the return code on exit.
 */

typedef LONG (*EXPENTRY PFNDEVREQ)(HVDM, ULONG, PVOID, PVOID);
/*end*/


/***ET+ PFNVDHRP - VDD property function
 *
 *      This function performs property setting and validation.
 *
 *      The set operation may be requested at any time after a VDM is created.
 *
 *      The Validation operation may be requested at any time (even before
 *      the a VDM is created.  Validation will be requested only for
 *      VDMP_STRING and VDMP_MLSTR types, since all other types can be
 *      validated using the information supplied by VDHRegisterProperty.
 *
 *      ENTRY op   = Operation to perform (enumeration)
 *                   VDHPROP_VALIDATE
 *                       Validate property for any process.  Only called
 *                       for VDMP_STRING and VDMP_MLSTR properties.
 *                   VDHPROP_SET
 *                       Set an already validated property for specified
 *                       HVDM.  The return code is ignored.
 *            hvdm = Handle of VDM.  Undefined if op = VDHPROP_VALIDATE.
 *            cb   = Count of bytes pointed to by pch.
 *                   Value depends upon proptype:
 *                     VDMPROP_BOOL
 *                        Undefined.
 *                     VDMPROP_INT
 *                        Undefined.
 *                     VDMPROP_ENUM
 *                        Length of ASCIIZ string, including NULL terminator.
 *                     VDMPROP_STRING
 *                        Length of ASCIIZ string, including NULL terminator.
 *                     VDMPROP_MLSTR
 *                        Length of ASCIIZ string, including NULL terminator.
 *
 *            pch  = Value to set/validate.
 *                   The format depends on the proptype:
 *                     VDMPROP_BOOL
 *                        pch is interpreted as a BOOL.
 *                        Value 0 is FALSE; !0 is TRUE.
 *                     VDMPROP_INT
 *                        pch is interpreted as a ULONG. It is guaranteed
 *                        to meet the registered bounds.
 *                     VDMPROP_ENUM
 *                        pch points to an ASCIIZ string. It is guaranteed
 *                        to be one of the registered enumeration strings.
 *                     VDMPROP_STRING
 *                        pch points to an ASCIIZ string.  It is guaranteed
 *                        to be less than or equal to the registered maximum
 *                        string length.
 *                     VDMPROP_MLSTR
 *                        pch points to an ASCIIZ string.  Multiple lines
 *                        are separated by a line feed (0x0A).  It is
 *                        guaranteed to be less than or equal to the
 *                        registered maximum string length.
 *
 *
 *      EXIT-SUCCESS
 *          returns 0
 *
 *      EXIT-FAILURE
 *          returns non-zero error code
 *              ERROR_INVALID_DATA
 *                  This should be returned if the value is not valid.
 *
 *      CONTEXT
 *          Task
 */

typedef ULONG (*EXPENTRY PFNVDHRP)(ULONG, HVDM, ULONG, PSZ);
/*end*/


/***ET+ PENUMHOOK - VDD hooks for VDHEnumerateVDMs
 *
 *  ENTRY
 *      hvdm  - VDM handle
 *      ulData - Handler data
 *
 *  EXIT
 *      SUCCESS
 *          TRUE
 *      FAILURE
 *           FALSE
 *
 *  CALLING CONVENTIONS
 *      32-bit small-model PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
 *      DS and ES.  DS is assumed to contain the callee's DGROUP selector
 *      on entry.  EAX must be set to the return code on exit.
 */

typedef BOOL (*HOOKENTRY PENUMHOOK)(HVDM,ULONG);                  /* penumh */
/*end*/


/***ET+ USERHOOK - VDD user-event-hook function
 *
 *  ENTRY
 *      hvdm  - VDM handle
 *
 *  EXIT
 *      SUCCESS
 *          !0
 *      FAILURE
 *           0 (failure is only support for the VDM_CREATE hook, and
 *              implies that the VDD cannot support/sustain another VDM).
 *
 *  CALLING CONVENTIONS
 *      32-bit small-model PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
 *      DS and ES.  DS is assumed to contain the callee's DGROUP selector
 *      on entry.  EAX must be set to the return code on exit.
 */

typedef BOOL (*HOOKENTRY PUSERHOOK)();                  /* puserh */
/*end*/

/* Definition for thread routine
 */
typedef VOID ((*PVDDTHREAD)(VOID));
typedef PULONG PVDDTID;


/***ET+ VDHPAGELIST - structure to describe dis-contiguous physical ranges
 *                    for VDHLockMem
 *
 *      vdhpl_paddr - base physical address of one range
 *      vdhpl_cb    - bytes contiguously mapped from vdhpl_paddr
 */
typedef struct VDHPageList_s { /* vdhpl */
    ULONG     vdhpl_paddr;
    ULONG     vdhpl_cb;
} VDHPAGELIST;
typedef VDHPAGELIST *PVDHPAGELIST; /* pvdhpl */


/***ET+ VDHMAPTARGET - target area definition for VDHMapPages
 *
 *      vdhmt_laddr - Target address in V86-space (0 <= vdhmt_laddr < 1M+64K)
 *                    to be mapped.
 *      vdhmt_cpg   - Count of pages to map
 *      vdhmt_hmap  - Mapping handle
 */
typedef struct VDHMapTarget_s { /* vdhmt */
    ULONG     vdhmt_laddr;  // address in v86-space for mapping
    ULONG     vdhmt_cpg;    // count of pages to map
    ULONG     vdhmt_hmap;   // handle of mapping.  Must be zero on first call
                            // to VDHMapPages for region.
} VDHMAPTARGET;
typedef VDHMAPTARGET *PVDHMAPTARGET; /* pvdhmt */
/*end*/


/***ET+ VDHMAPSOURCE - source area definition for VDHMapPages
 *
 *      vdhms_laddr - Source address to be mapped.
 *      vdhms_hobj  - Object handle.
 */
typedef struct VDHMapSource_s { /* vdhms */
    ULONG   vdhms_laddr;    // linear address of source memory object
    ULONG   vdhms_hobj;     // memory object handle
} VDHMAPSOURCE;
typedef VDHMAPSOURCE *PVDHMAPSOURCE; /* pvdhms */
/*end*/


/***ET+ VDHQuerySem: SemState structure                           */

typedef struct  VDHSemState_s {
        UCHAR   vss_SemType;         // VDH_EVENTSEM/VDH_MUTEXSEM
        UCHAR   vss_fOwned;          // 0 -> Not Owned; 1 -> Owned
        USHORT  vss_fWaiter;         // 0 -> No one waiting; 1 -> Waiting
        USHORT  vss_cRequest;        // request count in mutex case
        TID     vss_tid;             // tid of the owner if owned
}VDHSEMSTATE;

typedef VDHSEMSTATE    *PVDHSEMSTATE;
/*end*/

/* Defines for POS Registers */
#define     POS_MAX_SIZE          8     // Count of POS registers
#define     POS_MAX_SLOT          8     // Maximum number of slots in machine
#define     POS_CEN_ARB_REG    0x90     // Central Arbitration register
#define     POS_SEL_FDBK_REG   0x91     // Card Selected Feedback
#define     POS_SYS_CTRLA_REG  0x92     // System Control Port A
#define     POS_SYS_SETUP_REG  0x94     // System setup/enable register
#define     POS_ADP_SETUP_REG  0x96     // Adapter setup/enable register
#define     POS_BASE_REG      0x100     // Base POS register
#define     POS_TOP_REG       0x107     // Highest POS Register
#define     POS_NULLPORT       0xFF     // null value for ports not active
#define     POS_CARDFEEDBACK   0xFE     // pretend to send back card selected
#define     POS_PLANAR_SELECT  0x80     // port 94 bit value to select planar
#define     POS_VIDEO_SELECT   0x20     // port 94 bit value to select video
#define     POS_OPTION_SELECT  0x08     // port 96 bit value to select card

/* Structure for POS Registers */

typedef struct _POS_s {
  BYTE    aPOSReg[POS_MAX_SIZE];
} POS_s;
typedef POS_s *PPOS;

typedef struct _POSREGS {
  POS_s   VideoPOS;                     // Video POS data
  POS_s   PlanarPOS;                    // Planar POS data
  POS_s   SlotPOS[POS_MAX_SLOT];        // Adapter POS data
  BYTE    Port90;                       // Central Arbiter
  BYTE    Port92;                       // System Control Port A
  BYTE    Port94;                       // System setup/enable reg
  BYTE    Port96;                       // Adapter setup/enable reg
} POSREGS;
typedef POSREGS *PPOSREGS;

typedef struct _SYS_CONFIG_TABLE {
  USHORT  Reserve;                      // Reserved
  BYTE    Model;                        // Model byte
  BYTE    Submodel;                     // Submodel byte
  BYTE    BIOS_revision;                // BIOS revision level
  BYTE    Reserve1;                     // Reserved
  BYTE    Reserve2;                     // Reserved
  BYTE    Reserve3;                     // Reserved
  BYTE    Reserve4;                     // Reserved
  BYTE    Reserve5;                     // Reserved
} SYS_CONFIG_TABLE;
typedef SYS_CONFIG_TABLE *PSYS_CONFIG_TABLE;

/* VDD initialization routine prototype */

BOOL    EXPENTRY VDDInit(PSZ);


/****************************************/
/*            VDMM Macros               */
/****************************************/


/*  Accessing VDM data:
 *
 *  If the context is the desired VDM, then the following works for
 *  ROM BIOS data area access, using the dynamically-fixed-up-to-zero
 *  VDMBase data structure:
 *
 *      VDMBase.rb_field = 0;
 *
 *  For instance data, if you put all of it inside a structure, the instance
 *  of which is VDMData, and create a type, PVDMDATA, that points to such a
 *  structure, you can reference instance data like so:
 *
 *      VDMData.fFgnd = TRUE;
 *
 *  And if you have a PVOID (p) created with PFROMVP or PFROMVADDR, use:
 *
 *      *(PBYTE)p = 0;
 *
 *
 *  If the context is NOT the desired VDM, then you have to use the
 *  VDM's handle (hvdm).  For ROM BIOS data area access, use the "pVDMBase"
 *  macro:
 *
 *      pVDMBase(hvdm)->rb_field = 0;
 *
 *  For instance data, reference similarly with the "pVDMData" macro:
 *
 *      pVDMData(hvdm)->fFgnd = TRUE;
 *
 *  And if you have a PVOID (p) created with PFROMVP or PFROMVADDR, you can
 *  use the "pVDM" macro to convert the pointer to one that is valid for the
 *  given VDM and of the given type:
 *
 *      *pVDM(hvdm,PBYTE,p) = 0;
 *
 *  An alternative to "pVDMData" (but not preferred, because it is less
 *  maintainable and overrides type-checking) if you don't want to put all
 *  your instance data in a structure, is:
 *
 *      REFHVDM(hvdm,BOOL,VDMData.fFgnd) = TRUE;
 *
 *  Note that for any *resident* instance data, the same guidelines apply;
 *  simply substitute VDMRESDATA for VDMDATA.
 */


// To reference pre-defined VDM data (ie, ROM BIOS stuff) with an hvdm
#define pVDMBase(hvdm)          ((PVDMBASE)(hvdm))

// To reference instance data with an hvdm
#define pVDMData(hvdm)          ((PVDMDATA)((ULONG)&VDMData+(ULONG)(hvdm)))
#define pVDMResData(hvdm)       ((PVDMRESDATA)((ULONG)&VDMResData+(ULONG)(hvdm)))

// To reference arbitrary VDM data with an hvdm and arbitrary pointer
#define pVDM(hvdm,type,p)       ((type)((PBYTE)(p)+(ULONG)(hvdm)))

// To dereference an instance variable with a hvdm (not preferred)
#define REFHVDM(hvdm,type,var)  (*((type *)((PBYTE)&(var) + (ULONG)(hvdm))))

#ifdef  VDHSTRICT

#define ASSERTTASK            VDHAssertTask()
#define ASSERTVDMTASK         VDHAssertVDMTask()
#define ASSERTTASKONLY        VDHAssertTaskOnly()
#define ASSERTVDMTASKONLY     VDHAssertVDMTaskOnly()
#define ASSERTINITONLY        VDHAssertInitOnly()
#define CHECKHVDM(hvdm)       VDHCheckHVDM(hvdm)

#else

#define ASSERTTASK            //
#define ASSERTVDMTASK         //
#define ASSERTTASKONLY        //
#define ASSERTVDMTASKONLY     //
#define ASSERTINITONLY        //
#define CHECKHVDM(hvdm)       //

#endif




//ifdef VDHSTRICT
//      ASSERTTASK        macro
//                  PCall VDHAssertTask
//                  endm
//      ASSERTVDMTASK     macro
//                  PCall VDHAssertVDMTask
//                  endm
//      ASSERTTASKONLY    macro
//                  PCall VDHAssertTaskOnly
//                  endm
//      ASSERTVDMTASKONLY macro
//                  PCall VDHAssertVDMTaskOnly
//                  endm
//      ASSERTINITONLY    macro
//                  PCall VDHAssertInitOnly
//                  endm
//      CHECKHVDM         macro hvdm
//                  PCall VDHCheckHVDM, hvdm
//                  endm
//else
//      ASSERTTASK        macro
//                        endm
//      ASSERTVDMTASK     macro
//                        endm
//      ASSERTTASKONLY    macro
//                        endm
//      ASSERTVDMTASKONLY macro
//                        endm
//      ASSERTINITONLY    macro
//                        endm
//      CHECKHVDM         macro hvdm
//                        endm
//endif



/****************************************/
/*      VDMM Function Prototype         */
/****************************************/


//      Memory Management Function Prototypes

BOOL        VDHENTRY VDHAllocSharedMem(ULONG, PULONG, PULONG);     //175866
VOID        VDHENTRY VDHFreeSharedMem(ULONG, ULONG);               //175866
PVOID       VDHENTRY VDHAllocMem(ULONG cb, ULONG flOptions);
VOID        VDHENTRY VDHFreeMem(PVOID);
PVOID       VDHENTRY VDHAllocDosMem(ULONG cb);
HBLOCK      VDHENTRY VDHCreateBlockPool(ULONG cbBlock, ULONG flOptions);
PVOID       VDHENTRY VDHAllocBlock(HBLOCK);
VOID        VDHENTRY VDHFreeBlock(HBLOCK,PVOID);
VOID        VDHENTRY VDHDestroyBlockPool(HBLOCK);
VOID        VDHENTRY VDHCopyMem(PVOID,PVOID,ULONG);
BOOL        VDHENTRY VDHExchangeMem(PVOID,PVOID,ULONG);

/* bwb start */
struct pagelist_s *ppl;

HLOCK       VDHENTRY VDHLockMem(PVOID,
                                ULONG,
                                ULONG,
                                struct pagelist_s *ppl,
                                PULONG);
#if 0
HLOCK       VDHENTRY VDHLockMem(PVOID,ULONG,ULONG,PVOID,PVOID);
#endif
/* bwb end */

VOID        VDHENTRY VDHUnlockMem(HLOCK);
SEL         VDHENTRY VDHQuerySel(PVOID);
PVOID       VDHENTRY VDHQueryLin(F16PVOID);
SEL         VDHENTRY VDHCreateSel(PVOID,ULONG);
VOID        VDHENTRY VDHDestroySel(SEL);
ULONG       VDHENTRY VDHGetCodePageFont(ULONG,ULONG,PPVOID);
VOID        VDHENTRY VDHReleaseCodePageFont(PVOID);
ULONG       VDHENTRY VDHAllocUserMem(ULONG, ULONG, PVOID);
ULONG       VDHENTRY VDHFreeUserMem(PVOID);
ULONG       VDHENTRY VDHSetUserMem(PVOID, ULONG, ULONG);
ULONG       VDHENTRY VDHQueryUserMem(PVOID, PULONG, PULONG);
VOID        VDHENTRY VDHQueryArena(ULONG, PULONG);

//      Page Management Function Prototypes

PVOID       VDHENTRY VDHAllocPages(PVOID,ULONG,ULONG);
PVOID       VDHENTRY VDHReallocPages(PVOID,ULONG,ULONG);
VOID        VDHENTRY VDHFreePages(PVOID);
PVOID       VDHENTRY VDHFindFreePages(PVOID,PULONG);
ULONG       VDHENTRY VDHGetDirtyPageInfo(HVDM,PVOID,ULONG);
ULONG       VDHENTRY VDHQueryFreePages(VOID);
BOOL        VDHENTRY VDHReservePages(PVOID,ULONG);
VOID        VDHENTRY VDHUnreservePages(PVOID,ULONG);
BOOL        VDHENTRY VDHMapPages(PVDHMAPSOURCE,PVDHMAPTARGET,ULONG);
BOOL        VDHENTRY VDHInstallFaultHook(HVDM,PVOID,ULONG,PFAULTHOOK,BOOL);
VOID        VDHENTRY VDHRemoveFaultHook(HVDM,PVOID,ULONG,PFAULTHOOK);
PVOID       VDHENTRY VDHAllocDMABuffer(ULONG,BOOL,PULONG);
VOID        VDHENTRY VDHFreeDMABuffer(PVOID);


//      Inter-DD Communication Function Prototypes

FPFNPDD     VDHENTRY VDHOpenPDD(PSZ,FPFNVDD);
BOOL        VDHENTRY VDHRegisterVDD(PSZ,PFNSYSREQ,PFNDEVREQ);
HVDD        VDHENTRY VDHOpenVDD(PSZ);
VOID        VDHENTRY VDHCloseVDD(HVDD);
BOOL        VDHENTRY VDHRequestVDD(HVDD,HVDM,ULONG,PVOID,PVOID);
ULONG       VDHENTRY VDHGetError(VOID);
VOID        VDHENTRY VDHSetError(ULONG);
BOOL        VDHENTRY VDHInstallUserHook(ULONG,PUSERHOOK);
BOOL        VDHENTRY VDHEnumerateVDMs(PENUMHOOK,ULONG);
HVDM        VDHENTRY VDHHandleFromSGID(SGID);
HVDM        VDHENTRY VDHHandleFromPID(PID);
ULONG       VDHENTRY VDHQuerySysValue(HVDM,ULONG);
VOID        VDHENTRY VDHPutSysValue(ULONG,ULONG);
BOOL        VDHENTRY VDHOpen(PSZ,PHFILE,PULONG,ULONG,ULONG,ULONG,ULONG,PVOID);
VOID        VDHENTRY VDHClose(HFILE);
ULONG       VDHENTRY VDHRead(HFILE,PVOID,ULONG);
ULONG       VDHENTRY VDHWrite(HFILE,PVOID,ULONG);
ULONG       VDHENTRY VDHSeek(HFILE,ULONG,ULONG);
BOOL        VDHENTRY VDHDevIOCtl(HFILE,ULONG,ULONG,PVOID,ULONG,PULONG,PVOID,ULONG,PULONG);
BOOL        VDHENTRY VDHFSCtl(PVOID,ULONG,PULONG,PVOID,ULONG,PULONG,ULONG,PSZ,HFILE,ULONG);
BOOL        VDHENTRY VDHPhysicalDisk(ULONG,PULONG,ULONG,PULONG,ULONG);
ULONG       VDHENTRY VDHViewHandle(HFILE,USHORT);
ULONG       VDHENTRY VDHJFNFromHandle(HFILE);


//      Semaphore Function Prototypes

BOOL        VDHENTRY VDHCreateSem(PHVDHSEM, ULONG);
VOID        VDHENTRY VDHDestroySem(HVDHSEM);
VOID        VDHENTRY VDHQuerySem(HVDHSEM,PVDHSEMSTATE);
VOID        VDHENTRY VDHResetEventSem(HVDHSEM);
VOID        VDHENTRY VDHPostEventSem(HVDHSEM);
BOOL        VDHENTRY VDHWaitEventSem(HVDHSEM,ULONG);
BOOL        VDHENTRY VDHRequestMutexSem(HVDHSEM,ULONG);
VOID        VDHENTRY VDHReleaseMutexSem(HVDHSEM);

//      Timer Function Prototypes

VOID        VDHENTRY VDHArmTimerHook(HHOOK,ULONG,HVDM);
BOOL        VDHENTRY VDHDisarmTimerHook(HHOOK);

//      Sound Function Prototypes

BOOL        VDHENTRY VDHDevBeep(ULONG, ULONG);

//      Property Function Prototypes

BOOL        VDHENTRY VDHRegisterProperty(PSZ,PSZ,ULONG,VPTYPE,VPORD,ULONG,PVOID,PVOID,PFNVDHRP);
ULONG       VDHENTRY VDHQueryProperty(PSZ);
BOOL        VDHENTRY VDHDecodeProperty(PPSZ,PULONG,PULONG,ULONG);

//      Process Control Function Prototypes

BOOL        VDHENTRY VDHPopup(PSZZ,ULONG,ULONG,PULONG,ULONG,PSZ);
VOID        VDHENTRY VDHKillVDM(HVDM);
VOID        VDHENTRY VDHHaltSystem(VOID);
VOID        VDHENTRY VDHSetPriority(HVDM,ULONG,LONG);
BOOL        VDHENTRY VDHFreezeVDM(HVDM);
VOID        VDHENTRY VDHThawVDM(HVDM);
BOOL        VDHENTRY VDHIsVDMFrozen(HVDM);
VOID        VDHENTRY VDHYield(BOOL);
BOOL        VDHENTRY VDHCreateThread(PVDDTID, PVDDTHREAD);
VOID        VDHENTRY VDHExitThread(ULONG);
BOOL        VDHENTRY VDHInstallFaultHandler(PVOID);

//      Perfview Prototypes

INT         VDHENTRY VDHRegisterPerfCtrs(PBYTE,PBYTE,ULONG);

//      Miscellaneous Function Prototypes (for kernel debugging only)

VOID        VDHENTRY VDHAssertTask(VOID);
VOID        VDHENTRY VDHAssertVDMTask(VOID);
VOID        VDHENTRY VDHAssertTaskOnly(VOID);
VOID        VDHENTRY VDHAssertVDMTaskOnly(VOID);
VOID        VDHENTRY VDHAssertInitOnly(VOID);
VOID        VDHENTRY VDHCheckHVDM(HVDM);
INT         CENTRY   VDHPanic(PCHAR, ...);
VOID        VDHENTRY VDHSetTimer0(ULONG);
ULONG       VDHENTRY VDHSetMaxFileHandles(ULONG);


#define     PANIC VDHPanic



//;*** PANICXX - Issue a panic if condition XX is set
//;
//;   ENTRY
//;       msg    quoted error message
//;
//;   EXAMPLES
//;       PANIC  <"VPICSetVIRR: invalid handle %u">,<h>
//;
//;     or
//;
//;       IFDEF  VDDSTRICT
//;       or     ebx,ebx
//;       PANICZ <"em86ReflectInt: null pointer">
//;       ENDIF
//;
//
//?PANIC  macro   cond,msg,args
//        local   l1,szMsg
//        ifndef  VDHPanic
//        DefCode IMPORT,GLOBAL,C
//        DefFn   VDHPanic
//        EndCode
//        endif
//        ifnb    <cond>
//        j&cond  short l1
//        endif
//        ?a substr <msg>,1,1
//        % ifidn <?a>,<">
//        DefData LOCAL,CONST,C
//        SZ      szMsg,<msg,0>
//        EndData
//        CallFn  VDHPanic,<FLAToffset szMsg>,args
//        else
//        CallFn  VDHPanic,<FLAToffset msg>,args
//        endif
//        ifnb    <cond>
//l1:
//        endif
//        endm
//
//PANIC   equ     <?PANIC ,>
//PANICC  equ     <?PANIC nc,>
//PANICNC equ     <?PANIC c,>
//PANICZ  equ     <?PANIC nz,>
//PANICNZ equ     <?PANIC z,>
//PANICE  equ     <?PANIC ne,>
//PANICNE equ     <?PANIC e,>
//PANICB  equ     <?PANIC ae,>
//PANICBE equ     <?PANIC a,>
//PANICA  equ     <?PANIC be,>
//PANICAE equ     <?PANIC b,>


#ifdef  VDDDEBUG

INT     CENTRY   printf(PCHAR, ...);

//c-begin
#define PRINTDEBUG  printf
//c-end

//masm-begin
//PRINTDEBUG macro msg,args
//        local   szMsg
//        ifndef  printf
//        DefCode IMPORT,GLOBAL,C
//        DefFn   printf
//        EndCode
//        endif
//        ?a substr <msg>,1,1
//        % ifidn <?a>,<">
//        DefData LOCAL,CONST,C
//        SZ      szMsg,<msg,0>
//        EndData
//        CallFn  printf,< <FLAToffset szMsg>,<args> >
//        else
//        CallFn  printf,< <FLAToffset msg>,<args> >
//        endif
//        endm
//masm-end

#else


#define PRINTDEBUG(x)


//masm-begin
//PRINTDEBUG macro msg
//      endm
//masm-end

#endif
