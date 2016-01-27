/*
 * ulsitem.h:
 *
 * Function:
 *     Define items for use with UniQueryLocaleItem and
 *     UniQueryLocaleValue.
 *
 * Copyright:
 *     Copyright (C) International Business Machines Corp. 1989, 1997
 *     All Rights Reserved
 *     Licensed Materials - Property of IBM
 *
 * Notes:
 *     All items can be queried as strings.  Those items with a name
 *     prefix of 'i' or 'x' will return a binary value.
 *
 */

#ifdef _ULS_LANGINFO
#define _ULS_ITEM
#endif

#ifndef _ULS_ITEM
#define _ULS_ITEM
#define _ULS_LANGINFO

#ifdef __cplusplus
   extern "C" {
#endif

typedef int LocaleItem;

#define LOCI_sDateTime            1  /*1 Date and time format string       */
#define LOCI_sShortDate           2  /*1 Short date format                 */
#define LOCI_sTimeFormat          3  /*1 Time format string                */
#define LOCI_s1159                4  /*1 AM string  *                      */
#define LOCI_s2359                5  /*1 PM sring  *                       */
#define LOCI_sAbbrevDayName7      6  /*1 Abbreviation of day 7 (Sun)       */
#define LOCI_sAbbrevDayName1      7  /*1 Abbreviation of day 1 (Mon)       */
#define LOCI_sAbbrevDayName2      8  /*1 Abbreviation of day 2 (Tue)       */
#define LOCI_sAbbrevDayName3      9  /*1 Abbreviation of day 3 (Wed)       */
#define LOCI_sAbbrevDayName4     10  /*1 Abbreviation of day 4 (Thu)       */
#define LOCI_sAbbrevDayName5     11  /*1 Abbreviation of day 5 (Fri)       */
#define LOCI_sAbbrevDayName6     12  /*1 Abbreviation of day 6 (Sat)       */
#define LOCI_sDayName7           13  /*1 Name of day of week 7 (Sun)       */
#define LOCI_sDayName1           14  /*1 Name of day of week 1 (Mon)       */
#define LOCI_sDayName2           15  /*1 Name of day of week 2 (Tue)       */
#define LOCI_sDayName3           16  /*1 Name of day of week 3 (Wed)       */
#define LOCI_sDayName4           17  /*1 Name of day of week 4 (Thu)       */
#define LOCI_sDayName5           18  /*1 Name of day of week 5 (Fri)       */
#define LOCI_sDayName6           19  /*1 Name of day of week 6 (Sat)       */
#define LOCI_sAbbrevMonthName1   20  /*1 Abbreviation of month 1           */
#define LOCI_sAbbrevMonthName2   21  /*1 Abbreviation of month 2           */
#define LOCI_sAbbrevMonthName3   22  /*1 Abbreviation of month 3           */
#define LOCI_sAbbrevMonthName4   23  /*1 Abbreviation of month 4           */
#define LOCI_sAbbrevMonthName5   24  /*1 Abbreviation of month 5           */
#define LOCI_sAbbrevMonthName6   25  /*1 Abbreviation of month 6           */
#define LOCI_sAbbrevMonthName7   26  /*1 Abbreviation of month 7           */
#define LOCI_sAbbrevMonthName8   27  /*1 Abbreviation of month 8           */
#define LOCI_sAbbrevMonthName9   28  /*1 Abbreviation of month 9           */
#define LOCI_sAbbrevMonthName10  29  /*1 Abbreviation of month 10          */
#define LOCI_sAbbrevMonthName11  30  /*1 Abbreviation of month 11          */
#define LOCI_sAbbrevMonthName12  31  /*1 Abbreviation of month 12          */
#define LOCI_sMonthName1         32  /*1 Name of month 1                   */
#define LOCI_sMonthName2         33  /*1 Name of month 2                   */
#define LOCI_sMonthName3         34  /*1 Name of month 3                   */
#define LOCI_sMonthName4         35  /*1 Name of month 4                   */
#define LOCI_sMonthName5         36  /*1 Name of month 5                   */
#define LOCI_sMonthName6         37  /*1 Name of month 6                   */
#define LOCI_sMonthName7         38  /*1 Name of month 7                   */
#define LOCI_sMonthName8         39  /*1 Name of month 8                   */
#define LOCI_sMonthName9         40  /*1 Name of month 9                   */
#define LOCI_sMonthName10        41  /*1 Name of month 10                  */
#define LOCI_sMonthName11        42  /*1 Name of month 11                  */
#define LOCI_sMonthName12        43  /*1 Name of month 12                  */
#define LOCI_sDecimal            44  /*2 Decimal point  *                  */
#define LOCI_sThousand           45  /*2 Triad separator  *                */
#define LOCI_sYesString          46  /*4 Yes string  #                     */
#define LOCI_sNoString           47  /*4 No string  #                      */
#define LOCI_sCurrency           48  /*3 Currency symbol  *                */
#define LOCI_sCodeSet            49  /*0 Locale codeset                    */
#define LOCI_xLocaleToken        50  /*0 IBM Locale Token #                */
#define LOCI_xWinLocale          51  /*0 Win32 Locale ID                   */
#define LOCI_iLocaleResnum       52  /*0 Resource number for description   */
#define LOCI_sNativeDigits       53  /*2 String of native digits           */
#define LOCI_iMaxItem            54  /*0 Maximum item number               */
#define LOCI_sTimeMark           55  /*1 Time mark (am/pm) format          */
#define LOCI_sEra                56  /*1 Era definition                    */
#define LOCI_sAltShortDate       57  /*1 Alternate short date format string*/
#define LOCI_sAltDateTime        58  /*1 Alternate date and time format    */
#define LOCI_sAltTimeFormat      59  /*1 Alternate time format             */
#define LOCI_sAltDigits          60  /*2 XPG4 alternate digist             */
#define LOCI_sYesExpr            61  /*4 xpg4 yes expression  #            */
#define LOCI_sNoExpr             62  /*4 xpg4 no expression  #             */
#define LOCI_sDate               63  /*1 Short date separator  *           */
#define LOCI_sTime               64  /*1 Time separator  *                 */
#define LOCI_sList               65  /*0 List separator  *                 */
   /* New */
#define LOCI_sMonDecimalSep      66  /*3 Monetary currency separator       */
#define LOCI_sMonThousandSep     67  /*3 Monetary triad separator          */
#define LOCI_sGrouping           68  /*2 Grouping of digits                */
#define LOCI_sMonGrouping        69  /*3 Monetary groupings                */
#define LOCI_iMeasure            70  /*0 Measurement (Metric, British)     */
#define LOCI_iPaper              71  /*0 Normal paper size  #              */
#define LOCI_iDigits             72  /*2 Digits to right of decimal  *     */
#define LOCI_iTime               73  /*1 Clock format  *                   */
#define LOCI_iDate               74  /*1 Format of short date  *           */
#define LOCI_iCurrency           75  /*3 Format of currency *              */
#define LOCI_iCurrDigits         76  /*3 Digits to right for currency      */
#define LOCI_iLzero              77  /*2 Leading zero used                 */
#define LOCI_iNegNumber          78  /*2 Format of negative number  *      */
#define LOCI_iLDate              79  /*1 Format of long date               */
#define LOCI_iCalendarType       80  /*1 Type of default calandar          */
#define LOCI_iFirstDayOfWeek     81  /*1 First day of week (0=Mon)         */
#define LOCI_iFirstWeekOfYear    82  /*1 First week of year                */
#define LOCI_iNegCurr            83  /*3 Format of negative currency       */
#define LOCI_iTLzero             84  /*1 Leading zero on time  *           */
#define LOCI_iTimePrefix         85  /*1 AM/PM preceeds time  *            */
#define LOCI_iOptionalCalendar   86  /*1 Alternate calandar type           */
#define LOCI_sIntlSymbol         87  /*3 International currency symbol     */
#define LOCI_sAbbrevLangName     88  /*0 Windows language abbreviation     */
#define LOCI_sCollate            89  /*5 Collation table                   */
#define LOCI_iUpperType          90  /*0 Upper case algorithm              */
#define LOCI_iUpperMissing       91  /*0 Action for missing upper case     */
#define LOCI_sPositiveSign       92  /*2 Positive sign                     */
#define LOCI_sNegativeSign       93  /*2 Negative sign                     */
#define LOCI_sLeftNegative       94  /*4 Left paren for negative           */
#define LOCI_sRightNegative      95  /*4 Right paren for negative          */
#define LOCI_sLongDate           96  /*1 Long date formatting string       */
#define LOCI_sAltLongDate        97  /*1 Alternate long date format string */
#define LOCI_sMonthName13        98  /*1 Name of month 13                  */
#define LOCI_sAbbrevMonthName13  99  /*1 Abbreviation of month 13          */
#define LOCI_sName              100  /*0 OS/2 locale name  #               */
#define LOCI_sLanguageID        101  /*0 Abbreviation for language (ISO)   */
#define LOCI_sCountryID         102  /*0 Abbreviation for country (ISO)    */
#define LOCI_sEngLanguage       103  /*0 English name of Language          */
#define LOCI_sLanguage          104  /*0 Native name of language           */
#define LOCI_sEngCountry        105  /*0 English name of country           */
#define LOCI_sCountry           106  /*0 Localized country name  *         */
#define LOCI_sNativeCtryName    107  /*0 Name of country in native language*/
#define LOCI_iCountry           108  /*0 Country code  *                   */
#define LOCI_sISOCodepage       109  /*0 ISO codepage name                 */
#define LOCI_iAnsiCodepage      110  /*0 Windows codepage                  */
#define LOCI_iCodepage          111  /*0 OS/2 primary codepage             */
#define LOCI_iAltCodepage       112  /*0 OS/2 alternate codepage           */
#define LOCI_iMacCodepage       113  /*0 Mac codepage                      */
#define LOCI_iEbcdicCodepage    114  /*0 Ebcdic codepage                   */                            */
#define LOCI_sOtherCodepages    115  /*0 Other ASCII codepages             */
#define LOCI_sSetCodepage       116  /*0 Codpage to set on activation      */
#define LOCI_sKeyboard          117  /*0 Primary keyboard name             */
#define LOCI_sAltKeyboard       118  /*0 Alternate keyboard name           */
#define LOCI_sSetKeyboard       119  /*0 Keyboard to set on activation     */
#define LOCI_sDebit             120  /*4 Debit string  #                   */
#define LOCI_sCredit            121  /*4 Credit string                     */
#define LOCI_sLatin1Locale      122  /*0 Locale for Latin 1 names          */                      
#define LOCI_wTimeFormat        123  /*1 Win32 Time format                 */
#define LOCI_wShortDate         124  /*1 Win32 Date format                 */
#define LOCI_wLongDate          125  /*1 Win32 Long date format            */

#define LOCI_jISO3CountryName   126  /*0 Java Abbrev for country (ISO-3)   */ 
#define LOCI_jPercentPattern    127  /*2 Java percent pattern              */
#define LOCI_jPercentSign       128  /*2 Java percent symbol               */
#define LOCI_jExponent          129  /*2 Java exponential symbol           */
#define LOCI_jFullTimeFormat    130  /*1 Java full time format             */
#define LOCI_jLongTimeFormat    131  /*1 Java long time format             */
#define LOCI_jShortTimeFormat   132  /*1 Java short time format            */
#define LOCI_jFullDateFormat    133  /*1 Java full date format             */
#define LOCI_jMediumDateFormat  134  /*1 Java medium date format           */
#define LOCI_jDateTimePattern   135  /*1 Java date time format pattern     */
#define LOCI_jEraStrings        136  /*1 Java era strings                  */





/*
 * This max fields is used in the build, but programs should always
 * query iMaxItem from the locale.
 */
#define LOCI_MAXITEM            136  /*  Last item                         */
#define LOCI_NOUSEROVERRIDE  0x8000  /* Use system locale                  */

/*
 * These alternate names are for comptatibility with earlier releases
 */
#define D_T_FMT     LOCI_sDateTime           /* string for date and time   */
#define D_FMT       LOCI_sShortDate          /* string for formatting date */
#define T_FMT       LOCI_sTimeFormat         /* string for formatting time */
#define AM_STR      LOCI_s1159               /* string for a.m.            */
#define PM_STR      LOCI_s2359               /* string for p.m.            */
#define ABDAY_1     LOCI_sAbbrevDayName7     /* Abbrev day 1 (Sun)         */
#define ABDAY_2     LOCI_sAbbrevDayName1     /* Abbrev day 2 (Mon)         */
#define ABDAY_3     LOCI_sAbbrevDayName2     /* Abbrev day 3 (Tue)         */
#define ABDAY_4     LOCI_sAbbrevDayName3     /* Abbrev day 4 (Wed)         */
#define ABDAY_5     LOCI_sAbbrevDayName4     /* Abbrev day 5 (Thu)         */
#define ABDAY_6     LOCI_sAbbrevDayName5     /* Abbrev day 6 (Fri)         */
#define ABDAY_7     LOCI_sAbbrevDayName6     /* Abbrev day 7 (Sat)         */
#define DAY_1       LOCI_sDayName7           /* name of day 1 (Sunday)     */
#define DAY_2       LOCI_sDayName1           /* name of day 2 (Monday)     */
#define DAY_3       LOCI_sDayName2           /* name of day 3 (Tuesday)    */
#define DAY_4       LOCI_sDayName3           /* name of day 4 (Wednesday)  */
#define DAY_5       LOCI_sDayName4           /* name of day 5 (Thursday)   */
#define DAY_6       LOCI_sDayName5           /* name of day 6 (Friday)     */
#define DAY_7       LOCI_sDayName6           /* name of day 7 (Saturday)   */
#define ABMON_1     LOCI_sAbbrevMonthName1   /* Abbrev. name of month 1    */
#define ABMON_2     LOCI_sAbbrevMonthName2   /* Abbrev. name of month 2    */
#define ABMON_3     LOCI_sAbbrevMonthName3   /* Abbrev. name of month 3    */
#define ABMON_4     LOCI_sAbbrevMonthName4   /* Abbrev. name of month 4    */
#define ABMON_5     LOCI_sAbbrevMonthName5   /* Abbrev. name of month 5    */
#define ABMON_6     LOCI_sAbbrevMonthName6   /* Abbrev. name of month 6    */
#define ABMON_7     LOCI_sAbbrevMonthName7   /* Abbrev. name of month 7    */
#define ABMON_8     LOCI_sAbbrevMonthName8   /* Abbrev. name of month 8    */
#define ABMON_9     LOCI_sAbbrevMonthName9   /* Abbrev. name of month 9    */
#define ABMON_10    LOCI_sAbbrevMonthName10  /* Abbrev. name of month 10   */
#define ABMON_11    LOCI_sAbbrevMonthName11  /* Abbrev. name of month 11   */
#define ABMON_12    LOCI_sAbbrevMonthName12  /* Abbrev. name of month 12   */
#define MON_1       LOCI_sMonthName1         /* Name of month 1            */
#define MON_2       LOCI_sMonthName2         /* Name of month 2            */
#define MON_3       LOCI_sMonthName3         /* Name of month 3            */
#define MON_4       LOCI_sMonthName4         /* Name of month 4            */
#define MON_5       LOCI_sMonthName5         /* Name of month 5            */
#define MON_6       LOCI_sMonthName6         /* Name of month 6            */
#define MON_7       LOCI_sMonthName7         /* Name of month 7            */
#define MON_8       LOCI_sMonthName8         /* Name of month 8            */
#define MON_9       LOCI_sMonthName9         /* Name of month 9            */
#define MON_10      LOCI_sMonthName10        /* Name of month 10           */
#define MON_11      LOCI_sMonthName11        /* Name of month 11           */
#define MON_12      LOCI_sMonthName12        /* Name of month 12           */
#define RADIXCHAR   LOCI_sDecimal            /* radix character            */
#define THOUSEP     LOCI_sThousand           /* separator for thousands    */
#define YESSTR      LOCI_sYesString          /* yes response               */
#define NOSTR       LOCI_sNoString           /* no response                */
#define CRNCYSTR    LOCI_sCurrency           /* currency symbol            */
#define CODESET     LOCI_sCodeSet            /* codeset name               */
#define T_FMT_AMPM  LOCI_sTimeMark           /* am/pm time format          */
#define ERA         LOCI_sEra                /* era description segments   */
#define ERA_D_FMT   LOCI_sAltShortDate       /* era date format string     */
#define ERA_D_T_FMT LOCI_sAltDateTime        /* era date and time format   */
#define ERA_T_FMT   LOCI_sAltTimeFormat      /* era time format string     */
#define ALT_DIGITS  LOCI_sAltDigits          /* alternative digits         */
#define YESEXPR     LOCI_sYesExpr            /* yes response expression    */
#define NOEXPR      LOCI_sNoExpr             /* no response expression     */
#define DATESEP     LOCI_sDate               /* Date separator             */
#define TIMESEP     LOCI_sTime               /* Time separator             */
#define LISTSEP     LOCI_sList               /* List separator             */

#ifdef __cplusplus
   }
#endif

#endif
