#include <iostream>
using std::cout;
using std::endl;

    double add()
    {
        return 0;
    }

    template <class T, class ...Args>
    double add(T x, Args ...args)
    {
        return x + add( args...);
    }

void test(){
    cout << add(1, 2, 3.4, 1) << endl;

}

int main(){
    test();
    return 0;
}

