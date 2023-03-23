/*
 * @Description: 
 * @Version: 2.0
 * @Autor: ruog__
 * @Date: 2022-05-23 17:21:23
 * @LastEditors: ruog__
 * @LastEditTime: 2023-03-22 09:50:25
 */
#include <iostream>
using namespace std;


class vector3d
{
    vector3d(float,float,float);
    ~vector3d();
public:
    float a;
    float b;
    float c;

    bool distanceTo();
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
