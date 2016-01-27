/*****************************************************************************************************************************************************

    Filename: error.h

    Copyright (c) 1992,1993,1994, Intel Corporation.  All rights reserved.
    Copyright (c) 1993 - 1996, International Business Machines, Corporation. All rights reserved.

    Description: Service Layer error codes


****************************************************************************************************************************************************/

#ifndef ERROR_H_FILE
#define ERROR_H_FILE


/****************************************************************** INCLUDES ***********************************************************************/

/***************************************************************************************************************************************************/


/******************************************************************* DEFINES ***********************************************************************/

#define DMI_GENERAL_ERRORS 0x00000000

/* Non-error condition codes */
#define SLERR_NO_ERROR                                                  DMI_GENERAL_ERRORS + 0x00000000
#define SLERR_NO_ERROR_MORE_DATA                                DMI_GENERAL_ERRORS + 0x00000001


/* Database Errors */
#define DB_ERRORS                                                               DMI_GENERAL_ERRORS + 0x00000100

#define DBERR_ATTRIBUTE_NOT_FOUND                               DB_ERRORS + 0x00000000
#define DBERR_VALUE_EXCEEDS_MAXSIZE                             DB_ERRORS + 0x00000001
#define DBERR_COMPONENT_NOT_FOUND                               DB_ERRORS + 0x00000002
#define DBERR_ENUM_ERROR                                                DB_ERRORS + 0x00000003
#define DBERR_GROUP_NOT_FOUND                                   DB_ERRORS + 0x00000004
#define DBERR_ILLEGAL_KEYS                                              DB_ERRORS + 0x00000005
#define DBERR_ILLEGAL_TO_SET                                    DB_ERRORS + 0x00000006
#define DBERR_OVERLAY_NAME_NOT_FOUND                    DB_ERRORS + 0x00000007
#define DBERR_ILLEGAL_TO_GET                                    DB_ERRORS + 0x00000008
#define DBERR_NO_DESCRIPTION                                    DB_ERRORS + 0x00000009
#define DBERR_ROW_NOT_FOUND                                             DB_ERRORS + 0x0000000a
#define DBERR_DIRECT_INTERFACE_NOT_REGISTERED   DB_ERRORS + 0x0000000b
#define DBERR_DATABASE_CORRUPT                                  DB_ERRORS + 0x0000000c
#define DBERR_ATTRIBUTE_NOT_SUPPORTED                   DB_ERRORS + 0x0000000d
#define DBERR_NO_PRAGMA                         DB_ERRORS + 0x0000000e
#define DBERR_VALUE_UNKNOWN                     DB_ERRORS + 0x0000000f
#define DBERR_LIMITS_EXCEEDED                   DB_ERRORS + 0x00000010


/* Service Layer Errors */
#define SL_ERRORS                                                               DMI_GENERAL_ERRORS + 0x00000200

#define SLERR_BUFFER_FULL                                               SL_ERRORS + 0x00000000
#define SLERR_ILL_FORMED_COMMAND                                SL_ERRORS + 0x00000001
#define SLERR_ILLEGAL_COMMAND                                   SL_ERRORS + 0x00000002
#define SLERR_ILLEGAL_HANDLE                                    SL_ERRORS + 0x00000003
#define SLERR_OUT_OF_MEMORY                                             SL_ERRORS + 0x00000004
#define SLERR_NULL_COMPLETION_FUNCTION                  SL_ERRORS + 0x00000005
#define SLERR_NULL_RESPONSE_BUFFER                              SL_ERRORS + 0x00000006
#define SLERR_CMD_HANDLE_IN_USE                         SL_ERRORS + 0x00000007
#define SLERR_ILLEGAL_DMI_LEVEL                                 SL_ERRORS + 0x00000008
#define SLERR_UNKNOWN_CI_REGISTRY                               SL_ERRORS + 0x00000009
#define SLERR_COMMAND_CANCELED                                  SL_ERRORS + 0x0000000a
#define SLERR_INSUFFICIENT_PRIVILEGES                   SL_ERRORS + 0x0000000b
#define SLERR_NULL_ACCESS_FUNCTION                              SL_ERRORS + 0x0000000c
#define SLERR_FILE_ERROR                                                SL_ERRORS + 0x0000000d
#define SLERR_EXEC_FAILURE                                              SL_ERRORS + 0x0000000e
#define SLERR_BAD_MIF_FILE                                              SL_ERRORS + 0x0000000f
#define SLERR_INVALID_FILE_TYPE                                 SL_ERRORS + 0x00000010
#define SLERR_SL_INACTIVE                                               SL_ERRORS + 0x00000011
#define SLERR_UNICODE_NOT_SUPPORTED                             SL_ERRORS + 0x00000012
#define SLERR_CANT_UNINSTALL_SL_COMPONENT       SL_ERRORS + 0x00000013
#define SLERR_NULL_CANCEL_FUNCTION                              SL_ERRORS + 0x00000014

/* DMI DOS ERRORS                                                            */
/* --------------                                                            */
#define DMI_DOS_ERRORS 0x00001000

/* Overlay Manager Errors */
#define DOS_OM_ERRORS                                                   DMI_DOS_ERRORS + 0x00000000

#define OMERR_OUT_OF_PARTITION_MEMORY                   DOS_OM_ERRORS + 0x00000000
#define OMERR_OVERLAY_NOT_FOUND                                 DOS_OM_ERRORS + 0x00000001
#define OMERR_READING_FILE                                              DOS_OM_ERRORS + 0x00000002

#define DMI_DOS_SYSTEM_ERRORS                                   DMI_DOS_ERRORS + 0x00000100

#define DOSERR_SL_BUSY                                                  DMI_DOS_SYSTEM_ERRORS + 0x00000001

/* DMI WINDOWS ERRORS                                                        */
/* ------------------                                                        */
#define DMI_WINDOWS_ERRORS 0x00002000

#define WINERR_OUT_OF_DPMI_MEMORY                               DMI_WINDOWS_ERRORS + 0x00000000
#define WINERR_OUT_OF_DPMI_CALLBACKS                    DMI_WINDOWS_ERRORS + 0x00000001
#define WINERR_UNABLE_TO_FIND_TASK_HANDLE               DMI_WINDOWS_ERRORS + 0x00000002

/* DMI OS/2 ERRORS OR NT SPECIFIC ERRORS                                     */
/* ----------------                                                          */
#if defined(_WIN32)
#define DMI_OS2_ERRORS 0x00006000
#else
#define DMI_OS2_ERRORS 0x00003000
#endif

#define SLERR_NOT_INITIALIZED                   DMI_OS2_ERRORS + 0x00000000
#define SLERR_IPC_CREATE_ERROR                  DMI_OS2_ERRORS + 0x00000001
#define SLERR_THREAD_CREATE_ERROR               DMI_OS2_ERRORS + 0x00000002
#define SLERR_QUEUE_CREATE_ERROR                DMI_OS2_ERRORS + 0x00000003
#define SLERR_SL_TERMINATED                     DMI_OS2_ERRORS + 0x00000004
#define SLERR_CMD_EXCEPTION                     DMI_OS2_ERRORS + 0x00000005
#define SLERR_SYNC_SETUP_ERROR                  DMI_OS2_ERRORS + 0x00000006
#define SLERR_SL_DLL_MISMATCH                   DMI_OS2_ERRORS + 0x00000007
#define SLERR_IPC_ERROR                         DMI_OS2_ERRORS + 0x00000008


/* DMI UNIX ERRORS                                                           */
/* ----------------                                                          */
#define DMI_UNIX_ERRORS 0x00004000


/* DMI COMPONENT ERRORS                                                      */
/* --------------------                                                      */
#define DMI_COMPONENT_ERRORS 0x00010000

#define CPERR_MODULE_REMOVED                    DMI_COMPONENT_ERRORS + 0x00000000
#define CPERR_CANCEL_ERROR                              DMI_COMPONENT_ERRORS + 0x00000001               /* Who knows what this means; it's in the spec. */
#define CPERR_INVALID_OVERLAY                   DMI_COMPONENT_ERRORS + 0x00000002
#define CPERR_CI_TERMINATED             DMI_COMPONENT_ERRORS + 0x00000003
#define CPERR_GET_ERROR                 DMI_COMPONENT_ERRORS + 0x00000004
#define CPERR_KEY_ERROR                 DMI_COMPONENT_ERRORS + 0x00000005
#define CPERR_RELEASE_ERROR             DMI_COMPONENT_ERRORS + 0x00000006
#define CPERR_RESERVE_ERROR             DMI_COMPONENT_ERRORS + 0x00000007
#define CPERR_ROW_ERROR                 DMI_COMPONENT_ERRORS + 0x00000008
#define CPERR_SET_ERROR                 DMI_COMPONENT_ERRORS + 0x00000009

/***************************************************************************************************************************************************/


/******************************************************************** TYPEDEFS *********************************************************************/

typedef unsigned long SL_ErrorCode_t;

/***************************************************************************************************************************************************/


/********************************************************************** DATA ***********************************************************************/

/***************************************************************************************************************************************************/

/**************************************************************** FUNCTION PROTOTYPES **************************************************************/

/***************************************************************************************************************************************************/


#endif
