#!/usr/bin/env bash

if [[ $"$#" -lt 1 ]]; then
     echo 'missing file name'

elif [[ $"$#" -gt 1 ]]; then
     echo 'only one argument is allowed'
else
     filename=$1

     if [[ -f $filename ]]; then
         echo "$(wc -l $filename) lines in $filename"
     else
         echo "$filename not found"
     fi
 fi
