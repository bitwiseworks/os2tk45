ifndef __OS2_H__
OS2_INCLUDED	EQU	1

INCLUDE os2def.INC
ifndef INCL_NOBASEAPI
INCLUDE bse.INC
endif 
ifndef INCL_NOPMAPI
INCLUDE pm.INC
endif 
endif 
