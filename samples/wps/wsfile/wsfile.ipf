.*==============================================================*\
.*                                                              *
.* wsfile.ipf - Information Tag Language file for the WSFILE    *
.*                                                              *
.*==============================================================*/
:userdoc.

.*--------------------------------------------------------------*\
.*  Default Help for WSFILE   class                             *
.*      res = ID_HELP_DEFAULT                                   *
.*--------------------------------------------------------------*/
:h1 res=256 name=ID_HELP_DEFAULT.WSFILE Class -- Help
:i1 id=aboutMain.About WSFILE Class
:lines.
This object is an instance of WSFILE Class,
a subclass of WPDataFile.


The following behaviors describe what is unique
about an instance of WSFILE:

   - displays a unique icon
   - displays a unique title
   - displays its own unique help
   - non tempatable
   - adds an item to Open-> submenu to open its own
     view
   - sets default open behavior to its own view
   - adds an item to popup menu that beeps when
     pressed
   - removes "Create Shadow" from popup menu
   - inserts default text into data file contents
     when created
   - can only be dropped into folders of class
     WSFOLDER
   - adds page to settings notebook to allow entry
     of name of last person to work on file (Last
     Worker)
   - updates all views of class details when Last
     Worker field is modified
   - defines its own setup string to Sound Alarm
     when passed through the wpSetup, WinSetObjectData
     and SysSetObjectData APIs
:elines.


.*--------------------------------------------------------------*\
.*  Open Submenu item                                           *
.*      res = ID_HELP_OPENWSFILE
.*--------------------------------------------------------------*/
:h1 res=257 name=ID_HELP_OPENWSFILE.Open Special File View
:i1 id=Open.Special View
:p.This is a special, view of a wsfile's contents.

.*--------------------------------------------------------------*\
.*  Instance variable help                                      *
.*      res = ID_HELP_LASTWORKER
.*--------------------------------------------------------------*/
:h1 res=258 name=ID_HELP_LASTWORKER.Last Worker Instance Variable
:i1 id=IV1.LastWorker
:p.Last worker is the name of the person to last work on this file.

.*--------------------------------------------------------------*\
.*  Context Menu Item help                                      *
.*      res = ID_HELP_SOUNDALARM
.*--------------------------------------------------------------*/
:h1 res=259 name=ID_HELP_SOUNDALARM.Sound an alarm
:i1 id=IV2.SoundAlarm
:p.Pressing this menu option will sound several beeps.

:euserdoc.
