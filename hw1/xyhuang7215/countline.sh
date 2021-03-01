#!/bin/bash
#
# this script counts how many lines in a file

# setup variable
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
FILE=$1
ARGVNUM=$#

if [ $ARGVNUM -lt 1 ]; then
        echo "missing file name"
elif [ $ARGVNUM -ge 2 ]; then
 	echo "only one argument is allowed"
elif [ -f $FILE ]; then 
	echo `wc -l < $1` "lines in $FILE"
else
	echo "$FILE not found"
fi
