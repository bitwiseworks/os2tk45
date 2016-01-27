/*************************************************************************
 *
 * HIWORLD is a simple Win32 application that displays the text
 * "Hello, World" centered in the client area of the main window.
 *
 * The HIWORLD window contains a menu with two selections:
 *
 *      Information: Displays popup allowing user to:
 *                      view time (local and system up time)
 *                      view screen dimensions (mm and pixels)
 *                      manipulate colors (change, view, and restore menu colors)
 *                      exit the application
 *
 *      Beep: Displays a dialog in which the user selects a frequency
 *                      and a duration via sliders.  When the user clicks on the
 *                      BEEP button, the system beeps using the specified duration
 *                      and frequency.  When the user clicks on DONE or double-clicks
 *                      on the title bar icon, the dialog box terminates.
 *
 *************************************************************************/


/* include files */
#include <string.h>
#include <stdio.h>
#include "hiworld.h"

// under OS/2 you must include os2win.h in place of windows.h
#ifdef WIN32
#include <windows.h>
#else
#include <os2win.h>
#endif

/* function prototypes */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL WINAPI BeepDiaProc(HWND, UINT, WPARAM, LPARAM);
void RGB2Text(DWORD , char *);



/* variables needed by all of the routines */
char szClassName[] = "HiClass";       // class name
char szProgName[] = "Hi, World";      // name of this progam
char lpstrHiWorld[] = "Hello, World!";// text to be placed in window
int newX = 100;                       // x coordinate for window
int newY = 100;                       // y coordinate for window

//
// accelerator table.  There are 3 accelerators: (chars can be upper or lower)
//      F2 - generates an IDM_UPTIME message to display the uptime message box
//      ALT+L - generates an IDM_LOCALTIME message to display the local time
//      ALT+X - exits the program
//
#define NUMBER_OF_ACCELTABLE_ENTRIES 5
ACCEL accelTable[NUMBER_OF_ACCELTABLE_ENTRIES] = {
      {FVIRTKEY,VK_F2,IDM_UPTIME}, // F2 = up time display
      {FALT,'L',IDM_LOCALTIME},    // alt+L = local time
      {FALT,'l',IDM_LOCALTIME},    // alt+l = local time
      {FALT,'X',IDM_EXIT},         // alt+X = exit
      {FALT,'x',IDM_EXIT}};        // alt+x = exit
HACCEL hMyAccelTable;              // pointer to accelerator table




/*
 * WinMain - main program
 */

int WINAPI WinMain (HINSTANCE hInst,
                    HINSTANCE hPreInst,
                    LPSTR lpszCmdLine,
                    int nCmdShow)
{
        MSG lpMsg;
        WNDCLASS wcApp;
        ATOM classAtom;
                HWND hMainWindow;


        if (!hPreInst) {
           wcApp.lpszClassName = szClassName;   // name of this class
           wcApp.hInstance = hInst;             // instance handle
           wcApp.lpfnWndProc = WndProc;         // window procedure
           wcApp.hCursor =                      // load custom cursor
                  LoadCursor(hInst,MAKEINTRESOURCE(IDC_CURSOR1));
           wcApp.hIcon =                        // load custom icon
              LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON2));
           wcApp.lpszMenuName =
              MAKEINTRESOURCE(HIWORLDMENU);     // menu identifier
           wcApp.hbrBackground =                // background is white
              GetStockObject(WHITE_BRUSH);
           wcApp.cbClsExtra = 0;                // no class words
           wcApp.cbWndExtra = 0;                // no window words
           wcApp.style = CS_HREDRAW|CS_VREDRAW; // redraw on horizontal and vertical changes

           classAtom = RegisterClass(&wcApp);   // register the window class
           if (classAtom==0) {                  // was the register successful?
              return FALSE;                     // return to caller
                  }
        }

        hMainWindow = CreateWindow(szClassName, // class name
                            szProgName,         // window name
                            WS_OVERLAPPEDWINDOW,// window style
                            newX,               // x position
                            newY,               // y position
                            3*newX,             // width
                            3*newY,             // height
                            (HWND) NULL,        // parent handle - not used
                            (HMENU) NULL,       // menu handle - use class menu
                            (HANDLE) hInst,     // instance handle
                            (LPVOID) NULL);     // window creation data - not used

        ShowWindow(hMainWindow,nCmdShow);       // set show state
        UpdateWindow(hMainWindow);              // update client area of the window

        hMyAccelTable =                         // create accel table
           CreateAcceleratorTable(accelTable,NUMBER_OF_ACCELTABLE_ENTRIES);

        // standard message loop with accelerator handling
        while (GetMessage(&lpMsg,NULL,0,0)) {
                if (!TranslateAccelerator(hMainWindow,hMyAccelTable,&lpMsg)) {
                        TranslateMessage(&lpMsg);
                        DispatchMessage(&lpMsg);
                }
        }

        DestroyAcceleratorTable(hMyAccelTable); // cleanup accel table

        return (lpMsg.wParam);                  // return to caller
}



/*********************************************************************************************
 *
 * This is the window procedure for the main window.
 *
 *      Message:                Action:
 *      ========                =======
 *      WM_CREATE               save hInst for dialog box calls, and save menu and window colors
 *      WM_DESTROY              indicate that the window proc is complete
 *      WM_PAINT                print output message centered in client area
 *      WM_SIZE                 save new client dimensions
 *      WM_COMMAND              IDM_LOCALTIME: display message box with local time
 *                              IDM_UPTIME: display message box with time system has been up
 *                              IDM_SHOWCOLORS: display message box with text representation of
 *                                         menu text and background colors
 *                              IDM_CHANGECOLORS: change menu text and background colors
 *                              IDM_RESTORECOLORS: restore menu text and background colors to their
 *                                         original values
 *                              IDM_SCRDIM: display message box with screen dimensions
 *                              IDM_BEEP: display beep dialog
 *                              IDM_EXIT: leave the program
 *
 *********************************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd,
                 UINT messg,
                 WPARAM wParam,
                 LPARAM lParam)
{
    static HINSTANCE hInst;           // used for dialog box
    static BOOL bNeedToRestoreColors; // flag to do cleanup
    HDC hdc;                          // device context for painting
    PAINTSTRUCT ps;                   // paint structure for painting
    DWORD dElapsedTimeSysHasBeenUp;   // time (ms) since system was booted
    DWORD dOtherTime;                 // another variable to hold time
    char lpstrOutString[256];         // string for message boxes
    SIZE mySize, *lpSize=&mySize;     // used to calculate coordinates to center text
    SYSTEMTIME SysTime;               // system time
    SYSTEMTIME *lpSysTime = &SysTime; // pointer to previous
    DWORD dwCheckColorInfo[2];        // holds color information
    DWORD dwNewColorInfo[2] =         // colors to change (red windows, green menus)
        {RGB(255,0,0),RGB(0,255,0)};
    static DWORD dwSavedColorInfo[2]= // saved colors
        {0,0};
    static int iDisplayElements[2] =  // elements to change
        {COLOR_MENUTEXT, COLOR_MENU};
    int i;                            // temp
    int hsize,vsize,hres,vres;        // screen dimensions

    switch (messg) {
        /*
         * WM_CREATE: get instance handle for later use putting up
         *            dialog box.  Get initial colors for windows and menus
         *            so they can be restored upon exit (unless the user
         *            remembers to do it).
         */
        case WM_CREATE:
            hInst = ((LPCREATESTRUCT) lParam) -> hInstance;
            dwSavedColorInfo[0] = GetSysColor(COLOR_MENUTEXT);
            dwSavedColorInfo[1] = GetSysColor(COLOR_MENU);
            bNeedToRestoreColors = FALSE;
            break;
        /*
         * WM_DESTROY: If user forgot to restore colors, do it.  Then
         *                         terminate this window.
         */
        case WM_DESTROY:
            if (bNeedToRestoreColors) {
                SetSysColors(2,iDisplayElements,dwSavedColorInfo);
                bNeedToRestoreColors = FALSE;
            }
            PostQuitMessage(0);
            break;
        /*
         * WM_PAINT: Put the text string into the window, at the center
         *           of the client area.
         */
        case WM_PAINT:
            hdc = BeginPaint(hWnd,&ps);
            GetTextExtentPoint(hdc,lpstrHiWorld,strlen(lpstrHiWorld),lpSize);
            TextOut(hdc,(newX-(lpSize->cx))/2, (newY-(lpSize->cy))/2, lpstrHiWorld,strlen(lpstrHiWorld));
            EndPaint(hWnd,&ps);
            break;
        /*
         * WM_SIZE: save new x and y coordinates to use later in
         *          centering
         */
        case WM_SIZE:
            newX = LOWORD(lParam);
            newY = HIWORD(lParam);
            break;
        /*
         * WM_COMMAND: see individual items
         */
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                /*
                 * IDM_LOCALTIME: Get the local time, put it into a string,
                 *                then output it via a message box
                 */
                case IDM_LOCALTIME:
                    GetLocalTime(lpSysTime);
                    sprintf(lpstrOutString,"Today is \n\t%02d/%02d/%02d\n and the time is \n\t%02d:%02d:%02d",
                            lpSysTime->wMonth,
                            lpSysTime->wDay,
                            lpSysTime->wYear-1900,
                            lpSysTime->wHour,
                            lpSysTime->wMinute,
                            lpSysTime->wSecond);
                    MessageBox(hWnd,lpstrOutString,"Local time",MB_OK);
                    break;
                /*
                 * IDM_UPTIME: Get the current time, put it into a string,
                 *             then output it via a message box
                 */
                case IDM_UPTIME:
                    dElapsedTimeSysHasBeenUp = GetCurrentTime();
                    dOtherTime = timeGetTime();
                    sprintf(lpstrOutString,"System has been up for:\n\t%d milliseconds (GetCurrentTime)\n\t%d milliseconds (timeGetTime)",
                            dElapsedTimeSysHasBeenUp,dOtherTime);
                    MessageBox(hWnd,lpstrOutString,"Time since system was booted",MB_OK);
                    break;
                /*
                 * IDM_SHOWCOLORS: Get the current menu text and menu background
                 *                 colors.  Put them into a text string then
                 *                 output them via a message box.
                 */
                case IDM_SHOWCOLORS:
                    i = 0;
                    dwCheckColorInfo[0] = GetSysColor(COLOR_MENUTEXT);
                    strcpy(lpstrOutString,"Menu text is ");
                    i += strlen(lpstrOutString);
                    RGB2Text(dwCheckColorInfo[0],&lpstrOutString[i]);
                    dwCheckColorInfo[1] = GetSysColor(COLOR_MENU);
                    strcat(lpstrOutString,"Menu background is ");
                    i = strlen(lpstrOutString);
                    RGB2Text(dwCheckColorInfo[1],&lpstrOutString[i]);
                    MessageBox(hWnd,lpstrOutString,"Color Information",MB_OK);
                    break;
                /*
                 * IDM_CHANGECOLORS: Enable the "restore" menu item.  Set flag to
                 *                   ensure that we clean up, then change the
                 *                   menu text and menu background colors.
                 */
                case IDM_CHANGECOLORS:
                    EnableMenuItem(GetMenu(hWnd),IDM_RESTORECOLORS,MF_ENABLED|MF_BYCOMMAND);
                    bNeedToRestoreColors = TRUE;
                    SetSysColors(2,iDisplayElements,dwNewColorInfo);
                    break;
                /*
                 * IDM_RESTORECOLORS: Disable the "restore" menu item.  Clear the
                 *                    cleanup flag, then change the
                 *                    menu text and menu background colors
                 *                    back to their original values.
                 */
                case IDM_RESTORECOLORS:
                    EnableMenuItem(GetMenu(hWnd),IDM_RESTORECOLORS,MF_GRAYED|MF_BYCOMMAND);
                    bNeedToRestoreColors = FALSE;
                    SetSysColors(2,iDisplayElements,dwSavedColorInfo);
                    break;
                /*
                 * IDM_SCRDIM: Get the screen dimensions, put them into a string,
                 *             then output it via a message box
                 */
                case IDM_SCRDIM:
                    hdc = GetDC(hWnd);
                    hsize = GetDeviceCaps(hdc,HORZSIZE);
                    vsize = GetDeviceCaps(hdc,VERTSIZE);
                    hres = GetDeviceCaps(hdc,HORZRES);
                    vres = GetDeviceCaps(hdc,VERTRES);
                    sprintf(lpstrOutString,
                            "Horizontal size:\t\t%d mm\nVertical Size:\t\t%d mm\nHorizontal resolution:\t%d pixels\nVertical Resolution:\t%d pixels",
                            hsize,vsize,hres,vres);
                    ReleaseDC(hWnd,hdc);
                    MessageBox(hWnd,lpstrOutString,"Screen Dimensions",MB_OK);
                    break;
                /*
                 * IDM_BEEP: Display beep dialog box.
                 */
                case IDM_BEEP:
                    DialogBox(hInst,MAKEINTRESOURCE(BEEPDIALOG),hWnd,BeepDiaProc);
                    break;
                /*
                 * WM_DESTROY: If user forgot to restore colors, do it.  Then
                 *                 terminate this window.
                 */
                case IDM_EXIT:
                    if (bNeedToRestoreColors) {
                        SetSysColors(2,iDisplayElements,dwSavedColorInfo);
                        bNeedToRestoreColors = FALSE;
                    }
                    PostQuitMessage(0);
                    break;
                default:
                    return(DefWindowProc(hWnd,messg,wParam,lParam));
                }
                break;
        default:
            return(DefWindowProc(hWnd,messg,wParam,lParam));
        }
        return(0L);
}

/*********************************************************************************************
 *
 * This utility routine changes an RGB value into a character string.
 *
 *********************************************************************************************/

void RGB2Text(DWORD dwRGB, char *str)
{

        DWORD Red, Green, Blue;

    Red =   GetRValue(dwRGB);
    Green = GetGValue(dwRGB);
    Blue =  GetBValue(dwRGB);
    if ((Red!=0) && (Blue==0) && (Green==0)) {
        strcpy(str,"Red\n");
    } else if ((Red==0) && (Blue!=0) && (Green==0)) {
        strcpy(str,"Blue\n");
    } else if ((Red==0) && (Blue==0) && (Green!=0)) {
        strcpy(str,"Green\n");
    } else if ((Red!=0) && (Blue!=0) && (Green==0)) {
        strcpy(str,"Purpleish\n");
    } else if ((Red!=0) && (Blue==0) && (Green!=0)) {
        strcpy(str,"Yellowish\n");
    } else if ((Red==0) && (Blue!=0) && (Green!=0)) {
        strcpy(str,"Aquaish\n");
    } else if ((Red!=0) && (Red==Blue) && (Blue==Green)) {
        strcpy(str,"White\n");
    } else if (Red+Blue+Green==0) {
        strcpy(str,"Black\n");
    } else {
        strcpy(str,"Mixed\n");
    }
}

/*********************************************************************************************
 *
 * This is the dialog procedure for the beep dialog.
 *
 *      Message:                Action:
 *      ========                =======
 *      WM_INITDIALOG           initialize beep duration and frequency
 *      WM_CLOSE                end this dialog
 *      WM_PAINT                display text strings under each scroll bar to represent
 *                              the value of the slider
 *      WM_HSCROLL              adjust scroll bars and duration/frequency values
 *      WM_COMMAND              ID_BEEPNOW:  beep with the specified frequency/duration
 *                              ID_DONE: dismiss dialog box
 *
 *********************************************************************************************/
BOOL WINAPI BeepDiaProc(HWND hDlg, UINT messg, WPARAM wParam, LPARAM lParam)
{

        char lpstrOutString[256];        // string used to display text in dialog box
    RECT rect;
    HDC hdc;                                         // device context for painting
    PAINTSTRUCT ps;                          // paint structure for painting
    SIZE mySize;                 // used to place text in dialog box
    ULONG ulDlgUnitsToPixels;        // used to place text in dialog box
    static LONG ulBeepDuration; // beep duration - selected by slider
    static LONG ulBeepFreq;     // beep frequency - selected by slider
    LONG *pscrollpos;           // current slider value
    int id;                      // determines which slider
    char *templateStringMS = "Selected Value: %5d ms   ";
    char *templateStringHZ = "Selected Value: %5d hz   ";

    switch (messg) {
        /*
         * WM_INITDIALOG: initialize duration and frequency variables
         */
        case WM_INITDIALOG:
            ulBeepDuration = MINDURATION;
            ulBeepFreq = MINFREQ;
            break;
        /*
         * WM_CLOSE: Dismiss this dialog
         */
        case WM_CLOSE:
            EndDialog(hDlg,FALSE);
            break;
        /*
         * WM_PAINT: Determine position in dialog box for text output and
                 *           put the text strings there
         */
        case WM_PAINT:
            hdc = BeginPaint(hDlg,&ps);
            ulDlgUnitsToPixels = GetDialogBaseUnits();
            sprintf(lpstrOutString,templateStringHZ,ulBeepFreq*FREQSTEP);
            TextOut(hdc,(40*LOWORD(ulDlgUnitsToPixels))/4,(40*HIWORD(ulDlgUnitsToPixels))/8,lpstrOutString,strlen(lpstrOutString));
            sprintf(lpstrOutString,templateStringMS,ulBeepDuration*DURATIONSTEP);
            TextOut(hdc,(40*LOWORD(ulDlgUnitsToPixels))/4,(100*HIWORD(ulDlgUnitsToPixels))/8,lpstrOutString,strlen(lpstrOutString));
            EndPaint(hDlg,&ps);
            break;
        /*
         * WM_HSCROLL: handle horizontal scrolling.  For all scroll
         *             messages, determine which scroll bar this is (via GetDlgCtrlId)
         *             and set a pointer to the scroll value (pscrollpos).
         */
        case WM_HSCROLL:
            ulDlgUnitsToPixels = GetDialogBaseUnits();
            id = GetDlgCtrlID((HWND)lParam);
            hdc = GetDC(hDlg);
            GetTextExtentPoint(hdc,templateStringHZ,strlen(templateStringHZ),&mySize);
            DeleteDC(hdc);
            if (id==ID_FREQBAR) {
                SetScrollRange((HWND)lParam,SB_CTL,MINFREQ,MAXFREQ,TRUE);
                pscrollpos = &ulBeepFreq;
                rect.top = (40*HIWORD(ulDlgUnitsToPixels))/8;
                rect.bottom = rect.top + mySize.cy;
                rect.left = (40*LOWORD(ulDlgUnitsToPixels))/4;
                rect.right = rect.left + mySize.cx;
            } else {
                SetScrollRange((HWND)lParam,SB_CTL,MINDURATION,MAXDURATION,TRUE);
                pscrollpos = &ulBeepDuration;
                rect.top = (100*HIWORD(ulDlgUnitsToPixels))/8;
                rect.bottom = rect.top + mySize.cy;
                rect.left = (40*LOWORD(ulDlgUnitsToPixels))/4;
                rect.right = rect.left + mySize.cx;
            }
            switch(LOWORD(wParam)) {
                /*
                 * SB_LINERIGHT: user incremented by 1.  Update scroll
                 *               position and redraw.
                 */
                case SB_LINERIGHT:
                    (*pscrollpos)++;
                    if ((id==ID_FREQBAR) && (*pscrollpos > MAXFREQ)) {
                       *pscrollpos = MAXFREQ;
                    } else if ((id==ID_DURBAR) && (*pscrollpos > MAXDURATION)){
                       *pscrollpos = MAXDURATION;
                    }
                    SetScrollPos((HWND)lParam,SB_CTL,*pscrollpos,TRUE);
                    break;
                /*
                 * SB_LINELEFT: user decremented by 1.  Update scroll
                 *              position and redraw.
                 */
                case SB_LINELEFT:
                    (*pscrollpos)--;
                    if ((id==ID_FREQBAR) && (*pscrollpos < MINFREQ)) {
                       *pscrollpos = MINFREQ;
                    } else if ((id=ID_DURBAR) && (*pscrollpos < MINDURATION)) {
                       *pscrollpos = MINDURATION;
                    }
                    SetScrollPos((HWND)lParam,SB_CTL,*pscrollpos,TRUE);
                    break;
                /*
                 * SB_LINELEFT: user decremented by 1 "page".  Update scroll
                 *              position and redraw.
                 */
                case SB_PAGELEFT:
                    *pscrollpos -= BEEPSCROLLWIDTH;
                    if ((id==ID_FREQBAR) && (*pscrollpos < MINFREQ)) {
                       *pscrollpos = MINFREQ;
                    } else if ((id=ID_DURBAR) && (*pscrollpos < MINDURATION)) {
                       *pscrollpos = MINDURATION;
                    }
                    SetScrollPos((HWND)lParam,SB_CTL,*pscrollpos,TRUE);
                    break;
                /*
                 * SB_LINERIGHT: user incremented by 1 "page".  Update scroll
                 *               position and redraw.
                 */
                case SB_PAGERIGHT:
                    *pscrollpos += BEEPSCROLLWIDTH;
                    if ((id==ID_FREQBAR) && (*pscrollpos > MAXFREQ)) {
                       *pscrollpos = MAXFREQ;
                    } else if ((id==ID_DURBAR) && (*pscrollpos > MAXDURATION)){
                       *pscrollpos = MAXDURATION;
                    }
                    SetScrollPos((HWND)lParam,SB_CTL,*pscrollpos,TRUE);
                    break;
                /*
                 * SB_THUMBPOSITION: user moved to a specified position.  Update scroll
                 *                   position and redraw.
                 * SB_THUMBTRACK: same as SB_THUMBPOSITION
                 */
                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    *pscrollpos = HIWORD(wParam);
                    SetScrollPos((HWND)lParam,SB_CTL,*pscrollpos,TRUE);
                    break;
            }
            InvalidateRect(hDlg,&rect,FALSE);
            break;
        /*
         * WM_COMMAND: see individual actions
         */
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                /*
                 * ID_BEEPNOW: use Beep API to make the sound!
                 */
                case ID_BEEPNOW:
                    Beep(ulBeepFreq*FREQSTEP,ulBeepDuration*DURATIONSTEP);
                    break;
                /*
                 * ID_DONE: dismiss this dialog
                 */
                case ID_DONE:
                    EndDialog(hDlg,TRUE);
                    break;
                default:
                    return FALSE;
            }
            default:
                return FALSE;
        }
        return TRUE;
}
