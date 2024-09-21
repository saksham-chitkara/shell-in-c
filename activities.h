#ifndef _ACT_H
#define _ACT_H

typedef struct{
    int pid;
    char cmd_name[100];
    char state[10];
} pinfo;

void activities();

#endif