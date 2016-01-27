/*************************************************************************
 * File Name   : MCISTRNG.H
 *
 * Description : This file contains the definitions used by the MCISTRNG
 *               sample program.
 *
 * Concepts    : No Multimedia concepts will be shown.
 *
 * MMPM/2      : None used.
 *
 * Required
 *    Files    : None.
 *
 *
 * Copyright (C) IBM 1991, 1993
 *************************************************************************/

/*
 * Resource specific definitions.
 */

/*
 * Main Dialog Box.
 */
#define ID_MAIN_DIALOG_BOX                                      1000
#define ID_STRING_COMBO_BOX                                     1100
#define ID_MAIN_SEND_PB                                         1200
#define ID_DISPLAY_MESSAGES_PB                                  1300
#define ID_STRING_TITLE_TEXT                                    1310
#define ID_MAIN_CANCEL_PB                                       1400
#define ID_MAIN_HELP_PB                                         1500
#define ID_MCI_STRING_HELPTABLE                                 1510
#define ID_BATCH_FILE_PB                                        1511

/*
 * Notification Messages Display Dialog Box.
 */
#define ID_DISPLAY_DIALOG_BOX                                   2000
#define ID_DISPLAY_CONTROL                                      2100
#define ID_MESSAGES_TEXT                                        2110
#define ID_DISPLAY_INCLUDE_PB                                   2200
#define ID_DISPLAY_CANCEL_PB                                    2300
#define ID_DISPLAY_HELP_PB                                      2400

/*
 * Include Dialog Box.
 */
#define ID_INCLUDE_DIALOG_BOX                                   3000
#define ID_INSTRUCTION_TEXT_LINE_1                              3010
#define ID_INSTRUCTION_TEXT_LINE_2                              3020
#define ID_NOTIFY_CB                                            3110
#define ID_PASSDEVICE_CB                                        3120
#define ID_POSITION_CHANGE_PB                                   3130
#define ID_EVENT_CB                                             3140
#define ID_CUEPOINT_CB                                          3150
#define ID_PLAYLIST_CB                                          3160
#define ID_MCIPLAYLIST_MESSAGE_TEXT                             3165
#define ID_INCLUDE_PB                                           3200
#define ID_INCLUDE_CANCEL_PB                                    3300
#define ID_INCLUDE_HELP_PB                                      3400

#define ID_ICON                                                 1000

/*
 * Main Dialog Box and control positioning defines.
 */
#define MAIN_DIALOG_X_POSITION                                    10
#define MAIN_DIALOG_Y_POSITION                                    60
#define MAIN_DIALOG_X_SIZE                                        275
#define MAIN_DIALOG_Y_SIZE                                        102

/* Control and Pushbuttons */
#define COMBO_BOX_X_POSITION                                      4
#define COMBO_BOX_Y_POSITION                                      20
#define COMBO_BOX_X_SIZE                                          267
#define COMBO_BOX_Y_SIZE                                          70

#define MAIN_SEND_PB_X_POSITION                                   4
#define MAIN_DISPLAY_PB_X_POSITION                                43
#define MAIN_BATCH_FILE_PB_X_POSITION                             140
#define MAIN_CANCEL_PB_X_POSITION                                 197
#define MAIN_HELP_PB_X_POSITION                                   235

#define MAIN_PUSHBUTTONS_Y_POSITION                               5

#define MAIN_SEND_PUSHBUTTON_X_SIZE                               37
#define MAIN_DISPLAY_PUSHBUTTON_X_SIZE                            95
#define MAIN_BATCH_PUSHBUTTON_X_SIZE                              55
#define MAIN_CANCEL_PUSHBUTTON_X_SIZE                             37
#define MAIN_HELP_PUSHBUTTON_X_SIZE                               37
#define MAIN_PUSHBUTTONS_Y_SIZE                                   15

#define MAIN_STRING_TEXT_X_POSITION                               5
#define MAIN_STRING_TEXT_Y_POSITION                               92
#define MAIN_STRING_TEXT_X_SIZE                                   80
#define MAIN_STRING_TEXT_Y_SIZE                                   8

/*
 * Display Dialog Box and control positioning defines.
 */
#define DISPLAY_DIALOG_X_POSITION                                 20
#define DISPLAY_DIALOG_Y_POSITION                                 50
#define DISPLAY_DIALOG_X_SIZE                                     275
#define DISPLAY_DIALOG_Y_SIZE                                     105

/* Controls and Pushbuttons */
#define DISPLAY_CONTROL_X_POSITION                                5
#define DISPLAY_CONTROL_Y_POSITION                                23
#define DISPLAY_CONTROL_X_SIZE                                    265
#define DISPLAY_CONTROL_Y_SIZE                                    70

#define DISPLAY_TEXT_X_POSITION                                   5
#define DISPLAY_TEXT_Y_POSITION                                   95
#define DISPLAY_TEXT_X_SIZE                                       110
#define DISPLAY_TEXT_Y_SIZE                                       8

#define DISPLAY_INCLUDE_PB_X_POSITION                             5
#define DISPLAY_CANCEL_PB_X_POSITION                              65
#define DISPLAY_HELP_PB_X_POSITION                                125

#define DISPLAY_PUSHBUTTONS_Y_POSITION                            5
#define DISPLAY_PUSHBUTTONS_X_SIZE                                50
#define DISPLAY_PUSHBUTTONS_Y_SIZE                                15

/*
 * Include Dialog Box and control positioning defines.
 */
#define INCLUDE_DIALOG_X_POSITION                                 55
#define INCLUDE_DIALOG_Y_POSITION                                 55
#define INCLUDE_DIALOG_X_SIZE                                     300
#define INCLUDE_DIALOG_Y_SIZE                                     140

/* Controls and Pushbuttons */
#define INCLUDE_PB_X_POSITION                                     5
#define INCLUDE_CANCEL_PB_X_POSITION                              60
#define INCLUDE_HELP_PB_X_POSITION                                115
#define INCLUDE_PUSHBUTTONS_Y_POSITION                            5

#define INCLUDE_PUSHBUTTONS_X_SIZE                                45
#define INCLUDE_PUSHBUTTONS_Y_SIZE                                15

#define INCLUDE_TEXT_X_POSITION                                   5
#define INCLUDE_TEXT_1_Y_POSITION                                 125
#define INCLUDE_TEXT_2_Y_POSITION                                 115
#define INCLUDE_TEXT_X_SIZE                                       250
#define INCLUDE_TEXT_Y_SIZE                                       8

#define INCLUDE_NUMBER_OF_CHECKBOXES                              6

#define CB_COLUMN_Y_POSITION                                      45
#define CB_OFFSET_Y_POSITION                                      20

#define CB_FIRST_COLUMN_X_POSITION                                15
#define CB_SECOND_COLUMN_X_POSITION                               155

#define CB_COLUMN_Y_POSITION                                      45
#define CB_OFFSET_Y_POSITION                                      20

#define CB_X_SIZE                                                 135
#define CB_RIGHT_COLUMN_X_SIZE                                    140
#define CB_Y_SIZE                                                 10

#define CB_TEXT_X_OFFSET                                          12
#define CB_TEXT_Y_OFFSET                                          10

/* List Box Defines. */

#define LIST_BOX_X_SIZE                                           100
#define LIST_BOX_Y_SIZE                                           92

#define LIST_BOXS_Y_POSITION                                      54

#define FILES_DIRECTORY_TEXT_Y_POSITION                           147

#define DIRECTORY_LIST_BOX_X_POSITION                             120

#define FILE_TEXT_X_SIZE                                          24


/*
 * Misc. Definitions.
 */
#define MCI_STRING_LENGTH                                       256

#define MCI_ERROR_STRING_LENGTH                                 128

#define MCI_RETURN_STRING_LENTGH                                128

#define LIST_BOX_VERTICAL_SB_ID                                 0xC001

#define ID_MESSAGE_BOX                                          1
#define SUBTABLE_MESSAGEBOX                                     2

/*
 * This define is to make the code more readable by replacing the OS/2
 * definition with a more understandable one.
 */
#define FILE_NAME_SIZE                                          CCHMAXPATHCOMP
#define WM_BATCHFILE                                            WM_USER

#define NUMBER_OF_NOTIFICATION_MESSAGES                         16

#define ID_MM_MCINOTIFY                                         0
#define ID_MM_MCIPASSDEVICE                                     1
#define ID_MM_MCIPOSITIONCHANGE                                 2
#define ID_MM_MCIEVENT                                          3
#define ID_MM_MCICUEPOINT                                       4
#define ID_MM_MCIPLAYLISTMESSAGE                                5
#define ID_MCI_NOTIFY_SUCCESSFUL                                6
#define ID_MCI_NOTIFY_SUPERSEDED                                7
#define ID_MCI_NOTIFY_ABORTED                                   8
#define ID_MCI_NOTIFY_ERROR                                     9
#define ID_DEVICE_ID                                            10
#define ID_USER_PARM                                            11
#define ID_GAINING_USE                                          12
#define ID_LOSING_USE                                           13
#define ID_TIME                                                 14
#define ID_CMD_MSG                                              15

#define STRING_SIZE                                             150

/*
 * String defines for the string table.
 */
#define IDS_MAIN_WINDOW_TITLE                                   1
#define IDS_MAIN_HELP_WINDOW_TITLE                              2
#define IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT                       3
#define IDS_MAIN_HELP_LIBRARY_FILE                              4
#define IDS_RETURN_STRING                                       5
#define IDS_UNKNOWN                                             6
#define IDS_CONTINUE_BATCHFILE                                  7

#define NUMBER_OF_PROGRAM_STRINGS                               7

#define IDS_DEFAULTSIZE                                         8
#define IDS_END_OF_BATCH_FILE                                   9
#define IDS_CANNOT_FIND_BATCH_FILE                              10
#define IDS_OPEN_BATCHFILE_TITLE                                11
#define IDS_OPEN_BATCHFILE_BUTTON                               12
#define IDS_MAIN_HELP_CREATION_FAILED                           13
#define IDS_MCI_STRING_HAS_NO_SIZE                              14
#define IDS_MCI_GETTING_STRING_ERROR_FAILED                     15
#define IDS_ALL_CHECKBOXES_ARE_FALSE                            16

#define STARTING_IDS_VALUE_FOR_MCI_NOTIFICATION_MESSAGES        18

#define IDS_MM_MCINOTIFY                                        18
#define IDS_MM_MCIPASSDEVICE                                    19
#define IDS_MM_MCIPOSITIONCHANGE                                20
#define IDS_MM_MCIEVENT                                         21
#define IDS_MM_MCICUEPOINT                                      22
#define IDS_MM_MCIPLAYLISTMESSAGE                               23
#define IDS_MCI_NOTIFY_SUCCESSFUL                               24
#define IDS_MCI_NOTIFY_SUPERSEDED                               25
#define IDS_MCI_NOTIFY_ABORTED                                  26
#define IDS_MCI_NOTIFY_ERROR                                    27
#define IDS_DEVICE_ID                                           28
#define IDS_USER_PARM                                           29
#define IDS_GAINING_USE                                         30
#define IDS_LOSING_USE                                          31
#define IDS_TIME                                                32
#define IDS_CMD_MSG                                             33


/*
 * Program specific Typedefs
 */
typedef enum
{
   MAIN_DIALOG,
   INCLUDE_DIALOG,
   DISPLAY_DIALOG
} WHICH_DIALOG_T;

/*
 * For Save and Restore
 */
typedef struct _SWPRESTORE {
   SWP     swpMain;
   SWP     swpDisplay;
   BOOL    fDisplayMsgWindowStatus;
} SWPRESTORE;
