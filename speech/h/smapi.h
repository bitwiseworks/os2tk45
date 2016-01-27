/*========================================================================*/
/*                                                                        */
/*  smapi.h                                                               */
/*                                                                        */
/*   (C) COPYRIGHT International Business Machines Corp. 1992,1996        */
/*   All Rights Reserved                                                  */
/*   Licensed Materials - Property of IBM                                 */
/*   US Government Users Restricted Rights - Use, duplication or          */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.    */
/*                                                                        */
/*========================================================================*/

#ifndef SM_API_H_INCLUDED
#define SM_API_H_INCLUDED

/*------------------------------------------------------------------------*/
/* Define the running speech manager API level                            */
/*------------------------------------------------------------------------*/
#define SM_API_VERSION_STRING  "4.0"

/*------------------------------------------------------------------------*/
/* Define for parallel session handles.                                   */
/*------------------------------------------------------------------------*/
typedef void *SmSesId;

/*-------------------------------------------------------------------------*/
/* speech manager includes to handle arguments, communication and access   */
/* to reply messages                                                       */
/*-------------------------------------------------------------------------*/
#include <smrc.h>		/* SM_RC_... constants			   */
#include <smlimits.h>		/* SM_MAX_... constants			   */
#include <smcomm.h>		/* SM_MSG SM_[VOC]WORD & SM_... constants  */
#include <smargs.h>		/* SmSetArg functions & SmN... constants   */
#include <smmsg.h>		/* SmGet... access functions		   */
#include <smcallb.h>		/* SmAdd/RemoveCallback functions	   */

/*-------------------------------------------------------------------------*/
/* speech manager macros used by the application                           */
/*-------------------------------------------------------------------------*/
#define SmAsynchronous    ((SM_MSG *) 0)

/*------------------------------------------------------------------------*/
/* put a wrapper around these functions so that they can be called from   */
/* either C or C++ applications                                           */
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------*/
/* speech manager functions used by application                            */
/*-------------------------------------------------------------------------*/
int SmAddPronunciation    ( char          *spelling,
			    char          *pronunciation,
			    long           uttno,
                            short          repetitions,
			    long           options,
			    SM_MSG        *reply );

int SmAddToVocab          ( char          *vocab,
			    short          nvocwords,
                            SM_VOCWORD    *vocwords[],
			    SM_MSG        *reply );

int SmApiVersionCheck     ( char          *caller_version,
			    char         **sm_version );

int SmCancelPlayback      ( SM_MSG        *reply );

int SmClose               ( void );

int SmConnect             ( int            nargs,
			    SmArg         *Args,
			    SM_MSG        *reply );

int SmCorrectText         ( short          nwords,
			    SM_WORD       *words[],
			    SM_MSG        *reply );

int SmCorrectTextCancel   ( SM_MSG        *reply );

int SmDefineGrammar       ( char          *vocab,
		            char          *grammar,
			    long           options,
		            SM_MSG        *reply );

int SmDefineVocab         ( char          *vocab,
			    short          nvocwords,
                            SM_VOCWORD    *vocwords[],
			    SM_MSG        *reply );

int SmDisableVocab        ( char          *vocab,
			    SM_MSG        *reply );

int SmDiscardData         ( SM_MSG        *reply );

int SmDisconnect          ( int            nargs,
			    SmArg         *Args,
			    SM_MSG        *reply );

int SmDispatch            ( unsigned long  ap_val );

int SmDetachSessions      ( SM_MSG        *reply );

int SmEnableVocab         ( char          *vocab,
			    SM_MSG        *reply );

int SmEventNotify         ( long           event_id,
			    long           options,
			    SM_MSG        *reply );

int SmHaltRecognizer      ( SM_MSG        *reply );

int SmMicOff              ( SM_MSG        *reply );

int SmMicOn               ( SM_MSG        *reply );

int SmNewContext          ( short          nwords,
			    SM_WORD       *words[],
			    SM_MSG        *reply );

int SmOpen                ( int            nargs,
			    SmArg         *Args );

int SmPlayMessage         ( char          *message_name,
			    char          *language,
			    SM_MSG        *reply );

int SmPlayUtterance       ( long           uttno,
			    long           begtime,
			    long           endtime,
                            SM_MSG        *reply );

int SmPlayWords           ( short          ntags,
			    long           tags[],
			    long           options,
                            SM_MSG        *reply );

int SmQuery               ( short          item,
			    SM_MSG        *reply );

int SmQueryAddedWords     ( char          *vocab,
			    SM_MSG        *reply );

int SmQueryAlternates     ( short          ntags,
			    long           tags[],
			    SM_MSG        *reply);


int SmQueryDefault        ( long           item,
			    SM_MSG        *reply );

int SmQueryEnabledVocabs  ( SM_MSG        *reply );

int SmQueryEnrollIds      ( char          *user_id,
			    char          *enroll_id,
			    char          *language,
                            SM_MSG        *reply );


int SmQueryLanguages      ( SM_MSG        *reply );

int SmQueryPronunciation  ( char          *spelling,
			    SM_MSG        *reply );

int SmQueryPronunciations ( SM_MSG        *reply );

int SmQuerySessions       ( SM_MSG        *reply );

int SmQuerySpeechData     ( SM_MSG        *reply );

int SmQueryTasks          ( char          *language,
			    SM_MSG        *reply );

int SmQueryUserInfo       ( char          *user_id,
			    char          *enroll_id,
			    char          *itemname,
                            SM_MSG        *reply );

int SmQueryUsers          ( SM_MSG        *reply );

int SmQueryVocabs         ( SM_MSG        *reply );

int SmQueryWord           ( SM_WORD       *word,
			    SM_MSG        *reply );

int SmReceiveMsg          ( unsigned       long ap_val,
			    SM_MSG        *reply );

int SmRecognizeNextWord   ( SM_MSG        *reply );

int SmReleaseFocus        ( SM_MSG        *reply );

int SmRemoveFromVocab     ( char          *vocab,
			    short          nvocwords,
                            SM_VOCWORD    *vocwords[],
			    SM_MSG        *reply );

int SmRemovePronunciation ( char          *spelling,
			    char          *pronunciation,
			    SM_MSG        *reply );

int SmRequestFocus        ( SM_MSG        *reply );

int SmRequestMicOff       ( SM_MSG        *reply );

int SmRequestMicOn        ( SM_MSG        *reply );

int SmRestoreSpeechData   ( char          *archive,
			    long           version,
			    SM_MSG        *reply );

int SmSaveSpeechData      ( char          *archive,
			    long           version,
			    long           flags,
			    SM_MSG        *reply );

int SmSet                 ( short          item,
			    long           value,
			    SM_MSG        *reply );

int SmSetDefault          ( long           item,
			    char          *item_value,
			    SM_MSG        *reply );

int SmSetUserInfo         ( char          *user_id,
			    char          *enroll_id,
                            char          *itemname,
			    char          *itemvalue,
			    SM_MSG        *reply );

int SmUndefineVocab       ( char          *vocab,
			    SM_MSG        *reply );

int SmWordCorrection      ( short          ntags,
			    long           tags[],
			    short          nspells,
                            char          *spellings[],
			    char          *pronunciations[],
                            short          new_pronunciation,
			    SM_MSG        *reply );

/*------------------------------------------------------------------------*/
/* Parallel sessions.                                                     */
/*------------------------------------------------------------------------*/
int SmSesAddPronunciation    ( SmSesId        session_id,
                               char          *spelling,
			       char          *pronunciation,
			       long           uttno,
                               short          repetitions,
			       long           options,
			       SM_MSG        *reply );

int SmSesAddToVocab          ( SmSesId        session_id,
                               char          *vocab,
			       short          nvocwords,
                               SM_VOCWORD    *vocwords[],
			       SM_MSG        *reply );

int SmSesCancelPlayback      ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesClose               ( SmSesId        session_id );

int SmSesConnect             ( SmSesId        session_id,
			       int            nargs,
			       SmArg         *Args,
			       SM_MSG        *reply );

int SmSesDefineGrammar       ( SmSesId        session_id,
			       char          *vocab,
			       char          *grammar,
			       long           options,
			       SM_MSG        *reply );

int SmSesCorrectText         ( SmSesId        session_id,
                               short          nwords,
			       SM_WORD       *words[],
			       SM_MSG        *reply );

int SmSesCorrectTextCancel   ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesDefineVocab         ( SmSesId        session_id,
                               char          *vocab,
			       short          nvocwords,
                               SM_VOCWORD    *vocwords[],
			       SM_MSG        *reply );

int SmSesDisableVocab        ( SmSesId        session_id,
                               char          *vocab,
			       SM_MSG        *reply );

int SmSesDiscardData         ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesDisconnect          ( SmSesId        session_id,
                               int            nargs,
			       SmArg         *Args,
			       SM_MSG        *reply );

int SmSesDispatch            ( SmSesId        session_id,
			       unsigned long  ap_val );

int SmSesDetachSessions      ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesEnableVocab         ( SmSesId        session_id,
			       char          *vocab,
			       SM_MSG        *reply );

int SmSesEventNotify         ( SmSesId        session_id,
			       long           event_id,
			       long           options,
			       SM_MSG        *reply );

int SmSesHaltRecognizer      ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesMicOff              ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesMicOn               ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesNewContext          ( SmSesId        session_id,
			       short          nwords,
			       SM_WORD       *words[],
			       SM_MSG        *reply );

int SmSesOpen                ( SmSesId       *session_id,
			       int            nargs,
			       SmArg         *Args );

int SmSesPlayMessage         ( SmSesId        session_id,
			       char          *message_name,
			       char          *language,
			       SM_MSG        *reply );

int SmSesPlayUtterance       ( SmSesId        session_id,
			       long           uttno,
			       long           begtime,
			       long           endtime,
                               SM_MSG        *reply );

int SmSesPlayWords           ( SmSesId        session_id,
			       short          ntags,
			       long           tags[],
			       long           options,
                               SM_MSG        *reply );

int SmSesQuery               ( SmSesId        session_id,
			       short          item,
			       SM_MSG        *reply );

int SmSesQueryAddedWords     ( SmSesId        session_id,
			       char          *vocab,
			       SM_MSG        *reply );

int SmSesQueryAlternates     ( SmSesId        session_id,
			       short          ntags,
			       long           tags[],
			       SM_MSG        *reply);

int SmSesQueryDefault        ( SmSesId        session_id,
			       long           item,
			       SM_MSG        *reply );

int SmSesQueryEnabledVocabs  ( SmSesId       session_id,
			       SM_MSG        *reply );

int SmSesQueryEnrollIds      ( SmSesId        session_id,
			       char          *user_id,
			       char          *enroll_id,
			       char          *language,
                               SM_MSG        *reply );

int SmSesQueryLanguages      ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesQueryPronunciation  (SmSesId        session_id,
			       char          *spelling,
			       SM_MSG        *reply );

int SmSesQueryPronunciations ( SmSesId        session_id,
			          SM_MSG        *reply );

int SmSesQuerySessions       ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesQuerySpeechData     ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesQueryTasks          ( SmSesId        session_id,
			       char          *language,
			       SM_MSG        *reply );

int SmSesQueryUserInfo       ( SmSesId        session_id,
			       char          *user_id,
			       char          *enroll_id,
			       char          *itemname,
                               SM_MSG        *reply );

int SmSesQueryUsers          ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesQueryVocabs         ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesQueryWord           ( SmSesId        session_id,
			       SM_WORD       *word,
			       SM_MSG        *reply );

int SmSesReceiveMsg          ( SmSesId        session_id,
			       unsigned long  ap_val,
			       SM_MSG        *reply );

int SmSesRecognizeNextWord   ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesReleaseFocus        ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesRemoveFromVocab     ( SmSesId        session_id,
			       char          *vocab,
			       short          nvocwords,
                               SM_VOCWORD    *vocwords[],
			       SM_MSG        *reply );

int SmSesRemovePronunciation ( SmSesId       session_id,
			       char          *spelling,
			       char          *pronunciation,
			       SM_MSG        *reply );

int SmSesRequestFocus        ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesRequestMicOff       ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesRequestMicOn        ( SmSesId        session_id,
			       SM_MSG        *reply );

int SmSesRestoreSpeechData   ( SmSesId        session_id,
			       char          *archive,
			       long           version,
			       SM_MSG        *reply );

int SmSesSaveSpeechData      ( SmSesId        session_id,
			       char          *archive,
			       long           version,
			       long           flags,
			       SM_MSG        *reply );

int SmSesSet                 ( SmSesId        session_id,
			       short          item,
			       long           value,
			       SM_MSG        *reply );

int SmSesSetDefault          ( SmSesId        session_id,
			       long           item,
			       char          *item_value,
			       SM_MSG        *reply );

int SmSesSetUserInfo         ( SmSesId        session_id,
			       char          *user_id,
			       char          *enroll_id,
                               char          *itemname,
			       char          *itemvalue,
			       SM_MSG        *reply );

int SmSesUndefineVocab       ( SmSesId        session_id,
			       char          *vocab,
			       SM_MSG        *reply );

int SmSesWordCorrection      ( SmSesId        session_id,
			       short          ntags,
			       long           tags[],
			       short          nspells,
                               char          *spellings[],
			       char          *pronunciations[],
                               short          new_pronunciation,
			       SM_MSG        *reply );

#ifdef __cplusplus
   }
#endif

#endif /* not SM_API_H_INCLUDED */
