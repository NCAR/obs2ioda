#!/bin/bash

# check arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <directory> <function_name>"
    exit 1
fi

directory=$1
function_name=$2

find "$directory" -name "*.f" -o -name "*.f90" | while read file; do
    grep -ne "subroutine ${function_name}(\|function ${function_name}(" "$file" && echo -e "\tFound in $file"
done
