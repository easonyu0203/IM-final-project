#include<iomanip>
#include<iostream>
#include<tuple>
#include<utility>
#include<queue>
#include<set>
#include<tuple>
#include<vector>
#include<cmath>
#include<deque>
#include<stack>
#include<utility>
#include<iostream>
#include<iomanip>
#include<fstream>


int _less(const int a, const int b){
	return a < b;
}

int main(){

	auto cmp =  [](int a, int b){return a > b;};
	std::priority_queue<int, std::vector<int>, decltype(cmp)> q(cmp);
	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	q.push(5);
	q.push(6);
	std::cout << q.top() << std::endl;

	return 0;
}