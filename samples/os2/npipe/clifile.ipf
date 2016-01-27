.*===============================================================*\
.*                                                               *
.* Clifile.ipf - Information Tag Language file for the File menu *
.*              help panels.                                     *
.*                                                               *
.*===============================================================*/

.*--------------------------------------------------------------*\
.*  Main file menu                                              *
.*      res = PANEL_FILE                                        *
.*--------------------------------------------------------------*/
:h1 res=2210 name=PANEL_FILE.File Menu
:i1 id=File.File Menu
:p.The File menu contains the command that you use to start
the tic-tac-toe game. In addition, it also contains the
command that you use to exit the Sample application. The following
commands appear in the File menu:
:parml tsize=15 break=none.
:pt.Start Game
:pd.Starts a game of tic-tac-toe
:pt.Exit
:pd.Quits the Sample application
:eparml.


.*--------------------------------------------------------------*\
.*  File menu Exit command help panel                           *
.*      res = PANEL_FILESTART                                   *
.*--------------------------------------------------------------*/
:h1 res=2220 name=PANEL_FILESTART.Start Game
:i2 refid=File.Start Game
:p.You start the tic-tac-toe game by using the Start Game command.
To start the game, do the following:
:ol.
:li.Select the File menu and choose the Start Game command.
:li.Answer Yes or No when asked if you would like to move first.
:eol.


.*--------------------------------------------------------------*\
.*  File menu Exit command help panel                           *
.*      res = PANEL_FILEEXIT                                    *
.*--------------------------------------------------------------*/
:h1 res=2290 name=PANEL_FILEEXIT.Exit
:i2 refid=File.Exit
:p.You quit the Sample application by using the Exit command. To
quit the Sample application, do the following:
:ol.
:li.Select the File menu and choose the Exit command.
:eol.
