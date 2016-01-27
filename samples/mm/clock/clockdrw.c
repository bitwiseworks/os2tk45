/*static char *SCCSID = "@(#)clockdrw.c	13.6 92/04/16";*/
/*************************************************************************
 * Name        : Clockdrw.c
 *
 * Description : This file contains the C source code required for the
 *               graphics processing in the clock sample program.
 *
 * MMPM/2 API's: None
 *************************************************************************/
#define  INCL_OS2MM                      /* Required for MCI & MMIO headers   */
#define  INCL_WIN                        /* Required to use Win APIs.         */
#define  INCL_PM                         /* Required to use PM APIs.          */
#define  INCL_GPIPRIMATIVES              /* Required to use GPI OS/2 APIs.    */
#define  INCL_WINHELP                    /* Required to use IPF.              */

#include <os2.h>
#include <os2me.h>
#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
/************************* Procedure/Function Prototypes **********************/

VOID             PlaceTheTimeMarkers( HPS     hps,
                                      PPOINTL pptlMarkerPosition,
                                      USHORT  usClockNumeral );
VOID             DrawClockCircles( HPS     hps,
                                   PPOINTL pptlMarkerPosition,
                                   LONG    lCircleColor,
                                   USHORT  usCircleSize );
/****************************** Local Variables. ******************************/

POINTL  aptlMarkerPosition[ NUMBER_OF_MINUTE_MARKERS ] =
   {
      {   0,  131 }, {   13,  130 }, {   27,  128 }, {   40,  124 },
      {  53,  119 }, {   65,  113 }, {   76,  105 }, {   87,  97  },
      {  97,   87 }, {  105,   76 }, {  113,   65 }, {  119,  53  },
      { 124,   40 }, {  128,   27 }, {  130,   13 }, {  131,   0  },
      { 130,  -13 }, {  128,  -27 }, {  124,  -40 }, {  119, -53  },
      { 113,  -65 }, {  105,  -76 }, {   97,  -87 }, {   87, -97  },
      {  76, -105 }, {   65, -113 }, {   53, -119 }, {   40, -124 },
      {  27, -128 }, {   13, -130 }, {    0, -131 }, {  -13, -130 },
      { -27, -128 }, {  -40, -124 }, {  -53, -119 }, {  -65, -113 },
      { -76,  105 }, {  -87,  -97 }, {  -97,  -87 }, { -105,  -76 },
      {-113,  -65 }, { -119,  -53 }, { -124,  -40 }, { -128,  -27 },
      {-130,  -13 }, { -130,    0 }, { -130,   13 }, { -128,   27 },
      {-124,   40 }, { -119,   53 }, { -113,   65 }, {  105,   76 },
      { -97,   87 }, {  -87,   97 }, {  -76,  105 }, {  -65,  113 },
      { -53,  119 }, {  -40,  124 }, {  -27,  128 }, {  -13,  129 },
   };

POINTL aptlHandsPosition[ NUMBER_OF_MINUTE_MARKERS ] =
   {
       {  0,  111 }, {   11,  110 }, {   23,  108 }, {   34,  105 },
       { 45  ,101 }, {   55,   96 }, {   65,   90 }, {   74,   82 },
       { 82,   74 }, {   90,   65 }, {   96,   55 }, {  101,   45 },
       { 105,  34 }, {  108,   23 }, {  110,   11 }, {  111,    0 },
       { 110, -11 }, {  108,  -23 }, {  105,  -34 }, {  101,  -45 },
       { 96,  -55 }, {   90,  -65 }, {   82,  -74 }, {   74,  -82 },
       { 65,  -90 }, {   55,  -96 }, {   45, -101 }, {   34, -105 },
       { 23, -108 }, {   11, -110 }, {    0, -111 }, {  -11, -110 },
       { -23,-108 }, {  -34, -105 }, {  -45, -101 }, {  -55,  -96 },
       { -65, -90 }, {  -74,  -82 }, {  -82,  -74 }, {  -90,  -65 },
       { -96, -55 }, { -101,  -45 }, { -105,  -34 }, { -108,  -23 },
       { -110,-11 }, { -111,    0 }, { -110,   11 }, { -108,   23 },
       { -105, 34 }, { -101,   45 }, {  -96,   55 }, {  -90,   65 },
       { -82,  74 }, {  -74,   82 }, {  -65,   90 }, {  -55,   96 },
       { -45, 101 }, {  -34,  105 }, {  -23,  108 }, {  -11  ,110 }
   };

/*************************************************************************
 * Name        : DrawTheClockFace
 *
 * Description : This procedure will draw the clock face. Instead of
 *               numerals on the clock face, it will have lines in the place
 *               of numerals (called  time markers or sticks).
 *                  Set up the required values to draw a circle.
 *                  Move to the center of the dialog window.
 *                  Then place the sticks.
  *
 *               STICKS - Hour and Minute positions.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hps                - The presentation space in which
 *                                    to draw.
 *               pptlCircleCenter   - The coordinates of the window center.
 *
 * Return      : None.
 *
 *************************************************************************/
VOID  DrawTheClockFace( HPS     hps,
                        PPOINTL pptlCircleCenter )
{
   ARCPARAMS apsArcParameters;       /* Parameters used to form circle.       */
   POINTL    ptrMarkerPosition;      /* Marker position.                      */
   USHORT    usClockNumeral;         /* Clock face numeral.                   */

   /*
    * Set the arc parameters so that a circle will be drawn when the
    * GpiFullArc API is called.
    * p - scales in the X direction.
    * q - scales in the Y direction.
    * r - sheared ellipse or arc.
    * s - sheared ellipse or arc.
    */

   apsArcParameters.lP = (LONG) 1;
   apsArcParameters.lQ = (LONG) 1;
   apsArcParameters.lR = (LONG) 0;
   apsArcParameters.lS = (LONG) 0;

   GpiSetArcParams( hps,                   /* Presentation Space.             */
                    &apsArcParameters );   /* Arc parameters.                 */

   /*
    * Move to the center of the window so that the Clock Circle can be
    * drawn.
    */
   GpiMove( hps,                  /* Presentation Space.                      */
            pptlCircleCenter );   /* Center point in the window.              */

   /*
    * Place the time markers on the clock face.
    */
   for(usClockNumeral=1; usClockNumeral<13; usClockNumeral++)
   {
      ptrMarkerPosition.x = pptlCircleCenter->x;
      ptrMarkerPosition.y = pptlCircleCenter->y;

      PlaceTheTimeMarkers(
         hps,                       /* Presentation Space.                    */
         &ptrMarkerPosition,        /* Structure that will hold the position. */
         usClockNumeral );          /* Which numeral we are dealing with.     */

   }  /* End of Clock Numeral For Loop. */

} /* End of DrawTheClockFace */


/*************************************************************************
 * Name        : DrawTheHourHand
 *
 * Description : This procedure takes the given presentation space and
 *               draws the hour hand on the face of the clock.  In drawing
 *               the new hour's hand on the clock face the old hour hand
 *               has to be erased first.  After the old hour hand is erased
 *               the new hour hand position can be drawn.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hps              - The presentation space to in which
 *                                  to draw.
 *               pptlCircleCenter - The coordinates of the window center.
 *               usHour           - The hour to use to show the correct hand.
 *               usMinute         - The minute to use to place the hour hand.
 *
 * Return      : None.
 *
 *************************************************************************/
VOID EXPENTRY DrawTheHourHand( HPS     hps,
                      PPOINTL pptlCircleCenter,
                      USHORT  usHour,
                      USHORT  usMinute)
{
   POINTL ptlHandPosition;       /* Hour hand position.                       */
   USHORT usTempHour;            /* Temporary value for the hour.             */

   extern USHORT usLastHour;

   /*
    * Place the 'real' hour into a temporary variable that can be
    * changed as desired.
    */
   usTempHour = usHour % (USHORT) 12;

   /*
    * Move to the Center of the Clock Face.
    */
   GpiMove(
      hps,                 /* Presentation Space.                             */
      pptlCircleCenter );  /* Center point of the circle.                     */

   /*
    * Take the center of the circle as a starting point and initalize the
    * variable that is used to draw the hour hand.
    */
   ptlHandPosition.x = pptlCircleCenter->x;
   ptlHandPosition.y = pptlCircleCenter->y;

   /*
    * Check to make sure that the hour value that was passed into the
    * procedure is within bounds.  If it is not, the hour hand will NOT
    * be updated.
    */
   if ( usTempHour <= 12 )
   {
      /*
       * See if the usHour is at the top of the hour.  If it is, make
       * the variable look at the first cell in the array, i.e., the zero
       * cell.
       */
      if ( usTempHour == 12 )
      {
         usTempHour = 0;
      }

      /*
       * Erase the OLD hour.
       * - Set the draw color to the background color.
       * - Set the position for the minute hand.
       * - Draw the hour hand to erase the old hour.
       * BUT don't erase the hour hand if it has not been drawn before, i.e.,
       * this is the first time into this routine after startup.
       */
      if ( usLastHour == 99 )
      {
         GpiSetColor(
            hps,                    /* Presentation Space.                    */
            SYSCLR_FIELDBACKGROUND);/* Background color to erase the old hour.*/

         ptlHandPosition.x +=
            (aptlHandsPosition[
               (usLastHour * HOUR_HAND_INDEX) + (usMinute / 12) ].x / 2 );
         ptlHandPosition.y +=
            (aptlHandsPosition[
               (usLastHour * HOUR_HAND_INDEX) + (usMinute / 12) ].y / 2  );

         GpiLine(
            hps,                    /* Presentation Space.                    */
            &ptlHandPosition );     /* Center point of the circle.            */

         DrawClockCircles(
            hps,                    /* Handle to the Presentation space.      */
            &ptlHandPosition,       /* Position to place the Marker.          */
            SYSCLR_FIELDBACKGROUND, /* Color to make the end dot.             */
            (USHORT) HAND_DOT );    /* Size of the time marker circle.        */
    }

      /*
       * Draw the NEW hour hand.
       * - Reset the center values into the position points.
       * - Calculate the new hour hand position.
       * - Reset the color.
       * - Move to the center of the clock.
       * - Draw the new hour hand.
       */
      ptlHandPosition.x = pptlCircleCenter->x;
      ptlHandPosition.y = pptlCircleCenter->y;

      ptlHandPosition.x +=
         (aptlHandsPosition[
            (usTempHour * HOUR_HAND_INDEX) + (usMinute / 12) ].x / 2 );
      ptlHandPosition.y +=
         (aptlHandsPosition[
            (usTempHour * HOUR_HAND_INDEX) + (usMinute / 12) ].y / 2 );

     GpiSetColor(
        hps,                 /* Presentation Space.                          */
        CLOCK_HAND_COLOR  ); /* Color to use for the clock.                  */

     GpiMove(
        hps,                 /* Presentation Space.                          */
        pptlCircleCenter );  /* Center point of the circle.                  */

     GpiLine(
        hps,                 /* Presentation Space.                          */
        &ptlHandPosition );  /* End of the clock hand.                       */

     DrawClockCircles(
        hps,                 /* Handle to the Presentation space.            */
        &ptlHandPosition,    /* Position to place the Marker.                */
        CLR_BLUE,            /* Color to make the end dot.                   */
        (USHORT) HAND_DOT ); /* Size of the time marker circle.              */

     /*
      * Since the hour value passed into the routine has been used, make
      * this hour the Last Hour that was updated.  This allows the last
      * hour to be removed from the clock face before the new hour is
      * placed on the clock.
      */
     usLastHour = usHour;

  }  /* End of IF minute is within bounds. */

} /* End of DrawTheHourHand */


/*************************************************************************
 * Name        : DrawTheMinuteHand
 *
 * Description : This procedure will take the given presentation space
 *               and window rectangle and draw the minute hand.  The
 *               routine first erases the old minute hand and then draws
 *               the new minute hand.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hps                - The presentation space to in which
 *                                    to draw.
 *               pptlCircleCenter   - The coordinates of the window center.
 *               usMinute           - The minute hand to display.
 *
 * Return      : None.
 *
 *************************************************************************/
VOID EXPENTRY DrawTheMinuteHand( HPS hps,
                        PPOINTL pptlCircleCenter,
                        USHORT  usMinute)
{
   POINTL ptlHandPosition;       /* Minute hand position.                     */
   USHORT usTempMinute;          /* Temporary value for the minute.           */

   extern USHORT usLastMinute;

   /*
    * Place the 'real' Minute into a temporary variable that can be
    * changed as desired.
    */
   usTempMinute = usMinute;

   /*
    * Move to the Center of the Clock Face.
    */
   GpiMove(
      hps,                 /* Presentation Space.                             */
      pptlCircleCenter );  /* Center point of the circle.                     */

   /*
    * Take the center of the circle as a starting point and initalize the
    * variable that is used to draw the hour hand.
    */
   ptlHandPosition.x = pptlCircleCenter->x;
   ptlHandPosition.y = pptlCircleCenter->y;

   /*
    * Check to make sure that the minute value that was passed into the
    * procedure is within bounds.  If it is not, the minute hand will NOT
    * be updated.
    */
   if ( usTempMinute <= 60 )
   {
      /*
       * See if the usMinute is at the top of the hour.  If it is, make
       * the variable look at the first cell in the array, i.e., the zero
       * cell.
       */
      if ( usTempMinute == 60 )
      {
         usTempMinute = 0;
      }

      /*
       * Erase the OLD minute.
       * - Set the draw color to the background color.
       * - Set the position for the minute hand.
       * - Draw the minute to erase the old minute.
       */
      GpiSetColor(
         hps,                    /* Presentation Space.                       */
         SYSCLR_FIELDBACKGROUND);/* Background color to erase the old hour.   */

      ptlHandPosition.x +=
         (aptlHandsPosition[ usLastMinute ].x );
      ptlHandPosition.y +=
         (aptlHandsPosition[ usLastMinute ].y );

     GpiLine(
        hps,                     /* Presentation Space.                       */
        &ptlHandPosition );      /* Center point of the circle.               */

     DrawClockCircles(
        hps,                     /* Handle to the Presentation space.         */
        &ptlHandPosition,        /* Position to place the Marker.             */
        SYSCLR_FIELDBACKGROUND,  /* Color to make the end dot.                */
        (USHORT) HAND_DOT );     /* Size of the time marker circle.           */

     /*
      * Draw the NEW minute.
      * - Reset the center values into the position points.
      * - Calculate the new minute hand position.
      * - Reset the color.
      * - Move to the center of the clock.
      * - Draw the new minute hand.
      */
      ptlHandPosition.x = pptlCircleCenter->x;
      ptlHandPosition.y = pptlCircleCenter->y;

      ptlHandPosition.x +=
         (aptlHandsPosition[ usTempMinute ].x );
      ptlHandPosition.y +=
         (aptlHandsPosition[ usTempMinute ].y );

     GpiSetColor(
        hps,                 /* Presentation Space.                           */
        CLOCK_HAND_COLOR );  /* Color to use for the clock, Black             */

     GpiMove(
        hps,                 /* Presentation Space.                           */
        pptlCircleCenter );  /* Center point of the circle.                   */

     GpiLine(
        hps,                 /* Presentation Space.                           */
        &ptlHandPosition );  /* End of the clock hand.                        */

     DrawClockCircles(
        hps,                 /* Handle to the Presentation space.             */
        &ptlHandPosition,    /* Position to place the Marker.                 */
        CLR_BLUE,            /* Color to make the end dot.                    */
        (USHORT) HAND_DOT ); /* Size of the time marker circle.               */

     /*
      * Since the minute value passed into the routine has been used, make
      * this minute is the Last Minute that was updated.  This allows the last
      * minute to be removed from the clock face before the new minute is
      * placed on the clock.
      */
     usLastMinute = usMinute;

  }  /* End of IF minute is within bounds. */

} /* End of DrawTheMinuteHand */


/*************************************************************************
 * Name        : PlaceTheTimeMarkers
 *
 * Description : This procedure will take the given parameters and
 *               figure out where to place the given clock markers on the
 *               face of the clock, i.e., where a 1, 2, 3,.. would be
 *               placed on a real clock face.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hps                - Handle to the presentation space.
 *               pptlMarkerPosition - Will hold the point to place a numeral.
 *               usClockNumeral     - The current time marker.
 *
 * Return      : pptlMarkerPosition - Holds the coordinates for the numeral.
 *
 *************************************************************************/
VOID PlaceTheTimeMarkers( HPS     hps,
                          PPOINTL pptlMarkerPosition,
                          USHORT  usClockNumeral )
{
   /*
    * See if the time marker is for the 12 o' clock position, if so, make
    * the value 0 so that the index into the position array will work
    * correctly.
    */
   if ( usClockNumeral == 12 )
   {
      usClockNumeral = 0;
   }

   /*
    * If the clock value is in range draw the marker on the clock face.
    * - Get the position for the marker based on the given clock value.
    * - See if the clock value requires a BIG marker or a SMALL marker.
    * - Draw the appropriate marker.
    */
   if ( usClockNumeral <= 11 )
   {

      pptlMarkerPosition->x +=
         (aptlMarkerPosition[ usClockNumeral * HOUR_HAND_INDEX ].x );
      pptlMarkerPosition->y +=
         (aptlMarkerPosition[ usClockNumeral * HOUR_HAND_INDEX ].y );

      /*
       * If the clock numeral is 3, 6, 9, or 0(12) then place a big circle
       * on the clock face, otherwise place a little circle.
       */
      if ( usClockNumeral == 3 || usClockNumeral == 6 ||
           usClockNumeral == 9 || usClockNumeral == 0   )
      {
         DrawClockCircles(
            hps,                 /* Handle to the Presentation space.         */
            pptlMarkerPosition,  /* Position to place the Marker.             */
            CLR_BLUE,            /* Color to make the end dot.                */
            (USHORT) 4 );        /* Size of the time marker circle.  Bigger.  */
      }
      else
      {
         DrawClockCircles(
            hps,                 /* Handle to the Presentation space.         */
            pptlMarkerPosition,  /* Position to place the Marker.             */
            CLR_BLUE,            /* Color to make the end dot.                */
            (USHORT) 2 );        /* Size of the time marker circle.  Smaller .*/
      }  /* End of IF which marker to draw. */

   }  /* End of checking for valid value. */

} /* End of PlaceTheTimeMarkers */



/*************************************************************************
 * Name        : DrawClockCircles
 *
 * Description : This procedure will take the given parameters and place
 *               a circle at some point in the Presentation Space.  The
 *               circle size, color, and position is given by the calling
 *               routine.
 *
 * Concepts    : None.
 *
 * MMPM/2 API's: None.
 *
 * Parameters  : hps - The handle to the Presentation space.  This will
 *                     allow the procedure to draw the bitmaps onto the
 *                     screen.
 *               pptlMarkerPosition - The point to place the marker.
 *               lCircleColor       - The color to make the marker.
 *               usCircleSize       - How big to make the marker.
 *
 * Return      : None.
 *
 *************************************************************************/
VOID DrawClockCircles( HPS     hps,
                       PPOINTL pptlMarkerPosition,
                       LONG    lCircleColor,
                       USHORT  usCircleSize )
{
   /*
    * Set the color of the Time markers.
    */
   GpiSetColor(
      hps,              /* Presentation Space.                                */
      lCircleColor );   /* Color to use for the clock, Black                  */

   /*
    * Move to the starting position of the marker.
    */
   GpiMove(
      hps,
      pptlMarkerPosition );

   GpiFullArc(
      hps,                       /* Presentation Space.                       */
      DRO_OUTLINEFILL,           /* Fills the interior of the circle.         */
      (usCircleSize * 65536) );  /* How big is the circle.                    */

}  /* End of DrawClockCircles */


