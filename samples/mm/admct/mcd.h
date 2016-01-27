/**************************START OF SPECIFICATIONS **************************
*                                                                          *
* Module NAME:  MCD.H                                                      *
*                                                                          *
*                                                                          *
* OS/2 2.x Multimedia Extensions MCD structures and definitions            *
*                                                                          *
* Copyright (c) International Business Machines Corporation 1990,1991      *
*                         All Rights Reserved                              *
*                                                                          *
* The logical MCI Amp-mixer device is divided into two portions:           *
*                                                                          *
*   AMPMXMCD.DLL  << Device Independent >>                                 *
*   xxxxxxxx.DLL  << Vendor Specific DLL>                                  *
*                                                                          *
* These two DLLs communicate via a shared instance which is passed in      *
* on the VSD open in the pDevInfo field of the VSD_OPEN parms.  MCD.H      *
* contains the definition of this shared instance.                         *
*                                                                          *
* This file contains a shared instance structure which is passed to an     *
* audio VSD from the amp-mixer.  It is suggested that you use this         *
* structure as your instance structure, however it is not required.  What  *
* is required is that the fields indicated by REQUIRED by kept up to date. *
*                                                                          *
* The following fields must always be kept accurate:                       *
*                                                                          *
* Mixer related fields:                                                    *
*                                                                          *
*        LONG lLeftVolume;                                                 *
*        LONG lBalance;                                                    *
*        LONG lBass;                                                       *
*        LONG lTreble;                                                     *
*        LONG lPitch;                                                      *
*        ULONG ulMasterVolume;                                             *
*        LONG lMonitor;                                                    *
*        ULONG ulGainLevel;                                                *
*                                                                          *
* AUDIO Mode fields                                                        *
*                                                                          *
*        LONG lSRate;                                                      *
*        LONG lBitsPerSRate;                                               *
*        SHORT sMode;                                                      *
*        SHORT sChannels;                                                  *
*        ULONG ulOperation;                                                *
*                                                                          *
*                                                                          *
*                                                                          *
*       ULONG ulBestFitBPS;                                                *
*       ULONG ulBestFitChan;                                               *
*       ULONG ulBestFitRate;                                               *
*       SHORT sBestFitMode;                                                *
*                                                                          *
*        ULONG ulDataType;                                                 *
*        ULONG ulSubType;                                                  *
*                                                                          *
* Capability fields                                                        *
*                                                                          *
*        ULONG ulCanRecord;                                                *
*                                                                          *
*                                                                          *
*                                                                          *
* If you are writing an AUDIODD VSD--then the following must be accurate:  *
*                                                                          *
*       HFILE hFile;                                                       *
*       USHORT usDeviceID;                                                 *
*       ULONG ulGlobalFile;                                                *
*                                                                          *
*                                                                          *
************************** END OF SPECIFICATIONS ***************************/

#ifndef _SSM_H_
#define _SSM_H_
#include <ssm.h>
#include <dcb.h>
#endif


#define INCL_SEMAPHORES
#define LIST_LEN           8

#define LINE_IN_ON                              0x00000001
#define MIC_ON                                  0x00000002
#define LINE_OUT_ON                             0x00000004


#define VSD_SET_VOLUME          0x00000001
#define VSD_SET_TREBLE          0x00000002
#define VSD_SET_BASS            0x00000004
#define VSD_SET_PITCH           0x00000008
#define VSD_SET_GAIN            0x00000010
#define VSD_SET_CONN            0x00000020
#define VSD_SET_MONITOR         0x00000040
#define VSD_SET_BALANCE         0x00000080
#define VSD_SET_MASTER          0x00000100
#define VSD_SET_ALL             0xffffffff

#define MIXER_MULTIPLIER        0xffff / 100 // mixer ranges--0 to ffff
                                             // MCI 0 - 100.  Map ranges

#define REAL_MIXER              0x00000001
/*-------------------------------------------------------------------------*
* RIFF to ACPA Mode conversion Table
*-------------------------------------------------------------------------*/

typedef struct
        {
        ULONG  ulDataType;
        ULONG  ulAUDIODDMode;
        } RIFFTABLE;


typedef struct _VSD_MIXER {
   PVOID        hvsd;       /* Handle to the vsd */
   ULONG        hMix;       /* Handle to the mixer device */
   ULONG        hConn;
   ULONG        ulState;    /* State flag--who opened the device */
   ULONG        ulMasterTreble;
   ULONG        ulMasterBass;
   ULONG        ulMasterBalance;
   ULONG        ulMasterLoudness;
   BOOL         fHardwareMix;
   } VSD_MIX;

typedef VSD_MIX  *PVSD_MIX;


#pragma pack(4)
typedef struct _portlist {   /* devid */
                       ULONG ulDevType;
                       ULONG  ulDevNum;
                      } MCI_PORT_LIST;
#pragma pack()


/*-------------------------------------------------------------------------*
* Amp/Mixer instance structure declaration
*-------------------------------------------------------------------------*/

#pragma pack(4)

typedef struct _MCI_AMP_INSTANCE
        {
        CHAR szDeviceName[MAX_PDD_NAME];  // INPUT--name of associated device driver
                                          // used by AUDIODD VSD to figure out which
                                          // DD to open.  VSDs for specific hardaware
                                          // don't need this.
        CHAR szDriverName[MAX_DEVICE_NAME]; // INPUT--NAME of device driver in MMPM2.INI
        CHAR szProdInfo[MAX_PRODINFO];    // INPUT --product name

        HFILE hFile;                      // DosOpen FIle handle.  If streaming is used
                                          // this field must be filled in.
        USHORT usDeviceID;                // INPUT--Device id of mixer instance
        ULONG ulGlobalFile;               // OUTPUT--from DosOpen, Global Filenumber
                                          // this number is a unique identifier which
                                          // should be created for each instance of the
                                          // VSD.

        USHORT usMasterVolume;            // UNUSED
        LONG lLeftVolume;                 // INPUT/OUPUT--status of volume
        LONG  ( * APIENTRY pfnNewVSD) ( PVOID, LONG, LONG, PVOID );  // INPUT--ptr to VSD entry point.

        LONG lVolumeDelay;                // VSD PRIVATE--variable used to indicate the
                                          // delay effect for volume
        LONG lBalance;                    // INPUT/OUPUT--status of volume
        LONG lBalanceDelay;               // VSD PRIVATE--variable used to indicate the
                                          // delay effect for volume
        LONG lBass;                       // INPUT/OUPUT--status of bass setting
        LONG lTreble;                     // INPUT/OUPUT--status of treble
        LONG lPitch;                      // INPUT/OUPUT--status of pitch

        LONG lSRate;                      // INPUT/OUPUT--sampling rate
        LONG lBitsPerSRate;               // INPUT/OUPUT--bits/sample
        ULONG fConnected;                 // AMP-mixer--state variable.
        SHORT sMode;                      // INPUT/OUPUT--RIFF MODE to change device to
        SHORT sChannels;                  // INPUT/OUPUT--channels to use
        ULONG ulFlags;                    // Private field for VSD
        LONG lMonitor;                    // INPUT/OUPUT--is input monitoring on?
        ULONG ulAmpCaps;                  // Amp capability flags
        LONG lOutput;                     // Unused
        ULONG ulOperation;                // INPUT/OUPUT--Either OPERATION_PLAY or
                                          // OPERATION_RECORD << Audio.H >>
        ULONG ulGainLevel;                // INPUT/OUPUT--status of gain

        ULONG ulDosOpened;                // VSD private
                                          // in Template--this indicates if DosOpen was performed

        ULONG ulInitOpen;                 // VSD private
                                          // in Template--this indicates if AUDIO_INIT was called
        ULONG ulMasterVolume;             // INPUT/OUPUT--status of master volume
        ULONG ulActive;                   // INPUT/OUPUT
                                          // in Template--this BOOLEAN indicates if the
                                          // VSD is currently using the card.  If VSD_SAVE is
                                          // called, ulActive will be set to false.

        ULONG ulBestFitBPS;               // OUTPUT--if vsd can't do an AUDIO_INIT
        ULONG ulBestFitChan;              // OUTPUT--it sets the best fit fields up
        ULONG ulBestFitRate;              // OUTPUT--with the appropriate values
        SHORT sBestFitMode;               // OUTPUT
        SHORT sStoreChannels;             // VSD private
        ULONG ulSetAmpDefaults;           // VSD private
        ULONG ulDefaultsSet;              // VSD private
        ULONG ulOutputDev;                // VSD private
        ULONG ulInputDev;                 // VSD private
        ULONG ulResourcesUsed;            // VSD private
        ULONG ulRestoreNotify;            // VSD private
        ULONG ulCallback;                 // VSD private
        SHORT sUserParm;                  // VSD private
        LONG  lNotifyFlag;                // VSD private

        LONG  ( * APIENTRY pfnAUDIOIF) ( PVOID, LONG, LONG, PVOID );

        HMTX  hmtxInstanceSem;            // VSD private
        ULONG ulClass;                    // VSD private
        ULONG ulBlockAlignment;           // VSD private
        ULONG ulHardwareMode;             // VSD private
        MCI_PORT_LIST  rInputList [ LIST_LEN ];  // VSD private
        MCI_PORT_LIST  rOutputList[ LIST_LEN ];  // VSD private
        ULONG ulNumInputs;                // VSD private
        ULONG ulNumOutputs;               // VSD private
        ULONG ulDataType;                 // INPUT/OUTPUT--RIFF Datatype
        ULONG ulSubType;                  // INPUT/OUTPUT--RIFF Subtype
        ULONG ulBytesPerBlock;            //  Unused--excess field
        ULONG ulMMTimePerBlock;           //  Unused--excess field
        ULONG ulCanRecord;                // VSD private
        ULONG ulVolControl;               // VSD private

        PVOID    pAudioModeData;      // pointer to dev. specific resource data
        CHAR     pResourceDLL[ 128 ]; // INPUT--mixer fills in name of RESOURCE DLL (if any)
                                      // for the VSD.

        PSZ        pProdInfo;         // pointer to the product name
        ULONG      ulDeviceID;        // pointer to the device id.
        ULONG      ulNumMappingRows;  // number of RIFF->AUDIODD mapping modes
        ULONG      ulMatch;           // Private field for VSD
        RIFFTABLE  *pMapTable;        // additional RIFF->AUDIODD mappings

        ULONG      ulNumDataTypes;    // number of datatypes to check
        PVOID      pDataTypeTable;    // Private field for VSD
        BOOL       fMute;             // Private field for VSD
        ULONG      ulResourceID;      // offset in the resource dll to use
        ULONG      ulConnDefaults;    // Default values for connectors
        BOOL       fSSMState;         // Private field for VSD
        ULONG      hStream;           // Private field for VSD
        ULONG      ulLine;            // Current input line
        PVOID      hvsd;              /* Handle to the vsd */
        ULONG      hMix;              /* Handle to the mixer device */
        BOOL       fHardwareMix;      // Private field for VSD
        ULONG      ulState;           // not sure what this does anymore
        BOOL       fDriverCaps;       // Private field for VSD
        VSD_DCB    vsddcb;            /* Device Control Block for streaming */
        SPCBKEY    spcbkey;           /* Streaming key which describes mixer mode */
        } MCI_AMP_INSTANCE;



typedef MCI_AMP_INSTANCE FAR *PMCI_AMP_INSTANCE;

typedef MCI_AMP_INSTANCE  *PVSD_INSTANCE;
typedef MCI_AMP_INSTANCE  VSD_INSTANCE;
#pragma pack()


/*********************************************************************/
/*                                                                   */
/*  CONCB prototype                                                  */
/*                                                                   */
/*********************************************************************/

typedef struct _CONCB {
  ULONG    ulStructLength;
  HID      hid;
  PDCB     pdcb;
  PSPCBKEY pspcbkey;
  PEVCB    pevcb;
  PEVFN    pevfn;
  ULONG    ulFlags;
  HSTREAM  hstreamAssoc;
  HSTREAM  hstream;
  PVOID    pMediaHeader;
  ULONG    ulConnectorType;
  ULONG    ulConnectorIndex;
  struct _CONCB   *pConnectToConCB;
} CONCB;
typedef CONCB  *PCONCB;
