#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>  // For isalpha function

#include "Utils.h"

#define PORT 4159
#define IPADDRESS "127.0.0.1"

 int client_FileDescriptor = -1;


int main(void)
{
    struct sockaddr_in server_address;
    char *name = NULL; // holds the name of the user
    char guess = '\0'; // holds the character that was guessed by the user
    char *guessList = NULL; // holds all the characters that were guessed by the user
    // //create socket
    // if((client_FileDescriptor = socket( AF_INET , SOCK_STREAM , 0)) == -1)
    // {
    //     perror("Socket creation failed\n");
    //     exit(EXIT_FAILURE);
    // }
    //  printf("Socket Created\n");

    //  //setting socket structure
    // server_address.sin_family = AF_INET;
    // server_address.sin_port = htons(PORT);
    // inet_pton(AF_INET,IPADDRESS,&server_address.sin_addr);

    // socklen_t len  = sizeof(server_address);
    // //connect to server  
    // if((connect(client_FileDescriptor , (struct sockaddr*)&server_address , len)) == -1)
    // {
    //     perror("Connection Failed\n");
    //     exit(EXIT_FAILURE);
    // }
    // printf("Connected to Server\n");
    //name = stringInput("Enter Nickname");
    //printf("Entered Name: %s\n" , name);

    guess = characterInput("Enter a Letter");
    clearInputBuffer();
    char ess = characterInput("Enter2 a Letter");
    printf("Entered character: %c %c" , guess , ess);
    return EXIT_SUCCESS;
}