//
//  client.cpp
//  TicTacToe
//
//  Created by Josh Fishman on 11/4/18.
//  Copyright © 2018 Josh Fishman. All rights reserved.
//
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "const.h"

using namespace std;

//This function initializes the vector with just "_ "'s.
//vector<vector<char *>> initialize()
char * initialize()
{
    char underscore = '_';
    char *board = (char *) malloc(sizeof(char) * DIM);
    for (int i = 0; i < DIM; i++) {
        board[i] = underscore;
    }
    return board;
}

//This function takes in the character array argument. Pretty self-explanatory. It prints out the array.
void print_arr(char mat[DIM])
{
    for (int row = 0; row < DIM; row++)
    {
        
        printf("%c ", mat[row]);
        if(row % 3 == 2)
        {
            printf("\n");
        }
    }
}

/*This function takes in the character array argument and the row and column that we will be checking. If the element is a "_ " then this element is open to be played.
 */
bool check_move(char mat[DIM], int row)
{
    bool flag = false;
    if (mat[row] == '_')
    {
        flag = true;
    }
    return flag;
}

//This function takes in the character array argument. It checks if there are any more available moves left.
bool more_moves(char mat[DIM])
{
    bool flag = false;
    for(int row =0; row < DIM;row++)
    {
        
        if (mat[row] == '_')
            flag = true;
        
    }
    return flag;
}

/*This function takes in the character array argument. This is the logic for the end of a tic tac toe game. Either a player got 3 elements in a row or there are no more moves remaining.
 */
bool gameover(char mat[DIM])
{
    bool flag = false;
    if(!more_moves(mat))
    {
        flag = true;
    }
    else if (mat[0] == mat[1] && mat[1] == mat[2] && mat[0] != '_')
    {
        flag = true;
    }
    else if (mat[3] == mat[4] && mat[4] == mat[5] && mat[3] != '_')
    {
        flag = true;
    }
    else if (mat[6] == mat[7] && mat[7] == mat[8] && mat[6] != '_')
    {
        flag = true;
    }
    else if (mat[0] == mat[3] && mat[3] == mat[6] && mat[0] != '_')
    {
        flag = true;
    }
    else if (mat[1] == mat[4] && mat[4] == mat[7] && mat[1] != '_')
    {
        flag = true;
    }
    else if (mat[2] == mat[5] && mat[5] == mat[8] && mat[2] != '_')
    {
        flag = true;
    }
    else if (mat[0] == mat[4] && mat[4] == mat[8] && mat[0] != '_')
    {
        flag = true;
    }
    else if (mat[2] == mat[4] && mat[4] == mat[6] && mat[2] != '_')
    {
        flag = true;
    }
    return flag;
}

int main(int argc, char const *argv[])
{
    //initialize variables I will be using later.
    int new_socket;
    int port = atoi(argv[1]);
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(port);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    int row,serv_row;
    char * tictactoe = initialize();
    //This will loop for each move the player makes and it isn't gameover
    while(!gameover((tictactoe)))
    {
        print_arr(tictactoe);
        //Will loop until user enters valid move
        do {
            printf("Make move (0-8). ");
            scanf("%d", &row);
            
        } while (!check_move(tictactoe, row));
        //Places move on board
        tictactoe[row] = 'X';
        //If user's move results in a gameover then break out of the loop.
        if(gameover(tictactoe))
        {
            break;
        } 
        int x = 0;
        //Sends a message so the server will send a message. 
        //Reads the message from the server
        //Loop until the move is valid.
        do{
            write(sock,"",sizeof(""));
            read(sock, &x, sizeof(int));
            serv_row = ntohs(x);
        } while (!check_move(tictactoe,serv_row));
        //Have the server's move made.
        tictactoe[serv_row] = 'O';
    }
    //Show the resulting tic tac toe board. Then print gameover. Send a 9 so the server can start the process over.
    print_arr(tictactoe);
    printf("GAME OVER");
    write(new_socket , "9" , sizeof(int));
    sleep(5);
    return 0;
}


