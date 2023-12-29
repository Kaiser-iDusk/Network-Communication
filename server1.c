#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char* msg){
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "PORT No. not provided. Program terminated.\n");
        exit(1);
    }
    
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error: Unable to open socket.\n");
        exit(1);
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("Error: Unable to bind to socket.");
    }

    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &cli_len);
    if(newsockfd < 0){
        error("Error: Failed to accept connection.\n");
    }

    printf("New connection established with client.\n");
    
    while(1){
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);

        if(n < 0)
            error("Error: Could not read from Client.\n");
        
        printf("Client: %s\n", buffer);

        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(newsockfd, buffer, strlen(buffer));

        if(n < 0)
            error("Error: Unable to write to Client.\n");
        
        int flag = strncmp("BYE", buffer, 3);
        if(flag == 0)
            break;
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}