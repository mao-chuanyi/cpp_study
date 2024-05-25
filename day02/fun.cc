#include <iostream>

using std::cout;
using std::endl;

void f2(int &x, int &y)
{
int z = x;
x = y;
y = z;
}

void f3(int *x, int *y)
{
int z = *x;
*x = *y;
*y = z;
}

int main()
{
int x, y;
x = 10; y = 26;
cout << "x, y = " << x << ", " << y << endl;//x,y = 10, 26
f2(x, y);
cout << "x, y = " << x << ", " << y << endl;//x,y = 26, 10
f3(&x, &y);
cout << "x, y = " << x << ", " << y << endl;//x,y = 26, 10
x++; // ++x
y--;
f2(y, x);
cout << "x, y = " << x << ", " << y << endl;// x,y = 11, 25
return 0;
}
