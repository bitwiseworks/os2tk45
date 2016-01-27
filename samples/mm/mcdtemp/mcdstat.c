/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  MCISTAT.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCI_STATUS MESSAGE HANDLER                            */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1991 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains routines to process the MCI_STATUS message.*/
/*            Returns information on the status of this device instance.    */
/*            See the programming reference for a complete list of status   */
/*            items.                                                        */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*       MCIStatus - Process the mci_status message and return status       */
/*       MCIStatusErr - Process the mci_status message for errors           */
/****************************************************************************/
#define INCL_BASE
#define INCL_DOSSEMAPHORES


#define INCL_MCIOS2                  // use the OS/2 like MMPM/2 headers

#include <os2.h>                     // OS2 defines.
#include <string.h>                  // string prototypes
#include <stdlib.h>                  // standard C functions
#include <os2me.h>                   // MME includes files.
#include "mcdtemp.h"                 // MCD Function Prototypes and typedefs

/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  MCIStatus                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCI_STATUS message processor                          */
/*                                                                          */
/* FUNCTION:  Process the MCI_STATUS message.                               */
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
RC MCIStatus (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG                ulrc = MCIERR_SUCCESS;    // Propogated Error Code
  ULONG                ulParam1;                 // Message flags
  PMCI_STATUS_PARMS    pStatusParms;             // Pointer to status structure
  PINSTANCE            pInstance;                // Pointer to instance
  MMTIME               mmTime;                   // Current time in MMTIME format

  /*****************************************************/
  /* dereference the values from pFuncBlock            */
  /*****************************************************/
  ulParam1       = pFuncBlock->ulParam1;
  pInstance      = pFuncBlock->pInstance;
  pStatusParms   = (PMCI_STATUS_PARMS)pFuncBlock->pParam2;


  /**********************************************************/
  /* Lock access to this instance until this command is done*/
  /**********************************************************/
  DosRequestMutexSem (pInstance->hmtxAccessSem, -2);  // wait for semaphore
  switch (pStatusParms->ulItem)
    {

    case MCI_STATUS_TIME_FORMAT:
     ULONG_HIWD(ulrc) = MCI_TIME_FORMAT_RETURN;
     pStatusParms->ulReturn = pInstance->ulTimeFormat;
     break;

    case MCI_STATUS_SPEED_FORMAT:
     ULONG_HIWD(ulrc) = MCI_SPEED_FORMAT_RETURN;
     pStatusParms->ulReturn = pInstance->ulSpeedFormat;
     break;

    case MCI_STATUS_MODE:
     ULONG_HIWD(ulrc) = MCI_MODE_RETURN;
     if (pInstance->Active == TRUE)
        pStatusParms->ulReturn = pInstance->ulMode;
     else
        pStatusParms->ulReturn = MCI_MODE_NOT_READY;
     break;

    case MCI_STATUS_VOLUME:
     ULONG_HIWD(ulrc) = MCI_INTEGER_RETURNED;
     pStatusParms->ulReturn = pInstance->ulVolume;
     break;

    case MCI_STATUS_LENGTH:
     ULONG_HIWD(ulrc) = MCI_INTEGER_RETURNED;
     mmTime = pInstance->ulEndPosition - pInstance->ulStartPosition;
     pStatusParms->ulReturn = ConvertTime(mmTime, MCI_FORMAT_MMTIME, pInstance->ulTimeFormat);
     break;

    case MCI_STATUS_READY:
     ULONG_HIWD(ulrc) = MCI_TRUE_FALSE_RETURN;
     if (pInstance->Active == TRUE)
        {
        if (pInstance->ulMode == MCI_MODE_NOT_READY)
           pStatusParms->ulReturn = MCI_FALSE;
        else
           pStatusParms->ulReturn = MCI_TRUE;
        }
     else
        pStatusParms->ulReturn = MCI_FALSE;
     break;

    case MCI_STATUS_POSITION:
     ULONG_HIWD(ulrc) = MCI_INTEGER_RETURNED;
     pStatusParms->ulReturn = pInstance->ulCurrentPosition;       // need to convert from MMTIME
     pStatusParms->ulReturn =
         ConvertTime(pInstance->ulCurrentPosition, MCI_FORMAT_MMTIME, pInstance->ulTimeFormat);
     break;

    default:
      ulrc = MCIERR_UNSUPPORTED_FLAG;
      break;


    }   /* Switch */


  DosReleaseMutexSem (pInstance->hmtxAccessSem);      // release semaphore

  /***************************************************************/
  /* Send back a notification if the notify flag was on          */
  /***************************************************************/
  if ((ulParam1 & MCI_NOTIFY) & (!ulrc))
     mdmDriverNotify (pInstance->usDeviceID,
                      pStatusParms->hwndCallback,
                      MM_MCINOTIFY,
                      pFuncBlock->usUserParm,
                      MAKEULONG (MCI_STATUS, MCI_NOTIFY_SUCCESSFUL));


  return (ulrc);

}      /* end of MCIStatus */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  MCIStatusErr                                           */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCI_STATUS message processor for errors               */
/*                                                                          */
/* FUNCTION:  Process the MCI_STATUS message for errors                     */
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
RC MCIStatusErr (FUNCTION_PARM_BLOCK *pFuncBlock)
{
  ULONG                ulrc = MCIERR_SUCCESS;    // Propogated Error Code
  ULONG                ulParam1;                 // Message flags
  PMCI_STATUS_PARMS    pStatusParms;             // Pointer to status structure
  PINSTANCE            pInstance;                // Pointer to instance

  /*****************************************************/
  /* dereference the values from pFuncBlock            */
  /*****************************************************/
  ulParam1       = pFuncBlock->ulParam1;
  pInstance      = pFuncBlock->pInstance;
  pStatusParms   = (PMCI_STATUS_PARMS)pFuncBlock->pParam2;


  return (ulrc);

}      /* end of MCIStatusErr */

