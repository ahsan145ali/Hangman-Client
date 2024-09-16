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

#define PORT 8080 // port of the server
#define IPADDRESS "127.0.0.1" // IP address

int client_FileDescriptor = -1;
unsigned short int playerLives = 8;


int main(void)
{
    struct sockaddr_in server_address; // hols the address of the server
    char *name = NULL; // holds the name of the user
    char guess = '\0'; // holds the character that was guessed by the user
    char *guessList = NULL; // holds all the characters that were guessed by the user
    char *word = NULL; // this hold the word that needs to be guessed which is filled as user makes correct choices
    int wordSize = - 1; // this holds the size of the word which needs to be guessed which is sent by the server
    ssize_t res; // stores result bytes of send or recv from server 
    char *buffer = NULL; // stores received result from server , this is declared when server send length of word

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
    inet_pton(AF_INET, IPADDRESS, &server_address.sin_addr);

    socklen_t len = sizeof(server_address);
    
    //connect to server  
    if((connect(client_FileDescriptor , (struct sockaddr*)&server_address , len)) == -1)
    {
        perror("Connection Failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected to Server\n");

    name = stringInput("Enter Nickname"); // get nickname from the user
    printf("Entered Name: %s\n" , name);

    // send name to server
    char *nameforServer= prepareStringForServer(name); // convert name entered to the format expected by the server
    if(nameforServer!= NULL)
    {
        res = send(client_FileDescriptor, nameforServer , strlen(nameforServer), 0); 
        if(res == -1)
        {
            perror("sending to server failed\n");
        }
        else
        {
            printf("Name %s sent to server\n" , nameforServer +1);
        }
    }

    while (1) // Loop for multiple rounds
    {
        //get word size from server
        res = recv(client_FileDescriptor , &wordSize, sizeof(wordSize), 0 ); // the first recv from server sends an integer which has size of the word to guess
        if(res == -1)
        {
            perror("Receive Failed\n");
            close(client_FileDescriptor);
            exit(EXIT_FAILURE);
        }
        else if(res == 0)
        {
            printf("Connection Closed");
            close(client_FileDescriptor);
            exit(EXIT_SUCCESS);
        }
        else
        {
            //create array with the size received and create buffer with the same size
            word = malloc((wordSize + 1 ) * sizeof(char)); // +1 for NULL character
            buffer = malloc((wordSize + 1) * sizeof(char)); // +1 for NULL character
            if(word == NULL || buffer == NULL)
            {
                perror("Memory Allocation Failed\n");
                close(client_FileDescriptor);
                return EXIT_FAILURE;
            }
            //set all elements to '\0'
            for (int i = 0; i <= wordSize; i++) {
                word[i] = '_'; // set it underscore to simulate not guessed word
                buffer[i] = '\0'; // set to null as it received by the server
            }
            word[wordSize] = '\0'; // set last index to null
        }

        // Play one round
        while(1)
        {
            guess = characterInput("Enter a Letter"); // get guess character input from user
            clearInputBuffer(); // clear newline left in buffer
            
            //send guessed word to server
            res = send(client_FileDescriptor , &guess , sizeof(char), 0);
            if(res == -1)
            {
                perror("sending to server failed\n");
            }
            else
            {
                printf("Letter %c sent to server\n" , guess);
                
                // after it is successfully sent, wait for server to respond with result
                res = recv(client_FileDescriptor , buffer , wordSize , 0);
                if(res == -1)
                {
                    perror("Error on Receive:\n");
                }
                else if(res == 0 )
                {
                    printf("Connection Closed\n");
                    close(client_FileDescriptor);
                    break;
                }
                else
                {
                    // if receive is success, would receive a boolean char array in buffer here
                    addToGuessList(&guessList, guess);
                    int result = checkGuess(word , buffer, guess); // check if the guessed character was right or wrong
                    if(result == ALL_GUESSED)
                    {
                        // send lives to server
                        // break out of loop, and prepare to receive LeaderBoard
                        clearTerminal();
                        Hungman(playerLives);
                        printf("\nPoints earned: %d\n\n", playerLives + wordSize);
                        break;
                    }
                    else if(result == INCORRECT_GUESS)
                    {
                        printf("Incorrect Guess \n");
                        playerLives = playerLives - 1; // Decrement lives on wrong guess
                        printf("Lives Remaining: %d \n" , playerLives);
                        clearTerminal();
                        Hungman(playerLives);
                        if(playerLives == 0)
                        {
                            printf("\nPoints earned: 0\n\n");
                            break; // lost, exit loop as no more guess input is needed
                        }
                    }
                    
                    clearTerminal();
                    Hungman(playerLives);
                    printf("guessed letters: %s \n" , guessList);
                    printf("Lives Remaining: %d \n" , playerLives);
                    printf("Word: %s \n" , word);
                }
            }
        }

        // Prepare to receive Leaderboard
        unsigned char sizeLeaderboard = '\0'; // stores the size of leaderboard
        char *LeaderBoard = NULL; // Stores the leaderboard
        res = recv(client_FileDescriptor, &sizeLeaderboard, sizeof(sizeLeaderboard), 0);
        if (res == -1) {
            perror("Error on Receive");
            close(client_FileDescriptor);
            exit(EXIT_FAILURE);
        } else if (res == 0) {
            printf("Connection Closed");
            close(client_FileDescriptor);
            exit(EXIT_SUCCESS);
        }
        else
        {
            //create array according to size of leaderboard
            LeaderBoard = malloc(sizeLeaderboard + 1); // +1 for null character
            LeaderBoard[sizeLeaderboard] = '\0'; // set last index to null
            
            // get the leaderboard
            res = recv(client_FileDescriptor , LeaderBoard , sizeLeaderboard, 0 );
            if(res == -1)
            {
                perror("Error on Receive");
                close(client_FileDescriptor);
                exit(EXIT_FAILURE);
            }
            else if( res == 0)
            {
                perror("Connection Closed , Could not get Leaderboard, Error: \n");
                close(client_FileDescriptor);
                exit(EXIT_SUCCESS);
            } 
            else
            {
                printf("------ LEADERBOARD --------- \n \n");
                printf("%s \n" , LeaderBoard);
            } 
        }

        // Ready up for the next round
        char ready = '\0';
        do {
            printf("Press 'R' to ready up for the next round: ");
            scanf(" %c", &ready);
            ready = toupper(ready); // Convert to uppercase in case user enters lowercase 'r'
        } while (ready != 'R');

        // Send ready signal to server
        res = send(client_FileDescriptor, &ready, sizeof(ready), 0);
        if (res == -1) {
            perror("Failed to send ready signal to server\n");
            close(client_FileDescriptor);
            exit(EXIT_FAILURE);
        }
    }

    close(client_FileDescriptor);
    return EXIT_SUCCESS;
}
