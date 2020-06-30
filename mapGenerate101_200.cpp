
#include <fstream>
#include<cstring>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <set>
#include <vector>

using namespace std;

const pair<int, int> MAP_SIZE = make_pair(50, 50); // including boundary
const int NUM_FOOD = 6; // food each round

int main()
{
	srand(time(nullptr));
    vector<pair<int, int>> barriers;
	set<pair<int, int>> tmp;
	set<pair<int, int>> cant_point;
	cant_point.emplace(1,2);
	cant_point.emplace(2,1);
	cant_point.emplace(1,47);
	cant_point.emplace(2,48);
	cant_point.emplace(47,1);
	cant_point.emplace(48,2);
	cant_point.emplace(48,47);
	cant_point.emplace(47,48);
	
	for(int i = 0; i < 60; i++){
		auto point = make_pair(1 + rand()%48, 1+rand()%48);
		if(cant_point.find(point) == cant_point.end()){
			barriers.push_back(point);
		}
	}

    vector<vector<int>> tempMap;

    for (size_t j = 0; j < MAP_SIZE.second; j++) {
        tempMap.push_back(vector<int>());
        if (j == 0 || j == MAP_SIZE.second - 1) {
            for (size_t k = 0; k < MAP_SIZE.first; k++) {
                tempMap[j].push_back(-1);
            }
        }

        else {
            for (size_t k = 0; k < MAP_SIZE.second; k++) {
                if (k == 0 || k == MAP_SIZE.first - 1) {
                    tempMap[j].push_back(-1);
                } else {
                    tempMap[j].push_back(0);
                }
            }
        }
    }

    for (size_t i = 0; i < barriers.size(); i++) {
        tempMap[barriers[i].first][barriers[i].second] = -1;
    }

    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_int_distribution<int> dis(1, 48);
    std::uniform_int_distribution<int> foodScoreDis(1, 2);

    string filename = "";
    for (size_t i = 0; i < 100; i++) {
        vector<vector<int>> map = tempMap;
        filename = "./map/maps/map_" + to_string(i + 101);
        // cout << "writing " << filename << "\n";

        // if (!fp)
        // {
        //     cout << "fail\n";
        // }

        // else
        {
            bool success = false;
            pair<int, int> foodPos;
			bool have_2 = false;
            for (size_t j = 0; j < NUM_FOOD; j++) {
                success = false;
                // cout << j << "\n";
                while (!success) {
                    foodPos = make_pair(dis(gen), dis(gen));
                    // cout << "try put food at " << foodPos.first << "," << foodPos.second << "\n";

                    if (map[foodPos.first][foodPos.second] == 0) {
						// map[foodPos.first][foodPos.second] = rand()%2 + 1;
						if(have_2){
							map[foodPos.first][foodPos.second] = 1;
						}
						else{
                        	map[foodPos.first][foodPos.second] = 2;
							have_2 = true;
						}
                        // cout << "put food at " << foodPos.first << "," << foodPos.second << "\n";
                        success = true;
                    }
                }
            }
        }

        fstream fp;
        fp.open(filename, ios::out);

        for (size_t j = 0; j < MAP_SIZE.second; j++) {
            for (size_t k = 0; k < MAP_SIZE.first; k++) {
                if (!k == 0) {
                    fp << " ";
                }
                fp << to_string(map[j][k]);
            }
            if (j != MAP_SIZE.second - 1) fp << "\n";
        }

        fp.close();
    }

    // system("pause");

    return 0;
}
