main: main.o Snake.o
	g++ main.o Snake.o -std=c++11 -o main

main.o: main.cpp
	g++ -c main.cpp -std=c++11 -o main.o

Snake.o: Snake.cpp Snake.h func.cpp
	g++ -c Snake.cpp -std=c++11 -o Snake.o

maps:
	g++ mapGenerate1_100.cpp -std=c++11 -o map1_100
	g++ mapGenerate101_200.cpp -std=c++11 -o map101_200
	./map1_100
	./map101_200

play:
	make maps
	make main
	./main AAA

clean:
	rm -rf main.o Snake.o test main map1_100 map101_200 trivial
