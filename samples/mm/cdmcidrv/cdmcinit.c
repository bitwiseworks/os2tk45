/******************************STATE OF SPECIFICATIONS**********************/
/*                                                                         */
/*      SOURCE FILE NAME : CDMCINIT.C                                      */
/*                                                                         */
/*      DESCRIPTIVE NAME : CDMC DATA INITIALIZATION  DLL                   */
/*                                                                         */
/*      COPYRIGHT :                                                        */
/*                    COPYRIGHT (C) IBM CORPORATION 1990 - 1993            */
/*                          ALL RIGHTS RESERVED                            */
/*                                                                         */
/*                                                                         */
/*      FUNCTION : Initialize CDMC DLL with HHP HEAP and removes race      */
/*                 conditions when more than one process is trying to      */
/*                 load the DLL at the same time.                          */
/*                                                                         */
/*******************************END OF SPECIFICATIONS***********************/


#define INCL_BASE
#define INCL_PM
#define INCL_WINWINDOWMGR
#define INCL_MODULEMGR


#include <stdio.h>
#include <os2.h>
#include <stddef.h>
#include <string.h>
#define INCL_MCIOS2
#include <os2me.h>
#include "hhpheap.h"

int            CDMCInitialization(void);

PVOID          CDMC_hHeap;
ULONG          AccessSem = 0;
USHORT         CDMC_Init = 0;


int _CRT_init( VOID );

/*
 * NOTE:   This routine utilizes a feature of the IBM C Set/2 Compiler
 *         that frees programmers from having to write assembler stubs to
 *         initialize their re-entrant DLLs.
 *         C Set/2 provides an assembler initialization stub with the
 *         compiler, and that stub sets the entry point of the DLL to
 *         _DLL_InitTerm.  As long as the DLL entry point has this name,
 *         the DLL can take advantage of the C SET/2 assembler stub instead
 *         of writing one of it's own.
 */

#pragma linkage (_DLL_InitTerm, system)

unsigned long _DLL_InitTerm (unsigned long hModhandle, unsigned long ulTerm)
   {

   ULONG SemWait = 7000;
   USHORT InitCompleted;
   INT rc;

   /*
    * Checking this parameter will insure that this routine will only
    * be run on an actual initialization.  Return success from the
    * termination.
    */

   if (ulTerm)
      return (1L);


   rc = _CRT_init();
   if (rc)
   {
      return (0L);
   }

   InitCompleted = TRUE;   // Set InitCompleted = True;

   /**************************************************************************/
   /* Increment CDMC_Init. If this is the first call to CDMCAttach, then     */
   /* ++CDMC_Init will be 1 , AccessSem will be 0. This causes the semaphore */
   /* to be created, requested and CDMCInitialization called. If ++CDMC_Init */
   /* is >1 or AccessSem != 0, or DosCreateMutexSem fails, then              */
   /* Initialization was previously completed, and HhpAccessHeap will be     */
   /* called                                                                 */
   /**************************************************************************/
   if ( ((++CDMC_Init) == 1) && (!AccessSem) )
      {

      /**********************************************************************
      * If DosCreateMutexSem fails, then Init was previously completed.    **
      **********************************************************************/
      if (!(DosCreateMutexSem(NULL,&AccessSem,DC_SEM_SHARED,FALSE)))
         {

         /******************************************************************/
         /* Set InitCompleted to false (not Previously initialized)        */
         /******************************************************************/
         InitCompleted = 0;

         /******************************************************************/
         /* request the Mutex semaphore                                    */
         /******************************************************************/
         if (DosRequestMutexSem(AccessSem,SemWait))
            {

            /***************************************************************/
            /* close the Mutex semaphore                                   */
            /***************************************************************/
            DosCloseMutexSem(AccessSem);
            return(0);
            }

         /******************************************************************/
         /*   Initialize the HHP heap                                      */
         /******************************************************************/
         if (CDMCInitialization())
            {

            /***************************************************************/
            /* CDMCInitialize failed! Restore The CDMC_Init count          */
            /***************************************************************/
            --CDMC_Init;

            /***************************************************************/
            /*  release the Mutex semaphore                                */
            /***************************************************************/
            DosReleaseMutexSem(AccessSem);

            /***************************************************************/
            /* close the Mutex semaphore                                   */
            /***************************************************************/
            DosCloseMutexSem(AccessSem);
            return(0);
            } /* end if */
         } /* end if */
      } /* end if */


   if (InitCompleted)
   /*********************************************************************/
   /* If CDMCInitialization was called by a previous process:           */
   /* Open and request the semaphore and then Provide shared access to  */
   /* the HHP heap.                                                     */
   /*********************************************************************/
      {
      if ((DosOpenMutexSem(NULL,&AccessSem)))
         return(0);

      if ((DosRequestMutexSem(AccessSem,SemWait)) )
         {

         /****************************************************************/
         /* If request fails, close the Mutex and return an error        */
         /****************************************************************/
         DosCloseMutexSem(AccessSem);
         return(0);
         }

      /*****************************************************************/
      /* Provide access to the heap. Heap was initialized before this  */
      /* call.                                                         */
      /*****************************************************************/
      if (HhpAccessHeap(CDMC_hHeap,HhpGetPID()))
         {
         DosReleaseMutexSem(AccessSem);
         DosCloseMutexSem(AccessSem);
         return(0);
         } /* end if */

      } /* end if */

    /******************************************************************/
    /* Release the semaphore                                          */
    /******************************************************************/
   if (DosReleaseMutexSem(AccessSem))
      return(0);

   /*************************************/
   /* Successful initialization of DLL  */
   /*************************************/
   return(1);
   }



/****************************************************************************
**  CDMCInitialization                                                     **
*****************************************************************************
*
*  Arguments: no arguments
*
*  Return:
****************************************************************************/


int CDMCInitialization(void)
{

  /***********************************************************************/
  /* Obtain initial heap from memory management routines                 */
  /*   store as pHeap for easy reference                                 */
  /***********************************************************************/
  CDMC_hHeap  = HhpCreateHeap(4096L, HH_SHARED);

  return(0);
}

