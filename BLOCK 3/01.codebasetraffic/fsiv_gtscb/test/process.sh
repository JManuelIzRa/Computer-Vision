#!/bin/bash

set -eu

cat CLASSES.TXT | while read line; do 
	CL=$(echo $line|cut -d: -f1); 
	SAMPLES=$(echo $line|cut -d: -f2); 
	for s in $SAMPLES ; do 
		img=$(echo $s | awk '{printf("%05d\n", $1)}')
		echo "${img}.ppm:$CL" 
	done ; 
done
