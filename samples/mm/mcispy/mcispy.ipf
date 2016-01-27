:userdoc.
:title.Multimedia MCI Message Spy
.*:body.
.*--------------------------------------------------------
:h1 res=29 name=GENHLP.Multimedia MCI Message Spy (MCISPY)
:i1 id=GENHLP.MultiMedia MCI Message Spy
:p.The Multimedia MCI Message Spy (MCISPY) is a sample program that
can be used to monitor Media Control Interface (MCI) messages that
are exchanged between various devices in the OS/2 Multimedia
subsystem. The application is used to understand the device
interactions to application requests in the MMPM/2 subsystem.
In order to begin monitoring messages select Spy On pull down
from the menu bar and choose start spying. Messages will be
intercepted only after this choice is made.
A description of how MCISPY traps MCI messages Follows.
:cgraphic.



                        ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                        ³ Shared Memory  ³
                        ³  (Named)       ³
                        ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                     ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
       APIs Exported    ³                ³                     ³              ³
                        ³                ³                     ³  True MDM    ³
                        ³  Stub or       ³                     ³  renamed     ³
       mciSendCommand() ³  Proxy         ³ Forwarder entry     ³  to MCI.DLL  ³
       ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  MDM.DLL       ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ containing   ³
       mciSendString()  ³                ³ Forwarder entry     ³ the API      ³
       ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ maintain Open  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´ workers.     ³
       mdmDriverNotify()³ Device Ids     ³ Forwarder entry     ³              ³
       ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´              ³
                        ³ Notify Spy     ³                     ³              ³
                        ³ Application    ³                     ³              ³
                        ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                     ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
                       ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                       ³ Shared Memory   ³
                       ³  (Named)        ³
                       ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
                       ³                 ³
                       ³  MCISPY         ³
                       ³  Application    ³
                       ³                 ³
                       ³                 ³
                       ³                 ³
                       ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
:ecgraphic.
:figcap.MDM and MCI Message monitoring application interaction.
:p.
The above diagram depicts a high level view of how MCI messages are
monitored. MDM is the MCI interface provider. The MCISPY application
comprises of an EXE (MCISPY.EXE) and a corresponding DLL pair.
The above diagram depicts the MCISPY application implementation model. The
diagram shows a stub or proxy DLL which  replaces the true MDM.DLL
renaming existing MDM to MCI. The proxy MDM exports the same APIs at
the same ordinals as true MDM. The stub or proxy MDM has forwarder entries
into the true MDM (renamed to MCI.DLL) for API workers, ie it exports the
APIs and also imports the same APIs from the true MDM.
:p.
MCISPY monitors both the MCI string interface and the MCI command interface.
The display consits of originating Process ID (PID), device Id, MCI message
Flags, User parameter and Data. The above fields are displayed for the
MCI command interface. For the string interface the PID and the string input
is displayed.
:p.
The MCISPY application has 5 groups of possible user actions provided
for by the application. The user menu has the following groups of
choices. :hp2.Logfile:ehp2. enables the user to select a logfile to log
the displayed messages. The menubar pull down choice :hp2.Enable message
Logging:ehp2. item must be selected to turn on message logging.
The :hp2.Edit:ehp2. group of menu permit editing
operations on the displayed screen contents. Copy,Delete,Discard, and
Font operations on the application screen contents are permitted.
The :hp2.View:ehp2. permits viewing of installed multimedia devices in
the system and also provides a simple MCI Flag parser. The user enters
the Flag value (in hex) and selects a MCI message and the parser displays
what the flag combination is.
The :hp2.Filter:ehp2. pulldown has various means by which messages can
be filtered. The Disable choices (String Interface,Command Interface, and
Driver Notifications) respectively filter inputs from the above APIs. Filtering
by MCI Messages is permitted. If MCI_OPEN is filtered then the application
will not see MCI_OPEN messages at all till it is turned of. To turn off reinvoke
the Filter by Messages choice and select or deslect the message. Filter by
Device types allows a device type filter which behaves similarly. Filtering
by active device Ids is also permitted. There are provisions to disable
frequently occurring timer messages like MCI_SETPOSITION_ADVISE.
MCISPY also displays error return codes as they occur.
:p.
To enable logging of messages to a file select enable message logging
from the menu bar under :hp2.LogFile:ehp2.. Messages will be written
to the default log file "MCISPY.LOG". Select Open to specify a different
log file.
:p.
The :hp2.Edit:ehp2. choices are intended for clearing messages on the screen
and other related operations. Copy is supported. Clear All
clears all displayed messages. Select text first before copying. Set font
sets the window font.
.*--------------------------------------------------
:h1 res=23 name=KEYSHELP.Help for MCISPY Keys
:p.The following keys can be used to control the MCISPY App
:dl.
:dt.Alt+L
:dd.Select :hp2.LogFile:ehp2. from menu bar.
:dt.Alt+E
:dd.Select :hp2.Edit:ehp2. from menu bar.
:dt.Alt+V
:dd.Select :hp2.View:ehp2. from menu bar.
:dt.Alt+F
:dd.Select :hp2.Filter:ehp2. from menu bar.
:dt.Alt+H
:dd.Select :hp2.Help:ehp2. from menu bar.
:edl.
.*--------------------------------------------------------
:h1 res=30 name=FILTERMSGHLP.MCISPY Filter MCI Messages
:i1 id=FILTERMSGHLP.Filter By MCI Messages.
:p.You can filter or exclude messages you do not want to see. The
list box contains the list of all Multimedia MCI messages. You can select
the message(s) you want to :hp2.exclude:ehp2.(not displayed) and select the
"OK" pushbutton.
:p.
Initially no message filter is in effect and hence all
MCI messages are displayed. The "All" pushbutton selects all items in the
listbox. The "None" pushbutton deselects all items in the listbox. The
"Timer on" pushbutton selects the timer messages like MM_MCIPOSITIONCHANGE.
The "Timer Off" pushbutton deselects the timer messages.
the "Actions On" pushbutton selects the MCI Actions group of messages like
MCI_PLAY, MCI_RECORD, MCI_PAUSE, MCI_RESUME, MCI_SEEK, and MCI_STOP.
The 'Actions Off" pushbutton deselects the actions group. To cancel any
existing message filter in effect invoke the dialog again and exit making
a new selection or without selecting anything. All Internal Filter members
for the particular type are reset on entry to the dialog.
:p.
.*--------------------------------------------------------
:h1 res=31 name=FILTERDEVTYPHLP.MCISPY Device Type Filter
:i1 id=FILTERDEVTYPHLP.Filter By Device Types.
:p.You can filter or exclude messages you do not want to see for a particular
device. If the device type filter is in effect for a particular device
you will not see any messages (including String interface) for that device.
:hp2.CAVEAT:ehp2. If the device was automatically opened by using an element
to identify the device then the filtering may not work.
list box contains the list of all Multimedia device types . You can select
the device you want to :hp2.exclude:ehp2.(not displayed) and select the
"OK" pushbutton. To cancel any existing device type filter invoke the
dialog again and exit without choosing anything.
.*--------------------------------------------------------
:h1 res=32 name=FILTERDEVIDHLP.MCISPY Device Id  or Alias Filter
:i1 id=FILTERDEVIDHLP.Filter By Device Id or Alias.
:p.You can filter or exclude messages you do not want to see for a particular
device Id or alias. If the device Id filter is in effect for a particular device Id
you will not see any messages for that device id or alias.
:hp2.CAVEAT:ehp2. If the device was automatically opened by using an element
to identify the device then the filtering may not work.
list box contains the list of all Multimedia device Id(s) and alias(es) currently open .
You can select the device Id or alias you want to :hp2.exclude:ehp2.(not displayed) and select the
"OK" pushbutton. To cancel any existing device Id or alias filter invoke the
dialog again and exit without choosing anything or something else.
.*--------------------------------------------------------
:h1 res=33 name=SHOWFLAGHLP.MCISPY Display MCI Flag values
:i1 id=SHOWFLAGHLP.Display MCI Flag Values.
:p.
MCISPY Displays Flag values in hex as seen by the mciSendCommand interface.
For certain predetermined messages you can see the textual representation
of the hex value. For example on MCI_ACQUIRE message a flag value of 102(hex)
means MCI_EXCLUSIVE and MCI_WAIT.
Enter the value of the flag in the entry field at the top (in hex). Select
the message for which the flag value was used. Select :hp2.Display:ehp2. to
display the flag values in the :hp2.Results:ehp2. list box. Use :hp2.Clear:ehp2.
to clear the contents of the Results List box.
To dismiss dialog select :hp2.cancel:ehp2. In order to add additional messages and flag
values modify the source code provided.
.*--------------------------------------------------------
:h1 res=34 name=INSTDEVHLP.MCISPY View Installed Multimedia Devices
:i1 id=INSTDEVHLP.View Installed Multimedia Devices
:p.
This dialog displays all the installed multimedia devices in the system.
The display includes logical device, physical device and product information
associated with each device.

.*--------------------------------------------------------
:h1 res=35 name=STDHLP.MCISPY Open Log File
:i1 id=STDHLP.Open Log File
:p.You can open a Log file on any drive or in any directory
by using this dialog. Opening of the Log file merely provides
a file name to the application. Subsequently if logging is enabled
all messages will be logged to the file just opened. The default Log
file is MCISPY.LOG in the current directory.
Select OK to dismiss the dialog.
:euserdoc.

.*--------------------------------------------------------
