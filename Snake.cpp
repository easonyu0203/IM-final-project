#include "Snake.h"
#define MY_INT_MAX 2147483647

// Add anything else you need

Snake::Snake(std::queue<std::tuple<int, int> > startPosition) {

	//init position(queue)
	position = startPosition;
}


std::queue<std::tuple<int, int>> Snake::nextPosition(std::vector<std::vector<int>> map) {
	// Implement by yourself
	if(!take_moves.empty()){
		std::tuple<int, int> new_head = take_moves.top();
		int x, y;
		std::tie(x, y) = take_moves.top();
		take_moves.pop();
		if(map[x][y] == 0){
			position.push(new_head);
			position.pop();
		}
		else{
			position.push(new_head);
		}
		return position;
	}
	else{
		//find fruit position
		auto cmp = [&map](std::pair<int, int> a, std::pair<int, int> b){return map[a.first][a.second] < map[b.first][b.second];};
		std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >, decltype(cmp) > all_fruit_pos(cmp);
		for(int i = 0; i < map.size(); i++){
			for(int j = 0; j < map.size(); j++){
				if(map[i][j] > 0){
					all_fruit_pos.push(std::make_pair(i, j));
				}
			}
		}

		//dont step on snake itself
		std::queue<std::tuple<int, int> > tmp = position;
		std::set<std::pair<int, int> > _position;
		while(!tmp.empty()){
			_position.insert(std::make_pair(std::get<0>(tmp.front()), std::get<1>(tmp.front())));
			tmp.pop();
		}
		for(auto& par : _position){
			map[par.first][par.second] = -1;
		}

		//find path
		bool success = false;
		while(success == false && !all_fruit_pos.empty()){
			std::tie(take_moves, success) = a_star_pathfinding(std::make_pair(std::get<0>(position.back()), std::get<1>(position.back())), all_fruit_pos.top(), map);
			all_fruit_pos.pop();
			if(!success) while(!take_moves.empty()) take_moves.pop();
		}

		if(success){
			std::tuple<int, int> new_head = take_moves.top();
			int x, y;
			std::tie(x, y) = take_moves.top();
			take_moves.pop();
			if(map[x][y] == 0){
				position.push(new_head);
				position.pop();
			}
			else{
				position.push(new_head);
			}
			return position;
		}
		else{
			while(!take_moves.empty()) take_moves.pop();
			std::pair<int, int> head  = std::make_pair(std::get<0>(position.back()), std::get<1>(position.back()));
			std::tuple<int, int> next_head;
			if(map[head.first - 1][head.second] > -1) next_head = std::make_tuple(head.first - 1, head.second);
			else if(map[head.first][head.second + 1] > -1) next_head = std::make_tuple(head.first, head.second + 1);
			else if(map[head.first][head.second - 1] > -1) next_head = std::make_tuple(head.first, head.second - 1);
			else/*(map[head.first + 1][head.second] > -1) */next_head = std::make_tuple(head.first + 1, head.second);
			if(map[std::get<0>(next_head)][std::get<1>(next_head)] == 0){
				position.push(next_head);
				position.pop();
			}
			else{
				position.push(next_head);
			}
			return position;
		}
		
	}
}

// Add anything else you need



struct cell 
{ 
    // Row and Column index of its parent 
    int parent_i, parent_j;
    // f = g + h 
    int f, g, h; 
	cell(int i, int j, int f, int g, int h): parent_i(i), parent_j(j), f(f), g(g), h(h){}
}; 

bool is_valid_by_map(std::pair<int, int> pos, const std::vector<std::vector<int> >& map){
	return map[pos.first][pos.second] != -1;
}

int calculate_h(std::pair<int, int> pos, const std::pair<int, int>& des){
	return std::abs(des.first - pos.first) + std::abs(des.second - pos.second);
}

// A Utility Function to trace the path from the source 
// to destination 
std::stack<std::pair<int, int> > tracePath(const std::vector<std::vector<cell> >& cellDetails,const std::pair<int, int>& dest) 
{ 
    int row = dest.first;
    int col = dest.second; 
  
    std::stack<std::pair<int, int> > Path; 
  
    while (!(cellDetails[row][col].parent_i == row 
             && cellDetails[row][col].parent_j == col )) 
    { 
        Path.push(std::make_pair(row, col)); 
        int temp_row = cellDetails[row][col].parent_i; 
        int temp_col = cellDetails[row][col].parent_j; 
        row = temp_row; 
        col = temp_col; 
    } 
  
    // Path.push (std::make_pair (row, col));
    return Path; 
} 

std::pair<std::stack<std::pair<int, int> >, bool> Snake::a_star_pathfinding(std::pair<int, int> src, std::pair<int, int> des, std::vector<std::vector<int> > map){
	 // Create a closed list and initialise it to false which means 
    // that no cell has been included yet 
	std::vector<bool> row_bool(50, false);
	std::vector<std::vector<bool> > closedList(map.size(), row_bool);

	// Declare a 2D array of structure to hold the details 
    //of that cell 
	cell aCell(-1, -1,  MY_INT_MAX, MY_INT_MAX, MY_INT_MAX);
	std::vector<cell> row_ceil(map[0].size(), aCell);
    std::vector<std::vector<cell> > cellDetails(map.size(), row_ceil);

	// Initialising the parameters of the starting node 
    int i = src.first;
	int j = src.second; 
    cellDetails[i][j].f = 0; 
    cellDetails[i][j].g = 0; 
    cellDetails[i][j].h = 0; 
    cellDetails[i][j].parent_i = i; 
    cellDetails[i][j].parent_j = j; 
	

	// Create an open list having information as- 
    //  <f, <i, j>> 
    //  where f = g + h, 
    //  and i, j are the row and column index of that cell 
    std::set<std::pair<double, std::pair<int, int> > > openList;

	// Put the starting cell on the open list and set its 
    // 'f' as 0 
    openList.insert(std::make_pair (0, std::make_pair (i, j))); 

	// We set this boolean value as false as initially 
    // the destination is not reached. 
    bool foundDest = false; 

	while(!openList.empty()){
		auto p = *(openList.begin());

		// Remove this vertex from the open list 
        openList.erase(openList.begin()); 

		// Add this vertex to the closed list 
        i = p.second.first; 
        j = p.second.second; 
        closedList[i][j] = true;


		int gNew, hNew, fNew; 
		//----------- 1st Successor (west) ------------ 
  
        // Only process this cell if this is a valid one 
		std::pair<int, int> west_successor(i - 1, j);
		if(is_valid_by_map(west_successor, map) /*&& is_valid_by_hamilton_order(west_successor)*/){
			if(west_successor == des){
				// Set the Parent of the destination cell 
                cellDetails[west_successor.first][west_successor.second].parent_i = i; 
                cellDetails[west_successor.first][west_successor.second].parent_j = j;
				
				foundDest = true;
				return std::make_pair(tracePath(cellDetails, des), foundDest);
			}
			// If the successor is already on the closed then ignore it. 
            // Else do the following 
            else if (closedList[west_successor.first][west_successor.second] == false){
				gNew = cellDetails[i][j].g + 1; 
                hNew = calculate_h(west_successor, des);
                fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
                // the open list. Make the current square 
                // the parent of this square. Record the 
                // f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check 
                // to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cellDetails[west_successor.first][west_successor.second].f == MY_INT_MAX ||  cellDetails[west_successor.first][west_successor.second].f > fNew){
					openList.insert(std::make_pair(fNew, west_successor));
					// Update the details of this cell 
                    cellDetails[west_successor.first][west_successor.second].f = fNew; 
                    cellDetails[west_successor.first][west_successor.second].g = gNew; 
                    cellDetails[west_successor.first][west_successor.second].h = hNew; 
                    cellDetails[west_successor.first][west_successor.second].parent_i = i; 
                    cellDetails[west_successor.first][west_successor.second].parent_j = j; 
				}
			}
		}

		//----------- 2st Successor (east) ------------ 
  
        // Only process this cell if this is a valid one 
		std::pair<int, int> east_successor(i + 1, j);
		if(is_valid_by_map(east_successor, map)/* && is_valid_by_hamilton_order(east_successor)*/){
			if(east_successor == des){
				// Set the Parent of the destination cell 
                cellDetails[east_successor.first][east_successor.second].parent_i = i; 
                cellDetails[east_successor.first][east_successor.second].parent_j = j;

				foundDest = true;
				return std::make_pair(tracePath(cellDetails, des), foundDest);
			}
			// If the successor is already on the closed then ignore it. 
            // Else do the following 
            else if (closedList[east_successor.first][east_successor.second] == false){
				gNew = cellDetails[i][j].g + 1; 
                hNew = calculate_h(east_successor, des);
                fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
                // the open list. Make the current square 
                // the parent of this square. Record the 
                // f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check 
                // to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cellDetails[east_successor.first][east_successor.second].f == MY_INT_MAX ||  cellDetails[east_successor.first][east_successor.second].f > fNew){
					openList.insert(std::make_pair(fNew, east_successor));
					// Update the details of this cell 
                    cellDetails[east_successor.first][east_successor.second].f = fNew; 
                    cellDetails[east_successor.first][east_successor.second].g = gNew; 
                    cellDetails[east_successor.first][east_successor.second].h = hNew; 
                    cellDetails[east_successor.first][east_successor.second].parent_i = i; 
                    cellDetails[east_successor.first][east_successor.second].parent_j = j; 
				}
			}
		}

		//----------- 3st Successor (north) ------------ 
  
        // Only process this cell if this is a valid one 
		std::pair<int, int> north_successor(i, j + 1);
		if(is_valid_by_map(north_successor, map)/* && is_valid_by_hamilton_order(north_successor)*/){
			if(north_successor == des){
				// Set the Parent of the destination cell 
                cellDetails[north_successor.first][north_successor.second].parent_i = i; 
                cellDetails[north_successor.first][north_successor.second].parent_j = j;

				foundDest = true;
				return std::make_pair(tracePath(cellDetails, des), foundDest);
			}
			// If the successor is already on the closed then ignore it. 
            // Else do the following 
            else if (closedList[north_successor.first][north_successor.second] == false){
				gNew = cellDetails[i][j].g + 1; 
                hNew = calculate_h(north_successor, des);
                fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
                // the open list. Make the current square 
                // the parent of this square. Record the 
                // f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check 
                // to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cellDetails[north_successor.first][north_successor.second].f == MY_INT_MAX ||  cellDetails[north_successor.first][north_successor.second].f > fNew){
					openList.insert(std::make_pair(fNew, north_successor));
					// Update the details of this cell 
                    cellDetails[north_successor.first][north_successor.second].f = fNew; 
                    cellDetails[north_successor.first][north_successor.second].g = gNew; 
                    cellDetails[north_successor.first][north_successor.second].h = hNew; 
                    cellDetails[north_successor.first][north_successor.second].parent_i = i; 
                    cellDetails[north_successor.first][north_successor.second].parent_j = j; 
				}
			}
		}

		//----------- 4st Successor (south) ------------ 
  
        // Only process this cell if this is a valid one 
		std::pair<int, int> south_successor(i, j - 1);
		if(is_valid_by_map(south_successor, map) /*&& is_valid_by_hamilton_order(south_successor)*/){
			if(south_successor == des){
				// Set the Parent of the destination cell 
                cellDetails[south_successor.first][south_successor.second].parent_i = i; 
                cellDetails[south_successor.first][south_successor.second].parent_j = j;
				
				foundDest = true;
				return std::make_pair(tracePath(cellDetails, des), foundDest);
			}
			// If the successor is already on the closed then ignore it. 
            // Else do the following 
            else if (closedList[south_successor.first][south_successor.second] == false){
				gNew = cellDetails[i][j].g + 1; 
                hNew = calculate_h(south_successor, des);
                fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
                // the open list. Make the current square 
                // the parent of this square. Record the 
                // f, g, and h costs of the square cell 
                //                OR 
                // If it is on the open list already, check 
                // to see if this path to that square is better, 
                // using 'f' cost as the measure. 
                if (cellDetails[south_successor.first][south_successor.second].f == MY_INT_MAX ||  cellDetails[south_successor.first][south_successor.second].f > fNew){
					openList.insert(std::make_pair(fNew, south_successor));
					// Update the details of this cell 
                    cellDetails[south_successor.first][south_successor.second].f = fNew; 
                    cellDetails[south_successor.first][south_successor.second].g = gNew; 
                    cellDetails[south_successor.first][south_successor.second].h = hNew; 
                    cellDetails[south_successor.first][south_successor.second].parent_i = i; 
                    cellDetails[south_successor.first][south_successor.second].parent_j = j; 
				}
			}
		}
	}

	 // When the destination cell is not found and the open 
    // list is empty, then we conclude that we failed to 
    // reach the destiantion cell. This may happen when the 
    // there is no way to destination cell (due to blockages) 
	foundDest = false;
	return  std::make_pair(std::stack<std::pair<int, int> >(), false);
}

