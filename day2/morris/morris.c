// gcc -o morris morris.c -O0 -fno-pic -no-pie -mpreferred-stack-boundary=2 
#include <stdio.h>
int main(int argc, char* argv[])
{
    char line[512];
    gets(line);
    printf(line);
    return 0;
}
    
