/***************************************************************************
*                                                                          *
* SOURCE FILE NAME:  ADMCINI.C                                             *
*                                                                          *
* DESCRIPTIVE NAME:  Audio MCD Ini PARSER                                  *
*                                                                          *
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
* FUNCTION:  This file contains the routines which will retrieve the       *
*            defaults values from the ini file for the amp/mixer.          *
*                                                                          *
****************************************************************************/

#define INCL_BASE
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES

#include <os2.h>                        // OS2 defines.
#include <string.h>                     // String functions.
#include <os2medef.h>                   // MME includes files.
#include <math.h>                       // Standard Math Lib
#include <stdlib.h>                     // Standard Library
#include <ssm.h>                        // SSM spi includes.
#include <meerror.h>                    // MM Error Messages.
#include <mmioos2.h>                    // MMIO Include.
#include <mcios2.h>                     // MM System Include.
#include <mmdrvos2.h>                   // MCI Driver include.
#include <mcd.h>                        // AudioIFDriverInterface.
#include <hhpheap.h>                    // Heap Manager Definitions
#include <admcdat.h>
#include <qos.h>
#include <audiomcd.h>                   // Component Definitions.
#include <admcfunc.h>


/***************************************************************************
*                                                                          *
* SUBROUTINE NAME:  parse_DevParm             DEVELOPER:  Garry Lewis      *
*                                                                          *
* DESCRIPTIVE NAME:  Parse Device Parameter                                *
*                                                                          *
* FUNCTION:  Parses the Device Specific Parameter that is supplied on      *
*            an MCI_OPEN message.  A pointer to the string to be parsed    *
*            is supplied, changed to upper case and sets NULLS after the   *
*            drive and model tokens.  Pointers to these tokens are         *
*            returned.  The pointer points to a NULL string if tokens are  *
*            not found.                                                    *
*                                                                          *
* PARAMETERS:                                                              *
*                                                                          *
* EXIT CODES:  None                                                        *
*                                                                          *
* NOTES:  WARNING!! The input string will be modified.                     *
*         strtok() would have been cleaner to use, however, the global     *
*         memory used created system errors.                               *
*                                                                          *
****************************************************************************/

ULONG GetDefaults( PCHAR               pOrigSource,
                   INSTANCE            *pInstance )
{
   PCHAR        pDataType;
   PCHAR        pSamplingRate;
   PCHAR        pBitsPerSample;
   PCHAR        pChannels;
   PCHAR        pSource;
   PCHAR        pOperation;
   CHAR         chTemp[ 256 ] ;
   PCHAR        pTemp;

   ULONG        ulrc;
   ULONG        ulLength;

   /* convert string to upper case */

   strupr( pOrigSource );


   ulLength = strlen( pOrigSource );

   memmove( chTemp, pOrigSource, ulLength );

   pSource = chTemp;


   /* Find start of token labels */

   pDataType      = strstr(pSource, "FORMAT");
   pSamplingRate  = strstr(pSource, "SAMPRATE");
   pBitsPerSample = strstr(pSource, "BPS");
   pChannels      = strstr(pSource, "CHANNELS");
   pOperation     = strstr(pSource, "DIRECTION");


   /* Find start of tokens */

   ulrc = PutInInstance( &(pInstance->lDefaultFormat),  pDataType );

   if ( ulrc )
      {
      pInstance->lDefaultFormat = DATATYPE_WAVEFORM;
      }

   PutInInstance( &(pInstance->lDefaultSRate),   pSamplingRate   );

   if ( ulrc )
      {
      pInstance->lDefaultSRate = 22050;
      }

   PutInInstance( &(pInstance->lDefaultBPS),     pBitsPerSample   );


   if ( ulrc )
      {
      pInstance->lDefaultBPS = 8;
      }

   ulrc = PutInInstance( &(pInstance->lDefaultChannels),pChannels);


   if ( ulrc )
      {
      pInstance->lDefaultChannels = 1;
      }

// CONNECTION
   if ( pOperation == NULL )
      {
      pInstance->lDefaultOperation = MCIDRV_INPUT;
      }
   else
      {
      pTemp = GetToken( pOperation, TRUE);
      if ( !strncmp( pTemp, "PLAY", 4 ) )
         {
         pInstance->lDefaultOperation = MCIDRV_OUTPUT;
         }
      else
         {
         pInstance->lDefaultOperation = MCIDRV_INPUT;
         }
      }
// CONNECTION

   return ( MCIERR_SUCCESS );

}  /* GetDefaults */


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

PCHAR GetToken(PCHAR    pToken,
               USHORT   LastToken)
{
   CHAR *pReturn;

   /* find end of label */
   while (*pToken != ' '  && *pToken != '='  && *pToken != '\0')
      pToken++;

   /* find next token */
   if (*pToken == '\0')
      pReturn = pToken;
   else
   {
      while (*pToken == ' '  || *pToken == '='  || *pToken == '\0' )
         pToken++;

      pReturn = pToken;

      if (!LastToken)
         /* terminate token */
         if (*pToken != '\0')
         {
            while (*pToken != ' '  && *pToken != ','  && *pToken != '\0' )
               pToken++;

            *pToken = '\0';                 // terminate string

         }  /* of if token is found */
   }  /* of else white spaces exist */

   return(pReturn);

}  /* GetToken */


/***************************************************************************
*                                                                          *
* SUBROUTINE NAME:  PutInInstance                                          *
*                                                                          *
* DESCRIPTIVE NAME:  PutInInstance                                         *
*                                                                          *
* FUNCTION:  Places a string value from the INI file into the MCD          *
*            instance.                                                     *
*                                                                          *
* PARAMETERS:                                                              *
*                                                                          *
* EXIT CODES:  None                                                        *
*                                                                          *
* NOTES:                                                                   *
*                                                                          *
****************************************************************************/

ULONG PutInInstance( PLONG   pValue,
                    PCHAR   pString )

{
//  PCHAR         pTemp;

   if ( pString == NULL)
      {
      return ( MCIERR_INI_FILE );
      }
   else
      {
      pString = GetToken( pString, FALSE);
      *pValue = atol( pString );
      }

  return ( MCIERR_SUCCESS );

} /* PutInInstance */
