/****************************************************************************
* OS/2 Sample Print Application PRTSAMP
*
* name: prtsdlg.h
*
* description: #defines for dialog items
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
*
*
****************************************************************************/

#define ID_PRTSAMP                  1


/* id for menu options */

#define IDM_FILE                    10
#define IDM_OPEN                    11
#define IDM_SETUP                   12
#define IDM_PRINT                   13

#define IDM_OPTIONS                20
#define IDM_SETFONT                21
#define IDM_PAGEDOWN               22
#define IDM_VIEWHALF               23
#define IDM_VIEWFULL               24
#define IDM_PAGESETUP              25

#define IDM_HELP                   30
#define IDM_HELPINDEX              31
#define IDM_HELPEXTENDED           32
#define IDM_USINGHELP              33
#define IDM_PRODINFO               35
#define IDM_AUTHORS                36

#define IDD_QUERYPRINT              2000
#define IDD_QPTEXT                  2001
#define IDD_QPLISTBOX               2002
#define IDD_QPJOBPROP               2005
#define IDD_PAGESETUP               2006

#define IDD_PRINT                   2100
#define IDD_PRTCOPIES               2101

/*
 * The following controls are not used.  The values
 * are order sensitive for hiding the windows
 */
#define IDD_PRTALL                  2102
#define IDD_PRTSELECT               2103
#define IDD_PRTFROM                 2104
#define IDD_PRTTO                   2105
#define IDD_PRTFIRSTPAGE            2106
#define IDD_PRTLASTPAGE             2107
#define IDD_PRTPAGEGROUP            2108
#define IDD_PRTPREVIEW              2109

#define IDD_PRTDOCUMENT             2110
#define IDD_PRTPRINTER              2111

#define IDD_PRINTING                2200
#define IDD_PRINTINGTEXT            2201
#define IDD_PRINTDOCUMENT           2202
#define IDD_PRINTPRINTER            2203

#define IDD_FORM                    2300
#define IDD_DUPLEX                  2301
#define IDD_MARGIN_TOP              2302
#define IDD_MARGIN_BOTTOM           2303
#define IDD_MARGIN_LEFT             2304
#define IDD_MARGIN_RIGHT            2305


/* constants for string table and WinLoadString */

#define PROGRAM_TITLE               3000
#define PROGRAM_AUTHORS             3001
#define PRINT_ABORT_MSG             3002
#define PRINT_PAGE_MSG              3003
#define PRINT_SCAN_MSG              3004

#define ERRMSG_BAD_BITMAP           3100
#define ERRMSG_BITMAP_NOT_SUPPORTED 3101
#define ERRMSG_CLOSE_QUESTION       3102
#define ERRMSG_DEFAULT_FONT         3103
#define ERRMSG_FILE_NOT_FOUND       3104
#define ERRMSG_FORM_CHANGE          3105
#define ERRMSG_MARGINS_NOT_NUMERIC  3106
#define ERRMSG_NOTHING_PRINTED      3107
#define ERRMSG_NO_ENUMERATE         3108
#define ERRMSG_NO_HELP              3109
#define ERRMSG_NO_MEMORY            3110
#define ERRMSG_NO_QUEUES_DEFINED    3111
#define ERRMSG_READ_ERROR           3112
#define ERRMSG_WRONG_FORM           3113
#define ERRMSG_NEW_DRIVER_VERSION   3114
#define ERRMSG_DEVICE_CHANGED       3115


/* dialog id's */

#define IDC_CK_METAFILE              9000
#define IDC_CK_TEXT                  9001
#define IDC_CK_BITMAP                9002
#define IDD_FILETYPE                 9003
#define IDD_PRODINFO                 9004
#define IDR_PRODINFO                 9005

