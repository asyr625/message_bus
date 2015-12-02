#ifndef COMMON_COMMAND_H
#define COMMON_COMMAND_H

#include <functional>
#include <type_traits>

template<typename R = void>
struct Common_Command
{
public:
    template<class F, class... Args, class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
    void warp(F && f, Args && ... args)
    {
        //m_f = [&]{ return f(args...); };
        m_f = std::bind([=](F && f1, Args && ... ar){ return f1(ar...);}, f, args...);
        //m_f = [&, args]{ return f(args...); };
    }

    //template<class R, class C, class ... DArgs, class P, class... Args>
    template<class C, class ... DArgs, class P, class... Args>
    void warp(R(C::*f)(DArgs...) const, P && p, Args && ... args)
    {
        //m_f = [&, f]{ return (*p.*f)(args...); };

        m_f = std::bind([=](R(C::*f1)(DArgs...) const, P && p1, Args && ... args1){ return (*p1.*f1)(args1...);}, f, p, args...);
    }

    //template<class R, class C, class... DArgs, class P, class... Args>
    template<class C, class... DArgs, class P, class... Args>
    void wrap(R(C::*f)(DArgs...), P && p, Args && ... args)
    {
        //m_f = [&, f]{return (*p.*f)(args...); };
        m_f = std::bind([=](R(C::*f1)(DArgs...), P && p1, Args && ... args1){return (*p1.*f1)(args1...);}, f, p, args...);
    }

    R execute()
    {
        return m_f();
    }

private:
    std::function< R ()> m_f;
};

#endif // COMMON_COMMAND_H
