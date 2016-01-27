/*********************************************************************
*
* Module Name: vsqm.h
*
* OS/2 public header file.
*
* Copyright (c) 1993 IBM Corporation
*
*                                                                    */
*   Description     = Visual Spool Queue Manager Header for related  */
*                     constants. Note that there is a strong         */
*                     relationship between the values in this file   */
*                     and the Visual Spool Queue Manager resource    */
*                     file.                                          */
*                                                                    */
*   Function        = Contains all the IDs for the Visual Spool      */
*                     Queue Manager application.                     */
*                                                                    */
*                                                                    */
*                                                                    */
**********************************************************************/

/**********************************************************************/
/*                                                                    */
/* String table IDs                                                   */
/*                                                                    */
/**********************************************************************/
#define IDSNAME          10
#define IDSTITLE         20
#define IDSHELP          21    /* Help window title string
#define IDSDTLSTL        30
#define IDSQUEUE1        40
#define IDSQUEUE3        42
#define IDSQUEUE4        43
#define IDSJOB1          50
#define IDSJOB3          52
#define IDSJOB4          53
#define IDSJOB5          54
#define IDSJOB6          55
#define IDSJOB7          56
#define IDSJOB8          57
#define IDSJOB9          58
#define IDSJOB10         59
#define IDSJOB11         60
#define IDSJOB12         61
#define IDSJOB13         62
#define IDSJOB14         63
#define IDSJOB15         64
#define IDSPRIOSTR       69
#define IDSJOB16         70
#define VQCONFIRMCANCEL  71
#define VQCANNOTIPL      72
#define VQSPLRUNNING     73
#define VQSPLNOMEMORY    74
#define VQERRNOMEMORY    75
#define VQCONFIRMZAPQ    76
#define VQINIFILEERROR   77
#define VQSPLNODISKSPACE 78
#define VQNOFREEJOBID    79
#define VQINVALIDPRIORITY 80
#define VQBASEERROR      81
#define VQINTERNALERROR  82
#define VQDRIVENOTREADY  83
#define VQBADPRIOSTR     84

#define ID_LISTBOX       90     /* Queues listbox ID

/**********************************************************************/
/*                                                                    */
/* Menu IDs                                                           */
/*                                                                    */
/**********************************************************************/
#define VSQMID          170
#define VQQUEUE         171
#define IDMHOLDQ        172
#define IDMRELQ         173
#define IDMHOLDALL      174
#define IDMRELALL       175
#define IDMZAPALL       176
#define VQJOBS          180
#define IDMPRTNEXT      181
#define IDMCANCEL       182
#define IDMRST          183
#define IDMHOLDJ        184
#define IDMRLS          185
#define IDMRPT          186
#define IDMDTLS         187
#define IDMHELP         189
#define IDMHOWTOGETHELP 199
#define VQSPECIAL       190
#define VQSPREFRESH     191
#define VQSPAUTOONOFF   192



/**********************************************************************/
/*                                                                    */
/* Dialog box IDs                                                     */
/*                                                                    */
/**********************************************************************/
#define JOBINFO          1000          /* Dialog template ID         */

#define DEVICENMINFO     1001          /* Device information         */
#define DEVICENMTEXT     1002

#define FORMSCODEINFO    1003          /* Forms code information     */
#define FORMSCODETEXT    1004

#define JOBIDINFO        1005          /* Job ID string              */
#define JOBIDTEXT        1006

#define PRIORITYINFO     1007          /* Job priority information   */
#define PRIORITYTEXT     1008

#define PROCNMINFO       1009          /* Queue processor name string*/
#define PROCNMTEXT       1010

#define PROCPARAMSINFO   1011          /* QProc parameters string    */
#define PROCPARAMSTEXT   1012

#define DOCNAMETEXT      1013          /* Document Name              */
#define DOCNAMEINFO      1014

#define COMMENTTEXT      1015          /* Document comment string    */
#define COMMENTINFO      1016

#define NETTEXT          1017          /* Network parameters         */
#define NETS             1018

#define DATETEXT         1019          /* Date of creation           */
#define DATE             1020

#define TIMETEXT         1021          /* Time of creation           */
#define TIME             1022

#define OK               1023          /* OK and                     */
#define HELP             1024          /* Help push buttons          */

#define DOCNAMEINFOLFT   2001
#define DOCNAMEINFORT    2002
#define COMMENTINFOLFT   2003
#define COMMENTINFORT    2004
#define PROCPARAMSLFT    2005
#define PROCPARAMSRT     2006
#define NETLEFT          2007
#define NETRIGHT         2008
/**********************************************************************/
/*                                                                    */
/*  Help ID's                                                         */
/*                                                                    */
/**********************************************************************/
#define IDH_VQQUEUE        400
#define IDH_IDMHOLDQ       401
#define IDH_IDMRELQ        402
#define IDH_VQJOBS         403
#define IDH_IDMDTLS        404
#define IDH_IDMCANCEL      405
#define IDH_IDMPRTNEXT     406
#define IDH_IDMRPT         407
#define IDH_IDMRST         408
#define IDH_IDMHOLD        409
#define IDH_IDMRLS         410
#define IDH_JOBINFO        411
#define IDH_VQSPECIAL      412
#define IDH_VQSPREFRESH    413
#define IDH_VQSPAUTO       414
#define IDH_NOMEMORY       415
#define IDH_NODISKSPACE    416
#define IDH_INTERNALERROR  417
#define IDH_INIFILEERROR   418
#define IDH_HELP           419
#define IDH_IDMHOLDALL     420
#define IDH_IDMRLSALL   :‡ 421
#define IDH_IDMCANALL      422
#define IDH_IDMSYSMENU     423
#define IDH_IDMSMREST      424
#define IDH_IDMSMMOVE      425
#define IDH_IDMSMSIZE      426
#define IDH_IDMSMMIN       427
#define IDH_IDMSMMAX       428
#define IDH_IDMSMTMG       429
#define IDH_MAIN           430                                /* @D1A */
#define IDH_KEYS           431                                /* @D1A */
#define IDH_HOWTOGETHELP   432                                /* @D1A */

/**********************************************************************/
/*                                                                    */
/* Menu resource                                                      */
/*                                                                    */
/**********************************************************************/
#define QMMENU 1

/**********************************************************************/
/*                                                                    */
/* Bitmap resources                                                   */
/*                                                                    */
/**********************************************************************/
#define LEFT 2
#define RIGHT 2
/**********************************************************************/
/*                                                                    */
/* Also needed but not sure why                                       */
/*                                                                    */
/**********************************************************************/
#define IDH_QMCONTEXT 2


/**********************************************************************/
/*                                                                    */
/* Define the sizes of strings that get displayed in the QUEUES/JOBS  */
/* listbox. Note that string lengths include 1 character for the null */
/* terminator. In other words, for a full string, the actual length   */
/* of the string returned by a call to strlen() will be one less than */
/* the length value.                                                  */
/*                                                                    */
/**********************************************************************/
#define VQMAXLBSTRING  78 /* Maximum length of list box string        */
#define VQMAXJOBSTRLEN 20 /* Maximum total space for job(s) string    */

#define VQMAXJBFILLER   3 /* Maximum size of job filler string        */
#define VQQMAPIMAGELEN  4 /* Size of image elements in Queues map     */
#define VQJMAPIMAGELEN  4 /* Size of image elements in Jobs map       */
#define VQMAXJOBNOLEN   4 /* Maximum length of job number displayable */

#define VQMAXJIDLEN     4 /* Maximum length of job ID string          */
#define VQMAXCOMMENT    8 /* Maximum length of the comment string     */
#define VQMAXDOCNAME   14 /* Maximum length of the doc name string    */
#define VQMAXTIMEST    20 /* Maximum length of job timestamp string   */
#define VQMAXSTSTRING  17 /* Maximum length of job status string      */
#define VQMAXNETPSTRING 9 /* Maximum length of network params string  */

#define VQMAXPRIOSTRLEN 9 /* Maximum total space for job prio string  */
#define VQMAXQFILLER   21 /* Maximum size of queue filler string      */

/**********************************************************************/
/*                                                                    */
/* Absolute positions of items in the VSQM list box                   */
/*                                                                    */
/**********************************************************************/
#define VQQNAMESTART    0 /* Start position of the queue name string  */
#define VQQNAMESTOP    26 /* End position of the queue name string    */

#define VQJIDSTART      1
#define VQJIDSTOP       4

#define VQCOMMSTART     6 /* Start position of the comment string     */
#define VQCOMMSTOP     13 /* End position of the comment string       */

#define VQDOCNSTART    15 /* Start position of the doc name string    */
#define VQDOCNSTOP     28 /* End position of the doc name string      */

#define VQDATESTOP     38

#define VQTIMESTART    40 /* Start position of timestamp string       */
#define VQTIMESTOP     47 /* End position of timestamp string         */

#define VQSTATUSPOS    49 /* Start position of the status string      */

#define VQNETPSTART    68 /* Start position of network params string  */
#define VQNETPSTOP     76 /* End position of network params string    */

/**********************************************************************/
/*                                                                    */
/* Miscellaneous others.                                              */
/*                                                                    */
/**********************************************************************/
#define NUMQMENUITEMS   5 /* Number of items in the Queue menu        */
#define NUMJMENUITEMS   7 /* Number of items in the Job submenu       */
#define VQMAXTITLE     50 /* Size of title bar string                 */
#define VQMAXMSGSTRING 256/* Maximum msg box string length displayable*/

#define VSQM_MAXQNAME  sizeof(SPLUNCQUNAME)
/**********************************************************************/
/*                                                                    */
/* Define the possible values to be used when calling DosExit.        */
/*                                                                    */
/**********************************************************************/
#define VQ_KILL_PROCESS 1

#define    IDSQUEUE2  "                "
#define    IDSJOB2    "                "
#define    IDSJOBFILLER "  "
#define    IDSQUEUEFILLER "                         "
#define    IDSJBSTR  "    0 "

/***************************************************************/
/* Help SubTable and Table resource IDs                        */
/***************************************************************/
#define MAIN_SUBTABLE  1
#define DUMMY_SUBTABLE 2
#define MAIN_HELPTABLE 1
