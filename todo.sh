#!/bin/bash

#
# Searches for all TODOs and tosses them in a file.
#
TODO_COUNT=0
rm -f TODOS
touch TODOS

for file in $(find {src,Shaders,Scripts} -type f -name '*' -print)
do
	echo "########################################" >> TODOS
	echo $file >> TODOS
	echo "========================================" >> TODOS
	grep -n -B 5 -A 5 "TODO" $file | sed s/--/========================================/g >> TODOS
	TODO_COUNT=$((TODO_COUNT+$(grep -c "TODO" $file)))
done

echo "Found" $TODO_COUNT "TODOs."
