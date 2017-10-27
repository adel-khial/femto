/*
 * Display utility functions.
 * display.c implementation of display.h
 * Author: Adel Khial
 */

#include "display.h"
#include "constants.h"

void setCursorPosition(short x, short y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};
    SetConsoleCursorPosition(handle, position);
}
short* getConsoleSize() {
    short size[2];

    CONSOLE_SCREEN_BUFFER_INFO info;
    short cols, lines;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    cols = info.srWindow.Right - info.srWindow.Left;
    lines = info.srWindow.Bottom - info.srWindow.Top;

    size[0] = cols;
    size[1] = lines;

    return size;
}

bool setConsoleSize(short cols, short lines) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, cols, lines};
    return SetConsoleWindowInfo(handle, true, &windowSize);
}

void setCursorColor(unsigned char color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

bool setBufferSize() {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);
    COORD new_size =
    {
        info.srWindow.Right - info.srWindow.Left + 1,
        info.srWindow.Bottom - info.srWindow.Top + 1
    };
    return SetConsoleScreenBufferSize(handle, new_size);
}

bool isNewlineWord(char* string) {
    char *position;
    if((position = strchr(string, '\n')))
        return true;
    return false;
}

int wordNum(char* string) {
    int count = 0;

    const char* line = string;
    bool inWord = false;

    do switch(*line) {
        case '\0':
        case ' ':
            if(inWord) { inWord = false; ++count; }
            break;
        default: inWord = true;
    } while(*line++);

    return count;
}

void printFile(FILE* file, short* status, short cols, short lines) {
    char* line = malloc(MESSAGE_SIZE * sizeof(char));
    char* word = malloc(WORD_SIZE * sizeof(char));
    int i = 0, numOfSpaces = 0, intSpaces = 1;
    float spacesPerWord = 0, addedSpaces = 0;

    while(fgets(line, MESSAGE_SIZE, file)) {
        if(strncmp(line, "<c>", 3) == 0) {
            *status = CENTERED;
        } else if(strncmp(line, "</c>", 4) == 0) {
            *status = LEFT_ALIGNED;
        } else if(strncmp(line, "<j>", 3) == 0) {
            *status = JUSTIFIED;
        } else if(strncmp(line, "</j>", 4) == 0) {
            *status = LEFT_ALIGNED;
        } else if(strncmp(line, "<l>", 3) == 0) {
            *status = LEFT_ALIGNED;
        } else if(strncmp(line, "</l>", 4) == 0) {
            *status = LEFT_ALIGNED;
        } else if(strncmp(line, "<r>", 3) == 0) {
            *status = RIGHT_ALIGNED;
        } else if(strncmp(line, "</r>", 4) == 0) {
            *status = LEFT_ALIGNED;
        } else {
            switch(*status) {
            case CENTERED:
                for(i = 0; i < (cols - strlen(line)) / 2 - 1; ++i) printf(" ");
                printf(line);
                break;
            case LEFT_ALIGNED:
                printf(line);
                break;
            case RIGHT_ALIGNED:
                for(i = 0; i < (cols - strlen(line)) - 3; ++i) printf(" ");
                printf(line);
                break;
            case JUSTIFIED:
                numOfSpaces = cols - strlen(line);
                if(wordNum(line) != 0)
                    spacesPerWord = (float) numOfSpaces / (wordNum(line) - 1);
                word = strtok(line, " ");
                printf("  ");
                if(word)
                    printf(word);
                printf(" ");
                word = strtok(NULL, " ");
                while(word) {
                    addedSpaces += spacesPerWord;
                    if(addedSpaces > 1) {
                        for(i = 0; i < (int) addedSpaces; ++i) printf(" ");
                        addedSpaces -= (int) addedSpaces;
                    }
                    printf(word);
                    printf(" ");
                    word = strtok(NULL, " ");
                }
                break;
            }
        }
    }
    free(line);
    free(word);
}

void printTag(FILE* out, char* word, int* currentCol, int* currentLine) {
    fprintf(out, "\n%s", word);
    *currentCol = 8;
    ++(*currentLine);
    fprintf(out, "        ");
}

long printPage(FILE* file, short cols, short lines, long pos, int* currentWord, short* status) {
    int currentCol = 4, currentLine = 2, readWords = 1;
    int maxPos = cols / 2 + cols / 3 - 11;
    long lastPos = 0, beforeLastPos = 0;
    bool noBreak = true;
    char* line = malloc(LINE_SIZE * sizeof(char));
    char* word = malloc(WORD_SIZE * sizeof(char));
    FILE* out = fopen("temp", "w+");

    fprintf(out, "    ");
    fseek(file, pos, SEEK_SET);
    while(fgets(line, LINE_SIZE, file)) {

        word = strtok(line, " ");;
        if(currentLine >= lines - 2) break;

        readWords = 1;
        while(readWords < *currentWord) {
            word = strtok(NULL, " ");
            ++readWords;
        }
        beforeLastPos = lastPos;
        lastPos = ftell(file);

        while(word) {
            if(strncmp(word, "<c>", 3) == 0) {
                printTag(out, word, &currentCol, &currentLine);
                goto skip;
            } else if(strncmp(word, "</c>", 4) == 0) {
                printTag(out, word, &currentCol, &currentLine);
                goto skip;
            } else if(strncmp(word, "<j>", 3) == 0) {
                printTag(out, word, &currentCol, &currentLine);
                goto skip;
            } else if(strncmp(word, "</j>", 4) == 0) {
                printTag(out, word, &currentCol, &currentLine);
                goto skip;
            } else if(strncmp(word, "<l>", 3) == 0) {
                printTag(out, word, &currentCol, &currentLine);
                goto skip;
            } else if(strncmp(word, "</l>", 4) == 0) {
                printTag(out, word, &currentCol, &currentLine);
                goto skip;
            } else if(strncmp(word, "<r>", 3) == 0) {
                printTag(out, word, &currentCol, &currentLine);
                goto skip;
            } else if(strncmp(word, "</r>", 4) == 0) {
                printTag(out, word, &currentCol, &currentLine);
                goto skip;
            }

            currentCol += strlen(word);
            if(isNewlineWord(word)) {
                fprintf(out, "%s", word);
                ++currentLine;
                currentCol = 8;
                fprintf(out, "        ");
            } else if(currentCol > maxPos) {
                fprintf(out, "%s\n", word);
                ++currentLine;
                currentCol = 4;
                fprintf(out, "    ");
            } else {
                fprintf(out, "%s ", word);
            }
            ++readWords;

            skip:
            word = strtok(NULL, " ");
            if(currentLine >= lines - 2) {
                noBreak = false;
                break;
            }
        }
        *currentWord = 1;
    }

    fclose(out);
    out = fopen("temp", "r");
    printFile(out, status, cols, lines);

    free(line);
    free(word);
    *currentWord = readWords;
    if(noBreak) {
        *currentWord = 1;
        return 0;
    }
    return beforeLastPos;
}
