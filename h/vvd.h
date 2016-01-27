/***    VVD.H
 *
 *
 *      MVDM Virtual Video Exported Definitions
 *      Copyright (c) 1993 IBM Corporation
 *
 *      For include file hierarchy, see MVDM.H
 *
 *      Options:
 *          INCL_VIDEO      include all video definitions
 *          INCL_VIDEOHW    include hardware-dependent definitions
 *          INCL_VIDEOVDD   include VDD-support definitions
 *
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

#ifndef __VVD__
#define __VVD__


#ifndef INCL_NONE               /* include everything by default */
#define INCL_VIDEO
#endif

#ifdef  INCL_VIDEO
#define INCL_VIDEOHW
#define INCL_VIDEOPTR
#define INCL_VIDEOSYSREQ
#define INCL_VIDEODEVREQ
#define INCL_VIDEOINTERFACES
#endif

#ifdef  INCL_DBCSVDM
#define ATLAS
#define GALE
#endif  //INCL_DBCSVDM
#ifdef  GALE
#define ATLAS_GALE
#endif  //GALE
#ifdef  ATLAS
#define ATLAS_GALE
#endif  //ATLAS

#ifdef INCL_VIDEOPTR
#include <vmd.h>
#endif


#ifdef  INCL_VIDEOHW

/***EK+ Extended (non-BIOS) EGA INT 10h functions
 */

#define INT10_EGAREADONE    0xF0        /* Read one EGA reg */
#define INT10_EGAWRITEONE   0xF1        /* Write one EGA reg */
#define INT10_EGAREADRANGE  0xF2        /* Read multiple EGA regs */
#define INT10_EGAWRITERANGE 0xF3        /* Write multiple EGA regs */
#define INT10_EGAREADSET    0xF4        /* Read set of EGA regs */
#define INT10_EGAWRITESET   0xF5        /* Write set of EGA regs */
#define INT10_EGAREVERTDEF  0xF6        /* Revert to default */
#define INT10_EGADEFINEDEF  0xF7        /* Define default */
#define INT10_EGAQUERYDEF   0xF8        /* Query default */
#define INT10_EGACONTEXT    0xF9        /* Get context information */
#define INT10_EGAINQUIRE    0xFA        /* Get EGA driver version */

#define INT10_EGAFIRST      INT10_EGAREADONE
#define INT10_EGALAST       INT10_EGAINQUIRE


/* Default physical memory locations
 */

#define PLANE0              0
#define PLANE1              1
#define PLANE2              2
#define PLANE3              3

#define MONOMEM_START       0xB0000
#define MONOMEM_LEN         (4*1024)
#ifdef  PAGESIZE
#define MONOMEM_PAGES       (MONOMEM_LEN/PAGESIZE)
#endif

#define CGAMEM_START        0xB8000
#define CGAMEM_LEN          (16*1024)
#ifdef  PAGESIZE
#define CGAMEM_PAGES        (CGAMEM_LEN/PAGESIZE)
#endif

#define EGAVGAMEM_START     0xA0000
#define EGAVGAMEM_LEN       (128*1024)
#ifdef  PAGESIZE
#define EGAVGAMEM_PAGES     (EGAVGAMEM_LEN/PAGESIZE)
#endif


/*  Hardware ports
 *
 *  All are single-register ports unless otherwise specified;
 *  All single-register ports are W/O unless VGA or otherwise specified
 */

#define PORT_ATIVGA_INDX    0x1CE       /* used by ATI VGA Wonder */
#define PORT_ATIVGA_DATA    0x1CF       /* used by ATI VGA Wonder */
#define PORT_MONOCRTINDX    0x3B4
#define PORT_MONOCRTDATA    0x3B5       /* multi-register port */
#define PORT_MONOMODE       0x3B8       /* usually shadowed at uBiosVModeByte */
#define PORT_MONOLPENSET    0x3B9
#define PORT_MONOSTATUS1    0x3BA       /* (R/O) */
#define PORT_MONOFEATURE    0x3BA       /* (W/O) */
#define PORT_MONOLPENRESET  0x3BB
#define PORT_ATC0           0x3C0
#define PORT_ATC1           0x3C1       /* on VGA, distinct from ATC0 and R/O */
#define PORT_STATUS0        0x3C2       /* (R/O) */
#define PORT_MISCOUT        0x3C2       /* reset sequencer before writing */
#define PORT_VGAENABLE      0x3C3
#define PORT_SEQINDX        0x3C4
#define PORT_SEQDATA        0x3C5       /* multi-register port */
#define PORT_COMPAQ_CTRL    0x3C6
#define PORT_VGADACMASK     0x3C6       /* initialized to 0FFh, leave alone */
#define PORT_VGADACREAD     0x3C7       /* State (R), Index to read (W) */
#define PORT_VGADACWRITE    0x3C8       /* Index to write (R/W) */
#define PORT_VGADACDATA     0x3C9       /* multi-register port */
#define PORT_GDCPOS2        0x3CA       /* on VGA, R/O (returns FEATURE) */
#define PORT_ATIEGA_INDX    0x3CB       /* used by ATI EGA Wonder */
#define PORT_GDCPOS1        0x3CC       /* on VGA, R/O (returns MISCOUT) */
#define PORT_ATIEGA_DATA    0x3CD       /* used by ATI EGA Wonder */
#define PORT_GDCINDX        0x3CE
#define PORT_GDCDATA        0x3CF       /* multi-register port */
#define PORT_COLRCRTINDX    0x3D4
#define PORT_COLRCRTDATA    0x3D5       /* multi-register port */
#define PORT_CGAMODE        0x3D8       /* usually shadowed at uBiosVModeByte */
#define PORT_CGACOLOR       0x3D9       /* usually shadowed at uBiosVColorByte */
#define PORT_COLRSTATUS1    0x3DA       /* (R/O) */
#define PORT_COLRFEATURE    0x3DA       /* (W/O) */
#define PORT_CGALPENRESET   0x3DB
#define PORT_CGALPENSET     0x3DC
#define PORT_COMPAQ_ENV     0x7C6       /* (R/O) */
#define PORT_COMPAQ_DSP     0xBC6       /* (R/O) */
#define PORT_IBMP70_INDX    0xD00       /* special port for plasma display? */
#define PORT_IBMP70_DATA    0xD01       /* special port for plasma display? */
#define PORT_COMPAQ_MODE    0xFC6       /* (R/O) */

/* 8514/A Display Controller registers (word registers unless noted otherwise)
 */
#define A8514_DSPSTAT       0x02E8      /* (R/O) */
#define   DSPSTAT_PALSTAT       0x0001
#define   DSPSTAT_VERTRTRC      0x0002
#define   DSPSTAT_HORZRTRC      0x0004
#define A8514_HORZTOTAL     0x02E8      /* (W/O) */
#define A8514_DACMASK       0x02EA      /* pixel planes displayed   (R/W-byte) */
#define A8514_DACREADINDEX  0x02EB      /* DAC data reg/mode select (R/W-byte) */
#define A8514_DACWRITEINDEX 0x02EC      /* DAC data reg/mode select (R/W-byte) */
#define A8514_DACDATA       0x02ED      /* DAC data register        (R/W-byte) */
#define A8514_HORZDSP       0x06E8      /* (W/O) */
#define A8514_HORZSSTART    0x0AE8      /* (W/O) */
#define A8514_HORZSWIDTH    0x0EE8      /* (W/O) */
#define A8514_VERTTOTAL     0x12E8      /* (W/O) */
#define A8514_VERTDSP       0x16E8      /* (W/O) */
#define A8514_VERTSSTART    0x1AE8      /* (W/O) */
#define A8514_VERTSWIDTH    0x1EE8      /* (W/O) */
#define A8514_DSPCTRL       0x22E8      /* (W/O) */
#define   DSPCTRL_ODDEVEN       0x0001
#define   DSPCTRL_CAS2          0x0000  /* this implies 4-plane setting */
#define   DSPCTRL_CAS4          0x0002  /* this implies 8-plane setting */
#define   DSPCTRL_CAS6          0x0004
#define   DSPCTRL_CAS8          0x0006
#define   DSPCTRL_CASMASK       0x0006
#define   DSPCTRL_SCANDBL       0x0008
#define   DSPCTRL_INTERLACE     0x0010
#define   DSPCTRL_DSPENABLE     0x0020
#define   DSPCTRL_DSPDISABLE    0x0040
#define   DSPCTRL_DSPCTRLMASK   0x0060

/* 8514/A Configuration/Status registers (word registers unless noted otherwise)
 */
#define A8514_SUBSYSCTRL    0x42E8      /* (W/O) */
#define   SUBCTRL_RESETVERTF    0x0001
#define   SUBCTRL_RESETDTR      0x0002
#define   SUBCTRL_RESETQFULL    0x0004
#define   SUBCTRL_RESETGPIDLE   0x0008
#define   SUBCTRL_INTONVERTF    0x0100
#define   SUBCTRL_INTONDTR      0x0200
#define   SUBCTRL_INTONQFULL    0x0400
#define   SUBCTRL_INTONGPIDLE   0x0800
#define   SUBCTRL_PIXENABLE     0x1000
#define   SUBCTRL_PIXDISABLE    0x2000
#define   SUBCTRL_PIXCTRLMASK   0x3000
#define   SUBCTRL_SEQENABLE     0x4000
#define   SUBCTRL_SEQDISABLE    0x8000
#define   SUBCTRL_SEQCTRLMASK   0xC000
#define A8514_SUBSYSSTATUS  0x42E8      /* (R/O) */
#define   SUBSTATUS_VERTF       0x0001
#define   SUBSTATUS_DTR         0x0002
#define   SUBSTATUS_QFULL       0x0004
#define   SUBSTATUS_GPIDLE      0x0008
#define   SUBSTATUS_MONITORID   0x0070
#define   SUBSTATUS_8503        0x0050
#define   SUBSTATUS_8512_3      0x0060
#define   SUBSTATUS_8514        0x0020
#define   SUBSTATUS_MONOCHROME  0x0010
#define   SUBSTATUS_NOT8514     0x0040
#define   SUBSTATUS_MEMORYOPT   0x0080
#define A8514_ROMPAGESELECT 0x46E8      /* (W/O) */
#define A8514_ADVCTRL       0x4AE8      /* (W/O) */
#define   ADVCTRL_VGADISABLE    0x0001
#define   ADVCTRL_OUTPUTPIN     0x0002
#define   ADVCTRL_DOTCLOCK      0x0004  /* 0 implies 640x480 mode */

/* 8514/A Graphics Processor registers (word registers unless noted otherwise)
 */
#define A8514_COMMAND       0x9AE8      /* (W/O) */
#define   COMMAND_WRITE         0x0001  /* 0 implies read function */
#define   COMMAND_PLANAR        0x0002  /* 0 implies pixel-access mode */
#define   COMMAND_LASTPIXNULL   0x0004  /* 0 implies last pixel will be drawn */
#define   COMMAND_STROKEALG     0x0008  /* 0 implies H/W line-drawing algorithm */
#define   COMMAND_DRAW          0x0010  /* 0 implies move current position only */
#define   COMMAND_INCX          0x0020  /* 0 implies decrement X */
#define   COMMAND_MAJORY        0x0040  /* 0 implies X is major axis */
#define   COMMAND_INCY          0x0080  /* 0 implies decrement Y */
#define   COMMAND_PCDATAXFER    0x0100  /* 0 implies no PC intervention */
#define   COMMAND_PCDATA16BIT   0x0200  /* 0 implies base pixel count per access */
#define   COMMAND_PIXEL16BIT    0x0400  /* 0 implies 8-bit pixel transfers */
#define   COMMAND_SWAPBYTES     0x1000  /* 0 implies even pixels from low bytes */
#define   COMMAND_FUN_NONE      0x0000
#define   COMMAND_FUN_LINE      0x2000
#define   COMMAND_FUN_FILLX     0x4000
#define   COMMAND_FUN_FILLY     0x6000
#define   COMMAND_FUN_FILL      0x8000
#define   COMMAND_FUN_AXIALLINE 0xA000
#define   COMMAND_FUN_COPY      0xC000
#define   COMMAND_FUNMASK       0xF000
#define A8514_STATUS        0x9AE8      /* (R/O) */
#define   STATUS_QUEUE1_INUSE   0x0001
#define   STATUS_QUEUE2_INUSE   0x0002
#define   STATUS_QUEUE3_INUSE   0x0004
#define   STATUS_QUEUE4_INUSE   0x0008
#define   STATUS_QUEUE5_INUSE   0x0010
#define   STATUS_QUEUE6_INUSE   0x0020
#define   STATUS_QUEUE7_INUSE   0x0040
#define   STATUS_QUEUE8_INUSE   0x0080
#define   STATUS_DATA_AVAILABLE 0x0100
#define   STATUS_COMMAND_ACTIVE 0x0200

/* 8514/A Coordinate registers (word registers unless noted otherwise)
 */
#define A8514_CURRENTY      0x82E8      /* (R/W) */
#define A8514_CURRENTX      0x86E8      /* (R/W) */
#define A8514_DESTY_AXSTP   0x8AE8      /* (W/O) */
#define A8514_DESTX_DIASTP  0x8EE8      /* (W/O) */
#define A8514_ERRORTERM     0x92E8      /* suitable for presence test (R/W) */
#define A8514_MAJORAXISCNT  0x96E8      /* (W/O) */

/* 8514/A Multi-function control registers (word registers unless noted otherwise)
 */
#define A8514_MULTIFUNCTION 0xBEE8      /* (W/O) */
#define   MFINDX_MINORAXISCNT   0x0000
#define   MFINDX_TOPSCISSORS    0x1000
#define   MFINDX_LEFTSCISSORS   0x2000
#define   MFINDX_BOTTOMSCISSORS 0x3000
#define   MFINDX_RIGHTSCISSORS  0x4000
#define   MFINDX_MEMORYCTRL     0x5000
#define     MEMCTRL_WRITE4          0x0000
#define     MEMCTRL_WRITE5          0x0001
#define     MEMCTRL_WRITE8          0x0002
#define     MEMCTRL_WRITE10         0x0003
#define     MEMCTRL_WRITEMASK       0x0003
#define     MEMCTRL_CAS2            0x0000  /* this implies 4-plane setting */
#define     MEMCTRL_CAS4            0x0004  /* this implies 8-plane setting */
#define     MEMCTRL_CAS6            0x0008
#define     MEMCTRL_CAS8            0x000C
#define     MEMCTRL_CASMASK         0x000C
#define     MEMCTRL_CASSWAP         0x0010
#define   MFINDX_PATTERNLOW     0x8000
#define   MFINDX_PATTERNHIGH    0x9000
#define   MFINDX_PIXELCTRL      0xA000
#define     PIXELCTRL_5PIXELROTATE  0x0001
#define     PIXELCTRL_MASKSELECT    0x0002
#define     PIXELCTRL_PLANAR        0x0004
#define     PIXELCTRL_COLORFALSE    0x0000
#define     PIXELCTRL_COLORTRUE     0x0008
#define     PIXELCTRL_COLORGE       0x0010
#define     PIXELCTRL_COLORLT       0x0018
#define     PIXELCTRL_COLORNE       0x0020
#define     PIXELCTRL_COLOREQ       0x0028
#define     PIXELCTRL_COLORLE       0x0030
#define     PIXELCTRL_COLORGT       0x0038
#define     PIXELCTRL_MIXFGND       0x0000
#define     PIXELCTRL_FIXEDPATTERN  0x0040
#define     PIXELCTRL_PLANARWRITE   0x0080
#define     PIXELCTRL_SINGLEPLANE   0x00C0
#define   MFINDX_INDXMASK       0xF000
#define   MFINDX_DATAMASK       0x0FFF
#define   MFINDX_INDXSHIFT      12
#define   MFINDX_INDXTOTAL      16

/* 8514/A Pixel processing registers (word registers unless noted otherwise)
 */
#define A8514_SHORTSTROKE   0x9EE8      /* (W/O) */
#define A8514_BGNDCOLOR     0xA2E8      /* (W/O) */
#define A8514_FGNDCOLOR     0xA6E8      /* (W/O) */
#define A8514_WRITEMASK     0xAAE8      /* (W/O) */
#define A8514_READMASK      0xAEE8      /* (W/O) */
#define A8514_COLORCOMPARE  0xB2E8      /* (W/O) */
#define A8514_BGNDMIX       0xB6E8      /* (W/O) */
#define A8514_FGNDMIX       0xBAE8      /* (W/O) */
#define     FGNDMIX_SOURCE      0x0007
#define     FGNDMIX_BGNDCLR     0x0000
#define     FGNDMIX_FGNDCLR     0x0020
#define     FGNDMIX_PIXELDATA   0x0040
#define     FGNDMIX_PLANECOPY   0x0060
#define A8514_PIXELTRANSFER 0xE2E8      /* (R/W) */

/* XGA I/O Registers (all byte registers) */

#define  AXGA_OPERATING_MODE              0  /* private indexes into */
#define  AXGA_APERTURE_CONTROL            1  /*   awregXGAIOReg[].  See */
#define  AXGA_RESERVED1                   2  /*   VVXGAREGS structure */
#define  AXGA_RESERVED2                   3  /*   definition in VVXGAP.H */
#define  AXGA_INTERRUPT_ENABLE            4  /*   for detail */
#define  AXGA_INTERRUPT_STATUS            5
#define  AXGA_VIRTUAL_MEMORY_CONTROLLER   6
#define  AXGA_VIRTUAL_MEMORY_INT_STATUS   7
#define  AXGA_APERTURE_INDEX              8
#define  AXGA_MEMORY_ACCESS_MODE          9
#define  AXGA_INDEX_SELECT                10
#define  AXGA_INDEX_DATA_B                11
#define  AXGA_INDEX_DATA_C                12
#define  AXGA_INDEX_DATA_D                13
#define  AXGA_INDEX_DATA_E                14
#define  AXGA_INDEX_DATA_F                15
#define  AXGA_TOTAL                       16

/* XGA Index Registers (all byte register) */
#define  AINDX_RESERVED0               0x0000   /* (R/W) */
#define  AINDX_RESERVED1               0x0001   /* (R/O) */
#define  AINDX_RESERVED2               0x0002   /* (R/O) */
#define  AINDX_RESERVED3               0x0003   /* (R/O) */
#define  AINDX_AUTOCONFIG              0x0004   /* (R/O) */
#define  AINDX_RESERVED5               0x0005   /* (R/O) */
#define  AINDX_RESERVED6               0x0006   /* (R/O) */
#define  AINDX_RESERVED7               0x0007   /* (R/O) */
#define  AINDX_RESERVED8               0x0008   /* (R/O) */
#define  AINDX_RESERVED9               0x0009   /* (R/O) */
#define  AINDX_RESERVEDa               0x000a   /* (R/O) */
#define  AINDX_RESERVEDb               0x000b   /* (R/O) */
#define  AINDX_COPROCSAVERESTOREA      0x000c   /* (R/O) */
#define  AINDX_COPROCSAVERESTOREB      0x000d   /* (R/O) */
#define  AINDX_RESERVEDe               0x000e   /* (R/O) */
#define  AINDX_RESERVEDf               0x000f   /* (R/O) */
#define  AINDX_HORZTOTALLO             0x0010   /* (R/W) */
#define  AINDX_HORZTOTALHI             0x0011   /* (R/W) */
#define  AINDX_HORZDISPENDLO           0x0012   /* (R/W) */
#define  AINDX_HORZDISPENDHI           0x0013   /* (R/W) */
#define  AINDX_HORZBLANKINGSTARTLO     0x0014   /* (R/W) */
#define  AINDX_HORZBLANKINGSTARTHI     0x0015   /* (R/W) */
#define  AINDX_HORZBLANKINGENDLO       0x0016   /* (R/W) */
#define  AINDX_HORZBLANKINGENDHI       0x0017   /* (R/W) */
#define  AINDX_HORZSYNCPULSESTARTLO    0x0018   /* (R/W) */
#define  AINDX_HORZSYNCPULSESTARTHI    0x0019   /* (R/W) */
#define  AINDX_HORZSYNCPULSEENDLO      0x001a   /* (R/W)    132 Column Reg */
#define  AINDX_HORZSYNCPULSEENDHI      0x001b   /* (R/W)    132 Column Reg */
#define  AINDX_HORZSYNCPOSITION1       0x001c   /* (W/O) */
#define  AINDX_RESERVED1d              0x001d   /* (R/O) */
#define  AINDX_HORZSYNCPOSITION2       0x001e   /* (W/O) */
#define  AINDX_RESERVED1f              0x001f   /* (R/O) */
#define  AINDX_VERTTOTALLO             0x0020   /* (R/W) */
#define  AINDX_VERTTOTALHI             0x0021   /* (R/W) */
#define  AINDX_VERTDISPENDLO           0x0022   /* (R/W) */
#define  AINDX_VERTDISPENDHI           0x0023   /* (R/W) */
#define  AINDX_VERTBLANKINGSTARTLO     0x0024   /* (R/W) */
#define  AINDX_VERTBLANKINGSTARTHI     0x0025   /* (R/W) */
#define  AINDX_VERTBLANKINGENDLO       0x0026   /* (R/W) */
#define  AINDX_VERTBLANKINGENDHI       0x0027   /* (R/W) */
#define  AINDX_VERTSYNCPULSESTARTLO    0x0028   /* (R/W) */
#define  AINDX_VERTSYNCPULSESTARTHI    0x0029   /* (R/W) */
#define  AINDX_VERTSYNCPULSEEND        0x002a   /* (R/W)    132 Column Reg */
#define  AINDX_RESERVED2b              0x002b   /* (R/O) */
#define  AINDX_VERTLINECOMPLO          0x002c   /* (R/W) */
#define  AINDX_VERTLINECOMPHI          0x002d   /* (R/W) */
#define  AINDX_RESERVED2e              0x002e   /* (R/O) */
#define  AINDX_RESERVED2f              0x002f   /* (R/O) */
#define  AINDX_SPRITEHORZSTARTLO       0x0030   /* (R/W) */
#define  AINDX_SPRITEHORZSTARTHI       0x0031   /* (R/W) */
#define  AINDX_SPRITEHORZPRESET        0x0032   /* (R/W) */
#define  AINDX_SPRITEVERTSTARTLO       0x0033   /* (R/W) */
#define  AINDX_SPRITEVERTSTARTHI       0x0034   /* (R/W) */
#define  AINDX_SPRITEVERTPRESET        0x0035   /* (R/W) */
#define  AINDX_SPRITECONTROL           0x0036   /* (R/W) */
#define  AINDX_RESERVED37              0x0037   /* (R/O) */
#define  AINDX_SPRITECOLOR0RED         0x0038   /* (R/W) */
#define  AINDX_SPRITECOLOR0GREEN       0x0039   /* (R/W) */
#define  AINDX_SPRITECOLOR0BLUE        0x003a   /* (R/W) */
#define  AINDX_SPRITECOLOR1RED         0x003b   /* (R/W) */
#define  AINDX_SPRITECOLOR1GREEN       0x003c   /* (R/W) */
#define  AINDX_SPRITECOLOR1BLUE        0x003d   /* (R/W) */
#define  AINDX_RESERVED3e              0x003e   /* (R/O) */
#define  AINDX_RESERVED3f              0x003f   /* (R/O) */
#define  AINDX_DISPPIXELMAPOFFSETLO    0x0040   /* (R/W) */
#define  AINDX_DISPPIXELMAPOFFSETMI    0x0041   /* (R/W) */
#define  AINDX_DISPPIXELMAPOFFSETHI    0x0042   /* (R/W) */
#define  AINDX_DISPPIXELMAPWIDTHLO     0x0043   /* (R/W) */
#define  AINDX_DISPPIXELMAPWIDTHHI     0x0044   /* (R/W) */
#define  AINDX_RESERVED45              0x0045   /* (R/O) */
#define  AINDX_RESERVED46              0x0046   /* (R/O) */
#define  AINDX_RESERVED47              0x0047   /* (R/O) */
#define  AINDX_RESERVED48              0x0048   /* (R/O) */
#define  AINDX_RESERVED49              0x0049   /* (R/O) */
#define  AINDX_RESERVED4a              0x004a   /* (R/O) */
#define  AINDX_RESERVED4b              0x004b   /* (R/O) */
#define  AINDX_RESERVED4c              0x004c   /* (R/O) */
#define  AINDX_RESERVED4d              0x004d   /* (R/O) */
#define  AINDX_RESERVED4e              0x004e   /* (R/O) */
#define  AINDX_RESERVED4f              0x004f   /* (R/O) */
#define  AINDX_DISPCONTROL1            0x0050   /* (R/W)    132 Column Reg */
#define  AINDX_DISPCONTROL2            0x0051   /* (R/W) */
#define  AINDX_DISPIDCOMPARATOR        0x0052   /* (R/O) */
#define  AINDX_MYSTERYREG              0x0053   /* (R/O)    Referenced by AFI */
#define  AINDX_CLOCKFREQSELECT         0x0054   /* (R/W)    132 Column Reg */
#define  AINDX_BORDERCOLOR             0x0055   /* (R/W) */
#define  AINDX_RESERVED56              0x0056   /* (R/O) */
#define  AINDX_RESERVED57              0x0057   /* (R/O) */
#define  AINDX_RESERVED58              0x0058   /* (R/O) */
#define  AINDX_RESERVED59              0x0059   /* (R/O) */
#define  AINDX_RESERVED5a              0x005a   /* (R/O) */
#define  AINDX_RESERVED5b              0x005b   /* (R/O) */
#define  AINDX_RESERVED5c              0x005c   /* (R/O) */
#define  AINDX_RESERVED5d              0x005d   /* (R/O) */
#define  AINDX_RESERVED5e              0x005e   /* (R/O) */
#define  AINDX_RESERVED5f              0x005f   /* (R/O) */
#define  AINDX_SPRITEPALETTEIDXLO      0x0060   /* (R/W) */
#define  AINDX_SPRITEIDXHI             0x0061   /* (R/W) */
#define  AINDX_SPRITEPALETTEIDXLOPREF  0x0062   /* (R/W) */
#define  AINDX_SPRITEIDXHIPREFETCH     0x0063   /* (R/W) */
#define  AINDX_PALETTEMASK             0x0064   /* (R/W) */
#define  AINDX_PALETTEDATA             0x0065   /* (R/W) */
#define  AINDX_PALETTESEQUENCE         0x0066   /* (R/W) */
#define  AINDX_PALETTEREDPREFETCH      0x0067   /* (R/W) */
#define  AINDX_PALETTEGREENPREFETCH    0x0068   /* (R/W) */
#define  AINDX_PALETTEBLUEPREFETCH     0x0069   /* (R/W) */
#define  AINDX_SPRITEDATA              0x006a   /* (R/W) */
#define  AINDX_SPRITEPREFETCH          0x006b   /* (R/W) */
#define  AINDX_RESERVED6c              0x006c   /* (R/O) */
#define  AINDX_RESERVED6d              0x006d   /* (R/O) */
#define  AINDX_RESERVED6e              0x006e   /* (R/O) */
#define  AINDX_RESERVED6f              0x006f   /* (R/O) */
#define  AINDX_EXTCLOCKSELECT          0x0070   /* (R/W)    132 Column Reg */

#define  IRINDX_INDXTOTAL              0x0071
#define  IRINDX_INDXMASK               0x00FF
#define  IRINDX_DATAMASK               0xFF00
#define  IRINDX_DATASHIFT              8


/*  Registers indexed via multi-register ports
 *
 *  All are W/O unless VGA or otherwise specified
 */

#define REG_ATCPALETTE      0x00        /* 00h-0Fh are palette registers */
#define REG_ATCMODECTRL     0x10        /* Compaq VGA: wait for vert retrace */
#define REG_ATCOVERSCAN     0x11
#define REG_ATCPLANEENABLE  0x12
#define REG_ATCPELPANNING   0x13
#define REG_ATCVGAREG       0x14
#define TOTAL_EGA_ATCREGS   0x14
#define TOTAL_IBM_ATCREGS   0x15
#define TOTAL_TSENG_ATCREGS 0x18

#define REG_SEQRESET        0x00
#define REG_SEQCLKMODE      0x01        /* reset sequencer before writing */
#define REG_SEQMAPMASK      0x02
#define REG_SEQCHARMAPSEL   0x03
#define REG_SEQMEMMODE      0x04        /* reset sequencer before writing */
#define REG_SEQUNKNOWN_IBM1 0x07
#define TOTAL_IBM_SEQREGS   0x05
#define TOTAL_TSENG_SEQREGS 0x08

#define REG_GDCSR           0x00
#define REG_GDCESR          0x01
#define REG_GDCCLRCMP       0x02
#define REG_GDCROTATE       0x03
#define REG_GDCREADMAP      0x04
#define REG_GDCMODE         0x05
#define REG_GDCMISC         0x06        /* reset sequencer before writing */
#define REG_GDCDONTCARE     0x07
#define REG_GDCBITMASK      0x08
#define TOTAL_IBM_GDCREGS   0x09
#define TOTAL_TSENG_GDCREGS 0x0F

#define REG_CRTHORZTOTAL    0x00
#define REG_CRTHORZDSPEND   0x01
#define REG_CRTHORZSTBLNK   0x02
#define REG_CRTHORZENDBLNK  0x03
#define REG_CRTHORZSTRTRC   0x04
#define REG_CRTHORZENDRTRC  0x05
#define REG_CRTVERTTOTAL    0x06
#define REG_CRTOVERFLOW     0x07
#define REG_CRTPRESETROW    0x08
#define REG_CRTMAXSCAN      0x09
#define REG_CRTCURSTSCAN    0x0A
#define REG_CRTCURENDSCAN   0x0B
#define REG_CRTSTARTADDRHI  0x0C        /* R/W (W/O on CGA???) */
#define REG_CRTSTARTADDRLO  0x0D        /* R/W (W/O on CGA???) */
#define REG_CRTCURLOCHI     0x0E        /* R/W */
#define REG_CRTCURLOCLO     0x0F        /* R/W */
#define REG_CRTVERTSTRTRC   0x10
#define REG_CRTLPENHI       0x10        /* R/O (dual-purpose register index) */
#define REG_CRTVERTENDRTRC  0x11
#define REG_CRTLPENLO       0x11        /* R/O (dual-purpose register index) */
#define REG_CRTVERTDSPEND   0x12
#define REG_TSENG_CRTLPEN2  0x12        /* R/O (dual-purpose register index) */
#define REG_CRTOFFSET       0x13
#define REG_CRTUNDLINELOC   0x14
#define REG_CRTVERTSTBLNK   0x15
#define REG_CRTVERTENDBLNK  0x16
#define REG_CRTMODECTRL     0x17
#define REG_CRTLINECMP      0x18
#define REG_CRTUNKNOWN_IBM1 0x3F
#define TOTAL_CGA_CRTREGS   0x13
#define TOTAL_IBM_CRTREGS   0x19
#define TOTAL_TSENG_CRTREGS 0x26

#define TOTAL_DACREGS       256         /* on VGA, there are 256 color registers */


/* Individual bits for registers
 */

#define ATCPAL_ADDRMASK     0x01F
#define ATCPAL_ENABLE       0x020

#define SEQRESET_ASYNC      0x001
#define SEQRESET_SYNC       0x002

#define SEQCLK_8DOTS        0x001       /* reset sequencer before writing */
#define SEQCLK_CYCLES       0x002
#define SEQCLK_SHIFTLOAD    0x004
#define SEQCLK_DOTCLOCK     0x008       /* reset sequencer before writing */
#define SEQCLK_VGASHIFT4    0x010
#define SEQCLK_VGASCREENOFF 0x020

#define SEQMAP_PLANE0       0x001
#define SEQMAP_PLANE1       0x002
#define SEQMAP_PLANE2       0x004
#define SEQMAP_PLANE3       0x008
#define SEQMAP_ALL          (SEQMAP_PLANE0|SEQMAP_PLANE1| \
                             SEQMAP_PLANE2|SEQMAP_PLANE3)

#define SEQMEM_ALPHA        0x001
#define SEQMEM_EXTENDED     0x002
#define SEQMEM_ODDEVENSAME  0x004       /* otherwise, even->0/2, odd->1/3 */
#define SEQMEM_CHAIN4       0x008       /* low 2 bits select plane */
#define SEQMEM_ALL          (SEQMEM_ALPHA|SEQMEM_EXTENDED|SEQMEM_ODDEVENSAME|SEQMEM_CHAIN4)

#define GDCSR_PLANE0        0x001
#define GDCSR_PLANE1        0x002
#define GDCSR_PLANE2        0x004
#define GDCSR_PLANE3        0x008
#define GDCSR_ALL           (GDCSR_PLANE0|GDCSR_PLANE1| \
                             GDCSR_PLANE2|GDCSR_PLANE3)
#define GDCESR_ALL          GDCSR_ALL

#define GDCCLRCMP_ALL       0x00F

#define GDCROTATE_COUNT     0x007
#define GDCROTATE_AND       0x008
#define GDCROTATE_OR        0x010
#define GDCROTATE_XOR       0x018
#define GDCROTATE_NOFUNC    0x000
#define GDCROTATE_ALL       (GDCROTATE_COUNT|GDCROTATE_AND|GDCROTATE_OR| \
                             GDCROTATE_XOR)

#define GDCREADMAP_ALL      (PLANE0|PLANE1|PLANE2|PLANE3)

#define GDCMODE_WRITE0      0x000
#define GDCMODE_WRITE1      0x001
#define GDCMODE_WRITE2      0x002
#define GDCMODE_WRITE3      0x003       /* only on a VGA */
#define GDCMODE_TEST        0x004
#define GDCMODE_READ0       0x000
#define GDCMODE_READ1       0x008
#define GDCMODE_ODDEVENDIFF 0x010
#define GDCMODE_ODDEVENBITS 0x020
#define GDCMODE_256COLOR    0x040       /* only on a VGA */
#define GDCMODE_ALL         (GDCMODE_WRITE3|GDCMODE_TEST|GDCMODE_READ1| \
                             GDCMODE_ODDEVENDIFF|GDCMODE_ODDEVENBITS|GDCMODE_256COLOR)

#define GDCMISC_NOCHARGEN   0x001
#define GDCMISC_ODDTOEVEN   0x002
#define GDCMISC_64K_A0000   0x004
#define GDCMISC_32K_B0000   0x008
#define GDCMISC_32K_B8000   0x00C
#define GDCMISC_128K_A0000  0x000
#define GDCMISC_ADDRMASK    0x00C
#define GDCMISC_ALL         (GDCMISC_NOCHARGEN|GDCMISC_ODDTOEVEN|GDCMISC_32K_B8000)

#define GDCDONTCARE_ALL     0x00F

#define GDCBITMASK_ALL      0x0FF

#define CRTOVFL_VERTTOTAL   0x001
#define CRTOVFL_VERTDSPEND  0x002
#define CRTOVFL_VERTSTRTRC  0x004
#define CRTOVFL_VERTSTBLNK  0x008
#define CRTOVFL_LINECMP     0x010
#define CRTOVFL_CURLOC      0x020

#define CRTMAXSCAN_200CONV  0x080       /* VGA bit for 200->400 conversion */

#define CRTMCTRL_COMPAT     0x001       /* CRT compatibility mode (8K disp.) */


#define MISCOUT_COLRPORTS   0x001       /* select 3Dx (or possibly 2Dx) ports */
#define MISCOUT_ENABLERAM   0x002
#define MISCOUT_CLKSELECT   0x00C       /* reset sequencer before writing */
#define MISCOUT_DISABLEDRV  0x010
#define MISCOUT_HIGHPAGE    0x020
#define MISCOUT_NEGHORZRTRC 0x040
#define MISCOUT_NEGVERTRTRC 0x080

#define STATUS1_HORZRTRC    0x001       /* very frequent, but very short */
#define STATUS1_LPENSTROBE  0x002
#define STATUS1_LPENSWITCH  0x004
#define STATUS1_VERTRTRC    0x008       /* less frequent, but much longer */
#define STATUS1_ATCOUTPUT1  0x010
#define STATUS1_ATCOUTPUT2  0x020


#define MONOMODE_HIRES      0x001       /* must always be set */
#define MONOMODE_ENABLE     0x008
#define MONOMODE_BLINK      0x020

#define CGAMODE_80X25       0x001
#define CGAMODE_320X200     0x002
#define CGAMODE_BW          0x004
#define CGAMODE_ENABLE      0x008
#define CGAMODE_640X200     0x010
#define CGAMODE_BLINK       0x020

#define CGACOLOR_BORDER     0x00F       /* border in modes 0/1/2/3, bgnd in 4/5 */
#define CGACOLOR_INTENSE    0x010
#define CGACOLOR_SET        0x020

#endif  /* INCL_VIDEOHW */


#ifdef INCL_VIDEOPTR

/* Define current pointer-draw limitations
 */

#define PTRSAVWD            (MAX_PTRWIDTH+1)
#define PTRSAVHT            (MAX_PTRHEIGHT)
#define PTRSAVSIZE          (PTRSAVWD*PTRSAVHT)

#define BYTEPTRSAVWD        (PTRSAVWD*8)
#define BYTEPTRSAVHT        (PTRSAVHT*1)
#define BYTEPTRSAVSIZE      (BYTEPTRSAVWD*BYTEPTRSAVHT)

#define SCRSAVWD            (PTRSAVWD+2)
#define SCRSAVHT            (PTRSAVHT+4)
#define SCRSAVSIZE          (SCRSAVWD*SCRSAVHT)

#define BYTESCRSAVWD        (SCRSAVWD*8)
#define BYTESCRSAVHT        (SCRSAVHT*1)
#define BYTESCRSAVSIZE      (BYTESCRSAVWD*BYTESCRSAVHT)


/* Define EGA/VGA off-screen memory usage
 */

#define OFFSCREEN0D         ((320/8)*200)
#define OFFSCREEN0E         ((640/8)*200)
#define OFFSCREEN10         ((640/8)*350)
#define OFFSCREEN12         ((640/8)*480)
#define OFFSCREEN13         ((320/1)*200)

#define SIZBASIC            0x0004      /* reserved for BASIC */
#define SIZEGASYS           0x0001      /* reserved for EGA.SYS */
#define SIZMOUSE            0x0001      /* reserved for MOUSE drivers */

#define SIZMICROSOFT        0x0010      /* reserved for use by Microsoft */
#define SIZRESERVED         (SIZBASIC+SIZEGASYS+SIZMOUSE+SIZMICROSOFT)


#define OFFBASIC            0x0000
#define OFFEGASYS           (OFFBASIC  + SIZBASIC)
#define OFFMOUSE            (OFFEGASYS + SIZEGASYS)
#define OFFMICROSOFT        (OFFMOUSE  + SIZMOUSE)
//c-begin
#if OFFMICROSOFT+SIZMICROSOFT != SIZRESERVED
#error  Off-screen offsets do not match sizes
#endif
//c-end





#define OFFPTRSAV0D         (OFFSCREEN0D + SIZRESERVED)
#define OFFSCRSAV0D         (OFFPTRSAV0D + PTRSAVSIZE)
/* c-begin */
#if OFFSCRSAV0D+SCRSAVSIZE > 0x2000
#error  PTRSAVSIZE and/or SCRSAVSIZE too large for mode 0Dh
#endif
/* c-end */

#define OFFPTRSAV0E         (OFFSCREEN0E + SIZRESERVED)
#define OFFSCRSAV0E         (OFFPTRSAV0E + PTRSAVSIZE)
/* c-begin */
#if OFFSCRSAV0E+SCRSAVSIZE > 0x4000
#error  PTRSAVSIZE and/or SCRSAVSIZE too large for mode 0Eh
#endif
/* c-end */

#define OFFPTRSAV10         (OFFSCREEN10 + SIZRESERVED)
#define OFFSCRSAV10         (OFFPTRSAV10 + PTRSAVSIZE)
/* c-begin */
#if OFFSCRSAV10+SCRSAVSIZE > 0x7000
#error  PTRSAVSIZE and/or SCRSAVSIZE too large for mode 10h
#endif
/* c-end */

#define OFFPTRSAV12         (OFFSCREEN12 + SIZRESERVED)
#define OFFSCRSAV12         (OFFPTRSAV12 + PTRSAVSIZE)
/* c-begin */
#if OFFSCRSAV12+SCRSAVSIZE > 0x9800
#error  PTRSAVSIZE and/or SCRSAVSIZE too large for mode 12h
#endif
/* c-end */

#define OFFPTRSAV13         (OFFSCREEN13 + SIZRESERVED)
#define OFFSCRSAV13         (OFFPTRSAV13 + BYTEPTRSAVSIZE)
/* c-begin */
#if OFFSCRSAV13+SCRSAVSIZE > 0x10000
#error  BYTEPTRSAVSIZE and/or BYTESCRSAVSIZE too large for mode 13h
#endif
/* c-end */
/*end*/

#endif  /* INCL_VIDEOPTR */


#define VVD_NAME1   "VVIDEO1$"  /* registered name of primary VVD */
#define VVD_NAME2   "VVIDEO2$"  /* registered name of secondary VVD */


#ifdef  INCL_VIDEOSYSREQ

/***EK+ VVD constants for system communication
 */
#ifndef CF_TEXT
#define CF_TEXT     1           /* clipboard formats */
#define CF_BITMAP   2           /* (see PMWIN.H) */
#endif

#define ADAPTER_MONO    0       /* adapters supported */
#define ADAPTER_CGA     1       /* (same as VioGetConfig constants) */
#define ADAPTER_EGA     2
#define ADAPTER_VGA     3
#define ADAPTER_8514A   7

#ifndef MONITOR_8503            /* BUGBUG: avoid conflicts with bsesub.h */
#define MONITOR_MONO    0       /* monitors supported */
#define MONITOR_RGB     1       /* (typical for CGAs) */
#define MONITOR_ECD     2       /* (typical for EGAs) */
#define MONITOR_8503    3       /* (VGA or 8514/A monochrome) */
#define MONITOR_8512_3  4       /* (VGA or 8514/A color) */
#define MONITOR_8514    9       /* (VGA or 8514/A 1024x768 color, interlaced) */
#endif

#define FORMAT_CGA      2       /* LVB formats supported */
#define FORMAT_4BYTE    4
#define FORMAT_BITMAP   0

#define DDFORMAT_4PLANE 1       /* display driver formats supported */
#ifdef  ATLAS
#define DDFORMAT_8PLANE 2       /* display driver 8 plane formats supported      */
#define DDFORMAT_8PLANE_PALETTE 3
                                /* display driver 8 plane with palette supported */
#endif  //ATLAS

#define MODE_MONO            0x0001 /* monochrome mode in effect */
#define MODE_UNDERLINE       0x0002 /* underlining in effect */
#define MODE_SUP_XSCALE2     0x1000 /* X scaling supported by factor of 2 */
#define MODE_SUP_YSCALE2     0x2000 /* Y scaling supported by factor of 2 */
#define MODE_SUP_PARTIALSCAN 0x4000 /* partial scanline copy requests supported */

#define SUSPEND_NONE              0 /* VDM running normally */
#define SUSPEND_OUT_OF_MEMORY     1 /* VDM suspended due to low memory */
#define SUSPEND_UNSUPPORTED_MODE  2 /* VDM suspended due to unsupported mode */

#define ACCESS_RELEASE            0 /* values for SETACCESS request */
#define ACCESS_REQUEST            1
#define ACCESS_PMREQUEST          2

#define CONTROL_RELEASE           0 /* Release event thread */
#define CONTROL_VDMMINIMIZED      1 /* Disable video events for VDM */
#define CONTROL_VDMUNMINIMIZED    2 /* Enable video events for VDM */


/* VVD system functions
 */
#define VVDSYSREQ_SETACCESS     1   /* Obtain exclusive event access */
#define VVDSYSREQ_SETFOCUS      2   /* Notify VDM gaining/losing focus */
#define VVDSYSREQ_SETLOCK       3   /* Lock/unlock VDM's video state */
#define VVDSYSREQ_QUERYMODE     4   /* Query VDM's current mode */
#define VVDSYSREQ_QUERYCURSOR   5   /* Query VDM's current mode */
#define VVDSYSREQ_QUERYPALETTE  6   /* Query VDM's palette settings */
#define VVDSYSREQ_COPYLVB       7   /* Copy LVB data */
#define VVDSYSREQ_COPYBITMAP    8   /* Copy bitmap data */
#define VVDSYSREQ_WAITEVENT     9   /* Wait for VDM video update event */
#define VVDSYSREQ_CONTROLEVENT  10  /* Perform event control operations */

#define VVDSYSREQ_SETDRQ        11  /* Set display requirements */
#define VVDSYSREQ_REQMEM        12  /* Request off-screen video memory */
#define VVDSYSREQ_FREEMEM       13  /* Free off-screen video memory */
#define VVDSYSREQ_REQCTRL       14  /* Request video controller ownership */
#define VVDSYSREQ_FREECTRL      15  /* Free video controller ownership */

#define VVDSYSREQ_QUERYVRAMSTATUS 16/* Return VRAM status since last call */

#define VVDSYSREQ_SETOEMFLAG    17  /* Set OEM Status Flag */

#define VVDSYSREQ_MAX           17


/* VVD event IDs for VVDSYSREQ_WAITEVENT
 */
#define VVDEVENT_NONE           0   /* No change */
#define VVDEVENT_MODE           1   /* Change in VDM's mode */
#define VVDEVENT_PALETTE        2   /* Change in VDM's palette */
#define VVDEVENT_LVB            3   /* Change in VDM's LVB */
#define VVDEVENT_SCROLL         4   /* Scroll of VDM's LVB */
#define VVDEVENT_STRING         5   /* String output */
#define VVDEVENT_CURSOR         6   /* Cursor position/type change */
#define VVDEVENT_INPUT          7   /* VDM is checking for input data */
#define VVDEVENT_ENDPASTE       8   /* VDM has cancelled pasting */
#define VVDEVENT_PASTE          9   /* VDM is ready for additional pasting */
#define VVDEVENT_SWITCHERROR    10  /* VDM cannot be switched foreground */
#define VVDEVENT_TITLECHANGE    11  /* VDM title has changed */
#define VVDEVENT_DDE            12  /* Set/Clear DDE flag */

#define VVDEVENT_MAX            12
/*end*/


/***ET+ VVD DosRequestVDD communication structures
 */

typedef struct vvm_s {          /* vvm (output for MODE event) */
    ULONG  vvm_ulAdapter;       /* see the ADAPTER_* constants */
    ULONG  vvm_ulFormat;        /* see the FORMAT_* constants */
    ULONG  vvm_ulDDFormat;      /* see the DDFORMAT_* constants */
    ULONG  vvm_flMode;          /* mode descriptors (see MODE_* constants) */
    ULONG  vvm_nRows;           /* height of screen in rows (or y pixels) */
    ULONG  vvm_nCols;           /* width of screen in columns (or x pixels) */
    ULONG  vvm_nPlanes;         /* # of planes (must be 1 for Cruiser) */
    ULONG  vvm_nBitCount;       /* if TEXT, zero;  if BITMAP, bits per pixel */
    ULONG  vvm_ulCellWidth;     /* width of cells (normally 8;  1 for BITMAPs) */
    ULONG  vvm_ulCellHeight;    /* height of cells (1 for BITMAPs) */
    ULONG  vvm_fSuspended;      /* see the SUSPEND_* constants */
    ULONG  vvm_cpID;            /* current code-page ID */
    ULONG  vvm_FormatID;        /* current format ID */
#ifdef ATLAS_GALE
    ULONG  vvm_GridColor;       /* current grid color                     */
    ULONG  vvm_AttrFlag;        /* current screen intensity               */
    ULONG  vvm_nRows2;          /* height of screen in rows for superimpose text   */
    ULONG  vvm_nCols2;          /* width of screen in columns for superimpose text */
    ULONG  vvm_ulCellWidth2;    /* width of cells for superimpose text    */
    ULONG  vvm_ulCellHeight2;   /* height of cells for superimpose text   */
    ULONG  vvm_ulFormat2;       /* 4 for superimpose, otherwise 0         */
#endif  //ATLAS_GALE
} VVMODE;
typedef VVMODE *NPVVMODE;
typedef VVMODE FAR *PVVMODE;

#ifdef  ATLAS_GALE
#define FORMATID_EPOCHColor     0x41
#define FORMATID_EPOCHMono      0x81
#define FORMATID_MckColor       0x20
#define FORMATID_CommonFMT      0x70
#endif  //ATLAS_GALE

typedef struct vvr_s {          /* vvr (input for COPYBITMAP request) */
    ULONG  vvr_ulDDFormat;      /* display driver format (0 if DIB used) */
    ULONG  vvr_cx;              /* target bitmap width */
    ULONG  vvr_cy;              /* target bitmap height */
    RECTL  vvr_rcl;             /* rectangle being requested */
    PBYTE  vvr_pbColorXlate;    /* display driver color translation table */
} VVRECT;
typedef VVRECT *NPVVRECT;
typedef VVRECT FAR *PVVRECT;

typedef struct vvl_s {          /* vvl (output for LVB event) */
    RECTL  vvl_rcl;             /* rectangle being scrolled */
    ULONG  vvl_fCertain;        /* TRUE if entire area changed, FALSE if unknown */
#ifdef ATLAS_GALE
    ULONG  vvl_pFlags;          /* LSB indicate which page (text or graphics) is */
                                /* updated. 0 means graphics, 1 does text.       */
#endif  //ATLAS_GALE
} VVLVB;
typedef VVLVB *NPVVLVB;
typedef VVLVB FAR *PVVLVB;

typedef struct vvs_s {          /* vvs (output for SCROLL event) */
    RECTL  vvs_rcl;             /* rectangle being scrolled */
    LONG   vvs_fill;            /* fill value */
    LONG   vvs_nRows;           /* # rows (negative means reverse scroll) */
    LONG   vvs_nCols;           /* # columns (0,0 means rectangle blankfilled) */
} VVSCROLL;
typedef VVSCROLL *NPVVSCROLL;
typedef VVSCROLL FAR *PVVSCROLL;

typedef struct vva_s {          /* vva (output for STRING event) */
    ULONG  vva_row;             /* beginning row of string */
    ULONG  vva_col;             /* beginning column of string */
    ULONG  vva_nChars;          /* # characters in string */
} VVSTRING;
typedef VVSTRING *NPVVSTRING;
typedef VVSTRING FAR *PVVSTRING;

typedef struct vvc_s {          /* vvc (output for CURSOR event) */
    ULONG  vvc_row;             /* row (y position) of VDM's cursor */
    ULONG  vvc_col;             /* column (x position) of VDM's cursor */
    ULONG  vvc_ulScanStart;     /* starting scan-line for VDM's cursor */
    ULONG  vvc_ulScanEnd;       /* ending scan-line for VDM's cursor */
    ULONG  vvc_fVisible;        /* TRUE if VDM cursor visible, FALSE if not */
} VVCURSOR;
typedef VVCURSOR *NPVVCURSOR;
typedef VVCURSOR FAR *PVVCURSOR;

typedef struct vve_s {          /* vve (output for VVDSYSREQ_GET/WAITEVENT) */
    LONG   vve_iEvent;          /* one of the VVDEVENT_* constants */
    ULONG  vve_sgID;            /* screen group ID of VDM */
    ULONG  vve_nData;           /* # of entries of information returned */
} VVEVENT;
typedef VVEVENT *NPVVEVENT;
typedef VVEVENT FAR *PVVEVENT;

typedef struct vvreg_s {        /* vvreg (shadow entry for VVDSYSREQ_SETDRQ) */
    USHORT vvreg_port;          /* port # */
    CHAR   vvreg_indx;          /* register index # (-1 if index reg) */
    BYTE   vvreg_value;         /* last value written to register by VDD */
} VVREG;
typedef VVREG *NPVVREG;
typedef VVREG FAR *PVVREG;


/*** In the DRQ structure, all pointers are 16:16, and as such, cannot be
 *   dereferenced directly from C.  The selector portions are GDT aliases to
 *   the PM display driver's resident data, and are valid in any context, even
 *   interrupt-time.
 */
#define ALL_RESERVED 0xffffffff

#define DRQ_DIRTYREGS   0x01    /* video controller registers have been modified */

typedef struct vvdrq_s {        /* vvdrq (main structure for VVDSYSREQ_SETDRQ) */
    PBYTE  vvd_pPhysVRAM;       /* physical address of VRAM */
    ULONG  vvd_nbReserved;      /* # of reserved bytes */
    ULONG  vvd_offLatchByte;    /* offset of available latch storage */
    PBYTE  vvd_pfbDRQFlags;     /* pointer to flags (see DRQ_* constants) */
    PBYTE  vvd_pfCtrlOwned;     /* address of display.dll's fCtrlOwned flag */
    PBYTE  vvd_pfCtrlNotify;    /* address of display.dll's fCtrlNotify flag */
    ULONG  vvd_nShadowRegs;     /* # of registers to shadow */
    PVVREG vvd_pShadowData;     /* address of first entry in shadow list */
} VVDRQ;
typedef VVDRQ *NPVVDRQ;
typedef VVDRQ FAR *PVVDRQ;
/*end*/

#endif  /* INCL_VIDEOSYSREQ */


#ifdef  INCL_VIDEODEVREQ

/***EK+ VVD constants for inter-VDD communication
 */

#define VVDDEVREQ_POSTPEEK      1   /* VDM peeking for keyboard data */
#define VVDDEVREQ_POSTREAD      2   /* VDM reading for keyboard data */
#define VVDDEVREQ_POSTPASTE     3   /* VDM ready for more paste data */
#define VVDDEVREQ_POSTMOUSE     4   /* VDM mouse activity notification */
#define VVDDEVREQ_DEVACCEPT     5   /* request VVD to manage adapter */
#define VVDDEVREQ_DEVRELEASE    6   /* request VVD to free adapter */
#define VVDDEVREQ_DSPACCEPT     7   /* request VVD to manage display */
#define VVDDEVREQ_DSPRELEASE    8   /* request VVD to free display */
#define VVDDEVREQ_POSTEVENT     9   /* post a VDM event to the Shield */
#define VVDDEVREQ_SAVERESTORE  10   /* enable/disable save/restore     XGA */
#define VVDDEVREQ_REPAINT      11   /* keep VRAMISTOAST bit set        XGA */
#define VVDDEVREQ_REQCTRL      12   /* Request controller ownership          */
#define VVDDEVREQ_FREECTRL     13   /* Free video controller ownership       */
#define VVDDEVREQ_PM_WINDOW    14   /* Set Windows on PM DeskTop state       */
#define VVDDEVREQ_ENABLE_DDE   15   /* Set/Clear DDE flag                    */
#define VVDDEVREQ_XGAPRESENT   16   /* VXGA installed                        */
#define VVDDEVREQ_UNMAPVRAM    17   /* Unmap A0000 aperture                  */
#define VVDDEVREQ_8514PRESENT  18   /* V8514 installed                       */
#define VVDDEVREQ_VIDEOBUFFER  19
#define VVDDEVREQ_NOTIFYVTEXT  20

#define VVDDEVREQ_MAX          20

#define POSTEVENT_ADD      0x0001   /* add given event */
#define POSTEVENT_FLUSH    0x0002   /* flush given (or existing) event */
#define POSTEVENT_DELETE   0x0004   /* delete existing event */


/***ET+ VVD VDHRequestVDD communication structures
 */
typedef struct vvp_s {          /* vvp (input for POSTEVENT request) */
    LONG   vvp_iEvent;          /* see the VVDEVENT_* constants */
    PVOID  vvp_pEvent;          /* event data (varies from one event to another) */
    ULONG  vvp_flEvent;         /* event posting flags (see POSTEVENT_* constants) */
} VVPOST;
typedef VVPOST *NPVVPOST;
typedef VVPOST FAR *PVVPOST;
/*end*/

#endif  /* INCL_VIDEODEVREQ */

#ifdef INCL_VIDEOINTERFACES

typedef (*PASCAL PFNINT2FP)(HVDM, ULONG, PCRF);

/*** ULONG value sent to the registered INT 2F routine ***/
#define VDHVVD_INT2F_OFF      0
#define VDHVVD_INT2F_ON       1
#define VDHVVD_INT2F_FG_START 2
#define VDHVVD_INT2F_FG_END   3
#define VDHVVD_INT2F_BG_START 4
#define VDHVVD_INT2F_BG_END   5

#ifndef VDHENTRY
#define VDHENTRY PASCAL NEAR
#endif

VOID VDHENTRY VDHRegisterInt2FProc(FLAGS, PFNINT2FP);

/*** BOOL value indicates the type of notifications required ***/
#define VDHVVD_NOTIFY_ON_OFF  1
#define VDHVVD_NOTIFY_START   2
#define VDHVVD_NOTIFY_END     4

#endif   /* INCL_VIDEOINTERFACES */

/*** VVD exported function prototypes
 */

/* None */

#endif /* __VVD__ */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
