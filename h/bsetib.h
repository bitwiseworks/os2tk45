/*      SCCSID = @(#)bsetib.h   6.9 92/03/15 */
/***************************************************************************\
*
* Module Name: BSETIB.H
*
* OS/2 Thread Information Block Include File
*
* Copyright (c) 1989, 1992  IBM Corporation
* Copyright (c) 1989  Microsoft Corporation
*
*****************************************************************************/

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

#ifndef __BSETIB__
/* NOINC */
#define __BSETIB__
/* INC */

/*
 *      Thread Information Block (TIB)
 */

struct tib2_s
       {
       ULONG   tib2_ultid;             /* Thread I.D. */
       ULONG   tib2_ulpri;             /* Thread priority */
       ULONG   tib2_version;           /* Version number for this structure */
       USHORT  tib2_usMCCount;         /* Must Complete count */
       USHORT  tib2_fMCForceFlag;      /* Must Complete force flag */
       };

typedef struct tib2_s TIB2;
typedef struct tib2_s *PTIB2;

struct tib_s                           /* TIB Thread Information Block */
       {
        PVOID   tib_pexchain;           /* Head of exception handler chain */
        PVOID   tib_pstack;             /* Pointer to base of stack */
        PVOID   tib_pstacklimit;        /* Pointer to end of stack */
        PTIB2   tib_ptib2;              /* Pointer to system specific TIB */
        ULONG   tib_version;            /* Version number for this TIB structure */
        ULONG   tib_ordinal;            /* Thread ordinal number        */
       };


typedef struct tib_s    tib_t;
typedef struct tib_s    TIB;
typedef struct tib_s    *PTIB;


/*
 *      Process Information Block (PIB)
 */


struct pib_s {                         /* PIB Process Information Block */
       ULONG   pib_ulpid;              /* Process I.D. */
       ULONG   pib_ulppid;             /* Parent process I.D. */
       ULONG   pib_hmte;               /* Program (.EXE) module handle */
       PCHAR   pib_pchcmd;             /* Command line pointer */
       PCHAR   pib_pchenv;             /* Environment pointer */
       ULONG   pib_flstatus;           /* Process' status bits */
       ULONG   pib_ultype;             /* Process' type code */
       };


typedef struct pib_s    pib_t;
typedef struct pib_s    PIB;
typedef struct pib_s    *PPIB;

#endif /* __BSETIB__ */

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
/* INC */
