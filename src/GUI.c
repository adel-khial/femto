/*
 * GUI utility functions.
 * GUI.c implementation of GUI.h
 * Author: Adel Khial
 */

#include "GUI.h"
#include "display.h"
#include "constants.h"

void printOptions(int choice) {
    switch(choice % 4) {
    case 0:
        printf("1- SEARCH                         ");
        break;
    case 1:
        printf("2- REPLACE WORD                   ");
        break;
    case 2:
        printf("3- RETURN                         ");
        break;
    case 3:
        printf("4- EXIT                           ");
        break;
    }
}

void removeTrailingNewline(char* string) {
    char *position;
    if((position = strchr(string, '\n')))
        *position = '\0';
}

void showMessagePane(Word* index, short cols, short lines, char* wordToReplace, char* replace, bool* doReplace) {
    int i = 0, up = 223, down = 220, left = 219, right = 219;
    int charNum = 44;
    int xPos = cols / 2 - charNum / 2;
    int choice = 0, c = 0;
    char* query = malloc(WORD_SIZE * sizeof(char));

    start:
        choice = 0;
        c = 0;

    setCursorColor(WHITE_BLACK);

    setCursorPosition(xPos, lines / 2 - 6);
    printf("%s", "                                            ");
    setCursorPosition(xPos, lines / 2 - 5);
    printf("%s", "  ");
    for(i = 0; i < 40; ++i) printf("%c", down);
    printf("%s", "  ");
    for(i = 1; i < 10; ++i) {
        setCursorPosition(xPos, lines / 2 - 5 + i);
        printf("%s", "  ");
        printf("%c", left);
        printf("%s", "                                      ");
        printf("%c", right);
        printf("%s", "  ");
    }
    setCursorPosition(xPos, lines / 2 + 5);
    printf("%s", "  ");
    for(i = 0; i < 40; ++i) printf("%c", up);
    printf("%s", "  ");
    setCursorPosition(xPos, lines / 2 + 6);
    printf("%s", "                                            ");

    setCursorPosition(xPos + 5, lines / 2 - 3);
    printf("WHAT DO YOU WANT TO DO?");

    setCursorColor(BLACK_WHITE);
    setCursorPosition(xPos + 5, lines / 2);
    printf("1- SEARCH                         ");
    setCursorColor(WHITE_BLACK);

    setCursorPosition(xPos + 5, lines / 2 + 1);
    printf("2- REPLACE WORD                   ");

    setCursorPosition(xPos + 5, lines / 2 + 2);
    printf("3- RETURN                         ");

    setCursorPosition(xPos + 5, lines / 2 + 3);
    printf("4- EXIT                           ");

    while(c = getch() != 13) {
        c = getch();
        switch(c) {
        case 'H':
            setCursorColor(WHITE_BLACK);
            setCursorPosition(xPos + 5, lines / 2 + choice % 4);
            printOptions(choice);
            if(choice > 0) --choice;
            setCursorColor(BLACK_WHITE);
            setCursorPosition(xPos + 5, lines / 2 + choice % 4);
            printOptions(choice);
            break;
        case 'P':
            setCursorColor(WHITE_BLACK);
            setCursorPosition(xPos + 5, lines / 2 + choice % 4);
            printOptions(choice);
            if(choice != 3) ++choice;
            setCursorColor(BLACK_WHITE);
            setCursorPosition(xPos + 5, lines / 2 + choice % 4);
            printOptions(choice);
            //setCursorColor(WHITE_BLACK);
            break;
        /*case 'M':
            printf("RIGHT");
            break;
        case 'K':
            printf("LEFT");
            break;*/
        }
        setCursorPosition(0, 0);
    }
    switch(choice) {
    Word* w;
    Word** words;
    case 0:
        setCursorColor(WHITE_BLACK);
        for(i = 5; i < 10; ++i) {
            setCursorPosition(xPos, lines / 2 - 5 + i);
            printf("%s", "  ");
            printf("%c", left);
            printf("%s", "                                      ");
            printf("%c", right);
            printf("%s", "  ");
        }
        setCursorPosition(xPos + 5, lines / 2);
        printf("ENTER QUERY: ");
        fgets(query, WORD_SIZE, stdin);
        removeTrailingNewline(query);
        setCursorPosition(xPos + 5, lines / 2 + 2);
        if(w = findWord(query, index, 4096)) {
            printf("FOUND AT: line %d, word %d",
                   w->positions[0].line, w->positions[0].pos);
            setCursorPosition(xPos + 5, lines / 2 + 3);
            printf("TOTAL NUMBER OF OCCURENCES: %d", w->posCount + 1);
        } else {
            printf("NOT FOUND. DID YOU MEAN?");
            setCursorPosition(xPos + 5, lines / 2 + 3);
            words = closestWords(query, index);
            for(i = 0; i < 3; ++i) printf("%s, ", words[i]->word);
            printf("%s.", words[3]->word);
        }
        getch();
        break;
    case 1:
        setCursorColor(WHITE_BLACK);
        for(i = 5; i < 10; ++i) {
            setCursorPosition(xPos, lines / 2 - 5 + i);
            printf("%s", "  ");
            printf("%c", left);
            printf("%s", "                                      ");
            printf("%c", right);
            printf("%s", "  ");
        }
        setCursorPosition(xPos + 5, lines / 2);
        printf("WORD TO REPLACE: ");
        fgets(wordToReplace, WORD_SIZE, stdin);
        removeTrailingNewline(wordToReplace);
        if(findWord(wordToReplace, index, 4096)) {
            setCursorPosition(xPos + 5, lines / 2);
            printf("REPLACE BY:                       ");
            setCursorPosition(xPos + 17, lines / 2);
            fgets(replace, WORD_SIZE, stdin);
            removeTrailingNewline(replace);
            *doReplace = true;
        } else {
            setCursorPosition(xPos + 5, lines / 2);
            setCursorColor(BLACK_RED);
            printf("WORD NOT FOUND.                   ");
            getch();
            goto start;
        }

        break;
    case 2:
        break;
    case 3:
        setCursorColor(GRAY_BLACK);
        system("cls");
        exit(0);
        break;
    }
    setCursorColor(GRAY_BLACK);
    free(query);
}
