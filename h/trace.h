/********************** START OF HEADER SPECIFICATION ***********************/
/*                                                                          */
/* FILE NAME: TRACE.H                                                       */
/*                                                                          */
/* COMPONENT NAME: TRACE                                                    */
/*                                                                          */
/* FUNCTIONS:  This header file contains the data structures pertaining     */
/*             to the Trace API's                                           */
/*                                                                          */
/*             Specifically, the API's covered are:                         */
/*                                                                          */
/*             TraceCreateEntry -                                           */
/*                  Create an entry in the WPOS system event buffer         */
/*                                                                          */
/* (C) COPYRIGHT International Business Machines Corp., 1994, 1996          */
/* All Rights Reserved                                                      */
/* Licensed Materials - Property of IBM                                     */
/*                                                                          */
/* US Government Users Restricted Rights - Use, duplication or              */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.        */
/*                                                                          */
/* DESCRIPTION:                                                             */
/*     Trace facility api                                                   */
/*                                                                          */
/********************* END OF HEADER SPECIFICATION **************************/

/*--------------------------------------------------------------------------*/
/* Define Entries                                                           */
/*--------------------------------------------------------------------------*/

#ifndef __TRACEH__
  #define __TRACEH__

#ifndef INCL_TRACE
  #define INCL_TRACE
#endif

/*--------------------------------------------------------------------------*/
/* Definitions:                                                             */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* TRACE ERROR RETURN CODE DEFINES                                          */
/*--------------------------------------------------------------------------*/
#define ERROR_TRACE_COLLECTION_MODE_NOT_WRAP    91
#define ERROR_NOT_A_CFF_INFERRED_FILE           92
#define ERROR_NOT_A_CFF_FILE                    93
#define ERROR_NOT_A_CFF_RAW_FILE                94
#define ERROR_TRC_SYNTAX                      1055

#define INVALID_PACKET_REVISION_NUMBER        1801
#define INVALID_TRACE_MAJOR_CODE              1802
#define INVALID_TRACE_MINOR_CODE              1803
#define INVALID_TRACE_COMMAND_VALUE           1805
#define INVALID_PACKET_SIZE                   1806
#define ERROR_TRACE_SESSION_NOT_RUNNING       1807
#define ERROR_IN_MK_INTERFACE                 3004
#define ERROR_TRACE_COLLECTION_MODE_WRAP      3005
#define INVALID_BUFFER_SIZE                   5000
#define ERROR_TRC_OUT_OF_MEMORY               5001

#define MEMORY_ALLOC_ERROR  ERROR_TRC_OUT_OF_MEMORY

/*--------------------------------------------------------------------------*/
/* TRACE MISC DEFINES                                                       */
/*--------------------------------------------------------------------------*/
#define MIN_MAJOR_EVENT_CODE           1
#define MAX_MAJOR_EVENT_CODE           255
#define MIN_MINOR_EVENT_CODE           1
#define MAX_MINOR_EVENT_CODE           65535

#define MAX_SYSTEM_TRACE_BUFFER_SIZE   63*1024
#define MAX_TRACE_EVENT_DATA_SIZE      2044

#define TRACE_RELEASE   2

/*--------------------------------------------------------------------------*/
/* Structures for TraceCreateEntry API:                                     */
/*--------------------------------------------------------------------------*/
typedef struct _TCEREQUEST
{
  ULONG packet_size;                   /* Size of packet in bytes           */
  ULONG packet_revision_number;        /* Revision level of packet          */
  ULONG major_event_code;              /* Major code event to be logged     */
  ULONG minor_event_code;              /* Minor code event to be logged     */
  ULONG event_data_length;             /* Length of callers event buffer    */
  PVOID event_data;                    /* Pointer to callers buffer         */
} TCEREQUEST, *PTCEREQUEST;

/*--------------------------------------------------------------------------*/
/* Function prototypes                                                      */
/*--------------------------------------------------------------------------*/

USHORT _Far16 _Pascal DosSysTrace(
        USHORT  Major,                 /* major code                 */
        USHORT  Length,                /* length of variable buffer  */
        USHORT  Minor,                 /* minor code                 */
        PBYTE   Buffer);               /* variable length buffer     */

APIRET _System TraceCreateEntry(PTCEREQUEST pTraceCreateEntry);

#endif  /* __TRACEH__ */
