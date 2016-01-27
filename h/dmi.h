/****************************************************************************************************************************************************
    Filename: dmi.h

    Copyright (c) 1992,1993,1994, Intel Corporation.  All rights reserved.
    Copyright (c) 1993 - 1996, International Business Machines, Corporation. All rights reserved.

    Description: Desktop Management Interface Header


****************************************************************************************************************************************************/


/******************************************************************** INCLUDES ********************************************************************/

#ifndef DMI_H_FILE
#define DMI_H_FILE

#include "os_dmi.h"

/***************************************************************************************************************************************************/

/******************************************************************** DEFINES *********************************************************************/
/* The below spells "DM11" (left to right order) */
#define DMI_LEVEL_CHECK 0x444D3131

/* The below spells "DMI1" (left to right order)  NOTE: this is the version check string for DMI Version 1.0 */
#define DMI_LEVEL_CHECK_V1 0x444D4931

/***************************************************************************************************************************************************/

/*************************************************************** MIF DATABASE DEFINES **************************************************************/

/* iAttributeAccess Defines */
#define MIF_UNKNOWN_ACCESS              0
#define MIF_READ_ONLY                   1
#define MIF_READ_WRITE                  2
#define MIF_WRITE_ONLY                  3
#define MIF_UNSUPPORTED                 4
#define MIF_UNKNOWN             5

/* Masks for access mode */
#define MIF_COMMON_SPECIFIC_MASK 0x80000000
#define MIF_ACCESS_MODE_MASK     ~MIF_COMMON_SPECIFIC_MASK

/* Return values from List Component Command */
#define MIF_LIST_COMPONENT_MATCH                        0
#define MIF_LIST_COMPONENT_POSSIBLE_MATCH       1

/* iAttributeType Defines */
#define MIF_UNKNOWN_DATA_TYPE   0
#define MIF_COUNTER                             1
#define MIF_COUNTER64                   2
#define MIF_GAUGE                               3
#define MIF_INTEGER                             5
#define MIF_INT                                 MIF_INTEGER
#define MIF_INTEGER64           6
#define MIF_INT64               MIF_INTEGER64
#define MIF_OCTETSTRING                 7
#define MIF_DISPLAYSTRING               8
#define MIF_STRING                              MIF_DISPLAYSTRING
#define MIF_DATE                                11

/* iFileType defines */
#define MIF_RESERVED1_FILE_TYPE                                 0
#define MIF_RESERVED2_FILE_TYPE                                 1
#define MIF_MIF_FILE_NAME_FILE_TYPE                             2
#define MIF_MIF_FILE_POINTER_FILE_TYPE                  3
#define MIF_SNMP_MAPPING_FILE_NAME_FILE_TYPE    4
#define MIF_SNMP_MAPPING_FILE_POINTER_FILE_TYPE 5

/****************************************************************************************************************************************************/

/******************************************************************** DMI COMMANDS ******************************************************************/

#define DMI_MISCELLANEOUS_COMMANDS      0x00000100
#define DMI_LIST_COMMANDS                       0x00000200
#define DMI_GET_SET_COMMANDS            0x00000300
#define DMI_CI_COMMANDS                         0x00000400
#define DMI_PRIVATE_COMMAND_MASK        0x80000000

#define DMI_UNKNOWN_COMMAND             0x00000000

/* Miscellaneous commands */
#define DmiRegisterMgmtCmd                      DMI_MISCELLANEOUS_COMMANDS + 0x00
#define DmiUnregisterMgmtCmd            DMI_MISCELLANEOUS_COMMANDS + 0x01
#define DmiCancelCmd                            DMI_MISCELLANEOUS_COMMANDS + 0x02

/* List commands */
#define DmiListComponentCmd                     DMI_LIST_COMMANDS                       + 0x00
#define DmiListFirstComponentCmd        DMI_LIST_COMMANDS                       + 0x01
#define DmiListNextComponentCmd         DMI_LIST_COMMANDS                       + 0x02
#define DmiListComponentDescCmd         DMI_LIST_COMMANDS                       + 0x03
#define DmiListGroupCmd                         DMI_LIST_COMMANDS                       + 0x04
#define DmiListFirstGroupCmd            DMI_LIST_COMMANDS                       + 0x05
#define DmiListNextGroupCmd                     DMI_LIST_COMMANDS                       + 0x06
#define DmiListGroupDescCmd                     DMI_LIST_COMMANDS                       + 0x07
#define DmiListAttributeCmd                     DMI_LIST_COMMANDS                       + 0x08
#define DmiListFirstAttributeCmd        DMI_LIST_COMMANDS                       + 0x09
#define DmiListNextAttributeCmd         DMI_LIST_COMMANDS                       + 0x0a
#define DmiListAttributeDescCmd         DMI_LIST_COMMANDS                       + 0x0b
#define DmiListGroupPragmaCmd       DMI_LIST_COMMANDS           + 0x0c

/* Get and Set commands */
#define DmiGetAttributeCmd                      DMI_GET_SET_COMMANDS            + 0x00
#define DmiSetAttributeCmd                      DMI_GET_SET_COMMANDS            + 0x01
#define DmiSetReserveAttributeCmd       DMI_GET_SET_COMMANDS            + 0x02
#define DmiSetReleaseAttributeCmd       DMI_GET_SET_COMMANDS            + 0x03
#define DmiGetRowCmd                            DMI_GET_SET_COMMANDS            + 0x04
#define DmiGetFirstRowCmd                       DMI_GET_SET_COMMANDS            + 0x05
#define DmiGetNextRowCmd                        DMI_GET_SET_COMMANDS            + 0x06

/* CI commands */
#define DmiRegisterCiCmd                        DMI_CI_COMMANDS                         + 0x00
#define DmiUnregisterCiCmd                      DMI_CI_COMMANDS                         + 0x01
#define DmiCiInstallCmd                         DMI_CI_COMMANDS                         + 0x02
#define DmiCiUninstallCmd                       DMI_CI_COMMANDS                         + 0x03

/* File Types for DmiFileData */
#define DMI_MIF_FILE_NAME                       2
#define DMI_MIF_FILE_POINTER            3
#define DMI_SNMP_MAP_FILE_NAME          4
#define DMI_SNMP_MAP_FILE_POINTER       5

/* Indication types */
#define DMI_EVENT_INDICATION                    1
#define DMI_INSTALL_INDICATION                  2
#define DMI_UNINSTALL_INDICATION                3
#define DMI_INSTALL_LANGUAGE_MAPPING    4

#define DMI_PRIVATE_INDICATION_MASK             0x80000000
/***************************************************************** DMI TYPES ************************************************************************/

/* Definitions used in structures */
typedef unsigned char           DMI_BYTE;
typedef signed long int         DMI_INT;
typedef unsigned long int       DMI_OFFSET;
typedef unsigned long int       DMI_UNSIGNED;

typedef DMI_UNSIGNED (_FAR DMI_FUNC_ENTRY _DMI_FUNC_IN)(void _FAR *);
typedef _DMI_FUNC_IN *DMI_FUNC_IN;
typedef DMI_UNSIGNED (_FAR DMI_FUNC_ENTRY _DMI_FUNC0_IN)(void);
typedef _DMI_FUNC0_IN *DMI_FUNC0_IN;

typedef DMI_UNSIGNED (_FAR DMI_FUNC_CALLBACK _DMI_FUNC_OUT)(void _FAR *);
typedef _DMI_FUNC_OUT *DMI_FUNC_OUT;
typedef DMI_UNSIGNED (_FAR DMI_FUNC_CALLBACK _DMI_FUNC0_OUT)(void);
typedef _DMI_FUNC0_OUT *DMI_FUNC0_OUT;

typedef void (_FAR DMI_FUNC_CALLBACK _DMI_FUNC3_OUT)(void _FAR *);
typedef _DMI_FUNC3_OUT *DMI_FUNC3_OUT;

typedef struct {
        DMI_UNSIGNED    length;
        char                    body[1];
} DMI_STRING;

/****************************************************************************************************************************************************/

/**************************************************************** DMI STRUCTURES ********************************************************************/

/* 4.2 DmiCiCommand */

typedef struct DmiCiCommand {
        DMI_OFFSET              oCmdListEntry;
        DMI_UNSIGNED    iCnfBufLen;
        void _FAR               *pCnfBuf;
        DMI_FUNC3_OUT   pConfirmFunc;
} DMI_CiCommand_t;


/* 4.1 DmiMgmtCommand, DmiSecurity */

typedef struct DmiMgmtCommand {
        DMI_UNSIGNED    iLevelCheck;
        DMI_UNSIGNED    iCommand;
        DMI_UNSIGNED    iCmdLen;
        DMI_UNSIGNED    iMgmtHandle;
        DMI_UNSIGNED    iCmdHandle;
        DMI_OFFSET              osLanguage;
        DMI_OFFSET              oSecurity;                         /* Offset to a DMI_Security_t */
        DMI_UNSIGNED    iCnfBufLen;
        void _FAR               *pCnfBuf;
        DMI_UNSIGNED    iRequestCount;
        DMI_UNSIGNED    iCnfCount;
        DMI_UNSIGNED    iStatus;
        DMI_CiCommand_t DmiCiCommand;
} DMI_MgmtCommand_t;


typedef struct DmiSecurity {
        DMI_UNSIGNED    iSecurityLen;
        DMI_UNSIGNED    iSecurityType;
        DMI_BYTE                SecurityData[1];                /* Unspecified length */
} DMI_Security_t;


/* 4.3 DmiConfirm */

typedef struct DmiConfirm {
        DMI_UNSIGNED    iLevelCheck;
        void _FAR               *pDmiMgmtCommand;
        DMI_UNSIGNED    iStatus;
} DMI_Confirm_t;


/* 4.7 DmiTimeStamp */

typedef struct DmiTimeStamp {
        DMI_BYTE sYear[4];
        DMI_BYTE sMonth[2];
        DMI_BYTE sDay[2];
        DMI_BYTE sHour[2];
        DMI_BYTE sMinutes[2];
        DMI_BYTE sSeconds[2];
        DMI_BYTE cDot;
        DMI_BYTE sMicroSeconds[6];
    DMI_BYTE cPlusOrMinus;
        DMI_BYTE sUtcOffset[3];
        DMI_BYTE iPadding[3];                   /* For alignment */
} DMI_TimeStamp_t;


/* 4.4 DmiIndicate */

typedef struct DmiIndicate {
        DMI_UNSIGNED            iLevelCheck;
        DMI_UNSIGNED            iIndicationType;
        DMI_UNSIGNED            iCmdLen;
        DMI_UNSIGNED            iComponentId;
        DMI_TimeStamp_t DmiTimeStamp;
        DMI_FUNC3_OUT           pResponseFunc;
        DMI_OFFSET                      oIndicationData;                        /* Offset to either a DMI_ListComponentCnf_t or a DMI_EventData_t */
} DMI_Indicate_t;


/* 4.5 DmiVersion */

typedef struct DmiVersion {
        DMI_OFFSET osDmiSpecLevel;
        DMI_OFFSET osImplDesc;
} DMI_Version_t;


/* 4.6 DmiGroupKeyData */

typedef struct DmiGroupKeyData {
        DMI_UNSIGNED    iAttributeId;
        DMI_UNSIGNED    iAttributeType;
        DMI_OFFSET              oKeyValue;
} DMI_GroupKeyData_t;


/* 5.3 DmiRegisterMgmtReq, DmiRegisterMgmtCnf */

typedef struct DmiRegisterMgmtReq {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_FUNC3_OUT           pConfirmFunc;
        DMI_FUNC3_OUT           pIndicationFunc;
} DMI_RegisterMgmtReq_t;

typedef struct DmiRegisterCnf {
        DMI_Version_t   DmiVersion;
        DMI_UNSIGNED    iDmiHandle;
} DMI_RegisterCnf_t;


/* 5.6 DmiListComponentReq, DmiListComponentCnf, DmiClassNameData, DmiFileData */

typedef struct DmiListComponentReq {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iComponentId;
        DMI_OFFSET                      osClassString;
        DMI_UNSIGNED            iGroupKeyCount;
        DMI_OFFSET                      oGroupKeyList;          /* Offset to DMI_GroupKeyDat_t GroupKeyList[iGroupKeyCount */
} DMI_ListComponentReq_t;

typedef struct DmiFileData {
        DMI_UNSIGNED    iFileType;
        DMI_OFFSET              oFileData;
} DMI_FileData_t;

typedef struct DmiListComponentCnf {
        DMI_UNSIGNED    iComponentId;
        DMI_UNSIGNED    iMatchType;
        DMI_OFFSET              osComponentName;
        DMI_UNSIGNED    iClassListCount;
        DMI_OFFSET              oClassNameList; /* Offset to DMI_ClassNameData_t ClassNameList[iClassListCount] */
        DMI_UNSIGNED    iFileCount;
        DMI_OFFSET              oDmiFileList;    /* Offset to DMI_FileData_t DmiFileList[iFileCount] */
} DMI_ListComponentCnf_t;

typedef struct DmiClassNameData {
        DMI_UNSIGNED    iGroupId;
        DMI_OFFSET              osClassString;
} DMI_ClassNameData_t;


/* 5.7 DmiListGroupReq, DmiListGroupCnf, DmiListGroupPragmaReq */

typedef struct DmiListGroupReq {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iComponentId;
        DMI_UNSIGNED            iGroupId;
} DMI_ListGroupReq_t;

typedef struct DmiListGroupCnf {
        DMI_UNSIGNED    iGroupId;
        DMI_OFFSET              osGroupName;
        DMI_OFFSET              osClassString;
        DMI_UNSIGNED    iGroupKeyCount;
        DMI_OFFSET              oGroupKeyList;          /* Offset to DMI_GroupKeyDat_t GroupKeyList[iGroupKeyCount */
} DMI_ListGroupCnf_t;

typedef struct DmiListGroupPragmaReq {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iComponentId;
        DMI_UNSIGNED            iGroupId;
    DMI_UNSIGNED        iOffset;
} DMI_ListGroupPragmaReq_t;

/* 5.8 DmiListAttributeReq, DmiListAttributeCnf, DmiEnumData */

typedef struct DmiListAttributeReq {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iComponentId;
        DMI_UNSIGNED            iGroupId;
        DMI_UNSIGNED            iAttributeId;
} DMI_ListAttributeReq_t;

typedef struct DmiListAttributeCnf {
        DMI_UNSIGNED    iAttributeId;
        DMI_OFFSET              osAttributeName;
        DMI_UNSIGNED    iAttributeAccess;
        DMI_UNSIGNED    iAttributeType;
        DMI_UNSIGNED    iAttributeMaxSize;
        DMI_UNSIGNED    iEnumListCount;
        DMI_OFFSET              oEnumList;              /* Offset to DMI_EnumData_t EnumList[iEnumListCount] */
} DMI_ListAttributeCnf_t;

typedef struct DmiEnumData {
        DMI_INT         iEnumValue;
        DMI_OFFSET      osEnumName;
} DMI_EnumData_t;


/* 5.9 DmiListDescReq */

typedef struct DmiListDescReq {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iComponentId;
        DMI_UNSIGNED            iGroupId;
        DMI_UNSIGNED            iAttributeId;
        DMI_UNSIGNED            iOffset;
} DMI_ListDescReq_t;


/* 5.10 DmiGetAttributeData, DmiGetAttributeReq, DmiGetAttributeCnf */

typedef struct DmiGetAttributeData {
        DMI_UNSIGNED    iGroupId;
        DMI_UNSIGNED    iGroupKeyCount;
        DMI_OFFSET              oGroupKeyList;  /* Offset to DMI_GroupKeyDat_t GroupKeyList[iGroupKeyCount] */
        DMI_UNSIGNED    iAttributeId;
} DMI_GetAttributeData_t;

typedef struct DmiGetAttributeReq {
        DMI_MgmtCommand_t               DmiMgmtCommand;
        DMI_UNSIGNED                    iComponentId;
        DMI_GetAttributeData_t  DmiGetAttributeList[1];  /* For ANSI C */
} DMI_GetAttributeReq_t;

typedef struct DmiGetAttributeCnf {
        DMI_UNSIGNED    iGroupId;
        DMI_UNSIGNED    iAttributeId;
        DMI_UNSIGNED    iAttributeType;
        DMI_UNSIGNED    oAttributeValue;
} DMI_GetAttributeCnf_t;


/* 5.11 DmiSetAttributeData, DmiSetAttributeReq */

typedef struct DmiSetAttributeData {
        DMI_UNSIGNED    iGroupId;
        DMI_UNSIGNED    iGroupKeyCount;
        DMI_OFFSET              oGroupKeyList;  /* Offset to DMI_GroupKeyDat_t GroupKeyList[iGroupKeyCount] */
        DMI_UNSIGNED    iAttributeId;
        DMI_OFFSET              oAttributeValue;
} DMI_SetAttributeData_t;

typedef struct DmiSetAttributeReq {
        DMI_MgmtCommand_t               DmiMgmtCommand;
        DMI_UNSIGNED                    iComponentId;
        DMI_SetAttributeData_t  DmiSetAttributeList[1];  /* For ANSI C */
} DMI_SetAttributeReq_t;


/* 5.12 DmiGetRowReq, DmiGetRowCnf */

typedef struct DmiGetRowReq {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iComponentId;
        DMI_UNSIGNED            iGroupId;
        DMI_UNSIGNED            iGroupKeyCount;
        DMI_OFFSET                      oGroupKeyList;  /* Offset to DMI_GroupKeyDat_t GroupKeyList[iGroupKeyCount] */
        DMI_UNSIGNED            iAttributeId;
} DMI_GetRowReq_t;

typedef struct DmiGetRowCnf {
        DMI_UNSIGNED                    iGroupId;
        DMI_UNSIGNED                    iGroupKeyCount;
        DMI_OFFSET                              oGroupKeyList;  /* Offset to DMI_GroupKeyDat_t GroupKeyList[iGroupKeyCount */
        DMI_UNSIGNED                    iAttributeCount;
        DMI_GetAttributeCnf_t   DmiGetAttributeList[1];  /* For ANSI C */
} DMI_GetRowCnf_t;


/* 6.1 DmiCiInstallData */

typedef struct DmiCiInstallData {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iComponentId;
        DMI_UNSIGNED            iFileCount;
        DMI_FileData_t          DmiFileList[1];
} DMI_CiInstallData_t;


/* 6.3 DmiCiUninstallData */

typedef struct DmiCiUninstallData {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iComponentId;
} DMI_CiUninstallData_t;


/* 6.5 DmiRegisterCiInd, CiAccessData, DmiRegisterCiCnf, DmiUnregisterCiInd */

typedef struct DmiAccessData {
        DMI_UNSIGNED iGroupId;
        DMI_UNSIGNED iAttributeId;
} DMI_AccessData_t;

typedef struct DmiRegisterCiInd {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iComponentId;
        DMI_FUNC_OUT            pAccessFunc;
        DMI_FUNC0_OUT           pCancelFunc;
        DMI_UNSIGNED            iAccessListCount;
        DMI_AccessData_t        DmiAccessList[1];        /* For ANSI C */
} DMI_RegisterCiInd_t;

typedef struct DmiUnregisterCiInd {
        DMI_MgmtCommand_t       DmiMgmtCommand;
        DMI_UNSIGNED            iCiHandle;
} DMI_UnregisterCiInd_t;


/* 6.6 DmiEventData, DmiClassData; mapped onto the oIndiciationField in DmiIndicate */

typedef struct DmiClassData {
        DMI_UNSIGNED    iComponentId;
        DMI_OFFSET              osClassString;
        DMI_UNSIGNED    iRowCount;
        DMI_OFFSET      oDmiRowList;    /* Offset to DMI_OFFSET[iRowCount] to DMI_GetRowCnf_t */
} DMI_ClassData_t;

typedef struct DmiEventData {
    DMI_UNSIGNED        iClassCount;
    DMI_ClassData_t DmiClassList[1];
} DMI_EventData_t;

#endif
