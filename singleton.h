#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
    template<typename... Args>
    static T* instance(Args... args)
    {
        if(m_pinstance == nullptr)
        {
            m_pinstance = new T(std::forward<Args>(args)...);
        }
        return m_pinstance;
    }

    static T* get_instance()
    {
        if( m_pinstance == nullptr )
            throw std::logic_error("the instance is not init, please initialize the instance first");
        return m_pinstance;
    }

    static void distroy_instance()
    {
        delete m_pinstance;
        m_pinstance = nullptr;
    }

private:
    Singleton();
    virtual ~Singleton();
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
private:
    static T* m_pinstance;
};

template<class T>
T* Singleton<T>::m_pinstance = nullptr;

#endif // SINGLETON_H
