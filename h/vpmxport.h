/***    VPMXPORT.H
 *
 *
 *
 *      Copyright (c) IBM Corporation 1993
 *
 *      All Rights Reserved
 *
 *      Title:
 *
 *      DPMI host per task data
 *
 *      Description:
 *
 *      Contains the vpm definitions
 *
 *      MODIFICATION HISTORY
 *      --------------------
 *      05/07/91 RCW  Changed all occurences of the single line comment
 *                    notation to the standard notation to allow users to
 *                    maintain ANSI compatibility.  This is in response to
 *                    PTR B718526.  All future updates to this and all other
 *                    header files should use the standard commenting format.
 *
 *      Gordon Kuo         02/24/96     149007
 *      Add LDHD and PLDHD to link the list of DHD to enhance Extended
 *      Exception Handle (DPMI 1.0 Subset)
 *      Add VPMXCPT definitions
 *
 *     03-27-96 Gordon Kuo -- Fix Defect 149895
 *        Add dhd_ppDosX address to switch VDPX puxtd when switch DPMI client
 *
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

#ifndef __VPMXPORT__
#define __VPMXPORT__


#include <vpmx2.h>

/***ET+ DHD - DPMI host data block
 *
 *      This is the DPMI host data for a task.  The V86 mode
 *      host data block allocated (in the V86 mode address space)
 *      before the start protected mode execution call is used only
 *      for the client V86 mode stack.  All other per dpmi task
 *      data is kept in this structure as per dpmi program instance
 *      data.  Note: there is one structure per dpmi start call in a VDM.
 */
typedef struct dhd_s {
    struct dhd_s *dhd_pdhdParent;   /* pointer to parent program's DHD    */
    USHORT dhd_segStack;            /* V86 stack segment (host data area) */
    USHORT dhd_segEnviron;          /* V86 environment segment            */
    USHORT dhd_segPDB;              /* V86 pdb segment                    */
    SEL    dhd_selSS;               /* protect mode stack selector        */
    SEL    dhd_selDS;               /* protect mode ds  selector          */
    SEL    dhd_selCS;               /* protect mode cs  selector          */
    SEL    dhd_selPDB;              /* protect mode pdb selector          */
    SEL    dhd_selEnviron;          /* protect mode env selector          */
    PVOID  dhd_pdpmData;            /* DPMI API layer per DPMI task data  */
    PVOID  dhd_pvpmData;            /* em86 per DPMI task data            */
    PVOID  dhd_pDosX;               /* DOS API extension area             */
    PVOID  dhd_laddrLDT;            /* LDT linear address                 */
    SEL    dhd_selvpmbp;            /* protect mode vpm breakpoint sel    */
    ULONG  DHD_sig;                 /* assertion checking                 */
    PVOID  dhd_ppDosX;              /* pointer of puxtd (DOS API extension area) */
                               /* 149895   */
} DHD;

#define DHD_SIGNATURE                   0x20444844      /* "DHD " */

typedef DHD *PDHD;  /* pdhd */


typedef struct l_dhd {         /* GK : List structure for DHD 149007 */
   struct l_dhd *pNextLDHD;
   PDHD   pDHD;
} LDHD;

typedef LDHD *PLDHD;           /* GK : List structure for DHD 149007 */
/*end*/

/*c-begin*/
/* common external references */
extern PDHD    pdhdHostData;            /* instance data in em86vpm.c */
extern ULONG   flVpmIRQHooks;           /* instance data in em86vpm.c */
/*c-end*/

/* Definitions for get/set dpmi exception handler calls */
/* 32 bit or 16 bit APP flag                   149007   */
#define VPMXCPT32                 0x0001
/* 1.0 extended handler. Need to provide Expanded Stack */
#define VPMXCPT10                 0x0002
#define VPMXCPT_REFLECT     0x0004
/* 0.9 handler. old stack frame                         */
/* #define VPMXCPT09              0x0008                   */
/* V86 Mode Exception Handler or Protect mode handler   */
#define VPMXCPTV86          0x0010
/* Already set 1.0 extended handler at least once       */
/*                       Need to provide Expanded Stack */
#define VPMXCPT10SET        0x8000

/***ET+ PFNDPMRTE - DPMI router hook
 *
 *  ENTRY
 *      pcrf   - pointer to client register frame
 *
 *  EXIT
 *      NONE
 *
 *  CALLING CONVENTIONS
 *      32-bit small-model PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
 *      DS and ES.  DS is assumed to contain the callee's DGROUP selector
 *      on entry.  EAX must be set to the return code on exit.
 */
typedef VOID (PASCAL *PFNDPMRTE)(PCRF);           /* pdpmr */
/*end*/

/***ET+ PFNDPMSTART - DPMI task entry hook
 *
 *  ENTRY
 *      pcrf   - pointer to client register frame
 *
 *  EXIT
 *      SUCCESS == TRUE
 *
 *  CALLING CONVENTIONS
 *      32-bit small-model PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
 *      DS and ES.  DS is assumed to contain the callee's DGROUP selector
 *      on entry.  EAX must be set to the return code on exit.
 */
typedef BOOL (PASCAL *PFNDPMSTART)(PCRF);          /* pdpms */
/*end*/

/***ET+ PFNDPMEND - DPMI task end hook
 *
 *  ENTRY
 *      NONE
 *
 *  EXIT
 *      SUCCESS == TRUE
 *
 *  CALLING CONVENTIONS
 *      32-bit small-model PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
 *      DS and ES.  DS is assumed to contain the callee's DGROUP selector
 *      on entry.  EAX must be set to the return code on exit.
 */
typedef BOOL (PASCAL *PFNDPMEND)(VOID);          /* pdpme */
/*end*/

/***ET+ PFNDPMQUERY - Query DPMI function
 *     Designed for furture use
 *  ENTRY
 * ulQueryItem - The item ID that EM86 queries at
 * pulResult   - the result return
 *
 *  EXIT
 *      TRUE  -- The query successes
 *      FALSE -- The query fail
 *
 *  CALLING CONVENTIONS
 *      32-bit small-model PASCAL calling conventions.  More specifically:
 *
 *      Parameters are pushed on the stack from left to right, and are
 *      removed by the callee.  The callee will preserve EBX, ESI, EDI, EBP,
 *      DS and ES.  DS is assumed to contain the callee's DGROUP selector
 *      on entry.  EAX must be set to the return code on exit.
 */
typedef ULONG (PASCAL *PFNDPMQUERY)(ULONG,PULONG);       /* dpmQuery */
/*end*/


/***ET+ DPMX - DPMI VDD exports to kernel for VDHRegisterDPMI
 *
 *      These entry points are passed to the kernel during the
 *      exchange of entry points in VDHRegisterDPMI.
 */
typedef struct dpmx_s {
    PFNDPMRTE   dpmx_pfnINT31Rte;
    PFNDPMSTART dpmx_pfnCreateTask;
    PFNDPMEND   dpmx_pfnEndTask;
    PFNDPMQUERY dpmx_pfnQuery;
} DPMX;        /* dpmx */

typedef DPMX *PDPMX;  /* pdpmx */

#define DPM_QUERY_WP_SET 0

/*end*/

/* VDH service for registering DPMI VDD */
/*c-begin*/
BOOL VDHENTRY VDHRegisterDPMI (ULONG, PDPMX, PVPMX);
/*c-end*/
/*masm-begin*/
/* VDHENTRY VDHRegisterDPMI <ULONG, ULONG, ULONG> */
/*masm-end*/

#endif /* __VPMXPORT__ */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
