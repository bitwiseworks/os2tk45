/*==============================================================*\
 *  cliqhelp.h - Sample queue apps help include file
 *      Copyright 1991 IBM Corp.
 *--------------------------------------------------------------
 *
 *  This resource file contains constant and function
 *  definitions for resources used by the help manager.
 *
\*==============================================================*/
/*--------------------------------------------------------------*\
 *  Help table and subtables                                    *
\*--------------------------------------------------------------*/
#define CLIQ_HELP_TABLE                     1000

#define SUBTABLE_MAIN                       2000
#define SUBTABLE_ABOUTBOXDLG                3000



/*--------------------------------------------------------------*\
 *  Main window help panels                                     *
\*--------------------------------------------------------------*/
#define PANEL_MAIN                          2100
#define PANEL_ABOUTBOXDLG                   3100


#define PANEL_FILE                          2210
#define PANEL_FILEEXIT                      2290

#define PANEL_MESSAGE                       2310
#define PANEL_PRIORITY                      2320
#define PANEL_SEND_MSG                      2330
#define PANEL_SEND_DATE                     2340
#define PANEL_DATE                          2350

#define PANEL_HELP                          2910
#define PANEL_HELPHELPFORHELP               2920
#define PANEL_HELPEXTENDED                  2930
#define PANEL_HELPKEYS                      2940
#define PANEL_HELPINDEX                     2950
#define PANEL_HELPABOUT                     2960

/*--------------------------------------------------------------*\
 *  Enter text dialog help subtable                             *
\*--------------------------------------------------------------*/
#define PANEL_ABOUTBOX_OK                   3110


#define IDD_ABOUTBOX        1001

#define IDC_OK              1
#define IDC_ICON            4


VOID  InitHelp(VOID);
VOID  DestroyHelpInstance(VOID);
VOID  DisplayHelpPanel(SHORT sIdPanel);
VOID  HelpAbout(MPARAM mp2);
VOID  HelpTutorial(MPARAM mp2);
VOID  HelpIndex(MPARAM mp2);
VOID  HelpKeys (MPARAM mp2);
VOID  HelpExtended(MPARAM mp2);
VOID  HelpHelpForHelp(MPARAM mp2);
