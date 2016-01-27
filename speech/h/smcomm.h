/*========================================================================*/
/*                                                                        */
/*  smcomm.h                                                              */
/*                                                                        */
/*   (C) COPYRIGHT International Business Machines Corp. 1992,1996        */
/*   All Rights Reserved                                                  */
/*   Licensed Materials - Property of IBM                                 */
/*   US Government Users Restricted Rights - Use, duplication or          */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.    */
/*                                                                        */
/*========================================================================*/

#ifndef SM_COMM_H_INCLUDED
#define SM_COMM_H_INCLUDED

/*------------------------------------------------------------------------*/
/* Required include files.                                                */
/*------------------------------------------------------------------------*/
#include <limits.h>			/* For INT_MAX */
     
/*------------------------------------------------------------------------*/
/* User directory                                                         */
/*------------------------------------------------------------------------*/
#define SM_USER_DIRECTORY             "SM_USER_DIRECTORY"

/*------------------------------------------------------------------------*/
/* Item numbers for spch_set() and spch_query()                           */
/*------------------------------------------------------------------------*/
#define SM_SAVE_AUDIO               1

#define SM_AUDIO_INPUT_MODE         2
#define SM_AUDIO_INPUT_GAIN         3 
#define SM_AUDIO_OUTPUT_MODE        4 
#define SM_AUDIO_OUTPUT_GAIN        5 

/*------------------------------------------------------------------------*/
/* Item numbers for spch_set() and spch_query() for communicating with    */
/* PM enables.                                                            */
/*------------------------------------------------------------------------*/
#define SM_PM_ENABLE                6
#define SM_CONNECTION_ID            7

/*------------------------------------------------------------------------*/
/* Item numbers for spch_set() and spch_query()                           */
/*------------------------------------------------------------------------*/
#define SM_AUDIO_DEVICE             8
#define SM_AVAILABLE_AUDIO_DEVICES  9
#define SM_AUDIO_CONFIGURATION     10
#define SM_REJECTION_THRESHOLD     11

/*------------------------------------------------------------------------*/
/* Item numbers for SmSet and SmQuery().  These items numbers are passed  */
/* on SmSet by applications to join or leave membership in a multicast    */
/* group.  The item numbers are passed on SmQuery to test membership in   */
/* a multicast group by an application.                                   */
/*------------------------------------------------------------------------*/
#define SM_NOTIFY_AUDIO_LEVEL      12
#define SM_NOTIFY_COMMAND_WORD     13
#define SM_NOTIFY_MIC_STATE        14
#define SM_NOTIFY_FOCUS_STATE      15
#define SM_NOTIFY_ENGINE_STATE     16

/*------------------------------------------------------------------------*/
/* Item number for SmSet and SmQuery().                                   */
/*------------------------------------------------------------------------*/
#define SM_ENABLE_EXCLUSIVE_VOCABS 30
#define SM_REDUCED_CPU_MODE        31

/*------------------------------------------------------------------------*/
/* "PRIVATE" item numbers for spch_set() and spch_query().                */
/*------------------------------------------------------------------------*/
#define SM_SAVE_ALL                32

/*------------------------------------------------------------------------*/
/* Item numbers to support the mode of immediately firming up dictation   */
/* words.                                                                 */
/*------------------------------------------------------------------------*/
#define SM_IMMEDIATE_FIRMUP_MODE  33

/*------------------------------------------------------------------------*/
/* Command timeout parameters.                                            */
/*------------------------------------------------------------------------*/
#define SM_COMPLETE_COMMAND_TIMEOUT 35
#define SM_PARTIAL_COMMAND_TIMEOUT  36
     
/*------------------------------------------------------------------------*/
/* Speed/accuracy parameter.                                              */
/*------------------------------------------------------------------------*/
#define SM_OPTIMIZE_PERFORMANCE     37
     
/*------------------------------------------------------------------------*/
/* Global server trace flags                                              */
/*------------------------------------------------------------------------*/
#define SM_ENGINE_LOG             100
#define SM_ENGINE_DISPLAY         101                
#define SM_ENGINE_DEBUG           102                 
#define SM_ENGINE_TIMING          103                

/*------------------------------------------------------------------------*/
/* API trace flags                                                        */
/*------------------------------------------------------------------------*/
#define SM_API_LOG                104
#define SM_API_DISPLAY            105
#define SM_API_DEBUG              106
#define SM_API_TIMING             107

/*------------------------------------------------------------------------*/
/* Set values for SM_AUDIO_INPUT                                          */
/*------------------------------------------------------------------------*/
#define SM_AUDIO_INPUT_MIC                 0x00000001
#define SM_AUDIO_INPUT_LINE                0x00000002
#define SM_AUDIO_INPUT_LINE_LEFT           0x00000004
#define SM_AUDIO_INPUT_LINE_RIGHT          0x00000008
#define SM_AUDIO_INPUT_VARIABLE_GAIN       0x00000010
#define SM_AUDIO_OUTPUT_VARIABLE_GAIN      0x00000020
#define SM_AUDIO_OUTPUT_INTERNAL_SPEAKER   0x00000040
#define SM_AUDIO_OUTPUT_LINE_LEFT          0x00000080
#define SM_AUDIO_OUTPUT_LINE_RIGHT         0x00000100
#define SM_AUDIO_OUTPUT_SPEAKER            0x00000200
#define SM_AUDIO_OUTPUT_LINE               0x00000400
#define SM_AUDIO_OUTPUT_HEADPHONES         0x00000800

/*------------------------------------------------------------------------*/
/* Set values for SM_AUDIO_DEVICE                                         */
/*------------------------------------------------------------------------*/
#define SM_DEFAULT_SYSTEM_AUDIO            0x00000001              
#define SM_SYSTEM_AUDIO_0                  0x00000002             
#define SM_SYSTEM_AUDIO_1                  0x00000004              
#define SM_SYSTEM_AUDIO_2                  0x00000008              
#define SM_SYSTEM_AUDIO_3                  0x00000010             
#define SM_SYSTEM_AUDIO_4                  0x00000020             
#define SM_SYSTEM_AUDIO_5                  0x00000040             
#define SM_SYSTEM_AUDIO_6                  0x00000080             
#define SM_SYSTEM_AUDIO_7                  0x00000100             
#define SM_SYSTEM_AUDIO_8                  0x00000200            
#define SM_SYSTEM_AUDIO_9                  0x00000400            
#define SM_SYSTEM_AUDIO_10                 0x00000800            
#define SM_MWAVE_AUDIO                     0x00001000            
#define SM_VOICETYPE_DICTATION_AUDIO       0x00002000            

#define SM_ACPA_AUDIO                      0x00004000            

/*------------------------------------------------------------------------*/
/* Flags for the word correction call (bit position orientated)           */
/*------------------------------------------------------------------------*/
#define SM_FORCE_ADDWORD              1
#define SM_DEFAULT_ADDWORD            2
#define SM_REPLACE_ADDWORD            4
#define SM_NO_ADDWORD                 8
#define SM_CHECK_SPELLING             16

/*------------------------------------------------------------------------*/
/* Flags for the add pronunciation call                                   */
/*------------------------------------------------------------------------*/
#define SM_ADD_PRONUNCIATION_ADD       0
#define SM_ADD_PRONUNCIATION_REPLACE   1
#define SM_ADD_PRONUNCIATION_PHONETIC  2

/*------------------------------------------------------------------------*/
/* Flags for the play words call                                          */
/*------------------------------------------------------------------------*/
#define SM_PLAY_WORDS_SEPARATE         1
#define SM_PLAY_WORDS_CONTIGUOUS       2

/*------------------------------------------------------------------------*/
/* Values to define words typed into the speech window                    */
/*------------------------------------------------------------------------*/
#define SM_UNKNOWN_TAG                 -1
#define SM_UNKNOWN_VOCAB               NULL

/*------------------------------------------------------------------------*/
/* Option flags for event notify/synch                                    */
/*------------------------------------------------------------------------*/
#define SM_EVENT_HALT_RECOGNITION      0x00000001
#define SM_EVENT_CONTINUE_RECOGNITION  0x00000002
#define SM_EVENT_FIRM_UP               0x00000004
#define SM_EVENT_DISCARD               0x00000008

/*------------------------------------------------------------------------*/
/* report engine error severity flags                                     */
/*------------------------------------------------------------------------*/
#define SM_ENGINE_INFO                 0
#define SM_ENGINE_WARNING              1
#define SM_ENGINE_ERROR                2
#define SM_ENGINE_TERMINAL_ERROR       3

/*------------------------------------------------------------------------*/
/* report engine error mic state flags                                    */
/*------------------------------------------------------------------------*/
#define SM_ENGINE_MIC_ON               0
#define SM_ENGINE_MIC_OFF              1

/*------------------------------------------------------------------------*/
/* State flags that are received on SM_NOTIFY_MIC_STATE messages          */
/*------------------------------------------------------------------------*/
#define SM_NOTIFY_MIC_ON               0
#define SM_NOTIFY_MIC_OFF              1
#define SM_NOTIFY_MIC_PAUSED           2

/*------------------------------------------------------------------------*/
/* State flags that are received on SM_NOTIFY_FOCUS_STATE messages        */
/*------------------------------------------------------------------------*/
#define SM_NOTIFY_FOCUS_REQUESTED             0
#define SM_NOTIFY_FOCUS_GRANTED               1
#define SM_NOTIFY_FOCUS_DENIED                2
#define SM_NOTIFY_FOCUS_RELEASED              3

/*------------------------------------------------------------------------*/
/* Focus change flags that are received on SM_NOTIFY_FOCUS_STATE messages */
/* when state is SM_NOTIFY_FOCUS_RELEASED                                 */
/*------------------------------------------------------------------------*/
#define SM_NOTIFY_FOCUS_CHANGE_ON_RELEASE     1
#define SM_NOTIFY_FOCUS_CHANGE_ON_REQUEST     2

/*------------------------------------------------------------------------*/
/* State flags that are received on SM_NOTIFY_ENGINE_STATE messages       */
/*------------------------------------------------------------------------*/
#define SM_NOTIFY_ENGINE_BUSY                 0
#define SM_NOTIFY_ENGINE_IDLE                 1
#define SM_NOTIFY_NORMAL_CPU                  2
#define SM_NOTIFY_REDUCED_CPU                 3
#define SM_NOTIFY_NAVIGATOR_EXCLUSIVE         4
#define SM_NOTIFY_FOCUS_APP_EXCLUSIVE         5 
#define SM_NOTIFY_NONE_EXCLUSIVE              6 
#define SM_NOTIFY_PRONUNCIATIONS_ADDED        7 
#define SM_NOTIFY_PRONUNCIATIONS_DELETED      8 
#define SM_NOTIFY_ENGINE_SETTINGS_CHANGED     9 
#define SM_NOTIFY_APPLICATION_CONNECTED      10 
#define SM_NOTIFY_APPLICATION_DISCONNECTED   11 
#define SM_NOTIFY_RECOGNIZED_SPEECH          12
#define SM_NOTIFY_SPEECH_START               13
#define SM_NOTIFY_SPEECH_STOP                14

/*------------------------------------------------------------------------*/
/* Flag used to indicate that the engine should use default setting for a */
/* particular parameter.  Is set using SmSetArg prior to SmOpen/SmConnect.*/
/* For example: SmSetArg ( smArgs [ smc ], SmNuserId, SM_USE_CURRENT );   */
/*------------------------------------------------------------------------*/
#define SM_USE_CURRENT                ""

/*------------------------------------------------------------------------*/
/* Flags used to set defaults using SmSetDefault                          */
/*------------------------------------------------------------------------*/
#define SM_DEFAULT_USERID              0
#define SM_DEFAULT_ENROLLID            1
#define SM_DEFAULT_TASK                2

/*------------------------------------------------------------------------*/
/* Formatting flags returned in SM_WORD structure			  */
/*------------------------------------------------------------------------*/
#define SM_WORD_CAPITALIZE		0x00000001
#define SM_WORD_JOINPREVIOUS		0x00000002
#define SM_WORD_JOINNEXT		0x00000004
#define SM_WORD_DIGIT			0x00000008

/*------------------------------------------------------------------------*/
/* Flags for define-grammar and recognize-phrase                          */
/*------------------------------------------------------------------------*/
#define SM_PHRASE_ACCEPTED   1
#define SM_PHRASE_REJECTED   2
#define SM_PHRASE_HALTED     4

#define SM_PHRASE_HALT			0x00000004
#define SM_PHRASE_NOHALT		0x00000008

#define SM_PHRASE_ALLOW_SILENCES	0x00000010
#define SM_PHRASE_SHOW_SILENCES		0x00000020
#define SM_PHRASE_NO_SILENCES		0x00000040
#define SM_PHRASE_SILENCES		0x00000070

#define SM_PHRASE_ALLOW_INSERTIONS	0x00000100
#define SM_PHRASE_SHOW_INSERTIONS	0x00000200
#define SM_PHRASE_NO_INSERTIONS		0x00000400
#define SM_PHRASE_INSERTIONS		0x00000700

#define SM_PHRASE_NORMALIZE		0x00000800
#define SM_PHRASE_DONT_NORMALIZE	0x00001000

#define SM_PHRASE_LITERAL		0x80000000
     
/*------------------------------------------------------------------------*/
/* Flags for optimize-performance                                         */
/*------------------------------------------------------------------------*/
#define SM_OPTIMIZE_DEFAULT     1
#define SM_OPTIMIZE_ACCURACY    2
#define SM_OPTIMIZE_SPEED       4

/*------------------------------------------------------------------------*/
/* Session save/restore flags                                             */
/*------------------------------------------------------------------------*/
#define SM_NORMAL_FILE       		0x00000000
#define SM_COMPOUND_FILE                0x00000001

/*------------------------------------------------------------------------*/
/* Ignore lock flag                                                       */
/*------------------------------------------------------------------------*/
#define SM_IGNORE_LOCK                 -1

/*------------------------------------------------------------------------*/
/* Grammar annotation flags                                               */
/*------------------------------------------------------------------------*/
#define SM_ANNOTATION_NONE    1
#define SM_ANNOTATION_NUMERIC 2
#define SM_ANNOTATION_STRING  4
#define SM_ANNOTATION_OTHER   8
     
/*------------------------------------------------------------------------*/
/* All possible speech messages to/from applications                      */
/*------------------------------------------------------------------------*/
  /* Router messages */
#define SM_ROUTER                       0
#define SM_SERVER                       1
#define SM_CLIENT                       2
  /* Application to recognizer */
#define SM_SET                          3
#define SM_QUERY                        4
#define SM_QUERY_LANGUAGES              5
#define SM_QUERY_USERS                  6
#define SM_QUERY_ENROLLIDS              7
#define SM_QUERY_TASKS                  8
#define SM_INIT_RECOGNIZER              9
#define SM_SPARE_PUBLIC_1              10
#define SM_MIC_ON                      11
#define SM_MIC_OFF                     12
#define SM_RECOGNIZE_NEXT_WORD         13
#define SM_PLAY_WORDS                  14
#define SM_PLAY_UTTERANCE              15
#define SM_PLAY_MESSAGE                16
#define SM_CANCEL_PLAYBACK             17
#define SM_NEW_CONTEXT                 18
#define SM_EVENT_NOTIFY                19
#define SM_QUERY_ALTERNATES            20
#define SM_WORD_CORRECTION             21
#define SM_CORRECT_TEXT                22
#define SM_CORRECT_TEXT_CANCEL         23
#define SM_QUERY_WORD                  24
#define SM_ADD_PRONUNCIATION           25
#define SM_REMOVE_PRONUNCIATION        26
#define SM_QUERY_PRONUNCIATIONS        27
#define SM_DEFINE_VOCAB                28
#define SM_ADD_TO_VOCAB                29
#define SM_REMOVE_FROM_VOCAB           30
#define SM_QUERY_ADDED_WORDS           31
#define SM_ENABLE_VOCAB                32
#define SM_DISABLE_VOCAB               33
#define SM_UNDEFINE_VOCAB              34
#define SM_QUERY_VOCABS                35
#define SM_QUERY_ENABLED_VOCABS        36
#define SM_SET_USER_INFO               37
#define SM_QUERY_USER_INFO             38
#define SM_DISCARD_DATA                39
#define SM_HALT_RECOGNIZER             40
#define SM_TERMINATE_RECOGNIZER        41
#define SM_INIT_ENROLLMENT             42
#define SM_TERMINATE_ENROLLMENT        43
#define SM_INIT_DATABASE               44
#define SM_TERMINATE_DATABASE          45
#define SM_QUERY_PRONUNCIATION         46
#define SM_REQUEST_FOCUS               47
#define SM_RELEASE_FOCUS               48
#define SM_QUERY_SESSIONS              49
#define SM_DETACH_SESSIONS             50
#define SM_SET_DEFAULT                 51
#define SM_QUERY_DEFAULT               52
  /* Recognizer to application */
#define SM_SET_REPLY                   53
#define SM_QUERY_REPLY                 54
#define SM_QUERY_LANGUAGES_REPLY       55
#define SM_QUERY_USERS_REPLY           56
#define SM_QUERY_ENROLLIDS_REPLY       57
#define SM_QUERY_TASKS_REPLY           58
#define SM_INIT_RECOGNIZER_REPLY       59
#define SM_SPARE_PUBLIC_1_REPLY        60
#define SM_MIC_ON_REPLY                61
#define SM_MIC_OFF_REPLY               62
#define SM_REQUEST_MIC_ON_REPLY        63
#define SM_REQUEST_MIC_OFF_REPLY       64
#define SM_RECOGNIZE_NEXT_WORD_REPLY   65
#define SM_PLAY_WORDS_REPLY            66
#define SM_PLAY_UTTERANCE_REPLY        67
#define SM_PLAY_MESSAGE_REPLY          68
#define SM_CANCEL_PLAYBACK_REPLY       69
#define SM_NEW_CONTEXT_REPLY           70
#define SM_EVENT_NOTIFY_REPLY          71
#define SM_QUERY_ALTERNATES_REPLY      72
#define SM_WORD_CORRECTION_REPLY       73
#define SM_CORRECT_TEXT_REPLY          74
#define SM_CORRECT_TEXT_CANCEL_REPLY   75
#define SM_QUERY_WORD_REPLY            76
#define SM_ADD_PRONUNCIATION_REPLY     77
#define SM_REMOVE_PRONUNCIATION_REPLY  78
#define SM_QUERY_PRONUNCIATIONS_REPLY  79
#define SM_DEFINE_VOCAB_REPLY          80
#define SM_ADD_TO_VOCAB_REPLY          81
#define SM_REMOVE_FROM_VOCAB_REPLY     82
#define SM_QUERY_ADDED_WORDS_REPLY     83
#define SM_ENABLE_VOCAB_REPLY          84
#define SM_DISABLE_VOCAB_REPLY         85
#define SM_UNDEFINE_VOCAB_REPLY        86
#define SM_QUERY_VOCABS_REPLY          87
#define SM_QUERY_ENABLED_VOCABS_REPLY  88
#define SM_SET_USER_INFO_REPLY         89
#define SM_QUERY_USER_INFO_REPLY       90
#define SM_DISCARD_DATA_REPLY          91
#define SM_HALT_RECOGNIZER_REPLY       92
#define SM_TERMINATE_RECOGNIZER_REPLY  93
#define SM_INIT_ENROLLMENT_REPLY       94
#define SM_TERMINATE_ENROLLMENT_REPLY  95
#define SM_INIT_DATABASE_REPLY         96
#define SM_TERMINATE_DATABASE_REPLY    97
#define SM_QUERY_PRONUNCIATION_REPLY   98
#define SM_REQUEST_FOCUS_REPLY         99
#define SM_RELEASE_FOCUS_REPLY        100
#define SM_QUERY_SESSIONS_REPLY       101
#define SM_DETACH_SESSIONS_REPLY      102
#define SM_SET_DEFAULT_REPLY          103
#define SM_QUERY_DEFAULT_REPLY        104
  /* Unsolicited from recognizer to application */
#define SM_RECOGNIZED_TEXT            105
#define SM_RECOGNIZED_WORD            106
#define SM_UTTERANCE_COMPLETED        107
#define SM_PLAY_WORDS_STATUS          108
#define SM_PLAY_UTTERANCE_STATUS      109
#define SM_PLAY_MESSAGE_STATUS        110
#define SM_SERVER_STATUS              111
#define SM_EVENT_SYNCH                112
#define SM_REPORT_ENGINE_ERROR        113
#define SM_FOCUS_LOST                 114
#define SM_FOCUS_GRANTED              115
#define SM_REQUEST_DETACH             116
  /* Unsolicited multicast group messages */
#define SM_AUDIO_LEVEL                117
#define SM_COMMAND_WORD               118
#define SM_MIC_STATE                  119
#define SM_FOCUS_STATE                120
#define SM_ENGINE_STATE               121
  /* messages sent from application to engine and from engine to application */
#define SM_REQUEST_MIC_ON             122
#define SM_REQUEST_MIC_OFF            123
  /* Messages mapped from init mode to connect */
#define SM_CONNECT_REPLY              124
#define SM_DISCONNECT_REPLY           125
  /* Sent from client to engine (for queues) to detach client */
#define SM_CLIENT_DETACH              126
  /* Messages made public after 3.1 */
#define SM_DEFINE_GRAMMAR             143
#define SM_DEFINE_GRAMMAR_REPLY       150
  /* Messages added after 3.1 for converged engine... added here to keep */
  /* backward API compatibility */
#define SM_QUERY_SPEECH_DATA          169
#define SM_QUERY_SPEECH_DATA_REPLY    170
#define SM_SAVE_SPEECH_DATA           171
#define SM_SAVE_SPEECH_DATA_REPLY     172
#define SM_RESTORE_SPEECH_DATA        173
#define SM_RESTORE_SPEECH_DATA_REPLY  174
#define SM_RECOGNIZED_PHRASE          175

/*------------------------------------------------------------------------*/
/* public structures for the speech manager which have to be in sync      */
/* with the corresponding server structures                               */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/* Structure used for all information pertinent to a word                 */
/*------------------------------------------------------------------------*/
struct _SM_WORD {
  long   tag;
  long   flags;
  short  score;
  short  spelling_size;
  char  *spelling;
  short  vocab_size;
  char  *vocab;
};
typedef struct _SM_WORD SM_WORD;             /* word message from decoder */

/*------------------------------------------------------------------------*/
/* Structure used for all information pertinent to a vocabulary word      */
/*------------------------------------------------------------------------*/
struct _SM_VOCWORD {
  long   flags;
  short  spelling_size;
  char  *spelling;
};
typedef struct _SM_VOCWORD SM_VOCWORD;                 /* vocabulary item */

/*------------------------------------------------------------------------*/
/* Structure used for grammar annotations                                 */
/*------------------------------------------------------------------------*/
struct _SM_ANNOTATION {
   long    type;
   union 
   {
      long    numeric;
      char   *string;
      void   *other;
   } annodata; 
};
 
typedef struct _SM_ANNOTATION SM_ANNOTATION;
 

/*------------------------------------------------------------------------*/
/* Structure used for all messages between client and server              */
/* Be aware: the actual layout is not exposed                             */
/*------------------------------------------------------------------------*/
typedef void *SM_MSG;

#endif /* not SM_COMM_H_INCLUDED */
