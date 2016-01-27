/*static char *SCCSID = "@(#)hanoi.h	6.10 92/03/03";*/
/*==============================================================*\
 *                                                              *
 *  HANOI.H - Sample PM application hanoi header file           *
 *      Created 1989 - 1995 IBM Corp.                           *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This header file contains the application wide constants    *
 *  and structure definitions.                                  *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 * Resource IDs                                                 *
\*--------------------------------------------------------------*/
#define ID_RESOURCE    1
#define IDR_BITMAP     2
#define IDC_BITMAP     3

/*--------------------------------------------------------------*\
 *  Menu item IDs                                               *
\*--------------------------------------------------------------*/
#define IDM_START  (WM_USER+0)
#define IDM_STOP   (WM_USER+1)
#define IDM_SET    (WM_USER+2)
#define IDM_DONE   (WM_USER+3) /* Message posted when thread terminates */
#define IDM_ON     (WM_USER+4)
#define IDM_OFF    (WM_USER+5)

/*--------------------------------------------------------------*\
 *  Message item IDs                                            *
\*--------------------------------------------------------------*/
#define IDD_SETCOUNT                           1
#define IDD_ENTRYFLD                           2
#define IDD_MSGBOX                             3
#define IDD_PRODUCTINFO                        4
#define IDD_HELP                               5

#define IDM_HELPHELPFORHELP                 9100
#define IDM_HELPEXTENDED                    9200
#define IDM_HELPKEYS                        9300
#define IDM_HELPINDEX                       9400
#define IDM_HELPPRODUCTINFO                 9600

/*--------------------------------------------------------------*\
 *  Help table and subtables                                    *
\*--------------------------------------------------------------*/
#define HANOI_HELP_TABLE                    1000

/*--------------------------------------------------------------*\
 *  Main window help panels                                     *
\*--------------------------------------------------------------*/
#define SUBTABLE_MAIN                       2000
#define PANEL_MAIN                          2100
#define PANEL_HELP                          2910
#define PANEL_HELPHELPFORHELP               2920
#define PANEL_HELPEXTENDED                  2930
#define PANEL_HELPKEYS                      2940
#define PANEL_HELPINDEX                     2950
#define PANEL_HELPPRODUCTINFO               2960
#define PANEL_KEYSHELP                      2970

#define PANEL_HELPSTART                     2980
#define PANEL_HELPSTOP                      2981
#define PANEL_HELPSET                       2982
#define PANEL_HELPON                        2983
#define PANEL_HELPOFF                       2984

/*--------------------------------------------------------------*\
 *  Enter text dialog help subtable                             *
\*--------------------------------------------------------------*/
#define SUBTABLE_PRODUCTINFODLG             3000
#define PANEL_PRODUCTINFODLG                3100
#define PANEL_PRODUCTINFO_OK                3110

/*--------------------------------------------------------------*\
 *  Stringtable ids                                             *
\*--------------------------------------------------------------*/
#define IDS_TITLE                            101
#define IDS_UNTITLED                         102
#define IDS_APPNAME                          103
#define IDS_HELPLIBRARYNAME                  104
#define IDS_HELPWINDOWTITLE                  105
#define IDS_OBJECTCLASS                      106

/*--------------------------------------------------------------*\
 *  Messagetable ids                                            *
\*--------------------------------------------------------------*/
#define IDMSG_INITFAILED                     301
#define IDMSG_MAINWINCREATEFAILED            302
#define IDMSG_DISKNUMBERERROR                303
#define IDMSG_CANNOTLOADEXITLIST             304
#define IDMSG_CANNOTLOADSTRING               305
#define IDMSG_HELPLOADERROR                  306
#define IDMSG_HELPDISPLAYERROR               307

/*--------------------------------------------------------------*\
 *  Other constants                                             *
\*--------------------------------------------------------------*/
#define MAXDISKCNT         16    /* Maximum # of disks allowable */
#define BASEXOFFSET        10    /* X offset for platform base   */
#define BASEYOFFSET        10    /* Y offset for platform base   */
#define BASETHICK          10    /* Base thickness               */
#define BASELEN           300    /* Base width                   */
#define POSTOFFSET         62    /* First post offset from edge  */
#define POSTSPACE         100    /* Distance between posts       */
#define POSTWIDTH           5    /* Width of each post           */
#define POSTHALF            2    /* 1/2 width for centering      */
#define POSTEXTRAHT        25    /* Post height above top disk   */
#define DISKTHICK           3    /* Thickness of each disk       */
#define DISKSPACE         (DISKTHICK+1)     /* Thickness + space */
#define MINDISKWIDTH       11    /* Width of the smallest disk   */
#define MAXDISKWIDTH       91    /* Width of the thickest disk   */
#define BOTDISKYPOS        (BASEYOFFSET+BASETHICK+DISKSPACE-DISKTHICK)
#define STACK            8192    /* Stack size for thread        */
#define fDRAW               1    /* Indicate draw to DrawDisk()  */
#define fERASE              0    /* Indicate erase               */
#define DEFAULTSIZE         5    /* Default number of disks      */
#define MSGBUFSIZE         33    /* Space needed for sprintf msg */
#define RETURN_SUCCESS      0    /* successful return in DosExit */
#define RETURN_ERROR        1    /* error return in DosExit      */
#define BEEP_WARN_FREQ      60   /* frequency of warning beep    */
#define BEEP_WARN_DUR      100   /* duration of warning beep     */
#define MESSAGELEN          50   /* maximum length for messages  */
#define MAX_DIGITS          5
#define THREADSTACKSIZE   8192L
#define SEM_TIMEOUT      10000L
#define HELPLIBRARYNAMELEN  20
#define VERT_POS            15   /* from top to display text info*/
#define HORZ_POS            50
#define BUFF_SIZE          100
#define ONCE               while(0)
#define DELETE_WIDTH       50

/*--------------------------------------------------------------*\
 *  Standard Dialog box ids                                     *
\*--------------------------------------------------------------*/
#define OBJECTID        1002

/*--------------------------------------------------------------*\
 *  Macro constants                                             *
\*--------------------------------------------------------------*/
#define DrawRect(x1,y1,x2,y2,color)  ptl.x = (LONG) (x1);             \
                                     ptl.y = (LONG) (y1);             \
                                     GpiSetCurrentPosition(hps,&ptl); \
                                     ptl.x = (LONG) (x2);             \
                                     ptl.y = (LONG) (y2);             \
                                     GpiSetColor(hps,color);          \
                                     GpiBox(hps,DRO_FILL,&ptl,0L,0L);

/*--------------------------------------------------------------*\
 *  Ids of messages used between threads                        *
\*--------------------------------------------------------------*/
#define TM_THREADINITFAILED           WM_USER+300

