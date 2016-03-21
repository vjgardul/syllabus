#!/bin/bash

#Make the whole pipe error if a command fails (timeout | clingo |  judge)
#set -e
set -o pipefail

function try
{
    for (( i = 2; i < 5; i++ )); do
        if timeout --kill-after=7 5 ./$1doku $i fast > $1.txt; then
            echo "success"
    	elif [[ "$?" = "124" ]]; then
    		echo "$1 failed to solve $i-doku"
            echo "failure"
        fi
    done

}

make

tput setaf 2
try n
tput setaf 3
try pruneN
tput setaf 4
try heuristicN
tput setaf 5
try fastN
tput setaf 9

# diff fastN.txt heuristicN.txt
