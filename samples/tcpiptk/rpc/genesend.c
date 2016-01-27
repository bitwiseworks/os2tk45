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
/*   Date:   By:   Tag:    Reason:                                          */
/*   9.12.96 DRC   DRC01   Updated to include <stdlib.h>                    */
/*                         the exclusion was generating a warning           */
/*                         during compilation.                              */
/*                                                                          */
/*************************************************************copyrite.xic***/
/* GENESEND.C */
/* Send an integer to the remote host and receive the integer back at the local host */
/* PORTMAPPER AND REMOTE SERVER MUST BE RUNNING */

#include <stdio.h>
#include <stdlib.h>                         /* DRC01 - added include */
#include <rpc/rpc.h>

#define intrcvprog ((u_long)150000)
#define version    ((u_long)1)
#define intrcvproc ((u_long)1)

main(argc, argv)
   int argc;
   char *argv[];
{
   int innumber;
   int outnumber;
   int error;

   if (argc < 3) {
      fprintf(stderr,"USAGE: genesend hostname integer\n");
      exit (-1);
   } /* endif */
   svc_socks[1]=10;
   innumber = atoi(argv[2]);
   error = callrpc(argv[1],intrcvprog,version,intrcvproc,xdr_int,
                   (char *)&innumber,xdr_int,(char *)&outnumber);
   if (error != 0) {
      fprintf(stderr,"error: CALLRPC failed: %d \n",error);
      fprintf(stderr,"intrcprog: %d version: %d intrcvproc: %d",
                      intrcvprog, version,intrcvproc);
      exit(1);
   } /* endif */

   printf("value sent was: %d   value received is: %d\n", innumber, outnumber);
   exit(0);
}

