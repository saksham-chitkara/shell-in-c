#ifndef _HOP_H
#define _HOP_H

extern char prev_dir[4096];

void hop(char* args);
void write_dir_to_file(char* dir, char* file);
void read_dir_from_file(char* dir, char* file);

#endif