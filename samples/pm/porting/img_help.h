/*static char *SCCSID = "@(#)img_help.h	6.6 92/02/10";*/
/**************************************************************************
 *  File name  :  img_help.h
 *
 *  Description:  This header file contains the identifiers for the
 *                constants used by the help manager.
 *
 *  Concepts   :  help manager
 *
 *  API's      :  [none]
 *
 *  Required
 *    Files    :  [none]
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
#define IMAGE_HELP_TABLE                    1000

#define SUBTABLE_MAIN                       2000

/*
 *  Main window help panels
 */
#define PANEL_MAIN                          2100

/*
 *  Menu help panels
 */
#define PANEL_FILE                          2210
#define PANEL_FILEOPEN                      2220

#define PANEL_VIEW                          2310
#define PANEL_VIEWDETAIL                    2320
#define PANEL_VIEWCOLOR                     2330
#define PANEL_VIEWSAVEPOSITION              2340
#define PANEL_VIEWRESTOREPOSITION           2350

#define PANEL_HELP                          2910
#define PANEL_HELPUSINGHELP                 2920
#define PANEL_HELPGENERAL                   2930
#define PANEL_HELPINDEX                     2950
#define PANEL_HELPPRODUCTINFO               2960

#define HID_FS_OPEN_DLG_HELP_PANEL          4163
#define HID_FS_SAVE_DLG_HELP_PANEL          4210
/***************************  End of img_help.h  *************************/
