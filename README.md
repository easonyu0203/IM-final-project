# IM Final Project Snake Game AI
This repository contain my work and some stuff that may help you debug and visualize your snake game, if you have any question about how to use it, please contact me, I usually won't help you because I'm super busy.

- [IM Final Project Snake Game AI](#im-final-project-snake-game-ai)
  - [Makefile usage](#makefile-usage)
  - [Testing Script Usage](#testing-script-usage)
  - [Algorithms](#algorithms)
  - [Very Helpful People](#very-helpful-people)

## Makefile Usage
First, open terminal and change directory to here, and make sure you have Snake.cpp and Snake.h in work directory.<br>
input following to terminal<br>
+ `make main`: produce executable file `main`
+ `make maps`: produce random 200 maps
+ `make play`: produce `main` and random 200 maps and execute automatically
+ `make clean`: clean all executable file

## Main.cpp Macro
in main.cpp there is some macro at line 20 to 30
+ `COUT`: change to 1 if want to print game and status to std output, otherwise change to 0
+ `COUT_RESULT`: print game result to std output if 1
+ `WRITE_RESULT_TO_FILE`: write game result to `testing_data.txt`
+ `SKIP_POINT`: fast forward unwanted map index, for example, `SKIP_POINT 50` mean skip 50 map index and only show game from index 51 to 200

## Testing Script Usage
precondition:
change main.cpp macro to:
```cpp
	#define WRITE_RESULT_TO_FILE 1
	#define COUT 0
```
input following to terminal<br>
+ `testing.sh`<br>
This will run `main` for 100 times with different random 200 maps, and will produce the avg winnig rate, and avg map index.<br>
The result will be written in `testing_data.txt`<br>
This might consume a lot of time!!!

## Algorithms
...This is a little bit hard to explain^^ and I don't have time :P

## Very Helpful People
Thank Timicienio for random map generator<br>
Thank Brian Chen for READ.md format and visualization<br>
