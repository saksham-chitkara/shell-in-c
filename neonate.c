#include "headers.h"

int get_latest_pid(){
    DIR *proc_dir = opendir("/proc");
    if(proc_dir == NULL){
        printf("\033[31mFailed to open /proc!\033[0m\n");
        return -1;
    }

    struct dirent *entry;
    int latest_pid = -1;

    while((entry = readdir(proc_dir)) != NULL){
        if(entry->d_type == DT_DIR){

            int pid = atoi(entry->d_name);
            if(pid > latest_pid){
                latest_pid = pid;
            }
        }
    }

    closedir(proc_dir);
    return latest_pid;
}

static struct termios terminal;

void enable_raw_mode(){
    tcgetattr(STDIN_FILENO, &terminal);
    struct termios raw = terminal;
    raw.c_lflag &= ~(ICANON | ECHO); //canonical mode off krenge tbhi enter ki zarurat ni pdego
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal);
}

void neonate(char* cmd){
    // cmd hoga neonate -n time
    // 3 args
    //input .c mein neonate se cmp kraio

    int arg_cnt = 0;
    char* args[3];
    bool error = false;
    atexit(disable_raw_mode);

    char* token = strtok(cmd, " \t");
    while(token != NULL){
        if(arg_cnt == 3){
            error = true;
            break;
        }
        args[arg_cnt++] = token;
        token = strtok(NULL, " \t");
    }

    if(error | arg_cnt < 3 | strcmp(args[1], "-n") != 0){
        printf("\033[31mInvalid command!\033[0m\n");
        return;
    }

    int interval = atoi(args[2]);
    
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0); //save krra flags baad mein restore k liye
    if(flags == -1){
        printf("\033[31mfcntl(F_GETFL)!\033[0m\n");
        return;
    }
    
    // Set to non-blocking mode
    if(fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1){
        printf("\033[31mfcntl(F_SETFL, O_NONBLOCK)\033[0m\n");
        return;
    }

    // int pid = fork();

    // if(pid < 0){
    //     perror("fork");
    //     exit(1);
    // }

    // else if(pid == 0){
        char c;
        int latest_pid = -1;

        int check_interval = 100000; 
        int total_time = 0;          
        int sleep_microseconds = interval * 1000000; 

        latest_pid = get_latest_pid();
        printf("%d\n", latest_pid);

        while(1){
            if(read(STDIN_FILENO, &c, 1) == 1 && c == 'x') {
                break;  // Exit the loop if 'x' is pressed
            }

            if(total_time >= sleep_microseconds){
                latest_pid = get_latest_pid();
                printf("%d\n", latest_pid);
                total_time = 0; 
            }

            usleep(check_interval); 
            total_time += check_interval; 
        }

        // exit(0);
    // }

    // else{
    //     fg_pid = pid;
    //     fg_name = "neonate";
    //     int status;
    //     waitpid(pid, &status, 0);
    // }


    // isme x k baad wait krna pdra tha
    // while(1){
    //     if(read(STDIN_FILENO, &c, 1) == 1 && c == 'x'){
    //         break;  // Exit the loop if 'x' is pressed
    //     }

    //     latest_pid = get_latest_pid();
    //     printf("%d\n", latest_pid);

    //     sleep(interval);
    // }

    if(fcntl(STDIN_FILENO, F_SETFL, flags) == -1){
        printf("\033[31mfcntl(F_SETFL, blocking)\033[0m\n");
        exit(1);
    }

    disable_raw_mode();
    // printf("done\n");

    return;
}