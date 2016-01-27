#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __timeb_h
   #define __timeb_h

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
   /*  <sys\timeb.h> header file                                       */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if defined(__EXTENDED__)

      #ifndef  __time_t
         #define __time_t
         typedef long time_t;
      #endif


      #if ! __THW_PPC__
         #pragma pack( 2 )
      #endif
      struct timeb
         {
         time_t time;
         unsigned short millitm;
         short timezone;
         short dstflag;
         };
      #if ! __THW_PPC__
         #pragma pack( )
      #endif

      extern void _IMPORT _LNK_CONV _ftime( struct timeb * );

      #define ftime(a) _ftime(a)

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

