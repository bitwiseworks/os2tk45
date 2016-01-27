/*******************************************************************/
/*                                                                 */
/* Module Name:        RXNLSINF                                    */
/*                                                                 */
/* Description:        Provide limited NLS support for REXX.       */
/*                                                                 */
/* Function:           Returns country dependent information for   */
/*                     use in formatting currency amounts.         */
/*                                                                 */
/* Entry point:        RxNLSInfo()                                 */
/*                                                                 */
/* Notes:                                                          */
/*                                                                 */
/*   The entry point obeys the calling conventions for REXX        */
/*   external functions.                                           */
/*                                                                 */
/*******************************************************************/

/*******************************************************************/
/*                                                                 */
/* Include files.  We use the following:                           */
/*                                                                 */
/*    REXXSAA.H         REXX specific type definitions and         */
/*                      function prototypes.                       */
/*                                                                 */
/*    OS2.H             OS/2 specific type definitions and         */
/*                      function prototypes.                       */
/*                                                                 */
/*    stdlib.h          Standard C library function prototypes.    */
/*                                                                 */
/*    string.h          Standard C library string function         */
/*                      prototypes.                                */
/*                                                                 */
/*******************************************************************/

#define     INCL_DOSNLS
#include    <os2.h>
#define INCL_RXFUNC               /* include external function  info */
#include    <rexxsaa.h>
#include    <stdlib.h>
#include    <string.h>

/*******************************************************************/
/*                                                                 */
/* Function prototype follows.                                     */
/*                                                                 */
/*******************************************************************/
RexxFunctionHandler RxNLSInfo;

/*******************************************************************/
/*                                                                 */
/* Function:            RxNLSInfo()                                */
/*                                                                 */
/* Description:         Return country dependent information.      */
/*                      Please see notes for details.              */
/*                                                                 */
/* Input:               Name of desired information.               */
/*                                                                 */
/* Output:              Desired information.  Please see notes.    */
/*                      Returns 0 if the function executed OK,     */
/*                      -1 otherwise.  The interpreter will        */
/*                      raise an error condition if this function  */
/*                      returns a non-zero result.                 */
/*                                                                 */
/* Effects:             None.                                      */
/*                                                                 */
/* External References: DosQueryCtryInfo()                         */
/*                                                                 */
/* Internal References: none                                       */
/*                                                                 */
/* Notes:                                                          */
/*                                                                 */
/*   This routine takes one parameter.  The form of the call is:   */
/*                                                                 */
/*   NLS_Information = RxNLSInfo( request )                        */
/*                                                                 */
/*   The value of the request parameter determines the information */
/*   returned.  Valid values are:                                  */
/*                                                                 */
/*   request            value returned                             */
/*   -------            --------------                             */
/*                                                                 */
/*   prefix             Currency prefix string.  It is followed by */
/*                      a space if a space should separate the     */
/*                      currency symbol from the value.            */
/*                      If the currency string follows the amount, */
/*                      this request returns an empty string.      */
/*                                                                 */
/*   suffix             Currency suffix string.  If the currency   */
/*                      string preceeds the amount, this request   */
/*                      returns an empty string.                   */
/*                                                                 */
/*   infix              Currency infix string.  This is the        */
/*                      decimal point unless the currency string   */
/*                      goes between the integer and fractional    */
/*                      part of the amount.  In this case, it      */
/*                      is the currency string.                    */
/*                                                                 */
/*   thousand           Thousands separator.                       */
/*                                                                 */
/*   decimal            Decimal separator.                         */
/*                                                                 */
/*   The routine ignores the case of the request parameter.  If    */
/*   the parameter is invalid or the argument count is not one,    */
/*   the function returns an empty string.                         */
/*                                                                 */
/* Error handling:  If this function is called with the wrong      */
/*   number of arguments, or an argument that is not on the        */
/*   list above, then we return with a value of -1, which will     */
/*   raise REXX error 40, "Invalid call to routine", in the        */
/*   REXX program.                                                 */
/*   This function also raises that error if the call to           */
/*   DosQueryCtryInfo produces any non-zero return code.           */
/*                                                                 */
/*******************************************************************/

ULONG APIENTRY RxNLSInfo(
   PUCHAR    func,
   ULONG     argc,
   PRXSTRING argv,
   PSZ       que,
   PRXSTRING ret)
{
   COUNTRYINFO     country_data ;      /* Structure to receive      */
                                       /* country-dependent data.   */
   static
      COUNTRYCODE  country_code =      /* Country and code page     */
                   {                   /* desired.                  */
                      0,               /* Default country code.     */
                      0                /* Default code page.        */
                    } ;

   PSZ              infix ;            /* Currency infix pointer.   */

   ULONG            bytes_returned ;   /* Number of bytes returned  */
                                       /* by DosQueryCtryInfo().    */

   LONG             rc = -1 ;          /* Function return value.    */

   /*****************************************************************/
   /* If the call to DosGetCntryInfo() fails or the arguments are   */
   /* invalid, we want to return an empty RXSTRING.                 */
   /* So we set the return strlength to 0.                          */
   /*                                                               */
   /* Please note that the REXX interpreter allocates 255 bytes in  */
   /* the return string.  This will suffice for us.                 */
   /*****************************************************************/

   ret -> strlength = 0 ;

   /*****************************************************************/
   /* If the argument count is valid (i.e.  1) and the argument is  */
   /* not too long, try to get the country dependent information.   */
   /* If the call succeeds, return the information that the caller  */
   /* wants.                                                        */
   /*****************************************************************/

   if( ( argc == 1 ) &&                /* Arg count valid?          */
       ( ! DosQueryCtryInfo(           /* Have country info?        */
                sizeof( country_data ),
                & country_code,
                & country_data,
                & bytes_returned ) ) )
   {

      /**************************************************************/
      /* We have the country dependent data in the data structure.  */
      /* Extract the data the caller requested and return it in the */
      /* return buffer.                                             */
      /**************************************************************/

      if( ! stricmp( argv->strptr, "PREFIX" ) )
      {
         rc = 0 ;                      /* Function call is OK.     */

         /**********************************************************/
         /* Caller has requested currency prefix.  If the currency */
         /* string preceeds the amount, return it.  If a space     */
         /* should follow the currency string, insert a space      */
         /* after the currency string.                             */
         /**********************************************************/

         if( ! ( country_data . fsCurrencyFmt & 5 ) )
         {
            ( void ) strcpy( ret -> strptr,
                             country_data . szCurrency ) ;

            ret -> strlength =
                        strlen( country_data . szCurrency ) ;

            /********************************************************/
            /* If a space should follow the currency string,        */
            /* append one.                                          */
            /********************************************************/

            if( ( country_data . fsCurrencyFmt & 1 ) )
            {
               ( void ) strcat( ret -> strptr,
                                " " ) ;
               ++ ( ret -> strlength ) ;
            }                          /* If a space follows ...    */
         }                             /* If symbol preceeds ...    */
      }                                /* If caller wants prefix ...*/
      else if( ! stricmp( argv->strptr, "SUFFIX" ) )
      {
         rc = 0 ;                      /* Function call is OK.      */

         /***********************************************************/
         /* Caller has requested the currency suffix.  If the       */
         /* currency string follows the amount, return the string.  */
         /* Otherwise, leave the returned string empty.             */
         /***********************************************************/

         if( ( country_data . fsCurrencyFmt & 5 ) == 1 )
         {
            if( ( country_data . fsCurrencyFmt & 1 ) == 1 )
            {
               ( void ) strcpy( ret -> strptr, " " ) ;
               ret -> strlength = 1 ;
            }

            ( void ) strcat( ret -> strptr,
                             country_data . szCurrency ) ;

            ret -> strlength +=
                        strlen( country_data . szCurrency ) ;
         }                             /* If string follows amount. */
      }                                /* If Caller wants prefix ...*/
      else if( ! stricmp( argv->strptr, "INFIX" ) )
      {
         rc = 0 ;                      /* Function call is OK ...   */

         /***********************************************************/
         /* Caller has requested the currency infix.  If the        */
         /* currency symbol separates the integral and fractional   */
         /* portions of the amount, return the currency string.     */
         /* Otherwise, return the decimal separator.                */
         /***********************************************************/

         infix = (                     /* Select the string to      */
            country_data .             /* return...                 */
               fsCurrencyFmt & 4 ) ?
                  country_data . szCurrency :
                  country_data . szDecimal ;

            ( void ) strcpy( ret -> strptr,
                             infix ) ;

            ret -> strlength =
                        strlen( infix ) ;

      }                                /* If caller wants infix ... */
      else if( ! stricmp( argv->strptr, "THOUSAND" ) )
      {
         rc = 0 ;                      /* Function call is OK.      */

         /***********************************************************/
         /* Caller has requested thousands separator.  Return same. */
         /***********************************************************/

         ( void ) strcpy( ret -> strptr,
                          country_data . szThousandsSeparator ) ;

            ret -> strlength =
                        strlen( country_data . szThousandsSeparator ) ;
      }                                /* If thousand separator ..  */
      else if( ! stricmp( argv->strptr, "DECIMAL" ) )
      {
         rc = 0 ;                      /* Call is OK.               */

         /***********************************************************/
         /* Caller has requested the decimal point.  Return same.   */
         /***********************************************************/

         ( void ) strcpy( ret -> strptr,
                          country_data . szDecimal ) ;

            ret -> strlength =
                        strlen( country_data . szDecimal ) ;
      }                                /* If caller wants decimal.  */
   }                                   /* If args & call OK ...     */
   return( rc ) ;
}                                      /* End of function.          */

