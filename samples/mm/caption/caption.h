/*************************************************************************
 * File Name    :  caption.h
 *
 * Description  :  This file contains the definitions used by the Caption
 *                 Creation Utility  samplr program.
 *
 * Copyright (C) IBM 1993
 *************************************************************************/
#define UM_NEXTLINE                 WM_USER + 1
#define AUDIO_SLIDER_TICKS          20

#define ID_MAIN_DLG                 300
#define ID_PLAY                     301
#define ID_PAUSE                    302
#define ID_STOP                     303
#define ID_REWIND                   304
#define ID_VOLUME                   305
#define ID_NEXTLINE                 306
#define ID_SET_TIMING               307
#define ID_AUDIO_SLIDER             308
#define ID_AUDIO_POSITION           309
#define ID_DLG_PRODUCTINFO          310
#define ID_AUDIOTXT                 311
#define ID_TEXTTXT                  312
#define ID_CAPTIONTXT               313

/*
 * Menu ID's
 */
#define IDM_MAIN_MENU       1100
#define IDM_FILE            1101
#define IDM_OPEN            1102
#define IDM_OPEN_AUDIO      1103
#define IDM_OPEN_TEXT       1105
#define IDM_OPEN_CAPTIONS   1106
#define IDM_SAVE            1107
#define IDM_SAVEAS          1108
#define IDM_EXITPROG        1109
#define IDM_OPTIONS         1111
#define IDM_LINES           1112
#define IDM_2_LINES         1113
#define IDM_3_LINES         1114
#define IDM_4_LINES         1115
#define IDM_5_LINES         1116
#define IDM_6_LINES         1117
#define IDM_HELP            1110
#define IDM_GENERAL_HELP    1121
#define IDM_KEYS_HELP       1122
#define IDM_USING_HELP      1124
#define IDM_HELP_INDEX      1125
#define IDM_PRODUCT_INFO    1123

/*
 *  Help ID's
 */

#define HELPTABLE_ID            2000
#define SUBTABLE_MAIN_DLG       2001

#define PANEL_OVERVIEW         10500
#define PANEL_KEYS_HELP        10524
#define PANEL_FILEDLG_AUDIO     4163
#define PANEL_FILEDLG_TEXT      4164
#define PANEL_FILEDLG_SAVEAS    4165

/*
 * String ID's
 */
#define STRID_OPEN_AUDIO            600
#define STRID_OPEN_TEXT             601
#define STRID_SAVEAS_TITLE          602
#define STRID_SAVEAS_BUTTON         603
#define STRID_HELP_TITLE            604
#define STRID_DEFAULT_MAIN_TITLE    605
#define STRID_DEFAULT_SIZE          606
#define STRID_HELP_CREATION_FAILED  607
#define STRID_HELP_LIBRARY_NAME     608
#define STRID_AUDIO_POS_SECONDS     609
#define STRID_AUDIO_POS_MINUTES     610
#define STRID_UNKNOWN_ERROR         611
#define STRID_ERROR_MESSAGE_TITLE   612
#define STRID_SAVECHANGES_MESSAGE   613
#define STRID_UNTIMED_MESSAGE       614
#define STRID_OVERWRITE_QUERY       615
#define STRID_TRUNCATE_WARNING      616
#define STRID_TEXT_FILE_FORMAT      617
#define STRID_TOO_MANY_LINES        618
#define STRID_UNEXPECTED_EOF        619
#define STRID_DISK_FULL             620
#define STRID_GENERIC_DOS_ERROR     621
#define STRID_OPEN_CAPTION          622
#define STRID_AUDIOFILE_EXTENTION   624
#define STRID_FIRSTLINE_OF_CCFILE   625
#define STRID_CAPTION_FILE_NAME     626
#define STRID_TEXT_FILE_NAME        627
#define STRID_AUDIO_FILE_NAME       628
#define STRID_NONE_FILE_NAME        629
#define STRID_CANNOT_FIND_TEXTFILE  630


#define ERROR_LENGTH      180    /* Max length (in bytes) of an error message */
#define TITLE_LENGTH      40     /* Max length (in bytes) of a window title   */
#define MAX_TEXT_LINES    500    /* Max number of lines in a single text file */

#define CHAR_NULL         0     /*--------------------------------------------*/
#define CHAR_RETURN       13    /*Control characters present in ASCII files   */
#define CHAR_EOF          26    /*--------------------------------------------*/
#define CHAR_LINEFEED     10

#define UP_OPEN           1     /*--------------------------------------------*/
#define UP_CLOSE          2     /*Default user parameters for MM_MCINOTIFY    */
#define UP_PLAY           3     /*messages.                                   */
#define UP_STOP           4     /*--------------------------------------------*/
#define UP_PAUSE          5
#define UP_RESUME         6
#define UP_POSITION       7
#define UP_STATUS         8
#define UP_LOAD           9
#define UP_SET            11

#define ST_CLOSED         1     /*--------------------------------------------*/
#define ST_OPEN           2     /*Values for iState, which holds the current  */
#define ST_PLAYING        3     /*status of the audio device.                 */
#define ST_PAUSED         4     /*--------------------------------------------*/
#define ST_SEEKING        5

#define CC_WIDTH          600   /*--------------------------------------------*/
#define CC_DEFAULT_LINES  2     /*These constants control some of the size,   */
#define CC_XMARGIN        10    /*placement and scrolling characteristics of  */
#define CC_SCROLL_INC     3     /*the caption window.                         */
#define CC_TIMER_ID       1     /*--------------------------------------------*/

                                /*--------------------------------------------*/
#define LINE_NOT_TIMED 99999999 /*This is the value of dwTime for those lines */
                                /*which have not yet been timed.              */
                                /*--------------------------------------------*/

#define ERROR_TEXT_FILE_FORMAT 999  /*----------------------------------------*/
#define ERROR_TOO_MANY_LINES   998  /*                                        */
#define ERROR_UNEXPECTED_EOF   997  /*File errors specific to this application*/
#define ERROR_LINE_TRUNCATED   996  /*----------------------------------------*/
#define ERROR_FILE_EXISTS       80
#define ERROR_CANNOT_FIND_TEXTFILE  110

#define MAX_OUTPUT_LENGTH      150  /*Max length of line written to a _CC file*/
#define BASE_10                 10

                                 /*---------- Line data structure ------------*/
typedef _Packed struct _LINEDATA /*There is one of these for each caption line*/
   {                             /*                                           */
   PSZ    szText;                /*Pointer to a NULL-terminated text string   */
   LONG   lTextLen;              /*Number of characters in the string         */
   ULONG  ulTime;                /*Time (MM units) to display the string      */
   }                             /*                                           */
LINEDATA;                        /*An array of these structures is maintained */
                                 /*in a table pointed to by pvLinedataTable.  */
typedef LINEDATA FAR *PLINEDATA; /*-------------------------------------------*/
