@echo off
@rem @(#) /58 1.3 8/1/95 18:22:41 [10/1/95 15:22:26]

@rem COMPONENT_NAME: somc
@rem
@rem ORIGINS: 27
@rem
@rem
@rem 10H9767, 10H9769  (C) COPYRIGHT International Business Machines Corp. 1992,1994
@rem All Rights Reserved
@rem Licensed Materials - Property of IBM
@rem US Government Users Restricted Rights - Use, duplication or
@rem disclosure restricted by GSA ADP Schedule Contract with IBM Corp.

@rem Command script to run the hh emitter on all idl files in the include directory.

SETLOCAL
IF "%SOMBASE%"=="" GOTO sombase

IF "%1"=="" GOTO body
%1

:body
echo Generating the SOM DTS C++ Usage Bindings:
CD %SOMBASE%\include
IF NOT EXIST som.hc GOTO drive:
SET SMNOADDSTAR=
SC -%% -S100000 -shh -mnoqualifytypes -musexhpass *.idl
IF NOT ERRORLEVEL 0 GOTO end
echo somhh.bld > somhh.bld
IF EXIST somdtype.idl SC -shh -DEMIT_SOMDTYPES -mnoqualifytypes -musexhpass somdtype.idl
GOTO end

:sombase
    ECHO SOMBASE should be set before running somhh.
    GOTO end

:drive
    ECHO SOM is not installed on the current drive, please specify it.

:end
    ENDLOCAL
