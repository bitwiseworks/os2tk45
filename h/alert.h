/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/
/********************************************************************/


/********************************************************************
 *
 *  About this file ...  ALERT.H
 *
 *  This file contains information about the NetAlert APIs.
 *
 *      Function prototypes.
 *
 *      Data structure templates.
 *
 *      Definition of special values.
 *
 *
 *  NOTE:  You must include NETCONS.H before this file, since this
 *         file depends on values defined in NETCONS.H.
 *
 *  NOTE:  ALERT.H includes ALERTMSG.H, which defines the alert message
 *         numbers.
 *
 ********************************************************************/


#ifndef NETALERT_INCLUDED

#define NETALERT_INCLUDED

#ifdef  ANY_32_BIT
#pragma pack(1)
#endif

#include "alertmsg.h"

/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetAlertRaise ( const unsigned char LSFAR * pszEvent,
                  const unsigned char LSFAR * pbBuffer,
                  unsigned short              usBuffer,
                  unsigned long               ulTimeout );

extern API_FUNCTION
  NetAlertStart ( const unsigned char LSFAR * pszEvent,
                  const unsigned char LSFAR * pszRecipient,
                  unsigned short              usMaxData );

extern API_FUNCTION
  NetAlertStop ( const unsigned char LSFAR * pszEvent,
                 const unsigned char LSFAR * pszRecipient );



/****************************************************************
 *                                                              *
 *              Data structure templates                        *
 *                                                              *
 ****************************************************************/


/***   Standard alert structure
 */

struct std_alert
{
        long            alrt_timestamp;
        unsigned char   alrt_eventname[EVLEN+1];
        unsigned char   alrt_pad1;
        unsigned char   alrt_servicename[SNLEN+1];
};

/*
 *      The following macro gives a pointer to the other_info data.
 *      It takes a "struct std_alert *" and returns a "unsigned char *".
 */

#define ALERT_OTHER_INFO(x)    ((unsigned char *)(x)     + sizeof(struct std_alert))
#define ALERT_OTHER_INFO_F(x)  ((unsigned char LSFAR *)(x) + sizeof(struct std_alert))

/*
 *      The following macro gives a pointer to the variable-length data.
 *      It takes a pointer to one of the other-info structs and
 *      returns a "unsigned char *".
 */

#define ALERT_VAR_DATA(p)      ((unsigned char *)(p)     + sizeof(*p))
#define ALERT_VAR_DATA_F(p)    ((unsigned char LSFAR *)(p) + sizeof(*p))


/***   Print alert other-info structure
 */

struct print_other_info {
        short   alrtpr_jobid;       /* Job ID for job */
        short   alrtpr_status;      /* Status word from job info struct */
                                    /* bit 15 == 1 means job deleted */
        long    alrtpr_submitted;   /* When submitted */
        long    alrtpr_size;        /* Bytes in job */
};

/*      Followed by (consecutive ASCIIZ strings) ...
 *
 *              computername
 *              username
 *              queuename
 *              destination
 *              status string
 */

/***   Spool alert info structure
 */

struct spool_alert_info {
        unsigned long   splalrt_jobid;      /* Job ID for job          */
        unsigned long   splalrt_type;       /* spool alert type        */
        unsigned long   splalrt_flags;      /* is currently set to 0   */
};

/*      Followed by (consecutive ASCIIZ strings) ...
 *
 *              DestComputername
 *              ObjectName (username)
 *              Caption
 *              MsgText
 */

/***   Error log alert other-info structure
 */

struct errlog_other_info {
        short   alrter_errcode;
        long    alrter_offset;
};

/***   Admin alert other-info structure
 */

struct admin_other_info {
        short   alrtad_errcode;         /* code for message in net error message file */
        short   alrtad_numstrings;      /* the number of merge strings 0-9 */
};

/*      Followed by merge strings (consecutive ASCIIZ strings, count
 *      is in alrtad_numstrings field).
 */


/***   User alert other-info structure
 */

struct user_other_info {
        short   alrtus_errcode; /* code for message in net error message file */
        short   alrtus_numstrings;       /* the number of merge strings 0-9 */
};

/*      Followed by merge strings (consecutive ASCIIZ strings, count
 *      is in alrtus_numstrings field).
 *
 *      Further followed by (consecutive ASCIIZ strings) ...
 *
 *              username
 *              computername
 */




/***    Time set alert other-info structure.
 *
 *      Provides detailed information about a change in the system clock.
 */

struct timeset_other_info {
        unsigned long   alrtts_old_time;
        unsigned long   alrtts_new_time;
        unsigned long   alrtts_old_msec;
        unsigned long   alrtts_new_msec;
        unsigned char   alrtts_old_hsec;
        unsigned char   alrtts_new_hsec;
        unsigned short  alrtts_old_tz;
        unsigned short  alrtts_new_tz;
};



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes                             *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
  Net32AlertRaise ( const unsigned char    * pszEvent,
                    const unsigned char    * pbBuffer,
                    unsigned long            ulBuffer,
                    unsigned long            ulTimeout );

extern API32_FUNCTION
  Net32AlertStart ( const unsigned char    * pszEvent,
                    const unsigned char    * pszRecipient,
                    unsigned long            ulMaxData );

extern API32_FUNCTION
  Net32AlertStop ( const unsigned char * pszEvent,
                   const unsigned char * pszRecipient );
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants                    *
 *                                                              *
 ****************************************************************/

/*
 *      Names of standard Microsoft-defined alert events.
 */

#define ALERT_PRINT_EVENT       "PRINTING"
#define ALERT_MESSAGE_EVENT     "MESSAGE"
#define ALERT_ERRORLOG_EVENT    "ERRORLOG"
#define ALERT_ADMIN_EVENT       "ADMIN"
#define ALERT_USER_EVENT        "USER"

/* New alert event defined for NetworkSplAlert API */
#define ALERT_NWIPRT_EVENT      "SPLALERT"


/*
 *      Three suggested timeouts (in milliseconds) for NetAlertRaise.
 */

#define ALERT_SHORT_WAIT        100L
#define ALERT_MED_WAIT          1000L
#define ALERT_LONG_WAIT         10000L



#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETALERT_INCLUDED */
