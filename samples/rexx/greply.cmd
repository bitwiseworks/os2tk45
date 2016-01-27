/******************************************************************************/
/*  greply                   Object REXX Samples                              */
/*                                                                            */
/*  Using the GUARD instruction to control methods                            */
/*                                                                            */
/*  This program demonstrates the difference between Guarded and UnGuarded    */
/*  methods.  In the first example, the method is guarded, so it does not     */
/*  begin execution until the final result is tallied.  In the second         */
/*  example, the method is unguarded so it can begin execution while method   */
/*  sum_up is still looping.  In fact, unguarded_get often runs immediately   */
/*  after the Reply, so we use a guard instruction to ensure sum_up runs for  */
/*  a bit before unguarded_get returns with the intermediate sum.             */
/******************************************************************************/

/* guarded_get will wait until sum_up has completed                           */
Say 'Using GUARDED method to wait until sum is complete:'
Say 'Result should be 12502500:' .counter~new~~sum_up(5000)~guarded_get
Say ''

/* unguarded_get will begin execution before sum_up has completed             */
Say 'Using UNGUARDED method to obtain an intermediate result:'
Say 'Result should be less than 12502500:' .counter~new~~sum_up(5000)~unguarded_get
Exit


::CLASS counter
::Method sum_up
  Expose o_var
  o_var = 0                                 /* Initialize our counter         */
  Reply o_var                               /* Early reply so others may run  */
  Do i = 1 to arg(1)                        /* Loop here for a bit            */
    o_var = o_var + i
                                            /* Let them know we're still here */
    If i//1000 = 0 Then Say 'Inside method sum_up:  loop iteration' i
  End

::Method guarded_get                        /* GUARD is default               */
  Expose o_var
  Return o_var

::Method unguarded_get unguarded
  Expose o_var
  Guard off when o_var > 9999               /* Wait until count progresses a bit */
  Return o_var                              /* Return intermediate result     */
