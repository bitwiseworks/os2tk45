/*static char *SCCSID = "@(#)dllcomm.h	6.5 92/02/18";*/
/*==============================================================*\
 *                                                              *
 *  DLLCOMM.H - DLL Sample PM application common header file    *
 *     (C) Copyright IBM Corporation 1992.                      *
 *                                                              *
 *--------------------------------------------------------------*
 *                                                              *
 *  This header file contains the structure definitions and     *
 *    prototype of application                                  *
 *                                                              *
\*==============================================================*/

#define BUFF_SIZE           200L /* Size of buffers for various stuff */
#define OP_SIZE               4L /* Size of operand */

/*--------------------------------------------------------------*\
 *  Data structures and typedefs                                *
\*--------------------------------------------------------------*/
typedef struct _OBJSTRUCT{
    CHAR szCurDriver[BUFF_SIZE];
    CHAR szCurPath[CCHMAXPATH];
    CHAR szCurPattern[BUFF_SIZE];
    CHAR szNewDriver[BUFF_SIZE];
    CHAR szNewPath[CCHMAXPATH];
    CHAR szNewPattern[BUFF_SIZE];
    } OBJSTRUCT;
typedef OBJSTRUCT FAR *POBJSTRUCT;

typedef struct _FileList {
    CHAR      szFileName[CCHMAXPATH];        /* Found file's path & name */
    struct _FileList  *pNextFile;                /* Pointer of next file */
    } FILELIST;
typedef FILELIST FAR *PFILELIST;
typedef PFILELIST FAR *PPFILELIST;

typedef struct _FileInfo{
   CHAR    szFileName[CCHMAXPATH];      /* File name of selected to open */
   CHAR    szBuffer[BUFF_SIZE];         /* Text data from file selected  */
   } FILEINFO;
typedef FILEINFO FAR *PFILEINFO;

typedef struct _MathElement{
   LONG    fOperand1;
   LONG    fOperand2;
   CHAR    pszOperation[OP_SIZE];
   CHAR    szResult[BUFF_SIZE];
} MATHELEMENT;
typedef MATHELEMENT FAR *PMATHELEMENT;

/*--------------------------------------------------------------*\
 *  Prototype declarations                                      *
\*--------------------------------------------------------------*/
APIRET SearchFile  (POBJSTRUCT, PPFILELIST);
APIRET ReadFileProc(PFILEINFO);
APIRET CalCulProc  (PMATHELEMENT);
