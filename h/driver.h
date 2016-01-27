/***************************************************************************/
/* IBM CONFIDENTIAL                                                        */
/* OCO Source Materials                                                    */
/* IBM TCP/IP for OS/2                                                     */
/* (c) Copyright IBM Corp 1992,1997                                        */
/* The source code for this program is not published or otherwise          */
/* divested of its trade secrets, irrespective of what has been            */
/* deposited with the U.S. Copyright Office                                */
/***************************************************************************/
#ifndef DRIVER_H
#define DRIVER_H
#ifndef OS2_INCLUDED
typedef unsigned char  BYTE;
typedef unsigned long  ULONG;
typedef unsigned short USHORT;
typedef long           LONG;
#endif

/* request packet structure */
typedef struct Request_packet {
   BYTE reqlength;        /* length of request packet */
   BYTE devunit;          /* device unit code */
   BYTE reqcommand;       /* command passed to driver */
   USHORT reqstatus;      /* return status bits here */
   ULONG  reqreserved;    /* reserved */
   ULONG  quelink;        /* queue linkage */
   BYTE   fcategory;      /* IO function category */
   BYTE   fcode;          /* IO function code */
   LONG  *GioParams;   /* data passed to driver */
   LONG  *GioData;     /* data returned from driver */
   USHORT reqfileno;      /* System File number */
}  *lpRequest;

/* DD initialization time structure */
typedef struct Init_packet {
   BYTE reqlength;        /* length of request packet */
   BYTE devunit;          /* device unit code */
   BYTE reqcommand;       /* command passed to driver */
   USHORT reqstatus;      /* return status bits here */
   ULONG reqreserved;     /* reserved */
   ULONG quelink;         /* queue linkage */
   BYTE  fcategory;       /* IO function category */
   ULONG lpDevHlp;        /* address of device helper routine */
   ULONG InitArgs;        /* initialization args */
   BYTE  drivenum;        /* drive number for 1st block device unit */
}  *lpInit_Packet;

#define MAKEULONG(l, h)  ((ULONG)(((USHORT)(l)) | ((ULONG)((USHORT)(h))) << 16))

#define INIT  0
#define READ  4
#define OPEN  0xd
#define CLOSE 0xe
#define GENIO 0x10
#define GENDEV 11

#define INIT_COMPLETE 0x1f
#define DEINSTALL     0x14

#define DONE_BIT 0x100
#define GEN_ERR  0x8000
#define ERR_UNKNOWNCOMMAND 3
#define ERR_GENERALFAILURE 0xc

struct WORDREGS {
   unsigned short ax;
   unsigned short bx;
   unsigned short cx;
   unsigned short dx;
   unsigned short si;
   unsigned short di;
   unsigned short cflag;
   unsigned short es;
};

struct BYTEREGS {
   unsigned char al, ah;
   unsigned char bl, bh;
   unsigned char cl, ch;
   unsigned char dl, dh;
};

union REGS {
   struct WORDREGS x;
   struct BYTEREGS h;
};

#define DevHlp_Yield               2
#define DevHlp_TCYield             3
#define DevHlp_Block               4
#define DevHlp_Run                 5
#define DevHlp_SemRest             6
#define DevHlp_SemClear            7
#define DevHlp_SemHandle           8
#define DevHlp_QueueInit          15
#define DevHlp_QueueFlush         16
#define DevHlp_QueueWrite         17
#define DevHlp_QueueRead          18
#define DevHlp_Lock               19
#define DevHlp_PhysToVirt         21
#define DevHlp_VirtToPhys         22
#define DevHlp_PhysToUVirt        23
#define DevHlp_AllocPhys          24
#define DevHlp_FreePhys           25
#define DevHlp_SetIRQ             27
#define DevHlp_UnSetIRQ           28
#define DevHlp_SetTimer           29
#define DevHlp_MonitorCreate      31
#define DevHlp_Register           32
#define DevHlp_DeRegister         33
#define DevHlp_MonWrite           34
#define DevHlp_MonFlush           35
#define DevHlp_GetDOSVar          36
#define DevHlp_VerifyAccess       39
#define DevHlp_AllocGDTSelector   45
#define DevHlp_PhysToGDTSelector  46
#define DevHlp_RealToProt         47
#define DevHlp_ProtToReal         48
#define DevHlp_EOI                49
#define DevHlp_UnPhysToVirt       50
#define DevHlp_TickCount          51
/*
//#define MAXGDTS 128
//#define MAXGDTS 300
//#define MAXGDTS 400
//#define MAXGDTS 500
extern short MAXGDTS;
*/
/* sanjay Khanna : July 6, 1994. Increased MAx GDTs to 60 (from 40)*/
/* sanjay Khanna : Sept 26, 1994. Increased MAx GDTs to 80 (from 60)*/
/* added a variable called TotGDTs = 500, sk may 8 1995 */
/* This var is declared in memory.c and is the total size of GDTsels[] */
/* By doing this I can get variable amount of memory from the device= */
/* statement at init time for offload. I do not have to recompile also */


#define NDIS_INTERFACE_UP       1
#define NDIS_INTERFACE_DOWN     2
#define NDIS_ADM_INTERFACE_UP   3
#define NDIS_ADM_INTERFACE_DOWN 4
#define NDIS_MTU_CHANGE         5
#ifdef SOCKETS
//extern int _initflag; /* 0 until init is complete*/
extern short _initflag; /* 0 until init is complete*/
#endif


struct GlobalDescReg {
   unsigned short limit;
   unsigned long  base;
};

struct DescInfo {
   short limit16;       /* first 16 bits of limit (total 20 bits) */
   unsigned short base16; /* low 16 bits of base address */
   unsigned int
   base24:8, /* next 8 bits of base address */
   type:4, /* segment type, data(0-7) or code(8-15) */
   s:1, /* system segment if clear, else data or code seg */
   dpl:2, /* descr previlige level */
   p:1, /* segment preset if set */
   limit20:4, /* remaining limit 4 bits  */
   avl:1,  /* available for use by system software */
   o:1, /* default operation size, 0=16bit, 1=32bit only for code seg*/
   d:1, /* if set 32 bit operands, else 16 bit operands */
   g:1, /* granularity bit, if set truns scaling on by a factor of 4096 */
   base32:8;
};

#ifndef _16BITCODE

/* Global Info Seg */

typedef struct _GINFOSEG {
    unsigned long    time;               /* time in seconds                           */
    unsigned long    msecs;              /* milliseconds                              */
    unsigned char    hour;               /* hours                                     */
    unsigned char    minutes;            /* minutes                                   */
    unsigned char    seconds;            /* seconds                                   */
    unsigned char    hundredths;         /* hundredths                                */
    unsigned short  timezone;           /* minutes from UTC                          */
    unsigned short  cusecTimerInterval; /* timer interval (units = 0.0001 seconds)   */
    unsigned char    day;                /* day                                       */
    unsigned char    month;              /* month                                     */
    unsigned short  year;               /* year                                      */
    unsigned char    weekday;            /* day of week                               */
    unsigned char    uchMajorVersion;    /* major version number                      */
    unsigned char    uchMinorVersion;    /* minor version number                      */
    unsigned char    chRevisionLetter;   /* revision letter                           */
    unsigned char    sgCurrent;          /* current foreground session                */
    unsigned char    sgMax;              /* maximum number of sessions                */
    unsigned char    cHugeShift;         /* shift count for huge elements             */
    unsigned char    fProtectModeOnly;   /* protect mode only indicator               */
    unsigned short  pidForeground;      /* pid of last process in forground session  */
    unsigned char    fDynamicSched;      /* dynamic variation flag                    */
    unsigned char    csecMaxWait;        /* max wait in seconds                       */
    unsigned short  cmsecMinSlice;      /* minimum timeslice (milliseconds)          */
    unsigned short  cmsecMaxSlice;      /* maximum timeslice (milliseconds)          */
    unsigned short  bootdrive;          /* drive from which the system was booted    */
    unsigned char    amecRAS[32];        /* system trace major code flag bits         */
    unsigned char    csgWindowableVioMax;/* maximum number of VIO windowable sessions */
    unsigned char    csgPMMax;           /* maximum number of pres. services sessions */
} GINFOSEG;

/* Local Info Seg */

typedef struct _LINFOSEG {
    unsigned short     pidCurrent;        /* current process id                     */
    unsigned short     pidParent;         /* process id of parent                   */
    unsigned short     prtyCurrent;       /* priority of current thread             */
    unsigned short     tidCurrent;        /* thread ID of current thread            */
    unsigned short     sgCurrent;         /* session                                */
    unsigned char      rfProcStatus;      /* process status                         */
    unsigned char      dummy1;
    unsigned short     fForeground;       /* current process has keyboard focus     */
    unsigned char      typeProcess;       /* process type                           */
    unsigned char      dummy2;
    unsigned short     selEnvironment;    /* environment selector                   */
    unsigned short     offCmdLine;        /* command line offset                    */
    unsigned short     cbDataSegment;     /* length of data segment                 */
    unsigned short     cbStack;           /* stack size                             */
    unsigned short     cbHeap;            /* heap size                              */
    unsigned short     hmod;              /* module handle of the application       */
    unsigned short     selDS;             /* data segment handle of the application */
} LINFOSEG;

#ifdef SOCKETS
struct initfs {
  void  (* _Far32 _Pascal initfunction)(char *);
};
extern unsigned short    _GDTsels[];
#define GDTsels   _GDTsels

extern short _MAXGDTS;
#define MAXGDTS _MAXGDTS

extern char * _gdv_yieldflag;
#define gdv_yieldflag _gdv_yieldflag


extern unsigned long * _ptkssbase;
#define ptkssbase  _ptkssbase
extern char * _gdv_tcyieldflag;
#define gdv_tcyieldflag _gdv_tcyieldflag

extern unsigned long * _gmilseconds;
#define gmilseconds _gmilseconds

extern unsigned long * _gseconds;
#define gseconds _gseconds

#define INIT_FUNCTION_BASE      0x80

extern short _familyCount;
#define familyCount _familyCount

extern struct initfs _InitFunctionArray[];
#define InitFunctionArray  _InitFunctionArray
extern GINFOSEG *_pGInfo;
extern LINFOSEG *_pLInfo;
#define pGInfo _pGInfo
#define pLInfo _pLInfo
extern unsigned long _debug_flag;
#define debug_flag  _debug_flag

#else /* sockets only please */

#define in_cksum _in_cksum
extern unsigned long * hostid;
#define ether_output output

#endif /* sockets only pls */

#ifndef ComPort
extern short _ComPort;
#define ComPort _ComPort
#endif

/* Changed sstods function to macro. amol 10/10/96 */
extern unsigned long * ptkssbase;   /* this will be the pointer to SS base (For sstods) */
#define sstods(x)   ((void *) ((unsigned long) (x) + *ptkssbase))
#ifndef SSToDS
#define SSToDS(p)    sstods(p)
#endif


#endif /* ifndef _16BITCODE */

#define   FLAT2LDT(x)    ((((x&0xffff0000)<<3)+0x70000) + (x&0xffff))
#define   LDT2FLAT(ldt)  ((((u_long)(ldt)>>3) & 0xffff0000) + ((u_long)(ldt)&(0x0000ffff)))

//int sleep(unsigned long,unsigned);
/* int sleep(char *wait_addr, unsigned priority); */

#endif /* DRIVER_H */
