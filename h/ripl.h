/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/


/********************************************************************
 *                                                                  *
 *  About this file ...  RIPL.H                                     *
 *                                                                  *
 *  This file contains information about the RIPL (Remote IPL)      *
 *  APIs.  For each API class there is a section on:                *
 *                                                                  *
 *      Function prototypes.                                        *
 *                                                                  *
 *       Data structure templates.                                  *
 *                                                                  *
 *        Definition of special values.                             *
 *                                                                  *
 *                                                                  *
 *  NOTE:  You must include NETCONS.H before this file, since this  *
 *           file depends on values defined in NETCONS.H.           *
 *                                                                  *
 ********************************************************************/


#if !defined NETRIPL_INCLUDED
#define NETRIPL_INCLUDED

#ifdef  ANY_32_BIT
#pragma pack(2)
#endif

/****************************************************************
 *                                                              *
 *              Function prototypes - NetxxxRIPLMachine         *
 *                                    NetxxxRIPLMachineInfo     *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
 NetCreateRIPLMachine (const unsigned char LSFAR *,   /* server               */
                       unsigned short,                /* level (2, 12)        */
                       unsigned char LSFAR *,         /* buffer (send)        */
                       unsigned short);               /* buffer length        */

extern API_FUNCTION
 NetDeleteRIPLMachine (const unsigned char LSFAR *,   /* server               */
                       unsigned char LSFAR *);        /* machine name(client) */

extern API_FUNCTION
 NetGetRIPLMachineInfo (const unsigned char LSFAR *,  /* server               */
                        unsigned char LSFAR *,        /* machine name(client) */
                        unsigned short,               /* level (0, 1, 2)      */
                        unsigned char LSFAR *,        /* buffer (receive)     */
                        unsigned short,               /* buffer length        */
                        unsigned short LSFAR *);      /* bytes available      */

extern API_FUNCTION
 NetSetRIPLMachineInfo (const unsigned char LSFAR *,  /* server               */
                        unsigned char LSFAR *,        /* machine name(client) */
                        unsigned short,               /* level (1, 2)         */
                        unsigned char LSFAR *,        /* buffer (send)        */
                        unsigned short,               /* buffer length        */
                        unsigned short);              /* parameter number     */

extern API_FUNCTION
 NetEnumRIPLMachine (const unsigned char LSFAR *,     /* server               */
                     unsigned short,                  /* level (0, 1, 2)      */
                     unsigned short,                  /* type                 */
                     unsigned char LSFAR *,           /* buffer (receive)     */
                     unsigned short,                  /* buffer length        */
                     unsigned short LSFAR *,          /* entries returned     */
                     unsigned short LSFAR *);         /* entries available    */

/****************************************************************
 *                                                              *
 *              Data structure templates - RIPL                 *
 *                                                              *
 ****************************************************************/
#define IPALEN 15                    //  length of IP address
#define MAX_API_BUFFER_SIZE  0xffff
#define VGA_VIDEO_TYPE           "VGA_VIDEO_TYPE"
#define PARAMETER_NOT_SUPPORTED  "PARAMETER_NOT_SUPPORTED"
#if !defined MCNLEN
#define MCNLEN  8                    //  length of machine class name
#endif

struct ripl_machine_info_0
   {
    unsigned char                     rmi0_name[CNLEN + 1];
   };

struct ripl_machine_info_1
   {
    unsigned char                     rmi1_name[CNLEN + 1];
    unsigned char LSFAR * LSPTR       rmi1_remark;
   };

struct ripl_machine_info_2
   {
    unsigned char                     rmi2_name[CNLEN + 1];
    unsigned char LSFAR * LSPTR       rmi2_remark;
    unsigned char                     rmi2_adapter_address[NNLEN + 1];
    unsigned char                     rmi2_pad_1;
    unsigned char                     rmi2_OS2_boot_drive;
    unsigned char                     rmi2_pad_2;
    unsigned char                     rmi2_DOS_imageid[DOSIMGLEN + 1];
    unsigned char                     rmi2_pad_3;
    unsigned char LSFAR * LSPTR       rmi2_DOS_srv_rec_ID;
    unsigned char LSFAR * LSPTR       rmi2_OS2_srv_rec_ID;
    unsigned long                     rmi2_OS2_config_flags;
   };


struct ripl_machine_info_3
   {
    unsigned char                 rmi3_name[CNLEN+1];
    unsigned char  LSFAR * LSPTR  rmi3_remark;
    unsigned char                 rmi3_adapter_address[NNLEN+1];
    unsigned char                 rmi3_pad_1[3];
    unsigned char                 rmi3_machine_status;
    unsigned char                 rmi3_pad_2[3];
    unsigned char                 rmi3_OS_version[NNLEN+1];
    unsigned char                 rmi3_pad_3[3];
    unsigned char                 rmi3_OS_boot_drive;
    unsigned char                 rmi3_pad_4[3];
    unsigned char                 rmi3_local_swapper;
    unsigned char                 rmi3_pad_5[3];
    unsigned char                 rmi3_machine_class_name[MCNLEN+1];
    unsigned char                 rmi3_pad_6[3];
    unsigned char                 rmi3_boot_method;
    unsigned char                 rmi3_boot_protocol;
    unsigned char                 rmi3_additional_protocol;
    unsigned char                 rmi3_tcpbeui_node;
    unsigned char  LSFAR * LSPTR  rmi3_network_adapter_type;
    unsigned char  LSFAR * LSPTR  rmi3_video_monitor;
    unsigned char  LSFAR * LSPTR  rmi3_video_resolution;
    unsigned char  LSFAR * LSPTR  rmi3_nbns_ipaddress;
    unsigned char  LSFAR * LSPTR  rmi3_nbdd_ipaddress;
    unsigned char  LSFAR * LSPTR  rmi3_netbios_scope;
    unsigned char  LSFAR * LSPTR  rmi3_reserved_2;
    unsigned char                 rmi3_ip_DHCP;
    unsigned char                 rmi3_pad_7[3];
    unsigned char                 rmi3_ip_DDNS;
    unsigned char                 rmi3_pad_8[3];
    unsigned char                 rmi3_ip_ipaddress[IPALEN+1];
    unsigned char                 rmi3_ip_subnetmask[IPALEN+1];
    unsigned char                 rmi3_ip_router[IPALEN+1];
    unsigned char                 rmi3_ip_nameserver[IPALEN+1];
    unsigned char  LSFAR * LSPTR  rmi3_ip_domainname;
    unsigned char  LSFAR * LSPTR  rmi3_ip_hostname;
    unsigned char  LSFAR * LSPTR  rmi3_locally_administered_addr;
    unsigned long                 rmi3_number_printer_structs;
   };


struct ripl_machine_info_4
   {
    unsigned char                 rmi4_name[CNLEN+1];
    unsigned char  LSFAR * LSPTR  rmi4_DOS_srv_rec_ID;
    unsigned char  LSFAR * LSPTR  rmi4_OS2_srv_rec_ID;
    unsigned char  LSFAR * LSPTR  rmi4_NC_srv_rec_ID;
   };


struct ripl_machine_info_12
   {
    unsigned char                     rmi12_name[CNLEN + 1];
    unsigned char LSFAR * LSPTR       rmi12_remark;
    unsigned char                     rmi12_adapter_address[NNLEN + 1];
    unsigned char                     rmi12_pad_1;
    unsigned char                     rmi12_model_name[CNLEN + 1];
   };


struct ripl_machineclass_info_0
   {
    unsigned char                rmci0_name[MCNLEN+1];
    unsigned char                rmci0_pad_1[3];
   };

struct ripl_machineclass_info_1
   {
    unsigned char                rmci1_name[MCNLEN+1];
    unsigned char                rmci1_pad_1[3];
    unsigned char *              rmci1_remark;
   };

struct ripl_machineclass_info_3
   {
    unsigned char                rmci3_name[MCNLEN+1];
    unsigned char                rmci3_pad_1[3];
    unsigned char *              rmci3_remark;
    BYTE                         rmci3_bus_type;
    unsigned char                rmci3_pad_2[3];
    BYTE                         rmci3_diskette;
    unsigned char                rmci3_pad_3[3];
    BYTE                         rmci3_serial;
    unsigned char                rmci3_pad_4[3];
    BYTE                         rmci3_parallel;
    unsigned char                rmci3_pad_5[3];
    BYTE                         rmci3_hardfile_type;
    unsigned char                rmci3_pad_6[3];
    BYTE                         rmci3_boot_method;
    unsigned char                rmci3_pad_7[3];
    BYTE                         rmci3_PCMCIA_options;
    unsigned char                rmci3_pad_8[3];
    BYTE                         rmci3_keyboard_type;
    unsigned char                rmci3_pad_9[3];
    BYTE                         rmci3_keyboard_mode;
    unsigned char                rmci3_pad_10[3];
    unsigned long                rmci3_config_flags;
    unsigned char *              rmci3_mouse;
    unsigned char *              rmci3_network_adapter_type;
    unsigned char *              rmci3_video_type;
    unsigned char *              rmci3_video_monitor;
    unsigned char *              rmci3_video_resolution;
    unsigned char *              rmci3_country_code;
    unsigned char *              rmci3_country_keyboard;
    unsigned char *              rmci3_cdrom;
    unsigned char *              rmci3_multimedia;
    unsigned char *              rmci3_scsi_adapter;
    unsigned char *              rmci3_PCMCIA;
    unsigned char *              rmci3_user_config_sys;
    unsigned long                rmci3_number_printer_structs;
   };

struct ripl_machineclass_info_4
   {
    unsigned char                rmci4_name[MCNLEN+1];
    unsigned char                rmci4_pad_1[3];
    unsigned char *              rmci4_remark;
    BYTE                         rmci4_edit_enabled;
    BYTE                         rmci4_reserved_1[3];
   };

struct ripl_listparm_osversion_info
   {
    unsigned char                rlposi_osname[CNLEN + 1];
    unsigned char *              rlposi_remark;
   };

struct ripl_listparm_ascii_info
   {
    unsigned char *              rlpai_stringpointer;
   };

struct ripl_listparm_adapter_info
   {
    unsigned char *              rlpnai_adapter_title;
    BYTE                         rlpnai_boot_methods;
    BYTE                         rlpnai_laa_support;
    BYTE                         rlpnai_reserved_1[2];
   };

struct ripl_mclistparm_info
   {
    unsigned char *              rlpmci_description;
    BYTE                         rlpmci_value;
    BYTE                         rlpmci_reserved_1[3];
   };

/****************************************************************
 *                                                              *
 *           Data structure templates - WorkSpace Level         *
 *                                                              *
 ****************************************************************/
struct WSLevel_info_1
{
    ULONG  wsli_version_major;
    ULONG  wsli_version_minor;
    UCHAR  wsli_CSD_current[8];
    UCHAR  wsli_CSD_previous[8];
    UCHAR  wsli_country[1];
    UCHAR  wsli_pad[3];
    ULONG  wsli_reserved1;
    ULONG  wsli_reserved2;
    ULONG  wsli_reserved3;
};

#ifdef PURE_32

/* Include NCPRINTER structures */
#define INCL_NCPRINTER
#include <ncapi.h>

/****************************************************************
 *                                                              *
 *   32-bit Function prototypes - NetxxxRIPLMachine             *
 *                                NetxxxRIPLMachineInfo         *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
 Net32CreateRIPLMachine (const unsigned char *,       /* server               */
                         unsigned long,               /* level (2, 12)        */
                         unsigned char *,             /* buffer (send)        */
                         unsigned long);              /* buffer length        */

extern API32_FUNCTION
 Net32DeleteRIPLMachine (const unsigned char *,       /* server               */
                         unsigned char *);            /* machine name(client) */

extern API32_FUNCTION
 Net32GetRIPLMachineInfo (const unsigned char *,      /* server               */
                          unsigned char       *,      /* machine name(client) */
                          unsigned long,              /* level (0, 1, 2)      */
                          unsigned char       *,      /* buffer (receive)     */
                          unsigned long,              /* buffer length        */
                          unsigned long       *);     /* bytes available      */

extern API32_FUNCTION
 Net32SetRIPLMachineInfo (const unsigned char *,      /* server               */
                          unsigned char       *,      /* machine name(client) */
                          unsigned long,              /* level (1, 2)         */
                          unsigned char       *,      /* buffer (send)        */
                          unsigned long,              /* buffer length        */
                          unsigned long);             /* parameter number     */

extern API32_FUNCTION
 Net32EnumRIPLMachine (const unsigned char *,         /* server               */
                       unsigned long,                 /* level (0, 1, 2)      */
                       unsigned long,                 /* type                 */
                       unsigned char        *,        /* buffer (receive)     */
                       unsigned long,                 /* buffer length        */
                       unsigned long        *,        /* entries returned     */
                       unsigned long        *);       /* entries available    */

extern API32_FUNCTION
 Net32RIPLMachineCreate (const char *,       /* server               */
                         unsigned long,      /* level (2, 3, 12)     */
                         char       *,       /* buffer (send)        */
                         unsigned long);     /* buffer length        */

extern API32_FUNCTION
 Net32RIPLMachineGetInfo (const char     *,      /* server               */
                          char           *,      /* machine name(client) */
                          unsigned long,         /* level (0, 1, 2, 3)   */
                          char           *,      /* buffer (receive)     */
                          unsigned long,         /* buffer length        */
                          unsigned long  *);     /* bytes available      */

extern API32_FUNCTION
 Net32RIPLMachineSetInfo (const char *,      /* server               */
                          char       *,      /* machine name(client) */
                          unsigned long,     /* level (1, 2, 3)      */
                          char       *,      /* buffer (send)        */
                          unsigned long,     /* buffer length        */
                          unsigned long);    /* parameter number     */

extern API32_FUNCTION
 Net32RIPLMachineEnum (const char     *,        /* server               */
                       unsigned long,           /* level (0, 1, 2, 3, 4)*/
                       unsigned long,           /* type                 */
                       char           *,        /* buffer (receive)     */
                       unsigned long,           /* buffer length        */
                       unsigned long  *,        /* entries returned     */
                       unsigned long  *);       /* entries available    */

extern API32_FUNCTION
 Net32RIPLMachineListParmEnum (const char *,    /* server               */
                       unsigned long,           /* parm Id              */
                       char           *,        /* Machine name         */
                       char           *,        /* Machine class        */
                       char           *,        /* OS Version Id        */
                       char           *,        /* Monitor Id           */
                       char           *,        /* buffer (receive)     */
                       unsigned long,           /* buffer length        */
                       unsigned long  *,        /* entries returned     */
                       unsigned long  *);       /* entries available    */

extern API32_FUNCTION
 Net32RIPLMachineDelete(const char *,           /* server               */
                        char *,                 /* machine name         */
                        ULONG);                 /* reserved             */

extern API32_FUNCTION
 Net32RIPLMachineClassEnum (const char *,       /* server               */
                       char            *,       /* OS version id        */
                       unsigned long,           /* level (0, 1, 3)      */
                       char            *,       /* buffer (receive)     */
                       unsigned long,           /* buffer length        */
                       unsigned long   *,       /* entries returned     */
                       unsigned long   *);      /* entries available    */

extern API32_FUNCTION
 Net32RIPLMachineClassGetInfo (const char *,    /* server               */
                       char           *,        /* OS version id        */
                       char           *,        /* machine class name   */
                       unsigned long,           /* level (0, 1,  3)     */
                       char           *,        /* buffer (receive)     */
                       unsigned long,           /* buffer length        */
                       unsigned long  *);       /* bytes available      */

extern API32_FUNCTION
 Net32RIPLMachineClassCreate (const char *,     /* server               */
                       char           *,        /* OS version id        */
                       unsigned long,           /* level (3)            */
                       char           *,        /* buffer (send)        */
                       unsigned long);          /* buffer length        */

extern API32_FUNCTION
 Net32RIPLMachineClassSetInfo (const char *,    /* server               */
                       char           *,        /* OS version id        */
                       char           *,        /* machine class        */
                       unsigned long,           /* level (3)            */
                       char           *,        /* buffer (send)        */
                       unsigned long,           /* buffer length        */
                       unsigned long);          /* parameter number     */

extern API32_FUNCTION
 Net32RIPLMachineClassDelete (const char *,     /* server               */
                       char           *,        /* OS version id        */
                       char           *);       /* machine class        */

extern API32_FUNCTION
 Net32RIPLMachineClassListParmEnum (const char *,  /* server               */
                       unsigned long,              /* parm Id              */
                       char            *,          /* OS version id        */
                       char            *,          /* VideoType            */
                       char            *,          /* Monitor              */
                       char            *,          /* buffer (receive)     */
                       unsigned long,              /* buffer length        */
                       unsigned long   *,          /* entries returned     */
                       unsigned long   *);         /* entries available    */

/* typedef export entries */

typedef USHORT (* EXPENTRY pfnNet32CreateRIPLMachine)(const unsigned char *,
                         unsigned long,
                         unsigned char *,
                         unsigned long);

typedef USHORT (* EXPENTRY pfnNet32DeleteRIPLMachine)(const unsigned char *,
                         unsigned char *);

typedef USHORT (* EXPENTRY pfnNet32GetRIPLMachineInfo) (const unsigned char *,
                          unsigned char       *,
                          unsigned long,
                          unsigned char       *,
                          unsigned long,
                          unsigned long       *);

typedef USHORT (* EXPENTRY pfnNet32SetRIPLMachineInfo) (const unsigned char *,
                          unsigned char       *,
                          unsigned long,
                          unsigned char       *,
                          unsigned long,
                          unsigned long);

typedef USHORT (* EXPENTRY pfnNet32EnumRIPLMachine) (const unsigned char *,
                       unsigned long,
                       unsigned long,
                       unsigned char        *,
                       unsigned long,
                       unsigned long        *,
                       unsigned long        *);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineCreate) (const char *,
                         unsigned long,
                         char       *,
                         unsigned long);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineGetInfo) (const char     *,
                          char           *,
                          unsigned long,
                          char           *,
                          unsigned long,
                          unsigned long  *);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineSetInfo) (const char *,
                          char       *,
                          unsigned long,
                          char       *,
                          unsigned long,
                          unsigned long);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineEnum) (const char     *,
                       unsigned long,
                       unsigned long,
                       char           *,
                       unsigned long,
                       unsigned long  *,
                       unsigned long  *);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineListParmEnum) (const char *,
                       unsigned long,
                       char           *,
                       char           *,
                       char           *,
                       char           *,
                       char           *,
                       unsigned long,
                       unsigned long  *,
                       unsigned long  *);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineDelete) (const char *,
                       char           *,
                       ULONG);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineClassEnum) (const char *,
                       char            *,
                       unsigned long,
                       char            *,
                       unsigned long,
                       unsigned long   *,
                       unsigned long   *);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineClassGetInfo) (const char *,
                       char           *,
                       char           *,
                       unsigned long,
                       char           *,
                       unsigned long,
                       unsigned long  *);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineClassCreate) (const char *,
                       char           *,
                       unsigned long,
                       char           *,
                       unsigned long);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineClassSetInfo) (const char *,
                       char           *,
                       char           *,
                       unsigned long,
                       char           *,
                       unsigned long,
                       unsigned long);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineClassDelete) (const char *,
                       char           *,
                       char           *);

typedef USHORT (* EXPENTRY pfnNet32RIPLMachineClassListParmEnum) (const char *,
                       unsigned long,
                       char            *,
                       char            *,
                       char            *,
                       char            *,
                       unsigned long,
                       unsigned long   *,
                       unsigned long   *);

/****************************************************************
 *                                                              *
 *   32-bit Function prototype - Net32WSLevelGetInfo            *
 *                                                              *
 ****************************************************************/
extern API32_FUNCTION
 Net32WSLevelGetInfo(const PUCHAR,              /* server               */
                     ULONG,                     /* level                */
                     PUCHAR,                    /* buffer               */
                     ULONG,                     /* buflen               */
                     PULONG,                    /* bytes available      */
                     ULONG);                    /* reserved             */

/* typedef export entries */
typedef USHORT (* EXPENTRY pfnNet32WSLevelGetInfo)(const PUCHAR,
                                                   ULONG,
                                                   PUCHAR,
                                                   ULONG,
                                                   PULONG,
                                                   ULONG);

#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants - RIPL             *
 *                                                              *
 ****************************************************************/

/*
 * values for rmi1_machine_type
 */
#define MTYPE_OS2_RIPL_REQ             0x0001
#define MTYPE_DOS_RIPL_REQ             0x0002

/*
 * values for rmi4_machine_type
 */
#define MTYPE_NC_RIPL_REQ              0x0004
#define MTYPE_ALL_RIPL_REQ             0x00FF

/*
 * values for Machine Parmnum (level 1 and 2)
 */
#define RIPL_MACH_REMARK_PARMNUM                 2
#define RIPL_MACH_ADAPTER_PARMNUM                3
#define RIPL_MACH_OS2_BOOTDRIVE_PARMNUM          5
#define RIPL_MACH_DOS_IMAGEID_PARMNUM            7
#define RIPL_MACH_DOS_SRVRECID_PARMNUM           9
#define RIPL_MACH_OS2_SRVRECID_PARMNUM          10

/*
 * values for Machine Parmnum (level 3)
*/
#define RIPL_MACH_REMARK_PN               2
#define RIPL_MACH_ADAPTER_PN              3
#define RIPL_MACH_STATUS_PN               4
#define RIPL_MACH_OS_VERSION_PN           5
#define RIPL_MACH_OS_BOOT_DRIVE_PN        6
#define RIPL_MACH_LOCAL_SWAPPER_PN        7
#define RIPL_MACH_MCLASS_PN               8
#define RIPL_MACH_BOOT_METHOD_PN          9
#define RIPL_MACH_BOOT_PROTOCOL_PN        10
#define RIPL_MACH_ADDL_PROTOCOL_PN        11
#define RIPL_MACH_TCPBEUI_NODE_PN         12
#define RIPL_MACH_NET_ADAPTER_ID_PN       13
#define RIPL_MACH_VIDEO_MONITOR_PN        14
#define RIPL_MACH_VIDEO_RESOLUTION_PN     15
#define RIPL_MACH_NBNS_IPADDRESS_PN       16
#define RIPL_MACH_NBDD_IPADDRESS_PN       17
#define RIPL_MACH_NETBIOS_SCOPE_PN        18
// Reserved_2                             19
#define RIPL_MACH_IP_DHCP_PN              20
#define RIPL_MACH_IP_DDNS_PN              21
#define RIPL_MACH_IP_IPADDRESS_PN         22
#define RIPL_MACH_IP_SUBNETMASK_PN        23
#define RIPL_MACH_IP_ROUTER_PN            24
#define RIPL_MACH_IP_NAMESERVER_PN        25
#define RIPL_MACH_IP_DOMAINNAME_PN        26
#define RIPL_MACH_IP_HOSTNAME_PN          27
#define RIPL_MACH_LOCALLY_ADMIN_ADDR_PN   28
#define RIPL_MACH_PRINTER_PN              29
#define RIPL_MACH_OS_VERSION2_PN          101
#define RIPL_MACH_NET_DHCP_8022_PN        102


/*
 * Parmnum limits for each machine level
 */
#define RI1_NUM_ELEMENTS            2
#define RI2_NUM_ELEMENTS           10
#define RI3_LAST_ELEMENT           29
#define RI4_LAST_ELEMENT            4


/*
 * values for Machine Class Parmnum
 */
#define RIPL_MCLASS_NAME_PN                1
#define RIPL_MCLASS_REMARK_PN              2
#define RIPL_MCLASS_BUS_TYPE_PN            3
#define RIPL_MCLASS_DISKETTE_PN            4
#define RIPL_MCLASS_SERIAL_PN              5
#define RIPL_MCLASS_PARALLEL_PN            6
#define RIPL_MCLASS_HARDFILE_TYPE_PN       7
#define RIPL_MCLASS_BOOT_METHOD_PN         8
#define RIPL_MCLASS_PCMCIA_OPTIONS_PN      9
#define RIPL_MCLASS_KEYBOARD_TYPE_PN      10
#define RIPL_MCLASS_KEYBOARD_MODE_PN      11
#define RIPL_MCLASS_CONFIG_FLAGS_PN       12
#define RIPL_MCLASS_MOUSE_PN              16
#define RIPL_MCLASS_NETWORK_ADAPTER_PN    17
#define RIPL_MCLASS_VIDEO_TYPE_PN         18
#define RIPL_MCLASS_VIDEO_MONITOR_PN      19
#define RIPL_MCLASS_VIDEO_RESOLUTION_PN   20
#define RIPL_MCLASS_COUNTRY_CODE_PN       21
#define RIPL_MCLASS_COUNTRY_KEYBOARD_PN   22
#define RIPL_MCLASS_CDROM_PN              23
#define RIPL_MCLASS_MULTIMEDIA_PN         24
#define RIPL_MCLASS_SCSI_PN               25
#define RIPL_MCLASS_PCMCIA_PN             26
#define RIPL_MCLASS_USER_CONFIG_SYS_PN    27
#define RIPL_MCLASS_PRINTER_PN            28

/*
 * Parmnum limits for each machine class level
 */
#define RMC1_LAST_ELEMENT                  2
#define RMC3_LAST_ELEMENT                 28


/*
 * values for rmi2_OS2_config_flags
 */
#define OS2_SWAPPER_LOCAL          0x00000000L
#define OS2_SWAPPER_SERVER         0x00000001L
#define OS2_BUS_MCA                0x00000000L
#define OS2_BUS_ISA                0x00000002L
#define OS2_DISPLAY_IBMVGA32       0x00000000L
#define OS2_DISPLAY_IBMXGA32       0x00000010L
#define OS2_DISPLAY_IBM8514        0x00000020L
#define OS2_DISPLAY_SVGA           0x00000040L
#define OS2_DISPLAY_S3SVGA         0x00000080L
#define OS2_DISPLAY_IBMEGA         0x00000100L
#define OS2_DISPLAY_PS55_16        0x00008010L
#define OS2_DISPLAY_PS55_256       0x00008020L
#define OS2_DISPLAY_PS55_256P      0x00008040L
#define OS2_KEYBOARD_101           0x00000000L
#define OS2_KEYBOARD_DBCS_103      0x00001000L
#define OS2_KEYBOARD_DBCS_106      0x00002000L
#define OS2_KEYBOARD_DBCS_AX       0x00004000L

/*
 * values for rmi3
*/
#define RMI_MACHINE_STATUS_IGNORE   0x02      /* @F01a */
#define RMI_MACHINE_STATUS_ENABLED  0x01
#define RMI_MACHINE_STATUS_DISABLED 0x00
#define RMI_LOCAL_SWAPPER_ENABLED   0x01
#define RMI_LOCAL_SWAPPER_DISABLED  0x00
#define RMI_IP_DHCP_DISABLED        0x00
#define RMI_IP_DHCP_ENABLED         0x01
#define RMI_IP_DDNS_DISABLED        0x00
#define RMI_IP_DDNS_ENABLED         0x01
#define RMI_NETBEUI                 0x01
#define RMI_TCPBEUI                 0x02
#define RMI_8022_BOOT               0x01
#define RMI_DHCP_BOOT               0x02
#define RMI_TCPBEUI_NODE_B          'B'       /* Brodcast node       */
#define RMI_TCPBEUI_NODE_P          'P'       /* Point-to-point node */
#define RMI_TCPBEUI_NODE_H          'H'       /* Hybrid node         */


/*
 * values for rmci3
*/
#define RMC_BUS_MCA             0x00
#define RMC_BUS_ISA             0x01    /* also used for EISA and PCI */
#define RMC_DISKETTE_NO         0x00
#define RMC_DISKETTE_YES        0x01
#define RMC_SERIAL_NO           0x00
#define RMC_SERIAL_YES          0x01
#define RMC_PARALLEL_NO         0x00
#define RMC_PARALLEL_YES        0x01
#define RMC_HARDFILE_NONE       0x00
#define RMC_HARDFILE_IDE        0x01
#define RMC_HARDFILE_SCSI       0x02
#define RMC_HARDFILE_IDE_SCSI   0x03
#define RMC_KEYBOARD_TYPE_101   0x65
#define RMC_KEYBOARD_TYPE_103   0x67
#define RMC_KEYBOARD_TYPE_106   0x6A
#define RMC_KEYBOARD_TYPE_AG    0x96
#define RMC_KEYBOARD_MODE_2     0x02
#define RMC_KEYBOARD_MODE_3     0x03
#define RMC_8022_BOOT           0x01
#define RMC_DHCP_BOOT           0x02

/*
 * values for rmci3_PCMCIA_options
*/
#define RMC_PCMCIA_MODEM        0x01
#define RMC_PCMCIA_HARDFILE     0x02
#define RMC_PCMCIA_FLASH        0x04
/* remaining bits are reserved and must be set to 0 */

/*
 * values for rmci3_config_flags
*/
#define RMC_EDIT_ENABLED        0x00000001L
#define RMC_ALT_CODEPAGE        0x00000002L
/* remaining bits are reserved and must be set to 0 */

/*
 * values for rlpnai_boot_methods
*/
#define RMLP_8022_BOOT           0x01
#define RMLP_DHCP_BOOT           0x02
/* remaining bits are reserved and must be set to 0 */

/*
 * values for rlpnai_laa_support_flag
*/
#define RMLP_LAA_NOT_SUPPORTED   0x00
#define RMLP_LAA_SUPPORTED       0x01
/* remaining bits are reserved and must be set to 0 */

/*
 * value for Net32RIPLMachineDelete() Reserved parameter
 */
#define RIPLMDEL_RESERVED       0x00000001

#ifdef ANY_32_BIT
#pragma pack ()
#endif

#endif
