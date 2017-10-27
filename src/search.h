#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "index.h"

int levenshtein(const char* s, const char* t);

Word** closestWords(const char* word, Word* index);

#endif // SEARCH_H_INCLUDED
