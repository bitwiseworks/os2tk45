/* Copyright (C) International Business Machines Corp., 1995.               */
/* Browser Program                                                          */
/*   Browser will browse selected files in a Multiple Document              */
/*   Interface (MDI) environment.                                           */

/* Include files */
#include <os2win.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "browser.h"

/* Function prototypes */
LRESULT CALLBACK FrameWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK CloseChildren(HWND, LPARAM);
BOOL CALLBACK DeleteProperties(HWND, LPCTSTR, HANDLE);

BOOL DlgAskFile(HWND, LPSTR, LPSTR);


/* Identifying strings */
char szFrameName[] = "Browser";                /* Frame Class name          */
char szChildName[] = "BrowseFile";             /* Child Class name          */

HANDLE hInstance;                              /* Global application inst.  */
HMENU hClientMenu, hChildMenu;                 /* Menu handles for windows  */

/* Structures */
struct scrolldata                              /* Structure used by each    */
{                                              /* child wndw for scrolling  */
  int numlines;                                /* Number lines inside file  */
  int yPos;                                    /* Vertical scroll box pos   */
  int xPos;                                    /* Horizontal scroll box pos */
  int xChildw;                                 /* Width of child window.    */
  int yChildw;                                 /* Height of child window.   */
  int yunit;                                   /* Vertical size of charac.  */
  int xunit;                                   /* Horizontal size of charac */
  CHOOSEFONT cf;
  LOGFONT lf;
};

/* Main Program */
int WINAPI WinMain(HINSTANCE hInst,             /* Current instance of app  */
                   HINSTANCE hPreInst,
                   LPSTR lpszCmdLine,
                   int nCmdShow)
{
  HWND hFrameWnd, hClientWnd;                   /* Window handle            */
  MSG lpMsg;                                    /* Message structure        */
  WNDCLASS wcApp;                               /* Window Class Structure   */
  LPSTR CmdLine;                                /* Cmd Line gave file name  */
  hInstance = hInst;                            /* Save a global for later. */

  if (!hPreInst)                                /* If window class not yet  */
  {                                             /* registered, do so.       */
                                                /* Register Frame wndw class*/
    wcApp.lpszClassName = szFrameName;
    wcApp.hInstance = hInst;
    wcApp.lpfnWndProc = FrameWndProc;
    wcApp.hCursor = LoadCursor(NULL,IDC_ARROW);
    wcApp.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wcApp.lpszMenuName = MAKEINTRESOURCE(BROWSERM);
    wcApp.hbrBackground = GetStockObject(WHITE_BRUSH);
    wcApp.cbClsExtra = 0;
    wcApp.cbWndExtra = 0;
    wcApp.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&wcApp))
      return FALSE;

                                                /* Register Child wndw class*/
    wcApp.lpszClassName = szChildName;
    wcApp.lpfnWndProc = ChildWndProc;
    wcApp.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wcApp.lpszMenuName = NULL;                  /* Child wndws should have  */
                                                /* NULL as menu name.       */
    wcApp.cbWndExtra = _MAX_PATH;               /* Reserving extra space in */
                                                /* wndw struc to associate  */
                                                /* path name of browsed file*/
                                                /* w/ particular child wndw.*/

    if (!RegisterClass(&wcApp))
      return FALSE;
  }
                                                                                                 /* Get main menu handle for */
  hClientMenu = LoadMenu(hInst, MAKEINTRESOURCE(BROWSERM));  /* ... Client  */
  hChildMenu = LoadMenu(hInst, MAKEINTRESOURCE(BROWSFILEM)); /*   ... Child */

                                                /* Create the Frame window. */
  if (!lstrcmp(lpszCmdLine, ""))                /* Determine if Cmd line    */
    CmdLine = NULL;                                                              /* gave filename to browse. */
  else                                          /* Arguement was give...    */
    CmdLine = lpszCmdLine;

  hFrameWnd = CreateWindow(szFrameName,         /* Create wndw of this class*/
                           szFrameName,
                           WS_CLIPCHILDREN |    /* Exclude area occupied by */
                                                /* child window when drawing*/
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,
                           CW_USEDEFAULT,       /* Width                    */
                           CW_USEDEFAULT,       /* Height                   */
                          (HWND) NULL,
                          (HMENU) NULL,         /* Handle of menu           */
                          (HANDLE) hInst,       /* Handle of app. instance  */
                           CmdLine);            /* Window creation data     */


  hClientWnd = GetWindow (hFrameWnd, GW_CHILD); /* Get Client window handle.*/

  ShowWindow(hFrameWnd,nCmdShow);        /* Specifies how window to be shown*/
  UpdateWindow(hFrameWnd);               /* WM_PAINT msg sent to window proc*/

  while (GetMessage(&lpMsg,NULL,0,0))
  {
    if (!TranslateMDISysAccel(hClientWnd, &lpMsg)) /* If not MDI accelerator*/
    {                                           /* msg for child window...  */
      TranslateMessage(&lpMsg);                 /* Translates virtual key to*/
                                                /* character messages.      */
      DispatchMessage(&lpMsg);                  /* Dispatch msg to wndw proc*/
    }
  }

  DestroyMenu(hChildMenu);               /* Release child menu resource     */
                                         /* since not attached to a window. */

  return lpMsg.wParam;
}

/****************************************************************************/
/* Frame window procedure.  This creates the client window and processes    */
/* all menu selections, from either the initial frame menu, BROWSERM, or the*/
/* BROWSFILEM menu which exists when there is a child window.               */
/****************************************************************************/
LRESULT CALLBACK FrameWndProc(HWND hWnd,
                 UINT message,
                 WPARAM wParam,                 /* Unsigned int             */
                 LPARAM lParam)                 /* Long int                 */
{
  static HWND hClientWnd;                       /* Handle of client window. */
  CLIENTCREATESTRUCT clientstruc;
  HWND hChildWnd;                               /* Handle of child window.  */
  char FileName[_MAX_PATH] = {""};
  char TitleName[_MAX_FNAME + _MAX_EXT] = {""};
  char ChildTitle[6 + _MAX_FNAME + _MAX_EXT] = {"Browse "};

  switch (message)
  {
    case WM_CREATE:
                                         /* Create the client window        */
                                         /* Assign handle of Window submenu */
      clientstruc.hWindowMenu = GetSubMenu(hClientMenu, BROWSE_WINPOS);
                                         /* Assign 1st child window id.     */
      clientstruc.idFirstChild = ID_FIRSTCHILD;
                                         /* Create window.                  */
      hClientWnd = CreateWindow("MDICLIENT",    /* Predefined window class  */
                                NULL,           /* Window name              */
                                WS_CHILD | WS_VISIBLE |
                                WS_HSCROLL |    /* Allow for scrolling.     */
                                WS_VSCROLL |
                                WS_CLIPCHILDREN,/* Don't allow client wndw  */
                                                /* to paint over child wndws*/
                                0,0,0,0,
                                hWnd,           /* Handle of parent - frame */
                               (HMENU) 1,       /* Unique child wndw identi.*/
                               (HANDLE) hInstance, /* Hndle of app. instance*/
                               (LPSTR)&clientstruc);/* Window creation data */
      ShowWindow(hClientWnd, SW_SHOW);

      if (((CREATESTRUCT*)lParam)->lpCreateParams != NULL) /* If Cmd line arg*/
      {                                          /* create child wndw immed. */
        lstrcat(ChildTitle,((CREATESTRUCT*)lParam)->lpCreateParams);/* Set up*/
        CharUpper(ChildTitle);                                    /* title.  */
                                                 /* Create child window.     */
        hChildWnd = CreateMDIWindow(szChildName, /* Window class name.       */
                                  ChildTitle,                /* Window title.*/
                                  WS_HSCROLL | WS_VSCROLL,   /* Window style */
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  hClientWnd,    /* Handle of client window. */
                                  hInstance,     /* Handle of app. instance. */
                (LPARAM)((CREATESTRUCT*)lParam)->lpCreateParams);/* Path of  */
                                                             /* file selected*/
                                                 /* to store in window data. */
        SetClassLong(hChildWnd, GCL_HICON,
             (LONG) LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIZZARD)));
      }
      return 0;
      break;

    case WM_COMMAND:
      switch (wParam)
      {
        case IDM_BROWSE:                        /* Browse file selected.    */
                                                /* Ask for file to browse.  */
          if (DlgAskFile(hClientWnd, FileName, TitleName))/* If fle specified*/
          {
                                                /* Create title by concat.  */
            lstrcat(ChildTitle, TitleName);     /* "Browse" with file name. */
            CharUpper(ChildTitle);
                                                /* Create Browse child wndw.*/
            hChildWnd = CreateMDIWindow(szChildName, /* Window class name.  */
                                      ChildTitle,    /* Window title.       */
                                      WS_HSCROLL |   /* Window style        */
                                      WS_VSCROLL,
                                      CW_USEDEFAULT, CW_USEDEFAULT,
                                      CW_USEDEFAULT, CW_USEDEFAULT,
                                      hClientWnd,    /* Handle of client wnd*/
                                      hInstance,     /* Handle of app. inst */
                                     (LPARAM)FileName);  /* Path file select*/
                                                /* to store in window data. */
            SetClassLong(hChildWnd, GCL_HICON,
                 (LONG) LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIZZARD)));
          }
          else                                  /* No file name was selected*/
          {
            MessageBox(hClientWnd,
                       "No File was specified for browsing.",
                       "No File Specified", MB_OK);
          }
          return 0;
          break;

        case IDM_CLOSEF:                        /* Close the active child.  */
                                                /* Get handleof active child*/
          hChildWnd = (HWND)SendMessage(hClientWnd, WM_MDIGETACTIVE, 0, 0L);
                    /* NOTE: the WM_MDIGETACTIVE interface changed for WNT. */
                                                /* If application in child  */
                                                /* is at a pt of termination*/
          if (SendMessage(hChildWnd, WM_QUERYENDSESSION, 0, 0L))
                                                /* then close MDI child.    */
            SendMessage(hClientWnd, WM_MDIDESTROY, (WPARAM)hChildWnd, 0L);
          return 0;
          break;

        case IDM_EXIT:
          SendMessage(hWnd, WM_CLOSE, 0, 0L);
          return 0;
          break;

        case IDM_CASCADE:
          SendMessage(hClientWnd, WM_MDICASCADE, 0, 0L);
          return 0;
          break;

        case IDM_TILEV:
          SendMessage(hClientWnd, WM_MDITILE, (WPARAM)MDITILE_VERTICAL, 0L);
          return 0;
          break;

        case IDM_TILEH:
          SendMessage(hClientWnd, WM_MDITILE, (WPARAM)MDITILE_HORIZONTAL, 0L);
          return 0;
          break;

        case IDM_ARRANGE:
          SendMessage(hClientWnd, WM_MDIICONARRANGE, 0, 0L);
          return 0;
          break;

        case IDM_CLOSEALL:
          EnumChildWindows(hClientWnd, (WNDENUMPROC) CloseChildren, 0L);
          return 0;
          break;

        case IDM_HELP1:                  /* Help for main client window     */
          MessageBox(hClientWnd, "This is an MDI application.  You can select "
                                 "multiple files to browse by selecting File>>"
                                 "Browse File....  Each file selected should "
                                 "be displayed in its own child window. "
                                 "Select File>>Exit to leave the application.",
                                 "Browser Help", MB_OK);
          break;

        case IDM_HELP2:                  /* Help for when child window exits*/
          MessageBox(hClientWnd, "Select File>>Browse File... to open another "
                                 "file in a separate MDI child window to "
                                 "browse.  Select File>>Select Font... to "
                                 "select a new font for the currently active "
                                 "child window.  Select File>>Close to close "
                                 "the currently active child window.  Select "
                                 "File>>Exit to leave the Browser application. "
                                 "Use the Window menu item to arrange or "
                                 "select the existing child windows.",
                                 "Browser Help", MB_OK);

          break;

        default:                         /* Other menu msgs to be processed */
                                         /* by the active child window.     */
                                         /* Get handle of active child wndw.*/
          hChildWnd = (HWND)SendMessage(hClientWnd, WM_MDIGETACTIVE, 0, 0L);
                    /* NOTE: the WM_MDIGETACTIVE interface changed for WNT. */
          if (IsWindow(hChildWnd))
            SendMessage(hChildWnd, WM_COMMAND, wParam, lParam);
          break;
                                         /* Allow to fall through to        */
                                         /* DefFrameProc.                   */
      }
      break;

      case WM_QUERYENDSESSION:
      case WM_CLOSE:
        SendMessage(hWnd, WM_COMMAND, IDM_CLOSEALL, 0L);
        if (NULL != GetWindow(hClientWnd, GW_CHILD))
          return  FALSE;
        break;

      case WM_DESTROY:
        PostQuitMessage(0);             /* Sends WM_QUIT message.           */
        return 0;
        break;

      default:
        break;
  }

  return DefFrameProc(hWnd, hClientWnd, message, wParam, lParam);
}


BOOL CALLBACK CloseChildren(HWND hWnd, LONG lParam)
{
  HWND hParent;                                 /* Handle of parent window  */

  if (GetWindow(hWnd, GW_OWNER))         /* Check for icon title window.    */
                                         /* Non-title windows return NULL.  */
    return TRUE;                         /* Don't do anything if icon title.*/

  hParent = GetParent(hWnd);
                                         /* Restore child from Max/Min size.*/
  SendMessage(hParent, WM_MDIRESTORE, (WPARAM)hWnd, 0L);

                                         /* Is child window in state to end?*/
  if (SendMessage(hWnd, WM_QUERYENDSESSION, 0, 0L))
                                         /* Tell client to destroy this chld*/
    SendMessage(hParent, WM_MDIDESTROY, (WPARAM)hWnd, 0L);

  return TRUE;
}


LRESULT CALLBACK ChildWndProc(HWND hWnd,
                              UINT message,
                              WPARAM wParam,    /* Unsigned int             */
                              LPARAM lParam)    /* Long int                 */
{
  #define LINESIZE 256

  int i,y, x;

  HWND hClientWnd;                              /* Handle of parent-client  */
  HWND hFrameWnd;                               /* Handle of frame window.  */
  HMENU hWindowMenu;                            /* Window hndl of Windowmenu*/
                                                /* within Child Menu bar.   */
  char * pFilePath;                             /* Ptr File Pathname or data*/
  MDICREATESTRUCT * mcs;                        /* For use in WM_CREATE msg */
  LOCALHANDLE hFileName;                        /* Hanle to allocated space */
  HANDLE hFile;                                 /* File handle.             */
  DWORD FileSize, FileRead;                     /* File size, No.bytes read */
  HDC hdc;                                      /* Devic Context Handle.    */

  PAINTSTRUCT ps;                               /* Paint Structurefor disply*/
  static RECT rect;
  TEXTMETRIC tm;
  HFONT hFont;                                  /* Handle to font.          */

  struct scrolldata * psd;                      /* Internal struc. that has */
                                                /* scrolling info. of child */
  int charpos;                                  /* Character positin in file*/
  int clientlines;                              /* # lines to be displayed  */
  char line[LINESIZE];                          /* Line to be displayed     */


  switch (message)
  {
    case WM_CREATE:
    /************************************************************************/
    /* Client processed WM_MDICREATE msg which in turn issued WM_CREATE msg */
    /* to child window.  LParam is a ptr to CREATESTRUCT.                   */
    /* CREATESTRUCT->lpCreateParams is a ptr to an MDICREATESTRUCT.         */
    /************************************************************************/
      mcs = ((CREATESTRUCT *)lParam)->lpCreateParams;
                                         /* Allocate memory for file name.  */
      hFileName = LocalAlloc(LHND, _MAX_PATH);
      pFilePath = LocalLock(hFileName);
                                         /* Copy path name from MDICREATESTR*/
      lstrcpy(pFilePath, (char *)mcs->lParam);
                                         /* Save hndl to pathname in window */
      SetWindowLong(hWnd, GWL_USERDATA, (LONG)hFileName);  /* data.         */
                                                /* Get ptr to path name     */
      /**********************************************************************/
      /* Open the file (via CreateFile) for Read-only and read the data into*/
      /* globally allocated area. If unsuccessful, destroy child window.    */
      /**********************************************************************/
      hFile = CreateFile(pFilePath,             /* Full path to file        */
                         GENERIC_READ,          /* File access specification*/
                         FILE_SHARE_READ,       /* File sharing specificatn */
                         NULL,                  /* Security attributes.     */
                         OPEN_EXISTING,         /* Open file onlyif it exits*/
                         FILE_ATTRIBUTE_READONLY,  /* File attributes       */
                         NULL);
      LocalUnlock(hFileName);
      if (hFile != INVALID_HANDLE_VALUE)        /* No error on file open... */
      {
        FileSize = GetFileSize(hFile, NULL) + 1; /* Get file size.          */
        pFilePath = (unsigned char *)GlobalAlloc(GPTR, FileSize); /* Allocate memory for file*/
        if (pFilePath)                           /* Memory was allocated... */
        {                                        /* Read the file...        */
          if (ReadFile(hFile, pFilePath, FileSize , &FileRead, NULL))
          {
            pFilePath[FileRead + 1] = '\0';      /* Ensure file is null-term*/
            CloseHandle(hFile);                  /* Close the file.         */
            SetProp(hWnd, "FILE_DATA",
                    (HANDLE)pFilePath);          /* Save @ to data as a child*/
                                                /* window property.         */
          }
          else                                  /* Read error...            */
          {
            MessageBox(hWnd, "File Read Error", "File Error",
                                          MB_OK | MB_ICONEXCLAMATION);
                                                /* Kill child window        */
            hClientWnd = GetParent(hWnd);       /* by sending the parent    */
            SendMessage(hClientWnd,             /* client window a message. */
                        WM_MDIDESTROY,          /* This will remove child   */
                        (WPARAM)hWnd, 0L);      /* from window list.        */
            return 0;
          }                                     /* End read error.          */
        }
        else                                    /* Memory was not allocated.*/
        {
          MessageBox(hWnd, "Memory Allocation Problem", "File Error",
                                          MB_OK | MB_ICONEXCLAMATION);
                                                /* Kill child window        */
          hClientWnd = GetParent(hWnd);         /* by sending the parent    */
          SendMessage(hClientWnd,               /* client window a message. */
                      WM_MDIDESTROY,            /* This will remove child   */
                      (WPARAM)hWnd, 0L);        /* from window list.        */
          return 0;
        }                                       /* End memory not allocated.*/
      }
      else                                      /* Error opening file.      */
      {                                         /* Notify user              */
        MessageBox(hWnd, "Cannot Open File!", pFilePath,
                                          MB_OK | MB_ICONEXCLAMATION);
                                                /* Kill child window        */
        hClientWnd = GetParent(hWnd);           /* by sending the parent    */
        SendMessage(hClientWnd,                 /* client window a message. */
                    WM_MDIDESTROY,              /* This will remove child   */
                    (WPARAM)hWnd, 0L);          /* from window list.        */
        return 0;
      }
      /**********************************************************************/
      /* OK...we are able to get the data to display. Get info. on how.     */
      /**********************************************************************/
                                                /* Allocate scrolldata struc*/
      psd = (struct scrolldata *)LocalAlloc(LPTR, sizeof(struct scrolldata));
      SetProp(hWnd, "SCROLL_DATA", (HANDLE)psd);        /* Save address as wndw prop*/
                                                /* Get number of lines      */
      for(psd->numlines = 0, i=0; i < (int)FileRead; i++)/* in file to dsply*/
        if (pFilePath[i] == '\n')
           psd->numlines++;
                                                /* Set scroll range & box.  */
      SetScrollRange(hWnd, SB_VERT, 0, psd->numlines, TRUE);
      SetScrollPos(hWnd, SB_VERT, 0, TRUE);
      SetScrollRange(hWnd, SB_HORZ, 0, LINESIZE, TRUE);
      SetScrollPos(hWnd, SB_HORZ, 0, TRUE);
      psd->yPos = psd->xPos = 0;                /* Init x and y position    */

                                                /* Get current font info.   */
                                                /* and init font strucs.    */
      GetObject(GetStockObject(SYSTEM_FONT), sizeof(psd->lf), &psd->lf);
      psd->cf.lStructSize = sizeof(psd->cf);
      psd->cf.hwndOwner = hWnd;
      psd->cf.lpLogFont = &psd->lf;
      psd->cf.Flags = CF_EFFECTS | CF_FORCEFONTEXIST |
                              CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;

      InvalidateRect(hWnd, NULL, TRUE); /* Set so WM_PAINT will draw text.  */

      return 0;                         /* Return 0.See online documentation*/
      break;

    case WM_SIZE:
      psd = (struct scrolldata *)GetProp(hWnd, "SCROLL_DATA");       /* Get scroll info for wndw */
      psd->xChildw = LOWORD(lParam);            /* Save new width.          */
      psd->yChildw = HIWORD(lParam);            /* Save new height.         */
                                  /* Let msg fall through to DefMDIChildProc*/
      break;

    case WM_VSCROLL:
      psd = (struct scrolldata *)GetProp(hWnd, "SCROLL_DATA");       /* Get scroll info for wndw */
      pFilePath = (unsigned char *)GetProp(hWnd, "FILE_DATA");   /* Get ptr to file data.    */
      switch (LOWORD(wParam))
      {                                         /* Determine change in y.   */
        case SB_LINEDOWN:
          y = 1;
          break;

        case SB_LINEUP:
          y = -1;
          break;

        case SB_PAGEDOWN:                       /* Scroll one page size     */
          y = psd->yChildw/psd->yunit;
          break;

        case SB_PAGEUP:                         /* Scroll one page size     */
          y = -psd->yChildw/psd->yunit;
          break;

        case SB_THUMBPOSITION:
          y = HIWORD(wParam) - psd->yPos;
          break;

        case SB_ENDSCROLL:
          return 0;
          break;

        default:                                /* Default: no change       */
          y = 0;
        break;
      }

      psd->yPos += y;                    /* Calculate new vert scrollbox pos*/
      if (psd->yPos > psd->numlines)
      {
        psd->yPos = psd->numlines;
        InvalidateRect(hWnd, NULL, TRUE);
      }
      else
        if (psd->yPos < 0)
        {
          psd->yPos = 0;
          InvalidateRect(hWnd, NULL, TRUE);
        }
        else
        {                                 /* ScrollWindow makes it smooth   */
          ScrollWindow(hWnd, 0,
                       -psd->yunit * y,   /* Amount of vertical srolling    */
                      (CONST RECT *) NULL, (CONST RECT *) NULL);
          UpdateWindow(hWnd);             /* Sends WM_PAINT msg; not posted!*/
        }
      SetScrollPos(hWnd, SB_VERT, psd->yPos, TRUE); /* Set box on window    */
      return 0;                       /* Return 0. See online documentation*/
      break;

    case WM_HSCROLL:
      psd = (struct scrolldata *)GetProp(hWnd, "SCROLL_DATA");       /* Get scroll info for wndw */
      pFilePath = (unsigned char *)GetProp(hWnd, "FILE_DATA");   /* Get ptr to file data.    */
      switch (LOWORD(wParam))
      {                                  /* Determine change in x           */
        case SB_LINELEFT:
          x = -1;
          break;

        case SB_LINERIGHT:
          x = 1;
          break;

        case SB_PAGELEFT:                       /* I set paging to be 10    */
          x = -10;
          break;                                /* characters for horizontal*/

        case SB_PAGERIGHT:
          x = 10;
          break;

        case SB_THUMBPOSITION:
          x = HIWORD(wParam) - psd->xPos;
          break;

        case SB_ENDSCROLL:
          return 0;
          break;

        default:
          x = 0;                               /* Default: no change        */
          break;
      }

      psd->xPos += x;                    /* Calculate new horz scrollbox pos*/
      if (psd->xPos > LINESIZE)
      {
        x = 0;                           /* No change from current display. */
        psd->xPos = LINESIZE;
      }
      else
        if (psd->xPos < 0)
        {
          x = 0;                         /* No change from current display. */
          psd->xPos = 0;
        }

      SetScrollPos(hWnd, SB_HORZ, psd->xPos, TRUE); /* Set box on window    */
      InvalidateRect(hWnd, NULL, TRUE);
      return 0;                       /* Return 0. See online documentation*/
      break;

    case WM_COMMAND:
      switch (wParam)
      {
        case (IDM_FONT):                 /* User chose "Select Font..."     */
          psd = (struct scrolldata *)GetProp(hWnd, "SCROLL_DATA");

          if (ChooseFont(&psd->cf))      /* If user selects a new font      */
            InvalidateRect(hWnd, NULL, TRUE);  /* re-paint window with it.  */
          break;

        default:
          break;
      }
      return 0;                       /* Return 0. See online documentation*/
      break;

    case WM_PAINT:
      psd = (struct scrolldata *)GetProp(hWnd, "SCROLL_DATA");       /* Get scroll info for wndw */
      pFilePath = (unsigned char *)GetProp(hWnd, "FILE_DATA");   /* Get ptr to data to paint */

      hdc = BeginPaint(hWnd, &ps);              /* Gets device context.     */
                                                /* Set up the font.         */
      hFont = CreateFontIndirect(&psd->lf);     /* Creates font w/ character*/
                                                /* -istics specified in lf  */
      SelectObject(hdc, hFont);                 /* Sets up DC with new info */
      SetTextColor(hdc, psd->cf.rgbColors);     /* Sets up DC with new color*/

      GetTextMetrics(hdc, &tm);                 /* Get Font dimensions.     */
                                               /* Get horiz/vert scroll unit*/
      psd->xunit = tm.tmAveCharWidth;
      psd->yunit = tm.tmHeight + tm.tmExternalLeading;

                                         /* Determine offset in file.       */
                                         /* Psd->YPOS pts to line we want   */
                                         /* to start display at.            */
      for (i=0, charpos = 0; i < psd->yPos; charpos++)
        if (pFilePath[charpos] == '\n')
           i++;
                                         /* Determine # of lines to be      */
                                         /* displayed, smaller of lines left*/
                                         /* in file and amt that will fit   */
                                         /* on the currently sized area.    */
      clientlines = min((psd->numlines-psd->yPos),(psd->yChildw/psd->yunit)+1);

      memset(line, ' ', sizeof(line));   /* Init output line to blanks.     */
                                         /* Display the lines of text.      */
      for (i = 0, y = 0; y < clientlines; i++, charpos++)
      {                                  /* i is line index;y is lines done */
        if (pFilePath[charpos] == '\n')  /* Line is filled.                 */
        {
          TextOut(hdc, 0, psd->yunit * y, &line[psd->xPos],
                                           sizeof(line) - psd->xPos);
          y++;                           /* did 1 line                      */
          i = -1;               /* reset line index to 0 (post increment ++)*/
          memset(line, ' ', sizeof(line));
        }
        else                            /* Fill line with character.        */
        {
          if (i < LINESIZE)
          line[i] = pFilePath[charpos];
        }
      }

      SelectObject(hdc, GetStockObject(SYSTEM_FONT));/* Replace w/ original */
                                       /* default font object.              */
      DeleteObject(hFont);             /* Delete the font object created.   */
      EndPaint(hWnd, &ps);             /* Delete the device context.        */
      return 0;                        /* Return 0. See online documentation*/
      break;

    case WM_MDIACTIVATE:               /* MDI msg sent by client window when*/
                                       /* inactive child is clicked or      */
                                       /* brought to foreground.            */
      hClientWnd = GetParent(hWnd);    /* Get handle of client window.      */
      if ((HWND)lParam == hWnd)        /* WM_MDIACTIVATE msg sent to        */
      {                                /*  activate this child window.      */
                                       /* Get handle of Window submenu w/in */
                                       /*  the child menu.                  */
        hWindowMenu = GetSubMenu(hChildMenu,BROWSEF_WINPOS);
        SendMessage(hClientWnd, WM_MDISETMENU, (WPARAM)hChildMenu,
                   (LPARAM)hWindowMenu);
      }
      else                             /* Else the message was sent to      */
      {                                /*  deactivate this child window.    */
                                       /* Get handle of Window submenu w/in */
                                       /*  the client menu.                 */
        hWindowMenu = GetSubMenu(hClientMenu, BROWSE_WINPOS);
        SendMessage(hClientWnd, WM_MDISETMENU, (WPARAM)hClientMenu,
                   (LPARAM)NULL);
      }
      hFrameWnd = GetParent(hClientWnd);
      DrawMenuBar(hFrameWnd);          /* Draw change in menu bar.          */
      return 0;                        /* Return 0. See online documentation*/
      break;

    case WM_DESTROY:                   /* Free the memory belonging to child*/
                                       /* Free the path name stored as wndw */
      hFileName = (LOCALHANDLE)GetWindowLong(hWnd, GWL_USERDATA); /* instance data*/
      LocalFree(hFileName);

      EnumProps(hWnd, DeleteProperties); /* Delete any child properties     */
      return 0;                        /* Return 0. See online documentation*/
      break;

    default:
      break;
  }

  return DefMDIChildProc(hWnd, message, wParam, lParam);
}


BOOL DlgAskFile(HWND hWnd, LPSTR FileName, LPSTR TitleName)
{
  DWORD x;
  OPENFILENAME fn;                       /* Structure for file selection    */
  char filetypes[] = {"Code file\0*.C\0Header file\0*.h\0Text file\0*.TXT\0"
                      "Script file\0*.SCR\0All files\0*.*\0\0"};

                                                                                  /* Initialize the Open structure.  */
  fn.lStructSize = sizeof(OPENFILENAME);
  fn.hwndOwner = hWnd;                   /* Handle that owns dialog box     */
  fn.hInstance = NULL;
  fn.lpstrFilter ="Code file\0*.C\0\0";
  fn.lpstrCustomFilter = NULL;
  fn.nMaxCustFilter = 0;
  fn.nFilterIndex = 1;                   /* Initialize to C files.          */
  fn.lpstrFile = FileName;               /* FN to init File Name edit ctl.  */
  fn.nMaxFile = _MAX_PATH;               /* Buffer size of lpstrFile        */
  fn.lpstrFileTitle = TitleName;         /* Pts to buffer to receive title  */
  fn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
  fn.lpstrInitialDir = NULL;             /* Use current directory initially */
  fn.lpstrTitle = "Browse File";         /* Title of dialog box.            */
  fn.Flags = OFN_FILEMUSTEXIST |
             OFN_HIDEREADONLY |
             OFN_NONETWORKBUTTON;
  fn.nFileOffset = 0;
  fn.nFileExtension = 0;
  fn.lpstrDefExt = "c";
  fn.lCustData = 0;
  fn.lpfnHook = NULL;
  fn.lpTemplateName = NULL;

  x = GetOpenFileName(&fn);
  if (!x)
    x = CommDlgExtendedError();
  return x;
}


BOOL CALLBACK DeleteProperties(HWND hWnd, LPCTSTR PropName, HANDLE hData)
{

  GlobalFree(hData);
  RemoveProp(hWnd, PropName);

  return TRUE;
}
/*************************   end of browser.c *****************************/
