/******************************************************************************/
/*  semcls                   Object REXX Samples                              */
/*                                                                            */
/*  An Object REXX semaphore class.                                           */
/*                                                                            */
/* This file implements a semaphore class in Object REXX.  The class will be  */
/* defined to the Global OREXX Environment.  The following methods are        */
/* defined for this class:                                                    */
/*   init - initialize a new semaphore.  Will accept the following positional */
/*          parameters:                                                       */
/*                             'name' - global name for this semaphore        */
/*                                if named default to set name in             */
/*                                the class semDirectory                      */
/*                             noshare -  don't define named semaphore        */
/*                                into class semDirectory                     */
/*                             Inital state (0 or 1)                          */
/*   setInitialState - Allow for subclass to have some post initialization,   */
/*     and do setup based on inital state of semaphore.                       */
/*   Waiting - number of objects waiting on this semaphore                    */
/*   Shared - is this semaphore shared(Global)                                */
/*   Named  - is this semaphore named                                         */
/*   Name   - name of a named semaphore                                       */
/*   setSem   - Set the semaphore, and return previous state                  */
/*   resetSem - set state to unSet                                            */
/*   querySem - return current state of semaphore                             */
/*                                                                            */
/* SemaphoreMeta - is the metaclass for the semaphore classes.  This class is */
/*   setup so that when a namedSemaphore is shared, it maintains these        */
/*   named/shared semaphores as part of its state.  These semaphores are      */
/*   maintained in a directory, and an UNKNOWN method is installed on the     */
/*   class to forward unknown messages to the directory.  In this way the     */
/*   class can function as a class and a "like" a directory, so [] sytax may  */
/*   be used to retrieve a semaphore from the class.                          */
/*                                                                            */
/*                                                                            */
/* following are in the subclass EventSemaphore                               */
/*                                                                            */
/*   Post  - post this semaphore                                              */
/*   Query - number of posts since the last reset                             */
/*   Reset - reset the semaphore                                              */
/*   Wait  - wait on this semaphore                                           */
/*                                                                            */
/*                                                                            */
/* following are in the subclass MutexSemaphore                               */
/*                                                                            */
/*   requestMutex - get exclusive use of semaphore                            */
/*   releaseMutex - release to allow someone else to use semaphore.           */
/*        NOTE: currently anyone may issue release. not forced to be owner... */
/******************************************************************************/
 
/* ========================================================================== */
/* ===         Start of Semaphore class ......                            === */
/* ========================================================================== */
::class SemaphoreMeta subclass class
::method init
  expose semDict
                                            /* be sure to initialize parent   */
  .message~new(self, .array~of('INIT', super), 'a', arg(1,'a'))~send
  semDict = .directory~new

::method unknown
  expose semDict
  use arg msgName, args
                                            /* forward all unknown messages to*/
                                            /* the semaphore dictionary       */
  .message~new(semDict, msgName, 'a', args)~send
  if var('RESULT') then
    return result
  else
    return 


::class Semaphore subclass object metaclass SemaphoreMeta

::method init
  expose sem waits shared name
  use arg semname, shr, state
 
  waits = 0                                 /* no one waiting                 */
  name = ''                                 /* assume unnamed                 */
  shared = 0                                /* assume not shared              */
  sem = 0                                   /* default to not posted          */
                                                                               
  if state = 1 Then                         /* should initial state be set    */
   sem = 1                                                                     
                                            /* was a name specified?          */
  if VAR('SEMNAME') & semname \= '' Then Do
   name = semname                           /* yes, so set the name           */ 
                                                                                
                                                                                
   if shr \= 'NOSHARE' Then Do              /* do we want to share this sem   */ 
     shared = 1                             /* yes, mark it shared            */ 
                                            /* shared add to semDict          */ 
     self~class[name] = self                                                    
   End                                                                          
                                                                                
  End                                                                           
  self~setInitialState(sem)                 /* initialize initial stat        */ 
                                                                                
::method setInitialState                                                        
                                            /* this method intended to be     */ 
  nop                                       /* overriden by subclasses.       */ 
                                                                                
                                                                                
::method setSem                                                                 
  expose sem                                                                    
  oldState = sem                                                                
  sem = 1                                   /* set new state to 1.            */ 
  return oldState                                                               
                                                                                
::method resetSem                                                               
  expose sem                                                                    
  sem = 0                                                                       
  return 0                                                                      
                                                                                
::method querySem                                                               
  expose sem                                                                    
  return sem                                                                    
                                                                                
                                                                                
::method shared                                                                 
  expose shared                                                                 
  return shared                             /* return true 1 or false 0       */ 
                                                                                
::method named                                                                  
  expose name                                                                   
                                            /* does semaphore have a name     */ 
  if name = '' Then return 0                /* nope, not named                */ 
               Else return 1                /* yes, its named                 */ 
                                                                                
::method name                                                                   
  expose name                                                                   
  return name                               /* return name or ''              */ 
                                                                                
::method incWaits                                                               
  expose waits                                                                  
  waits = waits + 1                         /* one more object waiting        */ 
                                                                                
::method decWaits                                                               
  expose Waits                                                                  
  waits = waits - 1                         /* one less object waiting        */ 
                                                                                
::method Waiting                                                                
  expose Waits                                                                  
  return waits                              /* return num of objects waiting  */ 


 
/* ========================================================================== */
/* ===         Start of EventSemaphore class ......                       === */
/* ========================================================================== */
 
::class EventSemaphore subclass Semaphore public
::method setInitialState
  expose posted posts
  use arg posted

  if posted  then posts = 1
             else posts = 0
::method post
  expose posts posted
 
  self~setSem                               /* set semaphore state            */
  posted = 1                                /* mark as posted                 */
  reply                                                                        
  posts = posts + 1                         /* increase the number of posts   */
                                                                               
::method wait                                                                  
  expose posted                                                                
                                                                               
  self~incWaits                             /* increment number waiting.      */
  guard off                                                                    
  guard on when posted                      /* now wait until posted.         */
  reply                                     /* return to caller.              */
  self~decWaits                             /* cleanup, 1 less waiting.       */
                                                                               
::method reset                                                                 
  expose posts posted                                                          
                                                                               
  posted = self~resetSem                    /* reset semaphore                */
  reply                                     /* do an early reply              */
  posts = 0                                 /* reset number of posts          */
 
::method query
  expose posts
                                            /* return number of times         */
  return posts                              /* semaphore has been posted.     */
 


/* ========================================================================== */
/* ===         Start of MutexSemaphore class ......                       === */
/* ========================================================================== */
 
::class MutexSemaphore subclass Semaphore public
  
::method setInitialState
  expose owned 
  use arg owned 
 
 
::method requestMutex
  expose Owned
 
  Do forever                                /* do until we get the semaphore  */
   owned = self~setSem                                                         
   if Owned = 0                             /* was semaphore already set?     */
    Then leave                              /* wasn't owned, we now have it.  */
    else Do                                                                    
     self~incWaits                                                             
     guard off                              /* turn off guard status to let   */
                                            /* others come in.                */
     guard on when \Owned                   /* wait until not owned and get   */
                                            /* guard.                         */
     self~decWaits                          /* one less waiting for MUTEX.    */
    End                                                                        
                                            /* go up an see if we can get it  */
  End                                                                             
 
 
::method releaseMutex
  expose owned
  owned = self~resetSem                     /* reset semaphore                */
