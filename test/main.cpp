#include <iostream>
#include <vector>
#include <delegate.hpp>

bool compare(int a, int b){ return a == b; }

class MyClass {
    public:
        bool compare(int a, int b) { return a == b; }
        bool compare_const(int a, int b) const { return a == b; }
};

int main(int argc, char *argv[])
{
    MyClass mc;

    using namespace erasure;
    std::vector<delegate<bool(int, int)>> delegates;
    delegates.push_back(delegate(compare));
    delegates.push_back(delegate(&mc, &MyClass::compare));
    delegates.push_back(delegate(&mc, &MyClass::compare_const));

    for(auto& d : delegates)
    {
        std::cout << d(10, 10) << std::endl;
    }
}
