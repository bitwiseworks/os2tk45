/**************************************************************************/
/*                                                                        */
/*              IBM OS/2(tm) Local Area Network (LAN) Server              */
/*                            Version 5.03                                */
/*                   (C) Copyright IBM Corp. 1988, 1998                   */
/*                  Licensed Materials - Property of IBM                  */
/*                                                                        */
/**************************************************************************/

/********************************************************************
 *                                                                  *
 *  About this file ...  DASD.H                                     *
 *                                                                  *
 *  This file contains information about the NetDASD apis.          *
 *                                                                  *
 *      Function prototypes.                                        *
 *                                                                  *
 *      Data structure templates.                                   *
 *                                                                  *
 *      Definition of special values.                               *
 *                                                                  *
 *                                                                  *
 *  NOTE:  You must include NETCONS.H before this file, since this  *
 *         file depends on values defined in NETCONS.H.             *
 *                                                                  *
 ********************************************************************/


#if !defined NETDASD_INCLUDED
#define NETDASD_INCLUDED
#pragma pack(1)



/****************************************************************
 *                                                              *
 *              Function prototypes - API DASD                  *
 *                                                              *
 ****************************************************************/

extern  API_FUNCTION
    NetDASDAdd( const unsigned char LSFAR *server,
                short                      level,
                unsigned char LSFAR       *buf,
                unsigned short             buflen
              );

extern  API_FUNCTION
    NetDASDDel( const unsigned char LSFAR *server,
                unsigned char LSFAR       *resource
              );

extern  API_FUNCTION
    NetDASDGetInfo( const unsigned char LSFAR    *server,
                    unsigned char LSFAR          *resource,
                    short                         level,
                    unsigned char LSFAR          *buf,
                    unsigned short                buflen,
                    unsigned short LSFAR         *total
                  );


extern  API_FUNCTION
    NetDASDSetInfo( const unsigned char LSFAR *server,
                    unsigned char LSFAR       *resource,
                    short                      level,
                    unsigned char LSFAR       *buf,
                    unsigned short             buflen
                  );


extern  API_FUNCTION
    NetDASDEnum( const unsigned char LSFAR *server,
                 unsigned char LSFAR       *path,
                 short                      recursive,
                 short                      level,
                 unsigned char LSFAR       *buf,
                 unsigned short             buflen,
                 unsigned short LSFAR      *entries_returned,
                 unsigned short LSFAR      *total_entries
               );


extern  API_FUNCTION
    NetDASDCheck( const unsigned char LSFAR   *server,
                  unsigned char LSFAR         *path,
                  short                        level,
                  unsigned char LSFAR         *buf,
                  unsigned short               buflen,
                  unsigned short LSFAR        *TotalAvail
                );

extern  API_FUNCTION
    NetDASDCtl(  const unsigned char LSFAR *server,
                 short                      level,
                 unsigned char LSFAR       *buf,
                 unsigned short             buflen
               );


             /****************************
              *      DATA STRUCTURES     *
              ****************************/

struct dasd_info_0 {                     /* Level 0 data structure */
    unsigned char LSFAR * LSPTR d0_resource_name; /* Name of resource     */
    unsigned long d0_max;                /* DASD limit             */
    unsigned long d0_use;                /* Usage count (bytes)    */
    unsigned char d0_flag;               /* validate flags         */
    unsigned char d0_thresh;       /* Threshold value in percentage       */
    unsigned char d0_delta;        /* Incremental thresh value in percent.*/
};  /* dasd_info_0 */

struct dasd_init_0 {                     /* Used by NetDASDCtl        */
    unsigned char i0_CtlFlag;            /* Control flag              */
    unsigned char i0_Drive;              /* Drive letter: 'C', 'D'... */
};  /* dasd_init_0 */

struct dasd_check_0 {                      /* Used by NetDASDCheck  */
    unsigned char LSFAR * LSPTR dc0_path;  /* Determining path      */
    unsigned long               dc0_avail; /* Available space in KB */
    unsigned long               dc0_usage; /* Directory size in KB  */
};  /* dasd_check_0 */

/* On return, the dc0_path field contains the directory path
 * at which the dc0_avail value space was calculated.
 */

typedef struct dasd_info_0  DASDINFO0;
typedef DASDINFO0 * PDASDINFO0;

typedef struct dasd_init_0  DASDINIT0;
typedef DASDINIT0 * PDASDINIT0;

typedef struct dasd_check_0 DASDCHECK0;
typedef DASDCHECK0 * PDASDCHECK0;



#ifdef PURE_32
/****************************************************************
 *                                                              *
 *              Function prototypes - API DASD                  *
 *                  for "pure" 32-bit code                      *
 *                                                              *
 ****************************************************************/

extern  API32_FUNCTION
  Net32DASDAdd( const unsigned char  * pszServer,
                unsigned long          uLevel,        /* was signed short */
                unsigned char        * pbBuffer,
                unsigned long          ulBuffer       /* was USHORT */
              );

extern  API32_FUNCTION
  Net32DASDDel( const unsigned char  * pszServer,
                unsigned char        * pszResource
              );

extern  API32_FUNCTION
  Net32DASDGetInfo( const unsigned char   * pszServer,
                    unsigned char         * pszResource,
                    unsigned long           ulLevel,    /* was signed short */
                    unsigned char         * pbBuffer,
                    unsigned long           ulBuffer,   /* was USHORT  */
                    unsigned long         * pulTotal    /* was USHORT* */
                  );


extern  API32_FUNCTION
  Net32DASDSetInfo( const unsigned char  * pszServer,
                    unsigned char        * pszResource,
                    unsigned long          ulLevel,      /* was signed short */
                    unsigned char        * pbBuffer,
                    unsigned long          ulBuffer      /* was USHORT */
                  );


extern  API32_FUNCTION
  Net32DASDEnum( const unsigned char    * pszServer,
                 unsigned char          * pszPath,
                 unsigned long            ulRecursive,   /* was signed short */
                 unsigned long            ulLevel,       /* was signed short */
                 unsigned char          * pbBuffer,
                 unsigned long            ulBuffer,      /* was USHORT */
                 unsigned long          * pulEntriesReturned,  /* was USHORT* */
                 unsigned long          * pulTotalEntries      /* was USHORT* */
               );


extern  API32_FUNCTION
  Net32DASDCheck( const unsigned char   * pszServer,
                  unsigned char         * pszPath,
                  unsigned long           ulLevel,        /* was signed short */
                  unsigned char         * pbBuffer,
                  unsigned long           ulBuffer,       /* was USHORT  */
                  unsigned long         * pulTotalAvail   /* was USHORT* */
                );

extern  API32_FUNCTION
  Net32DASDCtl(  const unsigned char  * pszServer,
                 unsigned long          ulLevel,         /* was signed short*/
                 unsigned char        * pbBuffer,
                 unsigned long          ulBuffer         /* was USHORT*/
               );
#endif /* PURE_32 */


             /****************************
              *     MANIFESTS            *
              ****************************/

/*  Used with NetDASDCtl() - values for i0_CtlFlag                  */

#define DASD_CTL_INSTALL        1       /* Install DASD_LIMITS      */
#define DASD_CTL_REMOVE         2       /* Remove DASD_LIMITS       */
#define DASD_CTL_RECALC         3       /* recalculate usage        */


/*
 * Bit masks used with d0_flag
 */

#define DASD_VALIDATE_LIMIT_ON  0x00    /* Default--set if tree size < limit */
#define DASD_VALIDATE_LIMIT_OFF 0x01    /* Set limit regardless of tree size */

#pragma pack()
#endif /* NETDASD_INCLUDED */
