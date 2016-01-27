/******************************************************************************
*
*  File Name  : SHOWDESC.H
*
*  Description: This include file contains the constants, structures,
*               and function prototypes required for the application.
*
*  Copyright (C) 1993 IBM Corporation
*
*      DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*      sample code created by IBM Corporation. This sample code is not
*      part of any standard or IBM product and is provided to you solely
*      for  the purpose of assisting you in the development of your
*      applications.  The code is provided "AS IS", without
*      warranty of any kind.  IBM shall not be liable for any damages
*      arising out of your use of the sample code, even if they have been
*      advised of the possibility of such damages.                                                    *
*
******************************************************************************/

/*-----------------------------------------------------------------------------
*    INCLUDE FILES
*----------------------------------------------------------------------------*/
#include "methodid.h"

/*-----------------------------------------------------------------------------
*    WINDOW IDENTIFIERS
*----------------------------------------------------------------------------*/
#define IDR_MAIN           1     /* Frame window ID                          */
#define IDD_MSGBOX      1001     /* Message Box ID                           */
#define ID_MLE          1002     /* Multi-line Entry Field window ID         */

/*-----------------------------------------------------------------------------
*    STRINGTABLE IDS
*----------------------------------------------------------------------------*/
#define IDS_APPNAME              1  /* Application name                      */
#define IDS_TITLEBARSEPARATOR    2  /* Method name/description separator     */
#define IDS_WARNING              3  /* "Warning" string                      */
#define IDS_HARD_BREAK           4  /* Description separator                 */

/*-----------------------------------------------------------------------------
*    MESSAGETABLE IDS
*----------------------------------------------------------------------------*/
#define IDMSG_INITFAILED              1   /* Initialization failed           */
#define IDMSG_CANNOTLOADEXITLIST      2   /* Cannot load exit list processor */
#define IDM_UNIDENTIFIED_METHOD       3   /* Unidentified method (name)      */
#define IDM_UNIDENTIFIED_METHOD_MSG   4   /* Unidentified method             */
                                          /*   (description)                 */

        /*---------------------------------------------------------------------
        *    IDS FOR WPS INSTANCE METHOD DESCRIPTIONS
        *--------------------------------------------------------------------*/
#define IDS_WPADDFILEMENUPAGE_MSG      IDS_WPADDFILEMENUPAGE+1
#define IDS_WPADDFILETYPEPAGE_MSG      IDS_WPADDFILETYPEPAGE+1
#define IDS_WPADDFILE1PAGE_MSG         IDS_WPADDFILE1PAGE+1
#define IDS_WPADDFILE2PAGE_MSG         IDS_WPADDFILE2PAGE+1
#define IDS_WPADDFILE3PAGE_MSG         IDS_WPADDFILE3PAGE+1
#define IDS_WPADDOBJECTGENERALPAGE_MSG IDS_WPADDOBJECTGENERALPAGE+1
#define IDS_WPADDSETTINGSPAGES_MSG     IDS_WPADDSETTINGSPAGES+1
#define IDS_WPADDTOOBJUSELIST_MSG      IDS_WPADDTOOBJUSELIST+1
#define IDS_WPALLOCMEM_MSG             IDS_WPALLOCMEM+1
#define IDS_WPCLOSE_MSG                IDS_WPCLOSE+1
#define IDS_WPCOPIEDFROMTEMPLATE_MSG   IDS_WPCOPIEDFROMTEMPLATE+1
#define IDS_WPCOPYOBJECT_MSG           IDS_WPCOPYOBJECT+1
#define IDS_WPCREATEFROMTEMPLATE_MSG   IDS_WPCREATEFROMTEMPLATE+1
#define IDS_WPCREATESHADOWOBJECT_MSG   IDS_WPCREATESHADOWOBJECT+1
#define IDS_WPDELETE_MSG               IDS_WPDELETE+1
#define IDS_WPDELETEFROMOBJUSELIST_MSG IDS_WPDELETEFROMOBJUSELIST+1
#define IDS_WPDOESOBJECTMATCH_MSG      IDS_WPDOESOBJECTMATCH+1
#define IDS_WPDRAGGEDOVEROBJECT_MSG    IDS_WPDRAGGEDOVEROBJECT+1
#define IDS_WPDRAGOVER_MSG             IDS_WPDRAGOVER+1
#define IDS_WPDROP_MSG                 IDS_WPDROP+1
#define IDS_WPDROPPEDONOBJECT_MSG      IDS_WPDROPPEDONOBJECT+1
#define IDS_WPENDCONVERSATION_MSG      IDS_WPENDCONVERSATION+1
#define IDS_WPFILTERPOPUPMENU_MSG      IDS_WPFILTERPOPUPMENU+1
#define IDS_WPFINDUSEITEM_MSG          IDS_WPFINDUSEITEM+1
#define IDS_WPFORMATDRAGITEM_MSG       IDS_WPFORMATDRAGITEM+1
#define IDS_WPFREE_MSG                 IDS_WPFREE+1
#define IDS_WPFREEMEM_MSG              IDS_WPFREEMEM+1
#define IDS_WPHIDE_MSG                 IDS_WPHIDE+1
#define IDS_WPINITDATA_MSG             IDS_WPINITDATA+1
#define IDS_WPINSERTPOPUPMENUITEMS_MSG IDS_WPINSERTPOPUPMENUITEMS+1
#define IDS_WPINSERTSETTINGSPAGE_MSG   IDS_WPINSERTSETTINGSPAGE+1
#define IDS_WPMENUITEMSELECTED_MSG     IDS_WPMENUITEMSELECTED+1
#define IDS_WPMODIFYPOPUPMENU_MSG      IDS_WPMODIFYPOPUPMENU+1
#define IDS_WPOPEN_MSG                 IDS_WPOPEN+1
#define IDS_WPQUERYCONFIRMATIONS_MSG   IDS_WPQUERYCONFIRMATIONS+1
#define IDS_WPQUERYDEFAULTVIEW_MSG     IDS_WPQUERYDEFAULTVIEW+1
#define IDS_WPQUERYDETAILSDATA_MSG     IDS_WPQUERYDETAILSDATA+1
#define IDS_WPQUERYHANDLE_MSG          IDS_WPQUERYHANDLE+1
#define IDS_WPQUERYICON_MSG            IDS_WPQUERYICON+1
#define IDS_WPQUERYICONDATA_MSG        IDS_WPQUERYICONDATA+1
#define IDS_WPQUERYREALNAME_MSG        IDS_WPQUERYREALNAME+1
#define IDS_WPQUERYSTYLE_MSG           IDS_WPQUERYSTYLE+1
#define IDS_WPQUERYTITLE_MSG           IDS_WPQUERYTITLE+1
#define IDS_WPQUERYTYPE_MSG            IDS_WPQUERYTYPE+1
#define IDS_WPREGISTERVIEW_MSG         IDS_WPREGISTERVIEW+1
#define IDS_WPRENDER_MSG               IDS_WPRENDER+1
#define IDS_WPRENDERCOMPLETE_MSG       IDS_WPRENDERCOMPLETE+1
#define IDS_WPRESTORE_MSG              IDS_WPRESTORE+1
#define IDS_WPRESTOREDATA_MSG          IDS_WPRESTOREDATA+1
#define IDS_WPRESTORELONG_MSG          IDS_WPRESTORELONG+1
#define IDS_WPRESTORESTATE_MSG         IDS_WPRESTORESTATE+1
#define IDS_WPRESTORESTRING_MSG        IDS_WPRESTORESTRING+1
#define IDS_WPSAVEDATA_MSG             IDS_WPSAVEDATA+1
#define IDS_WPSAVEDEFERRED_MSG         IDS_WPSAVEDEFERRED+1
#define IDS_WPSAVEIMMEDIATE_MSG        IDS_WPSAVEIMMEDIATE+1
#define IDS_WPSAVELONG_MSG             IDS_WPSAVELONG+1
#define IDS_WPSAVESTATE_MSG            IDS_WPSAVESTATE+1
#define IDS_WPSAVESTRING_MSG           IDS_WPSAVESTRING+1
#define IDS_WPSETDEFAULTVIEW_MSG       IDS_WPSETDEFAULTVIEW+1
#define IDS_WPSETICON_MSG              IDS_WPSETICON+1
#define IDS_WPSETICONDATA_MSG          IDS_WPSETICONDATA+1
#define IDS_WPSETREALNAME_MSG          IDS_WPSETREALNAME+1
#define IDS_WPSETSTYLE_MSG             IDS_WPSETSTYLE+1
#define IDS_WPSETTITLE_MSG             IDS_WPSETTITLE+1
#define IDS_WPSETTYPE_MSG              IDS_WPSETTYPE+1
#define IDS_WPSETUP_MSG                IDS_WPSETUP+1
#define IDS_WPSWITCHTO_MSG             IDS_WPSWITCHTO+1
#define IDS_WPUNLOCKOBJECT_MSG         IDS_WPUNLOCKOBJECT+1
#define IDS_WPUNINITDATA_MSG           IDS_WPUNINITDATA+1

        /*---------------------------------------------------------------------
        *    IDS FOR WPS CLASS METHOD DESCRIPTIONS
        *--------------------------------------------------------------------*/
#define IDS_WPCLSCREATEDEFATULTTEMPLATES_MSG IDS_WPCLSCREATEDEFAULTTEMPLATES+1
#define IDS_WPCLSFINDOBJECTEND_MSG     IDS_WPCLSFINDOBJECTEND+1
#define IDS_WPCLSFINDOBJECTFIRST_MSG   IDS_WPCLSFINDOBJECTFIRST+1
#define IDS_WPCLSFINDOBJECTNEXT_MSG    IDS_WPCLSFINDOBJECTNEXT+1
#define IDS_WPCLSINITDATA_MSG          IDS_WPCLSINITDATA+1
#define IDS_WPCLSMAKEAWAKE_MSG         IDS_WPCLSMAKEAWAKE+1
#define IDS_WPCLSNEW_MSG               IDS_WPCLSNEW+1
#define IDS_WPCLSQUERYDEFAULTVIEW_MSG  IDS_WPCLSQUERYDEFAULTVIEW+1
#define IDS_WPCLSQUERYDETAILS_MSG      IDS_WPCLSQUERYDETAILS+1
#define IDS_WPCLSQUERYDETAILSINFO_MSG  IDS_WPCLSQUERYDETAILSINFO+1
#define IDS_WPCLSQUERYFOLDER_MSG       IDS_WPCLSQUERYFOLDER+1
#define IDS_WPCLSQUERYICON_MSG         IDS_WPCLSQUERYICON+1
#define IDS_WPCLSQUERYICONDATA_MSG     IDS_WPCLSQUERYICONDATA+1
#define IDS_WPCLSQUERYINSTANCEFILTER_MSG IDS_WPCLSQUERYINSTANCEFILTER+1
#define IDS_WPCLSQUERYINSTANCETYPE_MSG IDS_WPCLSQUERYINSTANCETYPE+1
#define IDS_WPCLSQUERYOBJECT_MSG       IDS_WPCLSQUERYOBJECT+1
#define IDS_WPCLSQUERYOPENFOLDERS_MSG  IDS_WPCLSQUERYOPENFOLDERS+1
#define IDS_WPCLSQUERYSETTINGSPAGESIZE_MSG IDS_WPCLSQUERYSETTINGSPAGESIZE+1
#define IDS_WPCLSQUERYSTYLE_MSG        IDS_WPCLSQUERYSTYLE+1
#define IDS_WPCLSQUERYTITLE_MSG        IDS_WPCLSQUERYTITLE+1
#define IDS_WPCLSUNINITDATA_MSG        IDS_WPCLSUNINITDATA+1

/*-----------------------------------------------------------------------------
*    WINDOW MESSAGES
*----------------------------------------------------------------------------*/
#define IDM_EDIT         (WM_USER+100)    /* "Edit" menu                     */
#define IDM_EDITCOPY     (WM_USER+101)    /* "Copy" item on "Edit" menu      */
#define IDM_EDITCLEAR    (WM_USER+102)    /* "Clear" item on "Edit" menu     */
#define IDM_EDITCLEARALL (WM_USER+103)    /* "Clear All" item on "Edit" menu */
#define IDM_SHOW_METHOD  (WM_USER+104)    /* Show method description msg     */

/*-----------------------------------------------------------------------------
*    OTHER DEFINES
*----------------------------------------------------------------------------*/
#define MESSAGELEN          100   /* Maximum length for message box message  */

#define TITLELENGTH          30   /* Maximum length for method title         */
#define METHODLENGTH        255   /* Maximum length for method description   */
#define TITLESEPARATORLEN     4   /* Length of name/description separator    */
#define HARD_BREAK_LENGTH     3   /* Length of the hard break string         */

/*-----------------------------------------------------------------------------
*    FUNCTION DECLARATIONS
*----------------------------------------------------------------------------*/
MRESULT EXPENTRY MainWndProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2);

SHORT MessageBox(HWND hwndOwner, SHORT idMsg, SHORT fsStyle, BOOL fBeep);

BOOL Init(VOID);

MRESULT InitMainWindow(HWND hwnd, MPARAM mp1, MPARAM mp2);

VOID APIENTRY ExitProc(USHORT usTermCode);

MRESULT ShowMethodDescription(ULONG ulMethodId);

VOID MonitorMethodQueue(VOID);
