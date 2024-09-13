// #include "headers.h"

// void temp(){
//     printf("hello chutiye\n");
// }

// void execute_cmd(char *cmd, int inp_fd, int out_fd){
//     char  *tokens[1024];
//     //f//printf(stderr, "%s\n", cmd);
//     if(strstr(cmd, ">") || strstr(cmd, ">>") || strstr(cmd, "<")){
//         redirect(cmd, true, inp_fd, out_fd);
//         return;
//     }

//     while(*cmd == ' ') cmd++;
//     char *end = cmd + strlen(cmd) - 1;
//     while(end > cmd && *end == ' '){
//         end--;
//     }
//     *(end + 1) = '\0';
    
//     int i = 0;
//     int len = strlen(cmd);
//     bool consider = true;
//     int token_cnt = 0;
//     int j = 0;

//     //sed mein error ara kyuki quotes mein space ko delimitor manra
//     //isliye ab yeh krna pdega

//     while(i < len){
//         if(cmd[i] == '\'' || cmd[i] == '"') consider = !consider;

//         if(cmd[i + 1] == '\0' || cmd[i] == ' ' && consider){
//             if(cmd[i + 1] == '\0'){
//                 i++;
//                 cmd[i] = '\0';
//             }

//             else cmd[i] = '\0';
        

//             char* next = cmd + j;

//             //remove quotes from ends
//             if(next[0] == '\'' && next[strlen(next) - 1] == '\'' || next[0] == '"' && next[strlen(next) - 1] == '"'){
//                 next[strlen(next) - 1] = '\0';
//                 next++;
//             }

//             // //printf("%s\n", next);
//             if(strlen(next))
//                tokens[token_cnt++] = next;
//             j = i + 1;
//         }
//         i++;
//     }
//     //
//     //

//     // char *tokens[1024];
//     // int arg_cnt = 0;

//     // char *token = strtok(cmd, " \t");
//     // while(token != NULL){
//     //     tokens[arg_cnt++] = token;
//     //     //printf("%s\n", token);
//     //     token = strtok(NULL, " \t");
//     // }

//     tokens[token_cnt] = NULL;
//     //f//printf(stderr, "%s\n", tokens[0]);

//     int pid = fork();
//     if(pid < 0){
//         perror("fork");
//         exit(1);
//     }

//     else if(pid == 0){ 
        
//         //printf("reached\n");
//         //printf("%s\n", tokens[0]);

//         // int inp_store = dup(0);
//         // int out_store = dup(1);
//         // // printf("%d %d\n", inp_store, out_store);

//         // if(inp_fd != 0){
//         //     dup2(inp_fd, 0);
//         //     close(inp_fd);
//         // }

//         // if(out_fd != 1){
//         //     dup2(out_fd, 1);
//         //     close(out_fd);
//         // }

//         // non built in k liye bhi krna
//         //
//         //
//         //
//         //

//         // //f//printf(stdout, "reached\n");
//         // fflush(stdout);

//         if(strcmp(tokens[0], "reveal") == 0){
//             // //printf("printed\n");
//             // fflush(stdout);
//             // fflush(stdout);
//             // reveal(tokens[0]);
//             temp();

//             // dup2(inp_store, 0);
//             // close(1);
//             // dup2(out_store, 1);
//             //printf("printed\n");
//             // fflush(stdout);
//             //printf("here\n");
//         }

//         else{
//             printf("print else log\n");
//             printf("%d %d\n", inp_fd, out_fd);
//             for(int i = 0; i < token_cnt; i++){
//                 printf("%s...", tokens[i]);
//                 fflush(stdout);
//             }

//             // dup2
//             // dup2(out_store, 1);
//             // close(0);
//             execvp(tokens[0], tokens);
//             perror("execvp");

//             // dup2(inp_store, 0);

//             //printf("print else log2\n");
//             exit(1);
//         }

//         //printf("here2\n");

//     } 
    
//     else{
//         //printf("reachedpar\n");
//         int status;
//         waitpid(pid, &status, 0);
//         //printf("reachedend\n");

//     }
    
// }


// void handle_pipes(char *cmd){
//     char *cmds[1024];
    
//     int num_pipes = 0;
//     int arg_cnt = 0;

//     int len = strlen(cmd);
//     for(int i = 0; i < len; i++){
//         if(*(cmd + i) == '|') num_pipes++;
//     }
    
//     char *token = strtok(cmd, "|");
//     while(token != NULL){
//         cmds[arg_cnt++] = token;
//         // //printf("%s\n", token);
//         token = strtok(NULL, "|");
//     }

//     if(arg_cnt != num_pipes + 1){
//         //printf("Invalid use of pipe\n");
//         return;
//     }

//     int pipefds[2 * num_pipes];
//     for(int i = 0; i < num_pipes; i++){
//         if(pipe(pipefds + i * 2) == -1){
//             perror("pipe");
//             return;
//         }
//     }

//     for(int i = 0; i < 2 * num_pipes; i++){
//         //printf("%d ", pipefds[i]);
//     }
//     //printf("\n");


//     int inp_fd = 0;

//     int inp_store = dup(0);
//     int out_store = dup(1);

//     for(int i = 0; i < arg_cnt; i++){
//         int out_fd = (i == arg_cnt - 1) ? 1 : pipefds[i * 2 + 1];
//         // int inp_fd = (i == 0) : 0 : pipefds[2 * i];
//         //f//printf(stderr, "%s...\n", cmds[i]);
//         printf("%d %d\n", inp_fd, out_fd);
//         // if(i != 0){
//             dup2(inp_fd, 0);
//             // close(inp_fd);
//         // }

//         // if(i != arg_cnt - 1){
//             dup2(out_fd, 1);
//             // close(out_fd);
//         // }

//         execute_cmd(cmds[i], inp_fd, out_fd);
        
//         //f//printf(stderr, "%d...\n", i);

//         // if(inp_fd != 0) close(inp_fd);
//         // if(out_fd != 1) close(out_fd);

//         if(i != 0){
//             dup2(inp_store, 0);
//             close(inp_fd);
//         }

//         if(i != arg_cnt - 1){
//             dup2(out_store, 1);
//             close(out_fd);
//         }
        
//         // dup2(inp_store, 0);
//         // dup2(out_store, 1);

//         if(i != arg_cnt - 1){
//             inp_fd = pipefds[i * 2];
//         }
//     }
//     dup2(inp_store, 0);
//     dup2(out_store, 1);
    
//     return;
// }


#include "headers.h"

void execute_cmd(char *cmd, int inp_fd, int out_fd, int bg){
    char  *tokens[1024];
    if(strstr(cmd, ">") || strstr(cmd, ">>") || strstr(cmd, "<")){
        redirect(cmd, true, inp_fd, out_fd, bg);
        return;
    }

    while(*cmd == ' ') cmd++;
    char *end = cmd + strlen(cmd) - 1;
    while(end > cmd && *end == ' '){
        end--;
    }
    *(end + 1) = '\0';
    
    int i = 0;
    int len = strlen(cmd);
    bool consider = true;
    int token_cnt = 0;
    int j = 0;

    char orig[1024];
    strcpy(orig, cmd);
    //sed mein error ara kyuki quotes mein space ko delimitor manra
    //isliye ab yeh krna pdega

    while(i < len){
        if(cmd[i] == '\'' || cmd[i] == '"') consider = !consider;

        if(cmd[i + 1] == '\0' || cmd[i] == ' ' && consider){
            if(cmd[i + 1] == '\0'){
                i++;
                cmd[i] = '\0';
            }

            else cmd[i] = '\0';
        

            char* next = cmd + j;

            //remove quotes from ends
            if(next[0] == '\'' && next[strlen(next) - 1] == '\'' || next[0] == '"' && next[strlen(next) - 1] == '"'){
                next[strlen(next) - 1] = '\0';
                next++;
            }

            // printf("%s\n", next);
            if(strlen(next))
                tokens[token_cnt++] = next;
            j = i + 1;
        }
        i++;
    }
   
    tokens[token_cnt] = NULL;


    int pid = fork();
    if(pid < 0){
        perror("fork");
        exit(1);
    }

    else if(pid == 0){ 
        int inp_store = dup(0);
        int out_store = dup(1);

        if(inp_fd != 0){
            dup2(inp_fd, 0); 
            close(inp_fd);    
        }

        if(out_fd != 1){
            dup2(out_fd, 1);  
            close(out_fd);   
        }

        if(bg){
            setup_sigchld_handler();
            execute(orig, 1);
        }

        else if(distribute(orig)){
            
        }

        else{
            execvp(tokens[0], tokens);
            perror("execvp");  
            exit(1);
        }

    
        // fprintf(stderr, "%s..\n", concatenated_args);
        // tokenise(cmd);

        dup2(inp_store, 1);
        dup2(out_store, 0);

        exit(0);
    } 
    
    else{
        int status;
        waitpid(pid, &status, 0);

        read_dir_from_file(prev_dir, "pwd.txt");
        read_dir_from_file(cwd, "cwd.txt");

        if(chdir(cwd) == -1){
            perror("chdir!");
        }
    }
    
}

void handle_pipes(char *cmd, int last_bg){
    setup_sigchld_handler();
    char *cmds[1024];
    
    int num_pipes = 0;
    int arg_cnt = 0;

    int len = strlen(cmd);
    for(int i = 0; i < len; i++){
        if(*(cmd + i) == '|') num_pipes++;
    }
    
    char *token = strtok(cmd, "|");
    while(token != NULL){
        cmds[arg_cnt++] = token;
        // //printf("%s\n", token);
        token = strtok(NULL, "|");
    }

    if(arg_cnt != num_pipes + 1){
        printf("\033[31mInvalid use of pipe!\033[0m\n");
        return;
    }

    int pipefds[2 * num_pipes];
    for(int i = 0; i < num_pipes; i++){
        if(pipe(pipefds + i * 2) == -1){
            perror("pipe");
            return;
        }
    }

    // for(int i = 0; i < 2 * num_pipes; i++){
    //     printf("%d ", pipefds[i]);
    // }
    // printf("\n");

    int inp_fd = 0;

    for(int i = 0; i < arg_cnt; i++){
        int out_fd = (i == arg_cnt - 1) ? 1 : pipefds[i * 2 + 1];

        if(last_bg && i == arg_cnt - 1)
            execute_cmd(cmds[i], inp_fd, out_fd, 1);
        else 
            execute_cmd(cmds[i], inp_fd, out_fd, 0);

        if(inp_fd != 0) close(inp_fd);
        if(out_fd != 1) close(out_fd);

        if(i != arg_cnt - 1){
            inp_fd = pipefds[i * 2];
        }  
    }
    
    return;
}
