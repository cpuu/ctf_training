.intel_syntax noprefix

# as -o call_echo.o call_echo.s
# gcc -o call_echo call_echo.o

# char* args[] = {"/bin/echo", "hello", "world", NULL};
# execve("/bin/echo", args, NULL);
# main () { execve("/bin/echo", args, NULL); }

.globl main
main:

mov eax, 0x0
push eax

push 0x0000006f
push 0x6863652f
push 0x6e69622f
mov ebx, esp

push eax
push 0x00646c72
push 0x6f77206f
push 0x6c6c6568
mov esi, esp

push eax
push esi
push ebx
mov ecx, esp

mov eax, 0xb 
int 0x80
