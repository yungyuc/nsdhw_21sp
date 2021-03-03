if [ $# -eq 1  ]; then
    if [ -e $1 ]; then
        lines=$(awk 'END{print NR}' "$1")
        echo "$lines lines in $1";
    else
        echo "$1 not found"
    fi
elif [ $# -eq 0 ]; then
    echo "missing file name";
else
    echo "only one argument is allowed";
fi
