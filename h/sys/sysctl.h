/*
 * Copyright (c) 1989, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Karels at Berkeley Software Design, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *      @(#)sysctl.h    8.2 (Berkeley) 3/30/95
 */

#ifndef _SYS_SYSCTL_H_
#define _SYS_SYSCTL_H_

#ifdef TCPV40HDRS
#error error: sys\sysctl.h is for TCP/IP toolkit 5.0 or later releases only
#else
/*
 * Definitions for sysctl call.  The sysctl call uses a hierarchical name
 * for objects that can be examined or modified.  The name is expressed as
 * a sequence of integers.  Like a file path name, the meaning of each
 * component depends on its place in the hierarchy.  The top-level and kern
 * identifiers are defined here, and other identifiers are defined in the
 * respective subsystem header files.
 */

#define CTL_MAXNAME     12      /* largest number of components supported */

/*
 * Each subsystem defined by sysctl defines a list of variables
 * for that subsystem. Each name is either a node with further
 * levels defined below it, or it is a leaf of some particular
 * type given below. Each sysctl level defines a set of name/type
 * pairs to be used by sysctl(1) in manipulating the subsystem.
 */
struct ctlname {
        char    *ctl_name;      /* subsystem name */
        int     ctl_type;       /* type of name */
};

#define CTLTYPE_NODE    1       /* name is a node */
#define CTLTYPE_INT     2       /* name describes an integer */
#define CTLTYPE_STRING  3       /* name describes a string */
#define CTLTYPE_QUAD    4       /* name describes a 64-bit number */
#define CTLTYPE_STRUCT  5       /* name describes a structure */
#define CTLTYPE_INETCFG 6       /* inetcfg sysctl code */
#define CTLTYPE_INEVER  7       /* inetver sysctl code */

/*
 * Top-level identifiers
 */
#define CTL_KERN        1               /* "high kernel": proc, limits */
#define CTL_NET         4               /* network, see socket.h */
#define CTL_OS2         9               /* OS/2 specific codes */

#define CTL_NAMES { \
        { 0, 0 }, \
        { "kern", CTLTYPE_NODE }, \
        { "net", CTLTYPE_NODE }, \
        { "os2", CTLTYPE_NODE }, \
}

/*
 * CTL_KERN identifiers
 */
#define KERN_MAXFILES            7      /* int: max open files */
#define KERN_HOSTNAME           10      /* string: hostname */
#define KERN_HOSTID             11      /* int: host identifier */

#define CTL_KERN_NAMES { \
        { 0, 0 }, \
        { "ostype", CTLTYPE_STRING }, \
        { "osrelease", CTLTYPE_STRING }, \
        { "osrevision", CTLTYPE_INT }, \
        { "version", CTLTYPE_STRING }, \
        { "maxvnodes", CTLTYPE_INT }, \
        { "maxproc", CTLTYPE_INT }, \
        { "maxfiles", CTLTYPE_INT }, \
        { "argmax", CTLTYPE_INT }, \
        { "securelevel", CTLTYPE_INT }, \
        { "hostname", CTLTYPE_STRING }, \
        { "hostid", CTLTYPE_INT }, \
        { "clockrate", CTLTYPE_STRUCT }, \
        { "vnode", CTLTYPE_STRUCT }, \
        { "proc", CTLTYPE_STRUCT }, \
        { "file", CTLTYPE_STRUCT }, \
        { "profiling", CTLTYPE_NODE }, \
        { "posix1version", CTLTYPE_INT }, \
        { "ngroups", CTLTYPE_INT }, \
        { "job_control", CTLTYPE_INT }, \
        { "saved_ids", CTLTYPE_INT }, \
        { "boottime", CTLTYPE_STRUCT }, \
}

/*
 * KERN_SYSCTL objects
 */
#define KERNCTL_INETVER      70          /* Sysctl code for sockets Inetversion */
#define OS2_MEMMAPIO         1           /* memory map io */
#define OS2_QUERY_MEMMAPIO   2           /* Query if mapped memory usable */

/* Generic Structure for Inetcfg calls */
struct inetcfg_ctl{
          unsigned long var_name;
          unsigned long var_cur_val;
          unsigned long var_max_val;
          unsigned long var_def_val;
          unsigned long var_min_val;
};

/* Inetversion */
struct inetvers_ctl {
         float version;
         char  versionstr[10];           /* Less than 10 chars in version string */
};

#include <sys/cdefs.h>
#ifndef KERNEL
__BEGIN_DECLS
int _System sysctl __TCPPROTO((int *, u_int, void *, size_t *, void *, size_t));
__END_DECLS
#endif

#endif /* TCPV40HDRS */
#endif /* !_SYS_SYSCTL_H_ */
