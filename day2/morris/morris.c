// gcc -o morris -O0 -fno-pic -mpreferred-stack-boundary=2 -no-pie -fno-stack-protector morris.c
#include <stdio.h>
int main(int argc, char* argv[])
{
    char line[512];
    gets(line);
    printf(line);
    return 0;
}
    
