/***************_******************************************************/
/*                                                                   */
/* REXXCALC - A simple PM based pocket calculator                    */
/*                                                                   */
/*********************************************************************/

/* Standard C libraries */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <process.h>

 /* Include specific parts of the OS/2 and presentation manager      */
 /* definitions                                                      */

#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_WIN
#include "os2.h"
#define INCL_REXXSAA
#include <rexxsaa.h>
#include "pmrexxio.h"

/* Include the constants file created by the Dialog Box Editor.      */
/* This include file defines symbols for all elements of the         */
/* calculator dialog.                                                */

#include "rexxcalc.h"

/* Declare some typedefs                                             */

typedef struct {                       /* Instorage descriptor       */
  RXSTRING    source;                  /* Program source             */
  RXSTRING    image;                   /* tokenized image            */
} INSTORE;

typedef INSTORE *PINSTORE;             /* pointer to instore desc.   */

/* Declare prototypes for all subroutines.                           */

INT main(void);                        /* Main PM calculator program */

                                       /* Calculator dialog          */
MRESULT EXPENTRY Calculator(HWND, ULONG, MPARAM, MPARAM);
                                       /* Button subclass winproc    */
MRESULT EXPENTRY Button(HWND, ULONG, MPARAM, MPARAM);
                                       /* CalcForm external function */
LONG    EXPENTRY CalcForm(PSZ, LONG, PRXSTRING, PSZ, PRXSTRING);
                                       /* CalcPrecision ext function */
LONG    EXPENTRY CalcPrecision(PSZ, LONG, PRXSTRING, PSZ, PRXSTRING);
                                       /* subcommand handler         */
APIRET  EXPENTRY CalcCommand(PRXSTRING, PUSHORT, PRXSTRING);
LONG    EXPENTRY init_routine(LONG, LONG, PUCHAR);
void    CalcThread(void);              /* thread for calculations    */
void    EditThread(void);              /* thread for editting        */
void    ProcessArithmetic(LONG);       /* Arithmetic processor       */
void    AddDigit(INT);                 /* add digit to display       */
void    SetDisplay(PRXSTRING);         /* set new display            */
                                       /* process a programmed key   */
void    ProcessProgram(ULONG, PRXSTRING, PRXSTRING, PRXSTRING);
void    ProcessMacro(ULONG, PRXSTRING, PRXSTRING, PRXSTRING, ULONG);
INT     Datatype(PSZ, PSZ);            /* validate a whole number    */
                                       /* read macro source          */
void    ReadSourceFile(PSZ, PINSTORE);
void    FreeSourceFile(PINSTORE);      /* free macro source          */
INT     SetRexxVariable(PSZ, PSZ);     /* set a Rexx variable        */

/* Define some constants for the calculator dialog                   */

#define DISPLAY_WIDTH    20            /* width of display area      */
#define MAX_DIGITS       18            /* maximum digits in display  */
#define SCIENTIFIC        0            /* Scientific floating point  */
#define ENGINEERING       1            /* Engineering floating point */
#define NUMBER_MEMORIES   50           /* size of memory registers   */
#define STACKSIZE         8192         /* thread execution stack     */
#define YES               1            /* YES and NO constants (I    */
#define NO                0            /* prefer these over TRUE and */
                                       /* FALSE                      */
#define WM_EDIT_DONE      WM_USER+1    /* edit completion user msg   */
#define WM_MEMORY_DONE    WM_USER+2    /* calc completion user msg   */
#define WM_OPERATOR_DONE  WM_USER+3    /* calc completion user msg   */
#define WM_PROGRAM_DONE   WM_USER+4    /* calc completion user msg   */
#define MACRO_COUNT       (sizeof(operator_names)/sizeof(PSZ))

/* Global data used for the dialog                                   */

 HAB    hab;                           /* Anchor block handle        */
 HWND   hwndCalc;                      /* Handle for calculator      */
                                       /*   dialog (created using    */
                                       /*   the dialog editor)       */

/* Global data used for the calculator                               */

                                       /* memory registers           */
 PSZ      memory_bank[NUMBER_MEMORIES + 1];
 UCHAR    membuff[3];                  /* memory function selector   */
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
 INT      memory_function;             /* memory function flag       */
 INT      memory_action;               /* save memory action         */
                                       /* saved window procedures    */
 PFNWP    oldwindowproc[MAX_MACRO - MIN_MACRO + 1];
 INT      calcdisabled;                /* calc interlock flag        */
 TID      editthreadid;                /* TID of editor thread       */
 TID      calcthreadid;                /* TID of calculator thread   */
 PID      process;                     /* PID of our process         */
 HEV      calcsem;                     /* calculation semaphore      */
 INT      editted_program;             /* macro editted              */
 RXSTRING global_args[2];              /* passed argument strings    */
 INT      global_argc;                 /* argument count             */
 PRXSTRING global_result;              /* returned result string     */
 UINT     global_completion;           /* completion message         */
 UINT     global_macro_name;           /* macro to invoke            */
 HWND     global_handle;               /* handle for completion msg  */
 HWND     hwndIOWindow;                /* handle of I/O window       */
 HWND     hwndIOClient;                /* handle of I/O client area  */
 BOOL     tracing = FALSE;             /* macro trace flag           */

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
    "PROGRAM2.CAL",                    /* second program key         */
    "PROGRAM3.CAL",                    /* third program key          */
    "PROGRAM4.CAL",                    /* fourth program key         */
    "PROGRAM5.CAL",                    /* fifth program key          */
    "PROGRAM6.CAL",                    /* sixth program key          */
    "PROGRAM7.CAL",                    /* seventh program key        */
    "PROGRAM8.CAL",                    /* eighth program key         */
    "MEMPLUS.CAL",                     /* memory plus key            */
    "MEMMINUS.CAL",                    /* memory minus key           */
    "MEMSTORE.CAL",                    /* memory recall key          */
    "MEMRECAL.CAL",                    /* memory store key           */
    "MEMCLEAR.CAL",                    /* memory clear key           */
    "DATATYPE.CAL"};                   /* data validation program    */

                                       /* loaded program sources     */
INSTORE    operator_programs[MACRO_COUNT];

/* exit_list is a list of the system exits PMREXX register's with REXX        */

static RXSYSEXIT exit_list[] =  {
    { PMRXIO_EXIT,   RXSIO },
    { "PMREXXINIT",  RXINI },
    { NULL,          RXENDLST }};

/*********************************************************************/
/* main() - the main calculator program entry point                  */
/*********************************************************************/

INT main()
{
  HMQ   hmq;                           /* Message queue handle       */
  QMSG  qmsg;                          /* Receive PM queue message   */
  PTIB  TI;                            /* Thread information block   */
  PPIB  PI;                            /* process information block  */

  DosGetInfoBlocks(&TI,&PI);           /* Get the thread information */
  process  = PI->pib_ulpid;            /* set the process id         */
                                       /* startup PM usage           */
  hab = WinInitialize(0);              /* Get the anchor block handle*/
  hmq = WinCreateMsgQueue(hab, 0);     /* Create the message queue   */
                                       /* create our I/O window      */
  hwndIOWindow = RexxCreateIOWindow(HWND_DESKTOP, &hwndIOClient);
                                       /* register I/O handler       */
  RexxRegisterExitExe("PMREXXINIT", (PFN)init_routine, NULL);
                                       /* register our window class  */
  WinRegisterClass(hab, "RexxCalc", Calculator, 0l, 0);

  /* Load the calculator dialog.  The dialog definitions are in      */
  /* rexxcalc.rc                                                     */

  hwndCalc = WinLoadDlg(HWND_DESKTOP, HWND_DESKTOP, NULL,
                        (HMODULE)NULL,
                        REXXCALC,      /* Dialog ID                  */
                        NULL);

  WinSendMsg(hwndCalc, WM_SETICON,     /* Set program icon           */
      (MPARAM)WinLoadPointer(HWND_DESKTOP, (HMODULE)NULL, REXXCALC), 
      NULL);
                                       /* set the window focus       */
  WinSetFocus(HWND_DESKTOP,
      WinWindowFromID(hwndCalc, (USHORT)FID_CLIENT));

  /* Process the standard Presentation Manager message loop until    */
  /* we are told to terminate                                        */

                                       /* While more messages        */
  while (WinGetMsg(hab, &qmsg, 0, 0, 0))
     WinDispatchMsg(hab, &qmsg);       /* dispatch application       */
                                       /*   message handler          */


  /* The close option has been selected so we need to clean up       */
  /* our context.                                                    */

  RexxDestroyIOWindow(hwndIOWindow);   /* Destroy the I/O window     */
                                       /* drop exit handler          */
  RexxDeregisterExit("PMREXXINIT", NULL);
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
  ULONG     action;                    /* Action to process          */
  INT       i;                         /* loop index                 */
  RXSTRING  memory_setting;            /* memory number              */

   switch (msg) {                      /* switch based on the message*/
                                       /* received                   */

      /* The initialization message has been received.  We do some   */
      /* additional fixup of the dialog to make it look a little     */
      /* nicer.                                                      */

      case WM_INITDLG:

         global_handle = hwnd;         /* remember handle for thread */
                                       /* register our external      */
                                       /* functions                  */
         RexxRegisterFunctionExe("CalcPrecision", (PFN)CalcPrecision);
         RexxRegisterFunctionExe("CalcForm", (PFN)CalcForm);
                                       /* register subcommand handler*/
         RexxRegisterSubcomExe("Calculator", (PFN)CalcCommand, NULL);
                                       /* read in the programs       */
         for (i = 0; i < MACRO_COUNT; i++)
           ReadSourceFile(operator_names[i],
               &operator_programs[i]);

                                       /* initialize the memory bank */
         for (i = 1; i <= NUMBER_MEMORIES; i++)
           memory_bank[i] = NULL;      /* all null pointers          */

                                       /* subclass programmable      */
                                       /* buttons so we can trap     */
                                       /* BUTTON2 events             */
         for (i = MIN_MACRO; i <= MAX_MACRO; i++)
           oldwindowproc[i - MIN_MACRO] =
               WinSubclassWindow(WinWindowFromID(hwnd, i),
               (PFNWP)Button);
                                       /* initialize accumulator     */
         MAKERXSTRING(accumulator, accbuff, 1);
         strcpy(accbuff, "0");         /* fill in a zero             */
         digits = 0;                   /* no digits in number        */
         hadperiod = NO;               /* no period yet              */
         hadexponent = NO;             /* no exponential yet         */
         oldaction = 0;                /* no pending operations      */
         memory_function = NO;         /* not waiting for mem number */
         strcpy(precision, "9");       /* set default precision      */
         form = SCIENTIFIC;            /* set default form           */
         calcdisabled = NO;            /* functions enabled          */
         SetDisplay(&accumulator);     /* set initial display value  */
                                       /* update the calculator      */
                                       /* display                    */
         WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
             display.strptr);
         calcsem = 0;                  /* zero semaphore handle      */
                                       /* create a semaphore         */
         DosCreateEventSem(NULL, &calcsem, FALSE, FALSE);
                                       /* create calculation thread  */
         DosCreateThread(&calcthreadid, (PFNTHREAD)CalcThread, 
             (ULONG)0, (ULONG)0, STACKSIZE);
         return FALSE;                 /* initialization complete    */

      /* We have finished editting a key macro.  Clean up the thread */
      /* and reenable the main calculator.                           */

      case WM_EDIT_DONE:

        calcdisabled = NO;             /* reenable the calculator    */
        return FALSE;                  /* resume normal operations   */

      case WM_MEMORY_DONE:             /* memory command completed   */

        calcdisabled = NO;             /* reenable the calculator    */
                                       /* display                    */
        WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
            display.strptr);
        return FALSE;                  /* resume normal operations   */

      case WM_OPERATOR_DONE:           /* math operator completed    */

        calcdisabled = NO;             /* reenable the calculator    */
                                       /* function return an error?  */
        if (!strcmp(accumulator.strptr,"Error"))
          oldaction = 0;               /* cancel next on error       */
                                       /* redisplay the accumlator   */
        SetDisplay(&accumulator);
                                       /* display                    */
        WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
            display.strptr);

                                       /* may need to process a      */
                                       /* a program key now          */
        switch (oldaction) {           /* now process then new key   */

                                       /* process function keys      */
                                       /* immediately                */
          case BUTTON_PROGRAM1:        /* program key one            */
          case BUTTON_PROGRAM2:        /* program key two            */
          case BUTTON_PROGRAM3:        /* program key three          */
          case BUTTON_PROGRAM4:        /* program key four           */
          case BUTTON_PROGRAM5:        /* program key five           */
          case BUTTON_PROGRAM6:        /* program key six            */
          case BUTTON_PROGRAM7:        /* program key seven          */
          case BUTTON_PROGRAM8:        /* program key eight          */

            oldaction = 0;             /* cancel next action         */
                                       /* call the appropriate       */
                                       /* Rexx operand processor     */
                                       /* Process the macro          */
            ProcessMacro(oldaction - MIN_MACRO, &accumulator, 
                &display, &accumulator, WM_PROGRAM_DONE);
            break;

        }
        return FALSE;                  /* resume normal operations   */

      case WM_PROGRAM_DONE:            /* program key completed      */

        calcdisabled = NO;             /* reenable the calculator    */
                                       /* redisplay the accumlator   */
        SetDisplay(&accumulator);
                                       /* display                    */
        WinSetWindowText(WinWindowFromID(hwnd, DISPLAY),
            display.strptr);
        return FALSE;                  /* resume normal operations   */

      /* We are going away, time to post a quit message to the       */
      /* message loop.                                               */

      case WM_CLOSE:

         if (calcdisabled)             /* currently editting?        */
                                       /* suspend the thread         */
           DosSuspendThread(editthreadid);
                                       /* free source programs       */
         for (i = 0; i < MACRO_COUNT; i++)
           FreeSourceFile(&operator_programs[i]);
                                       /* clear the memory bank      */
         for (i = 1; i <= NUMBER_MEMORIES; i++)
           if (memory_bank[i])         /* if has a value             */
             free(memory_bank[i]);     /* release it                 */

                                       /* Deregister our defined     */
                                       /* functions                  */
         RexxDeregisterFunction("CalcPrecision");
         RexxDeregisterFunction("CalcForm");
                                       /* Deregister command handler */
         RexxDeregisterSubcom("Calculator", NULL);

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

           case BUTTON_TRACE:          /* turn on trace              */

             tracing = ~tracing;       /* flip the flag              */
             if (tracing) {            /* trace on?                  */
               WinSetWindowText(WinWindowFromID(hwnd, BUTTON_TRACE),
                  "Trace On");
                                       /* turn on the trace          */
               RexxSetTrace(process, calcthreadid);
             }
             else {                    /* turn off the trace         */
               WinSetWindowText(WinWindowFromID(hwnd, BUTTON_TRACE),
                  "Trace Off");
               RexxResetTrace(process, calcthreadid);
             }

           default:                    /* Unknown, can't handle this */
             return FALSE;
         }

      /* We've received a WM_COMMAND message.  WM_COMMAND messages   */
      /* are generated by "pressing" buttons on the calculator.  The */
      /* additional parameters in the received message identify the  */
      /* button that was pressed                                     */

      case WM_COMMAND:

         if (calcdisabled) {           /* editting a function?       */
                                       /* give an error beep         */
           WinAlarm(HWND_DESKTOP, WA_ERROR);
           return FALSE;               /* done with message          */
         }

         action = SHORT1FROMMP(mp1);   /* Extract message sub-type   */
         if (memory_function) {        /* waiting for memory input?  */
           switch(action) {            /* process based on type      */

             case BUTTON_0:            /* Numeric keys are the only  */
             case BUTTON_1:            /* things allowed, except for */
             case BUTTON_2:            /* the clear key              */
             case BUTTON_3:
             case BUTTON_4:
             case BUTTON_5:
             case BUTTON_6:
             case BUTTON_7:
             case BUTTON_8:
             case BUTTON_9:

                                       /* store character            */
               membuff[sizeof(membuff) - (memory_function + 1)] =
                   (UCHAR)((action - BUTTON_0) + '0');
                                       /* got last digit?            */
               if (!(--memory_function)) {
                                       /* get memory number in an    */
                                       /* RXSTRING                   */
                 MAKERXSTRING(memory_setting, membuff,
                     sizeof(membuff) - 1);
                                       /* process the memory key     */
                 switch (memory_action) {

                                       /* memory recall              */
                   case BUTTON_MEMORY_RECALL:
                   case BUTTON_MEMORY_CLEAR:

                                       /* call the macro             */
                     ProcessMacro(memory_action - MIN_MACRO,
                         &memory_setting, NULL,
                         NULL, WM_MEMORY_DONE);
                     break;            /* finished                   */

                                       /* memory operation on display*/
                   case BUTTON_MEMORY_STORE:
                   case BUTTON_MEMORY_PLUS:
                   case BUTTON_MEMORY_MINUS:

                                       /* call the macro             */
                     ProcessMacro(memory_action - MIN_MACRO,
                         &memory_setting, &accumulator,
                         NULL, WM_MEMORY_DONE);
                     break;            /* finished                   */
                 }
               }
               return FALSE;           /* done with message          */

             case BUTTON_CLEAR:        /* Clear key                  */

               memory_function = 0;    /* disable memory function    */
               return FALSE;           /* done with message          */

             default:                  /* disabled key               */
                                       /* give an error beep         */
               WinAlarm(HWND_DESKTOP, WA_ERROR);
               return FALSE;           /* done with message          */
           }
         }

         switch (action) {

                                       /* memory operation on display*/
            case BUTTON_MEMORY_STORE:  /* store to memory            */
            case BUTTON_MEMORY_PLUS:   /* add to memory              */
            case BUTTON_MEMORY_MINUS:  /* subtract from memory       */
            case BUTTON_MEMORY_RECALL: /* recall memory to display   */
            case BUTTON_MEMORY_CLEAR:  /* clear a memory             */

                                       /* copy display into accum    */
              strcpy(accumulator.strptr, display.strptr);
                                       /* copy the length also       */
              accumulator.strlength = display.strlength;
              digits = 0;              /* no digits in display       */
              memory_action = action;  /* remember the key           */
              memory_function = 2;     /* need two digits from pad   */
              return FALSE;            /* wait for key info          */

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
              if (!Datatype(display.strptr, "Whole"))
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
            case BUTTON_PROGRAM3:      /* programmed Rexx function   */
            case BUTTON_PROGRAM4:      /* programmed Rexx function   */
            case BUTTON_PROGRAM5:      /* programmed Rexx function   */
            case BUTTON_PROGRAM6:      /* programmed Rexx function   */
            case BUTTON_PROGRAM7:      /* programmed Rexx function   */
            case BUTTON_PROGRAM8:      /* programmed Rexx function   */

                                       /* Process pending operations */
              global_handle = hwnd;    /* save window handle         */
              ProcessArithmetic(action);
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
/* Button() -     the PM WinProc for handling actions on             */
/* programmable buttons.  This only intercepts the WM_BUTTON2DOWN    */
/* events for programmable keys.                                     */
/*                                                                   */
/*********************************************************************/

MRESULT EXPENTRY Button(
  HWND    hwnd,                        /* window handle              */
  ULONG   msg,                         /* dispatched message id      */
  MPARAM  mp1,                         /* first message parameter    */
  MPARAM  mp2 )                        /* second message parameter   */
{
  LONG    id;                          /* id of pressed button       */

                                       /* get the button id          */
  id = WinQueryWindowUShort(hwnd, QWS_ID);

  switch(msg) {                        /* process different messages */

                                       /* Button 2 pressed on a      */
    case WM_BUTTON2DOWN:               /* programmed key.            */

      if (calcdisabled) {              /* already editting?          */
                                       /* give an error beep         */
        WinAlarm(HWND_DESKTOP, WA_ERROR);
        return FALSE;                  /* done with message          */
      }
                                       /* save the program           */
      editted_program = id - MIN_MACRO;
      calcdisabled = YES;              /* disable key functions      */
                                       /* spin off the thread        */
                                       /* spin off the thread        */
      DosCreateThread(&editthreadid, (PFNTHREAD)EditThread, 
         (ULONG)0, (ULONG)0, STACKSIZE);
      return FALSE;                    /* wait for completion msg    */

      /* Pass all other events on to the main handlers.              */

    default:
      return (*oldwindowproc[id - MIN_MACRO])(hwnd, msg, mp1, mp2);
  }

}
/*********************************************************************/
/*                                                                   */
/* AddDigit(digit) - add a digit to the calculator display           */
/*                                                                   */
/*********************************************************************/

void AddDigit(
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

void SetDisplay(
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

void ProcessArithmetic(
  LONG  newaction )                    /* new operator key pressed   */
{
   LONG    action;                     /* action to take             */

   action = oldaction;                 /* remember old actiion       */

   switch(oldaction) {                 /* process prior action key   */

     case BUTTON_MULTIPLY:             /* multiplication             */
     case BUTTON_DIVIDE:               /* division                   */
     case BUTTON_PLUS:                 /* addition                   */
     case BUTTON_MINUS:                /* subtraction                */
     case BUTTON_IDIV:                 /* integer division           */
     case BUTTON_REMAINDER:            /* remainder division         */
     case BUTTON_POWER:                /* raise a number to power    */

       oldaction = newaction;          /* remember the new action    */
                                       /* call the appropriate       */
                                       /* Rexx operand processor     */
       ProcessMacro(action - MIN_MACRO, &accumulator, &display, 
           &accumulator, WM_OPERATOR_DONE);
       break;

     default:

                                       /* copy display into accum    */
       strcpy(accumulator.strptr, display.strptr);
                                       /* copy the length also       */
       accumulator.strlength = display.strlength;
       digits = 0;                     /* no digits in display       */

       switch (newaction) {            /* now process the new key    */

                                       /* process function keys      */
                                       /* immediately                */
         case BUTTON_PROGRAM1:         /* program key one            */
         case BUTTON_PROGRAM2:         /* program key two            */
         case BUTTON_PROGRAM3:         /* program key three          */
         case BUTTON_PROGRAM4:         /* program key four           */
         case BUTTON_PROGRAM5:         /* program key five           */
         case BUTTON_PROGRAM6:         /* program key six            */
         case BUTTON_PROGRAM7:         /* program key seven          */
         case BUTTON_PROGRAM8:         /* program key eight          */

           oldaction = 0;              /* cancel next action         */
                                       /* call the appropriate       */
                                       /* Rexx operand processor     */
           SetDisplay(&accumulator);   /* update display             */
           ProcessMacro(newaction - MIN_MACRO, &accumulator, &display, 
               &accumulator, WM_PROGRAM_DONE);
           break;

         default:                      /* all other operations       */
           oldaction = newaction;      /* remember the new action    */
       }
       break;                          /* no more needed             */

   }

}

/*********************************************************************/
/*                                                                   */
/* ProcessProgram()  - Process a call to a programed Rexx function.  */
/*                     The program is passed both the accumulator    */
/*                     and the current display as strings.           */
/*                                                                   */
/*********************************************************************/

void ProcessProgram(
  ULONG      macro_name,               /* macro to invoke            */
  PRXSTRING  left,                     /* left operand               */
  PRXSTRING  right,                    /* right operand              */
  PRXSTRING  result )                  /* result location            */
{
  RXSTRING   args[2];                  /* passed argument strings    */
  INT        argc;                     /* argument count             */
  INT        rc;                       /* Rexx return code           */
  SHORT      return_code;              /* function return code       */


  args[0] = *left;                     /* set first argument         */
  if (right) {                         /* two arguments provided?    */
    args[1] = *right;                  /* copy RXSTRING              */
    argc = 2;                          /* two arguments              */
  }
  else
    argc = 1;                          /* only one argument          */

  rc = RexxStart(argc,                 /* two arguments              */
                 args,                 /* array of arguments         */
                                       /* name of the macro          */
                 operator_names[macro_name],
                                       /* instorage program          */
                 (PRXSTRING)&operator_programs[macro_name],
                 "Calculator",         /* default command address    */
                 RXFUNCTION,           /* calling this a function    */
                 NULL,
                 &return_code,         /* numeric value of return str*/
                 result);              /* returned string result     */
   if (result)                         /* if result requested        */
                                       /* make into an ASCII-Z string*/
     result->strptr[result->strlength] = '\0';
}

/*********************************************************************/
/*                                                                   */
/* ProcessMacro()    - Process a call to a programed Rexx function.  */
/*                     The program is passed both the accumulator    */
/*                     and the current display as strings and        */
/*                     executed on a separate thread.                */
/*                                                                   */
/*********************************************************************/

void ProcessMacro(
  ULONG      macro_name,               /* macro to invoke            */
  PRXSTRING  left,                     /* left operand               */
  PRXSTRING  right,                    /* right operand              */
  PRXSTRING  result,                   /* result location            */
  ULONG      completion )              /* completion message         */
{
  global_args[0] = *left;              /* set first argument         */
  if (right) {                         /* two arguments provided?    */
    global_args[1] = *right;           /* copy RXSTRING              */
    global_argc = 2;                   /* two arguments              */
  }
  else
    global_argc = 1;                   /* only one argument          */
  global_macro_name = macro_name;      /* save macro name            */
  global_result = result;              /* save result pointer        */
  global_completion = completion;      /* save completion message    */
  DosPostEventSem(calcsem);            /* kick off the thread        */

}

/*********************************************************************/
/*                                                                   */
/* Datatype()        - Call Rexx to validate a character string      */
/*                                                                   */
/*********************************************************************/

INT Datatype(
  PSZ        string,                   /* string to validate         */
  PSZ        operation )               /* datatype operation         */
{
  RXSTRING   args[2];                  /* passed argument strings    */
  RXSTRING   result;                   /* returned result string     */
  UCHAR      resbuf[10];               /* result buffer              */


                                       /* set first argment          */
  MAKERXSTRING(args[0], string, strlen(string));
                                       /* and second argument        */
  MAKERXSTRING(args[1], operation, strlen(operation));
  result.strptr = resbuf;              /* point to return point      */
  result.strlength = sizeof(resbuf);   /* set maximum size           */

                                       /* invoke the Rexx program    */
                                       /* DATATYPE is last one in    */
                                       /* the list                   */
  ProcessProgram(MACRO_COUNT - 1, &args[0], &args[1], &result);

   if (resbuf[0] == '1')               /* whole number?              */
     return YES;                       /* yes, this is good          */
   else
     return NO;                        /* nope, bad number           */
}

/*********************************************************************/
/*                                                                   */
/* ReadSourceFile()  - Read Rexx program source off of disk.         */
/*                                                                   */
/*********************************************************************/

void ReadSourceFile(
  PSZ        program,                  /* program to read            */
  PINSTORE   descriptor )              /* saved location             */
{
  HFILE      handle;                   /* file handle                */
  FILESTATUS status;                   /* queried file status        */
  ULONG      size;                     /* program size               */
  ULONG      action;                   /* DosOpen action             */
  ULONG      bytes_read;               /* bytes read with DosRead    */

                                       /* zero out image holder      */
  MAKERXSTRING(descriptor->image, NULL, 0);
  DosOpen(program,                     /* open the file              */
      &handle, &action, 0, 0, OPEN_ACTION_OPEN_IF_EXISTS,
      OPEN_ACCESS_READONLY + OPEN_SHARE_DENYREADWRITE +
      OPEN_FLAGS_FAIL_ON_ERROR + OPEN_FLAGS_WRITE_THROUGH,NULL);
                                       /* retrieve the file size     */
  DosQueryFileInfo(handle, 1, (PBYTE)&status, sizeof(FILESTATUS));
  size = status.cbFile;                /* get the file size          */
                                       /* allocate buffer for source */
  descriptor->source.strptr = malloc(size);
  descriptor->source.strlength = size; /* set the size               */
                                       /* read in the program        */
  DosRead(handle, descriptor->source.strptr, size, &bytes_read);
  DosClose(handle);                    /* close the file             */
}

/*********************************************************************/
/*                                                                   */
/* FreeSourceFile()  - Release storage for Rexx source program.      */
/*                                                                   */
/*********************************************************************/

void FreeSourceFile(
  PINSTORE   descriptor )              /* saved location             */
{
  if (descriptor->source.strptr)       /* if have a source           */
                                       /* release it                 */
    free(descriptor->source.strptr);
  if (descriptor->image.strptr)        /* if we have an image        */
                                       /* release it also            */
    DosFreeMem(descriptor->image.strptr);

                                       /* clear out both descriptors */
  MAKERXSTRING(descriptor->source, NULL, 0);
  MAKERXSTRING(descriptor->image, NULL, 0);
}

/*********************************************************************/
/*                                                                   */
/* SetRexxVariable - Set the value of a Rexx variable                */
/*                                                                   */
/*********************************************************************/

INT SetRexxVariable(
  PSZ        name,                     /* Rexx variable to set       */
  PSZ        value )                   /* value to assign            */
{
  SHVBLOCK   block;                    /* variable pool control block*/

  block.shvcode = RXSHV_SYSET;         /* do a symbolic set operation*/
  block.shvret=(UCHAR)0;               /* clear return code field    */
  block.shvnext=(PSHVBLOCK)0;          /* no next block              */
                                       /* set variable name string   */
  MAKERXSTRING(block.shvname, name, strlen(name));
                                       /* set value string           */
  MAKERXSTRING(block.shvvalue, value, strlen(value));
  block.shvnamelen=strlen(name);       /* set name length            */
  block.shvvaluelen=strlen(value);     /* set value length           */
  return RexxVariablePool(&block);     /* set the variable           */
}

/*********************************************************************/
/*                                                                   */
/* CalcPrecision - External Rexx function for obtaining current      */
/*                 calculator numeric precision                      */
/*                                                                   */
/*********************************************************************/


LONG   EXPENTRY CalcPrecision(
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

LONG   EXPENTRY CalcForm(
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

/*********************************************************************/
/*                                                                   */
/* CalcCommand   - Subcommand handler for the calculator             */
/*                                                                   */
/*********************************************************************/


APIRET EXPENTRY CalcCommand(
  PRXSTRING    command,                /* command to issue           */
  PUSHORT      flags,                  /* error/failure flags        */
  PRXSTRING    retstr )                /* return code                */
{
  PSZ          scan;                   /* scan position              */
  INT          memory_number;          /* number of memory value     */
  PSZ          memory_item;            /* returned memory item       */
  RXSTRING     newdisplay;             /* new display value          */

                                       /* leading blanks             */
  scan = command->strptr + strspn(command->strptr, " ");

                                       /* set operation?             */
  if (!memicmp(scan, "SET ", strlen("SET "))) {
    scan += strlen("SET ");            /* step past command          */
    scan += strspn(scan, " ");         /* skip to next non-blank     */
    memory_number = atol(scan);        /* get memory number          */
    if (memory_number <= 0 ||          /* validate the value         */
        memory_number > NUMBER_MEMORIES) {
      *flags = RXSUBCOM_ERROR;         /* raise error condition      */
      strcpy(retstr->strptr, "2");     /* give return code of 2      */
      retstr->strlength = 1;           /* set proper length          */
      return 0;                        /* finished                   */
    }
    scan = strchr(scan, ' ');          /* step past number           */
    if (!scan) {                       /* no value?                  */
      *flags = RXSUBCOM_ERROR;         /* raise error condition      */
      strcpy(retstr->strptr, "3");     /* give return code of 3      */
      retstr->strlength = 1;           /* set proper length          */
      return 0;                        /* finished                   */
    }
    scan += strspn(scan, " ");         /* skip to next non-blank     */
    if (!Datatype(scan, "Number")) {   /* if not a valid number      */
      *flags = RXSUBCOM_ERROR;         /* raise error condition      */
      strcpy(retstr->strptr, "4");     /* give return code of 4      */
      retstr->strlength = 1;           /* set proper length          */
      return 0;                        /* finished                   */
    }

    if (memory_bank[memory_number])    /* have a value already?      */
      free(memory_bank[memory_number]);/* release the value          */
                                       /* set the new value          */
    memory_bank[memory_number] = strdup(scan);
  }
                                       /* get operation?             */
  else if (!memicmp(scan, "GET ", strlen("GET"))) {
    scan += strlen("GET ");            /* step past command          */
    scan += strspn(scan, " ");         /* skip to next non-blank     */
    memory_number = atol(scan);        /* get memory number          */
    if (memory_number <= 0 ||          /* validate the value         */
        memory_number > NUMBER_MEMORIES) {
      *flags = RXSUBCOM_ERROR;         /* raise error condition      */
      strcpy(retstr->strptr, "2");     /* give return code of 2      */
      retstr->strlength = 1;           /* set proper length          */
      return 0;                        /* finished                   */
    }
    scan = strchr(scan, ' ');          /* step past number           */
    if (!scan) {                       /* no value?                  */
      *flags = RXSUBCOM_ERROR;         /* raise error condition      */
      strcpy(retstr->strptr, "3");     /* give return code of 3      */
      retstr->strlength = 1;           /* set proper length          */
      return 0;                        /* finished                   */
    }
    scan += strspn(scan, " ");         /* skip to next non-blank     */
                                       /* get memory contents        */
    memory_item = memory_bank[memory_number];
    if (!memory_item)                  /* empty memory register?     */
      memory_item = "0";               /* just use a zero            */
                                       /* set the variable ok?       */
    if (SetRexxVariable(scan, memory_item) > RXSHV_NEWV) {
      *flags = RXSUBCOM_ERROR;         /* raise error condition      */
      strcpy(retstr->strptr, "5");     /* give return code of 5      */
      retstr->strlength = 1;           /* set proper length          */
      return 0;                        /* finished                   */
    }
  }
                                       /* get operation?             */
  else if (!memicmp(scan, "DISPLAY ", strlen("DISPLAY"))) {
    scan += strlen("DISPLAY");         /* step past command          */
    scan += strspn(scan, " ");         /* skip to next non-blank     */
                                       /* make an RXSTRING           */
    MAKERXSTRING(newdisplay, scan, strlen(scan));
    SetDisplay(&newdisplay);           /* set the new display        */
  }
  else {                               /* unknown command            */
    *flags = RXSUBCOM_FAILURE;         /* raise failure condition    */
    strcpy(retstr->strptr, "1");       /* give return code of 1      */
    retstr->strlength = 1;             /* set proper length          */
    return 0;                          /* finished                   */
  }
                                       /* successful completion      */
  strcpy(retstr->strptr, "0");         /* give return code of 0      */
  retstr->strlength = 1;               /* set proper length          */
  return 0;                            /* finished                   */
}


/*********************************************************************/
/*                                                                   */
/* EditThread    - Edit a macro bound to a key                       */
/*                                                                   */
/*********************************************************************/

void EditThread(void)
{
  RESULTCODES   returncodes;           /* result of EDITing          */
  UCHAR         loaderror[150];        /* object name buffer         */
  UCHAR         args[150];             /* argument string            */
  PSZ           argptr;                /* used to build arg string   */

  strcpy(args, "E");                   /* fill in command name       */
  argptr = args + sizeof("E");         /* step past name             */
                                       /* copy in file name          */
  strcpy(argptr, (const char *)operator_names[editted_program]);
                                       /* terminate arguments        */
  argptr[strlen((const char *)operator_names[editted_program]) + 1] = 0;
                                       /* invoke the system editor   */
  DosExecPgm(loaderror, sizeof(loaderror),
      EXEC_SYNC, args, NULL,
      &returncodes, "E.EXE");
                                       /* release existing source    */
  FreeSourceFile(&operator_programs[editted_program]);
                                       /* read in new source         */
  ReadSourceFile(operator_names[editted_program],
      &operator_programs[editted_program]);

  DosEnterCritSec();                   /* allow thread to complete   */
                                       /* send completion message    */
  WinPostMsg(hwndCalc, WM_EDIT_DONE, NULL, NULL);
  DosExit(EXIT_THREAD, 0);             /* end the thread             */                                                                                                                                                                              

}

/*********************************************************************/
/*                                                                   */
/* EditThread    - Execute a macro bound to a key                    */
/*                                                                   */
/*********************************************************************/


void CalcThread(void)
{
  LONG      rc;                        /* rexx return code           */
  SHORT     return_code;               /* numeric return code        */
  ULONG     postcount;                 /* semaphore post count       */
  RXSTRING  result;                    /* result rxstring            */
  PRXSTRING resultptr;                 /* passed result pointer      */

  while (TRUE) {                       /* Do Forever                 */
    DosWaitEventSem(calcsem, -1);      /* wait for work to do        */
    DosResetEventSem(calcsem,          /* clear posted event         */
        &postcount);
                                       /* enable I/O system          */
    while (!WinPostMsg(hwndIOClient, REXX_STARTPROC,
        MPFROMLONG(calcthreadid), (MPARAM)0));
      DosSleep(0L);
    if (global_result) {               /* if result needed           */
      result = *global_result;         /* make temp copy             */
      result.strlength = DISPLAY_WIDTH;/* set result size            */
      resultptr = &result;             /* set result pointer         */
    }
    else
      resultptr = NULL;                /* no result                  */

    rc = RexxStart(global_argc,        /* two arguments              */
                   global_args,        /* array of arguments         */
                                       /* name of the macro          */
                   operator_names[global_macro_name],
                                       /* instorage program          */
                   (PRXSTRING)&operator_programs[global_macro_name],
                   "Calculator",       /* default command address    */
                   RXFUNCTION,         /* calling this a function    */
                   exit_list,          /* exits to use               */
                   &return_code,       /* numeric value of return str*/
                   resultptr);         /* returned string result     */
                                       /* disable I/O system         */
    while (!WinPostMsg(hwndIOClient, REXX_ENDPROC,
        (MPARAM)0, (MPARAM)0))
      DosSleep(0L);
    if (global_result) {               /* if result requested        */
                                       /* make into an ASCII-Z string*/
      global_result->strptr[result.strlength] = '\0';
                                       /* set returned length        */
      global_result->strlength = result.strlength;
    }
                                       /* signal completion          */
    WinPostMsg(hwndCalc, global_completion, NULL, NULL);
  }
  DosExit(EXIT_THREAD, 0);             /* end the thread             */                                                                                                                                                                              
}

/*********************************************************************/
/*                                                                   */
/* init_routine  - initialization exit for Rexx macros               */
/*                                                                   */
/*********************************************************************/

LONG  EXPENTRY init_routine(
  LONG   exitno,                       /* major exit code            */
  LONG   subfunc,                      /* exit subfunction           */
  PUCHAR parmblock)                    /* parameters                 */
{
  if (tracing)                         /* tracing at start?          */
                                       /* turn it on now             */
    RexxSetTrace(process, calcthreadid);
  return 0;                            /* return with no errors      */
}
