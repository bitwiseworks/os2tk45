/****************************************************************************/
/*                                                                          */
/* Module Name: DCB.H                                                       */
/*                                                                          */
/* OS/2 2.0 Multimedia Extensions Sync/Stream Manager Device Control        */
/* Block defintitions.                                                      */
/*                                                                          */
/* Copyright (c) International Business Machines Corporation 1991, 1992     */
/*                        All Rights Reserved                               */
/*                                                                          */
/****************************************************************************/
/* NOINC */
#ifdef __cplusplus
   extern "C" {
#endif
/* INC */


/* XLATOFF */
#pragma pack(1)
/* XLATON */

/***************************************************************************
 *
 * DCB - Common Device Control Block
 *               (*** PACKED STRUCTURE ***)
 ***************************************************************************/
typedef struct _DCB { /* dcb  -  Device Control Block                      */
   ULONG   ulDCBLen;                /* length of structure                 */
   SZ      szDevName[MAX_SPI_NAME]; /* device driver name                  */
   } DCB;
typedef DCB  FAR *PDCB;


/***************************************************************************
 *
 * E_DCB - Extended Device Control Block
 *
 *         This structure will grow over time as new fields are added
 *         to the end of the structure.  If you manipulate the structure,
 *         be sure to check the length field.
 *
 *               (*** PACKED STRUCTURE ***)
 ***************************************************************************/
typedef struct E_DCB { /* e_dcb  -  Extended Device Control Block          */

   ULONG   ulDCBLen;                /* length of structure                 */
   SZ      szDevName[MAX_SPI_NAME]; /* device driver name                  */
   ULONG   ulSysFileNum;            /* file handle number                  */

   /*    new fields will be added here                                     */

   } E_DCB, DCB_AUDIOSH;

typedef E_DCB  FAR  *PE_DCB;
typedef E_DCB  FAR  *PDCB_AUDIOSH;


/***************************************************************************
 *
 * VSD_DCB - VSD Device Control Block
 *
 *         This structure will allow stream handlers to use the VSD DLL
 *         by using by the additional fields in the structure.
 *
 *               (*** PACKED STRUCTURE ***)
 ***************************************************************************/
typedef struct _VSD_DCB { /* vsd_dcb - VSD Device Control Block            */

   ULONG   ulDCBLen;                /* length of structure                 */
   SZ      szDevName[MAX_SPI_NAME]; /* device driver name                  */
   ULONG   ulSysFileNum;            /* file handle number                  */
   ULONG   hvsd;                    /* Handle to VSD instance              */
   PFN     pfnvsdEntryPoint;        /* Address of VSD entry point          */
   ULONG   ulReserved1;             /* Reserved for system                 */
   ULONG   ulReserved2;             /* Reserved for system                 */

   } VSD_DCB;

typedef VSD_DCB FAR *PVSD_DCB;


/* XLATOFF */
#pragma pack()
/* XLATON */

/* NOINC */
#ifdef __cplusplus
}
#endif
/* INC */

