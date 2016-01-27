ifndef __BSE__
INCL_BASEINCLUDED	EQU	1

ifdef INCL_BASE
INCL_DOS	EQU	1

INCL_SUB	EQU	1

INCL_DOSERRORS	EQU	1

endif 
INCLUDE bsedos.INC
ifdef INCL_DOSDEVIOCTL
INCLUDE bsedev.INC
endif 
INCLUDE bsesub.INC
INCLUDE bseerr.INC
ifdef INCL_ORDINALS
INCLUDE bseord.INC
endif 
endif 
