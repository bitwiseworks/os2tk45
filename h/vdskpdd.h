/***************************************************************************
*
* Module Name: vdskpdd.h
*
* OS/2 public header file.
*
* Copyright (c) 1993  IBM Corporation
*
* Disk PDD/VDD shared structures/constants
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

#ifndef __VDSKPDD__
#define __VDSKPDD__

#define DSK_NAME                "DSK$"


/***    PDD function commands
 *
 *  Format:
 *      (*fpfnPDSKVDDProc)(PDSKCMD_*, ul1, ul2);
 *
 *  PDSKCMD_REGISTER:
 *      ENTRY
 *          ul1 == segment of 16:32 VDD entry point
 *          ul2 ==  offset of 16:32 VDD entry point
 *      EXIT
 *          returns TRUE
 *
 *  PDSKCMD_DEREGISTER:
 *      ENTRY
 *          ul1 == 0                    //not used
 *          ul2 == 0                    //not used
 *
 *  PDSKCMD_QUERYDRVTYPE: (for removable media drive only)
 *      ENTRY
 *          ul1 == nDrive               //drive number (USHORT)
 *          ul2 -> bDriveType           //16:16 pointer to BYTE
 *      EXIT-SUCCESS
 *          returns TRUE
 *          usDriveType is set to the appropriate drive type
 *              see DRVTYPE_* equates
 *      EXIT-FAILURE
 *          returns FALSE
 *          usDriveType is set to DRVTYPE_NONE
 *
 *  PDSKCMD_QUERYCHANGE: (for removable media drive only)
 *      ENTRY
 *          ul1 == nDrive               //drive number (USHORT)
 *          ul2 == 0                    //not used
 *      EXIT-SUCCESS
 *          returns TRUE (disk has changed)
 *      EXIT_FAILURE
 *          returns FALSE (disk has not changed)
 *
 *  PDSKCMD_QUERYMEDIA: (for removable media drive only)
 *      ENTRY
 *          ul1 == nDrive               //drive number (USHORT)
 *          ul2 -> bMediaType           //16:16 pointer to BYTE
 *      EXIT-SUCCESS
 *          returns TRUE
 *          usMediaType is set to appropriate media type
 *              see MEDIATYPE_* equates
 *      EXIT-FAILURE
 *          returns FALSE (no media in drive etc.)
 *          usMediaType is set to MEDIATYPE_NONE
 *
 *  PDSKCMD_DISKREQUEST:
 *      ENTRY
 *          ul1 -> DISKREQ packet       //16:16 pointer to disk request pkt.
 *          ul2 -> REQSTATUS packet     //16:16 pointer to req. status pkt.
 *      EXIT-SUCCESS
 *          returns TRUE (the request is accepted)
 *      EXIT-FAILURE
 *          returns FALSE (the request is rejected)
 *          REQSTATUS packet has the operation status
 *      NOTE
 *          The current supported disk requests for this function are:
 *              DISKREQ_READSECTORS
 *              DISKREQ_WRITESECTORS (for floppy only)
 *              DISKREQ_VERIFYSECTORS
 *              DISKREQ_FORMATTRACK  (for floppy only)
 *          The function will return immediately whether the request is
 *          accepted or not.  If the request is accepted, the status of
 *          the operation will not be available until the request is
 *          actually completed at which time VDSK will receive an asyn-
 *          chronous notification.  If the request is rejected, the request
 *          status packet will have the error status and no asynchronous
 *          notification will be made.
 */

#define PDSKCMD_REGISTER        PDDCMD_REGISTER      //register VDD
#define PDSKCMD_DEREGISTER      (PDSKCMD_REGISTER+1) //de-register of VDD
#define PDSKCMD_QUERYDRVTYPE    (PDSKCMD_REGISTER+2) //query drive type
#define PDSKCMD_QUERYDRVPARMS   (PDSKCMD_REGISTER+3) //query drive parms ; pb 789010
#define PDSKCMD_QUERYCHANGE     (PDSKCMD_REGISTER+4) //query disk changed
#define PDSKCMD_QUERYMEDIA      (PDSKCMD_REGISTER+5) //query media type
#define PDSKCMD_DISKREQUEST     (PDSKCMD_REGISTER+6) //disk request


/***    VDD function commands
 *
 *  Format:
 *      (*fpfnVDSKPDDProc)(VDSKCMD_*, ul1, ul2);
 *
 *  VDSKCMD_REQDONE:
 *      ul1 == 0        //not used
 *      ul2 == 0        //not used
 */

#define VDSKCMD_REQDONE         0x01    //notify VDSK of disk request completion


/***    VDSK-PDSK Communication Structures
 *
 *      Note that the structures are allocated as VDM fixed instance
 *      data so that the packet may be accessed at interrupt time.
 *      However, the user buffer (specified as a 16:16 pointer) is
 *      always swappable.  In addition, the user buffer pointer is
 *      specified in virtual address space.  Therefore, it is the
 *      physical device driver's responsibility to translate the
 *      address into physical address for DMA purpose and make sure
 *      the user buffer is locked down before any DMA operation can
 *      proceed.
 *
 *      Also note that the request status codes returned are made
 *      the same as Int 13h status codes for VDSK implementation
 *      efficiency, but additional status codes can also be added
 *      if necessary (see INT13STATUS_* equates in vdmbios.h).
 */

typedef struct diskreq_s {
    BYTE        dr_bReqFunc;            //request function
    BYTE        dr_bDrive;              //drive number
    BYTE        dr_bHead;               //head number
    WORD        dr_wCylinder;           //cylinder number
    BYTE        dr_bSector;             //sector number
    BYTE        dr_nSectors;            //number of sectors to process
    USHORT      dr_usFormatTracks;      //number of tracks to format
    USHORT      dr_usFormatSectors;     //number of sectors per track to format
    F16PVOID    dr_f16pBuffer;          //16:16 pointer to buffer
    F16PVOID    dr_f16pDiskParm;        //16:16 pointer to disk parameter table
} DISKREQ;


typedef struct reqstatus_s {
    BYTE    rs_bReqStatus;              //disk request status of operation
    BYTE    rs_nSectorsDone;            //number of sectors processed
} REQSTATUS;


/***    Drive Type Equates
 *
 *      Note that these equates are derived from the
 *      Diskette Drive Type Byte in the CMOS RAM.
 */

#define DRVTYPE_NONE    0               //no drive
#define DRVTYPE_360K    1               //360K drive
#define DRVTYPE_1200K   2               //1.2M high capacity drive
#define DRVTYPE_720K    3               //3.5" drive (720K)
#define DRVTYPE_1440K   4               //3.5" high capacity drive (1.44M)


/***    Media Type Equates
 *
 *      Note that the equates must be tied with DRVTYPE_* equates
 *      because VDSK component implementation assumes it so.
 */

#define MEDIATYPE_NONE  DRVTYPE_NONE    //no media in drive
#define MEDIATYPE_360K  DRVTYPE_360K    //360K double side double density disk
#define MEDIATYPE_1200K DRVTYPE_1200K   //1.2M high capacity disk
#define MEDIATYPE_720K  DRVTYPE_720K    //3.5" 720K disk
#define MEDIATYPE_1440K DRVTYPE_1440K   //3.5" 1.4M disk


/***    Disk Request Function Equates
 */

#define DISKREQ_READSECTORS     0
#define DISKREQ_WRITESECTORS    1
#define DISKREQ_VERIFYSECTORS   2
#define DISKREQ_FORMATTRACK     3

#endif /* __VDSKPDD__ */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
