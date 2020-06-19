/*
多态按字面的意思就是多种形态。当类之间存在层次结构，并且类之间是通过继承关联时，就会用到多态。

C++ 多态意味着调用成员函数时，会根据调用函数的对象的类型来执行不同的函数。

派生类若和父类有同名方法，若父类方法不是虚函数， 则子类调用同名方法默认会调用父类方法。 
*/

#include <iostream> 
using namespace std;
 
class Shape {
    protected:
        int width, height;
    public:
        Shape( int a=0, int b=0)
        {
            width = a;
            height = b;
        }
        /*
        虚函数： , 派生类调用的area函数就是他们本身定义的area而不是父类的area.
                若派生类没有该方法，而调用了area的虚函数， 此时就算父类是虚函数， 也会调用成功。
        */
        //   virtual int area()  
        //   {
        //      cout << "Parent class area :" <<endl;
        //      return 0;
        //   }

        /*
        纯虚函数：
            基类中定义虚函数，以便在派生类中重新定义该函数更好地适用于对象，
            但是您在基类中又不能对虚函数给出有意义的实现，这个时候就会用到纯虚函数。
            如果类中至少有一个函数被声明为纯虚函数，则这个类就是抽象类。
        */
        virtual int area() = 0; //抽象类
};
class Rectangle: public Shape{
   public:
      Rectangle( int a=0, int b=0):Shape(a, b) { }
      int area ()
      { 
         cout << "Rectangle class area :" <<endl;
         return (width * height); 
      }
};
class Triangle: public Shape{
   public:
      Triangle( int a=0, int b=0):Shape(a, b) { }
      int area ()
      { 
         cout << "Triangle class area :" <<endl;
         return (width * height / 2); 
      }
};
// 程序的主函数
int duoTaiTest()
{
   Shape *shape;
   Rectangle rec(10,7);
   Triangle  tri(10,5);
 
   // 存储矩形的地址
   shape = &rec;
   // 调用矩形的求面积函数 area
   shape->area();
 
   // 存储三角形的地址
   shape = &tri;
   // 调用三角形的求面积函数 area
   shape->area();
   
   return 0;
}