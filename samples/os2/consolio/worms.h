/*=========================================================================\
 *                                                                         *
 *       FILE:worms.h                                                      *
 *                                                                         *
 *       DESCRIPTION: This is the header file for the c module worms.c     *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *      Created 1991  IBM Corp.                                            *
 *                                                                         *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is        *
 *      sample code created by IBM Corporation and/or Microsoft            *
 *      Corporation. This sample code is not part of any standard          *
 *      Microsoft or IBM product and is provided to you solely for         *
 *      the purpose of assisting you in the development of your            *
 *      applications.  The code is provided "AS IS", without               *
 *      warranty of any kind.  Neither IBM or Microsoft  shall be          *
 *      liable for any damages arising out of your use of the sample       *
 *      code, even if they have been advised of the possibility of         *
 *      such damages.                                                      *
 *                                                                         *
 *-------------------------------------------------------------------------*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *--------------------------------------------------------------
 *
 *  This header file  is for the source  module: worms.c
 *
 *
 *
 *
 *
\*==============================================================*/

/*--------------------------------------------------------------*\
 *  Global macros, defined constants, and externs
\*--------------------------------------------------------------*/


typedef struct threaddata
{
     LONG     tidWorm;
     BOOL     fActive;
     USHORT   usCurrentRow;
     USHORT   usCurrentCol;
     BYTE     bAttribute;
     SHORT    sCurrentDirection;
}THREAD_DATA,*PTHREAD_DATA;


/*--------------------------------------------------------------*\
 *  Global variables  and definitions for this file
\*--------------------------------------------------------------*/

extern INT           StartWorms;
extern THREAD_DATA   ThreadData[];
extern BOOL          fThreadsContinue;
extern INT           CurrentThread;
extern CHAR          *pszWormMessages [];
extern SHORT         sStartWorms;
extern BOOL          fSilent;
extern VIOCURSORINFO viociCursor;
extern BOOL          fQuit;
extern BOOL          fDraw;
extern MOUEVENTINFO mouev;
extern VIOMODEINFO   VioModeInfo;
extern HMOU hmou;
extern HVIO  hvio;
extern BOOL  bHelpEnabled;
#ifndef __HEV__
#include <bsedos.h>
#endif
  extern HEV   hevWormSem;
  extern HEV   hevDrawSem;
  extern HEV   hevDrawOk;
#define WAIT_THREAD_EXIT     500L
#define SLEEP_TIME_THREADS   100L
#define PAUSE_TIME         ( 75L)

#define UP                   1
#define LEFT                 2
#define DOWN                 3
#define RIGHT                4
#define GOBBLE               5
#define ZIG_UP               6
#define BOX                  7
#define ZIG_DOWN             8

#define WM_BLACK             0x00
#define WM_BLUE              0x01
#define WM_GREEN             0x02
#define WM_CYAN              0x03
#define WM_RED               0x04
#define WM_MAGENTA           0x05
#define WM_BROWN             0x06
#define WM_PALEGRAY          0x07
#define WM_DKGREY            0x08
#define WM_LBLUE             0x09
#define WM_LGREEN            0x0A
#define WM_LCYAN             0x0B
#define WM_LRED              0x0C
#define WM_LMAGENTA          0x0D
#define WM_YELLOW            0x0E
#define WM_WHITE             0x0F
#define WM_MAX_COLOR         0X0F
#define TOP_ROW          0                   //Window Frame
#define LEFT_COL         0
#define BOT_ROW          25
#define RGT_COL          80

#define FIRST_ROW        TOP_ROW + 3   //   Drawable Window Coordinates
#define LAST_ROW         VioModeInfo.row
#define FIRST_COL        1
#define LAST_COL         VioModeInfo.col -1
#define WIDTH            2
#define MAX_DIRECTIONS   8
#define HOR_WIDTH        (RGT_COL - LEFT_COL)
#define HEIGHT           (BOT_ROW - TOP_ROW)


#define F1                   0x3B            //Function keys

#define WM_BACKGROUND     (WM_BLUE << 4 ) + WM_WHITE
#define MKATRB(a,b )     ((BYTE) (a << 4 ) + b )

/*
 *global  message/error  numbers
 */

 #define   WRM_NORMAL_EXIT          0
 #define   ERROR                    1
 #define   INIT_FAILURE             2
 #define   MAX_THREADS_EXCEEDED     3
 #define   ERROR_CREATING_THREAD    4
 #define   ERROR_DISP_HELP          5
 #define   ERROR_CREATING_WREADTHRD 6
 #define   ENTER_TO_EXIT            7
 #define   WAITING_THREADS_EXIT     8
 #define   ANY_KEY_TO_CONTINUE      9







/*--------------------------------------------------------------*\
 *  Global Entry point declarations
\*--------------------------------------------------------------*/
extern VOID   ErrorMessage(SHORT sExitStatus,BOOL fBeep);
extern VOID   WormDelete(VOID );
extern BOOL   WormCreate(VOID );
extern VOID   Message(SHORT sErrorMessage,BOOL fBeep,
                      BOOL fKeyBoardWait,BOOL fClsConsole,
                      BOOL fSuspendThreads);

/*--------------------------------------------------------------*\
 *  End of file : worms.h
\*--------------------------------------------------------------*/
