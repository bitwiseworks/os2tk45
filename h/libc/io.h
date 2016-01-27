#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __io_h
   #define __io_h

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
   /*  <io.h> header file                                              */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if ( defined(__SPC__) || defined(__EXTENDED__) )

      int  _IMPORT _LNK_CONV _access( const char *, int );
      int  _IMPORT _LNK_CONV _chmod( const char *, int );
      int  _IMPORT _LNK_CONV _chsize( int, long );
      int  _IMPORT _LNK_CONV _close( int );
      int  _IMPORT _LNK_CONV _creat( const char *, int );
      int  _IMPORT _LNK_CONV _dup( int );
      int  _IMPORT _LNK_CONV _dup2( int, int );
      int  _IMPORT _LNK_CONV __eof( int );
      long _IMPORT _LNK_CONV _filelength( int );
      int  _IMPORT _LNK_CONV _isatty( int );
      long _IMPORT _LNK_CONV _lseek( int, long, int );
      int  _IMPORT _LNK_CONV _open( const char *, int, ... );
      int  _IMPORT _LNK_CONV _read( int, void *, unsigned int );
      int  _IMPORT _LNK_CONV remove( const char * );
      int  _IMPORT _LNK_CONV rename( const char *, const char * );
      int  _IMPORT _LNK_CONV _sopen( const char *, int, int, ... );
      long _IMPORT _LNK_CONV _tell( int );
      int  _IMPORT _LNK_CONV _umask( int );
      int  _IMPORT _LNK_CONV _unlink( const char * );
      int  _IMPORT _LNK_CONV _write( int, const void *, unsigned int );
      int  _IMPORT _LNK_CONV _setmode(int handle, int mode);

      int  _IMPORT _LNK_CONV  access( const char *, int );
      int  _IMPORT _LNK_CONV  chmod( const char *, int );
      int  _IMPORT _LNK_CONV  chsize( int, long );
      int  _IMPORT _LNK_CONV  close( int );
      int  _IMPORT _LNK_CONV  creat( const char *, int );
      int  _IMPORT _LNK_CONV  dup( int );
      int  _IMPORT _LNK_CONV  dup2( int, int );
      int  _IMPORT _LNK_CONV  eof( int );
      long _IMPORT _LNK_CONV  filelength( int );
      int  _IMPORT _LNK_CONV  isatty( int );
      long _IMPORT _LNK_CONV  lseek( int, long, int );
      int  _IMPORT _LNK_CONV  open( const char *, int, ... );
      int  _IMPORT _LNK_CONV  read( int, void *, unsigned int );
      int  _IMPORT _LNK_CONV  sopen( const char *, int, int, ... );
      long _IMPORT _LNK_CONV  tell( int );
      int  _IMPORT _LNK_CONV  umask( int );
      int  _IMPORT _LNK_CONV  unlink( const char * );
      int  _IMPORT _LNK_CONV  write( int, const void *, unsigned int );
      int  _IMPORT _LNK_CONV  setmode(int handle, int mode);

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( access    , "?_access"     )
            #pragma map( chmod     , "?_chmod"      )
            #pragma map( chsize    , "?_chsize"     )
            #pragma map( close     , "?_close"      )
            #pragma map( creat     , "?_creat"      )
            #pragma map( dup       , "?_dup"        )
            #pragma map( dup2      , "?_dup2"       )
            #pragma map( eof       , "?__eof"       )
            #pragma map( filelength, "?_filelength" )
            #pragma map( isatty    , "?_isatty"     )
            #pragma map( lseek     , "?_lseek"      )
            #pragma map( open      , "?_open"       )
            #pragma map( read      , "?_read"       )
            #pragma map( sopen     , "?_sopen"      )
            #pragma map( tell      , "?_tell"       )
            #pragma map( umask     , "?_umask"      )
            #ifndef __unlink
               #define __unlink
               #pragma map( unlink    , "?_unlink"     )
            #endif
            #pragma map( write     , "?_write"      )
            #pragma map( setmode   , "?_setmode"    )
         #else
            #pragma map( access    , "_access"     )
            #pragma map( chmod     , "_chmod"      )
            #pragma map( chsize    , "_chsize"     )
            #pragma map( close     , "_close"      )
            #pragma map( creat     , "_creat"      )
            #pragma map( dup       , "_dup"        )
            #pragma map( dup2      , "_dup2"       )
            #pragma map( eof       , "__eof"       )
            #pragma map( filelength, "_filelength" )
            #pragma map( isatty    , "_isatty"     )
            #pragma map( lseek     , "_lseek"      )
            #pragma map( open      , "_open"       )
            #pragma map( read      , "_read"       )
            #pragma map( sopen     , "_sopen"      )
            #pragma map( tell      , "_tell"       )
            #pragma map( umask     , "_umask"      )
            #ifndef __unlink
               #define __unlink
               #pragma map( unlink , "_unlink"     )
            #endif
            #pragma map( write     , "_write"      )
            #pragma map( setmode   , "_setmode"    )
         #endif
      #else
         #define access _access
         #define chmod _chmod
         #define chsize _chsize
         #define close _close
         #define creat _creat
         #define dup _dup
         #define dup2 _dup2
         #define eof __eof
         #define filelength _filelength
         #define isatty _isatty
         #define lseek _lseek
         #define open _open
         #define read _read
         #define sopen _sopen
         #define tell _tell
         #define umask _umask
         #ifndef __unlink
            #define __unlink
            #define unlink _unlink
         #endif
         #define write _write
         #define setmode _setmode
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

