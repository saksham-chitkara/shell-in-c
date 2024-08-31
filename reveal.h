#ifndef _REVEAL_H
#define _REVEAL_H

// extern char* content[1024];
void read_dir(char* path, int show_hidden, int* num_of_files, int* total_blocks);
int compare(const void *a, const void *b);
void sort(int num_of_files);
void file_info(char* dir_path, char* file_name, int lflag);
void reveal(char* command);

#endif