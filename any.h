#ifndef ANY_H
#define ANY_H

#include <memory>
#include <typeindex>

struct Any
{
    Any(void)
        : m_tp_index( std::type_index(typeid(void)) )
    {
    }

    Any(const Any& that)
        : m_ptr(that.clone()),
          m_tp_index(that.m_tp_index)
    {
    }

    Any(Any&& that)
        : m_ptr(std::move(that.m_ptr)),
          m_tp_index(that.m_tp_index)
    {
    }


    template<typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type>
    Any(U&& value)
        : m_ptr(new Derived<typename std::decay<U>::type>(forward<U>(value))),
          m_tp_index(type_index(typeid(typename std::decay<U>::type)))
    {
    }

    bool is_null() const
    {
        return !bool(m_ptr);
    }

    template<class U>
    bool is() const
    {
        return m_tp_index == type_index(typeid(U));
    }

    template<class U>
    U& any_cast()
    {
        if( !is<U>())
        {
            cout << "can not cast " << typeid(U).name() << " to " << m_tp_index.name() << endl;
            throw bad_cast();
        }
        auto derived = dynamic_cast<Derived<U>*>(m_ptr.get());
        return derived->m_value;
    }

    Any& operator =(const Any&a)
    {
        if( m_ptr == a.m_ptr )
            return *this;

        m_ptr = a.clone();
        m_tp_index = a.m_tp_index;
        return *this;
    }

private:
    struct Base;
    typedef std::unique_ptr<Base> Base_Ptr;

    struct Base
    {
        virtual ~Base() {}
        virtual Base_Ptr clone() const = 0;
    };

    template<typename T>
    struct Derived : Base
    {
        template<typename U>
        Derived(U&& value)
            : m_value(forward<U>(value))
        {
        }

        Base_Ptr clone() const
        {
            return Base_Ptr(new Derived<T>(m_value));
        }
        T m_value;
    };

    Base_Ptr clone() const
    {
        if( m_ptr != nullptr )
            return m_ptr->clone();
        return nullptr;
    }

    Base_Ptr m_ptr;
    std::type_index m_tp_index;
};


#endif // ANY_H
