/*static char *SCCSID = "@(#)newexe.h   6.3 92/03/15";*/
/*
 *      SCCSID = @(#)newexe.h   6.3 92/03/15
 *
 *  Title
 *
 *      newexe.h
 *      (C) Copyright IBM Corp 1984-1992
 *      (C) Copyright Microsoft Corp 1984-1987
 *
 *  Description
 *
 *      Data structure definitions for the DOS 4.0/Windows 2.0/OS-2
 *      executable file format.  Includes WLO definitions.
 *
 */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
#ifdef __cplusplus
      extern "C" {
#endif
/* INC */

#ifndef __NEWEXE__
#define __NEWEXE__


    /*_________________________________________________________________*
     |                                                                 |
     |                                                                 |
     |  DOS3 .EXE FILE HEADER DEFINITION                               |
     |                                                                 |
     |_________________________________________________________________|
     *                                                                 */

/* NOINC */
#if __IBMC__ || __IBMCPP__
#pragma pack(1)
#endif
/* INC */


#define EMAGIC          0x5A4D          /* Old magic number */
#define ENEWEXE         sizeof(struct exe_hdr)
                                        /* Value of E_LFARLC for new .EXEs */
#define ENEWHDR         0x003C          /* Offset in old hdr. of ptr. to new */
#define ERESWDS         0x000d          /* No. of reserved words (OLD) */
#define ERES2WDS        0x000A          /* No. of reserved words in e_res2 */
#define ECP             0x0004          /* Offset in struct of E_CP */
#define ECBLP           0x0002          /* Offset in struct of E_CBLP */
#define EMINALLOC       0x000A          /* Offset in struct of E_MINALLOC */
#define EKNOWEAS        0x0001          /* e_flags -program understands EAs */

struct exe_hdr                     /* DOS 1, 2, 3 .EXE header */
  {
  unsigned short e_magic;          /* 00 Magic number */
  unsigned short e_cblp;           /* 02 Bytes on last page of file */
  unsigned short e_cp;             /* 04 Pages in file */
  unsigned short e_crlc;           /* 06 Relocations */
  unsigned short e_cparhdr;        /* 08 Size of header in paragraphs */
  unsigned short e_minalloc;       /* 0A Minimum extra paragraphs needed */
  unsigned short e_maxalloc;       /* 0C Maximum extra paragraphs needed */
  unsigned short e_ss;             /* 0E Initial (relative) SS value */
  unsigned short e_sp;             /* 10 Initial SP value */
  unsigned short e_csum;           /* 12 Checksum */
  unsigned short e_ip;             /* 14 Initial IP value */
  unsigned short e_cs;             /* 16 Initial (relative) CS value */
  unsigned short e_lfarlc;         /* 18 File address of relocation table */
  unsigned short e_ovno;           /* 1A Overlay number */
  unsigned long  e_sym_tab;        /* 1C offset of symbol table file */
  unsigned short e_flags;          /* 20 old exe header flags  */
  unsigned short e_res;            /* 22 RESERVED WORDS */
  unsigned short e_oemid;          /* 24 OEM identifier (for e_oeminfo) */
  unsigned short e_oeminfo;        /* 26 OEM information; e_oemid specific */
  unsigned short e_res2[ERES2WDS]; /* 28 RESERVED WORDS */
  long           e_lfanew;         /* 3C File address of new exe header */
  };                               /* 40 */

#define E_MAGIC(x)      (x).e_magic
#define E_CBLP(x)       (x).e_cblp
#define E_CP(x)         (x).e_cp
#define E_CRLC(x)       (x).e_crlc
#define E_CPARHDR(x)    (x).e_cparhdr
#define E_MINALLOC(x)   (x).e_minalloc
#define E_MAXALLOC(x)   (x).e_maxalloc
#define E_SS(x)         (x).e_ss
#define E_SP(x)         (x).e_sp
#define E_CSUM(x)       (x).e_csum
#define E_IP(x)         (x).e_ip
#define E_CS(x)         (x).e_cs
#define E_LFARLC(x)     (x).e_lfarlc
#define E_OVNO(x)       (x).e_ovno
#define E_SYM_TAB(x)    (x).e_sym_tab
#define E_FLAGS(x)      (x).e_flags
#define E_RES(x)        (x).e_res
#define E_OEMID(x)      (x).e_oemid
#define E_OEMINFO(x)    (x).e_oeminfo
#define E_RES2(x)       (x).e_res2
#define E_LFANEW(x)     (x).e_lfanew

struct packhdr
  {                             /* Header of unpack module in packed exe file*/
  unsigned short packed_ip;
  unsigned short packed_cs;
  unsigned short packed_rlc;
  unsigned short packed_cbUnpack;
  unsigned short packed_sp;              /* Original SP */
  unsigned short packed_ss;              /* Original SS */
  unsigned short packed_cparExp;
  };


    /*_________________________________________________________________*
     |                                                                 |
     |                                                                 |
     |  OS/2 & WINDOWS .EXE FILE HEADER DEFINITION - 286 version       |
     |                                                                 |
     |_________________________________________________________________|
     *                                                                 */

#define NEMAGIC     0x454E  /* New magic number */
#define NERESBYTES  4       /* Bytes reserved if Windows 3.0 struct not used */
#define NECRC       8       /* Offset into new header of NE_CRC */

struct new_exe                      /* New .EXE header */
  {
  unsigned short ne_magic;        /* 00 Magic number NE_MAGIC */
  unsigned char  ne_ver;          /* 02 Version number */
  unsigned char  ne_rev;          /* 03 Revision number */
  unsigned short ne_enttab;       /* 04 Offset of Entry Table */
  unsigned short ne_cbenttab;     /* 06 Number of bytes in Entry Table */
  long           ne_crc;          /* 08 Checksum of whole file */
  unsigned short ne_flags;        /* 0C Flag word */
  unsigned short ne_autodata;     /* 0E Automatic data segment number */
  unsigned short ne_heap;         /* 10 Initial heap allocation */
  unsigned short ne_stack;        /* 12 Initial stack allocation */
  long           ne_csip;         /* 14 Initial CS:IP setting */
  long           ne_sssp;         /* 18 Initial SS:SP setting */
  unsigned short ne_cseg;         /* 1C Count of file segments */
  unsigned short ne_cmod;         /* 1E Entries in Module Reference Table */
  unsigned short ne_cbnrestab;    /* 20 Size of non-resident name table */
  unsigned short ne_segtab;       /* 22 Offset of Segment Table */
  unsigned short ne_rsrctab;      /* 24 Offset of Resource Table */
  unsigned short ne_restab;       /* 26 Offset of resident name table */
  unsigned short ne_modtab;       /* 28 Offset of Module Reference Table */
  unsigned short ne_imptab;       /* 2A Offset of Imported Names Table */
  long           ne_nrestab;      /* 2C Offset of Non-resident Names Table */
  unsigned short ne_cmovent;      /* 30 Count of movable entries */
  unsigned short ne_align;        /* 32 Segment alignment shift count */
  unsigned short ne_cres;         /* 34 Count of resource entries */
  unsigned char  ne_exetyp;       /* 36 Target operating system */
  unsigned char  ne_flagsothers;  /* 37 Other .EXE flags */
  union choice
    {
    struct Windows3
      {
      unsigned short ne_pretthunks;  /* 38 Win 3.0 - offset to return thunks */
      unsigned short ne_psegrefbytes;/* 3A Win 3.0 - offset to segment ref. bytes */
      unsigned short ne_swaparea;    /* 3C Win 3.0 - minimum code swap size */
      unsigned short ne_expver;      /* 3E Win 3.0 - expected windows version number */
      } Win3;
    struct Module
      {
      unsigned long  ne_modver;          /* 38 Module Version Field */
      char           ne_res[NERESBYTES]; /* 3C pad structure to 64 bytes */
      } Mod1;
    } choice1;
  };                              /* Structure is 64 bytes in length */


#define ne_pWinFileStruc (ne_magic + 0x0a)
#define ne_cbModName     0
#define ne_pWinModName   8


#define NE_MAGIC(x)         (x).ne_magic
#define NE_VER(x)           (x).ne_ver
#define NE_REV(x)           (x).ne_rev
#define NE_ENTTAB(x)        (x).ne_enttab
#define NE_CBENTTAB(x)      (x).ne_cbenttab
#define NE_CRC(x)           (x).ne_crc
#define NE_FLAGS(x)         (x).ne_flags
#define NE_AUTODATA(x)      (x).ne_autodata
#define NE_HEAP(x)          (x).ne_heap
#define NE_STACK(x)         (x).ne_stack
#define NE_CSIP(x)          (x).ne_csip
#define NE_SSSP(x)          (x).ne_sssp
#define NE_CSEG(x)          (x).ne_cseg
#define NE_CMOD(x)          (x).ne_cmod
#define NE_CBNRESTAB(x)     (x).ne_cbnrestab
#define NE_SEGTAB(x)        (x).ne_segtab
#define NE_RSRCTAB(x)       (x).ne_rsrctab
#define NE_RESTAB(x)        (x).ne_restab
#define NE_MODTAB(x)        (x).ne_modtab
#define NE_IMPTAB(x)        (x).ne_imptab
#define NE_NRESTAB(x)       (x).ne_nrestab
#define NE_CMOVENT(x)       (x).ne_cmovent
#define NE_ALIGN(x)         (x).ne_align
#define NE_CRES(x)          (x).ne_cres
#define NE_RES(x)           (x).ne_res
#define NE_EXETYP(x)        (x).ne_exetyp
#define NE_FLAGSOTHERS(x)   (x).ne_flagsothers
#define NE_PRETTHUNKS(x)    (x.choice1.Win3).ne_pretthunks
#define NE_PSEGREFBYTES(x)  (x.choice1.Win3).ne_psegrefbytes
#define NE_SWAPAREA(x)      (x.choice1.Win3).ne_swaparea
#define NE_EXPVER(x)        (x.choice1.Win3).ne_expver
#define NE_MODVER(x)        (x.choice1.Mod1).ne_modver

#define NE_USAGE(x)         ((WORD)*((WORD *)(x)+1))
#define NE_PNEXTEXE(x)      ((WORD)(x).ne_cbenttab)
#define NE_ONEWEXE(x)       ((WORD)(x).ne_crc)
#define NE_PFILEINFO(x)     ((WORD)((DWORD)(x).ne_crc >> 16))


/*
 *  Target operating systems
 */

#define NE_UNKNOWN      0x0             /* Unknown (any "new-format" OS) */
#define NE_OS2          0x1             /* OS/2 (default)  */
#define NE_WINDOWS      0x2             /* Windows */
#define NE_DOS4         0x3             /* DOS 4.x */
#define NE_DEV386       0x4             /* Windows 386 */


/*
 *  Format of NE_FLAGS(x):
 *
 *  p                                   Library Module
 *   m                                  Not MP-safe
 *    e                                 Errors in image
 *     x                                Unused
 *      b                               Bound Family/API
 *       ttt                            Application type
 *          f                           Floating-point instructions
 *           3                          386 instructions
 *            2                         286 instructions
 *             0                        8086 instructions
 *              P                       Protected mode only
 *               p                      Per-process library initialization
 *                i                     Instance data
 *                 s                    Solo data
 */
#define NENOTP          0x8000          /* Library Module - Not an EXE */
#define NENOTMPSAFE     0x4000          /* Process is not multi-processor safe */
#define NEIERR          0x2000          /* Errors in image */
#define NEBOUND         0x0800          /* Bound Family/API */
#define NEAPPTYP        0x0700          /* Application type mask */
#define NENOTWINCOMPAT  0x0100          /* Not compatible with P.M. Windowing */
#define NEWINCOMPAT     0x0200          /* Compatible with P.M. Windowing */
#define NEWINAPI        0x0300          /* Uses P.M. Windowing API */
#define NEFLTP          0x0080          /* Floating-point instructions */
#define NEI386          0x0040          /* 386 instructions */
#define NEI286          0x0020          /* 286 instructions */
#define NEI086          0x0010          /* 8086 instructions */
#define NEPROT          0x0008          /* Runs in protected mode only */
#define NEPPLI          0x0004          /* Per-Process Library Initialization */
#define NEINST          0x0002          /* Instance data */
#define NESOLO          0x0001          /* Solo data */

/*
 *  Format of NE_FLAGSOTHERS(x):
 *
 *      7 6 5 4 3 2 1 0  - bit no
 *      |         | | |
 *      |         | | +---------------- Support for long file names
 *      |         | +------------------ Windows 2.x app runs in prot mode
 *      |         +-------------------- Windows 2.x app gets prop. font
 *      +------------------------------ WLO appl on OS/2 (markwlo.exe)
 *
 */

#define NELONGNAMES     0x01
#define NEWINISPROT     0x02
#define NEWINGETPROPFON 0x04
#define NEWLOAPPL       0x80



struct new_seg                          /* New .EXE segment table entry */
  {
    unsigned short      ns_sector;      /* File sector of start of segment */
    unsigned short      ns_cbseg;       /* Number of bytes in file */
    unsigned short      ns_flags;       /* Attribute flags */
    unsigned short      ns_minalloc;    /* Minimum allocation in bytes */
  };

struct new_seg1                         /* New .EXE segment table entry */
  {
    unsigned short      ns1_sector;     /* File sector of start of segment */
    unsigned short      ns1_cbseg;      /* Number of bytes in file */
    unsigned short      ns1_flags;      /* Attribute flags */
    unsigned short      ns1_minalloc;   /* Minimum allocation in bytes */
    unsigned short      ns1_handle;
  };

#define NS_SECTOR(x)    (x).ns_sector
#define NS_CBSEG(x)     (x).ns_cbseg
#define NS_FLAGS(x)     (x).ns_flags
#define NS_MINALLOC(x)  (x).ns_minalloc


/*
 *  Format of NS_FLAGS(x)
 *
 *  Flag word has the following format:
 *
 *      15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0  - bit no
 *          |  |  |  |  | | | | | | | | | | |
 *          |  |  |  |  | | | | | | | | +-+-+--- Segment type DATA/CODE
 *          |  |  |  |  | | | | | | | +--------- Iterated segment
 *          |  |  |  |  | | | | | | +----------- Movable segment
 *          |  |  |  |  | | | | | +------------- Segment can be shared
 *          |  |  |  |  | | | | +--------------- Preload segment
 *          |  |  |  |  | | | +----------------- Execute/read-only for code/data segment
 *          |  |  |  |  | | +------------------- Segment has relocations
 *          |  |  |  |  | +--------------------- Code conforming/Data is expand down
 *          |  |  |  +--+----------------------- I/O privilege level
 *          |  |  +----------------------------- Discardable segment
 *          |  +-------------------------------- 32-bit code segment
 *          +----------------------------------- Huge segment/GDT allocation requested
 *
 */

#define NSTYPE          0x0007          /* Segment type mask */

#if (EXE386 == 0)
#define NSCODE          0x0000          /* Code segment */
#define NSDATA          0x0001          /* Data segment */
#define NSITER          0x0008          /* Iterated segment flag */
#define NSMOVE          0x0010          /* Movable segment flag */
#define NSSHARED        0x0020          /* Shared segment flag */
#define NSPRELOAD       0x0040          /* Preload segment flag */
#define NSEXRD          0x0080          /* Execute-only (code segment), or
                                        *  read-only (data segment)
                                        */
#define NSRELOC         0x0100          /* Segment has relocations */
#define NSCONFORM       0x0200          /* Conforming segment */
#define NSEXPDOWN       0x0200          /* Data segment is expand down */
#define NSDPL           0x0C00          /* I/O privilege level (286 DPL bits) */
#define SHIFTDPL        10              /* Left shift count for SEGDPL field */
#define NSDISCARD       0x1000          /* Segment is discardable */
#define NS32BIT         0x2000          /* 32-bit code segment */
#define NSHUGE          0x4000          /* Huge memory segment, length of
                                         * segment and minimum allocation
                                         * size are in segment sector units
                                         */
#define NSGDT           0x8000          /* GDT allocation requested */

#define NSPURE          NSSHARED        /* For compatibility */

#define NSALIGN 9       /* Segment data aligned on 512 byte boundaries */

#define NSLOADED    0x0004      /* ns_sector field contains memory addr */
#endif


struct new_segdata                      /* Segment data */
  {
    union
      {
        struct
          {
            unsigned short      ns_niter;       /* number of iterations */
            unsigned short      ns_nbytes;      /* number of bytes */
            char                ns_iterdata;    /* iterated data bytes */
          } ns_iter;
        struct
          {
            char                ns_data;        /* data bytes */
          } ns_noniter;
      } ns_union;
  };

struct new_rlcinfo                      /* Relocation info */
  {
    unsigned short      nr_nreloc;      /* number of relocation items that */
  };                                    /* follow */

/* NOINC */
#if __IBMC__ || __IBMCPP__
#pragma pack(1)
#endif
/* INC */


struct new_rlc                          /* Relocation item */
  {
    char                nr_stype;       /* Source type */
    char                nr_flags;       /* Flag byte */
    unsigned short      nr_soff;        /* Source offset */
    union
      {
        struct
          {
            char        nr_segno;       /* Target segment number */
            char        nr_res;         /* Reserved */
            unsigned short nr_entry;    /* Target Entry Table offset */
          }             nr_intref;      /* Internal reference */
        struct
          {
            unsigned short nr_mod;      /* Index into Module Reference Table */
            unsigned short nr_proc;     /* Procedure ordinal or name offset */
          }             nr_import;      /* Import */
        struct
          {
            unsigned short nr_ostype;   /* OSFIXUP type */
            unsigned short nr_osres;    /* reserved */
          }             nr_osfix;       /* Operating system fixup */
      }                 nr_union;       /* Union */
  };

/* NOINC */
#if __IBMC__ || __IBMCPP__
#pragma pack()
#endif
/* INC */


#define NR_STYPE(x)     (x).nr_stype
#define NR_FLAGS(x)     (x).nr_flags
#define NR_SOFF(x)      (x).nr_soff
#define NR_SEGNO(x)     (x).nr_union.nr_intref.nr_segno
#define NR_RES(x)       (x).nr_union.nr_intref.nr_res
#define NR_ENTRY(x)     (x).nr_union.nr_intref.nr_entry
#define NR_MOD(x)       (x).nr_union.nr_import.nr_mod
#define NR_PROC(x)      (x).nr_union.nr_import.nr_proc
#define NR_OSTYPE(x)    (x).nr_union.nr_osfix.nr_ostype
#define NR_OSRES(x)     (x).nr_union.nr_osfix.nr_osres



/*
 *  Format of NR_STYPE(x) and R32_STYPE(x):
 *
 *       7 6 5 4 3 2 1 0  - bit no
 *               | | | |
 *               +-+-+-+--- source type
 *
 */

#define NRSTYP          0x0f            /* Source type mask */
#define NRSBYT          0x00            /* lo byte (8-bits)*/
#define NRSSEG          0x02            /* 16-bit segment (16-bits) */
#define NRSPTR          0x03            /* 16:16 pointer (32-bits) */
#define NRSOFF          0x05            /* 16-bit offset (16-bits) */
#define NRPTR48         0x06            /* 16:32 pointer (48-bits) */
#define NROFF32         0x07            /* 32-bit offset (32-bits) */
#define NRSOFF32        0x08            /* 32-bit self-relative offset (32-bits) */


/*
 *  Format of NR_FLAGS(x) and R32_FLAGS(x):
 *
 *       7 6 5 4 3 2 1 0  - bit no
 *                 | | |
 *                 | +-+--- Reference type
 *                 +------- Additive fixup
 */

#define NRADD           0x04            /* Additive fixup */
#define NRRTYP          0x03            /* Reference type mask */
#define NRRINT          0x00            /* Internal reference */
#define NRRORD          0x01            /* Import by ordinal */
#define NRRNAM          0x02            /* Import by name */
#define NRROSF          0x03            /* Operating system fixup */
#define NRICHAIN        0x08            /* Internal Chaining Fixup */

#if (EXE386 == 0)

/* Resource type or name string */
struct rsrc_string
    {
    char rs_len;            /* number of bytes in string */
    char rs_string[ 1 ];    /* text of string */
    };

#define RS_LEN( x )    (x).rs_len
#define RS_STRING( x ) (x).rs_string

/* Resource type information block */
struct rsrc_typeinfo
    {
    unsigned short rt_id;
    unsigned short rt_nres;
    long rt_proc;
    };

#define RT_ID( x )   (x).rt_id
#define RT_NRES( x ) (x).rt_nres
#define RT_PROC( x ) (x).rt_proc

/* Resource name information block */
struct rsrc_nameinfo
    {
    /* The following two fields must be shifted left by the value of  */
    /* the rs_align field to compute their actual value.  This allows */
    /* resources to be larger than 64k, but they do not need to be    */
    /* aligned on 512 byte boundaries, the way segments are           */
    unsigned short rn_offset;   /* file offset to resource data */
    unsigned short rn_length;   /* length of resource data */
    unsigned short rn_flags;    /* resource flags */
    unsigned short rn_id;       /* resource name id */
    unsigned short rn_handle;   /* If loaded, then global handle */
    unsigned short rn_usage;    /* Initially zero.  Number of times */
                                /* the handle for this resource has */
                                /* been given out */
    };

#define RN_OFFSET( x ) (x).rn_offset
#define RN_LENGTH( x ) (x).rn_length
#define RN_FLAGS( x )  (x).rn_flags
#define RN_ID( x )     (x).rn_id
#define RN_HANDLE( x ) (x).rn_handle
#define RN_USAGE( x )  (x).rn_usage

#define RSORDID     0x8000      /* if high bit of ID set then integer id */
                                /* otherwise ID is offset of string from
                                   the beginning of the resource table */

                                /* Ideally these are the same as the */
                                /* corresponding segment flags */
#define RNMOVE      0x0010      /* Moveable resource */
#define RNPURE      0x0020      /* Pure (read-only) resource */
#define RNPRELOAD   0x0040      /* Preloaded resource */
#define RNDISCARD   0xF000      /* Discard priority level for resource */

/* Resource table */
struct new_rsrc
    {
    unsigned short rs_align;    /* alignment shift count for resources */
    struct rsrc_typeinfo rs_typeinfo;
    };

#define RS_ALIGN( x ) (x).rs_align


#endif /* NOT EXE386 */

#endif /* __NEWEXE__ */


/* NOINC */
#ifdef __cplusplus
        }
#endif
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif

#if __IBMC__ || __IBMCPP__
#pragma pack()
#endif
/* INC */
