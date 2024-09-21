#ifndef _SEEK_H
#define _SEEK_H

void find(char* cmd);
void search_directory(char *dir_tobe_searched, char *search_name, int fflag, int dflag, int eflag, int* file_cnt, int* dir_cnt);
void print_with_color(char *path, int isdir, char* dir_tobe_searched);

#endif