.*==============================================================*\
.*                                                              *
.* Clioptn.ipf - Information Tag Language file for the Option   *
.*              menu help panels.                               *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main file menu                                              *
.*      res = PANEL_OPTION                                      *
.*--------------------------------------------------------------*/
:h1 res=2310 name=PANEL_OPTION.Option Menu
:i1 id=Option.Option Menu
:p.The Option menu contains commands that you use to manipulate
the display of the tic-tac-toe board.
The following commands appear in the File menu:
:parml tsize=15 break=none.
:pt.Swap Pieces.
:pd.Switches X's and O's.
:eparml.

.*--------------------------------------------------------------*\
.*  File menu New command help panel                            *
.*      res = PANEL_OPTIONPURGE                                 *
.*--------------------------------------------------------------*/
:h1 res=2320 name=PANEL_OPTIONCLEAR.Clear
:i2 refid=Option.Switch Pieces
:p. You can switch your playing pieces between X's and
O's, but you still control the same squares.
