/********************************************************Copyright.IBM*****/
/*                                                                        */
/*                                                                        */
/*                                                                        */
/*    OCO Source Materials                                                */
/*    IBM Confidential (IBM Confidential-Restricted when combined         */
/*    with the aggregated OCO source modules for this program)            */
/*                                                                        */
/*    5622-069                                                            */
/*    (C) Copyright International Business Machines Corp. 1993            */
/*                                                                        */
/*                                                                        */
/********************************************************Copyright.IBM*****/


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
 *      @(#)un.h        7.2 (Berkeley) 12/30/87
 */
#ifndef UN_H                           /* @A0a                             */
#define UN_H                           /* @A0a                             */

/*
 * Definitions for UNIX IPC domain.
 */
struct  sockaddr_un {
        short   sun_family;            /* AF_UNIX                          */
        char    sun_path[108];         /* path name (gag)                  */
};

#endif /* UN_H */                      /* @A0a                             */
