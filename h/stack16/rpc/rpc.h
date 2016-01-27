/********************************************************copyrite.xmc***/
/*                                                                     */
/*   Licensed Materials - Property of IBM                              */
/*                                                                     */
/*   This module is "Restricted Materials of IBM":                     */
/*      Program Number:   5798RXW                                      */
/*      Program Name:     IBM TCP/IP Version 1.2 for OS/2              */
/*   (C) Copyright IBM Corporation. 1990, 1991.                        */
/*                                                                     */
/*   See IBM Copyright Instructions.                                   */
/*                                                                     */
/********************************************************copyrite.xmc***/
#ifndef __RPC_32H
#define __RPC_32H

/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */
/*      @(#)rpc.h 1.1 86/02/03 SMI      */

/*
 * rpc.h, Just includes the billions of rpc header files necessary to
 * do remote procedure calling.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#pragma pack(4)

#include <rpc/rpctypes.h>          /* some typedefs */
#include <netinet/in.h>

/* external data representation interfaces */
#include <rpc/xdr.h>            /* generic (de)serializer */

/* Client side only authentication */
#include <rpc/auth.h>           /* generic authenticator (client side) */

/* Client side (mostly) remote procedure call */
#include <rpc/clnt.h>           /* generic rpc stuff */

/* semi-private protocol headers */
#include <rpc/rpc_msg.h>        /* protocol for rpc messages */
#include <rpc/auth_uni.h>      /* protocol for unix style cred */

/* Server side only remote procedure callee */
#include <rpc/svc.h>            /* service manager and multiplexer */
#include <rpc/svc_auth.h>       /* service side authenticator */

#include <rpc/rpcnetdb.h>       /* routines to read /etc/rpc file */
#include <rpc/pmap_cln.h>       /**** OJAN 11/17/95  *****/

extern int registerrpc(u_long, u_long, u_long, char *(*procname) (), xdrproc_t, xdrproc_t);   /**** OJAN 11/16/95  ****/


#pragma pack()

#endif /* __RPC_32H  */
