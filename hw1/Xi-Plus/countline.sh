#!/bin/bash

if [ "$1" == "" ] ; then
    echo "missing file name"
elif [ "$2" != "" ] ; then
    echo "only one argument is allowed"
elif [ ! -f $1 ]; then
    echo "$1 not found"
else
    ANS=0
    while read -r line || [ -n "$line" ]; do
        ANS=$(($ANS+1))
    done < $1
    echo "$ANS lines in $1"
fi
