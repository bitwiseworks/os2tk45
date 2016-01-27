#ifndef __RPCNETDB_32H
#define __RPCNETDB_32H

/*
 * Copyright (c) 1980,1983,1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *      @(#)netdb.h         5.9(Berkeley) 4/5/88
 */

struct rpcent {
        char    *r_name;        /* name of server for this rpc program */
        char    **r_aliases;    /* alias list */
        u_long  r_number;       /* rpc program number */
};

struct rpcent   *getrpcbyname(char *);
struct rpcent   *getrpcbynumber(u_long);
struct rpcent   *getrpcent(void);

#endif /* __RPCNETDB_32H */
