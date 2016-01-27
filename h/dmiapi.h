/**********************************************************************
    Filename: dmiapi.h

    Copyright (c) 1992,1993,1994, Intel Corporation.  All rights reserved.
    Copyright (c) 1993 - 1996, International Business Machines, Corporation. All rights reserved.

    Description: Header for DMI procedural library


**********************************************************************/

#ifndef DMILIB_H_FILE
#define DMILIB_H_FILE

#ifdef __cplusplus
extern "C" {
#endif

/************************ INCLUDES ***********************************/

#include "os_dmi.h"

/*********************************************************************/

/************************ DEFINES ************************************/

#define DMILIB_ERROR                              0x100
#define FILE_ERROR                                0x200
#define SL_ERROR                                  0x300
#define SYSTEM_ERROR                              0x400

#define DmiLibDirInstallNoError                   1
#define DmiLibSlInstallNoError                    2

#define DmiLibIllegalFileType                     DMILIB_ERROR + 1

#define DmiLibCannotCloseDestinationFile          FILE_ERROR   + 1
#define DmiLibCannotCreateDestinationFile         FILE_ERROR   + 2
#define DmiLibCannotCreateDirectory               FILE_ERROR   + 3
#define DmiLibCannotOpenSourceFile                FILE_ERROR   + 4
#define DmiLibCannotReadSourceFile                FILE_ERROR   + 5
#define DmiLibCannotWriteDestinationFile          FILE_ERROR   + 6

#define DmiLibCannotExecuteInstallCommand         SL_ERROR     + 1
#define DmiLibCannotRunServiceLayer               SL_ERROR     + 2

#define DmiLibCannotAllocateMemory                SYSTEM_ERROR + 1
#define DmiLibCannotSetDmiDirEnvironmentVariable  SYSTEM_ERROR + 2
#define DmiLibDmiDirEnvironmentVariableNotDefined SYSTEM_ERROR + 3

/*********************************************************************/

/************************ TYPEDEFS ***********************************/

typedef enum {MifFileName = 2, MifFilePointer} DmiLibFileType_t;
typedef enum {DmiLibFalse, DmiLibTrue} DmiLibBoolean_t;

typedef struct {
    DMI_UNSIGNED    iFileType;
    DMI_STRING _FAR *pFileData;
} DmiLibFileData_t;

typedef struct {
    DMI_UNSIGNED iComponentId;
    DMI_UNSIGNED iDmiLibStatus;
    DMI_UNSIGNED iSlStatus;
} DmiLibInstallData_t;

/*
    The DmiCiAttribute_t structure is used as a communication vehicle between
    DmiCiProcess(), and the user-provided attribute access routines (CiSkel.c).
    An array of DmiCiAttribute_t structures is used to contain a key list, and  a single
    DmiCiAttribute_t object is used to pass attribute values  from the user-provided
    attribute access routines back to DmiCiProcess().
*/

typedef struct {
    DMI_UNSIGNED iAttributeId;
    DMI_UNSIGNED iAttributeType;
    void _FAR *pAttributeValue;
} DmiCiAttribute_t;

enum CiBoolean {CiFalse, CiTrue};

typedef DmiCiAttribute_t *(_FAR DMI_FUNC_ENTRY _CI_FUNC_IN1)(DMI_UNSIGNED,DMI_UNSIGNED,DMI_UNSIGNED,DMI_UNSIGNED,DmiCiAttribute_t const _FAR *const);
typedef _CI_FUNC_IN1 *CI_FUNC_IN1;
typedef enum CiBoolean (_FAR DMI_FUNC_ENTRY _CI_FUNC_IN2)(DMI_UNSIGNED,DMI_UNSIGNED,DMI_UNSIGNED,DMI_UNSIGNED,DmiCiAttribute_t const _FAR *const,void const _FAR *const);
typedef _CI_FUNC_IN2 *CI_FUNC_IN2;

typedef struct {
    union {
        struct DmiMgmtCommand     dmiMgmtCommand;
        struct DmiGetAttributeReq dmiGetAttributeReq;
        struct DmiSetAttributeReq dmiSetAttributeReq;
        struct DmiGetRowReq       dmiGetRowReq;
        char                      requestBuffer[1];
    } _FAR                *pRequestBuffer;   /* this commands request buffer */
    union {
        struct DmiGetAttributeCnf dmiGetAttributeCnf[1];
        struct DmiGetRowCnf       dmiGetRowCnf;
        char                      confirmBuffer[1];
    } _FAR                *pConfirmBuffer;   /* this commands confirm buffer */
    DMI_FUNC3_OUT          DmiConfirmFunc;   /* this commands confirm entry point */
    enum CiBoolean        ciCancelFlag;      /* this flag is set when the SL requests that this command be conceled */
    struct DmiConfirm     dmiConfirm;        /* confirm data block used for this command */
    DmiCiAttribute_t _FAR *ciKeyList;        /* CISKEL.C will fill this in */
    DMI_UNSIGNED          MaxKeyCount;       /* maximum number of keys allowed in this component */
    CI_FUNC_IN1           CiGetAttribute;
    CI_FUNC_IN1           CiGetNextAttribute;
    CI_FUNC_IN2           CiReleaseAttribute;
    CI_FUNC_IN2           CiReserveAttribute;
    CI_FUNC_IN2           CiSetAttribute;
} DmiCiControl_t;

/*********************************************************************/

/************************ PUBLIC *************************************/

/* MI Invoke entry point. */
DMI_UNSIGNED DMI_FUNC_ENTRY DmiInvoke(void _FAR *dmiCommand);

/* CI Indication call entry point. */
DMI_UNSIGNED DMI_FUNC_ENTRY DmiIndicate(DMI_Indicate_t _FAR *dmiIndication);

/* CI instrumentation code entry points. */
DMI_UNSIGNED DMI_FUNC_ENTRY DmiCiInvoke(void _FAR *dmiCommand);
DMI_UNSIGNED DMI_FUNC_ENTRY DmiCiCancel(void);

/* DMIAPI.LIB library function prototypes. */
DmiLibInstallData_t _FAR * DMI_FUNC_ENTRY DmiInstall(DMI_UNSIGNED iFileCount,
                                                      DmiLibFileData_t _FAR *dmiLibFileList,
                                                      DMI_STRING _FAR *pDmiDir,
                                                      DmiLibBoolean_t iDefineDmiDir,
                                                      DMI_FUNC_OUT StatusCall);
/* StatusCall is a callback entry point that the SL will call with the MIF compiler messages. */
/* the prototype is DMI_UNSIGNED DMI_FUNC_CALLBACK StatusCall(void _FAR *message) */
/* where "message" is a null terminated string                            */

DMI_UNSIGNED DMI_FUNC_ENTRY DmiCiProcess(DmiCiControl_t _FAR *Command, void _FAR *dmiCommand);

/****************************************************************************/
#ifdef __cplusplus
}
#endif

#endif
