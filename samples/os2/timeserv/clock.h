/*static char *SCCSID = "@(#)clock.h	6.3 92/02/18";*/
/**************************************************************************\
*                                                                          *
*       MODULE:     clock.h                                                *
*                                                                          *
*       COMMENT:    Global declarations                                    *
*                                                                          *
\**************************************************************************/


#define HT_HOUR           1
#define HT_MINUTE         2
#define HT_HOUR_SHADE     3
#define HT_MINUTE_SHADE   4
#define HT_SECOND   5

#define DM_TIME         ( (USHORT)0x0001)
#define DM_DATE         ( (USHORT)0x0002)
#define DM_ANALOG       ( (USHORT)0x0010)
#define DM_DIGITAL      ( (USHORT)0x0020)
#define DM_SECONDHAND   ( (USHORT)0x0040)

#define MAXTITLELENGTH 256
#define MESSAGELEN     80

#define SHADE               0
#define LIGHT               1
#define SURFACE             2
#define BACKGROUND          3

#define DP_INSIDE           0
#define DP_UNDER            1
#define DP_LEFTDOWN         2
#define DP_LEFTMIDDLE       3

#define DM_REDRAW           0
#define DM_UPDATE           1

#define AM_ACTIVE           ( (USHORT) 0x0001)
#define AM_AUDIO            ( (USHORT) 0x0002)
#define AM_MSGBOX           ( (USHORT) 0x0004)
#define TIMER_ASYNC         1
#define TIMER_CONT          2
#define SEM_TIMER_NAME      "\\sem32\\applnam.sem"
#define NO_ERROR            (APIRET ) 0L

#define SEM_TIMER_VALUE     ( (ULONG) (1000L * 60 * 60) )

#define SZ_APPNAME  "ClockProgram 2.1"
#define SZ_KEYNAME  "Preferences"

#define HELP

typedef struct _ALARMTIME
{
   UCHAR uchHour;
   UCHAR uchMinutes;
   USHORT usMode;
} ALARMTIME;

typedef struct _TIMER_RESOURCES
{
     HEV     hTimerDev;
     CHAR    *pszSemName;
     ULONG   ulTimerValue;
     HTIMER  hTimer;
}TIMER_RESOURCES,*PTIMER_RESOURCES;


typedef struct _CLOCKPREF
{
    USHORT usMajorTickPref;    /*0,1*/
    USHORT usMinorTickPref;    /*2,3*/
    LONG clrBackground;        /*4,5,6,7*/
    LONG clrFace;              /*8,9,a,b*/
    LONG clrHourHand;          /*c,d,e,f*/
    LONG clrMinuteHand;        /*10,11,12,13*/
    LONG clrDateOnAnalog;      /*14,15,16,17*/
    BOOL fControlsHidden;      /*18,19*/
    USHORT usDispMode;         /*1a,1b*/
    ALARMTIME alarm ;          /*1c,1d,1e,1f*/
    SWP swp;                   /*20....*/
    SHORT sTimerType;

} CLOCKPREF;

typedef struct _INISAVERESTORE
{
    SWP    swp;
    USHORT yRestore;
    USHORT xRestore;
    USHORT yMinRestore;
    USHORT xMinRestore;
} INISAVERESTORE;
typedef struct _DATETIME FAR *LPDT;    /* For DosGet/SetDateTime     */




/*--------------------------------------------------------------*\
 *  Messagetable ids
\*--------------------------------------------------------------*/
#define IDMSG_INITFAILED                1
#define IDMSG_MAINWINCREATEFAILED       2
#define IDMSG_CANNOTOPENINPUTFILE       3
#define IDMSG_CANNOTOPENOUTPUTFILE      4
#define IDMSG_CANNOTRUNCOLOR            8
#define IDMSG_CANNOTGETHPS              9
#define IDMSG_HELPLOADERROR            10
#define IDMSG_CANNOTLOADSTRING         11
#define IDMSG_CANNOTOPENPRINTER        12
#define IDMSG_HELPDISPLAYERROR         13
#define IDMSG_CANNOTLOADEXITLIST       14
#define IDMSG_PRINTINITFAILED          15
#define IDMSG_OVERWRITEFILE            16
#define IDMSG_PRINTERROR               17
#define IDMSG_UNDOFAILED               18
#define IDMSG_CANNOTGETFILEINFO        19
#define IDMSG_CANNOTALLOCATEMEMORY     20
#define IDMSG_CANNOTREADFILE           21
#define IDMSG_CANNOTWRITETOFILE        22
#define IDMSG_CANNOTLOADFONTS          23
#define IDMSG_CANNOTGETPAGEINFO        24



/*--------------------------------------------------------------*\
 *  Standard Dialog box ids
\*--------------------------------------------------------------*/
#define FILEOPEN        2000
#define FILESAVE        2001

#define MSGBOXID        1001
#define OBJECTID        1002



APIRET MessageBox( HWND hwndOwner,SHORT sIdMsg,
                   SHORT fsStyle, BOOL fBeep );
VOID InitHelp(VOID);
