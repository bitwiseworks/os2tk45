.*==============================================================*\
.*                                                              *
.* img_view.ipf - Information Tag Language file the screens for *
.*              help on the View menu                           *
.* Copyright 1991 IBM Corporation                               *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main View menu                                              *
.*      res = PANEL_VIEW                                        *
.*--------------------------------------------------------------*/
:h1 res=2310 name=PANEL_VIEW.View Menu
:i1 id=View.View Menu
:p.The :hp1.View:ehp1. pulldown menu offers ways of changing the
appearance of the image being displayed.

.*--------------------------------------------------------------*\
.*  View menu Detail command help panel                         *
.*      res = PANEL_VIEWDETAIL                                  *
.*--------------------------------------------------------------*/
:h1 res=2320 name=PANEL_VIEWDETAIL.Detail
:i2 refid=View.Detail
:p.The :hp1.View Detail:ehp1. menu item allows you to display
any portion of the
image in a sizable window with scroll bars.

.*--------------------------------------------------------------*\
.*  View menu Color command help panel                          *
.*      res = PANEL_VIEWCOLOR                                   *
.*--------------------------------------------------------------*/
:h1 res=2330 name=PANEL_VIEWCOLOR.Color
:i2 refid=View.Color
:p.The :hp1.View Foreground/Background Color:ehp1. menu item allows
you to change the foreground or background color of the image to any
of six color values.

.*--------------------------------------------------------------*\
.*  View menu SavePosition command help panel                   *
.*      res = PANEL_VIEWSAVEPOSITION                            *
.*--------------------------------------------------------------*/
:h1 res=2340 name=PANEL_VIEWSAVEPOSITION.SavePosition
:i2 refid=View.SavePosition
:p.The :hp1.View Save position:ehp1. menu item will
 save the current position of the image in the viewing window.

.*--------------------------------------------------------------*\
.*  View menu RestorePosition command help panel                *
.*      res = PANEL_VIEWRESTOREPOSITION                         *
.*--------------------------------------------------------------*/
:h1 res=2350 name=PANEL_VIEWRESTOREPOSITION.RestorePosition
:i2 refid=View.RestorePosition
:p.The :hp1.View Restore position:ehp1. menu item will
 restore the previously-saved position of the image in the viewing window.
