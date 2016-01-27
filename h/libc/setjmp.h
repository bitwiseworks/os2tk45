#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __setjmp_h
   #define __setjmp_h

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
   /*  <setjmp.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if __THW_PPC__
      typedef double jmp_buf[128];
      extern int _IMPORT setjmp( jmp_buf );
      #if __IBMC__ || __IBMCPP__
         #pragma reachable(setjmp)
      #endif
   #else /* __THW_INTEL__ */
      typedef int jmp_buf[8];
      #if __IBMC__ || __IBMCPP__
         extern int _Builtin _IMPORT setjmp( jmp_buf );
      #else
         extern int _IMPORT _LNK_CONV setjmp( jmp_buf );
      #endif
   #endif

   #if (__IBMC__ || __IBMCPP__) && defined(__THW_PPC__)
      #pragma leaves(longjmp)
   #endif

   extern void _IMPORT _LNK_CONV longjmp( jmp_buf, int );

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

