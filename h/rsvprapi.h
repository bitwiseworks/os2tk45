/* Licensed Materials - Property of IBM                           */
/* IBM TCP/IP for OS/2                                            */
/* (c) Copyright IBM Corp. 1996, 1997.  All rights reserved.      */
/*                                                                */
/* US Government Users Restricted Rights - Use, duplication or    */
/* disclosure restricted by GSA ADP Schedule Contract with IBM    */
/* Corp.                                                          */
/*                                                                */
/* DISCLAIMER OF WARRANTIES.  The following [enclosed] code is    */
/* sample code created by IBM Corporation.  This sample code is   */
/* not part of any standard or IBM product and is provided to you */
/* solely for the purpose of assisting you in the development of  */
/* your applications.  The code is provided "AS IS", without      */
/* warranty of any kind.  IBM shall not be liable for any damages */
/* arising out of your use of the sample code, even if they have  */
/* been advised of the possibility of such damages.               */

#ifndef H_RSVP_RAPI
#define H_RSVP_RAPI

#ifdef TCPV40HDRS
#error error: rsvprapi.h is for TCP/IP toolkit 5.0 or later releases only
#else
/* on IBM compilers, automatically pull in the TCP/IP library */
#if defined(__IBMC__) && !defined(TCP_NODEFLIB) && !defined(KERNEL)
#pragma library("librsvp.lib")
#endif

#include <sys\cdefs.h>
#include <types.h>

#include <sys/socket.h> /* for sockaddr */
#include <netinet/in.h>  /* for sockaddr_in */

#pragma pack(1)

typedef float   float32_t;
typedef u_int32_t rbool_t;

/*****************************************************************************/

/*
 *   Filterspec Formats
 */

typedef enum {
        RAPI_FILTER_NONE      = 0,    /* Empty filter */
        RAPI_FILTERFORM_1     = 1,    /* Complex: sockaddr + (mask, value) */
        RAPI_FILTERFORM_BASE  = 2,    /* Simple v4: Only sockaddr */
        RAPI_FILTERFORM_BASE6 = 3     /* Simple v6: Only sockaddr */
} rapi_filterform_t;

#define RAPI_MAXFILTER_T1       48

typedef struct {
        struct sockaddr sender;    /* or use another defined struct */
        u_int32_t       len;
        u_char          v[RAPI_MAXFILTER_T1];
        u_char          m[RAPI_MAXFILTER_T1];
} rapi_filter_t1_t;

typedef struct {
        struct sockaddr_in sender;
} rapi_filter_base_t;

#ifdef INET6
typedef struct {
        struct sockaddr_in6 sender;
} rapi_filter_base6_t;
#endif

/*
 *      RAPI Filterspec
 */
typedef struct {
        /* len and form are a rapi_hdr_t object header */
        u_int32_t               len;    /* length of filter, in bytes */
        rapi_filterform_t       form;   /* filter type */
        union {
            rapi_filter_t1_t    t1;
            rapi_filter_base_t  base;
#ifdef INET6
            rapi_filter_base6_t base6;
#endif
        }           filter;             /* variable length */
} rapi_filter_t;


/*****************************************************************************/

/*
 *      Int-serv Flowspec/Tspec
 */

#define TB_RATE_MAX             40e12      /* Bytes/sec */
#define TB_RATE_MIN             1          /* Bytes/sec */
#define TB_DEPTH_MAX            250e9      /* Bytes */
#define TB_DEPTH_MIN            1          /* Bytes */
#define TB_PEAK_MAX             0x7f800000 /* +ve infinity */

typedef struct {
        float32_t  IS_Tspec_r;   /* Token bucket rate (IP packet bytes/sec) */
        float32_t  IS_Tspec_b;   /* Token bucket depth (bytes) */
        float32_t  IS_Tspec_p;   /* Peak data rate (IP packet bytes/sec) */
        u_int32_t  IS_Tspec_m;   /* Min Policed Unit (bytes) */
        u_int32_t  IS_Tspec_M;   /* Max packet size (bytes) */
}   IS_Tspec_t;


typedef struct {
        float32_t  Guar_Rspec_R; /* Guaranteed Rate (IP packet bytes/sec) */
        u_int32_t  Guar_Rspec_S; /* Slack term (microseconds) */
}   Guar_Rspec_t;


typedef struct {
        IS_Tspec_t      CL_Tspec;
}   CL_flowspec_t;

typedef struct {
        IS_Tspec_t      Guar_Tspec;
        Guar_Rspec_t    Guar_Rspec;
}   Guar_flowspec_t;


/*
 *      RAPI flowspec/Tspec formats
 */

typedef enum {
        RAPI_FORMAT_NONE       = 0,     /* empty flowspec/tspec        */
        RAPI_FORMAT_IS_CL      = 1,     /* Int-serv Controlled Load    */
        RAPI_FORMAT_IS_GUAR    = 2,     /* Int-serv Guaranteed         */
        RAPI_FORMAT_IS_GEN     = 3      /* Int-serv generic            */
} rapi_format_t;

/*
 *      RAPI Flowspec
 */

typedef struct {
        /* len and form are a rapi_hdr_t object header */
        u_int32_t       len;
        rapi_format_t   form;
        union {
                CL_flowspec_t    IS_CL;     /* Int-serv Controlled Load   */
                Guar_flowspec_t  IS_Guar;   /* Int-serv Guaranteed format */
        }               specbody_u;
}   rapi_flowspec_t;

#define cl_tspec_r     specbody_u.IS_CL.CL_Tspec.IS_Tspec_r
#define cl_tspec_b     specbody_u.IS_CL.CL_Tspec.IS_Tspec_b
#define cl_tspec_p     specbody_u.IS_CL.CL_Tspec.IS_Tspec_p
#define cl_tspec_m     specbody_u.IS_CL.CL_Tspec.IS_Tspec_m
#define cl_tspec_M     specbody_u.IS_CL.CL_Tspec.IS_Tspec_M

#define guar_tspec_r   specbody_u.IS_Guar.Guar_Tspec.IS_Tspec_r
#define guar_tspec_b   specbody_u.IS_Guar.Guar_Tspec.IS_Tspec_b
#define guar_tspec_p   specbody_u.IS_Guar.Guar_Tspec.IS_Tspec_p
#define guar_tspec_m   specbody_u.IS_Guar.Guar_Tspec.IS_Tspec_m
#define guar_tspec_M   specbody_u.IS_Guar.Guar_Tspec.IS_Tspec_M
#define guar_rspec_R   specbody_u.IS_Guar.Guar_Rspec.Guar_Rspec_R
#define guar_rspec_S   specbody_u.IS_Guar.Guar_Rspec.Guar_Rspec_S

/*
 *      RAPI Tspec
 */

typedef struct {
        /* len and form are a rapi_hdr_t object header */
        u_int32_t       len;
        rapi_format_t   form;
        union {
                IS_Tspec_t       IS_Tspec;        /* Int-serv (generic) */
        }               specbody_u;
}   rapi_tspec_t;

#define tspec_r   specbody_u.IS_Tspec.IS_Tspec_r
#define tspec_b   specbody_u.IS_Tspec.IS_Tspec_b
#define tspec_p   specbody_u.IS_Tspec.IS_Tspec_p
#define tspec_m   specbody_u.IS_Tspec.IS_Tspec_m
#define tspec_M   specbody_u.IS_Tspec.IS_Tspec_M


/*****************************************************************************/

/*
 *   Adspec Formats
 */

typedef struct {
        rbool_t         servBreak;        /* break in service */
        u_int16_t       ISHops;           /* num Int-serv aware hops */
        float32_t       pathBW;           /* min path band width (bytes/sec) */
        u_int32_t       pathLatency;      /* min path latency (microseconds) */
        u_int16_t       composedMTU;      /* composed path MTU */
} Gen_adspec_t;

typedef struct {
        rbool_t         servBreak;        /* break in service */
        u_int16_t       ISHops;           /* num Int-serv aware hops */
        float32_t       pathBW;           /* min path band width (bytes/sec) */
        u_int32_t       pathLatency;      /* min path latency (microseconds) */
        u_int16_t       composedMTU;      /* composed path MTU */
        u_int32_t       Ctot;             /* total rate dep. err (bytes) */
        u_int32_t       Dtot;             /* total rate indep. err (microseconds) */
        u_int32_t       Csum;             /* reshaped rate dep. err (bytes) */
        u_int32_t       Dsum;             /* reshaped rate indep. err (microseconds) */
} Guar_adspec_t;

typedef struct {
        rbool_t         servBreak;        /* break in service */
        u_int16_t       ISHops;           /* num Int-serv aware hops */
        float32_t       pathBW;           /* min path band width (bytes/sec) */
        u_int32_t       pathLatency;      /* min path latency (microseconds) */
        u_int16_t       composedMTU;      /* composed path MTU */
} CL_adspec_t;

typedef struct {
        Gen_adspec_t    Gen_Adspec;
        rbool_t         CL_present;
        CL_adspec_t     CL_Adspec;
        rbool_t         Guar_present;
        Guar_adspec_t   Guar_Adspec;
} IS_adspec_t;


/*
 *      RAPI Adspec formats
 */

typedef enum {
        RAPI_ADSPEC_NONE     = 0,     /* empty adspec */
        RAPI_ADSPEC_IS       = 1      /* Int-serv */
} rapi_adspec_form_t;

/*
 *      RAPI Adspec
 */

typedef struct {
        /* len and form are a rapi_hdr_t object header */
        u_int32_t            len;
        rapi_adspec_form_t   form;
        union {
                IS_adspec_t       IS_Adspec;       /* Int-serv */
        }               specbody_u;
}   rapi_adspec_t;

/*****************************************************************************/

/*
 *    Policy Formats
 */

typedef struct {
    char        name[1];

} rapi_policy_name_string_t;

typedef enum {
    RAPI_POLICY_TYPE_NAMESTRING   =  1
} rapi_policy_form_t;

typedef enum {
    RAPI_POLICYF_INTEGRITY        =  0x00000001,
    RAPI_POLICYF_GLOBAL           =  0x00000002,
    RAPI_POLICYF_RESP             =  0x00000004
} rapi_policy_flags_t;


/*
 *    RAPI Policy
 */

typedef struct {
    /* len and form are a rapi_hdr_t object header */
    u_int32_t               len;            /* actual length in bytes */
    rapi_policy_form_t      form;           /* policy data format */
    rapi_policy_flags_t     flags;
    union {
        rapi_policy_name_string_t    namestring;

    } pol_u;

} rapi_policy_t;


/*****************************************************************************/

/*
 *    RAPI Object Header
 */

typedef struct {
    u_int32_t                len;        /* actual length in bytes     */
    int                      form;       /* object format              */
#define RAPI_EMPTY_OTYPE     0           /* Format = 0 => empty object */

} rapi_hdr_t;


/*****************************************************************************/

/*
 *      Reservation style ids
 */
typedef enum {
        RAPI_RSTYLE_WILDCARD = 1,       /* STYLE_ID_WF */
        RAPI_RSTYLE_FIXED = 2,          /* STYLE_ID_FF */
        RAPI_RSTYLE_SE = 3              /* STYLE_ID_SE */
} rapi_styleid_t;

/*
 *      Reservation style extension
 *
 *      This structure is currently undefined; in the future, it will allow
 *      passing style-specific parameters for possible new styles.
 */
typedef void rapi_stylex_t;

/*
 *      Registration flags
 */
#define RAPI_REQ_CONFIRM        0x20    /* Request confirmation */

/*****************************************************************************/

/*
 *   RAPI client handle
 */

typedef unsigned int rapi_sid_t;
#define NULL_SID        0       /* Error return from rapi_session */


/*****************************************************************************/

/*
 *   RAPI User callback function
 */

typedef enum {
        RAPI_PATH_EVENT   = 1,
        RAPI_RESV_EVENT   = 2,
        RAPI_PATH_ERROR   = 3,
        RAPI_RESV_ERROR   = 4,
        RAPI_RESV_CONFIRM = 5,
} rapi_eventinfo_t;


typedef int ((* _System rapi_event_rtn_t)  __TCPPROTO((
        rapi_sid_t,              /* Which sid generated event      */
        rapi_eventinfo_t,        /* Event type                     */
        rapi_styleid_t,          /* Style ID                       */
        int,                     /* Error code                     */
        int,                     /* Error value                    */
        struct sockaddr *,       /* Error node address             */
        u_char,                  /* Error flags                    */
        int,                     /* Number of filter specs/sender  */
                                 /*      templates in array        */
        rapi_filter_t *,         /* Filter spec/sender templ array */
        int,                     /* Number of flowspecs/Tspecs     */
        rapi_flowspec_t *,       /* Flowspec/Tspec array           */
        int,                     /* Number of adspecs              */
        rapi_adspec_t *,         /* Adspec array                   */
        void *                   /* Client supplied arg            */
)));
/* values for error flags above */
#define RAPI_ERRF_InPlace  0x01  /*   Left resv in place        */
#define RAPI_ERRF_NotGuilty 0x02 /*   This rcvr not guilty      */


/*****************************************************************************/

/*
 *      RAPI External Functions
 */
rapi_sid_t _System rapi_session __TCPPROTO((
        struct sockaddr *,      /* Session: (Dst addr, port), net order */
        int,                    /* Protocol id                          */
        int,                    /* Flags - not defined                  */
        rapi_event_rtn_t,       /* Address of callback rtn - optional   */
        void *,                 /* App argument to callback - optional  */
        int *                   /* error return code                    */
));

int _System rapi_sender __TCPPROTO((
        rapi_sid_t,             /* Session ID                              */
        int,                    /* Flags - for traffic control             */
        struct sockaddr *,      /* Local host: (Src addr, port), net order */
        rapi_filter_t *,        /* Sender template - not supported         */
        rapi_tspec_t *,         /* Sender Tspec                            */
        rapi_adspec_t *,        /* Sender Adspec                           */
        rapi_policy_t *,        /* Sender policy data - not supported      */
        int                     /* TTL of multicast data                   */
));
/* values for flag for traffic control above */
#define TC_QOS_POLICE     0x1   /* turn policing on                        */
#define TC_QOS_NOPOLICE   0x2   /* turn policing off                       */
#define TC_QOS_SHAPE      0x4   /* turn shaping on                         */
#define TC_QOS_NOSHAPE    0x8   /* turn shaping off                        */

int _System rapi_reserve __TCPPROTO((
        rapi_sid_t,             /* Session ID                      */
        int,                    /* Flags - not defined             */
        struct sockaddr *,      /* Recv host addr - optional       */
        rapi_styleid_t,         /* Style ID                        */
        rapi_stylex_t *,        /* Style extension - not supported */
        rapi_policy_t *,        /* Receiver policy - not supported */
        int,                    /* # of filter specs               */
        rapi_filter_t *,        /* array of filter specs           */
        int,                    /* # of flowspecs                  */
        rapi_flowspec_t *       /* array of flowspecs              */
));

int _System rapi_release __TCPPROTO((rapi_sid_t     /* Session ID */));

int _System rapi_getfd __TCPPROTO((rapi_sid_t       /* Session ID */));

int _System rapi_dispatch __TCPPROTO((void));

int _System rapi_version __TCPPROTO((void));

/*
    Formatting routines
 */
void _System rapi_fmt_flowspec __TCPPROTO((rapi_flowspec_t *, char *, int));
void _System rapi_fmt_tspec __TCPPROTO((rapi_tspec_t *, char *, int));
void _System rapi_fmt_adspec __TCPPROTO((rapi_adspec_t *, char *, int));
void _System rapi_fmt_filtspec __TCPPROTO((rapi_filter_t *, char *, int));


/*****************************************************************************/

/*
 *      RAPI Error Codes
 */
#define RAPI_ERR_OK             0       /* No error                       */
#define RAPI_ERR_INVAL          1       /* Invalid parameter              */
#define RAPI_ERR_MAXSESS        2       /* Too many sessions              */
#define RAPI_ERR_BADSID         3       /* Sid out of legal range.        */
#define RAPI_ERR_N_FFS          4       /* Wrong n_filter/n_flow for style*/
#define RAPI_ERR_BADSTYLE       5       /* Illegal reservation style      */
#define RAPI_ERR_SYSCALL        6       /* system call error              */
#define RAPI_ERR_OVERFLOW       7       /* Parm list overflow             */
#define RAPI_ERR_MEMFULL        8       /* Not enough memory              */
#define RAPI_ERR_NORSVP         9       /* Daemon doesn't respond/exist   */
#define RAPI_ERR_OBJTYPE        10      /* Object type error              */
#define RAPI_ERR_OBJLEN         11      /* Object length error            */
#define RAPI_ERR_NOTSPEC        12      /* No sender tspec in rapi_sendr  */
#define RAPI_ERR_INTSERV        13      /* Intserv format error           */
/*                              14       * UNUSED                         */

/*  
 * The following occur only asynchronously
 */
#define RAPI_ERR_BADSEND        14      /* Send interface does not exist  */
#define RAPI_ERR_BADRECV        15      /* Recv interface does not exist  */

#define RAPI_ERR_UNSUPPORTED    254
#define RAPI_ERR_UNKNOWN        255

#pragma pack()

#endif /* TCPV40HDRS */
#endif /* H_RSVP_RAPI */
