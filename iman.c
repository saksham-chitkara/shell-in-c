#include "headers.h"

void rem_tags(char *str){
    int print = 1;
    for(int i = 0; str[i] != '\0'; i++){
        if (str[i] == '<'){
            print = 0;
        } 
        else if (str[i] == '>'){
            print = 1;
        } 
        else if(print){
            putchar(str[i]);
        }
    }
}

void man(char *cmd){
    char* token = strtok(cmd, " \t");
    token = strtok(NULL, " \t"); //skip iman

    char* target = NULL;
    while(token != NULL){
        target = token;
        break;
    }

    if(target == NULL){
        printf("Provide the target name!\n");
        return;
    }

    int socket_fd;
    struct hostent *server;
    struct sockaddr_in server_addr;
    int port = 80;

    server = gethostbyname("man.he.net"); //DNS k liye
    if(server == NULL){
        fprintf(stderr, "Error: Could not resolve the hostname.\n");
        exit(1);
    }

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //socket bnana
        perror("Error creating socket");
        return;
    }

    // Setting up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if(connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){ // to connect to server
        perror("Error connecting to server");
        return;
    }

    // Constructing the HTTP GET request
    char request[4096];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", target);

    // Sending the GET request to the server
    if(write(socket_fd, request, strlen(request)) < 0){
        perror("Error sending request to server");
        return;
    }

    char res[4096];
    int bytes;
    int not_found = 0;

    while((bytes = read(socket_fd, res, sizeof(res) - 1)) > 0){
        res[bytes] = '\0';

        if(strstr(res, "No matches") != NULL){
            not_found = 1;
        }

        rem_tags(res);
    }

    if(bytes < 0){
        perror("Error reading res from server");
        return;
    }

    if(not_found){
        printf("ERROR: No matches for %s command\n", target);
    }
    
    close(socket_fd);
}
