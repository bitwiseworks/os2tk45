/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHGPROT.C                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Get Protocol routine                   */
/*                                                                          */
/* FUNCTION: Get the protocols installed for this stream handler.           */
/*                                                                          */
/* ENTRY POINTS: ShcGetProtocol                                             */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS
#define  INCL_OS2MM
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShcGetProtocol                                          */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Get Protocol                    */
/*                                                                          */
/* FUNCTION: This returns a SPCB indicated by the key passed in.            */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShcGetProtocol                                              */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to shc get protocol parameter block (PARM_GPROT)          */
/*        containing:                                                       */
/*   ULONG   ulFunction  Handler command function SHC_GET_PROTOCOLS         */
/*   HID     hid         handler ID                                         */
/*   SPCBKEY spcbkey     key of SPCB                                        */
/*   PSBCB   pspcb       pointer to SPCB (output)                           */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_INVALID_SPCBKEY                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     ShFindESPCB                                                          */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     ESPCB                                                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShcGetProtocol(pgpparm)
PPARM_GPROT pgpparm;

{ /* Start of ShcGetProtocol */

RC rc = NO_ERROR;                       /* local return code */
PESPCB pTempEspcb;

  ENTERCRITX(rc);

  /*
   * Locate the spcb and return it
   */
  pTempEspcb = ShFindESPCB(pgpparm->spcbkey,0L);
  if (pTempEspcb)
    {
      *pgpparm->pspcb = pTempEspcb->spcb;
    }
  else
    {
      rc = ERROR_INVALID_SPCBKEY;
    }

  EXITCRIT;
  return(rc);

} /* End of ShcGetProtocol */
