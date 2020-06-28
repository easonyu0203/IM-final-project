#include<fstream>
#include<string>

int main(){

	std::ifstream in("testing_data.txt");
	int win = 0, lose = 0;
	std::string str;
	while(in >> str){
		if(str == "win") win++;
		else if(str == "lose") lose++;
		else continue;
	}
	in.close();
	std::ofstream out("testing_data.txt", std::ios:: app);
	out << "\n================================================\n";
	out << "total round: " << win + lose << std::endl;
	out << "total win  : " << win << std::endl;
	out << "total lose : " << lose << std::endl;
	out << "win rate   : " << float(win) / float(win+lose) << std::endl;
	out << "================================================\n";

	return 0;
}