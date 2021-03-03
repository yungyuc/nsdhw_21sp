if [ $# -eq 1  ]; then
    lines=$(awk 'END{print NR}' "$1")
    echo "$lines lines in $1";
elif [ $# -eq 0 ]; then
    echo "missing file name";
else
    echo "only one argument is allowed";
fi