#!/usr/bin/env bash
if [ $# -eq 1  ]; then
    if [ -e $1 ]; then
        lines=$(cat "$1" | wc -l)
        echo "$lines lines in $1";
    else
        echo "$1 not found"
    fi
elif [ $# -eq 0 ]; then
    echo "missing file name";
else
    echo "only one argument is allowed";
fi
