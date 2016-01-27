#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __float_h
   #define __float_h

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
   /*  <float.h> header file                                           */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #define    FLT_ROUNDS            1   /* 0, 2 and 3 also available on 80387 */

   #define    FLT_RADIX             2

   #define    FLT_MANT_DIG         24
   #define    DBL_MANT_DIG         53

   #define    FLT_DIG               6
   #define    DBL_DIG              15

   #define    FLT_MIN_EXP      (-125)
   #define    DBL_MIN_EXP     (-1021)

   #define    FLT_MIN_10_EXP    (-37)
   #define    DBL_MIN_10_EXP   (-307)

   #define    FLT_MAX_EXP         128
   #define    DBL_MAX_EXP        1024

   #define    FLT_MAX_10_EXP       38
   #define    DBL_MAX_10_EXP      308

   #define    FLT_MAX         3.402823466e+38F

   #define    FLT_EPSILON     1.192092896e-07F
   #define    DBL_EPSILON     2.2204460492503131e-016

   #define    FLT_MIN         1.175494351e-38F
   #define    DBL_MIN         2.2250738585072014e-308

   #if __THW_PPC__
      #define DBL_MAX            1.7976931348623157e+308
      #define LDBL_DIG           30
      #define LDBL_MANT_DIG      106
      #define LDBL_MIN_EXP       DBL_MIN_EXP
      #define LDBL_MIN           ((long double)DBL_MIN)
      #define LDBL_MIN_10_EXP    DBL_MIN_10_EXP
      #define LDBL_MAX_EXP       DBL_MAX_EXP
      #define LDBL_MAX           0.1797693134862315807937289714053023E+309L
      #define LDBL_MAX_10_EXP    DBL_MAX_10_EXP
      #define LDBL_EPSILON       0.24651903288156618919116517665087070E-31L
   #else /* __THW_INTEL__ */
      #define DBL_MAX            1.7976931348623158e+308
      #define LDBL_DIG           18
      #define LDBL_MANT_DIG      64
      #define LDBL_MIN_EXP       (-16381)
      #define LDBL_MIN           3.36210314311209350626e-4932L
      #define LDBL_MIN_10_EXP    (-4931)
      #define LDBL_MAX_EXP       16384
      #define LDBL_MAX           1.18973149535723176499e+4932L
      #define LDBL_MAX_10_EXP    4932
      #define LDBL_EPSILON       1.08420217248550443401e-19L
   #endif

   #if defined(__EXTENDED__)

      #if __THW_PPC__
         /* FPSCR information on PowerPC */

         /* A fpscr_t contains a full FPSCR word which includes */
         /* exception bits, exception enable bits and rounding  */
         /* control bits.                                       */
         typedef unsigned int fpscr_t;

         /* Exception mask in the FPSCR control portion */

         #define MCW_EM          ((fpscr_t)0x000000FC) /* enable bit summ.    */
         #define EM_INVALID      ((fpscr_t)0x00000080) /* invalid, bit 24     */
         #define EM_OVERFLOW     ((fpscr_t)0x00000040) /* overflow, bit 25    */
         #define EM_UNDERFLOW    ((fpscr_t)0x00000020) /* underflow, bit 26   */
         #define EM_ZERODIVIDE   ((fpscr_t)0x00000010) /* zero divide, bit 27 */
         #define EM_INEXACT      ((fpscr_t)0x00000008) /* inexact, bit 28     */
         #define EM_DENORMAL     ((fpscr_t)0x00000000) /* denormal, not supported */

         #define MCW_IC          ((fpscr_t) 0x00000000) /* Infinity Control    */
         #define IC_AFFINE       ((fpscr_t) 0x00000000) /* meaningful to PPC.  */
         #define IC_PROJECTIVE   ((fpscr_t) 0x00000000) /* regardless of its   */
                                   /* value, PPC treats infinity in the affine */
                                   /* sense:  -INF < finite number < +INF      */

         #define MCW_RC          ((fpscr_t) 0x00000001) /* Rounding Control   */
         #define RC_CHOP         ((fpscr_t) 0x00000001) /* toward zero (chop) */
         #define RC_UP           ((fpscr_t) 0x00000002) /* +infinity (up)     */
         #define RC_DOWN         ((fpscr_t) 0x00000003) /* -infinity (down)   */
         #define RC_NEAR         ((fpscr_t) 0x00000000) /* nearest            */

         #define MCW_PC ((fpscr_t)0x00000000) /* Precision control is not     */
         #define PC_24  ((fpscr_t)0x00000000) /* meanningful to PPC. To       */
         #define PC_53  ((fpscr_t)0x00000000) /* maintain the compatibility   */
         #define PC_64  ((fpscr_t)0x00000000) /* they can be programmed.      */
                                              /* However, regardless of its   */
                                              /* value, FP operations are     */
                                              /* done in 64 bits.             */

         /* initial Control Word value. */
         #define CW_DEFAULT  RC_NEAR
      
      
         /* #define CW_DEFAULT (RC_CHOP + EM_DENORMAL + EM_OVERFLOW + \
                             EM_UNDERFLOW + EM_ZERODIVIDE)
         Enabling these exceptions will not generate a real exception.
         To generate real exceptions the compiler option:
             -qflttrap= ov for detecting overflow 
                        und for detecting underflow
                        zero to detect zero divide 
                        inv to detect invalid operation exceptions
                        inex to detect floating point inexact exceptions
         must be used */
      

         /* Status Word portion in FPSCR */

         #define SW_FX_SUM       ((fpscr_t) 0x80000000) /* exception summary  */
         #define SW_INVALID      ((fpscr_t) 0x20000000) /* invalid, bit 2     */
         #define SW_DENORMAL     ((fpscr_t) 0x00000000) /* denormal, N/A      */
         #define SW_ZERODIVIDE   ((fpscr_t) 0x04000000) /* zero divide, bit 5 */
         #define SW_OVERFLOW     ((fpscr_t) 0x10000000) /* overflow, bit 3    */
         #define SW_UNDERFLOW    ((fpscr_t) 0x08000000) /* underflow, bit 4   */
         #define SW_INEXACT      ((fpscr_t) 0x02000000) /* inexact, bit 6     */
         #define SW_STACKFAULT   ((fpscr_t) 0x00000000) /* stack fault, N/A   */

         #define SW_STACKOVERFLOW ((fpscr_t) 0x00000000) /* stack overflow, N/A */
      #else /* __THW_INTEL__ */
         /* 80387 math control information */

         /* User Control Word Mask and bit definitions. */
         /* These definitions match the 80387 */

         #define MCW_EM          0x003f          /* interrupt Exception Masks */
         #define EM_INVALID      0x0001          /*  invalid */
         #define EM_DENORMAL     0x0002          /*  denormal */
         #define EM_ZERODIVIDE   0x0004          /*  zero divide */
         #define EM_OVERFLOW     0x0008          /*  overflow */
         #define EM_UNDERFLOW    0x0010          /*  underflow */
         #define EM_INEXACT      0x0020          /*  inexact (precision) */

         #define MCW_IC          0x1000          /* Infinity Control */
         #define IC_AFFINE       0x1000          /*  affine */
         #define IC_PROJECTIVE   0x0000          /*  projective */

         #define MCW_RC          0x0c00          /* Rounding Control */
         #define RC_CHOP         0x0c00          /*  chop */
         #define RC_UP           0x0800          /*  up */
         #define RC_DOWN         0x0400          /*  down */
         #define RC_NEAR         0x0000          /*  near */

         #define MCW_PC          0x0300          /* Precision Control */
         #define PC_24           0x0000          /*  24 bits */
         #define PC_53           0x0200          /*  53 bits */
         #define PC_64           0x0300          /*  64 bits */

         /* initial Control Word value */

         #define CW_DEFAULT ( RC_NEAR + PC_64 + EM_DENORMAL + EM_INEXACT )

         /* user Status Word bit definitions */

         #define SW_INVALID      0x0001          /* invalid */
         #define SW_DENORMAL     0x0002          /* denormal */
         #define SW_ZERODIVIDE   0x0004          /* zero divide */
         #define SW_OVERFLOW     0x0008          /* overflow */
         #define SW_UNDERFLOW    0x0010          /* underflow */
         #define SW_INEXACT      0x0020          /* inexact (precision) */
         #define SW_STACKFAULT   0x0040          /* stack fault (SW_INVALID also set) */

         /* stack problems (SW_INVALID and SW_STACKFAULT both set) */
         #define SW_STACKOVERFLOW        0x0200          /* stack overflow */
                                                      /* bit is off for stack underflow */
      #endif

      /*  Floating point error signals and return codes */

      #define FPE_INTDIV0             0x91            /* integer division by 0 */
      #define FPE_INT_OFLOW           0x92            /* integer overflow */
      #define FPE_BOUND               0x93            /* bounds failure */
      #define FPE_INVALID             0x81
      #define FPE_DENORMAL            0x82
      #define FPE_ZERODIVIDE          0x83
      #define FPE_OVERFLOW            0x84
      #define FPE_UNDERFLOW           0x85
      #define FPE_INEXACT             0x86

      #define FPE_STACKOVERFLOW       0x8a
      #define FPE_STACKUNDERFLOW      0x8b

      #define FPE_EXPLICITGEN         0x8c    /* raise( SIGFPE ); */

      /* function prototypes */

      #if __THW_PPC__

         extern unsigned int  _clear87( void );
         extern unsigned int  _control87( unsigned int, unsigned int );
         extern unsigned int  _status87( void );
         extern void          _fpreset( void );

         extern const float   _fposinfi;
         #define _INFF        _fposinfi
         #define _INFINITYF   _fposinfi

         extern const double _infinity;
         #define _INF        _infinity
         #define _INFINITY   _infinity

         extern const long double _ldposinfi;
         #define _INFL       _ldposinfi
         #define _INFINITYL  _ldposinfi

         extern const float       _fposqnan;
         extern const double      _dposqnan;
         extern const long double _ldposqnan;

         #define _NANF      _fposqnan
         #define _NAN       _dposqnan
         #define _NANL      _ldposqnan

      #else /* __THW_INTEL__ */

         extern unsigned int (_IMPORT _LNK_CONV _clear87)( void );
         extern unsigned int (_IMPORT _LNK_CONV _control87)( unsigned int, unsigned int );
         extern unsigned int (_IMPORT _LNK_CONV _status87)( void );
         extern void          _IMPORT _LNK_CONV _fpreset( void );

         #if __IBMC__ || __IBMCPP__
            unsigned int _Builtin __clear87( void );
            unsigned int _Builtin __control87( unsigned int, unsigned int );
            unsigned int _Builtin __status87( void );

            #define _clear87( )             __clear87( )
            #define _control87( x, y )      __control87( (x), (y) )
            #define _status87( )            __status87( )
         #endif  

         extern const float  _IMPORT _fposinfi;
         #define _INFF       _fposinfi
         #define _INFINITYF  _fposinfi

         extern const double _IMPORT _infinity;
         #define _INF        _infinity
         #define _INFINITY   _infinity

         extern const long double _IMPORT _ldposinfi;
         #define _INFL            _ldposinfi
         #define _INFINITYL       _ldposinfi

         extern const float       _IMPORT _fposqnan;
         extern const double      _IMPORT _dposqnan;
         extern const long double _IMPORT _ldposqnan;

         #define _NANF      _fposqnan
         #define _NAN       _dposqnan
         #define _NANL      _ldposqnan
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

