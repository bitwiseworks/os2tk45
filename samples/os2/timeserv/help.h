/*--------------------------------------------------------------*\
 *  Help table and subtables                                    *
\*--------------------------------------------------------------*/
#define CLOCK_HELP_TABLE                    1000

#define SUBTABLE_MAIN                       2000
#define SUBTABLE_ABOUTBOXDLG                3000
#define SUBTABLE_ALARM                      8000
#define SUBTABLE_TICKS                      6000
#define SUBTABLE_TIME                       7000
#define SUBTABLE_TYPEDATE                   9000



/*--------------------------------------------------------------*\
 *  Main window help panels                                     *
\*--------------------------------------------------------------*/
#define PANEL_MAIN                          2100
#define PANEL_ABOUTBOXDLG                   3100


#define PANEL_SETTINGS                  2200
#define PANEL_SETTINGSCOLORS            2300
#define PANEL_SETTINGSTICKS             2400
#define PANEL_SETTINGSDATETIME          2500
#define PANEL_SETTINGSALARM             2600
#define PANEL_SETTINGSSECONDHAND        2700
#define PANEL_SETTINGSTOGGLECONTROLS    2800

#define PANEL_VIEW                      4000
#define PANEL_VIEWTIME                  4100
#define PANEL_VIEWDATE                  4200
#define PANEL_VIEWANALOG                4300
#define PANEL_VIEWDIGITAL               4400

#define PANEL_HELP                      5000
#define PANEL_HELPHELPFORHELP           5100
#define PANEL_HELPKEYS                  5200
#define PANEL_HELPINDEX                 5300
#define PANEL_HELPABOUT                 5400

#define PANEL_COLOR_PARTS               9100
#define PANEL_COLOR_BUTTONS             9200

#define PANEL_TICKS_MAJOR               6100
#define PANEL_TICKS_MINOR               6200

#define PANEL_SET_TIME                  7100
#define PANEL_SET_DATE                  7200

#define PANEL_ALARM_TIME                8100
#define PANEL_ALARM_AMPM                8200
#define PANEL_ALARM_TOGGLE              8300
#define PANEL_ALARM_AUDIBLE             8400
#define PANEL_ALARM_MESSAGE             8500

/*---------------------------------------------------------------------------------*\
 *  Enter text dialog help subtable                             *
\*--------------------------------------------------------------*/
#define PANEL_ABOUTBOX_OK                   3110

#define HID_FS_OPEN_DLG_HELP_PANEL          4163
#define HID_FS_SAVE_DLG_HELP_PANEL          4210



VOID  InitHelp(VOID);
VOID  DestroyHelpInstance(VOID);
VOID  DisplayHelpPanel(SHORT sIdPanel);
VOID  HelpAbout(MPARAM mp2);
VOID  HelpTutorial(MPARAM mp2);
VOID  HelpIndex(MPARAM mp2);
VOID  HelpKeys (MPARAM mp2);
VOID  HelpExtended(MPARAM mp2);
VOID  HelpHelpForHelp(MPARAM mp2);
