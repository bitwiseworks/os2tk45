@echo off
@rem @(#) somc/somcorba.cmd 2.15 9/27/95 09:25:19 [7/30/96 14:44:17]

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
echo Generating the CORBA C Bindings:
CD %SOMBASE%\include
IF NOT EXIST som.hc GOTO drive:
ATTRIB -R som.h
ATTRIB -R gen_c.efw
ATTRIB -R gen_emit.efw
ATTRIB -R gen_make.efw
ATTRIB -R gen_mk32.efw
COPY som.hc       som.h > NUL
COPY gen_c.efc    gen_c.efw  > NUL
COPY gen_emit.efc gen_emit.efw  > NUL
COPY gen_make.efc gen_make.efw  > NUL
COPY gen_mk32.efc gen_mk32.efw > NUL
IF EXIST somstars.bld DEL somstars.bld
SET SMADDSTAR=
SC -%% -S150000 -mnoaddstar -sh *.idl
IF EXIST somdtype.idl SC -mnoaddstar -sh -DEMIT_SOMDTYPES somdtype.idl
IF NOT ERRORLEVEL 0 GOTO end
echo somcorba.bld  > somcorba.bld
GOTO end

:sombase
    ECHO SOMBASE should be set before running somcorba.
    GOTO end

:drive
    ECHO SOM is not installed on the current drive, please specify it.

:end
    ENDLOCAL
