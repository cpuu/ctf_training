# NX
Also known as Data Execution Prevention (DEP), this protection marks writable regions of memory as non-executable. This prevents the processor from executing in these marked regions of memory.

If we look at the memory map of a program compiled with NX protection, the stack and heap are typically marked non-executable.

In the following diagrams, we will be introducing a new indicator colour for the memory regions to denote 'writable and non-executable' mapped regions. Firstly, the stack before the read occurs looks like this:

When we perform the same attack, the buffer is overrun and the saved pointers are overwritten once again.

After the function returns, the program will set the instruction pointer to 0xbfff0000 and attempt to execute the instructions at that address. However, since the region of memory mapped at that address has no execution permissions, the program will crash.

Thus, the attacker's exploit is thwarted.

# Bypassing NX with Ret2Libc


![Fig 1. stack][classic1]

## 버퍼의 크기
## return address 위치 찾기
## system() 의 주소
```
gdb-peda$ p system
$1 = {<text variable, no debug info>} 0xb7e51b40 <__libc_system>

gdb-peda$ x/s 0xb7e51b40
0xb7e51b40 <__libc_system>:	"\203\354\f\213D$\020\350\061h\016"
```
## exit() 주소
```
gdb-peda$ p exit
$2 = {<text variable, no debug info>} 0xb7e457f0 <__GI_exit>

gdb-peda$ x/s 0xb7e457f0
0xb7e457f0 <__GI_exit>:	"\350\204+\017"
```

## /bin/sh
system()의 인자로 전달할 브레이크 포인트 걸어놓고 실행한 상태에서
```
gdb-peda$ find "/bin/sh"
Searching for '/bin/sh' in: None ranges
Found 1 results, display max 1 items:
libc : 0xb7f748c8 ("/bin/sh")
```



[classic1]: ./nxstack_view.png
