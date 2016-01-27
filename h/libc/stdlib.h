#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __stdlib_h
   #define __stdlib_h

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

   #if !(__IBMC__ || __IBMCPP__)
      #ifndef _System
        #define _System
      #endif
      #ifndef __cdecl
        #define __cdecl
      #endif
   #endif

   /********************************************************************/
   /*  <stdlib.h> header file                                          */
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

   #ifndef __wchar_t
     #define __wchar_t
     typedef unsigned short wchar_t;
   #endif

   typedef struct _div_t
       {
       int quot;        /* quotient of integer division       */
       int rem;         /* remainder of integer division      */
       } div_t;

   typedef struct _ldiv_t
       {
       long int quot;   /* quotient of long integer division  */
       long int rem;    /* remainder of long integer division */
       } ldiv_t;

#if _LONG_LONG
   typedef struct _lldiv_t
       {
       long long int quot;   /* quotient of long long integer division  */
       long long int rem;    /* remainder of long long integer division */
       } lldiv_t;
#endif

   #ifndef NULL
      #if (defined(__EXTENDED__)  || defined( __cplusplus ))
         #define NULL 0
      #else
         #define NULL ((void *)0)
      #endif
   #endif

   #define EXIT_SUCCESS      0
   #define EXIT_FAILURE      8
   #define RAND_MAX      32767

   #ifdef __BSEXCPT__
      typedef unsigned long _System
        _EH_RTN(PEXCEPTIONREPORTRECORD, struct _EXCEPTIONREGISTRATIONRECORD *,
                PCONTEXTRECORD, PVOID);
   #elif defined(_INC_EXCPT)
      typedef unsigned long __cdecl
        _EH_RTN(struct _EXCEPTION_RECORD*, PVOID, struct _CONTEXT*, PVOID);
   #else
      #ifdef __OS2__
         typedef unsigned long _System _EH_RTN(void *,void *,void *,void *);
      #else
         typedef unsigned long __cdecl _EH_RTN(void *,void *,void *,void *);
      #endif
   #endif

   extern int _IMPORT _LNK_CONV _eh_del(_EH_RTN *);
   extern int _IMPORT _LNK_CONV _eh_add(_EH_RTN *);

   #ifdef MB_CUR_MAX
      #undef MB_CUR_MAX
   #endif
   extern int _IMPORT _mb_cur_max;
   #define MB_CUR_MAX _mb_cur_max

   /* function prototypes */

   extern int      _IMPORT _LNK_CONV atoi( const char * );
   extern long int _IMPORT _LNK_CONV atol( const char * );
   extern long int _IMPORT _LNK_CONV strtol( const char *, char **, int );
   extern unsigned long int _IMPORT _LNK_CONV strtoul( const char *, char **, int );
   extern int      _IMPORT _LNK_CONV rand( void );
   extern void     _IMPORT _LNK_CONV srand( unsigned int );
   extern void *   (_IMPORT _LNK_CONV calloc)( size_t, size_t );
   extern void     (_IMPORT _LNK_CONV free)( void * );
   extern void *   (_IMPORT _LNK_CONV malloc)( size_t );
   extern void *   (_IMPORT _LNK_CONV realloc)( void *, size_t );
   extern void     _IMPORT _LNK_CONV abort( void );
   extern int      _IMPORT _LNK_CONV atexit( void ( * )( void ) );
   extern void     _IMPORT _LNK_CONV exit( int );
   extern char *   _IMPORT _LNK_CONV getenv( const char * );
   extern int      _IMPORT _LNK_CONV system( const char * );
   extern void *   _IMPORT _LNK_CONV bsearch( const void *, const void *, size_t, size_t,
                              int ( * _LNK_CONV __compare )( const void *, const void * ) );
   extern void     _IMPORT _LNK_CONV qsort( void *, size_t, size_t,
                            int ( * _LNK_CONV __compare )( const void *, const void * ) );
   extern div_t    _IMPORT _LNK_CONV div( int, int );
   extern ldiv_t   _IMPORT _LNK_CONV ldiv( long int, long int );
   extern int      _IMPORT _LNK_CONV mblen( const char *, size_t );
   extern int      _IMPORT _LNK_CONV mbtowc( wchar_t *, const char *, size_t );
   extern int      _IMPORT _LNK_CONV wctomb( char *, wchar_t );
   extern size_t   _IMPORT _LNK_CONV mbstowcs( wchar_t *, const char *, size_t );
   extern size_t   _IMPORT _LNK_CONV wcstombs( char *, const wchar_t *, size_t );
#if _LONG_LONG
   extern long long int _IMPORT _LNK_CONV atoll( const char * );
   extern lldiv_t  _IMPORT _LNK_CONV lldiv( long long int, long long int );
   extern long long int _IMPORT _LNK_CONV strtoll( const char *, char **, int );
   extern unsigned long long int _IMPORT _LNK_CONV strtoull( const char *, char **, int );
   extern long long int _IMPORT _LNK_CONV llabs( long long int );
#endif

   #if (__THW_INTEL__ || defined(__MATH__)) && (__IBMC__ || __IBMCPP__)
      #ifdef __cplusplus
         extern "builtin" int      __abs( int );
         extern "builtin" long int __labs( long int );
         inline int      _LNK_CONV abs ( int x )      { return __abs ( x ); }
         inline long int _LNK_CONV labs( long int l ) { return __labs( l ); }
#if _LONG_LONG
         extern "builtin" long long int __llabs( long long int );
         inline long long int _LNK_CONV llabs( long long int l ) { return __llabs( l ); }
#endif
      #else
         extern int      _IMPORT _LNK_CONV abs ( int );
         extern long int _IMPORT _LNK_CONV labs( long int );
         int           _Builtin __abs( int );
         long int      _Builtin __labs( long int );
         #define  abs( x )   __abs ( (x) )
         #define  labs( x )  __labs( (x) )
#if _LONG_LONG
         long long int _Builtin __llabs( long long int );
         #define  llabs( x )  __llabs( (x) )
#endif
      #endif
   #else
      extern int      _IMPORT _LNK_CONV abs ( int );
      extern long int _IMPORT _LNK_CONV labs( long int );
   #endif

   #ifndef __ANSI__

      #ifndef __SAA_L2__

         #define max(a,b) (((a) > (b)) ? (a) : (b))
         #define min(a,b) (((a) < (b)) ? (a) : (b))

         #if __IBMC__ || __IBMCPP__
            #ifndef _alloca
               void * _Builtin __alloca( size_t );
               #define _alloca( x ) __alloca( (x) )
               #define alloca( x ) __alloca( (x) )
            #endif

            #if __THW_INTEL__
               unsigned char _Builtin __parmdwords( void );
            #endif
         #endif

         extern long double _IMPORT _LNK_CONV _atold( const char * );
         extern char * _IMPORT _LNK_CONV  ecvt( double, int, int *, int * );
         extern char * _IMPORT _LNK_CONV _ecvt( double, int, int *, int * );
         extern char * _IMPORT _LNK_CONV  fcvt( double, int, int *, int * );
         extern char * _IMPORT _LNK_CONV _fcvt( double, int, int *, int * );
         extern int    _IMPORT _LNK_CONV _freemod( unsigned long );
         extern char * _IMPORT _LNK_CONV _fullpath(char *, char *, size_t);
         extern char * _IMPORT _LNK_CONV  gcvt( double, int, char * );
         extern char * _IMPORT _LNK_CONV _gcvt( double, int, char * );
         extern int    (_IMPORT _LNK_CONV _heapmin)( void );
         extern char * _IMPORT _LNK_CONV  itoa( int, char *, int );
         extern char * _IMPORT _LNK_CONV _itoa( int, char *, int );
         extern int    _IMPORT _LNK_CONV _loadmod( char *, unsigned long * );
         extern char * _IMPORT _LNK_CONV  ltoa( long, char *, int );
         extern char * _IMPORT _LNK_CONV _ltoa( long, char *, int );
         extern size_t _IMPORT _LNK_CONV _msize(void *);
         extern int    _IMPORT _LNK_CONV  putenv( const char * );
         extern int    _IMPORT _LNK_CONV _putenv( const char * );
         extern long double _IMPORT _LNK_CONV  strtold( const char *, char ** );
         extern long double _IMPORT _LNK_CONV _strtold( const char *, char ** );
         extern char * _IMPORT _LNK_CONV  ultoa( unsigned long, char *, int );
         extern char * _IMPORT _LNK_CONV _ultoa( unsigned long, char *, int );
#if _LONG_LONG
         extern char * _IMPORT _LNK_CONV  ulltoa( unsigned long long, char *, int );
         extern char * _IMPORT _LNK_CONV _ulltoa( unsigned long long, char *, int );
#endif

         #if __IBMC__ || __IBMCPP__
            #if __WINDOWS__ && __THW_INTEL__
               #pragma map( ecvt    , "?_ecvt"    )
               #pragma map( fcvt    , "?_fcvt"    )
               #pragma map( gcvt    , "?_gcvt"    )
               #pragma map( itoa    , "?_itoa"    )
               #pragma map( ltoa    , "?_ltoa"    )
               #pragma map( ultoa   , "?_ultoa"   )
               #pragma map( ulltoa  , "?_ulltoa"  )
               #pragma map( putenv  , "?_putenv"  )
               #pragma map( _strtold, "?strtold"  )
            #else
               #pragma map( ecvt    , "_ecvt"    )
               #pragma map( fcvt    , "_fcvt"    )
               #pragma map( gcvt    , "_gcvt"    )
               #pragma map( itoa    , "_itoa"    )
               #pragma map( ltoa    , "_ltoa"    )
               #pragma map( ultoa   , "_ultoa"   )
               #pragma map( ulltoa  , "_ulltoa"  )
               #pragma map( putenv  , "_putenv"  )
               #pragma map( _strtold, "strtold"  )
            #endif
         #else
            #define ecvt _ecvt
            #define fcvt _fcvt
            #define gcvt _gcvt
            #define itoa _itoa
            #define ltoa _ltoa
            #define ultoa _ultoa
            #define putenv _putenv
            #define _strtold strtold
         #endif

         #ifndef __malloc_h

            #if defined(__TILED__) && __OS2__ && __THW_INTEL__
               extern void * _IMPORT _LNK_CONV _tcalloc( size_t, size_t );
               extern void   _IMPORT _LNK_CONV _tfree( void * );
               extern void * _IMPORT _LNK_CONV _tmalloc( size_t );
               extern void * _IMPORT _LNK_CONV _trealloc( void *, size_t );
               extern void * _IMPORT _LNK_CONV _theapmin(void);
               #ifndef __DEBUG_ALLOC__
                  #if __IBMC__ || __IBMCPP__
                     #pragma map( calloc , "_tcalloc"  )
                     #pragma map( free   , "_tfree"    )
                     #pragma map( malloc , "_tmalloc"  )
                     #pragma map( realloc, "_trealloc" )
                     #pragma map( heapmin, "_theapmin" )
                  #else
                     #define calloc _tcalloc
                     #define free _tfree
                     #define malloc _tmalloc
                     #define realloc _trealloc
                     #define heapmin _theapmin
                  #endif
               #endif
            #endif

            #ifdef __DEBUG_ALLOC__
               #if defined(__TILED__) && __OS2__ && __THW_INTEL__
                  extern void * _IMPORT _LNK_CONV _debug_tcalloc( size_t, size_t, const char *, size_t );
                  extern void   _IMPORT _LNK_CONV _debug_tfree( void *, const char *, size_t );
                  extern void * _IMPORT _LNK_CONV _debug_tmalloc( size_t, const char *, size_t );
                  extern void * _IMPORT _LNK_CONV _debug_trealloc( void *, size_t, const char *, size_t );
                  extern int    _IMPORT _LNK_CONV _debug_theapmin( const char *, size_t );
                  extern void   _IMPORT _LNK_CONV _theap_check( void );
                  extern void   _IMPORT _LNK_CONV _tdump_allocated( int );
                  extern void   _IMPORT _LNK_CONV _tdump_allocated_delta( int );

                  extern void   _IMPORT _LNK_CONV __theap_check( const char *,size_t );
                  extern void   _IMPORT _LNK_CONV __tdump_allocated( int ,const char *, size_t);
                  extern void   _IMPORT _LNK_CONV __tdump_allocated_delta( int, const char *, size_t);

                  #define _tcalloc(x,y)  _debug_tcalloc( (x), (y), __FILE__, __LINE__ )
                  #define _tfree( x )    _debug_tfree( (x), __FILE__, __LINE__ )
                  #define _tmalloc( x )  _debug_tmalloc( (x), __FILE__, __LINE__ )
                  #define _trealloc(x,y) _debug_trealloc( (x), (y), __FILE__, __LINE__ )
                  #define _theapmin( )   _debug_theapmin( __FILE__, __LINE__ )

                  #define calloc(x,y)    _debug_tcalloc( (x), (y), __FILE__, __LINE__ )
                  #define free( x )      _debug_tfree( (x), __FILE__, __LINE__ )
                  #define malloc( x )    _debug_tmalloc( (x), __FILE__, __LINE__ )
                  #define realloc(x,y)   _debug_trealloc( (x), (y), __FILE__, __LINE__ )
                  #define _heapmin( )    _debug_theapmin( __FILE__, __LINE__ )

                  #define _heap_check()            __theap_check(__FILE__,__LINE__)
                  #define _dump_allocated(x)       __tdump_allocated((x),__FILE__,__LINE__)
                  #define _dump_allocated_delta(x) __tdump_allocated_delta((x),__FILE__,__LINE__)

               #else
                  extern void * _IMPORT _LNK_CONV _debug_calloc( size_t, size_t, const char *, size_t );
                  extern void   _IMPORT _LNK_CONV _debug_free( void *, const char *, size_t );
                  extern void * _IMPORT _LNK_CONV _debug_malloc( size_t, const char *, size_t );
                  extern void * _IMPORT _LNK_CONV _debug_realloc( void *, size_t, const char *, size_t );
                  extern int    _IMPORT _LNK_CONV _debug_heapmin( const char *, size_t );
                  extern void   _IMPORT _LNK_CONV _heap_check( void );
                  extern void   _IMPORT _LNK_CONV _dump_allocated( int );
                  extern void   _IMPORT _LNK_CONV _dump_allocated_delta( int );
                  extern void   _IMPORT _LNK_CONV __heap_check( const char *,size_t );
                  extern void   _IMPORT _LNK_CONV __dump_allocated( int ,const char *, size_t);
                  extern void   _IMPORT _LNK_CONV __dump_allocated_delta( int, const char *, size_t );

                  #define calloc( x, y )  _debug_calloc( (x), (y), __FILE__, __LINE__ )
                  #define free( x )       _debug_free( (x), __FILE__, __LINE__ )
                  #define malloc( x )     _debug_malloc( (x), __FILE__, __LINE__ )
                  #define realloc( x, y ) _debug_realloc( (x), (y), __FILE__, __LINE__ )
                  #define _heapmin( )     _debug_heapmin( __FILE__, __LINE__ )

                  #define _heap_check()            __heap_check(__FILE__,__LINE__)
                  #define _dump_allocated(x)       __dump_allocated((x),__FILE__,__LINE__)
                  #define _dump_allocated_delta(x) __dump_allocated_delta((x),__FILE__,__LINE__)

               #endif
            #else
               #define _heap_check( )
               #define _dump_allocated( x )
               #define _theap_check( )
               #define _tdump_allocated( x )
               #define _dump_allocated_delta( x )
               #define _tdump_allocated_delta( x )
            #endif

         #endif

         #ifdef __MULTI__
            extern int  _IMPORT _LNK_CONV _beginthread( void ( * _LNK_CONV )( void * ), void *, unsigned, void * );
            extern void _IMPORT _LNK_CONV _endthread( void );
            extern void ** _IMPORT _LNK_CONV _threadstore( void );
         #endif

         #if __IBMC__ || __IBMCPP__
            #if __WINDOWS__ && __THW_INTEL__
               #pragma map( atof  , "?_atofieee"   )
               #pragma map( strtod, "?_strtodieee" )
            #else
               #pragma map( atof  , "_atofieee"   )
               #pragma map( strtod, "_strtodieee" )
            #endif
         #else
            #define atof _atofieee
            #define strtod _strtodieee
         #endif

         #if defined(__EXTENDED__)

            #if __THW_INTEL__ && (__IBMC__ || __IBMCPP__)
               void  _Builtin __enable( void );
               void  _Builtin __disable( void );

               #define _enable( )  __enable( )
               #define _disable( ) __disable( )
            #endif

            #ifndef errno
               #ifdef __MULTI__
                  extern int * _IMPORT _LNK_CONV _errno( void );
                  #define errno (*_errno( ))
               #else
                  extern int _IMPORT errno;
                     #define errno errno
               #endif
            #endif

            #ifndef _doserrno
               #ifdef __MULTI__
                  extern int * _IMPORT _LNK_CONV __doserrno(void);
                  #define _doserrno (*__doserrno( ))
               #else
                  extern int _IMPORT _doserrno;
                  #define _doserrno _doserrno
               #endif
            #endif

            extern const    char **_IMPORT _environ;
            extern unsigned char   _IMPORT _osmajor;
            extern unsigned char   _IMPORT _osminor;
            extern unsigned char   _IMPORT _osmode;

            #define environ _environ

            #define DOS_MODE        0       /* Real Address Mode */
            #define OS2_MODE        1       /* Protected Address Mode */

            typedef int ( __onexit_t )( void );
            typedef __onexit_t * onexit_t;

            /* Sizes for buffers used by the _makepath() and _splitpath() functions.*/
            /* Note that the sizes include space for null terminating character.    */

            #define _MAX_PATH       260   /* max. length of full pathname           */
            #define _MAX_DRIVE      3     /* max. length of drive component         */
            #define _MAX_DIR        256   /* max. length of path component          */
            #define _MAX_FNAME      256   /* max. length of file name component     */
            #define _MAX_EXT        256   /* max. length of extension component     */

            extern void           _IMPORT _LNK_CONV _exit( int );
            extern onexit_t       _IMPORT _LNK_CONV  onexit( onexit_t );
            extern onexit_t       _IMPORT _LNK_CONV _onexit( onexit_t );

            #if __THW_INTEL__ && (__IBMC__ || __IBMCPP__)
#if _LONG_LONG
               unsigned long long _Builtin  __llrotl(unsigned long long, int);
               unsigned long long _Builtin  __llrotr(unsigned long long, int);
#endif
               unsigned long  _Builtin  __lrotl(unsigned long, int);
               unsigned long  _Builtin  __lrotr(unsigned long, int);
               unsigned short _Builtin  __srotl(unsigned short, int);
               unsigned short _Builtin  __srotr(unsigned short, int);
               unsigned char  _Builtin  __crotl(unsigned char, int);
               unsigned char  _Builtin  __crotr(unsigned char, int);

               #define  _rotl( x, y )   __lrotl( (x), (y) )
               #define  _rotr( x, y )   __lrotr( (x), (y) )
               #define  _llrotl( x, y ) __llrotl( (x), (y) )
               #define  _llrotr( x, y ) __llrotr( (x), (y) )
               #define  _lrotl( x, y )  __lrotl( (x), (y) )
               #define  _lrotr( x, y )  __lrotr( (x), (y) )
               #define  _srotl( x, y )  __srotl( (x), (y) )
               #define  _srotr( x, y )  __srotr( (x), (y) )
               #define  _crotl( x, y )  __crotl( (x), (y) )
               #define  _crotr( x, y )  __crotr( (x), (y) )
            #elif __THW_PPC__
#if _LONG_LONG
               unsigned long long _llrotl(unsigned long long, int);
               unsigned long long _llrotr(unsigned long long, int);
#endif
               unsigned long  _lrotl(unsigned long, int);
               unsigned long  _lrotr(unsigned long, int);
               unsigned short _srotl(unsigned short, int);
               unsigned short _srotr(unsigned short, int);
               unsigned char  _crotl(unsigned char, int);
               unsigned char  _crotr(unsigned char, int);

               #define  _rotl( x, y )   _lrotl( (x), (y) )
               #define  _rotr( x, y )   _lrotr( (x), (y) )
               #define  _llrotl( x, y ) _llrotl( (x), (y) )
               #define  _llrotr( x, y ) _llrotr( (x), (y) )
               #define  _lrotl( x, y )  _lrotl( (x), (y) )
               #define  _lrotr( x, y )  _lrotr( (x), (y) )
               #define  _srotl( x, y )  _srotl( (x), (y) )
               #define  _srotr( x, y )  _srotr( (x), (y) )
               #define  _crotl( x, y )  _crotl( (x), (y) )
               #define  _crotr( x, y )  _crotr( (x), (y) )
            #endif

            extern void _IMPORT _LNK_CONV _makepath( char *, const char *, const char *, const char *, const char * );
            extern void _IMPORT _LNK_CONV _splitpath( const char *, char *, char *, char *, char * );
            extern void _IMPORT _LNK_CONV _searchenv( char *, char *, char *);
            extern void _IMPORT _LNK_CONV  swab( char *, char *, int );
            extern void _IMPORT _LNK_CONV _swab( char *, char *, int );

            #if __IBMC__ || __IBMCPP__
               #if __WINDOWS__ && __THW_INTEL__
                  #pragma map( onexit , "?_onexit"  )
                  #pragma map( swab   , "?_swab"    )
               #else
                  #pragma map( onexit , "_onexit"  )
                  #pragma map( swab   , "_swab"    )
               #endif
            #else
               #define onexit _onexit
               #define swab _swab
            #endif

            #if __IBMC__ || __IBMCPP__
               #if __WINDOWS__ && __THW_INTEL__
                  #pragma map( rpmatch, "?_rpmatch" )
                  #pragma map( csid   , "?_csid"    )
                  #pragma map( wcsid  , "?_wcsid"   )
               #else
                  #pragma map( rpmatch, "_rpmatch" )
                  #pragma map( csid   , "_csid"    )
                  #pragma map( wcsid  , "_wcsid"   )
               #endif
            #else
               #define rpmatch _rpmatch
               #define csid _csid
               #define wcsid _wcsid
            #endif

            extern int _IMPORT _LNK_CONV rpmatch(const char *);
            extern int _IMPORT _LNK_CONV csid(const char *);
            extern int _IMPORT _LNK_CONV wcsid(const wchar_t);

            /* Define different memory model versions of memory management       */
            /* routines to the standard names.                                   */

            #define _ncalloc( x, y )  calloc( (x), (y) )
            #define _fcalloc( x, y )  calloc( (x), (y) )
            #define _nfree( x )       free( (x) )
            #define _ffree( x )       free( (x) )
            #define _nmalloc( x )     malloc( (x) )
            #define _fmalloc( x )     malloc( (x) )
            #define _nrealloc( x, y ) realloc( (x), (y) )
            #define _frealloc( x, y ) realloc( (x), (y) )
            #define _fheapmin( )      _heapmin( )
            #define _nheapmin( )      _heapmin( )

         #endif

      #endif
   #endif

   extern double   _IMPORT _LNK_CONV atof( const char * );
   extern double   _IMPORT _LNK_CONV strtod( const char *, char ** );

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