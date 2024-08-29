#!/bin/bash

binary_path=$1
dll_folder=$2
depth=$3
out_path=$(dirname $binary_path)

dlls=$(objdump -p "$binary_path" | grep -i "DLL Name" | awk '{print $3}')
echo "$dlls" | xargs -n1 -I{} cp "$dll_folder/{}" "$out_path"

if [ "$depth" -eq 0 ]; then
    exit 0
fi

echo "$dlls" | xargs -n1 -I{} ./grab_dlls.sh "$out_path/{}" "$dll_folder" "$((depth-1))"
