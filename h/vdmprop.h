/***    vdmprop.h - Common VDM properties definitions
 *
 *
 *
 *      Copyright (c) IBM Corporation 1987, 1992
 *
 *      All Rights Reserved
 *
 *      Title:
 *          VDM Property definitions common to VDDs and applications
 *
 *
 */

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
/* INC */

#ifndef __VDMPROP__
#define __VDMPROP__


/***ET+ Property Limits
 *
 */

#define MAX_PROPERTY_NAME_LENGTH    40  /* including NULL terminator */
#define MAX_AUTOEXEC_SIZE          128  // max string size              //53233


/***ET+ VPTYPE: VDM Property Types
 *
 */
#define VDMP_BOOL       0   /* boolean */
#define VDMP_INT        1   /* integer - ULONG size, but only USHORT is valid */
#define VDMP_ENUM       2   /* enumeration */
#define VDMP_STRING     3   /* asciiz string */
#define VDMP_MLSTR      4   /* multi-line string, separated by linefeed (0x0a) */

typedef USHORT   VPTYPE;                                /* property type */
/*end*/


/***ET+ VPORD: VDM Property Ordinals
 *
 */
#define VDMP_ORD_OTHER      0   /* custom VDD property */
#define VDMP_ORD_KERNEL     1   /* asciiz path of DOS kernel */
#define VDMP_ORD_SHELL      2   /* asciiz path of DOS shell */
#define VDMP_ORD_RMSIZE     3   /* integer size of DOS box (128K..640K) */
#define VDMP_ORD_FCB        4   /* integer total FCBs */
#define VDMP_ORD_FCB2       5   /* integer FCBs immune to close LRUing */
#define VDMP_ORD_BREAK      6   /* boolean BREAK flag */
#define VDMP_ORD_DOSDD      7   /* mlstr DOS device drivers */
#define VDMP_ORD_VMBOOT     8   /* mlstr virtual machine boot drive(s) */
/* #define VDMP_ORD_IOPL3      9 */  /* boolean IOPL3 flag               6.10 */
#define VDMP_ORD_VERSION    10  /* mlstr fake version entries */
#define VDMP_ORD_DOS_UMB    11  /* boolean flag - DOS owns UBMs */
#define VDMP_ORD_DOS_HIGH   12  /* boolean flag - DOS loaded high */
#define VDMP_ORD_LASTDRIVE  13  /* asciiz lastdrive */
#define VDMP_ORD_FILES      14  /* integer total FILES */
#define VDMP_ORD_AUTOEXEC   15  // asciiz Autoexec filespec
#define VDMP_ORD_INSTALL    16  // mlstr INSTALL= settings


typedef USHORT   VPORD;                                 /* property ordinal */
/*end*/


/***ET+ VDHRegisterProperty Flags
 *
 */
#define VDMP_CREATE         0x00000001  /* create-time only property */
#define VDMP_ADVANCED       0x00000002  /* advanced property (not shown by default) */
/*end*/


/***ET+ VDHRegisterProperty PFNVDHRP ordinals
 *
 */
#define VDHPROP_VALIDATE    0x00000000L /* validate parameter */
#define VDHPROP_SET         0x00000001L /* set parameter */
/*end*/


/***ET+ VPBOUND - limits for VDMP_INT properties
 *
 *  Notes:  (1) max > min must hold
 *          (2) (max-min) must be a multiple of step
 *          (3) step = 1 implies that all values between min and max are valid
 */
typedef struct VPBOUND_s { /* vpb */
    USHORT  min;    /* minimum allowed value */
    USHORT  max;    /* maximum allowed value */
    USHORT  step;   /* increment between values */
} VPBOUND;
typedef VPBOUND *PVPBOUND; /* pvpb */
/*end*/


/***ET+ VPINFO - VDD Property Info structure
 *
 *      This is filled in by a call to Dos32QueryDOSProperty
 *
 *
 *      VPTYPE  vi_vptype  - Property type (see VPTYPE).
 *      VPORD   vi_vpord   - Property ordinal (see VPORD).
 *      ULONG   vi_ulFlags - Property flags:
 *                  VDMP_CREATE
 *                    TRUE if the property can only be specified at
 *                        VDM creation.  Changing the property after
 *                        the VDM is created has no effect.
 *                    FALSE if the property can be specified at VDM
 *                        creation AND can be changed for a running
 *                        VDM.
 *      ULONG   vi_ulHelpID - ID of help topic
 *      char    vi_pszHelp[] - asciiz help file name.
 *                  A single NULL byte is present if no help is
 *                  available.
 *
 *      ???     vi_pvValid - This contains information that allows the property
 *                  value to be validated.  Its format depends upon
 *                  the property type:
 *                    VDMP_BOOL
 *                      This field is not present.
 *                    VDMP_INT
 *                      This field is a VPBOUND structure.
 *                    VDMP_ENUM
 *                      This field is a set of ASCIIZ strings,
 *                      terminated by a zero byte, which is the allowed
 *                      set of responses.
 *                    VDMP_STRING
 *                    VDMP_MLSTR
 *                      This field is a ULONG representing the maximum
 *                      allowed string length, including the terminating
 *                      NULL byte.
 *
 *      ???     vi_pvValue - The default value of the property.  In all cases
 *                  it is assumed that this value is valid according
 *                  to the constraints specified by pvValid.
 *                  The format depends upon the property type:
 *                    VDMP_BOOL
 *                      This field is a 4-byte BOOL.
 *                    VDMP_INT
 *                      This field is a ULONG.  The high half of the
 *                      value is always zero, so this type can only
 *                      take on the values of a USHORT.
 *                    VDMP_ENUM
 *                      This field is an ASCIIZ string.
 *                    VDMP_STRING
 *                    VDMP_MLSTR
 *                      This field is an ASCIIZ string.
 */

typedef struct VPINFO_s { /* vpinfo */
    VPTYPE  vi_vptype;     /* property type */
    VPORD   vi_vpord;      /* property ordinal */
    ULONG   vi_ulFlags;    /* flags */
    ULONG   vi_ulHelpID;   /* help topic id */
    /*      vi_pszHelp[];*/  /* help file */
    /*      vi_pvValid; */   /* validation information */
    /*      vi_pvValue; */    /* default value */
} VPINFO;
typedef VPINFO *PVPINFO;
/*end*/


/***ET+ PROPERTYBUFFER - buffer containing 0 or more property values
 *
 *      This buffer format is used when passing property values to
 *      DosStartSession.
 *
 *      Byte
 *      Offset  Type    Contents
 *      ------  ------- ------------------------------------------
 *         0    ULONG   Buffer length, including this ULONG.
 *         4    VPTYPE  Type of property (see VPTYPE in vdmprop.h)
 *         6    ASCIIZ  Null-terminated property name
 *         ?    ?       Property value.  Format depends on VPTYPE:
 *                        VDMP_BOOL
 *                          4-byte boolean value.  0 => false, !0 => true.
 *                        VDMP_INT
 *                          ULONG.  The high half of the value is always
 *                            zero, so this type can only take on the
 *                            values of a USHORT.
 *                        VDMP_ENUM
 *                        VDMP_STRING
 *                        VDMP_MLSTR
 *                          Null-terminated string.
 *         ?    VPTYPE  Type of second property
 *         ?+2  ASCIIZ  Name of second property
 *         ?    ?       Value of second property
 *         ...
 */

typedef struct _PROPERTYBUFFER { /* propbuf */
    ULONG   cb;
    CHAR    achPropBuf;
} PROPERTYBUFFER;
typedef PROPERTYBUFFER *PPROPERTYBUFFER; /* ppropbuf */
/*end*/

#endif /* __VDMPROP__ */


/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
