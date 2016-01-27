/*********** HELLO C Sample Program Include file (.H) ***********************
 *
 *  The HELLO include file defines symbolic constants used
 *  in the HELLO.C and .RC files.
 *  HELLO local procedure declarations may appear in this file to
 *  ensure have been declared before being used.
 *
 *  Copyright (C) 1991 IBM Corporation
 *
 *      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
 *      sample code created by IBM Corporation. This sample code is not
 *      part of any standard or IBM product and is provided to you solely
 *      for  the purpose of assisting you in the development of your
 *      applications.  The code is provided "AS IS", without
 *      warranty of any kind.  IBM shall not be liable for any damages
 *      arising out of your use of the sample code, even if they have been
 *      advised of the possibility of such damages.                                                    *
 ******************************************************************************/

#pragma linkage (main,optlink)
INT main(VOID);

extern VOID AbortHello(HWND hwndFrame,HWND hwndClient);

#define MSGBOXID    1001

#define ID_WINDOW   256

#define ID_OPTIONS  257
#define ID_OPTION1  258
#define ID_OPTION2  259
#define ID_OPTION3  260

#define ID_EXITPROG 261

#define IDS_HELLO   262
#define IDS_1       263
#define IDS_2       264
#define IDS_3       265

