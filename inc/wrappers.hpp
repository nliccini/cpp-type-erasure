#ifndef __WRAPPERS_H
#define __WRAPPERS_H

#include <string>

namespace erasure
{
    namespace i
    {
        class wrapper {
            public:
                virtual ~wrapper() = default;
                virtual size_t size() const = 0;
                virtual std::string type() const = 0;
        };
    }

    template<typename T>
    class wrapper : public i::wrapper {
        public:
            wrapper()
            { }

            wrapper(T const& t) : 
                _impl(t)
            { }

            size_t size() const override { return sizeof(_impl); }
            std::string type() const override {return typeid(T).name(); }
            
            T item() const { return _impl; }

        private:
            T _impl;
    };

    typedef wrapper<int> int_wrapper;
    typedef wrapper<double> double_wrapper;
    typedef wrapper<std::string> string_wrapper;
}

#endif // __WRAPPERS_H