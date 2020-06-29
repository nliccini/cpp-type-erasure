#ifndef __DELEGATE_H
#define __DELEGATE_H

#include <memory>
#include <functional>

namespace erasure
{
    namespace details
    {
        namespace i
        {
            template<typename RET, typename... ARGS>
            class func_wrapper {
                public:
                    virtual ~func_wrapper() = default;
                    virtual RET func(ARGS... args) = 0;
            };
        }

        template<typename T, typename RET, typename... ARGS>
        class member_func_wrapper : public i::func_wrapper<RET, ARGS...> {
            public:
                member_func_wrapper(T *t, RET (T::*func)(ARGS...)) : 
                    _t(t), _func(func)
                { }

                RET func(ARGS... args) override
                {
                    return std::invoke(_func, _t, args...);
                }
            
            private:
                T* _t;
                RET (T::*_func)(ARGS...);
        };


        template<typename T, typename RET, typename... ARGS>
        class const_member_func_wrapper : public i::func_wrapper<RET, ARGS...> {
            public:
                const_member_func_wrapper(const T *t, RET (T::*func)(ARGS...) const) : 
                    _t(t), _func(func)
                { }

                RET func(ARGS... args) override
                {
                    return std::invoke(_func, _t, args...);
                }
            
            private:
                const T* _t;
                RET (T::*_func)(ARGS...) const;
        };

        template<typename RET, typename... ARGS>
        class nonmember_func_wrapper : public i::func_wrapper<RET, ARGS...> {
            public:
                nonmember_func_wrapper(RET (*func)(ARGS...)) :
                    _func(func)
                { }

                RET func(ARGS... args) override
                {
                    return _func(args...);
                }   

            private:         
                RET (*_func)(ARGS...);
        };

        template<typename RET, typename... ARGS>
        class func_obj_wrapper : public i::func_wrapper<RET, ARGS...> {
            public:
                func_obj_wrapper(std::function<RET(ARGS...)> func) :
                    _func(func)
                { }

                RET func(ARGS... args) override
                {
                    return std::invoke(_func, args...);
                }

            private:
                std::function<RET(ARGS...)> _func;
        };
    }

    template<typename>
    class delegate;

    template<typename RET, typename... ARGS>
    class delegate<RET(ARGS...)> {
        public:
            template<typename T>
            delegate(T* t, RET (T::*func)(ARGS...)) :
                _wrapper(std::make_unique<details::member_func_wrapper<T, RET, ARGS...>>(t, func))
            { }
            
            template<typename T>
            delegate(const T* t, RET (T::*func)(ARGS...) const) :
                _wrapper(std::make_unique<details::const_member_func_wrapper<T, RET, ARGS...>>(t, func))
            { }

            delegate(RET (func)(ARGS...)) :
                _wrapper(std::make_unique<details::nonmember_func_wrapper<RET, ARGS...>>(func))
            { }

            delegate(std::function<RET(ARGS...)> func) :
                _wrapper(std::make_unique<details::func_obj_wrapper<RET, ARGS...>>(func))
            { }

            template<typename Lambda>
            delegate(Lambda func) :
                delegate(static_cast<std::function<RET(ARGS...)>>(func))
            { }

            RET operator()(ARGS... args)
            {
                return _wrapper->func(args...);
            }

        private:
            std::unique_ptr<details::i::func_wrapper<RET, ARGS...>> _wrapper;
    };

    template<typename T, typename RET, typename... ARGS>
    delegate(T*, RET(T::*)(ARGS...)) -> delegate<RET(ARGS...)>;

    template<typename T, typename RET, typename... ARGS>
    delegate(const T*, RET(T::*)(ARGS...) const) -> delegate<RET(ARGS...)>;

    template<typename RET, typename... ARGS>
    delegate(RET(*)(ARGS...)) -> delegate<RET(ARGS...)>;

    template<typename RET, typename... ARGS>
    delegate(std::function<RET(ARGS...)>) -> delegate<RET(ARGS...)>;
}

#endif // __DELEGATE_H