#include "headers.h"

void proclore(char* cmd){

    int arg_cnt = 0;
    int pid_given;

    char *token = strtok(cmd, " \t");
    token = strtok(NULL, " \t"); //proclore ko ignore kr

    while(token != NULL){
        arg_cnt++;
        pid_given = atoi(token);
        token = strtok(NULL, " \t");
    }

    if(arg_cnt == 0){
        pid_given = getpid(); //shell ki id lele
    }

    // printf("%d\n", pid_given);

    char status_file_path[1024];
    snprintf(status_file_path, sizeof(status_file_path), "/proc/%d/stat", pid_given);

    FILE *status_file = fopen(status_file_path, "r");
    if(!status_file){
        printf("\033[31mPlease enter the correct pid\033[0m\n");
        return;
    }

    char curr_line[1024];
    int pid;
    int line_num = 1;
    char state;
    long unsigned int vsize;
    int pgrp; //process grp ki id
    int tgpid;
    // The ID of the foreground process group of the controlling terminal of the process
    // man proc se dkha
    // agr ye dono same to foreground process


    //ek hi line h naki 52 lines
    fscanf(status_file, "%d %*s %c %*d %d %*d %*d %d %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %lu", &pid, &state, &pgrp, &tgpid, &vsize);

    char exec_path[1024];
    snprintf(status_file_path, sizeof(status_file_path), "/proc/%d/exe", pid);  //reuse

    int len = readlink(status_file_path, exec_path, sizeof(exec_path) - 1);
    if(len >= 0){
        exec_path[len] = '\0';
    } 

    else{
        printf("\033[31mCouldn't find the executable file\033[0m\n");
        // return;
    }

    printf("pid : %d\n", pid);
    if(pgrp == tgpid && !hmfind(pid)){
        // printf("process status : %s+\n", state); //for foreground
        if(state == 'Z') printf("process status : %c\n", state);
        else printf("process status : %c+\n", state); //for foreground
    }
    else{
        // printf("process status : %s\n", state);
        printf("process status : %c\n", state);
    }

    printf("Process Group : %d\n", pgrp);
    printf("Virtual memory : %lu\n", vsize);

    if(strncmp(home, exec_path, strlen(home)) == 0){
        printf("executbale path : ~%s\n\n", exec_path + strlen(home));
    }
    else{
        printf("executable path : %s\n\n", exec_path);
    }
    
    return;
}