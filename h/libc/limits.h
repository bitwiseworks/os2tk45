#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __limits_h
   #define __limits_h

   #ifdef __cplusplus
      extern "C" {
   #endif

   /********************************************************************/
   /*  <limits.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #define CHAR_BIT      8

   #define SCHAR_MIN     (-128)
   #define SCHAR_MAX     127
   #define UCHAR_MAX     255

   #ifdef _CHAR_UNSIGNED
      #define CHAR_MIN      0
      #define CHAR_MAX      UCHAR_MAX
   #else
      #define CHAR_MIN      SCHAR_MIN
      #define CHAR_MAX      SCHAR_MAX
   #endif

   #define MB_LEN_MAX    4

   #define NL_ARGMAX     255  /* max number of indexed printf args           */
   #define NL_MSGMAX   65535  /* max number of messages per set              */
   #define NL_SETMAX   65535  /* max number of set per catalog               */
   #define NL_TEXTMAX   8192  /* max message length in bytes                 */
   #define NL_LANGMAX     14  /* max number of of bytes in a LANG name       */
   #define NL_NMAX        10  /* max number of bytes in N-to-1 mapping chars */


   #define SHRT_MIN      (-32768)
   #define SHRT_MAX      32767
   #define USHRT_MAX     65535

   #define INT_MIN       (-2147483647 - 1)
   #define INT_MAX       2147483647
   #define UINT_MAX      0xffffffff          /* 4294967295  */

   #define LONG_MIN      (-2147483647L - 1)
   #define LONG_MAX      2147483647
   #define ULONG_MAX     0xffffffff          /* 4294967295U */

   #define LONGLONG_MIN  0x8000000000000000LL
   #define LONGLONG_MAX  0x7fffffffffffffffLL
   #define ULONGLONG_MAX 0xffffffffffffffffULL

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

