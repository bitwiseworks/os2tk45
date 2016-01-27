/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/
/*                                                                        */
/**************************************************************************/


/********************************************************************
 *                                                                  *
 *  About this file ...  CALLOUT.H                                  *
 *                                                                  *
 *  This file contains information about the NetCallout APIs        *
 *  The layout is:                                                  *
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

#ifndef __CALLOUT_H
#define __CALLOUT_H

#ifdef INCL_32
#pragma pack(1)
#endif




USHORT LSFAR LSPAS
NetCalloutAdd(char  LSFAR * LSPTR reserved1,
              USHORT              level,
              USHORT              function,
              char  LSFAR * LSPTR apiname,
              USHORT              ordinal,
              ULONG LSFAR * LSPTR handle);

USHORT LSFAR LSPAS
NetCalloutFree (char LSFAR * LSPTR reserved1,
               ULONG               handle,
               USHORT              function);





struct Callout_Access_Control_Auth_0 {
     USHORT             caca0_function;
     USHORT             caca0_level;
     char LSFAR  *      caca0_username;
     char LSFAR  *      caca0_compname;
     char LSFAR  *      caca0_resource;
     USHORT             caca0_reqperms;
     };

struct Callout_Session_Setup_0 {
     USHORT             cses0_function;
     USHORT             cses0_level;
     char LSFAR *       cses0_username;
     char LSFAR *       cses0_compname;
     };

struct Callout_Open_Failed_0  {
     USHORT             copn0_function;
     USHORT             copn0_level;
     USHORT             copn0_openmode;
     char LSFAR *       copn0_username;
     char LSFAR *       copn0_resource;
     };


struct Callout_User_Password_0 {
     USHORT             cup0_function;
     USHORT             cup0_level;
     USHORT             cup0_action;
     char               cup0_username[UNLEN+1];
     char               pad0;
     char               cup0_password[PWLEN+1];
     char               pad1;
     char               cup0_newpassword[PWLEN+1];
     };








#define CALLOUT_ACCESS_CONTROL_AUTH  1
#define CALLOUT_SESSION_SETUP        2
#define CALLOUT_OPEN_FAILED          3
#define CALLOUT_USER_PASSWORD        4



#define CUP_LOGON                    0
#define CUP_CHANGE                   1
#define CUP_SET                      2



#ifdef INCL_32
#pragma linkage (NetCalloutAdd, far16 pascal)
#pragma linkage (NetCalloutFree, far16 pascal)
#pragma pack()
#endif


#endif
