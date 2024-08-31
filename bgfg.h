#ifndef _BGFG_H
#define _BGFG_H


typedef struct node{
    int pid;
    char pname[200]; //assuming name to be max 200
    struct node* next;
}node;

typedef struct{
    node** pNext;
}hm;

extern hm* obj;
extern bool last_fg_more_than_2 ;
extern int last_time;
extern char lastfg[256];

hm* hmCreate();
int hashindex(int key);
void hmInsert(hm* obj, int num, char* name);
void hmdelete(hm* obj, int num, char* name);

void execute(char *cmd, int bg);
void setup_sigchld_handler();
void sigchld_handler(int signum);

#endif