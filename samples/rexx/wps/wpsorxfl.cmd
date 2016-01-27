/******************************************************************************/
/*  WPSORXFL                 Object REXX Samples                              */
/*                                                                            */
/*  A workplace shell example.                                                */
/*                                                                            */
/*  This program demonstrates a couple of useful routines for folders.        */
/*  The first routine will take a WPS folder as input and return an array of  */
/*  SOM objects; one for each object in that folder.                          */
/*  The second routine will take an array of WPS objects and a string;        */
/*  returning an array of objects that match the string.                      */
/*  It also uses 3 methods to iterate over arrays (supplier, do while, and    */
/*  do over) as iteration examples.                                           */ 
/******************************************************************************/
trace o
call wpconst                                /* Wpconst is needed by the       */
                                            /* GetFolderObjects routine       */
desktopArray = GetFolderObjects(.wpdesktop) /* find all objects on the desktop*/

cursor = desktopArray~supplier              /* use supplier to get all objects*/
do while cursor~available
    say 'Title:'cursor~item~wpQueryTitle'   String:'cursor~item~string
  cursor~next
end
say '   '
                                            /* Now get just WPS folders that  */
                                            /* are on the desktop             */
desktopFolders = FilterByString(desktopArray, 'a WPFolder')
say 'Number of folders on desktop:'desktopfolders~size
cursor = desktopFolders~supplier
do while cursor~available
    say 'Title:'cursor~item~wpQueryTitle
  cursor~next
end
exit

/******************************************************************************/
/* This procedure will create an array of som objects that are contained in   */
/* the folder passed in as the argument. wpconst is required.                 */
/******************************************************************************/
GetFolderObjects: procedure expose wpconst
use arg fld

fldArray = .array~new
num = 0

fld~wpPopulate(0, fld~wpQueryTitle, .false)
first = fld~wpQueryContent(fld,.wpconst[QC_FIRST])
last = fld~wpQueryContent(fld,.wpconst[QC_LAST])

this = first                                /* we start with the 1st object   */
do while this \= .nil
  num = num + 1
  fldArray[num] = this                      /* Save object ptr in array       */
  prev = this                               /* keep reference to prev object  */
  if this = last then                       /* Are we at end of list?         */
    this = .nil                             /* if so, indicate at end.        */
  else                                      /* otherwise, get next folder item*/
    this = fld~wpQueryContent(this,.wpconst[QC_NEXT])

  prev~wpUnLockObject                       /* unlock previous object.        */

end
return fldArray

/******************************************************************************/
/* FilterByString procedure                                                   */
/* This routine takes an array and an string for input and returns a new array*/
/* of the items whose "string" method output matches the input string.        */
/******************************************************************************/
FilterByString: procedure
use arg inarray, instring

newArray = .array~new
num = 0
do x over inarray
  if (x~string = instring) then
  do
    num = num + 1
    newArray[num] = x
  end
end
return newArray
