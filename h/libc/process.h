#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __process_h
   #define __process_h

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
   /*  <process.h> header file                                         */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if defined(__EXTENDED__)

      /* Modeflag values for spawnxx routines */

      #define P_WAIT          0
      #define P_NOWAIT        1
      #define P_OVERLAY       2

      /* Action codes used with cwait() */

      #define WAIT_CHILD      0
      #define WAIT_GRANDCHILD 1

      /* function prototypes */

      void _IMPORT _LNK_CONV abort( void );
      void _IMPORT _LNK_CONV exit( int );
      int  _IMPORT _LNK_CONV system( const char * );

      void _IMPORT _LNK_CONV _exit( int );

      int  _IMPORT _LNK_CONV _execl( char *, char *, ... );
      int  _IMPORT _LNK_CONV _execle( char *, char *, ... );
      int  _IMPORT _LNK_CONV _execlp( char *, char *, ... );
      int  _IMPORT _LNK_CONV _execlpe( char *, char *, ... );
      int  _IMPORT _LNK_CONV _spawnl( int, char *, char *, ... );
      int  _IMPORT _LNK_CONV _spawnle( int, char *, char *, ... );
      int  _IMPORT _LNK_CONV _spawnlp( int, char *, char *, ... );
      int  _IMPORT _LNK_CONV _spawnlpe( int, char *, char *, ... );

      int  _IMPORT _LNK_CONV  execl( char *, char *, ... );
      int  _IMPORT _LNK_CONV  execle( char *, char *, ... );
      int  _IMPORT _LNK_CONV  execlp( char *, char *, ... );
      int  _IMPORT _LNK_CONV  execlpe( char *, char *, ... );
      int  _IMPORT _LNK_CONV  spawnl( int, char *, char *, ... );
      int  _IMPORT _LNK_CONV  spawnle( int, char *, char *, ... );
      int  _IMPORT _LNK_CONV  spawnlp( int, char *, char *, ... );
      int  _IMPORT _LNK_CONV  spawnlpe( int, char *, char *, ... );

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map(  _execl   , "?_execl_ansi"    )
            #pragma map(  _execle  , "?_execle_ansi"   )
            #pragma map(  _execlp  , "?_execlp_ansi"   )
            #pragma map(  _execlpe , "?_execlpe_ansi"  )
            #pragma map(  _spawnl  , "?_spawnl_ansi"   )
            #pragma map(  _spawnle , "?_spawnle_ansi"  )
            #pragma map(  _spawnlp , "?_spawnlp_ansi"  )
            #pragma map(  _spawnlpe, "?_spawnlpe_ansi" )

            #pragma map(  execl   , "?_execl_ansi"    )
            #pragma map(  execle  , "?_execle_ansi"   )
            #pragma map(  execlp  , "?_execlp_ansi"   )
            #pragma map(  execlpe , "?_execlpe_ansi"  )
            #pragma map(  spawnl  , "?_spawnl_ansi"   )
            #pragma map(  spawnle , "?_spawnle_ansi"  )
            #pragma map(  spawnlp , "?_spawnlp_ansi"  )
            #pragma map(  spawnlpe, "?_spawnlpe_ansi" )
         #else
            #pragma map( _execl   , "_execl_ansi"    )
            #pragma map( _execle  , "_execle_ansi"   )
            #pragma map( _execlp  , "_execlp_ansi"   )
            #pragma map( _execlpe , "_execlpe_ansi"  )
            #pragma map( _spawnl  , "_spawnl_ansi"   )
            #pragma map( _spawnle , "_spawnle_ansi"  )
            #pragma map( _spawnlp , "_spawnlp_ansi"  )
            #pragma map( _spawnlpe, "_spawnlpe_ansi" )

            #pragma map(  execl   , "_execl_ansi"    )
            #pragma map(  execle  , "_execle_ansi"   )
            #pragma map(  execlp  , "_execlp_ansi"   )
            #pragma map(  execlpe , "_execlpe_ansi"  )
            #pragma map(  spawnl  , "_spawnl_ansi"   )
            #pragma map(  spawnle , "_spawnle_ansi"  )
            #pragma map(  spawnlp , "_spawnlp_ansi"  )
            #pragma map(  spawnlpe, "_spawnlpe_ansi" )
         #endif
      #else
         #define execl _execl
         #define execle _execle
         #define execlp _execlp
         #define execlpe _execlpe
         #define spawnl _spawnl
         #define spawnle _spawnle
         #define spawnlp _spawnlp
         #define spawnlpe _spawnlpe
      #endif

      int  _IMPORT _LNK_CONV _cwait( int *, int, int );
      int  _IMPORT _LNK_CONV _execv( char *, char ** );
      int  _IMPORT _LNK_CONV _execve( char *, char **, char ** );
      int  _IMPORT _LNK_CONV _execvp( char *, char ** );
      int  _IMPORT _LNK_CONV _execvpe( char *, char **, char ** );
      int  _IMPORT _LNK_CONV _getpid( void );
      int  _IMPORT _LNK_CONV _spawnv( int, char *, char ** );
      int  _IMPORT _LNK_CONV _spawnve( int, char *, char **, char ** );
      int  _IMPORT _LNK_CONV _spawnvp( int, char *, char ** );
      int  _IMPORT _LNK_CONV _spawnvpe( int, char *, char **, char ** );
      int  _IMPORT _LNK_CONV _wait( int * );

      int  _IMPORT _LNK_CONV  cwait( int *, int, int );
      int  _IMPORT _LNK_CONV  execv( char *, char ** );
      int  _IMPORT _LNK_CONV  execve( char *, char **, char ** );
      int  _IMPORT _LNK_CONV  execvp( char *, char ** );
      int  _IMPORT _LNK_CONV  execvpe( char *, char **, char ** );
      int  _IMPORT _LNK_CONV  getpid( void );
      int  _IMPORT _LNK_CONV  spawnv( int, char *, char ** );
      int  _IMPORT _LNK_CONV  spawnve( int, char *, char **, char ** );
      int  _IMPORT _LNK_CONV  spawnvp( int, char *, char ** );
      int  _IMPORT _LNK_CONV  spawnvpe( int, char *, char **, char ** );
      #ifdef __OS2__
         int  _IMPORT _LNK_CONV  wait( int * );
      #endif

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( cwait   , "?_cwait"    )
            #pragma map( execv   , "?_execv"    )
            #pragma map( execve  , "?_execve"   )
            #pragma map( execvp  , "?_execvp"   )
            #pragma map( execvpe , "?_execvpe"  )
            #pragma map( getpid  , "?_getpid"   )
            #pragma map( spawnv  , "?_spawnv"   )
            #pragma map( spawnve , "?_spawnve"  )
            #pragma map( spawnvp , "?_spawnvp"  )
            #pragma map( spawnvpe, "?_spawnvpe" )
         #else
            #pragma map( cwait   , "_cwait"    )
            #pragma map( execv   , "_execv"    )
            #pragma map( execve  , "_execve"   )
            #pragma map( execvp  , "_execvp"   )
            #pragma map( execvpe , "_execvpe"  )
            #pragma map( getpid  , "_getpid"   )
            #pragma map( spawnv  , "_spawnv"   )
            #pragma map( spawnve , "_spawnve"  )
            #pragma map( spawnvp , "_spawnvp"  )
            #pragma map( spawnvpe, "_spawnvpe" )
            #pragma map( wait    , "_wait"     )
         #endif
      #else
         #define cwait _cwait
         #define execv _execv
         #define execve _execve
         #define execvp _execvp
         #define execvpe _execvpe
         #define getpid _getpid
         #define spawnv _spawnv
         #define spawnve _spawnve
         #define spawnvp _spawnvp
         #define spawnvpe _spawnvpe
         #define wait _wait
      #endif

      #ifdef __MULTI__
         int  _IMPORT _LNK_CONV _beginthread( void ( * _LNK_CONV )( void * ), void *, unsigned, void * );
         void _IMPORT _LNK_CONV _endthread( void );
         void ** _IMPORT _LNK_CONV _threadstore( void );
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