#ifndef __YINCANG__
#define __YINCANG__
#include <iostream>
using namespace std;
namespace yinCang
{
    class Base
    {
    public:
        void f(int x) { cout << "Base::f(int) " << x << endl; }
        void f(float x) { cout << "Base::f(float) " << x << endl; }
        virtual void g(void) { cout << "Base::g(void)" << endl; }

        virtual void a(float x) { cout << "Base::a(float) " << x << endl; }
        virtual void aa(float x) { cout << "Base::aa(float) " << x << endl; }
        void b(float x) { cout << "Base::b(float) " << x << endl; }
        void c(float x) { cout << "Base::c(float) " << x << endl; }
    };

    class Derived : public Base
    {
    public:
        virtual void g(void) { cout << "Derived::g(void)" << endl; }

        virtual void a(float x) { cout << "Derived::a(float) " << x << endl; }
        virtual void aa(int x) { cout << "Derived::aa(int) " << x << endl; }
        void b(int x) { cout << "Derived::b(int) " << x << endl; }
        void c(float x) { cout << "Derived::c(float) " << x << endl; }
    };

    int test(void);
}

#endif