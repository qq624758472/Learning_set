#!/bin/bash

declare -i i=1
while ((i<=446))
do
    echo -e -n "\xaa" >> ./main.bin
    let i++
done
echo -e -n "\x55" >> ./main.bin
echo -e -n "\xaa" >> ./main.bin
