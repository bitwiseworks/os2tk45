/******************************************************************************/
/*  ccreply                  Object REXX Samples                              */
/*                                                                            */
/*  A concurrent programming example.                                         */
/*                                                                            */
/*  This program demonstrates how to use reply to run two methods at the same */
/*  time.                                                                     */
/******************************************************************************/
object1 = .example~new                      /* Create two new instances of    */
object2 = .example~new                      /* the example class.             */

say object1~repeat(10, 'Object 1 running')  /* Start first object running     */
say object2~repeat(10, 'Object 2 running')  /* Start second object running    */
say 'Main ended.'
exit                                        /* Exit and watch our kids run    */

                                            
::class example                             /* Define the example class       */
::method repeat                             /* Define the repeat method       */
  use arg reps,msg
  reply 'Repeating "'msg'",' reps 'times.'  /* Reply so others can run        */
  do reps
    say msg                                 /* Show that we're working        */
  end
