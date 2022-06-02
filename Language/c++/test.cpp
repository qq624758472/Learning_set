/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-05-23 17:21:23
 * @LastEditors: ruog__
 * @LastEditTime: 2022-05-23 17:27:29
 */
#include <iostream>
using namespace std;


class vector3d
{
    vector3d();
    ~vector3d();
public:
    float a;
    float b;
    float c;

    distanceTo();
};

vector3d::vector3d(float a,float b,float c)
{
    this->a = a;
    this->b = b;
    this->c = c;
}

vector3d::~vector3d()
{
    
}

int main()
{

}