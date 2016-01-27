/*------------------------------------------------------------------------

   SOURCE FILE NAME:  ultieyes.h

                   Copyright (c) IBM Corporation 1981, 1993
                             All Rights Reserved

--------------------------------------------------------------------------*/

/***************/
/*  Constants  */
/***************/
#define NUMFRAMENUMS    24
#define FILE_BUF_SIZE 4096
#define PI               3.14159
#define IDM_NOFRAME    200
#define IDM_RANDOM     210
#define ID_NOFRAME     100
#define RANDOM_SPEED   5
#define DEFAULT_EYE    "ultieyes.eye"
CHAR    szMinor[]   =  "POINTL";
#define ID_ICON        100

/***************************/
/*  Function Declarations  */
/***************************/
MRESULT EXPENTRY  MainClientWindowProc( HWND hwnd, ULONG msg,
                                        MPARAM mp1, MPARAM mp2 );
VOID  ParseControlFile( HFILE FileHandle, ULONG * ulFrameNums);
ULONG CalcFrame( double Angle);
BOOL  CrossEyed( PPOINTL pptlMouse, LONG lWidth);
BOOL  SendString( HWND hwnd, PCHAR pcMCIString, USHORT usUserParm );
VOID  ShowMCIErrorMessage( ULONG ulError, HWND hwnd, PSZ pszCmd );
VOID  CloseTheDevice( VOID );
BOOL  OpenTheDevice( HWND );
BOOL  LoadTheFile( HWND hwnd );
LONG  SetWindowHandle( HWND hwnd);
VOID  StartWait(HWND hwnd);
VOID  StopWait(HWND hwnd);
BOOL  QueryDirectMode( VOID );
