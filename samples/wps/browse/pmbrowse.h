/************************************************************************/
/*                               PMBROWSE.H                             */
/************************************************************************/
#define ID_WINDOW                   100

#define ID_ICON                     101

#define ID_PRODUCTINFO              120
#define ID_OK                       130

/**********************/
/* Menu option values */
/**********************/
#define ID_OPENMENU                 ( OPEN_USER + 1)
#define ID_OPEN_TEXT                ( OPEN_USER + 2)
#define ID_OPEN_HEX                 ( OPEN_USER + 3)

#define ID_PRODUCTINFOMENU          ( OPEN_USER + 4)
#define ID_OPEN_PRODUCTINFO         ( OPEN_USER + 5)

/****************************************/
/* Browse - O - Matic specific messages */
/****************************************/
#define IDL_SETFONTINFO             WM_USER + 0

/******************/
/* Help constants */
/******************/
#define SUBTABLE_MAIN               2000
#define SUBTABLE_PRODUCTINFO        3000

#define PANEL_MAIN                  2000
#define PANEL_HELPKEYS              3000

#define PANEL_HELP_OPEN_TEXT        6000
#define PANEL_HELP_OPEN_HEX         6010

/*********************/
/* Defined Constants */
/*********************/
#define  APPL_NAME            "Browse-O-Matic"
#define  BROWSE_WINDOW_CLASS  APPL_NAME

/*****************/
/* Display Modes */
/*****************/
#define PMBROWSE_TEXT_DISPLAY_MODE  1
#define PMBROWSE_HEX_DISPLAY_MODE   2

/*****************************************/
/*          Function Prototypes          */
/*****************************************/

/**************/
/* pmbrowse.c */
/**************/
MRESULT EXPENTRY BrowseWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY OpenFilterProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY AboutBoxDlgProc( HWND hwnd, ULONG msg, MPARAM mp1,MPARAM mp2);

#ifndef max
#define max(a,b) (((a) > (b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a) < (b))?(a):(b))
#endif
