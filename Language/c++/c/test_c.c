

#include "test_c.h"


static void arm_cpu_register_types(void)
{
    printf("%s,%d\n",__FUNCTION__,__LINE__);
}

type_init(arm_cpu_register_types);

int main()
{
    
}