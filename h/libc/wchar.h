#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __wchar_h
   #define __wchar_h

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
   /*  <wchar.h> header file                                           */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #include <stdio.h>
   #include <stdarg.h>
   #include <wctype.h>

   #ifndef NULL
      #ifdef __cplusplus
         #define NULL 0
      #else
         #define NULL ((void *)0)
      #endif
   #endif

   #ifndef __mbstate_t
      #define __mbstate_t
      typedef short      mbstate_t;
   #endif

   #ifndef __size_t
      typedef unsigned int size_t;
      #define __size_t 1
   #endif

   #ifndef __wchar_t
      typedef unsigned short wchar_t;
      #define __wchar_t 1
   #endif

   #ifndef __wint_t
      #define __wint_t
      typedef int        wint_t;
   #endif

   #ifndef WEOF
      #define WEOF -1
   #endif

   struct tm;

   /* Wide character I/O functions */

   wint_t    _IMPORT _LNK_CONV fgetwc(FILE *);
   wchar_t * _IMPORT _LNK_CONV fgetws(wchar_t *, int, FILE *);
   wint_t    _IMPORT _LNK_CONV fputwc(wchar_t, FILE *);
   int       _IMPORT _LNK_CONV fputws(const wchar_t *, FILE *);
   wint_t    _IMPORT _LNK_CONV getwc(FILE *);
   wint_t    _IMPORT _LNK_CONV getwchar(void);
   wint_t    _IMPORT _LNK_CONV putwc(wchar_t, FILE *);
   wint_t    _IMPORT _LNK_CONV putwchar(wchar_t);
   wint_t    _IMPORT _LNK_CONV ungetwc(wint_t, FILE *);

   double            _IMPORT _LNK_CONV wcstod(const wchar_t *, wchar_t **);
   long int          _IMPORT _LNK_CONV wcstol(const wchar_t *, wchar_t **, int);
   unsigned long int _IMPORT _LNK_CONV wcstoul(const wchar_t *, wchar_t **, int);

   /* Wide Character String handling */

   wchar_t *  _IMPORT _LNK_CONV wcscat (wchar_t *, const wchar_t *);
   wchar_t *  _IMPORT _LNK_CONV wcschr (const wchar_t *, wchar_t);
   int        _IMPORT _LNK_CONV wcscmp (const wchar_t *, const wchar_t *);
   wchar_t *  _IMPORT _LNK_CONV wcscpy (wchar_t *, const wchar_t *);
   size_t     _IMPORT _LNK_CONV wcslen (const wchar_t *);
   wchar_t *  _IMPORT _LNK_CONV wcsncat (wchar_t *, const wchar_t *, size_t);
   int        _IMPORT _LNK_CONV wcsncmp (const wchar_t *, const wchar_t *, size_t);
   wchar_t *  _IMPORT _LNK_CONV wcsncpy (wchar_t *, const wchar_t *, size_t);
   wchar_t *  _IMPORT _LNK_CONV wcsrchr (const wchar_t *, wchar_t);

   int       _IMPORT _LNK_CONV wcscoll(const wchar_t *, const wchar_t *);
   size_t    _IMPORT _LNK_CONV wcsxfrm(wchar_t *, const wchar_t *, size_t);
   size_t    _IMPORT _LNK_CONV wcscspn (const wchar_t *, const wchar_t *);
   wchar_t * _IMPORT _LNK_CONV wcspbrk (const wchar_t *, const wchar_t *);
   size_t    _IMPORT _LNK_CONV wcsspn (const wchar_t *, const wchar_t *);
   wchar_t * _IMPORT _LNK_CONV wcsstr (const wchar_t *, const wchar_t *);
   wchar_t * _IMPORT _LNK_CONV wcswcs (const wchar_t *, const wchar_t *);
   #ifdef _XPG_LEGACY_
      #define wcstok _wcstok_xpg
      #define wcsftime _wcsftime_xpg
      wchar_t * _IMPORT _LNK_CONV wcstok (wchar_t *, const wchar_t  *);
      size_t    _IMPORT _LNK_CONV wcsftime(wchar_t *, size_t, 
                                           const char *, const struct tm *);
   #else
      wchar_t * _IMPORT _LNK_CONV wcstok (wchar_t *, const wchar_t *, wchar_t **);
      size_t    _IMPORT _LNK_CONV wcsftime(wchar_t *, size_t, 
                                           const wchar_t *, const struct tm *);
   #endif
   int       _IMPORT _LNK_CONV wcswidth(const wchar_t *, size_t);
   int       _IMPORT _LNK_CONV wcwidth(const wchar_t);

   int       _IMPORT _LNK_CONV mbsinit(const mbstate_t *);
   size_t    _IMPORT _LNK_CONV mbrlen (const char *, size_t, mbstate_t *);
   size_t    _IMPORT _LNK_CONV mbrtowc (wchar_t *, const char *, size_t, mbstate_t *);
   size_t    _IMPORT _LNK_CONV wcrtomb (char *, wchar_t, mbstate_t *);
   size_t    _IMPORT _LNK_CONV mbsrtowcs (wchar_t *, const char **, size_t, mbstate_t *);
   size_t    _IMPORT _LNK_CONV wcsrtombs (char *, const wchar_t **, size_t, mbstate_t *);
   int       _IMPORT _LNK_CONV wctob (wint_t);

   #if (defined(__EXTENDED__)  || defined( __cplusplus ))
      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( vswprintf,"?_vswprintfieee" )
            #pragma map( swprintf, "?_swprintfieee" )
            #pragma map( swscanf,  "?_swscanfieee" )
         #else
            #pragma map( vswprintf,"_vswprintfieee" )
            #pragma map( swprintf, "_swprintfieee" )
            #pragma map( swscanf,  "_swscanfieee" )
         #endif
      #else
         #define vswprintf _vswprintfieee
         #define swprintf _swprintfieee
         #define swscanf _swscanfieee
      #endif
   #endif
   int       _IMPORT _LNK_CONV vswprintf(wchar_t *, size_t, const wchar_t *, va_list);
   int       _IMPORT _LNK_CONV swprintf(wchar_t *, size_t, const wchar_t *, ...);
   int       _IMPORT _LNK_CONV swscanf(const wchar_t *, const wchar_t *, ...);

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

