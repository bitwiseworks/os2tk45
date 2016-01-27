.*==============================================================*\
.*                                                              *
.* Help.ipf - Information Tag Language file for the Help menu   *
.*              help panels.                                    *
.*                                                              *
.*  Copyright 1991, IBM Corp.                                   *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main Help menu                                              *
.*      res = PANEL_HELP                                        *
.*--------------------------------------------------------------*/
:h1 res=2910 name=PANEL_HELP.Help Menu
:i1 id=Help.Help Menu
:p.Refer to standard Help menu for help on the sample queue
   application.

.*--------------------------------------------------------------*\
.*  Help menu New command help panel                            *
.*      res = PANEL_HELPHELPFORHELP                             *
.*--------------------------------------------------------------*/
:h1 res=2920 name=PANEL_HELPHELPFORHELP.Help For Help
:i2 refid=Help.Help for Help
:p.Refer to standard Help menu for help on the sample queue
   application.


.*--------------------------------------------------------------*\
.*  Help menu Save command help panel                           *
.*      res = PANEL_HELPKEYS                                    *
.*--------------------------------------------------------------*/
:h1 res=2940 name=PANEL_HELPKEYS.Keys
:i2 refid=Help.Keys
:p.Refer to standard Help menu for help on the sample queue
   application.


.*--------------------------------------------------------------*\
.*  Help menu Save As command help panel                        *
.*      res = PANEL_HELPINDEX                                   *
.*--------------------------------------------------------------*/
:h1 res=2950 name=PANEL_HELPINDEX.Index
:i2 refid=Help.Index
:p.Refer to standard Help menu for help on the sample queue
   application.


.*--------------------------------------------------------------*\
.*  Help menu About command help panel                          *
.*      res = PANEL_HELPABOUT                                   *
.*--------------------------------------------------------------*/
:h1 res=2960 name=PANEL_HELPABOUT.About
:i2 refid=Help.Product Information
:p.Sample queue program Copyright 1991, IBM Corp.
