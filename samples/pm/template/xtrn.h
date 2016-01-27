/**************************************************************************
 *  File name  :  xtrn.h
 *
 *  Description:  This header file contains the application-wide function
 *                declarations and global external variables.
 *
 *  Concepts   :  global definition
 *
 *  API's      :  [none]
 *
 *  Required
 *    Files    :  [none]
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
 *************************************************************************/

/*
 *  Global variables
 */
extern HWND hwndMainFrame;
extern HWND hwndMain;
extern HAB hab;
extern HMQ hmq;
extern HDC hdcMain;
extern CHAR szAppName[];
extern CHAR szUntitled[];
extern BOOL fPrintEnabled;
extern BOOL fHelpEnabled;

/*
 *  Entry point declarations
 */

/* from main.c */
int main(VOID);
MRESULT EXPENTRY MainWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
ULONG MessageBox(HWND hwndOwner, ULONG idMsg, ULONG fsStyle, BOOL fBeep);
VOID MainCommand(MPARAM mp1, MPARAM mp2);

/* from init.c */
BOOL Init(VOID);
MRESULT InitMainWindow(HWND hwnd, MPARAM mp1, MPARAM mp2);
VOID ExitProc(USHORT usTermCode);

/* from file.c */
VOID FileNew(MPARAM mp2);
VOID FileOpen(MPARAM mp2);
VOID FileSave(MPARAM mp2);
VOID FileSaveAs(MPARAM mp2);
VOID WriteFileToDisk(HFILE hf);
BOOL GetFileName(VOID);
VOID UpdateTitleText(HWND hwnd);
MRESULT EXPENTRY FAR TemplateOpenFilterProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY FAR TemplateSaveFilterProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

/* from edit.c */
VOID EditUndo(MPARAM mp2);
VOID EditCut(MPARAM mp2);
VOID EditCopy(MPARAM mp2);
VOID EditPaste(MPARAM mp2);
VOID EditClear(MPARAM mp2);

/* from user.c */
VOID UserCommand(MPARAM mp1, MPARAM mp2);
MRESULT UserWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID InitMenu(MPARAM mp1, MPARAM mp2);
VOID EnableMenuItem(HWND hwndMenu, USHORT idItem, BOOL fEnable);

/* from prnt.c */
VOID InitPrintingDialogs(VOID);

/* from pnt.c */
VOID MainPaint(HWND hwnd);

/* from help.c */
VOID InitHelp(VOID);
VOID HelpIndex(VOID);
VOID HelpGeneral(VOID);
VOID HelpUsingHelp(VOID);
VOID HelpKeys(VOID);
VOID HelpTutorial(VOID);
VOID HelpProductInfo(VOID);
VOID DisplayHelpPanel(ULONG idPanel);
VOID DestroyHelpInstance(VOID);

/* from thrd.c */
BOOL CreateBackgroundThread(VOID);
VOID DestroyBackgroundThread(VOID);
BOOL PostBkThreadMsg(ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT SendBkThreadMsg(ULONG msg, MPARAM mp1, MPARAM mp2);
/***************************  End of xtrn.h  ****************************/
