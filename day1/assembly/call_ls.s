.intel_syntax noprefix

# as -o call_ls.o call_ls.s
# gcc -o call_ls call_ls.o

# char* args[] = {"/bin/ls", "-lrh", NULL};
# main () { execve("/bin/ls", args, NULL);}

.globl main
main:

mov eax, 0x0
push eax

push 0x736c2f2f
push 0x6e69622f
mov ebx, esp

push eax
push 0x68726c2d
mov esi, esp

push eax
push esi
push ebx
mov ecx, esp

mov eax, 0xb 
int 0x80
