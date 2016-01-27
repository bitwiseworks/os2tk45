/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: MCISEEK.C
*
* DESCRIPTIVE
*
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
*
* FUNCTION:  Waveform Seek.
*
*  On a seek, a streaming MCD should perform the following commands:
*
*  A. Verify that the flags passed are valid.
*  B. Verify the MCI_FROM, MCI_TO parameter if they were passed in.
*  C. Ensure that any pointers passed are valid.
*  D. Stop any commands which are active on another thread.
*  E. If no stream has been created, then create one.
*  F. If a stream had previously been created, ensure that it is in
*     stopped state.
*
* NOTES:
*
* ENTRY POINTS:
*
* INPUT: MCI_SEEK message.
*
* EXIT-NORMAL: MCIERR_SUCCESS.
*
* EXIT_ERROR:  Error Code.
*
* EFFECTS:
*
* INTERNAL REFERENCES: MCIERR ().
*
* EXTERNAL REFERENCES: spiStopStream()     - SSM Spi
*                      spiSeekStream()     - SSM Spi
*
*********************** END OF SPECIFICATIONS **********************/


#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS

#include <os2.h>                        // OS2 defines.
#include <string.h>                     // String functions.
#include <os2medef.h>                   // MME includes files.
#include <ssm.h>                        // SSM spi includes.
#include <mcios2.h>                     // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mmdrvos2.h>                     // MCI Driver Include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <qos.h>
#include <hhpheap.h>                    // Heap Manager Definitions
#include <audiomcd.h>                   // Component Definitions
#include "admcfunc.h"                   // Function Prototypes
#include <checkmem.h>


RC MCISeek (FUNCTION_PARM_BLOCK *pFuncBlock)

{
  ULONG         ulrc;                    // Return Code
  ULONG         ulParam1;                // Flags For this message
  ULONG         ulAbortNotify = FALSE;   // abort notification messages
  ULONG         ulCnt;                    // Semaphore Post Count
  ULONG         ulTemp1;                 // Temporary Variable
  ULONG         ulSpiFlags = 0L;           // Flags For Spi Seek
  ULONG         ulSeekFlags;             // Incoming Seek Flags From MCI
  ULONG         ulFileLength;            // Element Length in MM

  PMCI_SEEK_PARMS pSeekParms;

  INSTANCE      * ulpInstance;           // Local Instance Ptr

  /********************************************
  * Dereference Pointers From Thread Block
  ********************************************/

  ulParam1 = pFuncBlock->ulParam1;
  pSeekParms = ( PMCI_SEEK_PARMS ) pFuncBlock->ulParam2;

  /*******************************************
  * Mask Wait and Notify Bits
  ********************************************/

  ulParam1 &= ~( MCI_NOTIFY + MCI_WAIT);
  ulSeekFlags = ulParam1;

  /********************************************
  * Mask For Valid Flags on MCI_SEEK
  *********************************************/

  ulSeekFlags &= ~(MCI_TO + MCI_TO_START + MCI_TO_END);

  /* If any excess flags are passed in, return an error */

  if (ulSeekFlags > 0)
      return (MCIERR_INVALID_FLAG);

  /* We cannot seek in two different directions at once */

  if ( ( ulParam1 & MCI_TO_START && ulParam1 & MCI_TO_END ) ||
       ( ulParam1 & MCI_TO_START && ulParam1 & MCI_TO     ) ||
       ( ulParam1 & MCI_TO && ulParam1 & MCI_TO_END ) )
     {
     return (MCIERR_FLAGS_NOT_COMPATIBLE);
     }

  /******************************************
  * We MUST be able to seek to some point in
  * the file, thus to, to end or to start
  * must be passed in.
  *******************************************/

  if ( !( ulParam1 & MCI_TO_START ||
          ulParam1 & MCI_TO_END   ||
          ulParam1 & MCI_TO))

      return (MCIERR_MISSING_FLAG);

  ulpInstance= (INSTANCE *)pFuncBlock->ulpInstance;


  /*************************************************
  ** If the to parameter was sent, then the caller
  ** is required to pass in the seek parms struct
  ** so ensure that the memory is valid
  **************************************************/

  if (ulParam1 & MCI_TO)
     {
     ulrc = CheckMem ( (PVOID) pSeekParms,
                       sizeof (MCI_SEEK_PARMS),
                       PAG_READ);

     if (ulrc != MCIERR_SUCCESS)
        {
        return (MCIERR_MISSING_PARAMETER);
        }

     }

  /************************************
  * If a file has not been opened, then
  * a seek is not possible
  *************************************/

  // change to ulFile + only use TRUE/FALSE for variable

  if ( ulpInstance->fFileExists == FALSE )
     {
     return (MCIERR_FILE_NOT_FOUND);
     }

  /*********************************************
  * MCI_SEEK aborts any ongoing PLAY/RECORD.
  * ensure that the play/record threads can be
  * aborted by acquire a semaphore and then
  * post the aborted message
  *********************************************/

  GetNotifyAbortAccess ( ulpInstance, &ulAbortNotify );

  if ( ulAbortNotify )
     {
     /* Stop the command on another thread */

     GenericThreadAbort( ulpInstance, pFuncBlock, 0  );

     }  /* There was a pending Notify */


  /***********************************************
  * If a set was performed on an existing stream,
  * destroy the stream and get new spcb keys
  ***********************************************/

  DestroySetStream ( ulpInstance );



  /*******************************************
  * In order to perform a seek, a stream must
  * exist.  If it does not exist, create one.
  ********************************************/

  if (ulpInstance->ulCreateFlag != PREROLL_STATE)
    {
    /*******************************
    * Do stream set up work and then
    * create the stream
    *******************************/


    ulrc = PrepareAndCreateStream( ulpInstance, ulpInstance->ulOperation, TRUE );

    if ( ulrc )
       {
       return ( ulrc );
       }

    /******************************
    * UpDate State to created
    *******************************/

    ulpInstance->ulCreateFlag = PREROLL_STATE;

    }  /* Stream Creation */

  /*right now, we do a brute stop always, should
  *  only do it if we are not in stopped state.*/

  DosResetEventSem (ulpInstance->hEventSem, &ulCnt);

  ulrc = ADMCStopStream (ulpInstance, SPI_STOP_DISCARD);

  if ( !ulrc )
     {
     DosWaitEventSem (ulpInstance->hEventSem, (ULONG) -1);
     }

  /*************************************************
  * Stream Seek.  Note, since we can not be sure how
  * long a playlist is, do not bother to check the
  * length
  *************************************************/

  if (ulpInstance->usPlayLstStrm != TRUE)
     {

     if (ulParam1 & MCI_TO)
        {
        /*
        ** Convert whatever time format we are currently
        ** in to mmtime
        */

        ConvertTimeUnits ( ulpInstance,
                           &ulFileLength,
                           FILE_LENGTH );

        /*-------------------------------------------------
        ** If the caller wants to seek past the end of the
        ** file refuse the request
        --------------------------------------------------*/

        ulTemp1 = pSeekParms->ulTo;

        if (ulTemp1 > ulFileLength)
            return (MCIERR_OUTOFRANGE);

        } /* To Flag On */

     } /* Non PlayList */

  /********************************************************
  * Parse Different Seek Flags to Translate into ulSpiFlags
  ********************************************************/

  if (ulParam1 & MCI_TO_START)
     {
     ulTemp1 = 0;
     ulSpiFlags = SPI_SEEK_ABSOLUTE;
     }

  if (ulParam1 & MCI_TO_END)
     {
     ulTemp1 = 0;
     ulSpiFlags = SPI_SEEK_FROMEND;
     }

  if (ulParam1 & MCI_TO)
     {
     ulSpiFlags = SPI_SEEK_ABSOLUTE;
     }

   /*********************************************
   * Convert Seek Units to MMTime
   * This translates Stream Seek units to MMTIME
   * ONLY if the user requested a number
   *********************************************/
   if ( pSeekParms && ( ulParam1 & MCI_TO ) )
      {
      ConvertToMM ( ulpInstance,
                    &ulTemp1,
                    pSeekParms->ulTo);
      }

  /* Do the seek */

  ulrc = ADMCSeekStream ( ulpInstance,
                         ulSpiFlags,
                         (LONG) ulTemp1);

  /**************************************************
  * Update the stream state so that other routines
  * such as status, play, record can take appropriate
  * actions
  **************************************************/

  STRMSTATE = MCI_STOP;

  return (ulrc);

} /* MCISeek */

