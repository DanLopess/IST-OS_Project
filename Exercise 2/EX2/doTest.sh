#!/bin/bash
# Speed_Up script
# Note: This script evaluates the efficiency of a parallel solution against a sequential one
echo "====== Speed_up: Efficiency meter ======"
option=$1
inputName=$2
$inputName.res
output=$inputName.speedup.csv
./CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $inputName
seqtime=$(grep "seconds" $inputName.res | cut -d = -f2 |cut -d s -f1)
echo "threads, exec_time, speedup" > $output
echo "1S, $seqtime, 1" >> $output
for i in $(seq 1 $option)
do
	./CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $inputName
	partime=$(grep "seconds" $inputName.res | cut -d = -f2 |cut -d s -f1)
	speedup=$(echo "scale=6; "${seqTime}/${partime} | bc)
	echo $i", "$parTime", "$speedup >> $output
done
echo "====== Leaving script... ======"
