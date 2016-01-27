/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  MCDDRVSV.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCIDRV_SAVE MESSAGE HANDLER                           */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1991 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains routines to handle the MCIDRV_SAVE message.*/
/*            The MCIDRV_SAVE message tells us that MDM is save this device */
/*            instance (making inactive).  Save the state of the device in  */
/*            the instance structure                                        */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*       MCIDRVSave() - MCIDRV_SAVE message handler                         */
/*       MCIDRVSaveErr() - Error handler for MCIDRV_SAVE message            */
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
/* SUBROUTINE NAME:  MCIDRVSave                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCIDRV_SAVE message processor                         */
/*                                                                          */
/* FUNCTION:  Process the MCIDRV_SAVE message.  Make the instance inactive. */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      FUNCTION_PARM_BLOCK  *pFuncBlock -- Pointer to function parameter   */
/*                                          block. This block is allocated  */
/*                                          by the main routine message     */
/*                                          router.                         */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- Action completed without error.                */
/*            .                                                             */
/*            .                                                             */
/*            .                                                             */
/*            .                                                             */
/*                                                                          */
/****************************************************************************/
RC MCIDRVSave (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG                ulrc = MCIERR_SUCCESS;    // Propogated Error Code
  ULONG                ulParam1;                 // Message flags
  PMCI_INFO_PARMS      pInfoParms;               // Pointer to info structure
  PMCI_GENERIC_PARMS   pDrvSaveParms;            // Pointer to GENERIC structure
  PINSTANCE            pInstance;                // Pointer to instance

  /*****************************************************/
  /* dereference the values from pFuncBlock            */
  /*****************************************************/
  ulParam1       = pFuncBlock->ulParam1;
  pInstance      = pFuncBlock->pInstance;
  pDrvSaveParms     = (PMCI_GENERIC_PARMS)pFuncBlock->pParam2;

  /*****************************************************/
  /* NOTE ----->>>                                     */
  /*  This is the basic function that should be        */
  /*  performed.  See the other samples in the toolkit */
  /*  for streaming and MMIO considerations            */
  /*****************************************************/
  DosRequestMutexSem (pInstance->hmtxAccessSem, -2);  // wait for semaphore
  pInstance->Active = FALSE;
  DosReleaseMutexSem (pInstance->hmtxAccessSem);      // release semaphore


  return (ulrc);

}      /* end of MCIDRVSave */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  MCIDRVSaveErr                                          */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCIDRV_SAVE message processor for errors.             */
/*                                                                          */
/* FUNCTION:  Process the MCIDRV_SAVE message for errors.                   */
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
RC MCIDRVSaveErr (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG                ulrc = MCIERR_SUCCESS;    // Propogated Error Code
  ULONG                ulParam1;                 // Message flags
  PMCI_INFO_PARMS      pInfoParms;               // Pointer to info structure
  PMCI_GENERIC_PARMS   pDrvSaveParms;            // Pointer to GENERIC structure
  PINSTANCE            pInstance;                // Pointer to instance

  /*****************************************************/
  /* dereference the values from pFuncBlock            */
  /*****************************************************/
  ulParam1       = pFuncBlock->ulParam1;
  pInstance      = pFuncBlock->pInstance;
  pDrvSaveParms     = (PMCI_GENERIC_PARMS)pFuncBlock->pParam2;



  return (ulrc);

}      /* end of MCIDRVSaveErr */
