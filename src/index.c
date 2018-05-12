/*
 * Index construction and search functions and structures.
 * index.c implementation of index.h
 * Author: Adel Khial
 */

#include "index.h"
#include "textstats.h"
#include "constants.h"
#include <strings.h>

void deleteSpecialChars(char* word) {
    int length = strlen(word);
    int i = 0, j = 0, k = 0;
    char* regex = "\".,;:?!(){}[]*";

    for(i = 0; i < length; ++i) {
        char* c = &(word[i]);
        for(j = 0; j < 14; ++j) {
            if(*c == regex[j] && i == 0) {
                for(k = 0; k < length; ++k)
                    word[k] = word[k + 1];
            }
            else if(*c == regex[j])
                *c = '\0';
        }
    }
}

void posSwap(Position* src, Position* dest, size_t len) {
    int i = 0;
    len %= 512;
    for(i = 0; i < len; ++i) {
        Position save;

        save.line = src[i].line;
        save.pos = src[i].pos;

        src[i].line = dest[i].line;
        src[i].pos = dest[i].pos;

        dest[i].line = save.line;
        dest[i].pos = save.pos;
    }
}

int maximum(int a, int b) {
    if(a > b) return a;
    else return b;
}

void swap(Word* w1, Word* w2) {
    Word save;

    save.posCount = w1->posCount;
    strncpy(save.word, w1->word, WORD_SIZE);

    w1->posCount = w2->posCount;
    strncpy(w1->word, w2->word, WORD_SIZE);

    w2->posCount = save.posCount;
    strncpy(w2->word, save.word, WORD_SIZE);

    posSwap(w1->positions, w2->positions, maximum(w1->posCount, w2->posCount) + 1);
}

void sort(Word* index, int length) {
    int i = 0, j = 0;

    for(i = 0; i < length - 1; ++i) {
        for(j = 0; j < length - i - 1; ++j) {
            if(strncmp(index[j].word, index[j + 1].word, WORD_SIZE) > 0) {
                swap(&index[j], &index[j + 1]);
            }
        }
    }
}

Word* findWord(char* word, Word* index, size_t length) {
    bool found = false;
    int i = 0;

    while(i < length) {
        if(strncmp(word, index[i].word, WORD_SIZE) == 0) {
            return &index[i];
        }
        ++i;
    }
    return NULL;
}

Word* buildIndex(FILE* file) {
    int wordPos = 0, currentLine = 0, i = 0, j = 0, k = 0;
    char* line = malloc(LINE_SIZE * sizeof(char));
    char* word = malloc(WORD_SIZE * sizeof(char));
    FILE* out = fopen("index", "w+");
    Word* index = malloc(4096 * 2 * sizeof(Word));

    rewind(file);

    while(fgets(line, LINE_SIZE, file)) {
        ++currentLine;
        word = strtok(line, " ");
        ++wordPos;
        while(word) {
            if(strncmp(line, "<c>", 3) == 0) {
                goto skip;
            } else if(strncmp(line, "</c>", 4) == 0) {
                goto skip;
            } else if(strncmp(line, "<j>", 3) == 0) {
                goto skip;
            } else if(strncmp(line, "</j>", 4) == 0) {
                goto skip;
            } else if(strncmp(line, "<l>", 3) == 0) {
                goto skip;
            } else if(strncmp(line, "</l>", 4) == 0) {
                goto skip;
            } else if(strncmp(line, "<r>", 3) == 0) {
                goto skip;
            } else if(strncmp(line, "</r>", 4) == 0) {
                goto skip;
            }
            deleteSpecialChars(word);
            Word* res;
            Position p;

            p.line = currentLine;
            p.pos = wordPos;

            if(res = findWord(word, index, i)) {
                ++(res->posCount);
                res->positions[res->posCount] = p;
            } else {
                Word w;

                strncpy(w.word, word, WORD_SIZE);
                w.posCount = 0;
                w.positions[0] = p;

                index[i] = w;
                ++i;
            }
            skip:
            word = strtok(NULL, " ");
            ++wordPos;
        }
        wordPos = 0;
    }

    sort(index, i);

    for(j = 0; j < i; ++j) {
        fprintf(out, "%s ", index[j].word);
        for(k = 0; k < index[j].posCount + 1; ++k)
            fprintf(out, "%d %d ; ", index[j].positions[k].pos, index[j].positions[k].line);
        fprintf(out, "\n");
    }

    free(line);
    free(word);
    return index;
}
