/*******************************copyrite.xic*********************************/
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
/*  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is             */
/*  sample code created by IBM Corporation. This sample code is not         */
/*  part of any standard or IBM product and is provided to you solely       */
/*  for  the purpose of assisting you in the development of your            */
/*  applications.  The code is provided "AS IS", without                    */
/*  warranty of any kind.  IBM shall not be liable for any damages          */
/*  arising out of your use of the sample code, even if they have been      */
/*  advised of the possibility of such damages.                             */
/*--------------------------------------------------------------------------*/
/***************************** SYSCTL.C *************************************/

typedef unsigned long tcp_seq;

#include <stdio.h>
#include <types.h>
#include <netinet\in.h>
#include <sys\socket.h>
#include <netinet\ip_var.h>
#include <netinet\tcp_var.h>
#include <netinet\udp.h>
#include <netinet\udp_var.h>
#include <netinet\in_systm.h>
#include <netinet\ip.h>
#include <netinet\ip_icmp.h>
#include <netinet\icmp_var.h>
#include <driver.h>
#include <os2.h>
#include <genio.h>
#include <sys\sysctl.h>
#include <sys\lipcpar.h>

/* Globals */
int
   mib[7], i, failures=0, Success=0;
unsigned int
   oldenp=0, newlen=0;

int main(void)
{
   struct a {
          int flag;
          } uap_old, uap_new;

   int org;

   /* Place a sysctl call filling the mib struct */
    mib[0]= CTL_NET;              //cmd
    mib[1]= PF_INET;              //Protocol Family
    mib[2]= IPPROTO_IP;           //Protocol
    mib[3]= IPCTL_FORWARDING;     //Control command for switch(name[0]) in tcp_sysctl

    /* Check the set value */
    uap_old.flag = 0;
    newlen = oldenp  = sizeof(struct a);

    if (sysctl(mib,4,(void *)&uap_old,&oldenp,(void *)NULL,newlen) < 0) {
         failures++;
         printf ("*** ERR ::: sysctl ipforwarding. \n");
    }
    else {
         Success++;
         org = uap_old.flag;
         printf ("sysctl ipforwarding value : %d\n",uap_old.flag);
    }

    /* set a new to 1 */
    uap_new.flag = 1;
    newlen = oldenp  = sizeof(struct a);

    if (sysctl(mib,4,(void *)NULL,&oldenp,(void *)&uap_new,newlen) < 0) {
         failures++;
         printf ("*** ERR ::: sysctl ipforwarding. \n");
    }
    else {
         Success++;
         printf ("sysctl ipforwarding now set to : %d\n",uap_new.flag);
    }

    /* Check the set value */
    uap_old.flag = 0;
    newlen = oldenp  = sizeof(struct a);

    if (sysctl(mib,4,(void *)&uap_old,&oldenp,(void *)NULL,newlen) < 0) {
         failures++;
         printf ("*** ERR ::: sysctl ipforwarding. \n");
    }
    else {
         Success++;
         printf ("sysctl ipforwarding value : %d\n",uap_old.flag);
    }

    /* Set a new to org */
    uap_new.flag = org;
    newlen = oldenp  = sizeof(struct a);

    if (sysctl(mib,4,(void *)NULL,&oldenp,(void *)&uap_new,newlen) < 0) {
         failures++;
         printf ("*** ERR ::: sysctl ipforwarding. \n");
    }
    else {
         Success++;
         printf ("sysctl ipforwarding now set to : %d\n",uap_new.flag);
    }

    /* Check the set value */
    uap_old.flag = 0;
    newlen = oldenp  = sizeof(struct a);

    if (sysctl(mib,4,(void *)&uap_old,&oldenp,(void *)NULL,newlen) < 0) {
         failures++;
         printf ("*** ERR ::: sysctl ipforwarding. \n");
    }
    else {
        Success++;
        printf ("sysctl ipforwarding value : %d\n",uap_old.flag);
   }

}
