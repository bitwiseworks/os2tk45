#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __ctype_h
   #define __ctype_h

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
   /*  <ctype.h> header file                                           */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   extern int _IMPORT _LNK_CONV isalnum( int );
   extern int _IMPORT _LNK_CONV isalpha( int );
   extern int _IMPORT _LNK_CONV iscntrl( int );
   extern int _IMPORT _LNK_CONV isdigit( int );
   extern int _IMPORT _LNK_CONV isgraph( int );
   extern int _IMPORT _LNK_CONV islower( int );
   extern int _IMPORT _LNK_CONV isprint( int );
   extern int _IMPORT _LNK_CONV ispunct( int );
   extern int _IMPORT _LNK_CONV isspace( int );
   extern int _IMPORT _LNK_CONV isupper( int );
   extern int _IMPORT _LNK_CONV isxdigit( int );
   extern int _IMPORT _LNK_CONV tolower( int );
   extern int _IMPORT _LNK_CONV toupper( int );

   #if defined(__EXTENDED__)

      int _LNK_CONV _tolower( int );
      int _LNK_CONV _toupper( int );
      int _LNK_CONV _isascii( int );
      int _LNK_CONV _iscsymf( int );
      int _LNK_CONV _iscsym( int );
      int _LNK_CONV _toascii( int );

      #define _tolower( c ) ( (c) + 'a' - 'A' )
      #define _toupper( c ) ( (c) + 'A' - 'a' )
      #define _isascii( c ) ( (unsigned)(c) < 0x80 )
      #define _iscsymf( c ) ( isalpha( c ) || (c) == '_' )
      #define _iscsym( c )  ( isalnum( c ) || (c) == '_' )
      #define _toascii( c ) ( (c) & 0x7f )

      extern int _IMPORT _LNK_CONV isblank( int );

      #ifdef __cplusplus
         inline int isascii( int c ) { return _isascii( c ); }
         inline int iscsymf( int c ) { return _iscsymf( c ); }
         inline int iscsym ( int c ) { return _iscsym ( c ); }
         inline int toascii( int c ) { return _toascii( c ); }
      #else
         #define  isascii( c )        _isascii( c )
         #define  iscsymf( c )        _iscsymf( c )
         #define  iscsym( c )         _iscsym( c )
         #define  toascii( c )        _toascii( c )
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

