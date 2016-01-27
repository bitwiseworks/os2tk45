/****************************************************************************/
/*                                                                          */
/* Module Name: MCIPRIV.H                                                   */
/*                                                                          */
/* This is the multimedia include file that has the typedefs, defines and   */
/* function prototyes for MCI not publicly available in Release 1           */
/*                                                                          */
/* Copyright (c) International Business Machines Corporation 1990, 1991     */
/*                        All Rights Reserved                               */
/*                                                                          */
/****************************************************************************/

/*********************************************************************/
/* Flags for mciConnection                                           */
/*********************************************************************/
#define MCI_MAKECONNECTION              0x0001
#define MCI_BREAKCONNECTION             0x0002


#pragma pack(1)

typedef struct {
    DWORD   dwDeviceTypeID1;
    WORD    wConnectorIndex1;
    DWORD   dwDeviceTypeID2;
    WORD    wConnectorIndex2;
} DEFAULTCONNECTIONS;
typedef DEFAULTCONNECTIONS   *LPDEFAULTCONNECTIONS;

typedef struct {
    DWORD   dwDeviceTypeID1;
    WORD    wConnectorIndex1;
    WORD    wConnectorType1;
    DWORD   dwDeviceTypeID2;
    WORD    wConnectorIndex2;
    WORD    wConnectorType2;
} DEFAULTCONNECTIONS2;
typedef DEFAULTCONNECTIONS2   *PDEFAULTCONNECTIONS2;


typedef struct {
    WORD    wDeviceID1;
    WORD    wConnectorIndex1;
    WORD    wDeviceID2;
    WORD    wConnectorIndex2;
} CONNECTIONS;
typedef CONNECTIONS   *LPCONNECTIONS;

/*********************************************************************/
/* parameters for the MCI_SYSINFO message                            */
/*     0xXXXXXX00 are reserved for MCI_SYSINFO flags                 */
/*********************************************************************/
    /********************************************************/
    /* Extended SYSINFO functions                           */
    /********************************************************/
    #define MCI_SYSINFO_FIND_LOGDEVICE             0x00008000L

typedef struct {
    LPSTR   lpstrFileName;                        /* File Name                   */
    CHAR    szLogicalName[MAX_DEVICE_NAME];       /* returned logical device name*/
} MCI_SYSINFO_FN_LOGDEVICE;
typedef MCI_SYSINFO_FN_LOGDEVICE *LPMCI_SYSINFO_FN_LOGDEVICE;

#pragma pack()
/*********************************************************************/
/*                                                                   */
/*  MCI Function prototypes                                          */
/*                                                                   */
/*********************************************************************/

DWORD APIENTRY mciQueryDefaultConnections  (DWORD   dwDeviceTypeID,
                                            PVOID   pDefaultConnections,
                                            WORD    *wLength);

DWORD APIENTRY mciQueryConnections  (WORD    wDeviceID,
                                     WORD    wConnectorIndex,
                                     PVOID   pConnections);

DWORD APIENTRY mciDefaultConnection   (DWORD  dwDeviceTypeID1,
                                       WORD   wConnectorIndex1,
                                       DWORD  dwDeviceTypeID2,
                                       WORD   wConnectorIndex2,
                                       WORD   wAction);

DWORD APIENTRY mciConnection  (WORD    wDeviceID1,
                               WORD    wConnectorIndex1,
                               WORD    wDeviceID2,
                               WORD    wConnectorIndex2,
                               WORD    wAction);

