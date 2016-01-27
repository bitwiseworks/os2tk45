/***************************************************************************
*
* Module Name: vpmx2.h
*
*
* Copyright (c) 1993  IBM Corporation
*
*
*      DPMI host per task data
*
*      Description:
*
*      Contains the vpm definitions
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
/* INC */

#ifndef VPMX2DEFINED

#define VPMX2DEFINED

#define VPMDBG_X  0x00010000    /* Execute   */
#define VPMDBG_W  0x00020000    /* Write     */
#define VPMDBG_R  0x00030000    /* ReadWrite */

/* Definitions used to determine if DPX initialization was successful */
#define     VPMINITOK      0x01              // VDPX and DPMI initialized OK
#define     DPXLEAVEV86    0x02              // DPX will switch VDM into VPM

/***ET+ PFNVPMDBGWPALLOCVDM - allocate a watchpoint for a VDM
 *
 *      This type is a pointer to a function that allocates
 *      a local watchpointfor the current VDM process.
 *
 *      (*PFNVPMDBGALLOCVDM) (laddr, cb, type, piwp)
 *
 *      ENTRY   laddr           - linear address of watchpoint
 *              cb              - size of watchpoint (1, 2, or 4 bytes)
 *              type            - type of watchpoint
 *              piwp            - place to return watchpoint index
 *      RETURN  int             - return code (NO_ERROR if successful)
 */
typedef INT (*PFNVPMDBGWPALLOCVDM)(ULONG,ULONG,ULONG,PULONG);
/*end*/

/***ET+ PFNVPMDBGWPFREEVDM - free a watchpoint for a VDM
 *
 *      This type is a pointer to a function that frees
 *      a local watchpointfor the current VDM process.
 *
 *      (*PFNVPMDBGWPFREEVDM) (iwp)
 *
 *      ENTRY   iwp             - watchpoint index
 *      RETURN  int             - return code (NO_ERROR if successful)
 */
typedef INT (*PFNVPMDBGWPFREEVDM)(ULONG);
/*end*/

/***ET+ PFNVPMDBGDR6QUERYVDM - query/update DR6 for a VDM process
 *
 *      This type is a pointer to a function that is used to query
 *      bits B0-B3 in DR6 for the current VDM process.  It is also
 *      used to reset any of those bits.
 *
 *      (*PFNVPMDBGDR6QUERYVDM) (flquery, flreset, pfl)
 *
 *      ENTRY   flquery         - bit mask of bits to query
 *              flreset         - bit mask of bits to reset
 *              pfl             - place to return bits
 *      RETURN  int             - return code (NO_ERROR if successful)
 */
typedef INT (*PFNVPMDBGDR6QUERYVDM)(ULONG,ULONG,PULONG);
/*end*/

/***ET+ PFNVPMSELCREATEVDMLDT - allocate an LDT for a VDM
 *
 *      This type is a pointer to a function that allocates
 *      an LDT object for a DPMI VDM process.
 *
 *      This routine must be called in the context of the VDM we are
 *      allocating the LDT for.
 *
 *      (*PFNVPMSELCREATEVDMLDT) (pladdr)
 *
 *      ENTRY:  pladdr - pointer to location to return
 *                       pointer to the new ldt
 *      EXIT:   NO_ERROR if success, else error code
 *
 */
typedef INT (*PFNVPMSELCREATEVDMLDT)(PVOID*);
/*end*/

/***ET+ PFNVPMSELDESTROYVDMLDT - free VDM protected mode LDT
 *
 *      This type is a pointer to a function that frees the
 *      VDM's protected mode LDT.
 *
 *      (*PFNVPMSELDESTROYVDMLDT) ()
 *
 *      ENTRY   NONE
 *
 *      EXIT    NONE
 *
 *      CONTEXT VDM Task
 *
 */
typedef VOID (*PFNVPMSELDESTROYVDMLDT)(VOID);
/*end*/

/***ET+ PFNVPMSELALLOCVDM - allocate consecutive LDT selectors
 *
 *      This type is a pointer to a function that returns "csel"
 *      consecutive free selectors from the current process's LDT.
 *
 *      (*PFNVPMSELALLOCVDM) (csel,psel)
 *
 *      ENTRY:  csel - number of contiguous selectors to allocate.
 *              psel - on entry: if *psel == -1, allocate any selector,
 *                            otherwise allocate that specific selector
 *      EXIT:   NO_ERROR if success, else error code
 */
typedef INT (*PFNVPMSELALLOCVDM)(USHORT,PUSHORT,PSEL);
/*end*/

/***ET+ PFNVPMSELFREEVDM - free a DPMI LDT selector
 *
 *      This type is a pointer to a function that marks a single LDT
 *      selector in the current process as free.
 *
 *      (*PFNVPMSELFREEVDM) (sel)
 *
 *      ENTRY:  sel - selector to free
 *      EXIT:   NO_ERROR if success, else error
 */
typedef INT (*PFNVPMSELFREEVDM)(SEL,USHORT);
/*end*/


/***ET+ PAGERANGE - page range structure used by *PFNVPMSELGETVDMINFO */
#ifndef SMP
typedef struct pagerange_s {
    ULONG pr_lastframe; 	      /* last valid page in range */
    ULONG pr_firstframe;	      /* first valid page in range */
} PAGERANGE;
#endif
/*end*/

#ifndef SMP
typedef PAGERANGE *PPAGERANGE;
#else
typedef struct pagerange_s *PPAGERANGE;
#endif


/***ET+ VPMVMINF - data returned by *PFNVPMSELGETVDMINFO */

typedef struct vpmvminfo_s {
        ULONG  vpmsel_linmax;       /* top linear address in user space   */
        ULONG  vpmsel_cphysranges;  /* number of entries in array below   */
	PPAGERANGE  vpmsel_physranges;   /* pointer to array of physical frame */
} VPMINF;

typedef VPMINF *PVPMINF;
/*end*/

/***ET+ PFNVPMSELGETVDMINFO - return memory manager info for VDM use
 *
 *      This type is a pointer to a function that fills in the
 *      vpmvminfo_s structure.
 *
 *      (*PFNVPMSELGETVDMINFO) (psvi)
 *
 *      ENTRY:  psvi - pointer to vpmvminfo_s struction to fill in
 *      EXIT:   none
 */
typedef INT (*PFNVPMSELGETVDMINFO)(PVPMINF);
/*end*/

/***ET+ PFNVPMPGMOVE - move page table entries.
 *
 *      This type is a pointer to a function that moves page table
 *      entries in the virtual address space. The destination must
 *      invalid pages.
 *
 *      Locked pages cannot be moved and will cause an internal error.
 *
 *      (*PFNVPMPGMOVE) (pagesrc, dummy1, pagedst, dummy2, npages, hob)
 *
 *      ENTRY   pagesrc - source virtual page
 *              reserved - must be NULL
 *              pagedst - destination virtual page
 *              reserved - must be NULL
 *              npages - number of pages
 *              hob - object handle for destination
 *      EXIT    0 on success, else error code
 */
typedef INT (*PFNVPMPGMOVE)(ULONG,PVOID,ULONG,PVOID,INT,USHORT);
/*end*/

/***ET+ PFNVPMPGGETACCESS - query access permissions for range of pages
 *
 *      This type is a pointer to a function that returns the permission
 *      bits for a range of pages in a byte array. The high bit of the
 *      byte is set if the page is unallocated. The caller must verify that
 *      the buffer is of the correct size and accessible.
 *
 *      (*PFNVPMPGGETACCESS) (page, reserved, npages, flags, pbuf)
 *
 *      ENTRY:  page - target virtual page number
 *              reserved - must be NULL
 *              npages - number of pages to query
 *              flags - VPMPG_SKIPFREE
 *                      - do not stop when invalid page is found
 *              pbuf - pointer to return array of bytes
 *      EXIT:   0 if success, else error code
 */
typedef INT (*PFNVPMPGGETACCESS)(ULONG,PVOID,INT,INT,PCHAR);
/*end*/

/***ET+ PFNVPMPGALLOCSIZEQUERY - return number of allocatable pages.
 *
 *      This type is a pointer to a function that returns the number
 *      of pages the page manager believes could be allocated as swappable.
 *      The returned value approximates a snapshot of reality and should
 *      only be used as a guide; its accuracy is not to be relied upon.
 *
 *      This routine is swappable.
 *
 *      (*PFNVPMPGALLOCSIZEQUERY) (pnpages)
 *
 *      ENTRY   pnpages - pointer to unitialized integer
 *      EXIT    0 on success
 *                  *pnpages = number of pages believed to be allocatable
 *              else error code
 *
 */
typedef INT (*PFNVPMPGALLOCSIZEQUERY)(PINT);
/*end*/

/* Structures and defines for *PFNVPMVMALLOCMEM */

/***ET+ VPMVMAC - VPMVMAllocMem packet structure */

typedef struct vpmvmac_s {
        ULONG  vpmac_va;            /* Object's virtual address */
        USHORT vpmac_res;           /* Reserved */
        USHORT vpmac_hob;           /* Object's handle */
} VPMVMAC;

typedef VPMVMAC *PVPMVMAC;
/*end*/

/* VPMPG_W, VPMPG_U,VPMPG_X, VPMPG_R  - see v8086.h */
#define VPMPG_SWAPPABLE    0x00000000      /* Swappable */
#define VPMVMAC_OBSHIFT    23              /* Object record flag shift */
#define VPMOB_SHRINKABLE   0x0001          /* Object is Shrinkable */
#define VPMVMAC_OBSHRINK   ((LONG) VPMOB_SHRINKABLE << VPMVMAC_OBSHIFT)
#define VPMVMAC_ALIGNPAGE  0x18000000L     /* Page alignment */
#define VPMVMAC_ARENAPRV   0x02000000L     /* Private arena */
#define VPMVMAC_LOCABOVE   0x40000000L     /* Allocate above address */
#define VPMVMAC_LOCSPECIFIC 0x80000000L    /* Allocate at specific address */
#define VPMVMAC_VDM        (0x00040000L)   /* VDM allocation */
#define VPMVMFM_VDM        (0x00000004L)   /* Request on behalf of VDM */

//c-begin
/***ET+ PFNVPMVMALLOCMEM - Reserve/commit virtual memory
 *
 *      This type is a pointer to a function that is used
 *      to allocate virtual address space and optionally commit
 *      physical storage to a contiguous region of the address
 *      space beginning at the base of the object.
 *
 *      WARNING: This function is in swappable code.
 *
 *      (*PFNVPMVMALLOCMEM) (cb, cbCommit, fl, dummy1, dummy2, dummy3,
 *                                              dummy4, dummy5, pac)
 *
 *      ENTRY  cb          - Size of virtual memory object in
 *                           bytes.  This may not be changed
 *                           subsequently.  Note that the size
 *                           will be rounded up to page
 *                           granularity, by adding the offset of
 *                           the specific base address (if any)
 *                           and rounding the result up to a
 *                           multiple of the page size (4K).
 *             cbCommit    - Amount of physical storage to be
 *                           committed to contiguous virtual
 *                           space  starting from the base of the
 *                           object.
 *             fl          - Allocation flags (see notes below)
 *             reserved    - Must be NULL.
 *             reserved    - Must be NULL.
 *             reserved    - Must be NULL.
 *             reserved    - Must be NULL.
 *             reserved    - Must be NULL.
 *             pac         - Address of output buffer.
 *                           The buffer has the following format:
 *
 *                         struct vpmvmac_s
 *                           {
 *                             ulong_t ac_va;
 *                             - Base virtual address
 *                               A specific base virtual address
 *                               or minimum base virtual address
 *                               may be requested by setting its
 *                               value here and specifying
 *                               VPMVMAC_LOCSPECIFIC/VPMVMAC_LOCANY in
 *                               the allocation flags.  The base
 *                               address is rounded down to the
 *                               nearest page boundary.  In all
 *                               cases, the actual base address
 *                               allocated to the object is
 *                               returned.
 *                             VMHOB ac_hob;
 *                             - Object's handle
 *                               The handle allocated to the object
 *                               is returned.
 *                             SEL dummy;
 *                           }
 *
 *      EXIT            int       - Result code (NO_ERROR if successful)
 *
 *      NOTES on flags usage and calling VMAllocMem
 *
 *      (1) Give information about the base virtual address
 *          of the object
 *
 *          VPMVMAC_LOCABOVE  - Base address must be above that given
 *                              Not valid for heap allocs.
 *
 *      (2) Indicate the alignment of the base address
 *          (4K page alignment is enforced; input addresses,
 *          byte counts will be rounded automatically so that
 *          the actual region allocated contains the whole of
 *          its first and last page)
 *
 *          VPMVMAC_ALIGNPAGE - Page granularity
 *
 *      (3) Specify properties of object.
 *
 *          VPMVMAC_OBSHRINK  - Object is shrinkable
 *          (physical commitment only, not virtual address space)
 *
 *      (4) Specify Page Manager information
 *
 *          Select one of each of the VPMPG_ state flags
 *          and zero or more VPMPG_ permission and modifier flags.
 *
 */
typedef INT (*PFNVPMVMALLOCMEM)(ULONG,ULONG,ULONG,USHORT,USHORT,USHORT, \
                ULONG,ULONG,PVPMVMAC);
/*end*/
//c-end
//masm-begin
/* DefType PFNVPMVMALLOCMEM,dd */
//masm-end

#define VPMVMS_COMMIT      (0x00100000L)    /* Commit physical storage */
#define VPMVMS_DECOMMIT    (0x00080000L)    /* Reserved for VMS_DECOMMIT */
#define VPMVMS_DFLTPERM    VPMVMAC_LOCABOVE /* Use (initial) permissions */
#define VPMPG_SKIPFREE     0x00008000       /* ignore free pages */

/***ET+ PFNVPMVMSETMEM - Commit/Decommit/Set Protection for Region
 *
 *      This type is a pointer to a function that performs one of three
 *      functions:
 *
 *      Commit memory resources for region
 *      Decommit memory resources for region
 *      Set protection for memory region
 *
 *      Attempts to commit memory that is already committed
 *      will return error.
 *
 *      WARNING: This procedure is in swappable code
 *
 *      (*PFNVPMVMSETMEM) (p, cb, reserved, fl, blkno)
 *
 *      ENTRY   p           - region's virtual address
 *              cb          - region's size in bytes
 *              reserved            - must be NULL
 *              fl          - flags
 *                            VPMVMS_COMMIT
 *                              Commit memory resources
 *                              NOTE:  The Page Manager storage
 *                                     type and state must also be
 *                                     specified using the appropriate
 *                                     VPMPG_x and PAS_y flags, see
 *                                     pgexport.h.
 *                            VPMVMS_DECOMMIT
 *                              Decommit memory resources
 *                            VPMVMS_DFLTPERM
 *                              Set access permissions to the default
 *                              value (initial).
 *                            VPMPG_SKIPFREE
 *                              Ignore non-committed pages when setting
 *                              access permissions or decommitting.  If
 *                              not specified, attempting to decommit or
 *                              set access permissions on uncommitted memory
 *                              will return error.
 *              dumm1       - reserved and must be 0.
 *
 *      RETURN  int         - return code (NO_ERROR if successful)
 */
typedef INT (*PFNVPMVMSETMEM)(ULONG,ULONG,USHORT,ULONG,ULONG);
/*end*/

/***ET+ PFNVPMVMFREEMEM - Free reference to an object given base address.
 *
 *      This type is a pointer to a function that frees a reference to an
 *      object from a given context given the base virtual address of the
 *      object.
 *
 *      If there are memory resources committed to the region, they are freed
 *      To decommit memory resources without dereserving address space a task
 *      must use the VPMVMSetMem function.
 *
 *      WARNING: This function is in swappable code
 *
 *      (*PFNVPMVMFREEMEM) (p, reserved, fl)
 *
 *      ENTRY   p           - region's virtual address
 *              reserved    - must be NULL
 *              fl          - Flags
 *                            VPMVMFM_VDM - call on behalf of VDM
 *      RETURN  int         - Result code
 *                              NO_ERROR = success
 *                              non-zero = failure
 *                              NOTE: ERROR_INTERRUPT will not
 *                              be returned.  All free operations
 *                              ignore interrupts.
 *
 */
typedef INT (*PFNVPMVMFREEMEM)(ULONG,USHORT,ULONG);
/*end*/


/***ET+ PFNVPMDPMITASKSWITCH - report task switch
 *
 *      This type is a pointer to a function that reports a DPMI
 *      task switch.
 *
 *      (*PFNVPMDPMITASKSWITCH) (pdhd)
 *
 *      ENTRY:  pdhd - pointer to new task
 *      EXIT:   NONE
 *
 *
 */
typedef VOID (*PFNVPMDPMITASKSWITCH)(PVOID);
/*end*/

 /* Definitions for the possible values returned in vpmx_CpuType */
#define VPMCPU_286            0               /* processor is a 286 */
#define VPMCPU_386            1               /* processor is a 386 */
#define VPMCPU_486            2               /* processor is a 486 */

/***ET+ PFNVPMPGSWAP - note that page can be swapped out
 *
 *      This type is a pointer to a function that receives
 *      notification that a page can be swapped.  The
 *      kernel can choose to ignore the call or it can
 *      arrange to swap out the page.
 *
 *      (*PFNVPMPGSWAP)(ipg, cpg)
 *
 *      ENTRY:  ipg      - target virtual page number
 *              npages   - number of pages to query
 *
 *      EXIT:   NONE
 */
typedef VOID (*PFNVPMPGSWAP)(ULONG,ULONG);
/*end*/

/***ET+ PFNVPMPGDISCARD - note that page can be discarded
 *
 *      This type is a pointer to a function that receives
 *      notification that a page can be discarded.  The
 *      kernel can choose to ignore the call or it can
 *      free the page and mark it as allocated on access.
 *
 *      (*PFNVPMPGDISCARD)(ipg, cpg)
 *
 *      ENTRY:  ipg      - target virtual page number
 *              npages   - number of pages to query
 *
 *      EXIT:   NONE
 */
typedef VOID (*PFNVPMPGDISCARD)(ULONG,ULONG);
/*end*/

/***ET+ PFNVPMPGMAPPHYS - map physical page
 *
 *      This type is a pointer to a function that receives
 *      requests to map physical pages in the VDM for
 *      use by the client.  The kernel can decide whether
 *      honoring a request is safe and refuse to honor
 *      those that are not.
 *
 *	(*PFNVPMPGMAPPHYS)(pvpmac, ulPhys, cb)
 *	ENTRY:	pvpmac	  - VPMVMAllocMem pack structure
 *			    pvpmac->vpmac_va == -1 means any arbitrary address
 *			    (must be on page boundary)
 *
 *      (*PFNVPMPGMAPPHYS)(pladdr, ulPhys, cb)
 *
 *      ENTRY:  pladdr    - pointer to target linear address
 *                          *pladdr == -1 means any arbitrary address
 *                          (must be on page boundary)
 *              ulPhys    - physical address to map
 *              cb        - count of bytes
 *
 *      EXIT:   TRUE     - indicates success
 *              FALSE    - indicates request was refused
 */
typedef BOOL (*PFNVPMPGMAPPHYS)(PVPMVMAC,ULONG,ULONG);
/*end*/

/***ET+ PFNVPMNPXSTATUS - get NPX status
 *
 *      This type is a pointer to a function that returns
 *      the information passed back for DPMI 1.0 function
 *      0E01H.
 *
 *      (*PFNVPMNPXSTATUS)(pfl)
 *
 *      ENTRY:  pfl       - pointer to flags variable
 *
 *      EXIT:   TRUE     - indicates success
 *                  *pfl filled as follows (see DPMI 1.0 spec)
 *                      bit 0 set == coprocessor enabled for client
 *                      bit 1 set == client is emulating
 *                      bit 2 set == numeric coprocessor is present
 *                      bit 3 set == host is emulating coprocessor
 *                      bits 4-7  == 00h nocoprocessor
 *                                   02h 80287
 *                                   03h 80387
 *                                   04h 80486 with coprocessor
 *                      bits 8-31 == 0
 *              FALSE    - indicates request was refused
 */
typedef BOOL (*PFNVPMNPXSTATUS)(PULONG);
/*end*/

/***ET+ PFNVPMNPXSET - set NPX status
 *
 *      This type is a pointer to a function that turns
 *      emulation on or off for a protect mode client.
 *
 *      (*PFNVPMNPXSET)(fl)
 *
 *      ENTRY:  fl       - flags variable
 *                         bit 0 reset == disable coprocessor (ignore)
 *                         bit 0 set   == enable coprocessor  (ignore)
 *                         bit 1 reset == turn EM bit off
 *                         bit 1 set   == turn EM bit on
 *
 *      EXIT:   TRUE     - indicates success
 *              FALSE    - indicates request was refused
 */
typedef BOOL (*PFNVPMNPXSET)(ULONG);
/*end*/

/***ET+  PFNVPMSELALLOCVDMSELS - allocate consecutive LDT sels for a DPMI VDM
 *
 *	This type is a pointer to a function that returns "csel" consecutive
 *	free selectors from the current process's LDT. If csel consecutive
 *	free selectors are not available and cselmin is not 0
 *
 *	(*PFNVPMSELALLOCVDMSELS) (csel,cselmin,psel)
 *
 *	The format of the free list is described in SELCreateVdmLDT().
 *
 *	ENTRY:	csel - number of contiguous selectors to allocate.
 *		cselmin -
 *		psel - on entry: if *psel == -1, allocate any selector,
 *			      otherwise allocate that specific selector
 *	EXIT:	NO_ERROR if success, else error code
 *
 */
typedef INT (*PFNVPMSELALLOCVDMSELS)(ULONG, ULONG, PSEL);
/*end*/

/***ET+  PFNSELFREEVDMSELS - free a group of DPMI LDT selectors
 *
 *	This type is a pointer to a function that frees a group of selectors
 *	in the current process's LDT, and coallesces them with other adjacent
 *	free selectors.
 *
 *     (*PFNSELFREEVDMSELS)(sel, cnt)
 *
 *	ENTRY:	sel - selector to free
 *		cnt - number of contiguous selectors to free
 *
 *	EXIT:	NO_ERROR if success, else error
 */
typedef INT (*PFNVPMSELFREEVDMSELS)(SEL, USHORT);
/*end*/


/***ET+ VPMEXP - kernel exports to DPMI VDD for VDHRegisterDPMI
 *
 *      These entry points are passed from the kernel during the
 *      exchange of entry points in VDHRegisterDPMI.
 */
typedef struct vpmx_s {
PFNVPMDBGWPALLOCVDM        vpmx_DBGWPAllocVDM;
PFNVPMDBGWPFREEVDM         vpmx_DBGWPFreeVDM;
PFNVPMDBGDR6QUERYVDM       vpmx_DBGDR6QueryVDM;
PFNVPMSELCREATEVDMLDT      vpmx_SELCreateVdmLDT;
PFNVPMSELDESTROYVDMLDT     vpmx_SELDestroyVdmLDT;
PFNVPMSELALLOCVDM          vpmx_SELAllocVdm;
PFNVPMSELFREEVDM           vpmx_SELFreeVdm;
PFNVPMSELGETVDMINFO        vpmx_SELGetVdmInfo;
PFNVPMPGMOVE               vpmx_PGMove;
PFNVPMPGGETACCESS          vpmx_PGGetAccess;
PFNVPMPGALLOCSIZEQUERY     vpmx_PGAllocSizeQuery;
PFNVPMVMALLOCMEM           vpmx_VMAllocMem;
PFNVPMVMSETMEM             vpmx_VMSetMem;
PFNVPMVMFREEMEM            vpmx_VMFreeMem;
PFNVPMDPMITASKSWITCH       vpmx_em86DPMITaskSwitch;
ULONG                      vpmx_SEL_VDM_LDT_INCR;
ULONG                      vpmx_CpuType;
ULONG                      vpmx_MaxAppLinAddr;
ULONG                      vpmx_V86StackSize;
PFNVPMPGSWAP               vpmx_pfnVPMPgSwap;
PFNVPMPGDISCARD            vpmx_pfnVPMPgDiscard;
PFNVPMPGMAPPHYS            vpmx_pfnVPMPgMapPhys;
PFNVPMNPXSTATUS            vpmx_pfnVPMNpxStatus;
PFNVPMNPXSET               vpmx_pfnVPMNpxSet;
} VPMX;      /* vpmx */

typedef VPMX *PVPMX;  /* pvpmx */
/*end*/

#endif


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
