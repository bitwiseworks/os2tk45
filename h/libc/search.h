#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __search_h
   #define __search_h

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
   /*  <search.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if defined(__EXTENDED__)

      #ifndef __size_t
        #define __size_t
        typedef unsigned int size_t;
      #endif

      /* function prototypes */

      char * _IMPORT _LNK_CONV _lsearch( const char *, const char *, unsigned int *,
                                unsigned int, int ( * _LNK_CONV __compare )( const void *, const void * ) );
      char * _IMPORT _LNK_CONV _lfind( const char *, const char *, unsigned int *,
                              unsigned int, int ( * _LNK_CONV __compare )( const void *, const void * ) );

      char * _IMPORT _LNK_CONV lsearch( const char *, const char *, unsigned int *,
                               unsigned int, int ( * _LNK_CONV __compare )( const void *, const void * ) );
      char * _IMPORT _LNK_CONV lfind( const char *, const char *, unsigned int *,
                             unsigned int, int ( * _LNK_CONV __compare )( const void *, const void * ) );

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( lsearch, "?_lsearch" )
            #pragma map( lfind  , "?_lfind"   )
         #else
            #pragma map( lsearch, "_lsearch" )
            #pragma map( lfind  , "_lfind"   )
         #endif
      #else
         #define lsearch _lsearch
         #define lfind _lfind
       #endif

      void * _IMPORT _LNK_CONV bsearch( const void *, const void *, size_t, size_t,
                               int ( * _LNK_CONV __compare )( const void *, const void * ) );
      void   _IMPORT _LNK_CONV qsort( void *, size_t, size_t,
                             int ( * _LNK_CONV __compare )( const void *, const void * ) );

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

