#!/bin/bash

array=("out.ca-cit-HepPh.uniq")
# array=("toy")
dir="../data"

echo ${#array[@]}
for ((i=0;i<${#array[@]};i++))
do
	file=${array[$i]}
	echo $file
    ./topchain 5 ${dir}/${file}_scc.txt ${dir}/${file}_query.txt ${dir}/${file}_info.txt 
done

