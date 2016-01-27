#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __math_h
   #define __math_h

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
   /*  <math.h> header file                                            */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   extern const double _infinity;
   #define HUGE_VAL    _infinity

   /* function prototypes */

   #if __IBMC__ || __IBMCPP__
      #ifdef __cplusplus
         extern "builtin" double __fabs( double );
         inline double _LNK_CONV fabs( double x ) { return __fabs( x ); }
      #else
         double _Builtin __fabs( double );
         double _IMPORT _LNK_CONV fabs( double );
         #define fabs( x ) __fabs( (x) )
      #endif
   #else
      extern double _IMPORT _LNK_CONV fabs( double );
   #endif

   extern double _IMPORT _LNK_CONV ceil( double );
   extern double _IMPORT _LNK_CONV cosh( double );
   extern double _IMPORT _LNK_CONV floor( double );
   extern double _IMPORT _LNK_CONV fmod( double, double );
   extern double _IMPORT _LNK_CONV frexp( double, int * );
   extern double _IMPORT _LNK_CONV ldexp( double, int );
   extern double _IMPORT _LNK_CONV modf( double, double * );
   extern double _IMPORT _LNK_CONV pow( double, double );
   extern double _IMPORT _LNK_CONV sinh( double );
   extern double _IMPORT _LNK_CONV tanh( double );

   #if __THW_PPC__

      #if (__IBMC__ || __IBMCPP__) && __MATH__
         #ifdef __cplusplus
            extern "builtin" double __sin(double);
            extern "builtin" double __cos(double);
            extern "builtin" double __tan(double);
            extern "builtin" double __atan(double);
            extern "builtin" double __acos(double);
            extern "builtin" double __asin(double);
            extern "builtin" double __sqrt(double);

            extern "builtin" double __exp(double);
            extern "builtin" double __log(double);
            extern "builtin" double __log10(double);
            extern "builtin" double __atan2(double, double);

            inline double acos(double __x) {return __acos(__x);}
            inline double asin(double __x) {return __asin(__x);}
            inline double atan(double __x) {return __atan(__x);}
            inline double atan2(double __x, double __y) {return __atan2(__x,__y);}
            inline double cos(double __x) {return __cos(__x);}
            inline double exp(double __x) {return __exp(__x);}
            inline double log(double __x) {return __log(__x);}
            inline double log10(double __x) {return __log10(__x);}
            inline double sin(double __x) {return __sin(__x);}
            inline double sqrt(double __x) {return __sqrt(__x);}
            inline double tan(double __x) {return __tan(__x);}
         #else
            extern double _Builtin __sin(double);
            extern double _Builtin __cos(double);
            extern double _Builtin __tan(double);
            extern double _Builtin __atan(double);
            extern double _Builtin __acos(double);
            extern double _Builtin __asin(double);
            extern double _Builtin __sqrt(double);

            extern double _Builtin __exp(double);
            extern double _Builtin __log(double);
            extern double _Builtin __log10(double);
            extern double _Builtin __atan2(double, double);

            #define acos(__x)         __acos(__x)
            #define asin(__x)         __asin(__x)
            #define atan(__x)         __atan(__x)
            #define atan2(__x,__y)    __atan2(__x,__y)
            #define cos(__x)          __cos(__x)
            #define exp(__x)          __exp(__x)
            #define log(__x)          __log(__x)
            #define log10(__x)        __log10(__x)
            #define sin(__x)          __sin(__x)
            #define sqrt(__x)         __sqrt(__x)
            #define tan(__x)          __tan(__x)
         #endif
      #else
         extern double _IMPORT sin(double);
         extern double _IMPORT cos(double);
         extern double _IMPORT tan(double);
         extern double _IMPORT atan(double);
         extern double _IMPORT acos(double);
         extern double _IMPORT asin(double);
         extern double _IMPORT sqrt(double);

         extern double _IMPORT exp(double);
         extern double _IMPORT log(double);
         extern double _IMPORT log10(double);
         extern double _IMPORT atan2(double, double);
      #endif

   #else /* __THW_INTEL__ */

      double _IMPORT _LNK_CONV atan2( double, double );
      double _IMPORT _LNK_CONV exp( double );
      double _IMPORT _LNK_CONV log( double );
      double _IMPORT _LNK_CONV log10( double );

      #if (__IBMC__ || __IBMCPP__) && defined( _FP_INLINE )
         #ifdef __cplusplus
            extern "builtin" double __fsin ( double );
            extern "builtin" double __fcos ( double );
            extern "builtin" double __fptan ( double );
            extern "builtin" double __fpatan ( double );
            extern "builtin" double __facos ( double );
            extern "builtin" double __fasin ( double );
            extern "builtin" double __fsqrt ( double );

            inline double _LNK_CONV sin ( double x ) { return __fsin  ( x ); }
            inline double _LNK_CONV cos ( double x ) { return __fcos  ( x ); }
            inline double _LNK_CONV tan ( double x ) { return __fptan ( x ); }
            inline double _LNK_CONV atan( double x ) { return __fpatan( x ); }
            inline double _LNK_CONV acos( double x ) { return __facos ( x ); }
            inline double _LNK_CONV asin( double x ) { return __fasin ( x ); }
            inline double _LNK_CONV sqrt( double x ) { return __fsqrt ( x ); }
         #else
            extern double _Builtin __fsin ( double );
            extern double _Builtin __fcos ( double );
            extern double _Builtin __fptan ( double );
            extern double _Builtin __fpatan ( double );
            extern double _Builtin __facos ( double );
            extern double _Builtin __fasin ( double );
            extern double _Builtin __fsqrt ( double );

            #define sin( x )       __fsin( (x) )
            #define cos( x )       __fcos( (x) )
            #define tan( x )       __fptan( (x) )
            #define atan( x )      __fpatan( (x) )
            #define acos( x )      __facos( (x) )
            #define asin( x )      __fasin( (x) )
            #define sqrt( x )      __fsqrt( (x) )
         #endif
      #else
         extern double _IMPORT _LNK_CONV asin( double );
         extern double _IMPORT _LNK_CONV acos( double );
         extern double _IMPORT _LNK_CONV atan( double );
         extern double _IMPORT _LNK_CONV sin( double );
         extern double _IMPORT _LNK_CONV cos( double );
         extern double _IMPORT _LNK_CONV tan( double );
         extern double _IMPORT _LNK_CONV sqrt( double );
      #endif

   #endif

   #ifndef __ANSI__
      extern double _IMPORT _LNK_CONV _erf( double );
      extern double _IMPORT _LNK_CONV _erfc( double );
      extern double _IMPORT _LNK_CONV _gamma( double );
      extern double _IMPORT _LNK_CONV _hypot( double, double );
      extern double _IMPORT _LNK_CONV _j0( double );
      extern double _IMPORT _LNK_CONV _j1( double );
      extern double _IMPORT _LNK_CONV _jn( int, double );
      extern double _IMPORT _LNK_CONV _y0( double );
      extern double _IMPORT _LNK_CONV _y1( double );
      extern double _IMPORT _LNK_CONV _yn( int, double );
      extern double _IMPORT _LNK_CONV erf( double );
      extern double _IMPORT _LNK_CONV erfc( double );
      extern double _IMPORT _LNK_CONV gamma( double );
      extern double _IMPORT _LNK_CONV hypot( double, double );
      extern double _IMPORT _LNK_CONV j0( double );
      extern double _IMPORT _LNK_CONV j1( double );
      extern double _IMPORT _LNK_CONV jn( int, double );
      extern double _IMPORT _LNK_CONV y0( double );
      extern double _IMPORT _LNK_CONV y1( double );
      extern double _IMPORT _LNK_CONV yn( int, double );
      #if __THW_PPC__
         extern int _ExtendedtoLD( __unaligned long double *ldptr );
         extern int _LDtoExtended( __unaligned long double *ldptr );
      #endif

      #if __IBMC__ || __IBMCPP__
         #if __WINDOWS__ && __THW_INTEL__
            #pragma map( erf, "?_erf" )
            #pragma map( erfc, "?_erfc" )
            #pragma map( gamma, "?_gamma" )
            #pragma map( hypot, "?_hypot" )
            #pragma map( j0, "?_j0" )
            #pragma map( j1, "?_j1" )
            #pragma map( jn, "?_jn" )
            #pragma map( y0, "?_y0" )
            #pragma map( y1, "?_y1" )
            #pragma map( yn, "?_yn" )
         #else
            #pragma map( erf, "_erf" )
            #pragma map( erfc, "_erfc" )
            #pragma map( gamma, "_gamma" )
            #pragma map( hypot, "_hypot" )
            #pragma map( j0, "_j0" )
            #pragma map( j1, "_j1" )
            #pragma map( jn, "_jn" )
            #pragma map( y0, "_y0" )
            #pragma map( y1, "_y1" )
            #pragma map( yn, "_yn" )
         #endif
      #else
         #define erf _erf
         #define erfc _erfc
         #define gamma _gamma
         #define hypot _hypot
         #define j0 _j0
         #define j1 _j1
         #define jn _jn
         #define y0 _y0
         #define y1 _y1
         #define yn _yn
      #endif

      #ifndef __SAA_L2__
         extern const long double _LHUGE_VAL;
         #define _LHUGE _LHUGE_VAL
      #endif

      #if defined(__EXTENDED__)

         #define HUGE   HUGE_VAL

         #define DOMAIN           1       /* argument domain error */
         #define SING             2       /* argument singularity */
         #define OVERFLOW         3       /* overflow range error */
         #define UNDERFLOW        4       /* underflow range error */
         #define TLOSS            5       /* total loss of precision */
         #define PLOSS            6       /* partial loss of precision */
         #define UNKNOWN          7       /* unknown error probably caused by */
                                          /* changing the 80387 control word */

         #if (!defined(__cplusplus) || defined(__C_complex))
            struct complex
               {
               double x,y;             /* real and imaginary parts */
               };

            double      _IMPORT _LNK_CONV _cabs( struct complex );
         #endif

         #define _cabs( z ) _hypot( (z).x, (z).y )
         #define cabs( a ) _cabs( a )

         struct exception
            {
            int type;               /* exception type - see below */
            char *name;             /* name of function where error occured */
            double arg1;            /* first argument to function */
            double arg2;            /* second argument (if any) to function */
            double retval;          /* value to be returned by function */
            };

         /* _matherr is defined by the user */

         int _matherr( struct exception * );
         int  matherr( struct exception * );

         #if __IBMC__ || __IBMCPP__
            #if __WINDOWS__ && __THW_INTEL__
               #pragma map( matherr, "?_matherr" )
            #else
               #pragma map( matherr, "_matherr" )
            #endif
         #else
            #define matherr _matherr
         #endif

         long double _LNK_CONV _atold( const char * );

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
