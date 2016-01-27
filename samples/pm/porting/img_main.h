/*static char *SCCSID = "@(#)img_main.h	6.6 92/02/10";*/
/**************************************************************************
 *  File name  :  img_main.h
 *
 *  Description:  This header file contains the application wide constants
 *                and structure definitions.
 *
 *  Concepts   :  menu items, string table, message table
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
#define IDR_MAIN                      1  /* resource identifier */
#define MSGBOXID                   1001       /* message box id */

/*
 *  Standard Dialog box ids
 */
#define FILEOPEN        2000
#define FILESAVE        2001

/*
 *  Menu item ids
 */
#define IDM_FILE                   1000
#define IDM_FILEOPEN               1100
#define IDM_FILEEXIT               1200

#define IDM_VIEW                   2000
#define IDM_VIEWDETAIL             2100
#define IDM_VIEWFOREGROUNDCOLOR    2200
#define IDM_VIEWFORECOLORBLACK     2301
#define IDM_VIEWFORECOLORWHITE     2302
#define IDM_VIEWFORECOLORBLUE      2303
#define IDM_VIEWFORECOLORGREEN     2304
#define IDM_VIEWFORECOLORYELLOW    2305
#define IDM_VIEWFORECOLORRED       2306
#define IDM_VIEWBACKCOLORBLACK     2307
#define IDM_VIEWBACKCOLORWHITE     2308
#define IDM_VIEWBACKCOLORBLUE      2309
#define IDM_VIEWBACKCOLORGREEN     2310
#define IDM_VIEWBACKCOLORYELLOW    2311
#define IDM_VIEWBACKCOLORRED       2312
#define IDM_VIEWBACKGROUNDCOLOR    2400
#define IDM_VIEWSAVEPOSITION       2500
#define IDM_VIEWRESTOREPOSITION    2600

#define IDM_HELP                   9000
#define IDM_HELPUSINGHELP          9100
#define IDM_HELPGENERAL            9200
#define IDM_HELPKEYS               9300
#define IDM_HELPINDEX              9400
#define IDM_HELPPRODUCTINFO        9500

/*
 *  String table ids
 */
#define IDS_APPNAME                   1
#define IDS_FILEOPENEXTENSION         2
#define IDS_HELPLIBRARYNAME           3
#define IDS_OPEN                      4
#define IDS_HELPWINDOWTITLE           5
#define IDS_UNTITLED                  6
#define IDS_TITLEBARSEPARATOR         7

/*
 *  Message table ids
 */
#define IDMSG_INITFAILED             10
#define IDMSG_MAINWINCREATEFAILED    20
#define IDMSG_CANNOTOPENINPUTFILE    30
#define IDMSG_OUTOFMEMORY            40
#define IDMSG_BACKEQFORE             50
#define IDMSG_LOADERROR              60
#define IDMSG_HELPLOADERROR          70
#define IDMSG_CANNOTLOADSTRING       80
#define IDMSG_CANNOTRUNFILEOPEN      90
#define IDMSG_HELPDISPLAYERROR      100
#define IDMSG_CANNOTLOADEXITLIST    110
#define IDMSG_HELPMANAGERERROR      120
#define IDMSG_WARNING               130

/*
 *  Other constants
 */
#define MESSAGELEN         80   /* maximum length for messages  */
#define TITLESEPARATORLEN   4   /* title seperator length       */
#define CCHAPPNAME         15                  /* sizeof(Image) */
#define COLOR_BASE IDM_VIEWFORECOLORBLACK         /* color base */
#define COLOR_COUNT         6    /* number of color selections  */
#define CCHFILETITLE       10    /* sizeof(Filename: )          */
#define VERTSCROLL          1    /* vertical scroll identifier  */
#define HORZSCROLL          2   /* horizontal scroll identifier */
#define LINESCROLL         10           /* no of pels to scroll */
/***************************  End of img_main.h  *************************/
