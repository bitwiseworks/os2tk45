
/*
 * This file was generated by the SOM Compiler.
 * FileName: G:\cwpsh\rel\os2c\include\base\os2\wpkeybd.h.
 * Generated using:
 *     SOM Precompiler somipc: 2.29.1.14
 *     SOM Emitter emith: 2.43
 */

/*
 *   Module Header
 * 
 *   Module Name: WPKEYBD
 * 
 *   OS/2 Presentation Manager Workplace class definitions
 * 
 *   Copyright (c) International Business Machines Corporation 1991, 1992
 * 
 */


#ifndef SOM_WPKeyboard_h
#define SOM_WPKeyboard_h


/*
 * Passthru lines: File: "C.h", "before"
 */


/***********************************************************/
/* Defines for wpclsQuerySetting and wpclsSetSetting       */
/***********************************************************/

#define REPEATRATE_DEFAULT        0
#define REPEATRATE_MAX           20
#define REPEATRATE_MIN            1
#define REPEATDELAY_DEFAULT      -1
#define REPEATDELAY_MAX         890
#define REPEATDELAY_MIN           0
#define CURSORBLINK_DEFAULT      -1
#define CURSORBLINK_MAX         890
#define CURSORBLINK_MIN           0
#define POPUPKEY_DEFAULT          0
#define TEXTEDITKEY_DEFAULT       0



#include <som.h>

/*
 * -- This emitter treats Method Tokens as Thunks by default.
 * -- Use the sc modifier "nothunks" to change this default
 */
#undef somresolve_
#define somresolve_(obj,mToken) ((somMethodProc*)((void)obj, mToken))

/*
 * Define the class name as an object type
 */
#ifndef WPKeyboard
#define WPKeyboard SOMObject
#endif
#include <wpabs.h>

/*
 * Start of bindings for IDL types
 */

#ifndef SOMClass
    #define SOMClass SOMObject
#endif /* SOMClass */
#ifndef WPFolder
    #define WPFolder SOMObject
#endif /* WPFolder */
#ifndef M_WPObject
    #define M_WPObject SOMObject
#endif /* M_WPObject */
#ifndef WPObject
    #define WPObject SOMObject
#endif /* WPObject */
#ifndef WPImageFile
    #define WPImageFile SOMObject
#endif /* WPImageFile */
#ifndef M_WPAbstract
    #define M_WPAbstract SOMObject
#endif /* M_WPAbstract */
#ifndef M_WPKeyboard
    #define M_WPKeyboard SOMObject
#endif /* M_WPKeyboard */

/*
 * End of bindings for IDL types.
 */

#define WPKeyboard_MajorVersion 1
#define WPKeyboard_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define WPKeyboardNewClass WPkbdc
#pragma linkage(WPkbdc, system)
#define WPKeyboardClassData WPkbdd
#define WPKeyboardCClassData WPkbdx
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */

/*
 * Include the meta class file
 */
#include <wpkeybd.h>
/*--------------Migration------------*/
#define WPKeyboard_classObj WPKeyboardClassData.classObject
#define _WPKeyboard WPKeyboard_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK WPKeyboardNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(WPKeyboardNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct WPKeyboardClassDataStructure {
	SOMClass *classObject;
	somMToken wpAddKeyboardMappingsPage;
	somMToken wpAddKeyboardTimingPage;
	somMToken wpAddKeyboardSpecialNeedsPage;
	somMToken wpAddKeyboardLayoutPage;
} SOMDLINK WPKeyboardClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct WPKeyboardCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK WPKeyboardCClassData;

/*
 * New and Renew macros for WPKeyboard
 */
#define WPKeyboardNew() \
   ( _WPKeyboard ? \
	_somNew(_WPKeyboard) \
	: ( WPKeyboardNewClass(\
		WPKeyboard_MajorVersion, \
		WPKeyboard_MinorVersion),\
	   _somNew(_WPKeyboard)))
#define WPKeyboardRenew(buf) \
   ( _WPKeyboard ? \
	_somRenew(_WPKeyboard, buf) \
	: ( WPKeyboardNewClass(\
		WPKeyboard_MajorVersion, \
		WPKeyboard_MinorVersion),\
	   _somRenew(_WPKeyboard, buf)))

/*
 * Override method: wpAddSettingsPages
 */
#define WPKeyboard_wpAddSettingsPages(somSelf,hwndNotebook) \
	WPObject_wpAddSettingsPages(somSelf,hwndNotebook)

/*
 * Override method: wpAddObjectWindowPage
 */
#define WPKeyboard_wpAddObjectWindowPage(somSelf,hwndNotebook) \
	WPObject_wpAddObjectWindowPage(somSelf,hwndNotebook)

/*
 * Override method: wpFilterMenu
 */
#define WPKeyboard_wpFilterMenu(somSelf,pFlags,hwndCnr,fMultiSelect,ulMenuType,ulView,ulReserved) \
	WPObject_wpFilterMenu(somSelf,pFlags,hwndCnr,fMultiSelect,ulMenuType,ulView,ulReserved)

/*
 * Override method: somDefaultInit
 */
#define WPKeyboard_somDefaultInit(somSelf,ctrl) \
	SOMObject_somDefaultInit(somSelf,ctrl)

/*
 * Override method: somDestruct
 */
#define WPKeyboard_somDestruct(somSelf,doFree,ctrl) \
	SOMObject_somDestruct(somSelf,doFree,ctrl)

/*
 * New Method: wpAddKeyboardMappingsPage
 */
typedef ULONG   SOMLINK somTP_WPKeyboard_wpAddKeyboardMappingsPage(WPKeyboard *somSelf, 
		HWND hwndNotebook);
#pragma linkage(somTP_WPKeyboard_wpAddKeyboardMappingsPage, system)
typedef somTP_WPKeyboard_wpAddKeyboardMappingsPage *somTD_WPKeyboard_wpAddKeyboardMappingsPage;
#define somMD_WPKeyboard_wpAddKeyboardMappingsPage "::WPKeyboard::wpAddKeyboardMappingsPage"
#define WPKeyboard_wpAddKeyboardMappingsPage(somSelf,hwndNotebook) \
    (SOM_Resolve(somSelf, WPKeyboard, wpAddKeyboardMappingsPage) \
	(somSelf,hwndNotebook))
#ifndef SOMGD_wpAddKeyboardMappingsPage
    #if (defined(_wpAddKeyboardMappingsPage) || defined(__wpAddKeyboardMappingsPage))
        #undef _wpAddKeyboardMappingsPage
        #undef __wpAddKeyboardMappingsPage
        #define SOMGD_wpAddKeyboardMappingsPage 1
    #else
        #define _wpAddKeyboardMappingsPage WPKeyboard_wpAddKeyboardMappingsPage
    #endif /* _wpAddKeyboardMappingsPage */
#endif /* SOMGD_wpAddKeyboardMappingsPage */

/*
 * New Method: wpAddKeyboardTimingPage
 */
typedef ULONG   SOMLINK somTP_WPKeyboard_wpAddKeyboardTimingPage(WPKeyboard *somSelf, 
		HWND hwndNotebook);
#pragma linkage(somTP_WPKeyboard_wpAddKeyboardTimingPage, system)
typedef somTP_WPKeyboard_wpAddKeyboardTimingPage *somTD_WPKeyboard_wpAddKeyboardTimingPage;
#define somMD_WPKeyboard_wpAddKeyboardTimingPage "::WPKeyboard::wpAddKeyboardTimingPage"
#define WPKeyboard_wpAddKeyboardTimingPage(somSelf,hwndNotebook) \
    (SOM_Resolve(somSelf, WPKeyboard, wpAddKeyboardTimingPage) \
	(somSelf,hwndNotebook))
#ifndef SOMGD_wpAddKeyboardTimingPage
    #if (defined(_wpAddKeyboardTimingPage) || defined(__wpAddKeyboardTimingPage))
        #undef _wpAddKeyboardTimingPage
        #undef __wpAddKeyboardTimingPage
        #define SOMGD_wpAddKeyboardTimingPage 1
    #else
        #define _wpAddKeyboardTimingPage WPKeyboard_wpAddKeyboardTimingPage
    #endif /* _wpAddKeyboardTimingPage */
#endif /* SOMGD_wpAddKeyboardTimingPage */

/*
 * New Method: wpAddKeyboardSpecialNeedsPage
 */
typedef ULONG   SOMLINK somTP_WPKeyboard_wpAddKeyboardSpecialNeedsPage(WPKeyboard *somSelf, 
		HWND hwndNotebook);
#pragma linkage(somTP_WPKeyboard_wpAddKeyboardSpecialNeedsPage, system)
typedef somTP_WPKeyboard_wpAddKeyboardSpecialNeedsPage *somTD_WPKeyboard_wpAddKeyboardSpecialNeedsPage;
#define somMD_WPKeyboard_wpAddKeyboardSpecialNeedsPage "::WPKeyboard::wpAddKeyboardSpecialNeedsPage"
#define WPKeyboard_wpAddKeyboardSpecialNeedsPage(somSelf,hwndNotebook) \
    (SOM_Resolve(somSelf, WPKeyboard, wpAddKeyboardSpecialNeedsPage) \
	(somSelf,hwndNotebook))
#ifndef SOMGD_wpAddKeyboardSpecialNeedsPage
    #if (defined(_wpAddKeyboardSpecialNeedsPage) || defined(__wpAddKeyboardSpecialNeedsPage))
        #undef _wpAddKeyboardSpecialNeedsPage
        #undef __wpAddKeyboardSpecialNeedsPage
        #define SOMGD_wpAddKeyboardSpecialNeedsPage 1
    #else
        #define _wpAddKeyboardSpecialNeedsPage WPKeyboard_wpAddKeyboardSpecialNeedsPage
    #endif /* _wpAddKeyboardSpecialNeedsPage */
#endif /* SOMGD_wpAddKeyboardSpecialNeedsPage */

/*
 * New Method: wpAddKeyboardLayoutPage
 */
typedef ULONG   SOMLINK somTP_WPKeyboard_wpAddKeyboardLayoutPage(WPKeyboard *somSelf, 
		HWND hwndNotebook);
#pragma linkage(somTP_WPKeyboard_wpAddKeyboardLayoutPage, system)
typedef somTP_WPKeyboard_wpAddKeyboardLayoutPage *somTD_WPKeyboard_wpAddKeyboardLayoutPage;
#define somMD_WPKeyboard_wpAddKeyboardLayoutPage "::WPKeyboard::wpAddKeyboardLayoutPage"
#define WPKeyboard_wpAddKeyboardLayoutPage(somSelf,hwndNotebook) \
    (SOM_Resolve(somSelf, WPKeyboard, wpAddKeyboardLayoutPage) \
	(somSelf,hwndNotebook))
#ifndef SOMGD_wpAddKeyboardLayoutPage
    #if (defined(_wpAddKeyboardLayoutPage) || defined(__wpAddKeyboardLayoutPage))
        #undef _wpAddKeyboardLayoutPage
        #undef __wpAddKeyboardLayoutPage
        #define SOMGD_wpAddKeyboardLayoutPage 1
    #else
        #define _wpAddKeyboardLayoutPage WPKeyboard_wpAddKeyboardLayoutPage
    #endif /* _wpAddKeyboardLayoutPage */
#endif /* SOMGD_wpAddKeyboardLayoutPage */
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define WPKeyboard_wpQueryHandle WPObject_wpQueryHandle
#define WPKeyboard_wpSaveImmediate WPObject_wpSaveImmediate
#define WPKeyboard_wpSaveState WPObject_wpSaveState
#define WPKeyboard_wpRestoreState WPObject_wpRestoreState
#define WPKeyboard_wpSetTitle WPObject_wpSetTitle
#define WPKeyboard_wpCopyObject WPObject_wpCopyObject
#define WPKeyboard_wpMoveObject WPObject_wpMoveObject
#define WPKeyboard_wpQueryIcon WPObject_wpQueryIcon
#define WPKeyboard_wpQueryIconData WPObject_wpQueryIconData
#define WPKeyboard_wpSetIconData WPObject_wpSetIconData
#define WPKeyboard_wpDragOver WPObject_wpDragOver
#define WPKeyboard_wpReplaceObject WPObject_wpReplaceObject
#define WPKeyboard_wpQueryNameClashOptions WPObject_wpQueryNameClashOptions
#define WPKeyboard_wpIdentify WPObject_wpIdentify
#define WPKeyboard_wpAddObjectGeneralPage WPObject_wpAddObjectGeneralPage
#define WPKeyboard_wpAddObjectGeneralPage2 WPObject_wpAddObjectGeneralPage2
#define WPKeyboard_wpAddToObjUseList WPObject_wpAddToObjUseList
#define WPKeyboard_wpAllocMem WPObject_wpAllocMem
#define WPKeyboard_wpAppendObject WPObject_wpAppendObject
#define WPKeyboard_wpAssertObjectMutexSem WPObject_wpAssertObjectMutexSem
#define WPKeyboard_wpClose WPObject_wpClose
#define WPKeyboard_wpCnrInsertObject WPObject_wpCnrInsertObject
#define WPKeyboard_wpCnrRemoveObject WPObject_wpCnrRemoveObject
#define WPKeyboard_wpCnrSetEmphasis WPObject_wpCnrSetEmphasis
#define WPKeyboard_wpConfirmDelete WPObject_wpConfirmDelete
#define WPKeyboard_wpConfirmObjectTitle WPObject_wpConfirmObjectTitle
#define WPKeyboard_wpCopiedFromTemplate WPObject_wpCopiedFromTemplate
#define WPKeyboard_wpCreateAnother WPObject_wpCreateAnother
#define WPKeyboard_wpCreateFromTemplate WPObject_wpCreateFromTemplate
#define WPKeyboard_wpCreateShadowObject WPObject_wpCreateShadowObject
#define WPKeyboard_wpDelete WPObject_wpDelete
#define WPKeyboard_wpDeleteFromObjUseList WPObject_wpDeleteFromObjUseList
#define WPKeyboard_wpDisplayHelp WPObject_wpDisplayHelp
#define WPKeyboard_wpDisplayMenu WPObject_wpDisplayMenu
#define WPKeyboard_wpDoesObjectMatch WPObject_wpDoesObjectMatch
#define WPKeyboard_wpDraggedOverObject WPObject_wpDraggedOverObject
#define WPKeyboard_wpDrop WPObject_wpDrop
#define WPKeyboard_wpDroppedOnObject WPObject_wpDroppedOnObject
#define WPKeyboard_wpEndConversation WPObject_wpEndConversation
#define WPKeyboard_wpFilterPopupMenu WPObject_wpFilterPopupMenu
#define WPKeyboard_wpFindUseItem WPObject_wpFindUseItem
#define WPKeyboard_wpFormatDragItem WPObject_wpFormatDragItem
#define WPKeyboard_wpFree WPObject_wpFree
#define WPKeyboard_wpFreeMem WPObject_wpFreeMem
#define WPKeyboard_wpHide WPObject_wpHide
#define WPKeyboard_wpInitData WPObject_wpInitData
#define WPKeyboard_wpInsertPopupMenuItems WPObject_wpInsertPopupMenuItems
#define WPKeyboard_wpInsertMenuItems WPObject_wpInsertMenuItems
#define WPKeyboard_wpInsertSettingsPage WPObject_wpInsertSettingsPage
#define WPKeyboard_wpMenuItemHelpSelected WPObject_wpMenuItemHelpSelected
#define WPKeyboard_wpMenuItemSelected WPObject_wpMenuItemSelected
#define WPKeyboard_wpModifyPopupMenu WPObject_wpModifyPopupMenu
#define WPKeyboard_wpModifyMenu WPObject_wpModifyMenu
#define WPKeyboard_wpOpen WPObject_wpOpen
#define WPKeyboard_wpPrintObject WPObject_wpPrintObject
#define WPKeyboard_wpQueryConcurrentView WPObject_wpQueryConcurrentView
#define WPKeyboard_wpQueryButtonAppearance WPObject_wpQueryButtonAppearance
#define WPKeyboard_wpQueryConfirmations WPObject_wpQueryConfirmations
#define WPKeyboard_wpQueryDefaultHelp WPObject_wpQueryDefaultHelp
#define WPKeyboard_wpQueryDefaultView WPObject_wpQueryDefaultView
#define WPKeyboard_wpQueryDetailsData WPObject_wpQueryDetailsData
#define WPKeyboard_wpQueryError WPObject_wpQueryError
#define WPKeyboard_wpSetFolder WPObject_wpSetFolder
#define WPKeyboard_wpQueryFolder WPObject_wpQueryFolder
#define WPKeyboard_wpQueryMinWindow WPObject_wpQueryMinWindow
#define WPKeyboard_wpQueryStyle WPObject_wpQueryStyle
#define WPKeyboard_wpSetTaskRec WPObject_wpSetTaskRec
#define WPKeyboard_wpFindTaskRec WPObject_wpFindTaskRec
#define WPKeyboard_wpQueryTitle WPObject_wpQueryTitle
#define WPKeyboard_wpRegisterView WPObject_wpRegisterView
#define WPKeyboard_wpReleaseObjectMutexSem WPObject_wpReleaseObjectMutexSem
#define WPKeyboard_wpRender WPObject_wpRender
#define WPKeyboard_wpRenderComplete WPObject_wpRenderComplete
#define WPKeyboard_wpRequestObjectMutexSem WPObject_wpRequestObjectMutexSem
#define WPKeyboard_wpRestore WPObject_wpRestore
#define WPKeyboard_wpRestoreData WPObject_wpRestoreData
#define WPKeyboard_wpRestoreLong WPObject_wpRestoreLong
#define WPKeyboard_wpRestoreString WPObject_wpRestoreString
#define WPKeyboard_wpSaveData WPObject_wpSaveData
#define WPKeyboard_wpSaveDeferred WPObject_wpSaveDeferred
#define WPKeyboard_wpSaveLong WPObject_wpSaveLong
#define WPKeyboard_wpSaveString WPObject_wpSaveString
#define WPKeyboard_wpScanSetupString WPObject_wpScanSetupString
#define WPKeyboard_wpSetConcurrentView WPObject_wpSetConcurrentView
#define WPKeyboard_wpSetButtonAppearance WPObject_wpSetButtonAppearance
#define WPKeyboard_wpSetDefaultHelp WPObject_wpSetDefaultHelp
#define WPKeyboard_wpSetDefaultView WPObject_wpSetDefaultView
#define WPKeyboard_wpSetError WPObject_wpSetError
#define WPKeyboard_wpQueryScreenGroupID WPObject_wpQueryScreenGroupID
#define WPKeyboard_wpSetupOnce WPObject_wpSetupOnce
#define WPKeyboard_wpSetIcon WPObject_wpSetIcon
#define WPKeyboard_wpSetMinWindow WPObject_wpSetMinWindow
#define WPKeyboard_wpModifyStyle WPObject_wpModifyStyle
#define WPKeyboard_wpSetup WPObject_wpSetup
#define WPKeyboard_wpSwitchTo WPObject_wpSwitchTo
#define WPKeyboard_wpUnInitData WPObject_wpUnInitData
#define WPKeyboard_wpViewObject WPObject_wpViewObject
#define WPKeyboard_wpQueryTrueStyle WPObject_wpQueryTrueStyle
#define WPKeyboard_wpUnlockObject WPObject_wpUnlockObject
#define WPKeyboard_wpObjectReady WPObject_wpObjectReady
#define WPKeyboard_wpIsObjectInitialized WPObject_wpIsObjectInitialized
#define WPKeyboard_wpCreateShadowObjectExt WPObject_wpCreateShadowObjectExt
#define WPKeyboard_wpCnrDeleteUseItem WPObject_wpCnrDeleteUseItem
#define WPKeyboard_wpIsDeleteable WPObject_wpIsDeleteable
#define WPKeyboard_wpQueryCoreRecord WPObject_wpQueryCoreRecord
#define WPKeyboard_wpSetObjectID WPObject_wpSetObjectID
#define WPKeyboard_wpQueryObjectID WPObject_wpQueryObjectID
#define WPKeyboard_wpSetDefaultIconPos WPObject_wpSetDefaultIconPos
#define WPKeyboard_wpQueryDefaultIconPos WPObject_wpQueryDefaultIconPos
#define WPKeyboard_wpCnrRefreshDetails WPObject_wpCnrRefreshDetails
#define WPKeyboard_wpFindViewItem WPObject_wpFindViewItem
#define WPKeyboard_wpLockObject WPObject_wpLockObject
#define WPKeyboard_wpIsLocked WPObject_wpIsLocked
#define WPKeyboard_wpQueryContainerFlagPtr WPObject_wpQueryContainerFlagPtr
#define WPKeyboard_wpWaitForClose WPObject_wpWaitForClose
#define WPKeyboard_wpSetMenuStyle WPObject_wpSetMenuStyle
#define WPKeyboard_wpQueryMenuStyle WPObject_wpQueryMenuStyle
#define WPKeyboard_wpSetStyle WPObject_wpSetStyle
#define WPKeyboard_somInit SOMObject_somInit
#define WPKeyboard_somUninit SOMObject_somUninit
#define WPKeyboard_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define WPKeyboard_somDefaultAssign SOMObject_somDefaultAssign
#define WPKeyboard_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define WPKeyboard_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define WPKeyboard_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define WPKeyboard_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define WPKeyboard_somDefaultVAssign SOMObject_somDefaultVAssign
#define WPKeyboard_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define WPKeyboard_somFree SOMObject_somFree
#define WPKeyboard_somGetClass SOMObject_somGetClass
#define WPKeyboard_somGetClassName SOMObject_somGetClassName
#define WPKeyboard_somGetSize SOMObject_somGetSize
#define WPKeyboard_somIsA SOMObject_somIsA
#define WPKeyboard_somIsInstanceOf SOMObject_somIsInstanceOf
#define WPKeyboard_somRespondsTo SOMObject_somRespondsTo
#define WPKeyboard_somDispatch SOMObject_somDispatch
#define WPKeyboard_somClassDispatch SOMObject_somClassDispatch
#define WPKeyboard_somCastObj SOMObject_somCastObj
#define WPKeyboard_somResetObj SOMObject_somResetObj
#define WPKeyboard_somDispatchV SOMObject_somDispatchV
#define WPKeyboard_somDispatchL SOMObject_somDispatchL
#define WPKeyboard_somDispatchA SOMObject_somDispatchA
#define WPKeyboard_somDispatchD SOMObject_somDispatchD
#define WPKeyboard_somPrintSelf SOMObject_somPrintSelf
#define WPKeyboard_somDumpSelf SOMObject_somDumpSelf
#define WPKeyboard_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_WPKeyboard_h */

#ifndef SOM_M_WPKeyboard_h
#define SOM_M_WPKeyboard_h


#include <som.h>

/*
 * -- This emitter treats Method Tokens as Thunks by default.
 * -- Use the sc modifier "nothunks" to change this default
 */
#undef somresolve_
#define somresolve_(obj,mToken) ((somMethodProc*)((void)obj, mToken))

/*
 * Define the class name as an object type
 */
#ifndef M_WPKeyboard
#define M_WPKeyboard SOMObject
#endif
#include <wpabs.h>

/*
 * Start of bindings for IDL types
 */


/*
 * End of bindings for IDL types.
 */

#define M_WPKeyboard_MajorVersion 1
#define M_WPKeyboard_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define M_WPKeyboardNewClass WPkbdc
#pragma linkage(WPkbdc, system)
#define M_WPKeyboardClassData WPkbdd
#define M_WPKeyboardCClassData WPkbdx
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */
/*--------------Migration------------*/
#define M_WPKeyboard_classObj M_WPKeyboardClassData.classObject
#define _M_WPKeyboard M_WPKeyboard_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK M_WPKeyboardNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(M_WPKeyboardNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct M_WPKeyboardClassDataStructure {
	SOMClass *classObject;
} SOMDLINK M_WPKeyboardClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct M_WPKeyboardCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK M_WPKeyboardCClassData;

/*
 * New and Renew macros for M_WPKeyboard
 */
#define M_WPKeyboardNew() \
   ( _M_WPKeyboard ? \
	_somNew(_M_WPKeyboard) \
	: ( M_WPKeyboardNewClass(\
		M_WPKeyboard_MajorVersion, \
		M_WPKeyboard_MinorVersion),\
	   _somNew(_M_WPKeyboard)))
#define M_WPKeyboardRenew(buf) \
   ( _M_WPKeyboard ? \
	_somRenew(_M_WPKeyboard, buf) \
	: ( M_WPKeyboardNewClass(\
		M_WPKeyboard_MajorVersion, \
		M_WPKeyboard_MinorVersion),\
	   _somRenew(_M_WPKeyboard, buf)))

/*
 * Override method: wpclsQueryTitle
 */
#define M_WPKeyboard_wpclsQueryTitle(somSelf) \
	M_WPObject_wpclsQueryTitle(somSelf)

/*
 * Override method: wpclsQueryDefaultHelp
 */
#define M_WPKeyboard_wpclsQueryDefaultHelp(somSelf,pHelpPanelId,pszHelpLibrary) \
	M_WPObject_wpclsQueryDefaultHelp(somSelf,pHelpPanelId,pszHelpLibrary)

/*
 * Override method: wpclsQueryDefaultView
 */
#define M_WPKeyboard_wpclsQueryDefaultView(somSelf) \
	M_WPObject_wpclsQueryDefaultView(somSelf)

/*
 * Override method: wpclsQueryStyle
 */
#define M_WPKeyboard_wpclsQueryStyle(somSelf) \
	M_WPObject_wpclsQueryStyle(somSelf)

/*
 * Override method: wpclsQueryIconData
 */
#define M_WPKeyboard_wpclsQueryIconData(somSelf,pIconInfo) \
	M_WPObject_wpclsQueryIconData(somSelf,pIconInfo)

/*
 * Override method: wpclsQuerySettingsPageSize
 */
#define M_WPKeyboard_wpclsQuerySettingsPageSize(somSelf,pSizl) \
	M_WPObject_wpclsQuerySettingsPageSize(somSelf,pSizl)

/*
 * Override method: wpclsQuerySetting
 */
#define M_WPKeyboard_wpclsQuerySetting(somSelf,pszSetting,pValue,ulValueLen) \
	M_WPAbstract_wpclsQuerySetting(somSelf,pszSetting,pValue,ulValueLen)

/*
 * Override method: wpclsSetSetting
 */
#define M_WPKeyboard_wpclsSetSetting(somSelf,pszSetting,pValue) \
	M_WPAbstract_wpclsSetSetting(somSelf,pszSetting,pValue)
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define M_WPKeyboard_wpclsObjectFromHandle M_WPObject_wpclsObjectFromHandle
#define M_WPKeyboard_wpclsCreateDefaultTemplates M_WPObject_wpclsCreateDefaultTemplates
#define M_WPKeyboard_wpclsInitData M_WPObject_wpclsInitData
#define M_WPKeyboard_wpclsMakeAwake M_WPObject_wpclsMakeAwake
#define M_WPKeyboard_wpclsNew M_WPObject_wpclsNew
#define M_WPKeyboard_wpclsQueryDetails M_WPObject_wpclsQueryDetails
#define M_WPKeyboard_wpclsQueryDetailsInfo M_WPObject_wpclsQueryDetailsInfo
#define M_WPKeyboard_wpclsQueryExtendedCriteria M_WPObject_wpclsQueryExtendedCriteria
#define M_WPKeyboard_wpclsQueryFolder M_WPObject_wpclsQueryFolder
#define M_WPKeyboard_wpclsQueryIcon M_WPObject_wpclsQueryIcon
#define M_WPKeyboard_wpclsQuerySearchInfo M_WPObject_wpclsQuerySearchInfo
#define M_WPKeyboard_wpclsUnInitData M_WPObject_wpclsUnInitData
#define M_WPKeyboard_wpclsFindObjectFirst M_WPObject_wpclsFindObjectFirst
#define M_WPKeyboard_wpclsFindObjectNext M_WPObject_wpclsFindObjectNext
#define M_WPKeyboard_wpclsFindObjectEnd M_WPObject_wpclsFindObjectEnd
#define M_WPKeyboard_wpclsSetError M_WPObject_wpclsSetError
#define M_WPKeyboard_wpclsQueryError M_WPObject_wpclsQueryError
#define M_WPKeyboard_wpclsQueryObject M_WPObject_wpclsQueryObject
#define M_WPKeyboard_wpclsQueryButtonAppearance M_WPObject_wpclsQueryButtonAppearance
#define M_WPKeyboard_wpclsDecUsage M_WPObject_wpclsDecUsage
#define M_WPKeyboard_wpclsIncUsage M_WPObject_wpclsIncUsage
#define M_WPKeyboard_wpclsInsertMultipleObjects M_WPObject_wpclsInsertMultipleObjects
#define M_WPKeyboard_wpclsFindOneObject M_WPObject_wpclsFindOneObject
#define M_WPKeyboard_wpclsSetSettingsPageSize M_WPObject_wpclsSetSettingsPageSize
#define M_WPKeyboard_wpclsSetIconData M_WPObject_wpclsSetIconData
#define M_WPKeyboard_wpclsSetIcon M_WPObject_wpclsSetIcon
#define M_WPKeyboard_wpclsRemoveObjects M_WPObject_wpclsRemoveObjects
#define M_WPKeyboard_somUninit SOMObject_somUninit
#define M_WPKeyboard_somClassReady SOMClass_somClassReady
#define M_WPKeyboard_somNew SOMClass_somNew
#define M_WPKeyboard_somRenew SOMClass_somRenew
#define M_WPKeyboard__get_somDataAlignment SOMClass__get_somDataAlignment
#define M_WPKeyboard__get_somInstanceDataOffsets SOMClass__get_somInstanceDataOffsets
#define M_WPKeyboard__get_somDirectInitClasses SOMClass__get_somDirectInitClasses
#define M_WPKeyboard__set_somDirectInitClasses SOMClass__set_somDirectInitClasses
#define M_WPKeyboard_somNewNoInit SOMClass_somNewNoInit
#define M_WPKeyboard_somRenewNoInit SOMClass_somRenewNoInit
#define M_WPKeyboard_somRenewNoZero SOMClass_somRenewNoZero
#define M_WPKeyboard_somRenewNoInitNoZero SOMClass_somRenewNoInitNoZero
#define M_WPKeyboard_somAllocate SOMClass_somAllocate
#define M_WPKeyboard_somDeallocate SOMClass_somDeallocate
#define M_WPKeyboard__get_somClassAllocate SOMClass__get_somClassAllocate
#define M_WPKeyboard__get_somClassDeallocate SOMClass__get_somClassDeallocate
#define M_WPKeyboard_somGetInstanceInitMask SOMClass_somGetInstanceInitMask
#define M_WPKeyboard_somGetInstanceDestructionMask SOMClass_somGetInstanceDestructionMask
#define M_WPKeyboard_somGetInstanceAssignmentMask SOMClass_somGetInstanceAssignmentMask
#define M_WPKeyboard_somInitClass SOMClass_somInitClass
#define M_WPKeyboard_somInitMIClass SOMClass_somInitMIClass
#define M_WPKeyboard_somAddStaticMethod SOMClass_somAddStaticMethod
#define M_WPKeyboard_somAddDynamicMethod SOMClass_somAddDynamicMethod
#define M_WPKeyboard_somOverrideSMethod SOMClass_somOverrideSMethod
#define M_WPKeyboard_somGetApplyStub SOMClass_somGetApplyStub
#define M_WPKeyboard_somGetClassData SOMClass_somGetClassData
#define M_WPKeyboard_somSetClassData SOMClass_somSetClassData
#define M_WPKeyboard_somGetClassMtab SOMClass_somGetClassMtab
#define M_WPKeyboard_somGetInstanceOffset SOMClass_somGetInstanceOffset
#define M_WPKeyboard_somGetInstancePartSize SOMClass_somGetInstancePartSize
#define M_WPKeyboard_somGetInstanceSize SOMClass_somGetInstanceSize
#define M_WPKeyboard_somGetInstanceToken SOMClass_somGetInstanceToken
#define M_WPKeyboard_somGetMemberToken SOMClass_somGetMemberToken
#define M_WPKeyboard_somGetMethodData SOMClass_somGetMethodData
#define M_WPKeyboard_somGetRdStub SOMClass_somGetRdStub
#define M_WPKeyboard_somGetMethodDescriptor SOMClass_somGetMethodDescriptor
#define M_WPKeyboard_somGetMethodIndex SOMClass_somGetMethodIndex
#define M_WPKeyboard_somGetMethodToken SOMClass_somGetMethodToken
#define M_WPKeyboard_somGetName SOMClass_somGetName
#define M_WPKeyboard_somGetNthMethodData SOMClass_somGetNthMethodData
#define M_WPKeyboard_somGetNthMethodInfo SOMClass_somGetNthMethodInfo
#define M_WPKeyboard_somGetNumMethods SOMClass_somGetNumMethods
#define M_WPKeyboard_somGetNumStaticMethods SOMClass_somGetNumStaticMethods
#define M_WPKeyboard_somGetParent SOMClass_somGetParent
#define M_WPKeyboard_somGetParents SOMClass_somGetParents
#define M_WPKeyboard_somGetPClsMtab SOMClass_somGetPClsMtab
#define M_WPKeyboard_somGetPClsMtabs SOMClass_somGetPClsMtabs
#define M_WPKeyboard_somGetVersionNumbers SOMClass_somGetVersionNumbers
#define M_WPKeyboard_somSetMethodDescriptor SOMClass_somSetMethodDescriptor
#define M_WPKeyboard_somFindMethod SOMClass_somFindMethod
#define M_WPKeyboard_somFindMethodOk SOMClass_somFindMethodOk
#define M_WPKeyboard_somFindSMethod SOMClass_somFindSMethod
#define M_WPKeyboard_somFindSMethodOk SOMClass_somFindSMethodOk
#define M_WPKeyboard_somLookupMethod SOMClass_somLookupMethod
#define M_WPKeyboard_somCheckVersion SOMClass_somCheckVersion
#define M_WPKeyboard_somDescendedFrom SOMClass_somDescendedFrom
#define M_WPKeyboard_somSupportsMethod SOMClass_somSupportsMethod
#define M_WPKeyboard_somDefinedMethod SOMClass_somDefinedMethod
#define M_WPKeyboard_somOverrideMtab SOMClass_somOverrideMtab
#define M_WPKeyboard_somDefaultInit SOMObject_somDefaultInit
#define M_WPKeyboard_somDestruct SOMObject_somDestruct
#define M_WPKeyboard_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define M_WPKeyboard_somDefaultAssign SOMObject_somDefaultAssign
#define M_WPKeyboard_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define M_WPKeyboard_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define M_WPKeyboard_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define M_WPKeyboard_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define M_WPKeyboard_somDefaultVAssign SOMObject_somDefaultVAssign
#define M_WPKeyboard_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define M_WPKeyboard_somInit SOMObject_somInit
#define M_WPKeyboard_somFree SOMObject_somFree
#define M_WPKeyboard_somGetClass SOMObject_somGetClass
#define M_WPKeyboard_somGetClassName SOMObject_somGetClassName
#define M_WPKeyboard_somGetSize SOMObject_somGetSize
#define M_WPKeyboard_somIsA SOMObject_somIsA
#define M_WPKeyboard_somIsInstanceOf SOMObject_somIsInstanceOf
#define M_WPKeyboard_somRespondsTo SOMObject_somRespondsTo
#define M_WPKeyboard_somDispatch SOMObject_somDispatch
#define M_WPKeyboard_somClassDispatch SOMObject_somClassDispatch
#define M_WPKeyboard_somCastObj SOMObject_somCastObj
#define M_WPKeyboard_somResetObj SOMObject_somResetObj
#define M_WPKeyboard_somDispatchV SOMObject_somDispatchV
#define M_WPKeyboard_somDispatchL SOMObject_somDispatchL
#define M_WPKeyboard_somDispatchA SOMObject_somDispatchA
#define M_WPKeyboard_somDispatchD SOMObject_somDispatchD
#define M_WPKeyboard_somPrintSelf SOMObject_somPrintSelf
#define M_WPKeyboard_somDumpSelf SOMObject_somDumpSelf
#define M_WPKeyboard_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_M_WPKeyboard_h */