# cpp-type-erasure
A header-only library implementation of the delegate pattern using type erasure in C++

# Requirements

Your system must have support for C++17 and cmake with basic compile tools (MSVC or GCC)

    sudo apt-get install build-essential cmake

# Usage

The delegate pattern makes use of C++ templating to erase type information without sacrificing type-safety or requiring our types to inherit pure virtual base classes. Thus, this library enables a user to encapsulate data types that do not share anything in common other than providing a similar set of APIs while supporting things such as containers. 

This library could be implemented using `void *` or inheritance, but those methods do not accomplish both type erasure AND type-safety. 

The `delegate.hpp` header-only library includes one public class called `delegate`. The `details` namespace contains private implementation details not intended for end-use. 

<b>NOTE:</b> This library does not support automatic type deduction for lambda delegates. You must specify the template parameters for your delegate object if you wish to pass in a lambda as an argument: 

    erasure::delegate<bool(int,int)> d([](int a, int b){ return a == b; });

To use the `delegate` class, consider these scenarios:

* A global-scope function
* A member function
* A const member function
* A function object
* A lambda function 

As long as each function has the same signature, the `delegate` class can be used to encapsulate the functionality without being tied to the source of the function.

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
        std::function<bool(int,int)> func = [&](int a, int b){ return mc.compare(a, b); };

        using namespace erasure;
        std::vector<delegate<bool(int, int)>> delegates;
        delegates.push_back(delegate(compare));
        delegates.push_back(delegate(&mc, &MyClass::compare));
        delegates.push_back(delegate(&mc, &MyClass::compare_const));
        delegates.push_back(delegate(func));
        delegates.push_back(delegate<bool(int,int)>([mc](int a, int b){ return mc.compare_const(a, b); }));

        for(auto& d : delegates)
        {
            std::cout << d(10, 10) << std::endl;
        }
    }

Should produce the following output:

    1
    1
    1
    1
    1

# Building the Test

To build, use CMake:

    cd cpp-type-erasure
    mkdir build && cd build
    cmake ..
    cmake --build .

To run the test application:

    ./type-erasure-test

