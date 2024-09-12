#ifndef UTILS_H
#define UTILS_H

typedef enum {
    CORRECT_GUESS,    // 0
    INCORRECT_GUESS,  // 1
    ALREADY_GUESSED,  // 2
    ALL_GUESSED,      // 3
    ALL_NOT_GUESSED   // 4    
} GuessStatus;

typedef enum {
    FUNCTION_SUCCESS,    // 0
    FUNCTION_FAILED     // 1
} ReturnStatus;


char* stringInput(char prompt[]);
void clearInputBuffer(void);
char characterInput(char prompt[]);
char* prepareStringForServer(char *st);
int checkAllCorrect(char* resultArray);
int checkGuess(char* wordArray , char* resultArray , char guessedWord);
int addToGuessList(char **guesslist , char guessedWord);
int allocateMemory(char **ptr);
void Hungman(int lives);
#endif // UTILS_H
