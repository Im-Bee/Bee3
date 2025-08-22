#!/bin/bash

#-###########-#
#- Functions -#
#-###########-#

# ----------------------------------------------------------------------------------------------------------------------
InsertCompileCommand()
{
    local command=$1
    local file=$2

    echo -n "
    {
        \"directory\": \"$g_CurrentDir\",
        \"command\": \"$command\",
        \"file\": \"$file\"
    }," >> "$_bg_CompileCommandsJsonPath"
}

# ----------------------------------------------------------------------------------------------------------------------
GatherPreprocesorMacros()
{
    if [ "$g_PreprocesorMacros" == "" ]; then
     
        echo ""
        return

    fi

    local macros=$1
    local interpretationForCmd=""

    while read -r macro; do
        
        interpretationForCmd="$interpretationForCmd -D$macro"

    done < <(xargs -n1 <<< "$macros")

    echo "$interpretationForCmd"
}

# ----------------------------------------------------------------------------------------------------------------------
ModuleCompile()
{
    local compilerBuildType=$_bg_BuildType

    _bg_Log "Compiling \e[36mmodule $g_TargetName\e[0m ($g_PathToTarget)..."
    
    local macros
    macros=$(GatherPreprocesorMacros "$g_PreprocesorMacros")
    
    
    if [ "$compilerBuildType" == "$_bg_ConstBuildTypeShip" ]; then
    
        compilerBuildType=$_bg_ConstBuildTypeRelease

    fi
    compilerBuildType=$(_bg_ToUpper "${compilerBuildType:0:1}")${compilerBuildType:1}


    local sourceFiles
    sourceFiles=$(find "$g_PathToTarget""Private/" -name "*.cpp")
    if [ "$_bg_BuildType" == "$_bg_ConstBuildTypeTests" ]; then 

        sourceFiles="$sourceFiles"$'\n'"$(find "$g_PathToTarget""Tests/" -name "*.cpp" 2>/dev/null)" 

    fi
    local objectsArray=()


    echo -ne "\e[32m"
    local objectOutPath
    local command
    while IFS= read -r filePath; do

        if [ "$filePath" == "" ]; then
            
            continue

        fi

        file=$(_bg_StripPathFromFile "$filePath")
        objectFile=$(_bg_StripExtensionFromFile "$file").o
        objectOutPath="$g_RelativePathToObjects$g_TargetName/$compilerBuildType/$objectFile"

        _bg_DebugLog "Includes: $g_PublicIncludes"
        command="$_bg_CppCompilator $macros $g_PublicIncludes $g_CompileFlags -c -fPIC $filePath -o $objectOutPath"

        if [ "$_bg_IsCompileCommandsJson" == true ]; then
            
            InsertCompileCommand "$command" "$filePath"

        fi
        
        if [ "$objectOutPath" -nt "$filePath" ] && [ "$_bg_IsForceBuild" == false ]; then
    
            _bg_Log "'$filePath' ready."
            continue

        fi

        _bg_Log "\e[32mCompiling '\e[0m$filePath\e[32m' to '\e[0m$objectFile\e[32m'..."

        _bg_RunCommandAndRedirect "$command"

        objectsArray+=("$objectFile")

    done <<< "$sourceFiles"
}



#-#########-#
#- Globals -#
#-#########-#


# 0 - Disables debugging
# 1 - Enables _bg_DebugLog() 
# 2 - Enables `set -x` 
g_DebugMode=0
g_RelativePathToLog="./Logs/Compiler.log"

g_CurrentDir=$(pwd)
g_RelativePathToIntermediate="../../Intermediate/"
g_RelativePathToObjects="$g_RelativePathToIntermediate""Object/"

g_PathToTarget=""
g_TargetName=""
g_PublicIncludes=""
g_PreprocesorMacros=""


#-########-#
#- Script -#
#-########-#

_bg_BeginDebugging

_bg_Log "-------------------------------------------- $(date +"%D %T") -------------------------------------------------------" 1> /dev/null

if ! _bg_CheckIfInBuildLinuxDir; then

    _bg_Panic "Please run the Build.sh script in root directory of the engine"

fi

_bg_Log "Compiling..."

g_PathToTarget=$1
g_TargetName=$2
g_PublicIncludes=$3
g_PreprocesorMacros=$4

ModuleCompile 

_bg_EndDebugging
