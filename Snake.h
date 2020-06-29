#pragma once
#include <queue>
#include<set>
#include <tuple>
#include <vector>
#include<cmath>
#include<deque>
#include<stack>
#include<utility>
#include<iostream>
#include<exception>

#define UN_FRUIT 10
#define UN_FRUIT_WEIGHT 10000000

class Snake {
private:
	//back() = head, front() = tail
	//snake positions
	std::queue<std::tuple<int, int>> position;
	//greedy mode
	std::stack<std::tuple<int, int>> scheduled_steps;
	//suvival mode
	std::stack<std::tuple<int, int>> backup_steps;
	//you are fuck up mode
public:
	// Don't edit interface
	Snake(std::queue<std::tuple<int, int>> startPosition);
	std::queue<std::tuple<int, int>> nextPosition(std::vector<std::vector<int>> map);
	std::queue<std::tuple<int, int>>& get_position(){return position; } 
	int size() const {return position.size();}
	const std::tuple<int, int>& head_pos() const {return position.back();}
	const std::tuple<int, int>& tail_pos() const {return position.front();}
};

