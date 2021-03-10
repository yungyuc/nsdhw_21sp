#!/bin/bash

if [[ $# < 1 ]]; then
    echo "missing file name"

    exit
elif [[ $# > 1 ]]; then
    echo "only one argument is allowed"

    exit
fi

FILENAME=$1

if [[ -e $FILENAME ]]; then
    echo "$(grep -e '.*' -c $FILENAME) lines in $FILENAME"
else
    echo "$FILENAME not found"
fi
