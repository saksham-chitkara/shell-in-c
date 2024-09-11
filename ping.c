#include "headers.h"

void ping(char* cmd){
    int args[10];
    int arg_cnt = 0;

    char *token = strtok(cmd, " \t");
    token = strtok(NULL, " \t"); //skip ping
    int i = 0;

    while(token != NULL && i < 2){
        args[i++] = atoi(token);
        token = strtok(NULL, " \t");
    }

    if(i != 2){
        printf("Give valid arguements!\n");
        return;
    }

    int pid = args[0];
    int sig_num = args[1];

    sig_num = sig_num % 32;

    if(kill(pid, sig_num) == -1){
        if(errno == ESRCH){
            printf("No such process found\n");
        } 

        else{
            perror("Error sending signal");
        }
    } 

    else{
        printf("Sent signal %d to process with pid %d\n", sig_num, pid);
    }
}

void ctrlC_handler(int signum){
    if(fg_pid != -1 && fg_name){
        kill(fg_pid, SIGINT);
    }
    printf("\n");
}

void ctrlZ_handler(int signum){
    // printf("...");

    // printf("%d\n", fg_pid);
    if(fg_pid != -1 && fg_name){
        // printf("ctrlZ_ pressed\n");
        hmInsert(obj, fg_pid, fg_pname, fg_cmdname);
        kill(fg_pid, SIGTSTP);
        fg_pid = -1;  // No fg process after stopping
        fg_name = NULL;
    }
    printf("\n");
}

void kb_signal_handlers(){
    struct sigaction sa;

    sa.sa_handler = ctrlC_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = ctrlZ_handler;
    sigaction(SIGTSTP, &sa, NULL);
}