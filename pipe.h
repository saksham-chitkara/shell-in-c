#ifndef _PIPE_H
#define _PIPE_H

void execute_cmd(char *cmd, int input_fd, int output_fd);
void handle_pipes(char *command);

#endif