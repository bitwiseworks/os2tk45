@rem @(#) somc/somenv.cmd 2.9 9/15/94 10:46:08 [10/20/94 10:47:24]

@echo off

@rem COMPONENT_NAME: some
@rem
@rem ORIGINS: 27
@rem
@rem
@rem 10H9767, 10H9769  (C) COPYRIGHT International Business Machines Corp. 1992,1994
@rem All Rights Reserved
@rem Licensed Materials - Property of IBM
@rem US Government Users Restricted Rights - Use, duplication or
@rem disclosure restricted by GSA ADP Schedule Contract with IBM Corp.

rem -- Uncomment and customize the following line for your file system.
rem -- set SOMBASE=C:\som
rem -- Once you have set SOMBASE you can delete the following line:

if "%SOMBASE%" == "" goto firstime

rem -- If you have not already customised your CONFIG.SYS file during the
rem -- installation process, you must at least change the LIBPATH variable
rem -- to pick up SOMobjects DLL's.
rem -- %SOMBASE%\lib; should be put at the beginning of the LIBPATH, to
rem -- permit OS/2 and the Workplace Shell to pick up the new SOM DLL.
rem -- For example: LIBPATH=%SOMBASE%\lib;%LIBPATH%

rem -- SOMIR enviroment variable is set in the CONFIG.SYS file.
rem -- Update CONFIG.SYS as required.
rem -- The created/updated IR is always the last file mentioned in the SOMIR
rem -- list.  IR searches always begin at the front of the list.

set SMINCLUDE=.;%SOMBASE%\include;
set SMTMP=%SOMBASE%\tmp
set PATH=%SOMBASE%\bin;%PATH%
set INCLUDE=.;%SOMBASE%\include;%INCLUDE%
set DPATH=%SOMBASE%\msg;%DPATH%
set LIB=.;%SOMBASE%\lib;%LIB%
goto end

:firstime
echo Edit SOMENV.CMD to set SOMBASE.

:end
