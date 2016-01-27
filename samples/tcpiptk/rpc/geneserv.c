/********************************************************copyrite.xic********/
/*                                                                          */
/*   Licensed Materials - Property of IBM                                   */
/*   IBM TCP/IP for OS/2                                                    */
/*   (C) Copyright IBM Corporation. 1990,1996.                              */
/*                                                                          */
/*   All rights reserved.                                                   */
/*                                                                          */
/*   US Government Users Restricted Rights -                                */
/*   Use, duplication or disclosure restricted by GSA ADP Schedule          */
/*   Contract with IBM Corp.                                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is             */
/*  sample code created by IBM Corporation. This sample code is not         */
/*  part of any standard or IBM product and is provided to you solely       */
/*  for  the purpose of assisting you in the development of your            */
/*  applications.  The code is provided "AS IS", without                    */
/*  warranty of any kind.  IBM shall not be liable for any damages          */
/*  arising out of your use of the sample code, even if they have been      */
/*  advised of the possibility of such damages.                             */
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Modification History:                                                  */
/*   Date:   By:   Tag:   Reason:                                           */
/*   9.12.96 DRC   DRC01  Updated to include <stdlib.h>                     */
/*                        the exclusion was generating a warning            */
/*                        during compilation.                               */
/*                                                                          */
/*************************************************************copyrite.xic***/

/* GENERIC SERVER      */
/* RECEIVE AN INTEGER OR FLOAT AND RETURN THEM RESPECTIVELY */
/* PORTMAPPER MUST BE RUNNING */

#include <stdio.h>
#include <stdlib.h>                         /* DRC01 - added include */
#include <rpc/rpc.h>

#define intrcvprog ((u_long)150000)
#define fltrcvprog ((u_long)150102)
#define intvers    ((u_long)1)
#define intrcvproc ((u_long)1)
#define fltrcvproc ((u_long)1)
#define fltvers    ((u_long)1)

main()
{
   char *intrcv();
   char *floatrcv();

  /*REGISTER PROG, VERS AND PROC WITH THE PORTMAPPER*/
      /*FIRST PROGRAM*/
   registerrpc(intrcvprog,intvers,intrcvproc,intrcv,xdr_int,xdr_int);
   printf("Intrcv Registration with Port Mapper completed\n");
      /*OR MULTIPLE PROGRAMS*/
   registerrpc(fltrcvprog,fltvers,fltrcvproc,floatrcv,xdr_float,xdr_float);
   printf("Floatrcv Registration with Port Mapper completed\n");

   svc_run();
   printf("Error:svc_run returned!\n");
   exit(1);
}

char *
intrcv(in)
   int *in;
{
   int *out;

   printf("integer received: %d\n",*in);
   out = in;
   printf("integer being returned: %d\n",*out);
   return ((char *)out);
}


char *
floatrcv(in)
   int *in;
{
   int *out;

   printf("float received: %e\n",*in);
   out=in;
   printf("float being returned: %e\n",*out);   
   return ((char *)out);
}


