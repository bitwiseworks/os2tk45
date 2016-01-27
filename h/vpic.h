/***************************************************************************
*
* Module Name: Vpic.h
*
*
* Copyright (c) 1987 - 1992  IBM Corporation
*      MVDM Virtual PIC Exported Definitions
*
*      For include file hierarchy, see MVDM.H
*
*
*/


/* VPIC constants */

/* VDHOpenVIRQ handler definitions */

#define VPIC_NOEOIHOOK                  NULL
#define VPIC_NOIRETHOOK                 NULL
#define VPIC_NOIRETHOOK_TIMEOUT         -1

/* VDHOpenVIRQ option definitions */

#define VPIC_SHARE_IRQ                  0x0001

/* VDHQueryVIRQ bit definitions */

#define VPICQ_REQUEST_PENDING           0x0001
#define VPICQ_IN_SERVICE                0x0002
#define VPICQ_VIRT_MASK                 0x0004
#define VPICQ_IRET_PENDING              0x0008

/* VDHWaitVIRRS hook type */

#define VDH_WAITVIRRS_HOOK              (VDH_RETURN_HOOK)

/* VPIC data types */

/***ET+ IRQN - IRQ number
 *
 *      IRQ number definition.
 */

typedef ULONG IRQN;
/*end*/

/***ET+ HIRQ - IRQ handle
 *
 *      IRQ handle defination
 */

typedef ULONG HIRQ;
/*end*/



/* VPIC function prototypes */

HIRQ VDHENTRY VDHOpenVIRQ(IRQN, PFN, PFN, ULONG, ULONG);
VOID VDHENTRY VDHCloseVIRQ(HIRQ);
VOID VDHENTRY VDHSetVIRR(HVDM, HIRQ);
VOID VDHENTRY VDHClearVIRR(HVDM, HIRQ);
ULONG VDHENTRY VDHQueryVIRQ(HVDM, HIRQ);
VOID VDHENTRY VDHSendVEOI(HIRQ);
BOOL VDHENTRY VDHWaitVIRRs(HHOOK);
VOID VDHENTRY VDHWakeVIRRs(HVDM);
