/****************************** Module Header ******************************\
*
* Module Name: PMAVIO.H
*
* OS/2 Presentation Manager AVIO constants, types and function declarations
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* Comments at the end of each typedef line give the name tags used in
* the assembler include version of this file.
*
* The assembler include version of this file excludes lines between XLATOFF
* and XLATON comments.
*
\***************************************************************************/

/* XLATOFF */
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
/* XLATON */

#define AVIO_INCLUDED


/* XLATOFF */

#define Vio16Associate         VIO16ASSOCIATE
#define Vio16CreateLogFont     VIO16CREATELOGFONT
#define Vio16CreatePS          VIO16CREATEPS
#define Vio16DeleteSetId       VIO16DELETESETID
#define Vio16DestroyPS         VIO16DESTROYPS
#define Vio16GetDeviceCellSize VIO16GETDEVICECELLSIZE
#define Vio16GetOrg            VIO16GETORG
#define Vio16QueryFonts        VIO16QUERYFONTS
#define Vio16QuerySetIds       VIO16QUERYSETIDS
#define Vio16SetDeviceCellSize VIO16SETDEVICECELLSIZE
#define Vio16SetOrg            VIO16SETORG
#define Vio16ShowPS            VIO16SHOWPS
#define Win16DefAVioWindowProc WIN16DEFAVIOWINDOWPROC

#define VioAssociate           VIO16ASSOCIATE
#define VioCreateLogFont       VIO16CREATELOGFONT
#define VioCreatePS            VIO16CREATEPS
#define VioDeleteSetId         VIO16DELETESETID
#define VioDestroyPS           VIO16DESTROYPS
#define VioGetDeviceCellSize   VIO16GETDEVICECELLSIZE
#define VioGetOrg              VIO16GETORG
#define VioQueryFonts          VIO16QUERYFONTS
#define VioQuerySetIds         VIO16QUERYSETIDS
#define VioSetDeviceCellSize   VIO16SETDEVICECELLSIZE
#define VioSetOrg              VIO16SETORG
#define VioShowPS              VIO16SHOWPS
#define WinDefAVioWindowProc   WIN16DEFAVIOWINDOWPROC
/* XLATON */

typedef USHORT HVPS;        /* hpvs */
typedef HVPS *PHVPS;    /* phpvs */

USHORT  APIENTRY16 VioAssociate(HDC hdc,
                                HVPS hvps);

USHORT  APIENTRY16 VioCreateLogFont(PFATTRS pfatattrs,
                                    LONG llcid,
                                    PSTR8 pName,
                                    HVPS hvps);

USHORT  APIENTRY16 VioCreatePS(PHVPS phvps,
                               SHORT sdepth,
                               SHORT swidth,
                               SHORT sFormat,
                               SHORT sAttrs,
                               HVPS hvpsReserved);

USHORT  APIENTRY16 VioDeleteSetId(LONG llcid,
                                  HVPS hvps);

USHORT  APIENTRY16 VioDestroyPS(HVPS hvps);

USHORT  APIENTRY16 VioGetDeviceCellSize(PSHORT psHeight,
                                        PSHORT psWidth,
                                        HVPS hvps);

USHORT  APIENTRY16 VioGetOrg(PSHORT psRow,
                             PSHORT psColumn,
                             HVPS hvps);

USHORT  APIENTRY16 VioQueryFonts(PLONG plRemfonts,
                                 PFONTMETRICS afmMetrics,
                                 LONG lMetricsLength,
                                 PLONG plFonts,
                                 PSZ pszFacename,
                                 ULONG flOptions,
                                 HVPS hvps);

USHORT  APIENTRY16 VioQuerySetIds(PLONG allcids,
                                  PSTR8 pNames,
                                  PLONG alTypes,
                                  LONG lcount,
                                  HVPS hvps);

USHORT  APIENTRY16 VioSetDeviceCellSize(SHORT sHeight,
                                        SHORT sWidth,
                                        HVPS hvps);

USHORT  APIENTRY16 VioSetOrg(SHORT sRow,
                             SHORT sColumn,
                             HVPS hvps);

USHORT  APIENTRY16 VioShowPS(SHORT sDepth,
                             SHORT sWidth,
                             SHORT soffCell,
                             HVPS hvps);

/************************ Public Function *******************************
* WinDefAVioWindowProc -- Default message processing for AVio PS's
************************************************************************/
/* mp1 and mp2 are defined as ULONG because MPARAM is a pointer type and */
/* 32-bit compilers will thunk pointer types passed to 16-bit routines.  */
/* This is not what is desired for WM_SIZE processing so we will declare */
/* the function with ULONGs, non-pointer types.                          */

MRESULT APIENTRY16 WinDefAVioWindowProc(HWND hwnd,
                                        USHORT msg,
                                        ULONG mp1,
                                        ULONG mp2);

/* XLATOFF */
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
/* XLATON */

/**************************** end of file **********************************/
