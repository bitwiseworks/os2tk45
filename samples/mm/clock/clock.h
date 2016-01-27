/*************************************************************************
 * File Name   : CLOCK.H
 *
 * Description : This file contains the definitions used by the clock
 *               sample program.
 *
 * Concepts    : See this sample program's C source file.
 *
 * MMPM/2 API's: None.
 *
 * Required
 *    Files    : See this sample program's C source file.
 *
 *
 * Copyright (C) IBM 1991, 1992, 1993
 *************************************************************************/

/*
 * Resource specific definitions.
 */
#define ID_CLOCK_DIALOG_BOX                                     1000
#define ID_ICON                                                 1000
#define ID_PLAY_CHIME_PB                                        1100
#define ID_CANCEL_PB                                            1200
#define ID_HELP_PB                                              1300
#define ID_CLOCK_HELPTABLE                                      1350

#define ID_CENTER_BELL                                          1050
#define ID_LEFT_BELL                                            1051
#define ID_RIGHT_BELL                                           1052
#define ID_FAR_RIGHT_BELL                                       1053
#define ID_FAR_LEFT_BELL                                        1054

/*
 * Bell and Chime specific definitions.
 */
#define NUMBER_OF_BELLS_IN_A_SWING                              8
#define NUMBER_OF_BELL_BITMAPS                                  5
#define NUMBER_OF_CHIME_FILES                                   3

#define NUMBER_OF_AUDIO_CHIMES                                  3
#define NUMBER_OF_COMMANDS                                      10

#define CHIME_PLAYING_HAS_STOPPED                               99

#define FILE_NAME_SIZE                                          81

/*
 * Index into the array that has the handles to the bitmaps.
 */
#define IX_CENTER_BELL                                          0
#define IX_LEFT_BELL                                            1
#define IX_RIGHT_BELL                                           2
#define IX_FAR_RIGHT_BELL                                       3
#define IX_FAR_LEFT_BELL                                        4

#define BELL_BITMAP_X_SIZE                                      64
#define BELL_BITMAP_Y_SIZE                                      64

#define PAUSE_BETWEEN_BITMAPS                                   500

/*
 * Program specific definitions
 */
#define SIZE_OF_CIRCLE                                          350
#define CLOCK_TIMER_ID                                          1
#define CLOCK_TIMER_TIMEOUT                                     2000

#define NUMBER_OF_MINUTE_MARKERS                                60
#define NUMBER_OF_DEGREES_IN_A_CIRCLE                           360

#define NUMBER_OF_HOUR_HAND_POINTS                              5
#define NUMBER_OF_MINUTE_HAND_POINTS                            5

#define HOUR_HAND_INDEX                                         5
#define HOUR_HAND_MULTIPLIER                                    (50/100)
#define MINUTE_HAND_MULTIPLIER                                  (85/100)

#define CLOCK_HAND_COLOR                                        CLR_RED
#define HAND_DOT                                                2

#define NO_ALLOC_ERROR                                          0

/*
 * These defines are used in placeing the time markers for the clock
 * face as well as drawing the clock hands.
 */
#define MARKER_POSITION_FROM_CENTER ( (LONG) (SIZE_OF_CIRCLE - 55) / 2)
#define PIE                                                    3.141592654
#define CONVERT_RADIAN_TO_DEGREES    ((double) (PIE / (double) 180))


/*
 * Pushbutton Sizes and Positions
 */
#define X_PLAY_CHIME_POSITION                                   5
#define Y_PLAY_CHIME_POSITION                                   2

#define X_CANCEL_POSITION                                       70
#define Y_CANCEL_POSITION                                       2

#define X_HELP_POSITION                                         135
#define Y_HELP_POSITION                                         2

#define X_BITMAP_POSITION                                       325
#define Y_BITMAP_POSITION                                       10

#define Y_SIZE_OF_THE_BUTTONS                                   12
#define X_SIZE_OF_THE_BUTTONS                                   60

#define Y_ADJUSTMENT_SIZE_OF_BUTTONS    Y_SIZE_OF_THE_BUTTONS + 25

/* Positioning of the dialog window */
#define X_CLOCK_DIALOG_POSTION                                  10
#define Y_CLOCK_DIALOG_POSTION                                  10
#define X_SIZE_FOR_THE_CLOCK_DIALOG                             270
#define Y_SIZE_FOR_THE_CLOCK_DIALOG                             175

#define THREAD_STACK_SIZE                                       4096

#define MCI_ERROR_STRING_LENGTH                                 128
#define STRING_SIZE                                             351

#define CHIME_HOUR_FILE                                         0
#define CHIME_QUARTERS_FILE                                     1
#define CHIME_HALF_HOUR_FILE                                    2

#define ID_MESSAGE_BOX                                          1
#define SUBTABLE_MESSAGEBOX                                     2


/*
 * String defines for the string table.
 */
#define IDS_MAIN_WINDOW_TITLE                                   1
#define IDS_HELP_CREATION_ERROR                                 2
#define IDS_HELP_LIBRARY_FILE                                   3
#define IDS_HELP_WINDOW_TITLE                                   4
#define IDS_NORMAL_ERROR_MESSAGE_BOX_TEXT                       5
#define IDS_SWING_THREAD_NOT_STARTED                            6
#define IDS_CHIME_FILE_A                                        7
#define IDS_CHIME_FILE_B                                        8
#define IDS_CHIME_FILE_C                                        9

#define NUMBER_OF_PROGRAM_STRINGS                               9

#define IDS_CANNOT_GET_MEMORY                                   10
#define IDS_CHIME_FILE_ERROR                                    11
#define IDS_FAILED_MMIO_CLOSE                                   12
#define IDS_CANNOT_PLAY_CHIME                                   13

/*
 * MMPM/2 Playlist defines
 */
#define DATA_OPERATION                                          0
#define BRANCH_OPERATION                                        1
#define LOOP_OPERATION                                          2
#define CALL_OPERATION                                          3
#define RETURN_OPERATION                                        4
#define EXIT_OPERATION                                          5
#define NOP_OPERATION                                           6
#define MESSAGE_OPERATION                                       7
#define CUEPOINT_OPERATION                                      8

#define QUARTERS_PLAYLIST                                       0
#define HALF_HOUR_PLAYLIST                                      1
#define THREE_QUARTERS_PLAYLIST                                 2
#define HOUR_PLAYLIST                                           3

#define LOOP_LINE                                               4
#define NUMBER_OF_PLAYLISTS                                     4


/*
 * Program specific Typedefs
 */
typedef enum
{
   STOP,
   GO,
   READY
} STOP_GO_READY_T;

typedef enum
{
   NONE,
   HOUR,
   HALF_HOUR,
   QUARTER_HOUR,
   THREE_QUARTER_HOUR

} CHIME_TIME_VALUE_T;

typedef struct pls
{
   ULONG ulCommand;
   ULONG ulOperandOne;
   ULONG ulOperandTwo;
   ULONG ulOperandThree;
} PLAY_LIST_STRUCTURE_T;
