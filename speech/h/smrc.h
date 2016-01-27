/*========================================================================*/
/*                                                                        */
/*  smrc.h                                                                */
/*                                                                        */
/*   (C) COPYRIGHT International Business Machines Corp. 1992,1996        */
/*   All Rights Reserved                                                  */
/*   Licensed Materials - Property of IBM                                 */
/*   US Government Users Restricted Rights - Use, duplication or          */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.    */
/*                                                                        */
/*========================================================================*/

#ifndef SM_RC_H_INCLUDED
#define SM_RC_H_INCLUDED

/*---------------------------------------------------------------------*/
/* send/receive message error codes                                    */
/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
/* SM_API_RC_MAX_NAMES should be set to the maximum absolute value     */
/* used for the send/receive message error codes.                      */
/*---------------------------------------------------------------------*/
#define SM_API_RC_MAX_NAMES           28
#define SM_API_RC_MIN_NAMES          -28

#define SM_RC_EAPIVERSION             -1
#define SM_RC_ENOMSG                  -2
#define SM_RC_ENOCONN                 -3
#define SM_RC_ENOSERVER               -4
#define SM_RC_EINVAL                  -5
#define SM_RC_EUNEXP                  -6
#define SM_RC_ETIMEOUT                -7
#define SM_RC_EUNKMSG                 -8
#define SM_RC_EMSGSIZE                -9
#define SM_RC_ENOHANDLES              -10
#define SM_RC_EBADHANDLE              -11
#define SM_RC_ENOMEM                  -12
#define SM_RC_EALLOC                  -13
#define SM_RC_EBADAPPNAME             -14
#define SM_RC_ENOACCEPT               -15
#define SM_RC_EALLBUSY                -16
#define SM_RC_CREATE_MBOX_FAILED      -17
#define SM_RC_CREATE_EVENT_SEM_FAILED -18
#define SM_RC_ASSOC_EVENT_SEM_FAILED  -19
#define SM_RC_DEALLOCATING_SH_MEM     -20
#define SM_RC_QUEUE_CLOSE_ERROR       -21
#define SM_RC_FREE_MEM_ERROR          -22
#define SM_RC_CLOSE_EVENT_SEM_FAILED  -23
#define SM_RC_SUB_UNSET_ERROR         -24
#define SM_RC_OPEN_QUEUE_FAILED       -25
#define SM_RC_CREATE_MUTEX_SEM_FAILED -26
#define SM_RC_ATTACH_MUTEX_SEM_FAILED -27

/*---------------------------------------------------------------------*/
/* Return codes.                                                       */
/*---------------------------------------------------------------------*/
#define SM_RC_NOT_YET                1000

/*---------------------------------------------------------------------*/
/* SM_RC_MAX_NAMES should be set to the maximum value used for return  */
/* codes.                                                              */
/*---------------------------------------------------------------------*/
#define SM_RC_MAX_NAMES               250

#define SM_RC_OK                        0
#define SM_RC_NOT_VALID_REQUEST         1
#define SM_RC_BAD_MODE                  2
#define SM_RC_NOT_WHILE_MIC_ON          3
#define SM_RC_MIC_ALREADY_ON            4
#define SM_RC_MIC_ALREADY_OFF           5
#define SM_RC_MIC_ON_PENDING            6
#define SM_RC_MIC_OFF_PENDING           7
#define SM_RC_NOT_WHILE_PLAYING         8

#define SM_RC_BAD_AUDIO                10
#define SM_RC_RECORD_OPEN_ERROR        11
#define SM_RC_PLAY_OPEN_ERROR          12
#define SM_RC_AUDIO_IN_USE             13
#define SM_RC_BAD_AUDIO_PROTOCOL       14
#define SM_RC_AUDIO_TIMEOUT            15
#define SM_RC_AUDIO_DISCONNECTED       16
#define SM_RC_AUDIO_OVERRUN            17
#define SM_RC_AUDIO_FORCED_MIC_OFF     18
#define SM_RC_NO_MORE_AUDIO_FILES      19

#define SM_RC_BAD_AP                   20
#define SM_RC_BAD_DECO                 30

#define SM_RC_BAD_ADDWORD              40
#define SM_RC_ADDED                    41
#define SM_RC_NOT_ADDED                42
#define SM_RC_MULTIPLE_SPELLINGS       43
#define SM_RC_ILLEGAL_SPELLING         44
#define SM_RC_ILLEGAL_SOUNDSLIKE       45
#define SM_RC_MISMATCHED_ACOUSTICS     46
#define SM_RC_BAD_ACOUSTICS            47
#define SM_RC_SPELLING_TOO_LONG        48
#define SM_RC_ACOUSTICS_TOO_LONG       49
#define SM_RC_ADDWORD_LIMIT_EXCEEDED   50
#define SM_RC_EXTENDED_VOCAB_ADD       51

#define SM_RC_SERVER_ERROR             60
#define SM_RC_SERVER_MALLOC_ERROR      61
#define SM_RC_SERVER_FILE_OPEN_ERROR   62
#define SM_RC_SERVER_FILE_WRITE_ERROR  63
#define SM_RC_SERVER_FILE_READ_ERROR   64
#define SM_RC_SERVER_FILE_CLOSE_ERROR  65
#define SM_RC_SERVER_PROCESS_ERROR     66
#define SM_RC_SERVER_TERMINATED        67

#define SM_RC_BAD_TAG                  70
#define SM_RC_BAD_UTTNO                71
#define SM_RC_BAD_MESSAGE              72

#define SM_RC_NOT_DELETED              80
#define SM_RC_NOT_INVOCAB              81
#define SM_RC_INVOCAB                  82
#define SM_RC_BAD_VOCAB                83
#define SM_RC_MISSING_EXTERN           84

#define SM_RC_BAD_USERID               90
#define SM_RC_BAD_ENROLLID             91
#define SM_RC_BAD_PASSWORD             92
#define SM_RC_BAD_TASKID               93
#define SM_RC_BAD_CLIENT               94
#define SM_RC_USERID_EXISTS            95
#define SM_RC_ENROLLID_EXISTS          96
#define SM_RC_USERID_BUSY              97
#define SM_RC_ENROLLID_BUSY            98
#define SM_RC_BAD_SCRIPT               99
#define SM_RC_BAD_DESCRIPTION         100
#define SM_RC_ENROLLID_RUNNING        101
#define SM_RC_ENROLLMENT_NOT_COMPLETE 102
#define SM_RC_MISMATCHED_LANGUAGE     103
#define SM_RC_MISMATCHED_ALPHABET     104
#define SM_RC_MISMATCHED_SCRIPT       105
#define SM_RC_BAD_LANGUAGE            106
#define SM_RC_BAD_NAME                107
#define SM_RC_INVALID_WINDOW_HANDLE   108

#define SM_RC_BAD_ITEM                110
#define SM_RC_BAD_VALUE               111

#define SM_RC_BUSY_LAST_UTTERANCE     120
#define SM_RC_BUSY_WORD_CORRECTION    121

#define SM_RC_NO_SPACE                130
#define SM_RC_NO_SPACE_INIT_RECO      131
#define SM_RC_NO_SPACE_INIT_ENROLL    132
#define SM_RC_NO_SPACE_TERM_ENROLL    133
#define SM_RC_NO_SPACE_MIC_ON         134

#define SM_RC_INVALID_PARM_MAX_LEN    140

#define SM_RC_NO_FOCUS_APP            170
#define SM_RC_FOCUS_GRANTED           171
#define SM_RC_FOCUS_REQUEST_PENDING   172
#define SM_RC_FOCUS_DENIED            173
#define SM_RC_NAV_ALREADY_DEFINED     174
#define SM_RC_NOT_IN_NOTIFY           175
#define SM_RC_EXISTS_IN_NOTIFY        176

#define SM_RC_INCOMPATIBLE_ENROLLMENT 178

/*---------------------------------------------------------------------*/
/* Status codes.                                                       */
/*---------------------------------------------------------------------*/
#define SM_STAT_PLAY_START            100
#define SM_STAT_PLAY_STOP             101
#define SM_STAT_BAD_AUDIO             102
#define SM_STAT_BAD_TAG               103
#define SM_STAT_BAD_UTTERANCE         104
#define SM_STAT_BAD_MESSAGE           105

#define SM_STAT_ENROLLMENT_RECORDING  106
#define SM_STAT_ENROLLMENT_RUNNING    107
#define SM_STAT_ENROLLMENT_FAILED     108
#define SM_STAT_ENROLLMENT_COMPLETE   109
#define SM_STAT_ENROLLMENT_BUSY       110

/*---------------------------------------------------------------------*/
/* Speech Manager Error codes                                          */
/*---------------------------------------------------------------------*/
#define SM_RC_SM_NOT_OPEN             200
#define SM_RC_WRONG_SM_VERSION        201
#define SM_RC_SM_NOT_ACTIVE_CLIENT    202
#define SM_RC_SM_EINVAL_MSG_TYPE      203
#define SM_RC_REPLY_NULL              204
#define SM_RC_NO_MORE_CONNECTIONS     205
#define SM_RC_NO_TOPLEVEL_WIDGET      206
#define SM_RC_CONNECTION_CHANGED      207
#define SM_RC_CALLBACK_LIST_CHANGED   208
#define SM_RC_ASCII_ALREADY_SET       209
#define SM_RC_NOTHING_TO_DISPATCH     210
#define SM_RC_MAX_MSG_QUEUES_EXCEEDED 211
#define SM_RC_OPEN_SYNCH_QUEUE_FAILED 214
#define SM_RC_ALREADY_OPENED          215
#define SM_RC_ALREADY_CONNECTED       216

#endif /* not SM_RC_H_INCLUDED */
