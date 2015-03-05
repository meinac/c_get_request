//
//  main.c
//  YelpParser
//
//  Created by Mehmet Emin İNAÇ on 04/03/15.
//  Copyright (c) 2015 Mehmet Emin İNAÇ. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void exit_with_error(char message[], int status){
    puts(message);
    exit(status);
}

int main(int argc, const char * argv[]) {
    char response[1000000];
    char *request = "GET /istanbul HTTP/1.1\r\n"
    "Host: www.yelp.com\r\n"
    "Connection: close\r\n"
    "\r\n";
    struct hostent *server;
    struct sockaddr_in serveraddr;
    int socketfd;
    
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        exit_with_error("Couldn't create tcp socket", -1);
    } else {
        puts("Tcp socket has been created");
    }
    
    server = gethostbyname("www.yelp.com");
    
    if(server == NULL) {
        exit_with_error("Couldn't get ip of host", -1);
    } else {
        puts("IP of host has been fetched");
    }
    
    bzero((char *) &serveraddr, sizeof(serveraddr));
    bcopy((char *) server->h_addr, (char *) &serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(80);
    
    if(connect(socketfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        exit_with_error("Couldn't connect server", -1);
    } else {
        puts("Connected to server");
    }
    
    if(send(socketfd, request, strlen(request), 0) < 0) {
        exit_with_error("Couldn't send request to the server", -1);
    } else {
        puts("Request has been sent");
    }

    bzero(response, sizeof(response) / sizeof(char));
    recv(socketfd, response, sizeof(response) / sizeof(char) - 1, 0);
    puts(response);
    
    close(socketfd);
    
    return 0;
}
