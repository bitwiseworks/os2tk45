/*==============================================================*\
 *                                                              *
 *  cliqxtrn.h - external definitions                           *
 *      Copyright 1991, IBM Corp.                               *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This header file contains the application wide function     *
 *  declarations and global external variables.                 *
 *                                                              *
\*==============================================================*/


/*--------------------------------------------------------------*\
 *  Entry point declarations                                    *
\*--------------------------------------------------------------*/

/* from main.c */
INT              main(VOID);
MRESULT EXPENTRY MainWndProc(HWND, ULONG, MPARAM, MPARAM);
ULONG            MessageBox(HWND, ULONG, LONG, ULONG, BOOL);
VOID             MainCommand(MPARAM, MPARAM);
BOOL             Init(VOID);
VOID             FileExit(VOID);

/* from thrd.c */
#ifdef INCL_DOSQUEUES

 struct _THREADPARAM {
    HEV     hevSend;
    PVOID   *ppvdMessage;
    PUSHORT pusPriority;
    PULONG  pulMsgSize;
    PULONG  pulMsgType;
    };

 typedef struct _THREADPARAM THREADPARAM;

 BOOL fSetUpMessage(THREADPARAM, PVOID *);
 BOOL InitQThrd(HEV, PVOID *, PUSHORT, PULONG, PULONG);


#endif

VOID _Optlink PostQMessage(PVOID);
