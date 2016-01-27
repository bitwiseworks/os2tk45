@echo off
@rem @(#) somc/somstars.cmd 2.15 9/27/95 09:25:15 [7/30/96 14:44:17]

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

@rem Shell script to convert to explicit stars for interface references.

SETLOCAL
IF "%SOMBASE%"=="" GOTO sombase

IF "%1"=="" GOTO body
%1

:body
echo Generating the SOM C Bindings:
echo (Warning, these bindings are not CORBA-compliant)
CD %SOMBASE%\include
IF NOT EXIST som.hc GOTO drive:
ATTRIB -R som.h
ATTRIB -R gen_c.efw
ATTRIB -R gen_emit.efw
ATTRIB -R gen_make.efw
ATTRIB -R gen_mk32.efw
COPY som.hs       som.h > NUL
COPY gen_c.efs    gen_c.efw  > NUL
COPY gen_emit.efs gen_emit.efw  > NUL
COPY gen_make.efs gen_make.efw  > NUL
COPY gen_mk32.efs gen_mk32.efw > NUL
IF EXIST somcorba.bld DEL somcorba.bld
SET SMNOADDSTAR=
SC -%% -S150000 -maddstar -sh *.idl
IF NOT ERRORLEVEL 0 GOTO end
echo somstars.bld > somstars.bld
IF EXIST somdtype.idl SC -maddstar -sh -DEMIT_SOMDTYPES somdtype.idl
GOTO end

:sombase
    ECHO SOMBASE should be set before running somstars.
    GOTO end

:drive
    ECHO SOM is not installed on the current drive, please specify it.

:end
    ENDLOCAL
