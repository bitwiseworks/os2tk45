.*==============================================================*\
.*                                                              *
.* Help.ipf - Information Tag Language file for the Help menu   *
.*              help panels.                                    *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main Help menu                                              *
.*      res = PANEL_HELP                                        *
.*--------------------------------------------------------------*/
:h1 res=5000 name=PANEL_HELP.Help Menu
:i1 id=Help.Help Menu
:p. Refer to standard help menu for  help on the sample clock
    application


.*--------------------------------------------------------------*\
.*  Help menu New command help panel                            *
.*      res = PANEL_HELPHELPFORHELP                             *
.*--------------------------------------------------------------*/
:h1 res=5100 name=PANEL_HELPHELPFORHELP.Help For Help
:i2 refid=Help.Help for Help
:p. Refer to standard help menu for help on the sample clock
application



.*--------------------------------------------------------------*\
.*  Help menu Save command help panel                           *
.*      res = PANEL_HELPKEYS                                    *
.*--------------------------------------------------------------*/
:h1 res=5200 name=PANEL_HELPKEYS.Keys
:i2 refid=Help.Keys
:p. Alt F toggles the display controls
:p. Alt H activates  the help menu
:p. Alt V activates  the view menu
:p. Alt S activates  the settings menu


.*--------------------------------------------------------------*\
.*  Help menu Save As command help panel                        *
.*      res = PANEL_HELPINDEX                                   *
.*--------------------------------------------------------------*/
:h1 res=5300 name=PANEL_HELPINDEX.Index
:i2 refid=Help.Index
:p. Refer to standard help keys for the sample clock application.


.*--------------------------------------------------------------*\
.*  Help menu About command help panel                          *
.*      res = PANEL_HELPABOUT                                   *
.*--------------------------------------------------------------*/
:h1 res=5400 name=PANEL_HELPABOUT.Product Information
:i2 refid=Help.Product Information
:p. Sample clock program Copyright 1992, IBM Corp.
