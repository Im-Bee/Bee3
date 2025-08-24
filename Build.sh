#!/bin/bash

# Everything with prefix `_bg_` is exported from script

#-###########-#
#- Functions -#
#-###########-#

# ----------------------------------------------------------------------------------------------------------------------
_bg_ShowHelp()
{
    echo "How to use: "
    echo ""
    echo "./Build.sh [options...] project_name"
    echo ""
    echo ""
    echo "Options:"
    echo "  --ccomands                   - Generate compile_commands.json"
    echo "  --sample                     - Compile project from 'Samples' directory"
    echo "  --debug                      - Compile with debug utilities"
    echo "  --force                      - Force to always compile"
    echo "  --forcepurge                 - Force to always compile, purge old intermediate files"
    echo "  --tests                      - Creates a test build with tests"
    echo "  --testsonly                  - Creates a test build only with tests"
    echo "  --customtargetpath <path>    - Compile project with custom path"
    echo "  --verbose                    - Print more informations"
    echo ""

    exit 0
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_Panic()
{
    _bg_Log "Error! Cound't finish building the project due to error (Optional message: $1)"
    exit 1
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_Log()
{
    local log=$1

    if [ "$g_RelativePathToLog" == "" ]; then

        if declare -F Log > /dev/null; then
        
            Log "$log"
            return 0

        fi

        echo "g_RelativePathToLog is not set, so can't _bg_Log(). There is no implementation of 'Log()' as well, can't log!!!"
        echo "Last message: $log"
        exit 1

    fi

    if ! [ -e "./Log.sh" ]; then

        echo "./Log.sh doesn't exists!"
        echo "Last message: $log"
        exit 1

    fi

    echo -e "$log" | ./Log.sh "$g_RelativePathToLog"
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_DebugLog()
{
    local log=$1

    if [ "$g_DebugMode" == 1 ] || [ "$g_DebugMode" == 2 ]; then

        _bg_Log "[\e[36mDEBUG\e[0m]: $log"

    fi
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_BeginDebugging()
{
    if [ "$g_DebugMode" == 2 ]; then

        set -x

    fi
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_EndDebugging()
{
    if [ "$g_DebugMode" == 2 ]; then

        set +x

    fi
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_CheckIfInBuildLinuxDir()
{
    local currentDir
    currentDir=$(pwd)

    if [ -e "$currentDir/Log.sh"  ] && [ -e "$currentDir/Compile.sh" ] && [ -e "$currentDir/Link.sh" ]; then

        return 0

    else

        return 1

    fi
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_ToLower()
{
    echo "$1"  | tr "[:upper:]" "[:lower:]"
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_ToUpper()
{
    echo "$1"  | tr "[:lower:]" "[:upper:]"
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_StripExtensionFromFile()
{
    echo "${1%.*}"
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_StripPathFromFile() 
{
    if [ "${#1}" -le 1 ]; then
        
        echo "$1"

    fi

    local path="${1%/*}"
    local file="${1#"$path"}"
    file="${file:1}"

    echo "$file"
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_RunCommandAndRedirect()
{
    local command=$1

    if [ "$_bg_IsVerbose" == true ]; then

        _bg_Log "Command: $command"

    fi

    local output
    output=$(eval "$command" 2>&1)
    local exitCode=$?

    if [ "$output" != "" ]; then

        _bg_Log "$output"

    fi


    if [ "$exitCode" != 0 ]; then

        _bg_Log "\e[31mFailed to run the command! Exit code: $exitCode"

    else 

        _bg_Log "\e[32mComplete!"

    fi
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_CreateIntermediateBin()
{
    local name=$1
    local type=$2
    local intermediateType="$type"
    local relativeToRoot=$3

    if [ "$relativeToRoot" == "" ]; then
    
        relativeToRoot="./"

    fi 

    if [ "$intermediateType" == "$_bg_ConstBuildTypeShip" ]; then

        intermediateType=$_bg_ConstBuildTypeRelease

    fi

    type=$(_bg_ToUpper "${type:0:1}")${type:1}
    intermediateType=$(_bg_ToUpper "${intermediateType:0:1}")${intermediateType:1}

    if  ! mkdir -p "$relativeToRoot/Engine/Intermediate/Object/$name/$intermediateType/" ||
        ! mkdir -p "$relativeToRoot/Bin/$name/$type/"; then

        exit 1
        
    fi
}

# ----------------------------------------------------------------------------------------------------------------------
_bg_CreateIntermediateLib()
{
    local name=$1
    local type=$2
    local relativeToRoot=$3

    if [ "$relativeToRoot" == "" ]; then
    
        relativeToRoot="./"

    fi 

    if [ "$type" == "$_bg_ConstBuildTypeShip" ]; then

        type=$_bg_ConstBuildTypeRelease

    fi

    type=$(_bg_ToUpper "${type:0:1}")${type:1}

    if  ! mkdir -p "$relativeToRoot/Engine/Intermediate/Object/$name/$type/" ||
        ! mkdir -p "$relativeToRoot/Engine/Intermediate/Lib/$name/$type/"; then

        exit 1
        
    fi
}

# ----------------------------------------------------------------------------------------------------------------------
Log()
{
    local log=$1

    if ! [ -e "$g_PathToLogScript" ]; then

        echo "$g_PathToLogScript doesn't exists!"
        echo "Last message: $log"
        exit 1

    fi

    echo -e "$log" | "$g_PathToLogScript" "$g_PathToLog"
}

# ----------------------------------------------------------------------------------------------------------------------
CheckIfInRootDir()
{
    local currentDir
    local relativeDir="/Engine/Build/Linux/"
    currentDir=$(pwd)


    if [ -e "$currentDir$relativeDir/Log.sh"  ] && 
       [ -e "$currentDir$relativeDir/Compile.sh" ] && 
       [ -e "$currentDir$relativeDir/Link.sh" ]; then

        return 0

    else

        return 1

    fi
}

# ----------------------------------------------------------------------------------------------------------------------
CheckDependencies()
{
    for dep in "${g_Dependencies[@]}"; do

        if ! command -v "$dep" >/dev/null 2>&1; then

            Log "$dep could not be found"
            return 1

        fi

    done

    return 0
}

# ----------------------------------------------------------------------------------------------------------------------
CheckIsSwitch()
{
    if [[ "$1" == "--"* ]]; then

        echo "true"

    else 

        echo "false"

    fi
}

# ----------------------------------------------------------------------------------------------------------------------
CheckIfArrayOfArgumnets()
{
    if [[ "$1" == "-"* ]]; then

        echo "true"

    else 

        echo "false"

    fi
}

# ----------------------------------------------------------------------------------------------------------------------
StripDashesFromFront()
{
    if [ "${#1}" -le 2 ]; then
        
        return 0

    fi

    echo "${1#*--}"
}


# ----------------------------------------------------------------------------------------------------------------------
HandleSwitch() 
{
    local switch=""
    switch=$(StripDashesFromFront "$1")
    switch=$(_bg_ToLower "$switch")

    _bg_DebugLog "Parsing switch '$switch'"

    if [ "$switch" == "sample" ]; then

        g_IsSample=true
        return 0

    fi

    if [ "$switch" == "debug" ]; then

        g_IsDebug=true
        g_Macros="_DEBUG $g_Macros"
        _bg_BuildType=$_bg_ConstBuildTypeDebug
        return 0

    fi

    if [ "$switch" == "tests" ]; then

        g_IsDebug=true
        g_Macros="_TESTS _DEBUG $g_Macros"
        _bg_BuildType=$_bg_ConstBuildTypeTests
        return 0

    fi

    if [ "$switch" == "testsonly" ]; then

        g_IsDebug=true
        g_Macros="_TESTS _TESTS_ONLY _DEBUG $g_Macros"
        _bg_BuildType=$_bg_ConstBuildTypeTests
        return 0

    fi

    if [ "$switch" == "ship" ]; then

        g_IsDebug=true
        _bg_BuildType=$_bg_ConstBuildTypeShip
        return 0

    fi

    if [ "$switch" == "customtargetpath" ]; then

        g_IsCustomTargetPath=true
        return 0

    fi

    if [ "$switch" == "help" ]; then

        _bg_ShowHelp
        exit 0

    fi

    if [ "$switch" == "verbose" ]; then

        _bg_IsVerbose=true
        return 0

    fi

    if [ "$switch" == "ccomands" ]; then

        _bg_IsCompileCommandsJson=true
        _bg_IsForceBuild=true
        return 0

    fi

    if [ "$switch" == "force" ]; then
        
        _bg_IsForceBuild=true

    fi

    if [ "$switch" == "forcepurge" ]; then
        
        eval "./Purge.sh all"

    fi

    return 1
}

# ----------------------------------------------------------------------------------------------------------------------
HandleArrayOfArguments() 
{
    local array=$1

    _bg_DebugLog "Parsing an array of arguments '$array'"

    _bg_Panic "HandleArrayOfArguments isn't implemented yet"
}

# ----------------------------------------------------------------------------------------------------------------------
HandleArgument() 
{
    local argument="$1"
    local isSwitch=0

    if [ "$g_IsCustomTargetPath" == "true" ] && [ "$_bg_PathToTargetSource" == "" ]; then
    
        _bg_PathToTargetSource=$argument
        return 0

    fi

    isSwitch=$(CheckIsSwitch "$argument")

    if [ "$(CheckIfArrayOfArgumnets "$argument")" == false ] && [ "$isSwitch" == false ]; then
        
        return 1

    fi 

    if [ "$isSwitch" == false ]; then

        (HandleArrayOfArguments "$argument")
        return 0

    fi

    HandleSwitch "$argument"
}

# ----------------------------------------------------------------------------------------------------------------------
InitializeCompileCommandsGathering()
{
    echo -n "[" > "$_bg_CompileCommandsJsonPath"
}

# ----------------------------------------------------------------------------------------------------------------------
FinishCompileCommandsGathering()
{
    echo "" >> "$_bg_CompileCommandsJsonPath"
    echo "]" >> "$_bg_CompileCommandsJsonPath"
}

# ----------------------------------------------------------------------------------------------------------------------
CreateResources()
{
    _bg_Log "Creating resources for '$_bg_TargetName'..."

    _bg_RootPath=$(pwd)

    if [ "$g_IsCustomTargetPath" == "false" ]; then

        _bg_PathToTargetSource=$_bg_RootPath

        if [ $g_IsSample ]; then

            _bg_Log "Building a sample..."
            _bg_PathToTargetSource=$_bg_PathToTargetSource/Samples/

        else 

            _bg_PathToTargetSource=$_bg_PathToTargetSource/Projects/

        fi

    else
        
        _bg_Log "Building with custom path to target..."

    fi

    
    _bg_PathToTargetSource=$_bg_PathToTargetSource$_bg_TargetName/

    _bg_Log "Path to target's source '$_bg_PathToTargetSource'"
    

    if  ! _bg_CreateIntermediateBin "$_bg_TargetName" "$_bg_BuildType" ||
        ! mkdir -p "./Engine/Build/Linux/Logs/"; then

        exit 1
        
    fi

    export -f _bg_ShowHelp
    export -f _bg_Panic
    export -f _bg_Log
    export -f _bg_BeginDebugging
    export -f _bg_EndDebugging
    export -f _bg_RunCommandAndRedirect
    export -f _bg_CreateIntermediateBin
    export -f _bg_CreateIntermediateLib
    export -f _bg_StripPathFromFile
    export -f _bg_StripExtensionFromFile
    export -f _bg_CheckIfInBuildLinuxDir
    export -f _bg_ToLower
    export -f _bg_ToUpper
    export -f _bg_DebugLog
    export _bg_IsVerbose
    export _bg_IsForceBuild
    export _bg_IsCompileCommandsJson
    export _bg_CompileCommandsJsonPath
    export _bg_TargetName
    export _bg_PathToTargetSource
    export _bg_BuildType
    export _bg_RootPath
    export _bg_ConstBuildTypeDebug
    export _bg_ConstBuildTypeRelease
    export _bg_ConstBuildTypeShip
    export _bg_ConstBuildTypeTests
    export _bg_CppCompilator
    export _bg_StaticLibraryCompilator
    export _bg_CompileFlags
    export _bg_ConstExecutableBinaryType
    export _bg_ConstDynamicLibBinaryType
    export _bg_ConstStaticLibBinaryType
}



#-#########-#
#- Globals -#
#-#########-#


# 0 - Disables debugging
# 1 - Enables _bg_DebugLog() 
# 2 - Enables `set -x`
g_DebugMode=0

g_Dependencies=("g++" "find")

g_IsSample=false
g_IsDebug=false
g_IsCustomTargetPath=false

g_PathToLog="./Engine/Build/Linux/Logs/Builder.log"
g_PathToLogScript="./Engine/Build/Linux/Log.sh"

g_BuildDirRelativePathToBin="../../../Bin/"
g_BuildDirRelativePathToEngineModules="../../Modules/"
g_BuildDirRelativePathToLib="../../Intermediate/Lib/"

g_Macros=""
g_EngineModules=""
g_PublicIncludes=""
g_Headers=""
g_EngineModuleBuildType=""

_bg_CppCompilator="g++"
_bg_StaticLibraryCompilator="ar"

_bg_IsVerbose=false
_bg_IsForceBuild=false
_bg_IsCompileCommandsJson=false
_bg_CompileCommandsJsonPath="$(pwd)/compile_commands.json"

_bg_TargetName=""
_bg_PathToTargetSource=""
_bg_RootPath=""
_bg_ConstBuildTypeDebug="Debug"
_bg_ConstBuildTypeRelease="Release"
_bg_ConstBuildTypeShip="Shipping"
_bg_ConstBuildTypeTests="Tests"
_bg_BuildType=$_bg_ConstBuildTypeRelease
_bg_CompileFlags="-Wall -Wextra -Wpedantic -Werror -std=c++17"
_bg_ConstExecutableBinaryType="exe"
_bg_ConstDynamicLibBinaryType="dlib"
_bg_ConstStaticLibBinaryType="slib"





#-########-#
#- Script -#
#-########-#

_bg_BeginDebugging

for arg in "$@"
do 
    if ! HandleArgument "$arg"; then 
    
        _bg_TargetName="$arg"

    fi
done

if ! mkdir -p "./Engine/Build/Linux/Logs/"; then

    _bg_Panic "Couldn't create logs directory"

fi

if ! CheckDependencies; then

    _bg_Panic "Cannot proceed, because not every dependency is satisfied"

fi

if ! CheckIfInRootDir; then

    _bg_Panic "Please run the script in root directory of the engine"

fi

if [ "$_bg_BuildType" == "$_bg_ConstBuildTypeRelease" ] || [ "$_bg_BuildType" == "$_bg_ConstBuildTypeShip" ]; then

    _bg_CompileFlags="$_bg_CompileFlags -O2"

fi


_bg_Log "-------------------------------------------- $(date +"%D %T") -------------------------------------------------------"

if [ "$_bg_TargetName" == "" ]; then

    _bg_ShowHelp

fi

if ! CreateResources; then

    _bg_ShowHelp
    _bg_Panic "Couldn't create resources"

fi

if ! [ -e "$_bg_PathToTargetSource" ]; then

    _bg_Panic "Path to target doesn't exists ($_bg_PathToTargetSource)!"

fi


if [ "$_bg_BuildType" != "$_bg_ConstBuildTypeDebug" ]; then

    _bg_CompileFlags="$_bg_CompileFlags -O2"

fi


cd ./Engine/Build/Linux/ || _bg_Panic
g_RelativePathToLog="./Logs/$(_bg_StripPathFromFile "$g_PathToLog")"

g_EngineModuleBuildType=$_bg_BuildType
if [ "$g_EngineModuleBuildType" == "$_bg_ConstBuildTypeShip" ]; then

    g_EngineModuleBuildType=$_bg_ConstBuildTypeRelease

fi

g_EngineModules="$(cat "$g_BuildDirRelativePathToEngineModules/Engine.dep")"
if [ "$g_EngineModules" == "" ]; then

    _bg_Panic "'ListDirs.sh' didn't work"

fi

_bg_DebugLog "Found this modules: $g_EngineModules"

if [ "$_bg_IsCompileCommandsJson" == true ]; then

    InitializeCompileCommandsGathering

fi


for module in $g_EngineModules; do

    _bg_Log "Building \e[34m$module\e[0m..."

    if ! _bg_CreateIntermediateLib "$module" "$_bg_BuildType" "../../../"; then

        _bg_Panic "Couldn't create intermediate directories for $module"

    fi

    g_PublicIncludes="-I$g_BuildDirRelativePathToEngineModules$module/Public/ $g_PublicIncludes"
    if [ "$_bg_IsForceBuild" == false ]; then
            
        g_Headers=$(find "$g_BuildDirRelativePathToEngineModules$module/" -name "*.h")
        while IFS= read -r header; do

            _bg_DebugLog "$header"

            if [ "$header" -nt "$g_BuildDirRelativePathToBin/$_bg_TargetName/$_bg_BuildType/Executable" ]; then 

                _bg_DebugLog "We need to force build from now on"

                _bg_IsForceBuild=true
                export _bg_IsForceBuild

            fi

        done <<< "$g_Headers"

    fi

    if [ "$g_EngineModuleBuildType" == "$_bg_ConstBuildTypeTests" ]; then

        g_PublicIncludes="-I$g_BuildDirRelativePathToEngineModules$module/ $g_PublicIncludes"

    fi

    bash ./Compile.sh "$g_BuildDirRelativePathToEngineModules$module/" "$module" "$g_PublicIncludes" "$g_Macros _EXPORT"

    bash ./Link.sh "$module" "$_bg_ConstDynamicLibBinaryType" "$(cat "$g_BuildDirRelativePathToEngineModules$module/Module.dep")"

    if [ "$_bg_BuildType" == "$_bg_ConstBuildTypeShip" ]; then

        cp "$g_BuildDirRelativePathToLib$module/$g_EngineModuleBuildType"/* "$g_BuildDirRelativePathToBin$_bg_TargetName/$_bg_BuildType/"

    fi

done

_bg_Log "Building \e[34m$_bg_TargetName\e[0m..."

g_PublicIncludes="-I""$_bg_PathToTargetSource""Public $g_PublicIncludes"
bash ./Compile.sh "$_bg_PathToTargetSource" "$_bg_TargetName" "$g_PublicIncludes" "$g_Macros"

if [ "$_bg_IsCompileCommandsJson" == true ]; then

    FinishCompileCommandsGathering

fi

bash ./Link.sh "$_bg_TargetName" "$_bg_ConstExecutableBinaryType" "$g_EngineModules"


_bg_EndDebugging
