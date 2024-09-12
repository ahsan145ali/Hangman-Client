#ifndef UTILS_H
#define UTILS_H

// enum to represent the result of the comparison
typedef enum {
    CORRECT_GUESS,    // 0
    INCORRECT_GUESS,  // 1
    ALREADY_GUESSED,  // 2
    ALL_GUESSED,      // 3
    ALL_NOT_GUESSED   // 4    
} GuessStatus;

// enum to represent return status of the function
typedef enum {
    FUNCTION_SUCCESS,    // 0
    FUNCTION_FAILED     // 1
} ReturnStatus;


/// @brief Get string input from user and sanitize it
/// @param prompt text to display to user
/// @return a pointer to a character string
char* stringInput(char prompt[]);

/// @brief Clear any newline character left on buffer after input
void clearInputBuffer(void);

/// @brief get character input from user and sanatize it 
/// @param prompt text to display to user
/// @return a character
char characterInput(char prompt[]);

/// @brief converts string into format that is accepted by the server
/// @param st string to be converted 
/// @return a char pointer to the converted string
char* prepareStringForServer(char *st);

/// @brief checks if the array returned from server is all true
/// @param resultArray array containing 1 and 0
/// @return integer to represent result
int checkAllCorrect(char* resultArray);

/// @brief checks if the current guess is correct
/// @param wordArray array that has the words that is being formed as the user guesses
/// @param resultArray array that is returned from the server containing 1 and 0
/// @param guessedWord the current guess
/// @return an integer to represent result
int checkGuess(char* wordArray , char* resultArray , char guessedWord);

/// @brief adds current guess to the guess list 
/// @param guesslist array that holds all the guesses made by the user
/// @param guessedWord current guess
/// @return int to represent result
int addToGuessList(char **guesslist , char guessedWord);

/// @brief Allocate memory
/// @param ptr pointer to the array to assign memory to
/// @return int to represent result
int allocateMemory(char **ptr);

/// @brief prints hangman representation
/// @param lives current lives of the user
void Hungman(int lives);

/// @brief clear terminal
void clearTerminal();
#endif // UTILS_H
