#!/bin/bash
# Count lines in a file

if [ $# -eq 0 ];
    then
        echo "missing file name"
        exit 1
fi

if [ $# -gt 1 ];
    then
        echo "only one argument is allowed"
    exit 1
fi

if [ -f $1 ];
    then
        lines=$(wc -l < $1)
#         lines=$(grep -c ^ $1)
        
        echo "$lines lines in $1"
    else
        echo "$1 not found"
fi