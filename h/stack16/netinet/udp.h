
/********************************************************copyrite.xmc***/
/*                                                                     */
/*   Licensed Materials - Property of IBM                              */
/*                                                                     */
/*   This module is "Restricted Materials of IBM":                     */
/*   (C) Copyright IBM Corporation. 1990, 1991.                        */
/*                                                                     */
/*   See IBM Copyright Instructions.                                   */
/*                                                                     */
/********************************************************copyrite.xmc***/
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *      @(#)udp.h       7.4.1.1 (Berkeley) 2/7/88
 */
#ifndef _UDP
#define _UDP


#pragma pack(1)

struct  udpstat {
      u_long    udps_hdrops;
      u_long    udps_badsum;
      u_long    udps_badlen;
      u_long    udps_udpInDatagrams;
      u_long    udps_udpNoPorts;
      u_long    udps_udpOutDatagrams;
};
#pragma pack()
#endif
