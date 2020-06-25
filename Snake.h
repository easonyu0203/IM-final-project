#include <queue>
#include<set>
#include <tuple>
#include <vector>
#include<cmath>
#include<deque>
#include<stack>
#include<utility>
#include<iostream>


class Snake {
private:
	//back() = head, front() = tail
	std::queue<std::tuple<int, int> > position;
	// std::vector<std::vector<int> > hamilton_map;
private:
	std::pair<std::stack<std::pair<int, int> >, bool> a_star_pathfinding(std::pair<int, int> src, std::pair<int, int> des, std::vector<std::vector<int> > map);
	std::stack<std::pair<int, int> > take_moves;
	// bool is_valid_by_hamilton_order(std::pair<int, int>);
	
	
public:
	// Don't edit interface
	Snake(std::queue<std::tuple<int, int>> startPosition);
	std::queue<std::tuple<int, int>> nextPosition(std::vector<std::vector<int>> map);
	
	// Add anything else you need
	std::queue<std::tuple<int, int>>& get_position(){return position; } 
};

