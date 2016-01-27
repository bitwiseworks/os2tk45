/***!Start!***                                                                     
 *                                                                                 
 *   ORIGINS: 27                                                                   
 *                                                                                 
 *   IBM CONFIDENTIAL -- (IBM Confidential Restricted when                         
 *   combined with the aggregated modules for this product)                        
 *   OBJECT CODE ONLY SOURCE MATERIALS                                             
 *                                                                                 
 *     25H7912  (C)   COPYRIGHT International Business Machines Corp. 1992,1994,1996,1996  
 *   All Rights Reserved                                                           
 *   Licensed Materials - Property of IBM                                          
 *   The source code for this program is not published or otherwise divested       
 *   of its trade secrets, irrespective of what has been deposited with the        
 *   U.S. Copyright Office.                                                        
 *                                                                                 
 ***!End!***/                                                                      
                                                                                   
/*
 *    @(#) 1.10 src/win/win/somport.h, somwin, som2.1 1/3/96 17:05:34 [7/30/96 15:13:48]
 */
/**************************************************************************\
 * SOMPORT.H
 *
 * Header file for functions written by CP for MS Windows 3.1 
 * version of SOM.
 * 
 * Has -> STUBLIB, WINPRINT, FINDORD, GETMSG
 *
\**************************************************************************/
#ifndef _SOMPORT_H_
#define _SOMPORT_H_

/**************************************************************************\
 * Short forms of SOM_METHOD_THUNKS, SOM_DATA_THUNKS and SOM_METHOD_STUBS *
\**************************************************************************/

#if (defined(_SOM_ALL) || defined(_SOM_MT)) && ! defined(SOM_METHOD_THUNKS)
    #define SOM_METHOD_THUNKS   1
#endif
#if (defined(_SOM_ALL) || defined(_SOM_DT)) && ! defined(SOM_DATA_THUNKS)
    #define SOM_DATA_THUNKS	1
#endif
#if (defined(_SOM_ALL) || defined(_SOM_MS)) && ! defined(SOM_METHOD_STUBS)
    #define SOM_METHOD_STUBS	1
#endif

/**************************************************************************\
 * 
 * WINDOWS 16 BIT port specific code for the runtime
 *
\**************************************************************************/
#ifdef _WIN16
  #define FAR _far
  #define SIZET_CAST (size_t)
#else
  #define FAR 
  #define SIZET_CAST 
  #define W16_FAR 
#endif

#ifdef _WIN16     
  #define IN
  #define OUT
  #define INOUT

  #include <stdarg.h>
  #include <process.h>
  #ifndef WIN16_NOINCL_STDLIB 
  #include <stdlib.h>
#endif

#ifdef _WIN16
  #define W16LOADDS __loadds 
  #define W16PASCAL __pascal 
  #if (defined(__ZTC__) && !defined(__SC__)) || defined(MULTINST)
    #undef W16LOADDS
    #define W16LOADDS 
    #undef SOMLINK
    #define SOMLINK 
    #undef SOMDLINK
    #define SOMDLINK
    #undef W16PASCAL
    #define W16PASCAL
  #endif
#endif

#include <stdio.h>
#undef FILENAME_MAX
#define FILENAME_MAX 260

#include <sys\types.h>
#ifndef __SC__
#include <sys\timeb.h>
#endif
#include <sys\stat.h>
#include <string.h>
#include <stddef.h>

#include <windows.h>
#undef ERROR

#include <toolhelp.h>

/*
===========================================================================
== Defines
===========================================================================
*/
#ifdef _WINDLL
#define getenv GetDllEnv
#define access _access
#define getcwd _getcwd
#define fdopen _fdopen
#define fstat _fstat
#define stat _stat
#define sopen _sopen
#define open   _open
#define read _read
#define write _write
#define lseek _lseek
#define chsize _chsize
#define close _close
#define ftime _ftime
#define timeb _timeb
#define sscanf somw16sscanf
#define fscanf somw16fscanf
#define fflush somw16fflush
#define fprintf somw16fprintf
#define vsprintf somw16Wvsprintf
#define sprintf somw16Wsprintf
#define LoadModule W16LoadModule
#define exit(x) TerminateApp(0,NO_UAE_BOX);
#define perror somPrintf
#define getpid _getpid 
#define fileno _fileno
#define stdout NULL
#define stderr NULL


#endif

/*
===========================================================================
== Macros
===========================================================================
*/

/*
===========================================================================
== WIN16THUNKS and WIN16STUBS
===========================================================================
*/
/* WIN16THUNKS and WIN16STUBS. need to call memcpy function and not 
 * intrinsic version that is generated with -Oi (which is needed for floats).
 */
#pragma function(memcpy)   

/* Turn off intrinsic function for memcpy. "-Oi" assumes _ThunkStub
 * is in current code segment 
 */
WORD FAR PASCAL SelectorAccessRights(WORD wSel, WORD wOp, WORD wParam);
#define RIGHTS(wSel) (SelectorAccessRights((wSel),0,0))
#define IS_CODE(wSel) (RIGHTS(wSel) & 8)

/*
===========================================================================
== TypeDefs
===========================================================================
*/
typedef unsigned long APIRET;
typedef unsigned long ULONG;

/*
===========================================================================
== Wide string character functions
===========================================================================
*/
size_t wcslen( const wchar_t * w_string);
wchar_t * wcschr( const wchar_t * w_string, wchar_t w_char );

/*
===========================================================================
== DosSleep
===========================================================================
*/
unsigned long _far DosSleep(unsigned long x);

/*
===========================================================================
== WINPRINT
==
== FUNCTION: Prints a message given in pszMsgString via SOMMSG.EXE
== RETURN  : 0 for success, 1 otherwise
===========================================================================
*/
extern char WinPrintBuf[256]; /* defined in winprint.c */
short _far _pascal WinPrint (char _far * pszMsgString);


/*
===========================================================================
== FILEIO
==
== FUNCTION: Replaces the buffered io with unbuffered
== RETURN  : 
===========================================================================
*/
long __cdecl somw16ftell(FILE *stream);
size_t __cdecl somw16fwrite (const void *buffer,size_t size,size_t count,
                             FILE *stream);
size_t __cdecl somw16fread(void *buffer,size_t size,size_t count,FILE *stream);
int __cdecl somw16fseek(FILE *stream, long offset, int origin);


/*
===========================================================================
== SOMW16WSPRINTF,SOMW16WVSPRINTF -> accomodate the %f %g %d *
== SOMW16VSCANF, SOMW16SSCANF, SOMW16FSCANF -> windows implementation
== SOMW16FFLUSH -> null function. No flushing in windows
== 
== Input: same as the sprintf and vsprintf functions
== 
== Returns: num of chars stored in buf
===========================================================================
*/
extern int somw16Wsprintf (char* buffer, char* format, ...);
extern int somw16Wvsprintf (char* buffer, char* format, va_list ap);
extern int somw16vsscanf(const char *s,const char *format, va_list ap);
extern int somw16sscanf(const char *s,const char *format, ...);
extern int somw16fscanf(FILE *fp,const char *format, ...);
extern int somw16fflush(FILE *stream);
extern int somw16fprintf(FILE *fp,const char *format, ...);

/*
===========================================================================
== FINDORD
==
== FUNCTION: Given an instance of a dll module and a function name, it 
==           will find the ordinal number in the dll's resident and 
==           nonresident name tables.
== RETURN  : The ordinal number if successful, 0 otherwise.
===========================================================================
*/
extern WORD somw16FindOrdinal(HINSTANCE hInst, LPSTR lpszFuncName);


/*
===========================================================================
== GETMSG
==
== FUNCTION: Replaces OS2 DosGetMessage. Looks up a message in file 
==           produced by mconvert.
==           
== RETURN  : 
===========================================================================
*/
int __far __pascal DosGetMessage( char* [], unsigned long, char*, unsigned long, unsigned long, char*, unsigned long* );
extern char * GetDllEnv(const char * s);
extern void SearchDllEnv(char *, char *, char *);

/*
 * Error codes that are returned by DosGetMessage()
 */

#ifndef NO_ERROR
#define NO_ERROR                    0
#endif
#define ERROR_FILE_NOT_FOUND        2
#define ERROR_FILENAME_EXCED_RANGE  206
#define ERROR_MR_MSG_TOO_LONG       316
#define ERROR_MR_MID_NOT_FOUND      317
#define ERROR_MR_UN_ACC_MSGF        318
#define ERROR_MR_INV_MSGF_FORMAT    319
#define ERROR_MR_INV_IVCOUNT        320
#define ERROR_MR_UN_PERFORM         321

/*
===========================================================================
== SOMYIELD -> yield control back to Windows, allow Windows to go idle
== 
== Input:   pwRc  - set to exit code from WM_QUIT message
== 
== Returns: TRUE  - Continue processing
==          FALSE - a WM_QUIT message was posted to the message queue
===========================================================================
*/
extern BOOL somw16Yield(WORD *pwRc);


/*
===========================================================================
== SOMW16STARTINVISWIN -> opens an invisible window. Used for queueing.
== 
== Input:   WndProc  - handle to window procedure
== 			data     - window creation data
== 
== Returns: TRUE  - Continue processing
==          FALSE - Could not create Window.
===========================================================================
*/
extern BOOL somw16StartInvisWin(WNDPROC WndProc, VOID *data);

/*
===========================================================================
== SOMW16ATEXIT-> register a function to be called on program exit, used
==                only from DLLs
== 
== Input:   func     - ptr to function to be called at program exit
== 
== Returns: N/A
===========================================================================
*/
void somw16Atexit(void (__cdecl *func)(void));

#endif /* _WIN16 */


/**************************************************************************\
 * 
 * WINDOWS 16 BIT port specific code for the SOM COMPILER
 *
\**************************************************************************/
#ifdef _WDOS

#ifndef __SOMEMIT__
    #define __SOMEMIT__ 1
#endif

/**************************************************************************
 * Includes
 **************************************************************************/
#include <stdio.h>
#undef FILENAME_MAX
#define FILENAME_MAX 260
#undef stderr
#define stderr stdout

/**************************************************************************
 * Typedefs
 **************************************************************************/
typedef unsigned long ULONG;
typedef unsigned long APIRET;

/**************************************************************************
 * WIDE character string function
 **************************************************************************/
size_t wcslen( const wchar_t * w_string);
wchar_t * wcschr( const wchar_t * w_string, wchar_t w_char );

/**************************************************************************
 * GETMSG
 **************************************************************************/
extern int DosGetMessage( char* [], unsigned long, char*, unsigned long, unsigned long, char*, unsigned long* );

#define NO_ERROR                    0
#define ERROR_FILE_NOT_FOUND        2
#define ERROR_FILENAME_EXCED_RANGE  206
#define ERROR_MR_MSG_TOO_LONG       316
#define ERROR_MR_MID_NOT_FOUND      317
#define ERROR_MR_UN_ACC_MSGF        318
#define ERROR_MR_INV_MSGF_FORMAT    319
#define ERROR_MR_INV_IVCOUNT        320
#define ERROR_MR_UN_PERFORM         321

#define MAXLEN                      256
#define MSGIDLEN                    11
#define FILEOPENMODE                "r"
#define NUMBASE                     48   // 48 is 0 on the ASCII table

/**************************************************************************
 * W16MEM
 **************************************************************************/
extern void * somw16Malloc(size_t numbytes);
extern void * somw16Calloc(size_t numelems, size_t sizelem);
extern void * somw16Realloc(void *ptr, size_t size);
extern void somw16Free(void* p);
extern void somw16FreeAll (void);

/**************************************************************************
 * FSOPEN for zortech.
 **************************************************************************/
FILE * somw16fsopen(const char *name,const char *mode, int shflag);
#endif /* WDOS */

/**************************************************************************
 * Test case macros.
 **************************************************************************/
#ifdef _WIN16
#include <io.h>        /* This is for the quickwin prototypes */
#define WIN16_SETEXIT_NOPERSIST _wsetexit(_WINEXITNOPERSIST);
#define WIN16_SETEXIT_PERSIST   _wsetexit(_WINEXITPERSIST);
#define WIN16_SOMEXIT           atexit(somEnvironmentEnd);
#define somFreeModules          somEnvironmentEnd
#ifndef __cplusplus
#define WIN16_FIX_WINDOW {extern HWND _hWndClient; \
        SetWindowText(GetWindow(_hWndClient,GW_CHILD),""); \
        ShowWindow(GetWindow(_hWndClient,GW_CHILD),SW_SHOWMAXIMIZED);}
#define WIN16_KILL_WINDOW {extern HWND _hWndFrame; \
                           PostMessage(_hWndFrame,WM_CLOSE,0,0L);}
#else
#define WIN16_FIX_WINDOW 
#define WIN16_KILL_WINDOW
#endif
#else
#define WIN16_SETEXIT_NOPERSIST 
#define WIN16_SETEXIT_PERSIST   
#define WIN16_SOMEXIT          
#define WIN16_FIX_WINDOW
#define WIN16_KILL_WINDOW 
#endif


#if defined(_WIN16) && defined(WIN16_OUTCHAR_OVERRIDES)
#define WIN16_OUTCHAR_SCREEN somSetOutChar(OutCharRoutineScreen);
#define WIN16_OUTCHAR_FILE   somSetOutChar(OutCharRoutineFile);
#define OUTCHAR_BUF 256
static char msgbuf[OUTCHAR_BUF];
static int msgindex=0;

#ifdef WIN16_OUTCHAR_FILENAME
char outcharfilename[]=WIN16_OUTCHAR_FILENAME;
#else
char outcharfilename[]="w16chout.fil";
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <share.h>

int _loadds OutCharRoutineScreen(char c)	/* Override outchar routine to screen */
{												 
    if (c == '\r')
      return 0;
    msgbuf[msgindex++] = c;
    if (c == '\0' || msgindex==OUTCHAR_BUF-1 )
    {
      msgbuf[msgindex] = '\0';
      printf("%s",msgbuf);
      msgindex = 0;
	 }

    return 0;
}

int _loadds OutCharRoutineFile(char c)	/* Override outchar routine to file */
{
    static FILE *pfFile;
    int rc;
	 
    if (c == '\r')
      return 1;
    msgbuf[msgindex++] = c;
    if (c == '\0' || msgindex==OUTCHAR_BUF-1 )
    {
      if ((pfFile=_fsopen(outcharfilename, "a", _SH_DENYNO)) == NULL)
          return 1; 

      msgbuf[msgindex] = '\0';
      rc = fputs(msgbuf, pfFile);
      printf("%s",msgbuf);
      msgindex = 0;

	   fclose(pfFile);             /* Close when newline encountered */
	 }

    return (rc == -1 ? 1 : 0);      /* if -1 then error. 0 = success */
}

#else
#define WIN16_OUTCHAR_SCREEN
#define WIN16_OUTCHAR_FILE  
#endif


#endif /* SOMPORT.H  No more after here */
