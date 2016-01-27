/******************************************************************************/
/*  ktguard                  Object REXX Samples                              */
/*                                                                            */
/*  A guard instruction example.                                              */
/*                                                                            */
/*  This program demonstrates the use of the start method and the guard       */
/*  instruction to control execution of several programs.  In this sample,    */
/*  the programs are controlled by one 'guarded' variable.                    */
/******************************************************************************/

/* Define sample class and add methods we will use */
sample = .object~subclass('Sample')
m1 = 'expose g; guard off when g; use arg text; say text'
sample~define('WAIT_ON',  m1)
m2 = 'expose g; use arg g'
sample~define('SETGUARD', m2)

/* Start up 10 invocations of WAIT_ON */
a = sample~new~~setguard(0)                 /* g is false so programs will    */
                                            /* wait on guard instruction      */
do i = 1 to 10
  say 'Start invocation' right(i,2) '(and wait)'
  a~start('WAIT_ON', '  Invocation' right(i,2) 'completing')
end

/* Now let them all run */
say 'Press enter to set guard expression true...'
parse pull .
a~setguard(1)                               /* Setting g to 1 allows programs */
                                            /* to run.                        */
