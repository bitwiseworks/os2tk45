/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  MCDDRVRT.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCIDRV_RESTORE MESSAGE HANDLER                        */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1991 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains routines to handle the MCIDRV_RESTORE      */
/*            message.  This message tells us to make this device instance  */
/*            active again.  Restore the device based on the saved state    */
/*            information in the instance structure.                        */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*       MCIDRVRestore() - MCIDRV_RESTORE message handler                   */
/*       MCIDRVRestoreErr() - Error handler for MCIDRV_RESTORE message      */
/****************************************************************************/
#define INCL_BASE                    // Base OS2 functions
#define INCL_DOSSEMAPHORES           // OS2 Semaphore function
#define INCL_MCIOS2                  // use the OS/2 like MMPM/2 headers

#include <os2.h>                     // OS2 defines.
#include <string.h>                  // string prototypes
#include <stdlib.h>                  // standard C functions
#include <os2me.h>                   // MME includes files.
#include "mcdtemp.h"                 // MCD Function Prototypes and typedefs

/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  MCIDRVRestore                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCIDRV_RESTORE message handler.                       */
/*                                                                          */
/* FUNCTION:  Query current master audio settings, and make this instance   */
/*            active.                                                       */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      FUNCTION_PARM_BLOCK  *pFuncBlock -- Pointer to function parameter   */
/*                                          block.                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- Action completed without error.                */
/*            .                                                             */
/*            .                                                             */
/*            .                                                             */
/*            .                                                             */
/*                                                                          */
/****************************************************************************/
RC MCIDRVRestore (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG                ulrc = MCIERR_SUCCESS;    // Propogated Error Code
  ULONG                ulParam1;                 // Message flags
  PMCI_GENERIC_PARMS   pDrvRestoreParms;         // Pointer to GENERIC structure
  PINSTANCE            pInstance;                // Pointer to instance

  /*****************************************************/
  /* dereference the values from pFuncBlock            */
  /*****************************************************/
  ulParam1       = pFuncBlock->ulParam1;
  pInstance      = pFuncBlock->pInstance;
  pDrvRestoreParms = (PMCI_GENERIC_PARMS)pFuncBlock->pParam2;


  /*****************************************************/
  /* NOTE ----->>>                                     */
  /*  This is the basic function that should be        */
  /*  performed.  See the other samples in the toolkit */
  /*  for streaming and MMIO considerations            */
  /*****************************************************/
  DosRequestMutexSem (pInstance->hmtxAccessSem, -2);  // wait for semaphore
  pInstance->Active = TRUE;                           // Set active to TRUE
  QMAudio(pInstance);                                 // Get master audio settings
  DosReleaseMutexSem (pInstance->hmtxAccessSem);      // release semaphore


  return (ulrc);

}      /* end of Open */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  MCIDRVRestoreErr                                       */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCIDRV_RESTORE message handler for errors             */
/*                                                                          */
/* FUNCTION:  Check for errors for the MCIDRV_RESTORE message.              */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      FUNCTION_PARM_BLOCK  *pFuncBlock -- Pointer to function parameter   */
/*                                          block.                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- Action completed without error.                */
/*            .                                                             */
/*            .                                                             */
/*            .                                                             */
/*            .                                                             */
/*                                                                          */
/****************************************************************************/
RC MCIDRVRestoreErr (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG                ulrc = MCIERR_SUCCESS;    // Propogated Error Code
  ULONG                ulParam1;                 // Message flags
  PMCI_GENERIC_PARMS   pDrvRestoreParms;         // Pointer to GENERIC structure
  PINSTANCE            pInstance;                // Pointer to instance

  /*****************************************************/
  /* dereference the values from pFuncBlock            */
  /*****************************************************/
  ulParam1       = pFuncBlock->ulParam1;
  pInstance      = pFuncBlock->pInstance;
  pDrvRestoreParms = (PMCI_GENERIC_PARMS)pFuncBlock->pParam2;


  return (ulrc);

}      /* end of Open */
