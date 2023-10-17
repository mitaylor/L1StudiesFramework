#!/bin/bash

menu=$1
seeds=( $( pcregrep -M "<algorithm>*\n.*<name>" $menu ) )
bits=( $( grep "<index>" $menu ) )
length=$( expr ${#seeds[@]} - 1 )

for i in $( seq 0 $length )
do
    if [ $(( $i % 2 )) -eq 1 ]
    then
        j=$(( ($i - 1) / 2 ))
        seed=$(echo ${seeds[$i]} | sed 's/<\/*name>//g')
        bit=$(echo ${bits[$j]} | sed 's/<\/*index>//g')
        echo "{$bit, \"$seed\"},"
        # echo "{\"$seed\", $bit},"
    fi
done