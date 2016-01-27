/******************************************************************************/
/*  pipe                     Object REXX Samples                              */
/*                                                                            */
/*  Show samples uses of the pipe implementation in PIPE.ORX                  */
/*                                                                            */
/*  This program demonstrates how one could use the pipes implemented in the  */
/*  pipe sample.                                                              */
/******************************************************************************/

info = .array~of('Tom','Mike','Rick','Steve')  /* Create an array to use      */
                                               /* filters on (any collection  */
                                               /* would work).                */

pipe = .filter[.sorter,.reverser,.displayer]   /* Pipe to sort, reverse       */
                                               /* elements and display.       */

pipe~go(info)                                  /* Run it                      */

say '-----------------------------------'

pipe = .filter[.selector['e'],.displayer]      /* Pipe to select elements with*/
                                               /* 'e' and display.            */

pipe~go(info)                                  /* Run it                      */

say '-----------------------------------'

pipe = .filter[.sorter,.stem~new(a.)]          /* Pipe to sort, put in a.     */

pipe~go(info)                                  /* Run it                      */

Do i = 1 To a.0                                /* Show stem values            */
  Say a.i
End

::REQUIRES pipe
