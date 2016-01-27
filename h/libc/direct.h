#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __direct_h
   #define __direct_h

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
   /*  <direct.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if defined(__EXTENDED__)

      int    _IMPORT _LNK_CONV _chdir( char * );
      char * _IMPORT _LNK_CONV _getcwd( char *, int );
      int    _IMPORT _LNK_CONV _mkdir( char * );
      int    _IMPORT _LNK_CONV _rmdir( char * );

      int    _IMPORT _LNK_CONV chdir( char * );
      char * _IMPORT _LNK_CONV getcwd( char *, int );
      int    _IMPORT _LNK_CONV mkdir( char * );
      int    _IMPORT _LNK_CONV rmdir( char * );

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( chdir , "?_chdir"  )
            #pragma map( getcwd, "?_getcwd" )
            #pragma map( mkdir , "?_mkdir"  )
            #pragma map( rmdir , "?_rmdir"  )
         #else
            #pragma map( chdir , "_chdir"  )
            #pragma map( getcwd, "_getcwd" )
            #pragma map( mkdir , "_mkdir"  )
            #pragma map( rmdir , "_rmdir"  )
         #endif
      #else
         #define chdir _chdir
         #define getcwd _getcwd
         #define mkdir _mkdir
         #define rmdir _rmdir
      #endif

      int    _IMPORT _LNK_CONV _chdrive( int );
      char * _IMPORT _LNK_CONV _getdcwd( int , char *, int );
      int    _IMPORT _LNK_CONV _getdrive( void );


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

