#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef _ICONV_H
   #define _ICONV_H

   #include <stddef.h>

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
   /*  <iconv.h> header file                                           */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #ifndef __ICONVP_H
      typedef void *iconv_t;
   #endif

   #ifndef CCSID_DEF
      #define CCSID_DEF
      typedef unsigned int       CCSID;
   #endif

   extern iconv_t _IMPORT _LNK_CONV iconv_open(const char *, const char *);
   extern size_t  _IMPORT _LNK_CONV iconv(iconv_t, const char **, size_t *, 
                                          char **, size_t *);
   extern int     _IMPORT _LNK_CONV iconv_close(iconv_t);

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