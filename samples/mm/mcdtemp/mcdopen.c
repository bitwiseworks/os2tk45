/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  MCDOPEN.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCI_OPEN MESSAGE HANDLER                              */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1991 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains routines to handle the MCI_OPEN message.   */
/*            The processing of this message show allocate the instance     */
/*            structure and initialize it but NOT make it active.  The      */
/*            MCIDRV_RESTORE message will make it active.                   */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*       MCIOpen() - MCI_OPEN message handler                               */
/*       MCIOpenErr() - Error handler for MCI_OPEN message                  */
/****************************************************************************/
#define INCL_BASE                    // Base OS2 functions
#define INCL_DOSSEMAPHORES           // OS2 Semaphore function
#define INCL_MCIOS2                  // use the OS/2 like MMPM/2 headers

#include <os2.h>                     // OS2 defines.
#include <string.h>                  // C string functions
#include <os2me.h>                   // MME includes files.
#include <stdlib.h>                  // Math functions
#include "mcdtemp.h"                 // Function Prototypes.


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  MCIOpen                                                */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCI_OPEN message processor                            */
/*                                                                          */
/* FUNCTION:  Process the MCI_OPEN message.                                 */
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
RC MCIOpen (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG                ulrc = MCIERR_SUCCESS;    // Propogated Error Code
  ULONG                ulParam1;                 // Message flags
  PMCI_INFO_PARMS      pInfoParms;               // Pointer to info structure
  PMMDRV_OPEN_PARMS    pDrvOpenParms;            // Pointer to MMDRV_OPEN structure
  PINSTANCE            pInstance;                // Pointer to instance

  /*****************************************************/
  /* dereference the values from pFuncBlock            */
  /*****************************************************/
  ulParam1       = pFuncBlock->ulParam1;
  pDrvOpenParms  = (PMMDRV_OPEN_PARMS)pFuncBlock->pParam2;

  /*******************************************************************/
  /* Allocate and initialize the instance structure                  */
  /*******************************************************************/
  if (!(pInstance = malloc(sizeof(INSTANCE))))
     ulrc = MCIERR_OUT_OF_MEMORY;
  else
     {
     if (DosCreateMutexSem(NULL,&(pInstance->hmtxAccessSem),DC_SEM_SHARED,FALSE))
        ulrc = MCIERR_DRIVER_INTERNAL;

     if (!ulrc)
        {
        /*******************************************************************/
        /* Set up default values in instance structure.                    */
        /*  NOTE this is a minimal instance structure                      */
        /*******************************************************************/
        memset(pInstance, 0, sizeof(INSTANCE));
        pInstance->usDeviceID = pDrvOpenParms->usDeviceID;
        pInstance->ulTimeFormat = MCI_FORMAT_MMTIME;
        pInstance->ulSpeedFormat = MCI_FORMAT_PERCENTAGE;
        pInstance->ulState = MCD_MODE_OPENING;
        pInstance->ulMode = MCI_MODE_NOT_READY;
        pInstance->Active = FALSE;
        pInstance->ulVolume = 75L;
        pInstance->ulMasterVolume = -1L;
        pInstance->Speaker = TRUE;
        pInstance->Headphone = TRUE;
        pInstance->usDeviceType = pDrvOpenParms->usDeviceType;
        pInstance->usDeviceOrd = pDrvOpenParms->usDeviceOrd;
        memcpy(pDrvOpenParms->pDevParm, pInstance->szDevParams, MAX_DEV_PARAMS);
        pDrvOpenParms->pInstance = pInstance;
        pDrvOpenParms->usResourceUnitsRequired = 1;
        pDrvOpenParms->usResourceClass = 1;
        GetINIInstallName(pInstance);
        GetDeviceInfo(pInstance);
        }
     }


  return (ulrc);

}      /* end of MCIOpen */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  MCIOpenErr                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCI_OPEN message processor for errors                 */
/*                                                                          */
/* FUNCTION:  Process the MCI_OPEN message for errors                       */
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
RC MCIOpenErr (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG                ulrc = MCIERR_SUCCESS;    // Propogated Error Code
  ULONG                ulParam1;                 // Message flags
  PMCI_INFO_PARMS      pInfoParms;               // Pointer to info structure
  PMMDRV_OPEN_PARMS    pDrvOpenParms;            // Pointer to MMDRV_OPEN structure
  PINSTANCE            pInstance;                // Pointer to instance

  /*****************************************************/
  /* dereference the values from pFuncBlock            */
  /*****************************************************/
  ulParam1       = pFuncBlock->ulParam1;
  pDrvOpenParms  = (PMMDRV_OPEN_PARMS)pFuncBlock->pParam2;

  /*******************************************************/
  /* Validate that we have only valid flags              */
  /*******************************************************/
  if (ulParam1 & ~(MCIOPENVALIDFLAGS))
     return(MCIERR_INVALID_FLAG);


  return (ulrc);

}      /* end of MCIOpenErr */

