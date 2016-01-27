.*==============================================================*\
.*                                                              *
.* svroption.ipf - Information Tag Language file for the        *
.*              Option menu help panels.                        *
.*                                                              *
.*  Copyright 1991, IBM Corp.                                   *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Main option menu                                            *
.*      res = PANEL_OPTION                                      *
.*--------------------------------------------------------------*/
:h1 res=2310 name=PANEL_OPTION.Option Menu
:i1 id=Option.Option Menu
:p.The Option menu contains commands that you use to manipulate
the IPC queue.
The following commands appear in the File menu:
:parml tsize=15 break=none.
:pt.Purge
:pd.Empties the Queue
:eparml.

.*--------------------------------------------------------------*\
.*  Option menu Purge command help panel                        *
.*      res = PANEL_OPTIONPURGE                                 *
.*--------------------------------------------------------------*/
:h1 res=2320 name=PANEL_OPTIONPURGE.Purge
:i2 refid=Option.Purge
:p. You can empty the IPC queue of its contents by
using the Purge command.
