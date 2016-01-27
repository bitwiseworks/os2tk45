#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef     _H_LOCALE
   #define _H_LOCALE

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
   /*  <locale.h> header file                                          */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #ifndef _ULS_UNIDEF
      /* These definitions are duplicated in unidef.h. */
      #define LC_ALL       (-1)
      #define LC_COLLATE    0
      #define LC_CTYPE      1
      #define LC_NUMERIC    2
      #define LC_MONETARY   3
      #define LC_TIME       4
      #define LC_MESSAGES   5
   #endif

   #define LC_SYNTAX LC_CTYPE
   #define LC_TOD LC_TIME

   /* Locales */

   #define LC_C          "C"
   #define LC_C_GERMANY  "DE_DE"
   #define LC_C_FRANCE   "FR_FR"
   #define LC_C_UK       "EN_GB"
   #define LC_C_ITALY    "IT_IT"
   #define LC_C_SPAIN    "ES_ES"
   #define LC_C_USA      "EN_US"

   #define LC_C_JAPAN    "JA_JP"
   #define LC_C_JAPAN2   "JA_JP"
   #define LC_C_JAPAN3   "JA_JP"
   
   typedef struct lconv {
       char *decimal_point;      /* decimal point character             */
       char *thousands_sep;      /* thousands separator                 */
       char *grouping;           /* digit grouping                      */
       char *int_curr_symbol;    /* international currency symbol       */
       char *currency_symbol;    /* national currency symbol            */
       char *mon_decimal_point;  /* currency decimal point              */
       char *mon_thousands_sep;  /* currency thousands separator        */
       char *mon_grouping;       /* currency digits grouping            */
       char *positive_sign;      /* currency plus sign                  */
       char *negative_sign;      /* currency minus sign                 */
       char int_frac_digits;     /* internat currency fractional digits */
       char frac_digits;         /* currency fractional digits          */
       char p_cs_precedes;       /* currency plus location              */
       char p_sep_by_space;      /* currency plus space ind.            */
       char n_cs_precedes;       /* currency minus location             */
       char n_sep_by_space;      /* currency minus space ind.           */
       char p_sign_posn;         /* currency plus position              */
       char n_sign_posn;         /* currency minus position             */
       char *debit_sign;         /* non-neg-valued monetary sym - "DB"  */
       char *credit_sign;        /* negative-valued monetary sym - "CR" */
       char *left_parenthesis;   /* negative currency left paren        */
       char *right_parenthesis;  /* negative currency right paren       */
   } lconv;
   
   extern char         * _IMPORT _LNK_CONV setlocale( int, const char * );
   extern struct lconv * _IMPORT _LNK_CONV localeconv( void );
   char   *querylocaleenv(void);

   #ifdef __EXTENDED__
      struct dtconv {
            char *abbrev_month_names[12]; /* Abbreviated month names      */
            char *month_names[12];        /* full month names             */
            char *abbrev_day_names[7];    /* Abbreviated day names        */
            char *day_names[7];           /* full day names               */
            char *date_time_format;       /* date and time format         */
            char *date_format;            /* date format                  */
            char *time_format;            /* time format                  */
            char *am_string;              /* AM string                    */
            char *pm_string;              /* PM string                    */
            char *time_format_ampm;       /* long date format             */
      };

      struct dtconv * _IMPORT _LNK_CONV localdtconv(void);
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

