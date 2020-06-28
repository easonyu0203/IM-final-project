#include<fstream>
#include<string>

int main(){

	std::ifstream in("testing_data.txt");
	int win = 0, lose = 0;
	int indexCnt = 0;
	int score = 0;
	std::string str;
	while(in >> str){
		if(str == "win") win++;
		else if(str == "lose") lose++;
		else if(str == "map_index:"){
			int tmpCnt;
			in >> tmpCnt;
			indexCnt += tmpCnt;
		}
		else if(str == "score"){
			std::string dummy;
			in >> dummy;
			int tmp;
			in >> tmp;
			score += tmp;
		}
		else continue;
	}
	in.close();
	std::ofstream out("testing_data.txt", std::ios:: app);
	out << "\n================================================\n";
	out << "total round: " << win + lose << std::endl;
	out << "total win  : " << win << std::endl;
	out << "total lose : " << lose << std::endl;
	out << "avg score  : " << float(score) / float(win+lose) << std::endl;
	out << "win rate   : " << float(win) / float(win+lose) << std::endl;
	out << "avg map num: " << float(indexCnt) / float(win + lose) << std::endl;
	out << "================================================\n";

	return 0;
}