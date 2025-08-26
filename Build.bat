@echo off

REM #-#########-#
REM #- Globals -#
REM #-#########-#

SET g_TargetName=""
SET g_IsCustomPath=0
SET g_PathToTargetSource=""

REM #-######-#
REM #- Main -#
REM #-######-#

REM --------------------------------------------------------------------------------------------------------------------
:main
    setlocal enabledelayedexpansion
    set result=0
    for %%A in (%*) do (

        call :HandleArg result %%A    

        if !result! == 1 (

            set g_TargetName=%%A

        )
    )
    
    if "!g_TargetName!" == "" (
    
        call :ShowHelp

    )

    if not exist "%~dp0Samples\!g_TargetName!\" (

        call :ShowHelp

    )
    
    endlocal
    exit /b 0

REM #-###########-#
REM #- Functions -#
REM #-###########-#

REM --------------------------------------------------------------------------------------------------------------------
:ShowHelp
    echo How to use:
    echo.
    echo ./Build.sh [options...] project_name
    echo. 
    echo.
    echo Options
    echo   --ccomands                   - Generate compile_commands.json
    echo   --sample                     - Compile project from 'Samples' directory
    echo   --debug                      - Compile with debug utilities
    echo   --force                      - Force to always compile
    echo   --forcepurge                 - Force to always compile, purge old intermediate files
    echo   --tests                      - Creates a test build with tests
    echo   --testsonly                  - Creates a test build only with tests
    echo   --customtargetpath [path]    - Compile project with custom path
    echo   --verbose                    - Print more informations
    echo.

    exit /b 0

REM --------------------------------------------------------------------------------------------------------------------
:Panic
    echo Error! Cound't finish building the project due to error (Optional message: %1)

    exit /b 1

REM --------------------------------------------------------------------------------------------------------------------
:CheckIfSwitch
    goto :eof

REM --------------------------------------------------------------------------------------------------------------------
:HandleArg
    setlocal enabledelayedexpansion
    set argument=%2
    set isSwitch=0

    if "%g_IsCustomPath%" == 1 (

        if "%g_PathToTargetSource%" == "" (

            set g_PathToTargetSource = "%argument%"
            set %1=0
            goto :eof

        )
    ) 

    endlocal
    set %1=1
    goto :eof

