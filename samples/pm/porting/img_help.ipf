.*==============================================================*\
.*                                                              *
.* img_help.ipf - Information Tag Language file the screens for *
.*              help on the Help menu                           *
.* Copyright 1991 IBM Corporation                               *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main Help menu                                              *
.*      res = PANEL_HELP                                        *
.*--------------------------------------------------------------*/
:h1 res=2910 name=PANEL_HELP.Help Menu
:i1 id=Help.Help Menu
:p.Insert standard Help menu text here.

.*--------------------------------------------------------------*\
.*  Help menu New command help panel                            *
.*      res = PANEL_HELPUSINGHELP                               *
.*--------------------------------------------------------------*/
:h1 res=2920 name=PANEL_HELPUSINGHELP.Using Help
:i2 refid=Help.Using Help
:p. Place information for the Help Using Help menu here.


.*--------------------------------------------------------------*\
.*  Help menu Open command help panel                           *
.*      res = PANEL_HELPGENERAL                                 *
.*--------------------------------------------------------------*/
:h1 res=2930 name=PANEL_HELPGENERAL.General
:i2 refid=Help.General
:p.Place information for the General Help menu item here.
:note.This screen will usually be handled by the default.


.*--------------------------------------------------------------*\
.*  Help menu Save As command help panel                        *
.*      res = PANEL_HELPINDEX                                   *
.*--------------------------------------------------------------*/
:h1 res=2950 name=PANEL_HELPINDEX.Index
:i2 refid=Help.Index
:p.Place information for the Help Index menu item here.


.*--------------------------------------------------------------*\
.*  Help menu Product information command help panel            *
.*      res = PANEL_HELPPRODUCTINFO                             *
.*--------------------------------------------------------------*/
:h1 res=2960 name=PANEL_HELPPRODUCTINFO.Product information
:i2 refid=Help.Product information
:p.The :hp1.Product information:ehp1. menu item allows you to open a
dialog which gives copyright information about this sample.
