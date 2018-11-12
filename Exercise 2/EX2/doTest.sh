#!/bin/bash
# Speed_Up script
# Note: This script evaluates the efficiency of a parallel solution against a sequential one
echo "====== Speed_up: Efficiency meter ======"
option=$1
inputName=$2
./CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $inputName
seqtime=`grep "seconds" $inputName.res | cut -d = -f2 |cut -d s -f1`
echo "threads, exec_time, speedup" > $inputName.speedup.csv
echo "1S, "$seqtime", 1" >> $inputName.speedup.csv
for i in $(seq 1 $option)
do
	./CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $inputName
	partime=`grep "seconds" $inputName.res | cut -d = -f2 |cut -d s -f1`
	speedup=$(echo "scale=6; ${seqTime}/${parTime}" | bc)
	echo "$var,$parTime,$speedup" >> $inputName.speedup.csv
	var=$((var+1))
done
echo "====== Leaving script... ======"
