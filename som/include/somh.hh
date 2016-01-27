/***!Start!***                                                                     
 *                                                                                 
 *   ORIGINS: 27                                                                   
 *                                                                                 
 *     25H7912  (C)   COPYRIGHT International Business Machines Corp. 1992,1994,1996,1996  
 *   All Rights Reserved                                                           
 *   Licensed Materials - Property of IBM                                          
 *   The source code for this program is not published or otherwise divested       
 *   of its trade secrets, irrespective of what has been deposited with the        
 *   U.S. Copyright Office.                                                        
 *                                                                                 
 ***!End!***/                                                                      
                                                                                   
// somh.hh for DTS C++ (Vx.y)
// SE: May 19/94

#ifndef SOMH_HH_DTS_Included__
#define SOMH_HH_DTS_Included__

#pragma SOM

#pragma SOMAsDefault(On)
  class SOMObject;
  class SOMClass;
  class SOMClassMgr;
#pragma SOMAsDefault(Pop)
  

#pragma SOMAsDefault(off)
  #include <somltype.h> // linkage definitions
  #include <sombtype.h> // base SOM types
  #include <somcorba.h> // CORBA types
  #include <somapi.h>   // API types and function interfaces
#pragma SOMAsDefault(pop)

#endif /* SOMH_HH_DTS_Included__ */
