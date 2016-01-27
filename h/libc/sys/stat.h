#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __stat_h
   #define __stat_h

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
   /*  <sys\stat.h> header file                                        */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if ( defined(__SPC__) || defined(__EXTENDED__))

      #ifndef __ino_t
         typedef unsigned short ino_t;
         #define __ino_t
      #endif

      #ifndef __time_t
         #define __time_t
         typedef long time_t;
      #endif

      #ifndef __dev_t
         typedef short dev_t;
         #define __dev_t
      #endif

      #ifndef __off_t
         typedef long off_t;
         #define __off_t
      #endif

      #pragma pack( 2 )
      struct stat
         {
         dev_t st_dev;
         ino_t st_ino;
         unsigned short st_mode;
         short st_nlink;
         short st_uid;
         short st_gid;
         dev_t st_rdev;
         unsigned short __filler;
         off_t st_size;
         time_t st_atime;
         time_t st_mtime;
         time_t st_ctime;
         };
      #pragma pack( )

      #define S_IFDIR         0x4000         /* directory                     */
      #define S_IFCHR         0x2000         /* character device              */
      #define S_IFREG         0x8000         /* regular file                  */
      #define S_IREAD         0x0100         /* read permission               */
      #define S_IWRITE        0x0080         /* write permission              */
      #define S_IEXEC         0x0040         /* execute/search permission     */

      int _IMPORT _LNK_CONV _fstat( int, struct stat * );
      int _IMPORT _LNK_CONV _stat( const char *, struct stat * );

      int _IMPORT _LNK_CONV  fstat( int, struct stat * );
      int _IMPORT _LNK_CONV  stat( const char *, struct stat * );

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( stat , "?_stat"  )
            #pragma map( fstat, "?_fstat" )
         #else
            #pragma map( stat , "_stat"  )
            #pragma map( fstat, "_fstat" )
         #endif
      #else
         #define stat(pathname, buffer) _stat(pathname, buffer)
         #define fstat(handle, buffer) _fstat(handle, buffer)
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