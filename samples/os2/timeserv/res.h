/**************************************************************************\
*                                                                          *
*       MODULE:     dialogs.h                                              *
*                                                                          *
*       COMMENT:    Constants for the Clock dialogs                        *
*                                                                          *
\**************************************************************************/


#define CLKTM_MAJOR 0x0100
#define CLKTM_MINOR 0x0200

#define CLKTM_ALWAYS    0x0001
#define CLKTM_NOTICONIC 0x0002
#define CLKTM_NEVER     0x0004

#define CLKCLR_BUTTONSHIFT 0x08

#define CLKCLR_OBJECTS      0x0100
#define CLKCLR_BACKGROUND   0x0101
#define CLKCLR_FACE         0x0102
#define CLKCLR_HOURHAND     0x0103
#define CLKCLR_MINUTEHAND   0x0104

#define  CCHMAXSTRING      256
#define  NOSELECT          -1     /* indices for SHORT Selected;     */
#define  HOUR               0      /* index into rDateTime, wDateTime,*/
                                   /* wRange                          */
#define  MINUTE             1
#define  SECOND             2
#define  MONTH              3
#define  DAY                4
#define  YEAR               5
#define  UPTIME             6
#define  DOWNTIME           7
#define  UPDATE             8
#define  DOWNDATE           9
#define  TYPETIME          10
#define  TYPEDATE          11
#define TIME_SEP_LEN        1
#define DATE_SEP_LEN        1
#define STRING_AM_LEN       2
#define STRING_PM_LEN       2
#define AM_PM  2                    /* index into rTimeSep        */
#define PATHMAX 259                 /* path cb     max            */
#define COUNTRY_USA     1           /* Country code               */
#define COUNTRY_UK      44          /* Country code               */

#define IDR_MAIN        1           /* resource identifier */

#define IDD_COLORS                11
#define IDD_TICKS                 12
#define IDD_ABOUT                 13
#define IDD_ALARM                 14

#define IDM_SETTINGS              40
#define IDM_VIEW                  41
#define IDM_TOGGLECONTROLS        14
#define IDM_COLORS                15
#define IDM_TICKS                 16
#define IDM_EXIT                  17
#define IDM_ABOUT                 18
#define IDM_ALARM                 19
#define IDM_SECONDHAND            50

#define IDM_SET                   20
#define IDM_CLEAR                 21

#define IDM_SELECT                22
#define IDM_ADD                   23
#define IDM_MODIFY                24
#define IDM_DELETE                25
#define IDM_DATETIME              26
#define IDM_TIME                  27
#define IDM_DATE                  28
#define IDM_ANALOG                29
#define IDM_DIGITAL               30
#define IDM_ALARM_EXPIRED         32



#define IDM_HELP                            9000
#define IDM_HELPHELPFORHELP                 9100
#define IDM_HELPEXTENDED                    9200
#define IDM_HELPKEYS                        9300
#define IDM_HELPINDEX                       9400
#define IDM_HELPTUTORIAL                    9500
#define IDM_HELPABOUT                       9600

#define IDD_TIME                             100


#define TIME                                  201
#define SCROLL_TIME                           202
#define DATE                                  203
#define SCROLL_DATE                           204
#define IDC_HOURMINUTES                       205
#define ALARM_TIME                            207
#define SCROLL_HOURS                          208
#define RB_ALARMON                            209
#define RB_ALARMOFF                           210
#define IDC_BEEP                              211
#define IDC_MSG                               212
#define RB_AM                                 213
#define RB_PM                                 214
#define SCROLL_MINUTES                        215



/*--------------------------------------------------------------*\
 *  Stringtable ids
\*--------------------------------------------------------------*/
#define IDS_FILEOPENEXT             1
#define IDS_APPNAME                 2
#define IDS_HELPLIBRARYNAME         3
#define IDS_OPENDLGTITLE            4
#define IDS_OPENDLGBUTTON           5
#define IDS_HELPWINDOWTITLE         6
#define IDS_SAVEDLGTITLE            7
#define IDS_SAVEDLGBUTTON           8
#define IDS_UNTITLED                9
#define IDS_TITLEBARSEPARATOR      10
#define IDS_OBJECTCLASS            14
#define IDS_RUNONLYONE                    300
#define IDS_TITLE                         301
#define IDS_HELPTITLE                     302


#define IDH_CLOCK                    412
#define IDH_TOGGLECONTROLS           413
#define IDH_COLORS                   414
#define IDH_TICKS                    415
#define IDH_DATETIME                 416
#define IDH_EXIT                     417
#define IDH_ABOUT                    418
#define IDH_TIME                     419
#define IDH_DATE                     420
#define IDH_ANALOG                   421
#define IDH_DIGITAL                  422


#define IDD_ABOUTBOX                 1001

#define IDC_OK                       1
#define IDC_CANCEL                   2
#define IDC_HELP                     3
#define IDC_ICON                     4

#define IDC_DATE                     101
