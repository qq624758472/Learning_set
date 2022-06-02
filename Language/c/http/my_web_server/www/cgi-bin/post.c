#include <stdio.h>
#include <stdlib.h>
void main()
{
    int i, n;
    printf("Content - type: text / plain\n\n");
    n = 0;
    if (getenv("CONTENT - LENGTH"))
        n = atoi(getenv("CONTENT - LENGTH"));
    for (i = 0; i < n; i++)
        putchar(getchar());
    putchar('\n');
    fflush(stdout);
}