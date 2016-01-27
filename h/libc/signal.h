#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __signal_h
   #define __signal_h

   #ifdef __cplusplus
      extern "C" {
   #endif

   #ifndef  _LNK_CONV
      #if __IBMC__ || __IBMCPP__ || defined(_OPTLINK_SUPPORTED)
         #define _LNK_CONV   _Optlink
      #else
         #define _LNK_CONV
      #endif
   #endif

   #ifndef _IMPORT
      #ifdef __IMPORTLIB__
         #define _IMPORT _Import
      #else
         #define _IMPORT
      #endif
   #endif

   /********************************************************************/
   /*  <signal.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #ifndef __sig_atomic_t
      typedef int sig_atomic_t;
      #define __sig_atomic_t
   #endif

   /* signal types */
   #define SIGILL       1       /* illegal instruction - invalid function image    */
   #define SIGSEGV      2       /* invalid access to memory                        */
   #define SIGFPE       3       /* floating point exception                        */
   #define SIGTERM      4       /* OS/2 SIGTERM (killprocess) signal               */
   #define SIGABRT      5       /* abort() signal                                  */
   #define SIGINT       6       /* OS/2 SIGINTR signal                             */
   #define SIGUSR1      7       /* user exception in range 0xa0000000 - 0xbfffffff */
   #define SIGUSR2      8       /* user exception in range 0xc0000000 - 0xdfffffff */
   #define SIGUSR3      9       /* user exception in range 0xe0000000 - 0xffffffff */
   #define SIGBREAK    10       /* OS/2 Ctrl-Break sequence                        */

   typedef void (*_SigFunc)(int);

   /* signal action codes */
   #define SIG_DFL ((_SigFunc)0)   /* default signal action */
   #define SIG_IGN ((_SigFunc)1)   /* ignore */

   /* signal error value (returned by signal call on error) */
   #define SIG_ERR ((_SigFunc)-1)  /* signal error value */

   /* function prototypes */

   extern _SigFunc _IMPORT _LNK_CONV signal( int, _SigFunc );
   extern int _IMPORT _LNK_CONV raise( int );

   #ifdef __cplusplus
      }
   #endif

#endif

#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( restore )
#endif
#pragma info( restore )
#endif

