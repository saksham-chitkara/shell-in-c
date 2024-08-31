#include "headers.h"
#define size 10000001

hm* obj = NULL;
bool last_fg_more_than_2 = false;
int last_time;
char lastfg[256];

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

void hmInsert(hm* obj, int num, char* name){
    node* new = (node*)malloc(sizeof(node));
    new->pid = num;
    strcpy(new->pname, name);

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


void execute(char *cmd, int bg){
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

    char *token = strtok(cmd, " \t");
    int i = 0;
    while(token != NULL){
        strcpy(args[i++], token);
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;

    char pname[256];
    strcpy(pname, args[0]);
    // bool error = false;

    int pid = fork();

    if(pid < 0){
        perror("Fork failed!");
        return;
    }

    else if(pid == 0){   //jo execute krna child mein krna

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

    else if(pid > 0){  
        // if(error) return;

        if(bg){
            printf("%d\n", pid); 
            hmInsert(obj, pid, pname);
        } 

        else{
            gettimeofday(&start, NULL);
            int status;
            waitpid(pid, &status, 0);
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
        char pname[256];
        hmdelete(obj, pid, pname);

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

