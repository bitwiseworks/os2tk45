
/**********************************************************************\
 * C RunTime DBCS                                                     *
\**********************************************************************/

#define STRIP_LEADING  1
#define STRIP_TRAILING 2

VOID APIENTRY stripblanks( PSZ pszString, USHORT flags );

VOID GetDbcsInfo (VOID);

BOOL  APIENTRY isdbcs (UCHAR ch);
PCH   APIENTRY nextpch (PCH pszString);
PCH   APIENTRY prevpch (PCH pszBegin, PCH pszString);

#define decpch(pszBegin,pszString) (pszString=prevpch(pszBegin, pszString))
#define incpch(pszString) (pszString=nextpch(pszString))

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#define toupper(c) (((c >= 'a') && (c <= 'z')) ? c + 'A' - 'a' : c )

#ifndef INCL_DBCSCRT_NOSTRINGS

ULONG APIENTRY SWstrlen( PSZ pszString );
PSZ   APIENTRY SWstrchr( PSZ pszString, UCHAR c );
PSZ   APIENTRY SWstrrchr( PSZ pszString, UCHAR c );
PSZ   APIENTRY SWstrcpy( PSZ pszDestination, PSZ pszSource );
PSZ   APIENTRY SWstrcat( PSZ pszDestination, PSZ pszSource );
LONG  APIENTRY SWstrcmp( PSZ pszString1, PSZ pszString2 );

/*
 * Mem functions should be used out of the C runtime library.
 * They are optimized assembly routines.
 */

PVOID _Optlink memcpy( PVOID pvDest, PVOID pvSource, ULONG cb );
PVOID _Optlink memmove( PVOID pvDest, PVOID pvSource, ULONG cb );
PVOID _Optlink memset( PVOID pvDest, ULONG uch, ULONG cb );

LONG  APIENTRY SWstrcmpi (PCH pszString1, PCH pszString2);
LONG  APIENTRY SWstrnicmp (PCH pszString1, PCH pszString2, USHORT n);
PCH   APIENTRY SWstrncat_trunc (PCH pszString1, PCH pszString2, USHORT n);
PCH   APIENTRY SWstrncat (PCH pszString1, PCH pszString2, USHORT n);
PCH   APIENTRY SWstrncpy_trunc (PCH pszString1, PCH pszString2, USHORT n);
PCH   APIENTRY SWstrncpy (PCH pszString1, PCH pszString2, USHORT n);
PCH   APIENTRY SWstrstr (PCH pszString1, PCH pszString2);
PCH   APIENTRY SWstrupr (PCH pszString);
VOID  APIENTRY xlatblank (PCH pszString);
LONG  APIENTRY SWstrncmp (PCH pszString1, PCH pszString2, ULONG n);
PSZ   APIENTRY SWstrpbrk (PSZ, PSZ);
ULONG APIENTRY SWstrspn( PSZ, PSZ );
PCH   APIENTRY SWstrstri( PCH pszString1, PCH pszString2 );

/*
 * SW Backward Compatibility.
 */

VOID APIENTRY  SWmemcpy( PSZ pszDest, PSZ pszSource, USHORT usCb );
VOID APIENTRY  SWmemset( PSZ pszDest, UCHAR uch, USHORT usCb );

#define strncmpi      (SWstrnicmp)
#define strlen        (SWstrlen)
#define strchr        (SWstrchr)
#define strrchr       (SWstrrchr)
#define strcpy        (SWstrcpy)
#define strcat        (SWstrcat)
#define strcmp        (SWstrcmp)
#define strcmpi       (SWstrcmpi)
#define strnicmp      (SWstrnicmp)
#define strncat_trunc (SWstrncat_trunc)
#define strncat       (SWstrncat)
#define strncpy_trunc (SWstrncpy_trunc)
#define strncpy       (SWstrncpy)
#define strstr        (SWstrstr)
#define strupr        (SWstrupr)
#define strncmp       (SWstrncmp)
#define strpbrk       (SWstrpbrk)
#define strstri       (SWstrstri)
#define strspn        (SWstrspn)

/*
 * Replacement for C _splitpath for DBCS awareness.
 */

VOID  APIENTRY DBCS_splitpath( PSZ pszPath,
                               PSZ szDrive,
                               PSZ szDir,
                               PSZ szName,
                               PSZ szExt );

#endif

/*
 * Unique file generation routine.
 */

ULONG APIENTRY DBCSGenerateUniqueFile( PSZ pszPathName,
                                       PULONG pulPathLength,
                                       PHFILE phfile );
