/*========================================================================*/
/*                                                                        */
/*  smlimits.h                                                            */
/*                                                                        */
/*   (C) COPYRIGHT International Business Machines Corp. 1992,1996        */
/*   All Rights Reserved                                                  */
/*   Licensed Materials - Property of IBM                                 */
/*   US Government Users Restricted Rights - Use, duplication or          */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.    */
/*                                                                        */
/*========================================================================*/

/*------------------------------------------------------------------------*/
/* Limits for certain data                                                */
/*------------------------------------------------------------------------*/
#define SM_MAX_USERID_LEN                       8
#define SM_MAX_ENROLLID_LEN                     8
#define SM_MAX_TASKNAME_LEN                     8
#define SM_MAX_SCRIPTNAME_LEN                   8
/*------------------------------------------------------------------------*
 * SM_MAX_DEFAULTS should be set to maximum of the above 4 values.        *
 *------------------------------------------------------------------------*/
#define SM_MAX_DEFAULTS                         8

#define SM_MAX_SESSIONS                         8

#define SM_MAX_DESCRIPTION_LEN                 80
#define SM_MAX_USERNAME_LEN                    80

#define SM_MAX_PASSWORD_LEN                     8
#define SM_MAX_LANGUAGE_LEN                     8
#define SM_MAX_AUDIOSOURCE_LEN                 80
#define SM_MAX_SPELLING_LEN                    80
#define SM_MAX_PRONUNCIATION_LEN               80
#define SM_MAX_VOCABNAME_LEN                   80
#define SM_MAX_TEMPLATE_NAME_LEN              500
#define SM_MAX_DIRECTORY_LEN                  128

#define SM_MAX_APPLICATION_NAME               100

#define SM_MAX_ALTERNATES                      16
#define SM_MAX_VOCABS                         100
#define SM_MAX_VOCWORDS                      2000
#define SM_MAX_WORDS                         1000
#define SM_MAX_PLAYWORDS                      100
#define SM_MAX_WORD_CORRECTION_SPELLINGS        1
#define SM_MAX_WORD_CORRECTION_TAGS            10
#define SM_LAST_NEW_CONTEXT_WORDS              10
#define SM_MAX_ADDED_PRONUNCIATIONS             5
#define SM_MAX_PHRASE_WORDS                    25

#define SM_MIN_REJECTION_THRESHOLD              0
#define SM_MAX_REJECTION_THRESHOLD             10

#define SM_MIN_AUDIO_LEVEL                      0
#define SM_MAX_AUDIO_LEVEL                     10
#define SM_MIN_GREEN_AUDIO_LEVEL                2
#define SM_MAX_GREEN_AUDIO_LEVEL                7

#define SM_MIN_INPUT_GAIN                       0
#define SM_MAX_INPUT_GAIN                     100
#define SM_MIN_OUTPUT_GAIN                      0
#define SM_MAX_OUTPUT_GAIN                    100

