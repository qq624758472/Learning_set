/*
 * @Description:
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2023-03-21 09:57:33
 * @LastEditors: ruog__
 * @LastEditTime: 2023-03-22 09:51:35
 */
/*
多态按字面的意思就是多种形态。当类之间存在层次结构，并且类之间是通过继承关联时，就会用到多态。
C++ 多态意味着调用成员函数时，会根据调用函数的对象的类型来执行不同的函数。
派生类若和父类有同名方法，若父类方法不是虚函数， 则子类调用同名方法默认会调用父类方法。

虚函数，在类成员方法的声明（不是定义）语句前加“virtual”, 如 virtual void func()
纯虚函数，在虚函数后加“=0”，如 virtual void func()=0
对于虚函数，子类可以（也可以不）重新定义基类的虚函数，该行为称之为复写Override。
对于纯虚函数，子类必须提供纯虚函数的个性化实现。
*/
#ifndef __DUOTAI__
#define __DUOTAI__
#include <iostream>
using namespace std;

class Base
{
protected:
    int width, height;

public:
    Base(int a = 0, int b = 0)
    {
        width = a;
        height = b;
    }
    /*
    虚函数： 派生类调用的area函数就是他们本身定义的area而不是父类的area.
            若派生类没有该方法，就会调用父类的area。
    */
    virtual int area2()
    {
        cout << "Parent class area2" <<endl;
        return 0;
    }

    /*
    纯虚函数：
        基类中定义虚函数，以便在派生类中重新定义该函数更好地适用于对象，
        但是您在基类中又不能对虚函数给出有意义的实现，这个时候就会用到纯虚函数。
        如果类中至少有一个函数被声明为纯虚函数，则这个类就是抽象类。
    */
    virtual int area() = 0; // 抽象类
    void test() { cout << "This is base" << endl; }
};
class Aclass : public Base
{
public:
    Aclass(int a = 0, int b = 0) : Base(a, b) {}
    int area()
    {
        cout << "Aclass class area" << endl;
        return (width * height);
    }

    int area2()
    {
        cout << "Aclass class area2" << endl;
        return (width * height);
    }

    void test() { cout << "This is Aclass" << endl; }
};
class Bclass : public Base
{
public:
    Bclass(int a = 0, int b = 0) : Base(a, b) {}
    int area()
    {
        cout << "Bclass class area" << endl;
        return (width * height / 2);
    }

    void test() { cout << "This is Bclass" << endl; }
};

int duoTaiTest();

#endif