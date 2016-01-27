/**************************************************************************

   Module Name    = UPM.H

   Descriptive Name = User Profile Management Include file

   (C) Copyright IBM Corporation 1988, 1994.  All rights reserved.
   (C) Copyright Microsoft Corporation 1988, 1991.  All rights reserved.
   Licensed Material - Program Property of IBM

   Function = Include File defining Api Logon/Logoff

   Operating System = OS/2

**************************************************************************/


#ifdef  PURE_32
#include <os2def.h>
#pragma pack(1)
#define LSFAR
#define LSPAS
#define LSPTR
#define LSINT  short
#define FARPASCAL
#define ANY_32_BIT

#endif /* PURE_32 */

#ifdef  INCL_32
#pragma pack(1)
#define LSFAR
#define LSPAS
#define LSPTR  _Seg16
#define LSINT  short
#define FARPASCAL  _Far16 _Pascal
#define ANY_32_BIT
#endif /* PURE_32 */

#ifndef ANY_32_BIT
#define LSFAR  far
#define LSPAS  pascal
#define LSPTR
#define LSINT  int
#define FARPASCAL  far pascal
#endif


#define upmelgn  UPMELGN
#define upmelgff UPMELGFF
#define upmelocu UPMELOCU
#define upmelocl UPMELOCL
#define upmeulgn UPMEULGN
#define upmeulgf UPMEULGF
#define upmeusrl UPMEUSRL
#define upmesetp UPMESETP
#define upmegetp UPMEGETP
#define upmglgn  UPMGLGN
#define upmgulgn UPMGULGN
#define upmglgff UPMGLGFF
#define upmgulgf UPMGULGF
#define upmglocu UPMGLOCU
#define upmglocl UPMGLOCL
#define upmgusrl UPMGUSRL

/*
     String lengths
*/
#define UPM_UIDLEN    10

/*
The following password length applies to user logon profile entries only.
*/
#define UPM_PWDLEN    10
#define UPM_REMLEN    17

/************************************************************/
/*                                                          */
/*   Function Prototypes, 16-bit and mixed-model            */
/*                                                          */
/************************************************************/
#ifndef PURE_32

/*
     Log (generic) an application process on the the system
*/
LSINT FARPASCAL UPMGLGN(
            unsigned short,          /*  userid length     */
            unsigned short,          /*  password length   */
            unsigned short,          /*  remotename length */
            unsigned short,          /*  remotetype        */
            unsigned short,          /*  authcheck         */
            unsigned char LSFAR *,   /*  userid            */
            unsigned char LSFAR *,   /*  password          */
            unsigned char LSFAR *);  /*  remotename        */

/*
     Log (generic) a user on to the system
*/
LSINT FARPASCAL UPMGULGN(
            unsigned short,          /*  userid length     */
            unsigned short,          /*  password length   */
            unsigned short,          /*  remotename length */
            unsigned short,          /*  remotetype        */
            unsigned short,          /*  flags             */
            unsigned char LSFAR *,   /*  userid            */
            unsigned char LSFAR *,   /*  password          */
            unsigned char LSFAR *);  /*  remotename        */
/*
     Log (generic) an application process off the system
*/
LSINT FARPASCAL UPMGLGFF(
            unsigned short,          /*  userid length     */
            unsigned short,          /*  remotename length */
            unsigned short,          /*  remotetype        */
            unsigned char LSFAR *,   /*  userid            */
            unsigned char LSFAR *);  /*  remotename        */

/*
     Log (generic) a user off the system
*/
LSINT FARPASCAL UPMGULGF(
            unsigned short,          /*  userid length     */
            unsigned short,          /*  remotename length */
            unsigned short,          /*  remotetype        */
            unsigned char LSFAR *,   /*  userid            */
            unsigned char LSFAR *);  /*  remotename        */

/*
     Get (generic) locally logged on user ID.
*/
LSINT FARPASCAL UPMGLOCU(
            unsigned short *,       /*  userid length     */
            unsigned char LSFAR *,  /*  userid            */
            unsigned short *);      /*  user type         */
/*
     Logon (generic) a local user ID.
*/
LSINT FARPASCAL UPMGLOCL(
            unsigned short *,         /*  userid length     */
            unsigned char LSFAR *,    /*  userid            */
            unsigned short *);        /*  user type         */

/*
     (generic) logon user list
*/
LSINT FARPASCAL UPMGUSRL(
            unsigned short,          /*  remotename length */
            unsigned short,          /*  remotetype        */
            unsigned short,          /*  bufferlen         */
            unsigned short LSFAR *,  /*  entriesread */
            unsigned short LSFAR *,  /*  totalentry  */
            unsigned char LSFAR *,   /*  remotename  */
            unsigned char LSFAR *);  /*  buffer      */

/*
     Log an application process on the the system
*/
LSINT FARPASCAL UPMELGN(
            unsigned char LSFAR *,   /*  userid      */
            unsigned char LSFAR *,   /*  password     */
            unsigned char LSFAR *,   /*  remotename  */
            unsigned short,          /*  remotetype  */
            unsigned short );        /*  authcheck   */

/*
     Log an application process off the system
*/
LSINT FARPASCAL UPMELGFF(
            unsigned char LSFAR *,   /*  userid      */
            unsigned char LSFAR *,   /*  remotename  */
            unsigned short );        /*  remotetype  */
/*
     Get the local user ID logged on the system
*/
LSINT FARPASCAL UPMELOCU(
            unsigned char LSFAR *,   /*  userid      */
            unsigned short *);       /*  type  */
/*
     Logon a local user ID on the system
*/
LSINT FARPASCAL UPMELOCL(
            unsigned char LSFAR *,   /*  userid      */
            unsigned short *);       /*  type  */

/*
     Log a user on the system
*/
LSINT FARPASCAL UPMEULGN(
            unsigned char LSFAR *,   /*  userid      */
            unsigned char LSFAR *,   /*  password    */
            unsigned char LSFAR *,   /*  remotename  */
            unsigned short,          /*  remotetype  */
            unsigned short );        /*  flags       */

/*
     Log a user off the system
*/
LSINT FARPASCAL UPMEULGF(
            unsigned char LSFAR *,   /*  userid      */
            unsigned char LSFAR *,   /*  remotename  */
            unsigned short );        /*  remotetype  */
/*
     Logged on user list
*/
LSINT FARPASCAL UPMEUSRL(
            unsigned char LSFAR *,     /*  remotename  */
            short,                     /*  remotetype  */
            char LSFAR *,              /*  buffer      */
            unsigned short,            /*  bufferlen   */
            unsigned short LSFAR *,    /*  entriesread */
            unsigned short LSFAR * );  /*  totalentry  */

/*
     Set a Users logon profile
*/
LSINT FARPASCAL UPMESETP (
            unsigned char LSFAR *,     /*  userid      */
            char LSFAR *,              /*  buffer      */
            unsigned short,            /*  entries     */
            unsigned short );          /*  level       */

/*
     Get a Users logon profile
*/
LSINT FARPASCAL UPMEGETP (
            unsigned char LSFAR *,     /*  userid      */
            char LSFAR *,              /*  buffer      */
            unsigned short,            /*  bufferlen   */
            unsigned short LSFAR *,    /*  entriesread */
            unsigned short LSFAR *,    /*  totalavail  */
            unsigned short);           /*  level       */


#endif /* Not PURE_32 */


/************************************************************/
/*                                                          */
/*   Data structures, 16-bit, mixed-model, and PURE_32      */
/*                                                          */
/************************************************************/

/*
     User logon structure for UPMGUSRL
*/

struct UPM_G_USER_LOGON
       {
         unsigned short useridlen;
         unsigned short remotelen;
         unsigned short remotetype;
         LSINT          sessionid;
         char           userid[UPM_UIDLEN + 1];
         char           remotename[UPM_REMLEN + 1];
       };

/*
     User logon structure for upmeusrl
*/

struct UPM_USER_LOGON
       {
         char    userid[UPM_UIDLEN + 1];
         short   remotetype;
         char    remotename[UPM_REMLEN + 1];
         LSINT   sessionid;
       };

/*
     User logon profile structure for upmegetp and upmesetp
*/

struct UPM_USER_LOGON_PROF_0 {
          unsigned char userid[ UPM_UIDLEN + 1];
          unsigned char password[ UPM_PWDLEN + 1];
          unsigned char remotename[ UPM_REMLEN + 1];
          unsigned short remotetype;
          };



#ifdef PURE_32

#define u32elgn  U32ELGN
#define u32elgff U32ELGFF
#define u32elocu U32ELOCU
#define u32elocl U32ELOCL
#define u32eulgn U32EULGN
#define u32eulgf U32EULGF
#define u32eusrl U32EUSRL
#define u32esetp U32ESETP
#define u32egetp U32EGETP

/************************************************************/
/*                                                          */
/*   Function Prototypes, PURE_32 only                      */
/*                                                          */
/************************************************************/


/*
     Log an application process onto the system
*/
LSINT APIENTRY U32ELGN(
            unsigned char *,         /*  userid      */
            unsigned char *,         /*  password    */
            unsigned char *,         /*  remotename  */
            unsigned long,           /*  remotetype  */
            unsigned long );         /*  authcheck   */

/*
     Log an application process off the system
*/
LSINT APIENTRY U32ELGFF(
            unsigned char *,         /*  userid      */
            unsigned char *,         /*  remotename  */
            unsigned long );         /*  remotetype  */
/*
     Get the local user ID logged on the system
*/
LSINT APIENTRY U32ELOCU(
            unsigned char *,         /*  userid      */
            unsigned long *);        /*  type  */
/*
     Logon a local user ID on the system
*/
LSINT APIENTRY U32ELOCL(
            unsigned char *,         /*  userid      */
            unsigned long *);        /*  type  */

/*
     Log a user onto the system
*/
LSINT APIENTRY U32EULGN(
            unsigned char *,         /*  userid      */
            unsigned char *,         /*  password    */
            unsigned char *,         /*  remotename  */
            unsigned long,           /*  remotetype  */
            unsigned long );         /*  flags       */

/*
     Log a user off the system
*/
LSINT APIENTRY U32EULGF(
            unsigned char *,         /*  userid      */
            unsigned char *,         /*  remotename  */
            unsigned long );         /*  remotetype  */
/*
     Logged on user list
*/

LSINT APIENTRY U32EUSRL(
            unsigned char *,           /*  remotename  */
            unsigned long,             /*  remotetype  */
            unsigned char *,           /*  buffer      */
            unsigned long,             /*  bufferlen   */
            unsigned long *,           /*  entriesread */
            unsigned long * );         /*  totalentry  */

/*
     Set a Users logon profile
*/

LSINT APIENTRY U32ESETP (
            unsigned char *,           /*  userid      */
            unsigned char *,           /*  buffer      */
            unsigned long,             /*  entries     */
            unsigned long );           /*  level       */

/*
     Get a Users logon profile
*/

LSINT APIENTRY U32EGETP (
            unsigned char *,           /*  userid      */
            unsigned char *,           /*  buffer      */
            unsigned long,             /*  bufferlen   */
            unsigned long *,           /*  entriesread */
            unsigned long *,           /*  totalavail  */
            unsigned long);            /*  level       */

#endif /* PURE_32 */


/************************************************************/
/*                                                          */
/*   More manifest constants                                */
/*                                                          */
/************************************************************/

/*
     Valid values for the remotetype parameter
*/
#define UPM_LOCAL            1
#define UPM_DNODE            2
#define UPM_DOMAIN           3
/*
     Additional remotetype value for upmeulgf
*/
#define UPM_DOMAIN_MAX_FORCE 4

/*
     Verbose upmeulgn (1.3.x style)
*/
#define UPM_DOMAIN_VERBOSE   5

/*
     Additional remotetype values for upmeusrl
*/
#define UPM_LOCAL_HPFS     21
#define UPM_ALL            22

/*
     Lan Server logon flags for UPMEULGN.
     If UPM_FL_LOCVER or UPM_FL_NOVER is specified remotename must null.
*/

#define UPM_FL_LOCVER               0x01   /* Local verification  */
#define UPM_FL_NOVER                0x02   /* No verification     */
#define UPM_FL_DOMVER               0x03   /* Domain verification */

/*
     Valid values for the authcheck parameter
*/
#define UPM_CONFIG         1
#define UPM_ADMIN          2
#define UPM_USER           3

/*
     Valid user type privilege levels
*/
#define UPM_PRIV_USER         1
#define UPM_PRIV_ADMIN        2
#define UPM_PRIV_LOCAL_ADMIN  9
/*
     Return values

     Note:  These values are all negative.  Any positive return
           value signifies an unexpected OS/2 system level error
           occurred.  The positive value is the OS/2 return code.
*/
#define UPM_OK                 0      /* Successful.                         */
#define UPM_LOG_INPROC         0xFB01 /* Another logon is in process.        */
#define UPM_BAD_TYPE           0xFB02 /* Bad remotetype.                     */
#define UPM_NOMEM              0xFB03 /* Cannot allocate required memory.    */
#define UPM_LOG_FILE_NOT_FOUND 0xFB04 /* An execute file could not be found. */
#define UPM_FAIL_SECURITY      0xFB05 /* User not logged, failed security
                                         clearance.                          */
#define UPM_BAD_PARAMETER      0xFB06 /* A parameter passed was invalid.     */
#define UPM_BAD_AUTHCHECK      0xFB07 /* Authcheck was not valid.            */
#define UPM_LOG_CANCEL         0xFB08 /* User has canceled from the logon
                                         panel.                              */
#define UPM_NOT_LOGGED         0xFB10 /* A logon has not occured for this
                                         userid.                             */
#define UPM_LOGGED             0xFB12 /* A domain logon is currently active. */
#define UPM_SYS_ERROR          0xFB13 /* An unexpected system error.         */
#define UPM_OPEN_SESSIONS      0xFB14 /* The domain logoff failed, The domain
                                         logon has active sessions.          */
#define UPM_ULP_LOADED         0xFB15 /* The local logon failed, a local
                                         logon with a user logon profile is
                                         active.                             */
#define UPM_LOGGED_ELSEWHERE   0xFB16 /* The domain logon failed, the userid
                                         is already logged on the domain.    */
#define UPM_PASSWORD_EXP       0xFB17 /* The users password is expired.      */
#define UPM_UNAVAIL            0xFB18 /* The logon failed, The remote node
                                         or server could not be contacted to
                                         process the logon request.          */
#define UPM_ACTIVE             0xFB19 /* The domain logon or logoff failed,
                                         a domain logon, logoff or net
                                         command is in process.              */
#define UPM_SS_PWDEXPWARNING   0xFB20 /* The local logon succeeded. The users
                                         password is expired.                */
#define UPM_SS_BUSY            0xFB21 /* The local logon failed. The Local
                                         security was busy.                     */
#define UPM_SS_DEAD            0xFB22 /* The local logon failed. Local
                                         security has terminated unexpectedly.  */
#define UPM_ERROR_MORE_DATA    0xFB23 /* More data is available, the buffer
                                         provided is not large enough.       */
#define UPM_MAX_ENT_EXCEEDED   0xFB24 /* Update failed, the input buffer
                                         contains more than 16 entries.      */
#define UPM_DUP_ULP_ENTRY      0xFB25 /* Two or more entries with the same
                                         remote name and user ID were
                                         detected.                           */
#define UPM_MAX_ULP_EXCEEDED   0xFB26 /* Database contains maximum number
                                         entries                             */
#define UPM_NODISK             0xFB27 /* Insufficient disk space was available to
                                         process this request   */
#define UPM_PROF_NOT_FOUND     0xFB28 /* Did not find user logon profile for
                                         user ID. */
#define UPM_ERROR_NONVAL_LOGON 0xFB29 /* Non validated Lan Server logons are not
                                         allowed with server service started  */





#ifdef  ANY_32_BIT
#pragma pack()
#endif

