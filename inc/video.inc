;**************************START OF SPECIFICATIONS *************************  
;                                                                             
;  Module NAME:  VIDEO.H                                                      
;                                                                             
;  OS/2 2.0 Multimedia Extensions Video structures and definitions            
;                                                                             
;  Copyright (c) International Business Machines Corporation 1993             
;                          All Rights Reserved                                
;************************** END OF SPECIFICATIONS **************************  

VIDEO_FRAME_HDR	STRUC
vfh_FrameNumber	DD	?      ;  relative frame number         
vfh_FramesSkipped	DD	?    ;  no. frames skipped between this and last frames   
vfh_StreamTime	DD	?       ;  stream time in milliseconds   
vfh_FrameSize	DD	?        ;  size in bytes                 
vfh_SHparm1	DD	?          ;  used by stream handler        
vfh_reserved3	DD	?        ;  unused                        
vfh_reserved2	DD	?        ;  unused                        
vfh_reserved1	DD	?        ;  unused                        
VIDEO_FRAME_HDR	ENDS
