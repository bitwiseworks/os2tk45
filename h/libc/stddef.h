#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __stddef_h
   #define __stddef_h

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
   /*  <stddef.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   typedef int ptrdiff_t;

   #ifndef __size_t
      #define __size_t
      typedef unsigned int size_t;
   #endif

   #ifndef __wchar_t
      #define __wchar_t
      typedef unsigned short wchar_t;
   #endif

   #ifndef NULL
      #if (defined(__EXTENDED__)  || defined( __cplusplus ))
         #define NULL 0
      #else
         #define NULL ((void *)0)
      #endif
   #endif

   #if __IBMC__ || __IBMCPP__
      #define offsetof( x, y ) __offsetof( x, y )
   #else
      #define offsetof(s_name, s_member) (size_t)&(((s_name *)0)->s_member)
   #endif

   #if defined(__EXTENDED__)

      #ifdef __THW_PPC__
         extern size_t _threadalloc(size_t len);
         extern void * _threadlocal(size_t var);
      #endif

      #ifndef errno
         #if defined __MULTI__ || defined _WIN32S
            extern int * _IMPORT _LNK_CONV _errno( void );
            #define errno (*_errno( ))
         #else
            extern int _IMPORT errno;
            #define errno errno
         #endif
      #endif

      #ifdef __MULTI__
         extern unsigned * _IMPORT _LNK_CONV __threadid( void );
         #define _threadid (__threadid( ))
      #endif

   #endif

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