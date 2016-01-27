@echo off
@rem @(#) somc/somxh.cmd 2.14 9/27/95 09:25:12 [7/30/96 14:44:12]

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

@rem Command script to convert to implict stars (like CORBA) for
@rem interface references.

SETLOCAL
IF "%SOMBASE%"=="" GOTO sombase

IF "%1"=="" GOTO body
%1

:body
echo Generating the SOM C++ Bindings:
CD %SOMBASE%\include
IF NOT EXIST som.hc GOTO drive:
SET SMNOADDSTAR=
SC -%% -S150000 -sxh *.idl
IF NOT ERRORLEVEL 0 GOTO end
echo somxh.bld > somxh.bld
IF EXIST somdtype.idl SC -sxh -DEMIT_SOMDTYPES somdtype.idl
GOTO end

:sombase
    ECHO SOMBASE should be set before running somxh.
    GOTO end

:drive
    ECHO SOM is not installed on the current drive, please specify it.

:end
    ENDLOCAL
