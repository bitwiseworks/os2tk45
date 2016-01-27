/*
 * COMPONENT_NAME:
 *
 * FUNCTIONS: ULS API prototypes and typedefs - public
 *
 * (C) COPYRIGHT International Business Machines Corp. 1994
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */

#ifndef _ULS_UNIDEF
#define _ULS_UNIDEF

#ifdef __cplusplus
   extern "C" {
#endif

/* compiler dependent include files */
#include <stddef.h>             /* define size_t and wchar_t types */
#include <time.h>               /* define tm struct */

/* ULS include files */
#include <os2def.h>
#include <ulserrno.h>           /* uls return code defs */
#include <errcmpat.h>
#include <ulsitem.h>            /* define LocaleItem type */
#include <callconv.h>

/*
 * ulsBool Type
 */
typedef int ulsBool;

/*
 * UniChar Type
 */
#ifndef _ULS_UNICHAR_DEFINED
    typedef unsigned  short  UniChar ;
    #define _ULS_UNICHAR_DEFINED
#endif

/*
 * LocaleObject Type
 */
typedef void  *LocaleObject;

/*
 * LocaleToken Type
 */
typedef unsigned int LocaleToken;

/*
 * AttrObject Type
 */
typedef void *AttrObject;

/*
 * XformObject type
 */
typedef void *XformObject;

/*
 * LocaleSpecType Values
 */
#define UNI_TOKEN_POINTER       1
#define UNI_MBS_STRING_POINTER  2
#define UNI_UCS_STRING_POINTER  3

/*
 * Locale Types
 */
#define UNI_SYSTEM_LOCALES      1
#define UNI_USER_LOCALES        2

/*
 * Categories
 */
#undef LANG
#undef LC_ALL
#undef LC_COLLATE
#undef LC_CTYPE
#undef LC_NUMERIC
#undef LC_MONETARY
#undef LC_TIME
#undef LC_MESSAGES

#define LANG         (-2)
#define LC_ALL       (-1)
#define LC_COLLATE    0
#define LC_CTYPE      1
#define LC_NUMERIC    2
#define LC_MONETARY   3
#define LC_TIME       4
#define LC_MESSAGES   5

#define N_LC_CATEGORIES  6     /* Number of categories                   */
#define ULS_LNAMEMAX     32    /* Maxlen of locale name (including null) */

/*
 * UniLconv locale conventions structure
 */
typedef struct UniLconv {
   UniChar *decimal_point;      /* non-monetary decimal point           */
   UniChar *thousands_sep;      /* non-monetary thousands separator     */
   short   *grouping;           /* non-monetary size of grouping        */
   UniChar *int_curr_symbol;    /* int'l currency symbol and separator  */
   UniChar *currency_symbol;    /* local  currency symbol               */
   UniChar *mon_decimal_point;  /* monetary decimal point               */
   UniChar *mon_thousands_sep;  /* monetary thousands separator         */
   short   *mon_grouping;       /* monetary size of grouping            */
   UniChar *positive_sign;      /* non-negative values sign             */
   UniChar *negative_sign;      /* negative values sign                 */
   short   int_frac_digits;     /* no of fractional digits int currency */
   short   frac_digits;         /* no of fractional digits loc currency */
   short   p_cs_precedes;       /* nonneg curr sym 1-precedes,0-succeeds*/
   short   p_sep_by_space;      /* nonneg curr sym 1-space,0-no space   */
   short   n_cs_precedes;       /* neg curr sym 1-precedes,0-succeeds   */
   short   n_sep_by_space;      /* neg curr sym 1-space 0-no space      */
   short   p_sign_posn;         /* positioning of nonneg monetary sign  */
   short   n_sign_posn;         /* positioning of negative monetary sign*/
   short   os2_mondecpt;        /* os2 curr sym positioning             */
   short   reserved;
   UniChar *debit_sign;         /* non-neg-valued monetary sym - "DB"   */
   UniChar *credit_sign;        /* negative-valued monetary sym - "CR"  */
   UniChar *left_parenthesis;   /* negative-valued monetary sym - "("   */
   UniChar *right_parenthesis;  /* negative-valued monetary sym - ")"   */
} UNILCONV;

/*
 * Locale Management Function Prototypes
 */
int     CALLCONV UniCreateLocaleObject ( int locale_spec_type, const void *locale_spec,
                            LocaleObject *locale_object_ptr );
int     CALLCONV UniQueryLocaleObject  ( const LocaleObject locale_object, int category,
                            int locale_spec_type, void **locale_spec_ptr );
int     CALLCONV UniFreeLocaleObject   ( LocaleObject locale_object );
int     CALLCONV UniFreeMem            ( void *memory_ptr );
int     CALLCONV UniLocaleStrToToken   ( int locale_string_type, const void *locale_string,
                            LocaleToken *locale_token_ptr );
int     CALLCONV UniLocaleTokenToStr   ( const LocaleToken locale_token,
                            int locale_string_type, void **locale_string_ptr );

/*
 * Locale Information Function Prototypes
 */
int     CALLCONV UniQueryLocaleInfo ( const LocaleObject locale_object,
                         struct UniLconv **unilconv_addr_ptr );
int     CALLCONV UniFreeLocaleInfo  ( struct UniLconv *unilconv_addr );
int     CALLCONV UniQueryLocaleItem ( const LocaleObject locale_object,
                         LocaleItem item,
                         UniChar **info_item_addr_ptr );
int     CALLCONV UniQueryLocaleValue ( const LocaleObject locale_object,
                         LocaleItem item,
                         int *info_item);

/*
 * Date and Time Function Prototypes
 */
size_t       CALLCONV UniStrftime ( const LocaleObject locale_object,
                       UniChar *ucs, size_t maxsize,
                       const UniChar *format, const struct tm *time_ptr );
UniChar    * CALLCONV UniStrptime ( const LocaleObject locale_object,
                       const UniChar *buf, const UniChar *format,
                       struct tm *time_ptr );

/*
 * Monetary Formatting Function Prototype
 */
int          CALLCONV UniStrfmon  ( const LocaleObject locale_object,
                       UniChar *ucs, size_t maxsize,
                       const UniChar *format, ... );

/*
 * String/Character Function Prototypes
 */
UniChar    * CALLCONV UniStrcat  ( UniChar *ucs1, const UniChar *ucs2 );
UniChar    * CALLCONV UniStrchr  ( const UniChar *ucs, UniChar uc );
int          CALLCONV UniStrcmp  ( const UniChar *ucs1, const UniChar *ucs2 );
UniChar    * CALLCONV UniStrcpy  ( UniChar *ucs1, const UniChar *ucs2 );
size_t       CALLCONV UniStrcspn ( const UniChar *ucs1, const UniChar *ucs2 );
size_t       CALLCONV UniStrlen  ( const UniChar *ucs1 );
UniChar    * CALLCONV UniStrncat ( UniChar *ucs1, const UniChar *ucs2, size_t n );
int          CALLCONV UniStrncmp ( const UniChar *ucs1, const UniChar *ucs2, size_t n );
UniChar    * CALLCONV UniStrncpy ( UniChar *ucs1, const UniChar *ucs2, size_t n );
UniChar    * CALLCONV UniStrpbrk ( const UniChar *ucs1, const UniChar *ucs2 );
UniChar    * CALLCONV UniStrrchr ( const UniChar *ucs, UniChar uc );
size_t       CALLCONV UniStrspn  ( const UniChar *ucs1, const UniChar *ucs2 );
UniChar    * CALLCONV UniStrstr  ( const UniChar *ucs1, const UniChar *ucs2 );
UniChar    * CALLCONV UniStrtok  ( UniChar *ucs1, const UniChar *ucs2 );


/*
 * Character Attribute Function Prototypes
 */
int   CALLCONV UniCreateAttrObject ( const LocaleObject locale_object,
                           const UniChar *attr_name,
                           AttrObject *attr_object_ptr );
int   CALLCONV UniQueryCharAttr    ( AttrObject attr_object, UniChar uc );
int   CALLCONV UniScanForAttr      ( AttrObject attr_object, const UniChar *ucs,
                           size_t num_elems, ulsBool inverse_op,
                           size_t *offset_ptr );
int   CALLCONV UniFreeAttrObject   ( AttrObject attr_object );
int   CALLCONV UniQueryAlnum       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryAlpha       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryBlank       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryCntrl       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryDigit       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryGraph       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryLower       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryPrint       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryPunct       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQuerySpace       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryUpper       ( const LocaleObject loc, UniChar uc );
int   CALLCONV UniQueryXdigit      ( const LocaleObject loc, UniChar uc );


/*
 * String Transformation Function Prototypes
 */
int   CALLCONV UniCreateTransformObject ( const LocaleObject locale_object,
                                const UniChar *xtype,
                                XformObject *xform_object_ptr );
int   CALLCONV UniTransformStr          ( XformObject xform_object,
                                const UniChar *inp_buf, int *inp_size,
                                UniChar *out_buf, int *out_size );
int   CALLCONV UniFreeTransformObject   ( XformObject xform_object );
UniChar   CALLCONV UniTransLower        ( const LocaleObject locale_object, UniChar uc );
UniChar   CALLCONV UniTransUpper        ( const LocaleObject locale_object, UniChar uc );

/*
 * String Conversion Function Prototypes
 */
int   CALLCONV UniStrtod  ( const LocaleObject locale_object, const UniChar *ucs,
                  UniChar **end_ptr, double *result_ptr );
int   CALLCONV UniStrtol  ( const LocaleObject locale_object, const UniChar *ucs,
                  UniChar **end_ptr, int base, long int *result_ptr );
int   CALLCONV UniStrtoul ( const LocaleObject locale_object, const UniChar *ucs,
                  UniChar **end_ptr, int base, unsigned long int *result_ptr );

/*
 * String Comparison Function Prototypes
 */
int      CALLCONV UniStrcoll  ( const LocaleObject locale_object,
                     const UniChar *ucs1, const UniChar *ucs2 );
size_t   CALLCONV UniStrxfrm  ( const LocaleObject locale_object,
                     UniChar *ucs1, const UniChar *ucs2, size_t n );
int      CALLCONV UniStrcmpi  ( const LocaleObject locale_object,
                     const UniChar *ucs1, const UniChar *ucs2 );
int      CALLCONV UniStrncmpi ( const LocaleObject locale_object,
                     const UniChar *ucs1, const UniChar *ucs2, const size_t n );

/*
 * Unicode Case Mapping Function Prototypes
 */
UniChar   CALLCONV UniToupper ( UniChar uc );
UniChar   CALLCONV UniTolower ( UniChar uc );
UniChar * CALLCONV UniStrupr  ( UniChar * ucs );
UniChar * CALLCONV UniStrlwr  ( UniChar * ucs );


int CALLCONV UniMapCtryToLocale( unsigned long Country,
                                 UniChar *LocaleName, size_t n );

/*
 * Functions for user locales.  These are designed to be externalizable,
 * and are used by WPShell, but are currently private functions.
 * These functions are in ulx.c
 */
int CALLCONV UniSetUserLocaleItem(UniChar * locale, int item, int type, void * value);
int CALLCONV UniMakeUserLocale(UniChar * name, UniChar * basename);
int CALLCONV UniDeleteUserLocale(UniChar * locale);
int CALLCONV UniCompleteUserLocale(void);
int CALLCONV UniQueryLocaleList(int, UniChar *, int);
int CALLCONV UniQueryLanguageName(UniChar *lang, UniChar *isolang,
                                  UniChar **infoitem);
int CALLCONV UniQueryCountryName(UniChar *country, UniChar *isolang,
                                  UniChar **infoitem);


/*
 * Typedef for QueryCharType and UniQueryStringType indexed
 */
typedef struct {
    USHORT  itype;        /* CTYPE1 - xpg4 attributes     */
    CHAR    bidi;         /* CTYPE2 - Bidi attributes     */
    CHAR    charset;      /* CHARSET- Character set       */
    USHORT  extend;       /* CTYPE3 - Extended attributes */
    USHORT  codepage;     /* Codepage mask (CCP)          */
} UNICTYPE;


/*
 * Locale independent character classification
 */
int   CALLCONV UniQueryChar          ( UniChar uc, ULONG attr);
ULONG CALLCONV UniQueryAttr          ( UniChar * name);
ULONG CALLCONV UniQueryStringType    ( UniChar * ustr, int size, USHORT * outstr, int kind );
UNICTYPE * CALLCONV UniQueryCharType ( UniChar uchr);
ULONG CALLCONV UniQueryCharTypeTable ( ULONG * count, UNICTYPE * * unictype);
int   CALLCONV UniQueryNumericValue  ( UniChar   uc);

/*
 * Character type constants
 */
#define CT_UPPER           0x0001      /* Upper case alphabetic character */
#define CT_LOWER           0x0002      /* Lower case alphabetic character */
#define CT_DIGIT           0x0004      /* Digits 0-9                      */
#define CT_SPACE           0x0008      /* White space and line ends       */
#define CT_PUNCT           0x0010      /* Punctuation marks               */
#define CT_CNTRL           0x0020      /* Control and format characters   */
#define CT_BLANK           0x0040      /* Space and tab                   */
#define CT_XDIGIT          0x0080      /* Hex digits                      */
#define CT_ALPHA           0x0100      /* Letters and linguistic marks    */
#define CT_ALNUM           0x0200      /* Alphanumeric                    */
#define CT_GRAPH           0x0400      /* All except controls and space   */
#define CT_PRINT           0x0800      /* Everything except controls      */
#define CT_NUMBER          0x1000      /* Integral number                 */
#define CT_SYMBOL          0x2000      /* Symbol                          */
#define CT_ASCII           0x8000      /* In standard ASCII set           */

/*
 * Define the Win32 equivalent ames for the bits
 */
#define C1_UPPER    CT_UPPER
#define C1_LOWER    CT_LOWER
#define C1_DIGIT    CT_DIGIT
#define C1_SPACE    CT_SPACE
#define C1_PUNCT    CT_PUNCT
#define C1_CNTRL    CT_CNTRL
#define C1_BLANK    CT_BLANK
#define C1_XDIGIT   CT_XDIGIT
#define C1_ALPHA    CT_ALPHA

/*
 * Define types used in the unicode layout function.
 */
#define C2_NOTAPPLICABLE     0x00      /*    - Not a character             */
#define C2_LEFTTORIGHT       0x01      /* L  - Left to Right               */
#define C2_RIGHTTOLEFT       0x02      /* R  - Right to Left               */
#define C2_EUROPENUMBER      0x03      /* EN - European number             */
#define C2_EUROPESEPARATOR   0x04      /* ES - European separator          */
#define C2_EUROPETERMINATOR  0x05      /* ET - European terminator         */
#define C2_ARABICNUMBER      0x06      /* AN - Arabic number               */
#define C2_COMMONSEPARATOR   0x07      /* CS - Common separator            */
#define C2_BLOCKSEPARATOR    0x08      /* B  - Block separator             */
#define C2_WHITESPACE        0x0A      /* WS - Whitespace                  */
#define C2_OTHERNEUTRAL      0x0B      /* ON - Other neutral               */
#define C2_MIRRORED          0x0C      /* M  - Symetrical   (not Win32)    */


/*
 * Define the character set values.  For alphabetics this defines the
 * lingusitic group.  For symbols and punctuation this defines some
 * subtypes.
 */
#define CHS_NONCHAR       0x00
#define CHS_OTHER         0x01
#define CHS_LATIN         0x02
#define CHS_CYRILLIC      0x03
#define CHS_ARABIC        0x04
#define CHS_GREEK         0x05
#define CHS_HEBREW        0x06
#define CHS_THAI          0x07
#define CHS_KATAKANA      0x08
#define CHS_HIRAGANA      0x09
#define CHS_HANGUEL       0x0a
#define CHS_BOPOMOFO      0x0b
#define CHS_DEVANAGARI    0x0c
#define CHS_TELUGU        0x0d
#define CHS_BENGALI       0x0e
#define CHS_GUJARATI      0x0f
#define CHS_GURMUKHI      0x10
#define CHS_TAMIL         0x11
#define CHS_LAO           0x12

#define CHS_PUNCTSTART    0x20
#define CHS_PUNCTEND      0x21
#define CHS_DINGBAT       0x22
#define CHS_MATH          0x23
#define CHS_APL           0x24
#define CHS_ARROW         0x25
#define CHS_BOX           0x26
#define CHS_DASH          0x27
#define CHS_CURRENCY      0x28
#define CHS_FRACTION      0x29
#define CHS_LINESEP       0x2a
#define CHS_USERDEF       0x2b

/*
 * Define typed for extensions to the POSIX types based on the Win32
 * text processing types.  These duplicate some of the other information
 * in the CT and CHS areas.
 */
#define C3_NONSPACING     0x0001       /* Nonspacing mark       */
#define C3_DIACRITIC      0x0002       /* Diacritic mark        */
#define C3_NSDIACRITIC    0x0003
#define C3_VOWELMARK      0x0004       /* Vowel mark            */
#define C3_NSVOWEL        0x0005
#define C3_SYMBOL         0x0008       /* Symbol (see CT_SYMBOL)*/
#define C3_KATAKANA       0x0010       /* Katakana character    */
#define C3_HIRAGANA       0x0020       /* Hiragana character    */
#define C3_HALFWIDTH      0x0040       /* Half-width varient    */
#define C3_FULLWIDTH      0x0080       /* Full-width varient    */
#define C3_IDEOGRAPH      0x0100       /* Kanji/Han character   */
#define C3_KASHIDA        0x0200       /* Arabic enlonger       */
#define C3_ALPHA          0x8000       /* Alphabetic            */
#define C3_MASK           0x83FF       /* Mask for Win32 bits   */

/*
 * Define containing codepage for UGL codepages
 */
#define CCP_437           0x0001       /* US PC                 */
#define CCP_850           0x0002       /* Multilingual PC       */
#define CCP_SYMB          0x0004       /* PostScript Symbol     */
#define CCP_1252          0x0008       /* Windows Latin 1       */
#define CCP_1250          0x0010       /* Windows Latin 2       */
#define CCP_1251          0x0020       /* Windows Cyrillic      */
#define CCP_1254          0x0040       /* Windows Turkish       */
#define CCP_1257          0x0080       /* Windows Baltic        */

/*
 * Kind of data return for UniQueryStringType
 */
#define CT_ITYPE     1
#define CT_BIDI      2
#define CT_CHARSET   3
#define CT_EXTENDED  4
#define CT_CODEPAGE  5
#define CT_INDEX     6
#define CT_CTYPE1    7    /* Win32 compat xpg4            */
#define CT_CTYPE2    8    /* Win32 compat bidi            */
#define CT_CTYPE3    9    /* Win32 compat extended        */

#ifdef __cplusplus
   }
#endif

#endif
