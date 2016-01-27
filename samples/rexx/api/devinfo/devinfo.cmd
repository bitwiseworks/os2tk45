/*******************************************************************/
/*                                                                 */
/* DEVINFO.CMD                                                     */
/*                                                                 */
/* This program will run the DEVINFO sample.                       */
/* This sample will shows REXX subcommand handler written in C.    */
/* The subcommand handler calls the OS/2 DosDevConfig function     */
/* to get some system information, and then passes it to REXX      */
/* through the REXX variable pool.                                 */
/*                                                                 */
/* Files used for this sample                                      */
/*   DEVINFO.CMD    This program.  Performs the compile and        */
/*                  link.  Also issues subcommands to the          */
/*                  handler.                                       */
/*   DEVINFO.C      The subcommand handler                         */
/*   DEVINFO.DEF    DEF file for the C program                     */
/*                                                                 */
/* Files created by this sample                                    */
/*   DEVINFO.OBJ    compiled version of DEVINFO.C                  */
/*   DEVINFO.DLL    linked version of DEVINFO.C                    */
/*                                                                 */
/*******************************************************************/


/* We assume the current directory is on the LIBPATH.              */
/* To use the DLL, we first register it with REXX, and then        */
/* name it on an ADDRESS instruction.                              */

'RXSUBCOM REGISTER DEVINFO DEVINFO DevInfo'
Select
   When rc = 30 then
      Say 'DEVINFO subcommand handler was already registered'
   When rc = 0 then  Nop          /* rc = 0 means normal           */
   otherwise
      Say 'DEVINFO.CMD:  Some error on RXSUBCOM REGISTER,',
          'return code is' rc
end  /* select */


/*******************************************************************/
/* Now we are ready to go.  This subcommand handler will return    */
/* numeric answers for the following commands.  The answers are    */
/* automatically assigned to the variable of the same name.        */
/* In addition, the command "ALL" will set all the variables.      */
/*                                                                 */
/*   Command    Meaning                                            */
/*   -------    ---------------------------------------------------*/
/*   PARALLEL   number of parallel ports                           */
/*   RS232      number of serial ports                             */
/*   DISKETTE   number of diskette drives                          */
/*   MATH       whether a math coprocessor is available (1=yes)    */
/*   SUBMODEL   system sub-model ID byte (returned in decimal)     */
/*   MODEL      system model id byte (returned in decimal)         */
/*   DISPLAY    display type (0=mono/printer, 1= other)            */
/*                                                                 */
/* The subcommand handler will return with RC=0 for success.       */
/* If the command is not valid, it will raise the ERROR condition  */
/* and put a message in the RC variable.                           */
/* If the subcommand handler has a problem calling DosDevConfig    */
/* or RexxVariablePool, it will raise the FAILURE condition        */
/* and put the bad return code into the RC variable.               */
/*******************************************************************/

Call On Error
Call On Failure

Say 'Now checking on your parallel port(s)'
Address DEVINFO 'PARALLEL'
If rc = 0 Then Say 'Your number of parallel port(s) is' parallel
Say ' '

Say 'Now checking on your diskette drive(s)'
Address DEVINFO 'DISKETTE'
If rc = 0 Then Say 'Your number of diskette drive(s) is' diskette
Say ' '

Say 'Now you can try other commands.  The commands are:'
Say 'PARALLEL, RS232, DISKETTE, MATH, SUBMODEL, MODEL, DISPLAY,'
Say 'and ALL.'
Say 'You may also enter an invalid command. ',
    'Enter a null line to quit'

Do Forever
   /* Drop all the variables, so the action of the subcommand      */
   /* handler will be more visible.                                */
   Drop parallel rs232 diskette math submodel model display

   /* Read in a line from the keyboard and make it upper case.     */
   Parse Upper Linein input
   If input = '' Then Leave       /* exit loop for null line       */
   Address DEVINFO input

   If rc = 0 then Do
      Say 'Now the value of the variable' input' is',
          value(input)
      If input = 'ALL' Then Do
         Say 'Here are the values of all the variables set'
         Say 'PARALLEL ="'parallel'"'
         Say 'RS232    ="'rs232'"'
         Say 'DISKETTE ="'diskette'"'
         Say 'MATH     ="'math'"'
         Say 'SUBMODEL ="'submodel'"'
         Say 'MODEL    ="'model'"'
         Say 'DISPLAY  ="'display'"'
         End  /* listing ALL */
      End /* rc = 0 */

   Say ' '
   Say 'Enter another command, or null line to quit.'
end /* Do Forever */

'RXSUBCOM DROP DEVINFO'           /* Clean up after ourselves      */
Say 'DEVINFO sample program complete.'
'@PAUSE'
exit


/*******************************************************************/
failure:
Say '   FAILURE condition raised.'
Say '   There was some problem calling OS/2 system services'
Say '   The return code from OS/2 is' rc
return


/*******************************************************************/
error:
say '   ERROR condition raised, the command was invalid.'
Say '   The command was "'Condition("D")'".'
Say '   The error code set by DEVINFO.DLL is' rc
return
