;       SCCSID = @(#)bsetib.h   6.9 92/03/15   
;****************************************************************************
; Module Name: BSETIB.H
;
; OS/2 Thread Information Block Include File
;
; Copyright (c) 1989, 1992  IBM Corporation
; Copyright (c) 1989  Microsoft Corporation
;
;***************************************************************************  
ifndef __BSETIB__
; 
;*      Thread Information Block (TIB)
;  

tib2_s	STRUC
tib2_ultid	DD	?             ;  Thread I.D.   
tib2_ulpri	DD	?             ;  Thread priority   
tib2_version	DD	?           ;  Version number for this structure   
tib2_usMCCount	DW	?         ;  Must Complete count   
tib2_fMCForceFlag	DW	?      ;  Must Complete force flag   
tib2_s	ENDS


tib_s	STRUC
tib_pexchain	DD	?           ;  Head of exception handler chain   
tib_pstack	DD	?             ;  Pointer to base of stack   
tib_pstacklimit	DD	?        ;  Pointer to end of stack   
tib_ptib2	DD	?              ;  Pointer to system specific TIB   
tib_version	DD	?            ;  Version number for this TIB structure   
tib_ordinal	DD	?            ;  Thread ordinal number          
tib_s	ENDS

; 
;*      Process Information Block (PIB)
;  

pib_s	STRUC
pib_ulpid	DD	?              ;  Process I.D.   
pib_ulppid	DD	?             ;  Parent process I.D.   
pib_hmte	DD	?               ;  Program (.EXE) module handle   
pib_pchcmd	DD	?             ;  Command line pointer   
pib_pchenv	DD	?             ;  Environment pointer   
pib_flstatus	DD	?           ;  Process' status bits   
pib_ultype	DD	?             ;  Process' type code   
pib_s	ENDS

endif ;  __BSETIB__   
