#include "search.h"

int levenshtein(const char* s, const char* t) {
    int ls = strlen(s), lt = strlen(t);
    int d[ls + 1][lt + 1];
    int i, j;
    for(i = 0; i <= ls; i++) {
        for (j = 0; j <= lt; j++)
            d[i][j] = -1;
    }
    int dist(int i, int j) {
        if(d[i][j] >= 0) return d[i][j];

        int x;
        if(i == ls)
			x = lt - j;
        else if(j == lt)
            x = ls - i;
        else if(s[i] == t[j])
            x = dist(i + 1, j + 1);
        else {
            x = dist(i + 1, j + 1);

            int y;
            if((y = dist(i, j + 1)) < x) x = y;
            if((y = dist(i + 1, j)) < x) x = y;
            x++;
        }
        return d[i][j] = x;
    }
    return dist(0, 0);
}

Word** closestWords(const char* word, Word* index) {
    int min1 = 1000, min2 = 1001, min3 = 1002, min4 = 1003;
    int i = 0;
    int n = 0;
    Word** words = malloc(4 * sizeof(Word*));

    for(i = 0; i < 4096; ++i) {
        n = levenshtein(word, index[i].word);
        if(n < min1) {
            min4 = min3;
            min3 = min2;
            min2 = min1;
            min1 = n;
            words[3] = words[2];
            words[2] = words[1];
            words[1] = words[0];
            words[0] = &index[i];
        } else if(n < min2) {
            min4 = min3;
            min3 = min2;
            min2 = n;
            words[3] = words[2];
            words[2] = words[1];
            words[1] = &index[i];
        } else if(n < min3) {
            min4 = min3;
            min3 = n;
            words[3] = words[2];
            words[2] = &index[i];
        } else if(n < min4) {
            min4 = n;
            words[3] = &index[i];
        }
    }
    return words;
}
