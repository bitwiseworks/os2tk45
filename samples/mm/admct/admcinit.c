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
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS

#define MCIERR_SUCCESS          0
#define INTIALIZE               0
#define TERMINATE               1
#define FAILURE                 0L
#define SUCCESS                 1L
#define ADMC_SEM_NAME           "\\SEM32\\ADMC"

#include <os2.h>                        // OS/2 System Include
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
#include "admcres.h"
#include <mmdsload.h>

int _CRT_init( VOID );

//MRESULT EXPENTRY ClipboardProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
extern int          UserCount;
extern HHUGEHEAP    heap;
extern HMTX         hmtxProcSem;
extern ULONG        hModuleHandle;

//MRESULT EXPENTRY ClipboardProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );

/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: ADMEXIT
*
* DESCRIPTIVE NAME: AudioMCD DLL Temination Routine
*
* FUNCTION: Cleanup and deallocate resources used.
*
*
* ENTRY POINTS:
*
* NOTES: The following concepts are illustrated in this file:
*  A. How to keep track of the number of copies of a dll which
*     are loaded in this system.
*  B. How to allocate and deallocate heaps.
*  C. How to use semaphores accross processes.
*
* INPUT:
*
* EXIT-NORMAL: Return Code 1 to the System Loader.
*
* EXIT_ERROR:  Failure To Load This Module.
*
* EFFECTS:
*
* INTERNAL REFERENCES: heap management Routines.
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
*                      DosExitList        ()    -  OS/2 API -- (DCR ed )
*
*********************** END OF SPECIFICATIONS ********************************/


USHORT ADMCEXIT ()                    /* AudioMCD Exit Routine    */
{

  ULONG               ulrc;        // Return Code

  ulrc = MCIERR_SUCCESS;

  /*************************************************
  * Since we are terminating, there is one less
  * copy of our dll alive, so reduce the global
  * memory count of the number of dll's alive by
  * one.  This count is important, since if we ever
  * are reduced to 0 copies of the dll, we should
  * free the heap memory that we allocatd.
  *************************************************/




  /*****************************************
  * Request the mutex semaphore.
  ******************************************/
  if ((ulrc = DosRequestMutexSem ( hmtxProcSem, SEM_INDEFINITE_WAIT)) != 0)
    {
    DosCloseMutexSem (hmtxProcSem);
    return (FAILURE);
    }

  UserCount--;

  /*********************************************
  * If there are no more dll's alive, destroy the
  * heap.
  **********************************************/
  if (!UserCount)
  {
     HhpDestroyHeap (heap);    /* Destroy the heap */
     heap = 0;
  }
  else
  {
    /*********************************************
    * If not the last dll, then release access to heap
    **********************************************/
    HhpReleaseHeap (heap, HhpGetPID());  /* Release access */
  }

  DosReleaseMutexSem (hmtxProcSem);
  DosCloseMutexSem (hmtxProcSem);
  return (SUCCESS);
} /* ADMCExit */


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

/* COVCC_!INSTR */
#pragma linkage (_DLL_InitTerm, system)

/* COVCC_!INSTR */
unsigned long APIENTRY _ADMCInitTerm (unsigned long hModhandle, unsigned long Flag );
unsigned long APIENTRY _ADMCInitTerm (unsigned long hModhandle, unsigned long Flag )


{


//  extern   HAB        hab;
//  extern   HMQ        hmq;
//  extern   QMSG       qmsg;
//  extern   HWND       hwndClipWin;               /* Clipboard win handle */



  ULONG               ulrc;
  USHORT              RCExit;
  ULONG               hmodSSM;
  ULONG               hmodMMIO;
  ulrc = hModhandle;      // Subdue the Warning for now
  ulrc = MCIERR_SUCCESS;

  /*********************************************
  * Store the module handle for future reference
  * (i.e. GpiLoadBitmap)
  **********************************************/


  hModuleHandle = hModhandle;

  if (Flag == INTIALIZE )
  {
       /**************************************
       * Create a semaphore which will be used
       * by all instances of this dll to prevent
       * one copy of the dll from overwriting
       * another's memory etc.
       ***************************************/
       _CRT_init();
       ulrc = DosCreateMutexSem (ADMC_SEM_NAME, &hmtxProcSem, 0L, FALSE);
       if (ulrc == ERROR_DUPLICATE_NAME)
       {
           ulrc = DosOpenMutexSem (ADMC_SEM_NAME, &hmtxProcSem);
       }

       if (ulrc)
          return (FAILURE);  /* Failed Sem Create */

       /****************************************
       * Acquire The Mutex Semaphore
       *****************************************/
       ulrc = DosRequestMutexSem ( hmtxProcSem, SEM_INDEFINITE_WAIT);
       if (ulrc)
       {
         DosCloseMutexSem (hmtxProcSem);
         return (FAILURE);
       }


       /**************************************
       * If we are the first copy of the dll
       * currently loaded, then create a heap
       * where all of our memory will be
       * allocated from.
       ***************************************/
       if (!UserCount && !heap)
       {
           heap = HhpCreateHeap (HEAP_SIZE, HH_SHARED);
           if (heap == (ULONG)NULL)
           {
              DosReleaseMutexSem (hmtxProcSem);
              DosCloseMutexSem (hmtxProcSem);
              return (FAILURE);
           }


       }
       else
       {
         /*************************************************
         * If the user count is greater than one
         * then we just provide access to the global heap
         * which was created on the load of the initial
         * copy of this dll.
         **************************************************/
          ulrc = HhpAccessHeap (heap, HhpGetPID());
          if (ulrc)
               return (FAILURE);
       }

       UserCount++;                         /* Increment Usage */

       /*****************************************
       * The stream handlers, but only for the
       * first DLL
       *****************************************/
       MM_DosLoadModule (NULL, 0L, "SSM",  &hmodSSM);
       MM_DosLoadModule (NULL, 0L, "MMIO", &hmodMMIO);
       ulrc = StreamSetup ( );
       if ( ulrc )
       {
          DosReleaseMutexSem (hmtxProcSem);
          return ( ulrc );
       }

       /******************************************
       * Release The Mutex Semaphore
       *******************************************/
       ulrc = DosReleaseMutexSem (hmtxProcSem);
       if (ulrc)
       {
          if (UserCount)
            DosCloseMutexSem (hmtxProcSem);
          return (FAILURE);
       }

       /***********************************************
       * Init Complete, Indicate success to the loader
       ************************************************/
       return (SUCCESS);

  } /* if this is DLL initialization */
  else
  {
      /********************************************
      * If the flag is anything but initialization
      * then it MUST be termination.
      *********************************************/
      RCExit = ADMCEXIT ();
      return (RCExit);
  } /* else this must be termination */

} /* of Init Term Function */

/* COVCC_INSTR */
