#include "headers.h"

alias aliases[10]; //assumed max 10 aliases and functions
func funct[10];

int alias_count = 0;
int func_count = 0;

void load_myshrc(){
    FILE *file = fopen(".myshrc", "r");
    if(!file){
        printf("\033[31mNo .myshrc file found!\033[0m\n");
        return;
    }

    char line[1024];
    while(fgets(line, sizeof(line), file)){

        char *comment_pos = strchr(line, '#');
        if(comment_pos){
            *comment_pos = '\0';  
        }

        if(strlen(line) == 0 || line[0] == '\n'){
            continue;
        }

        //alias ka
        if(strncmp(line, "alias ", 6) == 0){
            char *name = strtok(line + 6, "=");
            while(*name == ' ') name++;
            char* end = name + strlen(name) - 1;
            while (end > name && *end == ' ') end--; 
            *(end + 1) = '\0';

            char *command = strtok(NULL, "\n");
            while(*command == ' ') command++;
            end = command + strlen(command) - 1;
            while(end > command && *end == ' ' ) end--;
            *(end + 1) = '\0';

            if(name && command){
                aliases[alias_count].name = strdup(name);
                aliases[alias_count].command = strdup(command);
                alias_count++;
            }
        }
        
        // alias defined without keyword
        else if(strstr(line, "=")){
            char *name = strtok(line, "=");
            while(*name == ' ') name++;
            char* end = name + strlen(name) - 1;
            while (end > name && *end == ' ') end--; 
            *(end + 1) = '\0';

            char *command = strtok(NULL, "\n");
            while(*command == ' ') command++;
            end = command + strlen(command) - 1;
            while(end > command && *end == ' ' ) end--;
            *(end + 1) = '\0';

            if(name && command){
                aliases[alias_count].name = strdup(name);
                aliases[alias_count].command = strdup(command);
                alias_count++;
            }
        }

        // func
        else if(strchr(line, '{')){
            char *name = strtok(line, " \t");
            if(name){
                funct[func_count].name = strdup(name);
                funct[func_count].command_count = 0;

                funct[func_count].commands = malloc(sizeof(char*) * 20);
                for(int i = 0; i < 20; i++){
                    funct[func_count].commands[i] = malloc(sizeof(char) * 256); 
                }

                while(fgets(line, sizeof(line), file) && !strchr(line, '}')){
                    comment_pos = strchr(line, '#');
                    if(comment_pos){
                        *comment_pos = '\0';
                    }

                    char* l = line;
                    while(*l == ' ') l++; 

                    char *end = l + strlen(l) - 1;
                    while(end > l && (*end == ' ' || *end == '\t' || *end == '\n')) end--;
                    *(end + 1) = '\0';

                    if(*l != '\0'){
                        funct[func_count].commands[funct[func_count].command_count++] = strdup(l);
                    }
                }

                func_count++;
            }
        }
    }

    // for(int i = 0; i < func_count; i++){
    //     printf("%s\n", funct[i].name);
    //     for(int j = 0; j < funct[i].command_count; j++){
    //         printf("%s", funct[i].commands[j]);
    //     }
    //     printf("\n");
    // }

    fclose(file);
}


char* sub_alias(char *input){
    for(int i = 0; i < alias_count; i++){
        if(strcmp(input, aliases[i].name) == 0){
            return aliases[i].command;
        }
    }
    return input; 
}

int myshrc_func(char* cmd){
    char* args[20];
    char* token = strtok(cmd, " \t");
    int match = 0;

    int arg_cnt = 0;
    while(token != NULL){
        args[arg_cnt++] = token;
        token = strtok(NULL, " \t");
    }

    for(int i = 0; i < func_count; i++){
        if(strcmp(args[0], funct[i].name) == 0){
            match = 1;

            for(int j = 0; j < funct[i].command_count; j++){
                //suppose the line is mkdir -p "$i" so I want to replace $i with args[i] if i <= arg_cnt and get final string otherwise error

                char final_command[1024];
                memset(final_command, 0, sizeof(final_command));

                char *line = funct[i].commands[j];
                int k = 0;
                while(line[k]){

                    if(line[k] == '"' && line[k+1] == '$' && line[k+2] >= '0' && line[k+2] <= '9' && line[k+3] == '"'){

                        int arg_num = line[k+2] - '0'; 
                        if(arg_num < arg_cnt){
                            strcat(final_command, args[arg_num]); 
                        } 

                        else{
                            printf("\033[31mNot enough arguements for variable $%d!\033[0m\n", arg_num);
                            return 1;
                        }

                        k += 4;  
                    } 
                    
                    else{
                        strncat(final_command, &line[k], 1);
                        k++;
                    }
                }

                // printf("%s\n", final_command);
                tokenise(final_command);
            }
        }
    }

    return match;
}