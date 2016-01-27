#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __H_REGEX
  #define __H_REGEX

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
   /*  <regex.h> header file                                           */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #include <stddef.h>

   #define _REG_SUBEXP_MAX    23   /* Maximum # of subexpressions          */
   #define REG_EXTENDED    0x001   /* Use Extended RE syntax rules         */
   #define REG_ICASE       0x002   /* Ignore case in match                 */
   #define REG_NOSUB       0x008   /* regexec() not report subexpressions  */
   #define REG_NEWLINE     0x004   /* Convert <backslash><n> to <newline>  */

   #define REG_NOTBOL      0x100   /* First character not start of line    */
   #define REG_NOTEOL      0x200   /* Last character not end of line       */

   #define REG_NOMATCH     1       /* RE pattern not found                 */
   #define REG_BADPAT      2       /* Invalid Regular Expression           */
   #define REG_ECOLLATE    3       /* Invalid collating element            */
   #define REG_ECTYPE      4       /* Invalid character class              */
   #define REG_EESCAPE     5       /* Last character is \                  */
   #define REG_ESUBREG     6       /* Invalid number in \digit             */
   #define REG_EBRACK      7       /* [] imbalance                         */
   #define REG_EPAREN      8       /* \( \) or () imbalance                */
   #define REG_EBRACE      9       /* \{ \} or { } imbalance               */
   #define REG_BADBR       10      /* Invalid \{ \} range exp              */
   #define REG_ERANGE      11      /* Invalid range exp endpoint           */
   #define REG_ESPACE      12      /* Out of memory                        */
   #define REG_BADRPT      13      /* ?*+ not preceded by valid RE         */
   #define REG_ECHAR       14      /* invalid multibyte character          */
   #define REG_EBOL        15      /* ^ anchor and not BOL                 */
   #define REG_EEOL        16      /* $ anchor and not EOL                 */


   /* NOTE:  The size of regex_t must not change.  The size of the members
    *        __re_lsub + __re_esub + __re_map + __maxsub + __unsed must be
    *        exactly 336 bytes, so if _REG_SUBEXP_MAX above changes, you'll
    *        have to modify __unsed accordingly.
    */

   typedef struct {             /* regcomp() data saved for regexec()   */
        size_t  re_nsub;        /* # of subexpressions in RE pattern    */
        void    *re_comp;       /* compiled RE; freed by regfree()      */
        int     re_cflags;      /* saved cflags for regexec()           */
        size_t  re_erroff;      /* RE pattern error offset              */
        size_t  re_len;         /* # wchar_t chars in compiled pattern  */
        wchar_t re_ucoll[2];    /* min/max unique collating values      */

        void    *re_lsub[_REG_SUBEXP_MAX+1]; /* start subexp            */
        void    *re_esub[_REG_SUBEXP_MAX+1]; /* end subexp              */
        unsigned char *re_map;  /* map of valid pattern characters      */
        int     __maxsub;       /* maximum number of subs in pattern.   */
        void    *__unused[34];  /* Extra space if ever needed           */
   } regex_t;

   #ifndef __off_t
      typedef long  off_t;
      #define __off_t
   #endif

   typedef struct {             /* substring locations - from regexec() */
        off_t   rm_so;          /* Byte offset from start of string to  */
                                /*   start of substring                 */
        off_t   rm_eo;          /* Byte offset from start of string of  */
                                /*   first character after substring    */
    } regmatch_t;

   extern int _IMPORT _LNK_CONV regcomp(regex_t *preg, const char *pattern, int cflags);

   extern int _IMPORT _LNK_CONV regexec(const regex_t *preg, const char *string,
     size_t nmatch, regmatch_t pmatch[], int eflags);

   extern size_t _IMPORT _LNK_CONV regerror(int errcode, const regex_t *preg,
     char *errbuf, size_t errbuf_size);

   extern void _IMPORT _LNK_CONV regfree(regex_t *preg);

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

