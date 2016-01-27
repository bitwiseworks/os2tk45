/****************************************************************************/
/*                                                                          */
/*                    Copyright (c) IBM Corporation 1992, 1993              */
/*                           All Rights Reserved                            */
/*                                                                          */
/* SOURCE FILE NAME: SHROUTER.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME:  Stream Handler Command Router.                        */
/*                                                                          */
/* FUNCTION: This file contains the stream handler router routine.  It      */
/*           takes SHC commands from the stream manager and routes them     */
/*           to the appropriate routine.                                    */
/*                                                                          */
/* ENTRY POINTS: ShcRouter                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/
#define  INCL_NOPMAPI                  /* no PM include files required */
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSERRORS
#define  INCL_DOSPROCESS
#define  INCL_OS2MM
#include <os2.h>
#include <os2me.h>
#include <hhpheap.h>
#include <shi.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: ShcRouter                                               */
/*                                                                          */
/* DESCRIPTIVE NAME: Stream Handler Command Router routine                  */
/*                                                                          */
/* FUNCTION: This is the entry point routine that the stream manager will   */
/*   call.  It should take the first parameter (function) and call the      */
/*   appropriate routine to service this request.                           */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/* ENTRY POINT: ShcRouter                                                   */
/*   LINKAGE:   CALL NEAR (0:32)                                            */
/*                                                                          */
/* INPUT: pParmIn    - Addr of input parm structure.  The contents of this  */
/*                     structure is dependent on the function called.       */
/*                                                                          */
/* EXIT-NORMAL: NO_ERROR                                                    */
/*                                                                          */
/* EXIT-ERROR:                                                              */
/*        ERROR_INVALID_FUNCTION                                            */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/* INTERNAL REFERENCES:                                                     */
/*        ROUTINES:                                                         */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*        ROUTINES:                                                         */
/*           ShcAssociate                                                   */
/*           ShcCreate                                                      */
/*           ShcDestroy                                                     */
/*           ShcSeek                                                        */
/*           ShcStart                                                       */
/*           ShcStop                                                        */
/*           ShcGetProtocol                                                 */
/*           ShcInstallProtocol                                             */
/*           ShcEnumerateProtocols                                          */
/*           ShcNegotiateResults                                            */
/*           pfnShcSendMsg                                                  */
/*           pfnShcGetTime                                                  */
/*           pfnShcEnableEvent                                              */
/*           pfnShcDisableEvent                                             */
/*           pfnShcEnableSync                                               */
/*           pfnShcDisableSync                                              */
/*                                                                          */
/*        DATA STRUCTURES:                                                  */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

ULONG APIENTRY ShcRouter(pshc)
PVOID pshc;

{ /* Start of ShcRouter */

PSHC_COMMON pshcNow;
RC rc = NO_ERROR;                       /* local return code */

  pshcNow = (PSHC_COMMON)pshc;

  switch (pshcNow->ulFunction)
  {
    case SHC_ASSOCIATE:
         {
           rc = ShcAssociate((PPARM_ASSOC)pshcNow);
           break;
         }
    case SHC_CREATE:
         {
           rc = ShcCreate((PPARM_CREATE)pshcNow);
           break;
         }
    case SHC_DESTROY:
         {
           rc = ShcDestroy((PPARM_DESTROY)pshcNow);
           break;
         }
    case SHC_SEEK:
         {
           rc = ShcSeek((PPARM_SEEK)pshcNow);
           break;
         }
    case SHC_START:
         {
           rc = ShcStart((PPARM_START)pshcNow);
           break;
         }
    case SHC_STOP:
         {
           rc = ShcStop((PPARM_STOP)pshcNow);
           break;
         }
    case SHC_GET_PROTOCOL:
         {
           rc = ShcGetProtocol((PPARM_GPROT)pshcNow);
           break;
         }
    case SHC_INSTALL_PROTOCOL:
         {
           rc = ShcInstallProtocol((PPARM_INSTPROT)pshcNow);
           break;
         }
    case SHC_ENUMERATE_PROTOCOLS:
         {
           rc = ShcEnumerateProtocols((PPARM_ENUMPROT)pshcNow);
           break;
         }
    case SHC_NEGOTIATE_RESULT:
         {
           rc = ShcNegotiateResult((PPARM_NEGOTIATE)pshcNow);
           break;
         }

    case SHC_SENDMSG:
         {
           if (ulHandlerFlags & HANDLER_SHC_SENDMSG)
              rc = pfnShcSendMsg((PPARM_SNDMSG)pshcNow);
           else
              rc = ERROR_INVALID_FUNCTION;
           break;
         }
    case SHC_GET_TIME:
         {
           if (ulHandlerFlags & HANDLER_SHC_GET_TIME)
              rc = pfnShcGetTime((PPARM_GTIME)pshcNow);
           else
              rc = ERROR_INVALID_FUNCTION;
           break;
         }
    case SHC_ENABLE_EVENT:
         {
           if (ulHandlerFlags & HANDLER_SHC_EVENT_CALLS)
              rc = pfnShcEnableEvent((PPARM_ENEVENT)pshcNow);
           else
              rc = ERROR_INVALID_FUNCTION;
           break;
         }
    case SHC_DISABLE_EVENT:
         {
           if (ulHandlerFlags & HANDLER_SHC_EVENT_CALLS)
              rc = pfnShcDisableEvent((PPARM_DISEVENT)pshcNow);
           else
              rc = ERROR_INVALID_FUNCTION;
           break;
         }
    case SHC_ENABLE_SYNC:
         {
           if (ulHandlerFlags & HANDLER_SHC_SYNC_CALLS)
              rc = pfnShcEnableSync((PPARM_ENSYNC)pshcNow);
           else
              rc = ERROR_INVALID_FUNCTION;
           break;
         }
    case SHC_DISABLE_SYNC:
         {
           if (ulHandlerFlags & HANDLER_SHC_SYNC_CALLS)
              rc = pfnShcDisableSync((PPARM_DISSYNC)pshcNow);
           else
              rc = ERROR_INVALID_FUNCTION;
           break;
         }

    default:
         {
           rc = ERROR_INVALID_FUNCTION;
           break;
         }

  } /* endswitch */

  return(rc);

} /* End of ShcRouter */
