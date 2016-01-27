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
/* Modification History:
**  Date:    By:  Tag:    Reason:
**  9/23/96  DRC  DRC01   Rearranged include files to preserve defs
**  9/30/96  DRC  DRC02   removed unreferenced variable
*/

/*
 * Include Files.
 */
#include <netdb.h>            /* DRC01 - rearranged order of included files*/
#include <stdio.h>            /* DRC01 - rearranged order of included files*/
#include <stdlib.h>           /* DRC01 - rearranged order of included files*/
#include <types.h>
#include <netinet\in.h>
#include <sys\socket.h>
#include <string.h>
#include <arpa\inet.h>

char * s_gethostbyname(char * name)
{
    struct hostent *hp;    /* server host name information             */
    struct in_addr in;

    /*
     */
    hp = gethostbyname(name);
    if (hp) {
             memcpy(&in.s_addr, hp->h_addr, hp->h_length);
    } else {
             /* return appropriate error code to calling application */
             exit(1);
    }

    return(inet_ntoa(in));

}

