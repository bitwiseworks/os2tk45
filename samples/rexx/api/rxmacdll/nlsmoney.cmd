/*******************************************************************/
/*                                                                 */
/* Function:            NLSMONEY                                   */
/*                                                                 */
/* Description:         Format currency amounts according to NLS   */
/*                      conventions.                               */
/*                                                                 */
/* Input:               Amount                                     */
/*                                                                 */
/* Output:              Formatted amount                           */
/*                                                                 */
/* Effects:             None                                       */
/*                                                                 */
/* Notes:                                                          */
/*                                                                 */
/*    Requires the external function RxNLSInfo().  This function   */
/*    resides in RXNLSINF.DLL                                      */
/*                                                                 */
/*    If the input string is not a valid amount, NLSMONEY returns  */
/*    an empty string.  Valid amounts are nonnegative and not in   */
/*    exponential notation.                                        */
/*                                                                 */
/*    NLSMONEY does the following:                                 */
/*                                                                 */
/*    1.  Initialize the return value to an empty string.          */
/*                                                                 */
/*    2.  Check the format of the amount.  It must be a number     */
/*        greater than or equal to 0.                              */
/*                                                                 */
/*    3.  If the amount contains a leading '+' sign, drop it.      */
/*        We do this by adding 0 to the amount.                    */
/*                                                                 */
/*    4.  Split the amount into its integral and fractional parts. */
/*        validate the format of each part.  If the original       */
/*        amount was in exponential form, it will fail this step.  */
/*        If either part is empty, make it '0'.                    */
/*                                                                 */
/*    5.  If the external function RxNLSInfo() is not registered,  */
/*        register it.                                             */
/*                                                                 */
/*    6.  Examine the fractional part.  If it is longer than two   */
/*        digits, truncate to two digits.  If it is shorter than   */
/*        two digits, pad with zeros.                              */
/*                                                                 */
/*    7.  Get the thousands separator.  Break the integral part    */
/*        into groups of three digits, and insert the thousands    */
/*        separator between each group of three digits.            */
/*                                                                 */
/*    8.  Assemble the formatted amount.                           */
/*                                                                 */
/*    9.  Return to the caller.                                    */
/*                                                                 */
/*******************************************************************/

arg amount

/********************************************************************/
/* Initialize the function result to an empty string.               */
/********************************************************************/
nls_amount = ""

/********************************************************************/
/* The amount must be a number greater than or equal to 0.  If not, */
/* simply return the empty string set in the preceding step.        */
/********************************************************************/

if datatype( amount, number ) then do
   if 0 <= amount then do

      /**************************************************************/
      /* Remove any leading '+' sign from the amount by adding 0 to */
      /* the amount.                                                */
      /**************************************************************/

      amount = amount + 0

      /**************************************************************/
      /* Parse the amount into its integral and fractional parts.   */
      /* If either part is empty, make it '0'.  Validate both       */
      /* parts.                                                     */
      /**************************************************************/

      parse value amount with integer '.' fraction

      if length( integer ) = 0 then integer = 0
      if length( fraction ) = 0 then fraction = 0

      if datatype( integer, number ) & datatype( fraction, number ),
         then do

         /***********************************************************/
         /* If the external function RxNLSInfo() is not registered, */
         /* do so now.  Signal failure if we cannot register the    */
         /* external function.                                      */
         /***********************************************************/

         if rxfuncquery( 'RxNLSInfo' ) then do
            if rxfuncadd( 'RxNLSInfo', 'RxNLSInf', 'RxNLSInfo' ) then do
               signal failure
            end
         end

         /***********************************************************/
         /* If the fractional part is longer than two digits, trun- */
         /* cate it.  If it is shorter than two digits, padd it     */
         /* on the right with zeros.                                */
         /***********************************************************/

         fraction = left( fraction, 2, '0' )

         /***********************************************************/
         /* Break the integral portion of the amount into groups of */
         /* three digits.  Separate the groups of three with the    */
         /* thousand separator.  Note we parse right to left.  We   */
         /* insert a thousand separator only if digits remain on    */
         /* the left.                                               */
         /***********************************************************/

         thousand = RxNLSInfo( thousand )
         formint = ""

         do while 0 < length( integer )
            lenofint = length( integer )
            lenofgroup = min( lenofint, 3 )
            formint = right( integer, lenofgroup ) || formint
            lenofint = lenofint - lenofgroup
            integer = left( integer, lenofint )
            if 0 < lenofint then formint = thousand || formint
         end

         /***********************************************************/
         /* Assemble the formatted currency amount.                 */
         /***********************************************************/

         nls_amount = RxNLSInfo( prefix ) || ,
                      formint || ,
                      RxNLSInfo( infix ) || ,
                      fraction || ,
                      RxNLSInfo( suffix )
      end                              /* If integer & fraction OK. */
   end                                 /* If amount >= 0            */
end                                    /* If amount is numeric.     */
return nls_amount
