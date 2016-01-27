/*static char *SCCSID = "@(#)avcinst.h  13.4 92/04/21";*/
/*************************************************************************
 * File Name    : AVCINST.H
 *
 * Description  : This file contains the definitions used by the Avcinst
 *                sample program.
 *
 * Concepts     : No Multimedia concepts will be shown.
 *
 * MMPM/2 API's : None used.
 *
 * Required
 *    Files     : None.
 *
 *
 * Copyright (C) IBM 1991, 1993
 *************************************************************************/

/*
 * Resource specific definitions.
 */
#define ID_MAIN_DIALOG_BOX                                        1000
#define ID_INSTAL                                                 1001
#define ID_DEINSTAL                                               1002

#define ID_OK_PB                                                 1600
#define ID_CANCEL_PB                                              1700
#define ID_HELP_PB                                                1800

#define ID_AVC_HELPTABLE                                          2000

#define ID_ICON                                                   1000


/*
 * Misc. Definitions.
 */

#define NO_ALLOC_ERROR                                          0

#define STRING_SIZE                                             256

#define ID_MESSAGE_BOX                                          1
#define SUBTABLE_MESSAGEBOX                                     2001

/*
 * New Window Messages.
 */
#define MC_FINISHED_INITDLG                                    WM_USER + 1
#define FILE_NAME_SIZE                                         CCHMAXPATH
/*
 * String defines for the string table.
 */
#define IDS_HELP_WINDOW_TITLE                                   1
#define IDS_MESSAGE_BOX_TITLE                                   2
#define IDS_MMIO_INSTALLPROC_NAME                               3
#define IDS_MMIO_INSTALLPROC_DLL_NAME                           4
#define IDS_HELP_LIBRARY_FILE                                   5
#define IDS_MAIN_WINDOW_TITLE                                   6

#define NUMBER_OF_PROGRAM_STRINGS                               6

#define IDS_HELP_CREATION_FAILED                                7
#define IDS_INSTALLATION_IS_COMPLETE                            8
#define IDS_FIND_FIRST_FAILED                                   9
#define IDS_MMIOPROC_COULD_NOT_BE_INSTALLED                     10
#define IDS_ALLOCATION_OF_MEMORY_FAILED                         11
#define IDS_LOADMODULE_FAILED                                   12
#define IDS_QUERY_PROC_ADDR_FAILED                              13
#define IDS_TARGET_FILE_EXISTS                                  14
#define IDS_INSTALL_FIRST                                       15
#define IDS_ALREADY_INSTALLED                                   16
#define IDS_DEFAULTSIZE                                         17
