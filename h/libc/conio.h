#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __conio_h
   #define __conio_h

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
   /*  <conio.h> header file                                           */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if defined(__EXTENDED__)

      extern char * _IMPORT _LNK_CONV _cgets( char * );
      extern int    _IMPORT _LNK_CONV _cprintf( const char *, ... );
      extern int    _IMPORT _LNK_CONV _cputs( const char * );
      extern int    _IMPORT _LNK_CONV _cscanf( const char *, ... );
      extern int    _IMPORT _LNK_CONV _getch( void );
      extern int    _IMPORT _LNK_CONV _getche( void );
      extern int    _IMPORT _LNK_CONV _kbhit( void );
      extern int    _IMPORT _LNK_CONV _putch( int );
      extern int    _IMPORT _LNK_CONV _ungetch( int );

      #if __THW_INTEL__ && (__IBMC__ || __IBMCPP__)
         int            _Builtin __inpb( const unsigned int );
         unsigned short _Builtin __inpw( const unsigned int );
         unsigned long  _Builtin __inpd( const unsigned int );
         int            _Builtin __outpb( const unsigned int, const int );
         unsigned short _Builtin __outpw( const unsigned int, const unsigned short );
         unsigned long  _Builtin __outpd( const unsigned int, const unsigned long );

         #define _inp( x )       __inpb( x )
         #define _inpw( x )      __inpw( x )
         #define _inpd( x )      __inpd( x )
         #define _outp( x, y )   __outpb( (x), (y) )
         #define _outpw( x, y )  __outpw( (x), (y) )
         #define _outpd( x, y )  __outpd( (x), (y) )
      #endif

      #define cgets( a )       _cgets( a )
      #define cprintf          _cprintf
      #define cputs( a )       _cputs( a )
      #define cscanf           _cscanf
      #define getch            _getch
      #define getche           _getche
      #define kbhit            _kbhit
      #define putch( a )       _putch( a )
      #define ungetch( a )     _ungetch( a )

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