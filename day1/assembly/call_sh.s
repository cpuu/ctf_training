.intel_syntax noprefix

# as -o call_sh.o call_sh.s
# gcc -o call_sh call_sh.o

# char* args[] = {"/bin/sh", NULL};
# main () { execve("/bin/sh", args, NULL);}

.globl main
main:

push 0x0068732f
push 0x6e69622f

mov edx, 0
mov ecx, 0
mov ebx, esp
mov eax, 11
int 0x80

mov ebx, 0
mov eax, 1
int 0x80
