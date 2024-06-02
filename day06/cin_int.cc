#include <iostream>
#include <limits>
using std::cout;
using std::endl;
using std::cin;

void test(){
    cout << "input a int num" << endl;
    int num;
    cin >> num;
    while(!cin.good()) //输入有问题就不断循环
    {
        cout << "try again!" << endl;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin >> num;
    }
    cout << "num = " << num << endl;
}

int main(){
    test();
    return 0;
}

