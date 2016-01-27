/******************************************************************************/
/*  Object REXX Samples and Examples                                          */
/*                                                                            */
/*  animal.cmd - Interact with SOM class ANIMAL                               */
/*                                                                            */
/*  This program demonstrates creating instances and sending messages to a    */
/*  SOM class.                                                                */
/******************************************************************************/

spot = .dog~new

Say spot
say spot~somGetClassName
                                       /* Create new Big Dog Object         */
sadie = .bigDog~new
                                       /* setup sadie                       */
sadie~setup('Sadie', 'German Shepard', 'black and tan', 'Steve')
say sadie                             
say sadie~somGetClassName
sadie~display



                                       /* import som SOM Classes to use     */
::Class SomObject Public EXTERNAL 'SOM SOMObject'
::Class Animal    Public EXTERNAL 'SOM Animal'
::Class Dog       Public EXTERNAL 'SOM Dog'
::Class BigDog    Public SUBCLASS Dog
::method setup
  expose owner
  use arg name, breed, color, owner
                                       /* Owner assign on use Arg....       */

                                       /* Set the SOM attribute             */
  self~_set_name(name)
  self~_set_breed(breed)
  self~_set_color(color)
                                       /* set up the object objectName to   */
                                       /* Be the name as well.              */
  self~objectName = name

::method display
  expose owner

  say 'The Big 'self~_get_color' Dog "'self~_get_name'" is owned by 'owner
