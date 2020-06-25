#include<string>
#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include<random>

int main(){
	srand(time(nullptr));
	std::ofstream outFile;
	int map[50][50];
		memset(map, 0, 2500*sizeof(int));
		for(int j = 0; j < 50; j++){
			map[0][j] = -1;
			map[49][j] = -1;
			map[j][0] = -1;
			map[j][49] = -1;
		}
	for(int k = 1; k <= 9; k++){
		std::string fileName  = "./map/maps/map_00" + std::to_string(k);
		outFile.open(fileName);
		int x = rand() % 48 + 1;
		int y = rand() % 48 + 1;
		map[x][y] = 1;
		for(int i = 0; i < 50; i++){
			outFile << map[i][0];
			for(int j = 1; j < 50; j++)
				outFile << " " << map[i][j];
			if(i  != 49) outFile << "\n";
		}
		map[x][y] = 0;
		outFile.close();
	}
	for(int k = 10; k <= 99; k++){
		std::string fileName  = "./map/maps/map_0" + std::to_string(k);
		outFile.open(fileName);
		int x = rand() % 48 + 1;
		int y = rand() % 48 + 1;
		map[x][y] = 1;
		for(int i = 0; i < 50; i++){
			outFile << map[i][0];
			for(int j = 1; j < 50; j++)
				outFile << " " << map[i][j];
			if(i  != 49) outFile << "\n";
		}
		map[x][y] = 0;
		outFile.close();
	}
	std::string fileName  = "./map/maps/map_100";
		outFile.open(fileName);
		int x = rand() % 48 + 1;
		int y = rand() % 48 + 1;
		map[x][y] = 1;
		for(int i = 0; i < 50; i++){
			outFile << map[i][0];
			for(int j = 1; j < 50; j++)
				outFile << " " << map[i][j];
			if(i  != 49) outFile << "\n";
		}
		map[x][y] = 0;
		outFile.close();

	return 0;
}