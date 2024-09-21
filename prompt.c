#include "headers.h"

char user_name[4096];
char cwd[4096];
char sys_name[4096];
char home[4096];
char* content[1024];

void init(){
    gethostname(sys_name, sizeof(sys_name));
    // getlogin_r(user_name, sizeof(user_name));

    int uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if(pw == NULL){
        perror("getpwuid");
        return;
    }
    
    strcpy(user_name, pw->pw_name);
    getcwd(home, sizeof(home));
    getcwd(cwd, sizeof(cwd));

    for(int i = 0; i < 1024; i++){
        content[i] = (char*)malloc(sizeof(char) * 1024);
    }

    obj = hmCreate();
    load_myshrc();
}

void print_red(const char* text) {
    printf("\033[31m%s\033[0m\n", text);
}

void prompt(){
    if(strcmp(home, cwd) == 0){
        printf("\033[36m<%s@%s:~", user_name, sys_name);
    }

    else if(strncmp(home, cwd, strlen(home)) == 0){
        printf("\033[36m<%s@%s:~%s", user_name, sys_name, cwd + strlen(home));
    }

    else{
        printf("\033[36m<%s@%s:%s", user_name, sys_name, cwd);
    }

    if(last_fg_more_than_2){
        printf(" %s : %ds> \033[0m", lastfg, last_time);
    }
    else{
        printf("> \033[0m");
    }

}