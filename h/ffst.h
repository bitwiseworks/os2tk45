/********************** START OF HEADER SPECIFICATION *********************/
/*                                                                        */
/* FILE NAME:  FFST.H                                                     */
/*                                                                        */
/* FUNCTIONS:  This header file contains the data structures pertaining   */
/*             to the FFSTProbe, FFSTSetConfiguration, and                */
/*             FFSTQueryConfiguration.                                    */
/*                                                                        */
/* (C) COPYRIGHT International Business Machines Corp., 1994, 1996.       */
/* All Rights Reserved                                                    */
/* Licensed Materials - Property of IBM                                   */
/*                                                                        */
/* US Government Users Restricted Rights - Use, duplication or            */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.      */
/*                                                                        */
/********************* END OF HEADER SPECIFICATION ************************/

#ifndef __FFSTH__
  #define __FFSTH__

#ifndef INCL_FFST
  #define INCL_FFST
#endif

#ifndef NO_DOSUNICALLS
   #define NO_DOSUNICALLS
#endif

#ifndef _ULS_UCONV
   #include <uconv.h>
#endif

#ifndef _ULS_UNIDEF
   #include <unidef.h>
#endif

/********************************************************************************/
/*                                                                              */
/* Return codes from FFSTProbe to application and passed to ERRLOG              */
/*                                                                              */
/********************************************************************************/

/********************************************************************************/
/* FFSTProbe API Return codes for Invalid Pointers passed in by the API user    */
/********************************************************************************/

#define FFST_BASE_RETURN_CODE 100000ul

#define FFST_INVALID_PRODUCT_ADDRESS                 FFST_BASE_RETURN_CODE +  5
#define FFST_INVALID_FFST_ADDRESS                    FFST_BASE_RETURN_CODE + 10
#define FFST_INVALID_PRODDATA_ADDRESS                FFST_BASE_RETURN_CODE + 15
#define FFST_INVALID_MESSAGE_ADDRESS                 FFST_BASE_RETURN_CODE + 16
#define FFST_INVALID_PMSGINSDATA_ADDRESS             FFST_BASE_RETURN_CODE + 17
#define FFST_INVALID_DMI_ADDRESS                     FFST_BASE_RETURN_CODE + 20
#define FFST_INVALID_DUMP_ADDRESS                    FFST_BASE_RETURN_CODE + 25
#define FFST_INVALID_PDUMPUSERDATA_ADDRESS           FFST_BASE_RETURN_CODE + 26
#define FFST_INVALID_USER_AREA_ADDRESS               FFST_BASE_RETURN_CODE + 30
#define FFST_INVALID_LOG_DATA_ADDRESS                FFST_BASE_RETURN_CODE + 35

#define FFST_INVALID_INSERT_MESSAGES_ADDRESS         FFST_BASE_RETURN_CODE + 40
#define FFST_INVALID_VENDOR_TAG_ADDRESS              FFST_BASE_RETURN_CODE + 45
#define FFST_INVALID_TAG_ADDRESS                     FFST_BASE_RETURN_CODE + 50
#define FFST_INVALID_MODULENAME_ADDRESS              FFST_BASE_RETURN_CODE + 55
#define FFST_INVALID_REVISION_ADDRESS                FFST_BASE_RETURN_CODE + 60
#define FFST_INVALID_PRODUCT_ID_ADDRESS              FFST_BASE_RETURN_CODE + 65
#define FFST_INVALID_DMI_FIXLVL_ADDRESS              FFST_BASE_RETURN_CODE + 70
#define FFST_INVALID_TEMPFN_ADDRESS                  FFST_BASE_RETURN_CODE + 75

#define FFST_INVALID_USER_STRUCTURE_TITLE_ADDRESS    FFST_BASE_RETURN_CODE + 80
#define FFST_INVALID_CONFIG_SUBPRODDATA_ADDRESS      FFST_BASE_RETURN_CODE + 85
#define FFST_INVALID_DMI_MODIFICATION_LEVEL_ADDRESS  FFST_BASE_RETURN_CODE + 90

/********************************************************************************/
/* FFSTProbe API Return codes for Invalid Packet Revision Numbers               */
/********************************************************************************/

#define FFST_INVALID_PRODUCT_REVISION                FFST_BASE_RETURN_CODE + 95
#define FFST_INVALID_DMI_REVISION                    FFST_BASE_RETURN_CODE + 100
#define FFST_INVALID_FFST_REVISION                   FFST_BASE_RETURN_CODE + 105

/********************************************************************************/
/* Return code for Invalid FFSTProbe Severity                                   */
/********************************************************************************/

#define FFST_INVALID_SEVERITY                        FFST_BASE_RETURN_CODE + 110

/********************************************************************************/
/* Return code for Invalid Number of Dumps Specified by FFSTProbe User          */
/********************************************************************************/

#define FFST_INVALID_USER_DUMP_NUMBER                FFST_BASE_RETURN_CODE + 115

/********************************************************************************/
/* Return code for Invalid Number of Insert Texts                               */
/********************************************************************************/

#define FFST_INVALID_INSERTS_NUMBER                  FFST_BASE_RETURN_CODE + 120
#define FFST_INVALID_INSERT_TEXT_ADDRESS             FFST_BASE_RETURN_CODE + 125

/********************************************************************************/
/* Return code for Invalid Probe Flags                                          */
/********************************************************************************/

#define FFST_INVALID_PROBE_FLAGS                     FFST_BASE_RETURN_CODE + 130

/********************************************************************************/
/* Return codes for Invalid PSTAT data and Process Environment data             */
/********************************************************************************/

#define FFST_INVALID_PSTAT_DATA                      FFST_BASE_RETURN_CODE + 135
#define FFST_INVALID_PROCESS_ENV                     FFST_BASE_RETURN_CODE + 140

/********************************************************************************/
/* Return codes for Invalid Packet Sizes                                        */
/********************************************************************************/

#define FFST_INVALID_PRODUCTDATA_PACKET_SIZE         FFST_BASE_RETURN_CODE + 145
#define FFST_INVALID_DMIDATA_PACKET_SIZE             FFST_BASE_RETURN_CODE + 150
#define FFST_INVALID_FFSTPARMS_PACKET_SIZE           FFST_BASE_RETURN_CODE + 155

/********************************************************************************/
/* Return codes for DLL Load and Query                                          */
/********************************************************************************/

#define FFST_DLL_QUERY_PROC_ERROR                    FFST_BASE_RETURN_CODE + 160
#define FFST_DLL_LOAD_ERROR                          FFST_BASE_RETURN_CODE + 165

/********************************************************************************/
/* Return codes for FFSTProbe pipe errors                                       */
/********************************************************************************/

#define FFST_CLIENT_PIPE_NOT_CREATED                 FFST_BASE_RETURN_CODE + 170
#define FFST_CLIENT_PIPE_NOT_OPENED                  FFST_BASE_RETURN_CODE + 175

/********************************************************************************/
/* Return code for Multiple Errors Occurring during probe processing            */
/********************************************************************************/

#define FFST_MULTIPLE_SYSTEM_ERRORS                  FFST_BASE_RETURN_CODE + 180

/********************************************************************************/
/* Misc. Return codes                                                           */
/********************************************************************************/

#define CLIENT_PROC_IN_EXIT_LIST_PROCESSING          FFST_BASE_RETURN_CODE + 185
#define FFST_UNICODE_CONVERSION_ERROR                FFST_BASE_RETURN_CODE + 186

/********************************************************************************/
/* Return codes from FFSTProbe to application and not to ERRLOG                 */
/********************************************************************************/
/********************************************************************************/
/* Return code for FFST not active.                                             */
/********************************************************************************/

#define FFST_NOT_ACTIVE                              FFST_BASE_RETURN_CODE + 190

/********************************************************************************/
/*                                                                              */
/* Return codes from FFSTQueryConfiguration and FFSTSetConfiguration            */
/*                                                                              */
/********************************************************************************/

#define FFST_INVALID_CONFIG_USER_BUFFER_SIZE_ADDRESS           FFST_BASE_RETURN_CODE + 195
#define FFST_INVALID_CONFIG_ADDRESS                            FFST_BASE_RETURN_CODE + 200
#define FFST_INVALID_CONFIG_REVISION_NUMBER                    FFST_BASE_RETURN_CODE + 205
#define FFST_INVALID_CONFIG_PACKET_SIZE                        FFST_BASE_RETURN_CODE + 210
#define FFST_INVALID_CONFIG_DUMP_FILE_WRAP                     FFST_BASE_RETURN_CODE + 215
#define FFST_INVALID_CONFIG_DUMP_FILE_DIRECTORY_SIZE           FFST_BASE_RETURN_CODE + 220
#define FFST_INVALID_CONFIG_KEEP_DUP_DUMP                      FFST_BASE_RETURN_CODE + 225
#define FFST_INVALID_CONFIG_DUMP_FILE_DIRECTORY_NAME_ADDRESS   FFST_BASE_RETURN_CODE + 230
#define FFST_INVALID_CONFIG_DUMP_FILE_DIRECTORY_NAME_LENGTH    FFST_BASE_RETURN_CODE + 235
#define FFST_INVALID_CONFIG_NO_DISABLED_PRODUCTS               FFST_BASE_RETURN_CODE + 240
#define FFST_INVALID_CONFIG_DISABLED_PRODUCTS_ADDRESS          FFST_BASE_RETURN_CODE + 245
#define FFST_INVALID_CONFIG_VENDOR_TAG_ADDRESS                 FFST_BASE_RETURN_CODE + 250
#define FFST_INVALID_CONFIG_TAG_ADDRESS                        FFST_BASE_RETURN_CODE + 255
#define FFST_INVALID_CONFIG_REVISION_ADDRESS                   FFST_BASE_RETURN_CODE + 260
#define FFST_INVALID_CONFIG_MESSAGE_POPUP                      FFST_BASE_RETURN_CODE + 265

/********************************************************************************/
/* Return codes for file errors during Configuration processing.                */
/********************************************************************************/

#define FFST_CONFIG_FILE_OPEN_ERROR                            FFST_BASE_RETURN_CODE + 275
#define FFST_CONFIG_FILE_SEARCH_ERROR                          FFST_BASE_RETURN_CODE + 280
#define FFST_CONFIG_FILE_WRITE_ERROR                           FFST_BASE_RETURN_CODE + 285

/********************************************************************************/
/* Return code for an insufficient buffer passed to Configuration.              */
/********************************************************************************/

#define FFST_CONFIG_INSUFFICIENT_BUFFER                        FFST_BASE_RETURN_CODE + 290

/********************************************************************************/
/* Return code for invalid dump file directory driver passed to                 */
/* FFSTSetConfiguration.                                                        */
/********************************************************************************/

#define FFST_INVALID_CONFIG_DUMP_FILE_DIR_DRIVE                FFST_BASE_RETURN_CODE + 295

/********************************************************************************/
/* Return codes for an invalid drive or path passed to                          */
/* FFSTSetConfiguration.                                                        */
/********************************************************************************/

#define FFST_INVALID_DRIVE_REQUESTED                           FFST_BASE_RETURN_CODE + 300
#define FFST_INVALID_PATH_REQUESTED                            FFST_BASE_RETURN_CODE + 305

/********************************************************************************/
/*                                                                              */
/* Return codes from FFSTProbe, FFSTQueryConfiguration, FFSTSetConfiguration    */
/* or other FFST processing.                                                    */
/*                                                                              */
/********************************************************************************/

/********************************************************************************/
/* Return codes for shared memory errors.                                       */
/********************************************************************************/

#define FFST_GET_SHARED_MEM_ERROR                              FFST_BASE_RETURN_CODE + 310
#define FFST_ALLOC_SHARED_MEM_ERROR                            FFST_BASE_RETURN_CODE + 315
#define FFST_FREE_SHARED_MEM_ERROR                             FFST_BASE_RETURN_CODE + 320

/********************************************************************************/
/* Return codes for semaphore errors.                                           */
/********************************************************************************/

#define FFST_SEMAPHORE_TIMEOUT_ERROR                           FFST_BASE_RETURN_CODE + 325
#define FFST_SEMAPHORE_OPEN_ERROR                              FFST_BASE_RETURN_CODE + 330
#define FFST_SEMAPHORE_RELEASE_ERROR                           FFST_BASE_RETURN_CODE + 335
#define FFST_SEMAPHORE_CLOSE_ERROR                             FFST_BASE_RETURN_CODE + 340
#define FFST_SEMAPHORE_REQUEST_ERROR                           FFST_BASE_RETURN_CODE + 345
#define FFST_SEMAPHORE_G_ERROR                                 FFST_BASE_RETURN_CODE + 350

/********************************************************************************/
/* Return codes for dump engine processing which may be posted to the           */
/* SysLog.                                                                      */
/********************************************************************************/

#define FFST_DUMP_HDR_FILE_OPEN_ERROR                          FFST_BASE_RETURN_CODE + 355
#define FFST_INDEX_FILE_OPEN_ERROR                             FFST_BASE_RETURN_CODE + 360
#define FFST_DUMP_FILE_OPEN_ERROR                              FFST_BASE_RETURN_CODE + 365
#define FFST_MEMORY_ALLOCATION_ERROR                           FFST_BASE_RETURN_CODE + 370
#define FFST_QUERYFS_ERROR                                     FFST_BASE_RETURN_CODE + 375
#define FFST_DUMP_WRAP_ERROR                                   FFST_BASE_RETURN_CODE + 380
#define FFST_TRACE_RENAME_ERROR                                FFST_BASE_RETURN_CODE + 385
#define FFST_PROC_DUMP_RENAME_ERROR                            FFST_BASE_RETURN_CODE + 390

/********************************************************************************/
/* Return codes that may be in the SysLog during Worker bringup.                */
/********************************************************************************/

#define FFST_CREATE_CONFIG_SEMAPHORE_ERROR                     FFST_BASE_RETURN_CODE + 395
#define FFST_CREATE_DUMP_SEMAPHORE_ERROR                       FFST_BASE_RETURN_CODE + 400
#define FFST_WORKER_ALLOC_SHARED_MEM_ERROR                     FFST_BASE_RETURN_CODE + 405
#define FFST_WORKER_GET_SHARED_MEM_ERROR                       FFST_BASE_RETURN_CODE + 410
#define FFST_CREATE_PCT_SEMAPHORE_ERROR                        FFST_BASE_RETURN_CODE + 415
#define FFST_FILE_ALREADY_EXISTS                               FFST_BASE_RETURN_CODE + 420
#define FFST_SPECIFIED_FILE_NOT_FOUND                          FFST_BASE_RETURN_CODE + 425
#define FFST_DUMP_VALIDATION_ERROR                             FFST_BASE_RETURN_CODE + 430
#define FFST_CONFIG_MEMORY_FILLED                              FFST_BASE_RETURN_CODE + 435

#define FFST_WORKER_SETCONFIG_ERROR                            FFST_BASE_RETURN_CODE + 440
#define FFST_WORKER_PIPE_NOT_CREATED                           FFST_BASE_RETURN_CODE + 445
#define FFST_WORKER_FFST_CONFIG_NOT_OKAY                       FFST_BASE_RETURN_CODE + 450
#define FFST_WORKER_MUTEXSEM_NOT_RELEASED                      FFST_BASE_RETURN_CODE + 455
#define FFST_WORKER_ALREADY_ACTIVE                             FFST_BASE_RETURN_CODE + 460
#define FFST_WORKER_INITWORKER_FAILED                          FFST_BASE_RETURN_CODE + 465
#define FFST_WORKER_NOT_ACTIVE                                 FFST_BASE_RETURN_CODE + 470

/********************************************************************************/
/*                                                                              */
/* Miscellaneous defines used by FFST                                           */
/*                                                                              */
/********************************************************************************/

/********************************************************************************/
/* Maximum number of message inserts and dumps for FFSTProbe                    */
/********************************************************************************/

#define MAX_MSG_INSERTS                    9
#define MAX_USER_DUMPS                     30

/********************************************************************************/
/* Major and Minor error codes for FFST                                         */
/********************************************************************************/

#define FFST_MAJOR_CODE                    0x056
#define FFST_MINOR_CODE                    0x057

/********************************************************************************/
/* Packet Revision number defines                                               */
/********************************************************************************/

#define PRODUCTDATA_REVISION_NUMBER_1      1
#define PRODUCTDATA_UNICODE                1
#define PRODUCTDATA_ASCII                  2

#define DMIDATA_REVISION_NUMBER_1          1
#define DMIDATA_UNICODE                    1
#define DMIDATA_ASCII                      2

#define NOTIFYINFO_REVISION_NUMBER_1       1

#define FFSTPARMS_WPOS_REVISION_NUMBER_1   1
#define FFSTPARMS_OS2_REVISION_NUMBER_1    2
#define FFSTPARMS_OS2_UNICODE              1
#define FFSTPARMS_OS2_ASCII                3

/********************************************************************************/
/* Severity definitions                                                         */
/********************************************************************************/

#ifndef SEVERITY1
  #define SEVERITY1                        1
  #define SEVERITY2                        2
  #define SEVERITY3                        3
  #define SEVERITY4                        4
  #define SEVERITY5                        5
  #define SEVERITY6                        6
#endif

/********************************************************************************/
/* Probe Flags                                                                  */
/********************************************************************************/

#define PSTAT_FLAG                         0x01
#define PROC_ENV_FLAG                      0x02
#define RESERVED                           0x04
#define PROCESS_DUMP_FLAG                  0x08

/********************************************************************************/
/* Configuration API defines.                                                   */
/********************************************************************************/

#define   CONFIGPARMS_WPOS_REVISION_NUMBER_1                     1
#define   CONFIGPARMS_OS2_REVISION_NUMBER_1                      2

#define   FFST_DUMP_WRAP_OFF                                     0
#define   FFST_DUMP_WRAP_ON                                      1
#define   FFST_DUMP_WRAP_NO_CHANGE                               2

#define   FFST_KEEP_DUP_DUMP_OFF                                 0
#define   FFST_KEEP_DUP_DUMP_ON                                  1
#define   FFST_KEEP_DUP_DUMP_NO_CHANGE                           2

#define   FFST_DUMP_FILE_DIRECTORY_SIZE_NO_CHANGE                0
#define   FFST_DUMP_FILE_DIRECTORY_LENGTH_NO_CHANGE              0
#define   FFST_NO_OF_PROBE_DISABLED_PRODUCTS_NO_CHANGE           0xFFFFFFFF

/********************************************************************************/
/*                                                                              */
/* Structures used by FFST                                                      */
/*                                                                              */
/********************************************************************************/

/********************************************************************************/
/* FFSTProbe Structure Declares                                                 */
/********************************************************************************/

   typedef struct
   {                                            /* Product Information Packet   */
      ULONG    packet_size;                     /* Length of product data packet*/
      ULONG    packet_revision_number;          /* revision number of this pkt  */
      void   * DMI_vendor_tag;                  /* ptr to vendor short name     */
      void   * DMI_tag;                         /* ptr to vendor product name   */
      void   * DMI_revision;                    /* ptr to vendor revision num   */
   } PRODUCTDATA, * PPRODUCTDATA;


   typedef struct
   {                                            /* DMI data structure           */
      ULONG    packet_size;                     /* Length of packet             */
      ULONG    packet_revision_number;          /* Revision number of packet    */
      void   * DMI_product_ID;                  /* ptr to product number        */
      void   * DMI_modification_level;          /* dmi product mod level        */
      void   * DMI_fix_level;                   /* dmi product fix level        */
      ULONG    template_filename_length;        /* length of template filename  */
      void   * template_filename;               /* ptr to template filename     */
    } DMIDATA, * PDMIDATA;


   typedef struct
   {                                            /* Product Information Packet   */
      PPRODUCTDATA   pProductData;              /* ptr to Probe Information     */
      PDMIDATA       pDMIData;                  /* ptr to Product DMI Data      */
   } PRODUCTINFO, * PPRODUCTINFO;


   typedef struct
   {                                            /* Insert Text Data             */
      ULONG    insert_number;                   /* number of given insert       */
      void   * insert_text;                     /* ptr to insert text           */
   } MSGINSTXT, * PMSGINSTXT;


   typedef struct
   {                                            /* Insert Text Data             */
      ULONG     no_inserts;                     /* number of inserts provided   */
      MSGINSTXT MsgInsTxt [ MAX_MSG_INSERTS ];  /* struct containing inserts    */
   } MSGINSDATA, * PMSGINSDATA;


   typedef struct
   {                                            /* User defined dump data       */
      ULONG    var_n_length;                    /* length of data structure     */
      PVOID    var_n;                           /* pointer to data structure    */
   } DUMPDATAVAR, * PDUMPDATAVAR;


   typedef struct
   {                                            /* User defined dump data       */
      ULONG       no_of_variables;              /* number of dumps              */
      DUMPDATAVAR DumpDataVar [ MAX_USER_DUMPS ];    /* dump data variable      */
   } DUMPUSERDATA, * PDUMPUSERDATA;


   typedef struct
   {                                            /* FFSTPROBE Control Block      */
      ULONG         packet_size;                /* Length of packet             */
      ULONG         packet_revision_number;     /* Revision number of packet    */
      void        * module_name;                /* ptr to module identifier name*/
      ULONG         probe_ID;                   /* probe id                     */
      ULONG         severity;                   /* Severity of probe fired      */
      ULONG         template_record_ID;         /* Log Entry Format Template    */
      PMSGINSDATA   pMsgInsData;                /* Message inserts              */
      ULONG         probe_flags;                /* Flags set by application     */
      PDUMPUSERDATA pDumpUserData;              /* Dump Structures              */
      ULONG         log_user_data_length;       /* length of log user data      */
      PVOID         log_user_data;              /* Data to be placed in errorlog*/
   } FFSTPARMS, * PFFSTPARMS;

/********************************************************************************/
/* FFSTSetConfiguration and FFSTQueryConfiguration structures                   */
/********************************************************************************/

   typedef struct
   {                                            /* Disabled products structure  */
      void    * DMI_vendor_tag;                 /* ptr to vendor tag information*/
      void    * DMI_tag;                        /* ptr to tag information       */
      void    * DMI_revision;                   /* ptr to revision information  */
   } DISABLEPRODUCT, * PDISABLEPRODUCT;

   typedef struct
   {                                            /* Configuration API's structure*/
      ULONG           packet_size;              /* size of the packet           */
      ULONG           packet_revision_number;   /* revision of the packet       */
      ULONG           dump_file_wrap;           /* wrap the dump files flag     */
      ULONG           dump_file_directory_size; /* size to store dumps          */
      ULONG           keep_dup_dump;            /* keep duplicate dumps flag    */
      ULONG           dump_file_directory_name_length; /* length of dumpdir     */
      void          * dump_file_directory_name;        /* ptr to dump dir name  */
      ULONG           no_of_probe_disabled_products;   /* no of products dis    */
      PDISABLEPRODUCT pDisableProduct;          /* ptr to disabled products stuc*/
      ULONG           message_pop_up;           /* OS2 flag for message popup   */
   } CONFIGPARMS, * PCONFIGPARMS;


/********************************************************************************/
/* FFSTProbe Prototypes                                                         */
/********************************************************************************/
APIRET APIENTRY FFSTProbe ( PPRODUCTINFO pProductInfo
                          , PFFSTPARMS   pFFSTParms );

/********************************************************************************/
/* Prototypes for Configuration API's                                           */
/********************************************************************************/
APIRET APIENTRY FFSTSetConfiguration   ( PCONFIGPARMS   pConfigParms );
APIRET APIENTRY FFSTQueryConfiguration ( PULONG         buffer_length
                                       , PCONFIGPARMS   pConfigParms );

#endif /* __FFSTH__ */
