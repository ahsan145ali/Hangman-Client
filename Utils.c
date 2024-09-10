#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Utils.h"

char *stringInput(char prompt[]){
    char temp[100]; // temporary char[] to get input
    printf("%s : " , prompt);
    
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
     scanf(" %c",&temp); // Adding space before %c to skip any leading whitespace
     if(isalpha(temp)) 
     {
        
        printf("SUCCESS");
        return temp;

     }else
     {       
        printf("Input Failed");
         return '\0';

     }

}
char* prepareStringForServer(char *st)
{
    char *temp = malloc(strlen(st) + 2 ); // space for size and null character
    temp[0] = strlen(st); // store size of array at first index

     // Copy the string st into temp starting at index 1 (skipping index 0)
    memcpy(temp + 1, st, strlen(st) + 1); // +1 to include the null terminator

    return temp;

}
