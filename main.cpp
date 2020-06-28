// #define _CRT_SECURE_NO_WARNINGS

#include <cmath>
#include<fstream>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <tuple>
#include <vector>
#include<set>
#include <thread>
#include<iomanip>
#include <chrono>

#include "Snake.h"

//whether cout or not(print map)
#define COUT 0
//whether cout result or not
#define COUT_RESULT 1
//write result to file or not
#define WRITE_RESULT_TO_FILE 1
//only write win or lose?
#define STATUS_ONLY 0
//skip map index to 
#define SKIP_POINT 201
//milli second per frame
#define milli_sec_per_frame 60
using namespace std;

auto start_t = std::chrono::high_resolution_clock::now();
auto finish_t = std::chrono::high_resolution_clock::now();
#if WRITE_RESULT_TO_FILE
std::ofstream outFile("testing_data.txt", std::ios::app);
#endif

const int NUM_MAP = 200;

void print_game(Snake& snake, const vector<vector<int>>& _map);

void loadmaps(vector<vector<int>> map[NUM_MAP + 1])
{
    //    vector<vector<int>> map[101]; //maps
    fstream file;
    char buffer[150];
    string fname;
    for (int i = 1; i < NUM_MAP + 1; i++) {
        if (i < 10)
            fname = "00" + to_string(i);
        else if (i < 100)
            fname = "0" + to_string(i);
        else
            fname = to_string(i);
        file.open("map/maps/map_" + fname, ios::in);
        if (!file) {
			#if COUT == 1
            cout << "no file" << endl;
			#endif
        } else {
            do {
                vector<int> tempv;
                file.getline(buffer, sizeof(buffer));
                const char* d = " "; //分割依據
                char* row; //儲存每次分割結果
                row = strtok(buffer, d);
                while (row) {
                    string s = row;
                    int temp = stoi(s);
                    tempv.push_back(temp);
                    row = strtok(NULL, d);
                }
                map[i].push_back(tempv);
            } while (!file.eof());
        }
        file.close();
    }
}

//(所有地圖,下一張地圖,蛇的位置)
vector<vector<int>> generate_map(vector<vector<int>> map[NUM_MAP + 1], int mapindex, queue<tuple<int, int>> snack)
{
    vector<tuple<int, int>> positionList;
    vector<int> scoreList;
    tuple<int, int> position;
    vector<vector<int>> tempMap = map[mapindex];

    int row, col;
    int flag3 = 0;
    int flag5 = 0;
    int flag7 = 0;
    int flagall = 0;

    //尋找地圖得分點位置
    for (int m = 0; m < map[mapindex].size(); m++) {
        for (int n = 0; n < map[mapindex][m].size(); n++) {
            if (map[mapindex][m][n] != 0 && map[mapindex][m][n] != 0) {
                row = m;
                col = n;
                positionList.push_back(make_tuple(m, n));
                scoreList.push_back(map[mapindex][m][n]);
            }
        }
    }

    for (size_t h = 0; h < positionList.size(); h++) {
        position = positionList[h];
        //判斷得分點有沒有在蛇身上

        int flag = 0;

        queue<tuple<int, int>> tempsnack = snack;

        while (!tempsnack.empty() && flag == 0) {
            if (tempsnack.front() == position) {
                flag = 1;
            }
            tempsnack.pop();
        }

        if (flag == 0)
            continue;

        else {
            //      3*3找新得分點
            for (int i = row - 1; i < row + 2; i++) {
                if (i > 0 && i < 49 && flag3 == 0) {
                    for (int j = col - 1; j < col + 2; j++) {
                        if (j > 0 && j < 49 && flag3 == 0) {
                            tempsnack = snack;
                            flag = 0;
                            position = make_tuple(i, j);
                            //每一個點跟蛇的位置比較，有找到
                            while (!tempsnack.empty() && flag == 0) {
                                if (tempsnack.front() == position) {
                                    flag = 1;
                                }
                                tempsnack.pop();
                            }
                            if (flag == 0) {
                                flag3 = 1; //在3*3中找到可以放點的地方
                                row = i; //新點index
                                col = j;
                                break;
                            }
                        }
                        if (flag3 == 1)
                            break;
                    }
                }
                if (flag3 == 1) {
                    break;
                }
            }
            if (flag3 == 1) { //3*3找到
                tempMap[row][col] = scoreList[h];
            } else if (flag3 == 0 && flag5 == 0) { //5*5找得分點
                for (int i = row - 2; i < row + 3; i++) {
                    if (i > 0 && i < 49 && flag5 == 0) {
                        for (int j = col - 2; j < col + 3; j++) {
                            if (j > 0 && j < 49 && flag5 == 0) {
                                tempsnack = snack;
                                flag = 0;
                                position = make_tuple(i, j);
                                //每一個點跟蛇的位置比較，有找到
                                while (!tempsnack.empty() && flag == 0) {
                                    if (tempsnack.front() == position) {
                                        flag = 1;
                                    }
                                    tempsnack.pop();
                                }
                                if (flag == 0) {
                                    flag5 = 1; //在5*5中找到可以放點的地方
                                    row = i;
                                    col = j;
                                    break;
                                }
                            }
                            if (flag5 == 1)
                                break;
                        }
                    }
                    if (flag5 == 1) {
                        break;
                    }
                }
            }
            if (flag5 == 1) { //5*5找到
                tempMap[row][col] = scoreList[h];
            } else if (flag3 == 0 && flag5 == 0 && flagall == 0) {
                for (int i = 0; i < 50; i++) {
                    if (i > 0 && i < 49 && flagall == 0) {
                        for (int j = 0; j < 50; j++) {
                            if (j > 0 && j < 49 && flagall == 0) {
                                tempsnack = snack;
                                flag = 0;
                                position = make_tuple(i, j);
                                //每一個點跟蛇的位置比較，有找到
                                while (!tempsnack.empty() && flag == 0) {
                                    if (tempsnack.front() == position) {
                                        flag = 1;
                                    }
                                    tempsnack.pop();
                                }
                                if (flag == 0) {
                                    flag5 = 1; //在5*5中找到可以放點的地方
                                    row = i;
                                    col = j;
                                    break;
                                }
                            }
                            if (flagall == 1)
                                break;
                        }
                    }
                    if (flagall == 1) {
                        break;
                    }
                }
            }
            if (flagall == 1) {
                tempMap[row][col] = scoreList[h];
            }
        }
    }
    return tempMap;
}

queue<tuple<int, int>> get_start_position()
{
    queue<tuple<int, int>> re;

    tuple<int, int> pos1(1, 1);
    tuple<int, int> pos2(1, 2);
    tuple<int, int> pos3(1, 3);
    tuple<int, int> pos4(1, 4);
    tuple<int, int> pos5(1, 5);

    re.push(pos1);
    re.push(pos2);
    re.push(pos3);
    re.push(pos4);
    re.push(pos5);

    return re;
}

vector<vector<int>> get_map()
{
    vector<vector<int>> re;

    return re;
}
//
//vector<vector<int>> generate_map(vector<vector<int>> map[], int mapindex, queue<tuple<int, int>> snack) {
//	return map[mapindex];
//}

bool first =true;

int main(int argc, char* argv[])
{
    queue<tuple<int, int>> ori_pos;
    queue<tuple<int, int>> new_pos = get_start_position();

    Snake snake(new_pos);



    vector<vector<int>> whole_map[NUM_MAP + 1]; //1~100存一百張地圖
    loadmaps(whole_map); //從檔案中讀取地圖資料

    int cur_map_index = 1;
    vector<vector<int>> map = generate_map(whole_map, cur_map_index, new_pos);

    int step_limit = 8000;
    int point = 0;
    // cout << get<0>(new_pos.back()) << ", " << get<1>(new_pos.back()) << "|" << point << "\n";
	int i = 0;
    for (; i < step_limit; i++) {
		#if 1 == 1
		//output the current state
		#if COUT == 1
		std::cout << "current map index: " << cur_map_index << std::endl;
		std::cout << "current step: " << i << std::endl;
		std::cout << "point       : " << point << std::endl;
		#endif
		if(true && cur_map_index >= SKIP_POINT){

			finish_t = std::chrono::high_resolution_clock::now();
			auto past_time = std::chrono::duration_cast<std::chrono::milliseconds>(finish_t - start_t);
			if(!first && past_time < std::chrono::milliseconds(milli_sec_per_frame)){

				std::this_thread::sleep_for(std::chrono::milliseconds(milli_sec_per_frame) - past_time);
			}
			first = false;
			print_game(snake, map);
			start_t = std::chrono::high_resolution_clock::now();
		}
		#endif
        ori_pos = new_pos;
        new_pos = snake.nextPosition(map);

        int new_head_x = get<0>(new_pos.back());
        int new_head_y = get<1>(new_pos.back());
        // cout << i << ": " << get<0>(new_pos.back()) << ", " << get<1>(new_pos.back()) << "|" << map[new_head_x][new_head_y] << "$" << point << "\n";

        // Walk one step
        int one_step_limit = 1;
        one_step_limit -= abs(get<0>(ori_pos.back()) - new_head_x);
        one_step_limit -= abs(get<1>(ori_pos.back()) - new_head_y);
        if (one_step_limit != 0) {
			#if COUT == 1
            cout << argv[1] << " A: Invalid step... " << get<0>(new_pos.back()) << ", " << get<1>(new_pos.back()) << "\n";
			#endif
            break;
        }

        // Hit wall
        if (map[new_head_x][new_head_y] == -1) {
			#if COUT == 1
            cout << argv[1] << " B: GAME OVER! Hit wall... " << get<0>(new_pos.back()) << ", " << get<1>(new_pos.back()) << "\n";
			#endif
            break;
        }

        // Hit self
        queue<tuple<int, int>> tmp_queue = new_pos;
        bool ifGameOver = false;
        for (int i = 0; i < new_pos.size() - 1; i++) {
            if (get<0>(tmp_queue.front()) == new_head_x && get<1>(tmp_queue.front()) == new_head_y) {
				#if COUT == 1
                cout << argv[1] << " C: GAME OVER! Hit yourself... " << get<0>(new_pos.back()) << ", " << get<1>(new_pos.back()) << "\n";
				#endif
                ifGameOver = true;
                break;
            }
            tmp_queue.pop();
        }
        if (ifGameOver)
            break;
        // Check teleport
        // TBD...

        // Count point and check eat longer
        if (map[new_head_x][new_head_y] > 0) {
            if (new_pos.size() != ori_pos.size() + 1) {
				#if COUT == 1
                cout << argv[1] << " D: Invalid eat length... " << get<0>(new_pos.back()) << ", " << get<1>(new_pos.back()) << "\n";
				#endif
                break;
            }

            point += map[new_head_x][new_head_y];
			#if 1  == 1
			if(cur_map_index == 200){
				#if WRITE_RESULT_TO_FILE == 1
				outFile << "win\n";
				#if STATUS_ONLY == 0
				outFile << "used step: " << i << std::endl;
				outFile << "map index: " << cur_map_index << std::endl << std::endl;
				#endif
				#endif
				#if COUT_RESULT == 1
				cout << "status   : win\n";
				cout << "used step: " << i << std::endl;
				cout << "map index: " << cur_map_index << std::endl << std::endl;
				#endif
				//output the current state
				#if COUT == 1
				std::cout << "current step: " << i << std::endl;
				std::cout << "point       : " << point << std::endl;
				std::cout << "YOU WIN!!!\n";
				#endif
				print_game(snake, map);
				return 0;
			}
			#endif
			
            map = generate_map(whole_map, ++cur_map_index, new_pos);
        } else {
            if (new_pos.size() != ori_pos.size()) {
				#if COUT == 1
                cout << argv[1] << " E: Invalid length... " << get<0>(new_pos.back()) << ", " << get<1>(new_pos.back()) << "\n";
				#endif
                break;
            }
        }
        if (cur_map_index > NUM_MAP) {
            break;
        }
    }
	#if COUT == 1
    cout << argv[1] << " | Final: " << get<0>(new_pos.back()) << ", " << get<1>(new_pos.back()) << "$" << point << "\n";
	#endif
	#if COUT_RESULT == 1
	cout << "status   : lose\n";
	cout << "used step: " << i << std::endl;
	cout << "map index: " << cur_map_index << std::endl << std::endl;
	#endif
	#if WRITE_RESULT_TO_FILE == 1
	outFile << "lose\n";
	#if STATUS_ONLY == 0
	outFile << "used step: " << i << std::endl;
	outFile << "map index: " << cur_map_index << std::endl << std::endl;
	#endif
	#endif
    // system("pause");
    return 0;
}


void print_game(Snake& snake, const vector<vector<int>>& _map){
	#if COUT == 1
	auto map = _map;
	auto _position = snake.get_position();
	while(!_position.empty()){
		int x, y;
		std::tie(x, y) = _position.front();
		map[x][y] = -10;
		_position.pop();
	}
	for(auto& row : map){
		for(auto pos : row){
			if(pos == -10){
				std::cout << "X ";
			}
			else if(pos == -1){
				std::cout << "* ";
			}
			else if(pos == 0){
				std::cout << "  ";
			}
			else if(pos >= 1){
				std::cout << std::setw(2) << pos;
			}
			else{
				std::cout << "pos: " << pos << std::endl;
				throw std::logic_error("map out put wrong");
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	#endif
}
