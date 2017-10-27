/* *********************************************************************************** *
 * Femto is a rudimentary text viewing program capable of searching                    *
 * and word replacement.                                                               *
 *                                                                                     *
 * THE PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED      *
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF                *
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE    *
 * QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE       *
 * DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.    *
 *                                                                                     *
 * main.c                                                                              *
 * Author: Adel Khial                                                                  *
 * *********************************************************************************** */

#include <conio.h>
#include "textstats.h"
#include "display.h"
#include "constants.h"
#include "index.h"
#include "GUI.h"
#include "search.h"

void printCentered(char* message, short cols) {
    int i = 0;

    for(i = 0; i < (cols - strlen(message)) / 2 + 1; ++i) printf(" ");
    printf(message);
    for(i = 0; i < (cols - strlen(message)) / 2; ++i) printf(" ");
}

void printStatus(char* status, FILE* text, short cols, short lines, int currentPage) {
    setCursorPosition(0, lines);
    setCursorColor(BLACK_WHITE);
    setCursorPosition(0, lines);
    sprintf(status, "Page: %d / %d | Characters: %d, Words: %d.", currentPage, pageCount(text, cols, lines)
        , charCount(text), wordCount(text));
    printCentered(status, cols);
    setCursorColor(GRAY_BLACK);
}

int keyPressed() {
    if(kbhit()) {
        int c = getch();
        return c;
    }
    return 0;
}

void printRTag(FILE* file, char* word) {
    fprintf(file, "%s", word);
}

int main() {
    short oldCols = 0, oldLines = 0, alignementStatus = LEFT_ALIGNED, oldStatus = LEFT_ALIGNED;
    int currentWord =  0, currentPage = 1, oldWord = 1;
    long tell = 0, oldTell = 0;
    bool update = true, keyPress = false, exit = false, refresh = false, doReplace = false;
    char* message = malloc(MESSAGE_SIZE * sizeof(char));
    char* status = malloc(MESSAGE_SIZE * sizeof(char));
    char* menuMessage = malloc(MESSAGE_SIZE * sizeof(char));
    char* line = malloc(LINE_SIZE * sizeof(char));
    char* word = malloc(WORD_SIZE * sizeof(char));
    char* wordToReplace = malloc(WORD_SIZE * sizeof(char));
    char* replace = malloc(WORD_SIZE * sizeof(char));
    Word* index = NULL;
    FILE* text = fopen("input.txt", "r");
    FILE* replacedText;

    setConsoleSize(START_WIDTH, START_HEIGHT);
    setBufferSize();
    sprintf(message, "\"input.txt\"");
    index = buildIndex(text);

    for(;;) {
        short cols = getConsoleSize()[0];
        short lines = getConsoleSize()[1];

        if(oldCols == 0) keyPress = true; // For initial print.
        if(cols != oldCols || lines != oldLines) update = true;

        int c = keyPressed();
        switch(c) {
        case 13:
            keyPress = true;
            break;
        case 27:
            exit = true;
            break;
        case 109:
            showMessagePane(index, cols, lines, wordToReplace, replace, &doReplace);
            if(doReplace) {
                replacedText = fopen("replace.txt", "w+");
                rewind(text);

                while(fgets(line, LINE_SIZE, text)) {

                    word = strtok(line, " ");

                    while(word) {
                        if(strncmp(word, "<c>", 3) == 0) {
                            printRTag(replacedText, word);
                            goto skip;
                        } else if(strncmp(word, "</c>", 4) == 0) {
                            printRTag(replacedText, word);
                            goto skip;
                        } else if(strncmp(word, "<j>", 3) == 0) {
                            printRTag(replacedText, word);
                            goto skip;
                        } else if(strncmp(word, "</j>", 4) == 0) {
                            printRTag(replacedText, word);
                            goto skip;
                        } else if(strncmp(word, "<l>", 3) == 0) {
                            printRTag(replacedText, word);
                            goto skip;
                        } else if(strncmp(word, "</l>", 4) == 0) {
                            printRTag(replacedText, word);
                            goto skip;
                        } else if(strncmp(word, "<r>", 3) == 0) {
                            printRTag(replacedText, word);
                            goto skip;
                        } else if(strncmp(word, "</r>", 4) == 0) {
                            printRTag(replacedText, word);
                            goto skip;
                        } else if(strncmp(word, wordToReplace, WORD_SIZE) == 0) {
                            fprintf(replacedText, "%s ", replace);
                            if(isNewlineWord(word)) printf("\n");
                            goto skip;
                        } else {
                            if(isNewlineWord(word)) {
                                fprintf(replacedText, "%s", word);
                            } else {
                                fprintf(replacedText, "%s ", word);
                            }
                        }

                        skip:
                        word = strtok(NULL, " ");
                    }
                }
                fclose(text);
                text = fopen("The Wonderful Journey.temp", "w+");
                rewind(replacedText);
                while(fgets(line, LINE_SIZE, replacedText)) {
                    fprintf(text, line);
                }
                index = buildIndex(text);
            }
            refresh = true;
            break;
        }

        if(exit) {
            system("cls");
            break;
        }
        if(update || keyPress || refresh) {
            setBufferSize();
            setConsoleSize(cols, lines);
            oldCols = cols;
            oldLines = lines;

            system("cls");
            setCursorColor(BLACK_WHITE);
            printCentered(message, cols);
            printf("\n");

            if(tell == 0) currentPage = 0;

            setCursorColor(GRAY_BLACK);
            setCursorPosition(0, 2);
            rewind(text);
            if(keyPress) {
                ++currentPage;
                printStatus(status, text, cols, lines, currentPage);
                setCursorPosition(0, 2);
                oldTell = tell;
                oldWord = currentWord;
                oldStatus = alignementStatus;
                tell = printPage(text, cols, lines, tell, &currentWord, &alignementStatus);
            } else if(refresh) {
                printStatus(status, text, cols, lines, currentPage);
                setCursorPosition(0, 2);
                int save = oldWord;
                short statusSave = oldStatus;
                printPage(text, cols, lines, oldTell, &oldWord, &oldStatus);
                oldWord = save;
                oldStatus = statusSave;
            } else {
                currentPage = 1;
                currentWord = 0;
                tell = 0;
                alignementStatus = LEFT_ALIGNED;
                tell = printPage(text, cols, lines, tell, &currentWord, &alignementStatus);
            }

            printStatus(status, text, cols, lines, currentPage);
            update = false;
            keyPress = false;
            refresh = false;
        }
        Sleep(SLEEP_TIME);
    }
    free(message);
    free(status);
    free(wordToReplace);
    free(replace);
    free(menuMessage);
    free(index);
    free(line);
    free(word);
    fclose(text);
    return 0;
}
