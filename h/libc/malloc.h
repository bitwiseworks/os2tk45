#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __malloc_h
   #define __malloc_h

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
   /*  <malloc.h> header file                                          */
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

   #ifndef __umalloc_h

      extern int  _IMPORT _LNK_CONV _heap_walk(int (* _LNK_CONV callback)(const void *, size_t,
                                int,int ,const char *, size_t)) ;

      extern int  _IMPORT _LNK_CONV _heapset(unsigned int fill);
      extern int  _IMPORT _LNK_CONV _heapchk(void);

      #define _HEAPOK        (0)
      #define _HEAPEMPTY     (1)
      #define _HEAPBADNODE   (2)
      #define _HEAPBADBEGIN  (3)
      #define _FREEENTRY     (4)
      #define _USEDENTRY     (5)

   #endif

   #ifndef __stdlib_h

      #ifndef __size_t
         #define __size_t
         typedef unsigned int size_t;
      #endif

      #ifdef __EXTENDED__

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

         /* function prototypes */

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

         extern void * _IMPORT _LNK_CONV calloc( size_t, size_t );
         extern void   _IMPORT _LNK_CONV free( void * );
         extern void * _IMPORT _LNK_CONV malloc( size_t );
         extern void * _IMPORT _LNK_CONV realloc( void *, size_t );
         extern int    _IMPORT _LNK_CONV _heapmin(void);
         extern size_t _IMPORT _LNK_CONV _msize(void *);

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
               extern void   _IMPORT _LNK_CONV __tdump_allocated_delta( int, const char *,size_t);

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
               extern void   _IMPORT _LNK_CONV __dump_allocated_delta( int, const char *, size_t);

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

      #endif /* __EXTENDED__ */
   #endif /* !__stdlib_h */

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

