.*==============================================================*\
.*                                                              *
.* Clidlg.ipf - Information Tag Language file for the screens   *
.*           for help on the dialog boxes.                      *
.*                                                              *
.*==============================================================*/


.*--------------------------------------------------------------*\
.*  Help for REMOTESERVER                                       *
.*      res = PANEL_REMOTESERVER                                *
.*--------------------------------------------------------------*/
:h1 res=4100 name=PANEL_REMOTESERVER.Remote Server Dialog
:i1 id=Server.Remote Server
:p.This dialog is for the entry of a remote server's name, for
play over a LAN.  Select Cancel for local play.


.*--------------------------------------------------------------*\
.*  Remote Server 'Server Name' entry field                     *
.*      res = PANEL_REMOTESERVER_SERVERNAME                     *
.*--------------------------------------------------------------*/
:h1 res=4110 name=PANEL_REMOTESERVER_SERVERNAME.Server Name
:i2 refid=Server.Server Entry Field
:p.This field is for the entry of a remote server to connect to.
The server name can be at maximum 8 characters in length.

.*--------------------------------------------------------------*\
.*  Remote Server OK button                                     *
.*      res = PANEL_REMOTESERVER_OK                             *
.*--------------------------------------------------------------*/
:h1 res=4120 name=PANEL_REMOTESERVER_OK.Ok
:i2 refid=Server.OK push button
:p.This button completes the selection of the server entered in the
:link reftype=hd res=4110.
Server Entry Field.
:elink.


.*--------------------------------------------------------------*\
.*  Remote Server CANCEL button                                 *
.*      res = PANEL_REMOTESERVER_CANCEL                         *
.*--------------------------------------------------------------*/
:h1 res=4130 name=PANEL_REMOTESERVER_CANCEL.Cancel
:i2 refid=Server.Cancel push button
:p.This button cancels the selection of any remote server and play
is begun locally.
