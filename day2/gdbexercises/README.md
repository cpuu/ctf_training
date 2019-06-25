# Show the proof that your patch works!

## Source
```
$ cat gdbexercise.c
#include <stdio.h>

void secret()
{
    puts("SECRET!");
}

void normal()
{
    puts("NORMAL");
}

int main()
{
    normal();
    return 0;
}
```
현 상태는 main 이 normal만을 호출하고 종료되도록 의도된 프로그램임.
여기에서 secret이 수행되도록 바이너리 패치해보자.

## Patch Me 1
![Fig 1.1. Normal Program][img1]

/ main 함수에서 normal 함수를 호출하는 부분을 secret으로 바꿈 
![Fig 1.2. Patch 1 : ][img2]

## Patch Me 2
![Fig 1.3. Normal Program][img3]

/ main 함수가 normal 함수를 호출하는 것은 그대로 두고, normal의 실행 앞부분에 jmp secret 으로 덮어씀
![Fig 1.4. Patch 2 : ][img4]

[img1]: ./1.png
[img2]: ./2.png
[img3]: ./3.png
[img4]: ./4.png
