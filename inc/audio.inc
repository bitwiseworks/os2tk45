;***************************************************************************  
;                                                                             
;  Module Name: AUDIO.H                                                       
;                                                                             
;  OS/2 2.0 Multimedia Extensions Audio Structures and definitions.           
;                                                                             
;  Copyright (c) International Business Machines Corporation 1991, 1992       
;                         All Rights Reserved                                 
;                                                                             
;***************************************************************************  
; -------------------------------------------------------------------------*
; AUDIODD version level
;-------------------------------------------------------------------------  
CURRENT_VERSION	EQU	01020000H 
; -------------------------------------------------------------------------*
; Control definitions
;-------------------------------------------------------------------------  
AUDIO_IOCTL_CAT	EQU	80H 
AUDIO_INIT	EQU	40H 
AUDIO_STATUS	EQU	41H 
AUDIO_CONTROL	EQU	42H 
AUDIO_BUFFER	EQU	43H 
AUDIO_LOAD	EQU	44H 
AUDIO_WAIT	EQU	45H 
AUDIO_HPI	EQU	46H 
AUDIO_CAPABILITY	EQU	48H 
MIX_GETCONNECTIONS	EQU	60H 
MIX_SETCONNECTIONS	EQU	61H 
MIX_GETLINEINFO	EQU	62H 
MIX_GETCONTROL	EQU	63H 
MIX_SETCONTROL	EQU	64H 
; -------------------------------------------------------------------------*
; AUDIO_INIT declarations and defines
;-------------------------------------------------------------------------  
LOAD_PATH	EQU	260 
;  Values for AUDIO_INIT.sMode    
ADPCM	EQU	1                        ;  AVC type ADPCM                  
PCM	EQU	2                        ;  Pulse Coded Modulation          
MU_LAW	EQU	3                        ;  mu-law                          
MIDI	EQU	4                        ;  MIDI data                       
A_LAW	EQU	5                        ;  a-law                           
SOURCE_MIX	EQU	6                      ;  External audio source           
SPV2	EQU	7                        ;  Speech Viewer/2                 
ADPCMXA	EQU	8                        ;  XA CD ROM                       
SPV2BCPCM	EQU	25                 ;  Speech Viewer/2                 
SPV2PCM	EQU	26 
SPV2NONE	EQU	27 
IDLE	EQU	999 
CLAIM_HDWR	EQU	32000                  ;  Serialize access to hardware    
;  Values for AUDIO_INIT.ulFlags          
FIXED	EQU	00000001H l    ;  Fixed length data               
LEFT_ALIGNED	EQU	00000002H l    ;  Left align bits on byte bndry   
RIGHT_ALIGNED	EQU	00000004H l    ;  Right align bits on byte bndry  
TWOS_COMPLEMENT	EQU	00000008H l    ;  2's complement data             
SIGNED	EQU	00000010H l    ;  Signed data                     
BIG_ENDIAN	EQU	00000020H l    ;  MSB's first (motorola format)   
RIFF_DATATYPE	EQU	00000040H l    ;  sMode contains a RIFF datatype  
PITCH	EQU	00100000H l    ;  Pitch control is supported      
INPUT	EQU	00200000H l    ;  Input select is supported       
OUTPUT	EQU	00400000H l    ;  Output select is supported      
MONITOR	EQU	00800000H l    ;  Monitor is supported            
VOLUME	EQU	01000000H l    ;  Volume control is supported     
VOLUME_DELAY	EQU	02000000H l    ;  Volume delay is supported       
BALANCE	EQU	04000000H l    ;  Balance control is supported    
BALANCE_DELAY	EQU	08000000H l    ;  Balance delay is supported      
TREBLE	EQU	10000000H l    ;  Treble control is supported     
BASS	EQU	20000000H l    ;  Bass control supported          
BESTFIT_PROVIDED	EQU	40000000H l    ;  bestfit returned                
LOAD_CODE	EQU	80000000H l    ;  DSP load needed                 
;  Values for AUDIO_INIT.ulOperation      
OPERATION_PLAY	EQU	1 
OPERATION_RECORD	EQU	2 
PLAY_AND_RECORD	EQU	3 
ANALYSIS	EQU	6               ;  Speech Viewer/2                 
DISTANCE	EQU	7               ;  Speech Viewer/2                 
MIGRATION	EQU	8               ;  Speech Viewer/2                 
;  Values for AUDIO_INIT.sReturnCode      
NO_PLAY	EQU	1 
NO_RECORD	EQU	2 
NO_RECORD_AND_PLAY	EQU	3 
INVALID_REQUEST	EQU	4 
CONFLICT	EQU	5 
OVERLOADED	EQU	6 
DOWNLEVEL_DD	EQU	7   ;  DD is down level from appl.     
DSP_LOAD_PENDING_ON_OTHER_TRK	EQU	8   ;  Other trk hasn't loaded dsp     
AUDIO_IGNORE	EQU	-1 
;  Values for AUDIO_INIT.sDeviceID        
MINIDD	EQU	0 
ACPA	EQU	1 
MACPA	EQU	2 
MPU401	EQU	3 
SOUND_BLASTER	EQU	4 
IMF	EQU	5 
PS1	EQU	6 
PAS16	EQU	7 
;  AUDIO_INIT structure declaration       

init	STRUC
init_lSRate	DD	?
init_lBitsPerSRate	DD	?
init_lBsize	DD	?
init_sMode	DW	?
init_sChannels	DW	?
init_lResolution	DD	?
init_abLoadPath	DB	LOAD_PATH DUP (?)
init_ulFlags	DD	?
init_ulOperation	DD	?
init_sReturnCode	DW	?
init_sSlotNumber	DW	?
init_sDeviceID	DW	?
init_pvReserved	DD	?       ;  MMPM2 uses this to pass back sysfilenum   
init_ulVersionLevel	DD	?
init	ENDS
; -------------------------------------------------------------------------*
; AUDIO_DEVID declarations and defines
;-------------------------------------------------------------------------  

MCI_AUDIO_DEVID	STRUC
devid_ulDevType	DD	?
devid_ulDevNum	DD	?
MCI_AUDIO_DEVID	ENDS
;  Input devices          
NULL_INPUT	EQU	0 
STEREO_LINE_INPUT	EQU	1 
LEFT_LINE_INPUT	EQU	2 
RIGHT_LINE_INPUT	EQU	3 
MIC_INPUT	EQU	4 
BOOSTED_MIC_INPUT	EQU	5 
PHONE_LINE_INPUT	EQU	6 
HANDSET_INPUT	EQU	7 
SYNTH_INPUT	EQU	8 
DIGITAL_PHONE_LINE_INPUT	EQU	9 
DIGITAL_HANDSET_INPUT	EQU	10 
MIDI_IN_PORT	EQU	11 
LOOPBACK	EQU	11 
DEFAULT_INPUT	EQU	0FFFFFFFFH 
;  Output devices         
NULL_OUTPUT	EQU	0 
STEREO_LINE_OUTPUT	EQU	1 
LEFT_LINE_OUTPUT	EQU	2 
RIGHT_LINE_OUTPUT	EQU	3 
SPEAKER_OUTPUT	EQU	4 
HEADSET_OUTPUT	EQU	5 
PHONE_LINE_OUTPUT	EQU	6 
HANDSET_OUTPUT	EQU	7 
SYNTH_OUTPUT	EQU	8 
DIGITAL_PHONE_LINE_OUTPUT	EQU	9 
DIGITAL_HANDSET_OUTPUT	EQU	10 
MIDI_OUT_PORT	EQU	11 
DEFAULT_OUTPUT	EQU	0FFFFFFFFH 
;  Defined values for DevNum      
DEFAULT_DEVICE	EQU	0 
DEVICE_1	EQU	1 
DEVICE_2	EQU	2 
; -------------------------------------------------------------------------*
; Valid Return codes for the ulSupport field of MCI_AUDIO_CAPS
;-------------------------------------------------------------------------  
SUPPORT_SUCCESS	EQU	00000000H 
UNSUPPORTED_RATE	EQU	00000001H 
UNSUPPORTED_CHANNELS	EQU	00000002H 
UNSUPPORTED_BPS	EQU	00000004H 
UNSUPPORTED_DATATYPE	EQU	00000008H 
UNSUPPORTED_OPERATION	EQU	00000010H 

MCI_AUDIO_CAPS	STRUC
ulLength	DD	?                 ;  in     Structure length                      
ulSamplingRate	DD	?           ;  in out Sampling rate to query                
ulChannels	DD	?               ;  in out Channels to query                     
ulBitsPerSample	DD	?          ;  in out BPS to query                          
ulDataType	DD	?               ;  in out RIFF Datatype to query                
ulOperation	DD	?              ;  in out OPERATION_PLAY or OPERATION_RECORD    
ulSupport	DD	?                ;     out BOOLEAN-does DD support this mode     
ulDataSubType	DD	?            ;     out Data-subtype to use                   
ulResourceUnits	DD	?          ;     out Resource units this mode              
ulResourceClass	DD	?          ;     out Resource class for this mode          
ulBlockAlign	DD	?             ;     out Block alignment for this mode.        
fCanRecord	DD	?               ;     out Is recording possbile - this mode     
ulFlags	DD	?                  ;     out                                       
ulCapability	DD	?             ;     out Capability of the device.             
MCI_AUDIO_CAPS	ENDS
; -------------------------------------------------------------------------*
; Valid Return codes for the ul field of MCI_AUDIO_CAPS
;-------------------------------------------------------------------------  
SUPPORT_MIX	EQU	00000001H  ;  supports mixer functions             
SUPPORT_RIFF_MODES	EQU	00000002H  ;  supports RIFF modes for AUDIO_INIT   
SUPPORT_CAP	EQU	80000000H  ;  Capability IOCTL supported           
; -------------------------------------------------------------------------*
; AUDIO_CHANGE declarations and defines
;-------------------------------------------------------------------------  
;  Values for AUDIO_CHANGE.lMonitor       
MONITOR_OFF	EQU	0 
MONITOR_UNCOMPRESSED	EQU	1 
MONITOR_COMPRESSED	EQU	2 
;  Values for AUDIO_CHANGE.lInput         
HIGH_GAIN_MIC	EQU	0 
LINE_1	EQU	1 
LINE_2	EQU	2 
LINES_1AND2	EQU	3 
LOW_GAIN_MIC	EQU	4 
ALL_LINES	EQU	0FFFFH 
INPUTS_LISTED	EQU	5555H 
;  Values for AUDIO_CHANGE.lOutput        
EXTERNAL_SPEAKER	EQU	1 
INTERNAL_SPEAKER	EQU	2 
OUTPUT_1	EQU	4 
OUTPUTS_LISTED	EQU	5555H 
;  AUDIO_CHANGE structure declaration     

MCI_AUDIO_CHANGE	STRUC
chang_pvDevInfo	DD	?
chang_lInput	DD	?
chang_lOutput	DD	?
chang_lMonitor	DD	?
chang_lVolume	DD	?
chang_lVolumeDelay	DD	?
chang_lBalance	DD	?
chang_lBalanceDelay	DD	?
chang_lTreble	DD	?
chang_lBass	DD	?
chang_lPitch	DD	?
chang_rInputList	DB	SIZE MCI_AUDIO_DEVID * 8 DUP (?)
chang_rOutputList	DB	SIZE MCI_AUDIO_DEVID * 8 DUP (?)
chang_prMoreInputs	DD	?
chang_prMoreOutputs	DD	?
chang_lGain	DD	?
chang_pvModeInfo	DD	?
MCI_AUDIO_CHANGE	ENDS

mode_info	STRUC
mode_info_sTempo	DW	?
mode_info_sCPQN	DW	?
mode_info_sMidiSwitches	DW	?
mode_info_sReserved	DW	5 DUP (?)
mode_info	ENDS
;*************************************************************************
;  MIDI Switches
;*************************************************************************  
MIDI_THRU_THRU	EQU	0 
MIDI_THRU_OUT	EQU	1 
; -------------------------------------------------------------------------*
; AUDIO_STATUS declarations and defines
;-------------------------------------------------------------------------  
;  Values for AUDIO_STATUS.ulOperation    
STOPPED	EQU	0 
PLAYING	EQU	1 
RECORDING	EQU	2 
PLAYING_AND_RECORDING	EQU	3 
UNITIALIZED	EQU	0FFFFFFFFH 
;  AUDIO_STATUS structure declaration     

status	STRUC
status_lSRate	DD	?
status_lBitsPerSRate	DD	?
status_lBsize	DD	?
status_sMode	DW	?
status_sChannels	DW	?
status_ulFlags	DD	?
status_ulOperation	DD	?
status_rAudioChange	DB	SIZE MCI_AUDIO_CHANGE DUP (?)
status	ENDS
; -------------------------------------------------------------------------*
; AUDIO_CONTROL declarations and defines
;-------------------------------------------------------------------------  
;  Values for AUDIO_CONTROL.usIOCtlRequest        
AUDIO_CHANGE	EQU	0 
AUDIO_START	EQU	1 
AUDIO_STOP	EQU	2 
AUDIO_PAUSE	EQU	3 
AUDIO_RESUME	EQU	4 
;  Values for AUDIO_CONTROL.sReturnCode   
AC_UNINITED	EQU	1        ;  Device must be init'ed or loaded first      
FULL_QUEUE	EQU	2        ;  Maximum # requests exceeded          
AC_UNPAUSED	EQU	3        ;  Resume issued, but dev not paused    
INVALID_REQUEST	EQU	4        ;  bad audio_control.ioctl_request      
AC_UNSTARTED	EQU	5        ;  Device must be started first         
INVALID_INPUT_LIST	EQU	7        ;  invalid change.input_list entry      
INVALID_OUTPUT_LIST	EQU	8        ;  invalid change.output_list entry     
;  AUDIO_CONTROL structure declaration    

contr	STRUC
contr_usIOCtlRequest	DW	?
contr_pbRequestInfo	DD	?
contr_ulPosition	DD	?
contr_sReturnCode	DW	?
contr	ENDS
; -------------------------------------------------------------------------*
; AUDIO_BUFFER declarations and defines
;-------------------------------------------------------------------------  
;  Values for AUDIO_BUFFER.ulFlags        
AUDIO_UNDERRUN	EQU	1 
AUDIO_OVERRUN	EQU	2 
;  Values for AUDIO_BUFFER.ulPositionType         
POS_MSECS	EQU	0 
MIDI_CLOCKS	EQU	1 
SMPTE_24	EQU	24 
SMPTE_25	EQU	25 
SMPTE_30DF	EQU	29 
SMPTE_30	EQU	30 

buffer	STRUC
buffer_ulFlags	DD	?
buffer_ulReadBufSize	DD	?
buffer_ulWriteBufSize	DD	?
buffer_ulReadBufTime	DD	?
buffer_ulWriteBufTime	DD	?
buffer_ulReadBufMax	DD	?
buffer_ulWriteBufMax	DD	?
buffer_ulPosition	DD	?
buffer_ulPositionType	DD	?
buffer_lReadBufCap	DD	?
buffer_lWriteBufCap	DD	?
buffer_lRequestBufCap	DD	?
buffer	ENDS
; -------------------------------------------------------------------------*
; AUDIO_LOAD declarations and defines
;-------------------------------------------------------------------------  
;  Values for AUDIO_LOAD.ulFlags          
LOAD_START	EQU	01H 
LOAD_END	EQU	02H 
LOAD_32BIT	EQU	10H 

load	STRUC
load_pbBuffer	DD	?
load_ulSize	DD	?
load_ulFlags	DD	?
load	ENDS
; -------------------------------------------------------------------------*
; Track info declarations and defines
;-------------------------------------------------------------------------  
;  Track Info structure declaration       

info	STRUC
info_usMasterVolume	DW	?
info_usDitherPct	DW	?             ;  Percent of a bit dither during record    
info_usMasterVolumeDelay	DW	?
info_usMasterBalance	DW	?
info_usMasterBalanceDelay	DW	?
info	ENDS
; -------------------------------------------------------------------------*
; IOBUFFER declarations and defines
;-------------------------------------------------------------------------  
STARTED	EQU	1 
PAUSED	EQU	2 

MCI_AUDIO_IOBUFFER	STRUC
iobuffer_lSize	DD	?
iobuffer_pHead	DD	?
iobuffer_pTail	DD	?
iobuffer_lCount	DD	?
iobuffer_ulPosition	DD	?
iobuffer_lDelay	DD	?
iobuffer_usRunFlags	DW	?
iobuffer_usSelInc	DW	?
iobuffer_pBuffer	DD	?
MCI_AUDIO_IOBUFFER	ENDS
; -------------------------------------------------------------------------*
; AUDIO_HPI declarations and defines
;-------------------------------------------------------------------------  
CBXMIT	EQU	1 
CBREC	EQU	2 
CRTIMER	EQU	4 
EP_OPEN	EQU	0 
EP_CLOSE	EQU	1 
EP_READ	EQU	2 
EP_WRITE	EQU	3 
EP_INIT	EQU	4 
EP_STATUS	EQU	5 
EP_CONTROL	EQU	6 
EP_BUFFER	EQU	7 
EP_LOAD	EQU	8 
EP_WAIT	EQU	9 

hpi	STRUC
hpi_pvEntry	DD	?
hpi_pvCallBack	DD	?
hpi_prXBuff	DD	?
hpi_prRBuff	DD	?
hpi_usFlags	DW	?
hpi	ENDS
;*************************  
;  AUDIO_UPDATE Structure   
;*************************  

audio_update	STRUC
update_iobuf_type	DB	?              ;  0 - XMITIO, 1 - RECIO to be updated        
update_buffer_address	DD	?     ;  address to buffer to be added to array     
update_buffer_length	DD	?          ;  length of buffer to be added               
update_rc	DW	?                    ;  return code                                
update_reserved	DD	?           ;  future use                                 
audio_update	ENDS
;  audio_update.iobuf_type definitions                                        
XMIT_IOBUF	EQU	0 
REC_IOBUF	EQU	1 
;  audio_update.rc definitions                                                
MAX_NUM_BUFFERS_REACHED	EQU	9 
UPDATE_GENERAL_FAILURE	EQU	10 
INVALID_BUFFER_LENGTH	EQU	11 
