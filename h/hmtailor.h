/****************************** Module Header ******************************\
*
* Module Name: HMTAILOR.H
*
* OS/2 Help window menu identifiers
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

#define IDM_CHILDVPSYSMENU   810

#define IDM_FILE             0x7F00
#define IDM_SEARCH           0x7F01
#define IDM_PRINT            0x7F02
#define IDM_VIEWPORT         0x7F05

#define IDM_EDIT             0x7F10
#define IDM_CLIP_COPY        0x7F11
#define IDM_CLIP_CF          0x7F12
#define IDM_CLIP_AF          0x7F13
#define IDM_LIBRARIES        0x7F14

#define IDM_OPTIONS          0x7F20
#define IDM_VIEWPAGES        0x7F21
#define IDM_TOC              0x7F23
#define IDM_OPTION_EXPLEVEL  0x7F24
#define IDM_OPTION_EXPBRANCH 0x7F25
#define IDM_OPTION_EXPALL    0x7F26
#define IDM_OPTION_COLBRANCH 0x7F27
#define IDM_OPTION_COLALL    0x7F28
#define IDM_PREVIOUS         0x7F29

#define IDM_HELP             0x7F30
#define IDM_HELPHELP         0x7F31
#define IDM_HELP_EXTENDED    0x7F32
#define IDM_USING_HELP       IDM_HELPHELP
#define IDM_GENERAL_HELP     IDM_HELP_EXTENDED
#define IDM_HELP_KEYS        0x7F33
#define IDM_HELP_INDEX       0x7F34

#define IDM_TUTORIAL         0x7F37

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
