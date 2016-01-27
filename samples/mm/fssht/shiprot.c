/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHIPROT.C                                              */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Install Protocol Routine               */
/*                                                                          */
/* NOTES: This is a DLL file. This function is mainly for tuning streams    */
/*        performance by changing the buffer attributes.                    */
/*                                                                          */
/* ENTRY POINTS: ShcInstallProtocol                                         */
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
/* SUBROUTINE NAME: ShcInstallProtocol                                      */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Install Protocol                */
/*                                                                          */
/* FUNCTION: This updates the specified spcb with new values                */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShcInstallProtocol                                          */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: Pointer to shc install protocol parameter block (PARM_INSTPROT)   */
/*        containing:                                                       */
/*   ULONG   ulFunction  Handler command function SHC_ENUMERATE_PROTOCOLS   */
/*   HID     hid         handler ID                                         */
/*   SPCBKEY spcbkey     key of SPCB                                        */
/*   PSBCB   pspcb       pointer to SPCB to install                         */
/*   ULONG   ulFlags     Install / Deinstall flags                          */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR (0)                                                */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*   ERROR_INVALID_SPCBKEY                                                  */
/*   ERROR_ALLOC_RESOURCES                                                  */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES: None                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*   ROUTINES:                                                              */
/*     ShSearchESPCBList                                                    */
/*     ShFindESPCB                                                          */
/*     pfnShInstallCheck                                                    */
/*     HhpAllocMem                                                          */
/*                                                                          */
/*   DATA STRUCTURES:                                                       */
/*     ESPCB                                                                */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC ShcInstallProtocol(pipparm)
PPARM_INSTPROT pipparm;

{ /* Start of ShcInstallProtocol */

RC rc = NO_ERROR;                       /* local return code */
PESPCB pTempEspcb;
PESPCB pPrevEspcb;

  /* the ESPCB list is under semphore control */

  ENTERCRITX(rc);

  if (pipparm->ulFlags & SPI_DEINSTALL_PROTOCOL)
    { /* DeInstall */

      /* To Deinstall, Find the spcb, */
      /*               Take it off the espcb chain, */
      /*               Free the espcb memory allocated */

#ifdef FIXSH
      pTempEspcb = ShSearchESPCBList(&pipparm->spcbkey,0L,&pPrevEspcb);
#else
      pTempEspcb = ShSearchESPCBList(&pipparm->spcbkey,0L,pPrevEspcb);
#endif
      if (pTempEspcb)
        {
          /* Take the espcb off the chain */

          if (pPrevEspcb)
            {
              pPrevEspcb->pnxtESPCB = pTempEspcb->pnxtESPCB;
            }
          else
            {
              pESPCB_ListHead = pTempEspcb->pnxtESPCB;
            }
          HhpFreeMem(hHeap, pTempEspcb);
        }
      else
        {
          rc = ERROR_INVALID_SPCBKEY;
        }
    } /* DeInstall */

  else
    { /* Install */

      /* If the spcb already exists then error */

      if (ShFindESPCB(pipparm->spcbkey,0L))
        {
          rc = ERROR_INVALID_SPCBKEY;
        }
      else
        { /* Ok to add spcb */

          /* Do any special stream handler specific checks before installing */

          if (ulHandlerFlags & HANDLER_INSTALL_CHECKS)
            {
              rc = pfnShInstallCheck(pipparm);
            }

          if (!rc)
            {
              /* Allocate the espcb and put it on the front of the chain */

              pTempEspcb = (PESPCB)HhpAllocMem(hHeap, sizeof(ESPCB));

              if (pTempEspcb)
                {
                  pTempEspcb->spcb = *(pipparm->pspcb);
                  pTempEspcb->pnxtESPCB = pESPCB_ListHead;
                  pESPCB_ListHead = pTempEspcb;
                }
              else
                {
                  rc = ERROR_ALLOC_RESOURCES;
                }
            }
        } /* Ok to add spcb */
    } /* Install */

  EXITCRIT;
  return(rc);

} /* End of ShcInstallProtocol */
