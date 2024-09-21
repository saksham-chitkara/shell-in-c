#ifndef _ALIAS_H
#define _ALIAS_H

typedef struct {
    char *name;
    char *command;
} alias;

typedef struct{
    char *name;
    char **commands;
    int command_count;
} func;

extern int alias_count;
extern int func_count;
void load_myshrc();
char* sub_alias(char *input);
int myshrc_func(char* cmd);

#endif