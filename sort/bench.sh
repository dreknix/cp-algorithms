#!/usr/bin/env bash

TIME="/usr/bin/time"
TIME_FORMAT="--format=%M KB %e seconds"


for generator in "rand" "dup" "equal" "sort"
do
    echo ""
    echo "Generator = ${generator}"
    echo "  n   |       Quicksort      |      MyQuicksort     |"
    for n in 1000 5000 10000 50000 100000 200000
    do
        printf "%6d|" "${n}"
        for exe in "./sort-quicksort" "./sort-myquicksort"
        do
            # shellckeck SC2181
            if ! result="$("${TIME}" "${TIME_FORMAT}" "${exe}" "${generator}" "${n}" 2>&1)"
            then
                result="Killed by signal    "
            fi
            printf " %s |" "${result}"
        done
        printf "\n"
    done
done
