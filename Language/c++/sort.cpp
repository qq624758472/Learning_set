#include "sort.h"


int Sort::division()
{
    if( height == 0 )
    {
        throw "Division by zero condition!";
    }
    return width/height;    
}