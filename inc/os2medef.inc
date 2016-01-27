;***************************************************************************  
;                                                                             
;  Module Name: OS2MEDEF.H                                                    
;                                                                             
;  OS/2 2.0 Multimedia Extensions Common Definitions file                     
;                                                                             
;  Copyright (c) International Business Machines Corporation 1991, 1992       
;                         All Rights Reserved                                 
;                                                                             
;***************************************************************************  
ifdef XXXXX
endif
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
_SPCBKEY_DEFINED	EQU	1 
;**********************************************
;*
;* MMTRACKINFO - This structure is used to
;*            represent a video, audio or
;*            some other type of track
;*            within a digital video movie
;*            file.
;*
;*********************************************  

MMTRACKINFO	STRUC
mmtrackinfo_ulTrackID	DD	?         ;  track identifier   
mmtrackinfo_ulMediaType	DD	?       ;  media type   
mmtrackinfo_ulCountry	DD	?         ;  country code for the track   
mmtrackinfo_ulCodePage	DD	?        ;  country code page for the track   
mmtrackinfo_ulReserved1	DD	?       ;  reserved must be 0   
mmtrackinfo_ulReserved2	DD	?       ;  reserved must be 0   
MMTRACKINFO	ENDS
;************************
;* Clipboard formats
;***********************  
CF_RMID	EQU	10 
CF_RIFF	EQU	11 
CF_WAVE	EQU	12 
CF_AVI	EQU	13 
;************************
;* Resource formats
;***********************  
RT_RMID	EQU	100 
RT_RIFF	EQU	101 
RT_WAVE	EQU	102 
RT_AVI	EQU	103 
RT_AVS	EQU	104 
;************************
;* Drag formats
;***********************  
DRT_WAVE	EQU	<"Digital audio">
DRT_AVI	EQU	<"Digital video">
DRT_MIDI	EQU	<"MIDI">
;************************
;* Sampling Rate
;***********************  
HZ_8000	EQU	8000             ;  8.0 kHz  
HZ_11025	EQU	11025            ;  11.025 kHz  
HZ_14700	EQU	14700            ;  14.700 kHz (SPV/2)  
HZ_18900	EQU	18900            ;  18.900 kHz (CD/XA LVL C)  
HZ_22050	EQU	22050            ;  22.050 kHz  
HZ_37800	EQU	37800            ;  37.800 kHz (CD/XA LVL B)  
HZ_44100	EQU	44100            ;  44.100 kHz  
;************************
;* Bits per Sample
;***********************  
BPS_4	EQU	4                ;  4 bits/sample (ADPCM)  
BPS_8	EQU	8                ;  8 bits/sample (PCM)  
BPS_16	EQU	16               ;  16 bits/sample(PCM)  
;************************
;* Channels
;***********************  
CH_1	EQU	1                ;  Mono  
CH_2	EQU	2                ;  Stereo  
CH_4	EQU	4                ;  Quad  
;*************************************************************************
;* Multimedia Datatypes (spcbkey values)
;************************************************************************  
;****************
;* NULL datatype
;***************  
DATATYPE_NULL	EQU	0000H      ;  NULL stream data type (NULL)  
SUBTYPE_NONE	EQU	0000H    ;  No subtype for this data type  
;*********************
;* WAVEFORM datatypes
;********************  
DATATYPE_WAVEFORM	EQU	0001H      ;  WAVEFORM audio (PCM)  
;  Subtypes for DATATYPE_WAVEFORM:  
WAVE_FORMAT_1M08	EQU	0001H    ;  11.025kHz, Mono  , 8-bit  
WAVE_FORMAT_1S08	EQU	0002H    ;  11.025kHz, Stereo, 8-bit  
WAVE_FORMAT_1M16	EQU	0003H    ;  11.025kHz, Mono  , 16-bit  
WAVE_FORMAT_1S16	EQU	0004H    ;  11.025kHz, Stereo, 16-bit  
WAVE_FORMAT_2M08	EQU	0005H    ;  22.05kHz , Mono  , 8-bit  
WAVE_FORMAT_2S08	EQU	0006H    ;  22.05kHz , Stereo, 8-bit  
WAVE_FORMAT_2M16	EQU	0007H    ;  22.05kHz , Mono  , 16-bit  
WAVE_FORMAT_2S16	EQU	0008H    ;  22.05kHz , Stereo, 16-bit  
WAVE_FORMAT_4M08	EQU	0009H    ;  44.1kHz  , Mono  , 8-bit  
WAVE_FORMAT_4S08	EQU	000aH    ;  44.1kHz  , Stereo, 8-bit  
WAVE_FORMAT_4M16	EQU	000bH    ;  44.1kHz  , Mono  , 16-bit  
WAVE_FORMAT_4S16	EQU	000cH    ;  44.1kHz  , Stereo, 16-bit  
WAVE_FORMAT_8M08	EQU	000dH    ;   8.0kHz  , Mono  , 8-bit  
WAVE_FORMAT_8S08	EQU	000eH    ;   8.0kHz  , Stereo, 8-bit  
WAVE_FORMAT_8M16	EQU	000fH    ;   8.0kHz  , Mono  , 16-bit  
WAVE_FORMAT_8S16	EQU	0010H    ;   8.0kHz  , Stereo, 16-bit  
;*********************
;* DIGVIDEO datatypes
;********************  
DATATYPE_DIGVIDEO	EQU	0002H      ;  Digital Video   
;  Subtypes for DATATYPE_DIGVIDEO:  
DIGVIDEO_PHOTOM	EQU	544F4850H    ;  Photmotion video, FOURCC value for phio   
DIGVIDEO_ULTIM	EQU	49544c55H    ;  Ultimotion video, FOURCC value for ulio   
DIGVIDEO_AVI	EQU	20495641H    ;  AVI video, FOURCC value for avio   
DIGVIDEO_MONITOR	EQU	206E6F6DH    ;  MONITOR video, FOURCC value for monitoring   
DIGVIDEO_FLC	EQU	63696C66H    ;  FLIC video, FOURCC value for flic   
DIGVIDEO_MPEG	EQU	4745504DH    ;  MPEG video, FOURCC value for MPEG   
;******************
;* MuLaw datatypes
;*****************  
DATATYPE_MULAW	EQU	0101H      ;  MuLaw  
DATATYPE_RIFF_MULAW	EQU	0007H     ;  MuLaw  
;  Subtypes for DATATYPE_MULAW:  
MULAW_8B8KS	EQU	0001H    ;  8bit 8kHz stereo  
MULAW_8B11KS	EQU	0002H    ;  8bit 11kHz stereo  
MULAW_8B22KS	EQU	0003H    ;  8bit 22kHz stereo  
MULAW_8B44KS	EQU	0004H    ;  8bit 44kHz stereo  
MULAW_8B8KM	EQU	0005H    ;  8bit 8kHz mono  
MULAW_8B11KM	EQU	0006H    ;  8bit 11kHz mono  
MULAW_8B22KM	EQU	0007H    ;  8bit 22kHz mono  
MULAW_8B44KM	EQU	0008H    ;  8bit 44kHz mono  
;*****************
;* ALaw datatypes
;****************  
DATATYPE_ALAW	EQU	0102H      ;  ALaw  
DATATYPE_RIFF_ALAW	EQU	0006H      ;  ALaw  
;  Subtypes for DATATYPE_ALAW:  
ALAW_8B8KS	EQU	0001H    ;  8bit 8kHz stereo  
ALAW_8B11KS	EQU	0002H    ;  8bit 11kHz stereo  
ALAW_8B22KS	EQU	0003H    ;  8bit 22kHz stereo  
ALAW_8B44KS	EQU	0004H    ;  8bit 44kHz stereo  
ALAW_8B8KM	EQU	0005H    ;  8bit 8kHz mono  
ALAW_8B11KM	EQU	0006H    ;  8bit 11kHz mono  
ALAW_8B22KM	EQU	0007H    ;  8bit 22kHz mono  
ALAW_8B44KM	EQU	0008H    ;  8bit 44kHz mono  
;*******************************
;* AVC ADPCM WAVEFORM datatypes
;******************************  
DATATYPE_ADPCM_AVC	EQU	0103H      ;  ADPCM audio  
;  Subtypes for DATATYPE_ADPCM_AVC:  
ADPCM_AVC_VOICE	EQU	0001H 
ADPCM_AVC_MUSIC	EQU	0002H 
ADPCM_AVC_STEREO	EQU	0003H 
ADPCM_AVC_HQ	EQU	0004H 
;******************
;* CT_ADPCM datatypes
;*****************  
DATATYPE_CT_ADPCM	EQU	0200H      ;  Creative technologies   
;  Subtypes for DATATYPE_CT_ADPCM:  
CT_ADPCM_16B8KS	EQU	0001H    ;  16bit 8kHz stereo  
CT_ADPCM_16B11KS	EQU	0002H    ;  16bit 11kHz stereo  
CT_ADPCM_16B22KS	EQU	0003H    ;  16bit 22kHz stereo  
CT_ADPCM_16B44KS	EQU	0004H    ;  16bit 44kHz stereo  
CT_ADPCM_16B8KM	EQU	0005H    ;  16bit 8kHz mono  
CT_ADPCM_16B11KM	EQU	0006H    ;  16bit 11kHz mono  
CT_ADPCM_16B22KM	EQU	0007H    ;  16bit 22kHz mono  
CT_ADPCM_16B44KM	EQU	0008H    ;  16bit 44kHz mono  
;****************
;* MIDI datatype
;***************  
DATATYPE_MIDI	EQU	0201H      ;  MIDI  
;          SUBTYPE_NONE            0x0000L      No subtype for this data type  
;********************
;* GENERIC datatypes
;*******************  
DATATYPE_GENERIC	EQU	0202H      ;  Generic for files / memory. This  
                                             ;  datatype will match any other  
                                             ;  datatype at stream creation time.  
;**************************
;* Speech Viewer datatypes
;*************************  
DATATYPE_SPV2	EQU	0203H      ;  Speech Viewer  
;  Subtypes for DATATYPE_SPV2:  
SPV2_BPCM	EQU	0001H 
SPV2_PCM	EQU	0002H 
SPV2_NONE	EQU	0003H 
;******************
;* CD-XA datatypes
;*****************  
DATATYPE_CDXA_VIDEO	EQU	0204H      ;  CD-XA Video datatype  
DATATYPE_CDXA_DATA	EQU	0205H      ;  CD-XA sub-band data datatype  
DATATYPE_CDXA_AUDIO	EQU	0206H      ;  CD-XA ADPCM Audio datatype  
DATATYPE_CDXA_AUDIO_HD	EQU	0207H      ;  CD-XA ADPCM Audio with Headers  
DATATYPE_CDXA_HEADERS	EQU	0208H      ;  CD-XA Headers and Subheaders  
;  Subtypes for DATATYPE_CDXA_AUDIO:  
CDXA_LEVELB	EQU	0000H  ;  LEVEL B Audio Stereo  
CDXA_LEVELC	EQU	0001H  ;  LEVEL C Audio Stereo  
CDXA_LEVELB_MONO	EQU	0002H  ;  LEVEL B Audio Mono  
CDXA_LEVELC_MONO	EQU	0003H  ;  LEVEL C Audio Mono  
;*********************
;* DIGISPEECH datatype
;********************  
DATATYPE_DIGISPEECH	EQU	0208H      ;  DIGISPEECH   
DIGISPEECH_NATIVE	EQU	0001H    ;  Native digispeech   
;*********************
;* MPEG-1 Audio datatypes
;********************  
DATATYPE_MPEG1AUDIO	EQU	0050H      ;  MPEG-1 Audio   
;  Subtypes for DATATYPE_MPEG1AUDIO:  
MPEG1_FORMAT_3M16	EQU	0001H    ;  32.000kHz,  Mono  , 16-bit  
MPEG1_FORMAT_3S16	EQU	0002H    ;  32.000kHz,  Stereo, 16-bit  
MPEG1_FORMAT_4M16	EQU	0003H    ;  44.1/48kHz, Mono  , 16-bit  
MPEG1_FORMAT_4S16	EQU	0004H    ;  44.1/48kHz, Stereo, 16-bit  
;*******************
;* UNKNOWN datatype
;******************  
DATATYPE_UNKNOWN	EQU	0FFFFFFFFH    ;  Unknown datatype   
;          SUBTYPE_NONE            0x0000L      No subtype for this data type  
