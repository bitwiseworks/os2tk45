/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHNEGOT.C                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Negotiate Results routine              */
/*                                                                          */
/* FUNCTION: This function starts a stream instance.                        */
/*                                                                          */
/* ENTRY POINTS: ShcNegotiateResult                                         */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS
#define  INCL_DOSERRORS
#define  INCL_OS2MM
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShcNegotiateResult                                      */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Negotiate results               */
/*                                                                          */
/* FUNCTION: Receives the negotiated SPCB back from SSM for the Create      */
/*           stream call.                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShcNegotiateResult                                          */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to shc negotiate results parameter block (PARM_NEGOTIATE) */
/*        containing:                                                       */
/*   ULONG   ulFunction  Handler command function SHC_ASSOCIATE             */
/*   HID     hid         handler ID                                         */
/*   HSTREAM hstream     handle of stream instance                          */
/*   PSPCB   pspcb       pointer to negotiated SPCB                         */
/*   ULONG   ulErrorStatus SPCB failed the negotiation indicator            */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_INVALID_FUNCTION                                                 */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     ShFindSib                                                            */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     sib                                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShcNegotiateResult(pnrparm)
PARM_NEGOTIATE* pnrparm;

{ /* Start of ShcNegotiateResult */

RC rc = NO_ERROR;                       /* local return code */
PSIB psib;                              /* Stream instance block */

  ENTERCRITX(rc);

  /*
   * Find our Stream Instance Block
   */
  if (!(rc = ShFindSib(&psib, pnrparm->hstream, pnrparm->hid)))
    { /* We have SIB */
      /* error if we don't have something to stream */
      if (psib->ulStatus != SIB_INITIALIZED)
        {
          rc = ERROR_INVALID_FUNCTION;
        }
      else
        {
          psib->espcb.spcb = *pnrparm->pspcb;
          psib->ulStatus = SIB_NEGOTIATED;
        }
    } /* We have SIB */

  EXITCRIT;
  return(rc);

} /* End of ShcNegotiateResult */
