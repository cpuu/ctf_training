// gcc -o fms -fno-pic -mpreferred-stack-boundary=2 -no-pie -fno-stack-protector fms.c 

#include <stdio.h>

int main(int argc, char* argv[])
{
    char buf[512];
    fgets(buf, sizeof(buf), stdin);
    printf(buf);
    return 0;
}
