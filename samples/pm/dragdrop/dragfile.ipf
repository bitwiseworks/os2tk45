.*==============================================================*\
.*                                                              *
.* Dragfile.ipf - Information Tag Language file for the         *
.*         Directory menu help panels.                          *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main file menu                                              *
.*      res = PANEL_FILE                                        *
.*--------------------------------------------------------------*/
:h1 res=2210 name=PANEL_FILE.Directory Menu
:i1 id=File.Directory Menu
:p.The Directory menu contains a command that you use to change the
directory for which files are listed in the main window.

.*--------------------------------------------------------------*\
.*  Directory menu Select command help panel                    *
.*      res = PANEL_FILEOPEN                                    *
.*--------------------------------------------------------------*/
:h1 res=2220 name=PANEL_FILEOPEN.Select
:i2 refid=File.Select
:p.Choose the menu item :hp3.Select...:ehp3. to open a dialog in
which you will enter the name of the new directory.

