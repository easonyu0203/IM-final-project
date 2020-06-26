#pragma once

#include<iostream>
#include<vector>
#include<tuple>
#include<set>
#include<unordered_set>
#include<stack>
#include<algorithm>
#include<utility>
#include<exception>
#include"Snake_test.h"

#define MIN_HEAD_VISION_PERCENT 80

//find fruit position
std::set<std::tuple<int, int>, std::greater<std::tuple<int, int>>> find_fruit_positions(const std::vector<std::vector<int>>& map);

//make a vector contain snake position with tail at index 1 and head at index size()-1
std::vector<std::tuple<int, int>> make_vec_snake_position(const std::queue<std::tuple<int, int>> q_position);

//return a step that is valid to go, if there isn't any, then return random step
std::tuple<int, int> random_step(const  std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>> snake_positions);

class Cells_map{
	private:
		struct Cell{
			std::tuple<int, int> parent;
			int real_cost;
			int heuristic_cost;
			int total_estimate_cost;
			int step_count_to_here;
		};
	private:
		std::vector<std::vector<Cell>> data_container;
		std::tuple<int, int> scr;
		std::tuple<int, int> des;
	public:
		Cells_map(std::tuple<int, int> scr, std::tuple<int, int> des, int map_length);
		void update(std::tuple<int, int> successor, std::tuple<int, int> parent);
};

//determine a point is valid to go or not
//hit wall, hit snake itself, head's vision is > MIN_HEAD_VISION_PERCENT
bool big_head_vision(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent,\
	const std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& snake_positions, const Cells_map& cells_data_map);

//find valid shortest path from scr to des
//throw logic error if not find a path
std::stack<std::tuple<int, int>> shortest_path_finder(const std::tuple<int, int>& scr, const std::tuple<int, int>& des,\
	const std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& snake_positions,\
	bool (*is_valid_successor)(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent,\
	const std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& snake_positions, const Cells_map& cells_data_map));

//find valid longest path form scr to dex
//throw logic error if not find a path
std::stack<std::tuple<int, int>> longest_path_finder(const std::tuple<int, int>& scr, const std::tuple<int, int>& des,\
	const std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& snake_posisitions);