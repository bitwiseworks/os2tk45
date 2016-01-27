/*==============================================================*\
 *                                                              *
 *  Clnpxtrn.h - External declarations                          *
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
INT              main(VOID);
MRESULT EXPENTRY MainWndProc(HWND, ULONG, MPARAM, MPARAM);
ULONG            MessageBox(HWND, ULONG, LONG, ULONG, PSZ, BOOL);
VOID             MainCommand(MPARAM mp1, MPARAM mp2);
BOOL             Init(VOID);
VOID             FileExit(VOID);
MRESULT EXPENTRY ChildWndProc (HWND, ULONG, MPARAM, MPARAM);
VOID             DrawPiece(HPS, USHORT, RECTL);
VOID             PostPipeMsg(USHORT, USHORT);
VOID             InitBoard(VOID);

/* from thrd.c */
BOOL  InitThread(VOID);
MRESULT EXPENTRY ServerDlgProc(HWND, ULONG, MPARAM, MPARAM);
#ifdef INCL_DOSNMPIPES
VOID _Optlink ManagePipe(PVOID);
HPIPE ConnectGamePipe(HPIPE, PSZ);
BOOL  WritePipe(HPIPE, USHORT *);
BOOL  ReadPipe(HPIPE, USHORT *);
#endif

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
