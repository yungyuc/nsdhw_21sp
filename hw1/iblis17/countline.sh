#!/usr/bin/env bash


if [ $(echo "$# < 1" | bc) = '1' ]
then
    echo 'missing file name'

elif [ $(echo "$# > 1" | bc) = '1' ]
then
    echo 'only one argument is allowed'
else
    fname=$1
    echo $fname

    if [ -e $fname ]
    then
        echo "$(wc -l $fname) lines in $fname"
    else
        echo "$fname not found"
    fi
fi
