#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __stdio_h
   #define __stdio_h

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
   /*  <stdio.h> header file                                           */
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

   enum _OPERATIONS { _IOINIT, _IOREAD, _IOWRITE, _IOREPOSITION,
                      _IOFLUSH, _IOUNDEFOP };

   #define _MAX_UNGET 4

   #if __THW_PPC__
      typedef struct __file       /* visible portion of the FILE struct */
         {
         unsigned char     *_bufPtr;
         unsigned long int  _count;
         unsigned long int  _userFlags;
         unsigned long int  _bufLen;
         unsigned long int  _ungetCount;
         int                _tempStore;
         enum _OPERATIONS   _lastOp;
         unsigned char      _ungetBuf[_MAX_UNGET];
         char               _filler;
         } FILE;
   #else /* __THW_INTEL__ */
      #pragma pack( 1 )
      typedef struct __file       /* visible portion of the FILE struct */
         {
         unsigned char     *_bufPtr;
         unsigned long int  _count;
         unsigned long int  _userFlags;
         unsigned long int  _bufLen;
         unsigned long int  _ungetCount;
         int                _tempStore;
         unsigned char      _ungetBuf[_MAX_UNGET];
         enum _OPERATIONS   _lastOp;
         char               _filler;
         } FILE;
      #pragma pack( )
   #endif

   typedef struct __fpos_t      /* Definition of file positioning structure */
      {
      long int __fpos_elem[2];
      } fpos_t;

   #ifndef NULL
      #if (defined(__EXTENDED__)  || defined( __cplusplus ))
         #define NULL 0
      #else
         #define NULL ((void *)0)
      #endif
   #endif

   #define _IOFBF              1   /* Buffer modes */
   #define _IOLBF              2
   #define _IONBF              3
   #define BUFSIZ           4096   /* Default buffer size */
   #define EOF              (-1)
   #define L_tmpnam          260   /* Maximum length of temporary names */
   #define FOPEN_MAX          20   /* Minimum number of open files guaranteed */
   #define FILENAME_MAX      260   /* Maximum file name length */
   #define SEEK_SET            0   /* fseek constants */
   #define SEEK_CUR            1
   #define SEEK_END            2
   #define TMP_MAX        100000   /* Maximum guaranteed unique file names */

   #define _IOEOF         0x0001   /* EOF flag mask */
   #define _IOERR         0x0002   /* ERR flag mask */

   #if __THW_PPC__
      #ifndef __def_va_list
         #define __def_va_list
         typedef struct {
             char gpr;
             char fpr;
             char reserved[2];
             char *input_arg_area;
             char *reg_save_area;
         } __va_list[1];
      #endif
   #else /* __THW_INTEL__ */
      typedef char *__va_list;
   #endif

   /* Standard stream pointers. */

   #ifdef _WIN32S
      extern FILE ** const _IMPORT _LNK_CONV _stdin( void );
      extern FILE ** const _IMPORT _LNK_CONV _stdout( void );
      extern FILE ** const _IMPORT _LNK_CONV _stderr( void );

      #define stdin          (*_stdin())
      #define stdout         (*_stdout())
      #define stderr         (*_stderr())
   #else
      extern FILE * const _IMPORT stdin;
      extern FILE * const _IMPORT stdout;
      extern FILE * const _IMPORT stderr;

      #define stdin          stdin
      #define stdout         stdout
      #define stderr         stderr
   #endif

   /* Function declarations. */

   void     _IMPORT _LNK_CONV clearerr( FILE * );
   int      _IMPORT _LNK_CONV fclose( FILE * );
   int      _IMPORT _LNK_CONV feof( FILE * );
   int      _IMPORT _LNK_CONV ferror( FILE * );
   int      _IMPORT _LNK_CONV fflush( FILE * );
   int      _IMPORT _LNK_CONV fgetc( FILE * );
   int      _IMPORT _LNK_CONV fgetpos( FILE *, fpos_t * );
   char   * _IMPORT _LNK_CONV fgets( char *, int, FILE * );
   FILE   * _IMPORT _LNK_CONV fopen( const char *, const char * );
   int      _IMPORT _LNK_CONV fputc( int, FILE * );
   int      _IMPORT _LNK_CONV fputs( const char *, FILE * );
   size_t   _IMPORT _LNK_CONV fread( void *, size_t, size_t, FILE * );
   FILE   * _IMPORT _LNK_CONV freopen( const char *, const char *, FILE * );
   int      _IMPORT _LNK_CONV fseek( FILE *, long int, int );
   int      _IMPORT _LNK_CONV fsetpos( FILE *, const fpos_t * );
   long int _IMPORT _LNK_CONV ftell( FILE * );
   size_t   _IMPORT _LNK_CONV fwrite( const void *, size_t, size_t, FILE * );
   int              _LNK_CONV getc( FILE * );
   int              _LNK_CONV getchar( void );
   char   * _IMPORT _LNK_CONV gets( char * );
   void     _IMPORT _LNK_CONV perror( const char * );
   int              _LNK_CONV putc( int, FILE * );
   int              _LNK_CONV putchar( int );
   int      _IMPORT _LNK_CONV puts( const char * );
   int      _IMPORT _LNK_CONV remove( const char * );
   int      _IMPORT _LNK_CONV rename( const char *, const char * );
   void     _IMPORT _LNK_CONV rewind( FILE * );
   void     _IMPORT _LNK_CONV setbuf( FILE *, char * );
   int      _IMPORT _LNK_CONV setvbuf( FILE *, char *, int, size_t );
   FILE   * _IMPORT _LNK_CONV tmpfile( void );
   char   * _IMPORT _LNK_CONV tmpnam( char * );
   int      _IMPORT _LNK_CONV ungetc( int, FILE * );

   #define __getc(p) ( \
      (  ((p)->_ungetCount==0) && ((p)->_count!=0))? \
         (--(p)->_count, (int)(*(p)->_bufPtr++)) :\
         fgetc((p)) )

   #define __putc(c, p) ( \
      ((((p)->_tempStore = (c)) != '\n') && \
      ((p)->_lastOp == _IOWRITE) && ((p)->_count < (p)->_bufLen)) ? \
         (++(p)->_count, *(p)->_bufPtr++ = (p)->_tempStore): \
         fputc((p)->_tempStore,(p)) )

   #ifndef __MULTI__
      #ifdef __cplusplus
         inline int getc( FILE * p )        { return __getc( p ); }
         inline int putc( int c, FILE * p ) { return __putc( c, p ); }
      #else
         #define getc( p )    __getc( (p) )
         #define putc( c, p ) __putc( (c), (p) )
      #endif
   #else
      #ifdef __cplusplus
         inline int getc( FILE * p )        { return fgetc( p ); }
         inline int putc( int c, FILE * p ) { return fputc( c, p ); }
      #else
         #define getc( p )  fgetc( (p) )
         #define putc( c, p ) fputc( (c), (p) )
      #endif
   #endif

   #ifdef __cplusplus
      inline int getchar( void )   { return getc( stdin ); }
      inline int putchar( int c ) { return putc( c, stdout ); }
   #else
      #define getchar( )   getc( stdin )
      #define putchar( c ) putc( (c), stdout )
   #endif

   /* clearerr, feof, and ferror macros - single threaded only */

   #ifndef __MULTI__
      #define clearerr( stream ) ( ( void )( (stream)->_userFlags = 0L ) )
      #define feof( stream )     ( ( int  )( (stream)->_userFlags & _IOEOF ) )
      #define ferror( stream )   ( ( int  )( (stream)->_userFlags & _IOERR ) )
   #endif

   #if __EXTENDED__ || defined(__cplusplus)

      #define _IO_WRITETHRU  0x0004   /* WRITETHRU mask */

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( fprintf, "?_fprintfieee" )
            #pragma map( printf , "?_printfieee"  )
            #pragma map( sprintf, "?_sprintfieee" )
            #pragma map( fscanf , "?_fscanfieee"  )
            #pragma map( scanf  , "?_scanfieee"   )
            #pragma map( sscanf , "?_sscanfieee"  )
            #pragma map( vfprintf, "?_vfprintfieee" )
            #pragma map( vprintf , "?_vprintfieee"  )
            #pragma map( vsprintf, "?_vsprintfieee" )
         #else
            #pragma map( fprintf, "_fprintfieee" )
            #pragma map( printf , "_printfieee"  )
            #pragma map( sprintf, "_sprintfieee" )
            #pragma map( fscanf , "_fscanfieee"  )
            #pragma map( scanf  , "_scanfieee"   )
            #pragma map( sscanf , "_sscanfieee"  )
            #pragma map( vfprintf, "_vfprintfieee" )
            #pragma map( vprintf , "_vprintfieee"  )
            #pragma map( vsprintf, "_vsprintfieee" )
         #endif
      #else
         #define fprintf _fprintfieee
         #define printf _printfieee
         #define sprintf _sprintfieee
         #define fscanf _fscanfieee
         #define scanf _scanfieee
         #define sscanf _sscanfieee
         #define vfprintf _vfprintfieee
         #define vprintf _vprintfieee
         #define fsprintf _fsprintfieee
      #endif

      int _LNK_CONV _fcloseall( void );
      int _LNK_CONV _rmtmp( void );

      #define P_tmpdir "\\"
      FILE * _IMPORT _LNK_CONV fdopen( int, const char *);
      int    _IMPORT _LNK_CONV fgetchar( void );
      int    _IMPORT _LNK_CONV fileno( FILE * );
      int    _IMPORT _LNK_CONV flushall( void );
      int    _IMPORT _LNK_CONV fputchar( int );
      char * _IMPORT _LNK_CONV tempnam( char *, char * );
      int    _IMPORT _LNK_CONV unlink( const char * );

      FILE * _IMPORT _LNK_CONV _fdopen( int, const char *);
      int    _IMPORT _LNK_CONV _fgetchar( void );
      int    _IMPORT _LNK_CONV _fileno( FILE * );
      int    _IMPORT _LNK_CONV _flushall( void );
      int    _IMPORT _LNK_CONV _fputchar( int );
      char * _IMPORT _LNK_CONV _tempnam( char *, char * );
      int    _IMPORT _LNK_CONV _unlink( const char * );

      int    _IMPORT _LNK_CONV _set_crt_msg_handle( int );

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( fdopen  , "?_fdopen"   )
            #pragma map( fgetchar, "?_fgetchar" )
            #pragma map( fileno  , "?_fileno"   )
            #pragma map( flushall, "?_flushall" )
            #pragma map( fputchar, "?_fputchar" )
            #pragma map( tempnam , "?_tempnam"  )
            #ifndef __unlink
               #define __unlink
               #pragma map( unlink  , "?_unlink"   )
            #endif
         #else
            #pragma map( fdopen  , "_fdopen"   )
            #pragma map( fgetchar, "_fgetchar" )
            #pragma map( fileno  , "_fileno"   )
            #pragma map( flushall, "_flushall" )
            #pragma map( fputchar, "_fputchar" )
            #pragma map( tempnam , "_tempnam"  )
            #ifndef __unlink
               #define __unlink
               #pragma map( unlink  , "_unlink"   )
            #endif
         #endif
      #else
         #define fdopen _fdopen
         #define fgetchar _fgetchar
         #define fileno _fileno
         #define flushall _flushall
         #define fputchar _fputchar
         #define tempnam _tempnam
         #ifndef __unlink
            #define __unlink
            #define unlink _unlink
         #endif
      #endif

   #endif

   int      _IMPORT _LNK_CONV fprintf( FILE *, const char *, ... );
   int      _IMPORT _LNK_CONV fscanf( FILE *, const char *, ... );
   int      _IMPORT _LNK_CONV printf( const char *, ... );
   int      _IMPORT _LNK_CONV scanf( const char *, ... );
   int      _IMPORT _LNK_CONV sprintf( char *, const char *, ... );
   int      _IMPORT _LNK_CONV sscanf( const char *, const char *, ... );
   int      _IMPORT _LNK_CONV vfprintf( FILE *, const char *, __va_list );
   int      _IMPORT _LNK_CONV vprintf( const char *, __va_list );
   int      _IMPORT _LNK_CONV vsprintf( char *, const char *, __va_list );

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

