.*===============================================================*\
.*                                                               *
.* wsfolder.ipf - Information Tag Language file for the WSFOLDER *
.*                                                               *
.*===============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Default Help for WSFOLDER class                             *
.*      res = ID_HELP_DEFAULT                                   *
.*--------------------------------------------------------------*/
:h1 res=256 name=ID_HELP_DEFAULT.WSFOLDER Class -- Help
:i1 id=aboutMain.About WSFOLDER Class
:lines.
This object is an instance of Class WSFOLDER,
a subclass of WPFolder.

The following behaviors describe what is unique
about an instance of WSFOLDER:

   - displays its own unique open and closed icons
   - displays a unique title
   - untemplatable
   - opens in details view
   - displays the details column of the WSFILE class
     (Last Worker)
   - has an added item on popup menu "Close all
     but 1st view" which does just what it says
   - has its own help
   - sorts on details column of the WSFILE class
     (Last Worker)
   - pops up a message box reporting when an object
     has been added to or removed from it
   - add "Print" back to menu (and changes style to
     be able to print) and pops up a message box to
     say that its contents are printing.
   - only accepts drops of objects of class WSFILE
   - redefines default drop action to be COPY
   - sets default concurrent view behavior to allow
     multiple views of itself to be open at one time
:elines.


.*--------------------------------------------------------------*\
.*  Context Menu Item help                                      *
.*      res = ID_HELP_CLOSEVIEWS
.*--------------------------------------------------------------*/
:h1 res=257 name=ID_HELP_CLOSEVIEWS.Close Views Menu Item
:i1 id=IV.CloseViews
:p.When this context menu item is chosen, all currently open views
of this folder will be closed, except for the view that was
initially opened.  If no views are currently open, then no action will
be taken.

:euserdoc.
