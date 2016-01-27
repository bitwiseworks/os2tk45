#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __errno_h
   #define __errno_h

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
   /*  <errno.h> header file                                           */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #ifndef errno
      #if defined __MULTI__ || defined _WIN32S
         extern int * _IMPORT _LNK_CONV _errno( void );
         #define errno (*_errno( ))
      #else
         extern int _IMPORT errno;
         #define errno errno
      #endif
   #endif

   #ifndef _doserrno
      #if defined __MULTI__ || defined _WIN32S
         extern int * _IMPORT _LNK_CONV __doserrno( void );
         #define _doserrno (*__doserrno( ))
      #else
         extern int _IMPORT _doserrno;
         #define _doserrno _doserrno
      #endif
   #endif

   /* error codes */

   #define EDOM      1                 /* domain error                       */
   #define ERANGE    2                 /* range error                        */

   #define EBADMODE        3
   #define EBADNAME        4
   #define EISTEMPMEM      5
   #define EBADSHARE       6
   #define EBUFMODE        7
   #define EERRSET         8
   #define EISOPEN         9
   #define ENOTEXIST      10
   #define ENOTINIT       11
   #define ENULLFCB       12
   #define EOUTOFMEM      13
   #define ESMALLBF       14
   #define EEXIST         16
   #define ENOGEN         17
   #define ENOSEEK        19
   #define EBADPOS        20
   #define EBADSEEK       22
   #define ENOENT         23
   #define EACCESS        24
   #define EMFILE         25
   #define ENOCMD         26
   #define EGETANDPUT     28
   #define EPASTEOF       29
   #define ENOTREAD       30
   #define ETOOMANYUNGETC 31
   #define EUNGETEOF      32
   #define EPUTUNGET      33
   #define ECHILD         34
   #define EINTR          35
   #define EINVAL         36
   #define ENOEXEC        37
   #define EAGAIN         38
   #define EBADTYPE       39
   #define ENOTWRITE      40
   #define EPUTANDGET     41
   #define ELARGEBF       42
   #define EBADF          43
   #define EXDEV          44
   #define ENOSPC         45
   #define EMATH          46
   #define EMODNAME       47
   #define EMAXATTR       49
   #define EREADERROR     50
   #define EBADATTR       51
   #define EILSEQ         52
   #define E2BIG          53

   #define EOS2ERR 60                  /* OS/2 error                         */

   #define ENOMEM  EOUTOFMEM
   #define EACCES  EACCESS

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

