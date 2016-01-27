/*************************************************************************
* ToyBox Demo Program h include file.
*
* 08/08/95 - Original Version, Dan Mendrala, IBM Corporation, 1995.
*
*************************************************************************/


/*************************************************************************
 * Just the basic include files.
 ************************************************************************/

#include <windows.h>
#include <stdio.h>

/*************************************************************************
 * These are some limits that we set in the program.
 ************************************************************************/

#define MAX_OBJ                 100     /* Max balls active             */
#define MAX_OBJ_TYPE            6       /* MUST be EVEN # of types      */
#define MAX_OBJ_FRAME           6       /* # of frames per type         */

/*************************************************************************
 * Some constants that we need set.
 ************************************************************************/

#define BIT_NONOBJ              0               /* status for unused    */
#define BIT_ACTOBJ              1               /* active object (=)    */
#define BIT_NEWOBJ              2               /* Status for NEW       */
#define BIT_DELOBJ              4               /* Status for Delete obj*/


/*************************************************************************
 * These are all of the defines used by the resource file menu items.
 ************************************************************************/

#define IDM_START                                                                1
#define IDM_STOP                                                                 2

#define IDM_ADDOBJ                                                               10
#define IDM_DELOBJ                                                               11
#define IDM_MAXOBJ                                                               12
#define IDM_MINOBJ                                                               13
#define IDM_STEP                                                                 14

#define IDM_ABOUT                                                                97
#define IDM_QUIT                                                                 98
#define IDM_HELP                                                                 99

/*************************************************************************
 * These are the defines used by the bitmaps, icons and menus
 * used in the resource file.
 ************************************************************************/

#define BallR1                                   0x1001
#define BallR2                                   0x1002
#define BallR3                                   0x1003
#define BallR4                                   0x1004
#define BallR5                                   0x1005
#define BallR6                                   0x1006
#define BallB1                                   0x1007
#define BallB2                                   0x1008
#define BallB3                                   0x1009
#define BallB4                                   0x100A
#define BallY1                                   0x100B
#define BallY2                                   0x100C
#define BallY3                                   0x100D
#define BallY4                                   0x100E
#define BallY5                                   0x100F
#define BallY6                                   0x1010
#define BallG1                                   0x1011
#define BallG2                                   0x1012
#define BallG3                                   0x1013
#define BallG4                                   0x1014
#define BallG5                                   0x1015
#define BallG6                                   0x1016
#define BallL1                                   0x1017
#define BallL2                                   0x1018
#define BallL3                                   0x1019
#define BallL4                                   0x101A
#define BallP1                                   0x101B
#define BallP2                                   0x101C
#define BallP3                                   0x101D
#define BallP4                                   0x101E

#define ToyIcon                                  0x1100
#define ToyMenu                                  0x1101

