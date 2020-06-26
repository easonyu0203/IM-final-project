#include "Snake.h"
#include"func.cpp"

#define SNAKE_DEBUG 0

Snake::Snake(std::queue<std::tuple<int, int>> startPosition){
	position = startPosition;
}

std::queue<std::tuple<int, int>> Snake::nextPosition(std::vector<std::vector<int>> map){

	if(scheduled_steps.empty()){

		//try to find path to fruit
		//find fruit position
		auto fruit_positions = find_fruit_positions(map);
		//try find path to fruit, fruit bigger is better
		for(auto& fruit_pos : fruit_positions){
			try{
				scheduled_steps = shortest_path_finder(head_pos(), fruit_pos, map, position, basic_is_valid_successor);
				//find successful
				//clear backup steps
				while(!backup_steps.empty()) backup_steps.pop();
				break;
			}catch(std::logic_error e){
				//not found yet, try again
			}
		}

		//if not find any path to fruit
		if(scheduled_steps.empty() && backup_steps.empty()){
			std::cout << "not find path to fruit!\n"; std::cin.get();
			auto v_snake_positions = make_vec_snake_position(position);
			//try find longest path to tail, if not then tail-1 and so on
			for(int i = 1; i < v_snake_positions.size() - 2; i++){
				try{
					// backup_steps = longest_path_finder(head_pos(), v_snake_positions[i], map, position);
					//find successful
					break;
				}catch(std::logic_error e){
					//not found yet, try again
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
