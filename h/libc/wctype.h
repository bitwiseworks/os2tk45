#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __wctype_h
   #define __wctype_h

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
   /*  <wctype.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #include <stdio.h>
   #include <stdarg.h>

   #ifndef __wchar_t
      #define __wchar_t
      typedef unsigned short wchar_t;
   #endif

   #ifndef __wctype_t
      #define __wctype_t
      typedef unsigned int        wctype_t;
   #endif

   #ifndef __wint_t
      #define __wint_t
      typedef int        wint_t;
   #endif

   #ifndef WEOF
      #define WEOF -1
   #endif

   extern int      _IMPORT _LNK_CONV iswalnum (wint_t);
   extern int      _IMPORT _LNK_CONV iswalpha (wint_t);
   #ifdef __EXTENDED__
      extern int   _IMPORT _LNK_CONV iswblank (wint_t);
   #endif
   extern int      _IMPORT _LNK_CONV iswcntrl (wint_t);
   extern int      _IMPORT _LNK_CONV iswdigit (wint_t);
   extern int      _IMPORT _LNK_CONV iswgraph (wint_t);
   extern int      _IMPORT _LNK_CONV iswlower (wint_t);
   extern int      _IMPORT _LNK_CONV iswprint (wint_t);
   extern int      _IMPORT _LNK_CONV iswpunct (wint_t);
   extern int      _IMPORT _LNK_CONV iswspace (wint_t);
   extern int      _IMPORT _LNK_CONV iswupper (wint_t);
   extern int      _IMPORT _LNK_CONV iswxdigit(wint_t);
   extern wint_t   _IMPORT _LNK_CONV towlower (wint_t);
   extern wint_t   _IMPORT _LNK_CONV towupper (wint_t);
   extern wctype_t _IMPORT _LNK_CONV wctype   (const char *);
   extern int      _IMPORT _LNK_CONV iswctype (wint_t, wctype_t);

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

