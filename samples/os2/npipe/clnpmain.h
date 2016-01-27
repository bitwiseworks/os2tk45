/*==============================================================*\
 *
 *  Clnpmain.h - Sample pipe application main header file
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
#define IDD_ABOUTBOX    1002
#define IDD_SERVER      1003


/*--------------------------------------------------------------*\
 *  Menu item ids
\*--------------------------------------------------------------*/
#define IDM_FILE                            1000
#define IDM_FILESTART                       1100
#define IDM_FILEEXIT                        1800

#define IDM_HELP                            9000
#define IDM_HELPHELPFORHELP                 9100
#define IDM_HELPEXTENDED                    9200
#define IDM_HELPKEYS                        9300
#define IDM_HELPINDEX                       9400
#define IDM_HELPTUTORIAL                    9500
#define IDM_HELPABOUT                       9600

#define IDM_OPTION                          3000
#define IDM_OPTIONSWAPPIECES                3100


/*--------------------------------------------------------------*\
 *  Dialog item ids
\*--------------------------------------------------------------*/
#define ID_OK               1
#define ID_CANCEL           2

#define IDC_OK              1
#define DID_PIPE_SVR        3
#define IDC_ICON            4
#define DID_TEXT            5

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
#define IDMSG_CLIENT_WIN                6
#define IDMSG_SERVER_WIN                7
#define IDMSG_CREATE_SEM_FAILED         8
#define IDMSG_CREATE_THREAD_FAILED      9
#define IDMSG_PIPE_WRITE_FAILED        10
#define IDMSG_PIPE_READ_FAILED         11
#define IDMSG_SEMAPHORE_ERROR          12
#define IDMSG_ME_FIRST                 13
#define IDMSG_CONNECT_FAILED           14
#define IDMSG_DRAW                     15
#define IDMSG_INVALID_NAME             16

/*--------------------------------------------------------------*\
 *  Ids of messages used between threads (non-pipe related)
\*--------------------------------------------------------------*/

#define WM_MSG                  WM_USER+0

/*--------------------------------------------------------------*\
 *  Other constants
\*--------------------------------------------------------------*/
#define TITLE_LEN          124
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
