/*==============================================================*\
 *                                                              *
 *  Svrqxtrn.h - External declarations                          *
 *      Copyright 1991, IBM Corp.                               *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This header file contains the application wide function     *
 *  declarations.                                               *
 *                                                              *
\*==============================================================*/


/*--------------------------------------------------------------*\
 *  Entry point declarations                                    *
\*--------------------------------------------------------------*/

/* from main.c */
INT              main(int, char *[]);
MRESULT EXPENTRY MainWndProc(HWND, ULONG, MPARAM, MPARAM);
ULONG            MessageBox(HWND, ULONG, ULONG, BOOL);
VOID             MainCommand(MPARAM mp1, MPARAM mp2);
BOOL             Init(int, char *[]);
VOID             FileExit(VOID);

/* from pnt.c */
VOID    MainPaint(HWND, CHAR [MAX_MESSAGES][MESSAGE_LEN+38], unsigned int, SHORT);
MRESULT InitMainWindow(HWND, MPARAM, MPARAM);
MRESULT MainSize(HWND, USHORT, MPARAM, MPARAM);
MRESULT MainHorizScroll(HWND, MPARAM);
MRESULT MainVertScroll(HWND, MPARAM);
MRESULT MainCharHScroll(HWND, ULONG, MPARAM, MPARAM);
MRESULT MainCharVScroll(HWND, ULONG, MPARAM, MPARAM);
VOID    MainUpdateMsg(HWND, CHAR [MAX_MESSAGES][MESSAGE_LEN+38], unsigned int, SHORT);
VOID    MainPurgeWindow(VOID);

/* from help.c */
VOID  InitHelp(VOID);
VOID  DestroyHelpInstance(VOID);
VOID  DisplayHelpPanel(SHORT sIdPanel);
VOID  HelpAbout(MPARAM mp2);
VOID  HelpTutorial(MPARAM mp2);
VOID  HelpIndex(MPARAM mp2);
VOID  HelpKeys (MPARAM mp2);
VOID  HelpExtended(MPARAM mp2);
VOID  HelpHelpForHelp(MPARAM mp2);

#ifdef INCL_DOSQUEUES
 /* from thrd.c */
 HQUEUE InitQ(PID, HWND);
 VOID _Optlink ReadQMessage(PVOID);
#endif
