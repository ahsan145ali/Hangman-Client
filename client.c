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

#define PORT 8080
#define IPADDRESS "127.0.0.1"

int client_FileDescriptor = -1;
unsigned short int playerLives = 8;

void tobedeletedlater(char , char*);
int main(void)
{
    struct sockaddr_in server_address;
    char *name = NULL; // holds the name of the user
    char guess = '\0'; // holds the character that was guessed by the user
    char *guessList = NULL; // holds all the characters that were guessed by the user
    char *word = NULL; // this hold the word that needs to be guessed which is filled as user makes correct choices
    int wordSize = - 1; // this holds the size of the word which needs to be guessed which is sent by the server
    ssize_t res; // stores result bytes of send or recv from server 
    char *buffer = NULL; // stores received result from server , this is declared when server send lenght of word
    //create socket
    if((client_FileDescriptor = socket( AF_INET , SOCK_STREAM , 0)) == -1)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
     printf("Socket Created\n");

     //setting socket structure
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET,IPADDRESS,&server_address.sin_addr);

    socklen_t len  = sizeof(server_address);
    //connect to server  
    if((connect(client_FileDescriptor , (struct sockaddr*)&server_address , len)) == -1)
    {
        perror("Connection Failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected to Server\n");
    name = stringInput("Enter Nickname");
    printf("Entered Name: %s\n" , name);

    // send name to server
    char *nameforServer= prepareStringForServer(name);
    if(nameforServer!= NULL)
    {
        res = send(client_FileDescriptor, nameforServer , strlen(nameforServer),0); 
        if(res == -1)
            {
                perror("sending to server failed\n");
                
            }
            else
            {
                printf("Name %s sent to server\n" , nameforServer +1);
            }
    }
    //get word size from server
    res = recv(client_FileDescriptor , &wordSize,sizeof(wordSize),0 ); // the first recv from server sends a integer which has size of the word to guess
    if(res == -1)
    {
        perror("Receive Failed\n");
          exit(EXIT_FAILURE);
    }
    else if(res == 0)
    {
        printf("Connection Closed");
        exit(EXIT_SUCCESS);
    }
    else
    {
        //create array with the size received and create buffer with the same size
          word = malloc((wordSize + 1 ) * sizeof(char)); // +1 for NULL character
          buffer = malloc((wordSize + 1) * sizeof(char));
        if(word == NULL && buffer == NULL)
        {
            perror("Memory Allocation Failed\n");
            return EXIT_FAILURE;
        }
        //set all elements to '\0'
        for (int i = 0; i <= wordSize; i++) {
            word[i] = '_'; // set it underscore to simulate not guessed word
            buffer[i] = '\0'; // set to null as it received by the server
         }
       word[wordSize] = '\0'; // set last index to null
    }
     while(1)
     {
         guess = characterInput("Enter a Letter");
         clearInputBuffer();
         //send guessed word to server
         res = send(client_FileDescriptor , &guess , sizeof(char),0);
         if(res == -1)
         {
             perror("sending to server failed\n");
         }
         else{
            printf("Letter %c sent to server\n" , guess);
            // after it is successfully sent, wait for server to respond with result
            res = recv(client_FileDescriptor , buffer , strlen(buffer) , 0);
            if(res == -1)
            {
                perror("Error on Receive:\n");
              
            }
            else if(res == 0)
            {
                printf("Connection Closed\n");
                break;
            }
           else{
                // if receive is success , would receive a bolean char array in buffer here
                //store in guess list
                addToGuessList(&guessList,guess);
                int result = checkGuess(word , buffer,guess);
                if(result == ALL_GUESSED)
                {
                    // send lives to server
                    // break out of loop , and prepare to receive LeaderBoard
                    printf("All Guessed\n");
                    break;
                }
                else if(result == INCORRECT_GUESS)
                {
                    printf("Incorrect Guess \n");
                    playerLives = playerLives - 1 ;
                    if(playerLives == 0) break;// lost , exit loop as no more guess input is needed
                }

            }
         }
            // Prepare to receive Leader Bord
            printf("\nRemaining Lives: %d\n" , playerLives);
      }
     return EXIT_SUCCESS;
}
