/***************************************************************************
*
* Module Name: mvdm.h
*
* OS/2 public header file.
*
* Copyright (c) 1993  IBM Corporation
*
*      MVDM constants and types
*
*      Include file hierarchy:
*
*        mvdm.h
*          basemac.h
*          basedef.h
*          vdmprop.h
*          vdmm.h
*          v8086.h
*          vdos.h
*          vpic.h
*          vdmbios.h
*          vkbd.h
*          vlpt.h
*          vdma.h
*
*      By default, ALL portions of ALL include files are included (ie,
*      INCL_ALL is assumed).
*
*      To control what is included, use INCL_NONE and then select from
*      the following:
*
*          INCL_DEF
*          INCL_MI
*          INCL_TYPES
*          INCL_MVDMTYPES
*          INCL_SSTODS
*          INCL_ROMBIOS
*          INCL_PAGE
*          INCL_VDH
*          INCL_VDHVPIC
*          INCL_VDHALL
*          INCL_VDDHELPERS
*          INCL_VDHVKBD
*          INCL_VDHVLPT
*          INCL_VDHVDMA
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


#ifndef __MVDM__
#define __MVDM__

#ifndef INCL_NONE
#define INCL_DEF
#define INCL_TYPES
#define INCL_MVDMTYPES
#define INCL_SSTODS
#define INCL_ROMBIOS
#define INCL_PAGE
#define INCL_VDHALL
#define INCL_VDDHELPERS
#define INCL_VDHVKBD
#define INCL_VDHVLPT
#define INCL_VDHVDMA
#endif

#ifdef  INCL_VDHALL             // if all VDH services, set defines
#define INCL_VDH
#define INCL_VDHVPIC
#define INCL_VDHVKBD
#define INCL_VDHVLPT
#define INCL_VDHVDMA
#endif

#ifdef  INCL_VDH                // VDH services require MVDM types
#define INCL_MVDMTYPES
#endif

#ifdef  INCL_VDHVPIC            // VDH services require MVDM types
#define INCL_MVDMTYPES
#endif

#ifdef  INCL_VDHVKBD            // VDH services require MVDM types
#define INCL_MVDMTYPES
#endif

#ifdef  INCL_VDHVLPT            // VDH services require MVDM types
#define INCL_MVDMTYPES
#endif

#ifdef  INCL_VDHVDMA            // VDH services require MVDM types
#define INCL_MVDMTYPES
#endif

#ifdef  INCL_MVDMTYPES          // MVDM types depend on basic types
#define INCL_TYPES
#endif

#ifdef  INCL_TYPES              // types depend on basemaca.inc macros
#define INCL_DEF
#endif


/*** Required generic include files
 */


#include <basemac.h>            // macros for C


#include <basedef.h>            // basic OS/2 data types



/*** MVDM-specific equates (required for function prototypes)
 */
#define VDHENTRY PASCAL NEAR    // to declare exported VDH services
#define HOOKENTRY PASCAL NEAR   // to declare "hook" entry points

#define PDDENTRY FAR32 PASCAL   // to declare PDD and VDD entry points


#if (__IBMC__ >= 300)
#define VDDENTRY FAR32 PASCAL _loadds
#define VDDInit main		        // kludge to force compiler to ref. __acrtused
#else

#define VDDENTRY FAR32 PASCAL

#endif

/*** MVDM-specific macros
 */

//c-begin
#define BEGIN_GLOBAL_CODE       code_seg()
#define END_GLOBAL_CODE         code_seg()

#define BEGIN_GLOBAL_DATA       data_seg()
#define END_GLOBAL_DATA         data_seg()

#define BEGIN_INIT_CODE         code_seg("CINIT_TEXT", "CODE")
#define END_INIT_CODE           code_seg()

#define BEGIN_INIT_DATA         data_seg("CINIT_DATA", "CINITDATA")
#define END_INIT_DATA           data_seg()

#define BEGIN_INSTANCE_DATA     data_seg("MVDMINSTDATA", "MIDATA")
#define END_INSTANCE_DATA       data_seg()

#define BEGIN_SWAP_CODE         code_seg("CSWAP_TEXT","CODE")
#define END_SWAP_CODE           code_seg()

#define BEGIN_SWAP_DATA         data_seg("CSWAP_DATA","CSWAPDATA")
#define END_SWAP_DATA           data_seg()

#define BEGIN_SWAP_INSTANCE     data_seg("SWAPINSTDATA","SIDATA")
#define END_SWAP_INSTANCE       data_seg()
//c-end


#ifdef  INCL_VDDHELPERS

ULONG PRIVENTRY vdhSaveFlags(VOID);     // prototypes for helper externals
VOID  PRIVENTRY vdhRestoreFlags(ULONG);
VOID  PRIVENTRY vdhInt3(VOID);
ULONG PRIVENTRY vdhXCHG(PULONG, ULONG);
ULONG PRIVENTRY vdhBTC(PULONG, ULONG);
ULONG PRIVENTRY vdhBTR(PULONG, ULONG);
ULONG PRIVENTRY vdhBTS(PULONG, ULONG);
ULONG PRIVENTRY vdhBSF(ULONG);
ULONG PRIVENTRY vdhBSR(ULONG);
VOID  PRIVENTRY vdhOutSB(PORT, PBYTE, ULONG, BOOL);
PSZ   PRIVENTRY vdhFindToken(PSZ);
INT   PRIVENTRY vdhCompareStr(PSZ, PSZ, BOOL);
VOID  PRIVENTRY vdhMoveMem(PVOID, PVOID, INT, INT, UINT, BOOL);

//c-begin
#ifdef FUNCS
#pragma function(inp, inpw, outp, outpw)
#endif

#define STRTOKEN(p)       vdhFindToken(p)
#define STRCOMPARE(p,q,f) vdhCompareStr(p,q,f)

#define DISABLE()       _disable()      // _disable() intrinsic fixed now
#define ENABLE()        _enable()       // _enable() intrinsic fixed now
#define SAVEFLAGS()     vdhSaveFlags()  // preserve flags
#define RESTOREFLAGS(f) vdhRestoreFlags(f) // restore flags
#define INT3()          _interrupt(3)   // execute an INT 3

#define INB(p)          ((BYTE)inp(p))
#ifndef IO8BIT
#define INW(p)          inpw(p)
#else
#define INW(p)          ((WORD)(inp(p) + (inp((p)+1)<<8)))
#endif

#define OUTB(p,b)       outp(p,b)
#define OUTSB(p,pb,n,f) vdhOutSB(p,pb,n,f)
#ifndef IO8BIT
#define OUTW(p,w)       outpw(p,w)
#else
#define OUTW(p,w)       (outp(p,(BYTE)(w)), outp((p)+1,(BYTE)((w)>>8)))
#endif
//c-end

//DISABLE macro               ;alias for CLI (similar to C)
//        cli
//        endm
//
//ENABLE  macro               ;alias for STI (similar to C)
//        sti
//        endm
//
//PUSHFLAGS macro             ;alias for PUSHF
//        pushf
//        endm
//
//POPFLAGS macro              ;alias for POPF
//        popf
//        endm
//
//INT3    macro               ;alias for INT 3
//        int 3
//        endm
//
//INB     macro   ral,rdx     ;alias for IN AL,DX (also supports IN AH,DX)
//        ifidni  <ral>,<ah>
//          xchg    al,ah
//          in      al,rdx
//          xchg    al,ah
//        else
//          in      ral,rdx
//        endif
//        endm
//
//INW     macro   rax,rdx     ;alias for IN AX,DX (provides support for IO8BIT)
//        IFNDEF IO8BIT
//        in      rax,rdx
//        ELSE
//        in      al,rdx
//        xchg    al,ah
//        inc     rdx
//        jmp     $+2
//        in      al,rdx
//        xchg    al,ah
//        dec     rdx
//        ENDIF
//        endm
//
//OUTB    macro   rdx,ral     ;alias for OUT DX,AL (also supports OUT DX,AH)
//        ifidni  <ral>,<ah>
//          xchg    al,ah
//          out     rdx,al
//          xchg    al,ah
//        else
//          out     rdx,ral
//        endif
//        endm
//
//OUTW    macro   rdx,rax     ;alias for OUT DX,AX (provides support for IO8BIT)
//        IFNDEF IO8BIT
//        out     rdx,rax
//        ELSE
//        out     rdx,al
//        xchg    al,ah
//        inc     rdx
//        jmp     $+2
//        out     rdx,al
//        xchg    al,ah
//        dec     rdx
//        ENDIF
//        endm


#endif  // INCL_VDDHELPERS


/*** MVDM-specific types
 */

#ifdef  INCL_MVDMTYPES

typedef PVOID PVDM;             // linear pointer into VDM      /* pvdm */

typedef PBYTE PBVDM;            // same as PVDM, but often needs less casting

typedef BYTE VBREG;             // virtual byte register        /* vb */

#endif  // INCL_MVDMTYPES


/*** MVDM-specific include files
 */

#ifdef  INCL_VDH
#include <vdmprop.h>            // DOS property services
#include <vdmm.h>               // VDM Manager services
#include <v8086.h>              // 8086 emulation services
#include <vdos.h>               // DOS emulation services
#endif

#ifdef  INCL_VDHVPIC
#include <vpic.h>               // VPIC services
#endif

#ifdef  INCL_VDHVKBD
#include <vkbd.h>               // VKBD services
#endif

#ifdef  INCL_VDHVLPT
#include <vlpt.h>               // VLPT services
#endif

#ifdef  INCL_VDHVDMA
#include <vdma.h>               // VDMA services
#endif

#ifdef  INCL_ROMBIOS            // ROM BIOS Data Area definitions
#include <vdmbios.h>
#include <vdmbiosj.h>
#endif

#ifdef  INCL_MI                 // machine instructions
typedef UCHAR  uchar_t;
typedef USHORT ushort_t;
typedef ULONG  ulong_t;

#include <mi.h>
#endif

#endif  /* __MVDM__ */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
