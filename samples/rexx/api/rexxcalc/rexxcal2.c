/*********************************************************************/
/*                                                                   */
/* REXXCAL2 - A simple PM based pocket calculator                    */
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

#include "rexxcal2.h"

/* Declare prototypes for all subroutines.                           */

INT main(VOID);                        /* Main PM calculator program */

                                       /* Calculator dialog          */
MRESULT EXPENTRY Calculator(HWND, ULONG, MPARAM, MPARAM);
VOID    ProcessArithmetic(LONG);       /* Arithmetic processor       */
VOID    AddDigit(INT);                 /* add digit to display       */
INT     GetDisplay(VOID);              /* get display value          */
VOID    SetDisplay(LONG);              /* set new display            */
VOID    ProcessProgram(PSZ);           /* process a programmed key   */

/* Define some constants for the calculator dialog                   */

#define DISPLAY_WIDTH    20            /* width of display area      */
#define MAX_DIGITS        8            /* maximum digits in display  */

/* Global data used for the dialog                                   */

 HAB    hab;                           /* Anchor block handle        */
 HWND   hwndCalc;                      /* Handle for calculator      */
                                       /*   dialog (created using    */
                                       /*   the dialog editor)       */

/* Global data used for the calculator                               */

 UCHAR  display[DISPLAY_WIDTH];        /* Displayed number string    */
 LONG   accumulator;                   /* current register value     */
 LONG   operand;                       /* converted operand number   */
 INT    oldaction;                     /* pending key action         */
 INT    digits;                        /* number of digits in display*/

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

         accumulator = 0;              /* clear the accumulator      */
         oldaction = 0;                /* no pending operations      */
         operand = 0;                  /* no operand                 */
         digits = 0;                   /* no digits in display       */
         SetDisplay(accumulator);      /* set initial display value  */
                                       /* update the calculator      */
                                       /* display                    */
         WinSetWindowText(WinWindowFromID(hwnd, DISPLAY), display);
         return FALSE;                 /* initialization complete    */

      /* We are going away, time to post a quit message to the       */
      /* message loop.                                               */

      case WM_CLOSE:
                                       /* Standard Close processing  */
         WinPostMsg(hwnd, WM_QUIT, 0L, 0L);
         return FALSE;                 /* Exit now                   */

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

              accumulator = 0;         /* clear the accumulator      */
              oldaction = 0;           /* and any pending operations */
              SetDisplay(accumulator); /* set the new display value  */
                                       /* update the calculator      */
                                       /* display                    */
              WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
                  display);
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

                                       /* Add a digit to display     */
              AddDigit(action - BUTTON_0);
                                       /* update the accumulator     */
                                       /* display                    */
              WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
                  display);
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
            case BUTTON_ENTER:         /* "Total" key                */
            case BUTTON_PROGRAM1:      /* programmed Rexx function   */
            case BUTTON_PROGRAM2:      /* programmed Rexx function   */

                                       /* Process pending operations */
              ProcessArithmetic(action);
              SetDisplay(accumulator); /* set the new display value  */
                                       /* update the calculator      */
                                       /* display                    */
              WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
                  display);
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
  INT    digit )                       /* new digit to add           */
{

  digit += (INT)'0';                   /* convert to character value */

  if (digits < MAX_DIGITS) {           /* if small enough            */
     display[digits++] = (UCHAR)digit; /* add new digit to display   */
     display[digits] = '\0';           /* add new string terminator  */
  }
}

/*********************************************************************/
/*                                                                   */
/* GetDisplay() - Convert current display value into a binary long   */
/*                integer in the variable operand                    */
/*                                                                   */
/*********************************************************************/

INT  GetDisplay(void)
{

   if (!digits)                        /* if nothing entered         */
     operand = 0L;                     /* just use zero              */
   else {
     digits = 0;                       /* we've used this value      */
     operand = atol((const char *)display);          /* convert the display value  */
   }
   return operand;                     /* return the value           */
}

/*********************************************************************/
/*                                                                   */
/* SetDisplay(newdisplay) - Set the current display string to a new  */
/*                display value.                                     */
/*                                                                   */
/*********************************************************************/

VOID SetDisplay(
  LONG  newdisplay )                   /* new number to display      */
{
   sprintf(display,"%ld",newdisplay);  /* just use sprintf to update */

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

       accumulator *= GetDisplay();    /* multiplay accumulator by   */
                                       /* current display            */
       break;                          /* no more needed             */

     case BUTTON_DIVIDE:               /* division                   */

       accumulator /= GetDisplay();    /* divide accumulator by      */
                                       /* current display            */
       break;                          /* no more needed             */

     case BUTTON_PLUS:                 /* addition                   */

       accumulator += GetDisplay();    /* add accumulator to         */
                                       /* current display            */
       break;                          /* no more needed             */

     case BUTTON_MINUS:                /* subtraction                */

       accumulator -= GetDisplay();    /* subtract current display   */
                                       /* from accumulator           */
       break;                          /* no more needed             */

     case BUTTON_PROGRAM1:             /* function done as a REXX    */
                                       /* program.  Just call the    */
                                       /* external program.          */
       ProcessProgram("PROGRAM1.RXC"); /* process the program        */
       break;

     case BUTTON_PROGRAM2:             /* function done as a REXX    */
                                       /* program.  Just call the    */
                                       /* external program.          */
       ProcessProgram("PROGRAM2.RXC"); /* process the program        */
       break;

     case BUTTON_ENTER:                /* enter key                  */

       break;                          /* ignore old action          */

     default:
       accumulator = GetDisplay();     /* just move display into the */
                                       /* accumulator                */
       break;                          /* no more needed             */

   }
   oldaction = newaction;              /* remember the new action    */
}

/*********************************************************************/
/*                                                                   */
/* ProcessProgram()  - Process a call to a programed Rexx function.  */
/*                     The program is passed both the accumulator    */
/*                     and the current display as strings.           */
/*                                                                   */
/*********************************************************************/

VOID ProcessProgram(
  PSZ        macro_name )              /* macro to invoke            */
{
  RXSTRING   args[2];                  /* passed argument strings    */
  RXSTRING   result;                   /* returned result string     */
  UCHAR      accum[MAX_DIGITS+1];      /* buffer for converted accum */
  INT        rc;                       /* Rexx return code           */
  SHORT      return_code;              /* function return code       */


  sprintf(accum, "%ld", accumulator);  /* format accumulator         */
                                       /* build accumulator argument */
  MAKERXSTRING(args[0], accum, strlen((const char *)accum));
                                       /* and the current display    */
  MAKERXSTRING(args[1], display, digits);
                                       /* ask for result in display  */
  MAKERXSTRING(result, display, DISPLAY_WIDTH);

  rc = RexxStart(2,                    /* two arguments              */
                 args,                 /* array of arguments         */
                 macro_name,           /* name of the macro          */
                 NULL, NULL,
                 RXFUNCTION,           /* calling this a function    */
                 NULL,
                 &return_code,         /* numeric value of return str*/
                 &result);             /* returned string result     */
  display[result.strlength] = '\0';    /* convert back to ASCII-Z    */
  digits = result.strlength;           /* set new length             */

  accumulator = GetDisplay();          /* accumulator is new display */
  return;                              /* finished (ignore errors    */
                                       /* for now                    */
}
