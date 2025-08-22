#!/bin/bash

#-###########-#
#- Functions -#
#-###########-#

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

Panic()
{
    echo "Error! Cound't finish building the project due error (Optional message: $1)"
    exit 1
}

DeleteLogs()
{
    logs="$(find "./" -name "*.log")"

    while IFS= read -r line; do

        rm "$line"

    done <<< "$logs"

    rm -r "./Engine/Build/Linux/Logs/"
}

DeleteAll()
{
    DeleteLogs

    rm -r "./Bin/" 2> /dev/null
    rm -r "./Engine/Intermediate/" 2> /dev/null
}


#-########-#
#- Script -#
#-########-#

if ! CheckIfInRootDir; then

    Panic "Please run the script in root directory of the engine"

fi

if [ "$1" == "logs" ]; then

    DeleteLogs
    exit 0

fi

if [ "$1" == "all" ]; then

    DeleteAll
    exit 0

fi

question="Would you like to delete everything from 'Bin/...' and 'Engine/Intermediate/...' directory including logs? [y/N] "
echo -n "$question"
while IFS= read -r line; do

    if [ "$(echo "${line[0]}" | tr "[:upper:]" "[:lower:]")" = "n" ]; then 
    
        echo "Okay... :))"
        break

    fi

    if [ "$(echo "${line[0]}" | tr "[:upper:]" "[:lower:]")" = "y" ]; then 
        
        echo "Deleting..."
        DeleteAll
        echo "Done!"
        break

    fi
    echo -n "$question"

done

