
/*
 * Copyright (c) 1983, 1987 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *      @(#)resolv.h    5.6 (Berkeley) 9/20/88
 */
#define ioc(x,y)       ((x<<8)|y)

#define FIONREAD        ioc('f', 127)
#define FIONBIO         ioc('f', 126)
#define FIOASYNC        ioc('f', 125)
#define FIOTCPCKSUM     ioc('f', 128)
#define FIONSTATUS      ioc('f', 120)
#define FIONURG         ioc('f', 121)
#define FIOBSTATUS      ioc('f', 122)

#define SIOCSHIWAT      ioc('s',  0)
#define SIOCGHIWAT      ioc('s',  1)
#define SIOCSLOWAT      ioc('s',  2)
#define SIOCGLOWAT      ioc('s',  3)
#define SIOCATMARK      ioc('s',  7)
#define SIOCSPGRP       ioc('s',  8)
#define SIOCGPGRP       ioc('s',  9)
#define SIOCSHOSTID     ioc('s', 10)

/* NEW IOCTL COMMAND CODES FOR AFNB                                   */
/* SIOCGNCBFN  - Issue NETBIOS NCB.FIND.NAME                          */
/* SIOCGNBNAME - Get NETBIOS HOSTNAME                                 */
/* SIOCSNBNAME - Set NETBIOS HOSTNAME                                 */

#define SIOCGNBNAME     ioc('s', 11)
#define SIOCSNBNAME     ioc('s', 12)
#define SIOCGNCBFN      ioc('s', 13)

#define SIOCADDRT       ioc('r', 10)
#define SIOCDELRT       ioc('r', 11)
#define SIOMETRIC1RT    ioc('r', 12)
#define SIOMETRIC2RT    ioc('r', 13)
#define SIOMETRIC3RT    ioc('r', 14)
#define SIOMETRIC4RT    ioc('r', 15)

#define SIOCREGADDNET   ioc('r', 12)
#define SIOCREGDELNET   ioc('r', 13)
#define SIOCREGROUTES   ioc('r', 14)
#define SIOCFLUSHROUTES ioc('r', 15)

#define SIOCSIFADDR     ioc('i', 12)
#define SIOCGIFADDR     ioc('i',13)
#define SIOCSIFDSTADDR  ioc('i', 14)
#define SIOCGIFDSTADDR  ioc('i',15)
#define SIOCSIFFLAGS    ioc('i', 16)
#define SIOCGIFFLAGS    ioc('i',17)
#define SIOCGIFBRDADDR  ioc('i',18)
#define SIOCSIFBRDADDR  ioc('i',19)
#define SIOCGIFCONF     ioc('i',20)
#define SIOCGIFNETMASK  ioc('i',21)
#define SIOCSIFNETMASK  ioc('i',22)
#define SIOCGIFMETRIC   ioc('i',23)
#define SIOCSIFMETRIC   ioc('i',24)
#define SIOCSIFSETSIG   ioc('i', 25)
#define SIOCSIFCLRSIG   ioc('i', 26)
#define SIOCSIFBRD      ioc('i', 27) /* SINGLE-rt bcst. using old # for bkw cmpt */
#define SIOCSIFALLRTB   ioc('i', 63) /* added to configure all-route broadcst */

#define SIOCGIFLOAD     ioc('i', 27)
#define SIOCSIFFILTERSRC ioc('i', 28)
#define SIOCGIFFILTERSRC ioc('i',29)

#define SIOCSARP        ioc('i', 30)
#define SIOCGARP        ioc('i', 31)
#define SIOCDARP        ioc('i', 32)
#define SIOCSIFSNMPSIG  ioc('i', 33)
#define SIOCSIFSNMPCLR  ioc('i', 34)
#define SIOCSIFSNMPCRC  ioc('i', 35)
#define SIOCSIFPRIORITY ioc('i', 36)
#define SIOCGIFPRIORITY ioc('i', 37)
#define SIOCSIFFILTERDST ioc('i', 38)
#define SIOCGIFFILTERDST ioc('i',39)
#define SIOCSIF802_3     ioc('i',40)
#define SIOCSIFNO802_3   ioc('i',41)
#define SIOCSIFNOREDIR   ioc('i',42)
#define SIOCSIFYESREDIR  ioc('i',43)

#define SIOCSIFMTU      ioc('i',45)
#define SIOCSIFFDDI     ioc('i',46)
#define SIOCSIFNOFDDI   ioc('i',47)
#define SIOCSRDBRD      ioc('i',48)
#define SIOCSARP_TR     ioc('i',49)
#define SIOCGARP_TR     ioc('i',50)

/* multicast ioctls */
#define SIOCADDMULTI    ioc('i', 51)    /* add m'cast addr */
#define SIOCDELMULTI    ioc('i', 52)    /* del m'cast addr */
#define SIOCMULTISBC    ioc('i', 61)   /* use broadcast to send IP multicast*/
#define SIOCMULTISFA    ioc('i', 62)   /* use functional addr to send IP multicast*/

/* alias */
#define SIOCAIFADDR     ioc('i',63)   /* add an alias IP address */
#define SIOCDIFADDR     ioc('i',64)   /* del an alias IP address */


#ifdef SLBOOTP
#define SIOCGUNIT       ioc('i', 70)  /* Used to retreive unit number on */
                                      /* serial interface */
#endif

#define SIOCSIFSPIPE     ioc('i',71)   /* used to set pipe size on interface */
                                    /* this is used as tcp send buffer size */
#define SIOCSIFRPIPE     ioc('i',72)   /* used to set pipe size on interface */
                                    /* this is used as tcp recv buffer size */
#define SIOCSIFTCPSEG   ioc('i',73)  /* set the TCP segment size on interface*/
#define SIOCSIFUSE576   ioc('i',74)  /* enable/disable the automatic change of mss to 576 */
                                     /* if going through a router */
#define SIOCGIFVALID    ioc('i',75)   /* to check if the interface is Valid or not */

#define SIOCGIFBOUND    ioc('i',76)  /* ioctl to return bound/shld bind ifs */

/* Get multicast gp. info for an interface  */
#define SIOCGMCAST      ioc('i',81)  /* ret list of m-cast addrs for an if */

/* Interface Tracing Support */
#define SIOCGIFEFLAGS   ioc('i',150)
#define SIOCSIFEFLAGS   ioc('i',151)
#define SIOCGIFTRACE    ioc('i',152)
#define SIOCSIFTRACE    ioc('i',153)

#ifdef SLSTATS
#define SIOCSSTAT      ioc('i',154)
#define SIOCGSTAT      ioc('i',155)
#endif

/* NETSTAT stuff */
#define SIOSTATMBUF     ioc('n',40)
#define SIOSTATTCP      ioc('n',41)
#define SIOSTATUDP      ioc('n',42)
#define SIOSTATIP       ioc('n',43)
#define SIOSTATSO       ioc('n',44)
#define SIOSTATRT       ioc('n',45)
#define SIOFLUSHRT      ioc('n',46)
#define SIOSTATICMP     ioc('n',47)
#define SIOSTATIF       ioc('n',48)
#define SIOSTATAT       ioc('n',49)
#define SIOSTATARP      ioc('n',50)
#define SIOSTATIF42     ioc('n',51)
/* ioctls which return the cnt of the arp and route entris */
#define SIOSTATCNTRT   ioc('n', 52)
#define SIOSTATCNTAT   ioc('n', 53)
