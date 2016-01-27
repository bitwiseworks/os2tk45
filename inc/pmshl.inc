SHL_INCLUDED	EQU	1

ifdef INCL_ERRORS
INCL_SHLERRORS	EQU	1

endif 
ifndef SESMGR
MAXNAMEL	EQU	60 
HINI_PROFILE	EQU	       NULL
HINI_USERPROFILE	EQU	       -1 
HINI_SYSTEMPROFILE	EQU	       -2 
HINI_USER	EQU	HINI_USERPROFILE
HINI_SYSTEM	EQU	HINI_SYSTEMPROFILE

PRFPROFILE	STRUC
prfpro_cchUserName	DD	?
prfpro_pszUserName	DD	?
prfpro_cchSysName	DD	?
prfpro_pszSysName	DD	?
PRFPROFILE	ENDS
endif  
ifdef INCL_WINPROGRAMLIST
ifndef SESMGR
MAXPATHL	EQU	128 
SGH_ROOT	EQU	          -1 

HPROGARRAY	STRUC
hpga_ahprog	DD	1 DUP (?)
HPROGARRAY	ENDS
endif  
PROG_DEFAULT	EQU	              0 
PROG_FULLSCREEN	EQU	              1 
PROG_WINDOWABLEVIO	EQU	              2 
PROG_PM	EQU	              3 
PROG_GROUP	EQU	              5 
PROG_REAL	EQU	              4 
PROG_VDM	EQU	              4 
PROG_WINDOWEDVDM	EQU	              7 
PROG_DLL	EQU	              6 
PROG_PDD	EQU	              8 
PROG_VDD	EQU	              9 
PROG_WINDOW_REAL	EQU	              10 
PROG_WINDOW_PROT	EQU	              11 
PROG_30_STD	EQU	              11 
PROG_WINDOW_AUTO	EQU	              12 
PROG_SEAMLESSVDM	EQU	              13 
PROG_30_STDSEAMLESSVDM	EQU	              13 
PROG_SEAMLESSCOMMON	EQU	              14 
PROG_30_STDSEAMLESSCOMMON	EQU	              14 
PROG_31_STDSEAMLESSVDM	EQU	              15 
PROG_31_STDSEAMLESSCOMMON	EQU	              16 
PROG_31_ENHSEAMLESSVDM	EQU	              17 
PROG_31_ENHSEAMLESSCOMMON	EQU	              18 
PROG_31_ENH	EQU	              19 
PROG_31_STD	EQU	              20 
PROG_DOS_GAME	EQU	              21 
PROG_WIN_GAME	EQU	              22 
PROG_DOS_MODE	EQU	              23 
PROG_RESERVED	EQU	              255 
ifndef SESMGR

PROGTYPE	STRUC
progt_progc	DD	?
progt_fbVisible	DD	?
PROGTYPE	ENDS
SHE_VISIBLE	EQU	00H 
SHE_INVISIBLE	EQU	01H 
SHE_RESERVED	EQU	0FFH 
SHE_UNPROTECTED	EQU	00H 
SHE_PROTECTED	EQU	02H 

PROGDETAILS	STRUC
progde_Length	DD	?
progde_progt	DB	SIZE PROGTYPE DUP (?)
progde_pszTitle	DD	?
progde_pszExecutable	DD	?
progde_pszParameters	DD	?
progde_pszStartupDir	DD	?
progde_pszIcon	DD	?
progde_pszEnvironment	DD	?
progde_swpInitial	DB	SIZE SWP DUP (?)
PROGDETAILS	ENDS

PROGTITLE	STRUC
progti_hprog	DD	?
progti_progt	DB	SIZE PROGTYPE DUP (?)
progti_pszTitle	DD	?
PROGTITLE	ENDS
SAF_VALIDFLAGS	EQU	001FH 
SAF_INSTALLEDCMDLINE	EQU	0001H      
SAF_STARTCHILDAPP	EQU	0002H      
SAF_MAXIMIZED	EQU	0004H      
SAF_MINIMIZED	EQU	0008H      
SAF_BACKGROUND	EQU	0010H      
endif 
endif 
ifndef SESMGR
IFTEMP00 = 0
IFDEF INCL_WINSWITCHLIST
IFTEMP00 = 1
ENDIF
IFNDEF INCL_NOCOMMON
IFTEMP00 = 1
ENDIF
IF IFTEMP00

SWCNTRL	STRUC
swctl_hwnd	DD	?
swctl_hwndIcon	DD	?
swctl_hprog	DD	?
swctl_idProcess	DD	?
swctl_idSession	DD	?
swctl_uchVisibility	DD	?
swctl_fbJump	DD	?
swctl_szSwtitle	DB	MAXNAMEL+4 DUP (?)
swctl_bProgType	DD	?
SWCNTRL	ENDS
SWL_VISIBLE	EQU	04H 
SWL_INVISIBLE	EQU	01H 
SWL_GRAYED	EQU	02H 
SWL_JUMPABLE	EQU	02H 
SWL_NOTJUMPABLE	EQU	01H 
endif  
ifdef INCL_WINSWITCHLIST

SWENTRY	STRUC
swent_hswitch	DD	?
swent_swctl	DB	SIZE SWCNTRL DUP (?)
SWENTRY	ENDS

SWBLOCK	STRUC
swblk_cswentry	DD	?
swblk_aswentry	DB	SIZE SWENTRY * 1 DUP (?)
SWBLOCK	ENDS
endif 
ifdef INCL_WINSHELLDATA
PL_ALTERED	EQU	008EH   
endif 
ifdef INCL_SHLERRORS
INCLUDE pmerr.INC
endif 
endif 
