#include "headers.h"
#define size 100001

hm* obj = NULL;
int fg_pid = -1;
char* fg_name = NULL;
bool last_fg_more_than_2 = false;
int last_time;
char lastfg[256];
char fg_pname[256];
char fg_cmdname[256];

hm* hmCreate(){
    hm* new = (hm*)malloc(sizeof (hm));
    new->pNext = (node**)malloc(size * sizeof(node*));

    for(int i = 0; i < size; i++){
        new->pNext[i] = NULL;
    }

    return new;
}

int hashindex(int key){
    return key % size;
}

void hmInsert(hm* obj, int num, char* name, char* cname){
    node* new = (node*)malloc(sizeof(node));
    new->pid = num;
    strcpy(new->pname, name);
    strcpy(new->cmd_name, cname);

    int hashindx = hashindex(num);

    new->next = obj->pNext[hashindx];
    obj->pNext[hashindx] = new;
}

void hmdelete(hm* obj, int num, char* name){
    int hashindx = hashindex(num);
    node* prev = NULL;
    node* curr = obj->pNext[hashindx];

    while(curr != NULL && curr->pid != num){
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL) return;

    strcpy(name, curr->pname);

    if(prev == NULL){
        obj->pNext[hashindx] = obj->pNext[hashindx]->next;
    }

    else{
        prev->next = curr->next;
    }

    free(curr);
}

bool hmfind(int pid_given){
    int indx = hashindex(pid_given);
    node* curr = obj->pNext[indx];

    while(curr != NULL){
        if(curr->pid == pid_given) return true;
        curr = curr -> next;
    }
    
    return false;
}


void execute(char *cmd, int bground){
    //cmd mein & nhi hai

    struct timeval start, end;

    char *args[1024];
    for(int i = 0; i < 1024; i++){
        args[i] = (char *)malloc(256 * sizeof(char)); 

        if(args[i] == NULL){
            perror("malloc failed");
            exit(1);
        }
    }

    char orig[4096];
    strcpy(orig, cmd);
    
    char *token = strtok(cmd, " \t");
    int i = 0;
    while(token != NULL){
        strcpy(args[i++], token);
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;

    //
    //
    //
    // changed for "" 
    //
    //

    // while(*cmd == ' ' || *cmd == '\t') cmd++;
    // char *ending = cmd + strlen(cmd) - 1;
    // while(ending > cmd && *ending == ' '){
    //     ending--;
    // }
    // *(ending + 1) = '\0';

    
    
    // int i = 0;
    // int len = strlen(cmd);
    // bool consider = true;
    // int token_cnt = 0;
    // int j = 0;

    // //sed mein error ara kyuki quotes mein space ko delimitor manra
    // //isliye ab yeh krna pdega

    // while(i < len){
    //     if(cmd[i] == '\'' || cmd[i] == '"') consider = !consider;

    //     if((cmd[i + 1] == '\0' || cmd[i] == ' ') && consider){
    //         if(cmd[i + 1] == '\0'){
    //             i++;
    //             cmd[i] = '\0';
    //         }

    //         else cmd[i] = '\0';
        
    //         char* next = cmd + j;

    //         //remove quotes from ends
    //         if(next[0] == '\'' && next[strlen(next) - 1] == '\'' || next[0] == '"' && next[strlen(next) - 1] == '"'){
    //             next[strlen(next) - 1] = '\0';
    //             next++;
    //         }

    //         // printf("%s..\n", next);
    //         if(strlen(next))    
    //             args[token_cnt++] = next;

    //         j = i + 1;
    //     }
    //     i++;
    // }
   
    // args[token_cnt] = NULL;

    char pname[256];
    strcpy(pname, args[0]);
    // bool error = false;

    bool pipe_or_redirect = false;
    if(strstr(orig, "|") || strstr(orig, ">") || strstr(orig, ">>") || strstr(orig, "<")) pipe_or_redirect = true;

    int pid = fork();

    if(pid < 0){
        perror("Fork failed!");
        return;
    }

    else if(pid == 0){   //jo execute krna child mein krna
        
        setup_sigchld_handler();

        if(strstr(orig, "|")){
            handle_pipes(orig, bground);
        }

        // else if(strncmp("bg", orig, 2) == 0  && (orig[2] == '\0' || orig[2] == ' ' || orig[2] == '\t')){
        //     bg(orig);
        // }

        // else if(strncmp("fg", orig, 2) == 0  && (orig[2] == '\0' || orig[2] == ' ' || orig[2] == '\t')){
        //     fg(orig);
        // }

        else if(strstr(orig, ">") || strstr(orig, ">>") || strstr(orig, "<")){
            redirect(orig, false, -1, -1, bground);
        }

        // else if(myshrc_func(strdup(orig))){

        // }

        // else if(strncmp("neonate", orig, 7) == 0  && (orig[7] == '\0' || orig[7] == ' ' || orig[7] == '\t')){
        //     neonate(orig);
        // }

        // else if(strncmp("iMan", orig, 4) == 0  && (orig[4] == '\0' || orig[4] == ' ' || orig[4] == '\t')){
        //     man(orig);
        // }

        // else if(strncmp("hop", orig, 3) == 0  && (orig[3] == '\0' || orig[3] == ' ' || orig[3] == '\t')){
        //     hop(orig);

        // } 
        
        // else if(strncmp("seek", orig, 4) == 0 && (orig[4] == '\0' || orig[4] == ' ' || orig[4] == '\t')){
        //     find(orig);
        // } 
        
        // else if(strncmp("proclore", orig, 8) == 0 && (orig[8] == '\0' || orig[8] == ' ' || orig[8] == '\t')){
        //     proclore(orig);
        // } 
        
        // else if(strcmp("log purge", orig) == 0){
        //     purge();
        // } 
        
        // else if(strncmp("log execute", orig, 11) == 0 && (orig[11] == '\0' || orig[11] == ' ' || orig[11] == '\t')){
        //     int index = -1;

        //     char* sub_token;
        //     char* saveptr3;
        //     sub_token = strtok_r(orig, " \t", &saveptr3);

        //     int args = 0;
        //     while(sub_token != NULL){
        //         args++;
        //         if(args == 3){
        //             index = atoi(sub_token);
        //         }

        //         sub_token = strtok_r(NULL, " \t", &saveptr3);
        //     }
        //     exec(index);
        // } 
        
        // else if(strcmp("log", orig) == 0){
        //     get_all();
        // } 
        
        // else if(strncmp("reveal", orig, 6) == 0 && (orig[6] == '\0' || orig[6] == ' ' || orig[6] == '\t')){
        //     reveal(orig);
        // } 

        // else if(strncmp("ping", orig, 4) == 0 && (orig[4] == '\0' || orig[4] == ' ' || orig[4] == '\t')){
        //     ping(orig);
        // } 

        // else if(strcmp("activities", orig) == 0){
        //     activities();
        // }

        //
        //
        //
        //
        // iske upr ka add kra h
        //
        //
        //
        else{
            // printf("%s..\n", args[0]);
            int res = execvp(args[0], args);
            for(int i = 0; i < 1024; i++){
                free(args[i]);
            }
            
            if(res < 0){
                printf("\033[31mERROR: '%s' is not a valid command!\033[0m\n", pname);
                // error = true;
                exit(1);
            }
        }

        exit(0);
    } 

    else if(pid > 0){  
        // if(error) return;

        if(bground && !pipe_or_redirect){
            setpgid(pid, pid);
            //
            //

            printf("%d\n", pid); 
            hmInsert(obj, pid, pname, orig);
        } 

        else{
            fg_pid = pid;
            fg_name = args[0];
            // printf("fgpid %d\n", fg_pid);
            strcpy(fg_cmdname, orig);
            strcpy(fg_pname, pname);

            gettimeofday(&start, NULL);
            int status;
            waitpid(pid, &status, 0);
            // fg_name = NULL;
            // printf("%d exited\n", fg_pid);
            
            gettimeofday(&end, NULL);

            // time taken
            long long int seconds = end.tv_sec - start.tv_sec;
            long long int microseconds = end.tv_usec - start.tv_usec;
            double elapsed = seconds + microseconds * 1e-6;
            int time = round(elapsed);

            if(elapsed > 2.0){
                last_fg_more_than_2 = true;
                strcpy(lastfg, args[0]);
                last_time = time;
            }

            else{
                last_fg_more_than_2 = false;
            }

            // fg_pid = -1;
        }

    } 
}

// void handle_bg_process(){
//     int status;
//     int pid;

//     while((pid = waitpid(-1, &status, WNOHANG)) > 0){
//         if(WIFEXITED(status)){
//             printf("%d exited normally\n", pid);
//         } 
//         else if (WIFSIGNALED(status)){
//             printf("%d exited abnormally with signal %d\n", pid, WTERMSIG(status));
//         }
//     }
// }

void sigchld_handler(int signum){
    int status;
    int pid;
    
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        char *pname = (char*)malloc(sizeof(256));
        pname[0] = '\0';
        hmdelete(obj, pid, pname);

        if(strlen(pname) == 0){
            strcpy(pname, fg_name);
        }

        if(WIFEXITED(status)){
            printf("%s exited normally (%d)\n", pname, pid);
        } 

        else if(WIFSIGNALED(status)){
            printf("%s exited abnormally (%d)\n", pname, pid);
        }
    }
}

void setup_sigchld_handler(){
    struct sigaction sa;

    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGCHLD, &sa, NULL) == -1){
        perror("sigaction");
        exit(1);
    }
}

