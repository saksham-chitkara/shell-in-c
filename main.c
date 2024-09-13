#include "headers.h"

void terminate(){
    for(int i = 0; i < 100001; i++){
        node* curr = obj->pNext[i];
        while(curr){
            int pid = curr->pid;
            kill(SIGKILL, pid);

            curr = curr->next;
        }
    } 

    for(int i = 0; i < 100001; i++){
        node* curr = obj->pNext[i];
        while(curr){
            node* p = curr;
            curr = curr->next;
            free(p);
        }
    }    

    free(obj->pNext);
    free(obj);
}

int main(){
    init();
    setup_sigchld_handler();
    kb_signal_handlers();
    write_dir_to_file(cwd, "cwd.txt");
    write_dir_to_file(prev_dir, "pwd.txt");
    
    while(1){
        write_dir_to_file(cwd, "cwd.txt");
        write_dir_to_file(prev_dir, "pwd.txt");

        prompt();
        char command[4096];
        // scanf("%[^\n]", command);
        // char c;
        // scanf("%c", &c);

        if(fgets(command, sizeof(command), stdin) == NULL){
            // printf("%d\n", fg_pid);
            // kill(SIGKILL, fg_pid);
            //
            
            if(feof(stdin)){
                terminate();
                printf("Logging out of shell\n");
                break;
            }

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