/****************************** Module Header ******************************\
*
* Module Name: PMPIC.H
*
* OS/2 Presentation Manager Picture functions include file.
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

#define PIC_INCLUDED




/*** type of picture to print **********************************************/
#define PIP_MF       1L
#define PIP_PIF      2L

/*** type of conversion required *******************************************/
#define PIC_PIFTOMET 0L
#define PIC_SSTOFONT 2L

#ifdef __cplusplus
BOOL APIENTRY PicPrint(HAB hab,
                       PCSZ  pszFilename,
                       LONG lType,
                       PCSZ  pszParams);
#else
BOOL APIENTRY PicPrint(HAB hab,
                       PSZ pszFilename,
                       LONG lType,
                       PSZ pszParams);
#endif

#ifdef __cplusplus
BOOL APIENTRY PicIchg(HAB hab,
                      PCSZ  pszFilename1,
                      PCSZ  pszFilename2,
                      LONG lType);
#else
BOOL APIENTRY PicIchg(HAB hab,
                      PSZ pszFilename1,
                      PSZ pszFilename2,
                      LONG lType);
#endif


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
