.*==============================================================*\
.*                                                              *
.* Main.ipf - Information Tag Language file for the             *
.*              application's help manager                      *
.*  Copyright 1993 IBM Corporation                              *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  General Information                                         *
.*--------------------------------------------------------------*/
:h1 res=2000 name=PANEL_MAIN.About Browse-O-Matic
:i1 id=aboutMain.About Browse-O-Matic
:p.The Browse-O-Matic displays file system objects as a hex dump or text
dump. The hex dump displays the offset into the object, 16 bytes of data
as hex values, followed by the ASCII representation of the 16 bytes. The
text dump displays the object in it's ASCII representation only. 

:p.The user may change the fonts used by the Browse-O-Matic through OS/2's
Font Palette. The user may independently change the title bar, menu bar, and 
main window's fonts by dragging fonts from the Font Palette and dropping them 
on the Browse-O-Matic object. The fonts are saved when the Browse-O-Matic is 
closed. 

.*--------------------------------------------------------------*\
.*  Help menu Save command help panel                           *
.*--------------------------------------------------------------*/
:h1 res=3000 name=PANEL_HELPKEYS.Keys 
:i1 id=helpKeys.Keys Help
:p.:hp2.Up Arrow:ehp2. - Scrolls the window up one line.
:p.:hp2.Down Arrow:ehp2. - Scrolls the window down one line.
:p.:hp2.Left Arrow:ehp2. - Scrolls the window left.
:p.:hp2.Right Arrow:ehp2. - Scrolls the window right.
:p.:hp2.Page Up:ehp2. - Scrolls the window up one page.
:p.:hp2.Page Down:ehp2. - Scrolls the window down one page.
:p.:hp2.Ctrl+Left Arrow:ehp2. - Scrolls the window left one page.
:p.:hp2.Ctrl+Right Arrow:ehp2. - Scrolls the window right one page.
:p.:hp2.Home:ehp2. - Positions at the beginning of the line.
:p.:hp2.End:ehp2. - Positions at the end of the line.
:p.:hp2.Ctrl+Home:ehp2. - Positions at the top of the object.
:p.:hp2.Ctrl+End:ehp2. - Positions at the bottom of the object.

.*--------------------------------------------------------------*\
.* Help for the product information view.                       *
.*--------------------------------------------------------------*/
:h1 res=4000 name=PANEL_PROD_INFO_DLG.Product Information Okay
:p.Select OK to close the Product Information view.

.*--------------------------------------------------------------*\
.* Select filename dialog box                                   *
.*--------------------------------------------------------------*/
:h1 res=5000 name=PANEL_SELECT_FILE.Select File System Object
:p.Select the file system object, then choose Browse to view the object.

.*--------------------------------------------------------------*\
.*  Help for Views                                              *
.*--------------------------------------------------------------*/
:h1 res=6000 name=PANEL_HELP_OPEN_TEXT.Open Text View
:p.Browse the file system object in the text view mode.

:h1 res=6010 name=PANEL_HELP_OPEN_HEX.Open Hex View
:p.Browse the file system object in the hex view mode.

:h1 res=6020 name=PANEL_HELP_OPEN_PROD_INFO.Product Information
:p.Display Product Information for the Browse-O-Matic.

:euserdoc.
