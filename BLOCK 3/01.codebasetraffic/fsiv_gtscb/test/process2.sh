#!/bin/bash

set -eu

cat metadada.csv | while read line ; do 
   img="$(echo ${line}v | cut -d\; -f1)"
   cl="$(grep ${img} GT.TXT | cut -d: -f2)"
   echo "${line};${cl}"
done
