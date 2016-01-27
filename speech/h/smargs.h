/*========================================================================*/
/*                                                                        */
/*  smargs.h                                                              */
/*                                                                        */
/*   (C) COPYRIGHT International Business Machines Corp. 1992,1996        */
/*   All Rights Reserved                                                  */
/*   Licensed Materials - Property of IBM                                 */
/*   US Government Users Restricted Rights - Use, duplication or          */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.    */
/*                                                                        */
/*========================================================================*/

#ifndef SM_ARGS_H_INCLUDED
#define SM_ARGS_H_INCLUDED

/*-------------------------------------------------------------------------*/
/* Remember: - if you set arguments to the speech manager copies are held  */
/*             in internal structures keeping the state of the speech      */
/*             manager                                                     */
/*           - if you query arguments you get read only values pointing    */
/*             into state structure of the speech manager                  */
/*             Be aware: DO NOT free these pointers                        */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* system dependent speech manager definition for argument lists           */
/*                                                                         */
/* The typedef for SmArgVal has to be chosen such that                     */
/*                                                                         */
/*   sizeof (SmArgVal) >=    sizeof(char *)                                */
/*                           sizeof(SmCallback *)                          */
/*                           sizeof(SmLogger (*) ())                       */
/*                                                                         */
/*-------------------------------------------------------------------------*/
typedef long SmArgVal;

typedef struct {
 char      *name;                                 /* name of the argument  */
 SmArgVal  value;                                 /* value of the argument */
} SmArg;

/*-------------------------------------------------------------------------*/
/* Macro to set the components of a SmArg structure                        */
/*-------------------------------------------------------------------------*/
#define SmSetArg(a, n, d)    ( (a).name = (n), (a).value = (long)(d) )
#define SmSesSetArg(a, n, d) ( (a).name = (n), (a).value = (long)(d) )

/*-------------------------------------------------------------------------*/
/* set the application name                                                */
/*-------------------------------------------------------------------------*/
#define SmNapplicationName             "applicationName"

/*-------------------------------------------------------------------------*/
/* requested services                                                      */
/*-------------------------------------------------------------------------*/
#define SmNresetServices               "resetServices"
#define SmNdatabase                    "database"
#define SmNrecognize                   "recognize"

/*-------------------------------------------------------------------------*/
/* option flags for the disconnection from the SmNrecognize service        */
/*-------------------------------------------------------------------------*/
#define SmNdiscardSessionData        "discardSessionData"
#define SmNdiscardSessionAdaptation  "discardSessionAdaptation"
#define SmNsaveSessionData           "saveSessionData"
#define SmNsaveSessionAdaptation     "saveSessionAdaptation"

/*-------------------------------------------------------------------------*/
/* option flags for the disconnection from the SmNenrollment service       */
/*-------------------------------------------------------------------------*/
#define SmNsuspendEnrollment         "suspendEnrollment"
#define SmNcompleteEnrollment        "completeEnrollment"

/*-------------------------------------------------------------------------*/
/* use the X eventhandler to dispatch incoming replies, the handler is     */
/* accessed over the toplevel widget                                       */
/*-------------------------------------------------------------------------*/
#define SmNuseToplevelWidget           "useToplevelWidget"

/*-------------------------------------------------------------------------*/
/* name constant used by pm applications to set the window id             */
/*-------------------------------------------------------------------------*/
#define SmNwindowHandle                "windowHandle"

/*-------------------------------------------------------------------------*/
/* arguments to set the user                                               */
/*-------------------------------------------------------------------------*/
#define SmNuser                        "user"
#define SmNuserId                      "userId"
#define SmNenrollId                    "enrollId"
#define SmNenrollIdDescription         "enrollIdDescription"
#define SmNenrollDirectory             "enrollDirectory"
#define SmNpassword                    "password"
#define SmNtask                        "task"
#define SmNconnectionId                "connectionId"
#define SmNnavigator                   "navigator"
#define SmNoverrideLock                "overrideLock"

/*-------------------------------------------------------------------------*/
/* Callbacks for connection/disconnection to the Speech Manager            */
/*-------------------------------------------------------------------------*/
#define SmNconnectCallback             "connectCallback"
#define SmNdisconnectCallback          "disconnectCallback"

/*-------------------------------------------------------------------------*/
/* Callbacks for all other replies from the SM API                         */
/*-------------------------------------------------------------------------*/
#define SmNaddPronunciationCallback    "addPronunciationCallback"
#define SmNaddToVocabCallback          "addToVocabCallback"
#define SmNaudioLevelCallback          "audioLevelCallback"

#define SmNcancelPlaybackCallback      "cancelPlaybackCallback"
#define SmNcorrectTextCallback         "correctTextCallback"
#define SmNcorrectTextCancelCallback   "correctTextCancelCallback"

#define SmNdefineVocabCallback         "defineVocabCallback"
#define SmNdisableVocabCallback        "disableVocabCallback"
#define SmNdiscardDataCallback         "discardDataCallback"

#define SmNenableVocabCallback         "enableVocabCallback"
#define SmNeventNotifyCallback         "eventNotifyCallback"
#define SmNeventSynchCallback          "eventSynchCallback"


#define SmNhaltRecognizerCallback      "haltRecognizerCallback"

#define SmNmicOffCallback              "micOffCallback"
#define SmNmicOnCallback               "micOnCallback"

#define SmNrequestMicOffCallback       "requestMicOffCallback"
#define SmNrequestMicOnCallback        "requestMicOnCallback"
#define SmNmicOffRequestedCallback     "micOffRequestedCallback"
#define SmNmicOnRequestedCallback      "micOnRequestedCallback"

#define SmNnewContextCallback          "newContextCallback"

#define SmNplayMessageCallback         "playMessageCallback"
#define SmNplayMessageStatusCallback   "playMessageStatusCallback"
#define SmNplayUtteranceCallback       "playUtteranceCallback"
#define SmNplayUtteranceStatusCallback "playUtteranceStatusCallback"
#define SmNplayWordsCallback           "playWordsCallback"
#define SmNplayWordsStatusCallback     "playWordsStatusCallback"

#define SmNqueryAddedWordsCallback     "queryAddedWordsCallback"
#define SmNqueryAlternatesCallback     "queryAlternatesCallback"
#define SmNqueryCallback               "queryCallback"
#define SmNqueryDefaultCallback        "queryDefaultCallback"
#define SmNqueryEnabledVocabsCallback  "queryEnabledVocabsCallback"
#define SmNqueryEnrollIdsCallback      "queryEnrollIdsCallback"
#define SmNqueryLanguagesCallback      "queryLANGUAGESCallback"
#define SmNqueryPronunciationsCallback "queryPronunciationsCallback"
#define SmNqueryTasksCallback          "queryTasksCallback"
#define SmNqueryUsersCallback          "queryUsersCallback"
#define SmNqueryUserInfoCallback       "queryUserInfoCallback"
#define SmNqueryVocabsCallback         "queryVocabsCallback"
#define SmNqueryWordCallback           "queryWordCallback"

#define SmNremoveFromVocabCallback     "removeFromVocabCallback"
#define SmNrecognizeNextWordCallback   "recognizeNextWordCallback"
#define SmNrecognizedTextCallback      "recognizedTextCallback"
#define SmNrecognizedWordCallback      "recognizedWordCallback"
#define SmNrecognizedPhraseCallback    "recognizedPhraseCallback"
#define SmNremovePronunciationCallback "removePronunciationCallback"

#define SmNsetCallback                 "setCallback"
#define SmNsetDefaultCallback          "setDefaultCallback"
#define SmNsetUserInfoCallback         "setUserInfoCallback"

#define SmNwordCorrectionCallback      "wordCorrectionCallback"

#define SmNundefineVocabCallback       "undefineVocabCallback"
#define SmNutteranceCompletedCallback  "utteranceCompletedCallback"
#define SmNreportEngineErrorCallback   "reportEngineErrorCallback"
#define SmNqueryPronunciationCallback  "queryPronunciationCallback"

#define SmNcommandWordCallback         "commandWordCallback"
#define SmNmicStateCallback            "micStateCallback"
#define SmNfocusStateCallback          "focusStateCallback"
#define SmNengineStateCallback         "engineStateCallback"

#define SmNrequestFocusCallback        "requestFocusCallback"
#define SmNreleaseFocusCallback        "releaseFocusCallback"
#define SmNfocusLostCallback           "focusLostCallback"
#define SmNfocusGrantedCallback        "focusGrantedCallback"

#define SmNdefineGrammarCallback       "defineGrammarCallback"

#define SmNquerySessionsCallback       "querySessionsCallback"
#define SmNdetachSessionsCallback      "detachSessionsCallback"
#define SmNdetachRequestedCallback     "detachRequestedCallback"

#define SmNquerySpeechDataCallback     "querySpeechDataCallback"
#define SmNsaveSpeechDataCallback      "saveSpeechDataCallback"
#define SmNrestoreSpeechDataCallback   "restoreSpeechDataCallback"

#endif  /* SM_ARGS_H_INCLUDED */
