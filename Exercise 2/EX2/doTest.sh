#!/bin/bash
# Speed_Up script
# Note: This script evaluates the efficiency of a parallel solution against a sequential one
echo "====== Speed_up: Efficiency meter ======"
option=$1
inputName=$2
seqTime="time (./CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $inputName) -R 2>&1 | grep \^user"# execute sequential
echo $seqTime
echo "#threads, exec_time, speedup"
echo "1S, $seqTime, 1"
var=1
while [ $var -le $option ]
do # Runs ParSolver with 1 to n threads, n times
  parTime="time (./CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $var $inputName) -R 2>&1 | grep \^user"
  speedup=$(echo "scale=6; ${seqTime}/${parTime}" | bc)
  echo "$var,$parTime,$speedup"
	var=$((var+1))
done
echo "====== Leaving script... ======"
