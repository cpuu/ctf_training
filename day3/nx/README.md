# NX

버퍼의 크기
return address 위치 찾기
system() 의 주소
```
gdb-peda$ p system
$1 = {<text variable, no debug info>} 0xb7e51b40 <__libc_system>

gdb-peda$ x/s 0xb7e51b40
0xb7e51b40 <__libc_system>:	"\203\354\f\213D$\020\350\061h\016"
```
exit() 주소
```
gdb-peda$ p exit
$2 = {<text variable, no debug info>} 0xb7e457f0 <__GI_exit>

gdb-peda$ x/s 0xb7e457f0
0xb7e457f0 <__GI_exit>:	"\350\204+\017"
```

system()의 인자로 전달할 /bin/sh의 위치
브레이크 포인트 걸어놓고 실행한 상태에서
```
gdb-peda$ find "/bin/sh"
Searching for '/bin/sh' in: None ranges
Found 1 results, display max 1 items:
libc : 0xb7f748c8 ("/bin/sh")
```

![Fig 1. stack][classic1]


버퍼의 크기
return address 위치 찾기
system() 의 주소
system()의 인자로 전달할 /bin/sh의 위치


[classic1]: ./nxstack_view.png
