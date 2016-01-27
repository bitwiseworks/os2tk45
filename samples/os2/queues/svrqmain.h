/*==============================================================*\
 *
 *  Main.h - Sample PM application main header file
 *      Copyright 1991 IBM Corp.
 *
 *--------------------------------------------------------------
 *
 *  This header file contains the application wide constants
 *  and structure definitions.
 *
\*==============================================================*/

#define IDR_MAIN       1      /* resource identifier */

/*--------------------------------------------------------------*\
 *  Standard Dialog box ids
\*--------------------------------------------------------------*/

#define MSGBOXID        1001


/*--------------------------------------------------------------*\
 *  Menu item ids
\*--------------------------------------------------------------*/
#define IDM_FILE                            1000
#define IDM_FILEEXIT                        1800

#define IDM_HELP                            9000
#define IDM_HELPHELPFORHELP                 9100
#define IDM_HELPEXTENDED                    9200
#define IDM_HELPKEYS                        9300
#define IDM_HELPINDEX                       9400
#define IDM_HELPTUTORIAL                    9500
#define IDM_HELPABOUT                       9600

#define IDM_OPTION                          3000
#define IDM_OPTIONPURGE                     3100


/*--------------------------------------------------------------*\
 *  Dialog item ids
\*--------------------------------------------------------------*/
#define ID_OK               1
#define ID_CANCEL           2

/*--------------------------------------------------------------*\
 *  Stringtable ids
\*--------------------------------------------------------------*/
#define IDS_APPNAME                 2
#define IDS_HELPLIBRARYNAME         3
#define IDS_HELPWINDOWTITLE         6
#define IDS_TITLEBARSEPARATOR      10

/*--------------------------------------------------------------*\
 *  Messagetable ids
\*--------------------------------------------------------------*/
#define IDMSG_INITFAILED                1
#define IDMSG_MAINWINCREATEFAILED       2
#define IDMSG_HELPLOADERROR            10
#define IDMSG_CANNOTLOADSTRING         11
#define IDMSG_HELPDISPLAYERROR         13
#define IDMSG_MEM_ALLOC_FAILED         25
#define IDMSG_CREATE_Q_FAILED          26
#define IDMSG_CREATE_SEM_FAILED        27
#define IDMSG_CREATE_THREAD_FAILED     28
#define IDMSG_CLIENTS_HUNG             29
#define IDMSG_Q_SEMAPHORE_ERROR        30
#define IDMSG_READ_Q_ERROR             31
#define IDMSG_CANNOT_EXEC_CLIENT       17

/*--------------------------------------------------------------*\
 *  Ids of messages used between threads
\*--------------------------------------------------------------*/
#define WM_NEWQMSG                          WM_USER+0
#define WM_MSG                              WM_USER+1


/*--------------------------------------------------------------*\
 *  Other constants
\*--------------------------------------------------------------*/
#define TITLE_LEN           50
#define TITLESEPARATORLEN   4
#define MESSAGE_LEN  256          /* maximum length for messages */
#define MAX_MESSAGES 250

/* return values for initialization routines */
#define RETURN_SUCCESS      0   /* successful return in DosExit */
#define RETURN_ERROR        1   /* error return in DosExit */
#define BEEP_WARN_FREQ      60  /* frequency of warning beep */
#define BEEP_WARN_DUR      100  /* duration of warning beep */

/* data types for passing queue data between threads */

#ifdef INCL_DOSQUEUES
struct _THREADPARAM {
   PID  pidParent;
   HWND hwndParent;
   };

typedef struct _THREADPARAM THREADPARAM;

struct _QDATA {
   CHAR   pszMsg[MESSAGE_LEN];
   PID    pidProcess;
   ULONG  usPriority;
   };

typedef struct _QDATA QDATA;

#endif

#ifndef max
#define max(a,b) (((a) > (b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a) < (b))?(a):(b))
#endif
