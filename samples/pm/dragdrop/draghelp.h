/**************************************************************************
 *  File name  :  draghelp.h
 *
 *  Description:  The DRAGHELP include file defines symbolic constants
 *                used by the Help Manager.
 *
 *  Concepts   :  IPF
 *
 *  API's      :  [none]
 *
 *  Copyright (C) 1991 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 *************************************************************************/

/*
 *  Help table and subtables
 */
#define DRAGDROP_HELP_TABLE                 1000
#define SUBTABLE_MAIN                       2000
#define SUBTABLE_OPENDLG                    3000
#define SUBTABLE_PRODUCTINFODLG             4000


/*
 *  Main window help panels
 */
#define PANEL_MAIN                          2100
#define PANEL_OPENDLG                       2100
#define PANEL_PRODUCTINFODLG                4100


#define PANEL_FILE                          2210
#define PANEL_FILEOPEN                      2220
#define PANEL_WINDOW                        2310
#define PANEL_WINDOWREFRESH                 2320
#define PANEL_WINDOWSELECTALL               2330
#define PANEL_WINDOWDESELECTALL             2340
#define PANEL_HELP                          2910
#define PANEL_HELPINDEX                     2920
#define PANEL_HELPGENERAL                   2930
#define PANEL_HELPUSINGHELP                 2940
#define PANEL_HELPKEYS                      2950
#define PANEL_HELPPRODUCTINFO               2960

/*
 *  Definitions of help subtable objects
 */
#define PANEL_OPENDLG_OK                    3110
#define PANEL_OPENDLG_HELP                  3120
#define PANEL_PRODUCTINFO_OK                4110

#define HI_HELPMENU             64022
#define HI_HELPUSINGHELP        64023
#define HI_HELPGENERALHELP      64024
#define HI_HELPHELPINDEX        64026
#define HI_USINGHELP            64027
#define HI_HELPINDEX            64044
/***************************  End of draghelp.h  *************************/
