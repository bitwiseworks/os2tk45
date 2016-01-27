/****************************************************************************/
/*                                                                          */
/* SOURCE FILE NAME:  CDMCCOMN.C                                            */
/*                                                                          */
/* DESCRIPTIVE NAME:  CD MCD PARSER                                         */
/*                                                                          */
/* COPYRIGHT:  (c) IBM Corp. 1992, 1993                                     */
/*                                                                          */
/* FUNCTION:  This file contains the common functions between the CD MCD    */
/*            and the IBM CD-ROM Drive VSD.                                 */
/*                                                                          */
/*                                                                          */
/* FUNCTIONS:                                                               */
/*       parse_DevParm - Parses the device specific parameter.              */
/*       get_token     - Gets next token and terminates it with a null.     */
/*                                                                          */
/****************************************************************************/

#include <os2.h>
#include <string.h>

/* prototypes */
static CHAR * get_token(CHAR *, USHORT);

/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  parse_DevParm             DEVELOPER:  Garry Lewis      */
/*                                                                          */
/* DESCRIPTIVE NAME:  Parse Device Parameter                                */
/*                                                                          */
/* FUNCTION:  Parses the Device Specific Parameter that is supplied on      */
/*            an MCI_OPEN message.  A pointer to the string to be parsed    */
/*            is supplied, changed to upper case and sets NULLS after the   */
/*            drive and model tokens.  Pointers to these tokens are         */
/*            returned.  The pointer points to a NULL string if tokens are  */
/*            not found.                                                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      CHAR *pSource  -- Pointer to input string to be parsed.             */
/*      CHAR **ppDrive -- Pointer to Drive token.                           */
/*      CHAR **ppModel -- Pointer to Model token.                           */
/*                                                                          */
/* EXIT CODES:  None                                                        */
/*                                                                          */
/* NOTES:  WARNING!! The input string will be modified.                     */
/*         strtok() would have been cleaner to use, however, the global     */
/*         memory used created system errors.                               */
/*                                                                          */
/****************************************************************************/

VOID parse_DevParm(CHAR *pSource, CHAR **ppDrive, CHAR **ppModel)
{
   CHAR *pDrive, *pModel;
   int len;

   /* convert string to upper case */
   strupr(pSource);
   len = strlen(pSource);

   /* Find start of token labels */
   pDrive = strstr(pSource, "DRIVE");
   pModel = strstr(pSource, "MODEL");

   /* Find start of tokens */
   if (pDrive == NULL)
      *ppDrive = pSource + len;
   else
      *ppDrive = get_token(pDrive, FALSE);

   if (pModel == NULL)
      *ppModel = pSource + len;
   else
      *ppModel = get_token(pModel, TRUE);

}  /* of parse_DevParm() */


/****************************************************************************/
/*                                                                          */
/* SUBROUTINE NAME:  get_token                 DEVELOPER:  Garry Lewis      */
/*                                                                          */
/* DESCRIPTIVE NAME:  Get Token                                             */
/*                                                                          */
/* FUNCTION:  gets next token and terminates with a null.                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/*      CHAR *pToken     -- Pointer to input string to be parsed.           */
/*      USHORT LastToken -- TRUE if it is the last token, Model, that       */
/*                          uses the input EOL for the NUL.                 */
/*                                                                          */
/* EXIT CODES:  None                                                        */
/*                                                                          */
/* NOTES:  WARNING!! The input string will be modified.                     */
/*         The Model= label needs to be the last label in the PASRMSTRING   */
/*         because it could use white spaces and delimeters as valid        */
/*         characters, so the EOL null is used for the EOS null.            */
/*                                                                          */
/****************************************************************************/

static CHAR * get_token(CHAR *pToken, USHORT LastToken)
{
   CHAR *pReturn;

   /* find end of label */
   while (*pToken != ' '  && *pToken != '='  && *pToken != ','  &&
          *pToken != '\t' && *pToken != '\n' && *pToken != '\r' &&
          *pToken != '\0')
      pToken++;

   /* find next token */
   if (*pToken == '\0')
      pReturn = pToken;
   else
   {
      while (*pToken == ' '  || *pToken == '='  || *pToken == ','  ||
             *pToken == '\t' || *pToken == '\n' || *pToken == '\r')
         pToken++;

      pReturn = pToken;

      if (!LastToken)
         /* terminate token */
         if (*pToken != '\0')
         {
            while (*pToken != ' '  && *pToken != '='  && *pToken != ','  &&
                   *pToken != '\t' && *pToken != '\n' && *pToken != '\r' &&
                   *pToken != '\0')
               pToken++;

            *pToken = '\0';                 // terminate string

         }  /* of if token is found */
   }  /* of else white spaces exist */

   return(pReturn);

}  /* of get_token() */


