#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>


void error(const char* msg){
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){
    int sockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    if(argc < 2){
        fprintf(stderr, "IP_ADDRESS not specified. Program terminated.\n");
        exit(1);
    }
    if(argc < 3){
        fprintf(stderr, "PORT No. not provided. Program terminated.\n");
        exit(1);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
        error("Error: Unable to open socket.\n");

    server = gethostbyname(argv[1]);
    if(!server)
        fprintf(stderr, "Error: No such host/Host is down.\n");
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //bcopy((char *) server->h_name, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, (struct sockaddr*) &serv_addr, (socklen_t) sizeof(serv_addr)) < 0){
        error("Error: Connection to host has failed.\n");
    }

    printf("Connection Successful...\n");
    
    while(1){
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if(n < 0){
            error("Error: Not able to write.");
        }

        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);

        if(n < 0){
            error("Error: Unable to write.");
        }

        printf("Server: %s\n", buffer);

        int flag = strncmp("BYE", buffer, 3);
        if(flag == 0)
            break;
    }

    close(sockfd);

    return 0;
}