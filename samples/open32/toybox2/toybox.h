/*************************************************************************
* ToyBox Demo Program h include file.
*
* 08/08/95 - Original Version, Dan Mendrala, IBM Corporation, 1995.
*
*************************************************************************/


/*************************************************************************
 * Just the basic include files.
 ************************************************************************/

#include <os2win.h>
#include <stdio.h>
#include "toybox.hhh"

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
