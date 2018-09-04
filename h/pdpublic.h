/************************* Module Header *******************************\
*                                                                      *
* Module Name: PDPUBLIC.H     (new file for 180062)                    *
*                                                                      *
* Process Dump public interface for DosDumpProcess API                 *
*                                                                      *
* Copyright (c) International Business Machines Corporation 1997       *
************************************************************************
*  Change                                                              * 
                            *
*  Activity     Date            BY      Reason                         *
*  --------      ----                   ----------                     *
* 180062        09/01/97        LRN     New for DosDumpProcess and     *
*                                       DosQueryRasInfo enhancements   *
\***********************************************************************/

//XLATOFF
#ifndef  __PDPUBLIC__
#define  __PDPUBLIC__

#pragma pack(1)

/* The following are the error return codes for DosDumpProcess */

#define ERROR_INVALID_PATHNAME               70000
#define ERROR_INVALID_PROCESSNAME            70001
#define ERROR_INVALID_ADDRRANGE              70002
#define ERROR_INVALID_SYSTEM_OPTIONS         70003
#define ERROR_INVALID_USER_OPTIONS           70004
#define ERROR_OUT_OF_FREESPACE               70005
#define ERROR_INVALID_PARAMETERS             70006
#define ERROR_UPDATE_FAILED                  70007
#define ERROR_DATA_NOT_DISPLAYED             70008
#define ERROR_INVALID_LOCATION               70009
#define ERROR_INVALID_PID                    70010
#define ERROR_INVALID_PROCESS                70011
#define ERROR_INVALID_NUMBER                 70012
#define ERROR_PDUMP_NOT_ENABLED              70013
#define ERROR_NO_PROCESSES_FOUND             70014
#define ERROR_INTERNAL_ERROR                 70015
#define ERROR_INVALID_FREESPACE              70016
#define ERROR_DUPLICATE_PARAMETER            70017
#define ERROR_NOTVALID_IN_CONFIGSYS          70018
#define ERROR_INVALID_PARENTS                70019
#define ERROR_INVALID_CHILDREN               70020
#define ERROR_INVALID_KILL                   70021
#define ERROR_INVALID_UPDATE_OPTION          70022
#define ERROR_INVALID_DO_PARAMETER           70023
#define ERROR_INVALID_DO_PD_COMBINATION      70024
#define ERROR_CANT_OPEN_DUMP_FILE            70025  /* drive or directory invalid */
#define ERROR_INVALID_PADDRALL               70026

#define PACKET_DDPREQUEST_REV   1               /* 1st revision */

typedef struct _DDPREQUEST {
    ULONG              packet_size;             /* Size (in bytes) of this packet */
    ULONG              packet_revision_number;  /* PACKET_DDPREQUEST_REV */
    ULONG              flags;                   /* See DDPREQUEST flags below */
    PSZ                pPathName;               /* Ptr to drive letter or pathname Null terminated. */
    ULONG              FreeSpace;               /* Amount (in KB) of free space that should remain on drive after dump is complete. */
    struct _PDOPTIONS *pSystemDefaults;         /* Ptr to System Defaults struct  */
    struct _PDOPTIONS *pUserDefaults;           /* Ptr to User Defaults struct    */
    struct _PDPROCESS *pProcessInfo;            /* Ptr to linked list of structures which describe what to dump for particular process(es) */
    ULONG              NumberofProcesses;       /* Number of processes in the linked list pointed to by pProcessInfo. Only specified when pProcessInfo non-null. */
} DDPREQUEST;
typedef DDPREQUEST *PDDREQUEST;


/* Defines for Flag field (first param on DosDumpProcess). These are
    mutually exclusive. */
#define DDP_DISABLEPROCDUMP       0x00000000L   /* Disable PD facility */
#define DDP_ENABLEPROCDUMP        0x00000001L   /* Enable PD facility  */
#define DDP_PERFORMPROCDUMP       0x00000002L   /* Force a PD dump     */
#define DDP_DDPREQUEST            0x10000000L   /* New type request    */


/* Defines for DDPREQUEST flags (flags field within DDPREQUEST structure  */
/* DDP_DISABLEPROCDUMP, DDP_ENABLEPROCDUMP and DDP_PERFORMPROCDUMP can also be specified in the DDPREQUEST flags. */
/*#define DDP_ENABLEPROCDUMP      0x00000001L*/ /* Enable PD facility...mutually exclusive with other DDP flags except for DDP_CHANGE_FREESPACE. Can specify pPathName and/orFreeSpace (nulls will use defaults). To specify zero for FreeSpace, specify DDP_CHANGE_FREESPACE. */
/*#define DDP_PERFORMPROCDUMP     0x00000002L*/ /* Force a PD dump...mutually exclusive with other DDP flags. Must specify pProcessInfo and NumberOfProcesses. */
#define DDP_SETOPTIONS            0x00000010L   /* Set options...mutually exclusive with other DDP flags except for DDP_CHANGE_FREESPACE. Used for: pPathName, FreeSpace, pSystemDefaults, pUserDefaults, pProcessInfo/NumberOfProcesses. Specify null/zeros for options you do not want to set. To set zero for FreeSpace, specify DDP_CHANGE_FREESPACE. */
#define DDP_REQDISABLEPROCDUMP    0x00000020L   /* Disable PD facility...mutually exclusive with other DDP flags */

/* Any combination of these "RESET" flags which follow can be set together, but none of the following can be set when DDP_SETOPTIONS is specified. */
#define DDP_RESETSYSTEMDEFAULTS   0x00000040L   /* Reset System defaults */
#define DDP_RESETUSERDEFAULTS     0x00000080L   /* Reset User defaults */
#define DDP_RESETPATHNAME         0x00000100L   /* Reset Pathname */
#define DDP_RESETFREESPACE        0x00000200L   /* Reset Free Space */
#define DDP_RESETPROCESS          0x00000400L   /* Reset Process (by name or PID) */

/* The following flag can only be used when set with DDP_ENABLEPROCDUMP and
  * DDP_SETOPTIONS. The value passed in FreeSpace (even if zero) will be used.
  * This is needed because 0 is a valid value and we need to distinguish whether
  * the requestor really means 0 or didn't want to change it.
*/
#define DDP_CHANGE_FREESPACE      0x00001000L

/* Mask of reserved flags (not valid for flags field within DDPREQUEST structure) */
#define DDP_RESERVED_FLAGS        0xFFFFE80CL

#define PACKET_PDOPTIONS_REV   1                 /* 1st revision */

typedef struct _PDOPTIONS {
    ULONG                packet_size;            /* Size (in bytes) of this packet */
    ULONG                packet_revision_number; /* PACKET_PDOPTIONS_REV */
    ULONG                process_flags;          /* See PDOPTIONS process flags below.*/
    ULONG                system_flags;           /* See PDOPTIONS system flags below. */
    struct _PDADDRRANGE *pLAddrRanges;           /* Ptr to list of linear address ranges to dump. */
    ULONG                NumberofLAddrRanges;    /* Number of LAddrRanges. */
    struct _PDADDRRANGE *pPAddrRanges;           /* Ptr to list of physical address ranges to dump. */
    ULONG                NumberofPAddrRanges;    /* Number of PAddrRanges. */
    ULONG                options_flags;
} PDOPTIONS;

typedef PDOPTIONS *PPDOPTIONS;

/* PDOPTIONS options_flags field */
#define PDO_UPDATEOPTIONS       0x00000001  /* Update/merge options. This flag is only valid when updating system and user defaults (pSystemDefaults and pUserDefaults).*/


//XLATON
#define DDP_NONE                0x00000000  /* No bits set.  */
/* PDOPTIONS process flags -- flags with a process-wide scope */
#define DDP_SUMM                0x00000001  /* Summary data      */
#define DDP_PRIVATE             0x00000100  /* Process private data     */
#define DDP_SHARED              0x00000200  /* Shared data for process */
#define DDP_INSTANCE            0x00000400  /* Instance data for process */
#define DDP_MVDM                0x00000800  /* MVDM data                */
#define DDP_SEM                 0x00001000  /* Process semaphore data */
#define DDP_USER_DEFAULTS       0x40000000  /* Include user default data */

/* Reserved process flags */
#define DDP_RESERVED_PROCESS    0xBFFFE0FE

/* PDOPTIONS system flags -- flags with a system-wide scope */
#define DDP_SYSSUMM             DDP_SUMM    /* System summary data      */
#define DDP_IDT                 0x00000004  /* IDT*/
#define DDP_SYSLDR              0x00000010  /* System loader data       */
#define DDP_SYSFS               0x00000020  /* System file system loader data */
#define DDP_SYSVM               0x00000040  /* System virtual memory data */
#define DDP_SYSTK               0x00000080  /* System tasking data      */
#define DDP_SYSMVDM             DDP_MVDM    /* System MVDM data         */
#define DDP_SYSSEM              DDP_SEM     /* System  semaphore data */
#define DDP_KRHEAPS             0x00002000  /* System kernel resident heap */
#define DDP_KSHEAPS             0x00004000  /* System kernel swappable heap */
#define DDP_TRACE               0x00008000  /* System trace data        */
#define DDP_STRACE              0x00010000  /* System software trace data */
#define DDP_SMP                 0x00020000  /* System SMP data */
#define DDP_SYSIO               0x00040000  /* System I/O data  */
#define DDP_SYSPG               0x00080000  /* System paging data */
#define DDP_SYSTEM_DEFAULTS     0x20000000  /* Include system default data */
#define DDP_PADDRALL            0x80000000  /* All physical memory PADDR(ALL) */

//XLATOFF

/* Reserved system flags */
#define DDP_RESERVED_SYSTEM     0x5FF0070A

/* PDUMPSYS defaults. */
#define DDP_SYS_PROC_DEFAULTS   DDP_NONE                    /* process_flags */
#define DDP_SYS_SYSTEM_DEFAULTS DDP_SMP + DDP_SYSSUMM + DDP_IDT + DDP_SYSFS + \
                                 DDP_SYSVM + DDP_SYSSEM + DDP_SYSPG + DDP_SYSIO + \
                                 DDP_TRACE + DDP_STRACE + \
                                 DDP_SYSTK                  /* system_flags */
/* PDUMPUSR defaults. */
#define DDP_USR_PROC_DEFAULTS   DDP_SUMM + \
                                 DDP_MVDM + DDP_SEM         /* process_flags */
#define DDP_USR_SYSTEM_DEFAULTS DDP_SYSFS                   /* system_flags  */

/* Flags set when "ALL" keyword is specified */
#define DDP_ALL_PROCESS         DDP_SUMM + DDP_PRIVATE + DDP_SHARED + \
                                 DDP_INSTANCE + DDP_MVDM + DDP_SEM
#define DDP_ALL_SYSTEM          DDP_SYSSUMM + DDP_IDT + DDP_SYSLDR + \
                                 DDP_SYSFS + DDP_SYSVM + DDP_SYSTK + \
                                 DDP_SYSMVDM + DDP_SYSSEM + DDP_KRHEAPS + \
                                 DDP_KSHEAPS + DDP_TRACE + DDP_STRACE + \
                                 DDP_SMP + DDP_SYSIO + DDP_SYSPG

/* PDUNION is used in both the PDPROCESS and PDPROCESS2 structures. */
union _PDUNION {
    PID               ProcId;            /* Process ID. */
    PSZ               pProcessName;      /* Ptr to a process name that can either be a short name or a qualified name(w/extension). Can also have extension of ".DLL" to dump all processes using the  DLL. Must be null terminated.   */
};

#define PACKET_PDPROCESS_REV   1         /* 1st revision */

typedef struct _PDPROCESS {
    ULONG               packet_size;     /* Size (in bytes) of this packet  */
    ULONG               packet_revision_number;   /* PACKET_PDPROCESS_REV   */
    ULONG               flags;           /* See PDPROCESS flags below. */
    union  _PDUNION     Proc;
    struct _PDOPTIONS  *pOptions;        /* Pointer to dump options. */
    ULONG               Children;        /* See "Children values" below. */
    ULONG               Parents;         /* See "Parent values" below. */
    ULONG               Kill;            /* See "Kill values" below. */
    struct _PDPROCESS2 *pIncludeProcess; /* Ptr to structure of associated process(es) to include when this process is dumped. */
    struct _PDPROCESS  *pPDProcessNext;  /* Ptr to next in linked list */
} PDPROCESS;
typedef PDPROCESS *PPDPROCESS;

/* PDPROCESS flags */
#define PDP_ALLPIDS       0x00000001  /* Dump all active processes. This is only valid when you are doing a force (DDP_PERFORMPROCDUMP). Not a valid flag for PDPROCESS2.      */
#define PDP_PROCID        0x00000002  /* Structure contains a ProcId. Flag is mutually exclusive with PDP_PROCNAME. */
#define PDP_PROCNAME      0x00000004  /* Struct contains a pProcessName. Flag is mutually exclusive with PDP_PROCID.*/
#define PDP_UPDATEOPTIONS 0x00000008  /* Update/merge options with any existing options. */
/* Reserved PDProcess flags */
#define PDP_RESERVED_FLAGS  0xFFFFFFF0

/* Reserved PDProcess2 flags */
#define PDP_RESERVED_FLAGS2 0xFFFFFFF1

/* The following PDPROCESS flags are for system use only. They should not be specified on an API request. These flags are not valid for PDPROCESS2. */
#define PDP_HOTKEY        0x08000000  /* Hotkey. PDP_PROCID should also be set.*/
#define PDP_TRAP0         0x10000000  /* Ring 0 trap. */
#define PDP_TRAP3         0x20000000  /* Ring 3 (or ring 2) trap. */
#define PDP_REQ           0x40000000  /* Set by PLD_DeterminePids when request list does not have PDP_TEMP set.      */
#define PDP_TEMP          0x80000000  /* This is a temporary structure which is not pointed to by the pPDProcessHeadCurrent pointer. It is used with 'force' and contains options to use when dumping. */
#define PDP_TRAP          PDP_TRAP0 + PDP_TRAP3
#define PDP_SYS_MASK      PDP_HOTKEY + PDP_TRAP + PDP_REQ + PDP_TEMP

/* Children values -- these are mutually exclusive */
#define CHILDREN_NONE    0x00000001   /* No children will be dumped (default)*/
#define CHILDREN_ONLY    0x00000002   /* Direct children of the process(es) will be dumped. */
#define CHILDREN_GRAND   0x00000003   /* Children and grandchildren of the process(es) will be dumped. */
#define CHILDREN_ALL     0x00000004   /* All children and children of children will be dumped. */
/* Values used in validating Children */
#define CHILDREN_FIRST   CHILDREN_NONE
#define CHILDREN_LAST    CHILDREN_ALL

/* PARENT values -- these are mutually exclusive */
#define PARENTS_NONE     0x00000001   /* No parents will be dumped (default)*/
#define PARENTS_ONLY     0x00000002   /* Direct parent(s) of process(es) will be dumped. */
#define PARENTS_GRAND    0x00000003   /* Parents and grandparents of the process(es) will be dumped.*/
#define PARENTS_ALL      0x00000004   /* All parents and parents of parents will be dumped. */
/* Values used in validating Parents */
#define PARENTS_FIRST    PARENTS_NONE
#define PARENTS_LAST     PARENTS_ALL

/* KILL values -- these are mutually exclusive */
#define KILL_YES         0x00000001   /* Kill the process(es).    */
#define KILL_NO          0x00000002   /* Do not kill process(es). */

#define PACKET_PDPROCESS2_REV   1       /* 1st revision */

typedef struct _PDPROCESS2 {
    ULONG               packet_size;     /* Size (in bytes) of this packet */
    ULONG               packet_revision_number;   /* PACKET_PDPROCESS2_REV */
    ULONG               flags;           /* See PDPROCESS flags above. Valid flags within PDPROCESS2 are: PDP_PROCID, PDP_PROCNAME, and PDP_UPDATEOPTIONS. */
    union  _PDUNION     Proc;
    struct _PDOPTIONS  *pOptions;        /* Pointer to dump options. */
    ULONG               Children;        /* See "Children values" above. */
    ULONG               Parents;         /* See "Parent values" above. */
    ULONG               Kill;            /* See "Kill values" above. */
    ULONG               DataOption;      /* See "DataOption values" below. */
    struct _PDPROCESS2 *pIncludeProcess; /* Ptr to next element of linked list.  */
} PDPROCESS2;
typedef PDPROCESS2 *PPDPROCESS2;

/* DataOption values -- these are mutually exclusive */
#define DATA_PREVIOUS    0x00000001  /* Use data previously attached to this process. */
#define DATA_PRIMARY     0x00000002  /* Dump same data as primary process */
#define DATA_SPECIFIED   0x00000003  /* Data is specified in this structure (Options field). */
#define DATA_DEFAULTS    0x00000004  /* Use defaults.  */

/* Values used in validating DataOption */
#define DATA_FIRST       DATA_PREVIOUS
#define DATA_LAST        DATA_DEFAULTS


typedef struct _PDADDRRANGE {         /* Address ranges to dump  */
    ULONG    StartAddr;   /* Start of range */
    ULONG    EndAddr;     /* End of range (inclusive) */
} PDADDRRANGE;
typedef PDADDRRANGE *PPDADDRRANGE;



/* The following structures are used by the DosQueryRASInfo API */

#define PACKET_DQRIREQUEST_REV   1       /* 1st revision */
#define DQRI_DQRIREQUEST   0x10000000L   /* New Style request indicator */

typedef struct _DQRIREQUEST {
    ULONG               packet_size; /* Size (in bytes) of this packet */
    ULONG               packet_revision_number;  /* PACKET_DQRIREQUEST_REV */
    ULONG               flags;       /* See DQRIREQUEST flags below */
    ULONG               buffer_size; /* Buffer size */
    PBYTE               buffer;      /* Pointer to the buffer that will hold the returned information */
    struct _DQRITRACE  *pTrace;      /* Ptr to System Trace info  */
    struct _DQRIPDUMP  *pPdump;      /* Ptr to PDUMP info  */
    struct _DQRISTRACE *pSTrace;     /* Ptr to STrace info  */
    struct _DQRISDUMP  *pSdump;      /* Ptr to System Dump info  */
} DQRIREQUEST;
typedef DQRIREQUEST *PDQRIREQUEST;

/* Defines for DQRIREQUEST flags */
#define SYSTRACE_QUERY   0x00000001L /* Query the System Trace facility */
#define PDUMP_QUERY      0x00000002L /* Query the PDUMP facility */
#define STRACE_QUERY     0x00000004L /* Query the STRACE facility */
#define SDUMP_QUERY      0x00000008L /* Query the System Dump facility */

/* Mask of reserved flags */
#define DQRI_RESERVED_FLAGS        0xFFFFFFF0L


typedef struct _DQRITRACE {
    UCHAR                    flags1;       /* See DQRITRACE flags1 below */
    UCHAR                    flags2;       /* See DQRITRACE flags2 below */
    USHORT                   number_of_pids; /* Number of Pids in pPids, if tracing by PID */
    ULONG                    optional_data_flags; /* See DQRITRACE Optional data flags below */
    PBYTE                    addr;         /* Ptr to System Trace buffer */
    ULONG                    system_trace_buffer_size; /* Size of the Trace buffer */
    struct _DQRIMAJORMINOR  *pMajorMinor;  /* Ptr to the Static Tracepoints */
    struct _DQRIDYNAMIC     *pDynamic;     /* Ptr to Dynamic tracepoints  */
    PID                     *pPids;        /* Ptr to list of Pids if tracing by PID */
    struct _DQRINAMES       *pNames;       /* Ptr to the names defined as being traced */
} DQRITRACE;
typedef DQRITRACE *PDQRITRACE;

/* Defines for DQRITRACE flags1 */
/* These are defined in trcxport.h */
/* GST_TRCAVAIL         0x80    Not Busy        */
/* GST_TRCPAUSED        0x40    Paused  */
/* GST_TRCPID           0x20    bit set = Trace by PID  */
/*                              bit reset = Trace by all PIDs   */
/* GST_TRCERRCOUNT      0x10    Trace until error count reached */
/* GST_TRCSUSPEND       0x08    Trace suspended */
/* GST_TRCMINORCD       0x04    Trace by Minor code     */
/* GST_TRCSYSDATA       0x02    Record optional data    */
/* GST_TRCNOWRAP        0x01    Non Wrapping buffer     */

/* Defines for DQRITRACE flags2 */
/* These are defined in trxxport.h */
/* GST_QUEUED           0x01    Queued Mode             */
/* GST_DYNBYPASSIPI     0x02    Bypass Inter Processor */

/* Defines for DQRITRACE Optional Data flags */
/* These flags are defined in trcxport.h */
/* GST_SYSDATA_TID      0X00000020L     Thread ID */
/* GST_SYSDATA_SLOT     0X00000040L     Thread SLOT */
/* GST_SYSDATA_CSEIP    0X00010000L     Caller's CS:EIP */
/* GST_SYSDATA_MODNAME  0X00000080L     Module name */
/* GST_SYSDATA_SSESP    0X00020000L     Caller's SS:ESP */
/* GST_SYSDATA_PROCID   0X00000001L     Processor ID    */
/* GST_SYSDATA_TSC      0X00000400L     Pentium High Resolution Time-Stamp Counter */
/* GST_SYSDATA_PROCNAME 0X00000100L     Short name of caller */
/* GST_SYSDATA_MAJOR    0X00000002L     Extended Major code */


typedef struct _DQRIMAJORMINOR {
    ULONG                   major_code;       /* Major Code - NOT USED if in the Dynamic tracepoint structure */
    ULONG                   first_minor_code; /* First minor code in range */
    ULONG                   last_minor_code;  /* Last minor code in range */
    struct _DQRIMAJORMINOR *pMajorMinorNext;  /* Ptr to next record  */
} DQRIMAJORMINOR;
typedef DQRIMAJORMINOR *PDQRIMAJORMINOR;

typedef struct _DQRIDYNAMIC {
    CHAR                    module_name[9];   /* Name of the Module (null terminated)*/
    struct _DQRIDYNAMIC    *pDynamicNext;     /* Ptr to next record  */
    struct _DQRIMAJORMINOR *pMajorMinorNext;  /* Ptr to Major/Minor codes for this module  */
} DQRIDYNAMIC;
typedef DQRIDYNAMIC *PDQRIDYNAMIC;

typedef struct _DQRINAMES {
    char                   *pName;       /* Ptr to the name (null terminated string) */
    struct _DQRINAMES      *pNamesNext;  /* Ptr to next record  */
} DQRINAMES;
typedef DQRINAMES *PDQRINAMES;


typedef struct _DQRIPDUMP {
    ULONG              flags;            /* See DQRIPDUMP flags below */
    PSZ                pPathName;        /* Ptr to drive letter or pathname. Null terminated. */
    ULONG              FreeSpace;        /* Amount (in KB) of free space that should remain on drive after dump is complete. */
    struct _PDOPTIONS *pSystemDefaults;  /* Ptr to System Defaults struct  */
    struct _PDOPTIONS *pUserDefaults;    /* Ptr to User Defaults struct */
    struct _PDPROCESS *pProcessInfo;     /* Ptr to linked list of structures which describe what to dump for particular process(es)*/
} DQRIPDUMP;
typedef DQRIPDUMP *PDQRIPDUMP;

/* Defines for DQRIPDUMP flags */
#define DQRI_DISABLEPROCDUMP    0x00000000L /* Disabled PD facility */
#define DQRI_ENABLEPROCDUMP     0x00000001L /* Enabled PD facility */


typedef struct _DQRISTRACE {
    PPVOID   addr;                    /* Pointer to STrace buffer */
    ULONG    Strace_buffer_size;      /* Size of the Trace buffer */
    ULONG    Reserved1;               /* Reserved for later use */
} DQRISTRACE;
typedef DQRITRACE *PDQRISTRACE;


typedef struct _DQRISDUMP {
    CHAR     dump_type;                 /* System dump information */
    CHAR     dump_location;             /* Where the dump will be placed.     */
    CHAR     additional_features;       /* Options available in the facility  */
    CHAR     Reserved1;                 /* Reserved for later use */
    ULONG    kernel_data_flags;         /* Special data to be collected */
} DQRISDUMP;
typedef DQRISDUMP *PDQRISDUMP;

/* Values for dump_type */
#define SDD_OFF   0                    /* System Dump is disabled */
#define SDD_ON    1                    /* System Dump is enabled */
#define SDD_R0    2                    /* System Dump is enabled for Ring 0  */

/* Value for kernel_data_flags */
#define OTE_ON    0x00000001L          /* RASKDATA command */

/* Values for additional_feature */
/* The following are defines in module pldinfo.h */
/*#define PLD_TAKE_SYSDUMP        0*/  /* Value for PDDumpSequence (default) */
/*#define PLD_TAKE_PROCDUMP       1*/  /* Value for PDDumpSequence */


/* These structures are used by the registered device driver for process 
dump*/
typedef struct _DDREPEAT {
    ULONG                ulLinear;          /* Linear address if non-zero. Physical address if zero. */
    ULONG                pid;               /* Zero if physical addresses. Non-zero if linear addresses.  */
    ULONG                ulStartAddr;       /* Starting address. */
    ULONG                ulLength;          /* Length of data. */
} DDREPEAT;
typedef DDREPEAT *PDDREPEAT;

typedef struct _DDRANGES {
    ULONG                NumberOfEntries;   /* # Entries in this structure.   */
    DDREPEAT             Repeat;            /* This section repeats NumberOfEntries times. */
} DDRANGES;

typedef DDRANGES *PDDRANGES;

/* PLDMAXDIRNAME is the maximum size of a directory name */
/* To this directory name, we need to allow room to concatenate the filename */
/*   and the extension and the null terminator. */
/* Note: 260 is size of CCHMAXPATH defined in bsedos.h */
#define PLDMAXDIRNAME  260 - PLD_DUMPNAME_LENGTH
#define PLD_DUMPNAME_LENGTH  11       /* Length of "\PDUMP.xxx" plus null */

#pragma pack()

#endif 