;       SCCSID = @(#)devsym.inc 6.1 90/11/16
;      SCCSID = @(#)devsym.inc  13.12 90/06/14

;
;   IBM Confidential
;
;   OCO Source Materials
;
;
;   (c) Copyright IBM Corp. 1987, 1998
;
;   The source code for this program is not published or otherwise divested of its
;   tradesecrets, irrespective of what has been deposited with the U.S. Copyright Office.
;

page ,132

include devhdr.inc
include devcmd.inc

;**     Static Request Header
;
;       This structure defines the fixed first part of a device request
;       record.  Depending upon the request there may be other fields
;       which follow this header.
;
; Zibo notes that we should STOP using this weird stuff and use one set of
; structures and equates rather than lots of casting.  After this section is
; the new stuff we should use.
;

SrHead  STRUC
ReqLen  DB      ?               ; Length in bytes of request block
ReqUnit DB      ?               ; Device unit number
ReqFunc DB      ?               ; Type of request
ReqStat DW      ?               ; Status Word
        DD      ?               ; RESERVED
DevLink DD      ?               ; link field for use by driver to chain
                                ;   requests.  (A service in DevHelp)
SrHead  ENDS


;*      Other fields beyond the standard 13 byte header


; Status word masks
STERR   EQU     8000H           ; Bit 15 - Error
STINTER EQU     0400H           ; Bit 10 - Interim character
STBUI   EQU     0200H           ; Bit  9 - Busy
STDON   EQU     0100H           ; Bit  8 - Done
STECODE EQU     00FFH           ; Error code
WRECODE EQU     0

;**     Device Driver Request Packet Defintions
;


PktMax          equ     18
PktHeadSize     equ     13

Packet          struc
PktLen          db      ?               ; length in bytes of packet
PktUnit         db      ?               ; subunit number of block device
PktCmd          db      ?               ; command code
PktStatus       dw      ?               ; status word
PktFlag         db      ?               ; disk driver internal flags
                db      3 dup(?)        ; reserved
PktDevLink      dd      ?               ; device multiple-request link
PktData         db      PktMax dup (?)  ; data pertaining to specific packet
Packet          ends

PktDOSLink      equ     (dword ptr PktFlag)

;**     Device Driver Request Packet Equates
;


fPktInt13RP     equ     01H             ; Int 13 Request Packet
fPktCallOutDone equ     02H             ; Int 13 Callout completed
fPktDiskIOTchd  equ     04H             ; Disk_IO has touched this packet


;CMDInit                equ     0                       ; INIT command
;CMDInitBase    equ     27                      ; INIT command for base DDs
InitcUnit       equ     byte ptr PktData        ; number of units returned
InitpEnd        equ     dword ptr PktData+1     ; pointer to free mem after dev
InitDevHlp      equ     dword ptr PktData+1     ; address of Device Helper router
InitEcode       equ     word ptr PktData+1      ; size of code segment
InitEdata       equ     word ptr PktData+3      ; size of data segment
InitParms       equ     dword ptr PktData+5     ; pointer parameters
InitpBPB        equ     dword ptr PktData+5     ; pointer to BPBs
Initdrv         equ     byte ptr PktData+9      ; drive no. assigned to unit 0
                                                ; for resident drivers only:
InitSysiData    equ     byte ptr PktData+11     ; SysInit's DOSALIAS selector

LENInit         equ     10                      ; non-resident drivers
LENInitRes      equ     (LENInit + 2)           ; resident drivers
.errnz          (PktMax LT LENInit)
.errnz          (PktMax LT LENInitRes)

;CMDGetDevSupport equ   1dh                     ; query for extended capability
DCS_Addr        equ     dword ptr PktData+3     ; 16:16 of driver caps struc
VCS_Addr        equ     dword ptr PktData+7     ; 16:16 of volume char struc
LENGetDevSpprt  equ     11

;CMDMedChk      equ     1                       ; Media Check
MedChkmedia     equ     byte ptr PktData        ; last media byte seen
MedChkflag      equ     byte ptr PktData+1      ; -1=change 0=dont know 1=no change
MedChkpVID      equ     dword ptr PktData+2     ; pointer to VID
LENMedChk       equ     6
.errnz          (PktMax LT LENMedChk)

;CMDBldBPB      equ     2                       ; build BPB
BldBPBmedia     equ     byte ptr PktData        ; media byte
BldBPBbuffer    equ     dword ptr PktData+1     ; scratch buffer
BldBPBpBPB      equ     dword ptr PktData+5     ; returned pointer to BPB
LENBldBPB       equ     9
.errnz          (PktMax LT LENBldBPB)

;CMDIOCTLR      equ     3                       ; reserved for 3.x compatability
;CMDINPUT       equ     4                       ; read data from device
;CMDOUTPUT      equ     8                       ; write data to device
;CMDOUTPUTV     equ     9                       ; write data and verify
;CMDInputBypass      EQU    24                  ; cache bypass read data    ?? Needed any more ??
;CMDOutputBypass             EQU    25                  ; cache bypass write data   ?? Needed any more ??
;CMDOutputBypassV     EQU    26                 ; cache bypass write data and verify  ?? Needed any more ??
;CMDIOCTLW      equ     12                      ; reserved for 3.x compatability
IOmedia         equ     byte ptr PktData        ; media byte
IOpData         equ     dword ptr PktData+1     ; transfer address
IOcount         equ     word ptr PktData+5      ; count of bytes/sectors
IOstart         equ     word ptr PktData+7      ; starting sector (block)
                                                ; starting sector is a DD
IOPhysRBA       equ     dword ptr PktData+7     ; physical starting sector

; IOSFN allows char device to virtualize based on open instance. E.g. if user
; opens LPT twice the spooler can use IOSFN field to tell one from another.
; IOSFN is mapped as follows (let N = RESIOSFN):
;            0  to       MAXSFN-1: real SFN    [MAXSFN <= 65536-512-N]
;  65536-512-N  to  65536-256-N-1: fake SFN
;  65536-256-N  to      65536-N-1: FCB SFN
;      65536-N  to        65536-1: unknown
;
; Note for multiple 3xbox the range for fake SFN will have to be expanded by
; 256 per 3xbox. Either that or choose a different mapping scheme.

IOSFN           equ     word ptr PktData+11     ; for device only
LENIO           equ     13
PktAdvise       equ     word ptr PktData+13     ; for >= v12 only
.errnz          (PktMax LT LENIO)

ReadBypassAdj   equ     CMDInputBypass-CMDINPUT ; used internally by kernel
WriteBypassAdj  equ     CMDOutputBypass-CMDOUTPUT ; used internally by kernel

;CMDNDR         equ     5                       ; non-destructive read
NDRbyte         equ     byte ptr PktData        ; byte read from device
LENNDR          equ     1
.errnz          (PktMax LT LENNDR)

;CMDInputS      equ     6                       ; input status
;CMDOutputS     equ     10                      ; output status
LENStatus       equ     0
.errnz          (PktMax LT LENStatus)

;CMDInputF      equ     7                       ; input flush
;CMDOutputF     equ     11                      ; output flush
LENFlush        equ     0
.errnz          (PktMax LT LENFlush)

;CMDOpen                equ     13                      ; device open
;CMDClose       equ     14                      ; device close
OCSFN           equ     word ptr PktData        ; sfn of open instance for virtualization
OCLastRef       equ     word ptr PktData+2      ; flag indicating last close
LENOpnCls       equ     4
.errnz          (PktMax LT LENOpnCls)

;CMDRemMed      equ     15                      ; is media removable
LENRemMed       equ     0
.errnz          (PktMax LT LENRemMed)

;CMDStart EQU   98                              ; start console output
;CMDStop EQU    99                              ; stop console output
CStpSKG EQU     BYTE PTR PktData+0              ; Screen/Keyboard number
LenStp  EQU     1                               ; Size of request additions.
.errnz          (PktMax LT LenStp)

;CMDGenIOCTL    EQU     16                      ; Generic IOCTL
GIOCategory     EQU     BYTE PTR PktData        ; Category Code
GIOFunction     EQU     BYTE PTR PktData+1      ; Function code
GIOParaPack     EQU     DWORD PTR PktData+2     ; pointer to parameter packet
GIODataPack     EQU     DWORD PTR PktData+6     ; pointer to data packet
GIOSFN          EQU     WORD PTR PktData+10     ; (used by Spooler?)
LENGenIOCTL     EQU     12                      ; length w/o new parameters
.errnz          (PktMax LT LENGenIOCTL)
                                                ; new parameters
GIOParaLen      EQU     WORD PTR PktData+12     ; length of parameter packet
GIODataLen      EQU     WORD PTR PktData+14     ; length of data packet
LENGenIOCTL2    EQU     16                      ; length w/ new parameters
.errnz          (PktMax LT LENGenIOCTL2)


;CMDDeInstall   EQU     20                      ; De-Install driver
; DINEndLocn    EQU     DWORD PTR PktData
; DINLength     EQU     WORD PTR PktData+5
LENDeIn         EQU     0
.errnz          (PktMax LT LENDeIn)

;CMDResMed      EQU     17                      ; reset media uncertain
LENResMed       equ     0
.errnz          (PktMax LT LENResMed)

;CMDAddOnPrep   EQU     97                      ; Prepare for add on
Add_On_NewDD    EQU     DWORD PTR PktData
Add_On_SrvRout  EQU     DWORD PTR PktData
LenAddOn        EQU     4

;CMDGetLogMap   EQU     18
;CMDSetLogMap   EQU     19
Logical_Drive   EQU     BYTE PTR PktData; logical drive number
LenGetSetLog    EQU     1

;CMDPartfixeddisks      EQU  22            ; Partitionable Fixed Disks
;CMDGetfd_logunitsmap   EQU  23            ; Get Fixed Disk/Logical Unit Map

LENShutdown     equ     5
SDFuncCode      equ     byte ptr PktData
SDReserveLow    equ     word ptr PktData+1
SDReserveHigh   equ     word ptr PktData+3
;CMDShutdown    equ     28

; Begin additions for @yyyyyy
;CMDGetDriveAssign       EQU     34   ; Get drive assignment  @yyyyyy
LENGetDrvAsgn   equ     5
GDADriveAsgn    equ     byte ptr PktData
GDAReserveLow   equ     word ptr PktData+1
GDAReserveHigh  equ     word ptr PktData+3
; End of additions for @yyyyyy

STDDERROR            EQU     4000H               ; Equate for Device Driver Defined Error
ERROR_DEVICEDRIVER_DEFINED_BASE EQU 0FE00H       ; Equate for Device Driver Defined Error High Byte

;
; The next two symbols are used in the files DEV.ASM and SPROC.ASM to
; set up and verify the set of request packets that the kernel keeps in
; a segment, and to control req pkt allocation and initialization.
;
; See: dev.asm, sproc.asm
;
StaticPackets    EQU    128             ; 128 * 32 = 4k

ReqPktSize      EQU     32              ; "Nice" size for packet (power of 2)
.ERRNZ  (SIZE Packet - 31)              ; Device Driver packet size has changed
;

; ***********************************************
;  PhysDiskTbl - Physical Disk Entry Structure  *
; ***********************************************

 diskentry  STRUC

 ddheader       DD  0        ; Pointer to Device Driver Header
 ereserved      DB  0        ; Reserved
 reldisknum     DB  0        ; Relative Physical Disk # for Device Driver
 devpid         DW  0        ; PID
 deveflags      DW  0        ; Flags for the physical disk
 logunitbitmap  DD  0        ; Logical Unit Bit Map

 diskentry  ends

; PhysDiskTblHdr - Header of physical disk table                                                ;@RAWIO
;                                                                                               ;@RAWIO
PhysDiskTblHdr  struc                                                                           ;@RAWIO
    pdth_tbllen     DW      0           ; Length of PhysDiskTbl                                 ;@RAWIO
    pdth_reserved   DB      0           ; Reserved                                              ;@RAWIO
    pdth_totaldisks DB      0           ; Total number of Partitionable Disks                   ;@RAWIO
PhysDiskTblHdr  ends                                                                            ;@RAWIO
                                                                                                ;@RAWIO
RAWPARMPKT      EQU         524                                                                 ;@RAWIO
MAX_RAW_SECTORS EQU         64                                                                  ;@RAWIO
                                                                                                ;@RAWIO
RWV_COMPLEX_TRACK_TABLE EQU  0          ; NOTE: These bits are also defined in dmioctl.h        ;@RAWIO
RWV_SIMPLE_TRACK_TABLE  EQU  1                                                                  ;@RAWIO
PARMPKT_NOCHECK         EQU  4                                                                  ;@RAWIO

SUBTTL

;*      Obsolete information - use for background info only
;
;
; Attribute bit masks
;
;   Character devices:
;
;   Bit 15 -> must be 1
;       14 -> 1 if the device understands IOCTL control strings
;       13 -> 1 if the device supports output-until-busy
;       12 -> unused
;       11 -> 1 if the device understands Open/Close
;       10 -> must be 0
;        9 -> must be 0
;        8 -> 1 if the device understands Pause/Restart/Gen. IOCTL
;        7 -> unused
;        6 -> unused
;        5 -> unused
;        4 -> 1 if device is recipient of INT 29h
;        3 -> 1 if device is clock device
;        2 -> 1 if device is null device
;        1 -> 1 if device is console output
;        0 -> 1 if device is console input
;
;   Block devices:
;
;   Bit 15 -> must be 0
;       14 -> 1 if the device understands IOCTL control strings
;       13 -> 1 if the device determines media by examining the FAT ID byte.
;           This requires the first sector of the fat to *always* reside in
;           the same place.
;       12 -> unused
;       11 -> 1 if the device understands Open/Close/removable media
;       10 -> must be 0
;        9 -> must be 0
;        8 -> 1 if the device understands Pause/Restart/Gen. IOCTL
;        7 -> unused
;        6 -> unused
;        5 -> unused
;        4 -> unused
;        3 -> unused
;        2 -> unused
;        1 -> unused
;        0 -> unused
;
;DevTyp      EQU     8000H               ; Bit 15 - 1  if Char, 0 if block
;DevIOCtl    EQU     4000H               ; Bit 14 - CONTROL mode bit
;ISFATBYDEV  EQU     2000H               ; Bit 13 - Device uses FAT ID bytes,
;                                       ;  comp media.
;ISNET       EQU     1000H               ; Bit 12 - 1 if a NET device, 0 if
;                                       ;  not.  Currently block only.
;DEVOPCL     EQU     0800H               ; Bit 11 - 1 if this device has
;                                       ;  OPEN,CLOSE and REMOVABLE MEDIA
;                                       ;  entry points, 0 if not
;
; NOTE Bit 9 is currently used on IBM systems to indicate "drive is shared".
;    See IOCTL function 9.  THIS USE IS NOT DOCUMENTED, it is used by some
;    of the utilities which are supposed to FAIL on shared drives on server
;    machines (FORMAT,CHKDSK,RECOVER,..).
;
;IsVID       equ     0080h               ; bit 7 - 1 if device issues UNCERTAIN MEDIA
;;ISSPEC     EQU     0010H               ; Bit 4 - This is special UNUSED
;ISCLOCK     EQU     0008H               ; Bit 3 - This is the clock device.
;ISNULL      EQU     0004H               ; Bit 2 - This is the null device.
;ISCOUT      EQU     0002H               ; Bit 1 - This is the console output.
;ISCIN       EQU     0001H               ; Bit 0 - This is the console input.

OutTilBusy  EQU     2000h                ; Output until busy is enabled
DEV320      EQU     0040H                ; Bit 6 - If Device supports Dos 3.2

LENSaveRestore EQU  1
SRFuncCode     EQU  byte ptr PktData
SRFC_Save      EQU  0
SRFC_Restore   EQU  1
