/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHEPROT.C                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Enumerate Protocols routine            */
/*                                                                          */
/* FUNCTION: Enumerate the protocols installed in this stream handler.      */
/*                                                                          */
/* ENTRY POINTS: ShcEnumerateProtocols                                      */
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
/* SUBROUTINE NAME: ShcEnumerateProtocols                                   */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Enumerate Protocols (spcbs)     */
/*                                                                          */
/* FUNCTION: This returns a list of the protocols supported by the stream   */
/*           handler.                                                       */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShcEnumerateProtocols                                       */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to shc enumerate protocol parameter block (PARM_ENUMPROT) */
/*        containing:                                                       */
/*   ULONG   ulFunction  Handler command function SHC_ENUMERATE_PROTOCOLS   */
/*   HID     hid         handler ID                                         */
/*   PVOID   paSPCBKeys  pointer to buffer to fill with SPCB keys           */
/*   PULONG  pulNumSPCBKeys number of SPCB keys on output                   */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_INVALID_BUFFER_SIZE                                              */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     None                                                                 */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     ESPCB                                                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShcEnumerateProtocols(pepparm)
PPARM_ENUMPROT pepparm;

{ /* Start of ShcEnumerateProtocols */

RC rc = NO_ERROR;                       /* local return code */
ULONG  ulNumEspcbs;                     /* loop control */
PSPCBKEY aSpcbKeys;                     /* array of spcbkeys to return */
PESPCB pTempEspcb;                      /* temp pointer to espcb */


  ENTERCRITX(rc);

  /* Count the number of espcb's installed to */
  /* check if application passed large enough buffer. */
  /* If not return error and size needed. */

  pTempEspcb = pESPCB_ListHead;
  ulNumEspcbs = 0;
  while (pTempEspcb)
    {
      ulNumEspcbs++;
      pTempEspcb = pTempEspcb->pnxtESPCB;
    }

  /* check the buffer size passed */
  if (*pepparm->pulNumSPCBKeys < ulNumEspcbs)
    { /* Not enough space */
      rc = ERROR_INVALID_BUFFER_SIZE;
    } /* Not enough space */
  else
    { /* Get the spcb keys */

      /* Loop thru the espcb list and copy the spcbkey to the */
      /* callers buffer. */

      aSpcbKeys = pepparm->paSPCBKeys;
      ulNumEspcbs = 0;
      pTempEspcb = pESPCB_ListHead;
      while (pTempEspcb)
        { /* Loop thru espcbs */

          aSpcbKeys[ulNumEspcbs].ulDataType    = pTempEspcb->spcb.spcbkey.ulDataType;
          aSpcbKeys[ulNumEspcbs].ulDataSubType = pTempEspcb->spcb.spcbkey.ulDataSubType;
          aSpcbKeys[ulNumEspcbs].ulIntKey      = pTempEspcb->spcb.spcbkey.ulIntKey;
          ulNumEspcbs++;
          pTempEspcb = pTempEspcb->pnxtESPCB;

        } /* Loop thru espcbs */

    } /* Get the spcb keys */

  /* Return the real number of spcb keys in all cases */
  *pepparm->pulNumSPCBKeys = ulNumEspcbs;

  EXITCRIT;
  return(rc);

} /* End of ShcEnumerateProtocols */
