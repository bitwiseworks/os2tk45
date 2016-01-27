/********************* START OF SPECIFICATIONS *********************
*
* SUBROUTINE NAME: SPICALLS.C
*
* DESCRIPTIVE NAME: Contains routines to manipulate streams.
*
*
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION:
*
* NOTES: The following concepts are illustrated in the source file.
*        A. Calling connection manager to control streams.
*
* ENTRY POINTS:
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
#include <audiomcd.h>                   // Component Definitions.
#include "admcfunc.h"                   // Function Prototypes.


// ****************
// All calls to ADMCStopStream will have to send device id instead of hstream

#ifndef CONNECTION
ULONG ADMCStopStream ( INSTANCE  *pInstance, ULONG ulFlags )

{
 return (SpiStopStream( pInstance->StreamInfo.hStream, ulFlags ));
}
#else
ULONG ADMCStopStream ( INSTANCE  *pInstance, ULONG ulFlags )

{
  return( mciSendCommand( pInstance->usWaveDeviceID,
                          MCIDRV_STOP,
                          ulFlags,
                          0,
                          0) );

}
#endif

/********************* START OF SPECIFICATIONS *******************************
*
* SUBROUTINE NAME: DestroyStream()
*
* DESCRIPTIVE NAME:
*
* FUNCTION: call SpiDestroyStream
*
*
* NOTES:
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
* INTERNAL REFERENCES: None
*
* EXTERNAL REFERENCES: None
*
*********************** END OF SPECIFICATIONS *******************************/

// CONNECTOR FEATURE--this function should not be called.
#ifndef CONNECTION
RC DestroyStream ( INSTANCE  *pInstance)
{

  ULONG ulrc = MCIERR_SUCCESS;

  if ( pInstance->StreamInfo.hStream != (ULONG) NULL)

   {
   ulrc = SpiDestroyStream( pInstance->StreamInfo.hStream );
   }

   pInstance->StreamInfo.hStream = 0;

  return ( ulrc );
} // DestroyStream
#endif



#ifndef CONNECTION
ULONG ADMCSeekStream ( INSTANCE  *pInstance,
                       ULONG  ulFlags,
                       LONG   lSeekPoint )  /*mmtime or other          */

{
  return( SpiSeekStream( pInstance->StreamInfo.hStream, ulFlags, lSeekPoint ) );
}

#else
ULONG ADMCSeekStream ( INSTANCE  *pInstance,
                       ULONG  ulFlags,
                       LONG   lSeekPoint )  /*mmtime or other          */

{
 return ( mciSendCommand( pInstance->usWaveDeviceID,
                          MCIDRV_SEEK,
                          ulFlags,
                          (PVOID) lSeekPoint,
                          0) );

} /* ADMCSeekStream */
#endif

ULONG ADMCAssociate ( HSTREAM hstream, HID hid, PACB pacb )

{
  return ( SpiAssociate( hstream, hid, pacb ) );
}


ULONG APIENTRY ADMCDisableEvent ( HEVENT hevent )
{
  return ( SpiDisableEvent( hevent ) );
}


ULONG APIENTRY ADMCEnableEvent ( PEVCB pevcb,
                                PHEVENT phevent )

{
  return ( SpiEnableEvent( pevcb, phevent ) );
}







