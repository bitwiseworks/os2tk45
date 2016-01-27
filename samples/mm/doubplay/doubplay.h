/*************************************************************************
 * File Name    :  doubplay.h
 *
 * Description  :  This file contains the definitions used by the Double
 *                 Buffering Sample Program.
 *
 * Concepts     :  n/a
 *
 * MMPM/2 API's :  n/a
 *
 * Required
 *    Files     :  None.
 *
 *
 * Copyright (C) IBM 1993
 *************************************************************************/

#define ID_MESSAGE_BOX                1
#define ID_SAMPLE                   100
#define ID_PLAY                     101
#define ID_END                      102

#define ID_ICON                     105
#define ID_SUBTABLE_MESSAGEBOX      109

#define ID_BMP_PLAY0          1020
#define ID_BMP_PLAY1          1021
#define ID_BMP_PLAY2          1022
#define ID_BMP_PLAY3          1023
#define ID_BMP_PLAY4          1024

#define ID_BMP_STOP0          1040

#define ID_DOUBPLAY_DIALOG_BOX   1043

#define MAXSIZE            1048
#define MAXBUFF            40
#define STRING_SIZE        356


/* Id's for the Resources defined in the doubplay.rc file */
#define    IDS_MAIN_WINDOW_TITLE                1
#define    IDS_ERROR_TITLE                      5
#define    IDS_WAVE_FILE                        6
#define    IDS_CANNOT_GET_MEMORY                7
#define    IDS_CANT_PROCESS_MESSAGE             8
#define    IDS_CANT_OPEN_WAVE                   9
#define    IDS_DEFAULT_SIZE                    10
#define    IDS_CANT_CREATE_SEMAPHORE           11


/* New Playlist commands */
#define     SEMPOST_OPERATION       10



/* State of the application i.e. if it is playing or stopped */
#define STATE_IS_PLAYING 1
#define STATE_IS_STOP 2



/*
 * MMPM/2 Playlist strucuture which contains the commands,
 * and the operands.
 */
typedef struct pls
{
   ULONG ulCommand;
   ULONG ulOperandOne;
   ULONG ulOperandTwo;
   ULONG ulOperandThree;
} PLAY_LIST_STRUCTURE_T;

USHORT ShowAMessage( USHORT usTitleId,
                     USHORT usTextId,
                     ULONG  ulMessageBoxStyle,
                     BOOL   fMCIError);

