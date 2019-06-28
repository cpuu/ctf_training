#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_ALIENS (10)

typedef struct alien {
  int id;
  void (*action)(int);
} alien;

int go = 0;

void whatisthis()
{
  asm ("int3");
  if(go) {
    go = 0;
  } else {
    exit(-1);
  }
}

void oops(int signo) {
  go = 1;
}

void showflag(int id)
{
  FILE* f = fopen("/home/perplex/flag", "r");
  char buf[64];
  fgets(buf, sizeof(buf)-1, f);
  fclose(f);
  buf[32] = '\0';
  printf("Here is your flag: {%s}\n", buf);
}

void sleep(int id)
{
  printf("Alien (%d) is sleeping!\n", id);
}

unsigned int alien_cnt = 0;

alien* create_alien()
{
  alien* a = (alien*) malloc(sizeof(alien));
  if(a == NULL) return NULL;
  a->id = alien_cnt++;
  a->action = sleep;
  printf("(%p) an alien (%d) added!\n", a, a->id);
  return a;
}

void delete_alien(alien* a)
{
  printf("(%p) deleting alien %d\n", a, a->id);
  free(a);
}

unsigned long long* age = NULL;
alien* aliens[MAX_ALIENS] = {NULL, };

void show()
{
  int i = 0;
  for (i = 0; i < MAX_ALIENS; i++) {
    if(aliens[i]) {
      printf("(%p) Alien[%02d]\n", aliens[i], aliens[i]->id);
    }
  }
}

void add()
{
  int i = 0;
  for (i = 0; i < MAX_ALIENS; i++) {
    if(aliens[i]) continue;
    aliens[i] = create_alien();
    printf("added.. %d\n", i);
    break;
  }
}

int read_int()
{
  char buf[8];
  fgets(buf, sizeof(buf)-1, stdin);
  return atoi(buf);
}

void del()
{
  int id = 0;
  int i = 0;
  puts("Enter an alien ID:");
  id = read_int();
  for (i= 0; i < MAX_ALIENS; i++) {
    if(aliens[i] && aliens[i]->id == id) {
      delete_alien(aliens[i]);
      break;
    }
  }
}

void observe()
{
  int id = 0;
  int i = 0;
  puts("Enter an alien ID:");
  id = read_int();
  for (i= 0; i < MAX_ALIENS; i++) {
    if(aliens[i] && aliens[i]->id == id) {
      printf("observing.. %p\n", aliens[i]);
      aliens[i]->action(id);
      break;
    }
  }
}

void setage()
{
  char buf[128];
  if(age != NULL) free(age);
  age = (unsigned long long*) malloc(sizeof(unsigned long long));
  printf("(%p) Setting planet age: \n", age);
  fgets(buf, sizeof(buf)-1, stdin);
  *age = strtoll(buf, NULL, 10);
}

void showage()
{
  printf("Your planet's age: %llu\n", age ? *age : 0);
}

void perplexed()
{
  int choice = 0;
  char buf[8];

  puts("Menu:");
  puts("(1)\t Show aliens");
  puts("(2)\t Add alien");
  puts("(3)\t Delete alien");
  puts("(4)\t Observe alien");
  puts("(5)\t Set age.");
  puts("(6)\t Show age.");
  puts("Enter your choice (1-6):");
  whatisthis();

  fgets(buf, sizeof(buf)-1, stdin);
  choice = atoi(buf);
  switch(choice) {
  case 1: show(); break;
  case 2: add(); break;
  case 3: del(); break;
  case 4: observe(); break;
  case 5: setage(); break;
  case 6: showage(); break;
  default: break;
  }
  return perplexed();
}

int main(void)
{
  signal(SIGTRAP, oops);
  setbuf(stdout, NULL);
  whatisthis();
  printf("\033[2J\033[1;1H"); // clear screen
  puts("\n");

  perplexed();
  return 0;
}

