_SHDD_H_	EQU	1

;****************************************************************************
; Module Name: SHDD.H
;
; OS/2 2.0 Multimedia Extensions Stream Handler Device Driver Interfaces
; Block defintions.
;
; Copyright (c) International Business Machines Corporation 1990,1991
;                         All Rights Reserved
;
;**************************************************************************  
;***************************************************************************  
;                          D E F I N E S                                      
;***************************************************************************  
ifdef XXXXX                                 ;  Picked up by H2INC only      
                                             ;  when parsed by H2INC.        
endif                                       ;                               
;***************************************************************************  
;                        S T R U C T U R E S                                  
;***************************************************************************  
;***************************************************************************  
;****   DDCMD Interface                                                    *  
;***************************************************************************  
;  DDCMD Defines   
DDCMD_SETUP	EQU	0 
DDCMD_READ	EQU	1 
DDCMD_WRITE	EQU	2 
DDCMD_STATUS	EQU	3 
DDCMD_CONTROL	EQU	4 
DDCMD_REG_STREAM	EQU	5 
DDCMD_DEREG_STREAM	EQU	6 

ddcmd_common_parm	STRUC
comm_ulFunction	DD	?     ;  Function requested by SH   
comm_hStream	DD	?        ;  data stream instance       
ddcmd_common_parm	ENDS

ddcmd_setup_parm	STRUC
setu_ulFunction	DD	?     ;  Function requested by SH   
setu_hStream	DD	?
setu_pSetupParm	DD	?      ;  see SETUP_PARM struct     
setu_ulSetupParmSize	DD	? ;  see SETUP_PARM struct     
ddcmd_setup_parm	ENDS
                ;*********************************************  
                ;  SETUP_PARM structure                         
                ;*********************************************  

SETUP_PARM	STRUC
setu_parm_ulStreamTime	DD	? ;  stream time in milliseconds   
setu_parm_ulFlags	DD	?      ;  various flags (input/output)  
                        ;  NOTE: new fields will be added here                 
SETUP_PARM	ENDS
                ;*********************************************  
                ;  ulFlag defines                               
                ;*********************************************  
SETUP_RECURRING_EVENTS	EQU	00000001H 
                ;  the device driver sets this flag on return from the       
                ;  DDCMD_SETUP command if the device driver assumes events   
                ;  are recurring events---in this case, the stream handler   
                ;  will not have to re-enable a recurring event each time    
                ;  the event occurs by sending a DDCMD_CONTROL command to    
                ;  the device driver.  this is useful when CUE_TIME or       
                ;  DATA_CUE events are expected to be used as RECURRING      

ddcmd_readwrite_parm	STRUC
read_ulFunction	DD	?     ;  Function requested by SH   
read_hStream	DD	?
read_pBuffer	DD	?
read_ulBufferSize	DD	?
read_pProcessLin	DD	?
read_fEOS	DD	?           ;  Is this the EOS buffer???  
read_ulParm1	DD	?        ;  Reserved for future use    
read_ulParm2	DD	?        ;  Reserved for future use    
read_ulLength	DD	?
ddcmd_readwrite_parm	ENDS

ddcmd_status_parm	STRUC
stat_ulFunction	DD	?     ;  Function requested by SH   
stat_hStream	DD	?
stat_pStatus	DD	?        ;  (output)ptr to current position time   
stat_ulStatusSize	DD	?   ;  (output)size of position time          
ddcmd_status_parm	ENDS
                ;*********************************************  
                ;  STATUS_PARM structure                        
                ;*********************************************  

STATUS_PARM	STRUC
stat_parm_ulTime	DD	?       ;  current position time in milliseconds       
                        ;  NOTE: new fields will be added here                               
STATUS_PARM	ENDS

ddcmd_control_parm	STRUC
cont_ulFunction	DD	?     ;  Function requested by SH   
cont_hStream	DD	?
cont_hEvent	DD	?         ;  used for EVENTS only                   
cont_ulCmd	DD	?
cont_pParm	DD	?          ;  see CONTROL_PARM structure    
cont_ulParmSize	DD	?     ;  see CONTROL_PARM structure   
ddcmd_control_parm	ENDS
                ;*********************************************  
                ;  ulCmd defines                                
                ;*********************************************  
                                          ;  CHANGE and WAIT not implemented                     
DDCMD_START	EQU	1              ;  start device                                        
DDCMD_STOP	EQU	2              ;  stop device and return current position in pParm    
DDCMD_PAUSE	EQU	3              ;  pause device and return current position in pParm   
DDCMD_RESUME	EQU	4              ;  resume device                                       
DDCMD_ENABLE_EVENT	EQU	5              ;  ask PDD to create this event                        
DDCMD_DISABLE_EVENT	EQU	6              ;  ask PDD to purge this event                         
DDCMD_PAUSE_TIME	EQU	7              ;  pause time keeping, but not the stream              
DDCMD_RESUME_TIME	EQU	8              ;  resume time keeping.                                
                ;*********************************************  
                ;  CONTROL_PARM structure                       
                ;*********************************************  

CONTROL_PARM	STRUC
cont_parm_ulTime	DD	?       ;  time in milliseconds                         
                                              ;  SH sets cuetime when ulCmd is ENABLE_EVENT   
                                              ;  PDD returns current time for STOP, PAUSE     
cont_parm_evcb	DB	SIZE TIME_EVCB DUP (?)
CONTROL_PARM	ENDS
                ;******************************************************************************  
                ;  This structure is filled in by the amp mixer based on the instance            
                ;  data.                                                                         
                ;******************************************************************************  

AUDIOMODE	STRUC
audio_lSRate	DD	?            ;  sample rate                
audio_lBitsPerSRate	DD	?     ;  bits per sample            
audio_sChannels	DW	?        ;  Channels                   
AUDIOMODE	ENDS

ddcmd_register_parm	STRUC
regi_ulFunction	DD	?     ;  Function requested by SH                      
regi_hStream	DD	?        ;  Stream handle needed @ interrupt time         
regi_ulSysFileNum	DD	?   ;  Device Handle so pdd can map device instance to hstream   
regi_pSHDEntryPoint	DD	? ;  Stream handler entry point                    
regi_ulStreamOperation	DD	? ;  SH input Record or play                    
regi_spcbkey	DB	SIZE SPCBKEY DUP (?)
regi_ulBufSize	DD	?      ;  PDD output (optional) buffer size in bytes for SPCB   
regi_ulNumBufs	DD	?      ;  PDD output (optional) # of buffers for SPCB           
regi_ulAddressType	DD	?  ;  PDD output (required) addr ptr type to data buffer    
regi_ulBytesPerUnit	DD	? ; PDD output (required)                                  
regi_mmtimePerUnit	DD	?  ; PDD output (required)                                  
regi_dcbAudio	DB	SIZE E_DCB DUP (?)       ;  SH input Device Control Block   
regi_hid	DD	?            ;  SH input stream handler id   
regi_AudioMode	DB	SIZE AUDIOMODE DUP (?)    ;  PPC - Audio mode data structure   
ddcmd_register_parm	ENDS
                ;****************************  
                ;  ulStreamOperation  defines  
                ;****************************  
STREAM_OPERATION_MASK	EQU	0C000H 
STREAM_OPERATION_CONSUME	EQU	8000H 
STREAM_OPERATION_PRODUCE	EQU	0C000H 
                ;******************************************************************************  
                ;  ulAddressType  defines                                                        
                ;  The PDD will tell the SH what type of address ptr it expects the data buffer  
                ;  to be.  The SH will then request this address type to the SSM, so that the    
                ;  SSM will send the correct type of each buffer request.                        
                ;******************************************************************************  
ADDRESS_TYPE_VIRTUAL	EQU	0 
ADDRESS_TYPE_PHYSICAL	EQU	1               ; default                    
ADDRESS_TYPE_LINEAR	EQU	2 

ddcmd_deregister_parm	STRUC
dere_ulFunction	DD	?     ;  Function requested by SH   
dere_hStream	DD	?        ;  Stream handle needed @ interrupt time   
ddcmd_deregister_parm	ENDS
;***************************************************************************  
;**** RING 0 Stream Handler SHD Interfaces                                **  
;***************************************************************************  
;  RING 0 Stream Handler  SHD's:                                              
SHD_REPORT_INT	EQU	0       ;  PDD reports interrupt has arrived  
SHD_REPORT_EVENT	EQU	1       ;  PDD reports cuetime has arrived    

shd_common_parm	STRUC
shdc_ulFunction	DD	?     ;  Function requested by PDD                 
shdc_hStream	DD	?        ;  Stream handle needed @ interrupt time     
shd_common_parm	ENDS

shd_reportint_parm	STRUC
repi_ulFunction	DD	?     ;  Function requested by PDD  
repi_hStream	DD	?        ;  so SH knows which stream to process       
repi_pBuffer	DD	?        ;  return ptr to last used buffer            
repi_ulFlag	DD	?         ;  reason for interrupt       
repi_ulStatus	DD	?       ;  rc or bytes read/written   
repi_ulStreamTime	DD	?   ;  time in milliseconds of stream position    
shd_reportint_parm	ENDS
                ;******************  
                ;  ulFlag settings   
                ;******************  
ERROR	EQU	80H 
STREAM_STOP_NOW	EQU	40H 
SHD_READ_COMPLETE	EQU	1 
SHD_WRITE_COMPLETE	EQU	2 

shd_reportevent_parm	STRUC
repe_ulFunction	DD	?     ;  Function requested by PDD  
repe_hStream	DD	?        ;  so SH knows which stream to process       
repe_hEvent	DD	?         ;  event handle pass back to stream handler  
repe_ulStreamTime	DD	?   ;  time in milliseconds of stream position    
shd_reportevent_parm	ENDS
;***************************************************************************  
;**** RING 3 Stream Handler SHD Interfaces                                **  
;***************************************************************************  
;  RING 3 Stream Handler  SHD's: (Uses SpiSendMsg as interface)   
SHC_REPORT_INT	EQU	80000000H    ;  ulMsgType for SHD_REPORT_INT   
SHC_REPORT_EVENT	EQU	80000001H    ;  ulMsgType for SHD_REPORT_EVENT   
;**********************************************
;*
;* MSG_REPORTINT Control Block.
;*  pMsg field of SpiSendMsg api
;*
;*********************************************  

MSG_REPORTINT	STRUC
msgrepi_ulMsgLen	DD	?       ;  Length of structure                       
msgrepi_pBuffer	DD	?        ;  return ptr to last used buffer            
msgrepi_ulFlag	DD	?         ;  reason for interrupt                      
msgrepi_ulStatus	DD	?       ;  rc or bytes read/written                  
msgrepi_ulStreamTime	DD	?   ;  time in milliseconds of stream position   
MSG_REPORTINT	ENDS
   ;  For ulFlag defines, refer to the _shd_reportint_parm structure   
;**********************************************
;*
;* MSG_REPORTEVENT Control Block.
;*  pMsg field of SpiSendMsg api
;*
;*********************************************  

MSG_REPORTEVENT	STRUC
msgrepe_ulMsgLen	DD	?       ;  Length of structure                        
msgrepe_hevent	DD	?         ;  event handle pass back to stream handler   
msgrepe_ulStreamTime	DD	?   ;  time in milliseconds of stream position    
MSG_REPORTEVENT	ENDS
;***************************************************************************  
;               F U N C T I O N   P R O T O T Y P E S                         
;***************************************************************************  
