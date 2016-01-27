:userdoc.

.***********************************************************************
.*General help for the Caption Creation Utility
.***********************************************************************

:h1 res=10500 nosearch noprint.System overview
:i1.System overview
Welcome to MMPM/2's sample captioning system.  We call it our sample
captioning system because we've provided it, source code and all, as an
example of one way to implement captioning with MMPM/2.  The system can be
used as it's provided, or it can be modified to suit your specific needs.
:p.
This sample captioning system is made up of three parts.  The first part of
the system is this application, the Caption Creation Utility.  This allows
you to synchronize an audio file with a text file.  The second part of the
system is the Caption DLL.  This DLL provides three api's that allow an
application to display, manage, and hide a caption window.  The third part
of the system is Caption Sample Application.  It shows programmers how to
use the three api's provided by the Caption DLL in order to display a caption
window in an application.
:sl.
:li.To add captions to an audio file&colon.
:ol.
:li.Choose an audio file by selecting :hp2.Open audio...:ehp2. in the :hp2.File:ehp2. submenu.
:li.Choose a text file by selecting :hp2.Open text...:ehp2. in the :hp2.File:ehp2. submenu.
:li.Select :hp2.Start timing:ehp2. to start playing the audio.
:li.As the audio plays, select :hp2.Advance line:ehp2. to scroll the text to match the audio.
:li.Once the audio has completed, select :hp2.Review:ehp2. to check the timing.
:li.You may repeat steps :hp2.3:ehp2. through :hp2.5:ehp2. above until the timing is satisfactory.
:li.If you wish to retime just a portion of the file, use the :hp2.Audio position:ehp2.
slider bars to locate the part you wish to retime, and then
follow steps :hp2.3:ehp2. through :hp2.5:ehp2 above.
:li.Select :hp2.Save as...:ehp2. from the :hp2.File:ehp2. submenu to save the caption file.
:eol.
:li.To review an existing Caption file&colon.
:ol.
:li.Choose an Audio file by selecting :hp2.Open audio...:ehp2. in the :hp2.File:ehp2. submenu.
:li.Choose a Caption file by selecting :hp2.Open captions...:ehp2. in the :hp2.File:ehp2. submenu.
:li.Use the :hp2.Audio position:ehp2. or :hp2.Text position:ehp2. slider bars to locate
the portion of the file you wish to review.
:li.Select :hp2.Play:ehp2. to check the timing.
:eol.
:p.Select :hp2.Start timing :ehp2. to synchronize the text captions with
the audio file.  The audio file will begin playing from its current position
when this button is selected.  As the audio plays, select
the :hp2.Advance line:ehp2. button to scroll new caption lines into the
text window.
.*
:p.Select :hp2.Play:ehp2. to review a caption file.  While the audio
plays, the captions scroll by in the text window just as they would in the
target application. This function can be used to check the captioning
just after its timing has been set with the :hp2.Start timing:ehp2. function,
or it can be used to review an existing Caption file.
.*
:p.Select :hp2.Advance line:ehp2. to scroll a new line of text into the text
window during the :hp2.Start timing:ehp2. function.  The timing of
the :hp2.Advance line:ehp2. events will be stored for subsequent
:hp2.Play:ehp2., :hp2.Save:ehp2. and/or :hp2.Save as...:ehp2. operations.
.*
:p.Select :hp2.Pause:ehp2. to temporarily suspend a :hp2.Start timing:ehp2. or
:hp2.Play:ehp2. operation before the audio has finished playing.  While the
audio is paused, the indicator on the button will remain highlighted.  A paused
operation may be subsequently resumed by selecting the :hp2.Pause:ehp2. button
again, or it can be terminated altogether by selecting :hp2.Stop:ehp2..
.*
:p.Select :hp2.Stop:ehp2. to terminate a :hp2.Start timing:ehp2. or
:hp2.Play:ehp2. operation before the audio has completed playing.
.*
:p.Adjust the audio volume to a comfortable level by using the :hp2.Volume
control:ehp2..
.*
:p.The :hp2.Audio position:ehp2. control provides a graphic display of the
current position of the audio file.  To change the audio position, drag the
slider bar or select the pushbuttons at the left end of the slider.  If there
is caption text present in the text window, the program will adjust its
position to match the new position of the audio file.
.*
:p.Select the :hp2.File:ehp2. submenu to open or save the files
used by the application.
.*
:p.Select the :hp2.Open:ehp2. submenu to specify the Audio, Text or Caption
file which you wish to open.
.*
:p.Select the :hp2.Audio...:ehp2. menu item to specify the audio file which
you wish to caption. You will be promped by a file dialog box showing only
those files having an extension of :hp2.WAV:ehp2..
.*
:p.Select the :hp2.Text...:ehp2. menu item to specify a text file which you
want to use as captions for an audio file.  You will be prompted with a file
dialog box showing all of the files in your system having the extension
of :hp2.TXT:ehp2..
.*
:p.Select the :hp2.Caption...:ehp2. menu item to specify a Caption file which
was previously created by the Caption Creation Utility.  You will be prompted
with a file dialog box showing all of the files in your system having the
extension of :hp2._CC:ehp2..
.*
:p.Select :hp2.Save:ehp2. to save the current caption file.
.*
:p.Select :hp2.Save as...:ehp2. to save a Caption file after you have timed
and reviewed it.  Selecting :hp2. Save as...:ehp2. will allow you to choose
a name for the file being saved.
.*
:p.Select the :hp2.Exit:ehp2. menu item when you wish to exit the Caption
Creation Utility.
.*
:p.Select the :hp2.Options:ehp2. submenu to control how many caption lines are
displayed in the text window.
.*
:p.Select the :hp2.Caption lines:ehp2. menu item to select one of the menu
items to control the number of lines of captioning displayed in the text window.
A check mark appears to the left of the number which is currently selected.
.*
:p.Use the :hp2.Help:ehp2. submenu to access the help information for this program.
:esl.

.***********************************************************************
.*Help for Keys
.***********************************************************************

:h1 hide res=10524 nosearch noprint.Help for keys
:i1.Keys
:dl tsize=10.
:dt.:hp5.Key:ehp5.:hp2. :ehp2.
:dd.:hp5.Function:ehp5.
:dt.:hp2.Alt:ehp2.
:dd.Highlight menu items.
:dt.:hp2.Arrows:ehp2.
:dd.Move to another menu item.
:dt.:hp2.Enter:ehp2.
:dd.Select a control or a menu item.
:dt.:hp2.Tab:ehp2.
:dd.Move from one dialog box control to another.
:dt.:hp2.F1:ehp2.
:dd.Display help information for highlighted item.
:dt.:hp2.F3:ehp2.
:dd.Exit the program.
:dt.:hp2.A:ehp2.
:dd.Advance the text one line while setting timing.
:dt.:hp2.Alt + A:ehp2.
:dd.Open an audio file.
:dt.:hp2.Alt + C:ehp2.
:dd.Open a caption file.
:dt.:hp2.E:ehp2.
:dd.Pause or resume the current operation.
:dt.:hp2.P:ehp2.
:dd.Play the audio file while scrolling the captions.
:dt.:hp2.R:ehp2.
:dd.Rewind the audio and caption files.
:dt.:hp2.S:ehp2.
:dd.Stop the current operation.
:dt.:hp2.Alt + S:ehp2.
:dd.Save the caption file which is currently open.
:dt.:hp2.T:ehp2.
:dd.Start the timing operation.
:dt.:hp2.Alt + T:ehp2.
:dd.Open a text file
:dt.:hp2.Alt + V:ehp2.
:dd.Save captions under a selected name.
:dt.:hp2.Alt + 2:ehp2.
:dd.Display two lines in the text window.
:dt.:hp2.Alt + 3:ehp2.
:dd.Display three lines in the text window.
:dt.:hp2.Alt + 4:ehp2.
:dd.Display four lines in the text window.
:dt.:hp2.Alt + 5:ehp2.
:dd.Display five lines in the text window.
:dt.:hp2.Alt + 6:ehp2.
:dd.Display six lines in the text window.
:edl.

.***********************************************************************
.*Help for Open Audio file   (D)
.***********************************************************************
:h1 hide res=4163 nosearch noprint.Help for Open Audio
:p.The :hp2.Drive:ehp2. list box displays the drives on your system.  Select
the drive that contains the audio files you want to caption.
:p.The :hp2.Directory:ehp2. list box displays the directories on the selected
drive.  Select a directory to display the list of files from that directory
in the :hp2.File:ehp2. list box.
:p.Select an audio file from the :hp2.File:ehp2. list box or type in a file name
that you want to caption.
:p.Select the :hp2.OK:ehp2. pushbutton to load the selected audio file to
caption and to remove this pop-up window.
:p.Select the :hp2.Cancel:ehp2. pushbutton to remove this pop-up window.
:p.Select the :hp2.Help:ehp2. pushbutton to view this help panel.

.***********************************************************************
.*Help for Open Text file   (D)
.***********************************************************************
:h1 hide res=4164 nosearch noprint.Help for Open Text
:p.The :hp2.Drive:ehp2. list box displays the drives on your system.  Select the
drive that contains the text files you want to use.
:p.The :hp2.Directory:ehp2. list box displays the directories on the selected
drive.  Select a directory to display the list of files from that directory
in the :hp2.File:ehp2. list box.
:p.Select a text or caption file from the :hp2.File:ehp2. list or type in a file name
that you want to use.
:p.Select the :hp2.OK:ehp2. pushbutton to load the selected file to use
and to remove this pop-up window.
:p.Select the :hp2.Cancel:ehp2. pushbutton to remove this pop-up window.
:p.Select the :hp2.Help:ehp2. pushbutton to view this help panel.
.***********************************************************************
.*Help for Save Caption File (D)
.***********************************************************************
:h1 hide res=4165 nosearch noprint.Help for Save Caption
:p.Type the name of the file you want to save in the
:hp2.File name:ehp2. field
:p.The program is preset to use the name of the audio file with the extension
:hp2.._CC:ehp2.
:p.The :hp2.Drive:ehp2. list displays the drives on your system.  Select the
drive where you want to save the caption file.
:p.Select the :hp2.Save as:ehp2. pushbutton to save the caption data under
the selected file name.
:p.Select the :hp2.Cancel:ehp2. pushbutton to remove this pop-up window.
:p.Select the :hp2.Help:ehp2. pushbutton to view this help panel.
.*************************************************************************

:euserdoc.
