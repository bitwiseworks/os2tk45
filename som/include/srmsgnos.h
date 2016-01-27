/*
 *   COMPONENT_NAME: somr
 *
 *   ORIGINS: 27
 *
 *
 *   10H9767, 10H9769  (C) COPYRIGHT International Business Machines Corp. 1992,1994
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) 2.6 src/somr/srmsgnos.h, somr, som2.1 9/1/94 15:53:37 [7/30/96 14:49:37] */

/*
 */


#ifndef _SRMSGNOS_H
#define _SRMSGNOS_H  1

/* WIN16MERGE ADD1 - for definitions */
#ifdef _WIN16
#include <som.h>
#endif

#define SOFTDR_EB       4000
#define SOM_FATAL       4
#define SOM_WARN        3

/* The following are informative msgs only */
#define SOFTDR_INFO            (SOFTDR_EB +900)
#define SOFTDR_SOCKPORT_NUM    (SOFTDR_EB +901)

#define SOFTDR_IGNORED_MSG      (SOFTDR_EB +800 +SOM_WARN)
#define SOFTDR_SCF_OPEN_ERROR   (SOFTDR_EB +810 +SOM_WARN)
#define SOFTDR_SCF_OPEN_RETRIES (SOFTDR_EB +820 +SOM_WARN)


/* The following are error messages */
#define SOFTDR_SOCKET_RDERR      (SOFTDR_EB +110+SOM_FATAL)
#define SOFTDR_SOCKET_WRERR      (SOFTDR_EB +120+SOM_FATAL)
#define SOFTDR_SOCKACCEPT_ERR    (SOFTDR_EB +130+SOM_FATAL)
#define SOFTDR_UNKNOWN_HOST      (SOFTDR_EB +140+SOM_FATAL)
#define SOFTDR_SOCK_OPEN_ERR     (SOFTDR_EB +150+SOM_FATAL)
#define SOFTDR_SOCK_CONNECT_ERR  (SOFTDR_EB +160+SOM_FATAL)
#define SOFTDR_SOCK_BINDING_ERR  (SOFTDR_EB +170+SOM_FATAL)
#define SOFTDR_SOCK_NAME_ERR     (SOFTDR_EB +180+SOM_FATAL)

#define SOFTDR_NO_CONNECTION            (SOFTDR_EB +200+SOM_FATAL)
#define SOFTDR_ILLEGAL_HANDLE           (SOFTDR_EB +210+SOM_WARN)
#define SOFTDR_NO_SUCH_OBJ              (SOFTDR_EB +220+SOM_FATAL)
#define SOFTDR_SHR_OBJ_TAB_OVERFLOW     (SOFTDR_EB +230+SOM_FATAL)
#define SOFTDR_SHR_E_TAB_OVERFLOW       (SOFTDR_EB +235+SOM_FATAL)
#define SOFTDR_CONNTAB_OVERFLOW         (SOFTDR_EB +240+SOM_FATAL)
#define SOFTDR_CANNOT_UNREG             (SOFTDR_EB +250+SOM_FATAL)
#define SOFTDR_REPBOOT_NOT_REPLACED     (SOFTDR_EB +260+SOM_FATAL)
#define SOFTDR_OUT_OF_MEMORY            (SOFTDR_EB +270+SOM_FATAL)

/* The following are error messages from repmgr.c */

#define SOFTDR_DELAYED_REPLY            (SOFTDR_EB +300+SOM_WARN)
#define SOFTDR_ILLEGAL_OPCODE           (SOFTDR_EB +310+SOM_WARN)
#define SOFTDR_REG_UNKNOWN_SHADO        (SOFTDR_EB +320+SOM_FATAL)
#define SOFTDR_UNREG_UNKNOWN_SHADO      (SOFTDR_EB +330+SOM_FATAL)
#define SOFTDR_MODECHG__UNKNOWN_SHADO   (SOFTDR_EB +340+SOM_FATAL)
#define SOFTDR_NOLOCK_WITH_HOLDER       (SOFTDR_EB +350+SOM_FATAL)
#define SOFTDR_UNKNOWN_LOGTYPE          (SOFTDR_EB +360+SOM_FATAL)
#define SOFTDR_FIRED_BUT_MASTER         (SOFTDR_EB +370+SOM_FATAL)
#define SOFTDR_UNABLE_TO_CONNECT        (SOFTDR_EB +380+SOM_WARN)
#define SOFTDR_TIMESTAMP_UPDT_FAIL      (SOFTDR_EB +390+SOM_FATAL)
#define SOFTDR_NOTAKERS                 (SOFTDR_EB +400+SOM_FATAL)
#define SOFTDR_MASTER_CANT_CHGMODE      (SOFTDR_EB +410+SOM_FATAL)

/* Prototype definition for soft_getstr   */
#ifdef _WIN16
extern char * W16LOADDS soft_getstr(long msgcode);
#else
extern char *soft_getstr(long msgcode);
#endif

#endif /* _SRMSGNOS_H */
