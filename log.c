#include "headers.h"

// to track over all sessions ek file mein store krna pdega

char history[15][4096];

int cmd_cnt(){
    char* file_path = (char*)malloc((sizeof(char) * 1000));  
    if(file_path == NULL){
        printf("\033[31mMalloc failed!\033[0m\n");
        return -1;
    } 

    sprintf(file_path, "%s/%s", home, "log.txt");
    // printf("Log ki loc: %s\n", file_path);

    int fd = open(file_path, O_RDONLY| O_CREAT, 0644);
    if(fd == -1){
        printf("\033[31mError in opening log.txt!\033[0m\n");
        return 0;
    }

    int tot_lines = 0;

    FILE* file = fdopen(fd, "r");
    if(file == NULL){
        printf("\033[31mError in opening log.txt!\033[0m\n");
        fclose(file);
        return 0;
    }

    char buffer[4096];  

    while(fgets(buffer, sizeof(buffer), file) != NULL){
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        strcpy(history[tot_lines], buffer);
        // printf("%s ",history[tot_lines]);
        tot_lines++;
    }

    // printf("cmd cnt over\n\n");
    free(file_path);
    fclose(file);
    return tot_lines;
}

void add(char* cmd){
    // printf("%s\n", cmd);
    int cmd_till_now = cmd_cnt();

    if(strstr(cmd, "log") != NULL) return; //for log commands skip

    if(cmd_till_now > 0 && strcmp(cmd, history[cmd_till_now - 1]) == 0 && strlen(cmd) == strlen(history[cmd_till_now - 1])) return; //skip if same as prev
    
    if(cmd_till_now == 15){
        for(int i = 0; i < 14; i++){
            strcpy(history[i], history[i + 1]);
        }

        strcpy(history[14], cmd);
        // strcat(history[15], "\n");
        write_all(cmd_till_now);
    }
    
    else{
        strcpy(history[cmd_till_now], cmd);
        cmd_till_now++;
        
        write_all(cmd_till_now);
    }
    
}

void purge(){
    char* file_path = (char*)malloc((sizeof(char) * 1000)); 
    if(file_path == NULL){
        printf("\033[31mMalloc failed!\033[0m\n");
        return;
    }   

    sprintf(file_path, "%s/%s", home, "log.txt");

    int fd = open(file_path, O_WRONLY | O_TRUNC);
    if(fd == -1){
        printf("\033[31mError in opening log.txt!\033[0m\n");
        return;
    }

    free(file_path);
    close(fd);
}

void exec(int index){
    int cmd_till_now = cmd_cnt();
    
    if(index > cmd_till_now || index <= 0){
        printf("\033[31mEnter a valid index!\033[0m\n");
        return;
    }

    else{
        char curr_cmd[4096];
        strcpy(curr_cmd, history[cmd_till_now - index]);
        // printf("%s\n", curr_cmd);
        tokenise(curr_cmd);           
    }
}

void get_all(){
    int tot_lines = cmd_cnt();  
    if(tot_lines == 0){
        printf("\033[31mNo history available!\033[0m\n");
        return;
    }

    char* file_path = (char*)malloc((sizeof(char) * 1000));   
    if(file_path == NULL){
        printf("\033[31mMalloc failed!\033[0m\n");
        return;
    }   
    sprintf(file_path, "%s/%s", home, "log.txt");

    FILE* file = fopen(file_path, "r");
    if(file == NULL){
        printf("\033[31mError in opening log.txt!\033[0m\n");
        return;
    }

    char buffer[4096];  

    for(int i = 0; i < tot_lines; i++){
        if(fgets(buffer, sizeof(buffer), file) != NULL){
            int len = strlen(buffer);
            if(len > 0 && buffer[len - 1] == '\n'){
                buffer[len - 1] = '\0';
            }
            printf("%s\n", buffer); 
        }
    }

    free(file_path);
    fclose(file);
}

void write_all(int total_cmds){
    
    char* file_path = (char*)malloc((sizeof(char) * 1000));
    if(file_path == NULL){
        printf("\033[31mMalloc failed!\033[0m\n");
        return;
    }  

    sprintf(file_path, "%s/%s", home, "log.txt");

    FILE* file = fopen(file_path, "w");
    if(file == NULL){
        printf("\033[31mError in opening log.txt!\033[0m\n");
        return;
    }

    for(int i = 0; i < total_cmds; i++){
        fprintf(file, "%s\n", history[i]); 
    }

    // printf("written to file %d\n", total_cmds);
    free(file_path);
    fclose(file);
}

