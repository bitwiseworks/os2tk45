// include file for hiworld.c


// menu item for screen dimensions selection
#define IDM_SCRDIM			101

// menu item for beep dialog selection
#define IDM_BEEP			103

// menu items for time selections			
#define IDM_UPTIME			201
#define IDM_LOCALTIME		202
#define IDM_SYSTIME			203

// menu items for color selections
#define IDM_SHOWCOLORS		301
#define IDM_CHANGECOLORS	302
#define IDM_RESTORECOLORS 	303

// menu item for exit selection
#define IDM_EXIT			401

// identifiers for frequency and duration bars in beep dialog
#define ID_FREQBAR			501
#define ID_DURBAR			502

// identifiers for beep and done buttons in beep dialog
#define ID_BEEPNOW			601
#define ID_DONE				602

// constants - max and min frequencies and durations for beeping
#define MAXFREQ 500
#define MINFREQ 10
#define MAXDURATION 500
#define MINDURATION 10

// step values for beep dialog sliders
#define FREQSTEP 10
#define DURATIONSTEP 10

// width of beep dialog sliders
#define BEEPSCROLLWIDTH 50

// defines for icon, menu, dialog and cursor IDs
#define IDI_ICON2                       102
#define HIWORLDMENU						103
#define BEEPDIALOG						104
#define IDC_CURSOR1                     105
