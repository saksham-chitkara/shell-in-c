#include "headers.h"

char prev_dir[4096] = "";

void write_dir_to_file(char* dir, char* file){
    char* file_path = (char*)malloc((sizeof(char) * 1000));
    if(file_path == NULL){
        printf("\033[31mMalloc failed!\033[0m\n");
        return;
    }  

    sprintf(file_path, "%s/%s", home, file);

    FILE* fd = fopen(file_path, "w");
    if(file == NULL){
        printf("\033[31mError in opening cwd.txt!\033[0m\n");
        return;
    }

    fprintf(fd, "%s\n", dir); 
    
    free(file_path);
    fclose(fd);
}

void read_dir_from_file(char* dir, char* file){
    char* file_path = (char*)malloc((sizeof(char) * 1000));
    if(file_path == NULL){
        printf("\033[31mMalloc failed!\033[0m\n");
        return;
    }  

    sprintf(file_path, "%s/%s", home, file);

    FILE* fd = fopen(file_path, "r");
    if(fd == NULL){
        printf("\033[31mError in opening cwd.txt!\033[0m\n");
        return;
    }

    // fscanf(file, "%s\n", dir); 
    if(fgets(dir, 1000, fd) == NULL){
        printf("\033[31mError reading from file %s!\033[0m\n", file_path);
    } 
    else{
        int len = strlen(dir);
        if(len > 0 && dir[len - 1] == '\n'){
            dir[len - 1] = '\0';
        }
    }
    
    free(file_path);
    fclose(fd);
}

void hop(char* args){
    // printf("%s\n", args);
    char* single[1000];
    for(int i = 0; i < 1000; i++){
        single[i] = (char*) malloc(sizeof(char*) * 4096);
        if(single[i] == NULL){
            printf("\033[31mMalloc failed!\033[0m\n");
            return;
        }
    }

    int total_args = 0;

    char* tmp = (char*)malloc(sizeof(char) * 4096);
    if(tmp == NULL){
        printf("\033[31mMalloc failed!\033[0m\n");
        return;
    }
    
    char* token = strtok(args, " \t");
    if (token != NULL) token = strtok(NULL, " \t"); //skip hop wala word

    while(token != NULL){
        strcpy(single[total_args], token); 
        total_args++;
        token = strtok(NULL, " \t");
    }

    if(total_args == 0){
        //for no args go to home dir
        if(chdir(home) != -1){
            strcpy(cwd, home);
            printf("%s\n\n", cwd);
            write_dir_to_file(cwd, "cwd.txt");
        }
        else{
            printf("\033[31mChdir failed!\033[0m\n");
            return;
        }
    }

    else{
        int i = 0;
        while(i < total_args){
            strcpy(tmp, cwd);
            if(strcmp(".", single[i]) == 0){
                //whi dir rhegi
            }

            else if(strncmp("~", single[i], 1) == 0){
                if(strlen(single[i]) == 1){
                    if(chdir(home) != -1){
                        strcpy(cwd, home);
                        write_dir_to_file(cwd, "cwd.txt");
                    }
                    else{
                        printf("\033[31mChdir failed!\033[0m\n");
                        return;
                    }
                }

                else{
                    char* new_path = (char*)malloc((sizeof(char) * 4096));   
                    sprintf(new_path, "%s/%s", home, single[i] + 2);  

                    if(realpath(new_path, new_path) == NULL){
                        printf("\033[31mNo such directory exists!\033[0m\n");
                        return;
                    }               
                    
                    if(chdir(new_path) != -1){
                        strcpy(cwd, new_path);
                        write_dir_to_file(cwd, "cwd.txt");
                    }
                    else{
                        printf("\033[31mChdir failed!\033[0m\n");
                        free(new_path);
                        return;
                    }

                    free(new_path);
                }
                
            }

            else if(strcmp("-", single[i]) == 0){

                if(strlen(prev_dir)){
                    if(chdir(prev_dir) != -1){
                        strcpy(cwd, prev_dir);
                        write_dir_to_file(cwd, "cwd.txt");
                    }
                    else{
                        printf("\033[31mChdir failed!\033[0m\n");
                        return;
                    }
                }

                else{
                    printf("\033[31mOLDPWD does not exist!\033[0m\n");
                    return;
                }
            }

            else{
                char* new_path = (char*)malloc((sizeof(char) * 4096));
                if(new_path == NULL){
                    printf("\033[31mMalloc failed!\033[0m\n");
                    return;
                }

                if(single[i][0] == '/'){ //absolute path
                    strcpy(new_path, single[i]);
                }

                else{ //relative path

                    if(realpath(single[i], new_path) == NULL){
                        printf("\033[31mNo such directory exists!\033[0m\n");
                        return;
                    }

                    // printf("%s\n", new_path);
                }
                
                if(chdir(new_path) != -1){
                    strcpy(cwd, new_path);
                    write_dir_to_file(cwd, "cwd.txt");
                }

                else{
                    printf("\033[31mNo such directory exists!\033[0m\n");
                    free(new_path);
                    return;
                }

                free(new_path);
            }

            printf("%s\n\n", cwd);
            i++;
            strcpy(prev_dir, tmp);
            write_dir_to_file(prev_dir, "pwd.txt");
       }
    }

    for(int i = 0; i < 1000; i++){
        free(single[i]);
    }

    free(tmp);

}