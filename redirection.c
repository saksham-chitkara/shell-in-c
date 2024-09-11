#include "headers.h"

void redirect(char* cmd, bool pipe_se_ara, int pipe_in, int pipe_out){
    //cmd mein ya to > / >> / < hai to iss func mein ayga
    //wc < 1.txt > 2.txt
    printf("in redirection\n");
    int inp_fd = 0;
    int out_fd = 1; //default

    
    char* append = strstr(cmd, ">>");
    char* output = append ? NULL : strstr(cmd, ">");
    char* input = strstr(cmd, "<");

    // printf("%s\n", output);

    if(input != NULL){
        //ab mujhe input jaha s lena vo file chahiye
        char* token = strtok(input + 1, " \t"); //phla word hi filaname hoga
        if(token){
            char abs_path[1024];  
            if(realpath(token, abs_path) == NULL){
                printf("Error converting to absolute path\n");
                return;
            }

            inp_fd = open(abs_path, O_RDONLY);
            if(inp_fd == -1){
                printf("No such input file found!\n");
                inp_fd = 0; 
                
                return;
            }
        }
        else{
            printf("Please give correct arguements for redirection\n");
            return;
        }

        *input = '\0';

    }

    if(output != NULL){
        // printf("here\n");
        char* token= strtok(output + 1, " \t"); //phla word hi filaname hoga
        if(token){
            char abs_path[1024];  
            if(realpath(token, abs_path) == NULL){
                printf("Error converting to absolute path\n");
                return;
            }

            out_fd = open(abs_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if(out_fd == -1){
                printf("No such input file found!\n");
                out_fd = 1; 
                
                return;
            }
        }
        else{
            printf("Please give correct arguements for redirection\n");
            return;
        }

        *output = '\0';

    }

    if(append != NULL){
        char* token= strtok(append + 2, " \t"); //phla word hi filaname hoga
        if(token){
            char abs_path[1024];  
            if(realpath(token, abs_path) == NULL){
                printf("Error converting to absolute path\n");
                return;
            }

            out_fd = open(abs_path, O_WRONLY | O_CREAT | O_APPEND, 0644);

            if(out_fd == -1){
                printf("No such input file found!\n");
                out_fd = 1; 
                
                return;
            }
        }
        else{
            printf("Please give correct arguements for redirection\n");
            return;
        }

        *append = '\0';
    }

    
    // file vgairah nikaal li
    // printf("%s..\n", cmd);
    // printf("%d..\n", inp_fd);
    // printf("%d..\n", out_fd);

    char* token = strtok(cmd," \t");  //phla h cmd hoga jo run hoga
    char* args[10];
    int cnt = 0;

    while(token){
        args[cnt++] = token;
        // printf("%s\n", token);
        token = strtok(NULL, " \t");
    }

    args[cnt] = NULL;

    if(pipe_se_ara){
        //agar input wala redirection h to pipein ka mtlb nhi h file se hi input lera h
        if(pipe_out != 1 && !output && !append){
            out_fd = pipe_out;
        }
        if(pipe_in != 0 && !input){
            inp_fd = pipe_in;
        }
    }

    // printf("reached\n");

    if(cnt){
        int pid = fork();
        if(pid == -1){
            perror("fork");
            exit(1);
        } 

        else if(pid == 0){ 
            int inp_store = dup(0);
            int out_store = dup(1);
            // printf("%d %d\n", inp_fd, out_fd);
            if(inp_fd != 0){
                // printf("%d..", inp_fd);
                dup2(inp_fd, 0);
                close(inp_fd);
            }

            if(out_fd != 1){
                // fprintf(stdout, "%d..", out_fd);
                // fflush(stdout);
                dup2(out_fd, 1);
                close(out_fd);
            }

            // if(strcmp(args[0], "log") == 0){
            //     get_all();
            // }

            // else{
            //     execvp(args[0], args);
            //     perror("execvp");
            //     exit(1);
            // }

            
            tokenise(cmd);

            // free(concatenated_args);
            dup2(inp_store, 1);
            dup2(out_store, 0);

            exit(0);
        } 
        else { 
            int status;
            waitpid(pid, &status, 0);
        }
    }
}
