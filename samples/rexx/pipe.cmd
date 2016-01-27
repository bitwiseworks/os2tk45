/******************************************************************************/
/*  pipe                     Object REXX Samples                              */
/*                                                                            */
/*  A pipeline implementation                                                 */
/*                                                                            */
/*  This program demonstrates the use of ::class and ::method directives to   */
/*  create a simple implementation of a CMS-like pipeline function.           */
/******************************************************************************/
::class filter public                       /* base filter class              */
                                                                                   
::method '[]' class                         /* create a "chain" of filters    */
start = self~new                            /* create a new filter            */
current = start                             /* current is the starting filter */
do i = 1 to arg()                           /* loop through the other filters */
  next = arg(i)                             /* get the next filter            */
  next = next~new                           /* create a new instance (maybe)  */
  current~next = next                       /* chain this up                  */
  current = next                            /* this is now the current one    */
end                                                                                
return start                                /* return the chain start         */
                                                                                   
::method go                                 /* execute using a provided object*/
expose source                               /* get the source supplier        */
use arg source                              /* set to the supplied object     */
self~begin                                  /* now go feed the pipeline       */
                                                                                   
::method next attribute                     /* next stage of the filter       */
::method source attribute                   /* source of the initial data     */
                                            /* that they are class objects for*/
::method new                                /* the filter chaining process    */
return self                                 /* just return ourself            */
                                                                                   
::method begin                              /* start pumping the pipeline     */
expose source next                          /* access the data and next chain */
                                                                                   
engine = source~supplier                    /* get a data supplier            */
do while engine~available                   /* while more data                */
  next~process(engine~item)                 /* pump this down the pipe        */
  engine~next                               /* get the next data item         */
end                                                                                
next~done                                   /* signal that we're finished     */
                                                                                   
::method process                            /* default data processing        */
use arg value                               /* get the data item              */
self~next~process(value)                    /* send this down the line        */
                                                                                   
::method done                               /* process "end-of-pipe" condition*/
use arg value                               /* get the data item              */
self~next~done                              /* send this down the line        */
                                                                                   
::class sorter public subclass filter       /* sort piped data                */
::method init                               /* sorter initialization method   */
expose items                                /* list of sorted items           */
items = .list~new                           /* create a new list              */
self~init:super                             /* forward the initialization     */
                                                                                   
::method process                            /* process sorter piped data item */
expose items                                /* access internal state data     */
use arg value                               /* access the passed value        */
index = items~first                         /* get the first list item        */
do while .nil <> index                      /* while we still have an index   */
  if items[index] > value then do           /* found the insertion point?     */
                                            /* insert this                    */
    items~insert(value, items~previous(index))
    return                                  /* we're finished                 */
  end  
  index = items~next(index)                 /* step to the next item          */
end                                                                                
items~insert(value)                         /* add this item to the end       */
                                                                                   
::method done                               /* process the "end-of-pipe"      */
expose items                                /* expose the list                */
index = items~first                         /* get the first item             */
do while .nil <> index                      /* while we still have items      */
  self~next~process(items[index])           /* send along this item           */
  index = items~next(index)                 /* step to the next item          */
end                                                                             
                                                                                   
::class reverser public subclass filter     /* a string reversal filter       */
::method process                            /* filter processing item         */
use arg value                               /* get the data item              */
self~next~process(value~reverse)            /* send it along in reversed form */
                                                                                
                                            /* terminal filter item           */
::class displayer subclass filter public    
::method process                            /* process a data item            */ 
use arg value                               /* get the data value             */ 
say value                                   /* display this item              */ 
                                                                                    
::method done                               /* all finished                   */ 
return                                      /* this is a terminal node        */ 
                                                                                   
::class selector public subclass filter     /* a string selector filter       */ 
::method init                               /* process initial strings        */ 
expose patterns                             /* access the exposed item        */ 
patterns = arg(1,'a')                       /* get the patterns list          */ 
self~init:super                             /* forward the initialization     */ 
                                                                                    
::method '[]' class                         /* initialize a selector filter   */ 
use arg a                                   /* just create a new item         */ 
return self~new(a)                

::method process                            /* process a selection filter     */
expose patterns                             /* expose the pattern list        */
use arg value                               /* access the data item           */
do i = 1 to patterns~size                   /* loop through all the patterns  */
                                            /* this pattern in the data?      */
  if (value~pos(patterns[i]) <> 0) then do
    self~next~process(value)                /* send it along                  */
    leave                                   /* stop the loop                  */
  end
end
                                            
::class stem subclass filter public         /* collect items in a stem        */                                       
::method init                               /* initialize a collector         */
expose stem.                                /* expose target stem             */
use arg stem.                               /* get the stem variable target   */
stem.0 = 0                                  /* start with zero items          */
self~init:super                             /* forward the initialization     */
                                                                                   
::method process                            /* process a stem filter item     */
expose stem.                                /* expose the stem                */
use arg value                               /* get the data item              */
stem.0 = stem.0 + 1                         /* stem the item count            */
stem.[stem.0] = value                       /* save the value                 */
                                                                                   
::method done                               /* all finished                   */
return                                      /* this is a terminal node        */
