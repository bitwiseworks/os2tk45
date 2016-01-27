.*==============================================================*\
.*                                                              *
.* Dlg.ipf - Information Tag Language file for the screens      *
.*           for help on the dialog boxes.                      *
.*                                                              *
.*==============================================================*/

.*--------------------------------------------------------------*\
.*  Open dialog Ok button                                       *
.*      res = PANEL_OPENDLG_OK                                  *
.*--------------------------------------------------------------*/
:h1 res=3110 name=PANEL_OPENDLG_OK.OK
:p.The OK button closes the Select subdirectory dialog.

.*--------------------------------------------------------------*\
.*  Open dialog Help button                                     *
.*      res = PANEL_OPENDLG_HELP                                *
.*--------------------------------------------------------------*/
:h1 res=3120 name=PANEL_OPENDLG_HELP.Select subdirectory -- Help
:p.Use this dialog to navigate upward in the directory tree.  Enter the
full pathname of the directory whose members you want to be listed.
:p.To move downward in the tree to subdirectories, you may
close this dialog and double click on the directory name in the list box.

.*--------------------------------------------------------------*\
.*  Help for Product information                                *
.*      res = PANEL_PRODUCTINFO                                 *
.*--------------------------------------------------------------*/
:h1 res=4100 name=PANEL_PRODUCTINFO.Product information dialog
:p.This dialog displays information about the application, such
    as version number and copyright notice.


.*--------------------------------------------------------------*\
.*  About Box Dialog Ok button                                  *
.*      res = PANEL_PRODUCTINFO_OK                              *
.*--------------------------------------------------------------*/
:h1 res=4110 name=PANEL_PRODUCTINFO_OK.OK
:p.The OK button closes the Product information dialog.
