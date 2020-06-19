#ifndef P_CALSS
#define P_CALSS

#include "common.h"

class Shape{
    //protected:
    public:
        int  width, height;
        
        const int c_count; //常数据成员不能在构造函数中进行赋值。 可以在参数初始化表中进行初始化。
        mutable int m_count; //使用mutable修饰的是可变数据成员。 可以用const修饰的常成员函数修改它的值。

    public:
        Shape(int a=0, int b=0):c_count(a) 
        {
            m_count = 0;
            width = a;
            height = b;
        }

        int area()
        {
            cout << "Parent class area:" << endl;
            return 0;
        }

        /*
        定义为常成员函数，只能引用本类中的数据成员，且不能修改他们的值。
        可以被常对象调用。 否则编译报错。
        */
        int const_area() const   
        {
            // width = 10;  //常成员函数不能改变数据成员的值，否则，编译报错。   
            m_count = 5;    //常成员函数只能改变mutable修饰的可变数据成员。
            cout << "Parent class area: " << m_count <<  endl;
            return 0;
        }
};

class Rectangle: public Shape{
    public:
        //int width, height;
        Rectangle(int a=0,int b=0):Shape(a,b){}

        int print_data()
        {
            cout << "Rectangle.width="   << width  << "\n"
                 << "Rectangle.height="  << height << "\n"
            << endl;
			return 0;
        }
};

int p_class();
int const_class();
#endif