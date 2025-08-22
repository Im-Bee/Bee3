#!/bin/bash


#-#########-#
#- Globals -#
#-#########-#

g_OutFile=$1
g_Log=""


#-########-#
#- Script -#
#-########-#

while IFS= read -r line; do

    g_Log="\e[0m[\e[35m$(date +"%H:%M:%S.%3N")\e[0m]: $line"
    echo -e "$g_Log" | sed -r 's/\x1B\[[0-9;]*[mK]//g' >> "$g_OutFile"
    echo -e "$g_Log"

done 
