;***	vdmprop.inc - Common VDM properties definitions
;
;
;	IBM/Microsoft Confidential
;
;	Copyright (c) IBM Corporation 1987, 1989
;	Copyright (c) Microsoft Corporation 1987, 1989
;
;	All Rights Reserved
;
;	Title:
;	    VDM Property definitions common to VDDs and applications
;
;	Modification History:
;	    08-Nov-1989 bens	Initial version.
;
;    -	001	2/22/91	  Mark Rogalski,   IBM
;	CP20 DCR 1329  Add DOS=UMB  and DEVICEHIGH support
;
;    -	002	3/22/91	  Mark Rogalski,   IBM
;	CP20 DCR 1360  Add DOS=HIGH support
;
;***********************************************************************;BN011
; 19Jun91 - G.L.Fulk							;BN011
; Add support for a DOS ONLY "FILES=" statement in CONFIG.SYS.		;BN011
;***********************************************************************;BN011
;* 6.10 09/23/91 CP20 B726841  Jeff Muir   Removed IOPL property
;***********************************************************************;53233
; 24Jul92 - G.L.Fulk							;53233
; Add DOS_AUTOEXEC and DOS_INSTALL properties.				;53233
;***********************************************************************;53233
;


;***	Property Limits
;
;

MAX_PROPERTY_NAME_LENGTH	EQU	40  ;including NULL terminator
MAX_AUTOEXEC_SIZE	EQU	128  ;max string size		   ;53233


;***	VPTYPE: VDM Property Types
;
;
VDMP_BOOL	EQU	0   ;boolean
VDMP_INT	EQU	1   ;integer - ULONG size, but only USHORT is valid
VDMP_ENUM	EQU	2   ;enumeration
VDMP_STRING	EQU	3   ;asciiz string
VDMP_MLSTR	EQU	4   ;multi-line string, separated by linefeed (00ah)

DefType VPTYPE,USHORT			;property type



;***	VPORD: VDM Property Ordinals
;
;
VDMP_ORD_OTHER	EQU	0   ;custom VDD property
VDMP_ORD_KERNEL		EQU	1   ;asciiz path of DOS kernel
VDMP_ORD_SHELL	EQU	2   ;asciiz path of DOS shell
VDMP_ORD_RMSIZE		EQU	3   ;integer size of DOS box (128K..640K)
VDMP_ORD_FCB	EQU	4   ;integer total FCBs
VDMP_ORD_FCB2	EQU	5   ;integer FCBs immune to close LRUing
VDMP_ORD_BREAK	EQU	6   ;boolean BREAK flag
VDMP_ORD_DOSDD	EQU	7   ;mlstr DOS device drivers
VDMP_ORD_VMBOOT		EQU	8   ;mlstr virtual machine boot drive(s)
;VDMP_ORD_IOPL3		EQU	9   ; boolean IOPL3 flag	      // 6.10
VDMP_ORD_VERSION	EQU	10  ;mlstr fake version entries
VDMP_ORD_DOS_UMB	EQU	11  ;boolean flag - DOS owns UBMs	;BN001
VDMP_ORD_DOS_HIGH	EQU	12  ;boolean flag - DOS loaded high	;BN002
VDMP_ORD_LASTDRIVE	EQU	13  ;asciiz lastdrive			;BN003
VDMP_ORD_FILES	EQU	14  ;integer total FILES		;BN011
VDMP_ORD_AUTOEXEC	EQU	15  ;asciiz Autoexec filespec		  ;53233
VDMP_ORD_INSTALL	EQU	16  ;mlstr INSTALL= settings		  ;53233


DefType VPORD,USHORT			;property ordinal



;***	VDHRegisterProperty Flags
;
;
VDMP_CREATE	EQU	000000001h  ;create-time only property
VDMP_ADVANCED	EQU	000000002h  ;advanced property (not shown by default)



;***	VDHRegisterProperty PFNVDHRP ordinals
;
;
VDHPROP_VALIDATE	EQU	000000000h ;validate parameter
VDHPROP_SET	EQU	000000001h ;set parameter



;***	VPBOUND - limits for VDMP_INT properties
;
;   Notes:  (1) max > min must hold
;	    (2) (max-min) must be a multiple of step
;	    (3) step = 1 implies that all values between min and max are valid
;
DefStruc ,VPBOUND_s			;vpb
    USHORT  min	   ;minimum allowed value
    USHORT  max	   ;maximum allowed value
    USHORT  step   ;increment between values
EndStruc VPBOUND
DefType PVPBOUND,dd			;pvpb



;***	VPINFO - VDD Property Info structure
;
;	This is filled in by a call to Dos32QueryDOSProperty
;
;
;	VPTYPE	vi_vptype  - Property type (see VPTYPE).
;	VPORD	vi_vpord   - Property ordinal (see VPORD).
;	ULONG	vi_ulFlags - Property flags:
;		    VDMP_CREATE
;		      TRUE if the property can only be specified at
;			  VDM creation.  Changing the property after
;			  the VDM is created has no effect.
;		      FALSE if the property can be specified at VDM
;			  creation AND can be changed for a running
;			  VDM.
;	ULONG	vi_ulHelpID - ID of help topic
;	char	vi_pszHelp[] - asciiz help file name.
;		    A single NULL byte is present if no help is
;		    available.
;
;	???	vi_pvValid - This contains information that allows the property
;		    value to be validated.  Its format depends upon
;		    the property type:
;		      VDMP_BOOL
;			This field is not present.
;		      VDMP_INT
;			This field is a VPBOUND structure.
;		      VDMP_ENUM
;			This field is a set of ASCIIZ strings,
;			terminated by a zero byte, which is the allowed
;			set of responses.
;		      VDMP_STRING
;		      VDMP_MLSTR
;			This field is a ULONG representing the maximum
;			allowed string length, including the terminating
;			NULL byte.
;
;	???	vi_pvValue - The default value of the property.  In all cases
;		    it is assumed that this value is valid according
;		    to the constraints specified by pvValid.
;		    The format depends upon the property type:
;		      VDMP_BOOL
;			This field is a 4-byte BOOL.
;		      VDMP_INT
;			This field is a ULONG.  The high half of the
;			value is always zero, so this type can only
;			take on the values of a USHORT.
;		      VDMP_ENUM
;			This field is an ASCIIZ string.
;		      VDMP_STRING
;		      VDMP_MLSTR
;			This field is an ASCIIZ string.
;

DefStruc ,VPINFO_s			;vpinfo
    VPTYPE  vi_vptype	  ;property type
    VPORD   vi_vpord	  ;property ordinal
    ULONG   vi_ulFlags	  ;flags
    ULONG   vi_ulHelpID	  ;help topic id
    ;	  vi_pszHelp,,	; help file
    ;	  vi_pvValid	; validation information
    ;	  vi_pvValue	; default value
EndStruc VPINFO
DefType PVPINFO,dd



;***	PROPERTYBUFFER - buffer containing 0 or more property values
;
;	This buffer format is used when passing property values to
;	DosStartSession.
;
;	Byte
;	Offset	Type	Contents
;	------	------- ------------------------------------------
;	   0	ULONG	Buffer length, including this ULONG.
;	   4	VPTYPE	Type of property (see VPTYPE in vdmprop.inc)
;	   6	ASCIIZ	Null-terminated property name
;	   ?	?	Property value.  Format depends on VPTYPE:
;			  VDMP_BOOL
;			    4-byte boolean value.  0 => false, !0 => true.
;			  VDMP_INT
;			    ULONG.  The high half of the value is always
;			      zero, so this type can only take on the
;			      values of a USHORT.
;			  VDMP_ENUM
;			  VDMP_STRING
;			  VDMP_MLSTR
;			    Null-terminated string.
;	   ?	VPTYPE	Type of second property
;	   ?+2	ASCIIZ	Name of second property
;	   ?	?	Value of second property
;	   ...
;

DefStruc ,_PROPERTYBUFFER			;propbuf
    ULONG   cb
    CHAR    achPropBuf
EndStruc PROPERTYBUFFER
DefType PPROPERTYBUFFER,dd			;ppropbuf

