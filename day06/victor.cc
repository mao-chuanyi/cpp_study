#include <iostream>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

void test(){
    vector<int> number3{1,2,3,4,5,6,7};
    vector<int> number4(number3.begin(),number3.end() - 3);
    vector<int>::iterator pBegin = number4.begin();
	vector<int>::iterator pEnd = number4.end();

	//第一种遍历方式：
	while (pBegin != pEnd) {
		cout << *pBegin << endl;
		pBegin++;
	}
}

int main(){
    test();
    return 0;
}

