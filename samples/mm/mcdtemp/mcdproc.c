/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  MCDPROC.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCD TEMPLATE MAIN PROCEDURE                           */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1991 - 1993                                    */
/*                                                                          */
/* FUNCTION:  This file contains the main procedure for the MCD template.   */
/*                                                                          */
/* NOTES:  For specific streaming or MMIO samples see the CD audio and      */
/*         waveform audio samples in the toolkit.  See the programming      */
/*         reference and programming guide for more details on MCDs and     */
/*         MCI messages and structures.                                     */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*       mciDriverEntry - performs component specific messages of this MCI  */
/*                        Driver.                                           */
/*                                                                          */
/****************************************************************************/
#define INCL_BASE
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES


#define INCL_MCIOS2                  // use the OS/2 like MMPM/2 headers

#include <os2.h>                     // OS2 defines.
#include <string.h>                  // string prototypes
#include <stdlib.h>                  // standard C functions
#include <os2me.h>                   // MME includes files.
#include "mcdtemp.h"                 // MCD Function Prototypes and typedefs


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  mciDriverEntry                                         */
/*                                                                          */
/* DESCRIPTIVE NAME:  MCI Driver Entry                                      */
/*                                                                          */
/* FUNCTION:  Processes the MCI messages sent from MDM..                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      PVOID  lpInstance -- Pointer to device handle.                      */
/*      USHORT usMessage  -- Command message.                               */
/*      ULONG  ulParam1   -- Flag for this message.                         */
/*      PVOID  pParam2    -- Pointer to data record structure.              */
/*      USHORT usUserParm -- User Parameter for mciDriverNotify.            */
/*                                                                          */
/* EXIT CODES:                                                              */
/*      MCIERR_SUCCESS    -- Action completed without error.                */
/*            .                                                             */
/*            .                                                             */
/*            .                                                             */
/*            .                                                             */
/*                                                                          */
/****************************************************************************/

/* This MCI driver is compiled with optlink linkage in C-SET/2
   The following pragma allows other dll's etc to call in */

#pragma linkage(mciDriverEntry, system )

ULONG APIENTRY mciDriverEntry(PVOID pInstance,
                              USHORT usMessage,
                              ULONG ulParam1,
                              PVOID pParam2,
                              USHORT usUserParm)


{
  ULONG                   ulrc;                // Return Code
  FUNCTION_PARM_BLOCK     ParamBlock;          // Encapsulate Parameters

  /***********************************************/
  /* Copy the mciDriverEntry parameters to a     */
  /* structure which contains all of them.  This */
  /* simplifies calling functions etc. since we  */
  /* can pass one parameter rather than 5 or 6.  */
  /***********************************************/

  ParamBlock.usMessage    = usMessage;
  ParamBlock.pInstance    = (PINSTANCE)pInstance;
  ParamBlock.usUserParm   = usUserParm;
  ParamBlock.ulParam1     = ulParam1;
  ParamBlock.pParam2      = (PVOID)pParam2;

  /***********************************************/
  /* Switch based on the MCI message.            */
  /* For each message perform error checking and */
  /* then message processing.                    */
  /*                                             */
  /***********************************************/
  switch (usMessage)
    {

    case MCI_OPEN:
      ulrc = MCIOpenErr(&ParamBlock);
      if (!ulrc)
         ulrc = MCIOpen(&ParamBlock);
     break;

    case MCI_CLOSE:
      ulrc = MCICloseErr(&ParamBlock);
      if (!ulrc)
         ulrc = MCIClose(&ParamBlock);
     break;

    case MCIDRV_SAVE:
      ulrc = MCIDRVSaveErr(&ParamBlock);
      if (!ulrc)
         ulrc = MCIDRVSave(&ParamBlock);
     break;

    case MCIDRV_RESTORE:
      ulrc = MCIDRVRestoreErr(&ParamBlock);
      if (!ulrc)
         ulrc = MCIDRVRestore(&ParamBlock);
     break;

    case MCI_STATUS:
      ulrc = MCIStatusErr(&ParamBlock);
      if (!ulrc)
         ulrc = MCIStatus(&ParamBlock);
     break;

    case MCI_INFO:
      ulrc = MCIInfoErr(&ParamBlock);
      if (!ulrc)
         ulrc = MCIInfo(&ParamBlock);
     break;

    case MCI_ESCAPE:
    case MCI_PLAY:
    case MCI_SEEK:
    case MCI_STOP:
    case MCI_PAUSE:
    case MCI_GETDEVCAPS:
    case MCI_SPIN:
    case MCI_SET:
    case MCI_STEP:
    case MCI_RECORD:
    case MCI_SAVE:
    case MCI_CUE:
    case MCI_UPDATE:
    case MCI_SET_CUEPOINT:
    case MCI_SET_POSITION_ADVISE:
    case MCI_SET_SYNC_OFFSET:
    case MCI_LOAD:
    case MCI_MASTERAUDIO:
    case MCI_GETTOC:
    case MCI_DEVICESETTINGS:
    case MCI_CONNECTOR:
    case MCI_RESUME:
    case MCI_CONNECTORINFO:
    case MCI_CONNECTION:
    case MCI_CAPTURE:
    case MCI_FREEZE:
    case MCI_GETIMAGEBUFFER:
    case MCI_GETIMAGEPALETTE:
    case MCI_PUT:
    case MCI_REALIZE:
    case MCI_REWIND:
    case MCI_RESTORE:
    case MCI_SETIMAGEBUFFER:
    case MCI_SETIMAGEPALETTE:
    case MCI_UNFREEZE:
    case MCI_WHERE:
    case MCI_WINDOW:
    case MCI_DELETE:
    case MCI_CUT:
    case MCI_PASTE:
    case MCI_COPY:
    case MCI_REDO:
    case MCI_UNDO:
    default:
      ulrc = MCIERR_UNRECOGNIZED_COMMAND;
      break;


    }   /* Switch */


  return (ulrc);    /* Return to MDM */

} /* mciDriverEntry */
