/******************************************************************************/
/*  factor                   Object REXX Samples                              */
/*                                                                            */
/*  A factorial program.                                                      */
/*                                                                            */
/*  This program demonstrates a way to define a factorial class using the     */
/*  subclass method and the .methods environment symbol.                      */
/******************************************************************************/
say "Enter a number"
pull number
If \datatype(number, '9') then do
  Say 'Invalid whole number' number
  Exit
end

factorial_class = .string~subclass("Sample")~~define('FACTORIAL', .methods['FACTORIAL'])
say number 'factorial is' factorial_class~new(number)~factorial

Exit

/* Define the factorial method.  Note that this method contains a recursive   */
/* call to itself.                                                            */
::method factorial 
  If self <= 1 then return 1
  else do
    t = self~class~new(self - 1)
    Return self * (t~factorial)
  end
