/**************************************************************************
 *  File name  :  main.h
 *
 *  Description:  This header file contains the application wide
 *                constants and structure definitions.
 *
 *  Concepts   :  [none]
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

#define IDR_MAIN       1      /* resource identifier */

/*
 *  Standard Dialog box ids
 */
#define FILEOPEN        2000
#define FILESAVE        2001

#define MSGBOXID        1001
#define OBJECTID        1002

/*
 *  Menu item ids
 */
#define IDM_FILE                            1000
#define IDM_FILENEW                         1100
#define IDM_FILEOPEN                        1200
#define IDM_FILESAVE                        1300
#define IDM_FILESAVEAS                      1400

#define IDM_EDIT                            2000
#define IDM_EDITUNDO                        2100
#define IDM_EDITCUT                         2200
#define IDM_EDITCOPY                        2300
#define IDM_EDITPASTE                       2400
#define IDM_EDITCLEAR                       2500

#define IDM_HELP                            9000
#define IDM_HELPUSINGHELP                   9100
#define IDM_HELPGENERAL                     9200
#define IDM_HELPKEYS                        9300
#define IDM_HELPINDEX                       9400
#define IDM_HELPTUTORIAL                    9500
#define IDM_HELPPRODUCTINFO                 9600

/*
 *  Dialog item ids
 */
#define ID_OK               1
#define ID_CANCEL           2

/*
 *  Stringtable ids
 */
#define IDS_FILEOPENEXT             1
#define IDS_APPNAME                 2
#define IDS_HELPLIBRARYNAME         3
#define IDS_OPEN                    4
#define IDS_HELPWINDOWTITLE         5
#define IDS_SAVE                    6
#define IDS_UNTITLED                7
#define IDS_TITLEBARSEPARATOR       8
#define IDS_OBJECTCLASS             9

/*
 *  Messagetable ids
 */
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
#define IDMSG_YOURTUTORIAL             25

/*
 *  Ids of messages used between threads
 */
#define TM_THREADINITFAILED         WM_USER+300

/*
 *  Other constants
 */
#define MESSAGELEN          50    /* maximum length for messages */
#define TITLESEPARATORLEN   4

/* return values for initialization routines */
#define RETURN_SUCCESS      0   /* successful return in DosExit */
#define RETURN_ERROR        1   /* error return in DosExit */
#define BEEP_WARN_FREQ      60  /* frequency of warning beep */
#define BEEP_WARN_DUR      100  /* duration of warning beep */


/*
 *  Included in the sample template is code for that creates a
 *  second thread that can be used for background processing.
 *  To use this thread, enable the BACKGROUND_THREAD constant.
 *  This will enable the routines that create and destroy the
 *  background processing thread.  The routines for the thread
 *  itself are located in the thrd.c file.
 */

#define BACKGROUND_THREAD
/***************************  End of main.h  ****************************/
