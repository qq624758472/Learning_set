
#ifndef SORT
#define SORT

#include "common.h"

class Sort{
    //protected:
    public:
        int  width, height;
        int  a,b;

    public:
        Sort(int a=0,int b=0):width(a),height(b){}

        int area()
        {
            cout << "Parent class area:" << endl;
            return 0;
        }

        int division();
};


#endif