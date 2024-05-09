//
//  main.c
//  webserver
//
//  Created by Victor Padron on 17/04/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "8080"

#define BACKLOG 10


int main(int argc, const char * argv[]) {
    // hints can be used to provide hints concerning the type of socket that the caller supports or wishes to use.
    // check 'man getaddrinfo' for more info on this parameter
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);    // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;        // we don't care if it's either ipv4 or ipv6
    hints.ai_socktype = SOCK_STREAM;    // since we are using TCP
    hints.ai_flags = AI_PASSIVE;        // we are providing a NULL pointer to the hostname
                                        // if this is set then it indicates it is intended for use in a call to bind(2)
    
    struct addrinfo *servinfo;          // will store the result of getaddrinfo
    int status = getaddrinfo(NULL, PORT, &hints, &servinfo);
    
    if (status != 0) {
        fprintf(stderr, "getaddrinfo call failed with status: %s\n", gai_strerror(status)); // gai_strerror returns the error that corresponds to the returned status
    }
    
    
    int sockfd = -1;     // socket file descriptor
    // iterate through the linked list until we successfully bind
    struct addrinfo* pointer;
    for (pointer = servinfo; pointer != NULL; pointer = pointer->ai_next) {
        
        sockfd = socket(pointer->ai_family, pointer->ai_socktype, pointer->ai_protocol);
        if (sockfd == -1) {
            perror("server: socket");
            continue;
        }
        
        int bindResult = bind(sockfd, pointer->ai_addr, pointer->ai_addrlen);
        if (bindResult == -1) {
            perror("bind error");
            continue;
        }
        
        int reuseAddress = 1;
        // reuse port and prevent the "Address already in use" error message
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof reuseAddress) == -1) {
            perror("error while setting socket options");
            exit(1);
        }
        
        break;
    }
    
    freeaddrinfo(servinfo);
    
    if (pointer == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }
    
    // listen on given address, backlog is the number of connections allowed on the incoming queue
    int listenResult = listen(sockfd, BACKLOG);
    if (listenResult == -1) {
        perror("error while listening");
        exit(1);
    }
    
    // accept and handle a connection
    struct sockaddr_storage incomingAddr;
    socklen_t addrLen = sizeof incomingAddr;
    int connsockfd = accept(sockfd, (struct sockaddr *)&incomingAddr, &addrLen);
    if (connsockfd == -1) {
        perror("could not accept connection");
        exit(1);
    }
    
    char* response = "HTTP/1.1 200 OK\r\nContent-Type: text\r\n\r\nHello World\n";
    ssize_t resLen = strlen(response);
    ssize_t bytes_sent = send(connsockfd, response, resLen, 0);
    
    close(connsockfd);
    close(sockfd);
}
