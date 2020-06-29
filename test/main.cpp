#include <iostream>
#include <vector>
#include <delegate.hpp>
#include <wrappers.hpp>

bool compare(int a, int b){ return a == b; }

class MyClass {
    public:
        bool compare(int a, int b) { return a == b; }
        bool compare_const(int a, int b) const { return a == b; }
};

int main(int argc, char *argv[])
{
    MyClass mc;
    std::function<bool(int,int)> func = [&](int a, int b){ return mc.compare(a, b); };

    using namespace erasure;
    std::vector<delegate<bool(int, int)>> delegates;
    delegates.push_back(delegate(compare));
    delegates.push_back(delegate(&mc, &MyClass::compare));
    delegates.push_back(delegate(&mc, &MyClass::compare_const));
    delegates.push_back(delegate(func));
    delegates.push_back(delegate<bool(int,int)>([mc](int a, int b){ return mc.compare_const(a, b); }));

    std::cout << "Testing delegates" << std::endl;
    for(auto& d : delegates)
    {
        std::string ret = d(10, 10) == 0 ? "false" : "true";
        std::cout << ret << std::endl;
    }

    std::cout << std::endl;
    std::vector<std::unique_ptr<i::wrapper>> wrappers;
    wrappers.push_back(std::make_unique<int_wrapper>(10));
    wrappers.push_back(std::make_unique<double_wrapper>(10.));
    wrappers.push_back(std::make_unique<string_wrapper>("hello"));

    std::cout << "Testing type wrappers" << std::endl;
    for(auto& w : wrappers)
    {
        std::cout << "type: " << w->type() << " size: " << w->size() << " bytes" << std::endl;
    }
}
