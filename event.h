#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>
#include <functional>
#include <map>
using namespace std;

#include "non_copyable.h"

template <typename Func>
class Events : Non_Copyable
{
public:
    Events() {}
    ~Events() {}

    int connect(Func&&f)
    {
        return assign(f);
    }

    int connect(const Func& f)
    {
        return assign(f);
    }

    void disconnect(int key)
    {
        m_connectons.erase(key);
    }

    template<typename... Args>
    void notify(Args&&... args)
    {
        for(auto& it : m_connectons)
        {
            it.second(std::forward<Args>(args)...);
        }
    }
private:
    template<typename F>
    int assign(F&&f)
    {
        int k = m_observer_id ++;
        m_connectons.emplace(k, std::forward<F>(f));
        return k;
    }

private:
    int m_observer_id = 0;
    std::map<int, Func> m_connectons;
};

#endif // EVENT_H
