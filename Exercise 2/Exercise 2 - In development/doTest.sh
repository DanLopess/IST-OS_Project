#!/bin/bash
# Speed_Up script
# Note: This script evaluates the efficiency of a parallel solution against a sequential one

clear
echo "Speed_up: Efficiency meter"

read option
read inputName

cd CircuitRouter-SeqSolver

seqTime = time ./CircuitRouter-SeqSolver $inputName # execute sequential
echo "#threads, exec_time, speedup"
echo "1S,$seqTime,1"

cd ../CircuitRouter-ParSolver

var = 1
while [ $var -le $option ]
do # Runs ParSolver with 1 to n threads, n times
  parTime = time ./CircuitRouter-ParSolver $var $inputName
  speedup = $(echo "scale=6; ${seqTime}/${parTime}" | bc)
  echo "$var,$parTime,$speedup"
	var = $((var+1))
done

echo "Leaving script..."
