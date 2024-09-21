#ifndef _LOG_H
#define _LOG_H

extern char history[15][4096];

void get_all();
void write_all(int total_cmds);
void exec(int indx);
void purge();
void add(char* cmd);
int cmd_cnt();

#endif