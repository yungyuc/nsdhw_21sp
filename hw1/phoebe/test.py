#!/bin/sh
"""": 
if type python3 > /dev/null 2>&1
then
    exec python3 "$0"
elif type python > /dev/null 2>&1
then
    exec python "$0"
else
    echo >&2 "Python not installed"
fi
exit 1
""" #"

print("Hello World")

