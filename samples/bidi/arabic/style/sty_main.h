/*************************************************************************
*
*  File Name   : STY_MAIN.H
*
*  Description : This header file contains the application wide constants
*                and structure definitions.
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
************************************************************************/

#define IDR_MAIN         1      /* resource identifier */
#define IDR_ICON1        2      /* resource identifier */
#define IDR_ICON2        3      /* resource identifier */
#define IDR_ICON3        4      /* resource identifier */
#define IDR_ICON4        5      /* resource identifier */
#define IDR_BITMAP       6      /* resource identifier */
#define IDR_TOWER        7      /* resource identifier */
#define IDR_GLOBE        8      /* resource identifier */
#define IDR_WATERFALL    9      /* resource identifier */
#define IDR_KEY         10      /* resource identifier */

/*--------------------------------------------------------------*\
 *  Dialog box ids                                              *
\*--------------------------------------------------------------*/
#define IDD_FILEOPEN    2000
#define IDD_FILESAVE    2001
#define IDD_FONT        2002

#define IDD_MSGBOX      1001
#define ID_MLE          1002

/*--------------------------------------------------------------*\
 *  Menu item ids                                               *
\*--------------------------------------------------------------*/
#define IDM_FILE                            1000
#define IDM_FILENEW                         1100
#define IDM_FILEOPEN                        1200
#define IDM_FILESAVE                        1300
#define IDM_FILESAVEAS                      1400
#define IDM_EXIT                            1900

#define IDM_EDIT                            2000

#define IDM_EDITCOPY_NOCONV                 2100
#define IDM_EDITCOPY_AUTOCONV               2101
#define IDM_EDITCOPY_CONV_VISUAL            2102
#define IDM_EDITCOPY_CONV_IMPLICIT          2103

#define IDM_EDITPASTE_NOCONV                2300
#define IDM_EDITPASTE_AUTOCONV              2301
#define IDM_EDITPASTE_CONV_VISUAL           2302
#define IDM_EDITPASTE_CONV_IMPLICIT         2303

#define IDM_OPTIONS                         3000
#define IDM_OPTIONSFORECOLOR                3100
#define IDM_OPTIONSFORECOLORBLACK           3110
#define IDM_OPTIONSFORECOLORBLUE            3120
#define IDM_OPTIONSFORECOLORRED             3130
#define IDM_OPTIONSFORECOLORDEFAULT         3140
#define IDM_OPTIONSBACKCOLOR                3200
#define IDM_OPTIONSBACKCOLORYELLOW          3210
#define IDM_OPTIONSBACKCOLORPINK            3220
#define IDM_OPTIONSBACKCOLORCYAN            3230
#define IDM_OPTIONSBACKCOLORDEFAULT         3240
#define IDM_OPTIONSFONT                     3300

#define IDM_DEMO                            4000
#define IDM_DEMODLG                         4100
#define IDM_DEMODLGBUTTONS                  4110
#define IDM_DEMODLGLISTBOXES                4120
#define IDM_DEMODLGCOMBOBOXES               4130
#define IDM_DEMODLG_SL_ENTRYFIELDS          4140
#define IDM_DEMODLG_ML_ENTRYFIELDS          4141
#define IDM_DEMODLGSTATIC                   4150
#define IDM_DEMODLGPP                       4160
#define IDM_DEMODLGSLIDER                   4170
#define IDM_DEMODLGSPINBUTTON               4180
#define IDM_DEMODLGVALUESET                 4190
#define IDM_DEMODLGNOTEBOOK                 4191
#define IDM_DEMODLGCONTAINER                4192
#define IDM_DEMOCUACONTROLS                 4193

#define IDM_DEMOMSGBOX                      4200
#define IDM_DEMOMSGBOXOK                    4210
#define IDM_DEMOMSGBOXOKCANCEL              4220
#define IDM_DEMOMSGBOXYESNO                 4230

#define IDM_DEMOMSGBOXYESNOCANCEL           4240
#define IDM_DEMOMSGBOXRETRYCANCEL           4250
#define IDM_DEMOMSGBOXABORT                 4260
#define IDM_DEMOMSGBOXENTER                 4270
#define IDM_DEMOMSGBOXENTERCANCEL           4280
#define IDM_DEMOMSGBOXQUERY                 4290
#define IDM_DEMOMSGBOXWARNING               4300
#define IDM_DEMOMSGBOXINFO                  4310
#define IDM_DEMOMSGBOXCRITICAL              4320
#define IDM_DEMOMSGBOXAPP                   4330
#define IDM_DEMOMSGBOXSYS                   4340
#define IDM_DEMOMSGBOXHELP                  4360

#define IDM_TEXTCONV                        5000

#define IDM_MENU                            6000
#define IDM_MENU_SIMPLETEXT                 6100
#define IDM_MENU_COLUMNS                    6200
#define IDM_MENU_TABTEXT                    6300
#define IDM_MENU_TABTEXTCOL                 6400
#define IDM_MENU_ALLIGNTEXT                 6500
#define IDM_MENU_ALLIGNTEXTCOL              6600
#define IDM_MENU_ALIGNTABTEXTCOL            6700
#define IDM_MENU_INVERSE                    6800

#define IDM_HELP                            9000
#define IDM_HELPUSINGHELP                   9100
#define IDM_HELPGENERAL                     9200
#define IDM_HELPKEYS                        9300
#define IDM_HELPINDEX                       9400
#define IDM_HELPTUTORIAL                    9500
#define IDM_HELPPRODINFO                    9600

#define IDM_NAME                            9701
#define IDM_TEXT                            9702
#define IDM_ICON                            9703
#define IDM_DETAIL                          9704
#define IDM_VIEWS                           9705
#define IDD_CONTAINERMENU                   9706

/*--------------------------------------------------------------*\
 *  Dialog item ids                                             *
\*--------------------------------------------------------------*/
#define ID_OK               1
#define ID_CANCEL           2

/*--------------------------------------------------------------*\
 *  Stringtable ids                                             *
\*--------------------------------------------------------------*/
#define ARA_OFF                     0x1000

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
#define IDS_PPFONTPOINT            14
#define IDS_FONTDLGTITLE           15

#define IDS_DEMOMSGBOXOK           20
#define IDS_DEMOMSGBOXOKCANCEL     21
#define IDS_DEMOMSGBOXYESNO        22
#define IDS_DEMOMSGBOXYESNOCANCEL  23
#define IDS_DEMOMSGBOXRETRYCANCEL  24
#define IDS_DEMOMSGBOXABORT        25
#define IDS_DEMOMSGBOXENTER        26
#define IDS_DEMOMSGBOXENTERCANCEL  27
#define IDS_DEMOMSGBOXQUERY        28
#define IDS_DEMOMSGBOXWARNING      29
#define IDS_DEMOMSGBOXINFO         30
#define IDS_DEMOMSGBOXCRITICAL     31
#define IDS_DEMOMSGBOXAPP          32
#define IDS_DEMOMSGBOXSYS          33
#define IDS_DEMOMSGBOXHELP         35


#define IDS_LISTBOX1_1               40
#define IDS_LISTBOX1_2               41
#define IDS_LISTBOX1_3               42
#define IDS_LISTBOX1_4               43
#define IDS_LISTBOX1_5               44
#define IDS_LISTBOX1_6               45
#define IDS_LISTBOX1_7               46
#define IDS_LISTBOX1_8               47
#define IDS_LISTBOX1_9               48
#define IDS_LISTBOX1_10              49

#define IDS_BLUE                   50
#define IDS_RED                    51
#define IDS_PINK                   52
#define IDS_GREEN                  53
#define IDS_CYAN                   54
#define IDS_YELLOW                 55
#define IDS_DARKGRAY               56
#define IDS_DARKBLUE               57
#define IDS_DARKRED                58
#define IDS_DARKPINK               59
#define IDS_DARKGREEN              60
#define IDS_DARKCYAN               61
#define IDS_BROWN                  62
#define IDS_PALEGRAY               63
#define IDS_DEFAULT                64

#define IDS_FIRSTCOLOR             IDS_BLUE


#define IDS_LISTBOX2_1               70
#define IDS_LISTBOX2_2               71
#define IDS_LISTBOX2_3               72
#define IDS_LISTBOX2_4               73
#define IDS_LISTBOX2_5               74
#define IDS_LISTBOX2_6               75
#define IDS_LISTBOX2_7               76
#define IDS_LISTBOX2_8               77
#define IDS_LISTBOX2_9               78
#define IDS_LISTBOX2_10              79

#define IDS_CNR_TITLE                80
#define IDS_CNR_COLUMNTITLE          82
#define IDS_CNR_PICTURE              84
#define IDS_CNR_TOWER                86
#define IDS_CNR_WATERFALL            88
#define IDS_CNR_GLOBE                90
#define IDS_CNR_KEY                  92
#define IDS_BOOK_PAGE2               94
#define IDS_BOOK_STL1                96
#define IDS_BOOK_STL2                98
#define IDS_BOOK_STL3                99
#define IDS_SPIN_WHITE              100
#define IDS_SPIN_BLACK              102
#define IDS_SPIN_BLUE               104
#define IDS_SPIN_RED                106
#define IDS_SPIN_PINK               108
#define IDS_SPIN_GREEN              110
#define IDS_SPIN_CYAN               112
#define IDS_SPIN_YELLOW             114
#define IDS_SPIN_DARKGREY           116
#define IDS_SPIN_DARKBLUE           118
#define IDS_SPIN_DARKRED            120
#define IDS_SPIN_DARKPINK           122
#define IDS_SPIN_DARKGREEN          124
#define IDS_SPIN_DARKCYAN           126
#define IDS_SPIN_BROWN              128
#define IDS_SPIN_PALEGREY           130
#define IDS_BOOK_TAB1MA             131
#define IDS_BOOK_TAB1MI1            132
#define IDS_BOOK_TAB1MI2            133
#define IDS_BOOK_TAB2MA             134
#define IDS_BOOK_TITLE              135

#define IDS_COPYVISUALTEXT          140
#define IDS_COPYIMPLICITTEXT        141

#define IDS_CONVTEXT                142
/*--------------------------------------------------------------*\
 *  Messagetable ids                                            *
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
#define IDMSG_OVERWRITEFILE            16
#define IDMSG_UNDOFAILED               18
#define IDMSG_CANNOTGETFILEINFO        19
#define IDMSG_CANNOTALLOCATEMEMORY     20
#define IDMSG_CANNOTREADFILE           21
#define IDMSG_CANNOTWRITETOFILE        22
#define IDMSG_CANNOTLOADFONTS          23
#define IDMSG_CANNOTGETPAGEINFO        24
#define IDMSG_CANNOTSETPP              26
#define IDMSG_FILECHANGED              27

#define IDMSG_TEST_SUCCESSFUL          28
#define IDMSG_TEST_ERROR               29

/*--------------------------------------------------------------*\
 *  Other constants                                             *
\*--------------------------------------------------------------*/
#define MESSAGELEN          50    /* maximum length for messages */
#define TITLESEPARATORLEN   4     /* length of title bar separator string */

#ifndef CCHMAXFILE
#define CCHMAXFILE          80
#endif

#define SM_SETFOCUS     WM_USER+100     /* message to set the focus to MLE */



