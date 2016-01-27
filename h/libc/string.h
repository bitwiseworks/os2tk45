#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __string_h
   #define __string_h

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
   /*  <string.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #ifndef __size_t
      #define __size_t
      typedef unsigned int size_t;
   #endif

   #ifndef NULL
      #if (defined(__EXTENDED__)  || defined( __cplusplus ))
         #define NULL 0
      #else
         #define NULL ((void *)0)
      #endif
   #endif

   #if __THW_INTEL__ && (__IBMC__ | __IBMCPP__)

      char * _Builtin __strcat( char *, const char * );
      char * _Builtin __strchr( const char *, int );
      int    _Builtin __strcmp( const char *, const char * );
      char * _Builtin __strcpy( char*, const char * );
      size_t _Builtin __strlen( const char * );
      char * _Builtin __strncat( char *, const char *, size_t );
      int    _Builtin __strncmp( const char *, const char *, size_t );
      char * _Builtin __strncpy( char *, const char *, size_t );
      char * _Builtin __strrchr( const char *, int );

      #ifdef __cplusplus
         inline char * _LNK_CONV strcat( char * s1, const char * s2 )
                               { return __strcat( s1, s2 ); }
         inline char * _LNK_CONV strchr( const char * s, int ch )
                               { return __strchr( s, ch ); }
         inline int    _LNK_CONV strcmp( const char * s1, const char * s2 )
                               { return __strcmp( s1, s2 ); }
         inline char * _LNK_CONV strcpy( char * s1, const char * s2 )
                               { return __strcpy( s1, s2 ); }
         inline size_t _LNK_CONV strlen( const char * s )
                               { return __strlen( s ); }
         inline char * _LNK_CONV strncat( char * s1, const char * s2, size_t len )
                               { return __strncat( s1, s2, len ); }
         inline int    _LNK_CONV strncmp( const char * s1, const char * s2, size_t len )
                               { return __strncmp( s1, s2, len ); }
         inline char * _LNK_CONV strncpy( char * s1, const char * s2, size_t len )
                               { return __strncpy( s1, s2, len ); }
         inline char * _LNK_CONV strrchr( const char * s1, int ch )
                               { return __strrchr( s1, ch ); }
      #else
         char * _IMPORT _LNK_CONV strcat( char *, const char * );
         char * _IMPORT _LNK_CONV strchr( const char *, int );
         int    _IMPORT _LNK_CONV strcmp( const char *, const char * );
         char * _IMPORT _LNK_CONV strcpy( char *, const char * );
         size_t _IMPORT _LNK_CONV strlen( const char * );
         char * _IMPORT _LNK_CONV strncat( char *, const char *, size_t );
         int    _IMPORT _LNK_CONV strncmp( const char *, const char *, size_t );
         char * _IMPORT _LNK_CONV strncpy( char *, const char *, size_t );
         char * _IMPORT _LNK_CONV strrchr( const char *, int );

         #define strcat( x, y )     __strcat( (x), (y) )
         #define strchr( x, y )     __strchr( (x), (y) )
         #define strcmp( x, y )     __strcmp( (x), (y) )
         #define strcpy( x, y )     __strcpy( (x), (y) )
         #define strlen( x )        __strlen( (x) )
         #define strncat( x, y, z ) __strncat( (x), (y), (z) )
         #define strncmp( x, y, z ) __strncmp( (x), (y), (z) )
         #define strncpy( x, y, z ) __strncpy( (x), (y), (z) )
         #define strrchr( x, y )    __strrchr( (x), (y) )
      #endif

   #elif __THW_PPC__ && (__IBMC__ || __IBMCPP__)

      #ifdef __cplusplus
         extern "builtin" char *__strcpy(char *s1, const char *s2);
         extern "builtin" int   __strcmp(const char *s1, const char *s2);
         extern "builtin" size_t __strlen(const char *s);
         extern "builtin" char *__strchr(const char *s, int c);
         extern "builtin" char *__strrchr(const char *s, int c);
         extern "builtin" char  *__strcat(char *s1, const char *s2);
      #else
         char * _Builtin  __strcpy(char *s1, const char *s2);
         int    _Builtin  __strcmp(const char *s1, const char *s2);
         size_t _Builtin  __strlen(const char *s);
         char * _Builtin  __strchr(const char *s, int c);
         char * _Builtin  __strrchr(const char *s, int c);
         char * _Builtin  __strcat(char *s1, const char *s2);
      #endif
 
      char * strncat( char *, const char *, size_t );
      int    strncmp( const char *, const char *, size_t );
      char * strncpy( char *, const char *, size_t );
      char * strrchr( const char *, int );

      #if defined(__STR__)
         #ifdef __cplusplus
            inline char *strcpy(char *s1, const char *s2) { return __strcpy (s1, s2);}
            inline int   strcmp(const char *s1, const char *s2)     { return __strcmp (s1, s2);}
            inline size_t strlen(const char *s) { return __strlen (s); }
            inline char  *strchr(const char *s, int c) { return __strchr (s, c); }
            inline char  *strrchr(const char *s, int c) { return __strrchr (s, c); }
            inline char  *strcat(char *s1, const char *s2) { return __strcat (s1, s2); }
         #else
            #define strcpy(__s1,__s2) __strcpy(__s1,__s2)
            #define strcmp(__s1,__s2) __strcmp(__s1,__s2)
            #define strlen(__s1) __strlen(__s1)
            #define strchr(__s1,__c) __strchr(__s1,__c)
            #define strrchr(__s1,__c) __strrchr(__s1,__c)
            #define strcat(__s1,__s2) __strcat(__s1,__s2)
         #endif
      #else
         char * strcpy(char *s1, const char *s2);
         int  strcmp(const char *s1, const char *s2);
         size_t strlen(const char *s);
         char * strchr(const char *s, int c);
         char * strrchr(const char *s, int c);
         char * strcat(char *s1, const char *s2);
      #endif

   #else

      char * _IMPORT _LNK_CONV strcat( char *, const char * );
      char * _IMPORT _LNK_CONV strchr( const char *, int );
      int    _IMPORT _LNK_CONV strcmp( const char *, const char * );
      char * _IMPORT _LNK_CONV strcpy( char *, const char * );
      size_t _IMPORT _LNK_CONV strlen( const char * );
      char * _IMPORT _LNK_CONV strncat( char *, const char *, size_t );
      int    _IMPORT _LNK_CONV strncmp( const char *, const char *, size_t );
      char * _IMPORT _LNK_CONV strncpy( char *, const char *, size_t );
      char * _IMPORT _LNK_CONV strrchr( const char *, int );

   #endif

      
   int    _IMPORT _LNK_CONV strcoll( const char *, const char * );
   size_t _IMPORT _LNK_CONV strcspn( const char *, const char * );
   char * _IMPORT _LNK_CONV strerror( int );
   char * _IMPORT _LNK_CONV strpbrk( const char *, const char * );
   size_t _IMPORT _LNK_CONV strspn( const char *, const char * );
   char * _IMPORT _LNK_CONV strstr( const char *, const char * );
   char * _IMPORT _LNK_CONV strtok( char*, const char * );
   size_t _IMPORT _LNK_CONV strxfrm( char *, const char *, size_t );


   #ifndef __memory_h
      #define __memory_h

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

      #if defined( __EXTENDED__ )

         void * _IMPORT _LNK_CONV memccpy( void *, void *, int, unsigned int );
         int    _IMPORT _LNK_CONV memicmp( void *, void *, unsigned int );

         #define _fmemcpy( x, y, z )     memcpy( (x), (y), (z) )
         #define _fmemchr( x, y, z )     memchr( (x), (y), (z) )
         #define _fmemcmp( x, y, z )     memcmp( (x), (y), (z) )
         #define _fmemset( x, y, z )     memset( (x), (y), (z) )
         #define _fmemccpy( w, x, y, z ) memccpy( (w), (x), (y), (z) )
         #define _fmemicmp( x, y, z )    memicmp( (x), (y), (z) )

      #endif

   #endif

   #if defined(__EXTENDED__)

      char * _IMPORT _LNK_CONV strdup( const char * );
      int    _IMPORT _LNK_CONV stricmp( const char *, const char * );
      char * _IMPORT _LNK_CONV strlwr( char * );
      int    _IMPORT _LNK_CONV strnicmp( const char *, const char *, size_t );
      char * _IMPORT _LNK_CONV strupr( char * );

      int    _IMPORT _LNK_CONV strcmpi( const char *, const char * );
      char * _IMPORT _LNK_CONV strrev( char * );
      char * _IMPORT _LNK_CONV strset( char *, int );
      char * _IMPORT _LNK_CONV strnset( char *, int, size_t );
      char * _IMPORT _LNK_CONV _strerror( const char * );

      #define _fmemmove( x, y, z )  memmove( (x), (y), (z) )
      #define _fstrcat( x, y )      strcat( (x), (y) )
      #define _fstrchr( x, y )      strchr( (x), (y) )
      #define _fstrcmp( x, y )      strcmp( (x), (y) )
      #define _fstrcpy( x, y )      strcpy( (x), (y) )
      #define _fstrlen( x )         strlen( (x) )
      #define _fstrncat( x, y, z )  strncat( (x), (y), (z) )
      #define _fstrncmp( x, y, z )  strncmp( (x), (y), (z) )
      #define _fstrncpy( x, y, z )  strncpy( (x), (y), (z) )
      #define _fstrrchr( x, y )     strrchr( (x), (y) )

      size_t _IMPORT _LNK_CONV _fstrcspn( const char *, const char * );
      char * _IMPORT _LNK_CONV _fstrdup( const char * );
      char * _IMPORT _LNK_CONV _nstrdup( const char * );
      int    _IMPORT _LNK_CONV _fstricmp( const char *, const char * );
      char * _IMPORT _LNK_CONV _fstrlwr( char * );
      int    _IMPORT _LNK_CONV _fstrnicmp( const char *, const char *, size_t );
      char * _IMPORT _LNK_CONV _fstrnset( char *, int, size_t );
      char * _IMPORT _LNK_CONV _fstrpbrk( const char *, const char * );
      char * _IMPORT _LNK_CONV _fstrrev( char * );
      char * _IMPORT _LNK_CONV _fstrset( char *, int );
      size_t _IMPORT _LNK_CONV _fstrspn( const char *, const char * );
      char * _IMPORT _LNK_CONV _fstrstr( const char *, const char * );
      char * _IMPORT _LNK_CONV _fstrtok( char*, const char * );
      char * _IMPORT _LNK_CONV _fstrupr( char * );

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( _fstrcspn , "?strcspn"  )
            #pragma map( _fstrdup  , "?strdup"   )
            #pragma map( _nstrdup  , "?strdup"   )
            #pragma map( _fstricmp , "?stricmp"  )
            #pragma map( _fstrlwr  , "?strlwr"   )
            #pragma map( _fstrnicmp, "?strnicmp" )
            #pragma map( _fstrnset , "?strnset"  )
            #pragma map( _fstrpbrk , "?strpbrk"  )
            #pragma map( _fstrrev  , "?strrev"   )
            #pragma map( _fstrset  , "?strset"   )
            #pragma map( _fstrspn  , "?strspn"   )
            #pragma map( _fstrstr  , "?strstr"   )
            #pragma map( _fstrtok  , "?strtok"   )
            #pragma map( _fstrupr  , "?strupr"   )
         #else
            #pragma map( _fstrcspn , "strcspn"  )
            #pragma map( _fstrdup  , "strdup"   )
            #pragma map( _nstrdup  , "strdup"   )
            #pragma map( _fstricmp , "stricmp"  )
            #pragma map( _fstrlwr  , "strlwr"   )
            #pragma map( _fstrnicmp, "strnicmp" )
            #pragma map( _fstrnset , "strnset"  )
            #pragma map( _fstrpbrk , "strpbrk"  )
            #pragma map( _fstrrev  , "strrev"   )
            #pragma map( _fstrset  , "strset"   )
            #pragma map( _fstrspn  , "strspn"   )
            #pragma map( _fstrstr  , "strstr"   )
            #pragma map( _fstrtok  , "strtok"   )
            #pragma map( _fstrupr  , "strupr"   )
         #endif
      #else
         #define _fstrcspn strcspn
         #define _fstrdup strdup
         #define _nstrdup strdup
         #define _fstricmp stricmp
         #define _fstrlwr strlwr
         #define _fstrnicmp strnicmp
         #define _fstrnset strnset
         #define _fstrpbrk strpbrk
         #define _fstrrev strrev
         #define _fstrset strset
         #define _fstrspn strspn
         #define _fstrstr strstr
         #define _fstrtok strtok
         #define _fstrupr strupr
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