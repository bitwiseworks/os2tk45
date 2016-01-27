/*==============================================================*\
 *
 *  Main.h - Sample Pipe application main header file
 *      Copyright 1990 IBM Corp.
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
#define IDD_ABOUTBOX    1002


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
#define IDM_OPTIONCLEAR                     3100


/*--------------------------------------------------------------*\
 *  Dialog item ids
\*--------------------------------------------------------------*/
#define ID_OK               1
#define ID_CANCEL           2

#define IDC_OK              1
#define IDC_ICON            4

/*--------------------------------------------------------------*\
 *  Stringtable ids
\*--------------------------------------------------------------*/
#define IDS_APPNAME                 1
#define IDS_HELPLIBRARYNAME         2
#define IDS_HELPWINDOWTITLE         3
#define IDS_TITLEBARSEPARATOR       4

/*--------------------------------------------------------------*\
 *  Messagetable ids
\*--------------------------------------------------------------*/
#define IDMSG_INITFAILED                1
#define IDMSG_MAINWINCREATEFAILED       2
#define IDMSG_HELPLOADERROR             3
#define IDMSG_CANNOTLOADSTRING          4
#define IDMSG_HELPDISPLAYERROR          5
#define IDMSG_CREATE_PIPE_FAILED        7
#define IDMSG_CREATE_SEM_FAILED         8
#define IDMSG_CREATE_THREAD_FAILED      9
#define IDMSG_PIPE_WRITE_FAILED        10
#define IDMSG_PIPE_READ_FAILED         11
#define IDMSG_SEMAPHORE_ERROR          12
#define IDMSG_START_PIPE_ERROR         13
#define IDMSG_CONNECT_FAILED           14
#define IDMSG_GAME_EXITTING            15
#define IDMSG_GAME_PIPE_CONNECTED      16
#define IDMSG_CANNOT_EXEC_CLIENT       17

/*--------------------------------------------------------------*\
 *  Ids of messages used between threads (non-pipe related)
\*--------------------------------------------------------------*/

#define WM_MSG                  WM_USER+0  /* popup message */
#define WM_PIPEMSG              WM_USER+1  /* client window message */

/*--------------------------------------------------------------*\
 *  Other constants
\*--------------------------------------------------------------*/
#define TITLE_LEN           50
#define TITLESEPARATORLEN    4
#define MAX_MESSAGES       255
#define MESSAGE_LEN         64

/* return values for initialization routines */
#define RETURN_SUCCESS      0   /* successful return in DosExit */
#define RETURN_ERROR        1   /* error return in DosExit */
#define BEEP_WARN_FREQ      60  /* frequency of warning beep */
#define BEEP_WARN_DUR      100  /* duration of warning beep */

#ifndef max
#define max(a,b) (((a) > (b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a) < (b))?(a):(b))
#endif
