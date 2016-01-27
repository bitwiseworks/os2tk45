/*static char *SCCSID = "@(#)sort.h	6.15 92/02/27";*/
/*==============================================================*\
 *                                                              *
 *  SORT.H - Sample PM application sort header file             *
 *     (C) Copyright IBM Corporation 1992.                      *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This header file contains the application wide constants    *
 *  and structure definitions.                                  *
 *                                                              *
\*==============================================================*/

/*--------------------------------------------------------------*\
 * Resource IDs                                                 *
\*--------------------------------------------------------------*/
#define ID_NULL                               -1
#define ID_RESOURCE                            1
#define IDR_BITMAP                             2
#define IDC_BITMAP                             3

/*--------------------------------------------------------------*\
 *  Menu item IDs                                               *
\*--------------------------------------------------------------*/
#define IDM_START                              5
#define IDM_STOP                               6
#define IDM_SET                                7
#define IDM_RANDOM                             8
#define IDM_DONE        (WM_USER+0) /* Message posted when thread terminates */

#define IDM_GENERALHELP                     9100
#define IDM_USINGHELP                       9200
#define IDM_TUTORIAL                        9300
#define IDM_HELPINDEX                       9400
#define IDM_HELPPRODUCTINFO                 9500

/*--------------------------------------------------------------*\
 *  Help table and subtables                                    *
\*--------------------------------------------------------------*/
#define SORT_HELP_TABLE                     1000

/*--------------------------------------------------------------*\
 *  Main window help panels                                     *
\*--------------------------------------------------------------*/
#define SUBTABLE_MAIN                       2000
#define PANEL_MAIN                          2100
#define PANEL_HELP                          2910
#define PANEL_GENERALHELP                   2920
#define PANEL_USINGHELP                     2930
#define PANEL_TUTORIAL                      2940
#define PANEL_HELPINDEX                     2950
#define PANEL_HELPPRODUCTINFO               2960
#define PANEL_KEYSHELP                      2970

#define PANEL_HELPSTART                     2980
#define PANEL_HELPSTOP                      2981
#define PANEL_HELPSET                       2982
#define PANEL_HELPRANDOM                    2983

/*--------------------------------------------------------------*\
 *  Enter text dialog help subtable                             *
\*--------------------------------------------------------------*/
#define SUBTABLE_PRODUCTINFODLG             3000
#define PANEL_PRODUCTINFODLG                3100
#define PANEL_PRODUCTINFO_OK                3110

/*--------------------------------------------------------------*\
 *  Message item IDs                                            *
\*--------------------------------------------------------------*/
#define IDD_SETCOUNT                          10
#define IDD_ENTRYFLD                          11
#define IDD_PRODUCTINFO                       12
#define IDD_MSGBOX                            13

/*--------------------------------------------------------------*\
 *  Stringtable ids
\*--------------------------------------------------------------*/
#define IDS_TITLE                            101
#define IDS_UNTITLED                         102
#define IDS_APPNAME                          103
#define IDS_HELPLIBRARYNAME                  104
#define IDS_HELPWINDOWTITLE                  105

/*--------------------------------------------------------------*\
 *  Messagetable ids                                            *
\*--------------------------------------------------------------*/
#define IDMSG_INITFAILED                     301
#define IDMSG_MAINWINCREATEFAILED            302
#define IDMSG_CANNOTLOADEXITLIST             304
#define IDMSG_CANNOTLOADSTRING               305
#define IDMSG_HELPLOADERROR                  306
#define IDMSG_HELPDISPLAYERROR               307
#define IDMSG_ERRORSIZE                      308

/*--------------------------------------------------------------*\
 *  Stringtable ids                                             *
\*--------------------------------------------------------------*/
#define IDS_HELPLIBRARYNAME                  104
#define IDS_HELPWINDOWTITLE                  105

/* This structure is passed into various routines and contains various
   information, often depending on who is passing the information.
   hwnd holds the main window handle for routines that need to draw.
   fContinueCalc is used by routines that have the ability to abort
         when the calling routine sets this flag to FALSE.
   usID is set to the offset number of a thread and is used to pass
         a message back to the window proc when the thread terminates.
   (*pFunc)() passes the function to be called to the generic thread
         starting routine CalcThread.
   Array points to the beginning of the data set to be used by the
         various sorts.  This pointer is used in the Display thread
         to point to the variable that is dynamically modified to
         contain the current height of the client window.
   cArray is the number of elements in Array.  It serves no purpose in
         the Display thread.
   pcSetSize is used only by the Display thread to point to a dynamically
         modified variable which contains the user-modifiable data
         set size.                                                    */

typedef struct _CalcParam
{
   HWND   hwnd;
   BOOL   fContinueCalc;
   BYTE   *pcThreadCnt;
   USHORT usID;
   VOID   (*pFunc)(struct _CalcParam *);
   USHORT *Array;
   USHORT cArray;
   USHORT *pcSetSize;
} CALCPARAM;
typedef CALCPARAM FAR *PCALCPARAM;

#define STACKSIZE        16384    /* Arbitrary size for a stack     */
#define LISTCNT              4    /* Number of data sets            */


#define MAX_ELEMS       1024
#define NELEMS          (rcl.xRight * rcl.yTop)/* Number of items in a data set  */


#define XOFFSET             10    /* Pixel offset for axis in X dir */
#define YOFFSET             40    /* Pixel offset for axis in Y dir */

#define COLUMNOFFSET       125    /* Column offset for sort names   */
#define ROWOFFSET           18    /* Row offset for sort names      */
#define COLUMNCNT            4    /* Max # of text columns          */
#define ROWCNT               2    /* Max # of text rows             */
#define HELPLIBRARYNAMELEN  20    /* The length of library name     */
#define MESSAGELEN          50      /* maximum length for messages  */
#define RETURN_ERROR         1      /* Error return from procedure  */
#define RETURN_SUCCESS       0      /* Error return from procedure  */

/*--------------------------------------------------------------*\
 *  Macro constants
\*--------------------------------------------------------------*/
/* Macro for drawing a rectangle.  Assumes hps and ptl structs are defined
   in the calling module. */

#define DrawRect(x1,y1,x2,y2,color)  ptl.x = (LONG) (x1);             \
                                     ptl.y = (LONG) (y1);             \
                                     GpiSetCurrentPosition(hps,&ptl); \
                                     ptl.x = (LONG) (x2);             \
                                     ptl.y = (LONG) (y2);             \
                                     GpiSetColor(hps,color);          \
                                     GpiBox(hps,DRO_FILL,&ptl,0L,0L);

/* Macro for drawing a line.  Assumes hps and ptl structs are defined
   in the calling module. */

#define DrawLine(x1,y1,x2,y2,color)  ptl.x = (LONG) (x1);             \
                                     ptl.y = (LONG) (y1);             \
                                     GpiSetCurrentPosition(hps,&ptl); \
                                     ptl.x = (LONG) (x2);             \
                                     ptl.y = (LONG) (y2);             \
                                     GpiSetColor(hps,color);          \
                                     GpiLine(hps,&ptl);

/* Macro for drawing a point.  Assumes hps and ptl structs are defined
   in the calling module. */

#define Draw2Pel(x1,y1,color)        ptl.x = (LONG) (x1);             \
                                     ptl.y = (LONG) (y1);             \
                                     GpiSetColor(hps,color);          \
                                     GpiSetPel(hps,&ptl);             \
                                     ptl.y++;                         \
                                     GpiSetPel(hps,&ptl);
