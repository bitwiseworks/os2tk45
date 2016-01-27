/******************************************************************************
*
*  File Name   : JIGHELP.H
*
*  Description : Header file for Jigsaw help routines
*
*  Copyright (C) 1992 IBM Corporation
*
*      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*      sample code created by IBM Corporation. This sample code is not
*      part of any standard or IBM product and is provided to you solely
*      for  the purpose of assisting you in the development of your
*      applications.  The code is provided "AS IS", without
*      warranty of any kind.  IBM shall not be liable for any damages
*      arising out of your use of the sample code, even if they have been
*      advised of the possibility of such damages.                                                    *
*
******************************************************************************/

#define HELPLIBRARYNAMELEN          20

#define JIGSAW_HELP_TABLE           1000     /* id for help table resource  */
#define HID_FS_OPEN_DLG_HELP_PANEL  20809    /* id for open dialog box      */

#define SUBTABLE_MAIN               2500

/*
 *  Main window help panels
 */

#define PANEL_MAIN                  20815

/*
 *  Main window subtable, includes menu item help
 */

#define PANEL_FILE                  20801
#define PANEL_FILELOAD              20803
#define PANEL_FILEEXIT              20804

#define PANEL_OPTIONS               20802
#define PANEL_OPTIONSHOW            20805
#define PANEL_SIZE_SMALL            20805
#define PANEL_SIZE_MEDIUM           20805
#define PANEL_SIZE_LARGE            20805
#define PANEL_SIZE_FULL             20805
#define PANEL_OPTIONJUMBLE          20806
#define PANEL_HELPKEYS              20810

#define PANEL_HELP                  64022
#define PANEL_HELPINDEX             64026
#define PANEL_HELPEXTENDED          64024
#define PANEL_HELPHELPFORHELP       64023
#define PANEL_HELPABOUT             64096

#define PANEL_EXTENDED_CONTENTS     20815

/*
 *  Message box help resource entry points
 */

#define PANEL_INFO_OK               20818
