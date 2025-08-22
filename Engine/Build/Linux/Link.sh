#!/bin/bash

#-###########-#
#- Functions -#
#-###########-#

# ----------------------------------------------------------------------------------------------------------------------
DeductFinalBuildTypeForDirName()
{
    if [ "$g_CompiledBuildType" == "$_bg_ConstBuildTypeShip" ]; then

        g_CompiledBuildType=$_bg_ConstBuildTypeRelease

    else 

        g_CompiledBuildType=$g_BinaryBuildType

    fi
}

# ----------------------------------------------------------------------------------------------------------------------
GatherObjects()
{
    local targetPath=$1
    local objects=""

    listOfObjects=$(find "$targetPath" -name "*.o")
    while IFS= read -r objectFile; do
        
        objects="$objectFile $objects"

    done <<< "$listOfObjects"

    echo "$objects"
}

# ----------------------------------------------------------------------------------------------------------------------
GatherLibs()
{
    if [ "$g_LinkAgainst" == "" ]; then
    
        echo ""
        return

    fi

    local libs=$1
    local interpretationForCmd=""
    local path=""
    local libCmd=""

    for lib in $libs; do

        path="$(pwd)/$g_RelativePathToLib$lib/$g_CompiledBuildType/"
        libCmd="-L$path -Wl,-rpath,$path -l$lib"

        interpretationForCmd="$libCmd $interpretationForCmd"

    done 

    echo "$interpretationForCmd"
}

# ----------------------------------------------------------------------------------------------------------------------
Link()
{
    _bg_Log "Linking \e[36mmodule $g_TargetName\e[0m..."


    local outputFile
    local switches
    local command
    local logMessage


    local libs
    libs=$(GatherLibs "$g_LinkAgainst")
    _bg_DebugLog "Linking against: $libs"
    

    if [ "$g_BinaryType" == "$_bg_ConstExecutableBinaryType" ]; then

        outputFile=$g_RelativePathToBin$g_TargetName/$g_BinaryBuildType/Executable
        switches="-o"

        logMessage="\e[32mLinking executable in to $outputFile"

        command="$_bg_CppCompilator $libs $switches $outputFile"

    elif [ "$g_BinaryType" == "$_bg_ConstDynamicLibBinaryType" ]; then
        
        outputFile="$g_RelativePathToLib$g_TargetName/$g_CompiledBuildType/lib$g_TargetName.so"
        switches="-shared -o"

        logMessage="\e[32mLinking dynamic library in to $outputFile"

        command="$_bg_CppCompilator $libs $switches $outputFile"

    elif [ "$g_BinaryType" == "$_bg_ConstStaticLibBinaryType" ]; then

        outputFile="$g_RelativePathToLib$g_TargetName/$g_CompiledBuildType/lib$g_TargetName.a"
        switches="rsc"

        logMessage="\e[32mLinking static library in to $outputFile.a"

        command="$_bg_StaticLibraryCompilator $switches $outputFile"

    fi


    local targetPath=$g_RelativePathToObjects$g_TargetName/$g_CompiledBuildType/
    local objects=""
    local needToLink=false

    if [ "$g_LinkAgainst" != "" ] || [ "$_bg_IsForceBuild" == true ]; then

        needToLink=true

    fi

    listOfObjects=$(find "$targetPath" -name "*.o")
    while IFS= read -r objectFile; do
        
        objects="$objectFile $objects"
        
        if [ "$needToLink" == false ]; then

            if ! [ -e "$outputFile" ] && [ "$objectFile" -nt "$outputFile" ]; then

                needToLink=true

            fi

        fi

    done <<< "$listOfObjects"

    if [ "$needToLink" == false ]; then

        _bg_Log "$g_TargetName ready"
        return

    fi

    _bg_Log "$logMessage"
    _bg_RunCommandAndRedirect "$command $objects"
}


#-#########-#
#- Globals -#
#-#########-#

# 0 - Disables debugging
# 1 - Enables _bg_DebugLog() 
# 2 - Enables `set -x`
g_DebugMode=0
g_RelativePathToLog="./Logs/Linker.log"

g_RelativePathToBin="../../../Bin/"
g_RelativePathToIntermediate="../../Intermediate/"
g_RelativePathToLib="$g_RelativePathToIntermediate""Lib/"
g_RelativePathToObjects="$g_RelativePathToIntermediate""Object/"
g_RelativePathToEngineModules="../../Modules/"

g_CompiledBuildType=$_bg_BuildType
g_BinaryBuildType=$g_CompiledBuildType

g_TargetName=""
g_BinaryType=""
g_LinkAgainst=""

#-########-#
#- Script -#
#-########-#

_bg_BeginDebugging

_bg_Log "-------------------------------------------- $(date +"%D %T") -------------------------------------------------------" 1> /dev/null

if ! _bg_CheckIfInBuildLinuxDir; then

    _bg_Panic "Please run the Build.sh script in root directory of the engine"

fi

_bg_Log "Linking..."


g_EngineModules=$(./ListDirs.sh "$g_RelativePathToEngineModules")
if [ "$g_EngineModules" == "" ]; then
    
    _bg_Panic "'ListDirs.sh' didn't work"

fi


DeductFinalBuildTypeForDirName

g_TargetName=$1
g_BinaryType=$2
g_LinkAgainst=$3

Link 


_bg_EndDebugging
