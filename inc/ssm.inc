MAX_SPI_NAME	EQU	9       ;  Maximum ASCIIZ stream handler name/class    
                             ;  and device driver name                      
;***************************************************************************  
;                                                                             
;   Global Type Declarations                                                  
;                                                                             
;***************************************************************************  
;  The following define was deleted because it conflicts with an OS/2 PM define   
; typedef HID  FAR *PHID;                Pointer to Stream Handler ID         

LOCKH	STRUC
Memory_lock	DB	16 DUP (?)
LOCKH	ENDS
RC	EQU	ULONG                      ;  Used for all return codes            

SLAVE	STRUC
slave_hstreamSlave	DD	?          ;  Handle for this slave              
slave_mmtimeStart	DD	?           ;  Offset in master stream time to start   
SLAVE	ENDS

MASTER	STRUC
master_hstreamMaster	DD	?         ;  Handle for this master             
MASTER	ENDS

HAND	STRUC
hand_szHandlerClass	DB	MAX_SPI_NAME DUP (?) ;  Stream handler class        
hand_szHandlerName	DB	MAX_SPI_NAME DUP (?)  ;  Stream handler name         
HAND	ENDS
INCLUDE spcb.INC
INCLUDE evcb.INC
INCLUDE acb.INC
INCLUDE dcb.INC
ifdef XXXXX
endif
ifdef CONNECTION
INCLUDE concb.INC
endif
;******************
;* ulNetworkState:
;*****************  
NET_ACTIVE_MASK	EQU	00000FFFH     ;  Network running mask   
NET_RUNNING	EQU	00000000H     ;  Network is running   
NET_PAUSED	EQU	00000001H     ;  Network is paused   
NET_STOPPED	EQU	00000002H     ;  Network is stopped (flush/discard)   
NET_PAUSE_PENDING	EQU	00000004H     ;  Network is pending a pause stop   
NET_FLUSH_PENDING	EQU	00000010H     ;  Network is pending a flush stop   
NET_DISCARD_PENDING	EQU	00000020H     ;  Network is pending a discard stop   
NET_CLOSE_PENDING	EQU	00000040H     ;  Network is pending a CLOSE   
NET_SEEK_PENDING	EQU	00000040H     ;  Network is pending a seek operation   
NET_OPEN	EQU	00001000H     ;  Network has been opened   
NET_ASSEMBLED	EQU	00004000H     ;  Network has been assembled   
;********************
;* ulActivationState:
;*******************  
STREAM_ACTIVE	EQU	00000010H     ;  Stream is active   
STREAM_NOT_ACTIVE	EQU	00000020H     ;  Stream is not active   
;***************************************************************************  
;  INI file support defines                                                   
;***************************************************************************  
;  RCDATA defines:                                                            
SPI_RESOURCE	EQU	1                ;  RCDATA resource number              
SH_DLL_TYPE	EQU	0                ;  DLL type stream handler             
SH_PDD_TYPE	EQU	1                ;  PDD type stream handler             
SPCBSIZE	EQU	72              ;  SPCB length, the Resource compiler  
                                       ;    doesn't allow sizeof(SPCB).       
;***************************************************************************  
;                                                                             
;   Function Prototypes for SPI functions                                     
;   All use Pure C calling convention                                         
;   Six major categories:                                                     
;      - stream creation                                                      
;      - stream control                                                       
;      - event handling                                                       
;      - sync handling                                                        
;      - configuration                                                        
;      - stream protocol programming                                          
;***************************************************************************  
;  The first thing in the pMsg must be the length of the structure!   

MSG_COMMON	STRUC
msg_ulMsgLen	DD	?              ;  length of structure   
MSG_COMMON	ENDS
;  SpiStopStream() ulFlags defines:                                           
SPI_STOP_STREAM	EQU	0000H          ;  Only stop this one stream       
                                           ;  The default is to pause the data            
                                           ;   stream. No change to data buffers.         
                                           ;   synchronous call (no event notification)   
SPI_STOP_SLAVES	EQU	0001H          ;  Stop this master and any slaves             
SPI_STOP_FLUSH	EQU	0002H          ;  Stop and flush all filled buffers.          
                                           ;   Stops the Src & Tgt SH, lets the           
                                           ;   Tgt SH run until out of buffers            
                                           ;   thereby, "flushing" all buffers.           
                                           ;   Tgt SH stops when out of buffers           
                                           ;   (This is a ASYNCHRONOUS call, event        
                                           ;    notification sent to appl when done.)     
SPI_STOP_DISCARD	EQU	0004H          ;  Stop and discard all filled buffers.        
                                           ;   Stops the Src & Tgt SH                     
                                           ;   then discards all buffers current for      
                                           ;   this stream.                               
                                           ;   (This is a ASYNCHRONOUS call, event        
                                           ;    notification sent to appl when done.)     
SPI_STOP_PAUSE	EQU	(SPI_STOP_STREAM)
;***************************************************************************  
;                                                                             
;   Function Prototypes for SMH functions                                     
;   All use Pure C calling convention                                         
;  NOTE ABOUT POINTERS:                                                       
;   0:32 linear addresses for ring 3 (DLL) stream handlers                    
;   16:16 selector/offset addresses for ring 0 (PDD) stream handlers          
;***************************************************************************  

SMH_COMMON	STRUC
smhfunc_ulFunction	DD	?        ;  SMH command function                     
smhfunc_hid	DD	?               ;  handler ID                               
SMH_COMMON	ENDS
;***************************************************************************  
SMH_REGISTER	EQU	0               ;  (Valid only from ring 3 stream   
                                               ;   handler)                   

PARM_REG	STRUC
smhreg_ulFunction	DD	?                  ;  SMH command function        
smhreg_pszSHName	DD	?                   ;  Max size = 8 characters     
smhreg_phidSrc	DD	?                     ;  (9 including null)          
smhreg_phidTgt	DD	?
smhreg_pshcfnEntry	DD	?                 ;  Handler entry point for SHC's    
                                               ;   Only used for Ring 0 handlers.  
smhreg_ulFlags	DD	?
smhreg_ulMaxNumStreams	DD	?             ;  Max # of streams            
smhreg_ulMaxNumEvents	DD	?              ;  Max # of events/stream      
PARM_REG	ENDS
REGISTER_TGT_HNDLR	EQU	0001H          ;  This handler is Target.     
REGISTER_SRC_HNDLR	EQU	0002H          ;  This handler is Source.     
                                               ;  Handlr can be both.         
REGISTER_NONSTREAMING	EQU	0004H          ;  Handler is non-streaming.   
VALIDREGISTERFLAGS	EQU	(REGISTER_TGT_HNDLR  OR  REGISTER_SRC_HNDLR  OR  REGISTER_NONSTREAMING)
;  Notes:                                                                       
;   - Since ring 0 (DD) stream handlers can not use the IDC interface to the    
;     sync/stream manager during DD initialization time, it must use the IOCTL  
;     function to perform the SMH_REGISTER function. The addresses are 16:16    
;     for this IOCTl instead of 0:32, because it comes form a device driver.    
SSMDD_CATEGORY	EQU	0081H     ;  Category (For ring 0 stream handler)   
IOCTL_FUNC_SMH_REGISTER	EQU	0040H     ;  Function (For ring 0 stream handler)   
;***************************************************************************  
SMH_DEREGISTER	EQU	1               ;  SMH_Deregister()            

PARM_DEREG	STRUC
smhder_ulFunction	DD	?                  ;  SMH command function        
smhder_pszSHName	DD	?                   ;  Max size = 8 char, 9 includes   
PARM_DEREG	ENDS
;***************************************************************************  
SMH_REPORTEVENT	EQU	2               ;  SMH_ReportEvent()           

PARM_EVENT	STRUC
smhrev_ulFunction	DD	?                  ;  SMH command function        
smhrev_hid	DD	?
smhrev_hevent	DD	?                      ;  Used only for explicit events     
                                               ;   Must be 0 for sync and implicit  
                                               ;   events (i.e. error events)       
smhrev_pevcbEvent	DD	?                  ;  Event status & HSTREAM            
PARM_EVENT	ENDS
;***************************************************************************  
SMH_NOTIFY	EQU	3               ;  SMH_Notify()                

PARM_NOTIFY	STRUC
smhnot_ulFunction	DD	?       ;  Input - SMH command function           
smhnot_hid	DD	?              ;  Input                                  
smhnot_hstream	DD	?          ;  Input                                  
smhnot_ulFlags	DD	?          ;  Input/Output                           
smhnot_ulGetNumEntries	DD	?  ;  Input/Output - # of entries in table   
smhnot_ulRetNumEntries	DD	?  ;  Input/Output - # of entries in table   
smhnot_pGetBufTab	DD	?       ;  Output - Ptr to buffer/record table    
smhnot_pRetBufTab	DD	?       ;  Input - Ptr to buffer/record table     
PARM_NOTIFY	ENDS

SRCBUFTAB	STRUC
srcbuftab_pBuffer	DD	?      ;  Ptr to buffer                               
srcbuftab_pRecord	DD	?      ;  Ptr to record in buffer (split-streams only)    
srcbuftab_ulLength	DD	?     ;  Max buf length on GetEmpty, Filled          
                               ;  (actual) rec/buf length on ReturnFull.      
srcbuftab_ulMessageParm	DD	? ;  Message to passed to application           
srcbuftab_mmtimeOffset	DD	? ;  mmtime offset from beginning of buffer      
SRCBUFTAB	ENDS

TGTBUFTAB	STRUC
tgtbuftab_pBuffer	DD	?      ;  Ptr to buffer                               
tgtbuftab_ulBufId	DD	?      ;  Buffer ID. Passed to SH on GetFull, must    
                               ;  be passed back to SSM on ReturnEmpty.       
tgtbuftab_ulLength	DD	?     ;  Filled (actual) buffer length on GetFull,   
                               ;  unused on ReturnEmpty.                      
tgtbuftab_ulMessageParm	DD	? ;  Message to passed to application           
tgtbuftab_mmtimeOffset	DD	? ;  mmtime offset from beginning of buffer      
TGTBUFTAB	ENDS
;  EXTENDED SMH_NOTIFY Structures   

PARM_ENOTIFY	STRUC
smhenot_ulFunction	DD	?       ;  Input - SMH command function           
smhenot_hid	DD	?              ;  Input                                  
smhenot_hstream	DD	?          ;  Input                                  
smhenot_ulFlags	DD	?          ;  Input/Output                           
smhenot_ulGetNumEntries	DD	?  ;  Input/Output - # of entries in table   
smhenot_ulRetNumEntries	DD	?  ;  Input/Output - # of entries in table   
smhenot_pGetBufTab	DD	?       ;  Output - Ptr to buffer/record table    
smhenot_pRetBufTab	DD	?       ;  Input - Ptr to buffer/record table     
smhenot_ulParm1	DD	?          ;  Reserved for future use   
smhenot_ulParm2	DD	?          ;  Reserved for future use   
PARM_ENOTIFY	ENDS

ESRCBUFTAB	STRUC
esrcbuftab_pBuffer	DD	?      ;  Ptr to buffer                               
esrcbuftab_pRecord	DD	?      ;  Ptr to record in buffer (split-streams only)    
esrcbuftab_ulLength	DD	?     ;  Max buf length on GetEmpty, Filled          
                               ;  (actual) rec/buf length on ReturnFull.      
esrcbuftab_ulMessageParm	DD	? ;  Message to passed to application           
esrcbuftab_mmtimeOffset	DD	? ;  mmtime offset from beginning of buffer      
esrcbuftab_ulParm1	DD	?      ;  Record or buffer specific data   
esrcbuftab_ulParm2	DD	?      ;  Record or buffer specific data   
ESRCBUFTAB	ENDS

ETGTBUFTAB	STRUC
etgtbuftab_pBuffer	DD	?      ;  Ptr to buffer                               
etgtbuftab_ulBufId	DD	?      ;  Buffer ID. Passed to SH on GetFull, must    
                               ;  be passed back to SSM on ReturnEmpty.       
etgtbuftab_ulLength	DD	?     ;  Filled (actual) buffer length on GetFull,   
                               ;  unused on ReturnEmpty.                      
etgtbuftab_ulMessageParm	DD	? ;  Message to passed to application           
etgtbuftab_mmtimeOffset	DD	? ;  mmtime offset from beginning of buffer      
etgtbuftab_ulParm1	DD	?      ;  Buffer specific data   
etgtbuftab_ulParm2	DD	?      ;  Buffer specific data   
ETGTBUFTAB	ENDS

EPSRCBUFTAB	STRUC
epsrcbuftab_pBuffer	DD	?      ;  Ptr to buffer                               
epsrcbuftab_pRecord	DD	?      ;  Ptr to record in buffer (split-streams only)    
epsrcbuftab_ulLength	DD	?     ;  Max buf length on GetEmpty, Filled          
                               ;  (actual) rec/buf length on ReturnFull.      
epsrcbuftab_ulMessageParm	DD	? ;  Message to passed to application           
epsrcbuftab_mmtimeOffset	DD	? ;  mmtime offset from beginning of buffer      
epsrcbuftab_ulParm1	DD	?      ;  Record or buffer specific data   
epsrcbuftab_ulParm2	DD	?      ;  Record or buffer specific data   
epsrcbuftab_pProcessLin	DD	?  ;  Process Linear record ptr   
EPSRCBUFTAB	ENDS

EPTGTBUFTAB	STRUC
eptgtbuftab_pBuffer	DD	?      ;  Ptr to buffer                               
eptgtbuftab_ulBufId	DD	?      ;  Buffer ID. Passed to SH on GetFull, must    
                               ;  be passed back to SSM on ReturnEmpty.       
eptgtbuftab_ulLength	DD	?     ;  Filled (actual) buffer length on GetFull,   
                               ;  unused on ReturnEmpty.                      
eptgtbuftab_ulMessageParm	DD	? ;  Message to passed to application           
eptgtbuftab_mmtimeOffset	DD	? ;  mmtime offset from beginning of buffer      
eptgtbuftab_ulParm1	DD	?      ;  Buffer specific data   
eptgtbuftab_ulParm2	DD	?      ;  Buffer specific data   
eptgtbuftab_pProcessLin	DD	?  ;  Process Linear buffer ptr   
EPTGTBUFTAB	ENDS
; SMH_ulFlags:                                                                
BUF_GETEMPTY	EQU	00000001H    ;  Ptr returned in SMH_pGetBuffer    
BUF_RETURNFULL	EQU	00000002H    ;  Ptr passed in SMH_pRetBuffer      
BUF_GETFULL	EQU	00000004H    ;  Ptr returned in SMH_pGetBuffer    
BUF_RETURNEMPTY	EQU	00000008H    ;  Ptr passed in SMH_pRetBuffer      
BUF_GIVEBUF	EQU	00000010H    ;  Ptr passed in SMH_pRetBuffer      
BUF_EOS	EQU	00000100H    ;  End of stream. Source sets this on   
                                         ;   last buffer. Target sets it after   
                                         ;   consuming the laset buffer.         
BUF_RECORDS	EQU	00004000H    ;  One or more records (Split-stream)   
BUF_LASTRECORD	EQU	00008000H    ;  (BUF_RETURNFULL only) Mark           
                                         ;  these record(s) as being the         
                                         ;  last in the buffer. So the SSM       
                                         ;  knows the maximum # of records       
                                         ;  produced for this buffer.            
                                         ;  (Split stream only)                  
; SMH_ulFlags:                                                                     
BUF_LINEAR	EQU	00010000H    ;  Ptrs are Global linear                 
BUF_PHYSICAL	EQU	00020000H    ;  Ptrs are physical                      
                                         ;  (valid only for phyically              
                                         ;   contiguous buffers)                   
BUF_RESERVED	EQU	00040000H    ;  Reserved                               
BUF_GDT	EQU	00080000H    ;  Ptrs are GDT sel:offset                
BUF_EXTENDED	EQU	80000000H    ;  Use Extended SMH_NOTIFY structures     
BUF_EXTENDEDPTR	EQU	40000000H    ;  Use Extendedptr SMH_NOTIFY structures  
                                         ;   Implies BUF_EXTENDED                  
VALIDSOURCEFLAGS	EQU	(BUF_RETURNFULL OR BUF_GETEMPTY OR BUF_EOS OR BUF_LASTRECORD OR BUF_PHYSICAL OR BUF_LINEAR OR BUF_GDT OR BUF_RECORDS OR BUF_GIVEBUF OR BUF_EXTENDED OR BUF_EXTENDEDPTR)
VALIDTARGETFLAGS	EQU	(BUF_RETURNEMPTY OR BUF_GETFULL OR BUF_PHYSICAL OR BUF_LINEAR OR BUF_GDT OR BUF_EOS OR BUF_EXTENDED OR BUF_EXTENDEDPTR)
;***************************************************************************  
SMH_LOCKMEM	EQU	4               ;  SMH_LockMem()               

PARM_LOCKM	STRUC
smhloc_ulFunction	DD	?                  ;  SMH command function        
smhloc_pBuffer	DD	?
smhloc_ulBufSize	DD	?
smhloc_plockh	DD	?
smhloc_ulFlags	DD	?
PARM_LOCKM	ENDS
; SMH_ulFlags:                                                                
SSM_LOCKMEM	EQU	0000H          ;  Lock Memory.                
SSM_UNLOCKMEM	EQU	0001H          ;  Unlock memory.              
SSM_CONTIGLOCK	EQU	0002H          ;  Lock memory contiguously.   
SSM_LOCKMEM_RESERVED1	EQU	0100H          ;  reserved                    
SSM_LOCKMEM_RESERVED2	EQU	0200H          ;  reserved                    
;***************************************************************************  
;***************************************************************************  
;                                                                             
;   Function Prototypes for SHC functions (Stream Handler Functions)          
;   All use Pure C calling convention                                         
;                                                                             
;  NOTE ABOUT POINTERS:                                                       
;   0:32 linear addresses for ring 3 (DLL) stream handlers                    
;   16:16 selector/offset addresses for ring 0 (PDD) stream handlers          
;***************************************************************************  

SHC_COMMON	STRUC
shcfunc_ulFunction	DD	?        ;  Handler command function                 
shcfunc_hid	DD	?               ;  handler ID                               
SHC_COMMON	ENDS
;***************************************************************************  
SHC_ASSOCIATE	EQU	0 

PARM_ASSOC	STRUC
shcass_ulFunction	DD	?        ;  Handler command function               
shcass_hid	DD	?               ;  handler ID                             
shcass_hstream	DD	?           ;  handle of stream instance              
shcass_pacb	DD	?              ;  pointer to associate control block     
PARM_ASSOC	ENDS
;***************************************************************************  
SHC_CLOSE	EQU	1 

PARM_CLOSE	STRUC
shcclo_ulFunction	DD	?        ;  Handler command function               
shcclo_hid	DD	?               ;  handler ID                             
PARM_CLOSE	ENDS
;***************************************************************************  
SHC_CREATE	EQU	2 

PARM_CREATE	STRUC
shccrt_ulFunction	DD	?        ;  Handler command function               
shccrt_hid	DD	?               ;  handler ID                             
shccrt_hstream	DD	?           ;  handle of stream instance              
shccrt_spcbkey	DB	SIZE SPCBKEY DUP (?)           ;  type of stream to create               
shccrt_pspcb	DD	?             ;  Pointer to SPCB from handler           
shccrt_hstreamBuf	DD	?        ;  Used for split streams or to associate  
                                    ;  another stream with this stream.       
shccrt_pdcb	DD	?              ;  pointer to device control block        
PARM_CREATE	ENDS
;***************************************************************************  
SHC_DESTROY	EQU	3 

PARM_DESTROY	STRUC
shcdes_ulFunction	DD	?        ;  Handler command function               
shcdes_hid	DD	?               ;  handler ID                             
shcdes_hstream	DD	?           ;  handle of stream instance              
PARM_DESTROY	ENDS
;***************************************************************************  
SHC_START	EQU	4 

PARM_START	STRUC
shcstr_ulFunction	DD	?        ;  Handler command function               
shcstr_hid	DD	?               ;  handler ID                             
shcstr_hstream	DD	?           ;  handle of stream instance              
shcstr_ulFlags	DD	?           ;  Start flags                            
PARM_START	ENDS
;  ulFlags:                                                                   
FLUSH_START	EQU	0001H     ;  This start is a result of a Flush Stop command.   
;***************************************************************************  
SHC_STOP	EQU	5 

PARM_STOP	STRUC
shcstp_ulFunction	DD	?        ;  Handler command function               
shcstp_hid	DD	?               ;  handler ID                             
shcstp_hstream	DD	?           ;  handle of stream instance              
shcstp_ulFlags	DD	?           ;  Stop flags (refer to ulFlags defines under   
                                    ;   SpiStopStream() call.                 
PARM_STOP	ENDS
;***************************************************************************  
SHC_SEEK	EQU	6 

PARM_SEEK	STRUC
shcsek_ulFunction	DD	?        ;  Handler command function               
shcsek_hid	DD	?               ;  handler ID                             
shcsek_hstream	DD	?           ;  handle of stream instance              
shcsek_ulFlags	DD	?           ;  Seek flag (refer to ulFlags defines    
                                    ;   under SpiSeekStream() call).          
shcsek_lSeekPoint	DD	?        ;  seek to point , mmtime or other        
PARM_SEEK	ENDS
;***************************************************************************  
SHC_ENABLE_EVENT	EQU	7 

PARM_ENEVENT	STRUC
shceev_ulFunction	DD	?        ;  Handler command function               
shceev_hid	DD	?               ;  handler ID                             
shceev_hevent	DD	?            ;  handle of event to enable              
shceev_pevcbUser	DD	?         ;  User event information (e.g. hstream   
                                    ;   EventType, EventSubType)              
PARM_ENEVENT	ENDS
;***************************************************************************  
SHC_DISABLE_EVENT	EQU	8 

PARM_DISEVENT	STRUC
shcdev_ulFunction	DD	?        ;  Handler command function               
shcdev_hid	DD	?               ;  handler ID                             
shcdev_hstream	DD	?           ;  handle to stream for this event        
shcdev_hevent	DD	?            ;  handle of event to disable             
PARM_DISEVENT	ENDS
;***************************************************************************  
SHC_ENABLE_SYNC	EQU	9 

PARM_ENSYNC	STRUC
shcesy_ulFunction	DD	?        ;  Handler command function               
shcesy_hid	DD	?               ;  handler ID                             
shcesy_hstream	DD	?           ;  handle of stream instance              
shcesy_ulFlags	DD	?           ;  Sync flags                             
shcesy_mmtimeSync	DD	?        ;  granularity of sync interval (if null, use default)   
shcesy_pevcbSyncPulse	DD	?  ;  Sync pulse EVCB information            
shcesy_ulSyncPulseSem	DD	?    ;  Optional 16 bit system                 
                                    ;  Semaphore for handler                  
PARM_ENSYNC	ENDS
;  ulFlags:                                                                   
SYNC_MASTER	EQU	0000H     ;  The handler will be a master for this sync  
SYNC_SLAVE	EQU	0001H     ;  The handler will be a slave for this sync   
;***************************************************************************  
SHC_DISABLE_SYNC	EQU	10 

PARM_DISSYNC	STRUC
shcdsy_ulFunction	DD	?        ;  Handler command function               
shcdsy_hid	DD	?               ;  handler ID                             
shcdsy_hstream	DD	?           ;  handle of stream instance              
PARM_DISSYNC	ENDS
;***************************************************************************  
SHC_GET_TIME	EQU	11 

PARM_GTIME	STRUC
shcgti_ulFunction	DD	?        ;  Handler command function               
shcgti_hid	DD	?               ;  handler ID                             
shcgti_hstream	DD	?           ;  handle of stream instance              
shcgti_mmtimeCurrent	DD	?     ;  Returns current stream time            
PARM_GTIME	ENDS
;***************************************************************************  
SHC_GET_PROTOCOL	EQU	12 

PARM_GPROT	STRUC
shcgpt_ulFunction	DD	?        ;  Handler command function               
shcgpt_hid	DD	?               ;  handler ID                             
shcgpt_spcbkey	DB	SIZE SPCBKEY DUP (?)           ;  key of SPCB                            
shcgpt_pspcb	DD	?             ;  ptr to SPCB (output)                   
PARM_GPROT	ENDS
;***************************************************************************  
SHC_INSTALL_PROTOCOL	EQU	13 

PARM_INSTPROT	STRUC
shcipt_ulFunction	DD	?        ;  Handler command function               
shcipt_hid	DD	?               ;  handler ID                             
shcipt_spcbkey	DB	SIZE SPCBKEY DUP (?)           ;  key of SPCB                            
shcipt_pspcb	DD	?             ;  ptr to SPCB to install                 
shcipt_ulFlags	DD	?           ;  Install / Deinstall flags              
PARM_INSTPROT	ENDS
;***************************************************************************  
SHC_ENUMERATE_PROTOCOLS	EQU	14 

PARM_ENUMPROT	STRUC
shcqpi_ulFunction	DD	?        ;  Handler command function               
shcqpi_hid	DD	?               ;  handler ID                             
shcqpi_paSPCBKeys	DD	?        ;  ptr to buffer to fill with SPCB keys   
shcqpi_pulNumSPCBKeys	DD	?    ;  # of entries buffer on input,          
                                    ;   # of SPCB key on output.              
PARM_ENUMPROT	ENDS
;***************************************************************************  
SHC_NEGOTIATE_RESULT	EQU	15 

PARM_NEGOTIATE	STRUC
shcneg_ulFunction	DD	?        ;  Handler command function               
shcneg_hid	DD	?               ;  handler ID                             
shcneg_hstream	DD	?           ;  handle of stream instance              
shcneg_pspcb	DD	?             ;  Ptr to negotiated SPCB                 
shcneg_ulErrorStatus	DD	?     ;  On error, indicates which field in the  
PARM_NEGOTIATE	ENDS
;***************************************************************************  
SHC_SENDMSG	EQU	16 

PARM_SNDMSG	STRUC
shcsndmsg_ulFunction	DD	?        ;  Handler command function               
shcsndmsg_hid	DD	?               ;  handler ID                             
shcsndmsg_hstream	DD	?           ;  handle of stream instance              
shcsndmsg_ulMsgType	DD	?         ;  Stream handler message type            
shcsndmsg_pMsg	DD	?              ;  Pointer to Message control block       
PARM_SNDMSG	ENDS
;***************************************************************************  
