.*==============================================================*\
.*                                                              *
.* clientry.ipf - Information Tag Language file for the         *
.*              entry panel help panels.                        *
.*                                                              *
.*  Copyright 1991, IBM Corp.                                   *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Message Entry Field                                         *
.*      res = PANEL_MESSAGE                                     *
.*--------------------------------------------------------------*/
:h1 res=2310 name=PANEL_MESSAGE.Message Entry Field
:i1 id=Message.Message Entry Field
:p.The Message Entry Field is where the message to be sent to the
server through the IPC queue is typed.  This demostrates sending
a simple data stream as a message.

.*--------------------------------------------------------------*\
.*  Date Entry Fields                                           *
.*      res = PANEL_DATE                                        *
.*--------------------------------------------------------------*/
:h1 res=2350 name=PANEL_DATE.Date Entry Fields
:i1 id=Date.Date Entry Fields
:p.The Date Entry Fields are where the date to be sent to the
server through the IPC queue is typed.  This demostrates sending
a structure or other defined message through a queue.

.*--------------------------------------------------------------*\
.*  Priority Spin Button                                        *
.*      res = PANEL_PRIORITY                                    *
.*--------------------------------------------------------------*/
:h1 res=2320 name=PANEL_PRIORITY.Priority Spin Button
:i1 id=Priority.Priority Spin Button
:p.The Priority Spin Button is where the priority of the queue
message is entered either manually, or by clicking on the spin
arrows.  Valid priorities are 0 to 15, with 0 having the lowest
priority.

.*--------------------------------------------------------------*\
.*  Send Push Button                                            *
.*      res = PANEL_SEND_MSG                                    *
.*--------------------------------------------------------------*/
:h1 res=2330 name=PANEL_SEND_MSG.Send Push Button
:i1 id=Send-Message.Send Message Push Button
:p.The Send Push Button is clicked to send the message in the
:link reftype=hd res=2310.
Message Entry Field
:elink.
to the IPC queue with the given
:link reftype=hd res=2320.
Priority.
:elink.

.*--------------------------------------------------------------*\
.*  Send Push Button                                            *
.*      res = PANEL_SEND_DATE                                   *
.*--------------------------------------------------------------*/
:h1 res=2340 name=PANEL_SEND_DATE.Send Date Push Button
:i1 id=Send-Date.Send Date Push Button
:p.The Send Push Button is clicked to send the message in the
:link reftype=hd res=2350.
Date Entry Fields
:elink.
to the IPC queue with the given
:link reftype=hd res=2320.
Priority.
:elink.
