#!/bin/bash
# Speed_Up script
# Note: This script evaluates the efficiency of a parallel solution against a sequential one
echo "====== Speed_up: Efficiency meter ======"
option=$1
inputName=$2
echo "$inputName"
execute="."`pwd`"/$output"
seqTime="time"
seqTime=time ./CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $inputName -R 2>&1 | grep \^user
echo "test" > "$inputName.res"
echo "threads, exec_time, speedu" > "$inputName.res"
echo "1S, "$seqTime", 1" > "$inputName.res"
var=1
while [ $var -le $option ]
do
  parTime="time (./CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $var $inputName) -R 2>&1 | grep \^user"
  speedup=$(echo "scale=6; ${seqTime}/${parTime}" | bc)
  echo "$var,$parTime,$speedup" > "$inputName.res"
	var=$((var+1))
done
echo "====== Leaving script... ======"