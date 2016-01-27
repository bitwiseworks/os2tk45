/****************************** Module Header *******************************\
*
* Module Name: PMSEI.H
*
* OS/2 Presentation Manager SetErrorInfo constants and function declaration
*
* Copyright (c) International Business Machines Corporation 1981, 1988-1992
*
* ===========================================================================
*
* This is included from PMWIN.H when appropriate INCL symbols are defined
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

#define SEI_INCLUDED

ERRORID APIENTRY WinSetErrorInfo(ERRORID,
                                 ULONG,
                                 ...);

#define SEI_BREAKPOINT   0x8000  /* Always enter an INT 3 breakpt       */
#define SEI_NOBEEP       0x4000  /* Do not call DosBeep                 */
#define SEI_NOPROMPT     0x2000  /* Do not prompt the user              */
#define SEI_DBGRSRVD     0x1000  /* Reserved for debug use              */
#define SEI_DEBUGONLY    (SEI_BREAKPOINT | SEI_NOBEEP | SEI_NOPROMPT | SEI_RESERVED)

#define SEI_STACKTRACE   0x0001  /* save the stack trace                */
#define SEI_REGISTERS    0x0002  /* save the registers                  */
#define SEI_ARGCOUNT     0x0004  /* first USHORT in args is arg count   */
#define SEI_DOSERROR     0x0008  /* first USHORT in args is OS2 error code */
#define SEI_RESERVED     0x0FE0  /* Reserved for future use             */

/****************************************************************************
* Note that when SEI_ARGCOUNT, SEI_DOSERROR are specified
* together, then the implied order of the parameters is:
*
*
*  WinSetErrorInfo( MAKEERRORID( .... ),
*                   SEI_ARGCOUNT | SEI_DOSERROR,
*                   argCount,
*                   dosErrorCode);
*
****************************************************************************/

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
