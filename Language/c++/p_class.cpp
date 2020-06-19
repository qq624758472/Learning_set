#include "p_class.h"

int p_class()
{
    Rectangle b(43,89);

    Shape shape,*a;  //定义类，和类指针。

    Shape *p = new Shape;   //动态定义类。

    a = &shape;

    cout << b.height << "  "<< a->width << "  " << p->height << endl;
	return 0;
}


int const_class()
{
    const Shape shape(11,8);
    // shape.area(); //错误，因为该方法不是const方法。
    shape.const_area();

    //子类初始化父类
    Rectangle test_son(77,88);
    test_son.print_data();
	return 0;
}