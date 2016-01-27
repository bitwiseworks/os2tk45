/******************************************************************************/
/*  usecomp                  Object REXX Samples                              */
/*                                                                            */
/*  A simple demonstration of the complex number class                        */
/*                                                                            */
/*  This program demonstrates use of the ::requires directive using the       */
/*  complex number class included in the samples.                             */
/******************************************************************************/

Say 'A simple example of complex number arithmetic:'
comp1 = .complex[8,4]                       /* create two complex numbers     */
comp2 = .complex[4,1]

/* Note that SAY uses the STRING method of the COMPLEX class for display      */
say '('comp1') + ('comp2') is' comp1+comp2
say '('comp1') - ('comp2') is' comp1-comp2
say '('comp1') * ('comp2') is' comp1*comp2
say '('comp1') / ('comp2') is' comp1/comp2
say '('comp1') % ('comp2') is' comp1%comp2
say '('comp1') // ('comp2') is' comp1//comp2

::REQUIRES complex
