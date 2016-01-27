/*************************START OF SPECIFICATIONS *************************/
/* SOURCE FILE NAME:  IODLL.C                                             */
/*                                                                        */
/* DESCRIPTIVE NAME: File Format IO Proc DLL Init/Term                    */
/*                                                                        */
/* COPYRIGHT:     IBM - International Business Machines                   */
/*            Copyright (c) IBM Corporation  1991, 1992, 1993             */
/*                        All Rights Reserved                             */
/*                                                                        */
/* STATUS: OS/2 Release 2.0                                               */
/*                                                                        */
/*                                                                        */
/************************* END OF SPECIFICATIONS **************************/

#include        <stdio.h>

#define         INCL_DOS                       /* #define  INCL_DOSPROCESS. */
#define         INCL_ERRORS
#define         INCL_WIN
#define         INCL_GPI
#include        <os2.h>                        /* OS/2 headers.             */
#include        <pmbitmap.h>

#define         INCL_OS2MM
#define         INCL_MMIO_CODEC
#define         INCL_MMIO_DOSIOPROC
#include        <os2me.h>                      /* Multi-Media IO extensions.*/
#include        <hhpheap.h>
#include        <ioi.h>


/********************************************************************/
/*  DLL Global Process Instance Data - one copy per each process    */
/********************************************************************/
HMTX      hmtxGlobalHeap = (HMTX)0;   // Global heap access
HHUGEHEAP hheap = 0L;
ULONG     ulBufferLength = DEFAULTBUFFERSIZE;
ULONG     ulNumColors   = 0;


/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: _DLL_InitTerm                                           */
/*                                                                          */
/* DESCRIPTIVE NAME: DLL initialization/Termination routine                 */
/*                                                                          */
/* FUNCTION: This function is executed by OS/2 at DLL load time and DLL     */
/*           unload time.                                                   */
/*                                                                          */
/* NOTES: None                                                              */
/*                                                                          */
/* ENTRY POINT: _DLL_InitTerm                                               */
/*   LINKAGE:   CALL FAR (00:32)                                            */
/*                                                                          */
/* INPUT:                                                                   */
/*              HMODULE hmod     - DLL module handle                        */
/*              ULONG   fTerm    - Init/Term flag                           */
/*                                                                          */
/* EXIT-NORMAL:                                                             */
/*              TRUE (1)                                                    */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*              FALSE (0)                                                   */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
int _CRT_init(void);
#pragma linkage (_DLL_InitTerm, system)
unsigned long _DLL_InitTerm ( ULONG hmod,
                              ULONG fTerm )
{
   LONG   rc= 1L;

   if(!fTerm) {
      if(_CRT_init()) {
         return(0L);
         }

      if (rc = DosCreateMutexSem(NULL, &hmtxGlobalHeap, DC_SEM_SHARED, FALSE)) {
         return(0L);
         }

      hheap = HhpCreateHeap((ULONG)HEAP_SIZE,(USHORT)0L);
      if (hheap == NULL) {
          return(0L);
          }
      rc = 1L;
      }


   if (fTerm) {    /* Initalize Terminate ? */
      HhpDestroyHeap(hheap);
      DosCloseMutexSem(hmtxGlobalHeap);
      }

   return(rc);
}
