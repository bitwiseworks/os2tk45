/*****************************************************************************/
/* Copyright:                                                                */
/*   Licensed Materials - Property of IBM                                    */
/*   (C) Copyright IBM Corp. 1995                                            */
/*   All Rights Reserved                                                     */
/*****************************************************************************/
/* File: dllmain.c                                                           */
/*****************************************************************************/
/* Description:                                                              */
/*   This file contains the DLL initialization/Termination function, which   */
/*   is called when a process gains or loses access to the DLL.  The DEF     */
/*   file used to build the DLL needs to specify INITINSTANCE and            */
/*   TERMINSTANCE, otherwise this function will only be called for the       */
/*   first process to gain access and the last process to free up the DLL.   */
/*                                                                           */
/*   This implementation is for IBM C/Set++ and assumes that the 'C'         */
/*   Runtime library is being statically linked to the DLL and that the      */
/*   library uses C++ classes.                                               */
/*                                                                           */
/* Restrictions:                                                             */
/*   DLL_THREAD_ATTACH/DETACH is not supported. DisableThreadLibraryCalls()  */
/*   is also not supported.                                                  */
/*                                                                           */
/*   lpvReserved always indicates that the DLL was loaded statically even    */
/*   when the DLL was dynamically loaded.                                    */
/*                                                                           */
/*   If DllEntryPoint is used, HAVE_DLLENTRYPOINT needs to be defined        */
/*   during compilation of this file.                                        */
/*****************************************************************************/
#include <os2win.h>


int _CRT_init(void);
void _CRT_term(void);
void __ctordtorInit(void);
void __ctordtorTerm(void);
void _System WinInitializeOpen32(ULONG);
void _System WinTerminateOpen32(ULONG);

#ifdef _DEKKO_
 #pragma map (Dekko32Trace,"_DEKKO32TRACE")
 #include <dekko32.h>
#else
  #define Dekko32Trace
#endif /* _DEKKO_ */

unsigned long _System _DLL_InitTerm(unsigned long handle, unsigned long flag)
{

   unsigned long reserved;

  Dekko32Trace(0xAA00,757,0,NULL);

   if ( flag )
   {
      /* Termination: A process is losing access to this DLL */

   #ifdef HAVE_DLLENTRYPOINT
      /* Call Win32 Initialization/Termination function                 */
      /* (NOTE: this assumes the DLL entry point is a function called   */
      /*        DllEntryPoint)                                          */
      DllEntryPoint((HANDLE) handle, DLL_PROCESS_DETACH, &reserved);
   #endif

      __ctordtorTerm();                      /* Termination code for C++ Objects */
      _CRT_term();                           /* Assumes static linking of C Runtime */
      WinTerminateOpen32(0);                 /* Tell system we're done using Open32 */
      Dekko32Trace(0xAA80,757,0,NULL);
      return TRUE;
   }
   else
   {
      /* Initialization: A process is gaining access to this DLL */

      if ( _CRT_init() == -1 )               /* Initialize C Runtime */
      {
         Dekko32Trace(0xAA80,757,0,NULL);
         return 0;                           /* If initialization failed return 0 */
      }

      WinInitializeOpen32(0);                /* Tell system we're using Open32 */

      __ctordtorInit();                      /* Initialization code for C++ Objects */

      Dekko32Trace(0xAA80,757,0,NULL);
   #ifdef HAVE_DLLENTRYPOINT
      /* Call Win32 Initialization/Termination function                 */
      /* (NOTE: this assumes the DLL entry point is a function called   */
      /*        DllEntryPoint)                                          */
      return DllEntryPoint((HANDLE) handle, DLL_PROCESS_ATTACH, &reserved) != 0;
   #else
      return TRUE;
   #endif
   }
}
