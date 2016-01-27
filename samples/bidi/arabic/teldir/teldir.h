/* --------------------------- */
/*    teldir - header file     */
/* --------------------------- */

/* Identifiers Predeclarations  */

#define DID_OK              1
#define DID_CANCEL          2
#define ID_ABOUT            3
#define MAX_NAMES           100            /* maximum number of entries in */
                                           /* the telephone directory      */
#define IDM_ORGMENU         9000
#define IDM_FILE            9001
#define IDM_VIEW            9002
#define IDM_HELP            9003
#define IDM_SAVE            9006
#define IDM_QUIT            9007
#define IDM_ABOUT           9008
#define IDM_ASCENDING       9009
#define IDM_DESCENDING      9010
#define IDM_OK              9011
#define IDM_ARB             9012
#define IDM_ENG             9013
#define IDM_ENGLISHMENU     9014
#define IDM_LANG            9015
//Start_Help
#define IDM_HELPEXTENDED    9016
//End_Help

#define ID_NAMES_LIST       2000
#define ID_LISTBOX          2001
#define ID_CHOICES          2002
#define ID_ADDNAME          2003
#define ID_DELETE           2004
#define ID_UPDATE           2005
#define ID_SEARCH           2006
#define ID_NAME             2007
#define ID_TELEPHONE        2008
#define ID_INPUT_NAME       2009
#define ID_TELEPHONE_NUMBER 2010
#define ID_ENTER            200

#define ID_TELDIR           2
#define ID_CLOCK            3
#define ID_TIMER            100

// IDs for Arabic Strings
#define ID_TITLE_A            500
#define ID_NAMES_A            501
#define ID_ADD_A              502
#define ID_REMOVE_A           503
#define ID_MODIFY_A           504
#define ID_FIND_A             505
#define ID_CHOICE_A           506
#define ID_ENTERNAME_A        507
#define ID_TELNUMBER_A        508
#define ID_DO_A               509
#define ID_SAVEFILE_A         510
#define ID_EXIT_A             511
#define ID_USES_A             512
#define ID_ERROR_A            513
#define ID_HELP_A             514
#define ID_ERRINADD_A         515
#define ID_ERRINMODIFY_A      516
#define ID_ERRINSEARCH_A      517
#define ID_ERRINREMOVE_A      518
#define ID_INFORMATION_A      519
#define ID_CANNOTLOADSTRING_A 520
#define ID_HELPLOADERROR_A    521
#define ID_HELPDISPLAYERROR_A 522
#define ID_HELPLIBRARYNAME_A  523
#define ID_HELPWINDOWTITLE_A  524

// IDs for English Strings (offset 100 from English)
#define ID_TITLE              600
#define ID_NAMES              601
#define ID_ADD                602
#define ID_REMOVE             603
#define ID_MODIFY             604
#define ID_FIND               605
#define ID_CHOICE             606
#define ID_ENTERNAME          607
#define ID_TELNUMBER          608
#define ID_DO                 609
#define ID_SAVEFILE           610
#define ID_EXIT               611
#define ID_USES               612
#define ID_ERROR              613
#define ID_HELP               614
#define ID_ERRINADD           615
#define ID_ERRINMODIFY        616
#define ID_ERRINSEARCH        617
#define ID_ERRINREMOVE        618
#define ID_INFORMATION        619
#define ID_CANNOTLOADSTRING   620
#define ID_HELPLOADERROR      621
#define ID_HELPDISPLAYERROR   622
#define ID_HELPLIBRARYNAME    623
#define ID_HELPWINDOWTITLE    624

//Start_Help
#define HELPLIBRARYNAMELEN  20

/*--------------------------------------------------------------*\
 *  Help table and subtables                                    *
\*--------------------------------------------------------------*/
#define TELDIR_HELP_TABLE                   1000

/*--------------------------------------------------------------*\
 *  Main window help panels                                     *
\*--------------------------------------------------------------*/
#define SUBTABLE_MAIN                       2000
#define PANEL_IDM_ABOUT                     IDM_ABOUT
#define PANEL_IDM_FILE                      IDM_FILE
#define PANEL_IDM_VIEW                      IDM_VIEW
#define PANEL_IDM_HELP                      IDM_HELP
//End_Help


/* Function Pre declarations  */

int  main(VOID);
MRESULT EXPENTRY WinProc1(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2);
VOID  MessageBox(USHORT sMsg, USHORT sTitle);
VOID  Button_Handling(VOID);
VOID  SaveData(VOID);
VOID  LoadFile(VOID);
VOID  ChangeLayer(VOID);
VOID  Arb_Interface(VOID);
VOID  Eng_Interface(VOID);
PCHAR Message(USHORT);
VOID ConvertBidiString (PUCHAR, PUCHAR, ULONG, ULONG, ULONG);
VOID  DisplayTitle(USHORT);
//Start_Help
VOID InitHelp(VOID);
VOID DisplayHelpPanel(LONG);
VOID DestroyHelpInstance(VOID);
//End_Help

/* type definitions */

typedef struct     {
     char Name[100];
     char Number[10];
   } TELLIST;

typedef TELLIST *PTELLIST;
