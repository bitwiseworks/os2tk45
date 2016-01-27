#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __memory_h
   #define __memory_h

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
   /*  <memory.h> header file                                          */
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

      #if __THW_INTEL__ && (__IBMC__ || __IBMCPP__)
         void * _Builtin __memcpy( void *, const void *, size_t );
         void * _Builtin __memchr( const void *, int, size_t );
         int    _Builtin __memcmp( const void *, const void *, size_t );
         void * _Builtin __memset( void *, int, size_t );
         void * _Builtin __memmove( void *, const void *, size_t );

         #ifdef __cplusplus
            inline void * _LNK_CONV memcpy( void * m1, const void * m2, size_t len )
                                  { return __memcpy( m1, m2, len ); }
            inline void * _LNK_CONV memchr( const void * m, int ch, size_t len )
                                  { return __memchr( m, ch, len ); }
            inline int    _LNK_CONV memcmp( const void * m1, const void * m2, size_t len )
                                  { return __memcmp( m1, m2, len ); }
            inline void * _LNK_CONV memset( void * m, int ch, size_t len )
                                  { return __memset( m, ch, len ); }
            inline void * _LNK_CONV memmove( void * m1, const void * m2, size_t len )
                                  { return __memmove( m1, m2, len ); }
         #else
            void * _IMPORT _LNK_CONV memcpy( void *, const void *, size_t );
            void * _IMPORT _LNK_CONV memchr( const void *, int, size_t );
            int    _IMPORT _LNK_CONV memcmp( const void *, const void *, size_t );
            void * _IMPORT _LNK_CONV memset( void *, int, size_t );
            void * _IMPORT _LNK_CONV memmove( void *, const void *, size_t );

            #define memcpy( x, y, z )  __memcpy( (x), (y), (z) )
            #define memchr( x, y, z )  __memchr( (x), (y), (z) )
            #define memcmp( x, y, z )  __memcmp( (x), (y), (z) )
            #define memset( x, y, z )  __memset( (x), (y), (z) )
            #define memmove( x, y, z ) __memmove( (x), (y), (z) )
         #endif
      #elif __THW_PPC__ && (__IBMC__ || __IBMCPP__)
         #ifdef __cplusplus
            extern "builtin" void  *__memchr(const void *s, int c, size_t n);
            extern "builtin" void  *__memcpy(void *s1, const void *s2, size_t n);
            extern "builtin" void  *__memmove(void *s1, const void *s2, size_t n);
            extern "builtin" int __memcmp(const void *s1, const void *s2, size_t n);
            extern "builtin" void  *__memset(void *s, int c, size_t n);
         #else
            void  * _Builtin __memchr(const void *s, int c, size_t n);
            void  * _Builtin __memcpy(void *s1, const void *s2, size_t n);
            void *  _Builtin __memmove(void *s1, const void *s2, size_t n);
            int     _Builtin __memcmp(const void *s1, const void *s2, size_t n);
            void *  _Builtin __memset(void *s, int c, size_t n);
         #endif
         #if defined(__STR__)
            #ifdef __cplusplus
               inline void  *memchr(const void *s, int c, size_t n) { return __memchr (s, c, n); }
               inline void  *memcpy(void *s1, const void *s2, size_t n) { return __memcpy (s1, s2, n); }
               inline void  *memmove(void *s1, const void *s2, size_t n) { return __memmove (s1, s2, n); }
               inline int    memcmp(const void *s1, const void *s2, size_t n) { return __memcmp (s1, s2, n); }
               inline void  *memset(void *s, int c, size_t n) { return __memset(s, c, n); }
            #else
               #define memchr(__s1,__c,__n) __memchr(__s1,__c,__n)
               #define memcpy(__s1,__s2,__n) __memcpy(__s1,__s2,__n)
               #define memmove(__s1,__s2,__n) __memmove(__s1,__s2,__n)
               #define memcmp(__s1,__s2,__n) __memcmp(__s1,__s2,__n)
               #define memset(__s1,__c,__n) __memset(__s1,__c,__n)
            #endif
         #else
            void * memcpy( void *, const void *, size_t );
            void * memchr( const void *, int, size_t );
            int    memcmp( const void *, const void *, size_t );
            void * memset( void *, int, size_t );
            void * memmove( void *, const void *, size_t );
         #endif

      #else
         void * _IMPORT _LNK_CONV memcpy( void *, const void *, size_t );
         void * _IMPORT _LNK_CONV memchr( const void *, int, size_t );
         int    _IMPORT _LNK_CONV memcmp( const void *, const void *, size_t );
         void * _IMPORT _LNK_CONV memset( void *, int, size_t );
         void * _IMPORT _LNK_CONV memmove( void *, const void *, size_t );
      #endif

      void * _IMPORT _LNK_CONV memccpy( void *, void *, int, unsigned int );
      int    _IMPORT _LNK_CONV memicmp( void *, void *, unsigned int );

      #define _fmemcpy( x, y, z )     memcpy( (x), (y), (z) )
      #define _fmemchr( x, y, z )     memchr( (x), (y), (z) )
      #define _fmemcmp( x, y, z )     memcmp( (x), (y), (z) )
      #define _fmemset( x, y, z )     memset( (x), (y), (z) )
      #define _fmemccpy( w, x, y, z ) memccpy( (w), (x), (y), (z) )
      #define _fmemicmp( x, y, z )    memicmp( (x), (y), (z) )

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

