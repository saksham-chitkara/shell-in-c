#include "headers.h"

void print_with_color(char *path, int isdir, char* dir_tobe_searched){
    char npath[4096];

    if(strncmp(path, dir_tobe_searched, strlen(dir_tobe_searched)) == 0){
        snprintf(npath, sizeof(npath), ".%s", path + strlen(dir_tobe_searched));
    } 
    else{
        snprintf(npath, sizeof(npath), ".%s", path);
    }

    if(isdir){
        printf("\033[1;34m%s\033[0m\n", npath);
    } 

    else{
        printf("\033[1;32m%s\033[0m\n", npath);
    }

}

char found_file[4096][400];
char found_dir[4096][400];

void search_directory(char *dir_tobe_searched, char *search_name, int fflag, int dflag, int eflag, int* file_cnt, int* dir_cnt){
    struct dirent* content;
    struct stat file_ka_stat;
    char path[1024];

    DIR *dir = opendir(dir_tobe_searched);
    if(!dir){
        printf("\033[31mCan't open the directory!\033[0m\n");
        return;
    }
 
    while((content = readdir(dir)) != NULL){
        if(strncmp(content->d_name, ".", 1) == 0 || strncmp(content->d_name, "..", 2) == 0){
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_tobe_searched, content->d_name);
        // printf("%s\n", path);
        // printf("%s\n\n", content->d_name);
    
        if(stat(path, &file_ka_stat) == -1){
            printf("\033[31mUnable to get the status of file %s!\033[0m\n", path);
            continue;
        }

        int isdir = S_ISDIR(file_ka_stat.st_mode);
        int match = (isdir && dflag) || (!isdir && fflag);

        if(dflag == 0 && fflag == 0) match = 1; //dono not set to file dir sab chlega

        if(match && strncmp(content->d_name, search_name, strlen(search_name)) == 0){
            if(isdir){
               strcpy(found_dir[(*dir_cnt)++], path);
            }
            else{
                strcpy(found_file[(*file_cnt)++], path);
            }

            // strcpy(result_path, path);
            // print_with_color(path, isdir);
        }

        if(isdir){ //dir k andar bhi dhundenge
            search_directory(path, search_name, fflag, dflag, eflag, file_cnt, dir_cnt);
        }
    }

    closedir(dir);
}

void find(char* cmd){
    
    char* token = strtok(cmd, " \t");
    token = strtok(NULL, " \t"); //phla to seek hoga usse htao

    int eflag = 0;
    int dflag = 0;
    int fflag = 0;
    int arg_cnt = 0;
    char* args[2];

    args[0] = malloc(4096 * sizeof(char));
    args[1] = malloc(4096 * sizeof(char));

    char dir_tobe_searched[4096];
    char search_name[4096];

    while(token != NULL){
        if(token[0] == '-'){ //for flags
            int len = strlen(token);
            if(len != 1){
                for(int i = 1; i < len; i++){
                    if(token[i] == 'e') eflag = 1;
                    else if(token[i] == 'f') fflag = 1;
                    else if(token[i] == 'd') dflag = 1;
                }
            }
            else{
                strcpy(args[arg_cnt], token);
                arg_cnt++;
            }
        }

        else{
            strcpy(args[arg_cnt], token);
            arg_cnt++;
        }

        token = strtok(NULL, " \t");
    }

    strcpy(search_name, args[0]);
    // printf("%s", args[0]);
    // printf("%s", args[1]);

    if(arg_cnt >= 2){
        if(strcmp(args[1], "-") == 0){
            if(strlen(prev_dir) == 0){
                printf("\033[31mOLDPWD not set!\033[0m\n");
                return;
            }
            else{
                strcpy(dir_tobe_searched, prev_dir);
            }
        }

        else if(strcmp(args[1], ".") == 0) strcpy(dir_tobe_searched, cwd);
        else if(strcmp(args[1], "..") == 0){
            char par_path[1024];
            int n = strlen(cwd);

            int j = n - 1;
            while(j >= 0 && cwd[j] != '/') j--;

            strncpy(par_path, cwd, j);
            par_path[j] = '\0';
            strcpy(dir_tobe_searched, par_path);
        }
        
        else if(strcmp(args[1], "~") == 0){
            strcpy(dir_tobe_searched, home);

            if(strlen(args[1]) == 1){
                strcpy(dir_tobe_searched, home);
            }

            else{
                char* new_path = (char*)malloc((sizeof(char) * 4096));   
                sprintf(new_path, "%s/%s", home, args[1] + 2);  

                if(realpath(new_path, new_path) == NULL){
                    printf("\033[31mRealpath failed!\033[0m\n");
                    return;
                }               
                
                strcpy(dir_tobe_searched, new_path);
                free(new_path);
            }
        }

        else{
            char* new_path = (char*)malloc((sizeof(char) * 4096));
            if(new_path == NULL){
                printf("\033[31mMalloc failed!\033[0m\n");
                return;
            }

            if(args[1][0] == '/'){ //absolute path
                strcpy(new_path, args[1]);
            }

            else{ //relative path
                sprintf(new_path, "%s/%s", cwd, args[1]);
                // printf("%s\n", new_path);

                if(realpath(new_path, new_path) == NULL){
                    printf("\033[31mRealpath failed!\033[0m\n");
                    return;
                }
            }
            
            strcpy(dir_tobe_searched, new_path);
            free(new_path);
        }
    }

    else strcpy(dir_tobe_searched, cwd);

    // printf("%s\n", dir_tobe_searched);

    if(dflag && fflag){
        printf("\033[31mInvalid Flags!\033[0m\n");
        return;
    }

    int fcnt = 0;
    int dcnt = 0;
    // printf("%s\n", dir_tobe_searched);
    search_directory(dir_tobe_searched, search_name, fflag, dflag, eflag, &fcnt, &dcnt);

    if(fcnt == 0 && dcnt == 0){
        printf("\033[31mNo match found!\033[0m\n");
        return;
    }

    if(eflag){
        if(fcnt == 1 && dcnt == 0){
            //readable ya ni
            if(access(found_file[0], R_OK) != 0){
                printf("\033[31mMissing permissions for task!\033[0m\n");
            } 

            else{
                struct stat file_stat;
                if(stat(found_file[0], &file_stat) == -1){
                    printf("\033[31mUnable to get status of the found file %s!\033[0m\n", found_file[0]);
                    return;
                }
                
                //executable ya ni
                if(file_stat.st_mode & S_IXUSR){
                    printf("Executing file: %s\n", found_file[0]);
                    char *args[] = {(char *)found_file[0], NULL};

                    if(execv(found_file[0], args) == -1){
                        perror("execv");
                    }
                } 

                else{
                    print_with_color(found_file[0], 0, dir_tobe_searched);

                    FILE *file = fopen(found_file[0], "r");
                    if(file == NULL){
                        printf("\033[31mError opening file!\033[0m\n");
                    } 

                    else{
                        char line[1024];
                        while(fgets(line, sizeof(line), file)){
                            printf("%s", line);
                        }
                        printf("\n");

                        fclose(file);
                    }
                }
            }
        }

        else if(dcnt == 1 && fcnt == 0){ 
            print_with_color(found_dir[0], 1, dir_tobe_searched);

            if(access(found_dir[0], X_OK) != 0){
                printf("\033[31mMissing permissions for task!\033[0m\n");
            } 
            else{
                if(chdir(found_dir[0]) != 0){
                    printf("\033[31mChdir failed!\033[0m\n");
                }
                else{
                    strcpy(prev_dir, cwd);
                    write_dir_to_file(prev_dir, "pwd.txt");
                    strcpy(cwd, found_dir[0]);  
                    write_dir_to_file(cwd, "cwd.txt");
                }
            }
            
        }

        else{ 
            for(int i = 0; i < fcnt; i++){
                print_with_color(found_file[i], 0, dir_tobe_searched);
            }

            for(int i = 0; i < dcnt; i++){
                print_with_color(found_dir[i], 1, dir_tobe_searched);
            }
        }
    }

    else{ 
        for(int i = 0; i < fcnt; i++){
            print_with_color(found_file[i], 0, dir_tobe_searched);
        }

        for(int i = 0; i < dcnt; i++){
            print_with_color(found_dir[i], 1, dir_tobe_searched);
        }
    }

    free(args[0]);
    free(args[1]);
    return;
}