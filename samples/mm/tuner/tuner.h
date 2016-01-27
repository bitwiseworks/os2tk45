#define UWC_CLIENTCLASS   "Tv Tuner Window Class"

#define WM_INITTELEVISION  WM_USER + 1
#define WM_PAINTMOVIE      WM_USER + 2



#define IDC_DONE        101
#define IDC_UP          102
#define IDC_DOWN        103
#define IDC_ENTRY       104
#define IDC_SLIDER      105
#define IDC_TEXT        106

#define ID_MAINWND      256
#define ID_OPTIONS      257
#define ID_EXIT         258
#define ID_CHANNEL      259
#define ID_MSGBOX       260
#define ID_DIALOG       261
#define ID_ICON         262
#define ID_MOVIEWINDOW  263


/*
 * Window Instance Data
 */
typedef struct _APPDATA
{
    CHAR    szDevice[20];
    ULONG   ulChannel;
    ULONG   ulHighChannel;
    ULONG   ulLowChannel;
    ULONG   ulConnector;
    ULONG   ulOverlayColor;
    HWND    hwndMovie;
} APPDATA, * PAPPDATA ;

