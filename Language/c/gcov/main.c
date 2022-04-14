#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{   
    if (argc >=2) {
        printf("=====argc>=2\n");
        return;
    }
    printf("helloworld begin\n");

    if (argc <2){
        printf("=====argc<2\n");
        return;
    }
    return;
}
