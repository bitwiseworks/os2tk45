/******************************************************************************/
/*  WPSORXCl                 Object REXX Samples                              */
/*                                                                            */
/*  A workplace shell example.                                                */
/*                                                                            */
/*  This program demonstrates how to use the OS/2 workplace shell folders to  */
/*  represent the OORexx classes. This is presented in a tree view where an   */
/*  OORexx class is a folder and subclasses are subfolders. The top folder    */
/*  title is updated as classes are added for a progress indicator.           */
/*  Methods in a class can be represented as abstract WPS objects; but the    */
/*  default is not to create them; since it is quite time consuming.          */
/******************************************************************************/
trace o
wantMethods=0                               /* 0= do not create method object */
                                            /* 1= create method abstract objs */
wpAbstract = .wps~import('WPAbstract')
if importerror('WPAbstract',wpAbstract) then exit 8

call wpconst
xcl= .wpfolder~new('OORexx .Object Class','',.wpdesktop,1)
xcl~wpsetup('NODELETE=NO')
xcl~wpSetDefaultView(.wpconst[OPEN_TREE])
status = xcl 
xcl~wpopen(0,0,0)
call classfolders .object,xcl
xcl~wpSetTitle('OORexx .Object Class')
return 0


/******************************************************************************/
/* This routine will just check the object by using the string method on      */
/* itself. If the value returned is of the form: 'The xxxxxx class'; then the */
/* import is assumed ok. A 0 is returned if the import was ok; non-zero       */
/* otherwise.                                                                 */
/******************************************************************************/
importerror: procedure 
trace o
use arg classname, testobject
x = testobject~string
validstring = 'The 'classname' class'
if (x = validstring) then return 0
say '**** Error in importing class: 'classname
return 8


/******************************************************************************/
/* This routine will get all the OORExx subclasses of the class passed in; and*/
/* either build an folder if there are no subclasses of it, or a folder with  */
/* subfolders (using a recursive call). If wantMethods=1; then an abstract    */
/* object will be created in each folder for every class method.              */
/******************************************************************************/
classfolders: procedure expose wpAbstract status wantmethods
use arg inClass, pfolder
do class over inClass~subclasses
   status~wpSetTitle('Adding class 'class)
   if ((class~subclasses~size)=0) then
   do
     /* this class has no subclasses; add it */
     y = .wpfolder~new(class~string,'',pfolder,1)
     if wantMethods=1 then
     DO
       allmethods = class~methods
       do while allmethods~available
         status~wpSetTitle('Adding class method 'allmethods~index' for 'class)
         x = wpAbstract~new(allmethods~index,'',y,1)
         x~wpsetup('NODELETE=NO')
         allmethods~next
       end /* do */
     end/* if wantMethods=1 */
   end
   else
   DO 
     /* this class has subclasses */
     y = .wpfolder~new(class~string,'',pfolder,1) 
     y~wpSetDefaultView(.wpconst[OPEN_TREE])
     call classfolders class,y
     allmethods = class~methods
     if wantMethods=1 then
     DO
       do while allmethods~available
         status~wpSetTitle('Adding class method 'allmethods~index' for 'class)
         x = wpAbstract~new(allmethods~index,'',y,1)
         x~wpsetup('NODELETE=NO')
         allmethods~next
       end /* do */
     end/* if wantMethods=1 */
   end /* do */
end /* do */
return 0
