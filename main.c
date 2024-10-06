/*-------------------------------------------
A Dynamically Evil Word Guessing Game
------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Pattern_struct {
    char* pat;  //string pattern - exs: "hello", "-ell-", "-----"
    int count;  //frequency of pattern in the current word list
    int changes;//number of occurences of new letter in this pattern
} Pattern;


void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {
     // Check if the current number of words exceeds the capacity
    if (*numWords >= *maxWords) {
        // Double the capacity
        *maxWords *= 2;
        *words = (char**)realloc(*words, (*maxWords) * sizeof(char*));
    }

    // Allocate memory for the new word and copy the characters
    (*words)[*numWords] = (char*)malloc((strlen(newWord) + 1) * sizeof(char));
    strcpy((*words)[*numWords], newWord);
    (*numWords)++;

    //free((*words)[*numWords]);
}

// Function to count character differences between two strings
int strNumMods(char* str1, char* str2) {
    int count = 0;  // Initialize the count of differences

    // Loop until the end of either string is reached
    while (*str1 != '\0' || *str2 != '\0') {
        // Check if characters at the current position differ
        if (*str1 != *str2) {
            count++;  // Increment the count of differences
        }

        // Move to the next character in the first string if not at its end
        if (*str1 != '\0') {
            str1++;
        }

        // Move to the next character in the second string if not at its end
        if (*str2 != '\0') {
            str2++;
        }
    }

    return count;  // Return the count of differences
}


int strDiffInd(char* str1, char* str2) {
    int index = 0;

    // Compare characters until a difference is found or both strings end
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return index;
        }      
        // Move to the next characters in the strings
        str1++;
        str2++;
        index++;
    }

    // If both strings are identical, return the length of either string
    return index; //modify this
}

// Function to count occurrences of a specific character in a string
int countOccurrences(char* str, char ch) {
    int count = 0;  // Initialize the count of occurrences to 0

    // Iterate through the characters of the string until the end
    while (*str != '\0') {
        // Check if the current character matches the target character
        if (*str == ch) {
            count++;  // Increment the count of occurrences
        }
        str++;  // Move to the next character in the string
    }

    return count;  // Return the total count of occurrences
}


int main(int argc, char* argv[]) {

    printf("Welcome to the (Evil) Word Guessing Game!\n\n");
    
    bool solved = false;
    int wordSize = 5;  // Default word size
    int numGuesses = 26;  // Default number of guesses
    bool statsMode = false;  // Flag for stats mode
    bool wordListMode = false;  // Flag for word list mode
    bool letterListMode = false;  // Flag for letter list mode
    bool patternListMode = false;  // Flag for pattern list mode

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            int size = atoi(argv[++i]);
            if (size < 2 || size > 29) {
                printf("Invalid word size.\nTerminating program...\n");
                return 1;
            }
            wordSize = size;  // Set word size based on command-line argument
        } else if (strcmp(argv[i], "-g") == 0 && i + 1 < argc) {
            int guesses = atoi(argv[++i]);
            if (guesses <= 0) {
                printf("Invalid number of guesses.\nTerminating program...\n");
                return 1;
            }
            numGuesses = guesses;  // Set number of guesses based on command-line argument
        } else if (strcmp(argv[i], "-s") == 0) {
            statsMode = true;  // Enable stats mode
        } else if (strcmp(argv[i], "-w") == 0) {
            wordListMode = true;  // Enable word list mode
        } else if (strcmp(argv[i], "-l") == 0) {
            letterListMode = true;  // Enable letter list mode
        } else if (strcmp(argv[i], "-p") == 0) {
            patternListMode = true;  // Enable pattern list mode
        } else if (strcmp(argv[i], "-v") == 0) {
            // Enable all modes (stats, word list, letter list, pattern list)
            statsMode = true;
            wordListMode = true;
            letterListMode = true;
            patternListMode = true;
        } else {
            printf("Invalid command-line argument.\nTerminating program...\n");
            return 1;
        }
    }

    // Print game settings
    printf("Game Settings:\n");
    printf("  Word Size = %d\n", wordSize);
    printf("  Number of Guesses = %d\n", numGuesses);
    if (statsMode) {
        printf("  View Stats Mode = ON\n");
    }
    else {
        printf("  View Stats Mode = OFF\n");
    }
    if (wordListMode) {
        printf("  View Word List Mode = ON\n");
    }
    else {
        printf("  View Word List Mode = OFF\n");
    }
    if (letterListMode) {
        printf("  View Letter List Mode = ON\n");
    }
    else {
        printf("  View Letter List Mode = OFF\n");
    }
    if (patternListMode) {
        printf("  View Pattern List Mode = ON\n");
    }
    else {
        printf("  View Pattern List Mode = OFF\n");
    }

    FILE* file = fopen("dictionary.txt", "r");  // Open the dictionary file in read mode

    int totalWords = 0;  // Initialize the total word count
    int longestWordLength = 0;  // Initialize the length of the longest word
    char longestWordGlobal[100] = "";  // Initialize the buffer to store the longest word
    int wordsOfCorrectLength = 0;  // Initialize the count of words with the correct length
    int capacity = 4;  // Initial capacity for the word list array

    char** wordList = (char**)malloc(capacity * sizeof(char*));  // Allocate memory for word list
    char buffer[100];  // Initialize a buffer to read words from the file

    // Read words from the file and process them
    while (fscanf(file, "%s", buffer) == 1) {
        totalWords++;  // Increment the total word count
        int wordLength = strlen(buffer);  // Get the length of the current word

        // Update the longest word information if needed
        if (wordLength > longestWordLength) {
            longestWordLength = wordLength;
            strcpy(longestWordGlobal, buffer);
        }

        // Check if the word has the correct length
        if (wordLength == wordSize) {
            // Add the word to the word list
            addWord(&wordList, &wordsOfCorrectLength, &capacity, buffer);
        }
    }

    fclose(file);  // Close the file after reading


    if (statsMode) {
        // Display statistics if statsMode is enabled
        printf("The dictionary contains %d words total.\n", totalWords);
        printf("The longest word %s has %d chars.\n", longestWordGlobal, longestWordLength);
        printf("The dictionary contains %d words of length %d.\n", wordsOfCorrectLength, wordSize);
        printf("Max size of the dynamic words array is %d.\n", capacity);
    }

    if (wordListMode) {
        // Display the word list if wordListMode is enabled

        if (wordsOfCorrectLength == 0) {
            // If there are no words of the specified length, display a message and terminate
            printf("Words of length %d:\n", wordSize);
            printf("Dictionary has no words of length %d.\nTerminating program...\n", wordSize);
            return 0;
        } else {
            // Display words of the specified length
            printf("Words of length %d:\n", wordSize);
            for (int i = 0; i < wordsOfCorrectLength; i++) {
                printf("  %s\n", wordList[i]);  // Print each word of the correct length
            }
            printf("\n");
        }
    }

    char guessedLetters[numGuesses];  // store guessed letters
    int numGuessedLetters = 0;
    int remainingGuesses = numGuesses;
    int correctWordIndex = rand() % (wordsOfCorrectLength + 1);
    char* correctWord = wordList[correctWordIndex];
    int wordLength = wordSize;

    Pattern* patterns = (Pattern*)malloc(totalWords * sizeof(Pattern));
    
    char guess;
    char solvedWord[wordLength + 1];
    int bestPatternIndex = -1;
    int bestPatternChanges = wordLength + 1;
    int diffIndex, i;
    int guessCorrectly = 0;
    int letterCount = 0;
    int wordsRemaining = 0;

    
    for (i = 0; i < wordsOfCorrectLength; i++) {
        (patterns + i)->pat = (char*)malloc((wordLength + 1) * sizeof(char));
    }

    for (i = 0; i < wordLength; i++) {         
        (patterns)->pat[i] = '-';
    }
    printf("The word pattern is: %s \n\n", (patterns)->pat);  // Print the pattern character

    (patterns)->pat[wordLength] = 0x00;
    (patterns)->count = 0;
    (patterns)->changes = 0;


    //printf("Number of guesses remaining: %d\n", remainingGuesses);
    while (remainingGuesses > 0 && !solved) {
        //if(!isalpha(guess) || countOccurrences(guessedLetters, guess) > 0) {
            printf("Number of guesses remaining: %d\n", remainingGuesses);
        //}
        
        if(letterListMode) {
        printf("Previously guessed letters: ");
            for (i = 0; i < numGuessedLetters; i++) {
                printf("%c ", guessedLetters[i]);
            }
            printf("\n");
        }

        printf("Guess a letter (# to end game): ");
        if(guess == '#'){
            break;
        }

        for (int i = 0; i < wordsOfCorrectLength; i++) {
            wordsRemaining++;
        }

        scanf(" %c", &guess);
        guess = tolower(guess);

        if (patternListMode) {
            printf("\nPatterns:\n");
            for (i = 0; i < wordLength; i++) {
                printf("  %s\n", patterns->pat);
            }
            printf("\n");
        }

        if (!isalpha(guess)) {
            printf("Invalid letter...");
        } 
        else if (countOccurrences(guessedLetters, guess) > 0) {
            printf("Letter previously guessed...");
        }
        else  {
            guessedLetters[numGuessedLetters] = guess;
            numGuessedLetters++;
            
            for (i = 0; i < wordLength; i++) {
                if (correctWord[i] == guess) {
                    letterCount++;

                    diffIndex = strDiffInd(patterns->pat, correctWord);
                    
                    guessCorrectly++;

                    patterns->pat[i] = guess;

                     if (strNumMods(patterns->pat, correctWord) < bestPatternChanges) {
                        bestPatternChanges = strNumMods(patterns->pat, correctWord);
                        bestPatternIndex = i;
                    }
                }
            }
            
            if (letterCount > 0) {
                printf("\nGood guess! This word has at least one %c.\n", guess);
                if(statsMode) {
                    printf("Number of possible words remaining: %d\n", wordsRemaining);
                }
                printf("The word pattern is: %s\n", patterns->pat); 
                letterCount = 0;
            }
            else {
                printf("\nOops, there are no %c's. You used a guess.\n", guess);
                if(statsMode) {
                    printf("Number of possible words remaining: %d\n", wordsRemaining);
                }
                
                remainingGuesses--;
                printf("The word pattern is: %s\n", patterns->pat);
            }

            for (i = 0; i < wordLength; i++) {
                patterns->count = countOccurrences(wordList[i], patterns->pat[i]);
                patterns->changes = strNumMods(patterns->pat, correctWord);
            }

             if (patternListMode) {
                 printf("Patterns:\n");
                 for (i = 0; i < wordLength; i++) {
                     printf("  %s ⇥count = %d ⇥changes = %d\n", patterns->pat, patterns->count, patterns->changes);
                 }
                 printf("\n");
            }



            if (bestPatternIndex != -1 && (patterns + bestPatternIndex)->changes == 0) {
                for (i = 0; i < wordLength; i++) {
                    if (strNumMods(patterns->pat, correctWord) < patterns->changes) {
                        patterns->count = countOccurrences(wordList[i], patterns->pat[i]);
                        patterns->changes = strNumMods(patterns->pat, correctWord);
                   }
                }

                if (patternListMode) {
                    printf("All patterns for the letter %c:\n", guess);
                    for (i = 0; i < wordLength; i++) {
                        printf("  %s ⇥count = %d ⇥changes = %d\n", patterns->pat, patterns->count, patterns->changes);
                    }
                    printf("\n");
                }
            }

            if(guessCorrectly == wordLength) {
                    printf("\nYou solved the word!\nThe word is %s\nGame over.\n\n", correctWord);
                    solved = true;
                }
        } 

        printf("\n");
    }

    if (!solved) {
        printf("You ran out of guesses! The correct word was: %s\n", correctWord);
    }    
    
    // Free dynamically allocated memory for the word list
    for (int i = 0; i < wordsOfCorrectLength; i++) {
        free(wordList[i]);
    }
    free(wordList);

    // Free dynamically allocated memory for patterns
    // for (int i = 0; i < totalWords; i++) {
    //     free((patterns + i)->pat);
    // }
    // free(patterns);

  
    
    return 0;
}
