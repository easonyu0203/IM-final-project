#!/bin/bash

#give the result of 10 play
for i in {1..1000}
do 
	echo "testing $i"
	make play
done

make clean
g++ get_avg.cpp -o get_avg
./get_avg
rm get_avg