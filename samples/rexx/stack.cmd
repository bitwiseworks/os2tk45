/******************************************************************************/
/*  stack                    Object REXX Samples                              */
/*                                                                            */
/*  A stack class                                                             */
/*                                                                            */
/*  This program demonstrates how to implement a stack class using ::class    */
/*  and ::method directives.  Also included is a short example of the use of  */
/*  a stack.                                                                  */
/******************************************************************************/

mystack = .stack~new                        /* Create a new instance of stack */
mystack~push('John Smith')
mystack~push('Bill Brown')
say 'Size of stack:' mystack~items          /* Displays size of stack         */
say 'Pop stack:    ' mystack~pop            /* Displays item popped off stack */
say 'Pop stack:    ' mystack~pop            /* Displays next item popped off  */
say 'Pop stack:    ' mystack~pop            /* Stack now empty - displays     */
                                            /*   "The NIL Object"             */

/* Define the stack class (a subclass of Object Class) */
::class stack

/* Define the init method on the stack class */
::method init
expose contents                             /* Object var with stack contents */
self~init:super                             /* Run init method of superclass  */
contents = .list~new                        /* Use a list to keep stack items */

/* Define the push method on the stack class */
::method push
expose contents
use arg item
contents~insert(item, .nil)

/* Define the pop method on the stack class */
::method pop
expose contents
if contents~items > 0 then
  return contents~remove(contents~first)
else
  return .nil

/* Define the items method on the stack class to return number of items */
::method items
expose contents
return contents~items
