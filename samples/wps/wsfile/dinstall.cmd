/* */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

if SysDestroyObject("<WSFILE>") then
   say 'Workplace Shell File Example successfully uninstalled.'
else
   say 'Error, Workplace Shell File Example not uninstalled properly.'

if SysDestroyObject("<WSFOLDER>") then
   say 'Workplace Shell Folder Example successfully uninstalled.'
else
   say 'Error, Workplace Shell Folder Example not uninstalled properly.'

if SysDestroyObject("<EXAMPLE>") then
   say 'Example Folder removed successfully.'
else
   say 'Error, Example Folder not removed properly.'

