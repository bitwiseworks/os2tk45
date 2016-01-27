/*static char *SCCSID = "@(#)clkdata.h	6.3 92/02/18";*/
/*external declarations of global data*/
extern HWND hwndFrame ;
extern HWND hwndTitleBar ;
extern HWND hwndSysMenu ;
extern HWND hwndMinMax ;
extern HWND hwndMenu ;
extern HAB hab ;
extern HMQ hmq ;
extern HSWITCH hsw ;
extern BOOL fStartAsIcon ;
extern CLOCKPREF cp;
extern TIMER_RESOURCES TimerResources;
extern BOOL fEndThread;
extern BOOL fHelpEnabled;



extern SHORT      Selected ;
extern SHORT      PreviousSelected;

extern BOOL     bLeadingZero;
extern BOOL     bIsIconic;


extern WRECT rDateTime[6];
extern WRECT rAlarmTime[3];      /* placement of time and date matters */
extern WRECT rRepaintTime;
extern WRECT rDateAssign[3];
extern SHORT sFirstTime;
extern SHORT sFirstDate;
extern SHORT asLeftOf[6];
extern SHORT asRightOf[6];
extern SHORT wDateTime[6];
extern SHORT wModulos[3] ;
extern SHORT wPrevDateTime[6];

extern HWND  hDate,hTime;
extern HWND  hDateScroll;
extern HWND  hTimeScroll;
extern HWND  hCurrentFocus;
extern HWND  hDateTime;
extern HWND  hAlarmTime;
extern HWND  hwndHelpInstance;

extern BOOL bTwelveHourFormat;
extern WRECT rTimeSep[3];
extern CHAR  szTimeSep[TIME_SEP_LEN+1];
extern CHAR  szAnteMeridian[STRING_AM_LEN+1];
extern CHAR  szPostMeridian[STRING_PM_LEN+1];
extern CHAR  szTitle   [81];
extern CHAR  szErrClocks[PATHMAX+1];
extern WRECT rDateSep[2];
extern CHAR  szDateSep[DATE_SEP_LEN+1];
extern BOOL fTimeFreez ;
extern BOOL fDateFreez ;
extern USHORT vusDateFormat;
extern MATRIXLF vmatlfDateTrans;
extern MATRIXLF vmatlfDateScale;
extern SWP swp ;
extern HPS hps ;
extern HDC hdc ;
extern RECTL rclPage ;
extern DATETIME dt ;
extern BOOL f ;
extern BOOL fIconic , fShowSecondHand ;
extern SIZEL sizl;
extern LONG cxRes , cyRes ;
extern char achOldTime[9];
extern char achAmPm[3];
extern char achOldAmPm[3];
extern char achOldDate[9];
extern HPS hpsWnd;
extern LONG cxScreen, cyScreen;  /* screen dimensions */
extern LONG vclrBG[3];
extern LONG vclrFace[3];
extern LONG vclrRing[3];
extern LONG vclrHands[3];
extern LONG vclrMajorTicks[3];
extern LONG vclrMinorTicks[3];
extern LONG vclrDate[4];
extern USHORT vusDatePos;
extern BOOL vbInAlarm;
extern PFNWP pfnwpEntryField;
extern HWND hwndEntryField;

extern HWND hwndClient;
extern HPS hpsBuffer;
extern HDC hdcBuffer;
extern BOOL fBufferDirty;
extern LONG cColorPlanes, cColorBitcount;

/*
 *prototypes
 *used by the various modules
 */
extern VOID ClkDrawDigitalString (HPS, char *, RECTL *, LONG, LONG);
extern VOID DrawClock (HPS);
extern VOID UpdateScreen (HPS hps, RECTL *prclUpdate);
extern VOID AlarmSetTimer(UCHAR uchHour,UCHAR uchMinutes);
MRESULT EXPENTRY AboutBoxDlgProc( HWND hwnd, ULONG usMsg,
                                     MPARAM mp1,
                                     MPARAM mp2 );

extern VOID ClkDrawFace (HPS hpSpace);
extern VOID ClkDrawHand (HPS hpSpace,SHORT sId, SHORT sMessage);

