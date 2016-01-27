/*========================================================================*/
/*                                                                        */
/*  smmsg.h                                                               */
/*                                                                        */
/*   (C) COPYRIGHT International Business Machines Corp. 1992,1996        */
/*   All Rights Reserved                                                  */
/*   Licensed Materials - Property of IBM                                 */
/*   US Government Users Restricted Rights - Use, duplication or          */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.    */
/*                                                                        */
/*========================================================================*/

#ifndef SM_MSG_H_INCLUDED
#define SM_MSG_H_INCLUDED

/*------------------------------------------------------------------------*/
/* put a wrapper around these functions so that they can be called from   */
/* either C or C++ applications                                           */
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int SmGetAlphabets        ( SM_MSG sm_reply, unsigned long *nalphabets,
                                                      char ***alphabets  );
int SmGetApplication      ( SM_MSG reply, char **application);
int SmGetApplications     ( SM_MSG sm_reply, unsigned long *nsessions,
                                                  char ***reco_sessions  );
int SmGetAlternates       ( SM_MSG reply, unsigned long  *nwords,
                                                        SM_WORD **words  );
int SmGetAudioLevel       ( SM_MSG sm_reply, short *volume );
int SmGetDescriptions     ( SM_MSG reply, unsigned long *ndescriptions,
                                                   char ***descriptions  );
int SmGetEngineState      ( SM_MSG sm_reply, unsigned long *engine_state );
int SmGetEnrollId         ( SM_MSG reply, char **enroll_ids              );

int SmGetEnrollIds        ( SM_MSG reply, unsigned long *nenroll_ids,
                                                     char ***enroll_ids  );
int SmGetEventId          ( SM_MSG reply, unsigned long  *event_id       );
int SmGetEventOptions     ( SM_MSG reply, unsigned long  *options        );
int SmGetFirmWords        ( SM_MSG reply, unsigned long  *nwords,
                                                        SM_WORD **words  );
int SmGetFocusState       ( SM_MSG sm_reply, unsigned long *focus_state  );
int SmGetFocusChangeReason( SM_MSG sm_reply, unsigned long *focus_change );
int SmGetGrammarPath      ( SM_MSG reply, char **grammar_path            );

int SmGetInfirmWords      ( SM_MSG reply, unsigned long  *nwords,
                                                        SM_WORD **words  );
int SmGetItemValue        ( SM_MSG reply, unsigned long  *item,
                                                   unsigned long *value  );
int SmGetLanguages        ( SM_MSG reply, unsigned long   *nlanguages,
                                                      char ***languages  );
int SmGetMicState         ( SM_MSG sm_reply, unsigned long *mic_state    );
int SmGetMsgName          ( SM_MSG reply, char **message_name            );
int SmGetMsgType          ( SM_MSG sm_reply, int *msg_type               );
int SmGetNameValue        ( SM_MSG reply, char **name, char **value      );
int SmGetNumberWordMsgs   ( SM_MSG reply, unsigned long  *nwords         );
int SmGetPercentages      ( SM_MSG reply, unsigned long  *npercentages,
                                                    short **percentages  );
int SmGetPhraseState      ( SM_MSG reply, unsigned long *phrase_state    );
int SmGetPronunciations   ( SM_MSG reply, unsigned long *npronun,
                                                        char ***pronuns  );
int SmGetRc               ( SM_MSG reply, int *rc                        );
int SmGetRcName           ( SM_MSG sm_reply, char **rc_name              );
int SmGetRcDescription    ( SM_MSG sm_reply, char **rc_description       );
int SmReturnRc            ( SM_MSG sm_reply                              );
char *SmReturnRcName      ( int rc                                       );
char *SmReturnRcDescription ( int rc                                     );
int SmGetScripts          ( SM_MSG reply, unsigned long   *nscripts,
                                                        char ***scripts  );
int SmGetService          ( SM_MSG reply, char **service                 );
int SmGetSessionId        ( SM_MSG reply, char **session_id              );
int SmGetSeverity         ( SM_MSG reply, unsigned long *severity        );

int SmGetSpeechDataArchive ( SM_MSG reply, char         **archive );
int SmGetSpeechDataOptions ( SM_MSG reply, long          *options );
int SmGetSpeechDataSize    ( SM_MSG reply, unsigned long *size    );
int SmGetSpeechDataVersion ( SM_MSG reply, long          *version );
  
int SmGetSpelling         ( SM_MSG reply, char **spelling                );
int SmGetSpellings        ( SM_MSG reply, unsigned long *nspellings,
                                                      char ***spellings  );
int SmGetStates           ( SM_MSG reply, unsigned long  *nstates,
                                                         short **states  );
int SmGetStatus           ( SM_MSG reply, int *status                    );
int SmGetTags             ( SM_MSG reply, unsigned long *ntags,
                                                            long **tags  );
int SmGetTask             ( SM_MSG reply, char **tasks                   );
int SmGetTasks            ( SM_MSG reply, unsigned long *ntasks,
                                                          char ***tasks  );
int SmGetTimes            ( SM_MSG reply, unsigned long *ntimes,
                                                  unsigned long **times  );
int SmGetUserId           ( SM_MSG reply, char **user_ids                );
int SmGetUserIds          ( SM_MSG reply, unsigned long *nuser_ids,
                                                       char ***user_ids  );
int SmGetUsers            ( SM_MSG reply, unsigned long *nusers,
                                                          char ***users  );
int SmGetUtteranceNumber  ( SM_MSG reply, unsigned long  *nutterance     );
int SmGetVocabList        ( SM_MSG reply, unsigned long   *nvocabs,
                                                         char ***vocabs  );
int SmGetVocabName        ( SM_MSG reply, char **vocabname               );
int SmGetVocWords         ( SM_MSG reply, unsigned long  *nwords,
                                                     SM_VOCWORD **words  );
int SmGetWords            ( SM_MSG reply, unsigned long  *nwords,
                                                        SM_WORD **words  );
int SmGetAnnotations      ( SM_MSG reply, unsigned long *nwords,
                                             SM_ANNOTATION **annotations );
/*------------------------------------------------------------------------*/
/* put a wrapper around these functions so that they can be called from   */
/* either C or C++ applications                                           */
/*------------------------------------------------------------------------*/
#ifdef __cplusplus
   }
#endif

#endif
