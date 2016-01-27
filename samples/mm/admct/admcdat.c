/********************** START OF SPECIFICATIONS ****************************
*
*   SOURCE FILE NAME:   ADMCDAT.C
*
*   DESCRIPTIVE NAME: mci Waveform Driver include file.
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
*   STATUS: MM Extensions 1.0
*
*   FUNCTION: Global Data structure definitions.
*
*
*   NOTES: The Global Data Structures are the following.
*          UserCount.    -- Indicates the number of dynamic Links to the DLL
*
*          hmtxProcSem   -- Governs DLL intialization per processes
*                        at load Time, provides sharead access to global heap.
*
*          hevInitCompleted -- Ensures DLL Intialization is complete
*
*          lPost          -- Posting frequency  of above
*
*          Heap          -- Global Heap. All instance Data is allocated off this
*                           heap.
*
*          DataFormat[]. -- Time Format Conversions are driven from this table.
*                           It also defines the known data types and associated
*                           Information.
*
*   DEPENDENCIES:
*
*   EXTERNAL ENTRY POINTS: None.
*   INTERNAL ENTRY POINTS: None.
*
*   EXTERNAL REFERENCES (system):
*
*   MODIFICATION HISTORY:
*   DATE        DEVELOPER           CHANGE DESCRIPTION
*****************************************************************************/
#define INCL_BASE
#define INCL_DOSSEMAPHORES

#include <os2.h>                        // OS/2 Include.
#include <os2medef.h>                   // MME includes files.
#include <ssm.h>

#include <hhpheap.h>                    // Heap Manager Definitions

#pragma data_seg(SHR_SEG)


     /***********************************
     * Global Variables Used
     ***********************************/

     int                 UserCount =  0;         // Process Count.
     ULONG               lPost = 0;              // Posting Freq
     HMTX                hmtxProcSem = 0;        // Access Semaphore.
     HHUGEHEAP           heap = 0;           // Global Heap
     ULONG               ulAcquireMessage = 0;

     HID                 hidASource = 0;        // File system source/target
     HID                 hidATarget = 0;
     HID                 hidA2Source = 0;        // Memory source/target stream handlers
     HID                 hidA2Target = 0;
     HID                 hidBSource = 0;         // Audio source/target stream handlers
     HID                 hidBTarget = 0;

//     HAB                 hab;
//     HMQ                 hmq;
//     QMSG                qmsg;
//     HWND                hwndClipWin;               /* Clipboard win handle */

     ULONG               hModuleHandle;          // Module handle for dll
