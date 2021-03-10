#!/bin/bash

argc=$#

if [ $argc -gt 1 ]; then
	echo "only one argument is allowed"
elif [ $argc -lt 1 ]; then
	echo "missing file name"
else
	fname=$1
	if [ ! -f ${fname} ]; then
		echo "${fname} not found"
	else
		line=$(wc -l <"${fname}" | sed -e 's/^[[:space:]]*//')
		echo "${line} lines in ${fname}"
	fi
fi


