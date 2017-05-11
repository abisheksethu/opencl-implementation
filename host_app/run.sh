#!/bin/bash

make clean

make

echo "Data logging for pthread device"

export POCL_DEVICES=pthread

for num1 in {1..16}
do
	export DATA_SIZE=$num1
	echo "DATA_SIZE: $DATA_SIZE K"
	for num2 in {1..10}
	do
	 echo "TEST: $num2"
	 ./pocl_test
	done
done

mv report.txt report_cpu.txt

echo "Data logging for xillybus device"
export POCL_DEVICES=xillybus

for num1 in {1..16}
do
	export DATA_SIZE=$num1
	echo "DATA_SIZE: $DATA_SIZE"
	for num2 in {1..10}
	do
	 echo "TEST: $num2"
	 ./pocl_test
	done
done

mv report.txt report_fpga.txt

exit
