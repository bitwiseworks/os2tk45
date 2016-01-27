/***************************************************************************\
*
* Module Name: MIDIDLL.H
*
* OS/2 Warp. Multimedia Extensions - MIDI
*
* Copyright (c) International Business Machines Corporation 1995
*                         All Rights Reserved
*
\***************************************************************************/
#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _MIDIDLL_H_
#define _MIDIDLL_H_
typedef ULONG        MINSTANCE;
typedef MINSTANCE    *PMINSTANCE;
typedef PVOID        TIMEHANDLE;
typedef TIMEHANDLE   *PTIMEHANDLE;
typedef PULONG       *PPULONG;

#define MIDI_NAME_LENGTH      32
#define MIDI_SUCCESS          0L
#define TIMER_SUCCESS         0L // return code of high-resolution timer

// used with MIDIEnableInstance and MIDIDisableInstance

#define MIDI_ENABLE_RECEIVE   1  // enables hdw to receive messages
#define MIDI_ENABLE_SEND      2  // enables hdw to send messages
#define MIDI_DISABLE_RECEIVE  1  // disables hdw from receiving messages
#define MIDI_DISABLE_SEND     2  // disables hdw from sending messages

// used with MIDITimer

// MIDI_START_TIMER and MIDI_STOP_TIMER flag are mutually exclusive

#define MIDI_START_TIMER      1  // starts MIDI Timer
#define MIDI_STOP_TIMER       2  // stops MIDI Timer

typedef struct {
   ULONG ulSourceInstance; // inst # of the instance which generated the msg
   ULONG ulTime;           // time the message is to be (or was) sent
   ULONG ulTrack;
   union {
      ULONG ulMessage;
      struct {
         BYTE bStatus;     // The 1st byte of the message
         BYTE abData[3];   // the rest of the message
      } bytes;
      BYTE abData[4];
   } msg;
} MESSAGE;

typedef MESSAGE   *PMESSAGE;

typedef struct _MIDICLASSINFO {
   ULONG       ulStructLength;
   ULONG       ulClassNumber;
   CHAR        szmClassName[MIDI_NAME_LENGTH];
   ULONG       ulNumSlots;
   ULONG       ulAttributes;
} MIDICLASSINFO;

typedef MIDICLASSINFO    *PMIDICLASSINFO;

typedef struct _MIDIINSTANCEINFO {
   ULONG       ulStructLength;
   MINSTANCE   minstance;
   ULONG       ulClassNumber;
   CHAR        szmInstanceName[MIDI_NAME_LENGTH];
   ULONG       ulNumLinks;
   ULONG       ulAttributes;
} MIDIINSTANCEINFO;

typedef MIDIINSTANCEINFO    *PMIDIINSTANCEINFO;

// MIDIINSTANCEINFO ulAttributes values:

#define MIDI_INST_ATTR_CAN_RECV     1  // instance can accept messages
#define MIDI_INST_ATTR_CAN_SEND     2  // instance can send messages
#define MIDI_INST_ATTR_ENABLE_R     4  // receive is enabled
#define MIDI_INST_ATTR_ENABLE_S     8  // send is enabled


typedef (*PFNMIDI_NOTIFYCALLBACK) (ULONG ulNotification,
                                   ULONG ulParam);

// typedef (VOID *PFNMIDI_NOTIFYCALLBACK) (ULONG ulNotification,
//                                         ULONG ulParam);

typedef struct _MIDISETUP {
   ULONG                      ulStructLength;
   PULONG                     pulMaxRTSysexLength;
   PPULONG                    ppulMIDICurrentTime;
   ULONG                      ulFlags;
   PFNMIDI_NOTIFYCALLBACK     pfnMIDI_NotifyCallback;
   HWND                       hwndCallback;
   HQUEUE                     hqueueCallback;
} MIDISETUP;

typedef MIDISETUP    *PMIDISETUP;

// MIDISETUP ulFlags values:

#define  MIDICALLBACK_PFN     1
#define  MIDICALLBACK_HWND    2
#define  MIDICALLBACK_QUEUE   3


typedef struct _MIDIDEVICEINFO {
   ULONG       ulStructLength;
   MINSTANCE   minstance;
   CHAR        szmInstanceName[MIDI_NAME_LENGTH];
//   PMIDIDEVICEINFO pParentDevice;
//   PMIDIDEVICEINFO pSiblingDevice;
//   PMIDIDEVICEINFO pChildDevice;
   CHAR        szDeviceName[MIDI_NAME_LENGTH];
   CHAR        szManufacturer[MIDI_NAME_LENGTH];
   CHAR        szModelName[MIDI_NAME_LENGTH];
   ULONG       ulFlags;
   ULONG       ulChannels;
} MIDIDEVICEINFO;

typedef MIDIDEVICEINFO      *PMIDIDEVICEINFO;


// DEVICEINFO ulFlags values:

#define  MIDIDEV_IS_PATCHER         0x00000001
#define  MIDIDEV_IS_CONTROLLER      0x00000002
#define  MIDIDEV_IS_RECEIVER        0x00000004
#define  MIDIDEV_IS_MULTITIMBRAL    0x00000008
#define  MIDIDEV_SENDS_MTC          0x00000010
#define  MIDIDEV_RECEIVES_MTC       0x00000020
#define  MIDIDEV_SENDS_CLOCK        0x00000040
#define  MIDIDEV_RECEIVES_CLOCK     0x00000080
#define  MIDIDEV_SENDS_SYNC         0x00000100
#define  MIDIDEV_RECEIVES_SYNC      0x00000200
#define  MIDIDEV_SENDS_MMC          0x00000400
#define  MIDIDEV_RECEIVES_MMC       0x00000800

typedef struct _MIDILINKINFO {
   ULONG       ulStructLength;
   ULONG       ulSlotNumber;
   MINSTANCE   minstance;
} MIDILINKINFO;

typedef MIDILINKINFO     *PMIDILINKINFO;


// Function Prototypes

ULONG APIENTRY MIDICreateInstance ( ULONG      ulClassNumber,
                                    PMINSTANCE pminstance,
                                    PSZ        pszInstanceName,
                                    ULONG      ulFlag );

ULONG APIENTRY MIDIDeleteInstance ( MINSTANCE minstance,
                                    ULONG     ulFlag );

ULONG APIENTRY MIDIEnableInstance ( MINSTANCE minstance,
                                    ULONG     ulFlag );

ULONG APIENTRY MIDIDisableInstance ( MINSTANCE minstance,
                                     ULONG     ulFlag );

ULONG APIENTRY MIDIAddLink ( MINSTANCE minstanceSource,
                             MINSTANCE minstanceTarget,
                             ULONG     ulSlotNumber,
                             ULONG     ulFlag );

ULONG APIENTRY MIDIRemoveLink ( MINSTANCE minstanceSource,
                                MINSTANCE minstanceTarget,
                                ULONG     ulSlotNumber,
                                ULONG     ulFlag );

ULONG APIENTRY MIDISetDefaultHardwareNode ( PSZ   pszmInstanceName,
                                            ULONG ulFlag );

ULONG APIENTRY MIDISetInstanceConfiguration (MINSTANCE minstance,
                                             PVOID     pBuffer,
                                             ULONG     ulBufferLength,
                                             ULONG     ulFlag );

ULONG APIENTRY MIDISimpleOpen ( PMINSTANCE pminstance,
                                ULONG      ulFlag );

ULONG APIENTRY MIDISimpleClose ( MINSTANCE minstance,
                                 ULONG     ulFlag );

ULONG APIENTRY MIDIQueryClassList ( ULONG          ulNumClasses,
                                    PMIDICLASSINFO paClassInfo,
                                    ULONG          ulFlag );

ULONG APIENTRY MIDIQueryDeviceInformation ( ULONG           ulNumDevices,
                                            PMIDIDEVICEINFO paDeviceInfo,
                                            ULONG           ulFlag );

ULONG APIENTRY MIDIQueryInstanceConfiguration ( ULONG minstance,
                                                PVOID pBuffer,
                                                ULONG ulBufferLength,
                                                ULONG ulFlag );

ULONG APIENTRY MIDIQueryInstanceLinks ( MINSTANCE     minstance,
                                        PMIDILINKINFO paLinkInfo,
                                        ULONG         ulNumLinks,
                                        ULONG         ulFlag );

ULONG APIENTRY MIDIQueryInstanceList ( ULONG             ulNumInstances,
                                       PMIDIINSTANCEINFO paInstanceInfo,
                                       ULONG             ulFlag );

ULONG APIENTRY MIDIQueryInstanceName ( MINSTANCE minstance,
                                       PSZ       pszInstanceName,
                                       ULONG     ulFlag );

ULONG APIENTRY MIDIQueryNumClasses ( PULONG pulNumClasses,
                                     ULONG  ulFlag );

ULONG APIENTRY MIDIQueryNumDevices ( PULONG pulNumDevices,
                                     ULONG  ulFlag );

ULONG APIENTRY MIDIQueryNumInstances ( PULONG pulNumInstances,
                                       ULONG  ulFlag );

ULONG APIENTRY MIDIQueryVersion ( PULONG pulVersion);

ULONG APIENTRY MIDISetDeviceInformation ( MINSTANCE       minstance,
                                          PMIDIDEVICEINFO pDeviceInfo,
                                          ULONG           ulFlag );

ULONG APIENTRY MIDISendMessages ( PMESSAGE paMessage,
                                  ULONG    ulNumMessages,
                                  ULONG    ulFlag );

ULONG APIENTRY MIDISendSysexMessage ( PMESSAGE pMessage,
                                      ULONG    ulBufferLength,
                                      ULONG    ulFlag );

ULONG APIENTRY MIDIRetrieveMessages ( MINSTANCE minstance,
                                      PVOID     pBuffer,
                                      PULONG    pulBufferLength,
                                      ULONG     ulFlag );

ULONG APIENTRY MIDISetup ( PMIDISETUP pMidiSetup,
                           ULONG      ulFlag );

ULONG APIENTRY MIDITimer ( ULONG ulAction,
                           ULONG ulFlag );

ULONG APIENTRY TimerSleep ( ULONG ulDuration,
                            ULONG ulFlag );

ULONG APIENTRY TimerGetPointer ( PPULONG ppulHRTCurrentTime,
                                 ULONG   ulFlag );

#endif /* _MIDIDLL_H_ */

#ifdef __cplusplus
}
#endif

