#include <iostream>

using namespace std;

#include "event.h"
#include "visitor.h"
#include "common_command.h"
//#include "CommCommand.h"

#include "Object_Pool.h"

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
int add_one(int n)
{
    cout << "add_one: n " << n << endl;
    return n + 1;
}

int add_one_two(int n, int m )
{
    cout << "add_one: n " << n << " m " << m << endl;
    return n + m;
}

class Bloop
{
public:
    int operator ()()
    {
        cout << "Bloop: operator() "<< endl;
        return 0;
    }
    int operator ()(int a)
    {
        cout << "Bloop: operator "<< endl;
        return a;
    }
};

struct STA
{
    int m_a;
    int operator ()() { return m_a; }
    int operator ()(int n) { return m_a + n; }
    int triple0()
    {
        cout << "triple0" << endl;
        return m_a * 3;
    }
    int triple(int a)
    {
        cout << "triple" << endl;
        return m_a * 3 + a;
    }
    int triple1() const
    {
        cout << "triple1 const" << endl;
        return m_a * 3;
    }
    const int triple2(int a) const
    {
        cout << "const triple2 const" << endl;
        return m_a * 3 + a;
    }

    void triple3() { cout << " " << endl;}
};

void test_wrap()
{
    //CommCommand<int> cmd;
    Common_Command<int> cmd;

    cmd.wrap(add_one, 0);
    cmd.execute();

    cmd.wrap(add_one_two, 0, 1);
    cout << cmd.execute();

    Bloop bloop;
    cmd.wrap(bloop);
    cmd.execute();
    cout << endl;
    cmd.wrap(bloop, 4);
    cmd.execute();

    STA t = {10};
    int x = 3;

    cmd.wrap(&STA::triple0, &t);
    cmd.execute();

    //cmd.warp(&STA::triple1, &t);
    cmd.execute();

    cmd.wrap(&STA::triple, &t, x);
    cmd.execute();

    cmd.wrap(&STA::triple, &t, 3);
    cmd.execute();

    cmd.wrap(&STA::triple2, &t, 3);
    cmd.execute();

    cmd.wrap([](int n){ return n + 1;}, 1);
}


struct Big_Object
{
    Big_Object() {}
    Big_Object(int a) {}
    Big_Object(const int & a, const int & b) {}
    ~Big_Object() { cout << "~Big_Object." << endl;}

    void print(const string& str)
    {
        cout << str << endl;
    }
};

void print(shared_ptr<Big_Object> p, const string& str)
{
    if( p != nullptr )
        p->print(str);
}


void test_big_object()
{
    Object_Pool<Big_Object> pool;
    pool.init(2);
    {
        auto p = pool.get();
        print(p, "p");

        auto p2 = pool.get();
        print(p2, "p2");
    }
    //cout << "1size " << pool.get_size() << endl;
    auto p = pool.get();
    auto p2 = pool.get();
    print(p, "p");
    print(p2, "p2");
    //cout << "2size " << pool.get_size() << endl;

    pool.init(2,1);
    auto p4 = pool.get<int>();
    print(p4, "p4");
    //cout << "3size " << pool.get_size() << endl;
    pool.init(2,1, 2);
    //cout << "4size " << pool.get_size() << endl;
    auto p5 = pool.get<int, int>();
    print(p5, "p5");
    #if 0
#endif
    cout << "size " << pool.get_size() << endl;
    cout << "11111" << endl;
}

int main()
{
    //test_event();
    //test_visitor();
    //test_wrap();
    test_big_object();
    cout << "Hello World!" << endl;
    return 0;
}

