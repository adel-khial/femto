/*
 * Text statistics utility functions.
 * textstats.c implementation of textStats.h
 * Author: Adel Khial
 */

#include "textstats.h"
#include "display.h"
#include "constants.h"

int paragraphCount(FILE* file) {
    int count = 0;

    rewind(file);

    // NOTE: using while(!feof(file)) here is inherently illogical since
    // we cannot act on a file having or lacking further input at a certain
    // time 't', because any file in a concurrent environment may reach the EOF
    // or receive information outside of the current scope, however it is
    // acceptable in this usage since we can effectively guarrantee that the
    // file will not be ACCESSED or CHANGED by any other agent.
    //
    // The correct way is to attempt an I/O operation and consume the result
    // after checking if the operation succeeded.

    // while(!feof(file)) {} <- Wrong
    for(;;) {
        int c = fgetc(file);
        if(c == EOF) break;
        if(c == '\n') ++count;
    }
    return count;
}

int totalCharCount(FILE* file) {
    int count = 0;

    rewind(file);

    for(;;) {
        int c = fgetc(file);
        if(c == EOF) break;
        ++count;
    }
    return count;
}

int charCount(FILE* file) {
    int count = 0, i = 0;
    bool alphaNumeric = false;
    char* regex = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

    rewind(file);

    for(;;) {
        int c = fgetc(file);
        if(c == EOF) break;
        for(i = 0; i < 62; ++i) {
            if(c == regex[i]) {
                alphaNumeric = true;
            }
        }
        if(alphaNumeric) ++count;
    }
    free(regex);
    return count;
}

int wordCount(FILE* file) {
    int count = 0;
    bool inSpace = false;

    // inSpace saves the current state of reading, if we encounter a space
    // we set it to true, then as soon as we find a character we reset
    // the state and increment count.

    rewind(file);

    for(;;) {
        int c = fgetc(file);
        if(c == EOF) break;
        if(c == ' ') inSpace = true;
        if(inSpace && c != ' ') {
            ++count;
            inSpace = false;
        }
    }
    return count;
}

int pageCount(FILE* file, short cols, short lines) {
    int pages = 0, currentCol = 8, currentLine = 2;
    int maxPos = cols / 2 + cols / 3 - 11;
    char* line = malloc(LINE_SIZE * sizeof(char));
    char* word = malloc(WORD_SIZE * sizeof(char));

    rewind(file);
    while(fgets(line, LINE_SIZE, file)) {
        word = strtok(line, " ");

        while(word != NULL) {
            currentCol += strlen(word);
            if(isNewlineWord(word)) {
                ++currentLine;
                currentCol = 8;
            } else if(currentCol > maxPos) {
                ++currentLine;
                currentCol = 4;
            }
            word = strtok(NULL, " ");
            if(currentLine >= lines - 2) {
                pages++;
                currentLine = 2;
                currentCol = 8;
            }
        }
    }

    free(line);
    free(word);
    return pages + 1;
}
