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
/*  DISCLAIMER OF WARRANTIES.  The following [enclosed] code is             */
/*  sample code created by IBM Corporation. This sample code is not         */
/*  part of any standard or IBM product and is provided to you solely       */
/*  for  the purpose of assisting you in the development of your            */
/*  applications.  The code is provided "AS IS", without                    */
/*  warranty of any kind.  IBM shall not be liable for any damages          */
/*  arising out of your use of the sample code, even if they have been      */
/*  advised of the possibility of such damages.                             */
/*--------------------------------------------------------------------------*/
/************************** OS2IOCTL.C *****************************************/
/*   Note that no special care is taken to restore the stack parameters.    */
/****************************************************************************/
#define INC_DOS
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <types.h>
#include <sys/socket.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <unistd.h>

/* Globals */
int failures, success;
char interfaces[256],allinterfaces[512];

/* Prototypes */
void make_interface_list(char *intflist, char *allintflist);
void ioctl_synattack(void);
void ioctl_cntrt(void);
int soclose(int);

void main()
{
        fflush(stdout);
        fflush(stdin);

        printf("Issuing SIOSTAT_CNTRT & SIOSTAT_RT\n");
        ioctl_cntrt();

        printf("\nIssuing SIOCSSYN for setting the SYN Attack ON/OFF \n");
        ioctl_synattack();
        printf("Can check the flag being set thru \"inetcfg -g syn\"\n\n");

        printf ("Total Successes:%d\n",success);
        printf ("Total Failures :%d\n",failures);

        exit(0);
}


// SYN ATTACK IOCTL
void ioctl_synattack()
{
        int s;
        int on_off =1;    /* set flag ON */

        if((s = socket(AF_INET, SOCK_RAW, 0)) < 0) {
           printf(" *** Err in getting a socket\n");
           exit(1);
        } /* endif */

        /* Set the SYN attack flag to ON/OFF */
        if (ioctl(s,SIOCSSYN,(char *)&on_off) < 0) {
              printf(" *** IOCTL failure for SIOCSSYN\n");
             failures++;
        }
        else{
              printf ("SYN Attack flag set to %d\n\n", on_off);
              success++;
        }
        soclose (s);
}


//SIO STAT CNTRT & SIO STAT RT
void ioctl_cntrt()
{
   int s;
   struct rtentries *p;
   int nohosts, nonets, i;
   struct sockaddr_in *sin, *singate;
   struct ortentry *rt;
   u_int cnt = 0;
   u_long size;
   char *ptr =NULL;
   short netmasksupport = 0;
   char *name;

   if((s = socket(AF_INET, SOCK_RAW, 0)) < 0) {
      printf(" *** Err in getting a socket\n");
      exit(1);
   } /* endif */

   printf("Issuing ioctl SIOSTATCNTRT \n");
   if(ioctl(s, SIOSTATCNTRT, (char *)&cnt) < 0) {
      printf("ioctl(SIOSTATCNTRT) failure\n");
      failures++;
      size=65536;
      return ;
   } else {
      if(!cnt)
          return;
      success++;
      printf ("\nSize of RTables returned by SIOSTATCNTRT : %d\n",cnt);
      size = cnt*(IFNAMSIZ+sizeof(struct ortentry))+4;
   }

   if(size > 65535)
      size = (65535 / (IFNAMSIZ + sizeof(struct ortentry))) *
                    sizeof(struct ortentry) + 4;
   printf ("Size of RTables recomputed to be  : %d\n",size);

   p =  malloc(size);

   if (p==NULL){
      printf("Malloc failed ...\n");
      return;
   }


   printf("\nIssuing ioctl SIOSTATRT \n");
   if (os2_ioctl(s, SIOSTATRT, (caddr_t)p,(long)size) < 0) {
      free(p);
      printf("SIOSTATRT Err:\n");
      failures++;
      return;
   } else
      success++;

   nohosts = p->hostcount;
   nonets  = p->netcount;
   rt      = p->rttable;

   printf("\n");
   printf("ROUTE found FOR \tGATEWAY \n" );
   if(nohosts+nonets>0) {
     for(i=0;i<nohosts+nonets;i++) {
        sin = (struct sockaddr_in *)(&rt->rt_dst);
        if(sin->sin_addr.s_addr == 0L && rt->rt_hash == 0)
        printf("%15s ","default");
        else printf("%15s ",inet_ntoa(sin->sin_addr));
        sin = (struct sockaddr_in *)(&rt->rt_gateway);
        printf("%15s ",inet_ntoa(sin->sin_addr));
        rt++;
        ptr = name = (char *) rt;
        name = name + strlen(name) + 1;
        rt = (struct ortentry *) name;
        printf("\n");
     }
   }

   free(p);
   soclose(s);
}


