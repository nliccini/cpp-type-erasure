#include <iostream>
#include <delegate.hpp>

bool test_func(int a, int b)
{
    return a == b;
}

int main(int argc, char* arg[])
{
    erasure::delegate d(test_func);
    std::cout << d(10, 10) << std::endl;
    std::cout << d(10, 5) << std::endl;
    std::cout << d(5, 10) << std::endl;
}