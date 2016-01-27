/******************************************************************************/
/*  month                    Object REXX Samples                              */
/*                                                                            */
/*  Display the days of the month for January 1994                            */
/*                                                                            */
/*  This program is similar to the month1.cmd exec found in the OS/2 2.0      */
/*  REXX User's Guide.  This version demonstrates the use of arrays to        */
/*  replace stems.                                                            */
/******************************************************************************/

/* First, create an array initialized to the days of the week                 */
days = .array~of("Sunday",    "Monday",   "Tuesday", ,
                 "Wednesday", "Thursday", "Friday",  ,
                 "Saturday" )
month = .array~new(31)                      /* Another way to create an array */
startday = 7                                /* First day of month is Saturday */

Do dayofmonth = 1 to 31
  dayofweek = (dayofmonth+startday+days~size-2) // days~size + 1
  Select
    When right(dayofmonth,1) = 1 & dayofmonth <> 11 then th = "st"
    When right(dayofmonth,1) = 2 & dayofmonth <> 12 then th = "nd"
    When right(dayofmonth,1) = 3 & dayofmonth <> 13 then th = "rd"
    Otherwise th = "th"
  end
  /* Store text in the month array, using names in the days array             */
  month[dayofmonth] = days[dayofweek] 'the' dayofmonth||th "of January"
end

month~put( month[1]', New Years day', 1 )   /* Another way to set an array    */
                                            /* element                        */
Do dayofmonth = 1 to 31
  Say month~at(dayofmonth)                  /* Another way to access array    */
                                            /* elements                       */
end
