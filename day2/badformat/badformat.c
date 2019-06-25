#include <stdio.h>

void printer()
{
  char buf[256];
  fgets(buf, sizeof(buf) - 1, stdin);
  fprintf(stdout, buf);
}

int main(int argc, char* argv[])
{
  if ( argc > 1 ) return -1;

  printer();

  return 0;
}
