/* Register the WPSTutorial class and create an object instance. */

/* Load the REXX utility */
Call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
Call SysLoadFuncs

/* Make sure the proper directories have been created and copy   */
/* the required files to those directories.                      */
Call SysMkDir '..\..\..\DLL'
COPY 'wpstutor.dll' '..\..\..\DLL'
Call SysMkDir '..\..\..\BIN'
COPY 'showdesc.exe' '..\..\..\BIN'

/* Register the class with the Workplace Shell                   */
if SysRegisterObjectClass( 'WPSTutorial', 'WPSTUTOR')
        then say 'WPSTutorial class registered sucessfully.'
        else say 'Error registering the WPSTutorial class.'

/* Create an object instance of the class                        */
if SysCreateObject( 'WPSTutorial', 'WPSTutorial', '<WP_DESKTOP>',,
                    'OBJECTID=WPSTUT001')
        then say 'WPSTutorial instance created sucessfully.'
        else say 'Error creating WPSTutorial instance.'

Exit 0
