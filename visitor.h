#ifndef VISITOR_H
#define VISITOR_H

template<typename... Types>
struct Visitor;

template <typename T, typename... Types>
struct Visitor<T,Types...> : Visitor<Types...>
{
    using Visitor<Types...>::visit;
    virtual void visit(const T&) = 0;
};

template<typename T>
struct Visitor<T>
{
    virtual void visit(const T&) = 0;
};

#endif // VISITOR_H
