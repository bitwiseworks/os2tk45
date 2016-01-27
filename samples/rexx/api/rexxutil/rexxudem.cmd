/*******************************************************************/
/*                                                                 */
/* REXXUDEM.CMD                                                    */
/*                                                                 */
/* This program calls the various REXX external functions          */
/* provided in the REXXUTIL.C sample function package.             */
/*                                                                 */
/* Each function is called once to illustrate how it is used.      */
/* Some of the functions have multiple options and multiple        */
/* types of output, but not all options are used here.             */
/*                                                                 */
/* Some of the functions in REXXUTIL only work in windowed         */
/* or fullscreen command prompt sessions, not in PMREXX.           */
/* This is because they are doing vio-type operations such as      */
/* placing the cursor at a particular place on the screen.         */
/* So if this program is run in PMREXX, it will start a new        */
/* windowed session and run itself there.                          */
/*                                                                 */
/*******************************************************************/
'@echo off'

/* Detect if we are running under PMREXX.                          */
/* Some of the REXXUTIL functions (such as SysCurPos) only work    */
/* in OS/2 command prompt sessions, not in PMREXX.                 */
If Address() = 'PMREXX' Then  Do
  Say 'This program will now run itself in a windowed '
  Say '  command prompt session'
  'START REXXUDEM.CMD /C /WIN'
  Exit
  End
  /* We now know we are running in a line-mode session.            */

/* For several of the functions, we will need to write to some     */
/* drive on the system.  We decide in advance what drive that      */
/* will be.  Our method is:  Look for :\OS2\SYSTEM in the PATH.    */
/* If found, use the preceding character.  Otherwise, default to C.*/
path = value('PATH',,'OS2ENVIRONMENT')
if pos(':\OS2\SYSTEM', path) > 1 then
   Parse Upper var path . ":\OS2\SYSTEM" -1 bootdrive +1 .
   else bootdrive = 'C'


/* SysLoadFuncs:  This function will load all the rest,            */
/* so we do not have to make many calls to RxFuncAdd.              */

Say 'This program will call many of the REXX functions provided'
Say '  in REXXUTIL.DLL.  The first one is SysLoadFuncs, which makes'
Say '  all the others available.'
call RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
call SysLoadFuncs
Say 'SysLoadFuncs is complete.'
call SysPause                     /* This is an internal function. */
                                  /* See below.                    */


/*** Demonstrate SysCls ***/
call SysCls; say
call SysPause 'Demonstrating SysCls, press Enter key...'
call SysCls
say; say 'Did you see that quick screen-clear via SysCls?'
call SysPause


/*** Demonstrate SysCurPos ***/
call SysCls
say; say 'Demonstrating SysCurPos...'
call SysCurPos '3', '0'
call SysPause 'Press a key'
call SysCurPos '10', '50'
call SysPause 'Press a key'
call SysCurPos '20', '20'
call SysPause 'Press a key'
call SysCurPos '5', '65'
call SysPause 'Press a key'
call SysCurPos '23', '0'
call SysPause


/*** Demonstrate SysCurState ***/
call SysCls
call SysCurState 'OFF'
say; say 'Demonstrating SysCurState..'; say
call SysPause 'Note that the cursor is hidden.  Press Enter key...'
call SysCurState 'ON'


/*** Demonstrate SysDriveInfo ***/
call SysCls
say; say 'Demonstrating SysDriveInfo by displaying',
          bootdrive': drive info:'
say SysDriveInfo(bootdrive':')
call SysPause


/*** Demonstrate SysDriveMap ***/
call SysCls
say; say 'Demonstrating SysDriveMap by displaying all available drives:'
say SysDriveMap()
call SysPause


/*** Demonstrate SysGetMessage ***/
call SysCls
say; say 'Demonstrating SysGetMessage by reading message #34 from OSO001.MSG'; say
say SysGetMessage(34, , 'A:', 'the diskette labeled "Disk 2"', 'SER0002')
Say '  (that was just a demonstration.  You do not have to insert a diskette)'
call SysPause

/*** Demonstrate SysPutEA ***/
call SysCls
say; say 'Demonstrating SysPutEA...'
call SysPause
call SysSay 'Creating rxtemp.fil...'
call lineout 'rxtemp.fil', 'xyzpdq'
call stream 'rxtemp.fil', 'C', 'CLOSE'
say 'done.'
OS2_rc = SysPutEA('rxtemp.fil', 'REXXUDEM', 'Demonstration of SysPutEA')
If OS2_rc = 0 Then Say 'rxtemp.fil now has an Extended Attribute',
                       'named REXXUDEM.'
Else Say 'We have a problem, the OS/2 file system returned error' OS2_rc
call SysPause

/*** Demonstrate SysGetEA ***/
call SysCls
say; say 'Demonstrating SysGetEA...'
OS2_rc = SysGetEA('rxtemp.fil', 'REXXUDEM', 'my_output_variable')
If OS2_rc = 0 Then Do
  Say 'the value of the REXXUDEM Exnded Attribute',
      'is "'my_output_variable'"'
  If my_output_variable == 'Demonstration of SysPutEA' then
    Say '  ...and that is the right value.'
  else Say '  ...but that is the wrong value!'
  End
Else Say 'We have a problem, the OS/2 file system returned error' OS2_rc
call SysPause


/*** Demonstrate SysFileDelete ***/
call SysCls
say; say 'Demonstrating SysFileDelete by deleting the file rxtemp.fil:'
call SysPause
if SysFileDelete('rxtemp.fil')=0 then
  say 'The file rxtemp.fil has been deleted.'
else
  say 'Could not delete rxtemp.fil!'
call SysPause


/*** Demonstrate SysFileSearch ***/
call SysCls
say; say 'Demonstrating SysFileSearch:'
say 'About to search the 'bootdrive':\CONFIG.SYS files for'
say 'the word "device":'; say
call SysPause; say
call SysFileSearch 'device', bootdrive':\config.sys', 'file'
say 'Lines found:'; say
do num=1 to file.0
  say file.num
end
say
call SysPause


/*** Demonstrate SysFileTree ***/
call SysCls
say; say 'Demonstrating SysFileTree by displaying all entries in',
          bootdrive': root subdir:'
call SysPause
call SysFileTree bootdrive':\*', 'files', 'B'
say;
do num=1 to files.0
  say files.num
end
call SysPause


/*** Demonstrate SysIni ***/
call SysCls
say; say 'Demonstrating SysIni by placing time information in OS2.INI file...'
time=Time()
call SysIni , 'REXXUDEM', 'TIME', time
call SysPause
say; say 'Demonstrating SysIni by retrieving time information from OS2.INI file...'
val = SysIni( , 'REXXUDEM', 'TIME')
say 'The retrieved information = 'val; say
call SysPause
say; say 'Deleting the time information from the OS2.INI file...'
call SysINI , 'REXXUDEM', 'TIME', '$RXDEL'
call SysPause


/*** Demonstrate SysMkDir ***/
call SysCls
say; say 'Demonstrating SysMkDir:'
say 'RC when trying to create directory',
    bootdrive':\RXTMPDIR = 'SysMkDir(bootdrive':\RXTMPDIR')
call SysPause


/*** Demonstrate SysRmDir ***/
call SysCls
say; say 'Demonstrating SysRmDir:'
say 'RC when trying to remove directory',
    bootdrive':\RXTMPDIR = 'SysRmDir(bootdrive':\RXTMPDIR')
call SysPause


/*** Demonstrate SysOS2Ver ***/
call SysCls

say; say 'Demonstrating SysOS2Ver'
Say 'You are running OS/2 version 'SysOS2Ver()
call SysPause


/*** Demonstrate SysSearchPath ***/
call SysCls
say; say 'Demonstrating SysSearchPath by searching for CMD.EXE in PATH:'
call SysPause
say 'Directory containing CMD.EXE is:  'SysSearchPath('PATH', 'cmd.exe')
call SysPause


/*** Demonstrate SysSleep ***/
call SysCls
say; say 'Demonstrating SysSleep:  Sleeping for 2 seconds...'
call SysSleep '2'
call SysPause


/*** Demonstrate SysTempFileName ***/
call SysCls
say; say 'Demonstrating SysTempFileName:'; say
say 'Unique file in' bootdrive':\OS2 =',
    SysTempFileName(bootdrive':\OS2\OS2.???'); say
call SysPause


/*** Demonstrate SysTextScreenRead ***/
call SysCls
say; say 'Demonstrating SysTextScreenRead by reading the entire screen.'; say
do i=0 to 10
 say copies(' ', i*5)'Hello'
end
say
call SysPause 'Press Enter key to read the screen...'
screen = SysTextScreenRead(0, 0)
say 'Screen read.'
call SysPause 'Press Enter key to clear the screen then restore it...'
call SysCls
call SysSay screen
call SysPause


/*** Demonstrate SysTextScreenSize ***/
call SysCls
say; say 'Demonstrating SysTextScreenSize...'
parse value SysTextScreenSize() with row col
say 'Rows='row', Columns='col
call SysPause

/*** Demonstrate SysDropFuncs ***/
call SysCls
say; say 'Demonstrating SysDropFuncs...'
Call SysDropFuncs
Say 'The functions have now been dropped.'

say; say 'REXXUDEM demonstration program is complete.'
exit



SysPause:
parse arg prompt
  if prompt='' then
    prompt='Press Enter key when ready . . .'
  call SysSay prompt
  Pull .
  say
return



SysSay:
parse arg string
  call charout 'STDOUT', string
return
