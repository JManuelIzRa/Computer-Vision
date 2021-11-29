#!/bin/bash
set -eu

#class:tipo
# 00001:p prohibición
# 00011:t aviso
# 00012:d diamante
# 00013:T Ceda
# 00014:h Stop
# 00032:f fin prohibición
# 00033:o obligación.
function get_seq_num()
{
	name="$(basename ${1} .ppm)"
	seq_num=$(echo "$name"|cut -d_ -f2)
	echo "$seq_num"|bc -q
}

function rand()
{
	n=$(echo "scale=3; ${RANDOM}.0/32768.0 < ${1}" | bc -q)
	echo $n
}

mkdir -p fsiv_gtrb_train
echo "" > fsiv_gtrb_train/train_all.csv
echo "" > fsiv_gtrb_train/train_train.csv
echo "" > fsiv_gtrb_train/train_valid.csv

for class in 00001 00011 00012 00013 00014 00032 00033; do
	echo "Processing class $class ..."
	CLASS_PATH="GTSRB_Final_Training_Images/GTSRB/Final_Training/Images/$class"
	for img in $(ls $CLASS_PATH/*.ppm); do
		seq_num=$(get_seq_num $img)
		if [ $seq_num -ge 15 ] && [ $seq_num -le 25 ]; then
			name="$(basename $img)"
			metadata="$(grep $name $CLASS_PATH/GT-${class}.csv)"
			echo "${class}_${metadata}" >> fsiv_gtrb_train/train_all.csv
			if [ "$(rand 0.6)" == "1" ]; then
				echo "${class}_${metadata}" >> fsiv_gtrb_train/train_train.csv
			else
				echo "${class}_${metadata}" >> fsiv_gtrb_train/train_valid.csv
			fi
			cp -n ${img} fsiv_gtrb_train/${class}_${name}
		fi
	done
done
	 
