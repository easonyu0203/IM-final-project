#!/bin/bash

#give the result of 10 play
rm -f testing_data.txt
make clean
for i in {1..10}
do 
	echo "testing $i"
	make play
done

make clean
g++ get_avg.cpp -o get_avg
./get_avg
rm get_avg