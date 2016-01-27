/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/
/* The source code for this program is not published or otherwise divested of  */
/* its tradesecrets, irrespective of what has been deposited with the U.S.     */
/* Copyright Office.                                                           */
/*                                                                             */
/*******************************************************************************/
/*
 *  Change History:
 *
 *        DCR 797  TRK 07/01/92 Add PARMNUM_PASSWD_EXPIRED
 *
 *     Post ISO/CMVC Revision history: (f = feature d = defect)                *
 *  Flag Change Fixer Date    Tester Date    Review Date    Release  Describe  *
 *  ---- ------ ----- ------- ------ ------- ------ ------- -------  --------  *
 *  @f00 F02633 TRK   93Oct21 TRK    93Nov05 SAW    93Nov11  LS 4.0
 *   Add user_info_3 and group_info_2 data structures and sundry constants
 *  @f01 F02359 MBB   93Nov10
 *   Add Net32 entry points, internal-only structs; change all char to UCHAR
 *  @d00 D04871 TRK   93Dec02                                LS 4.0
 *   Remove feature 2633 from ls.r40
 *       F01659 SAW   94Feb04                                LS 4.0
 *   Added NetAccessApply
 *  @d02 D06991 MBB   94Apr09                                LS 4.0
 *   Declare all 32-bit functions as API32_FUNCTION
 *  @d03 D07721 MBB   94May10 MBB    94May11                 LS 4.0
 *   Add Net32AccessApply
 *   Probable Cause: Thought that all new code was to be 32-bit; it wasn't.
 *  @d04 D6702  MEB   94May23 MEB    94May23                 LS 4.0
 *   Add a prototype for NetUserDel100
 *  @d05 D04701 sl    94Jun02                                LS 4.0
 *   Add UF_DELETE_PROHIBITED to legal bit list.
 *  @f06 F09512 MBB   94Jul26                                LS 4.0
 *   Add a reserved parameter to each Net32Alias/App/Group/User API
 *  @f07 F05438  MBB  94Aug08                                LS 4.0
 *   Make LS 4.0 header files compatible with C++
 *  @d08 d18491 sl    95sep19                                4.0/5.0/Peer
 *   Move NetUserDel100 to internal-only area.
 *  @d09 d25125 sl    95sep28                                4.0/5.0/Peer
 *   Four parmnums (lastlogon/lastlogoff/badpwcount/numlogons) are internal-only.
 *  @d10 40498 LC     96Jun25 LC                             LS 4.0
 *   Added changes for new I_NetAccessEnum API
 *  @d11 d47833 SLD   98May27 SLD    98May27                 LS 52
 *   Put #ifndef NEOS around @d10
 *  @d12 d47910 SLD   98Jun01 SLD    98Jun01                 LS 52
 *   Undo defect 47883
 *  @f13 f49490 LC    98Sep11 LC     98Sep15                 LS 53
 *   Add structure for JFS I_NetAccessEnum resume info
 *  @f14 f55952 LC    99Jun14 LC     99Jun17  sld   99Jul22  LS 53
 *   Add internal user_info_200 and group_info_100 structures.
 */
/**END_INTERNAL**/

/********************************************************************/
/**                        IBM LAN Server                          **/
/** Copyright (c) International Business Machines Corp., 1987-1993 **/
/** Copyright (c) Microsoft Corp., 1987-1990                       **/
/********************************************************************/

/********************************************************************
 *                                                                  *
 *  About this file ...  ACCESS.H                                   *
 *                                                                  *
 *  This file contains information about the NetUser, NetGroup, and *
 *  NetAccess APIs.  There is a section for each set of APIs.  Each *
 *  section contains:                                               *
 *                                                                  *
 *      Function prototypes.                                        *
 *                                                                  *
 *      Data structure templates.                                   *
 *                                                                  *
 *      Definition of special values.                               *
 *                                                                  *
 *                                                                  *
 *  NOTE:  You must include NETCONS.H before this file, since this  *
 *         file depends on values defined in NETCONS.H.             *
 *                                                                  *
 *         This file is always included by LAN.H                    *
 *                                                                  *
 ********************************************************************/





/****************************************************************
 *                                                              *
 *              User Class                                      *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETUSER ) || !defined( LAN_INCLUDED )) \
    && !defined( NETUSER_INCLUDED )

#define NETUSER_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *              Function prototypes - USER                      *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetUserAdd ( const unsigned char LSFAR * pszServer,
               short                       sLevel,
               unsigned char LSFAR *       pbBuffer,
               unsigned short              cbBuffer );

extern API_FUNCTION
  NetUserDel ( const unsigned char LSFAR * pszServer,
               unsigned char LSFAR *       pszUserName );

extern API_FUNCTION
  NetUserEnum ( const unsigned char LSFAR *     pszServer,
                short                           sLevel,
                unsigned char LSFAR *           pbBuffer,
                unsigned short                  cbBuffer,
                unsigned short LSFAR *          pcEntriesRead,
                unsigned short LSFAR *          pcTotalAvail );

extern API_FUNCTION
  NetUserGetInfo ( const unsigned char LSFAR *     pszServer,
                   unsigned char LSFAR *           pszUserName,
                   short                           sLevel,
                   unsigned char LSFAR *           pbBuffer,
                   unsigned short                  cbBuffer,
                   unsigned short LSFAR *          pcbTotalAvail );

extern API_FUNCTION
  NetUserSetInfo ( const unsigned char LSFAR * pszServer,
                   unsigned char LSFAR *       pszUserName,
                   short                       sLevel,
                   unsigned char LSFAR *       pbBuffer,
                   unsigned short              cbBuffer,
                   short                       sParmNum );

extern API_FUNCTION
  NetUserPasswordSet ( const unsigned char LSFAR * pszServer,
                       unsigned char LSFAR *       pszUserName,
                       unsigned char LSFAR *       pszOldPassword,
                       unsigned char LSFAR *       pszNewPassword );

extern API_FUNCTION
  NetUserGetGroups ( const unsigned char LSFAR *     pszServer,
                     const unsigned char LSFAR *     pszUserName,
                     short                           sLevel,
                     unsigned char LSFAR *           pbBuffer,
                     unsigned short                  cbBuffer,
                     unsigned short LSFAR *          pcEntriesRead,
                     unsigned short LSFAR *          pcTotalAvail );

extern API_FUNCTION
  NetUserSetGroups ( const unsigned char LSFAR * pszServer,
                     const unsigned char LSFAR * pszUserName,
                     short                       sLevel,
                     unsigned char LSFAR *       pbBuffer,
                     unsigned short              cbBuffer,
                     unsigned short              cEntries );

extern API_FUNCTION
  NetUserModalsGet ( const unsigned char LSFAR *     pszServer,
                     short                           sLevel,
                     unsigned char LSFAR *           pbBuffer,
                     unsigned short                  cbBuffer,
                     unsigned short LSFAR *          pcbTotalAvail );

extern API_FUNCTION
  NetUserModalsSet ( const unsigned char LSFAR * pszServer,
                     short                       sLevel,
                     unsigned char LSFAR *       pbBuffer,
                     unsigned short              cbBuffer,
                     short                       sParmNum );

extern API_FUNCTION
  NetUserValidate ( unsigned char LSFAR *  pszReserved,
                    unsigned char LSFAR *  pszUserName,
                    unsigned char LSFAR *  pszPassword,
                    unsigned short LSFAR * pusPrivilege );

extern API_FUNCTION
  NetUserValidate2 ( unsigned char LSFAR *  pszServer,
                     short                  sLevel,
                     unsigned char LSFAR *  pbBuffer,
                     unsigned short         cbBuffer,
                     unsigned short         usReserved2,
                     unsigned short LSFAR * pcbTotalAvail );



/****************************************************************
 *                                                              *
 *              Data structure templates - USER                 *
 *                                                              *
 ****************************************************************/

struct user_info_0 {
        unsigned char      usri0_name[UNLEN+1];
};      /* user_info_0 */

struct user_info_1 {
        unsigned char      usri1_name[UNLEN+1];
        unsigned char      usri1_pad_1;
        unsigned char               usri1_password[ENCRYPTED_PWLEN];/* See note below */
        long                        usri1_password_age;
        unsigned short              usri1_priv;                     /* See values below */
        unsigned char LSFAR * LSPTR usri1_home_dir;
        unsigned char LSFAR * LSPTR usri1_comment;
        unsigned short              usri1_flags;                    /* See values below */
        unsigned char LSFAR * LSPTR usri1_script_path;
};      /* user_info_1 */

/*
 *      NOTE:  The maximum length of a user password is PWLEN.  The
 *      field usri1_password contains extra room for transporting
 *      the encrypted form of the password over the network.  When
 *      setting the user's password, check length vs. PWLEN, not
 *      the size of this field.  PWLEN is defined in NETCONS.H.
 */



struct user_info_2 {
        unsigned char               usri2_name[UNLEN+1];
        unsigned char               usri2_pad_1;
        unsigned char               usri2_password[ENCRYPTED_PWLEN];
        long                        usri2_password_age;
        unsigned short              usri2_priv;
        unsigned char LSFAR * LSPTR usri2_home_dir;
        unsigned char LSFAR * LSPTR usri2_comment;
        unsigned short              usri2_flags;
        unsigned char LSFAR * LSPTR usri2_script_path;
        unsigned long               usri2_auth_flags;
        unsigned char LSFAR * LSPTR usri2_full_name;
        unsigned char LSFAR * LSPTR usri2_usr_comment;
        unsigned char LSFAR * LSPTR usri2_parms;
        unsigned char LSFAR * LSPTR usri2_workstations;
        long                        usri2_last_logon;
        long                        usri2_last_logoff;
        long                        usri2_acct_expires;
        unsigned long               usri2_max_storage;
        unsigned short              usri2_units_per_week;
        unsigned char LSFAR * LSPTR usri2_logon_hours;
        unsigned short              usri2_bad_pw_count;
        unsigned short              usri2_num_logons;
        unsigned char LSFAR * LSPTR usri2_logon_server;
        unsigned short              usri2_country_code;
        unsigned short              usri2_code_page;
};      /* user_info_2 */

struct user_info_10 {
        unsigned char               usri10_name[UNLEN+1];
        unsigned char               usri10_pad_1;
        unsigned char LSFAR * LSPTR usri10_comment;
        unsigned char LSFAR * LSPTR usri10_usr_comment;
        unsigned char LSFAR * LSPTR usri10_full_name;
};      /* user_info_10 */

struct user_info_11 {
        unsigned char               usri11_name[UNLEN+1];
        unsigned char               usri11_pad_1;
        unsigned char LSFAR * LSPTR usri11_comment;
        unsigned char LSFAR * LSPTR usri11_usr_comment;
        unsigned char LSFAR * LSPTR usri11_full_name;
        unsigned short              usri11_priv;
        unsigned long               usri11_auth_flags;
        long                        usri11_password_age;
        unsigned char LSFAR * LSPTR usri11_home_dir;
        unsigned char LSFAR * LSPTR usri11_parms;
        long                        usri11_last_logon;
        long                        usri11_last_logoff;
        unsigned short              usri11_bad_pw_count;
        unsigned short              usri11_num_logons;
        unsigned char LSFAR * LSPTR usri11_logon_server;
        unsigned short              usri11_country_code;
        unsigned char LSFAR * LSPTR usri11_workstations;
        unsigned long               usri11_max_storage;
        unsigned short              usri11_units_per_week;
        unsigned char LSFAR * LSPTR usri11_logon_hours;
        unsigned short              usri11_code_page;
};      /* user_info_11 */



/*
 *      For User Modals
*/

struct  user_modals_info_0 {
        unsigned short  usrmod0_min_passwd_len;
        unsigned long   usrmod0_max_passwd_age;
        unsigned long   usrmod0_min_passwd_age;
        unsigned long   usrmod0_force_logoff;
        unsigned short  usrmod0_password_hist_len;
        unsigned short  usrmod0_maxbadpw;
};      /* user_modals_info_0 */


struct  user_modals_info_1 {
        unsigned short              usrmod1_role;
        unsigned char LSFAR * LSPTR usrmod1_primary;
};      /* user_modals_info_1 */


#ifdef PURE_32
/****************************************************************
 *                                                              *
 *                  Function prototypes - USER                  *
 *                   for "pure" 32-bit only                     *
 *                                                              *
 ****************************************************************/

extern API32_FUNCTION
  Net32UserAdd ( const unsigned char * pszServer,
                 unsigned long         ulLevel,
                 unsigned char       * pbBuffer,
                 unsigned long         ulBuffer,         /*was USHORT */
                 unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserDel ( const unsigned char   * pszServer,
                 unsigned char         * pszUserName,
                 unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserEnum ( const unsigned char * pszServer,
                  unsigned long         ulLevel,         /*was signed short */
                  unsigned char       * pbBuffer,
                  unsigned long         ulBuffer,        /*was USHORT */
                  unsigned long       * pulEntriesRead,  /*was USHORT */
                  unsigned long       * pulTotalAvail,   /*was USHORT */
                  unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserGetInfo ( const unsigned char * pszServer,
                     unsigned char       * pszUserName,
                     unsigned long         ulLevel,
                     unsigned char       * pbBuffer,
                     unsigned long         ulBuffer,         /*was USHORT */
                     unsigned long       * pulTotalAvail,    /*was USHORT */
                     unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserSetInfo ( const unsigned char * pszServer,
                     unsigned char       * pszUserName,
                     unsigned long         ulLevel,
                     unsigned char       * pbBuffer,
                     unsigned long         ulBuffer,      /*was USHORT */
                     unsigned long         ulParmNum,     /*was signed short */
                     unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserPasswordSet ( const unsigned char * pszServer,
                         unsigned char       * pszUserName,
                         unsigned char       * pszOldPassword,
                         unsigned char       * pszNewPassword,
                         unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserGetGroups ( const unsigned char * pszServer,
                       const unsigned char * pszUserName,
                       unsigned long         ulLevel,      /*was signed short*/
                       unsigned char       * pbBuffer,
                       unsigned long         ulBuffer,        /*was USHORT*/
                       unsigned long       * pulEntriesRead,  /*was USHORT*/
                       unsigned long       * pulTotalAvail,   /*was USHORT*/
                       unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserSetGroups ( const unsigned char * pszServer,
                       const unsigned char * pszUserName,
                       unsigned long         ulLevel,    /*was signed short*/
                       unsigned char       * pbBuffer,
                       unsigned long         ulBuffer,       /*was USHORT*/
                       unsigned long         ulEntries,  /*was USHORT*/
                       unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserModalsGet ( const unsigned char * pszServer,
                       unsigned long         ulLevel,
                       unsigned char       * pbBuffer,
                       unsigned long         ulBuffer,        /*was USHORT */
                       unsigned long       * pulTotalAvail,   /*was USHORT */
                       unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserModalsSet ( const unsigned char * pszServer,
                       unsigned long         ulLevel,      /*was signed short */
                       unsigned char       * pbBuffer,
                       unsigned long         ulBuffer,     /*was USHORT */
                       unsigned long         ulParmNum,    /*was USHORT */
                       unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32UserValidate2 ( unsigned char   * pszServer,
                       unsigned long     ulLevel,        /*was signed short */
                       unsigned char   * pbBuffer,
                       unsigned long     ulBuffer,       /*was SHORT */
                       unsigned long     ulReserved2,    /*was SHORT */
                       unsigned long   * pulTotalAvail,  /*was SHORT */
                       unsigned char   * pszReserved);

#endif /* PURE_32 */




/*
 *      For User Logon Validation
*/

struct user_logon_req_1 {
        unsigned char               usrreq1_name[UNLEN+1];
        unsigned char               usrreq1_pad_1;
        unsigned char               usrreq1_password[SESSION_PWLEN];
        unsigned char LSFAR * LSPTR usrreq1_workstation;
};      /* user_logon_req_1 */


struct user_logon_info_0 {
        unsigned char   usrlog0_eff_name[UNLEN+1];
        unsigned char   usrlog0_pad_1;
};      /* user_logon_info_0 */

struct user_logon_info_1 {
        unsigned short              usrlog1_code;
        unsigned char               usrlog1_eff_name[UNLEN+1];
        unsigned char               usrlog1_pad_1;
        unsigned short              usrlog1_priv;
        unsigned long               usrlog1_auth_flags;
        unsigned short              usrlog1_num_logons;
        unsigned short              usrlog1_bad_pw_count;
        unsigned long               usrlog1_last_logon;
        unsigned long               usrlog1_last_logoff;
        unsigned long               usrlog1_logoff_time;
        unsigned long               usrlog1_kickoff_time;
        long                        usrlog1_password_age;
        unsigned long               usrlog1_pw_can_change;
        unsigned long               usrlog1_pw_must_change;
        unsigned char LSFAR * LSPTR usrlog1_computer;
        unsigned char LSFAR * LSPTR usrlog1_domain;
        unsigned char LSFAR * LSPTR usrlog1_script_path;
        unsigned long               usrlog1_reserved1;
};      /* user_logon_info_1 */


struct user_logon_info_2 {
        unsigned char               usrlog2_eff_name[UNLEN+1];
        unsigned char               usrlog2_pad_1;
        unsigned char LSFAR * LSPTR usrlog2_computer;
        unsigned char LSFAR * LSPTR usrlog2_full_name;
        unsigned char LSFAR * LSPTR usrlog2_usrcomment;
        unsigned long               usrlog2_logon_time;
};      /* user_logon_info_2 */




struct user_logoff_req_1 {
        unsigned char   usrlfreq1_name[UNLEN+1];
        unsigned char   usrlfreq1_pad_1;
        unsigned char   usrlfreq1_workstation[CNLEN+1];
};      /* user_logoff_req_1 */

struct user_logoff_info_1 {
        unsigned short  usrlogf1_code;
        unsigned long   usrlogf1_duration;
        unsigned short  usrlogf1_num_logons;
};      /* user_logoff_info_1 */

/****************************************************************
 *                                                              *
 *              Special values and constants - USER             *
 *                                                              *
 ****************************************************************/


/*
 *      Bit masks for field usriX_flags of user_info_X (X = 0/1).
 */

#define         UF_SCRIPT               0x1
#define         UF_ACCOUNTDISABLE       0x2
#define         UF_DELETE_PROHIBITED    0x4
#define         UF_HOMEDIR_REQUIRED     0x8
#define         UF_PASSWD_NOTREQD       0x20
#define         UF_PASSWD_CANT_CHANGE   0x40

/*
 *      Bit masks for field usri2_auth_flags of user_info_2.
*/

#define         AF_OP_PRINT             0x1
#define         AF_OP_COMM              0x2
#define         AF_OP_SERVER            0x4
#define         AF_OP_ACCOUNTS          0x8


/*
 *      UAS role manifests under NETLOGON
 */
#define         UAS_ROLE_STANDALONE     0
#define         UAS_ROLE_MEMBER         1
#define         UAS_ROLE_BACKUP         2
#define         UAS_ROLE_PRIMARY        3

/*
 *      Values for parmnum for NetUserSetInfo.
 */

/* LM1.0 style */
#define         U1_ALL                  0
#define         U1_NAME                 1
#define         U1_PAD                  2
#define         U1_PASSWD               3
#define         U1_PASSWDAGE            4
#define         U1_PRIV                 5
#define         U1_DIR                  6
#define         U1_COMMENT              7
#define         U1_USER_FLAGS           8
#define         U1_SCRIPT_PATH          9


/* LM2.0 style */
#ifndef  PARMNUM_ALL
#define         PARMNUM_ALL             0
#endif

#define         PARMNUM_NAME            1
#define         PARMNUM_PAD             2
#define         PARMNUM_PASSWD          3
#define         PARMNUM_PASSWDAGE       4
#define         PARMNUM_PRIV            5
#define         PARMNUM_DIR             6
#define         PARMNUM_COMMENT         7
#define         PARMNUM_USER_FLAGS      8
#define         PARMNUM_SCRIPT_PATH     9
#define         PARMNUM_AUTH_FLAGS      10
#define         PARMNUM_FULL_NAME       11
#define         PARMNUM_USR_COMMENT     12
#define         PARMNUM_PARMS           13
#define         PARMNUM_WORKSTATIONS    14
#define         PARMNUM_ACCT_EXPIRES    17
#define         PARMNUM_MAX_STORAGE     18
#define         PARMNUM_UNITS_PER_WEEK  19
#define         PARMNUM_LOGON_HOURS     20
#define         PARMNUM_LOGON_SERVER    23
#define         PARMNUM_COUNTRY_CODE    24
#define         PARMNUM_CODE_PAGE       25
#define         PARMNUM_PASSWD_EXPIRED 100


/*
 *      For SetInfo call (parmnum 0) when password change not required
 */
#define         NULL_USERSETINFO_PASSWD         "              "



#define         TIMEQ_FOREVER                   ((unsigned long) -1L)
#define         USER_MAXSTORAGE_UNLIMITED       ((unsigned long) -1L)
#define         USER_NO_LOGOFF                  ((unsigned long) -1L)
#define         UNITS_PER_DAY                   24
#define         UNITS_PER_WEEK                  UNITS_PER_DAY * 7


/*
 *      Privilege levels (user_info_X field usriX_priv (X = 0/1)).
 */
#define         USER_PRIV_MASK          0x3
#define         USER_PRIV_GUEST         0
#define         USER_PRIV_USER          1
#define         USER_PRIV_ADMIN         2

/*
 *      user modals related defaults
 */
#define         MAX_PASSWD_LEN          PWLEN
#define         DEF_MIN_PWLEN           6
#define         DEF_PWUNIQUENESS        5
#define         DEF_MAX_PWHIST          8
#define         DEF_MAX_PWAGE           (unsigned long) 90*24*3600 /* 90 days */
#define         DEF_MIN_PWAGE           (unsigned long) 0L         /* 0 days */
#define         DEF_FORCE_LOGOFF        (unsigned long) 0xffffffff /* never */
#define         DEF_MAX_BADPW           0                          /* no limit*/
#define         ONE_DAY                 (unsigned long) 01*24*3600 /* 01 day  */
/*
 *      User Logon Validation (codes returned)
*/
#define         VALIDATED_LOGON         0
#define         PASSWORD_EXPIRED        2
#define         NON_VALIDATED_LOGON     3

#define         VALID_LOGOFF            1


/*
 *      parmnum manifests for user modals
*/

#define         MODAL0_PARMNUM_ALL              0
#define         MODAL0_PARMNUM_MIN_LEN          1
#define         MODAL0_PARMNUM_MAX_AGE          2
#define         MODAL0_PARMNUM_MIN_AGE          3
#define         MODAL0_PARMNUM_FORCEOFF         4
#define         MODAL0_PARMNUM_HISTLEN          5
#define         MODAL0_PARMNUM_MAX_BADPW        6
#define         MODAL0_PARMNUM_RESERVED1        6

#define         MODAL1_PARMNUM_ALL              0
#define         MODAL1_PARMNUM_ROLE             1
#define         MODAL1_PARMNUM_PRIMARY          2


#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETUSER_INCLUDED */


/****************************************************************
 *                                                              *
 *              Group Class                                     *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETGROUP ) || !defined( LAN_INCLUDED )) \
    && !defined( NETGROUP_INCLUDED )

#define NETGROUP_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *              Function prototypes - GROUP                     *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetGroupAdd ( const unsigned char LSFAR * pszServer,
                short                       sLevel,
                unsigned char LSFAR *       pbBuffer,
                unsigned short              usBuffer );

extern API_FUNCTION
  NetGroupDel ( const unsigned char LSFAR * pszServer,
                unsigned char LSFAR *       pszGroupName );

extern API_FUNCTION
  NetGroupEnum ( const unsigned char LSFAR *     pszServer,
                 short                           sLevel,
                 unsigned char LSFAR *           pbBuffer,
                 unsigned short                  usBuffer,
                 unsigned short LSFAR *          pusEntriesRead,
                 unsigned short LSFAR *          pusTotalAvail );

extern API_FUNCTION
  NetGroupAddUser ( const unsigned char LSFAR * pszServer,
                    unsigned char LSFAR *       pszGroupName,
                    unsigned char LSFAR *       pszUserName );

extern API_FUNCTION
  NetGroupDelUser ( const unsigned char LSFAR * pszServer,
                    unsigned char LSFAR *       pszGroupName,
                    unsigned char LSFAR *       pszUserName );

extern API_FUNCTION
  NetGroupGetUsers ( const unsigned char LSFAR *     pszServer,
                     const unsigned char LSFAR *     pszGroupName,
                     short                           sLevel,
                     unsigned char LSFAR *           pbBuffer,
                     unsigned short                  usBuffer,
                     unsigned short LSFAR *          pusEntriesRead,
                     unsigned short LSFAR *          pusTotalAvail );

extern API_FUNCTION
  NetGroupSetUsers ( const unsigned char LSFAR * pszServer,
                     const unsigned char LSFAR * pszGroupName,
                     short                       sLevel,
                     unsigned char LSFAR *       pbBuffer,
                     unsigned short              usBuffer,
                     unsigned short              usEntries );

extern API_FUNCTION
  NetGroupGetInfo ( const unsigned char LSFAR *     pszServer,
                    unsigned char LSFAR *           pszGroupName,
                    short                           sLevel,
                    unsigned char LSFAR *           pbBuffer,
                    unsigned short                  usBuffer,
                    unsigned short LSFAR *          pusTotalAvail );

extern API_FUNCTION
  NetGroupSetInfo ( const unsigned char LSFAR * pszServer,
                    unsigned char LSFAR *       pszGroupName,
                    short                       sLevel,
                    unsigned char LSFAR *       pbBuffer,
                    unsigned short              usBuffer,
                    short                       sParmNum );


/****************************************************************
 *                                                              *
 *             Data structure templates - GROUP                 *
 *                                                              *
 ****************************************************************/


struct group_info_0 {
        unsigned char               grpi0_name[GNLEN+1];
};      /* group_info_0 */

struct group_info_1 {
        unsigned char               grpi1_name[GNLEN+1];
        unsigned char               grpi1_pad;
        unsigned char LSFAR * LSPTR grpi1_comment;
};      /* group_info_1 */


struct group_users_info_0 {
        unsigned char               grui0_name[UNLEN+1];
};      /* group_users_info_0 */



/****************************************************************
 *                                                              *
 *              Function prototypes - GROUP                     *
 *              for "pure" 32-bit code                          *
 *                                                              *
 ****************************************************************/
#ifdef PURE_32
extern API32_FUNCTION
  Net32GroupAdd ( const unsigned char   * pszServer,
                  unsigned long           ulLevel,
                  unsigned char         * pbBuffer,
                  unsigned long           ulBuffer,
                  unsigned char         * pszReserved);

extern API32_FUNCTION
  Net32GroupDel ( const unsigned char* pszServer,
                  unsigned char      * pszGroupName,
                  unsigned char      * pszReserved);

extern API32_FUNCTION
  Net32GroupEnum ( const unsigned char    * pszServer,
                   unsigned long            ulLevel,
                   unsigned char          * pbBuffer,
                   unsigned long            ulBuffer,
                   unsigned long          * pulEntriesRead,
                   unsigned long          * pulTotalAvail,
                   unsigned char          * pszReserved);

extern API32_FUNCTION
  Net32GroupAddUser ( const unsigned char* pszServer,
                      unsigned char      * pszGroupName,
                      unsigned char      * pszUserName,
                      unsigned char      * pszReserved);

extern API32_FUNCTION
  Net32GroupDelUser ( const unsigned char* pszServer,
                      unsigned char      * pszGroupName,
                      unsigned char      * pszUserName,
                      unsigned char      * pszReserved);

extern API32_FUNCTION
  Net32GroupGetUsers ( const unsigned char    * pszServer,
                       const unsigned char    * pszGroupName,
                       unsigned long            ulLevel,
                       unsigned char          * pbBuffer,
                       unsigned long            ulBuffer,
                       unsigned long          * pulEntriesRead,
                       unsigned long          * pulTotalAvail,
                       unsigned char          * pszReserved);

extern API32_FUNCTION
  Net32GroupSetUsers ( const unsigned char    * pszServer,
                       const unsigned char    * pszGroupName,
                       unsigned long            ulLevel,
                       unsigned char          * pbBuffer,
                       unsigned long            ulBuffer,
                       unsigned long            ulEntries,
                       unsigned char          * pszReserved);

extern API32_FUNCTION
  Net32GroupGetInfo ( const unsigned char * pszServer,
                      unsigned char       * pszGroupName,
                      unsigned long         ulLevel,
                      unsigned char       * pbBuffer,
                      unsigned long         ulBuffer,
                      unsigned long       * pulTotalAvail,
                      unsigned char       * pszReserved);

extern API32_FUNCTION
  Net32GroupSetInfo ( const unsigned char * pszServer,
                      unsigned char       * pszGroupName,
                      unsigned long         ulLevel,
                      unsigned char       * pbBuffer,
                      unsigned long         ulBuffer,
                      unsigned long         ulParmNum,
                      unsigned char       * pszReserved);
#endif /* PURE_32*/


/****************************************************************
 *                                                              *
 *              Special values and constants - GROUP            *
 *                                                              *
 ****************************************************************/

#define         GROUPIDMASK     0x8000  /* MSB set if uid refers to a group */

/*
 *      Predefined group for all normal users, administrators and guests
 *      LOCAL is a special group for pinball local security.
*/

#define         GROUP_SPECIALGRP_USERS          "USERS"
#define         GROUP_SPECIALGRP_ADMINS         "ADMINS"
#define         GROUP_SPECIALGRP_GUESTS         "GUESTS"
#define         GROUP_SPECIALGRP_LOCAL          "LOCAL"


/*
 *      parmnum manifests for SetInfo calls (only comment is settable)
*/

#define         GRP1_PARMNUM_ALL        0
#define         GRP1_PARMNUM_NAME       1
#define         GRP1_PARMNUM_COMMENT    2

#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETGROUP_INCLUDED */

/****************************************************************
 *                                                              *
 *                 Access Class                                 *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETACCESS ) || !defined( LAN_INCLUDED )) \
    && !defined( NETACCESS_INCLUDED )

#define NETACCESS_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *                  Function prototypes - ACCESS                *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetAccessAdd ( const unsigned char LSFAR * pszServer,
                 short                       sLevel,
                 unsigned char LSFAR *       pbBuffer,
                 unsigned short              cbBuffer );

extern API_FUNCTION
  NetAccessCheck ( unsigned char LSFAR *  pszReserved,
                   unsigned char LSFAR *  pszUserName,
                   unsigned char LSFAR *  pszResource,
                   unsigned short         usOperation,
                   unsigned short LSFAR * pusResult );

extern API_FUNCTION
  NetAccessDel ( const unsigned char LSFAR * pszServer,
                 unsigned char LSFAR *       pszResource );

extern API_FUNCTION
  NetAccessEnum ( const unsigned char LSFAR * pszServer,
                  unsigned char LSFAR *       pszBasePath,
                  short                       fsRecursive,
                  short                       sLevel,
                  unsigned char LSFAR *       pbBuffer,
                  unsigned short              cbBuffer,
                  unsigned short LSFAR *      pcEntriesRead,
                  unsigned short LSFAR *      pcTotalAvail );

extern API_FUNCTION
  NetAccessGetInfo ( const unsigned char LSFAR * pszServer,
                     unsigned char LSFAR *       pszResource,
                     short                       sLevel,
                     unsigned char LSFAR *       pbBuffer,
                     unsigned short              cbBuffer,
                     unsigned short LSFAR *      pcbTotalAvail );

extern API_FUNCTION
  NetAccessSetInfo ( const unsigned char LSFAR * pszServer,
                     unsigned char LSFAR *       pszResource,
                     short                       sLevel,
                     unsigned char LSFAR *       pbBuffer,
                     unsigned short              cbBuffer,
                     short                       sParmNum );

extern API_FUNCTION
  NetAccessGetUserPerms ( unsigned char LSFAR *  pszServer,
                          unsigned char LSFAR *  pszUgName,
                          unsigned char LSFAR *  pszResource,
                          unsigned short LSFAR * pusPerms );

extern API_FUNCTION
  NetAccessApply( const unsigned char LSFAR * pszServer,
                  unsigned char LSFAR *       pszApplydir,
                  unsigned char LSFAR *       pbBuffer,
                  unsigned short              cbBuffer,
                  unsigned char LSFAR *       pszreserved1,
                  unsigned long               ulreserved2,
                  unsigned long               uloptions);

/****************************************************************
 *                                                              *
 *              Data structure templates - ACCESS               *
 *                                                              *
 ****************************************************************/

struct access_list {
        unsigned char   acl_ugname[UNLEN+1];
        unsigned char   acl_ugname_pad_1;
        short           acl_access;
};      /* access_list */

struct access_info_0 {
        unsigned char LSFAR * LSPTR acc0_resource_name;
};      /* access_info_0 */

struct access_info_1 {
        unsigned char LSFAR * LSPTR acc1_resource_name;
        short                       acc1_attr;  /* See values below */
        short                       acc1_count;
};      /* access_info_1 */

struct applyerr_info_1 {
        unsigned char LSFAR * LSPTR error_buf;
        unsigned short error_code;
};      /* applyerr_info_1 */

/* Supported values for "uloptions" parameter of NetAccessApply */
#define APPLY_ABSOLUTE      0x01



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *           32-Bit Function prototypes - ACCESS                *
 *                                                              *
 ****************************************************************/
extern API32_FUNCTION
  Net32AccessAdd ( const unsigned char   * pszServer,
                   unsigned long           ulLevel,      /*was signed short*/
                   unsigned char         * pbBuffer,
                   unsigned long           ulBuffer );   /*was USHORT*/

extern API32_FUNCTION
  Net32AccessApply( const unsigned char * pszServer,
                    unsigned char       * pszApplydir,
                    unsigned char       * pbBuffer,
                    unsigned long         ulBuffer,    /*was USHORT*/
                    unsigned char       * pszreserved1,
                    unsigned long         ulreserved2,
                    unsigned long         uloptions);

extern API32_FUNCTION
  Net32AccessCheck ( unsigned char * pszReserved,
                     unsigned char * pszUserName,
                     unsigned char * pszResource,
                     unsigned long   ulOperation,  /*was short*/
                     unsigned long * pulResult );  /*was short*/

extern API32_FUNCTION
  Net32AccessDel ( const unsigned char* pszServer,
                   unsigned char      * pszResource );

extern API32_FUNCTION
  Net32AccessEnum ( const unsigned char   * pszServer,
                    unsigned char         * pszBasePath,
                    short                   fsRecursive,
                    unsigned long           ulLevel,        /*was signed short*/
                    unsigned char         * pbBuffer,
                    unsigned long           cbBuffer,       /*was USHORT*/
                    unsigned long         * pcEntriesRead,  /*was USHORT*/
                    unsigned long         * pcTotalAvail ); /*was USHORT*/

extern API32_FUNCTION
  Net32AccessGetInfo ( const unsigned char * pszServer,
                       unsigned char       * pszResource,
                       unsigned long         ulLevel,
                       unsigned char       * pbBuffer,
                       unsigned long         cbBuffer,        /*was short*/
                       unsigned long       * pcbTotalAvail ); /*was short*/

extern API32_FUNCTION
  Net32AccessSetInfo ( const unsigned char * pszServer,
                       unsigned char       * pszResource,
                       unsigned long         ulLevel,     /*was signed short*/
                       unsigned char       * pbBuffer,
                       unsigned long         ulBuffer,    /*was short*/
                       unsigned long         ulParmNum ); /*was signed short*/

extern API32_FUNCTION
  Net32AccessGetUserPerms ( unsigned char * pszServer,
                            unsigned char * pszUgName,
                            unsigned char * pszResource,
                            unsigned long * pulPerms );  /*was short*/
#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants - ACCESS           *
 *                                                              *
 ****************************************************************/

/*
 *      Maximum number of permission entries for each resource.
 */

#define MAXPERMENTRIES  64


/*
 *      Bit values for the access permissions.  ACCESS_ALL is a handy
 *      way to specify maximum permissions.  These are used in
 *      acl_access field of access_list structures.
 */
#define         ACCESS_NONE     0
#define         ACCESS_ALL     (ACCESS_READ|ACCESS_WRITE|ACCESS_CREATE|ACCESS_EXEC|ACCESS_DELETE|ACCESS_ATRIB|ACCESS_PERM)
#define         ACCESS_READ     0x1
#define         ACCESS_WRITE    0x2
#define         ACCESS_CREATE   0x4
#define         ACCESS_EXEC     0x8
#define         ACCESS_DELETE   0x10
#define         ACCESS_ATRIB    0x20
#define         ACCESS_PERM     0x40

#define         ACCESS_GROUP    0x8000

/*
 *      Bit values for the acc1_attr field of the access_info_1 structure.
 *      Only one bit is currently defined.
 */

#define         ACCESS_AUDIT            0x1

/*
 *      Parmnum value for NetAccessSetInfo.
 */

#define         ACCESS_ATTR_PARMNUM     2


/*
 *      ACCESS_LETTERS defines a letter for each bit position in
 *      the acl_access field of struct access_list.  Note that some
 *      bits have a corresponding letter of ' ' (space).
 */

#define         ACCESS_LETTERS          "RWCXDAP         "


#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETACCESS_INCLUDED */

/****************************************************************
 *                                                              *
 *              Domain Class                                    *
 *                                                              *
 ****************************************************************/

#if (defined( INCL_NETDOMAIN ) || !defined( LAN_INCLUDED )) \
    && !defined( NETDOMAIN_INCLUDED )

#define NETDOMAIN_INCLUDED

#ifdef ANY_32_BIT
#pragma pack(1)
#endif


/****************************************************************
 *                                                              *
 *                  Function prototypes - DOMAIN                *
 *                                                              *
 ****************************************************************/

extern API_FUNCTION
  NetGetDCName ( const unsigned char LSFAR * pszServer,
                 const unsigned char LSFAR * pszDomain,
                 unsigned char LSFAR *       pbBuffer,
                 unsigned short              cbBuffer );

extern API_FUNCTION
  NetLogonEnum ( const unsigned char LSFAR * pszServer,
                 short                       sLevel,
                 unsigned char LSFAR *       pbBuffer,
                 unsigned short              cbBuffer,
                 unsigned short LSFAR *      pusEntriesRead,
                 unsigned short LSFAR *      pusTotalAvail );


#ifdef PURE_32
/********************************************************************
 *                                                                  *
 * Function prototypes - DOMAIN - for 32-bit code, mixed and "pure" *
 *                                                                  *
 ********************************************************************/

extern API32_FUNCTION
  Net32GetDCName ( const unsigned char * pszServer,
                   const unsigned char * pszDomain,
                   unsigned char       * pbBuffer,
                   unsigned long         ulBuffer );

extern API32_FUNCTION
  Net32LogonEnum ( const unsigned char * pszServer,
                   unsigned long         ulLevel,
                   unsigned char       * pbBuffer,
                   unsigned long         ulBuffer,
                   unsigned long       * pulEntriesRead,
                   unsigned long       * pulTotalAvail );

#endif /* PURE_32 */


/****************************************************************
 *                                                              *
 *              Special values and constants - DOMAIN           *
 *                                                              *
 ****************************************************************/

#define         LOGON_INFO_UNKNOWN      -1


#ifdef ANY_32_BIT
#pragma pack()
#endif

#endif /* NETDOMAIN_INCLUDED */
