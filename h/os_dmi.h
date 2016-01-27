/*    Filename: OS_DMI.H                                                       */

/*    Copyright (c) 1993 - 1994, International Business Machines, Corporation. */
/*    All rights reserved.                                                     */

/*    Description: Include file for using the OS/2 DMTF Service Layer          */


#ifndef OS_DMI_H_FILE
#define OS_DMI_H_FILE

/**************************************  INCLUDES   ********************************************** */

#include "os2def.h"
#define _FAR
#define OS_SL_VERSION "IBM OS/2 Warp SL Ver. 1.10"    /* Version 1.00 was internal build 0.21 */
                                                      /* Version 1.10 was internal build 0.24 */
#define SL_ENVIRONMENT_ID MIF_OS2_ENVIRONMENT         /* ID of this service layer environment */
#define PR_TARGET_OS "OS2"                            /* target OS used in the parser functions */
#define DMI_FUNC_ENTRY _System                        /* define the system calling conventions */
#define DMI_FUNC_CALLBACK _System

typedef unsigned short boolean;

#ifndef WPOS_SL
#define _M_I86       /*** NOTE this is only used on machines that use the i86 byte ordering ****/
#endif

#pragma pack(2)      /* this will keep the old DOS code happy                 */
#include "dmi.h"     /* the Management Interface primitives                  */
#include "error.h"   /* include the service layer error file                  */
#pragma pack()       /* return to default allignment                          */

/**************************************  PROTOTYPES ********************************************** */


/* Entry points that are unique to the OS/2 Service Layer implementation     */
DMI_UNSIGNED DMI_FUNC_ENTRY sDmiInvoke(void _FAR *dmiCommand);   /* syncronout invoke entry point */

#endif
