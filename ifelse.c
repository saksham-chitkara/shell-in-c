#include "headers.h"

int distribute(char* cmd){
    while(*cmd == ' ' || *cmd == '\t') cmd++; 
    char *end = cmd + strlen(cmd) - 1;
    while(end > cmd && (*end == ' ' || *end == '\t')) end--;
    *(end + 1) = '\0';

    cmd = sub_alias(cmd);
            
    if(strlen(cmd) == 0){
        return 0;
    }

    if(strstr(cmd, "|")){
        handle_pipes(cmd, 0);
        return 1;
    }

    else if(strncmp("bg", cmd, 2) == 0  && (cmd[2] == '\0' || cmd[2] == ' ' || cmd[2] == '\t')){
        bg(cmd);
        return 1;
    }

    else if(strncmp("fg", cmd, 2) == 0  && (cmd[2] == '\0' || cmd[2] == ' ' || cmd[2] == '\t')){
        fg(cmd);
        return 1;
    }

    else if(strstr(cmd, ">") || strstr(cmd, ">>") || strstr(cmd, "<")){
        redirect(cmd, false, -1, -1, 0);
        return 1;
    }

    else if(myshrc_func(strdup(cmd))){
        return 1;
    }

    else if(strncmp("neonate", cmd, 7) == 0  && (cmd[7] == '\0' || cmd[7] == ' ' || cmd[7] == '\t')){
        neonate(cmd);
        return 1;
    }

    else if(strncmp("iMan", cmd, 4) == 0  && (cmd[4] == '\0' || cmd[4] == ' ' || cmd[4] == '\t')){
        man(cmd);
        return 1;
    }

    else if(strncmp("hop", cmd, 3) == 0  && (cmd[3] == '\0' || cmd[3] == ' ' || cmd[3] == '\t')){
        hop(cmd);
        return 1;
    } 
    
    else if(strncmp("seek", cmd, 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' ' || cmd[4] == '\t')){
        find(cmd);
        return 1;
    } 
    
    else if(strncmp("proclore", cmd, 8) == 0 && (cmd[8] == '\0' || cmd[8] == ' ' || cmd[8] == '\t')){
        proclore(cmd);
        return 1;
    } 
    
    else if(strcmp("log purge", cmd) == 0){
        purge();
        return 1;
    } 
    
    else if(strncmp("log execute", cmd, 11) == 0 && (cmd[11] == '\0' || cmd[11] == ' ' || cmd[11] == '\t')){
        int index = -1;

        char* sub_token;
        char* saveptr3;
        sub_token = strtok_r(cmd, " \t", &saveptr3);

        int args = 0;
        while(sub_token != NULL){
            args++;
            if(args == 3){
                index = atoi(sub_token);
            }

            sub_token = strtok_r(NULL, " \t", &saveptr3);
        }
        exec(index);
        return 1;
    } 
    
    else if(strcmp("log", cmd) == 0){
        get_all();
        return 1;
    } 
    
    else if(strncmp("reveal", cmd, 6) == 0 && (cmd[6] == '\0' || cmd[6] == ' ' || cmd[6] == '\t')){
        reveal(cmd);
        return 1;
    } 

    else if(strncmp("ping", cmd, 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' ' || cmd[4] == '\t')){
        ping(cmd);
        return 1;
    } 

    else if(strcmp("activities", cmd) == 0){
        activities();
        return 1;
    }
        
    return 0;
}