/*static char *SCCSID = "@(#)checkmem.c 13.1 91/09/04";*/
/**************************START OF SPECIFICATIONS **************************/
/*                                                                          */
/* SOURCE FILE NAME:  CHECKMEM.C                                            */
/*                                                                          */
/* DISCRIPTIVE NAME: Check memory address and length                        */
/*                                                                          */
/* COPYRIGHT:                                                               */
/*              Copyright (c) IBM Corporation  1991                         */
/*                        All Rights Reserved                               */
/*                                                                          */
/* STATUS: OS/2 Release 2.0                                                 */
/*                                                                          */
/* FUNCTION: Checks memory address passed to make sure it is valid memory   */
/*                                                                          */
/* NOTES:                                                                   */
/*    DEPENDENCIES: NONE                                                    */
/*    RESTRICTIONS: NONE                                                    */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*                                                                          */
/************************** END OF SPECIFICATIONS ***************************/

#define  INCL_NOPMAPI
#define  INCL_ERRORS
#define  INCL_DOSMEMMGR
#include <os2.h>
#include <os2me.h>
#include <checkmem.h>

/************************** START OF SPECIFICATIONS *************************/
/*                                                                          */
/* SUBROUTINE NAME: CheckMem                                                */
/*                                                                          */
/* DESCRIPTIVE NAME: Memory Check                                           */
/*                                                                          */
/* FUNCTION: Tests memory at specified address and length to see if it      */
/*           exists for the application and if it has the right access.     */
/*                                                                          */
/* NOTES:    This routine contains OS/2 system specific functions.          */
/*           DosQueryMem                                                    */
/*                                                                          */
/* INPUT:    pmem      - Address of memory to test                          */
/*           ulLength  - Length of memory to test                           */
/*           ulFlags   - Memory flags where:                                */
/*                          PAG_EXECUTE                                     */
/*                          PAG_READ                                        */
/*                          PAG_WRITE                                       */
/*                                                                          */
/* OUTPUT:   rc = error return code.                                        */
/*                                                                          */
/* SIDE EFFECTS:                                                            */
/*                                                                          */
/*************************** END OF SPECIFICATIONS **************************/

RC CheckMem ( PVOID pMem,
              ULONG ulLength,
              ULONG ulFlags )
{

  RC rc = NO_ERROR;                       // local return code
  ULONG ulLengthLeft;                     // length left to check
  ULONG ulTotLength = 0L;                 // Total length checked
  ULONG ulRetFlags = 0L;                  // Flags returned from Dos call

  /**************************************************************************/
  /*                                                                        */
  /*   Set up to check memory.                                              */
  /*                                                                        */
  /**************************************************************************/

  ulLengthLeft = ulLength;
  while ((!rc) && (ulTotLength < ulLength))
    {                                             // Call OS to check mem
      if (!(rc = DosQueryMem(pMem, &ulLengthLeft, &ulRetFlags)))
        {                                         // We have the flags
          if ((ulRetFlags & PAG_FREE) ||          // if free then error
              !(ulRetFlags & PAG_COMMIT) ||       // if not committed then error
                                                  // if execute only
//------------((ulRetFlags & PAG_EXECUTE) && !(ulFlags & PAG_EXECUTE)) ||
              ((ulRetFlags & ulFlags) != ulFlags))
            {
              rc = ERROR_INVALID_BLOCK;
            }
          else
            {
              pMem = (PVOID)( (ULONG)pMem + ulLengthLeft );
              ulTotLength += ulLengthLeft;
              ulLengthLeft = ulLength - ulTotLength;
            }
        }
    }
  return(rc);
}
