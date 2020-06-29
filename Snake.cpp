#include "Snake.h"
#include"func.cpp"

#define SNAKE_DEBUG 0

Snake::Snake(std::queue<std::tuple<int, int>> startPosition){
	position = startPosition;
}

std::queue<std::tuple<int, int>> Snake::nextPosition(std::vector<std::vector<int>> map){

	if(scheduled_steps.empty() && backup_steps.empty()){

		//try to find path to fruit
		//find fruit position, return all fruit with biggest at begin()
		auto fruit_positions = find_fruit_positions(map, position);
		auto biggest_fruit_iter = fruit_positions.begin();
		bool find_short_path = false;

		//use low around snake weight map to find short path
		try{
			scheduled_steps = shortest_path_finder(head_pos(), *biggest_fruit_iter, true, map, position, dynamic_is_valid_succesor, make_low_around_snake_weight_map);
			//find successful
			//check wether after eat can survive
			if(check_path(scheduled_steps, position, map) == false){
				//clear scheduled path
				while(!scheduled_steps.empty()){scheduled_steps.pop();}
			}
			else{
				//this scheduled path can use
				//clear backup steps
				find_short_path = true;
				while(!backup_steps.empty()) backup_steps.pop();
			}
		}
		catch(std::logic_error e){
			//not find so do nothing
		}
		
		//if not find any path to fruit
		if(scheduled_steps.empty()){
			
			auto v_snake_position = make_vec_snake_position(position);
			for(int i = 1; i < v_snake_position.size() - 1; i++){
				try{
					bool have_bigger_path;
					std::tie(have_bigger_path, backup_steps) = bigger_path_finder(head_pos(), v_snake_position[i], map, position, i + 1);
					if(have_bigger_path){
						//can suvive
						break;
					}
					else{
						//path not long enough
						continue;
					}
				}
				catch(std::logic_error e){
					//didn't find any path, try again
					continue;
				}
			}
		}
	}

	

	//return positions
	if(!scheduled_steps.empty()){ //have path to fruit
		position.push(scheduled_steps.top());
		scheduled_steps.pop();
		if(map[std::get<0>(position.back())][std::get<1>(position.back())] == 0) position.pop();
		else{ // have eat fruit
			//clear all steps
			while(!scheduled_steps.empty()) scheduled_steps.pop();
			while(!backup_steps.empty()) backup_steps.pop();
		}
	}
	else if(!backup_steps.empty()){ //try to suvive by taking longest path
		#if SNAKE_DEBUG == 1
		std::cout << "using backup path\n";
		std::cout << "head pos: " <<  std::get<0>(head_pos()) << ", "  << std::get<1>(head_pos()) << "\n";
		std::cout << "next step: " << std::get<0>(backup_steps.top()) << ", "  << std::get<1>(backup_steps.top()) << "\n";
		#endif
		position.push(backup_steps.top());
		backup_steps.pop();
		if(map[std::get<0>(position.back())][std::get<1>(position.back())] == 0) position.pop();
		else{ // have eat fruit
			//clear all steps
			while(!scheduled_steps.empty()) scheduled_steps.pop();
			while(!backup_steps.empty()) backup_steps.pop();
		}
	}
	else{//if can't find any path to fruit and any longest path
		//fuck up and random walk
		position.push(random_step(map, position));
		if(map[std::get<0>(position.back())][std::get<1>(position.back())] == 0) position.pop();
		else{ // have eat fruit
			//clear all steps
			while(!scheduled_steps.empty()) scheduled_steps.pop();
			while(!backup_steps.empty()) backup_steps.pop();
		}
	}

	return position;

}
