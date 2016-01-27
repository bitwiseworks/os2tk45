/*********************************************************************/
/*                                                                   */
/*  File Name:          CALLREXX.C                                   */
/*                                                                   */
/*  Description:        Provides a sample call to the REXX           */
/*                      interpreter, passing in an environment name, */
/*                      a file name, and a single argument string.   */
/*                                                                   */
/*  Entry Points:       main - main entry point                      */
/*                                                                   */
/*  Input:              None                                         */
/*                                                                   */
/*  Output:             returns 0 in all cases.                      */
/*                                                                   */
/*********************************************************************/

#define INCL_REXXSAA
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include <stdio.h>                     /* needed for printf()        */
#include <string.h>                    /* needed for strlen()        */

int main(void);                        /* main entry point           */

int main()
    {
    RXSTRING arg;                       /* argument string for REXX  */
    RXSTRING rexxretval;                /* return value from REXX    */

    UCHAR    *str = "These words will be swapped"; /* text to swap   */

    APIRET   rc;                        /* return code from REXX     */
    SHORT    rexxrc = 0;                /* return code from function */

    printf("\nThis program will call the REXX interpreter ");
    printf("to reverse the order of the\n");
    printf("\twords in a string.  ");
    printf("The interpreter is invoked with an initial\n");
    printf("\tenvironment name of 'FNC' ");
    printf("and a file name of 'BACKWARD.FNC'\n\n");

    /* By setting the strlength of the output RXSTRING to zero, we   */
    /* force the interpreter to allocate memory and return it to us. */
    /* We could provide a buffer for the interpreter to use instead. */
    rexxretval.strlength = 0L;          /* initialize return to empty*/

    MAKERXSTRING(arg, str, strlen((const char *)str));/* create input argument     */

    /* Here we call the interpreter.  We don't really need to use    */
    /* all the casts in this call; they just help illustrate         */
    /* the data types used.                                          */
    rc=RexxStart((LONG)      1,             /* number of arguments   */
                (PRXSTRING)  &arg,          /* array of arguments    */
                (PSZ)        "BACKWARD.FNC",/* name of REXX file     */
                (PRXSTRING)  0,             /* No INSTORE used       */
                (PSZ)        "FNC",         /* Command env. name     */
                (LONG)       RXSUBROUTINE,  /* Code for how invoked  */
                (PRXSYSEXIT) 0,             /* No EXITs on this call */
                (PSHORT)     &rexxrc,       /* Rexx program output   */
                (PRXSTRING)  &rexxretval ); /* Rexx program output   */

    printf("Interpreter Return Code: %d\n", rc);
    printf("Function Return Code:    %d\n", (int) rexxrc);
    printf("Original String:         '%s'\n", arg.strptr);
    printf("Backwards String:        '%s'\n", rexxretval.strptr);
    DosFreeMem(rexxretval.strptr);          /* Release storage       */
                                            /* given to us by REXX.  */
    return 0;
    }
