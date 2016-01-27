/*
 *   Clnpglbl.h - Variables global to the application.
 *      Copyright 1991 IBM Corp.
 */

/*--------------------------------------------------------------*\
 *  Global variables                                            *
\*--------------------------------------------------------------*/

extern HWND hwndMainFrame;            /* handle to the main frame window */
extern HWND hwndMain;                 /* handle to the main client window */
extern HAB  hab;                      /* anchor block for the process */
extern BOOL fHelpEnabled;             /* flag to determine if help is enabled */
extern BOOL fAppExit;                 /* flag to notify of application termination */
extern BOOL fThrdsDead;               /* notification of thread exitting */
#ifdef INCL_DOSSEMAPHORES
extern HEV    hevWritePipe;           /* handle to event semaphore */
#endif
#ifdef SERVER
extern PIPE_MSG  pmsgPipeMsg;         /* pipe message buffer */
#endif
