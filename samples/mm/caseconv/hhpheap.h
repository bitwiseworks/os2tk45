/*static char *SCCSID = "@(#)hhpheap.h	13.1 92/02/15";*/

/********************** START OF SPECIFICATIONS *****************************/
/*                                                                          */
/* SOURCE FILE NAME: HHPHEAP.H                                              */
/*                                                                          */
/* DESCRIPTIVE NAME: Heap manager include file.                             */
/*                                                                          */
/* COPYRIGHT:                                                               */     
/*                 Copyright (c) IBM Corporation 1992, 1993                 */
/*                         All Rights Reserved                              */
/*                                                                          */
/* STATUS: OS/2 MM Release 1.10                                             */
/*                                                                          */
/* ABSTRACT: Contains data structures and prototypes that are used in       */
/*           HHP API's.                                                     */
/*                                                                          */
/*********************** END OF SPECIFICATIONS ******************************/


#ifndef _HHPHEAP_H_
#define _HHPHEAP_H_

typedef PVOID HHUGEHEAP;

HHUGEHEAP APIENTRY HhpCreateHeap( ULONG uInitSize, 
                                  USHORT fFlags );

PVOID     APIENTRY HhpAllocMem( HHUGEHEAP hheap, 
                                ULONG cbLen );

BOOL      APIENTRY HhpFreeMem( HHUGEHEAP hheap, 
                               PVOID pData );

BOOL      APIENTRY HhpDestroyHeap( HHUGEHEAP hheap );

/* Shared Heap Support */

USHORT    APIENTRY HhpAccessHeap( HHUGEHEAP hheap, 
                                  PID pid );

USHORT    APIENTRY HhpReleaseHeap( HHUGEHEAP hheap, 
                                   PID pid );

PID       APIENTRY HhpGetPID( VOID );

/* Low-Level Independent (Not Suballocated) Buffer Support */

PBYTE     APIENTRY HhpAllocBuffer( ULONG lSize, 
                                   USHORT fFlags );

USHORT    APIENTRY HhpFreeBuffer( PBYTE pBuffer );

USHORT    APIENTRY HhpAccessBuffer( PBYTE pBuffer );

#define HH_SHARED         0x1    /* Cause the Heap to be Shared */

USHORT    APIENTRY HhpDumpHeap( HHUGEHEAP hheap );

#endif /* _HHPHEAP_H_ */
