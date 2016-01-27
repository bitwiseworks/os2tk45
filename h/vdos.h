/*static char *SCCSID = "@(#)vdos.h     6.2 92/01/28";*/
/*
 *      VDOS.H
 *      Copyright (c) 1992 IBM Corporation
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

#ifndef __VDOS__
#define __VDOS__

#pragma pack(4)

/***ET+ Low memory defines - used by VBIOS */

#define DCA_START               0x501   /* DOS communication area start */
#define DCA_LEN                 0x1FF   /*  "        "        "   length */

#define DOSVEC_FIRST            0x20    /* DOS-reserved vectors */
#define DOSVEC_LAST             0x3F    /* initialized to -> IRET by VBIOS */
/*end*/

/***ET+ DOSDDEOL - End of DOS DD List Tag
 */

#define DOSDDEOL        ((VPDOSDDTYPE) -1L)
/*end*/

/***ET+ DOSDDTYPE - DOS DD Header
 *
 *      This structure defines a DOS DD Header
 */

typedef struct _dosddtype {
    struct _dosddtype FARV86 *ddd_vpdosddtype;
    USHORT ddd_usAttribute;
    USHORT ddd_pfnStrategy;
    USHORT ddd_pfnInterrupt;
    CHAR ddd_achName[8];
} DOSDDTYPE;

typedef DOSDDTYPE *PDOSDDTYPE;
typedef DOSDDTYPE FARV86 *VPDOSDDTYPE;
/*end*/

/* PROCEDURES */
BOOL VDHENTRY VDHSetDosDevice(VPDOSDDTYPE);

#pragma pack()

#endif /* __VDOS__ */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
