#include "headers.h"
#define size 100001

pinfo processes[4096];

int compare_process_info(const void *a, const void *b) {
    return (((pinfo *)a)->pid - ((pinfo *)b)->pid);
}

bool status(int pid){
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    
    FILE *status_file = fopen(path, "r");
    if(status_file == NULL){
        return false;  
    }

    char line[256];
    bool stopped = false;
    while(fgets(line, sizeof(line), status_file) != NULL){
        if(strncmp(line, "State:", 6) == 0){

            char state = line[7];
            if(state == 'T'){
                stopped = true;
            }
            break;
        }
    }

    fclose(status_file);
    return stopped;
}

void activities(){
    int count = 0;

    for(int i = 0; i < size; i++){
        node* curr = obj->pNext[i];

        while(curr != NULL){
            processes[count].pid = curr->pid;
            strcpy(processes[count].cmd_name, curr->cmd_name);

            if(status(curr->pid)){
                strcpy(processes[count].state, "Stopped");
            } 

            else{
                strcpy(processes[count].state, "Running");
            }

            count++;
            curr = curr->next;
        }
    }

    qsort(processes, count, sizeof(pinfo), compare_process_info);

    if(count == 0){
        printf("\033[31mNo currently running processes!\033[0m\n");
        return;
    }
    
    for(int i = 0; i < count; i++){
        printf("[%d] : %s - %s\n", processes[i].pid, processes[i].cmd_name, processes[i].state);
    }
}
