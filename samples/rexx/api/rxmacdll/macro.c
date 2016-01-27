/*********************************************************************/
/*                                                                   */
/*  File Name:          MACRO.C                                      */
/*                                                                   */
/*  Description:        Provides access to the macrospace interface  */
/*                      functions for REXXSAA from REXX programs     */
/*                      via the Available Function Table, which is   */
/*                      composed of external functions for the REXX  */
/*                      interpreter written in languages other than  */
/*                      REXX.                                        */
/*                                                                   */
/*  Entry Points:       MacroLoad  - load a file or library          */
/*                      MacroSave  - save macrospace to a library    */
/*                      MacroErase - clear the macrospace            */
/*                      MacroQuery - check for a function            */
/*                                                                   */
/*  Notes:              Each of the entry points to this library     */
/*                      conforms to the external function interface  */
/*                      format for REXXSAA, i.e., each function has  */
/*                      five (5) parameters: the name the function   */
/*                      was called as, the argument count, the list  */
/*                      of argument strings, the current active      */
/*                      external data queue, and the address of the  */
/*                      RXSTRING structure to contain any return     */
/*                      information.                                 */
/*                                                                   */
/*                      For each function, the returned value is set */
/*                      to the return code from the macrospace API   */
/*                      function call which it performs; therefore,  */
/*                      to properly use these functions a program    */
/*                      should compare the returned value with '0'   */
/*                      to check for successful completion.          */
/*                                                                   */
/*  Return Codes:        0 == Function completed without error       */
/*                      -1 == Illegal call to function               */
/*                                                                   */
/*  Usage Notes:        Since each function has its own entry point  */
/*                      in the library, they can be registered in    */
/*                      the Available Function Table by any name     */
/*                      which is convenient for a program.  The      */
/*                      function name parameter passed to each       */
/*                      routine by the interpreter is ignored.       */
/*                                                                   */
/*********************************************************************/

#define INCL_RXMACRO              /* include macrospace info         */
#define INCL_RXFUNC               /* include external function  info */
#include <rexxsaa.h>              /* REXXSAA header information      */
#include <string.h>
#include <stdio.h>



/*********************************************************************/
/* MacroLoad                                                         */
/* This function will do one of two functions:                       */
/*   When called with one argument:                                  */
/*     The argument must be the name of a file created by            */
/*     RexxSaveMacroSpace on the current release of OS/2.            */
/*     All macros saved in the file will be loaded.                  */
/*                                                                   */
/*   When called with three arguments:                               */
/*     The first argument must be the name of a REXX program file.   */
/*     The second must be "BEFORE" or "AFTER" (upper/lower case does */
/*       not matter)                                                 */
/*     The third is the name that the REXX program will be called by.*/
/*                                                                   */
/* If called with a different number of arguments, it will raise an  */
/*   error.                                                          */
/*                                                                   */
/* The return value of the function is the return code from          */
/*   calling the RexxLoadMacroSpace or RexxAddMacro API.             */
/*                                                                   */
/*********************************************************************/

RexxFunctionHandler MacroLoad;
RexxFunctionHandler MacroSave;
RexxFunctionHandler MacroErase;
RexxFunctionHandler MacroQuery;

/*$PE*/
/*$FD.MacroLoad*/

ULONG APIENTRY MacroLoad(
   PUCHAR    func,
   ULONG     ac,
   PRXSTRING av,
   PSZ       que,
   PRXSTRING ret)
  {
  APIRET rc = -1;                      /* return code from function  */
  ULONG  srch_ord = -1;                /* search order position flag */
                                       /*                            */
  if (ac == 1) {                       /* 1 arg == library load      */
    rc = RexxLoadMacroSpace(0L,        /* set return code from call  */
                     (PSZ *)0,         /*   to RexxLoadMacroSpace    */
                     av[0].strptr);    /*                            */
    sprintf(ret->strptr, "%d", (int)rc);
    ret->strlength=strlen(ret->strptr);/*                            */
    rc = 0;                            /* no errors in call          */
    }                                  /*                            */

  else if (ac == 3) {                  /* 3 args == function load    */
    if (!stricmp(av[1].strptr,"BEFORE"))   /* set 'before' search    */
      srch_ord = RXMACRO_SEARCH_BEFORE;/*                            */
    else if(!stricmp(av[1].strptr,"AFTER"))/* set 'after' search     */
      srch_ord = RXMACRO_SEARCH_AFTER; /*                            */

    rc = RexxAddMacro(av[2].strptr,    /* set return code from call  */
                       av[0].strptr,   /*   to RexxAddMacro          */
                       srch_ord),      /*                            */
    sprintf(ret->strptr, "%d", (int)rc);
    ret->strlength=strlen(ret->strptr);/*                            */
    rc = 0;                            /* no errors in call          */
    }                                  /*                            */

  /* We usually return 0 to indicate our processing was successful   */
  /* and we have returned a valid result.  If we return non-zero,    */
  /* then REXX will raise error 40, "invalid call to routine".       */
  /* For all the functions in this program, we only raise an         */
  /* error if we are called with an incorrect number of arguments.   */
  return rc;                           /*                            */
  }                                    /*                            */


/*********************************************************************/
/* MacroSave                                                         */
/* This function must be called with one argument.                   */
/*   The argument must be a valid OS/2 file name.                    */
/*   This function will call RexxSaveMacroSpace to create a file     */
/*   containing all the REXX programs currently loaded in the        */
/*   macrospace.  NOTE:  This file may have to be re-created after   */
/*   applying service or upgrading to a new release of OS/2.         */
/*                                                                   */
/* If called with a different number of arguments, it will raise an  */
/*   error.                                                          */
/*                                                                   */
/* The return value of the function is the return code from          */
/*   calling the RexxSaveMacroSpace API.                             */
/*                                                                   */
/*********************************************************************/
/*$PE*/
/*$FD.MacroSave*/
ULONG APIENTRY MacroSave(
   PUCHAR    func,
   ULONG     ac,
   PRXSTRING av,
   PSZ       que,
   PRXSTRING ret)
  {
  APIRET rc = -1;                      /* return code from function  */
                                       /*                            */
  if (ac == 1) {                       /* need only one arg          */
    rc = RexxSaveMacroSpace(0,         /* set return code from call  */
                     (PSZ *)0,         /*   to RexxSaveMacroSpace    */
                     av[0].strptr);    /*                            */
    sprintf(ret->strptr, "%d",(int)rc);/*                            */
    ret->strlength=strlen(ret->strptr);/*                            */
    rc = 0;                            /* no errors in call          */
    }                                  /*                            */
  return rc;                           /*                            */
  }                                    /*                            */


/*********************************************************************/
/* MacroErase                                                        */
/* This function must be called with no arguments.                   */
/*   This function will call RexxClearMacroSpace to remove all REXX  */
/*   macros from the macrospace.                                     */
/*                                                                   */
/* If called with any arguments, it will raise an error              */
/*                                                                   */
/* The return value of the function is the return code from          */
/*   calling the RexxSaveMacroSpace API.                             */
/*                                                                   */
/*********************************************************************/
/*$PE*/
/*$FD.MacroErase*/
ULONG APIENTRY MacroErase(
   PUCHAR    func,
   ULONG     ac,
   PRXSTRING av,
   PSZ       que,
   PRXSTRING ret)
  {
  APIRET rc = -1;                      /* return code from function  */
                                       /*                            */
  if (ac == 0) {                       /* no args allowed in call    */
    rc = RexxClearMacroSpace();        /* set return code from call  */
    sprintf(ret->strptr, "%d", (int)rc);
    rc = 0;                            /* no errors in call          */
    }                                  /*                            */
  return rc;                           /*                            */
  }                                    /*                            */


/*********************************************************************/
/* MacroQuery                                                        */
/* This function must be called with one argument.                   */
/*   The argument gives the name of a macro that may be in the       */
/*   macrospace.  This function will call RexxQueryMacro             */
/*   to find out if the macro is present.  This function returns the */
/*   return code of the RexxQueryMacro API.  This function does not  */
/*   return the search order flag.                                   */
/*                                                                   */
/* If called with a different number of arguments, it will raise an  */
/*   error.                                                          */
/*                                                                   */
/* The return value of the function is the return code from          */
/*   calling the RexxQueryMacro API.                                 */
/*                                                                   */
/*********************************************************************/
/*$PE*/
/*$FD.MacroQuery*/
ULONG APIENTRY MacroQuery(
   PUCHAR    func,
   ULONG     ac,
   PRXSTRING av,
   PSZ       que,
   PRXSTRING ret)
  {
  APIRET rc = -1;                      /* return code from function  */
  USHORT pos = 0;                      /* return of search position  */
                                       /*                            */
  if (ac == 1) {                       /* need only one arg          */
    rc = RexxQueryMacro(av[0].strptr,&pos);/* set return code from   */
                                       /*   call to RexxQueryMacro   */
    sprintf(ret->strptr, "%d", (int) rc);
    ret->strlength=strlen(ret->strptr);/*                            */
    rc = 0;                            /* no errors in call          */
    }                                  /*                            */
  return rc;                           /*                            */
  }                                    /*                            */
