#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <string>
#include <functional>
#include <memory>
#include <map>
#include "non_copyable.h"

using namespace std;


const int max_object_num = 10;

template <typename T>
class Object_Pool : Non_Copyable
{
    template<typename... Args>
    using Constructor = std::function<std::shared_ptr<T>(Args...)>;

public:
    template<typename... Args>
    void init(size_t num, Args&&... args)
    {
        if( num <= 0 || num > max_object_num )
            throw std::logic_error("object num out of range.");

        auto construct_name = typeid(Constructor<Args...>).name();
        for(size_t i = 0; i < num; i++)
        {
            cout << "size " << m_object_map.size() << endl;
            m_object_map.emplace(construct_name, shared_ptr<T>(new T(std::forward<Args>(args)...), [this, construct_name](T*p)
            {
                cout << "delete ...." << endl;
                m_object_map.emplace(std::move(construct_name), std::shared_ptr<T>(p));
            }));
            cout << construct_name << endl;
        }
    }

    template<typename... Args>
    std::shared_ptr<T> get()
    {
        string construct_name = typeid(Constructor<Args...>).name();

        cout << "get() size111 " << m_object_map.size() << endl;
        auto range = m_object_map.equal_range(construct_name);
        for(auto it = range.first; it != range.second; ++it )
        {
            auto ptr = it->second;
            m_object_map.erase(it);
            cout << "get() size22 " << m_object_map.size() << endl;
            return ptr;
        }
        cout << "nullptr "<< endl;
        return nullptr;
    }

    int get_size()
    {
        return m_object_map.size();
    }

private:
    multimap<string, std::shared_ptr<T>> m_object_map;
};

#endif // OBJECT_POOL_H
