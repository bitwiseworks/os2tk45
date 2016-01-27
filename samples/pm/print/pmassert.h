/****************************************************************************
* OS/2 Sample Print Application PRTSAMP
*
* name: pmassert.h
*
* description: similar to C assert.h, but presents debugging information
*   in a PM message box instead of standard error.
*
*   The pmassert macro works when the symbol NDEBUG is not defined to the
*   compiler; thus, the macro works by default.
*
*   To build a non-debug version of the program, define the symbol NDEBUG
*   with the proper compiler switch, usually /DNDEBUG
*
*   The message box shows the last error in hex. The high word is severity,
*   and the low word is an error code. Refer to pmerr.h for an explanation
*   of the error code.
*
*   Do not pmassert any assignment statements or function calls, for these
*   statements will vanish in the non-debug version of the code. Assert only
*   Boolean expressions you think should always be true.
*
****************************************************************************/

#ifndef NDEBUG

#define pmassert(hab,exp)\
{\
if(!(exp)) {\
  char ebuff[ 64 ]; unsigned long errorid; unsigned long rc;\
  errorid = WinGetLastError( hab ); \
  sprintf( ebuff, "Line %d\nFile %s\nLast Error %p\nExpression %s\n",\
                 __LINE__, __FILE__, errorid, #exp );\
  rc = WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, ebuff,\
                 "Assertion failed. Continue?", 0, MB_YESNO  );\
  if( rc == MBID_NO ) exit( 1 );\
}\
}


#else
  #define pmassert(hab,exp)
#endif









