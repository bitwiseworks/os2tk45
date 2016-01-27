/*************************************************************************
 * File Name    :  captions.h
 *
 * Description  :  This file contains the definitions used by the Caption
 *                 sample application.
 *
 * Copyright (C) IBM 1993
 *************************************************************************/
/*------------------ Commands accepted by ccSendCommand -----------------*/

#define CC_START                 1
#define CC_STOP                  2
#define CC_SET                   3
#define CC_STATUS                4

#define CCERR_UNEXPECTED_EOF         1000
#define CCERR_FILE_FORMAT            1001
#define CCERR_TOO_MANY_LINES         1002
#define CCERR_INVALID_WINDOW_HANDLE  1003
#define CCERR_INVALID_COMMAND        1004
#define CCERR_NO_DEVICE_NAME         1005
#define CCERR_INVALID_STATUS_REQUEST 1006
#define CCERR_CANNOT_CREATE_BITMAP   1007

#define CC_WINDOW_ID           150



/*------------------------ Messages used by this application -----------------*/

#define UMCC_NEXTLINE  WM_USER + 1

#define UMCC_START     WM_USER + 2  /* This message starts captioning.        */
                                    /* Mp2 holds a pointer to a CC_START_PARMS*/
                                    /* struct. Cast the return value to APIRET*/

#define UMCC_STOP      WM_USER + 3  /* This message hides the caption window  */
                                    /* and stops captioning.                  */

#define UMCC_STATUS    WM_USER + 4  /* This message queries the status of a   */
                                    /* captioning parameter                   */

#define UMCC_SET       WM_USER + 5  /* This message sets the status of a      */
                                    /* captioning parameter.                  */

#define UMCC_ERROR     WM_USER + 6


/*------------- Parameter structure used by CC_START command -----------------*/

typedef _Packed struct _CC_START_PARMS
{
  HWND hwndOwner;        /* Handle of user window to be sent MM_POSITIONCHANGE*/
                         /* messages after the captioning system is through   */
                         /* with them.  If set to NULL, the cc system will    */
                         /* discard the messages after using them.            */

  PSZ   pszDeviceName;   /* Alias of the MCI device playing the audio. Supply */
                         /* this alias to have the captioning system request  */
                         /* the MM_MCIPOSITIONCHANGE messages from MMPM/2. If */
                         /* no device name is specified, an error will be     */
                         /* returned to the application.                      */

  PSZ   pszCaptionFile;/*   Pointer to a NULL terminated string which is the  */
                         /* file containing the captioning data.              */
}
CC_START_PARMS;

typedef CC_START_PARMS FAR *PCC_START_PARMS;


/*------------- Parameter structure used by CC_STATUS command ----------------*/

typedef struct _CC_STATUS_PARMS
{
   ULONG ulItem;             /* Item field for status flags                   */
   ULONG ulReturn;           /* Value associated with the item queried        */
}
CC_STATUS_PARMS;

typedef CC_STATUS_PARMS FAR *PCC_STATUS_PARMS;
                                         /*---------- Status flags -----------*/
                                         /*                                   */
#define CC_STATUS_TEXT_COLUMNS      1    /* Columns of text in caption window */
#define CC_STATUS_TEXT_ROWS         2    /* Rows of text in caption window    */
#define CC_STATUS_TEXT_COLOR        3    /* Color index of text color         */
#define CC_STATUS_BACKGROUND_COLOR  4    /* Color index of background color   */
#define CC_STATUS_X_POSITION        5    /* X coordinate of captioning window */
#define CC_STATUS_Y_POSITION        6    /* Y coordinate of captioning window */
                                         /*-----------------------------------*/


/*------------------- Parameter structure used by CC_SET command ---------------------*/

typedef struct _CC_SET_PARMS
{
   ULONG ulRows;
   ULONG ulBackgroundColor;
   ULONG ulTextColor;
   ULONG ulXposition;
   ULONG ulYposition;
   ULONG ulColumns;
}
CC_SET_PARMS;

typedef CC_SET_PARMS FAR *PCC_SET_PARMS;
                                        /*------------ Set flags -------------*/
                                        /*                                    */
#define CC_SET_TEXT_COLUMNS      1      /* Columns of text in caption window  */
#define CC_SET_TEXT_ROWS         2      /* Rows of text in caption window     */
#define CC_SET_TEXT_COLOR        3      /* Color index of text color          */
#define CC_SET_BACKGROUND_COLOR  4      /* Color index of background color    */
#define CC_SET_X_POSITION        5      /* X coordinate of captioning window  */
#define CC_SET_Y_POSITION        6      /* Y coordinate of captioning window  */
                                        /*------------------------------------*/

/*-------------- Captioning function prototypes ------------------------------*/

ULONG APIENTRY ccInitialize  ( HWND hwndParent );
ULONG APIENTRY ccSendCommand ( USHORT  usMsg, MPARAM mp1, MPARAM mp2 );
VOID  APIENTRY ccTerminate   ( HWND hwndCaption );
