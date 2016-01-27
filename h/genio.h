/***************************************************************************/
/* IBM CONFIDENTIAL                                                        */
/* OCO Source Materials                                                    */
/* IBM TCP/IP for OS/2                                                     */
/* (c) Copyright IBM Corp 1992,1997                                        */
/* The source code for this program is not published or otherwise          */
/* divested of its trade secrets, irrespective of what has been            */
/* deposited with the U.S. Copyright Office                                */
/***************************************************************************/
/* [db3l] Changes for flexible support for serial interfaces */
#ifndef GENIO_H
#define GENIO_H
#define  CALL_SOCKET          0x40
#define  CALL_BIND            0x41
#define  CALL_LISTEN          0x42
#define  CALL_ACCEPT          0x43
#define  CALL_CONNECT         0x44
#define  CALL_SENDTO          0x45
#define  CALL_SEND            0x46
#define  CALL_SENDMSG         0x47
#define  CALL_RECVFROM        0x48
#define  CALL_RECV            0x49
/* start 40518 send_file */
#define  CALL_SEND_FILE       0x4C
/* end 40518 send_file */

/* for AF_NB (netbios) support */
#define  CALL_NB_HOST         0x4f

#define  CALL_RECVMSG         0x50
#define  CALL_SHUTDOWN        0x51
#define  CALL_SETSOCKOPT      0x52
#define  CALL_GETSOCKOPT      0x53
#define  CALL_GETSOCKNAME     0x54
#define  CALL_GETPEERNAME     0x55
#define  CALL_CLOSE           0x56
#define  CALL_IOCTL           0x57
#define  CALL_SELECT          0x58
#define  CALL_GETHOSTID       0x59
#define  CALL_START_INTHREAD  0x5A
#define  CALL_SYSCTL          0x5B /* this is actually a callgate */
#define  CALL_WINSELECT       0x5C /* winsock style select */

/* #ifdef ACC_EX */
#define CALL_ACCEPTEX         0x5E /* AcceptEx() call */
/* #endif */

#ifdef SNACKETS
#define  CALL_SNACKETS        0x5f
#endif
#define  CALL_DEBUG           0x60
#define  CALL_GETVERSION      0x61

#define  CALL_INIT            0x62
#define  CALL_UIPCTIM1        0x63
#define  CALL_UIPCTIM2        0x64
#define  CALL_IFTIMER         0x65
#define  CALL_ARPTIMER        0x66
#define  CALL_LOINPUT         0x67
#define  CALL_GETVARS         0x68
#define  CALL_ENTRYS          0x69
#define  CALL_OLD_SLATTACH    0x6a      /* [db3l] No longer used, but not */
#define  CALL_OLD_SLIPINPUT   0x6b      /* reused to avoid conflicts with */
#define  CALL_OLD_SLIPOUTPUT  0x6c      /* older code.                    */
#define  CALL_FORWARD         0x6d
#define  CALL_CANCEL          0x6e
#define  CALL_PUSH            0x6f
#define  CALL_CONNEXISTS      0x70
#define  CALL_ABORT           0x71
#define  CALL_ASYNCHEVENT     0x72

#ifdef X25
#define  CALL_X25ATTACH       0x73
#define  CALL_X25INPUT        0x74
#define  CALL_X25OUTPUT       0x75
#endif

#define  CALL_READV           0x78
#define  CALL_WRITEV          0x79

#define  CALL_20              0x7A
#define  CALL_ISFORWARD       0x7b
#define  CALL_WASCANCEL       0x7c
#define  CALL_ENQEVENT        0x7d
#define  CALL_STARTTRACE      0x7E
#define  CALL_GETTRACE        0x7F
#define  CALL_STOPTRACE       0x51

/*012594JRM add CALL_SETINET*/
#define  CALL_SETINET         0x4a
#define  CALL_GETINET         0x4b

/* [db3l] Serial Interface IOCtls - Provides interface for COM driver code */
#define  CALL_SLATTACH        0xC0      /* Attach a serial interface    */
#define  CALL_SLDETACH        0xC1      /* Detach a serial interface    */
#define  CALL_SLGETPARMS      0xC2      /* Obtain interface parameters  */
#define  CALL_SLSETPARMS      0xC3      /* Change interface parameters  */
#define  CALL_SLINPUT         0xC4      /* Input from serial interface  */
#define  CALL_SLOUTPUT        0xC5      /* Output to serial interface   */
#define  CALL_SLOUTPUTCANCEL  0xC6      /* Cancel blocked SLOUTPUT      */

#ifdef SNALINK
#define  CALL_SNAL_ATTACH     0xC7
#define  CALL_SNAL_INPUT      0xC8
#define  CALL_SNAL_OUTPUT     0xC9
#endif
#define  CALL_RECV2           0xCA
#define  CALL_RECV3           0xCB

#define  CALL_GETDBGBUF       0xf0      /* gets debug flags for tracing */
#define  CALL_SETDBGFLAG      0xf1      /* sets debug flags for tracing */
#define  CALL_SELTOFLAT       0xf2      /* convert 16:16 to linear addr */
#define  CALL_FREEDBGMEM      0xf3      /* free acquired debug mem */
#define  CALL_VERIFY          0xf4      /* verify pointers */

#define DeviceIO 11
#define ICFG_INETVER          1


/*------------------------------------------------------------*/

/* [db3l] Structure parameters for new IOCtls */

/* Structures used in various IOCtls */

/* INET Pointers */

typedef char * inetptr_t;
#define _INETPTR_T_

/* Structures for Serial Interface IOCtls */

#define IFATTACH_VERSION  1
#define IFATTACH_MAXNAME 12      /* Maximum name size (including ending \0) */
#define IFATTACH_MAXDESCR 45     /* Matches ifmib.ifDescr[] size in if.h    */

/* Flags for high portion of if_flags - low portion is standard IFF_* flags */
#define IFF_PRIORITYQUEUE 0x00010000L   /* Priority interactive queuing     */
#define IFF_NOICMP        0x00020000L   /* Don't allow ICMP traffic         */
#define IFF_NOINTFLAGS    0x80000000L   /* Non-zero value with no meaning   */

#ifndef __32BIT__
#pragma pack(1)
#endif
/* This is the old ifattach struct with u_short for if_speed */
struct ifAttachOld {                    /* For ATTACH/DETACH,GET/SETPARMS   */
   unsigned short version;              /* Structure version                */
   unsigned short length;               /* Length of supplied structure     */
   char intf_name[IFATTACH_MAXNAME];    /* Interface name                   */
   unsigned short intf_unit;            /* Unit number (0-3)                */
   unsigned short intf_mtu;             /* MTU for interface                */
   unsigned short intf_speed;           /* Estimated speed (for ifmib)      */
   unsigned long  intf_flags;           /* Initial interface flags          */
   unsigned short intf_rtt;             /* Estimated rtt, rttvar and rttmin */
   unsigned short intf_rttvar;          /*   for routes making use of this  */
   unsigned short intf_rttmin;          /*   interface (in ms)              */
   unsigned short intf_sendpipe;        /* Maximum send/recv pipes (socket  */
   unsigned short intf_recvpipe;        /*   buffers and TCP windows)       */
   unsigned short intf_ssthresh;        /* Slow-start threshold (segments)  */
   unsigned short intf_maxqueue;        /* Maximum interface queue size     */
   unsigned short intf_maxfastq;        /* Maximum fast queue size          */
   inetptr_t      intf_eflags;          /* pointer to ifp->if_eflags (u_long) */
   char           intf_descr[IFATTACH_MAXDESCR];
                                        /* MIB II ifDescr for this intf     */
   unsigned short intf_type;            /* MIB II ifType for this intf      */
};
#ifndef __32BIT__
#pragma pack()

#pragma pack(1)
#endif
struct ifattach {                       /* For ATTACH/DETACH,GET/SETPARMS   */
   unsigned short version;              /* Structure version                */
   unsigned short length;               /* Length of supplied structure     */
   char intf_name[IFATTACH_MAXNAME];    /* Interface name                   */
   unsigned short intf_unit;            /* Unit number (0-3)                */
   unsigned short intf_mtu;             /* MTU for interface                */
   unsigned short intf_speed;           /* Estimated speed (for ifmib)      */
   unsigned long  intf_flags;           /* Initial interface flags          */
   unsigned short intf_rtt;             /* Estimated rtt, rttvar and rttmin */
   unsigned short intf_rttvar;          /*   for routes making use of this  */
   unsigned short intf_rttmin;          /*   interface (in ms)              */
   unsigned short intf_sendpipe;        /* Maximum send/recv pipes (socket  */
   unsigned short intf_recvpipe;        /*   buffers and TCP windows)       */
   unsigned short intf_ssthresh;        /* Slow-start threshold (segments)  */
   unsigned short intf_maxqueue;        /* Maximum interface queue size     */
   unsigned short intf_maxfastq;        /* Maximum fast queue size          */
   inetptr_t      intf_eflags;          /* pointer to ifp->if_eflags (u_long) */
   char           intf_descr[IFATTACH_MAXDESCR];
                                        /* MIB II ifDescr for this intf     */
   unsigned short intf_type;            /* MIB II ifType for this intf      */
   /* sk jan 24 1995 */
   unsigned long  intf_lspeed;          /* if_speed declared up is short.   */
                                        /* cannot change it for back-compat */
};
#ifndef __32BIT__
#pragma pack()
#endif

#define IFATTACH_MINLEN ((u_long)&(((struct ifattach *)0)->intf_eflags) & 0xffff)
#define VALID_IFATTACH_MEMBER(_i, _m) ((((u_long)&((_i)->_m) - (u_long)(_i)) + \
                                        sizeof((_i)->_m)) <= (_i)->length)

/* if (if_unit==IFATTACH_UNIT) then if_sl.c will try to allocate if_unit*/
#define IFATTACH_UNIT                  (unsigned short)-1

#define PKTF_PRIORITY   1               /* Priority queued traffic          */
#ifndef __32BIT__
#pragma pack(1)
#endif
struct slpacket {                       /* For INPUT/OUTPUT                 */
   unsigned short version;              /* Structure version                */
   unsigned short length;               /* Length of supplied structure     */
   unsigned short if_unit;              /* Interface unit number            */
   unsigned short pkt_flags;            /* Packet flags                     */
   unsigned short datalength;           /* Data length                      */
   inetptr_t      data;                 /* Packet data                      */
   unsigned long  ip_dest;              /* IP destination                   */
};
#ifndef __32BIT__
#pragma pack()
#endif

#define SLPACKET_VERSION 1
#define SLPACKET_MINLEN ((u_long)&(((struct slpacket *)0)->ip_dest) & 0xffff)
#define VALID_SLPACKET_MEMBER(_i, _m) ((((u_long)&((_i)->_m) - (u_long)(_i)) + \
                                        sizeof((_i)->_m)) <= (_i)->length)

#define MAXTRACEBYTES 31000
#endif /* GENIO_H */
