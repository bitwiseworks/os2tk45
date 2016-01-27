:userdoc.
.***********************************************************************
.*General help for the Caption Sample Application Program
.***********************************************************************
:h1 res=001.Help for Caption Sample Application
:i1 id=CAPSAMP.Help for Caption Sample Application
.*
:p.
Welcome to MMPM/2's sample captioning system.  We call it our sample
captioning system because we've provided it, source code and all, as an
example of one way to implement captioning with MMPM/2.  The system can be
used as it's provided, or it can be modified to suit your specific needs.
:p.
This sample captioning system is made up of three parts.  The first part of
the system is the Caption Creation Utility.  This allows you to synchronize
an audio file with a text file.  The second part of the system is the Caption
DLL.  This DLL provides three api's that allow an application to display,
manage, and hide a caption window.  The third part of the system is this
application, the Caption Sample Application.  It shows programmers how to
use the three api's provided by the Caption DLL in order to display a caption
window in an application.
.*
:p.
Select the :hp2.Play:ehp2. push button to start playing the audio file.
If MMPM/2's caption flag is selected (in the Setup applet) then a caption
window will be displayed
in the application's window as the audio file plays.
While the audio file plays,
the text in the caption window will scroll in syncronization
with the audio file.
:p.
Select the :hp2.Stop:ehp2. push button to stop playing an audio file.
If the caption window was displayed while the audio file was playing, then
it will be hidden when the audio file stops.
.*
:p.
Select the :hp2.Pause:ehp2. push button to pause the playing of an audio file.
To resume playing the audio file,
select the :hp2.Pause:ehp2. or :hp2.Play:ehp2. push button.
.*
:p.
Select the :hp2.Rewind:ehp2. push button to stop the audio file and
seek to it's beginning.
.*
:p.
The :hp2.Audio position:ehp2. control provides a graphic display of the
current position of the audio file.  To change position in the audio file,
drag the
slider bar or select the buttons at the left end of the slider.
.*
:p.Adjust the audio volume of the currently loaded audio file by using
the :hp2.Volume:ehp2. control.
.*
:p.
The caption window may be dragged around the application's window.
To accomplish this,
click button one while the mouse is over the caption window, drag the window to
the desired location and release the mouse button.
.*
:p.
Select the :hp2.Options:ehp2. menu to configure the captioning system.
.*
:p.
Select the :hp2.Help:ehp2. menu to access various help functions.
.*
.*************************************************************************
.* Help for the Settings dialog box
.*************************************************************************
:h1 res=002.Help for Settings Dialog Box
:i1 id=DIALOG.Help for Setting Dialog Box
.*
:p.
Select the :hp2.Text columns:ehp2. spin button to change the number of
characters in each line of text in the caption window (ie. the horizontal size
of the caption window).
.*
:p.
Select the :hp2.Text rows:ehp2. spin button to change the number of text rows
displayed in the caption window (ie. the height of the caption window).
.*
:p.
Select the :hp2.Background color:ehp2. spin button to change the
background color of the caption window.
.*
:p.
Select the :hp2.Text color:ehp2. spin button to change the text color in the
caption window.
.*
:p.
Select the :hp2.Window position:ehp2. spin button to change the caption's
window position inside of the application.
.*
:p.
Select the :hp2.OK:ehp2. pushbutton to save the current state of the spin buttons
and to remove this pop-up window.
.*
:p.
Select the :hp2.Cancel:ehp2. pushbutton to to remove this pop-up window without
saving the changes made to the spin buttons.
.*
:p.
Select :hp2.Help:ehp2. pushbutton to display this help panel.
.*
.***********************************************************************
.*Keys help for the Caption Sample Application Program
.***********************************************************************
:h1 res=003.Help for Caption Sample Application Keys
:i1 id=KEYS.Help for Caption Sample Application Keys
.*:i1.keys help
:p.The following keys can be used to control the Caption Sample Application
program&colon.
:dl tsize=5 break=all.
:dt.:hp2.Tab:ehp2.
:dd.Move among choices.
:dt.:hp2.Alt:ehp2.
:dd.Switch to or from the menu of an application window.
:dt.:hp2.Up:ehp2. and :hp2.Down:ehp2. arrow keys
:dd.Move among the push buttons, when focus is on the push button.
:dt.:hp2.Left:ehp2. and :hp2.Right:ehp2. arrow keys
:dd.Move among the push buttons, when focus is on the push button.
When focus is on the volume control use left arrow key to decrease,
right arrow key to increase the volume level.
:dt.:hp2.F1:ehp2.
:dd.Obtain help for the Caption Sample Application program.
:edl.
:euserdoc.
