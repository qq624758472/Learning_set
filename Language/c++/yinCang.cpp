/*
 * @Description:重载，覆盖，与隐藏
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-04-04 09:41:37
 * @LastEditors: ruog__
 * @LastEditTime: 2023-04-04 11:03:45
 */
#include <iostream>
#include "yinCang.h"
using namespace std;
using namespace yinCang;
namespace yinCang
{
    int test(void)
    {
        Derived d;
        Base e;
        Base *pb = &d;
        Derived *pd = &d;
        Base *pe = &e;

        pb->f(42);    // Base::f(int) 42        重载
        pb->f(3.14f); // Base::f(float) 3.14    重载
        pb->g();      // Derived::g(void)       覆盖

        //=======================隐藏
        /**
         * 除了虚函数定义，且参数相同的情况下是覆盖，其他类型都会根据指针来决定。
         */
        cout << endl;
        // a:虚函数,参数相同，. Good : behavior depends solely on type of the object
        pb->a(3.14f); // Derived::a(float) 3.14             覆盖
        pd->a(3.14f); // Derived::a(float) 3.14             覆盖
        pe->a(3.14f); // Base::a(float) 3.14
        // aa:虚函数,参数不同，. Good : behavior depends solely on type of the object
        pb->aa(3.14f); // Base::aa(float) 3.14            根据指针类型决定
        pd->aa(3.14f); // Derived::aa(int) 3               会隐藏基类方法
        pe->aa(3.14f); // Base::aa(float) 3.14
        // b:非虚函数，参数不同.  Bad : behavior depends on type of the pointer
        pb->b(3.14f); // Base::b(float) 3.14
        pd->b(3.14f); // Derived::b(int) 3 (surprise!)      隐藏,虽然参数不同，也不会调用基类
        pe->b(3.14f); // Base::b(float) 3.14
        // b:非虚函数，参数相同.  Bad : behavior depends on type of the pointer
        pb->c(3.14f); // Base::c(float) 3.14 (surprise!)
        pd->c(3.14f); // Derived::c(float) 3.14             参数相同，取决于指针类型
        pe->c(3.14f); // Base::c(float) 3.14                base指针的话，不管啥都是调用base类的方法
    }
}