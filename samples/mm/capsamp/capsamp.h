/*************************************************************************
 * File Name    :  capsamp.h
 *
 * Description  :  This file contains the definitions used by the Caption
 *                 sample application.
 *
 * Copyright (C) IBM 1993
 *************************************************************************/
#define INIT_VOLUME                75           /* initial volume level  */
#define AUDIO_SLIDER_TICKS         20

#define ID_MESSAGEBOX              100
#define ID_ICON                    101

/*
 * Main dialog box
 */
#define IDD_MAIN_WINDOW            101
#define IDC_GPB_STOP               103
#define IDC_GPB_PLAY               104
#define IDC_GPB_PAUSE              105
#define IDC_GPB_REWIND             106
#define IDC_SL_VOLUME              107
#define IDC_AUDIO_SLIDER           108
#define IDC_AUDIO_POSITION         109

#define IDM_OPTIONS_MENU           120
#define IDM_SETTINGS               121

#define IDM_HELP_MENU              130
#define IDM_HELP_INDEX             131
#define IDM_HELP_GENERAL           132
#define IDM_HELP_USING             133
#define IDM_HELP_KEYS              134
#define IDM_HELP_PRODUCTINFO       135

/*
 * Settings dialog box
 */
#define IDD_SETTINGS_DLG           250
#define IDC_TEXT_COLUMNS           251
#define IDC_TEXT_COLUMNS_SB        252
#define IDC_TEXT_ROWS              253
#define IDC_TEXT_ROWS_SB           254
#define IDC_BACKGROUND_COLOR       255
#define IDC_BACKGROUND_COLOR_SB    256
#define IDC_TEXT_COLOR             257
#define IDC_TEXT_COLOR_SB          258
#define IDC_WINDOW_POSITION        259
#define IDC_WINDOW_POSITION_SB     260
#define IDC_OK_PB                  261
#define IDC_HELP_PB                262
#define IDC_CANCEL_PB              263

/*
 * Product Info dialog box
 */
#define IDD_PRODUCTINFO_DLG        290

#define ID_AUDIO_HELPTABLE         300
#define ID_SUBTABLE_MESSAGEBOX     301

/*
 * User parms for the mci operations - these will identify which
 * action generated a received notification message.
 */
#define UP_PLAY                    1

#define OPTIONSLEN                 24
#define NUMBER_OF_SETTINGS         6

/*
 * Button bitmap id's
 */
#define ID_BMP_PLAY0               401
#define ID_BMP_PLAY1               402
#define ID_BMP_PLAY2               403
#define ID_BMP_PLAY3               404
#define ID_BMP_PLAY4               405
#define ID_BMP_PAUSE               406
#define ID_BMP_STOP                407
#define ID_BMP_REWIND              408

/*
 * String defines for the string table.
 */
#define LEN_ERROR_TITLE                     60
#define LEN_ERROR_MESSAGE                   128
#define LEN_PROGRAM_TITLE                   60
#define LEN_HELP_WINDOW_TITLE               60
#define LEN_HELP_LIBRARY_NAME               13 /* DOS limit   */

#define IDS_PROGRAM_TITLE                   1
#define IDS_CAP_SAM_ERROR                   2
#define IDS_HELP_WINDOW_TITLE               3
#define IDS_HELP_LIBRARY_NAME               4
#define IDS_DEFAULTSIZE                     5
#define IDS_HELP_CREATION_FAILED            6
#define IDS_UNKNOWN                         7
#define IDS_CANNOT_FIND_AUDIO_FILE          8
#define IDS_UNEXPECTED_EOF                  9
#define IDS_INVALID_FILE_FORMAT             10
#define IDS_TOO_MANY_LINES                  11
#define IDS_WHITE                           12
#define IDS_YELLOW                          13
#define IDS_GRAY                            14
#define IDS_BLUE                            15
#define IDS_BLACK                           16
#define IDS_RED                             17
#define IDS_CENTER                          18
#define IDS_LEFT                            19
#define IDS_RIGHT                           20
#define IDS_INVALID_WINDOW_HANDLE           21
#define IDS_INVALID_FILE_NAME               22
