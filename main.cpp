#include <iostream>

using namespace std;

#include "event.h"
#include "visitor.h"
#include "common_command.h"

struct stA;
struct stB;
struct stC;

struct Base
{
    typedef Visitor<stA, stB, stC> My_Visitor;
    virtual void accept(My_Visitor&) = 0;
};

struct stA : Base
{
    double val;
    void accept(Base::My_Visitor& v)
    {
        v.visit(*this);
    }

    int a;
    int b;
    void print(int a, int b)
    {
        cout << "stA: a" << a << ", " << b << endl;
    }
};

struct stB : Base
{
    int val;
    void accept(Base::My_Visitor& v)
    {
        v.visit(*this);
    }
};

struct stC : Base
{
    int val;
    void accept(Base::My_Visitor& v)
    {
        v.visit(*this);
    }
};

struct Print_Visitor : Base::My_Visitor
{
    void visit(const stA& a)
    {
        std::cout << "from stA: " << a.val << std::endl;
    }

    void visit(const stB& b)
    {
        std::cout << "from stB: " << std::endl;
    }

    void visit(const stC& b)
    {
        std::cout << "from stC: " << std::endl;
    }
};


void test_visitor()
{
    Print_Visitor vis;
    stA a;
    a.val = 8.97;
    stB b;
    b.val = 8;

    stC c;
    c.val = 987656;

    Base* base = &a;
    base->accept(vis);

    base = &b;
    base->accept(vis);

    base = &c;
    base->accept(vis);
}

void print(int a, int b)
{
    cout << "a " << a << ", " << b << endl;
}

void test_event()
{
    Events<std::function<void(int,int)>> my_event;

    auto key = my_event.connect(print);

    stA t;
    auto lambda_key = my_event.connect([&t](int a,int b){t.a = a; t.b = b;});

    std::function<void(int, int)> f = std::bind(&stA::print, &t, std::placeholders::_1, std::placeholders::_2);

    cout << "key " << key << "lambda_key "<< lambda_key << " count " << my_event.connect(f) << endl;

    int a = 1;
    int b = 2;

    my_event.notify(a,b);
    cout << "t a" << t.a << "," << t.b << endl;
}


//
struct STA
{
    int m_a;
    int operator ()() { return m_a; }
    int operator ()(int n) { return m_a + n; }
    int triple0() { return m_a * 3; }
    int triple(int a) { return m_a * 3 + a; }
    int triple1() const { return m_a * 3; }
    const int triple2(int a) const { return m_a * 3 + a; }
};

int add_one(int n)
{
    return n + 1;
}

class Bloop
{
public:
    int operator ()(int a)
    {
        return a;
    }
};

void test_wrap()
{

    Common_Command<int> cmd;
    cmd.warp(add_one, 0);
#if 0
    cmd.warp([](int n){ return n + 1;});

    Bloop bloop;
    cmd.warp(bloop);
    cmd.warp(bloop, 4);
#endif
}

int main()
{
    //test_event();
    test_visitor();
    cout << "Hello World!" << endl;
    return 0;
}

