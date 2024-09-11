#ifndef _BGFG_H
#define _BGFG_H


typedef struct node{
    int pid;
    char pname[200]; //assuming name to be max 200
    char cmd_name[4096];
    struct node* next;
}node;

typedef struct{
    node** pNext;
}hm;

extern hm* obj;
extern bool last_fg_more_than_2 ;
extern int last_time;
extern char lastfg[256];
extern char fg_pname[256];
extern char fg_cmdname[256];
extern int fg_pid;
extern char* fg_name;


hm* hmCreate();
int hashindex(int key);
void hmInsert(hm* obj, int num, char* name, char* cmd_name);
void hmdelete(hm* obj, int num, char* name);
bool hmfind(int pid_given);

void execute(char *cmd, int bg);
void setup_sigchld_handler();
void sigchld_handler(int signum);

#endif