#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSGS    (42)

typedef enum MsgType {
    NAME,
    ID
} MsgType;

typedef struct Message {
    MsgType msgType;
    union {
        char* name;
        int id;
    };
} Message;

static Message* msgArray[MAX_MSGS] = {NULL, };
static unsigned int msgNum = 0;

void error(const char* msg)
{
    puts(msg);
    exit(-1);
}

int getIntInput()
{
    char buf[32];
    if (!fgets(buf, sizeof(buf)-1, stdin)) error("Bye bye.");
    return atoi(buf);
}

char* getStringInput()
{
    char buf[128];
    if (!fgets(buf, sizeof(buf)-1, stdin)) error("Bye bye.");
    return strdup(buf);
}

int pushMessage(Message* m)
{
    if (msgNum < MAX_MSGS) {
        msgArray[msgNum++] = m;
    } else {
        error("Too many messages in the storage.");
    }

    return msgNum - 1;
}

Message* msgAlloc(MsgType t)
{
    Message* m = (Message*) malloc(sizeof(Message));
    if (!m) error("malloc");
    m->msgType = t;
    return m;
}

Message* createName()
{
    Message* m = msgAlloc(NAME);
    puts("Enter name:");
    m->name = getStringInput();
    if (!m->name) error("getStringInput");
    return m;
}

Message* createID()
{
    Message* m = msgAlloc(ID);
    puts("Enter id:");
    m->id = getIntInput();
    return m;
}

void insertMessage()
{
    Message* m = NULL;
    unsigned int slot = 0;

    puts("Message type? (0 for NAME and 1 for ID)");
    switch ( getIntInput() ) {
    case 0:
        m = createName();
        break;
    case 1:
    default:
        m = createID();
        break;
    }

    slot = pushMessage(m);
    printf("Message created at [%d]\n", slot);
}

const char* type2str(MsgType t)
{
    return (t == NAME ? "Name" : "ID");
}

unsigned int getMsgNum()
{
    unsigned int n = 0;
    printf("Message number? (0-%d)", MAX_MSGS);
    n = getIntInput();
    if (n >= MAX_MSGS || n >= msgNum) error("Wrong message number.");
    return n;
}

void viewMessage()
{
    Message *m = NULL;
    unsigned int n = getMsgNum();
    m = msgArray[n];
    printf("Msg[%02d]\nType: %s\n", n, type2str(m->msgType));
    if (m->msgType == NAME) printf("%s\n", m->name);
    else printf("%d\n", m->id);
}

void editMessage()
{
    unsigned int n = getMsgNum();
    Message *m = msgArray[n];
    puts("Message type? (0 for NAME and 1 for ID)");
    m->msgType = getIntInput() == 0 ? NAME : ID;
    switch ( m->msgType ) {
    case NAME:
        printf("Old name was: %s, %p\n", m->name, m->name);
        puts("New name?");
        m->name = getStringInput();
        break;
    case ID:
        printf("Old ID was: %d\n", m->id);
        puts("New ID?");
        m->id = getIntInput();
        break;
    }
}

void listMessage()
{
    unsigned int i = 0;
    if (msgNum > MAX_MSGS) error("Invalid number of messages.");
    else if (msgNum == 0) {puts("No messages in the storage."); return;}
    else {}

    for(i=0; i<msgNum; i++) {
        printf("[%02d] [%s]\n", i, type2str(msgArray[i]->msgType));
    }
}

void magic()
{
    FILE *fp = fopen("/home/msg/flag.txt", "r");
    size_t flagLen = 0;
    char* buf;

    fseek(fp, 0L, SEEK_END);
    flagLen = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    buf = (char*) malloc(flagLen);
    if (!buf) goto failure;
    printf("%zd\n", flagLen);
    if (fread(buf, flagLen, 1, fp) <= 0 && feof(fp) == 0) goto failure;
    fclose(fp);
    return;
failure:
    fclose(fp);
    error("Failed to run magic.");
}

void process()
{
    int menu = 0;
    puts("Menu: (1) Insert; (2) View; (3) Edit; (4) List");
    menu = getIntInput();
    switch (menu) {
    case 1:
        insertMessage();
        break;
    case 2:
        viewMessage();
        break;
    case 3:
        editMessage();
        break;
    case 4:
        listMessage();
        break;
    case 7777:
        magic();
        break;
    default:
        break;
    }
}

int main(void)
{
    setbuf(stdout, NULL);
    printf("\033[2J\033[1;1H"); // clear screen
    puts("\n");
    puts("== Message storage ==");
    puts("Insert/View/Edit Messages as you wish.");
    while (1) {
        process();
    }
    return 0;
}

