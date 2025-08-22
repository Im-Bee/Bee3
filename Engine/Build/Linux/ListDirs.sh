#!/bin/bash

#-#########-#
#- Globals -#
#-#########-#

g_RootDir="$1"
g_Dirs=""
g_FormatedDirs=""



#-########-#
#- Script -#
#-########-#

cd "$g_RootDir" || exit 1
g_Dirs=$(ls -d *)

if [ "$g_Dirs" == "" ]; then

    exit 1

fi

echo "$g_Dirs"
exit 0
