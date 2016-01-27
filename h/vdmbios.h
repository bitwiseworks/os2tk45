/*static char *SCCSID = "@(#)vdmbios.h  6.2 92/01/28";*/
/***    VDMBIOS.H
 *
 *
 *      MVDM ROM BIOS Data Definitions
 *      Copyright (c) 1992 IBM Corporation
 *
 *      For include file hierarchy, see MVDM.H
 *
 *
 */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
/* INC */

#ifndef __VDMBIOS__
#define __VDMBIOS__

#pragma pack(4)

/***ET+ Low memory/ROM and EBIOS defines - used by VBIOS */

/* c-begin */
#define ROMDATA_START   (IVT_START+IVT_LEN)
#define ROMDATA_LEN     0x101   /* sizeof(ROMDATA)-IVT_LEN */

#define ROMBIOS_START   0xF0000
#define ROMBIOS_LEN     0x10000
#define ROMBIOS_PAGES   PAGESFROMBYTES(ROMBIOS_LEN)

#define PS2BIOS_START   0xE0000
#define PS2BIOS_LEN     0x20000
#define PS2BIOS_PAGES   PAGESFROMBYTES(PS2BIOS_LEN)

#define EXTROM_START    0xC0000 /* where and how far to */
#define EXTROM_LEN      0x40000 /* search for external adapter ROMs */
#define EXTROM_PAGES    PAGESFROMBYTES(EXTROM_LEN)

#define EBIOSDATA_START 0x9FC00
#define EBIOSDATA_LEN   0x00400
#define EBIOSDATA_PAGES PAGESFROMBYTES(EBIOSDATA_LEN)

#if     ROMDATA_START   != IVT_START+IVT_LEN
#error  ROMDATA_START not equal to IVT_START+IVT_LEN
#endif

#if     defined(IVT_START) && defined(DCA_START)

#if     DCA_START       != ROMDATA_START+ROMDATA_LEN
#error  DCA_START not equal to ROMDATA_START+ROMDATA_LEN
#endif

#define LOWMEM_START    IVT_START
#define LOWMEM_LEN      (IVT_LEN+ROMDATA_LEN+DCA_LEN)
#define LOWMEM_COPYLEN  (IVT_LEN+ROMDATA_LEN)
#define LOWMEM_PAGES    PAGESFROMBYTES(LOWMEM_LEN)

#endif
/* c-end */
/*end*/


/***    BIOS Interrupt Vectors
 */

#define BIOSINT_PARITYCHECK     0x02
#define BIOSINT_PRINTSCREEN     0x05
#define BIOSINT_TIMERINTERRUPT  0x08
#define BIOSINT_KEYINTERRUPT    0x09
#define BIOSINT_COM2INTERRUPT   0x0B
#define BIOSINT_COM1INTERRUPT   0x0C
#define BIOSINT_LPT2INTERRUPT   0x0D
#define BIOSINT_FLOPPYINTERRUPT 0x0E
#define BIOSINT_LPT1INTERRUPT   0x0F
#define BIOSINT_VIDEO           0x10
#define BIOSINT_EQUIPMENTCHECK  0x11
#define BIOSINT_MEMORYCHECK     0x12
#define BIOSINT_DISKIO          0x13
#define BIOSINT_COMIO           0x14
#define BIOSINT_OSHOOK          0x15
#define BIOSINT_KEYBOARD        0x16
#define BIOSINT_PRINTERIO       0x17
#define BIOSINT_ROMBASIC        0x18
#define BIOSINT_REBOOT          0x19
#define BIOSINT_TIMEOFDAY       0x1A
#define BIOSINT_KEYBREAK        0x1B
#define BIOSINT_CLOCKTICK       0x1C
#define BIOSINT_VIDEOPARMS      0x1D
#define BIOSINT_DISKPARMS       0x1E
#define BIOSINT_VIDEOGRAPH      0x1F
#define BIOSINT_OLDDISKIO       0x40
#define BIOSINT_HDISK1PARMS     0x41
#define BIOSINT_OLDVIDEO        0x42
#define BIOSINT_EXTVIDEOGRAPH   0x43
#define BIOSINT_HDISK2PARMS     0x46


/***    Int 10h Equates (AH values)
 */

#define INT10_SETMODE           0x00
#define INT10_SETCURSORTYPE     0x01
#define INT10_SETCURSORPOS      0x02
#define INT10_QUERYCURSORPOS    0x03
#define INT10_QUERYLPEN         0x04
#define INT10_SETACTIVEPAGE     0x05
#define INT10_SCROLLUP          0x06
#define INT10_SCROLLDOWN        0x07
#define INT10_READCHARATTR      0x08
#define INT10_WRITECHARATTR     0x09
#define INT10_WRITECHAR         0x0A
#define INT10_SETPALETTE        0x0B
#define INT10_WRITEPIXEL        0x0C
#define INT10_READPIXEL         0x0D
#define INT10_WRITETTY          0x0E
#define INT10_QUERYMODE         0x0F
#define INT10_EGASETPALETTE     0x10
#define INT10_EGASELECTFONT     0x11
#define INT10_EGASELECTMISC     0x12
#define INT10_EGAWRITESTRING    0x13
#define INT10_VGADISPLAYCODES   0x1A
#define INT10_VGAQUERYSTATE     0x1B
#define INT10_VGASAVERESTORE    0x1C


/***    Int 13h Equates (AH values)
 */

#define INT13_DISKRESET         0x00
#define INT13_DISKSTATUS        0x01
#define INT13_READSECTORS       0x02
#define INT13_WRITESECTORS      0x03
#define INT13_VERIFYSECTORS     0x04
#define INT13_FORMATTRACK       0x05
#define INT13_QUERYDRVPARMS     0x08
#define INT13_DISKSEEK          0x0C            /*@235 */
#define INT13_QUERYDASDTYPE     0x15
#define INT13_QUERYCHANGE       0x16
#define INT13_SETDISKTYPE       0x17
#define INT13_SETMEDIATYPE      0x18


/***    Int 13h Status Equates (return in AH)
 */

#define INT13STATUS_SUCCESS     0x00    /* successful completion */
#define INT13STATUS_BADCMD      0x01    /* bad command */
#define INT13STATUS_BADADDRMARK 0x02    /* address mark not found */
#define INT13STATUS_WRTPROTECT  0x03    /* write on write-protected disk */
#define INT13STATUS_RECNOTFOUND 0x04    /* sector not found */
#define INT13STATUS_BADRESET    0x05    /* reset failed (HD) */
#define INT13STATUS_MEDIACHANGE 0x06    /* media changed */
#define INT13STATUS_INITFAIL    0x07    /* parm. act. failed (HD) */
#define INT13STATUS_BADDMA      0x08    /* DMA overrun */
#define INT13STATUS_DMABOUNDARY 0x09    /* DMA across 64K boundary */
#define INT13STATUS_BADSECTOR   0x0a    /* bad sector detected (HD) */
#define INT13STATUS_BADTRACK    0x0b    /* bad track detected (HD) */
#define INT13STATUS_BADMEDIATYPE 0x0c   /* unsupported track (HD) */
#define INT13STATUS_BADFMTSECNUM 0x0d   /* bad # of sectors on format (HD) */
#define INT13STATUS_ADDRMARKDET 0x0e    /* ctrl data addr mark detected (HD */
#define INT13STATUS_DMAARBERR   0x0f    /* DMA arbitration error (HD) */
#define INT13STATUS_BADCRCECC   0x10    /* bad CRC/ECC */
#define INT13STATUS_DATACORRECT 0x11    /* data ECC corrected */
#define INT13STATUS_BADCNTLR    0x20    /* controller failure */
#define INT13STATUS_BADSEEK     0x40    /* seek failed */
#define INT13STATUS_TIMEOUT     0x80    /* time out */
#define INT13STATUS_DRVNOTREADY 0xaa    /* drive not ready (HD) */
#define INT13STATUS_UNDEFERR    0xbb    /* undefined error (HD) */
#define INT13STATUS_WRITEFAULT  0xcc    /* write fault (HD) */
#define INT13STATUS_STATUSERROR 0xe0    /* status register error (HD) */
#define INT13STATUS_SENSEFAIL   0xff    /* sense operation failed (HD) */


/***    Int 15h Equates (AH values)
 */

#define INT15AH_KEYINTERCEPT    0x4f
#define INT15AH_KEYSYSREQ       0x85
#define INT15AH_DEVBUSY         0x90
#define INT15AH_INTCOMPLETE     0x91


/***    Int 15h Equates (AL values)
 */

#define INT15AL_DISKETTE        0x01
#define INT15AL_KEYBOARD        0x02


/***    Int 16h Equates (AH values)
 */
#define INT16_READCHAR          0x00
#define INT16_PEEKCHAR          0x01
#define INT16_QUERYSHIFT        0x02
#define INT16_SETDELAYS         0x03
#define INT16_WRITECHAR         0x05
#define INT16_READEXTCHAR       0x10
#define INT16_PEEKEXTCHAR       0x11
#define INT16_QUERYEXTSHIFT     0x12
#define INT16_READ122CHAR       0x20
#define INT16_PEEK122CHAR       0x21
#define INT16_QUERY122SHIFT     0x22


/***    Diskette Parameter Table (pointed by vector BIOSINT_DISKPARMS (1Eh))
 */

typedef struct fdparmtable_s {
    BYTE    fdp_bSpecify1;              /* step-rate, head-unload */
    BYTE    fdp_bSpecify2;              /* head-load, DMA mode */
    BYTE    fdp_bMotorOff;              /* motor-off delay */
    BYTE    fdp_bSectorSize;            /* bytes/sec (0=128,1=256,2=512,3=1024) */
    BYTE    fdp_nLastSector;            /* (or think of it as # sectors/track) */
    BYTE    fdp_lenGapRW;
    BYTE    fdp_lenData;
    BYTE    fdp_lenGapFormat;
    BYTE    fdp_bFormatData;            /* format initialization byte */
    BYTE    fdp_bSettle;                /* head settle time */
    BYTE    fdp_bMotorOn;               /* motor start-up time */
} FDPARMTABLE;
typedef FDPARMTABLE *PFDPARMTABLE;      /* pointer to diskette parameter table */


/***    Fixed Disk Parameter Table
 */

typedef struct hdparmtable_s {
    WORD    hdp_wMaxCylinders;          /* maximum number of cylinders */
    BYTE    hdp_bMaxHeads;              /* maximum number of heads */
    WORD    hdp_wReserve1;              /* reserved (not used) */
    WORD    hdp_wWritePrecompCyl;       /* starting write precompensation cyl. */
    BYTE    hdp_bMaxECCDataBurstLen;    /* maximum ECC data burst length */
    BYTE    hdp_bControl;               /* control byte */
    BYTE    hdp_abReserve2[3];          /* reserved (not used) */
    WORD    hdp_wLandingZone;           /* landing zone for head parking */
    BYTE    hdp_bSectorsPerTrack;       /* number of sectors per track */
    BYTE    hdp_bReserve3;              /* reserved for future use */
} HDPARMTABLE;
typedef HDPARMTABLE *PHDPARMTABLE;      /* pointer to hard disk parameter table */


/***    Hard disk parameter - control byte bit mask
 */

#define HDPCTRL_DISABLERETRY    0xc0    /* disable retries */
#define HDPCTRL_EXCEED8HEADS    0x08    /* more than 8 heads */


/***    Model byte equates (located at F000:FFFE in actual ROM)
 */
#define ROMMODEL_PC1    0x0FF           /* PC-1, Expansion, Old PC2 (PC) */
#define ROMMODEL_PC2    0x0FE           /* PC-2, Newer              (XT) */
#define ROMMODEL_JR     0x0FD           /* HALO                     (JR) */
#define ROMMODEL_AT     0x0FC           /* Salmon (PC-3)            (AT) */


/***    ROM format
 */
#define ROMHDR_SIGNATURE        0xAA55
#define ROMHDR_INCR             2048
#define BLOCKSIZE               512
#define BLOCKSPERPAGE           (PAGESIZE / BLOCKSIZE)

/* c-begin */
#define PAGESFROMBLOCKS(nbl)    ((nbl+BLOCKSPERPAGE-1) / BLOCKSPERPAGE)
#define BLOCKSFROMBYTES(nb)     ((nb+BLOCKSIZE-1) / BLOCKSIZE)
/* c-end */

typedef struct romhdr_s {
    WORD romhdr_wSignature;     /* should be ROMHDR_SIGNATURE */
    BYTE romhdr_bnBlocks;       /* # of ROM blocks */
    BYTE romhdr_abFiller[ROMHDR_INCR-3];
} ROMHDR;

typedef ROMHDR *PROMHDR;


/***    ROM BIOS Data Area
 */
typedef struct romdata_s {

VPVOID  rb_avpIVT[IVT_LEN/4];   /* interrupt vector table */


/***    Start of ROM BIOS Data Areas (0400h)
 */
WORD    rb_awCOM[4];            /* RS232_BASE   (addresses of serial cards) */
WORD    rb_awLPT[4];            /* PRINTER_BASE (addresses of printer cards) */
WORD    rb_fsEquip;             /* EQUIP_FLAG   (installed hardware) */

#define BIOSEQUIP_FLOPPY        0x00001     /* machine has a floppy */
#define BIOSEQUIP_X87           0x00002     /* X87=1 if coprocessor installed */
#define BIOSEQUIP_VIDEOMASK     0x00030     /* video configuration bits */
#define BIOSEQUIP_COLOR40VIDEO  0x00010
#define BIOSEQUIP_COLOR80VIDEO  0x00020
#define BIOSEQUIP_MONOVIDEO     0x00030
#define BIOSEQUIP_FLOPPYMASK    0x000C0     /* # floppies-1 (if IPLDISK=1) */
#define BIOSEQUIP_COMMASK       0x00E00     /* # COM ports */
#define BIOSEQUIP_PRINTERMASK   0x0C000     /* # LPT ports */

BBOOL   rb_bfMfgTest;           /* MFG_TST      (initialization flag) */
WORD    rb_wMemSize;            /* MEMORY_SIZE  (memory size in Kbytes) */
BBOOL   rb_bfMfgError[2];       /* MFG_ERR_FLAG (scratchpad for manufacturing) */


/***    Start of keyboard data area (0417h)
 */
BYTE    rb_fbKFlag;             /* KB_FLAG      (keyboard shift states) */

#define BIOSKFLAG_RIGHTSHIFT    0x01
#define BIOSKFLAG_LEFTSHIFT     0x02
#define BIOSKFLAG_SHIFTMASK     0x03
#define BIOSKFLAG_CTRL          0x04
#define BIOSKFLAG_ALT           0x08
#define BIOSKFLAG_SCROLLLOCK    0x10
#define BIOSKFLAG_NUMLOCK       0x20
#define BIOSKFLAG_CAPSLOCK      0x40
#define BIOSKFLAG_INSERT        0x80

BYTE    rb_fbKFlag1;            /* KB_FLAG_1    (second byte of keyboard status) */

#define BIOSKFLAG1_LEFTCTRL     0x01        /* left CTRL down */
#define BIOSKFLAG1_LEFTALT      0x02        /* left ALT down */
#define BIOSKFLAG1_SYSSHIFT     0x04        /* SysReq pressed and held */
#define BIOSKFLAG1_HOLDSTATE    0x08        /* Ctrl-NumLock has been pressed */
#define BIOSKFLAG1_SCROLLDOWN   0x10
#define BIOSKFLAG1_NUMDOWN      0x20
#define BIOSKFLAG1_CAPSDOWN     0x40
#define BIOSKFLAG1_INSDOWN      0x80

BYTE    rb_bKAlt;               /* ALT_INPUT    (storage for Alt-keypad entry) */
WORD    rb_npKHead;             /* BUFFER_HEAD  (advanced at task-time) */
WORD    rb_npKTail;             /* BUFFER_TAIL  (advanced at interrupt-time) */
WORD    rb_abKBuf[16];          /* KB_BUFFER */


/***    Start of floppy data area (043Eh)
 */
BYTE    rb_fbFSeekStatus;       /* SEEK_STATUS */

#define BIOSSEEK_DRV0NORECAL    0x01
#define BIOSSEEK_DRV1NORECAL    0x02
#define BIOSSEEK_DRV2NORECAL    0x04
#define BIOSSEEK_DRV3NORECAL    0x08
#define BIOSSEEK_DRVINTERRUPT   0x80

BYTE    rb_fbFMotorStatus;      /* MOTOR_STATUS */

#define BIOSMOTOR_DRV0RUNNING   0x01
#define BIOSMOTOR_DRV1RUNNING   0x02
#define BIOSMOTOR_DRV2RUNNING   0x04
#define BIOSMOTOR_DRV3RUNNING   0x08
#define BIOSMOTOR_DRVSELECT     0x30
#define BIOSMOTOR_WRTOPERATION  0x80

BYTE    rb_bFMotorCount;        /* MOTOR_COUNT */
BYTE    rb_bFStatus;            /* DISKETTE_STATUS */
BYTE    rb_abFCtlStatus[7];     /* NEC_STATUS (also CMD_BLOCK and HD_ERROR) */


/***    Start of video data area (0449h)
 */
BYTE    rb_bVMode;              /* CRT_MODE */

#define BIOSVMODE_BW40           0x00
#define BIOSVMODE_CO40           0x01
#define BIOSVMODE_BW80           0x02
#define BIOSVMODE_CO80           0x03
#define BIOSVMODE_CO320X200X4    0x04
#define BIOSVMODE_BW320X200X4    0x05
#define BIOSVMODE_CO640X200X2    0x06
#define BIOSVMODE_MONO80         0x07
#define BIOSVMODE_PCJR1          0x08
#define BIOSVMODE_PCJR2          0x09
#define BIOSVMODE_PCJR3          0x0A
#define BIOSVMODE_COLRFONT       0x0B
#define BIOSVMODE_MONOFONT       0x0C
#define BIOSVMODE_CO320X200X16   0x0D
#define BIOSVMODE_CO640X200X16   0x0E
#define BIOSVMODE_MONO640X350X4  0x0F
#define BIOSVMODE_CO640X350X16   0x10
#define BIOSVMODE_CO640X480X2    0x11
#define BIOSVMODE_CO640X480X16   0x12
#define BIOSVMODE_CO320X200X256  0x13

WORD    rb_nVCols;              /* CRT_COLS */
WORD    rb_wVLen;               /* CRT_LEN */
WORD    rb_wVStart;             /* CRT_START */
WORD    rb_awVCursorPos[8];     /* CURSOR_POSN */
WORD    rb_wVCursorMode;        /* CURSOR_MODE */
BYTE    rb_bVPage;              /* ACTIVE_PAGE */
WORD    rb_wVPort;              /* ADDR_6845 */
BYTE    rb_bVModeByte;          /* CRT_MODE_SET */
BYTE    rb_bVPalByte;           /* CRT_PALETTE */


/***    Start of POST data area (0467h)
 */
VPVOID  rb_vpRomInit;           /* IO_ROM_INIT and IO_ROM_SEG */
BBOOL   rb_fInterrupt;          /* INTR_FLAG */


/***    Start of timer data area (046Ch)
 */
ULONG   rb_ulTickCount;         /* TIMER_LOW and TIMER_HIGH */
BYTE    rb_bTickOverflow;       /* TIMER_OFL */


/***    Start of system data area (0471h)
 */
BBOOL   rb_fBreak;              /* BIOS_BREAK */

#define BIOSBREAK_SET           0x80

BOOL   rb_fReset;              /* RESET_FLAG */

#define BIOSRESET_WARMBOOT      0x1234


/***    Start of hard disk data area (0474h)
 */
BYTE    rb_bDStatus;            /* DISK_STATUS1 (fixed disk status byte) */
BYTE    rb_nDTotal;             /* HF_NUM       (count of fixed disk drives) */
BYTE    rb_bDControl;           /* CONTROL_BYTE (control byte drive options) */
BYTE    rb_bDPortOffset;        /* PORT_OFF     (port offset) */


/***    Start of COM and LPT time-out data area (0478h)
 */
BYTE    rb_abLPTtimeout[4];     /* PRINT_TIM_OUT */
BYTE    rb_abCOMtimeout[4];     /* RS232_TIM_OUT */


/***    Start of additional keyboard data area (0480h)
 */
WORD    rb_npKBufStart;         /* BUFFER_START */
WORD    rb_npKBufEnd;           /* BUFFER_END */


/***    Start of EGA/VGA data area (0484h)
 */
BYTE    rb_nVRows;              /* ROWS         (# rows-1) */
WORD    rb_nVCharHeight;        /* POINTS       (height of char in pixels) */
BYTE    rb_bVInfo;              /* INFO */

#define BIOSVINFO_NOEMULATE     0x01    /* (controls cursor emulation) */
#define BIOSVINFO_EGAMONO       0x02
#define BIOSVINFO_WAITRETRC     0x04
#define BIOSVINFO_EGAINACTIVE   0x08
#define BIOSVINFO_MEMMASK       0x60    /* (00=64k, 01=128k, 10=192k, 11=256k) */
#define BIOSVINFO_DONTCLEAR     0x80    /* (don't clear screen on resets) */

BYTE    rb_bVSwitches;          /* INFO_3 */

#define BIOSVSW_SW1OFF          0x01
#define BIOSVSW_SW2OFF          0x02
#define BIOSVSW_SW3OFF          0x04
#define BIOSVSW_SW4OFF          0x08
#define BIOSVSW_FEATUREBITS     0xF0

BYTE    rb_bVFlags;             /* Flags (for VGA and MCGA) */

#define BIOSVFLAGS_VGAACTIVE    0x01
#define BIOSVFLAGS_GRAYSCALES   0x02
#define BIOSVFLAGS_VGAMONO      0x04
#define BIOSVFLAGS_NODEFPALETTE 0x08
#define BIOSVFLAGS_DSPSWITCHING 0x40

#define BIOSVFLAGS_350LINES     0x00
#define BIOSVFLAGS_400LINES     0x10
#define BIOSVFLAGS_200LINES     0x80
#define BIOSVFLAGS_RESRVDLINES  0x90


/***    Reserved area (048Ah)
 */
BYTE    rb_bReserved1[1];       /* reserved */


/***    Start of additional diskette data area (048Bh)
 */
BYTE    rb_bFLastRate;          /* LASTRATE */


/***    Start of additional hard disk data area (048Ch)
 */
BYTE    rb_bDStatusReg;         /* HF_STATUS */
BYTE    rb_bDErrorReg;          /* HF_ERROR */
BBOOL   rb_fDInterrupt;         /* HF_INT_FLAG */
BBOOL   rb_fDCombo;             /* HF_CNTRL     (combo hard disk/diskette card) */


/***    Start of additional diskette data area (0490h)
 */
BYTE    rb_bF0State;            /* DSK_STATE    (drive 0 media state) */
BYTE    rb_bF1State;            /*              (drive 1 media state) */
BYTE    rb_bF0StartState;       /*              (drive 0 operation start state) */
BYTE    rb_bF1StartState;       /*              (drive 1 operation start state) */
BYTE    rb_bF0Cyl;              /* DSK_TRK      (drive 0 current cylinder) */
BYTE    rb_bF1Cyl;              /*              (drive 1 current cylinder) */


/***    Start of additional keyboard data area (0496h)
 */
BYTE    rb_fbKFlag3;            /* KB_FLAG_3 */

#define BIOSKFLAG3_E1PREFIX     0x01    /* supported */
#define BIOSKFLAG3_SECPREFIX    0x02    /* supported */
#define BIOSKFLAG3_RIGHTCTRL    0x04    /* right CTRL down */
#define BIOSKFLAG3_RIGHTALT     0x08    /* right ALT down */
#define BIOSKFLAG3_KBX          0x10    /* supported */
#define BIOSKFLAG3_SETNUMLOCK   0x20    /* supported */
#define BIOSKFLAG3_LASTID       0x40    /* supported */
#define BIOSKFLAG3_READINGID    0x80    /* supported (we never SET it though!) */

BYTE    rb_fbKFlag2;            /* KB_FLAG_2 */

#define BIOSKFLAG2_LEDS         0x07    /* supported */

#ifdef CYRILLIC
#define BIOSKFLAG2_CYR          0x08    // 0 = Latin, 1 = Cyrillic
#endif

#define BIOSKFLAG2_ACK          0x10    /* supported */
#define BIOSKFLAG2_RESEND       0x20    /* supported */
#define BIOSKFLAG2_LEDUPDATE    0x40    /* always clear */
#define BIOSKFLAG2_XMITERROR    0x80    /* always clear */


/***    Start of real-time CMOS clock data area (0498h)
 */
VPVOID  rb_vpWaitFlag;          /* USER_FLAG and USER_FLAG_SEG */
ULONG   rb_ulWaitFlag;          /* RTC_LOW and RTC_HIGH */
BBOOL   rb_fWaitActive;         /* RTC_WAIT_FLAG */

BYTE    rb_bReserved2[7];        /* this should "ORG" us to 04A8h */


/***    Start of additional EGA/VGA data area (04A8h)
 */
VPVOID  rb_vpVideoPtrTable;

/***    Format of VideoPtrTable:
 *
 *       VPVOID[0]      Parameter Table Pointer
 *                      Initialized to BIOS Video Parameter Table.
 *                      This value MUST exist.  For a VGA, the table is:
 *
 *                      BYTE    Value for CRT_COLS
 *                      BYTE    Value for ROWS
 *                      BYTE    Value for POINTS
 *                      WORD    Value for CRT_LEN
 *                      BYTE*4  Values for SEQ registers 1-4
 *                      BYTE    Value for Miscellaneous Output register
 *                      BYTE*25 Values for CRT registers 0-18h
 *                      BYTE*20 Values for ATC registers 0-13h
 *                      BYTE*9  Values for GDC registers 0-8
 *
 *       VPVOID[1]      Parameter Save Area Pointer
 *                      Initialized to 0000:0000, this value is optional.
 *                      When non-zero, this pointer will be used as pointer
 *                      to a RAM area where certain dynamic values are to
 *                      be saved. When in EGA operation this RAM area will
 *                      hold the 16 EGA palette register values plus
 *                      the overscan value in bytes 00h-10h respectively.
 *                      At least 256 bytes must be allocated for this area.
 *
 *       VPVOID[2]      Alpha Character Set Override Pointer
 *                      Initialized to 0000:0000, this value is optional.
 *                      When non-zero, this pointer is used as a pointer
 *                      to a tables described as follows:
 *
 *                      BYTE    Bytes/character
 *
 *                      BYTE    Block to load, should be zero for normal
 *                              operation
 *
 *                      WORD    Count to store, should be 256 for normal
 *                              operation
 *
 *                      WORD    Character offset, should be zero for normal
 *                              operation
 *
 *                      VPVOID  Pointer to a font table
 *
 *                      BYTE    Displayable rows
 *                              if 'FF' the maximum calculated value will be
 *                              used, else this value will be used
 *
 *                      BYTE    Consecutive bytes of mode values for which
 *                              this font description is to be used.
 *                              The end of this stream is indicated by a
 *                              byte code of 'FF'
 *
 *       VPVOID[3]      Graphics Character Set Override Pointer
 *                      Initialized to 0000:0000, this value is optional.
 *                      When non-zero, this pointer is used as a pointer
 *                      to a tables described as follows:
 *
 *                      BYTE    Displayable rows
 *
 *                      WORD    Bytes per character
 *
 *                      VPVOID  Pointer to a font table
 *
 *                      BYTE    Consecutive bytes of mode values for which
 *                              this font description is to be used.
 *                              The end of this stream is indicated by a
 *                              byte code of 'FF'
 *
 *       VPVOID[4]      Address of Secondary Video Pointer Table (VGA ONLY):
 *
 *                      WORD    Length of this table, in bytes
 *                      VPVOID  Pointer to Display Combination Code Table
 *                      VPVOID  Pointer to second Alpha Character Set Override
 *                      VPVOID  Pointer to User Palette Profile Table
 *                      VPVOID  Reserved
 *                      VPVOID  Reserved
 *                      VPVOID  Reserved
 *
 *       VPVOID[5]      Reserved and set to 0000:0000.
 *
 *       VPVOID[6]      Reserved and set to 0000:0000.
 */

BYTE    rb_bReserved3[0x54];    /* this should "ORG" us to 0500h */

/***    Extra data area (0500h)
 */
BYTE    rb_bPrtScrStatus;       /* STATUS_BYTE */
} ROMDATA;

typedef ROMDATA *PROMDATA;


/* c-begin */
typedef PROMDATA PVDMBASE;
#define VDMBase (*(PVDMBASE)0)
/* c-end */

/* masm-begin */
/* VDMBase   equ <ds:[0]> */
/* .errnz  (size romdata_s NE 0501h) */
/* masm-end */

#pragma pack()

#endif /* __VDMBIOS__ */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
