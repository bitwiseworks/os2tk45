;***	VDMBIOS.INC
;
;
;	MVDM ROM BIOS Data Definitions
;	Copyright (c) 1988,1989 Microsoft Corporation
;
;	For include file hierarchy, see MVDM.INC
;
;
;	MODIFICATION HISTORY
;	    09/20/88  JTP    Created.
;	    12/04/88  JTP    Converted to structure.
;   @235    03/18/91 GAG	Allow disk seek but do verify sector instead
;   @IBMJ   07/08/96  EA     Converge DBCS Bios Header.
;

;***	Low memory/ROM and EBIOS defines - used by VBIOS




;***	BIOS Interrupt Vectors
;

BIOSINT_PARITYCHECK	EQU	002h
BIOSINT_PRINTSCREEN	EQU	005h
BIOSINT_TIMERINTERRUPT	EQU	008h
BIOSINT_KEYINTERRUPT	EQU	009h
BIOSINT_COM2INTERRUPT	EQU	00Bh
BIOSINT_COM1INTERRUPT	EQU	00Ch
BIOSINT_LPT2INTERRUPT	EQU	00Dh
BIOSINT_FLOPPYINTERRUPT		EQU	00Eh
BIOSINT_LPT1INTERRUPT	EQU	00Fh
BIOSINT_VIDEO	EQU	010h
BIOSINT_EQUIPMENTCHECK	EQU	011h
BIOSINT_MEMORYCHECK	EQU	012h
BIOSINT_DISKIO	EQU	013h
BIOSINT_COMIO	EQU	014h
BIOSINT_OSHOOK	EQU	015h
BIOSINT_KEYBOARD	EQU	016h
BIOSINT_PRINTERIO	EQU	017h
BIOSINT_ROMBASIC	EQU	018h
BIOSINT_REBOOT	EQU	019h
BIOSINT_TIMEOFDAY	EQU	01Ah
BIOSINT_KEYBREAK	EQU	01Bh
BIOSINT_CLOCKTICK	EQU	01Ch
BIOSINT_VIDEOPARMS	EQU	01Dh
BIOSINT_DISKPARMS	EQU	01Eh
BIOSINT_VIDEOGRAPH	EQU	01Fh
BIOSINT_OLDDISKIO	EQU	040h
BIOSINT_HDISK1PARMS	EQU	041h
BIOSINT_OLDVIDEO	EQU	042h
BIOSINT_EXTVIDEOGRAPH	EQU	043h
BIOSINT_HDISK2PARMS	EQU	046h


;***	Int 10h Equates (AH values)
;

INT10_SETMODE	EQU	000h
INT10_SETCURSORTYPE	EQU	001h
INT10_SETCURSORPOS	EQU	002h
INT10_QUERYCURSORPOS	EQU	003h
INT10_QUERYLPEN		EQU	004h
INT10_SETACTIVEPAGE	EQU	005h
INT10_SCROLLUP	EQU	006h
INT10_SCROLLDOWN	EQU	007h
INT10_READCHARATTR	EQU	008h
INT10_WRITECHARATTR	EQU	009h
INT10_WRITECHAR		EQU	00Ah
INT10_SETPALETTE	EQU	00Bh
INT10_WRITEPIXEL	EQU	00Ch
INT10_READPIXEL		EQU	00Dh
INT10_WRITETTY	EQU	00Eh
INT10_QUERYMODE		EQU	00Fh
INT10_EGASETPALETTE	EQU	010h
INT10_EGASELECTFONT	EQU	011h
INT10_EGASELECTMISC	EQU	012h
INT10_EGAWRITESTRING	EQU	013h
INT10_VGADISPLAYCODES	EQU	01Ah
INT10_VGAQUERYSTATE	EQU	01Bh
INT10_VGASAVERESTORE	EQU	01Ch


;***	Int 13h Equates (AH values)
;

INT13_DISKRESET		EQU	000h
INT13_DISKSTATUS	EQU	001h
INT13_READSECTORS	EQU	002h
INT13_WRITESECTORS	EQU	003h
INT13_VERIFYSECTORS	EQU	004h
INT13_FORMATTRACK	EQU	005h
INT13_QUERYDRVPARMS	EQU	008h
INT13_DISKSEEK	EQU	00Ch		;@235
INT13_QUERYDASDTYPE	EQU	015h
INT13_QUERYCHANGE	EQU	016h
INT13_SETDISKTYPE	EQU	017h
INT13_SETMEDIATYPE	EQU	018h


;***	Int 13h Status Equates (return in AH)
;

INT13STATUS_SUCCESS	EQU	000h	;successful completion
INT13STATUS_BADCMD	EQU	001h	;bad command
INT13STATUS_BADADDRMARK		EQU	002h	;address mark not found
INT13STATUS_WRTPROTECT	EQU	003h	;write on write-protected disk
INT13STATUS_RECNOTFOUND		EQU	004h	;sector not found
INT13STATUS_BADRESET	EQU	005h	;reset failed
INT13STATUS_MEDIACHANGE		EQU	006h	;media changed
INT13STATUS_INITFAIL	EQU	007h	;parm. act. failed
INT13STATUS_BADDMA	EQU	008h	;DMA overrun
INT13STATUS_DMABOUNDARY		EQU	009h	;DMA across 64K boundary
INT13STATUS_BADSECTOR	EQU	00ah	;bad sector detected
INT13STATUS_BADTRACK	EQU	00bh	;bad track detected
INT13STATUS_BADMEDIATYPE	EQU	00ch   ;unsupported track
INT13STATUS_BADFMTSECNUM	EQU	00dh   ;bad # of sectors on format
INT13STATUS_ADDRMARKDET		EQU	00eh	;ctrl data addr mark detected (HD
INT13STATUS_DMAARBERR	EQU	00fh	;DMA arbitration error
INT13STATUS_BADCRCECC	EQU	010h	;bad CRC/ECC
INT13STATUS_DATACORRECT		EQU	011h	;data ECC corrected
INT13STATUS_BADCNTLR	EQU	020h	;controller failure
INT13STATUS_BADSEEK	EQU	040h	;seek failed
INT13STATUS_TIMEOUT	EQU	080h	;time out
INT13STATUS_DRVNOTREADY		EQU	0aah	;drive not ready
INT13STATUS_UNDEFERR	EQU	0bbh	;undefined error
INT13STATUS_WRITEFAULT	EQU	0cch	;write fault
INT13STATUS_STATUSERROR		EQU	0e0h	;status register error
INT13STATUS_SENSEFAIL	EQU	0ffh	;sense operation failed


;***	Int 15h Equates (AH values)
;

INT15AH_KEYINTERCEPT	EQU	04fh
INT15AH_KEYSYSREQ	EQU	085h
INT15AH_DEVBUSY		EQU	090h
INT15AH_INTCOMPLETE	EQU	091h


;***	Int 15h Equates (AL values)
;

INT15AL_DISKETTE	EQU	001h
INT15AL_KEYBOARD	EQU	002h


;***	Int 16h Equates (AH values)
;
INT16_READCHAR	EQU	000h
INT16_PEEKCHAR	EQU	001h
INT16_QUERYSHIFT	EQU	002h
INT16_SETDELAYS		EQU	003h
INT16_WRITECHAR		EQU	005h
INT16_READEXTCHAR	EQU	010h
INT16_PEEKEXTCHAR	EQU	011h
INT16_QUERYEXTSHIFT	EQU	012h
INT16_READ122CHAR	EQU	020h
INT16_PEEK122CHAR	EQU	021h
INT16_QUERY122SHIFT	EQU	022h


;***	Diskette Parameter Table (pointed by vector BIOSINT_DISKPARMS (1Eh))
;

DefStruc ,fdparmtable_s
    _BYTE    fdp_bSpecify1		;step-rate, head-unload
    _BYTE    fdp_bSpecify2		;head-load, DMA mode
    _BYTE    fdp_bMotorOff		;motor-off delay
    _BYTE    fdp_bSectorSize		;bytes/sec (0=128,1=256,2=512,3=1024)
    _BYTE    fdp_nLastSector		;(or think of it as # sectors/track)
    _BYTE    fdp_lenGapRW		;
    _BYTE    fdp_lenData		;
    _BYTE    fdp_lenGapFormat		;
    _BYTE    fdp_bFormatData		;format initialization byte
    _BYTE    fdp_bSettle		;head settle time
    _BYTE    fdp_bMotorOn		;motor start-up time
EndStruc FDPARMTABLE
DefType PFDPARMTABLE,dd			;pointer to diskette parameter table


;***	Fixed Disk Parameter Table
;

DefStruc ,hdparmtable_s
    _WORD    hdp_wMaxCylinders		;maximum number of cylinders
    _BYTE    hdp_bMaxHeads		;maximum number of heads
    _WORD    hdp_wReserve1		;reserved (not used)
    _WORD    hdp_wWritePrecompCyl	;starting write precompensation cyl.
    _BYTE    hdp_bMaxECCDataBurstLen	;maximum ECC data burst length
    _BYTE    hdp_bControl		;control byte
    _BYTE    hdp_abReserve2,,3		;reserved (not used)
    _WORD    hdp_wLandingZone		;landing zone for head parking
    _BYTE    hdp_bSectorsPerTrack	;number of sectors per track
    _BYTE    hdp_bReserve3		;reserved for future use
EndStruc HDPARMTABLE
DefType PHDPARMTABLE,dd			;pointer to hard disk parameter table


;***	Hard disk parameter - control byte bit mask
;

HDPCTRL_DISABLERETRY	EQU	0c0h	;disable retries
HDPCTRL_EXCEED8HEADS	EQU	008h	;more than 8 heads


;***	Model byte equates (located at F000:FFFE in actual ROM)
;
ROMMODEL_PC1	EQU	00FFh		;PC-1, Expansion, Old PC2
ROMMODEL_PC2	EQU	00FEh		;PC-2, Newer
ROMMODEL_JR	EQU	00FDh		;HALO
ROMMODEL_AT	EQU	00FCh		;Salmon (PC-3)


;***	ROM format
;
ROMHDR_SIGNATURE	EQU	0AA55h
ROMHDR_INCR	EQU	2048
BLOCKSIZE	EQU	512
BLOCKSPERPAGE	EQU	(PAGESIZE / BLOCKSIZE)


DefStruc ,romhdr_s
    _WORD romhdr_wSignature	;should be ROMHDR_SIGNATURE
    _BYTE romhdr_bnBlocks	;# of ROM blocks
    _BYTE romhdr_abFiller,,ROMHDR_INCR-3
EndStruc ROMHDR

DefType PROMHDR,dd


;***	ROM BIOS Data Area
;
DefStruc ,romdata_s

VPVOID	rb_avpIVT,,IVT_LEN/4   ;interrupt vector table


;***	Start of ROM BIOS Data Areas (0400h)
;
_WORD	 rb_awCOM,,4		;RS232_BASE   (addresses of serial cards)
_WORD	 rb_awLPT,,4		;PRINTER_BASE (addresses of printer cards)
_WORD	 rb_fsEquip		;EQUIP_FLAG   (installed hardware)

BIOSEQUIP_FLOPPY	EQU	000001h	    ;machine has a floppy
BIOSEQUIP_X87	EQU	000002h	    ;X87=1 if coprocessor installed
BIOSEQUIP_VIDEOMASK	EQU	000030h	    ;video configuration bits
BIOSEQUIP_COLOR40VIDEO	EQU	000010h	    ;
BIOSEQUIP_COLOR80VIDEO	EQU	000020h	    ;
BIOSEQUIP_MONOVIDEO	EQU	000030h	    ;
BIOSEQUIP_FLOPPYMASK	EQU	0000C0h	    ;# floppies-1 (if IPLDISK=1)
BIOSEQUIP_COMMASK	EQU	000E00h	    ;# COM ports
BIOSEQUIP_PRINTERMASK	EQU	00C000h	    ;# LPT ports

BBOOL	rb_bfMfgTest	       ;MFG_TST	     (initialization flag)
_WORD	 rb_wMemSize		;MEMORY_SIZE  (memory size in Kbytes)
BBOOL	rb_bfMfgError,,2       ;MFG_ERR_FLAG (scratchpad for manufacturing)


;***	Start of keyboard data area (0417h)
;
_BYTE	 rb_fbKFlag		;KB_FLAG      (keyboard shift states)

BIOSKFLAG_RIGHTSHIFT	EQU	001h
BIOSKFLAG_LEFTSHIFT	EQU	002h
BIOSKFLAG_SHIFTMASK	EQU	003h
BIOSKFLAG_CTRL	EQU	004h
BIOSKFLAG_ALT	EQU	008h
BIOSKFLAG_SCROLLLOCK	EQU	010h
BIOSKFLAG_NUMLOCK	EQU	020h
BIOSKFLAG_CAPSLOCK	EQU	040h
BIOSKFLAG_INSERT	EQU	080h

_BYTE	 rb_fbKFlag1		;KB_FLAG_1    (second byte of keyboard status)

BIOSKFLAG1_LEFTCTRL	EQU	001h	    ;left CTRL down
BIOSKFLAG1_LEFTALT	EQU	002h	    ;left ALT down
BIOSKFLAG1_SYSSHIFT	EQU	004h	    ;SysReq pressed and held
BIOSKFLAG1_HOLDSTATE	EQU	008h	    ;Ctrl-NumLock has been pressed
BIOSKFLAG1_SCROLLDOWN	EQU	010h
BIOSKFLAG1_NUMDOWN	EQU	020h
BIOSKFLAG1_CAPSDOWN	EQU	040h
BIOSKFLAG1_INSDOWN	EQU	080h

_BYTE	 rb_bKAlt		;ALT_INPUT    (storage for Alt-keypad entry)
_WORD	 rb_npKHead		;BUFFER_HEAD  (advanced at task-time)
_WORD	 rb_npKTail		;BUFFER_TAIL  (advanced at interrupt-time)
_WORD	 rb_abKBuf,,16		;KB_BUFFER


;***	Start of floppy data area (043Eh)
;
_BYTE	 rb_fbFSeekStatus	;SEEK_STATUS

BIOSSEEK_DRV0NORECAL	EQU	001h
BIOSSEEK_DRV1NORECAL	EQU	002h
BIOSSEEK_DRV2NORECAL	EQU	004h
BIOSSEEK_DRV3NORECAL	EQU	008h
BIOSSEEK_DRVINTERRUPT	EQU	080h

_BYTE	 rb_fbFMotorStatus	;MOTOR_STATUS

BIOSMOTOR_DRV0RUNNING	EQU	001h
BIOSMOTOR_DRV1RUNNING	EQU	002h
BIOSMOTOR_DRV2RUNNING	EQU	004h
BIOSMOTOR_DRV3RUNNING	EQU	008h
BIOSMOTOR_DRVSELECT	EQU	030h
BIOSMOTOR_WRTOPERATION	EQU	080h

_BYTE	 rb_bFMotorCount	;MOTOR_COUNT
_BYTE	 rb_bFStatus		;DISKETTE_STATUS
_BYTE	 rb_abFCtlStatus,,7	;NEC_STATUS (also CMD_BLOCK and HD_ERROR)


;***	Start of video data area (0449h)
;
_BYTE	 rb_bVMode		;CRT_MODE

BIOSVMODE_BW40	EQU	000h
BIOSVMODE_CO40	EQU	001h
BIOSVMODE_BW80	EQU	002h
BIOSVMODE_CO80	EQU	003h
BIOSVMODE_CO320X200X4	EQU	004h
BIOSVMODE_BW320X200X4	EQU	005h
BIOSVMODE_CO640X200X2	EQU	006h
BIOSVMODE_MONO80	EQU	007h
BIOSVMODE_PCJR1		EQU	008h
BIOSVMODE_PCJR2		EQU	009h
BIOSVMODE_PCJR3		EQU	00Ah
BIOSVMODE_COLRFONT	EQU	00Bh
BIOSVMODE_MONOFONT	EQU	00Ch
BIOSVMODE_CO320X200X16	EQU	00Dh
BIOSVMODE_CO640X200X16	EQU	00Eh
BIOSVMODE_MONO640X350X4		EQU	00Fh
BIOSVMODE_CO640X350X16	EQU	010h
BIOSVMODE_CO640X480X2	EQU	011h
BIOSVMODE_CO640X480X16	EQU	012h
BIOSVMODE_CO320X200X256		EQU	013h

_WORD	 rb_nVCols		;CRT_COLS
_WORD	 rb_wVLen		;CRT_LEN
_WORD	 rb_wVStart		;CRT_START
_WORD	 rb_awVCursorPos,,8	;CURSOR_POSN
_WORD	 rb_wVCursorMode	;CURSOR_MODE
_BYTE	 rb_bVPage		;ACTIVE_PAGE
_WORD	 rb_wVPort		;ADDR_6845
_BYTE	 rb_bVModeByte		;CRT_MODE_SET
_BYTE	 rb_bVPalByte		;CRT_PALETTE


;***	Start of POST data area (0467h)
;
VPVOID	rb_vpRomInit	       ;IO_ROM_INIT and IO_ROM_SEG
BBOOL	rb_fInterrupt	       ;INTR_FLAG


;***	Start of timer data area (046Ch)
;
ULONG	rb_ulTickCount	       ;TIMER_LOW and TIMER_HIGH
_BYTE	 rb_bTickOverflow	;TIMER_OFL


;***	Start of system data area (0471h)
;
BBOOL	rb_fBreak	       ;BIOS_BREAK

BIOSBREAK_SET	EQU	080h

SBOOL	rb_fReset	       ;RESET_FLAG

BIOSRESET_WARMBOOT	EQU	01234h


;***	Start of hard disk data area (0474h)
;
_BYTE	 rb_bDStatus		;DISK_STATUS1 (fixed disk status byte)
_BYTE	 rb_nDTotal		;HF_NUM	      (count of fixed disk drives)
_BYTE	 rb_bDControl		;CONTROL_BYTE (control byte drive options)
_BYTE	 rb_bDPortOffset	;PORT_OFF     (port offset)


;***	Start of COM and LPT time-out data area (0478h)
;
_BYTE	 rb_abLPTtimeout,,4	;PRINT_TIM_OUT
_BYTE	 rb_abCOMtimeout,,4	;RS232_TIM_OUT


;***	Start of additional keyboard data area (0480h)
;
_WORD	 rb_npKBufStart		;BUFFER_START
_WORD	 rb_npKBufEnd		;BUFFER_END


;***	Start of EGA/VGA data area (0484h)
;
_BYTE	 rb_nVRows		;ROWS	      (# rows-1)
_WORD	 rb_nVCharHeight	;POINTS	      (height of char in pixels)
_BYTE	 rb_bVInfo		;INFO

BIOSVINFO_NOEMULATE	EQU	001h	;(controls cursor emulation)
BIOSVINFO_EGAMONO	EQU	002h	;
BIOSVINFO_WAITRETRC	EQU	004h	;
BIOSVINFO_EGAINACTIVE	EQU	008h	;
BIOSVINFO_MEMMASK	EQU	060h	;(00=64k, 01=128k, 10=192k, 11=256k)
BIOSVINFO_DONTCLEAR	EQU	080h	;(don't clear screen on resets)

_BYTE	 rb_bVSwitches		;INFO_3

BIOSVSW_SW1OFF	EQU	001h
BIOSVSW_SW2OFF	EQU	002h
BIOSVSW_SW3OFF	EQU	004h
BIOSVSW_SW4OFF	EQU	008h
BIOSVSW_FEATUREBITS	EQU	0F0h

_BYTE	 rb_bVFlags		;Flags (for VGA and MCGA)

BIOSVFLAGS_VGAACTIVE	EQU	001h
BIOSVFLAGS_GRAYSCALES	EQU	002h
BIOSVFLAGS_VGAMONO	EQU	004h
BIOSVFLAGS_NODEFPALETTE		EQU	008h
BIOSVFLAGS_DSPSWITCHING		EQU	040h

BIOSVFLAGS_350INES	EQU	000h
BIOSVFLAGS_400INES	EQU	010h
BIOSVFLAGS_200INES	EQU	080h
BIOSVFLAGS_RESRVDLINES	EQU	090h


;***	Reserved area (048Ah)
;
_BYTE	 rb_bReserved1,,1	;reserved


;***	Start of additional diskette data area (048Bh)
;
_BYTE	 rb_bFLastRate		;LASTRATE


;***	Start of additional hard disk data area (048Ch)
;
_BYTE	 rb_bDStatusReg		;HF_STATUS
_BYTE	 rb_bDErrorReg		;HF_ERROR
BBOOL	rb_fDInterrupt	       ;HF_INT_FLAG
BBOOL	rb_fDCombo	       ;HF_CNTRL     (combo hard disk/diskette card)


;***	Start of additional diskette data area (0490h)
;
_BYTE	 rb_bF0State		;DSK_STATE    (drive 0 media state)
_BYTE	 rb_bF1State		;	      (drive 1 media state)
_BYTE	 rb_bF0StartState	;	      (drive 0 operation start state)
_BYTE	 rb_bF1StartState	;	      (drive 1 operation start state)
_BYTE	 rb_bF0Cyl		;DSK_TRK      (drive 0 current cylinder)
_BYTE	 rb_bF1Cyl		;	      (drive 1 current cylinder)


;***	Start of additional keyboard data area (0496h)
;
_BYTE	 rb_fbKFlag3		;KB_FLAG_3

BIOSKFLAG3_E1PREFIX	EQU	001h	;supported
BIOSKFLAG3_SECPREFIX	EQU	002h	;supported
BIOSKFLAG3_RIGHTCTRL	EQU	004h	;right CTRL down
BIOSKFLAG3_RIGHTALT	EQU	008h	;right ALT down
BIOSKFLAG3_KBX	EQU	010h	;supported
BIOSKFLAG3_SETNUMLOCK	EQU	020h	;supported
BIOSKFLAG3_LASTID	EQU	040h	;supported
BIOSKFLAG3_READINGID	EQU	080h	;supported (we never SET it though!)

_BYTE	 rb_fbKFlag2		;KB_FLAG_2

BIOSKFLAG2_LEDS		EQU	007h	;supported

BIOSKFLAG2_CYR	EQU	008h	;0 = Latin, 1 = Cyrillic

BIOSKFLAG2_ACK	EQU	010h	;supported
BIOSKFLAG2_RESEND	EQU	020h	;supported
BIOSKFLAG2_LEDUPDATE	EQU	040h	;always clear
BIOSKFLAG2_XMITERROR	EQU	080h	;always clear


;***	Start of real-time CMOS clock data area (0498h)
;
VPVOID	rb_vpWaitFlag	       ;USER_FLAG and USER_FLAG_SEG
ULONG	rb_ulWaitFlag	       ;RTC_LOW and RTC_HIGH
BBOOL	rb_fWaitActive	       ;RTC_WAIT_FLAG

_BYTE	 rb_bReserved2,,7	 ;this should "ORG" us to 04A8h


;***	Start of additional EGA/VGA data area (04A8h)
;
VPVOID	rb_vpVideoPtrTable

;***	Format of VideoPtrTable:
;
;	 VPVOID[0]	Parameter Table Pointer
;			Initialized to BIOS Video Parameter Table.
;			This value MUST exist.  For a VGA, the table is:
;
;			_BYTE	 Value for CRT_COLS
;			_BYTE	 Value for ROWS
;			_BYTE	 Value for POINTS
;			_WORD	 Value for CRT_LEN
;			_BYTE*4	 Values for SEQ registers 1-4
;			_BYTE	 Value for Miscellaneous Output register
;			_BYTE*25 Values for CRT registers 0-18h
;			_BYTE*20 Values for ATC registers 0-13h
;			_BYTE*9	 Values for GDC registers 0-8
;
;	 VPVOID[1]	Parameter Save Area Pointer
;			Initialized to 0000:0000, this value is optional.
;			When non-zero, this pointer will be used as pointer
;			to a RAM area where certain dynamic values are to
;			be saved. When in EGA operation this RAM area will
;			hold the 16 EGA palette register values plus
;			the overscan value in bytes 00h-10h respectively.
;			At least 256 bytes must be allocated for this area.
;
;	 VPVOID[2]	Alpha Character Set Override Pointer
;			Initialized to 0000:0000, this value is optional.
;			When non-zero, this pointer is used as a pointer
;			to a tables described as follows:
;
;			_BYTE	 Bytes/character
;
;			_BYTE	 Block to load, should be zero for normal
;				operation
;
;			_WORD	 Count to store, should be 256 for normal
;				operation
;
;			_WORD	 Character offset, should be zero for normal
;				operation
;
;			VPVOID	Pointer to a font table
;
;			_BYTE	 Displayable rows
;				if 'FF' the maximum calculated value will be
;				used, else this value will be used
;
;			_BYTE	 Consecutive bytes of mode values for which
;				this font description is to be used.
;				The end of this stream is indicated by a
;				byte code of 'FF'
;
;	 VPVOID[3]	Graphics Character Set Override Pointer
;			Initialized to 0000:0000, this value is optional.
;			When non-zero, this pointer is used as a pointer
;			to a tables described as follows:
;
;			_BYTE	 Displayable rows
;
;			_WORD	 Bytes per character
;
;			VPVOID	Pointer to a font table
;
;			_BYTE	 Consecutive bytes of mode values for which
;				this font description is to be used.
;				The end of this stream is indicated by a
;				byte code of 'FF'
;
;	 VPVOID[4]	Address of Secondary Video Pointer Table (VGA ONLY):
;
;			_WORD	 Length of this table, in bytes
;			VPVOID	Pointer to Display Combination Code Table
;			VPVOID	Pointer to second Alpha Character Set Override
;			VPVOID	Pointer to User Palette Profile Table
;			VPVOID	Reserved
;			VPVOID	Reserved
;			VPVOID	Reserved
;
;	 VPVOID[5]	Reserved and set to 0000:0000.
;
;	 VPVOID[6]	Reserved and set to 0000:0000.
;

_BYTE	 rb_bReserved3,,054h	;this should "ORG" us to 0500h

;***	Extra data area (0500h)
;
_BYTE	 rb_bPrtScrStatus	;STATUS_BYTE
EndStruc ROMDATA

DefType PROMDATA,dd



VDMBase	  equ <ds:[0]>
.errnz	(size romdata_s NE 0501h)
