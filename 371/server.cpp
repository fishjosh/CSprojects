//
//  server.cpp
//  TicTacToe
//
//  Created by Josh Fishman on 11/4/18.
//  Copyright © 2018 Josh Fishman. All rights reserved.
//

// NOTE: To compile, run this command in the terminal window: `gcc -o server server.cpp`, then use `./server` to run.

#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "const.h"
#include <time.h>

using namespace std;

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    while(1)
    {
        //initiialize the variables that I'll use later
        const int SIZE = 9;
        uint32_t range;
        int port = 8080; // *argv[2];
        printf("Port = %d\n", port);
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};
        
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
    
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        bind(server_fd, (struct sockaddr *)&address, sizeof(address));
        
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        //Server reads from the client. Generates random value for a move. 
        //Then sends it to the server. This will loop until the client sends a 9
        do {
            read( new_socket , buffer, sizeof(int));
            range = rand() % SIZE;
            range = htons(range);
            write(new_socket , &range , sizeof(int));
        } while(strncmp(buffer, "9",sizeof("9")) != 0);
        //My do-while loop should exit when it receives a 9 then start the process over indefinitely. On my machine, 
        //this hasn't been the case but I can't figure out the logic of why?
    }
    return 0;
}
