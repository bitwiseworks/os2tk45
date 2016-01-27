.*==============================================================*\
.*                                                              *
.* Dragwnd.ipf - Information Tag Language file for the Window   *
.*         menu help panels.                                    *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main file menu                                              *
.*      res = PANEL_WINDOW                                      *
.*--------------------------------------------------------------*/
:h1 res=2310 name=PANEL_WINDOW.Window Menu
:i1 id=Window.Window Menu
:p.The Window menu contains commands that you use to change the
files displayed or selected in the main window.

.*--------------------------------------------------------------*\
.*  File menu Open command help panel                           *
.*      res = PANEL_WINDOWREFRESH                               *
.*--------------------------------------------------------------*/
:h1 res=2320 name=PANEL_WINDOWREFRESH.Refresh
:i2 refid=File.Refresh
:p.Choose the menu item :hp3.Refresh:ehp3. to restore the original list of
files and subdirectories.

.*--------------------------------------------------------------*\
.*  File menu Open command help panel                           *
.*      res = PANEL_WINDOWSELECTALL                             *
.*--------------------------------------------------------------*/
:h1 res=2330 name=PANEL_WINDOWSELECTALL.Selectall
:i2 refid=Window.Selectall
:p.Choose the menu item :hp3.Select all:ehp3. to mark all of the files and
subdirectories whose names are displayed.

.*--------------------------------------------------------------*\
.*  File menu Open command help panel                           *
.*      res = PANEL_WINDOWDESELECTALL                             *
.*--------------------------------------------------------------*/
:h1 res=2340 name=PANEL_WINDOWDESELECTALL.Deselectall
:i2 refid=Window.Deselectall
:p.Choose the menu item :hp3.Deselect all:ehp3. to unmark all of the files and
subdirectories whose names are displayed.

