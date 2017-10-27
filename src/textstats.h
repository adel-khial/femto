/*
 * Text statistics utility functions.
 * textstats.h
 * Author: Adel Khial
 */

#ifndef TEXTSTATS_H_INCLUDED
#define TEXTSTATS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * Calculates the number of paragraphs in the file delimited by \n.
 *
 * @param file the input file.
 */

int paragraphCount(FILE* file);

/*
 * Calculates the number of characters in the file including
 * non-alphanumeric characters.
 *
 * @param file the input file.
 */

int totalCharCount(FILE* file);

/*
 * Calculates the number of characters in the file.
 *
 * @param file the input file.
 */

int charCount(FILE* file);

/*
 * Calculates the number of "words" in the file, a "word" in this
 * context is any string delimited by spaces.
 * For example: "a", "2016" and "hello" are considered words.
 *
 * @param file the input file.
 */

int wordCount(FILE* file);

/*
 * Calculates the number of "pages" required to fully display the file,
 * a "page" is defined by the size of the console.
 *
 * @param file the input file.
 */

int pageCount(FILE* file, short cols, short lines);

#endif // TEXTSTATS_H_INCLUDED
