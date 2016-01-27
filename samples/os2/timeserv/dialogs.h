/*static char *SCCSID = "@(#)dialogs.h	6.3 92/02/18";*/
/**************************************************************************\
*                                                                          *
*       MODULE:     dialogs.h                                              *
*                                                                          *
*       COMMENT:    Constants for the Clock dialogs                        *
*                                                                          *
\**************************************************************************/


#define CLKTM_MAJOR 0x0100
#define CLKTM_MINOR 0x0200

#define CLKTM_ALWAYS    0x0001
#define CLKTM_NOTICONIC 0x0002
#define CLKTM_NEVER     0x0004

#define CLKCLR_BUTTONSHIFT 0x08

#define CLKCLR_OBJECTS      0x0100
#define CLKCLR_BACKGROUND   0x0101
#define CLKCLR_FACE         0x0102
#define CLKCLR_HOURHAND     0x0103
#define CLKCLR_MINUTEHAND   0x0104

#define CCHMAXSTRING                    256
#define TIME_SEP_LEN        1
#define DATE_SEP_LEN        1
#define STRING_AM_LEN       2
#define STRING_PM_LEN       2
#define AM_PM  2                    /* index into rTimeSep */
#define PATHMAX 259           /* path cb     max                     */
#define COUNTRY_USA     1              /* Country code               */
#define COUNTRY_UK      44             /* Country code               */
