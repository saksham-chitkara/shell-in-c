#include "headers.h"

void bg(char* cmd){
    char* args[20];
    int arg_cnt = 0;

    char* token = strtok(cmd, " \t");
    while(token != NULL){
        args[arg_cnt++] = token;
        token = strtok(NULL, " \t");
    }

    if(arg_cnt != 2){
        printf("Provide valid arguements\n");
        return;
    }

    int pid = atoi(args[1]);

    if(pid < 0){
        printf("Enter a valid pid!\n");
        return;
    }

    // if(!hmfind(pid)){
    //     printf("No such process found\n");
    //     return;
    // }

    // general krna tha isliye

    struct stat st;
    char proc_path[256];

    snprintf(proc_path, sizeof(proc_path), "/proc/%d", pid);

    if(stat(proc_path, &st) == -1){
        printf("No such process found\n");
        return;
    }

    if(kill(pid, SIGCONT) < 0){
        perror("bg");
    }

    return;
}

void fg(char* cmd){
    char* args[20];
    int arg_cnt = 0;

    char* token = strtok(cmd, " \t");
    while(token != NULL){
        args[arg_cnt++] = token;
        token = strtok(NULL, " \t");
    }

    if(arg_cnt != 2){
        printf("Provide valid arguements\n");
        return;
    }

    int pid = atoi(args[1]);

    if(pid < 0){
        printf("Enter a valid pid!\n");
        return;
    }

    // if(!hmfind(pid)){
    //     printf("No such process found\n");
    //     return;
    // }

    struct stat st;
    char proc_path[256];

    snprintf(proc_path, sizeof(proc_path), "/proc/%d", pid);

    if(stat(proc_path, &st) == -1){
        printf("No such process found\n");
        return;
    }

    signal(SIGTTIN, SIG_IGN); // ye do signal ignore krenge SIG_IGN se
    signal(SIGTTOU, SIG_IGN);

    //ye dono signal tb aate jb bg process terminal se interact krti,,,jab usse fg mein laynge
    //ye dono signal interrupt na kre isliye ignore krdia

    // in man page of tcsetpgrp
    // The function tcsetpgrp() makes the process group with process group ID pgrp the foreground process group on the terminal associated
    //  to fd, which must be the controlling terminal of the calling process, and still be associated with its session. 
    // Moreover, pgrp must be a (nonempty) process group belonging to the same session as the calling process.

    // If tcsetpgrp() is called by a member of a background process group in its session, and the calling process is not blocking 
    // or ignoring SIGTTOU, a SIGTTOU signal is sent to all members of this background process group.

    // If you're writing a shell or managing multiple processes, you might use tcsetpgrp to switch control between different processes.
    // For example, when a new foreground process starts, it needs terminal control, so you would call tcsetpgrp to give it that control.

    int pgid = getpgid(pid);
    tcsetpgrp(0, pgid);

    if(kill(pid, SIGCONT) < 0){ //stop h to run kradenge
        printf("Error in kill command!\n");
        return;
    }

    int status;
    // printf("waiting\n");
    int res = waitpid(pid, &status, WUNTRACED); //stop hua to bhi return krdega WUNTRACED isliye lgaya
    if(res == -1){
        perror("waitpid");
        return;
    }

    // printf("done\n");

    // printf("Process %d exited with status %d\n", pid, status);
    // if(WIFEXITED(status)){
    //     printf("Process %d exited normally with exit status %d\n", pid, WEXITSTATUS(status));
    // } 
    // else if(WIFSIGNALED(status)){
    //     printf("Process %d was killed by signal %d\n", pid, WTERMSIG(status));
    // } 
    // else if(WIFSTOPPED(status)){
    //     printf("Process %d was stopped by signal %d\n", pid, WSTOPSIG(status));
    // }

    tcsetpgrp(0, getpgid(0)); // to return terminal control back to shell
    signal(SIGTTIN, SIG_DFL); //restore krdiye dono signal
    signal(SIGTTOU, SIG_DFL);
   
    if(WIFSTOPPED(status)){ //agar dobara stop hogyi to fg mein ni jaygi bg hi rah jaygi
        return;
    }

    else{
        char* faltu = (char*)malloc(sizeof(char) * 256);
        hmdelete(obj, pid, faltu); //agar shell ne bnaio hogi to delete ho jaygi ni to kuch ni hoga
    }

    return;
}