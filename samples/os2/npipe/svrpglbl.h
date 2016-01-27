/*
 *  Svrpglbl.h - variables global across modules.
 *     Copyright 1991 IBM Corp.
 */

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/

extern HWND hwndMainFrame;            /* handle to the main frame window */
extern HWND hwndMain;                 /* handle to the main client window */
extern HAB  hab;                      /* anchor block for the process */
extern BOOL fHelpEnabled;             /* flag to determine if help is enabled */
extern BOOL fAppExit;
extern BOOL fThrdsDead;               /* notification of thread exitting */
