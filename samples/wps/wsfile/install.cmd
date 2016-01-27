/* */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

parse arg bootdrive

/* change following drive letter to your boot drive */
if (bootdrive = "") then bootdrive = "d:"

libdir = bootdrive||"\os2\dll"
helpdir = bootdrive||"\os2\help"

'copy wsfile.dll' libdir
'copy wsfile.sym' libdir
'copy wsfile.hlp' helpdir
'copy wsfolder.dll' libdir
'copy wsfolder.sym' libdir
'copy wsfolder.hlp' helpdir

if SysCreateObject("WPFolder", "Example", "<WP_DESKTOP>",,
                                "OBJECTID=<EXAMPLE>") then
   say 'Example Folder successfully installed.'
else
   say 'Error, Example Folder not installed properly.'


if SysCreateObject("WSFILE","Workplace Shell File Example","<EXAMPLE>",,
                            "SOUND=YES;OBJECTID=<WSFILE>") then
   say 'Workplace Shell File Example successfully installed.'
else
   say 'Error, Workplace Shell File Example not installed properly.'


if SysCreateObject("WSFOLDER","Workplace Shell Folder Example","<EXAMPLE>",,
                              "OBJECTID=<WSFOLDER>") then
   say 'Workplace Shell Folder Example successfully installed.'
else
   say 'Error, Workplace Shell Folder Example not installed properly.'




