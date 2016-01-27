/*static char *SCCSID = "@(#)bdcalls.h  6.6 92/03/15";*/
/***************************************************************************
*
* Module Name: BDCALLS.H
*
* OS/2 Bidi public header file.
*
* Copyright (c) 1987 - 1992  IBM Corporation
*
*****************************************************************************/

/* NOINC */
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( none )
      #endif
   #pragma info( restore )
#endif
#ifdef __cplusplus
      extern "C" {
#endif
/* INC */

#ifndef __BDCALLS__
#define __BDCALLS__

/********************  BidiHotKeys in BIDISTRUCT ****************************/
/*                                                                          */
/* The following defined value may be used to Enable/Disable the Bidi       */
/* keyboard hot keys.                                                       */
/* To enable a hot key, perform an 'OR' operation on the Bidi hot key flag  */
/* word in the KBVCBTYPE  structure with the required defined value.        */
/* To disable a hot key, perform an 'AND NOT' operation on the Bidi hot key */
/* flag word in the KBVCBTYPE  structure with the required defined value.   */
/*                                                                          */
/****************************************************************************/

#define BDHKFLAG_PUSH        (USHORT)0x0004
#define BDHKFLAG_END_PUSH    (USHORT)0x0008
#define BDHKFLAG_AUTO_PUSH   (USHORT)0x0010
#define BDHKFLAG_FIELD_REV   (USHORT)0x0020
#define BDHKFLAG_SCREEN_REV  (USHORT)0x0040
#define BDHKFLAG_BIDI_POPUP  (USHORT)0x0200
#define BDHKFLAG_AUTOMATIC   (USHORT)0x0400
#define BDHKFLAG_INITIAL     (USHORT)0x0800
#define BDHKFLAG_MIDDLE      (USHORT)0x1000
#define BDHKFLAG_FINAL       (USHORT)0x2000
#define BDHKFLAG_ISOLATED    (USHORT)0x4000

/*******************  Bidi Flags in BIDISTRUCT ******************************/
/*                                                                          */
/* The following defined value may be used to Enable/Disable the Bidi       */
/* functions.                                                               */
/* To enable a Bidi function, perform an 'OR' operation on the Bidi flags   */
/* word in the BDKBVCB  structure with the required defined value.          */
/* To disable a Bidi fuction, perform an 'AND NOT' operation on the Bidi    */
/* flags word in the BDKBVCB  structure with the required defined value.    */
/*                                                                          */
/****************************************************************************/

#define BDFLAG_AUTOPUSH_RTL  (USHORT)0x0001
#define BDFLAG_AUTOPUSH_LTR  (USHORT)0x0002
#define BDFLAG_PUSHLEVEL     (USHORT)0x0004
#define BDFLAG_LAYER         (USHORT)0x0010
#define BDFLAG_FIELD_REV     (USHORT)0x0020

/***********************  CSD in BIDIATT ************************************/
/*                                                                          */
/* The following defined value may be used to select the specified          */
/* bit in the Bidi Attributes.                                              */
/*                                                                          */
/****************************************************************************/

#define BDCSD_AUTOMATIC      0x00000000U
#define BDCSD_PASSTHRU       0x00000001U
#define BDCSD_BASE           0x00000010U
#define BDCSD_INITIAL        0x00000011U
#define BDCSD_MIDDLE         0x00000012U
#define BDCSD_FINAL          0x00000013U
#define BDCSD_ISOLATED       0x00000014U
#define BDNUM_ARABIC         0x00000000U
#define BDNUM_PASSTHRU       0x00001000U
#define BDNUM_HINDU          0x00002000U
#define BDORIENT_LTR         0x00000000U
#define BDORIENT_RTL         0x00010000U
#define BD_LEVEL             0x30000000U
#define BD_SUPPORT           0x00000000U


/********* the keyboard control block structure *********/
#pragma pack(2)
typedef struct  _bdkvcb   {
                             USHORT   BDLength;
                             USHORT   BDVersionId;
                             ULONG    BDAtts;
                             ULONG    Reserved;
                             USHORT   BDHotKeyFlags;
                             USHORT   BDFlags;
                             UCHAR    Reserved2[64];
                           } BDKVCB;


/******* the printer control block structure ************/
typedef struct  _bdprcb    {
                             USHORT   PRLength;
                             ULONG    PRAtts;
                             UCHAR    Reserved[14];
                           } BDPRCB;

/***** the Character Shape Determination structure *******/
typedef struct _csdrec
                           {
                             ULONG    RecLength;
                             PCHAR16  Buffer;
                             ULONG    BufferLength;
                             ULONG    BufferIndex;
                           } CSDRec;
#pragma pack()

typedef   CSDRec   * _Seg16  PCSDRec;
typedef   PBYTE              PRETINFO;
typedef   PBYTE              PSETINFO;



#define NlsQueryBidiAtt Nls16QueryBidiAtt
 APIRET16 APIENTRY16 NlsQueryBidiAtt ( LONG       Effect,
                                       PRETINFO   RetInfo);

#define NlsSetBidiAtt Nls16SetBidiAtt
  APIRET16 APIENTRY16 NlsSetBidiAtt ( LONG       Effect,
                                      PSETINFO   SetInfo);

#define NlsShapeBidiString Nls16ShapeBidiString
 APIRET16 APIENTRY16 NlsShapeBidiString ( ULONG      BidiAtts,
                                          ULONG      Reserved,
                                          PCHAR16    Source  ,
                                          PCHAR16    Target  ,
                                          ULONG      Length  ,
                                          ULONG      Increment);

#define NlsEditShape Nls16EditShape
 APIRET16 APIENTRY16 NlsEditShape ( ULONG      BidiAtts,
                                    LONG       Effect,
                                    PCSDRec    Source,
                                    PCSDRec    Target,
                                    PULONG     CSDState,
                                    ULONG      Increment);

#define NlsInverseString Nls16InverseString
 APIRET16 APIENTRY16 NlsInverseString( PCHAR16    Source,
                                       PCHAR16    Target,
                                       ULONG      Length,
                                       ULONG      Increment);

#define NlsConvertBidiNumerics Nls16ConvertBidiNumerics
 APIRET16 APIENTRY16 NlsConvertBidiNumerics ( ULONG    BidiAtts,
                                              LONG     Reserved,
                                              PUCHAR16 Source,
                                              PUCHAR16 Target,
                                              ULONG    Length,
                                              ULONG    Increment);

#define NlsSetBidiPrint Nls16SetBidiPrint
 APIRET16 APIENTRY16 NlsSetBidiPrint ( ULONG  BidiAtts,
                                       ULONG  Length,
                                       USHORT FileHandle);

#define NlsConvertBidiString Nls16ConvertBidiString
 APIRET16 APIENTRY16 NlsConvertBidiString ( PUCHAR16 Source,
                                            PUCHAR16 Target,
                                            ULONG    Length,
                                            ULONG    Increment,
                                            ULONG    SBidiAtts,
                                            ULONG    TBidiAtts,
                                            ULONG    Reserved);

#endif /* __BDCALLS__ */

/* NOINC */
#ifdef __cplusplus
        }
#endif
#if __IBMC__ || __IBMCPP__
   #pragma info( none )
      #ifndef __CHKHDR__
         #pragma info( restore )
      #endif
   #pragma info( restore )
#endif
/* INC */
