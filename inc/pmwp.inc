PMWP_INCLUDED	EQU	1

ifdef INCL_WINWORKPLACE
INCL_WPCLASS	EQU	1

endif
IFTEMP08 = 0
IFDEF INCL_WPCLASS
IFTEMP08 = 1
ENDIF
IFNDEF INCL_NOCOMMON
IFTEMP08 = 1
ENDIF
IF IFTEMP08
CCHMAXCLASS	EQU	3       
QC_First	EQU	0       
QC_Next	EQU	1 
QC_Last	EQU	2 
ifndef LOCATION_DESKTOP
LOCATION_DESKTOP	EQU	(     0FFFF0001H ) 
endif

OBJECTIMAGE	STRUC
oimg_hptrObject	DD	?
OBJECTIMAGE	ENDS

OBJCLASS	STRUC
ocls_pNext	DD	?
ocls_pszClassName	DD	?
ocls_pszModName	DD	?
OBJCLASS	ENDS
CO_FAILIFEXISTS	EQU	0 
CO_REPLACEIFEXISTS	EQU	1 
CO_UPDATEIFEXISTS	EQU	2 
endif  
INCL_WPERRORS	EQU	1

INCLUDE pmerr.INC

PAGEINFO	STRUC
pginf_cb	DD	?
pginf_hwndPage	DD	?
pginf_pfnwp	DD	?
pginf_resid	DD	?
pginf_pCreateParams	DD	?
pginf_dlgid	DW	?
pginf_usPageStyleFlags	DW	?
pginf_usPageInsertFlags	DW	?
pginf_usSettingsFlags	DW	?
pginf_pszName	DD	?
pginf_idDefaultHelpPanel	DW	?
pginf_usReserved2	DW	?
pginf_pszHelpLibraryName	DD	?
pginf_pHelpSubtable	DD	?
pginf_hmodHelpSubtable	DD	?
pginf_ulPageInsertId	DD	?
PAGEINFO	ENDS
SETTINGS_PAGE_NUMBERS	EQU	01H 
IFTEMP09 = 0
IFNDEF INCL_NOCOMMON
IFTEMP09 = 1
ENDIF
IF IFTEMP09

ICONPOS	STRUC
icp_ptlIcon	DB	SIZE POINTL DUP (?)
icp_szIdentity	DB	1 DUP (?)
ICONPOS	ENDS
ifdef INCL_WINPOINTERS
endif
endif
