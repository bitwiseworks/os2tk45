;       SCCSID = @(#)gas.inc    6.2 91/03/18
;       SCCSID = @(#)gas.inc    6.2 91/03/18
; ****************************************************************************
; *                                                                          *
; *                 This file is IBM unique.                                 *
; *                 IBM Confidential                                         *
; *                 Copyright (c) IBM Corporation  1981, 1990                *
; *                 All Rights Reserved                                      *
; *                                                                          *
; ****************************************************************************
;
; @PEN - added 32 bit log record declarations(LOGHEAD32) for CP20 DCR 1152
;
;
;   logging facility devhlp call equate
;
LOGERROR             EQU    0001H    ; Log data in 16 bit format(LOGHEAD)
LOGERROR32           EQU    0090H    ; Log data in 32 bit format(LOGHEAD32)

;
;   packet ids
;
STDERROR             EQU    0001H    ; Packet id - OS/2 Standard Error
SNAGENALERT          EQU    0002H    ; Packet id - SNA Generic Alert Subvector

;
;   Status Field Definitions
;
PROCLOGNOTIFY        EQU    00000001h ; Room for Proc Name, Log and Alert

;
;   logging facility structure for 16 bit log packets
;

LOGHEAD STRUC
        dw      1                    ; Number of log packets
        dw      ?                    ; Length of log packet
        dw      ?                    ; Packet id -  sna gen alert
        db      'OS/2    '           ; Originator name
        db      'GA  '               ; Ascii selector
        dd      0                    ; Reserved
        dd      PROCLOGNOTIFY        ; Status (Process Name, Log, Notify)
        dd      0                    ; Time field
        dd      0                    ; Date field
        db      260 dup(0)           ; Process Name Field
LOGHEAD ENDS

;
;   logging facility structure for 32 bit log packets(CP20 DCR 1152)
;

LOGHEAD32 STRUC
        dw      1                    ; Packet version number(1 is for 32 bit)
        dw      1                    ; Number of log packets
        dw      ?                    ; Length of log packet
        dw      ?                    ; Packet id -  sna gen alert
        dd      0                    ; Status (No Process Name)
        db      'GA  '               ; Qualifier name
        dd      0                    ; Reserved
        dd      0                    ; Time field
        dd      0                    ; Date field
        db      'OS/2    '           ; Originator name
        db      12 dup(0)            ; Formatting DLL name(none)
LOGHEAD32 ENDS

;
;   generic alert subvectors
;
;

;   vector/subvector keys
;
GAS_GEN_ALERT         EQU   92H      ; Generic Alert
GAS_PROB_CAUSE        EQU   93H      ; Probable Causes
GAS_USER_CAUSE        EQU   94H      ; User Caused
GAS_INST_CAUSE        EQU   95H      ; Install Caused
GAS_FAIL_CAUSE        EQU   96H      ; Failure Caused
;
;   alert types
;
GAS_PERM_LOSS         EQU   01H      ; Permanent loss of availability
GAS_TEMP_LOSS         EQU   02H      ; Temporary loss of avaibability
GAS_PERF_LOSS         EQU   03H      ; Performance < acceptable level
GAS_IMPENDING         EQU   11H      ; Impending problem
GAS_UNKNOWN           EQU   12H      ; Unknown severity
;
;   alert description codes
;
GAS_ADC_DEVOUTERROR   EQU   0012H    ; JT output device error
GAS_ADC_PRINTER       EQU   0112H    ; Output device error (printer)
GAS_ADC_DSKFAIL       EQU   1113H    ; I/O device error (disk failure)
GAS_ADC_DSKOPER       EQU   1213H    ; I/O device error (operation error)
GAS_ADC_DEVCOMERROR   EQU   0233H    ; JT unable to communicate with device
GAS_ADC_DEVNOTREADY   EQU   1090H    ; JT device not ready
GAS_ADC_PRTNOTREADY   EQU   1190H    ; operatore intervention required (printer)
GAS_ADC_PRTOUTOFPAPER EQU   0292H    ; stock exhausted (printer)
;
;   probable causes description codes
;
GAS_PCD_COMSTARTSTOP  EQU   0120H    ; JT Start/Stop Communications
GAS_PCD_DEVOUTERROR   EQU   0062H    ; JT Output Device
GAS_PCD_PRINTER       EQU   1062H    ; Output device (printer)
GAS_PCD_DASD          EQU   1063H    ; I/O device (dasd device)
;
;   user causes subfields
;
GAS_UC_USER           EQU   01H      ; User causes
GAS_UC_RECACT         EQU   81H      ; Recommended action
;
;   user cause codes
;
GAS_UCC_PRTPOWEROFF   EQU   0E02H    ; Printer powered off
GAS_UCC_OUTOFPAPER    EQU   0353H    ; Printer out of paper
GAS_UCC_PRTNOTREADY   EQU   1360H    ; Printer not ready
;
;   failure causes subfields
;
GAS_FC_FAILURE        EQU   01H      ; Failure causes
GAS_FC_RECACT         EQU   81H      ; Recommended action
;
;   failure cause codes
;
GAS_FCC_DASD          EQU   0150H    ; DASD media
GAS_FCC_DEVOUT        EQU   0062H    ; JT Output Device
GAS_FCC_PRINTER       EQU   1062H    ; printer
GAS_FCC_DISKDRV       EQU   1063H    ; Disk drive
;
;   recommended action codes
;
GAS_RAC_ONLINEPD      EQU   0100H    ; Run online problem determination
GAS_RAC_REF_GUIDE     EQU   0110H    ; Refer to operator's guide
GAS_RAC_READY_DEVICE  EQU   0113H    ; Ready the device then retry
GAS_RAC_ADD_PAPER     EQU   0616H    ; Add paper
GAS_RAC_REVIEW_LOG    EQU   0122H    ; Review remote device logs
GAS_RAC_CNTCT_SERV    EQU   0030H    ; Contact service representative



;
;   Description of Product Set ID subvector follows
;

PSID_KEY        EQU     10H             ; Product Set ID Key
SW_PID_KEY      EQU     11H             ; Software Product ID Key
SWCLASS_IBM     EQU     04H             ; IBM Software Classification
COMPID_KEY      EQU     02H             ; Component ID Subfield Key
SW_CN_KEY       EQU     06H             ; Software Common Name key

PSID_MAJOR      EQU     0F2H            ; '2' Major version
PSID_MINOR      EQU     0F1H            ; '1' Minor version
PSID_RELEASE    EQU     0F0H            ; '0' Version release

; --------------------------------------------------------------------
;
;   Product Set ID subvector
;
; --------------------------------------------------------------------

PSIDS           STRUC
PSIDS_LEN       DB      52              ; Product Set ID length
PSIDS_KEY       DB      PSID_KEY        ; Product Set ID key
PSIDS_RETIRED   DB      0               ; Retired field
PSIDS_SW_LEN    DB      49              ; Software Subvector length
PSIDS_SW_KEY    DB      SW_PID_KEY      ; Software Subvector key
PSIDS_CLASS     DB      SWCLASS_IBM     ; Software class
PSIDS_COMP_LEN  DB      14              ; Component ID Subfield length
PSIDS_COMP_KEY  DB      COMPID_KEY      ; Component ID Subfield key
PSIDS_COMPID    DB      0F5H            ; '5'   Component ID
                DB      0F6H            ; '6'
                DB      0F2H            ; '2'
                DB      0F1H            ; '1'
                DB      0F0H            ; '0'
                DB      0F7H            ; '7'
                DB      0F7H            ; '7'
                DB      0F0H            ; '0'
                DB      0F1H            ; '1'
PSIDS_COMPREL   DB      PSID_MAJOR      ; '2'   Release level, major version
                DB      PSID_MINOR      ; '1'   minor version
                DB      PSID_RELEASE    ; '0'   version release
PSIDS_CN_LEN    DB      32              ; Software Common Name length
PSIDS_SWCN_KEY  DB      SW_CN_KEY       ; Software Common Name key
PSIDS_SW_CN     DB      0D6H            ; 'O'
                DB      0E2H            ; 'S'
                DB      061H            ; '/'
                DB      0F2H            ; '2'
                DB      040H            ; ' '
                DB      0C2H            ; 'B'
                DB      0C1H            ; 'A'
                DB      0E2H            ; 'S'
                DB      0C5H            ; 'E'
                DB      040H            ; ' '
                DB      0D6H            ; 'O'
                DB      0D7H            ; 'P'
                DB      0C5H            ; 'E'
                DB      0D9H            ; 'R'
                DB      0C1H            ; 'A'
                DB      0E3H            ; 'T'
                DB      0C9H            ; 'I'
                DB      0D5H            ; 'N'
                DB      0C7H            ; 'G'
                DB      040H            ; ' '
                DB      0E2H            ; 'S'
                DB      0E8H            ; 'Y'
                DB      0E2H            ; 'S'
                DB      0E3H            ; 'T'
                DB      0C5H            ; 'E'
                DB      0D4H            ; 'M'
                DB      040H            ; ' '
                DB      040H            ; ' '
                DB      040H            ; ' '
                DB      040H            ; ' '
PSIDS   ENDS

; --------------------------------------------------------------------
;
;   generic alert subvector
;
; --------------------------------------------------------------------
GAS         STRUC
GAS_LEN     DB      0BH                 ; length of G.A. Subvector
GAS_KEY     DB      GAS_GEN_ALERT       ; Generic Alert
GAS_FLAGS   DW      0                   ; Flags = !initiated, !held, !delayed
GAS_TYPE    DB      ?                   ; Alert type
GAS_DESC    DW      ?                   ; Alert description
GAS_ALERTID DD      ?                   ; Alert ID number
GAS         ENDS
; --------------------------------------------------------------------
;
;   probable cause subvector
;
; --------------------------------------------------------------------
GAS_PCS         STRUC
GAS_PCS_LEN     DB      4               ; length of P.C. Subvector
GAS_PCS_KEY     DB      GAS_PROB_CAUSE  ; Probable causes
GAS_PCS_DESC    DW      ?               ; Description code
GAS_PCS         ENDS
; --------------------------------------------------------------------
;
;   user cause subvector
;
; --------------------------------------------------------------------
GAS_UCS         STRUC
GAS_UCS_LEN     DB      0AH             ; length of U.C. Subvector
GAS_UCS_KEY     DB      GAS_USER_CAUSE  ; User causes
GAS_UCS_SFLEN1  DB      4               ; length of U.C. Subvector
GAS_UCS_SFKEY1  DB      GAS_UC_USER     ; User causes
GAS_UCS_CAUSECD DW      ?               ; Cause code
GAS_UCS_SFLEN2  DB      4               ; length of R.A. Subvector
GAS_UCS_SFKEY2  DB      GAS_UC_RECACT   ; Recommended actions
GAS_UCS_RECACT  DW      ?               ; Recommended action code
GAS_UCS         ENDS
; --------------------------------------------------------------------
;
;   failure causes subvector
;
; --------------------------------------------------------------------
GAS_FCS         STRUC
GAS_FCS_LEN     DB      0AH             ; length of F.C. Subvector
GAS_FCS_KEY     DB      GAS_FAIL_CAUSE  ; Failure causes
GAS_FCS_SFLEN1  DB      4               ; length of F.C. Subvector
GAS_FCS_SFKEY1  DB      GAS_FC_FAILURE  ; Failure causes
GAS_FCS_CAUSECD DW      ?               ; Cause code
GAS_FCS_SFLEN2  DB      4               ; length of R.A. Subvector
GAS_FCS_SFKEY2  DB      GAS_FC_RECACT   ; Recommended actions
GAS_FCS_RECACT  DW      ?               ; Recommended action code
GAS_FCS         ENDS
