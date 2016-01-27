/******************************************************************************
*
*  File Name  : METHODID.H
*
*  Description: This include file contains all information shared between the
*               WPSTutorial Object Class (WPSTUTOR.DLL) and the Show Method
*               Description program (SHOWDESC.EXE).
*
*  Copyright (C) 1993 IBM Corporation
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

/*-----------------------------------------------------------------------------
*    DEFINES
*----------------------------------------------------------------------------*/
#define PIPE_NAME  "\\PIPE\\SHOWDESC\\NAME.QUE"
                                         /* Name of pipe used to communicate */
                                         /*    with WPSTUTOR.DLL             */

#define STACKSIZE         11228          /* Size of stack for thread         */
#define START_WP_STRINGS      5          /* ID at which method name string-  */
                                         /*   table strings begin            */


/*-----------------------------------------------------------------------------
*    STRINGTABLE IDS
*
*    These IDs represent the names of the methods for which descriptions are
*    displayed by the SHOWDESC.EXE program.  These IDs are passed from the
*    WPSTutorial class to the SHOWDESC.EXE program to indicate which method
*    description is to be displayed.
*----------------------------------------------------------------------------*/
        /*---------------------------------------------------------------------
        *    IDS FOR INSTANCE METHOD NAMES
        *--------------------------------------------------------------------*/
#define IDS_WPADDFILEMENUPAGE            START_WP_STRINGS
#define IDS_WPADDFILETYPEPAGE            START_WP_STRINGS+2
#define IDS_WPADDFILE1PAGE               START_WP_STRINGS+4
#define IDS_WPADDFILE2PAGE               START_WP_STRINGS+6
#define IDS_WPADDFILE3PAGE               START_WP_STRINGS+8
#define IDS_WPADDOBJECTGENERALPAGE       START_WP_STRINGS+10
#define IDS_WPADDSETTINGSPAGES           START_WP_STRINGS+12
#define IDS_WPADDTOOBJUSELIST            START_WP_STRINGS+14
#define IDS_WPALLOCMEM                   START_WP_STRINGS+16
#define IDS_WPCLOSE                      START_WP_STRINGS+18
#define IDS_WPCOPIEDFROMTEMPLATE         START_WP_STRINGS+20
#define IDS_WPCOPYOBJECT                 START_WP_STRINGS+22
#define IDS_WPCREATEFROMTEMPLATE         START_WP_STRINGS+24
#define IDS_WPCREATESHADOWOBJECT         START_WP_STRINGS+26
#define IDS_WPDELETE                     START_WP_STRINGS+28
#define IDS_WPDELETEFROMOBJUSELIST       START_WP_STRINGS+30
#define IDS_WPDOESOBJECTMATCH            START_WP_STRINGS+32
#define IDS_WPDRAGGEDOVEROBJECT          START_WP_STRINGS+34
#define IDS_WPDRAGOVER                   START_WP_STRINGS+36
#define IDS_WPDROP                       START_WP_STRINGS+38
#define IDS_WPDROPPEDONOBJECT            START_WP_STRINGS+40
#define IDS_WPENDCONVERSATION            START_WP_STRINGS+42
#define IDS_WPFILTERPOPUPMENU            START_WP_STRINGS+44
#define IDS_WPFINDUSEITEM                START_WP_STRINGS+46
#define IDS_WPFORMATDRAGITEM             START_WP_STRINGS+48
#define IDS_WPFREE                       START_WP_STRINGS+50
#define IDS_WPFREEMEM                    START_WP_STRINGS+52
#define IDS_WPHIDE                       START_WP_STRINGS+54
#define IDS_WPINITDATA                   START_WP_STRINGS+56
#define IDS_WPINSERTPOPUPMENUITEMS       START_WP_STRINGS+58
#define IDS_WPINSERTSETTINGSPAGE         START_WP_STRINGS+60
#define IDS_WPMENUITEMSELECTED           START_WP_STRINGS+62
#define IDS_WPMODIFYPOPUPMENU            START_WP_STRINGS+64
#define IDS_WPOPEN                       START_WP_STRINGS+66
#define IDS_WPQUERYCONFIRMATIONS         START_WP_STRINGS+68
#define IDS_WPQUERYDEFAULTVIEW           START_WP_STRINGS+70
#define IDS_WPQUERYDETAILSDATA           START_WP_STRINGS+72
#define IDS_WPQUERYHANDLE                START_WP_STRINGS+74
#define IDS_WPQUERYICON                  START_WP_STRINGS+76
#define IDS_WPQUERYICONDATA              START_WP_STRINGS+78
#define IDS_WPQUERYREALNAME              START_WP_STRINGS+80
#define IDS_WPQUERYSTYLE                 START_WP_STRINGS+82
#define IDS_WPQUERYTITLE                 START_WP_STRINGS+84
#define IDS_WPQUERYTYPE                  START_WP_STRINGS+86
#define IDS_WPREGISTERVIEW               START_WP_STRINGS+88
#define IDS_WPRENDER                     START_WP_STRINGS+90
#define IDS_WPRENDERCOMPLETE             START_WP_STRINGS+92
#define IDS_WPRESTORE                    START_WP_STRINGS+94
#define IDS_WPRESTOREDATA                START_WP_STRINGS+96
#define IDS_WPRESTORELONG                START_WP_STRINGS+98
#define IDS_WPRESTORESTATE               START_WP_STRINGS+100
#define IDS_WPRESTORESTRING              START_WP_STRINGS+102
#define IDS_WPSAVEDATA                   START_WP_STRINGS+104
#define IDS_WPSAVEDEFERRED               START_WP_STRINGS+106
#define IDS_WPSAVEIMMEDIATE              START_WP_STRINGS+108
#define IDS_WPSAVELONG                   START_WP_STRINGS+110
#define IDS_WPSAVESTATE                  START_WP_STRINGS+112
#define IDS_WPSAVESTRING                 START_WP_STRINGS+114
#define IDS_WPSETDEFAULTVIEW             START_WP_STRINGS+116
#define IDS_WPSETICON                    START_WP_STRINGS+118
#define IDS_WPSETICONDATA                START_WP_STRINGS+120
#define IDS_WPSETREALNAME                START_WP_STRINGS+122
#define IDS_WPSETSTYLE                   START_WP_STRINGS+124
#define IDS_WPSETTITLE                   START_WP_STRINGS+126
#define IDS_WPSETTYPE                    START_WP_STRINGS+128
#define IDS_WPSETUP                      START_WP_STRINGS+130
#define IDS_WPSWITCHTO                   START_WP_STRINGS+132
#define IDS_WPUNINITDATA                 START_WP_STRINGS+134
#define IDS_WPUNLOCKOBJECT               START_WP_STRINGS+136

        /*---------------------------------------------------------------------
        *    IDS FOR CLASS METHOD NAMES
        *--------------------------------------------------------------------*/
#define IDS_WPCLSCREATEDEFAULTTEMPLATES  START_WP_STRINGS+138
#define IDS_WPCLSFINDOBJECTEND           START_WP_STRINGS+140
#define IDS_WPCLSFINDOBJECTFIRST         START_WP_STRINGS+142
#define IDS_WPCLSFINDOBJECTNEXT          START_WP_STRINGS+144
#define IDS_WPCLSINITDATA                START_WP_STRINGS+146
#define IDS_WPCLSMAKEAWAKE               START_WP_STRINGS+148
#define IDS_WPCLSNEW                     START_WP_STRINGS+150
#define IDS_WPCLSQUERYDEFAULTVIEW        START_WP_STRINGS+152
#define IDS_WPCLSQUERYDETAILS            START_WP_STRINGS+154
#define IDS_WPCLSQUERYDETAILSINFO        START_WP_STRINGS+156
#define IDS_WPCLSQUERYFOLDER             START_WP_STRINGS+158
#define IDS_WPCLSQUERYICON               START_WP_STRINGS+160
#define IDS_WPCLSQUERYICONDATA           START_WP_STRINGS+162
#define IDS_WPCLSQUERYINSTANCEFILTER     START_WP_STRINGS+164
#define IDS_WPCLSQUERYINSTANCETYPE       START_WP_STRINGS+166
#define IDS_WPCLSQUERYOBJECT             START_WP_STRINGS+168
#define IDS_WPCLSQUERYOPENFOLDERS        START_WP_STRINGS+170
#define IDS_WPCLSQUERYSETTINGSPAGESIZE   START_WP_STRINGS+172
#define IDS_WPCLSQUERYSTYLE              START_WP_STRINGS+174
#define IDS_WPCLSQUERYTITLE              START_WP_STRINGS+176
#define IDS_WPCLSUNINITDATA              START_WP_STRINGS+178
