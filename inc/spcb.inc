;***************************************************************************  
;                                                                             
;  Module Name: SPCB.H                                                        
;                                                                             
;  OS/2 2.0 Multimedia Extensions Sync/Stream Manager Stream Protocol         
;  Control Block Definitions.                                                 
;                                                                             
;  Copyright (c) International Business Machines Corporation 1991, 1992       
;                         All Rights Reserved                                 
;                                                                             
;***************************************************************************  
ifndef  _SPCBKEY_DEFINED
_SPCBKEY_DEFINED	EQU	1

;**********************************************
;*
;* SPCBKEY - Stream Protocol Key
;*
;*********************************************  

SPCBKEY	STRUC
spcbkey_ulDataType	DD	?
spcbkey_ulDataSubType	DD	?
spcbkey_ulIntKey	DD	?                 ;  generated internal key              
SPCBKEY	ENDS
endif
;**********************************************
;*
;* SPCB - Stream Protocol Control Block
;*
;*********************************************  

SPCB	STRUC
spcb_ulSPCBLen	DD	?             ;  SPCB struture length                     
spcb_spcbkey	DB	SIZE SPCBKEY DUP (?)
spcb_ulDataFlags	DD	?           ;  Data type flags                          
spcb_ulNumRec	DD	?              ;  Max # records/buffer (Only used for Split streams)   
spcb_ulBlockSize	DD	?           ;  Block alignment in bytes.                     
spcb_ulBufSize	DD	?             ;  Buffer size (SSM allocated buffers) must be   
                                  ;   a multiple of Block size                     
spcb_ulMinBuf	DD	?              ;  Minimum number of buffers needed to stream    
spcb_ulMaxBuf	DD	?              ;  Maximum number of buffers needed to stream    
spcb_ulSrcStart	DD	?            ;  # of EMPTY buffers required to start Source   
spcb_ulTgtStart	DD	?            ;  # of FULL buffers required to start Target    
spcb_ulBufFlags	DD	?            ;  Handler Protocol negotiation flags            
spcb_ulHandFlags	DD	?           ;  Handler Protocol flags                        
spcb_mmtimeTolerance	DD	?       ;  Sync tolerance value...Used as check by SSM   
                                  ;   to determine whether to send a sync pulse    
                                  ;   to this specific slave Stream Handler.       
                                  ;   Algorithm:                                   
                                  ;    diff = abs(master time - slave time)        
                                  ;    if  diff >= tolerance                       
                                  ;      then send sync pulse                      
                                  ;  (Valid only for Slave stream handlers)        
spcb_mmtimeSync	DD	?            ;  Used to save sync pulse generation granularity   
                                  ;   if the master uses the Stream Manager Timer.    
                                  ;   if SpiSetSync passed NULL value for sync gran   
                                  ;    then use this default value. (since hardware   
                                  ;    must have it's own interrupt time interval.    
                                  ;  (Valid only for Master stream handlers)          
spcb_ulBytesPerUnit	DD	?        ;  Bytes/unit of time. This is used to do seeks     
                                  ;  on linear data that is not compressed or         
                                  ;  of variable length.                              
spcb_mmtimePerUnit	DD	?         ;  The amount of mmtime each unit represents.       
                                  ;  A unit can be a second, minute or whatever.      
SPCB	ENDS
SPCB_MAX_BUF_SIZE	EQU	(1024 *1024 )   ;  Largest buffer size in bytes    
;*******************
;* SPCB_ulDataFlags:
;******************  
SPCBDATA_CUETIME	EQU	0002H  ;  This data type can support time cue points  
SPCBDATA_CUEDATA	EQU	0004H  ;  This data type can support data cue points  
SPCBDATA_SEEK	EQU	0000H  ;  Seeks can be performed on this data type.   
SPCBDATA_NOSEEK	EQU	0008H  ;  NO seeks can be performed on this data type.  
SPCBDATA_YIELDTIME	EQU	0010H  ;  The ulBytes per unit field is used for a millisecond value      
                                       ;  It represents the amount of yield time between reads of         
                                       ;  each buffer. If this bit is set, the value of ulBytesPerUnit    
                                       ;  is used as input to DosSleep to yield for that period of time   
;********************
;* SPCB_ulBufFlags:
;*******************  
SPCBBUF_USERPROVIDED	EQU	0001H  ;  User provides buffers for stream          
                                       ;  SSMgr will not allocate buffers,          
                                       ;  but must lock down provided buffers,      
                                       ;  so this will affect performance!!         
                                       ;  (Source Handler only)                     
SPCBBUF_FIXEDBUF	EQU	0002H  ;  Buffer size is fixed for this data type   
                                       ;   in this handler. Can not be used         
                                       ;   with the SPCBBUF_USERPROVIDED flags.     
SPCBBUF_NONCONTIGUOUS	EQU	0004H  ;  Buffer do not need to be contiguous       
                                       ;   in physical memory.                      
SPCBBUF_INTERLEAVED	EQU	0008H  ;  Stream buffers can be interleaved.        
                                       ;   Can not be use with the                  
                                       ;   SPCBBUF_USERPROVIDED flag.               
                                       ;   (Source Handler only)                    
SPCBBUF_MAXSIZE	EQU	0010H  ;  ulBufSize is the maximum size that        
                                       ;   can be used by this stream handler       
SPCBBUF_16MEG	EQU	0020H  ;  The Stream buffers may be allocated       
                                       ;   above the 16 Meg line. This is used      
                                       ;   by stream handlers that can support      
                                       ;   greater than 16 Megabyte addresses.      
SPCBBUF_FIXEDBLOCK	EQU	0040H  ;  Use the ulBlockSize field to represent    
                                       ;  the size of the IO reads that should      
                                       ;  be performed (Interleaved streams)        
;*******************
;* SPCB_ulHandFlags:
;******************  
SPCBHAND_GENSYNC	EQU	0001H  ;  (INPUT FROM HANDLER ONLY)                 
                                       ;  This handler can generate sync pulses     
                                       ;  if it is the master.                      
SPCBHAND_RCVSYNC	EQU	0002H  ;  (INPUT FROM HANDLER ONLY)                 
                                       ;  This handler can receive sync pulses      
                                       ;  if it is a slave                          
SPCBHAND_TIMER	EQU	0004H  ;  Use Stream Manager Timer for Sync         
SPCBHAND_NONSTREAM	EQU	0008H  ;  Stream Handler is non-streaming           
SPCBHAND_GENTIME	EQU	0010H  ;  This handler contains real                
                                       ;  stream time. The handler that             
                                       ;  supports the SpiGetTime, data/time        
                                       ;  cue point calls for this stream.          
SPCBHAND_NOPREROLL	EQU	0020H  ;  This stream can not be prerolled.         
                                       ;  (i.e. recording streams)                  
SPCBHAND_NOSYNC	EQU	0040H  ;  This stream can be group into a           
                                       ;  sync group, but does not send or          
                                       ;  receive sync pulses.                      
SPCBHAND_PHYS_SEEK	EQU	0080H  ;  This handler does a seek to a physical    
                                       ;  device or memory not just a time adjustment.  
                                       ;  (ssm will always call this handler first on   
                                       ;   an SpiSeekStream call).                      
