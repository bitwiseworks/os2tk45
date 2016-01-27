/*********************************************************************/
/*                                                                   */
/* REXXCAL3 - A simple PM based pocket calculator                    */
/*                                                                   */
/*********************************************************************/

/* Standard C libraries */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

 /* Include specific parts of the OS/2 and presentation manager      */
 /* definitions                                                      */

#define INCL_WIN
#include <os2.h>
#define INCL_RXFUNC
#include <rexxsaa.h>

/* Include the constants file created by the Dialog Box Editor.      */
/* This include file defines symbols for all elements of the         */
/* calculator dialog.                                                */

#include "rexxcal3.h"

/* Declare prototypes for all subroutines.                           */

INT main(VOID);                        /* Main PM calculator program */

                                       /* Calculator dialog          */
MRESULT EXPENTRY Calculator(HWND, ULONG, MPARAM, MPARAM);
                                       /* CalcForm external function */
LONG    APIENTRY CalcForm(PSZ, LONG, PRXSTRING, PSZ, PRXSTRING);
                                       /* CalcPrecision ext function */
LONG    APIENTRY CalcPrecision(PSZ, LONG, PRXSTRING, PSZ, PRXSTRING);
VOID    ProcessArithmetic(LONG);       /* Arithmetic processor       */
VOID    AddDigit(INT);                 /* add digit to display       */
VOID    SetDisplay(PRXSTRING);         /* set new display            */
INT     ProcessProgram(PSZ);           /* process a programmed key   */
INT     CheckWhole(PRXSTRING);         /* validate a whole number    */

/* Define some constants for the calculator dialog                   */

#define DISPLAY_WIDTH    20            /* width of display area      */
#define MAX_DIGITS       18            /* maximum digits in display  */
#define SCIENTIFIC        0            /* Scientific floating point  */
#define ENGINEERING       1            /* Engineering floating point */
#define YES               1            /* YES and NO constants (I    */
#define NO                0            /* prefer these over TRUE and */
                                       /* FALSE                      */

/* Global data used for the dialog                                   */

 HAB    hab;                           /* Anchor block handle        */
 HWND   hwndCalc;                      /* Handle for calculator      */
                                       /*   dialog (created using    */
                                       /*   the dialog editor)       */

/* Global data used for the calculator                               */

 UCHAR    accbuff[DISPLAY_WIDTH + 1];  /* accumulator buffer         */
 UCHAR    dispbuff[DISPLAY_WIDTH + 1]; /* display buffer             */
 UCHAR    precision[MAX_DIGITS + 1];   /* current precision          */
 INT      form;                        /* engineering/scientific flag*/
 RXSTRING accumulator;                 /* current register value     */
 RXSTRING display;                     /* current display            */
 INT      oldaction;                   /* pending key action         */
 INT      digits;                      /* digits in display          */
 INT      hadperiod;                   /* period in number           */
 INT      hadexponent;                 /* exponent in number         */

                                       /* table of external function */
                                       /* macro names                */
PUCHAR     operator_names[] = {
    "MULTIPLY.CAL",                    /* * key                      */
    "MINUS.CAL",                       /* - key                      */
    "PLUS.CAL",                        /* + key                      */
    "DIVIDE.CAL",                      /* / key                      */
    "INTEGER.CAL",                     /* % key                      */
    "REMAIND.CAL",                     /* // key                     */
    "POWER.CAL",                       /* ** key                     */
    "PROGRAM1.CAL",                    /* first program key          */
    "PROGRAM2.CAL" };                  /* second program key         */

/*********************************************************************/
/* main() - the main calculator program entry point                  */
/*********************************************************************/

INT main()
{
  HMQ   hmq;                           /* Message queue handle       */
  QMSG  qmsg;                          /* Receive PM queue message   */

                                       /* startup PM usage           */
  hab = WinInitialize(0);              /* Get the anchor block handle*/
  hmq = WinCreateMsgQueue(hab, 0);     /* Create the message queue   */
                                       /* register our window class  */
  WinRegisterClass(hab, "RexxCalc", Calculator, 0l, 0);

  /* Load the calculator dialog.  The dialog definitions are in      */
  /* rexxcalc.rc                                                     */

  hwndCalc = WinLoadDlg(HWND_DESKTOP, HWND_DESKTOP,
                        NULL,
                        0,
                        REXXCALC,      /* Dialog ID                  */
                        NULL);

  WinSendMsg(hwndCalc, WM_SETICON,     /* Set program icon           */
      (MPARAM)WinLoadPointer(HWND_DESKTOP, 0, REXXCALC), NULL);

                                       /* set the window focus       */
  WinSetFocus(HWND_DESKTOP,
      WinWindowFromID(hwndCalc, FID_CLIENT));

  /* Process the standard Presentation Manager message loop until    */
  /* we are told to terminate                                        */

                                       /* While more messages        */
  while (WinGetMsg(hab, &qmsg, 0, 0, 0))
     WinDispatchMsg(hab, &qmsg);       /* dispatch application       */
                                       /*   message handler          */


  /* The close option has been selected so we need to clean up       */
  /* our context.                                                    */

  WinDestroyWindow(hwndCalc);          /* Destroy the dialog window  */
  WinDestroyMsgQueue(hmq);             /* Destroy the message queue  */
  WinTerminate(hab);                   /* Terminate PM usage         */
  return (0);                          /* Indicate successful        */
                                       /*   completion               */
}

/*********************************************************************/
/*                                                                   */
/* Calculator() - the PM WinProc for handling the calculator dialog. */
/* The dialog itself has been created using the dialog box editor.   */
/* This seperates the actual form of the calculator from the program */
/* source.                                                           */
/*                                                                   */
/*********************************************************************/

MRESULT EXPENTRY Calculator(
  HWND    hwnd,                        /* window handle              */
  ULONG   msg,                         /* dispatched message id      */
  MPARAM  mp1,                         /* first message parameter    */
  MPARAM  mp2 )                        /* second message parameter   */
{
  ULONG   action;                      /* Action to process          */

   switch (msg) {                      /* switch based on the message*/
                                       /* received                   */

      /* The initialization message has been received.  We do some   */
      /* additional fixup of the dialog to make it look a little     */
      /* nicer.                                                      */

      case WM_INITDLG:

                                       /* initialize accumulator     */
         MAKERXSTRING(accumulator, accbuff, 1);
         strcpy(accbuff, "0");         /* fill in a zero             */
         digits = 0;                   /* no digits in number        */
         hadperiod = NO;               /* no period yet              */
         hadexponent = NO;             /* no exponential yet         */
         oldaction = 0;                /* no pending operations      */
         strcpy(precision, "9");       /* set default precision      */
         form = SCIENTIFIC;            /* set default form           */
         SetDisplay(&accumulator);     /* set initial display value  */
                                       /* update the calculator      */
                                       /* display                    */
         WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
             display.strptr);
                                       /* register our external      */
                                       /* functions                  */
         RexxRegisterFunctionExe("CalcPrecision", (PFN)CalcPrecision);
         RexxRegisterFunctionExe("CalcForm", (PFN)CalcForm);

         return FALSE;                 /* initialization complete    */

      /* We are going away, time to post a quit message to the       */
      /* message loop.                                               */

      case WM_CLOSE:

                                       /* Deregister our defined     */
                                       /* functions                  */
         RexxDeregisterFunction("CalcPrecision");
         RexxDeregisterFunction("CalcForm");
                                       /* Standard Close processing  */
         WinPostMsg(hwnd, WM_QUIT, 0L, 0L);
         return FALSE;                 /* Exit now                   */

      /* We've received a WM_CONTROL message.  This was              */
      /* generated by the "Form" check blox.                         */

      case WM_CONTROL:                 /* change current form        */

         action = SHORT1FROMMP(mp1);   /* Extract message sub-type   */

         switch (action) {             /* process the control        */

           case BUTTON_FORM:           /* form switch button         */

             if (form == SCIENTIFIC) { /* current scientific?        */
               form = ENGINEERING;     /* make it engineering        */
                                       /* update label               */
               WinSetWindowText(WinWindowFromID(hwnd, BUTTON_FORM),
                   "Engineering");
             }
             else {
               form = SCIENTIFIC;      /* otherwise scientfic        */
               WinSetWindowText(WinWindowFromID(hwnd, BUTTON_FORM),
                  "Scientific");
             }
             return FALSE;             /* all done                   */
           default:                    /* Unknown, can't handle this */
             return FALSE;
         }

      /* We've received a WM_COMMAND message.  WM_COMMAND messages   */
      /* are generated by "pressing" buttons on the calculator.  The */
      /* additional parameters in the received message identify the  */
      /* button that was pressed                                     */

      case WM_COMMAND:

         action = SHORT1FROMMP(mp1);   /* Extract message sub-type   */

         switch (action) {

            /* The following buttons will be processed by the actual */
            /* calculator routine                                    */

            case BUTTON_CLEAR:         /* Clear key                  */

                                       /* initialize accumulator     */
              MAKERXSTRING(accumulator, accbuff, 1);
              strcpy(accbuff, "0");    /* fill in a zero             */
                                       /* initialize the display     */
              MAKERXSTRING(display, dispbuff, 1);
              strcpy(dispbuff, "0");   /* fill in a zero display     */
              digits = 0;              /* no digits yet              */
              hadperiod = NO;          /* turn off number processing */
              hadexponent = NO;        /* flags.                     */
              oldaction = 0;           /* and any pending operations */
                                       /* update the calculator      */
                                       /* display                    */
              WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
                  display.strptr);
              return FALSE;            /* All done                   */

            case BUTTON_DIGITS:        /* set new digits             */
                                       /* not a whole number?        */
              if (!CheckWhole(&display))
                                       /* give an error beep         */
                WinAlarm(HWND_DESKTOP, WA_ERROR);
              else {                   /* process the number         */
                                       /* copy the number            */
                strcpy(precision, display.strptr);
                                       /* redisplay the accumlator   */
                SetDisplay(&accumulator);
                                       /* display                    */
                WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
                    display.strptr);
              }
              return FALSE;            /* All done                   */

            case BUTTON_0:             /* Numeric keys               */
            case BUTTON_1:
            case BUTTON_2:
            case BUTTON_3:
            case BUTTON_4:
            case BUTTON_5:
            case BUTTON_6:
            case BUTTON_7:
            case BUTTON_8:
            case BUTTON_9:
            case BUTTON_PERIOD:        /* decimal point              */
            case BUTTON_EXPONENT:      /* exponent value             */
            case BUTTON_SIGN:          /* sign change                */

                                       /* Add a digit to display     */
              AddDigit(action);
                                       /* update the accumulator     */
                                       /* display                    */
              WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
                    display.strptr);
              return FALSE;            /* All done                   */

            /* The arithmetic operation keys all have a deferred     */
            /* execution.  When one of these is pressed, the previous*/
            /* arithmetic operation is processed using the           */
            /* accumulator and the current display.  The new operator*/
            /* is saved for later execution.  If no operator exists, */
            /* then the current display is moved to the accumulator  */
            /* and no arithmetic is done                             */

            case BUTTON_MULTIPLY:      /* Multiply key               */
            case BUTTON_DIVIDE:        /* Division key               */
            case BUTTON_PLUS:          /* Addition key               */
            case BUTTON_MINUS:         /* Subtraction key            */
            case BUTTON_IDIV:          /* integer division           */
            case BUTTON_REMAINDER:     /* remainder division         */
            case BUTTON_POWER:         /* raise a number to power    */
            case BUTTON_ENTER:         /* "Total" key                */
            case BUTTON_PROGRAM1:      /* programmed Rexx function   */
            case BUTTON_PROGRAM2:      /* programmed Rexx function   */

                                       /* Process pending operations */
              ProcessArithmetic(action);
              SetDisplay(&accumulator);/* reset display buffer       */
                                       /* update the calculator      */
                                       /* display                    */
              WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
                    accumulator.strptr);
              return FALSE;            /* All done                   */

            default:                   /* Unknown, can't handle this */
               return FALSE;
         }

      case WM_ERASEBACKGROUND:         /* disable background erase   */
        return MRFROMLONG(TRUE);       /* don't allow this           */

      /* Message not handled by us.  PM gives us first chance at all */
      /* messages.  Those we don't want to process we pass on to the */
      /* default dialog procedure.                                   */

      default:
         return WinDefWindowProc(hwnd, msg, mp1, mp2);

   }

   return FALSE;                       /* Should never reach here    */
 }

/*********************************************************************/
/*                                                                   */
/* AddDigit(digit) - add a digit to the calculator display           */
/*                                                                   */
/*********************************************************************/

VOID AddDigit(
INT    digit )                         /* new digit to add           */
{

  if (digit == BUTTON_SIGN) {          /* negate number              */
    if (!digits ||                     /* first press?               */
                                       /* sign after exponent?       */
        display.strptr[digits-1] == 'E') {
      display.strptr[digits++] = '-';  /* add the minus sign         */
      display.strptr[digits] = '\0';   /* add new string terminator  */
      display.strlength = digits;      /* keep length in check       */
    }
    else                               /* invalid sign press         */
      WinAlarm(HWND_DESKTOP, WA_ERROR);/* beep at the user           */
  }

  else if (digit == BUTTON_PERIOD) {   /* decimal point              */
    if (hadperiod ||                   /* have one already?          */
        hadexponent ||                 /* or an exponent?            */
        digits >= MAX_DIGITS)          /* or too big                 */
      WinAlarm(HWND_DESKTOP, WA_ERROR);/* beep at the user           */
    else {                             /* add a decimal point        */
      if (!digits)                     /* have digits already?       */
        display.strptr[digits++] = '0';/* no, add leading zero       */
      display.strptr[digits++] = '.';  /* add the decimal point      */
      display.strptr[digits] = '\0';   /* add new string terminator  */
      display.strlength = digits;      /* keep length in check       */
      hadperiod = YES;                 /* remember this              */
    }
  }

  else if (digit == BUTTON_EXPONENT) { /* use scientific             */
    if (hadexponent ||                 /* have one already?          */
        digits >= MAX_DIGITS)          /* or too big                 */
      WinAlarm(HWND_DESKTOP, WA_ERROR);/* beep at the user           */
    else {
      if (!digits)                     /* have digits already?       */
        display.strptr[digits++] = '0';/* no, add leading zero       */
      display.strptr[digits++] = 'E';  /* add the exponent           */
      display.strptr[digits] = '\0';   /* add new string terminator  */
      display.strlength = digits;      /* keep length in check       */
      hadexponent = YES;               /* remember this              */
    }
  }

  else {                               /* real digit                 */
    digit -= BUTTON_0;                 /* make base zero             */
    digit += (INT)'0';                 /* convert to character value */
    if (digits >= MAX_DIGITS)          /* or too big                 */
      WinAlarm(HWND_DESKTOP, WA_ERROR);/* beep at the user           */

    else {                             /* if small enough            */
                                       /* add new digit to display   */
      display.strptr[digits++] = (UCHAR)digit;
      display.strptr[digits] = '\0';   /* add new string terminator  */
      display.strlength = digits;      /* keep length in check       */
    }
  }
}

/*********************************************************************/
/*                                                                   */
/* SetDisplay(newdisplay) - Set the current display string to a new  */
/*                display value.                                     */
/*                                                                   */
/*********************************************************************/

VOID SetDisplay(
  PRXSTRING  newdisplay )              /* new number to display      */
{
                                       /* copy the new value         */
   memcpy(dispbuff, newdisplay->strptr, newdisplay->strlength);
   display.strptr = dispbuff;          /* point at current buffer    */
                                       /* update the length          */
   display.strlength = newdisplay->strlength;
   dispbuff[display.strlength] = '\0'; /* make into ASCII-Z          */
   digits = 0;                         /* no digits in number        */
   hadperiod = NO;                     /* no period yet              */
   hadexponent = NO;                   /* no exponential yet         */
}

/*********************************************************************/
/*                                                                   */
/* ProcessArithmetic(newaction)  -  Process any pending arithmetic   */
/*                operation updating the accumulator and the pending */
/*                operation.                                         */
/*                                                                   */
/*********************************************************************/

VOID ProcessArithmetic(
  LONG  newaction )                    /* new operator key pressed   */
{

   switch(oldaction) {                 /* process prior action key   */

     case BUTTON_MULTIPLY:             /* multiplication             */
     case BUTTON_DIVIDE:               /* division                   */
     case BUTTON_PLUS:                 /* addition                   */
     case BUTTON_MINUS:                /* subtraction                */
     case BUTTON_IDIV:                 /* integer division           */
     case BUTTON_REMAINDER:            /* remainder division         */
     case BUTTON_POWER:                /* raise a number to power    */

                                       /* call the appropriate       */
                                       /* Rexx operand processor     */
       if (ProcessProgram(operator_names[oldaction-BUTTON_MULTIPLY]))
         oldaction = 0;                /* cancel next on error       */
       else
         oldaction = newaction;        /* remember the new action    */
       break;

     default:

                                       /* copy display into accum    */
       strcpy(accumulator.strptr, display.strptr);
                                       /* copy the length also       */
       accumulator.strlength = display.strlength;
       display.strlength = 0;          /* clear out digits           */
       break;                          /* no more needed             */

   }
   switch (newaction) {                /* now process then new key   */

                                       /* process function keys      */
                                       /* immediately                */
     case BUTTON_PROGRAM1:             /* program key one            */
     case BUTTON_PROGRAM2:             /* program key two            */

                                       /* call the appropriate       */
                                       /* Rexx operand processor     */
       SetDisplay(&accumulator);       /* update display             */
       ProcessProgram(operator_names[newaction-BUTTON_MULTIPLY]);
       oldaction = 0;                  /* cancel next action         */
       break;

     default:                          /* all other operations       */
      oldaction = newaction;           /* remember the new action    */
   }

}

/*********************************************************************/
/*                                                                   */
/* ProcessProgram()  - Process a call to a programed Rexx function.  */
/*                     The program is passed both the accumulator    */
/*                     and the current display as strings.           */
/*                                                                   */
/*********************************************************************/

INT  ProcessProgram(
  PSZ        macro_name )              /* macro to invoke            */
{
  RXSTRING   args[2];                  /* passed argument strings    */
  RXSTRING   result;                   /* returned result string     */
  INT        rc;                       /* Rexx return code           */
  SHORT      return_code;              /* function return code       */


  args[0] = accumulator;               /* set first argument         */
  args[1] = display;                   /* set second argument        */
                                       /* ask for result in accum    */
  result = accumulator;                /* copy accumulator data      */
  result.strlength = DISPLAY_WIDTH;    /* set maximum size           */

  rc = RexxStart(2,                    /* two arguments              */
                 args,                 /* array of arguments         */
                 macro_name,           /* name of the macro          */
                 NULL, NULL,
                 RXFUNCTION,           /* calling this a function    */
                 NULL,
                 &return_code,         /* numeric value of return str*/
                 &result);             /* returned string result     */
                                       /* update accumulator length  */
   accumulator.strlength = result.strlength;
                                       /* make into an ASCII-Z string*/
   accumulator.strptr[accumulator.strlength] = '\0';
   if (accumulator.strptr[0] == 'E')   /* function return an error?  */
     return YES;                       /* yes, indicate so           */
   else
     return NO;                        /* no errors                  */
}

/*********************************************************************/
/*                                                                   */
/* CheckWhole()      - Call Rexx to check if the digits setting is   */
/*                     a whole number.                               */
/*                                                                   */
/*********************************************************************/

INT CheckWhole(
  PRXSTRING  number )                  /* number to check            */
{
  RXSTRING   argv;                     /* passed argument string     */
  RXSTRING   result;                   /* returned result string     */
  UCHAR      resbuf[10];               /* result buffer              */
  INT        rc;                       /* Rexx return code           */
  SHORT      return_code;              /* function return code       */


  argv = *number;                      /* set the operand            */
  result.strptr = resbuf;              /* point to return point      */
  result.strlength = sizeof(resbuf);   /* set maximum size           */

  rc = RexxStart(1,                    /* two arguments              */
                 &argv,                /* array of arguments         */
                 "WHOLE.CAL",          /* Whole number checker       */
                 NULL, NULL,
                 RXFUNCTION,           /* calling this a function    */
                 NULL,
                 &return_code,         /* numeric value of return str*/
                 &result);             /* returned string result     */

   if (resbuf[0] == '1')               /* whole number?              */
     return YES;                       /* yes, this is good          */
   else
     return NO;                        /* nope, bad number           */
}

/*********************************************************************/
/*                                                                   */
/* CalcPrecision - External Rexx function for obtaining current      */
/*                 calculator numeric precision                      */
/*                                                                   */
/*********************************************************************/


LONG   APIENTRY CalcPrecision(
  PSZ        name,                     /* function name              */
  LONG       argc,                     /* count of arguments         */
  PRXSTRING  argv,                     /* argument RXSTRINGs         */
  PSZ        queue,                    /* current Rexx queue         */
  PRXSTRING  retstr )                  /* returned string value      */
{
  strcpy(RXSTRPTR(*retstr), (const char *)precision);/* copy over current precision*/
                                       /* set new length             */
  retstr->strlength = strlen((const char *)precision);
  return 0;                            /* completed successfully     */
}


/*********************************************************************/
/*                                                                   */
/* CalcForm      - External Rexx function for obtaining current      */
/*                 caculator numeric form                            */
/*                                                                   */
/*********************************************************************/

LONG   APIENTRY CalcForm(
  PSZ        name,                     /* function name              */
  LONG       argc,                     /* count of arguments         */
  PRXSTRING  argv,                     /* argument RXSTRINGs         */
  PSZ        queue,                    /* current Rexx queue         */
  PRXSTRING  retstr )                  /* returned string value      */
{
  if (form == SCIENTIFIC) {            /* scientific precision?      */
                                       /* return that word           */
    strcpy(RXSTRPTR(*retstr), "SCIENTIFIC");
                                       /* set new length             */
    retstr->strlength = strlen("SCIENTIFIC");
  }
  else {                               /* engineering precision      */
                                       /* return that word           */
    strcpy(RXSTRPTR(*retstr), "ENGINEERING");
                                       /* set new length             */
    retstr->strlength = strlen("ENGINERRING");
  }
  return 0;                            /* completed successfully     */
}
