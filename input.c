#include "headers.h"

void tokenise(char* input){
    // printf("%s\n", input);
    add(input); //to add to log file

    char *token = NULL;
    char *saveptr1 = NULL;

    token = strtok_r(input, ";", &saveptr1);
    while(token != NULL){

        int and_cnt = 0;

        for(char *p = token; *p != '\0'; p++){
            if(*p == '&'){
                and_cnt++;
            }
        }

        int part_number = 1;

        char dup_token[4096];
        strncpy(dup_token, token, sizeof(dup_token) - 1);
        dup_token[sizeof(dup_token) - 1] = '\0';

        char *and_token = NULL;
        char *saveptr2 = NULL;

        and_token = strtok_r(dup_token, "&", &saveptr2);

        while(and_token != NULL){
            // printf("token : %s...%s", strlen(and_token), and_token);

            while(*and_token == ' ' || *and_token == '\t') and_token++; 

            char *end = and_token + strlen(and_token) - 1;
            while(end > and_token && (*end == ' ' || *end == '\t')) end--;
            *(end + 1) = '\0';

            and_token = sub_alias(and_token);
            
            // printf("%s\n", and_token);
            if(strlen(and_token) == 0){
                part_number++;
                and_token = strtok_r(NULL, "&", &saveptr2);
                continue;
            }

            // last hai to fg
            if(part_number == and_cnt + 1){

                if(strstr(and_token, "|")){
                    // printf("here\n");
                    handle_pipes(and_token, 0);
                }

                else if(strncmp("bg", and_token, 2) == 0  && (and_token[2] == '\0' || and_token[2] == ' ' || and_token[2] == '\t')){
                    bg(and_token);
                }

                else if(strncmp("fg", and_token, 2) == 0  && (and_token[2] == '\0' || and_token[2] == ' ' || and_token[2] == '\t')){
                    fg(and_token);
                }

                else if(strstr(and_token, ">") || strstr(and_token, ">>") || strstr(and_token, "<")){
                    redirect(and_token, false, -1, -1, 0);
                }

                else if(myshrc_func(strdup(and_token))){

                }

                else if(strncmp("neonate", and_token, 7) == 0  && (and_token[7] == '\0' || and_token[7] == ' ' || and_token[7] == '\t')){
                    neonate(and_token);
                }

                else if(strncmp("iMan", and_token, 4) == 0  && (and_token[4] == '\0' || and_token[4] == ' ' || and_token[4] == '\t')){
                    man(and_token);
                }

                else if(strncmp("hop", and_token, 3) == 0  && (and_token[3] == '\0' || and_token[3] == ' ' || and_token[3] == '\t')){
                    hop(and_token);
                } 
                
                else if(strncmp("seek", and_token, 4) == 0 && (and_token[4] == '\0' || and_token[4] == ' ' || and_token[4] == '\t')){
                    find(and_token);
                } 
                
                else if(strncmp("proclore", and_token, 8) == 0 && (and_token[8] == '\0' || and_token[8] == ' ' || and_token[8] == '\t')){
                    proclore(and_token);
                } 
                
                else if(strcmp("log purge", and_token) == 0){
                    purge();
                } 
                
                else if(strncmp("log execute", and_token, 11) == 0 && (and_token[11] == '\0' || and_token[11] == ' ' || and_token[11] == '\t')){
                    int index = -1;

                    char* sub_token;
                    char* saveptr3;
                    sub_token = strtok_r(and_token, " \t", &saveptr3);

                    int args = 0;
                    while(sub_token != NULL){
                        args++;
                        if(args == 3){
                            index = atoi(sub_token);
                        }

                        sub_token = strtok_r(NULL, " \t", &saveptr3);
                    }
                    exec(index);
                } 
                
                else if(strcmp("log", and_token) == 0){
                    get_all();
                } 
                
                else if(strncmp("reveal", and_token, 6) == 0 && (and_token[6] == '\0' || and_token[6] == ' ' || and_token[6] == '\t')){
                    reveal(and_token);
                } 

                else if(strncmp("ping", and_token, 4) == 0 && (and_token[4] == '\0' || and_token[4] == ' ' || and_token[4] == '\t')){
                    ping(and_token);
                } 

                else if(strcmp("activities", and_token) == 0){
                    activities();
                }
                
                else{ 
                    execute(and_token, 0);
                }
            } 

            //bg
            else{
                execute(and_token, 1);
            }

            part_number++;
            and_token = strtok_r(NULL, "&", &saveptr2);
        }

        token = strtok_r(NULL, ";", &saveptr1);
    }
}
