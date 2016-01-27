;***************************************************************************  
;                                                                             
;  Module Name: ACB.H                                                         
;                                                                             
;  OS/2 2.0 Multimedia Extensions Sync/Stream Manager Associate               
;  Control Block Definitions.                                                 
;                                                                             
;  Copyright (c) International Business Machines Corporation 1991, 1992       
;                         All Rights Reserved                                 
;                                                                             
;***************************************************************************  
MAX_PORT_NAME	EQU	40            ;  Max port name length           
MAX_PORTS	EQU	16            ;  Max number ports               
;**********************************************
;*
;* ACB - Associate Control Block
;*
;*       Each stream handler must define the
;*       ACB for each data object type that
;*       expect ACB's from the application.
;*
;*********************************************  

ACB	STRUC
acb_ulACBLen	DD	?              ;  length of structure                     
acb_ulObjType	DD	?
acb_ulParm1	DD	?
acb_ulParm2	DD	?
ACB	ENDS
;******************************************************
;* FSSH - File system stream handler MMIO Object ACB
;*****************************************************  
ACBTYPE_MMIO	EQU	0001H   ;  MMIO object                        

ACB_MMIO	STRUC
acbmmio_ulACBLen	DD	?              ;  length of structure                     
acbmmio_ulObjType	DD	?             ;  ACBTYPE_MMIO                            
acbmmio_hmmio	DD	?                 ;  Handle of media element mgr obj         
ACB_MMIO	ENDS
;******************************************************
;* MSH - Memory stream handler Playlist Object ACB
;*****************************************************  
ACBTYPE_MEM_PLAYL	EQU	0003H   ;  Memory playlist object             

ACB_MEM_PLAYL	STRUC
acbplyl_ulACBLen	DD	?              ;  length of structure                     
acbplyl_ulObjType	DD	?             ;  ACBTYPE_MEM_PLAYL                       
acbplyl_pMemoryAddr	DD	?           ;  Starting address of memory obj          
ACB_MEM_PLAYL	ENDS
;******************************************************
;* CDDASH - CD DA stream handler Object ACB
;*****************************************************  
ACBTYPE_CDDA	EQU	0004H   ;   Compact disk - digital audio obj  

ACB_CDDA	STRUC
acbcdda_ulACBLen	DD	?              ;  length of structure                     
acbcdda_ulObjType	DD	?             ;  ACBTYPE_CDDA                            
acbcdda_bCDDrive	DB	?              ;  CD drive letter                         
ACB_CDDA	ENDS
;******************************************************
;* MISH - MIDI stream handler port-stream table ACB
;*****************************************************  
ACBTYPE_MISH	EQU	0005H       ;  MIDI port-stream table         

ACB_MISH	STRUC
acbmish_ulACBLen	DD	?                     ;  length of structure            
acbmish_ulObjType	DD	?                    ;  ACBTYPE_MISH                   
acbmish_hstreamDefault	DD	?               ;  Default hstream to use when mapper   
                                           ;  is turned off.                 
acbmish_ulDeviceTypeID	DD	?               ;  device type id                 
acbmish_ulpMapperPorts	DD	?               ;  pointer to mapper port table   
acbmish_ulNumInStreams	DD	?
acbmish_hstreamIn	DD	MAX_PORTS DUP (?)         ;  Array of Input streams         
acbmish_ulNumOutStreams	DD	?
acbmish_hstreamOut	DD	MAX_PORTS DUP (?)        ;  Array of Output streams        
                                           ;  The index into the array is    
                                           ;  the source channel for that    
                                           ;  stream.                        
ACB_MISH	ENDS
;******************************************************
;* MISH - MIDI stream handler SET ACB
;*****************************************************  
ACBTYPE_SET	EQU	0006H       ;  MIDI set function              

ACB_SET	STRUC
acbset_ulACBLen	DD	?                     ;  length of structure            
acbset_ulObjType	DD	?                    ;  ACBTYPE_SET                    
acbset_ulFlags	DD	?                      ;  Set flags                      
acbset_ulReserved	DD	?                   ;  Reserved                       
ACB_SET	ENDS
;  ulFlags defines:                                                          
MIDI_MAP_ON	EQU	0000H       ;  turn mapping function on in MISH     
MIDI_MAP_OFF	EQU	0001H       ;  turn mapping function off in MISH    
;******************************************************
;* NULLSH - Null stream handler user ACB
;*****************************************************  
ACBTYPE_NULLSH	EQU	0007H   ;  User od Null stream handler        

ACB_NULLSH	STRUC
acbnullsh_ulACBLen	DD	?              ;  length of structure                     
acbnullsh_ulObjType	DD	?             ;  ACBTYPE_NULLSH                          
acbnullsh_pfnEntry	DD	?              ;  user callback entry point               
acbnullsh_ulReserved	DD	?            ;  Reserved                                
ACB_NULLSH	ENDS
;******************************************************
;* MTSH - MultiTrack Stream Handler Object ACB
;*****************************************************  
ACBTYPE_MTSH	EQU	0008H   ;  MTSH object                        

ACB_MTSH	STRUC
acbmtsh_ulACBLen	DD	?         ;  length of structure                     
acbmtsh_ulObjType	DD	?        ;  ACBTYPE_MTSH                            
acbmtsh_hmmio	DD	?            ;  Handle of media element mgr obj         
acbmtsh_mmtrackInfo	DB	SIZE MMTRACKINFO DUP (?)      ;  track for this stream                   
acbmtsh_ulFlags	DD	?          ;  flags                                   
acbmtsh_ulMaxBytesPerSec	DD	? ;  Max bytes per second                    
acbmtsh_ulAvgBytesPerSec	DD	? ;  Avg bytes per second                    
acbmtsh_mmtimePerFrame	DD	?   ;  frame display time or 0L                
acbmtsh_ulTotalFrames	DD	?    ;  total number of video frames            
ACB_MTSH	ENDS
;  ulFlags defines:                                                          
TRACK_ACTIVATE	EQU	0000H     ;  Activate track in stream handler    
TRACK_DEACTIVATE	EQU	0001H     ;  Deactivate track in stream handler    
;******************************************************
;* CSH - CODEC stream handler Object ACB
;*****************************************************  
ACBTYPE_CODECSH	EQU	0009H       ;  CODEC object                   

ACB_CODECSH	STRUC
acbcodecsh_ulACBLen	DD	?              ;  length of structure            
acbcodecsh_ulObjType	DD	?             ;  ACBTYPE_CODECSH                
acbcodecsh_hstreamToPair	DD	?         ;  2nd stream of pair             
acbcodecsh_pMmioInfo	DD	?             ;  info for IO Proc               
acbcodecsh_ulInfoLength	DD	?          ;  length of MmioInfo             
acbcodecsh_pCodecControl	DD	?         ;  CODEC control info             
acbcodecsh_ulControlLength	DD	?       ;  length of CodecControl         
ACB_CODECSH	ENDS
