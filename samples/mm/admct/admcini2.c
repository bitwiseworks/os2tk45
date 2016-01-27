/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: ADMCINIT.c
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* DESCRIPTIVE NAME: Audio MCD DLL Intialization and termination
*                  functions.
*

* FUNCTION: Create Global heap, carry out per process intialization.
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT:
*
* EXIT-NORMAL: Return Code 0.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* Global Vars Referenced:
*              hmtxProcSem, heap, UserCount,
*              hEventInitCompleted,
*
* INTERNAL REFERENCES:   ADMCEXIT () -- DLL Termination Routine.
*                        InitADMC () -- DLL Intialization Routine
*
* EXTERNAL REFERENCES:   DosResetEventSem ()        - OS/2 API
*                        DosPostEventSem  ()        - OS/2 API
*                        DosCreateMutexSem()        - OS/2 API
*                        HhpCreateHeap    ()        - MME  API
*                        HhpDestroyHeap   ()        - MME  API
*                        HhpGetPID()      ()        - MME  API
*
*********************** END OF SPECIFICATIONS **********************/
#define INCL_BASE
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES

#include <os2.h>                        // OS2 defines.
#include <string.h>
#include <os2medef.h>                   // MME includes files.
#include <stdlib.h>                     // Math functions
#include <ssm.h>                        // SSM Spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mcios2.h>                     // MM System Include.
#include <mcipriv.h>                    // MCI Connection stuff
#include <mmdrvos2.h>                   // MCI Driver include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <qos.h>
#include "audiomcd.h"                   // Component Definitions.
#include "admcfunc.h"                   // Function Prototypes.
#include <sw.h>

/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: ADMCINIT.C
*
* DESCRIPTIVE NAME: AudioMCD DLL Ibtialization.
*
* FUNCTION: Intialize the DLL and provide mem access to multiple processes.
*
* NOTES: Access to The Global heap is provided using the heap manager
*        Routines. The DLL_InitTerm function is the first function
*        called by the OS/2 DLL Loader.  If the flag variable is set to
*        0, then this is DLL initialization, else it will be termination.
*
*        With the global variable ulUserCount, we keep track of how many
*        times this dll is loaded (i.e. how many process have loaded the
*        DLL).  This variable is used to monitor access to the global
*        memory heap that all instances of the dll maintain.
*
*
* ENTRY POINTS:
*
* INPUT:
*
* EXIT-NORMAL: Return Code 1 to the System Loader.
*
* EXIT_ERROR:  Failure To Load This Module.
*
* EFFECTS:
*
* INTERNAL REFERENCES: heap management Routines,
*
*               Global Variables referenced:
*                      heap.
*                      hmtxProcSem.
*                      UserCount.
*
* EXTERNAL REFERENCES: DosCreateMutexSem  ()    -  OS/2 API
*                      DosOpenMutexSem    ()    -  OS/2 API
*                      DosReleaseMutexSem ()    -  OS/2 API
*                      DosCloseMutexSem   ()    -  OS/2 API
*                      DosExitList        ()    -  OS/2 API
*
*********************** END OF SPECIFICATIONS ********************************/

#pragma linkage (_DLL_InitTerm, system)

unsigned long APIENTRY _ADMCInitTerm (unsigned long hModhandle, unsigned long Flag );


unsigned long _DLL_InitTerm (unsigned long hModhandle, unsigned long Flag)
{
  return(_ADMCInitTerm ( hModhandle,  Flag ));
}


ULONG APIENTRY  mciDriverEntry ( PVOID    ulpInstance,     // Instance Ptr
                                 USHORT   usMessage,       // Message
                                 ULONG    ulParam1,        // Flags
                                 ULONG    ulParam2,        // Data
                                 USHORT   usUserParm)      // Data
{
  return(MM_mciDriverEntryADMC (ulpInstance,
                                usMessage,
                                ulParam1,
                                ulParam2,
                                usUserParm));

}

