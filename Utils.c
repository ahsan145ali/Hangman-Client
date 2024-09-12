#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Utils.h"

char *stringInput(char prompt[]){
    char temp[100]; // temporary char[] to get input
    printf("%s : " , prompt);

    //handling for max players (no more than 6)
    //mid round joins
    //mid round disconnects
    //multiple rounds
    
        if(fgets(temp,sizeof(temp) , stdin) != NULL){ // get input from user which does not exceed size of temp
            // removing the new line character
            if(strlen(temp) > 0 && temp[strlen(temp) - 1] == '\n'){ // check if input size is more then 0 and last character is a newline character
                temp[strlen(temp) - 1] = '\0'; // replace newline character with the Null character
            }
            // Allocate the memory for result
            char *result = malloc(strlen(temp) +1);
            if(result == NULL){
                printf("Memory allocation failed to result");
                exit(1);
            }
            strcpy(result,temp); // copy input from temp to result
            return result;
            
        }
        else{
            printf("Error reading input\n.");
            printf("Enter Again\n.");
            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }

            return NULL;
         }
    

}
void clearInputBuffer(void)
{

    while(getchar()!='\n');
}
char characterInput(char prompt[])
{
     char temp ;
     printf("%s : " , prompt);
     while(1)
     {
        scanf(" %c",&temp); // Adding space before %c to skip any leading whitespace
        if(isalpha(temp)) 
        {
            
            return temp;

        }else
        {       
            printf("Input Failed\n");
            printf("Enter Again: ");

        }  
     }

}
char* prepareStringForServer(char *st)
{
    char *temp = malloc(strlen(st) + 2 ); // space for size and null character
    if(temp == NULL)
    {
        perror("Memory Allocation Failed");
        return NULL;
    }
    temp[0] = strlen(st); // store size of array at first index

     // Copy the string st into temp starting at index 1 (skipping index 0)
    memcpy(temp + 1, st, strlen(st) + 1); // +1 to include the null terminator

    if(temp == NULL )
    {
        perror("memcpy function failed\n");
        return NULL;
    }
    return temp;

}

int checkAllCorrect(char* resultArray)
{
    for(int i=0 ; i<strlen(resultArray) ; i++)
    {
        if(resultArray[i] == '0')
        {
            return ALL_NOT_GUESSED; 
        }
    }
    return ALL_GUESSED;
}
int checkGuess(char* wordArray , char* resultArray, char guessedWord)
{   
    if(checkAllCorrect(resultArray) == ALL_GUESSED) // All words guessed
    {
        return ALL_GUESSED;
    }
    int wasItCorrectGuess = -1;
    for(int i=0 ; i<strlen(resultArray) ; i++)
    {
        if(resultArray[i] == '1')
        {
            // check if same index in word array is NULL or not which is represented by _
            // if it is not null then that means it was previous guess else it is new
            if(wordArray[i] == '_')
            {
                wordArray[i] = guessedWord; //replace with guessed word
                wasItCorrectGuess = CORRECT_GUESS;
            }
        }
    }
    if(wasItCorrectGuess == CORRECT_GUESS) return CORRECT_GUESS;
    
    return INCORRECT_GUESS; //wrong guess
}

int allocateMemory(char **ptr)
{
    if (*ptr == NULL) // initial empty
    {
        *ptr = malloc(sizeof(char) + 1); // +1 for null character
        if (*ptr == NULL)
        {
            return FUNCTION_FAILED;
        }
        (*ptr)[0] = '\0'; // Initialize with null character
        return FUNCTION_SUCCESS;
    }
    else // initial not empty
    {
        // Reallocate memory with space for one more character and null terminator
        *ptr = realloc(*ptr, (strlen(*ptr) + 2) * sizeof(char)); // +2: +1 for NULL, +1 for new character
        if (*ptr == NULL)
        {
            return FUNCTION_FAILED;
        }
        return FUNCTION_SUCCESS;
    }
}


int addToGuessList(char **guesslist , char guessedWord)
{
    if (allocateMemory(guesslist) == FUNCTION_FAILED)
    {
        perror("Memory Allocation Failed\n");
        exit(EXIT_FAILURE);
    }
    // Find the length of the current string
    size_t len = strlen(*guesslist);
    (*guesslist)[len] = guessedWord; // Add the new character
    (*guesslist)[len + 1] = '\0'; // Null terminate the string
    return FUNCTION_SUCCESS;
}

void Hungman(int lives){
    switch (lives) {
    case 8:
        printf("      \n");
        printf("      \n");
        printf("      \n");
        printf("      \n");
        printf("      \n");
        printf("      \n");
        printf("=========\n");


    break;
    case 7:

        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    case 6:
    
        printf("  +---+\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    case 5:

        printf("  +---+\n");
        printf("  |   |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    
    case 4:

        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf("      |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");

        break;
    case 3:
        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf("  |   |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");
     
        break;
    case 2:
        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf(" /|\\  |\n");
        printf("      |\n");
        printf("      |\n");
        printf("=========\n");
     

        break;
    case 1:
        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf(" /|\\  |\n");
        printf(" /    |\n");
        printf("      |\n");
        printf("=========\n");
     
        break;
    case 0:
        printf("  +---+\n");
        printf("  |   |\n");
        printf("  O   |\n");
        printf(" /|\\  |\n");
        printf(" / \\  |\n");
        printf("      |\n");
        printf("GAME OVER!\n");
        printf("=========\n");

        break;

     
        break;

}
 }

 void clearTerminal(void) {
#ifdef _WIN32
    system("cls");  // For Windows
#else
    system("clear");  // For Linux/macOS
#endif
}


 
