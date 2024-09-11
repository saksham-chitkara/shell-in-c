#include "headers.h"

int main(){
    init();
    setup_sigchld_handler();
    kb_signal_handlers();
    
    while(1){
        prompt();
        char command[4096];
        // scanf("%[^\n]", command);
        // char c;
        // scanf("%c", &c);

        if(fgets(command, sizeof(command), stdin) == NULL){
        //     // printf("%d\n", fg_pid);
        //     kill(SIGKILL, fg_pid);
        //     printf("Logging out of shell\n");
        //     break;
            continue;
        }

        int len = strlen(command);
        if (len > 0 && command[len-1] == '\n') {
            command[len-1] = '\0';
        }

        if(strlen(command) == 0){
            continue;
        }

        char cmd_copy[4096];
        strcpy(cmd_copy, command);
        char * p = cmd_copy;

        while (*p == ' ' || *p == '\t') p++; 
        if(strlen(p) == 0){
            continue;
        }

        tokenise(command);
    }
}