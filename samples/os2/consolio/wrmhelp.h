
/*=========================================================================\
 *                                                                         *
 *       FILE:wrmhelp.h                                                    *
 *                                                                         *
 *       DESCRIPTION:wrmhelp.h                                             *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Created 1991  IBM Corp.
 *                                                                         *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *      sample code created by IBM Corporation and/or Microsoft            *
 *      Corporation. This sample code is not part of any standard          *
 *      Microsoft or IBM product and is provided to you solely for         *
 *      the purpose of assisting you in the development of your            *
 *      applications.  The code is provided "AS IS", without               *
 *      warranty of any kind.  Neither IBM or Microsoft  shall be          *
 *      liable for any damages arising out of your use of the sample       *
 *      code, even if they have been advised of the possibility of         *
 *      such damages.                                                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *--------------------------------------------------------------
 *
 *  This header file  is for the source  module:
 *
 *
 *
 *
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Global macros, defined constants, and externs
\*--------------------------------------------------------------*/






/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/










/*--------------------------------------------------------------*\
 *  Global Entry point declarations
\*--------------------------------------------------------------*/
extern BOOL SusPendAllThreads(VOID);
extern BOOL DisplayHelp(VOID );
extern BOOL SuspendAllThreads(VOID);
extern BOOL SaveScreen( PVOID *ppvWindBuf );
extern VOID ReDisplayScreen( PVOID  pvWindBuf);
extern BOOL ResumeAllThreads(VOID );
extern BOOL WormHelp(VOID );
/*--------------------------------------------------------------*\
 *  End of file :wrmhelp.h
\*--------------------------------------------------------------*/
