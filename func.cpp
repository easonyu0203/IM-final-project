#pragma once

#include<iostream>
#include<vector>
#include<tuple>
#include<set>
#include<unordered_set>
#include<stack>
#include<iomanip>
#include<algorithm>
#include<utility>
#include<exception>
#include<cmath>
#include"Snake.h"

#define MIN_HEAD_VISION_PERCENT 80
#define BASIC_WEIGHT 100
#define AROUND_SNAKE_WEIGHT 99
#define IN_SNAKE_WEIGHT 98

#define FUNC_DEBUG 0

//find fruit position
std::vector<std::tuple<int, int>> find_fruit_positions(const std::vector<std::vector<int>>& map);

//make a vector contain snake position with tail at index 1 and head at index size()-1
std::vector<std::tuple<int, int>> make_vec_snake_position(const std::queue<std::tuple<int, int>>& q_position);

//return a step that is valid to go, if there isn't any, then return random step
std::tuple<int, int> random_step(const  std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& snake_positions);

class Cells_map{
	private:
		struct Cell{
			std::tuple<int, int> parent;
			int real_cost;
			int heuristic_cost;
			int total_estimate_cost;
			int step_count_to_here;
			Cell(int r_c, int h_c, int t_c, int s_cnt):real_cost(r_c), heuristic_cost(h_c), total_estimate_cost(t_c), step_count_to_here(s_cnt) {}
		};
	private:
		std::vector<std::vector<Cell>> data_container;
	public:
		std::tuple<int, int> src;
		std::tuple<int, int> des;
	public:
		Cells_map(const std::tuple<int, int>& src, const std::tuple<int, int>& des, int map_length);
		void update(std::tuple<int, int> successor, std::tuple<int, int> parent, const std::vector<std::vector<int>>& weight_map);
		const std::tuple<int, int>& get_parent(const std::tuple<int, int>& target) const {return data_container[std::get<0>(target)][std::get<1>(target)].parent;}
		const int get_real_cost(const std::tuple<int, int>& target) const {return data_container[std::get<0>(target)][std::get<1>(target)].real_cost;}
		const int get_heuristic_cost(const std::tuple<int, int>& target) const {return data_container[std::get<0>(target)][std::get<1>(target)].heuristic_cost;}
		const int get_total_estimate_cost(const std::tuple<int, int>& target) const {return data_container[std::get<0>(target)][std::get<1>(target)].total_estimate_cost;}
		const int get_step_count_to_here(const std::tuple<int, int>& target) const {return data_container[std::get<0>(target)][std::get<1>(target)].step_count_to_here;}
};

Cells_map::Cells_map(const std::tuple<int, int>& src, const std::tuple<int, int>& des, int map_length): src(src), des(des) {
	Cell default_cell(INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX);
	std::vector<Cell> row_of_cell(map_length, default_cell);
	std::vector<std::vector<Cell>> map_of_cell(map_length, row_of_cell);
	data_container = std::move(map_of_cell);
	data_container[std::get<0>(src)][std::get<1>(src)].parent = src;
	data_container[std::get<0>(src)][std::get<1>(src)].real_cost = 0;
	data_container[std::get<0>(src)][std::get<1>(src)].heuristic_cost = 0;
	data_container[std::get<0>(src)][std::get<1>(src)].total_estimate_cost = 0;
	data_container[std::get<0>(src)][std::get<1>(src)].step_count_to_here = 0;
	#if FUNC_DEBUG
	std::cout << "src: " << std::get<0>(src) << ", " << std::get<1>(src) << std::endl;
	std::cout << "des: " << std::get<0>(des) << ", " << std::get<1>(des) << std::endl;
	#endif
}

void Cells_map::update(std::tuple<int, int> successor, std::tuple<int, int> parent, const std::vector<std::vector<int>>& weight_map){
	#if FUNC_DEBUG == 1
	if(get_total_estimate_cost(parent) == INT32_MAX) throw std::runtime_error("when updating cell, successor's parent is unreach yet");
	#endif
	data_container[std::get<0>(successor)][std::get<1>(successor)].parent = parent;
	data_container[std::get<0>(successor)][std::get<1>(successor)].real_cost = get_real_cost(parent) + weight_map[std::get<0>(successor)][std::get<1>(successor)];
	data_container[std::get<0>(successor)][std::get<1>(successor)].heuristic_cost = (std::abs(std::get<0>(successor) - std::get<0>(des)) + std::abs(std::get<1>(successor) - std::get<1>(des))) * BASIC_WEIGHT;
	data_container[std::get<0>(successor)][std::get<1>(successor)].total_estimate_cost = get_real_cost(successor) + get_heuristic_cost(successor);
	data_container[std::get<0>(successor)][std::get<1>(successor)].step_count_to_here = get_step_count_to_here(parent) + 1;
}


//find valid shortest path from scr to des
//throw logic error if not find a path
std::stack<std::tuple<int, int>> shortest_path_finder(const std::tuple<int, int>& scr, const std::tuple<int, int>& des,\
	const std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& snake_positions,\
	bool (*is_valid_successor)(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent,\
	const std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& snake_positions, const Cells_map& cells_data_map));

/*

//find valid longest path form scr to dex
//throw logic error if not find a path
std::stack<std::tuple<int, int>>&& longest_path_finder(const std::tuple<int, int>& scr, const std::tuple<int, int>& des,\
	const std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& snake_posisitions);

*/

//determine a point is valid to go or not
//hit wall, hit snake itself(dynamic), head's vision is > MIN_HEAD_VISION_PERCENT
bool big_head_vision_is_valid_succesor(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent,\
	const std::vector<std::vector<int>>& map, const std::vector<std::tuple<int, int>>& v_snake_positions, const Cells_map& cells_data_map);



//determine a point is valid to go or not
//hit wall, hit snake itself(static)
bool basic_is_valid_successor(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent,\
	const std::vector<std::vector<int>>& map, const std::vector<std::tuple<int, int>>& v_snake_positions, const Cells_map& cells_data_map);

//find fruit position, return a sorted vector which sort from large to small
std::vector<std::tuple<int, int>> find_fruit_positions(const std::vector<std::vector<int>>& map){
	std::vector<std::tuple<int, int>> out;
	for(int i = 0; i < map.size(); i++)
		for(int j = 0; j < map[0].size(); j++){
			if(map[i][j] > 0){
				out.push_back(std::make_tuple(i, j));
			}
		}

	if(out.size() > 1) std::sort(out.begin(), out.end(), [&map](std::tuple<int, int> a, std::tuple<int, int> b){return map[std::get<0>(a)][std::get<1>(a)] > map[std::get<0>(b)][std::get<1>(b)];});

	return out;
}

//make a vector contain snake position with tail at index 1 and head at index size()-1
std::vector<std::tuple<int, int>> make_vec_snake_position(const std::queue<std::tuple<int, int>>& q_position){
	auto tmp = q_position;
	std::vector<std::tuple<int, int>> out(1);
	while(!tmp.empty()){
		out.push_back(tmp.front());
		tmp.pop();
	}
	return out;
}

std::queue<std::tuple<int, int>> make_que_snake_position(const std::vector<std::tuple<int, int>>& v_position){
	std::queue<std::tuple<int, int>> q;
	for(int i = 1; i < v_position.size(); i++){
		q.push(v_position[i]);
	}
	return q;
}

//return a step that is valid to go, if there isn't any, then return random step
std::tuple<int, int> random_step(const std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& snake_positions){
	auto v_snake_positions = make_vec_snake_position(snake_positions);
	auto head_pos = v_snake_positions[v_snake_positions.size()-1];
	std::tuple<int, int> out;
	//walk from left up right down
	auto left_pos = std::make_tuple(std::get<0>(head_pos), std::get<1>(head_pos) - 1);
	auto up_pos = std::make_tuple(std::get<0>(head_pos) - 1, std::get<1>(head_pos));
	auto right_pos = std::make_tuple(std::get<0>(head_pos), std::get<1>(head_pos) + 1);
	auto down_pos = std::make_tuple(std::get<0>(head_pos) + 1, std::get<1>(head_pos));

	//don't hit itself and wall as possible
	if(map[std::get<0>(left_pos)][std::get<1>(left_pos)] >= 0 && std::find(v_snake_positions.begin(), v_snake_positions.end(), left_pos) == v_snake_positions.end()){
		out = left_pos;
	}
	else if(map[std::get<0>(up_pos)][std::get<1>(up_pos)] >= 0 && std::find(v_snake_positions.begin(), v_snake_positions.end(), up_pos) == v_snake_positions.end()){
		out = up_pos;
	}
	else if(map[std::get<0>(right_pos)][std::get<1>(right_pos)] >= 0 && std::find(v_snake_positions.begin(), v_snake_positions.end(), right_pos) == v_snake_positions.end()){
		out = right_pos;
	}
	else{
		out = down_pos;
	}
	return out;
}

std::vector<std::tuple<int, int>> get_successors_positions(const std::tuple<int, int> target);

//return vec<vec<int>> wieght map that take in consideration of snake position
std::vector<std::vector<int>> make_weight_map(const std::vector<std::vector<int>>& map, const std::vector<std::tuple<int, int>>& v_snake_posistions){
	std::vector<std::vector<int>> weight_map;
	weight_map = map;
	
	//point that >= 0, empty spot and fruit spot
	for(auto& row : weight_map)
		for(int& v : row)
			if(v >= 0) v = BASIC_WEIGHT;
	
	//draw around snake
	weight_map[std::get<0>(v_snake_posistions.back()) - 1][std::get<1>(v_snake_posistions.back()) - 1] = AROUND_SNAKE_WEIGHT;
	weight_map[std::get<0>(v_snake_posistions.back()) + 1][std::get<1>(v_snake_posistions.back()) + 1] = AROUND_SNAKE_WEIGHT;
	weight_map[std::get<0>(v_snake_posistions.back()) - 1][std::get<1>(v_snake_posistions.back()) + 1] = AROUND_SNAKE_WEIGHT;
	weight_map[std::get<0>(v_snake_posistions.back()) + 1][std::get<1>(v_snake_posistions.back()) - 1] = AROUND_SNAKE_WEIGHT;
	for(int i = 1; i < v_snake_posistions.size(); i++){
		auto successors_positions = get_successors_positions(v_snake_posistions[i]);
		for(auto& successor : successors_positions){
			weight_map[std::get<0>(successor)][std::get<1>(successor)] = AROUND_SNAKE_WEIGHT;
		}
	}
	
	//draw inside snake
	for(int i = 1; i < v_snake_posistions.size(); i++){
		weight_map[std::get<0>(v_snake_posistions[i])][std::get<1>(v_snake_posistions[i])] = IN_SNAKE_WEIGHT;
	}

	//draw wall
	for(int i = 0; i < map.size(); i++)
		for(int j = 0; j < map.size(); j++){
			if(map[i][j] == -1) weight_map[i][j] = INT32_MAX;
		}
	
	#if FUNC_DEBUG == 1
	for(int i = 0; i < map.size(); i++){
		for(int j = 0; j < map.size(); j++){
			if(weight_map[i][j] != INT32_MAX) std::cout << std::setw(3) << weight_map[i][j] << " ";
			else std::cout << "max ";
		}
		std::cout << std::endl;
	}
	std::cin.get();
	#endif

	return weight_map;
}

//get the positions of this poit near by points
std::vector<std::tuple<int, int>> get_successors_positions(const std::tuple<int, int> target){
	std::vector<std::tuple<int, int>> out; out.reserve(4);
	out.push_back(std::make_tuple(std::get<0>(target), std::get<1>(target) - 1)); //left_successor
	out.push_back(std::make_tuple(std::get<0>(target), std::get<1>(target) + 1)); //right_successor
	out.push_back(std::make_tuple(std::get<0>(target) - 1, std::get<1>(target))); //up_successor
	out.push_back(std::make_tuple(std::get<0>(target) + 1, std::get<1>(target))); //down_successor
	return out;
}

std::stack<std::tuple<int, int>> trace_path(const std::tuple<int, int>& src, const std::tuple<int, int>& des, const Cells_map& cells_data_map){
	std::stack<std::tuple<int, int>> out;
	std::tuple<int, int> iter = des;
	while(cells_data_map.get_parent(iter) != iter){ //if the point parent is itself this must be src position
		out.push(iter);
		iter = cells_data_map.get_parent(iter);
	}
	#if FUNC_DEBUG == 1
	if(iter != src) throw std::runtime_error("trace path not correct!");
	#endif
	return out;
}

//find valid shortest path from scr to des
//throw logic error if not find a path
std::stack<std::tuple<int, int>> shortest_path_finder(const std::tuple<int, int>& src, const std::tuple<int, int>& des,\
	const std::vector<std::vector<int>>& map, const std::queue<std::tuple<int, int>>& q_snake_positions,\
	bool (*is_valid_successor)(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent,\
	const std::vector<std::vector<int>>& map, const std::vector<std::tuple<int, int>>& v_snake_positions, const Cells_map& cells_data_map)){
	//set up and init
	auto v_snake_positions = make_vec_snake_position(q_snake_positions);
	auto weight_map = make_weight_map(map, v_snake_positions);
	Cells_map cells_data_map(src, des, map.size());
	std::set<std::tuple<int, int>> closed_list;
	std::set<std::tuple<int, int>> open_list;
	open_list.insert(src);

	while(!open_list.empty()){
		//pick the smallest cost point in openlist and move it to closed list
		auto examing_point_iter = std::min_element(open_list.begin(), open_list.end(), [&cells_data_map](std::tuple<int, int> a, std::tuple<int, int>b){
			if(cells_data_map.get_total_estimate_cost(a) < cells_data_map.get_total_estimate_cost(b)) return true;
			if(cells_data_map.get_total_estimate_cost(a) > cells_data_map.get_total_estimate_cost(b)) return false;
			return a < b;
		});
		auto examing_point = *examing_point_iter;
		open_list.erase(examing_point_iter);
		closed_list.insert(examing_point);
		#if FUNC_DEBUG == 1
		std::cout << "pull " << std::get<0>(examing_point) << ", " << std::get<1>(examing_point) << " from open and insert to closed\n";
		std::cout << "real coust: " << cells_data_map.get_real_cost(examing_point) <<std::endl;
		std::cout << "heuristic coust: " << cells_data_map.get_heuristic_cost(examing_point) <<std::endl;
		std::cout << "total coust: " << cells_data_map.get_total_estimate_cost(examing_point) <<std::endl;
		std::cout << "open list:\n";
		for(auto tuple : open_list){
			std::cout << "(" << std::get<0>(tuple) << ", " << std::get<1>(tuple) << ") cost: " << cells_data_map.get_total_estimate_cost(tuple) << std::endl;
		}
		#endif

		//check this point successor and update their status
		auto successors_positions = get_successors_positions(examing_point);
		for(auto& successor : successors_positions){
			//if des is tail, we need to check des here
			if(successor == des && successor == q_snake_positions.front()){
				cells_data_map.update(successor, examing_point, weight_map);
				return trace_path(src, des, cells_data_map);
			}
			//basic condition, this successor must be valid
			if(closed_list.find(successor) == closed_list.end() && is_valid_successor(successor, examing_point, map, v_snake_positions, cells_data_map)){
				//if this is des, then we successful find a path
				if(successor == des){
					cells_data_map.update(successor, examing_point, weight_map);
					return trace_path(src, des, cells_data_map);
				}
				//if this successor isn't in openlist, or have a lower cost then the old one, then replace it
				//succesor isn't in open list
				if(open_list.find(successor) == open_list.end()){
					//add this successor to open list and udate cells data map
					open_list.insert(successor);
					cells_data_map.update(successor, examing_point, weight_map);
				}
				else{//this successor is in open list
					int new_cost = cells_data_map.get_total_estimate_cost(examing_point) + weight_map[std::get<0>(successor)][std::get<1>(successor)];
					//if new cost is smaller than old one, update it
					if(new_cost < cells_data_map.get_total_estimate_cost(successor)){
						//update open list and cells data map
						open_list.erase(successor);
						open_list.insert(successor);
						cells_data_map.update(successor, examing_point, weight_map);
					}
				}
			}
		}

	}

	//when can't find any path to des
	throw std::logic_error("can't find path");
	
}

//determine a point is valid to go or not
//hit wall, hit snake itself(static)
bool basic_is_valid_successor(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent,\
	const std::vector<std::vector<int>>& map, const std::vector<std::tuple<int, int>>& v_snake_positions, const Cells_map& cells_data_map){
	
	//hit wall
	if(map[std::get<0>(successor)][std::get<1>(successor)] < 0) return false;
	//ht itself
	if(std::find(v_snake_positions.begin() + 1, v_snake_positions.end(), successor) != v_snake_positions.end()) return false;
	//safe
	return true;

}

//forwar_pos--successor--parent will on one line
std::tuple<int, int> make_forward_position(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent){
	return std::make_tuple((std::get<0>(successor))*2 - std::get<0>(parent), (std::get<1>(successor))*2 - std::get<1>(parent));
}

std::pair<std::tuple<int, int>, std::tuple<int, int>> make_side_position(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent){
	if(std::get<0>(successor) == std::get<0>(parent)){
		return std::make_pair(std::make_tuple(std::get<0>(successor) - 1, std::get<1>(successor)), std::make_tuple(std::get<0>(successor) + 1, std::get<1>(successor)));
	}else{
		return std::make_pair(std::make_tuple(std::get<0>(successor), std::get<1>(successor) - 1), std::make_tuple(std::get<0>(successor), std::get<1>(successor) + 1));
	}
}

//forward a snake to a virtaul snake that it head is at successor
std::vector<std::tuple<int, int>> make_v_virtual_snake(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent, const Cells_map& cells_data_map,const std::vector<std::tuple<int, int>>& v_snake_positions){
	auto q_snake_positions = make_que_snake_position(v_snake_positions);
	auto path_to_parent = trace_path(q_snake_positions.back(), parent, cells_data_map);
	while(!path_to_parent.empty()){
		q_snake_positions.push(path_to_parent.top()); path_to_parent.pop();
		q_snake_positions.pop();
	}
	q_snake_positions.push(successor);
	q_snake_positions.pop();
	return make_vec_snake_position(q_snake_positions);
}

//a virtual map that treat snake as wall
std::vector<std::vector<int>> make_virtual_map(const std::vector<std::vector<int>>& map, const std::vector<std::tuple<int,int>>& snake_positions){
	auto virtual_map = map;
	for(int i = 1; i < snake_positions.size(); i++)
		virtual_map[std::get<0>(snake_positions[i])][std::get<1>(snake_positions[i])] = -1;
	return virtual_map;
}

//count the src vision and return whether contain des
std::pair<int, bool> bruforce_finding(const std::tuple<int, int> src, const std::tuple<int, int> des, const std::vector<std::vector<int>>& map){
	//check src is valid
	if(map[std::get<0>(src)][std::get<1>(src)] == -1) return std::make_pair(0, false);

	int count = 0;
	bool have_des = false;
	//use set to find des and count
	std::vector<std::tuple<int, int>> closed_list;
	std::vector<std::tuple<int, int>> open_list;
	open_list.push_back(src);
	while(!open_list.empty()){
		auto examing_pos = open_list.back(); open_list.pop_back();
		closed_list.push_back(examing_pos);
		auto successors = get_successors_positions(examing_pos);
		for(auto& successor : successors){
			//not hit wall and not in closed list and open list
			if(map[std::get<0>(successor)][std::get<1>(successor)] != -1 &&\
			 std::find(closed_list.begin(), closed_list.end(), successor) == closed_list.end() &&\
			 std::find(open_list.begin(), open_list.end(), successor) == open_list.end()){
				open_list.push_back(successor);
			}
			//check whether contain des
			if(successor == des) have_des = true;
		}
	}
	count = closed_list.size();

	return std::make_pair(count, have_des);
}

//determine a point is valid to go or not
//hit wall, hit snake itself(dynamic), head's vision is > MIN_HEAD_VISION_PERCENT
bool big_head_vision_is_valid_succesor(const std::tuple<int, int>& successor, const std::tuple<int, int>& parent,\
	const std::vector<std::vector<int>>& map, const std::vector<std::tuple<int, int>>& v_snake_positions, const Cells_map& cells_data_map){
	
	//check hit wall
	if(map[std::get<0>(successor)][std::get<1>(successor)] == -1) return false;

	//check hit itself(dynamic)
	if(cells_data_map.get_step_count_to_here(parent) < v_snake_positions.size()){
		if(std::find(v_snake_positions.begin() + cells_data_map.get_step_count_to_here(parent), v_snake_positions.end(), successor) != v_snake_positions.end()){
			return false;
		}
	}
	
	// check head vision 
	//get arounding position
	std::tuple<int, int> forward_pos = make_forward_position(successor, parent);
	std::tuple<int, int> a_side_pos, b_side_pos;
	std::tie(a_side_pos, b_side_pos) = make_side_position(successor, parent);
	auto fruit_pos = cells_data_map.des;

	//if forward pos is wall or snake, need to check vision
	if(map[std::get<0>(forward_pos)][std::get<1>(forward_pos)] == -1 ||\
	(cells_data_map.get_step_count_to_here(parent) < v_snake_positions.size() && std::find(v_snake_positions.begin() + cells_data_map.get_step_count_to_here(parent), v_snake_positions.end(), forward_pos) != v_snake_positions.end()) ){

		//if have a path to tail, then no need to worry about vision
		try{
			auto to_tail_path = shortest_path_finder(successor, make_v_virtual_snake(successor, parent, cells_data_map, v_snake_positions)[1], map, make_que_snake_position(make_v_virtual_snake(successor, parent, cells_data_map, v_snake_positions)), basic_is_valid_successor);
			//find successful
			//no need to check vision

		}catch(std::logic_error e){

			return false;
			
			auto virtual_snake = make_v_virtual_snake(successor, parent, cells_data_map, v_snake_positions);
			auto virtual_map = make_virtual_map(map, virtual_snake);
			//check a side
			int vision;
			bool contain_des;
			std::tie(vision, contain_des) = bruforce_finding(a_side_pos, fruit_pos, virtual_map);
			//see whether it >= min vision, have des
			//if not check other side
			if( !( (float(vision)/float(map.size()*map.size()) >= float(MIN_HEAD_VISION_PERCENT)/float(100)) && contain_des ) ){
				std::tie(vision, contain_des) = bruforce_finding(b_side_pos, fruit_pos, virtual_map);
			}
			//if both doesn't satisfy
			if( !( (float(vision)/float(map.size()*map.size()) >= float(MIN_HEAD_VISION_PERCENT)/float(100)) && contain_des ) ){
				return false;
			}
			
		}

	}

	//if both side is wall or snake, need check vision
	if( ( map[std::get<0>(a_side_pos)][std::get<1>(a_side_pos)] == -1 ||\
		(cells_data_map.get_step_count_to_here(parent) < v_snake_positions.size() && std::find(v_snake_positions.begin() + cells_data_map.get_step_count_to_here(parent), v_snake_positions.end(), a_side_pos) != v_snake_positions.end() ) ) &&\
		( map[std::get<0>(b_side_pos)][std::get<1>(b_side_pos)] == -1 ||\
		(cells_data_map.get_step_count_to_here(parent) < v_snake_positions.size() && std::find(v_snake_positions.begin() + cells_data_map.get_step_count_to_here(parent), v_snake_positions.end(), b_side_pos) != v_snake_positions.end() ) ) ){

		//if have a path to tail, then no need to worry about vision
		try{
			auto to_tail_path = shortest_path_finder(successor, make_v_virtual_snake(successor, parent, cells_data_map, v_snake_positions)[1], map, make_que_snake_position(make_v_virtual_snake(successor, parent, cells_data_map, v_snake_positions)), basic_is_valid_successor);
			//find successful
			//no need to check vision
			
		}catch(std::logic_error e){

			return false;

			auto virtual_snake = make_v_virtual_snake(successor, parent, cells_data_map, v_snake_positions);
			auto virtual_map = make_virtual_map(map, virtual_snake);
			int vision;
			bool contain_des;
			std::tie(vision, contain_des) = bruforce_finding(forward_pos, fruit_pos, virtual_map);
			if( !( (float(vision)/float(map.size()*map.size()) >= float(MIN_HEAD_VISION_PERCENT)/float(100)) && contain_des ) ){
				return false;
			}
		}

	
	}

	
	return true;
}

