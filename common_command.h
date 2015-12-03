#ifndef COMMON_COMMAND_H
#define COMMON_COMMAND_H

#include <functional>
#include <type_traits>

template<typename R = void>
struct Common_Command
{
public:
    template<class F, class... Args, class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type> //
    void wrap(F && f, Args && ... args)
    {
        //m_f = [&]{ return f(args...); };
        //m_f = std::bind([=](F && f1, Args && ... ar){ return f1(ar...);}, f, args...);
        //m_f = [&, args]{ return f(args...); };

        m_f = std::bind(f, args...);
    }


    //template<class C, class... DArgs, class P, class... Args>
    template<class R1, class C, class... DArgs, class P, class... Args>
    void wrap(R1(C::*f)(DArgs...), P && p, Args && ... args)
    {
        m_f = [&, f]{return (*p.*f)(args...); };
        //m_f = std::bind([=](R1(C::*f1)(DArgs...), P && p1, Args && ... args1){return (*p1.*f1)(args1...);}, f, p, args...);
    }


    //template<class C, class ... DArgs, class P, class... Args>
    template<class R1, class C, class ... DArgs, class P, class... Args>
    void wrap(R1(C::*f)(DArgs...) const, P && p, Args && ... args)
    {
        m_f = [&, f]{ return (*p.*f)(args...); };

        //m_f = std::bind([=](R1(C::*f1)(DArgs...) const, P && p1, Args && ... args1){ return (*p1.*f1)(args1...);}, f, p, args...);
        //m_f = std::bind( f, args...);

    }

    R execute()
    {
        return m_f();
    }

private:
    std::function< R ()> m_f;
};

#endif // COMMON_COMMAND_H
