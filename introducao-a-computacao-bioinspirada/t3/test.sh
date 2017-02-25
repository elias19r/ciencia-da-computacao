#!/bin/bash

# Shell script to run ./aco-tsp many times int order to check errors
ERR=1
MAX=1000
COUNT=0
while [ $COUNT -lt $MAX ]; do
	echo ""
	echo ">> running ("$COUNT")"
	./aco-tsp -t 2 -n 10 -k 10
	if [ $? -eq $ERR ]; then
		echo ">> error: exit failure occurred"
		exit 0
	fi
	let COUNT=COUNT+1
done
