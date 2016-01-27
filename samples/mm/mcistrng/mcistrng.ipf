:userdoc.
:h1 res=001.Help for String Test
:p.
The String Test sample program allows the user to control any media device
that is
installed on the system.  String Test
accepts Media Control Interface string commands from the user, and
passes them on to the appropriate multimedia component for execution.
There are three ways to pass string commands to MMPM/2 using String Test.
:p.
The first way
is to type a string command in the
the :hp2.String command:ehp2. entry field on
the String Test main window.
The valid format for a string command is <COMMAND> <DEVICE> <PARAMETERS>.
For example, the following three string commands will play a waveform
file that resides in the current directory:
:ul compact.
:li."open filename.wav alias xxx"
:li."play xxx notify"
:li."close xxx"
:eul.
:p.
Select the :hp2.Send:ehp2. push button to send the string command
to
the appropriate multimedia component for execution.  In the example above,
pressing the :hp2.Send:ehp2. push button after each string command
would send each command
to the wave audio device.
Selecting the :hp2.Send:ehp2. push button will also add the string command
to the list box below the :hp2.String command:ehp2.
entry field.  The user may scroll through this list box and select a command
from it in order to complete the :hp2.String command:ehp2. entry field.
:p.
The second way to send string commands
is to specify a batch file of string commands as a command line parameter
when starting String Test from an OS/2 command prompt.
The batch file of string commands is simply a text file, with any valid
filename, that has one string
command on each line.
:p.
For example, if the name of the
batch file is test.dat, the correct way to start String Test
and have it execute the string commands in test.dat would be to type
"mcistrng test.dat"
from an OS/2 command line.
String Test will process each string command in the batch file as if it
had been typed in the :hp2.String command:ehp2. entry field
on String Test's main window.
:p.
The third way to send string commands using String Test
also involves a batch file of string commands.
In addition to executing a batch file of string commands by
specifying a command line parameter to String Test, a batch
file of string commands may also be executed by
selecting the :hp2.Batch file:ehp2. push button on String Test's main window.
When this push button is selected, a dialog box will be displayed allowing
a file to be selected.  The selected file can then be executed as a batch
file of String Commands.
:p.
For more information about string commands, please refer to the MMPM/2
Programmer's
Reference.
:p.
Select the :hp2.Send:ehp2. push button to send the string command that was
just entered in the :hp2.String command:ehp2. entry field to
the appropriate multimedia component for execution.
:p.
Select the :hp2.Display messages:ehp2. push button to display a pop-up
window containing notification messages that String Test
is receiving from media devices.
:p.
Select the :hp2.Batch file:ehp2. push button to display a pop-up window
that allows a batch file of string commands to be selected and executed.
:p.
Select the :hp2.Cancel:ehp2. push button to end the application.
:p.
Select the :hp2.Help:ehp2. push button to view this help panel.
.*********************************************************************
:h1 res=002.Help for Display Messages
:p.The Media Control Interface returns asynchronous response messages
to applications to indicate
such things as completion of a media device function or the passing of
ownership of a media device from one process to another.
:p.
Notification messages that are received by the String Test utility are
shown in the :hp2.Notification messages:ehp2. list.  This list is only for
displaying information to the user.  It does not allow any selections
to be made.
:p.
Select the :hp2.Include:ehp2. push button to display a pop-up
window which allows the user to filter the notification messages that
are displayed in this window.
:p.
Select the :hp2.Cancel:ehp2. push button to remove this pop-up window.
:p.
Select the :hp2.Help:ehp2. push button to view this help panel.
.**********************************************************************
:h1 res=003.Help for Include Messages
:p.
Filter the incoming notification messages to be shown in the
:hp2.Display Messages:ehp2. pop-up window by checking the check
boxes next to the corresponding messages to be shown.
If none of these check boxes are selected then no messages will be shown
in the :hp2.Display Messages:ehp2. pop-up window.
:p.
An MM_MCINOTIFY message is returned to an application when a device
successfully completes the action indicated by the message, or when
an error occurs.
:p.
An MM_MCIPASSDEVICE message notifies an application that the use of a media
device
is being gained or lost.
:p.
An MM_MCIPOSITIONCHANGE message notifies an application of the current media
position of the media device.  This message is generated periodically when the
MCI_SET_POSITION_ADVISE message is sent to the media device.
:p.
An MM_MCICUEPOINT message notifies an application that the device has
encountered a cuepoint.
:p.
An MM_MCIPLAYLISTMESSAGE message notifies the application that the playlist
processor has encountered a MESSAGE instruction.
:p.
An MM_MCIEVENT message notifies an application of an event generated by a
device.
:p.
Select the :hp2.Include:ehp2 push button after selecting the desired
checkboxes.  This action will set the appropriate filters for the
:hp2.Display Messages:ehp2. pop-up window and will remove this pop-up
window.
:p.
Select the :hp2.Cancel:ehp2. remove this pop-up window without saving any
of the checkbox selections made.
:p.
Select the :hp2.Help:ehp2. push button to view this help panel.
.**********************************************************************
:h1 res=004.Help for Open and execute batch file
:p.
Choose the desired batch file of string commands from the pop-up
window, and then select the :hp2.Execute batch file:ehp2. push button
to execute the selected batch file.
:euserdoc.
