#include "common.h"
#include "pipe.h"

#define DEBUG1

int main()
{
    Pipe test;
#ifdef DEBUG1
    test.pipe_use();
#endif

#ifdef DEBUG
    test.pipe_name_use();
#endif

}