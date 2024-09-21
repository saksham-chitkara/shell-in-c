#ifndef _PROMPT_H
#define _PROMPT_H

// char user[4096], cwd[4096], home_dir[4096], prev_dir[4096], system_name[4096];

extern char user_name[4096];
extern char cwd[4096];
extern char sys_name[4096];
extern char home[4096];
extern char* content[1024];

void init();
void prompt();

#endif
