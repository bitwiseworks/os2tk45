;***************************************************************************  
;                                                                             
;  Module Name: EVCB.H                                                        
;                                                                             
;  OS/2 2.0 Multimedia Extensions Sync/Stream Manager Event Control           
;  Block defintitions.                                                        
;                                                                             
;  Copyright (c) International Business Machines Corporation 1991, 1992       
;                         All Rights Reserved                                 
;                                                                             
;***************************************************************************  
;**********************************************
;* List of Implicit events:
;*    EVENT_EOS                 - Use IMPL_EVCB
;*    EVENT_ERROR               - Use IMPL_EVCB
;*    EVENT_STREAM_STOPPED      - Use IMPL_EVCB
;*    EVENT_SYNC_PREROLLED      - Use IMPL_EVCB
;*    EVENT_PLAYLISTMESSAGE     - Use PLAYL_EVCB
;*    EVENT_PLAYLISTCUEPOINT    - Use PLAYL_EVCB
;*    EVENT_QUEUE_OVERFLOW      - Use IMPL_EVCB
;*********************************************  
;**********************************************
;* List of Explicit events:
;*    EVENT_SYNCOVERRUN         - Use OVRU_EVCB
;*    EVENT_CUE_TIME            - Use TIME_EVCB
;*    EVENT_CUE_DATA            - Use DATA_EVCB
;*    EVENT_DATAUNDERRUN        - Use EVCB
;*    EVENT_DATAOVERRUN         - Use EVCB
;*    EVENT_CUE_TIME_PAUSE      - Use TIME_EVCB
;*********************************************  
EVENT_SYNCOVERRUN	EQU	4 
EVENT_CUE_TIME	EQU	5        ;  Cue point in terms of stream time  
EVENT_CUE_DATA	EQU	6        ;  Cue point in terms of data items   
EVENT_DATAUNDERRUN	EQU	7        ;  data underrun event from SH      
EVENT_DATAOVERRUN	EQU	8        ;  data underrun event from SH      
EVENT_CUE_TIME_PAUSE	EQU	9        ;  Pause when cue-point reached.    
EVENT_STATUS_LEVEL	EQU	10       ;  Report status level              
;**********************************************
;* SYNC_EVCB - Sync Pulse Event Control Block
;*             (This event is only seen by
;*              stream handlers)
;*********************************************  
EVENT_SYNC	EQU	1      ;  Synchronization point in stream    

SYNC_EVCB	STRUC
syevcb_ulType	DD	?                     ;  EVENT_SYNC                         
syevcb_ulSubType	DD	?                  ;  Not used                           
syevcb_ulSyncFlags	DD	?                ;  0 (input),status (output)          
syevcb_hstream	DD	?                    ;  handle to stream for this event    
syevcb_hid	DD	?                        ;  Handler id                         
syevcb_ulStatus	DD	?                   ;  Event status (output)              
syevcb_mmtimeStart	DD	?                ;  Filled in by Sync/Stream manager   
                                       ;   at SpiEnableSync time. (input)    
syevcb_mmtimeMaster	DD	?               ;  Filled in by Master SH (input)     
syevcb_mmtimeSlave	DD	?                ;  Filled in by slave SH. (output)    
SYNC_EVCB	ENDS
;  ulSyncFlags:                                                              
SYNCOVERRUN	EQU	0001H     ;  Set by SSM if sync pulse comes in before      
                                       ;   a has processed the current sync pulse.      
SYNCPOLLING	EQU	0002H     ;  SSM set this after filling the handler EVCB.  
                                       ;   Handler resets it after it processes the     
                                       ;   contents of EVCB. The SSM must check this    
                                       ;   bit before modifying EVCB. If bit already    
                                       ;   set, set the SYNC overrun bit.               
EVENT_RESERVED	EQU	2      ;  RESERVED                           
;**********************************************
;* IMPL_EVCB - Implicit Event Control Block
;*             (This is a parameter on the
;*              SpiCreatStrem API)
;*********************************************  
EVENT_IMPLICIT_TYPE	EQU	3      ;  These events are always supported  

IMPL_EVCB	STRUC
imevcb_ulType	DD	?                     ;  EVENT_IMPLICIT_TYPE                
imevcb_ulSubType	DD	?                  ;  One of the implicit events (input)   
imevcb_ulFlags	DD	?                    ;  0 (Input), status (Output)           
imevcb_hstream	DD	?                    ;  handle to stream for this event (input/output)   
imevcb_hid	DD	?                        ;  0 (input), handler Id (output)       
imevcb_ulStatus	DD	?                   ;  Event status/error return code (output)   
imevcb_unused1	DD	?                    ;  0 (input)                          
imevcb_unused2	DD	?                    ;  0 (input)                          
imevcb_unused3	DD	?                    ;  0 (input)                          
IMPL_EVCB	ENDS
;  ulSubtype:                                                                
EVENT_EOS	EQU	1      ;  End of Stream                      
EVENT_ERROR	EQU	2      ;  Error in Stream handler or device driver          
EVENT_STREAM_STOPPED	EQU	3      ;  Stream is in a stopped state (Discard or Flush)   
EVENT_SYNC_PREROLLED	EQU	4      ;  All streams are prerolled (buffers     
                                       ;   have been filled by the Source SH's.  
EVENT_PLAYLISTMESSAGE	EQU	5      ;  Memory SH playlist message event       
EVENT_PLAYLISTCUEPOINT	EQU	6      ;  Memory SH playlist cue point           
EVENT_QUEUE_OVERFLOW	EQU	7      ;  Event Queue overflow. Indicates lost   
                                       ;   events. Application must use this     
                                       ;   to clear any waiting conditions.      
EVENT_START	EQU	8      ;  Start stream                           
;**********************************************
;* PLAYL_EVCB - Playlist Message Event Control Block
;*********************************************  
;  (IMPLICIT EVENT)                                                          

PLAYL_EVCB	STRUC
plevcb_ulType	DD	?                     ;  EVENT_IMPLICIT_TYPE                
plevcb_ulSubType	DD	?                  ;  EVENT_PLAYLISTMESSAGE or EVENT_PLAYLISTCUEPOINT   
plevcb_ulFlags	DD	?                    ;  0 (input), status (output)                        
plevcb_hstream	DD	?                    ;  handle to stream for this event (input/output)    
plevcb_hid	DD	?                        ;  0 (input), handler Id (output)                    
plevcb_ulStatus	DD	?                   ;  Playlist Instruction number (output)              
plevcb_ulMessageParm	DD	?              ;  0 (input), Message from playlist (output)         
plevcb_unused1	DD	?                    ;  0 (input)                          
plevcb_unused2	DD	?                    ;  0 (input)                          
PLAYL_EVCB	ENDS
;**********************************************
;* OVRU_EVCB - Sync Overrun Event Control Block
;*             (Applications can request to be
;*              notified whenever a Sync overrun
;*              occurs.
;*********************************************  

OVRU_EVCB	STRUC
ovevcb_ulType	DD	?                     ;  EVENT_SYNCOVERRUN                  
ovevcb_ulSubType	DD	?                  ;  Not used                           
ovevcb_ulFlags	DD	?                    ;  0 (input), status (output)         
ovevcb_hstream	DD	?                    ;  handle to stream for this event (input/output)   
ovevcb_hid	DD	?                        ;  handler Id (input/output)          
ovevcb_ulStatus	DD	?                   ;  Event status/error return code (output)   
ovevcb_mmtimeSlave	DD	?                ;  0 (input), Slave stream time (output)     
ovevcb_mmtimeStart	DD	?                ;  0 (input), Slave start offset (offset)    
ovevcb_mmtimeMaster	DD	?               ;  0 (input), Master stream time (output)    
OVRU_EVCB	ENDS
;**********************************************
;* TIME_EVCB - Cue Time Event Control Block
;*             (Applications can request to be
;*              notified for a time cue point)
;*********************************************  

TIME_EVCB	STRUC
tievcb_ulType	DD	?                     ;  EVENT_CUE_TIME                     
tievcb_ulSubType	DD	?                  ;  Not used                           
tievcb_ulFlags	DD	?                    ;  Single/Recurring(input/output)     
tievcb_hstream	DD	?                    ;  handle to stream for this event (input/output)  
tievcb_hid	DD	?                        ;  handler Id (input/output)                       
tievcb_ulStatus	DD	?                   ;  Event status/error return code (output)         
tievcb_mmtimeStream	DD	?               ;  Stream time (input/output)         
tievcb_unused1	DD	?                    ;  0 (input)                          
tievcb_unused2	DD	?                    ;  0 (input)                          
TIME_EVCB	ENDS
;                                                                            
;  ulFlags:                                                                  
EVENT_SINGLE	EQU	0      ;  This is single one time event      
EVENT_RECURRING	EQU	1      ;  This is recurring event            
;**********************************************
;* DATA_EVCB - Cue Data Event Control Block
;*             (Applications can request to be
;*              notified for a data cue point)
;*********************************************  

DATA_EVCB	STRUC
daevcb_ulType	DD	?                     ;  EVENT_CUE_DATA                     
daevcb_ulSubType	DD	?                  ;  Event SubType                      
daevcb_ulFlags	DD	?                    ;  Single/Recurring(input/output)     
daevcb_hstream	DD	?                    ;  handle to stream for this event (input/output)   
daevcb_hid	DD	?                        ;  handler Id (input/output)                        
daevcb_ulStatus	DD	?                   ;  Event status/error return code (output)          
daevcb_mmtimeStream	DD	?               ;  Stream time (input/output)                       
daevcb_ulEventParm1	DD	?               ;  Data to Cue on (input/output)                    
                                       ;   This is interpreted by the stream handler       
daevcb_ulEventParm2	DD	?               ;  Length of data cue buffer (input/output)         
DATA_EVCB	ENDS
;  ulFlags:                                                                  
; #define EVENT_SINGLE          0L        This is single one time event      
; #define EVENT_RECURRING       1L        This is recurring event            
EVENT_DATAPTR	EQU	2      ;  ulEventParm1 if ptr to buffer and  
                                       ;   ulEventParm2 is a length of buffer,   
                                       ;   else ulEventParm1 is data.            
;******************
;* EVCB_ulType:  (Stream Handler defined types)
;*****************  
SH_TYPE	EQU	80000000H   ;  Stream Handler defined types must   
                                       ;  have the high order bit set in the  
                                       ;  ulType field.                       
                                       ;  events: 0H - 7FFFFFFFH are reserved!  
;**********************************************
;* EVCB - Generic Event Control Block (Use the
;*        specific one in the SSM.H if possible)
;*********************************************  

EVCB	STRUC
evcb_ulType	DD	?                     ;  Event type (input)                 
evcb_ulSubType	DD	?                  ;  Event SubType (input)              
evcb_ulFlags	DD	?                    ;  0 (Input), status (Output)         
evcb_hstream	DD	?                    ;  handle to stream for this event    
evcb_hid	DD	?                        ;  handler Id (input/output)          
evcb_ulStatus	DD	?                   ;  Event status (output)              
evcb_ulEventParm1	DD	?               ;  Event parameters (input), HID if implicit event   
evcb_ulEventParm2	DD	?               ;  Event parameters (input)           
evcb_ulEventParm3	DD	?               ;  Event parameters (input)           
EVCB	ENDS
;**********************************************
;* STATUS_LEVEL_EVCB - Status Level Event Control Block
;*********************************************  

STATUS_LEVEL_EVCB	STRUC
   ;  standard part   
stevcb_ulType	DD	?                     ;  Event type (input)                 
stevcb_ulSubType	DD	?                  ;  Event SubType (input)              
stevcb_pMCDInstance	DD	?               ;  for MCD use   
stevcb_hstream	DD	?                    ;  handle to stream for this event    
stevcb_hid	DD	?                        ;  handler Id (input/output)          
   ;  custom part   
stevcb_ulSample	DD	4 DUP (?)                ;  Samples   
STATUS_LEVEL_EVCB	ENDS
