;********************** START OF SPECIFICATIONS ****************************  
;                                                                             
;  SOURCE FILE NAME: HHPHEAP.H                                                
;                                                                             
;  DESCRIPTIVE NAME: Heap manager include file.                               
;                                                                             
;  COPYRIGHT:                                                                 
;                  Copyright (c) IBM Corporation 1991                         
;                          All Rights Reserved                                
;                                                                             
;  STATUS: OS/2 MM Release 1.00                                               
;                                                                             
;  ABSTRACT: Contains data structures and prototypes that are used in         
;            HHP API's.                                                       
;                                                                             
;*********************** END OF SPECIFICATIONS *****************************  
ifndef _HHPHEAP_H_
_HHPHEAP_H_	EQU	1

ifdef MMDEBUG
MM_HEAP_DEBUG_VAR	EQU	<"MM_HEAP">
else
endif
;  Shared Heap Support   
;  Low-Level Independent (Not Suballocated) Buffer Support   
HH_SHARED	EQU	1H     ;  Cause the Heap to be Shared   
endif ;  _HHPHEAP_H_   
