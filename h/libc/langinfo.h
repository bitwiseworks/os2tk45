#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef     _H_LANGINFO
   #define _H_LANGINFO

   #include <nl_types.h>

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
   /*  <langinfo.h> header file                                        */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   extern char * _IMPORT _LNK_CONV nl_langinfo(nl_item item);

   #ifndef _ULS_LANGINFO  
      /* These definitions are duplicated in ulsitem.h. */
   
      #define D_T_FMT    1  /* string for formatting date and time */
      #define D_FMT      2  /* string for formatting date */
      #define T_FMT      3  /* string for formatting time */
      #define AM_STR     4  /* string for a.m. */
      #define PM_STR     5  /* string for p.m. */
      
      #define ABDAY_1    6  /* abbreviated first day of the week (Sun) */
      #define ABDAY_2    7  /* abbreviated second day of the week (Mon) */
      #define ABDAY_3    8  /* abbreviated third day of the week (Tue) */
      #define ABDAY_4    9  /* abbreviated fourth day of the week (Wed) */
      #define ABDAY_5   10  /* abbreviated fifth day of the week (Thu) */
      #define ABDAY_6   11  /* abbreviated sixth day of the week (Fri) */
      #define ABDAY_7   12  /* abbreviated seventh day of the week (Sat) */
      
      #define DAY_1     13  /* name of the first day of the week (Sunday) */
      #define DAY_2     14  /* name of the second day of the week (Monday) */
      #define DAY_3     15  /* name of the third day of the week (Tuesday) */
      #define DAY_4     16  /* name of the fourth day of the week (Wednesday) */
      #define DAY_5     17  /* name of the fifth day of the week (Thursday) */
      #define DAY_6     18  /* name of the sixth day of the week (Friday) */
      #define DAY_7     19  /* name of the seventh day of the week (Saturday) */
      
      #define ABMON_1   20  /* abbreviated first month (Jan) */
      #define ABMON_2   21  /* abbreviated second month (Feb) */
      #define ABMON_3   22  /* abbreviated third month (Mar) */
      #define ABMON_4   23  /* abbreviated fourth month (Apr) */
      #define ABMON_5   24  /* abbreviated fifth month (May) */
      #define ABMON_6   25  /* abbreviated sixth month (Jun) */
      #define ABMON_7   26  /* abbreviated seventh month (Jul) */
      #define ABMON_8   27  /* abbreviated eighth month (Aug) */
      #define ABMON_9   28  /* abbreviated ninth month (Sep) */
      #define ABMON_10  29  /* abbreviated tenth month (Oct) */
      #define ABMON_11  30  /* abbreviated eleventh month (Nov) */
      #define ABMON_12  31  /* abbreviated twelveth month (Dec) */
      
      #define MON_1     32  /* name of the first month (January) */
      #define MON_2     33  /* name of the second month (February) */
      #define MON_3     34  /* name of the third month (March) */
      #define MON_4     35  /* name of the fourth month (April) */
      #define MON_5     36  /* name of the fifth month (May) */
      #define MON_6     37  /* name of the sixth month (June) */
      #define MON_7     38  /* name of the seventh month (July) */
      #define MON_8     39  /* name of the eighth month (August) */
      #define MON_9     40  /* name of the ninth month (September) */
      #define MON_10    41  /* name of the tenth month (October) */
      #define MON_11    42  /* name of the eleventh month (November) */
      #define MON_12    43  /* name of the twelveth month (December) */
      
      #define RADIXCHAR 44  /* radix character */
      #define THOUSEP   45  /* separator for thousands */
      #define YESSTR    46  /* affiramitive response for yes/no queries */
      #define NOSTR     47  /* negative response for yes/no queries */
      #define CRNCYSTR  48  /* currency symbol; - leading, + trailing */
      #define CODESET   49  /* codeset name */
      
      #define T_FMT_AMPM    55  /* am/pm time formating string */
      #define ERA           56  /* era description segments */
      #define ERA_D_FMT     57  /* era date format string */
      #define ERA_D_T_FMT   58  /* era date and time format string */
      #define ERA_T_FMT     59  /* era time format string */
      #define ALT_DIGITS    60  /* alternative symbols for digits */
      #define YESEXPR       61  /* affirmative response expression */
      #define NOEXPR        62  /* negative response expression */
      
      #define DATESEP       63
      #define TIMESEP       64
      #define LISTSEP       65
      
      #define _NL_NUM_ITEMS 66

   #endif

   #ifdef __cplusplus
      }
   #endif

#endif   /* _H_LANGINFO */

#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( restore )
#endif
#pragma info( restore )
#endif

