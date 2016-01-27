/*========================================================================*/
/*                                                                        */
/*  smcallb.h                                                             */
/*                                                                        */
/*   (C) COPYRIGHT International Business Machines Corp. 1992,1996        */
/*   All Rights Reserved                                                  */
/*   Licensed Materials - Property of IBM                                 */
/*   US Government Users Restricted Rights - Use, duplication or          */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.    */
/*                                                                        */
/*========================================================================*/

#ifndef SM_CALLB_H_INCLUDED
#define SM_CALLB_H_INCLUDED

/*-------------------------------------------------------------------------*/
/* public macros for the speech manager callback interface                 */
/*-------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/* Speech Manager Callback handler definition. Sample looks like:         */
/*                                                                        */
/* SmHandler Callback_Handler ( SM_MSG  reply,       message from speech  */
/*                                                   manager              */
/*                              caddr_t client_data, data supplied during */
/*                                                   registration to      */
/*                                                   message dispatcher   */
/*                              caddr_t call_data);  dynamic data         */
/*                                                   supplied by message  */
/*                                                   dispatcher           */
/*------------------------------------------------------------------------*/

typedef int	SmHandler;

#ifndef _AIX
typedef	char*	caddr_t;
#else
#include <sys/types.h>
#endif

/*------------------------------------------------------------------------*/
/* callback reasons                                                       */
/*------------------------------------------------------------------------*/
#define SmCR_RECV_ERROR                    1
#define SmCR_REPLY_MESSAGE                 2

/*------------------------------------------------------------------------*/
/* Reply message structure.                                               */
/*------------------------------------------------------------------------*/
typedef struct {
 int                   reason;
 SmSesId               ses_id;
 char                 *app_name;
 char                 *reply_name;
 int                   reply_rc;
} SmReplyMessageStruct;

/*------------------------------------------------------------------------*/
/* put a wrapper around these functions so that they can be called from   */
/* either C or C++ applications                                           */
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/* function prototypes for the setting of callbacks                       */
/*------------------------------------------------------------------------*/
int SmAddCallback      ( char            *reply_name,
                         SmHandler       (*handler) ( SM_MSG  reply,
                                                      caddr_t client,
                                                      caddr_t call ),
                         caddr_t          client_data );

int SmSesAddCallback   ( SmSesId          session_id,
                         char            *reply_name,
                         SmHandler       (*handler) ( SM_MSG reply,
			                              caddr_t client,
					              caddr_t call ),
                         caddr_t          client_data );

/*------------------------------------------------------------------------*/
/* function prototypes for the remove of callbacks                        */
/*------------------------------------------------------------------------*/
int SmRemoveCallback     ( char            *reply_name,
                           SmHandler       (*handler) ( SM_MSG  reply,
                                                        caddr_t client,
							caddr_t call ),
                           caddr_t          client_data );

int SmSesRemoveCallback  ( SmSesId          session_id,
                           char            *reply_name,
                           SmHandler       (*handler) ( SM_MSG  reply,
 						        caddr_t client,
						        caddr_t call ),
			   caddr_t          client_data );

#ifdef __cplusplus
   }
#endif

#endif /* not SM_CALLB_H_INCLUDED */
