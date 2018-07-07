#!/bin/bash

array=("out.ca-cit-HepPh.uniq") 
array=("toy") 

echo ${#array[@]}
for ((i=0;i<${#array[@]};i++))
do
    temporalGraph=${array[$i]}
    echo $temporalGraph

    queryNum=1000

    scc=_scc.txt
    query=_query.txt
    in=_info.txt

    tscc=$temporalGraph$scc
    tquery=$temporalGraph$query
    tinfo=$temporalGraph$in

    #transform graph   create scc and info
    echo transfrom...
    ./tran $temporalGraph ./


    #create query
    echo create query...
    ./info $tinfo $tquery $queryNum
done
