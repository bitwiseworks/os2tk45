/*==============================================================*\
 *
 *  cliqmain.h - sample queue application main header file
 *      Copyright 1991, IBM Corp.
 *
 *--------------------------------------------------------------
 *
 *  This header file contains the application wide constants.
 *
\*==============================================================*/

#define IDR_MAIN       1      /* resource identifier */

/*--------------------------------------------------------------*\
 *  Standard Dialog box ids
\*--------------------------------------------------------------*/
#define MSGBOXID        1001

/* WINDOW IDS */
#define  WID_CLIENT     101
#define  WID_Q_MSG      102
#define  WID_Q_PRIORITY 103
#define  WID_DAY        104
#define  WID_MONTH      105
#define  WID_YEAR       106
#define  WID_SEND_MSG   107
#define  WID_SEND_DATE  108

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
#define IDMSG_HELPLOADERROR             3
#define IDMSG_CANNOTLOADSTRING          4
#define IDMSG_HELPDISPLAYERROR          5

#define IDMSG_SVR_DOWN                  6
#define IDMSG_SEM_ERR                   7
#define IDMSG_Q_ERR                     8
#define IDMSG_MEM_ERR                   9
#define IDMSG_SEM_TIMEOUT              10
#define IDMSG_MEM_FULL                 11
#define IDMSG_GIVE_MEM_ERR             12
#define IDMSG_SUBALLOC_MEM_ERR         13

/*--------------------------------------------------------------*\
 *  Ids of messages used between threads
\*--------------------------------------------------------------*/

#define WM_MSG                              WM_USER+0

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

#ifndef max
#define max(a,b) (((a) > (b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a) < (b))?(a):(b))
#endif
