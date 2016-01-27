/*************************************************************************
 * File Name    :  recorder.h
 *
 * Description  :  This file contains the definitions used by the Audio
 *                 Recorder sample program.
 *
 * Copyright (C) IBM 1993
 *************************************************************************/
#define INIT_VOLUME                75           /* initial volume level  */
#define IDD_FILEWINDOW             2000
#define FILESAVE                   1111
#define NEW_FILE                   0
#define EXISTING_FILE              1

#define ID_MESSAGEBOX              100
#define ID_ICON                    101

/*
 * Main dialog box
 */
#define ID_DLG_MAIN                101
#define ID_GPB_STOP                102
#define ID_GPB_PLAY                103
#define ID_GPB_RECORD              104
#define ID_GPB_REWIND              105
#define ID_GPB_FF                  106
#define ID_SL_VOLUME               107
#define ID_STATUSLINE              108
#define ID_STATUSTXT               109

/*
 * File menu
 */
#define IDM_FILE_MENU              110
#define IDM_FILE_NEW               111
#define IDM_FILE_OPEN              112
#define IDM_FILE_SAVE              113
#define IDM_FILE_SAVE_AS           114

/*
 * Options menu
 */
#define IDM_OPTIONS_MENU           150
#define IDM_MICROPHONE             151
#define IDM_LINEIN                 152
#define IDM_LOW_INPUT              153
#define IDM_MEDIUM_INPUT           154
#define IDM_HIGH_INPUT             155
#define IDM_MONITOR_INPUT          156
#define IDM_OPTIONS_STATUSLINE     157

/*
 * Type menu
 */
#define IDM_TYPE_MENU              170
#define IDM_MONO                   171
#define IDM_STEREO                 172
#define IDM_VOICE                  173
#define IDM_MUSIC                  174
#define IDM_HIGH_FIDELITY          175
#define IDM_LOW_QUALITY            176
#define IDM_HIGH_QUALITY           177

/*
 * Help menu
 */
#define IDM_HELP_MENU              201
#define IDM_HELP_INDEX             202
#define IDM_HELP_GENERAL           203
#define IDM_HELP_USING             204
#define IDM_HELP_KEYS              205
#define IDM_HELP_PRODUCTINFO       206

/*
 * Product Info dialog box
 */
#define ID_DLG_PRODUCTINFO         290

#define MONO                       1
#define STEREO                     2
#define BIT_8                      8
#define BIT_16                     16
#define VOICE                      11025
#define MUSIC                      22050
#define HIGH_FIDELITY              44100
#define LOW_INPUT_LEVEL            60
#define MEDIUM_INPUT_LEVEL         70
#define HIGH_INPUT_LEVEL           90

#define ID_AUDIO_HELPTABLE         300
#define ID_SUBTABLE_MESSAGEBOX     301

/*
 * User parms for the mci operations - these will identify which
 * action generated a received notification message.
 */
#define UP_OPEN                    1
#define UP_PLAY                    2
#define UP_STOP                    3
#define UP_CLOSE                   4
#define UP_VOLUME                  5
#define UP_RECORD                  6
#define UP_SET                     7
#define UP_CONNECTION              8
#define UP_SETMONITOR              9
#define UP_SETAMP                  10
#define UP_SEEK                    11

#define FILE_NAME_SIZE             CCHMAXPATH
#define STRING_COMMAND_SIZE        50
#define MESSAGELEN                 20
#define TITLESEPARATORLEN          4
#define NUMBER_OF_DEVICE_RB        6
#define NUMBER_OF_FILE_RB          7

/*
 * Button bitmap id's
 */
#define ID_BMP_REC1                401
#define ID_BMP_REC0                402
#define ID_BMP_PLAY0               403
#define ID_BMP_PLAY1               404
#define ID_BMP_PLAY2               405
#define ID_BMP_PLAY3               406
#define ID_BMP_PLAY4               407
#define ID_BMP_PAUSE               408
#define ID_BMP_STOP                409
#define ID_BMP_REW                 410
#define ID_BMP_FASTF               411

/*
 * String defines for the string table.
 */
#define LEN_ERROR_TITLE                     60
#define LEN_ERROR_MESSAGE                   128
#define LEN_PROGRAM_TITLE                   60
#define LEN_HELP_WINDOW_TITLE               60
#define LEN_HELP_LIBRARY_NAME               13 /* DOS limit   */

#define IDS_PROGRAM_TITLE                   1
#define IDS_AUDIO_RECORDER_ERROR            2
#define IDS_HELP_WINDOW_TITLE               3
#define IDS_HELP_LIBRARY_NAME               4
#define IDS_DEFAULTSIZE                     5
#define IDS_UNTITLED                        6
#define IDS_TITLEBARSEPARATOR               7
#define IDS_FILEOPENEXT                     8
#define IDS_SAVE_AS                         9
#define IDS_STOPPED                         10
#define IDS_PLAYING                         11
#define IDS_REWINDING                       12
#define IDS_FASTFORWARDING                  13
#define IDS_RECORDING                       14
#define IDS_PASSED                          15
#define IDS_OPENINGDEVICE                   16
#define IDS_LOADINGFILE                     17
#define IDS_SAVINGFILE                      18
#define IDS_HELP_CREATION_FAILED            19
#define IDS_UNKNOWN                         20
#define IDS_COMMAND_CAN_PLAY                21
#define IDS_COMMAND_CAN_RECORD              22
#define IDS_COMMAND_STATUS_BITSPERSAMPLE    23
#define IDS_FILE_CHANGED                    24
#define IDS_FILE_EXISTS                     25
#define IDS_MMIO_GET_HEADER_FAIL            26
#define IDS_CANNOT_OPEN_INPUT_FILE          27
#define IDS_FILEOPEN_TITLE                  28
#define IDS_FILESAVEAS_TITLE                29
#define IDS_CANNOT_SAVE_FILE                30

#ifndef max
#define max(a,b) (((a) > (b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a) < (b))?(a):(b))
#endif
