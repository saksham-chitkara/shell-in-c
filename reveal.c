#include "headers.h"

void read_dir(char* path, int aflag, int* num_of_files, int* blocks){
    DIR *dir = opendir(path);
    if(!dir){
        printf("\033[31mUnable to open the directory %s\033[0m\n", path);
        return;
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if (!aflag && entry->d_name[0] == '.'){
            continue;
        }

        if(*num_of_files < 4096){
            strcpy(content[*num_of_files], entry->d_name);

            struct stat fstat;
            char full_path[4096];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

            if(stat(full_path, &fstat) == 0){
                *blocks += fstat.st_blocks;
            }
            else if(stat(full_path, &fstat) == -1){
                printf("\033[31mUnable to get file status of the file : %s\033[0m\n", full_path);
            }

            (*num_of_files)++;
        }
    }

    int total = *(num_of_files);
    closedir(dir);
    sort(total);
}

int compare(const void *a, const void *b){
    return strcmp(*(const char **)a, *(const char **)b);
}

void sort(int num_of_files){
    qsort(content, num_of_files, sizeof(char*), compare);
}

void file_info(char* dir_path, char* file_name, int lflag){
    char file_path[4096];
    snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, file_name);

    struct stat fstat;
    if(stat(file_path, &fstat) == -1){
        printf("\033[31mUnable to get file status of the file : %s\033[0m\n", file_path);
        return;
    }

    if(lflag){
        // Permissions
        printf((S_ISDIR(fstat.st_mode)) ? "d" : "-");
        printf((fstat.st_mode & S_IRUSR) ? "r" : "-");
        printf((fstat.st_mode & S_IWUSR) ? "w" : "-");
        printf((fstat.st_mode & S_IXUSR) ? "x" : "-");
        printf((fstat.st_mode & S_IRGRP) ? "r" : "-");
        printf((fstat.st_mode & S_IWGRP) ? "w" : "-");
        printf((fstat.st_mode & S_IXGRP) ? "x" : "-");
        printf((fstat.st_mode & S_IROTH) ? "r" : "-");
        printf((fstat.st_mode & S_IWOTH) ? "w" : "-");
        printf((fstat.st_mode & S_IXOTH) ? "x" : "-");

        //no of links
        printf(" %ld", (long)fstat.st_nlink);

        //user and group
        struct passwd *pw = getpwuid(fstat.st_uid);
        struct group *gr = getgrgid(fstat.st_gid);
        if(pw && gr){
            printf(" %s %s", pw->pw_name, gr->gr_name);
        } 

        else{
            printf("\033[31m User/group info not found!\033[0m"); 
        }

        //file ka size
        printf(" %4lld", (long long)fstat.st_size);

        //last modification time
        char time_buf[80];
        struct tm* tm_info = localtime(&fstat.st_mtime);
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm_info);
        printf(" %s", time_buf);

        printf(" ");
    }

    if(S_ISDIR(fstat.st_mode)){
        printf("\033[1;34m%s\033[0m", file_name);
    } 
    else if(fstat.st_mode & S_IXUSR){
        printf("\033[1;32m%s\033[0m", file_name);
    } 
    else{
        printf("\033[1;37m%s\033[0m", file_name);
    }

    printf("\n");
}

void reveal(char* command){
    char command_copy[4096];
    strcpy(command_copy, command);

    char* token = strtok(command_copy, " \t");
    if (token != NULL) token = strtok(NULL, " \t"); //skip reveal wala word

    int lflag = 0;
    int aflag = 0;
    char dir_path[4096] = "";

    while(token != NULL){
        if(token[0] == '-'){ 
            int len = strlen(token);
            if(len == 1){
                // printf("%s\n", prev_dir);
                strcpy(dir_path, prev_dir);
                if(strlen(prev_dir) == 0){
                    printf("\033[31mOLDPWD not set!\033[0m\n");
                    return;
                }
            }

            else{
                for(int i = 1; i < len; i++){
                    if (token[i] == 'l') lflag = 1;
                    if (token[i] == 'a') aflag = 1;
                }
            }
        } 

        else{
            if(strcmp(token, ".") == 0){
                strcpy(dir_path, cwd);
            } 
            
            else if (strcmp(token, "..") == 0){
                char par_path[4096];
                int n = strlen(cwd);
                int j = n - 1;

                while (j >= 0 && cwd[j] != '/') j--;
                if(j >= 0){
                    strncpy(par_path, cwd, j);
                    par_path[j] = '\0';
                } 

                else{
                    par_path[0] = '\0'; 
                }
                strcpy(dir_path, par_path);
            } 
            
            else if(strncmp(token, "~", 1) == 0){
                if(strlen(token) == 1){
                    strcpy(dir_path, home);
                }
                else{
                    snprintf(dir_path, sizeof(dir_path), "%s%s", home, token + 1);
                    if(realpath(dir_path, dir_path) == NULL){
                        printf("\033[31mNo such file or directory!\033[0m\n");
                        return;
                    }    
                }
            } 
            
            else{
                // strncpy(dir_path, token, sizeof(dir_path) - 1);

                if(realpath(token, dir_path) == NULL){
                    printf("\033[31mNo such file or directory!\033[0m\n");
                    return;
                }

                // printf("%s\n", dir_path);
                dir_path[sizeof(dir_path) - 1] = '\0'; 
            }
        }
        
        token = strtok(NULL, " \t");
    }

    // printf("3\n");

    if(strlen(dir_path) == 0){
        strcpy(dir_path, cwd); 
    }
    
    struct stat path_stat;
    if(stat(dir_path, &path_stat) == -1){
        printf("\033[31mUnable to get status!\033[0m\n");
        return;
    }

    if(S_ISREG(path_stat.st_mode)){
        //if file 
        file_info("", dir_path, lflag);
    } 

    else if(S_ISDIR(path_stat.st_mode)){
        // if dir
        int num_of_files = 0;
        int tot_blocks = 0;
        read_dir(dir_path, aflag, &num_of_files, &tot_blocks);

        if(lflag){
            printf("total %d\n", tot_blocks);
        }

        for(int i = 0; i < num_of_files; i++){
            file_info(dir_path, content[i], lflag);
        }
    }
}
