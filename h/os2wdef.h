/******************************* Module Header ******************************
 *
 * Module Name: OS2WDEF.H
 *
 * OS/2 Developer API Extensions Common Definitions File
 *
 * Copyright (c) International Business Machines Corporation 1995
 *
 ***************************************************************************/
#ifndef _OS2WDEF_H
#define _OS2WDEF_H


#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( none )
   #endif
   #pragma info( restore )
#endif


typedef char                CHAR ,   *PCHAR ,  *LPCHAR ;
typedef short               SHORT,   *PSHORT,  *LPSHORT;
typedef int                 INT  ,   *PINT  ,  *LPINT  ;
typedef long                LONG ,   *PLONG ,  *LPLONG ;

typedef unsigned char       UCHAR ,  *PUCHAR , *LPUCHAR ;
typedef unsigned short      USHORT,  *PUSHORT, *LPUSHORT;
typedef unsigned int        UINT  ,  *PUINT  , *LPUINT  ;
typedef unsigned long       ULONG ,  *PULONG , *LPULONG ;

typedef void                VOID ,   *PVOID ,  *LPVOID ;
typedef float               FLOAT,   *PFLOAT,  *LPFLOAT;

typedef UCHAR             BYTE,    *PBYTE,   *LPBYTE;
typedef USHORT            WORD,    *PWORD,   *LPWORD;
typedef ULONG             DWORD,   *PDWORD,  *LPDWORD;

#ifndef _BOOL_DEFINED
#define _BOOL_DEFINED
typedef INT               BOOL,    *PBOOL,   *LPBOOL;
#endif

typedef const VOID       *PCVOID,  *LPCVOID;
typedef CHAR             *PSZ   ,  *LPSZ;
typedef const CHAR       *PCSZ  ,  *LPCSZ;

#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif


typedef UINT              WPARAM;
typedef LONG              LPARAM;
typedef LONG              LRESULT;
typedef LONG              HRESULT;

typedef ULONG             HANDLE,  *PHANDLE,  *LPHANDLE,  *SPHANDLE;

#ifndef _WCHAR_T_DEFINED
 #define _WCHAR_T_DEFINED
#endif
#ifndef __wchar_t
 #define __wchar_t
 typedef unsigned short wchar_t;
#endif
typedef wchar_t WCHAR;

typedef WCHAR *PWCHAR;
typedef WCHAR *LPWCH, *PWCH;
typedef const    WCHAR  *LPCWCH, *PCWCH;

typedef WCHAR *NWPSTR;
typedef WCHAR *LPWSTR, *PWSTR;
typedef const    WCHAR   *LPCWSTR, *PCWSTR;

typedef CHAR  *LPCH,   *PCH;

typedef const CHAR *LPCCH,  *PCCH;
typedef CHAR       *NPSTR;
typedef CHAR       *LPSTR,  *PSTR;
typedef const CHAR *LPCSTR, *PCSTR;

typedef CHAR     TCHAR, *PTCHAR;
typedef UCHAR    TBYTE, *PTBYTE;

typedef LPSTR  LPTCH, PTCH;
typedef LPSTR  PTSTR, LPTSTR;
typedef LPCSTR LPCTSTR;
#define __TEXT(quote) quote

#define TEXT(quote) __TEXT(quote)

#define CALLBACK    _System
#define WINAPI      _System
#define WINAPIV     _System
#define APIENTRY    _System
#define APIPRIVATE  _System
#define PASCAL      _System
#define EXPENTRY    _System

#ifndef NULL
  #define NULL   0
#endif

#define UNREFERENCED_PARAMETER(p)    (p)

#define CONST   const

#if __IBMC__ || __IBMCPP__
   #pragma info( none )
   #ifndef __CHKHDR__
      #pragma info( restore )
   #endif
   #pragma info( restore )
#endif

#endif /* _OS2WDEF_H */
