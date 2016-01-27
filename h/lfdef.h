/********************** START OF HEADER SPECIFICATION ***********************/

/*                                                                          */
/* FILE NAME:  LFDEF.H                                                      */
/*                                                                          */
/* FUNCTIONS:  This header file contains the data structures pertaining     */
/*             to the Public Logging Framework APIs.                        */
/*                                                                          */
/* (C) COPYRIGHT International Business Machines Corp., 1994.               */
/* All Rights Reserved                                                      */
/* Licensed Materials - Property of IBM                                     */
/*                                                                          */
/* US Government Users Restricted Rights - Use, duplication or              */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.        */
/*                                                                          */
/********************* END OF HEADER SPECIFICATION **************************/
#ifndef _LFDEFH_
#define _LFDEFH_

#include "Unidef.h"

#ifndef INCL_LOGGING
  #define INCL_LOGGING
#endif

#define ERROR_LOGGING_SERVICE                           1
#define CURRENT_LOG_FILE_ID                             1

/*--------------------------------------------------------------------------*/
/* Packet Revision numbers                                                  */
/*--------------------------------------------------------------------------*/
#define LF_UNI_API                                      2
#define LF_ASCII_API                                    3
#define WPOS_RELEASE_1                                  LF_UNI_API
#define FROM_LOGDAEM_EXE                                4
#define DET1_PACKET                                     2
#define DET4_PACKET                                     4

/*--------------------------------------------------------------------------*/
/* Error Log Record Types                                                   */
/*--------------------------------------------------------------------------*/
#define LF_DET1                                         0xD1D1D1D1
#define LF_DET2                                         0xD2D2D2D2
#define LF_DET4                                         0xD4D4D4D4

/*--------------------------------------------------------------------------*/
/* Return Codes  :                                                          */
/*--------------------------------------------------------------------------*/
#ifndef NO_ERROR
  #define NO_ERROR                                      0
#endif
#ifndef ERROR_FILE_NOT_FOUND
  #define ERROR_FILE_NOT_FOUND                          2
#endif
#ifndef ERROR_PATH_NOT_FOUND
  #define ERROR_PATH_NOT_FOUND                          3
#endif
#ifndef ERROR_ACCESS_DENIED
  #define ERROR_ACCESS_DENIED                           5
#endif
#ifndef ERROR_INVALID_ACCESS
  #define ERROR_INVALID_ACCESS                          12
#endif
#ifndef ERROR_SHARING_VIOLATION
  #define ERROR_SHARING_VIOLATION                       32
#endif
#ifndef ERROR_DEVICE_IN_USE
  #define ERROR_DEVICE_IN_USE                           99
#endif
#ifndef ERROR_DRIVE_LOCKED
  #define ERROR_DRIVE_LOCKED                            108
#endif
#ifndef ERROR_OPEN_FAILED
  #define ERROR_OPEN_FAILED                             110
#endif
#ifndef ERROR_LF_INVALID_REQUEST
  #define ERROR_LF_INVALID_REQUEST                      517
#endif
#ifndef ERROR_LF_NOT_AVAIL
  #define ERROR_LF_NOT_AVAIL                            518
#endif
#ifndef ERROR_LF_SUSPENDED
  #define ERROR_LF_SUSPENDED                            519
#endif
#ifndef ERROR_LF_BUF_TOO_SMALL
  #define ERROR_LF_BUF_TOO_SMALL                        520
#endif
#ifndef ERROR_LF_BUFFER_FULL
  #define ERROR_LF_BUFFER_FULL                          521
#endif
#ifndef ERROR_LF_INVALID_RECORD
  #define ERROR_LF_INVALID_RECORD                       522
#endif
#ifndef ERROR_LF_INVALID_SERVICE
  #define ERROR_LF_INVALID_SERVICE                      523
#endif
#ifndef ERROR_LF_GENERAL_FAILURE
  #define ERROR_LF_GENERAL_FAILURE                      524
#endif
#ifndef ERROR_LF_OUT_OF_STORAGE
  #define ERROR_LF_OUT_OF_STORAGE                       528
#endif

#define INVALID_LF_LOG_FILE_ID                          1701
#define RAS_INVALID_LOG_FILE_ID                         1701
#define INVALID_LF_PACKET_REVISION_NUMBER               1702
#define INVALID_DATA_POINTER                            1703
#define INVALID_LF_FILENAME_LENGTH                      1704
#define RAS_INCORRECT_FILENAME_LENGTH                   1704
#define INVALID_LF_FILENAME_PTR                         1705
#define RAS_INVALID_FILENAME_PTR                        1705
#define INVALID_LF_PARM_PACKET_PTR                      1706
#define RAS_INVALID_PARM_PACKET_PTR                     1706
#define INVALID_LF_ENTRY_ID                             1707
#define INVALID_LF_LOG_RECORD_BUFFER_PTR                1709
#define RAS_INVALID_LOGRECORD_BUFFER_PTR                1709
#define INVALID_LF_LOG_BUFFER_TOO_SMALL                 1710
#define INVALID_LF_STATIC_PACKET_PTR                    1711
#define INVALID_LF_DYNAMIC_PACKET_PTR                   1712
#define INVALID_LF_STATIC_REVISION_NUMBER               1713
#define INVALID_LF_STATIC_PRODUCT_MFG_NAME              1714
#define INVALID_LF_STATIC_PRODUCT_NAME                  1715
#define INVALID_LF_STATIC_SUBPRODUCT_NAME               1716
#define INVALID_LF_STATIC_MODULE_NAME                   1717
#define INVALID_LF_STATIC_PROCESS_PATHNAME              1718
#define INVALID_LF_DYNAMIC_REVISION_NUMBER              1719
#define INVALID_LF_DYNAMIC_SEVERITY                     1720
#define INVALID_LF_MSGINSDATA_NUM_INSERTS               1721
#define INVALID_LF_MSGINSDATA_MSGNUM_MISMATCH           1722
#define INVALID_LF_MSGINSDATA_MSG_TEXT_PTR              1723
#define INVALID_LF_DYNAMIC_DUMP_FILENAME_PTR            1724
#define INVALID_LF_DYNAMIC_DUMP_FORMATTER_PARMS_PTR     1725
#define INVALID_LF_DYNAMIC_TRACE_FILENAME_PTR           1726
#define INVALID_LF_DYNAMIC_TRACE_FORMATTER_PARMS_PTR    1727
#define INVALID_LF_DYNAMIC_PROCESS_FILENAME_PTR         1728
#define INVALID_LF_DYNAMIC_PROCESS_FORMATTER_PARMS_PTR  1729
#define INVALID_LF_DYNAMIC_PCT_PARMS_PTR                1730
#define INVALID_LF_DYNAMIC_USER_ENTRY_DATA_PTR          1731
#define INVALID_LF_LOG_FILE                             1732
#define RAS_INVALID_LOG_FILE                            1732
#define FATAL_LF_ALL_LOG_FILES_INVALID                  1733
#define INVALID_LF_LOG_RECORD_TOO_BIG                   1734
#define RAS_QUERY_PIPEBUFFER_TOO_SMALL                  1735
#define INVALID_LF_RECORD_TYPE                          1736

#define INVALID_LF_ATTRIBUTE_ID                         1740
#define INVALID_LF_ATTR_BUFFER_TOO_SMALL                1741
#define INVALID_LF_ATTR_BUFFER_PTR                      1742

#define INVALID_LF_AT_END_OF_LOG                        1750
#define INVALID_LF_FLAG                                 1751
#define RAS_INVALID_FLAG                                1751
#define INVALID_LF_OPERATOR_ID                          1752
#define ERROR_LF_ENTRY_NOT_FOUND                        1753
#define INVALID_LF_UPDATE_COUNT                         1754
#define INVALID_LF_FIELD_HAS_DATA                       1755
#define INVALID_LF_SUBBLOCK_DATA_LENGTH                 1756
#define INVALID_LF_LOG_NOTIFY_PTR                       1757
#define RAS_INVALID_LOG_NOTIFY_PTR                      1757
#define INVALID_LF_LOG_NOTIFY_ID                        1758
#define RAS_INVALID_LOG_NOTIFY_ID                       1758
#define INVALID_LF_ENTRY_KEY                            1759

#define STATE_SUM_BUFFER_2_SMALL                        1760
#define ERROR_LF_INVALID_PACKET_SIZE                    1761
#define RAS_INVALID_PACKET_SIZE                         1761
#define ERROR_LF_NO_NOTIFICATION_ENTRIES                1762

#define ERROR_LF_DECREASE_TOO_MUCH                      1765
#define ERROR_LF_FILTER_LOOP                            1766
#define INVALID_LOG_ENTRY_RECORD                        1770
#define NO_LOG_ENTRY_FORMAT_TEMPLATE_AVAILABLE          1771
#define ERROR_LF_TIMEOUT                                2055
#define ERROR_LF_REDIRECT_FAILURE                       2060

#define ERROR_LF_DOSLOADMODULE_ERROR                    1798
#define ERROR_LF_DOSQUERYPROCADDR_ERROR                 1799

#define LF_INVALID_FILE_HANDLE                          2002
#define RAS_INVALID_FILE_HANDLE                         2002

#define RAS_EVENT_SEM_FAILURE                           2500
#define RAS_QUEUE_FAILURE                               2501
#define RAS_INTERNAL_MEMORY_FAILURE                     2502
#define RAS_NOTIF_ENTRY_NOT_FOUND                       2503
#define RAS_NOTIF_ENTRY_DELETED                         2504
#define RAS_ENTRY_FILTER_UNCHANGED                      2505
#define RAS_NO_MORE_QUEUE_MEMORY                        2506
#define RAS_UNICODE_CONVERSION_ERROR                    2507
#define RAS_INVALID_EVENTKEY_PTR                        2508
#define RAS_INVALID_PATHLEN_PTR                         2509
#define RAS_INVALID_BUFLEN_PTR                          2510

/**********************************************************************/
/* Possible returns from FormatDLL routine (in addition to NO_ERROR)  */
/**********************************************************************/

#define RAS_FMTDLL_EXCEPTION                            2511
#define RAS_FMTDLL_VAR_NOT_FOUND                        2512
#define RAS_FMTDLL_BAD_VARIABLE                         2513
#define RAS_FMTDLL_PROC_NOT_FOUND                       2514
#define RAS_FMTDLL_BAD_SIZE_RETURN                      2515
#define RAS_FMTDLL_NOT_FOUND                            2516

#define RAS_INVALID_LOCALE_OBJECT                       2600

#define MALLOC_ERROR                                    3001

/*--------------------------------------------------------------------*/
/* Informational messages.                                            */
/*--------------------------------------------------------------------*/
#define LF_FILE_CREATED                                 4001
#define RAS_FILE_CREATED                                4001
#define SWITCHED_TO_DEFAULT_LOG                         4002
#define LOG_ALREADY_SUSPENDED                           4003
#define LOG_ALREADY_ACTIVE                              4004
#define LF_REDIRECT_FAILED                              4005
#define LF_REDIRECTED_TO_CURRENT                        4006
#define LF_SIZE_IS_THE_SAME                             4007
#define LF_FILE_CREATED_MINSIZE_USED                    4008

#define DEFAULT_LOG_FILE_ID   1
#define RECSIZE 4096

/*--------------------------------------------------------------------------*/
/* Event Notification Flags                                                 */
/*--------------------------------------------------------------------------*/
#define KEEP_EVENT_NOTIFICATION  0
#define PURGE_EVENT_NOTIFICATION 1     /* LogChangeEventFilter flag         */
#define RETURN_NO_DATA           1     /* LogOpenEvent read_flags           */
#define DEQ_ON_ERROR             1     /* LogWaitEvent queue_flags          */

/*--------------------------------------------------------------------------*/
/* LogReadEntry flags :                                                     */
/*--------------------------------------------------------------------------*/
/* START_AFTER_KEY is optional (default if no other start position specified*/
/* START_AFTER_KEY, START_AT_FIRST_ENTRY and READ_LAST_RECORD are mutially  */
/* exclusive and should not be specified together.                          */
/* READ_MULTIPLE_RECORDS is ignored if specified with READ_LAST_RECORD      */
#define START_AFTER_KEY       0x00000001L
#define START_AT_FIRST_ENTRY  0x00000004L
#define START_AT_THIS_ENTRY   0x00000008L
#define READ_LAST_RECORD      0x00000010L
#define READ_MULTIPLE_RECORDS 0x00010000L

/*--------------------------------------------------------------------------*/
/* LogFormatEntry Flags:                                                    */
/*--------------------------------------------------------------------------*/
#define ERR_FORMAT_DETAIL_DATA                0x00000001
#define FORMAT_ONLY_ERROR_DESCRIPTION         0X00000002
#define TEMPLATE_NOT_FOUND                    0x00000004
#define ERROR_DESCRIPTION_FILE_NOT_FOUND      0x00000008
#define CAUSE_MSG_FILE_NOT_FOUND              0x00000010
#define ACTION_MSG_FILE_NOT_FOUND             0x00000020
#define DETAIL_DATA_FILE_NOT_FOUND            0x00000040

/*--------------------------------------------------------------------------*/
/* defines for data description types                                       */
/*--------------------------------------------------------------------------*/
#define SUMMARY_HEADING                         1
#define STANDARD_DETAIL1_HEADING                1
#define STANDARD_DETAIL1_TEXT                   2
#define STANDARD_DETAIL2_HEADING                3
#define STANDARD_DETAIL2_TEXT                   4
#define DATE_HEADING                            5
#define DATE                                    6
#define TIME_HEADING                            7
#define TIME                                    8
#define ENTRY_ID_HEADING                        9
#define ENTRY_ID                               10
#define SEVERITY_HEADING                       11
#define SEVERITY                               12
#define MODULE_NAME_HEADING                    13
#define MODULE_NAME                            14
#define DIRECTORY_NAME_HEADING                 15
#define DIRECTORY_NAME                         16
#define ERROR_MESSAGE_HEADING                  17
#define ERROR_MESSAGE_TEXT                     18
#define PROBE_ID_HEADING                       19
#define PROBE_ID_TEXT                          20
#define PROBE_FLAGS_HEADING                    21
#define PROBE_FLAGS_TEXT                       22
#define TEMPLATE_REPOSITORY_PATHNAME_HEADING   23
#define TEMPLATE_REPOSITORY_PATHNAME_TEXT      24
#define TEMPLATE_ID_HEADING                    25
#define TEMPLATE_ID_TEXT                       26
#define DUMP_GENERATED_HEADING                 27
#define DUMP_GENERATED_TEXT                    28
#define TRACE_FILE_GENERATED_HEADING           29
#define TRACE_FILE_GENERATED_TEXT              30
#define PROCESS_DUMP_GENERATED_HEADING         31
#define PROCESS_DUMP_GENERATED_TEXT            32
#define FAILURE_CAUSES_HEADING                 40
#define FAILURE_CAUSES                         41
#define FAILURE_ACTIONS_HEADING                50
#define FAILURE_ACTIONS                        51
#define INSTALL_CAUSES_HEADING                 60
#define INSTALL_CAUSES                         61
#define INSTALL_ACTIONS_HEADING                70
#define INSTALL_ACTIONS                        71
#define USER_CAUSES_HEADING                    80
#define USER_CAUSES                            81
#define USER_ACTIONS_HEADING                   90
#define USER_ACTIONS                           91
#define RETURN_CODE_HEADING                   100
#define RETURN_CODE_TEXT                      101
#define DUMP_FILENAME_HEADING                 110
#define DUMP_FILENAME_TEXT                    111
#define DUMP_FORMATTER_HEADING                112
#define DUMP_FORMATTER_TEXT                   113
#define DUMP_FILE_DELETION_DATE_HEADING       114
#define DUMP_FILE_DELETION_DATE               115
#define DUMP_FILE_DELETION_TIME_HEADING       116
#define DUMP_FILE_DELETION_TIME               117
#define TRACE_FILE_NAME_HEADING               120
#define TRACE_FILE_NAME_TEXT                  121
#define TRACE_FORMATTER_HEADING               122
#define TRACE_FORMATTER_TEXT                  123
#define TRACE_FILE_DELETION_DATE_HEADING      124
#define TRACE_FILE_DELETION_DATE              125
#define TRACE_FILE_DELETION_TIME_HEADING      126
#define TRACE_FILE_DELETION_TIME              127
#define PROCESS_DUMP_FILE_NAME_HEADING        130
#define PROCESS_DUMP_FILE_NAME_TEXT           131
#define PROCESS_DUMP_FORMATTER_HEADING        132
#define PROCESS_DUMP_FORMATTER_TEXT           133
#define PROCESS_DUMP_DELETION_DATE_HEADING    134
#define PROCESS_DUMP_FILE_DELETION_DATE       135
#define PROCESS_DUMP_DELETION_TIME_HEADING    136
#define PROCESS_DUMP_FILE_DELETION_TIME       137
#define PCT_HEADING                           140
#define PCT_EXECUTION_PARAMETERS              141
#define DMI_VENDOR_TAG_HEADING                150
#define DMI_VENDOR_TAG_TEXT                   151
#define DMI_TAG_HEADING                       155
#define DMI_TAG_TEXT                          156
#define PRODUCT_ID_HEADING                    165
#define DMI_PRODUCT_ID_HEADING                165
#define PRODUCT_ID_TEXT                       166
#define DMI_PRODUCT_ID_TEXT                   166
#define DMI_REVISION_HEADING                  170
#define DMI_REVISION_TEXT                     171
#define DMI_MODIFICATION_LEVEL_HEADING        172
#define DMI_MODIFICATION_LEVEL_TEXT           173
#define DMI_FIX_LEVEL_HEADING                 174
#define DMI_FIX_LEVEL_TEXT                    175
#define MODULE_NAME_TEXT                      181
#define PROCESS_ID_HEADING                    185
#define PROCESS_ID_TEXT                       186
#define TASK_ID_HEADING                       190
#define TASK_ID_TEXT                          191
#define MACHINE_TYPE_HEADING                  195
#define MACHINE_TYPE_TEXT                     196
#define MACHINE_SERIAL_NUMBER_HEADING         200
#define MACHINE_SERIAL_NUMBER_TEXT            201
#define HOSTNAME_HEADING                      205
#define HOSTNAME_TEXT                         206
#define USER_DATA_HEADING                     210
#define USER_DATA_TEXT                        211
#define ACTION_HEADING                        213
#define ACTION_TEXT                           214
#define OLD_VALUE_DET2_HEADING                215
#define OLD_VALUE_DET2_TEXT                   216
#define NEW_VALUE_DET2_HEADING                220
#define NEW_VALUE_DET2_TEXT                   221

#define SUMMARY_HEADING_DET2                    7
#define YES_STR                                 2
#define YES_DELETED_STR                         3
#define NO_STR                                  4

/*--------------------------------------------------------------------------*/
/* defines for DET4 data description types                                  */
/*--------------------------------------------------------------------------*/
#define CREATED_BY_BACKLEVEL_TEXT             222
#define RECORD_ID_HEADING                     223
#define RECORD_ID                             224
#define QUALIFIER_HEADING                     225
#define QUALIFIER                             226
#define ORIGINATOR_HEADING                    227
#define ORIGINATOR                            228
#define DET4_USER_DATA                        229
#define PROCESS_NAME_HEADING                  230
#define PROCESS_NAME                          231
#define FMTDLL_HEADING                        232
#define FMTDLL_NAME                           233
#define FMTDLL_TEXT                           234
#define GA_COMPONENT_ID_HEADING               235
#define GA_COMPONENT_ID                       236
#define GA_RELEASE_LEVEL_HEADING              237
#define GA_RELEASE_LEVEL                      238
#define GA_SOFTWARE_NAME_HEADING              239
#define GA_SOFTWARE_NAME                      240
#define GENERIC_ALERT_SUBVECTOR_HEADING       241
#define GENERIC_ALERT_SUBVECTOR_TEXT          242
#define PROBABLE_CAUSES_SUBVECTOR_HEADING     243
#define PROBABLE_CAUSES_SUBVECTOR_TEXT        244
#define USER_CAUSES_SUBVECTOR_HEADING         245
#define INSTALL_CAUSES_SUBVECTOR_HEADING      246
#define FAILURE_CAUSES_SUBVECTOR_HEADING      247
#define SUBVECTOR_KEY_HEADING                 248
#define SUBVECTOR_KEY_TYPE                    249
#define SUBVECTOR_KEY_TEXT                    250
#define HEX_DUMP_TEXT                         251

/*--------------------------------------------------------------------------*/
/* Log Format flag constants                                                */
/*--------------------------------------------------------------------------*/
#define RESERVED_FLAGS                  0x00000003
#define CK_BIT1                         0x00000002
#define FULL_FORMAT                     0x00000000

/*--------------------------------------------------------------------------*/
/* Structure for packets returned by logformat                              */
/*--------------------------------------------------------------------------*/
typedef struct _LFEDETAIL2                       /*                         */
       {
       ULONG    length;                          /*                         */
       ULONG    type;                            /*                         */
/*     PVOID    data;                                                       */
      }LFEDETAIL2   , *PLFEDETAIL2;

/*--------------------------------------------------------------------------*/
/* Structure for buffer returned by logformat                               */
/*--------------------------------------------------------------------------*/
typedef struct _LFEDETAIL                        /*                         */
       {
       ULONG    packet_size;                     /*                         */
       ULONG    packet_revision_number;          /*                         */
       ULONG    number_of_detail_records;        /*                         */
       LFEDETAIL2 detail_records;                /*                         */
      }LFEDETAIL   , *PLFEDETAIL;

/*--------------------------------------------------------------------------*/
/* Standard Date & Time structures used by Logging Framework                */
/*--------------------------------------------------------------------------*/
typedef struct _LOGDATE
       {
       BYTE   day;
       BYTE   month;
       USHORT year;
       }LOGDATE, *PLOGDATE;

typedef struct _LOGTIME
       {
       BYTE   hours;
       BYTE   minutes;
       BYTE   seconds;
       BYTE   hundredths;
       }LOGTIME, *PLOGTIME;

/*--------------------------------------------------------------------------*/
/* Structure for LogOpenFile API:                                           */
/*--------------------------------------------------------------------------*/
typedef struct _LOFREQUEST                       /* Log Open File structure */
       {
       ULONG    packet_size;                     /* size in bytes of packet */
       ULONG    packet_revision_number;          /* revision level of pkt.  */
       PULONG   log_file_ID;                     /* Id of file to be opened */
       PULONG    filename_length;                /* length of filename      */
       PVOID     filename;                       /* pointer to the filename */
      }LOFREQUEST, *PLOFREQUEST;

/*--------------------------------------------------------------------------*/
/* Structure for LogCloseFile API:                                          */
/*--------------------------------------------------------------------------*/
typedef struct _LCFREQUEST                    /* Log Close File structure   */
       {
       ULONG    packet_size;                  /* size in bytes of packet    */
       ULONG    packet_revision_number;       /* revision level of parm pkt.*/
       ULONG    log_file_ID;                  /* Id of the file to be closed*/
      }LCFREQUEST, *PLCFREQUEST;

/*--------------------------------------------------------------------------*/
/* Structure for the header information for DET1, DET2, and DET4 records    */
/*--------------------------------------------------------------------------*/
typedef struct _DETAILHDR                        /* HDR format structure    */
       {
       ULONG    packet_size;                     /* size in bytes of packet */
       ULONG    packet_revision_number;          /* revision level of pkt.  */
       ULONG    record_type;                     /* 1=DET1, 2=DET2, 4=DET4  */
       LOGDATE  date_event_occurred;             /* date of failure         */
       LOGTIME  time_event_occurred;             /* time of failure         */
       ULONG    entry_ID;                        /* # assigned to the record*/
       ULONG    next_record_location;          /* loc of next record in file*/
       ULONG    previous_record_location;      /* loc of prev record in file*/
       }DETAILHDR, * PDETAILHDR;

/*--------------------------------------------------------------------------*/
/* Structure for Detail 1 error log record                                  */
/*--------------------------------------------------------------------------*/
typedef struct _ERRORDET1                        /* DET1 format structure   */
       {
       DETAILHDR hdr;                            /* DETx common section     */
       ULONG    system_machine_type_offset;      /* offset to machine type  */
       ULONG    system_serial_number_offset;     /* offset to serial num    */
       ULONG    system_hostname_offset;          /* offset to host name     */
       ULONG    template_filename_length;
       ULONG    template_filename_offset;        /* offset to templ filename*/
       ULONG    template_record_ID;              /* id of entry in T.R. file*/
       ULONG    DMI_vendor_tag_offset;
       ULONG    DMI_tag_offset;                  /* offset to a product name*/
       ULONG    DMI_product_ID_offset;           /* offset to a product #   */
       ULONG    DMI_revision_number_offset;      /* vers. number offset     */
       ULONG    DMI_modification_level_offset;   /* modification lvel offset*/
       ULONG    DMI_fix_level_offset;            /* fix level offset        */
       ULONG    module_offset;                /*detecting module name offset*/
       ULONG    process_pathname_length;         /* len of process pathname */
       ULONG    process_pathname_offset;         /* offset to process path  */
       ULONG    severity;                        /* severity code 1-6       */
       ULONG    probe_ID;                        /* Id of firing probe      */
       ULONG    description_inserts_offset;      /* offset to msg inserts   */
       ULONG    dump_filename_length;            /* length of dump filename */
       ULONG    dump_filename_offset;            /* offset to dump file name*/
       LOGDATE  dump_deletion_date;              /*date file deleted by FFST*/
       LOGTIME  dump_deletion_time;
       ULONG    dump_formatter_parms_offset;     /* offset to name & parms  */
                                                 /* to start Dump formatter */
       ULONG    trace_filename_length;           /* length of trace filename*/
       ULONG    trace_filename_offset;           /* offset to trace filename*/
       LOGDATE  trace_deletion_date;             /*date file deleted by FFST*/
       LOGTIME  trace_deletion_time;
       ULONG    trace_formatter_parms_offset;    /* offset to name & parms  */
                                                 /* to start Trace formatter*/
       ULONG    process_filename_length;         /* Len of process dump name*/
       ULONG    process_filename_offset;         /* offset to dump file name*/
       LOGDATE  process_deletion_date;           /* Date deleted by FFST... */
       LOGTIME  process_deletion_time;
       ULONG    process_formatter_parms_offset;  /* offset to name & parms  */
                                     /* to start the Process Dump formatter */
       ULONG    PCT_formatter_parms_offset;      /* offset to name & parms  */
                                                 /* to start PCT utility    */
       ULONG    probe_flags;                   /*type of info placed in dump*/
       ULONG    reserved_1_ULONG;                /* RESERVED                */
       ULONG    reserved_2_ULONG;
       ULONG    reserved_3_ULONG;
       ULONG    return_code;                     /* return code from ffst   */
       ULONG    user_entry_data_length;          /*length of user entry data*/
       ULONG    user_entry_data_offset;          /* offset to user data     */
      }ERRORDET1, *PERRORDET1;


/*--------------------------------------------------------------------------*/
/* Structure for Detail 2 error log record                                  */
/*--------------------------------------------------------------------------*/
typedef struct _ERRORDET2      /* Control log record (DET2) format structure*/
       {
       DETAILHDR hdr;                        /* DETx common section         */
       ULONG    action;                      /* action that created this rec*/
      }ERRORDET2, *PERRORDET2;

typedef struct _DET2_FILESIZE                    /* ERRORDET2 for action max*/
        {                                        /* size change and redirect*/
          ULONG old_max_size;                    /* old max file size       */
          ULONG new_max_size;                    /* new max file size       */
          ULONG old_pathname_length;             /* old file path len       */
          ULONG old_pathname_offset;             /* offset to old path      */
          ULONG new_pathname_length;             /* new file path len       */
          ULONG new_pathname_offset;             /* offset to new path      */
        }DET2_FILESIZE, *PDET2_FILESIZE;

/* ERRORDET2 for action file name change - file names follow ERRORDET2 record*/
typedef UniChar *POLD_PATHNAME;                  /* previous file name      */
typedef UniChar *PNEW_PATHNAME;                  /* new file name           */

/*--------------------------------------------------------------------------*/
/* Structure for Detail 4 error log record                                  */
/*--------------------------------------------------------------------------*/
typedef struct _ERRORDET4                        /* DET4 format structure   */
       {
       DETAILHDR hdr;                            /* DETx common section     */
       USHORT    rec_ID;                         /* record ID               */
       ULONG     status;                         /* status bytes            */
       union {
         UniChar   uniQual[4];                   /* Unicode qualifier name  */
                                                 /*       - or -            */
         UCHAR     ascQual[4];                   /* ASCII qualifier name    */
                } qualifier;
       ULONG     error_log_entry_data_length;  /* length of user_entry_data */
       ULONG     error_log_entry_data_offset; /* offset to user_entry_data  */
      }ERRORDET4, *PERRORDET4;

/*--------------------------------------------------------------------------*/
/* Filter data structures                                                   */
/*--------------------------------------------------------------------------*/

typedef struct _EVENTKEY                         /* Event Key data structure*/
       {
       ULONG    location;                        /* location within Log File*/
       ULONG    entry_ID;                        /* entry ID of the record  */
      }EVENTKEY, *PEVENTKEY;

typedef struct _SUBBLOCK               /* Format selection criteria subblock*/
       {
       ULONG    entry_attribute_ID;            /* attribute id              */
       ULONG    comparison_operator_ID;        /* comparison operator       */
       ULONG    comparison_data_length;        /* len for non implicit attr */
       PVOID    comparison_data_ptr;           /* ptr to item being compared*/
       struct _SUBBLOCK * next_subblock;       /* next subblock in chain    */
      }SUBBLOCK, *PSUBBLOCK;


typedef struct _HEADERBLOCK                /* Header block format structure */
       {
       PSUBBLOCK pSubblock;                /* size of this packet           */
       struct _HEADERBLOCK * pNextBlock;   /* ptr to next block in chain    */
      }HEADERBLOCK, *PHEADERBLOCK;


typedef struct _FILTERBLOCK                     /* Filter block structure   */
       {
       ULONG    packet_size;                    /* size of this packet      */
       ULONG    packet_revision_number;         /* revision level           */
       PHEADERBLOCK header_block;               /* ptr to first header block*/
      }FILTERBLOCK, *PFILTERBLOCK;

/*--------------------------------------------------------------------------*/
/* attribute id's for error log entries - used in filter sub blocks         */
/* NOTE: update LOG_ERROR_FILTER_MAX when adding filters                    */
/*--------------------------------------------------------------------------*/
#define LOG_ERROR_DATE                    1      /* date type               */
#define LOG_ERROR_TIME                    2      /* time type               */
#define LOG_ERROR_ENTRY_ID                3      /* unsigned long integer   */
#define LOG_ERROR_RECORD_TYPE             4      /* unsigned long integer   */
#define LOG_ERROR_MACHINE_TYPE            5      /* string type             */
#define LOG_ERROR_SERIAL_NUMBER           6      /* string type             */
#define LOG_ERROR_SEVERITY                7      /* unsigned long integer   */
#define LOG_ERROR_DMI_VENDOR_TAG          8      /* string type             */
#define LOG_ERROR_DMI_TAG                 9      /* string type             */
#define LOG_ERROR_DMI_REVISION            10     /* string type             */
#define LOG_ERROR_SOURCE_MODULE_NAME      11     /* string type             */
#define LOG_ERROR_PROCESS_PATHNAME        12     /* string type             */
#define LOG_ERROR_PROBE_ID                13     /* unsigned long integer   */
#define LOG_ERROR_USER_DATA               14     /* string type             */

#define LOG_ERROR_FILTER_MAX              14     /* number of filter attrib */
/*--------------------------------------------------------------------------*/
/* comparison operator IDs - used in filter sub blocks */
/*--------------------------------------------------------------------------*/
#define LOG_ERROR_EQUAL                   1      /* all types               */
#define LOG_ERROR_NOT_EQUAL               2      /* all types               */
#define LOG_ERROR_GREATER_THAN            3      /* date, time unsigned long*/
#define LOG_ERROR_GREATER_THAN_OR_EQUAL   4      /* date, time unsigned long*/
#define LOG_ERROR_LESS_THAN               5      /* date, time unsigned long*/
#define LOG_ERROR_LESS_THAN_OR_EQUAL      6      /* date, time unsigned long*/
#define LOG_ERROR_SUBSTRING_MATCH         7      /* string type only        */

/*--------------------------------------------------------------------------*/
/* Structure for LogReadFile API:                                           */
/*--------------------------------------------------------------------------*/
typedef struct _LREREQUEST                   /* Log Read Request parm packet*/
       {
       ULONG     packet_size;                   /* size in bytes of packet  */
       ULONG     packet_revision_number;        /* revision level of pkt.   */
       ULONG     log_file_ID;                   /* ID of Log File to be used*/
       ULONG     flags;                  /*rel position to begin file search*/
       PEVENTKEY pEventKey;                     /* ptr to event key data str*/
       PFILTERBLOCK pFilter;             /* ptr to event filter for search  */
       PULONG    pLogEntryBufferLength;         /* IN: len of callers buffer*/
                                                /*OUT: # bytes put in buffer*/
       PVOID     pLogEntryBuffer;        /* ptr to buffer containing Log Rec*/
      }LREREQUEST, *PLREREQUEST;

/*--------------------------------------------------------------------------*/
/* Structure for LogFormatEntry API:                                        */
/*--------------------------------------------------------------------------*/
typedef struct _LFEREQUEST            /* Log Format Request parameter packet*/
       {
       ULONG    packet_size;                     /* size in bytes of packet */
       ULONG    packet_revision_number;          /* revision level of pkt.  */
       PVOID    log_entry_buffer;                /* ptr to Log Entry record */
       PVOID    locale_object;                   /* this should be          */
                                                 /* LocaleObject PVOID here */
                                                 /* so as to not include    */
                                                 /* unicode lib in this file*/
       PULONG   number_of_detail_records;        /* ltd count               */
       ULONG    flags;                           /* 4 byte flag word        */
       PULONG   string_buffer_length;            /* IN: ptr to caller string*/
                     /*buffer length; OUTPUT: bytes of strings placed on buf*/
       PVOID    string_buffer;                   /* ptr to outputbuffer     */
      }LFEREQUEST, *PLFEREQUEST;

/*--------------------------------------------------------------------------*/
/* Structure  for LogOpenEventNotification:                                 */
/*--------------------------------------------------------------------------*/
typedef struct _LOENREQUEST      /* LogOpenEventNotification paramete packet*/
       {
       ULONG    packet_size;                     /* size in bytes of packet */
       ULONG    packet_revision_number;          /* revision level of pkt.  */
       ULONG    log_file_ID;                     /* ID for this Log File    */
       ULONG    read_flags;                      /* return record           */
       PULONG   pLogNotify;                      /* ptr to hdl of event que */
       PFILTERBLOCK pFilter;                     /* ptr to event filter data*/
      }LOENREQUEST, *PLOENREQUEST;

/*--------------------------------------------------------------------------*/
/* Structure  for LogCloseEventNotification :                               */
/*--------------------------------------------------------------------------*/
typedef struct _LCENREQUEST           /* LogCloseEventQueue parameter packet*/
       {
       ULONG    packet_size;                     /* size in bytes of packet */
       ULONG    packet_revision_number;          /* revision level of pkt.  */
       ULONG    LogNotify;                       /* handle of the event que */
      }LCENREQUEST, *PLCENREQUEST;


/*--------------------------------------------------------------------------*/
/* Structure  for LogChangeEventfilter :                                    */
/*--------------------------------------------------------------------------*/
typedef struct _LCEFREQUEST         /* LogChangeEventFilter parameter packet*/
       {
       ULONG    packet_size;                     /* size in bytes of packet */
       ULONG    packet_revision_number;          /* rev level of parm pkt   */
       ULONG    purge_flags;                     /* purge event notif queue */
       ULONG    LogNotify;                       /* handle of event notif q */
       PFILTERBLOCK pFilter;                     /* ptr to event filter data*/
      }LCEFREQUEST, *PLCEFREQUEST;


/*--------------------------------------------------------------------------*/
/* Structure  for LogWaitEvent :                                            */
/*--------------------------------------------------------------------------*/
typedef struct _LWEREQUEST                  /* LogWaitEvent parameter packet*/
       {
       ULONG     packet_size;                    /* size in bytes of packet */
       ULONG     packet_revision_number;         /* revision level of pkt.  */
       ULONG     LogNotify;                      /* event notification id   */
       PEVENTKEY pEventKey;                      /* ptr to event key data   */
       PULONG    log_entry_buffer_length;
       PVOID     pLogEntryBuffer;
       ULONG     timeout;                  /*msec that caller will wait     */
       ULONG     queue_flags;                    /* 1 - dequeue on error    */
       PULONG    pathname_length;                /* len of filename         */
       PVOID     pathname;                       /* ptr to filename         */
      }LWEREQUEST, *PLWEREQUEST;

/*--------------------------------------------------------------------------*/
/* Function prototypes                                                      */
/*--------------------------------------------------------------------------*/
extern APIRET APIENTRY LogOpenFile(ULONG service, PVOID pOpenFile);
extern APIRET APIENTRY LogCloseFile(ULONG service, PVOID pCloseFile);
extern APIRET APIENTRY LogReadEntry(ULONG service, PVOID pReadEntry);
extern APIRET APIENTRY LogFormatEntry(ULONG service, PVOID pFormatEntry);
extern APIRET APIENTRY LogOpenEventNotification(ULONG service, PVOID pOpenEventNotification);
extern APIRET APIENTRY LogCloseEventNotification(ULONG service, PVOID pCloseEventNotification);
extern APIRET APIENTRY LogChangeEventFilter(ULONG service, PVOID pChangeEventFilter);
extern APIRET APIENTRY LogWaitEvent(ULONG service, PVOID pWaitEvent);

#endif
