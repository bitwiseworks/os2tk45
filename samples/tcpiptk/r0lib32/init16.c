/********************************************************copyrite.xic********/
/*                                                                          */
/*   Licensed Materials - Property of IBM                                   */
/*   IBM TCP/IP for OS/2                                                    */
/*   (C) Copyright IBM Corporation. 1990,1996.                              */
/*                                                                          */
/*   All rights reserved.                                                   */
/*                                                                          */
/*   US Government Users Restricted Rights -                                */
/*   Use, duplication or disclosure restricted by GSA ADP Schedule          */
/*   Contract with IBM Corp.                                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is             */
/*  sample code created by IBM Corporation. This sample code is not         */
/*  part of any standard or IBM product and is provided to you solely       */
/*  for  the purpose of assisting you in the development of your            */
/*  applications.  The code is provided "AS IS", without                    */
/*  warranty of any kind.  IBM shall not be liable for any damages          */
/*  arising out of your use of the sample code, even if they have been      */
/*  advised of the possibility of such damages.                             */
/*--------------------------------------------------------------------------*/
/************************** INIT16.C ****************************************/
#define  _16BITCODE
#define  INCL_DOS
#define  INCL_DOSINFOSEG
#include <os2.h>

#include <sys\itypes.h>
#include <driver.h>
#include <genio.h>
#include <pddentry.h>
#include <sys\dprintf.h>
#include <sys\socket.h>
#include <entry.h>

extern void setjmp32(void);
extern struct DOSTABLE *GetDosVarLinearPtr(void);          /* index=09 default*/

void initdevice();

short initflag;
unsigned long * ptkssbase=0;        /* Ptr to kernel ssbase. amol 10/10/96 */

struct DOSTABLE {
  unsigned char   NumEntries;    /* Number of entries is DOSTABLE */
  unsigned long   HardErr;       /* Hard error routine            */
  unsigned long   uCase;         /* h_UCase                       */
  unsigned long   Dummy[3];
  unsigned long   MemMapAlias;   /* h_MemMapAlias                 */
  unsigned long   MemUnMapAlias; /* h_UCase                       */
  unsigned long   GoProtAll;     /* h_GoProtAll                   */
  unsigned long   GoRealAll;     /* h_GoRealAll                   */
  unsigned long   RedirDevice;   /* h_doshelp_RedirDev            */
  unsigned long   Dummy2[2];
  unsigned long   SFFromSFN;     /* h_SFFromSFN                   */
  unsigned long   SegGetInfo;    /* h_SegGetInfo                  */
  unsigned long   Dummy3[3];
  unsigned char   NumEntries2;
  unsigned long   DosTab2[18];
};
struct DOSTABLE *PDosTbl;

typedef unsigned long physaddr;  /* A physical address is 4 bytes */
typedef void far * farptr;

typedef struct _2words_def {
  unsigned short low;
  unsigned short high;
  } _2words_type;

typedef struct _segaddr_def {
  unsigned short offset;
  unsigned short segment;
  } _segadr_type;

typedef union _32bits_def {
   physaddr phys;
   void far * fptr;
   _2words_type _2words;
   _segadr_type _segadr;
   } _32bits;

void devopen(lpreq)
lpRequest lpreq;
{
   lpreq->reqstatus |= DONE_BIT;
}

void devclose(lpreq)
lpRequest lpreq;
{
   lpreq->reqstatus |= DONE_BIT;
}

void init16_c(lpreq)
lpRequest lpreq;
{
   switch(lpreq->reqcommand) {
      case INIT:
           /* int3break(); */
           initdevice(lpreq);
           initflag=1;
           setjmp32();
           break;
      case OPEN:
           devopen(lpreq);
           break;
      case CLOSE:
           devclose(lpreq);
           break;
      case GENIO:
           /* this case should be handled in driver.c (32bit mode) */
           lpreq->reqstatus = GEN_ERR + ERR_UNKNOWNCOMMAND + DONE_BIT;
           break;
      case INIT_COMPLETE:
           lpreq->reqstatus |= DONE_BIT;
           break;
      default:
           lpreq->reqstatus = GEN_ERR + ERR_UNKNOWNCOMMAND + DONE_BIT;
           break;
   } /* end switch */
}


void initdevice(lpRequest lpreq)
{
   extern ULONG near devhelp;
   extern void near last_c();
   extern void near last_d();
   lpInit_Packet lpi;
   int  lc ,ld;

   lpi  = (lpInit_Packet)lpreq;

   devhelp = lpi->lpDevHlp;     /* save the devhelp entry point */

   PDosTbl=(struct DOSTABLE *)GetDosVarLinearPtr();       /* index=09 default*/
   /*This Magic#11 (DOSTAB2_TKSSBASE) points to the
    *  required base address of ss.
    */
   ptkssbase=(unsigned long *) PDosTbl->DosTab2[11];

   lc            = ((int) last_c)-1;
   ld            = ((int) last_d)-1;
   lpi->lpDevHlp = MAKEULONG(lc, ld);   /* return last_c and last_d to kernel */
   lpi->InitArgs = 0L;

   lpreq->reqstatus = DONE_BIT;
}
