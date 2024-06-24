#include <iostream>
using std::cout;
using std::endl;

class Data{
public:
    Data(){
    }
    Data(int x):
        _data(x)
    {}
    int getData() const{
        return _data;
    }
    ~Data(){
    }
private:
    int _data = 10;
};

class MiddleLayer{
public:
    MiddleLayer(Data * p):
        _pdata(p)
    {}
    ~MiddleLayer(){
        if(_pdata){
            delete _pdata;
            _pdata = nullptr;
        }
    }
    Data * operator->(){
       return _pdata; 
    }
    Data & operator*(){
        return *_pdata;
    }
private:
    Data * _pdata;
};

class ThirdLayer{
public:
    ThirdLayer(MiddleLayer * pml):
        _ml(pml)
    {}
    ~ThirdLayer(){
        if(_ml){
            delete _ml;
            _ml = nullptr;
        }
    }
    MiddleLayer & operator->(){
        return *_ml;
    }
    
    /*Data & operator*(){//这个需要使用友元
        return *(*_ml)._pdata;
    }*/

    Data & operator*(){//不用调用友元的版本
        return *(*_ml);//在这里利用MiddleLayer中的"*"运算符重载
    }

private:
    MiddleLayer * _ml;
};

void test02(){
    ThirdLayer tl(new MiddleLayer(new Data()));
    cout << "tl-> getData(): " << tl->getData() << endl;
    cout << "(*tl).getData(): " << (*tl).getData() << endl;
}

void test01(){
    MiddleLayer ml(new Data());
    cout << "ml->getData(): " << ml->getData() << endl;
    cout << "(*ml).getData(): " << (*ml).getData() << endl;
}

int main(){
    test02();
    return 0;
}

