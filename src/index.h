/*
 * Index construction and search functions and structures.
 * index.h
 * Author: Adel Khial
 */

#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>

typedef struct position {
    int line;
    int pos;
} Position;

typedef struct word {
    char word[64];
    int posCount;
    Position positions[512];
} Word;

/*
 * Deletes special characters from the beginning and the end of a word,
 * please note that this function assumes no special characters are
 * present in the middle of the word as it truncates the word at the first
 * special character found.
 * The characters which are considered special are: " . , ; : ? ! ( ) { } [ ]
 *
 * @param word the string to process.
 */

void deleteSpecialChars(char* word);

void posSwap(Position* src, Position* dest, size_t len);

int maximum(int a, int b);

void swap(Word* w1, Word* w2);

void sort(Word* index, int length);

Word* findWord(char* word, Word* index, size_t length);

Word* buildIndex(FILE* file);

#endif // INDEX_H_INCLUDED
